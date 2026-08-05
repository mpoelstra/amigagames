/* Sparkpaw bare-metal PAL Copper benchmark: 3+3 dual playfield. */
#include <exec/types.h>
#include <exec/memory.h>
#include <graphics/gfxbase.h>
#include <graphics/copper.h>
#include <graphics/rastport.h>
#include <graphics/view.h>
#include <intuition/intuitionbase.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <proto/intuition.h>

#define H 256
#define BW 672
/*
 * Low-resolution fetch count is:
 *   DDFSTRT = DDFSTOP - 8 * (word_count - 1)
 * $30..$d0 therefore fetches 21 words, not 22.
 */
#define FETCH_BYTES 42
#define COP_WORDS 128
#define BUILD_ID "2026-08-04-rb13"

struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;
static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static struct BitMap *frontBM,*rearBM;
static struct View *oldView;
static UWORD *cop;
static UWORD copPos;
static UWORD ptrValue[6];
static UWORD scrollValue;
static UWORD oldDma;
static UWORD oldIntena;
static BOOL systemLocked;
static BOOL interruptsDisabled;
static LONG lastCamera;
static LONG frameCount;
static BOOL exitedByMouse;

static void move(UWORD reg,UWORD value)
{
    cop[copPos++]=reg; cop[copPos++]=value;
}

static void pointer(UWORD reg,APTR value,UWORD plane)
{
    ULONG p=(ULONG)value;
    move(reg,(UWORD)(p>>16)); ptrValue[plane]=copPos-1;
    move(reg+2,(UWORD)p);
}

static void paint(struct BitMap *bm,BOOL rear)
{
    struct RastPort rp;
    WORD x;
    InitRastPort(&rp); rp.BitMap=bm; SetRast(&rp,rear?1:0);
    if(rear) {
        for(x=16;x<BW-80;x+=96) {
            SetAPen(&rp,2+(x/96)%5);
            RectFill(&rp,x,42+(x%58),x+72,218);
        }
    } else {
        SetAPen(&rp,2); RectFill(&rp,0,220,BW-1,255);
        for(x=96;x<BW-90;x+=150) {
            SetAPen(&rp,3+(x/150)%4);
            RectFill(&rp,x,150-(x%48),x+78,166-(x%48));
        }
    }
    WaitBlit();
}

static void buildCopper(void)
{
    static const UWORD colors[16]={0x001,0x113,0x225,0x447,0x779,0xaac,0xddf,0xfff,
        0x012,0x124,0x236,0x348,0x45a,0x66b,0x99d,0xccf};
    WORD i;
    copPos=0;
    move(0x08e,0x2c81); /* DIWSTRT: PAL line 44, x=129 */
    move(0x090,0x2cc1); /* DIWSTOP: PAL line 300, x=449 */
    move(0x092,0x0030); /* one early word for smooth horizontal scroll */
    move(0x094,0x00d0);
    move(0x100,0x6600); /* six planes, colour, dual playfield */
    move(0x102,0x0000); scrollValue=copPos-1;
    move(0x104,0x0024);
    move(0x106,0x0c00); /* AGA PF2 colour-table offset 8 */
    move(0x108,frontBM->BytesPerRow-FETCH_BYTES);
    move(0x10a,rearBM->BytesPerRow-FETCH_BYTES);
    move(0x10c,0x0000);
    move(0x1fc,0x0000); /* FMODE: 16-bit fetch */
    pointer(0x0e0,frontBM->Planes[0],0);
    pointer(0x0e4,rearBM->Planes[0],1);
    pointer(0x0e8,frontBM->Planes[1],2);
    pointer(0x0ec,rearBM->Planes[1],3);
    pointer(0x0f0,frontBM->Planes[2],4);
    pointer(0x0f4,rearBM->Planes[2],5);
    for(i=0;i<16;i++) move(0x180+i*2,colors[i]);
    cop[copPos++]=0xffff; cop[copPos++]=0xfffe;
}

static void setPlanePointer(UWORD plane,APTR base,LONG byteOffset)
{
    ULONG p=(ULONG)base+byteOffset;
    UWORD hi=ptrValue[plane];
    cop[hi]=(UWORD)(p>>16); cop[hi+2]=(UWORD)p;
}

static void setScroll(LONG foreground,LONG background)
{
    UWORD ff=(UWORD)(15-(foreground&15));
    UWORD bf=(UWORD)(15-(background&15));
    LONG fo=((foreground>>4)<<1);
    LONG bo=((background>>4)<<1);
    setPlanePointer(0,frontBM->Planes[0],fo);
    setPlanePointer(2,frontBM->Planes[1],fo);
    setPlanePointer(4,frontBM->Planes[2],fo);
    setPlanePointer(1,rearBM->Planes[0],bo);
    setPlanePointer(3,rearBM->Planes[1],bo);
    setPlanePointer(5,rearBM->Planes[2],bo);
    cop[scrollValue]=(bf<<4)|ff;
}

static BOOL leftButton(void)
{
    return ((*(volatile UBYTE *)0xbfe001)&0x40)==0;
}

static UWORD rasterLine(void)
{
    UWORD high=hw->vposr;
    UWORD low=hw->vhposr;
    return (UWORD)(((high&7)<<8)|(low>>8));
}

static void waitRasterFrame(void)
{
    /* PAL has 312 lines. Cross the bottom and return near the next line 0. */
    while(rasterLine()<300) { }
    while(rasterLine()>=300) { }
}

static void writeLog(void)
{
    BPTR file=Open("PROGDIR:renderbench.log",MODE_NEWFILE);
    if(!file) return;
    FPrintf(file,"Sparkpaw AGA renderbench %s\n",BUILD_ID);
    FPrintf(file,"result=clean-exit exit=%s frames=%ld camera=%ld\n",
        exitedByMouse?"left-mouse":"frame-limit",frameCount,lastCamera);
    FPrintf(file,"bitmap=%ldx%ld depth=3 rowbytes=%ld/%ld\n",
        (LONG)BW,(LONG)H,(LONG)frontBM->BytesPerRow,(LONG)rearBM->BytesPerRow);
    FPrintf(file,"diwstrt=$2c81 diwstop=$2cc1 ddfstrt=$0030 ddfstop=$00d0\n");
    FPrintf(file,"fetch_words=21 fetch_bytes=%ld modulo=%ld/%ld old_dma=$%04lx\n",
        (LONG)FETCH_BYTES,(LONG)(frontBM->BytesPerRow-FETCH_BYTES),
        (LONG)(rearBM->BytesPerRow-FETCH_BYTES),(LONG)oldDma);
    FPrintf(file,"restore=load-and-force-saved-copper-then-restore-interrupts\n");
    FPrintf(file,"old_intena=$%04lx intena_after=$%04lx\n",
        (LONG)oldIntena,(LONG)(hw->intenar&0x7fff));
    FPrintf(file,"old_view=$%08lx active_view=$%08lx dma_after=$%04lx\n",
        (LONG)oldView,(LONG)GfxBase->ActiView,(LONG)(hw->dmaconr&DMAF_ALL));
    Close(file);
}

static void restore(void)
{
    if(interruptsDisabled) {
        /* Stop private fetches, then put back the saved DMA mask. */
        hw->dmacon=DMAF_ALL;
        hw->dmacon=DMAF_SETCLR|DMAF_MASTER|oldDma;
        /*
         * LoadView may regard oldView as already active.  Explicitly start
         * the system-generated LOF/SHF lists while interrupts are still
         * isolated so no VBlank server can race this transition.
         */
        if(oldView) {
            LoadView(oldView);
            if(oldView->LOFCprList&&oldView->LOFCprList->start) {
                hw->cop1lc=(ULONG)oldView->LOFCprList->start;
                if(oldView->SHFCprList&&oldView->SHFCprList->start)
                    hw->cop2lc=(ULONG)oldView->SHFCprList->start;
                else
                    hw->cop2lc=(ULONG)oldView->LOFCprList->start;
                hw->copjmp1=0;
            }
        }
        hw->intena=0x7fff;
        hw->intena=0x8000|oldIntena;
        Enable(); interruptsDisabled=FALSE;
    }
    /* Let two complete system fields consume the restored Copper list. */
    if(oldView) {
        WaitTOF(); WaitTOF();
    }
    if(systemLocked) {
        DisownBlitter(); Permit(); systemLocked=FALSE;
    }
    if(frontBM&&rearBM) writeLog();
    if(cop) FreeMem(cop,COP_WORDS*2);
    if(frontBM) FreeBitMap(frontBM);
    if(rearBM) FreeBitMap(rearBM);
    if(IntuitionBase) CloseLibrary((struct Library *)IntuitionBase);
    if(GfxBase) CloseLibrary((struct Library *)GfxBase);
}

int main(void)
{
    LONG camera=0,frames=0;
    GfxBase=(struct GfxBase *)OpenLibrary("graphics.library",39);
    IntuitionBase=(struct IntuitionBase *)OpenLibrary("intuition.library",39);
    if(!GfxBase||!IntuitionBase) { restore(); return 5; }
    frontBM=AllocBitMap(BW,H,3,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    rearBM=AllocBitMap(BW,H,3,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    cop=(UWORD *)AllocMem(COP_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    if(!frontBM||!rearBM||!cop) { restore(); return 10; }
    paint(frontBM,FALSE); paint(rearBM,TRUE); buildCopper(); setScroll(0,0);
    Printf("Copper renderbench: row bytes %ld/%ld; click left mouse to exit.\n",
        (LONG)frontBM->BytesPerRow,(LONG)rearBM->BytesPerRow);
    oldView=GfxBase->ActiView;
    oldDma=hw->dmaconr&DMAF_ALL;
    oldIntena=hw->intenar&0x7fff;
    LoadView(NULL); WaitTOF(); WaitTOF();
    OwnBlitter(); WaitBlit(); Forbid(); systemLocked=TRUE;
    Disable(); interruptsDisabled=TRUE;
    /*
     * LoadView(NULL) does not disable the old Workbench DMA channels.
     * In particular, leaving sprite DMA active lets stale Workbench sprite
     * pointers fetch while our private Copper list is displayed.  Start from
     * a known DMA state and enable only the two channels this test needs.
     */
    hw->dmacon=DMAF_ALL;
    hw->cop1lc=(ULONG)cop; hw->copjmp1=0;
    hw->dmacon=DMAF_SETCLR|DMAF_MASTER|DMAF_RASTER|DMAF_COPPER;
    while(frames<3000 && !leftButton()) {
        waitRasterFrame();
        while(rasterLine()<100) { }
        /*
         * rb08 deliberately holds at the far edge.  Earlier builds bounced
         * here and the reported one-frame fault follows that cadence.  This
         * separates a turnaround bug from an unstable steady-state fetch.
         */
        if(camera<300) camera++;
        setScroll(camera,camera>>2);
        frames++;
    }
    exitedByMouse=leftButton(); frameCount=frames; lastCamera=camera;
    /* Do not pass the exit click through to Workbench after restoring it. */
    while(leftButton()) waitRasterFrame();
    waitRasterFrame();
    restore(); return 0;
}
