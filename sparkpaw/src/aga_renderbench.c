/* Sparkpaw bare-metal PAL matched 4+3/4+4 production-load benchmark. */
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
#if defined(AGA32_LEFT_GUARD_PROOF)||defined(AGA64_FETCH_PROOF)
#define BW 704
#else
#define BW 672
#endif
#ifdef AGA64_FETCH_PROOF
#define AGA_SCROLL_CALIBRATION
#define AGA64_FETCH_MODE
#endif
#ifdef AGA32_SCROLL_PROOF
/* Retain the archived Stage 5D3 build target for reproducibility. */
#define AGA_SCROLL_CALIBRATION
#define AGA32_FETCH_MODE
#define AGA32_DDF_SHIFT
#endif
/*
 * Low-resolution fetch count is:
 *   DDFSTRT = DDFSTOP - 8 * (word_count - 1)
 * $30..$d0 therefore fetches 21 words, not 22.
 */
#ifdef AGA64_FETCH_MODE
#define FETCH_BYTES 48
#ifdef AGA64_ORIGIN_CORRECTION
#define BUILD_ID "2026-08-20-stage5h2-fmode3-aligned-plus16-phase-correction"
#else
#define BUILD_ID "2026-08-20-stage5h-fmode3-relative-origin-candidate"
#endif
#elif defined(AGA32_LEFT_GUARD_PROOF)
#define FETCH_BYTES 48
#define BUILD_ID "2026-08-20-stage5f-aga32-left-prefetch-guard"
#elif defined(AGA32_ORIGIN_CORRECTION)
#define FETCH_BYTES 44
#define BUILD_ID "2026-08-20-stage5d5-fmode1-aligned-plus16-phase-correction"
#elif defined(AGA32_DDF_SHIFT)
#define FETCH_BYTES 44
#define BUILD_ID "2026-08-20-stage5d3-aga32-coordinated-ddf-calibration"
#elif defined(AGA32_FETCH_MODE)
#define FETCH_BYTES 44
#define BUILD_ID "2026-08-20-stage5d4-fmode1-relative-origin-candidate"
#elif defined(AGA_SCROLL_CALIBRATION)
#define FETCH_BYTES 42
#define BUILD_ID "2026-08-20-stage5d4-fmode0-relative-origin-reference"
#else
#define FETCH_BYTES 42
#define BUILD_ID "2026-08-14-rb21-calibrated-production-worst-case"
#endif
#define COP_WORDS 512
#ifndef REAR_PLANES
#define REAR_PLANES 4
#endif
#define FRONT_PLANES 4
#define TOTAL_PLANES (FRONT_PLANES+REAR_PLANES)
#define HUD_TOP 208
#define WORK_WORDS 5
#define WORK_H 64
#define SPRITE_H 48
#define SPRITE_WORDS (2+SPRITE_H*2+2)
#define STRIDER_W 64
#define STRIDER_H 64
#define STRIDER_ROW_BYTES 8
#define STRIDER_PLANE_BYTES (STRIDER_ROW_BYTES*STRIDER_H)
#define STRIDER_RAW_BYTES (STRIDER_PLANE_BYTES*5)

struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;
static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static struct BitMap *frontBM,*frontCleanBM,*rearBM,*hudBM;
static struct View *oldView;
static UWORD *cop;
static UWORD copPos;
static UWORD ptrValue[TOTAL_PLANES];
static UWORD *spriteData[8];
static UWORD *workMask,*workBits;
static UWORD scrollValue;
static UWORD oldDma;
static UWORD oldIntena;
static BOOL copperOverflow;
static BOOL systemLocked;
static BOOL interruptsDisabled;
static LONG lastCamera;
static LONG frameCount;
static BOOL exitedByMouse;
static BOOL striderLoaded;
static BOOL rear16Loaded;
static LONG maxWorkEnd;
static LONG maxWorkElapsed;
static LONG overBudgetFrames;
static UWORD workLastLine;
static BOOL workWrapped;

static void move(UWORD reg,UWORD value)
{
    if(copPos+2>COP_WORDS) { copperOverflow=TRUE; return; }
    cop[copPos++]=reg; cop[copPos++]=value;
}

static void pointer(UWORD reg,APTR value,UWORD plane)
{
    ULONG p=(ULONG)value;
    if(copPos+4>COP_WORDS) { copperOverflow=TRUE; return; }
    move(reg,(UWORD)(p>>16)); ptrValue[plane]=copPos-1;
    move(reg+2,(UWORD)p);
}

static void plainPointer(UWORD reg,APTR value)
{
    ULONG p=(ULONG)value;
    move(reg,(UWORD)(p>>16)); move(reg+2,(UWORD)p);
}

static void paint(struct BitMap *bm,BOOL rear)
{
    struct RastPort rp;
    WORD x;
    InitRastPort(&rp); rp.BitMap=bm; SetRast(&rp,rear?1:0);
    if(rear) {
        SetAPen(&rp,1); RectFill(&rp,0,0,BW-1,H-1);
        for(x=16;x<BW-80;x+=96) {
            SetAPen(&rp,2+(x/96)%4);
            RectFill(&rp,x,72+(x%42),x+72,218);
        }
    } else {
        SetAPen(&rp,2); RectFill(&rp,0,208,BW-1,255);
        for(x=96;x<BW-90;x+=150) {
            SetAPen(&rp,8+(x/150)%4);
            RectFill(&rp,x,152-(x%48),x+94,167-(x%48));
        }
    }
    WaitBlit();
}

#ifdef AGA_SCROLL_CALIBRATION
static void paintCalibration(struct BitMap *bm,BOOL hud)
{
    struct RastPort rp;
    WORD x,y;
    InitRastPort(&rp); rp.BitMap=bm; SetRast(&rp,0);
    for(x=0;x<BW;x+=16) {
        SetAPen(&rp,(LONG)(1+((x>>4)&(bm->Depth==3?3:7))));
        RectFill(&rp,x,0,x+15,hud?47:H-1);
        SetAPen(&rp,15);
        RectFill(&rp,x,0,x,hud?47:H-1);
        if((x&31)==0) {
            SetAPen(&rp,0);
            RectFill(&rp,x+1,0,x+2,hud?47:H-1);
        }
    }
    if(!hud) {
        SetAPen(&rp,15);
        for(y=16;y<H;y+=16) RectFill(&rp,0,y,BW-1,y);
        /* Unique source-coordinate marker. At camera phase zero its left edge
           must coincide with the fixed hardware-sprite marker at screen x=32. */
#ifdef AGA32_LEFT_GUARD_PROOF
        SetAPen(&rp,0); RectFill(&rp,64,24,71,199);
        SetAPen(&rp,14); RectFill(&rp,66,24,69,199);
#else
        SetAPen(&rp,0); RectFill(&rp,32,24,39,199);
        SetAPen(&rp,14); RectFill(&rp,34,24,37,199);
#endif
    }
    WaitBlit();
}

static UWORD aga32Field(UWORD delay,UWORD playfield)
{
#ifdef AGA64_FETCH_MODE
    delay&=63;
    if(playfield==1) return (UWORD)((delay&15)|((delay&48)<<6));
    return (UWORD)(((delay&15)<<4)|((delay&48)<<10));
#else
    delay&=31;
    if(playfield==1) return (UWORD)((delay&15)|((delay&16)<<6));
    return (UWORD)(((delay&15)<<4)|((delay&16)<<10));
#endif
}
#endif

static BOOL placeStriderIdle(void)
{
    UBYTE *raw=(UBYTE *)AllocMem(STRIDER_RAW_BYTES,MEMF_PUBLIC);
    BPTR file;
    WORD x,y; UBYTE plane;
    const WORD targetX=286,targetY=146;
    if(!raw) return FALSE;
    file=Open("PROGDIR:renderbench-strider-idle.raw",MODE_OLDFILE);
    if(!file)
        file=Open("PROGDIR:assets/runtime/renderbench-strider-idle.raw",
                  MODE_OLDFILE);
    if(!file||Read(file,raw,STRIDER_RAW_BYTES)!=STRIDER_RAW_BYTES) {
        if(file) Close(file);
        FreeMem(raw,STRIDER_RAW_BYTES); return FALSE;
    }
    Close(file);
    /* Static rb17 setup only: compose before display takeover. Production
       gameplay remains on its synchronized packed Blitter pipeline. */
    for(y=0;y<STRIDER_H;y++) for(x=0;x<STRIDER_W;x++) {
        LONG sourceAt=(LONG)y*STRIDER_ROW_BYTES+(x>>3);
        UBYTE bit=(UBYTE)(0x80>>(x&7));
        LONG destAt=(LONG)(targetY+y)*frontBM->BytesPerRow+
                    ((targetX+x)>>3);
        UBYTE destBit=(UBYTE)(0x80>>((targetX+x)&7));
        if(!(raw[STRIDER_PLANE_BYTES*4+sourceAt]&bit)) continue;
        for(plane=0;plane<4;plane++) {
            UBYTE *dest=frontBM->Planes[plane]+destAt;
            if(raw[(LONG)plane*STRIDER_PLANE_BYTES+sourceAt]&bit)
                *dest|=destBit;
            else *dest&=(UBYTE)~destBit;
        }
    }
    FreeMem(raw,STRIDER_RAW_BYTES); return TRUE;
}

static BOOL loadRear16(void)
{
    BPTR file=Open("PROGDIR:renderbench-rear16.raw",MODE_OLDFILE);
    UBYTE plane;
    LONG planeBytes=(LONG)rearBM->BytesPerRow*rearBM->Rows;
    if(!file)
        file=Open("PROGDIR:assets/runtime/renderbench-rear16.raw",
                  MODE_OLDFILE);
    if(!file) {
        PutStr("rb19: cannot open renderbench-rear16.raw\n");
        return FALSE;
    }
    for(plane=0;plane<REAR_PLANES;plane++) {
        LONG got=Read(file,rearBM->Planes[plane],planeBytes);
        if(got!=planeBytes) {
            Printf("rb19: rear plane %ld read %ld, expected %ld\n",
                   (LONG)plane,got,planeBytes);
            Close(file); return FALSE;
        }
    }
    Close(file); return TRUE;
}

static void buildCopper(void)
{
    static const UBYTE colors[32][3]={
        {0,0,17},{9,10,24},{18,27,54},{32,47,86},
        {55,55,65},{101,98,103},{163,157,158},{229,225,219},
        {67,29,100},{112,45,157},{166,77,218},{210,139,246},
        {0,112,170},{0,207,239},{201,246,255},{224,35,104},
        {0,0,17},{0,17,51},{17,34,85},{34,68,119},
        {68,68,153},{102,85,170},{153,119,187},{204,187,221},
        {0,29,43},{0,48,58},{13,67,69},{31,86,82},
        {56,105,91},{79,103,117},{45,145,194},{151,211,224}
    };
    WORD pen,i;
    copPos=0;
    move(0x08e,0x2c81); /* DIWSTRT: PAL line 44, x=129 */
    move(0x090,0x2cc1); /* DIWSTOP: PAL line 300, x=449 */
#ifdef AGA32_LEFT_GUARD_PROOF
    move(0x092,0x0020);
    move(0x094,0x00d0);
#elif defined(AGA32_DDF_SHIFT)
    move(0x092,0x0038);
    move(0x094,0x00d8);
#else
    move(0x092,0x0030); /* one early word for smooth horizontal scroll */
    move(0x094,0x00d0);
#endif
    move(0x100,REAR_PLANES==4?0x0610:0x7600);
    move(0x102,0x0000); scrollValue=copPos-1;
    move(0x104,0x0024);
    move(0x106,0x1020); /* AGA border blank; PF2 colour-table offset 16 */
    move(0x108,frontBM->BytesPerRow-FETCH_BYTES);
    move(0x10a,rearBM->BytesPerRow-FETCH_BYTES);
    move(0x10c,0x0000);
#ifdef AGA64_FETCH_MODE
    move(0x1fc,0x0003); /* AGA 64-bit bitplane fetch; sprites remain 16-bit. */
#elif defined(AGA32_FETCH_MODE)
    move(0x1fc,0x0001); /* Isolated AGA 32-bit bitplane fetch proof. */
#else
    move(0x1fc,0x0000); /* FMODE: 16-bit fetch */
#endif
    pointer(0x0e0,frontBM->Planes[0],0);
    pointer(0x0e4,rearBM->Planes[0],1);
    pointer(0x0e8,frontBM->Planes[1],2);
    pointer(0x0ec,rearBM->Planes[1],3);
    pointer(0x0f0,frontBM->Planes[2],4);
    pointer(0x0f4,rearBM->Planes[2],5);
    pointer(0x0f8,frontBM->Planes[3],6);
    if(REAR_PLANES==4) pointer(0x0fc,rearBM->Planes[3],7);
    /* Load AGA palette registers 0-31 in high- then low-nibble passes. */
    move(0x106,0x1020); /* bank 0, high nibbles, PF2OF=16 */
    for(pen=0;pen<32;pen++) {
        const UBYTE *rgb=colors[pen];
        move((UWORD)(0x180+pen*2),
             (UWORD)(((rgb[0]>>4)<<8)|((rgb[1]>>4)<<4)|(rgb[2]>>4)));
    }
    move(0x106,0x1220); /* bank 0, low nibbles, PF2OF=16 */
    for(pen=0;pen<32;pen++) {
        const UBYTE *rgb=colors[pen];
        move((UWORD)(0x180+pen*2),
             (UWORD)(((rgb[0]&15)<<8)|((rgb[1]&15)<<4)|(rgb[2]&15)));
    }
    move(0x106,0x1020);
#ifdef AGA32_ORIGIN_CORRECTION
    /* Make sprite colour 17 unambiguously orange in D5. D4 demonstrated that
       the original dark colour could not serve as an absolute marker. */
    move(0x1a2,0x0f80);
    move(0x106,0x1220);
    move(0x1a2,0x0f00);
    move(0x106,0x1020);
#endif
    /* Six active player channels plus two null channels, matching production
       sprite DMA ownership. The small streams remain transparent. */
    for(i=0;i<8;i++) plainPointer((UWORD)(0x120+i*4),spriteData[i]);
    /* Match the production HUD boundary and fixed foreground-only display. */
    if(copPos+2<=COP_WORDS) {
        cop[copPos++]=(UWORD)(((44+HUD_TOP)<<8)|1);
        cop[copPos++]=0xfffe;
    } else copperOverflow=TRUE;
#ifdef AGA64_FETCH_MODE
    move(0x1fc,0x0001);
#endif
    move(0x102,0x000f);
    for(i=0;i<TOTAL_PLANES;i++) {
        APTR value=(i&1)||(i==6)?(APTR)rearBM->Planes[0]:
                   (APTR)hudBM->Planes[i>>1];
        ULONG p=(ULONG)value;
        move((UWORD)(0x0e0+i*4),(UWORD)(p>>16));
        move((UWORD)(0x0e2+i*4),(UWORD)p);
    }
#if defined(AGA32_LEFT_GUARD_PROOF)||defined(AGA64_FETCH_MODE)
    move(0x092,0x0030);
    move(0x094,0x00d0);
    move(0x108,hudBM->BytesPerRow-44);
    move(0x10a,rearBM->BytesPerRow-44);
#else
    move(0x108,hudBM->BytesPerRow-FETCH_BYTES);
    move(0x10a,rearBM->BytesPerRow-FETCH_BYTES);
#endif
    if(copPos+2<=COP_WORDS) {
        cop[copPos++]=0xffff; cop[copPos++]=0xfffe;
    } else copperOverflow=TRUE;
}

static void setPlanePointer(UWORD plane,APTR base,LONG byteOffset)
{
    ULONG p=(ULONG)base+byteOffset;
    UWORD hi=ptrValue[plane];
    cop[hi]=(UWORD)(p>>16); cop[hi+2]=(UWORD)p;
}

static void setScroll(LONG foreground,LONG background)
{
#ifdef AGA64_FETCH_MODE
#ifdef AGA64_ORIGIN_CORRECTION
    foreground+=16;
    background+=16;
#endif
    UWORD ff=(UWORD)((15-(foreground&63))&63);
    UWORD bf=(UWORD)((15-(background&63))&63);
    LONG fo=(foreground>>6)<<3;
    LONG bo=(background>>6)<<3;
#elif defined(AGA32_FETCH_MODE)
#ifdef AGA32_ORIGIN_CORRECTION
    /* D4 measured FMODE1 one 16-pixel calibration cell to the right of the
       matched FMODE0 reference. Bias the logical fetch phase, not the pointer
       address: this keeps every DMA pointer longword aligned and advances the
       coarse pointer exactly when the extended 0..31 delay wraps. */
    foreground+=16;
    background+=16;
#endif
    UWORD ff=(UWORD)((15-(foreground&31))&31);
    UWORD bf=(UWORD)((15-(background&31))&31);
    LONG fo=(foreground>>5)<<2;
    LONG bo=(background>>5)<<2;
#else
    UWORD ff=(UWORD)(15-(foreground&15));
    UWORD bf=(UWORD)(15-(background&15));
    LONG fo=((foreground>>4)<<1);
    LONG bo=((background>>4)<<1);
#endif
    setPlanePointer(0,frontBM->Planes[0],fo);
    setPlanePointer(2,frontBM->Planes[1],fo);
    setPlanePointer(4,frontBM->Planes[2],fo);
    setPlanePointer(1,rearBM->Planes[0],bo);
    setPlanePointer(3,rearBM->Planes[1],bo);
    setPlanePointer(5,rearBM->Planes[2],bo);
    setPlanePointer(6,frontBM->Planes[3],fo);
    if(REAR_PLANES==4) setPlanePointer(7,rearBM->Planes[3],bo);
#if defined(AGA32_FETCH_MODE)||defined(AGA64_FETCH_MODE)
    cop[scrollValue]=(UWORD)(aga32Field(ff,1)|aga32Field(bf,2));
#else
    cop[scrollValue]=(bf<<4)|ff;
#endif
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

static void waitWorkBlit(void)
{
    UWORD line;
    (void)hw->dmaconr;
    while(hw->dmaconr&DMAF_BLTDONE) { }
    line=rasterLine();
    if(line<workLastLine) workWrapped=TRUE;
    workLastLine=line;
}

static void blitCopy(struct BitMap *source,struct BitMap *target,
                     WORD x,WORD y,WORD width,WORD height)
{
    UBYTE plane; UWORD words=(UWORD)(((x&15)+width+15)>>4);
    LONG at=(LONG)y*target->BytesPerRow+(x>>4)*2;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        waitWorkBlit();
        hw->bltcon0=0x09f0; hw->bltcon1=0;
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=(UWORD)(source->BytesPerRow-words*2);
        hw->bltdmod=(UWORD)(target->BytesPerRow-words*2);
        hw->bltapt=source->Planes[plane]+at;
        hw->bltdpt=target->Planes[plane]+at;
        hw->bltsize=(UWORD)((height<<6)|words);
    }
}

static void blitBob(WORD x,WORD y,WORD width,WORD height)
{
    UBYTE plane; UWORD shift=(UWORD)(x&15);
    UWORD words=(UWORD)((width>>4)+(shift?1:0));
    LONG at=(LONG)y*frontBM->BytesPerRow+(x>>4)*2;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        waitWorkBlit();
        hw->bltcon0=(UWORD)((shift<<12)|0x0fca);
        hw->bltcon1=(UWORD)(shift<<12);
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=(UWORD)((WORK_WORDS-words)*2);
        hw->bltbmod=(UWORD)((WORK_WORDS-words)*2);
        hw->bltcmod=(UWORD)(frontBM->BytesPerRow-words*2);
        hw->bltdmod=(UWORD)(frontBM->BytesPerRow-words*2);
        hw->bltapt=workMask; hw->bltbpt=workBits+(LONG)plane*WORK_H*WORK_WORDS;
        hw->bltcpt=frontBM->Planes[plane]+at;
        hw->bltdpt=frontBM->Planes[plane]+at;
        hw->bltsize=(UWORD)((height<<6)|words);
    }
}

static void runMatchedWorkload(void)
{
    WORD i;
    LONG end,elapsed;
    workWrapped=FALSE; workLastLine=253;
    /* Exact production ordering with a calibrated current-level worst case:
       eight projectile slots, two 64x64 Striders plus two 32x24 beetles, nine
       camera-visible diamonds, one 32x16 splash and the every-other-frame
       double water update. The actor mix is conservative but runtime-valid. */
    for(i=0;i<8;i++) blitCopy(frontCleanBM,frontBM,(WORD)(18+i*37),184,16,9);
    for(i=0;i<2;i++) blitCopy(frontCleanBM,frontBM,(WORD)(24+i*144),120,64,64);
    for(i=0;i<2;i++) blitCopy(frontCleanBM,frontBM,(WORD)(104+i*144),176,32,24);
    for(i=0;i<9;i++) blitCopy(frontCleanBM,frontBM,(WORD)(8+i*35),88,16,21);
    blitCopy(frontCleanBM,frontBM,145,181,32,16);
    blitCopy(frontCleanBM,frontCleanBM,160,197,80,11);
    blitCopy(frontCleanBM,frontBM,160,197,80,11);
    blitBob(145,181,32,16);
    for(i=0;i<9;i++) blitBob((WORD)(8+i*35),88,16,21);
    for(i=0;i<2;i++) blitBob((WORD)(24+i*144),120,64,64);
    for(i=0;i<2;i++) blitBob((WORD)(104+i*144),176,32,24);
    for(i=0;i<8;i++) blitBob((WORD)(18+i*37),184,16,9);
    waitWorkBlit();
    end=rasterLine();
    elapsed=(workWrapped?312:0)+end-253;
    if(end>maxWorkEnd) maxWorkEnd=end;
    if(elapsed>maxWorkElapsed) maxWorkElapsed=elapsed;
    if(workWrapped) overBudgetFrames++;
}

static void writeLog(void)
{
    BPTR file=Open("PROGDIR:renderbench.log",MODE_NEWFILE);
    if(!file) return;
    FPrintf(file,"Sparkpaw AGA renderbench %s\n",BUILD_ID);
    FPrintf(file,"result=clean-exit exit=%s frames=%ld camera=%ld\n",
        exitedByMouse?"left-mouse":"frame-limit",frameCount,lastCamera);
    FPrintf(file,"bitmap=%ldx%ld depth=4+%ld rowbytes=%ld/%ld\n",
        (LONG)BW,(LONG)H,(LONG)REAR_PLANES,(LONG)frontBM->BytesPerRow,
        (LONG)rearBM->BytesPerRow);
    FPrintf(file,"copper_words=%ld capacity=%ld overflow=%ld\n",
        (LONG)copPos,(LONG)COP_WORDS,(LONG)copperOverflow);
    FPrintf(file,"strider_idle=aga15 loaded=%ld size=64x64\n",
        (LONG)striderLoaded);
    FPrintf(file,"rear_loaded=%ld source=%ldx256 planes=%ld\n",
        (LONG)rear16Loaded,(LONG)BW,(LONG)REAR_PLANES);
    FPrintf(file,"matched_load=sprites6x48 hud_line=252 bobs_line=253 enemies=2x64x64+2x32x24 projectiles=8x16x9 collectibles=9x16x21 splash=32x16 water_targets=2\n");
    FPrintf(file,"work_max_end_line=%ld work_max_elapsed_lines=%ld over_budget_frames=%ld result=%s\n",
        maxWorkEnd,maxWorkElapsed,overBudgetFrames,
        overBudgetFrames?"OVER_BUDGET":"WITHIN_FRAME");
#ifdef AGA64_FETCH_MODE
#ifdef AGA64_ORIGIN_CORRECTION
    FPrintf(file,"proof=aga64-corrected-origin role=fmode3-corrected phase0_hold_fields=150 bias_pixels=16 wrap=63-to-0\n");
#else
    FPrintf(file,"proof=aga64-relative-origin role=fmode3-candidate phase0_hold_fields=150 correction_pixels=0\n");
#endif
    FPrintf(file,"diwstrt=$2c81 diwstop=$2cc1 ddfstrt=$0030 ddfstop=$00d0 fmode=3 alignment_bytes=8\n");
#elif defined(AGA32_LEFT_GUARD_PROOF)
    FPrintf(file,"proof=left-prefetch-guard role=guarded-candidate phase0_hold_fields=150 bias_pixels=16 guard_bytes=4\n");
    FPrintf(file,"diwstrt=$2c81 diwstop=$2cc1 ddfstrt=$0020 ddfstop=$00d0 fmode=1\n");
#elif defined(AGA32_DDF_SHIFT)
    FPrintf(file,"proof=absolute-origin role=rejected-ddf-shift phase0_hold_fields=150\n");
    FPrintf(file,"diwstrt=$2c81 diwstop=$2cc1 ddfstrt=$0038 ddfstop=$00d8 fmode=1\n");
#elif defined(AGA32_ORIGIN_CORRECTION)
    FPrintf(file,"proof=absolute-origin role=aligned-phase-correction phase0_hold_fields=150 bias_pixels=16\n");
    FPrintf(file,"diwstrt=$2c81 diwstop=$2cc1 ddfstrt=$0030 ddfstop=$00d0 fmode=1\n");
#elif defined(AGA32_FETCH_MODE)
    FPrintf(file,"proof=relative-origin role=candidate phase0_hold_fields=150\n");
    FPrintf(file,"diwstrt=$2c81 diwstop=$2cc1 ddfstrt=$0030 ddfstop=$00d0 fmode=1\n");
#elif defined(AGA_SCROLL_CALIBRATION)
    FPrintf(file,"proof=relative-origin role=reference phase0_hold_fields=150\n");
    FPrintf(file,"diwstrt=$2c81 diwstop=$2cc1 ddfstrt=$0030 ddfstop=$00d0 fmode=0\n");
#else
    FPrintf(file,"diwstrt=$2c81 diwstop=$2cc1 ddfstrt=$0030 ddfstop=$00d0 fmode=0\n");
#endif
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
    if(frontCleanBM) FreeBitMap(frontCleanBM);
    if(rearBM) FreeBitMap(rearBM);
    if(hudBM) FreeBitMap(hudBM);
    { WORD i; for(i=0;i<8;i++) if(spriteData[i])
        FreeMem(spriteData[i],SPRITE_WORDS*2); }
    if(workMask) FreeMem(workMask,WORK_H*WORK_WORDS*2);
    if(workBits) FreeMem(workBits,FRONT_PLANES*WORK_H*WORK_WORDS*2);
    if(IntuitionBase) CloseLibrary((struct Library *)IntuitionBase);
    if(GfxBase) CloseLibrary((struct Library *)GfxBase);
}

int main(void)
{
    LONG camera=0,frames=0;
    GfxBase=(struct GfxBase *)OpenLibrary("graphics.library",39);
    IntuitionBase=(struct IntuitionBase *)OpenLibrary("intuition.library",39);
    if(!GfxBase||!IntuitionBase) { restore(); return 5; }
    frontBM=AllocBitMap(BW,H,FRONT_PLANES,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    frontCleanBM=AllocBitMap(BW,H,FRONT_PLANES,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    rearBM=AllocBitMap(BW,H,REAR_PLANES,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    hudBM=AllocBitMap(BW,48,FRONT_PLANES,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    cop=(UWORD *)AllocMem(COP_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    { WORD i; for(i=0;i<8;i++) {
        WORD vstart=92,vstop=vstart+SPRITE_H,hstart=145+i*8;
#ifdef AGA_SCROLL_CALIBRATION
        /* The hardware sprite origin is four low-resolution pixels early. */
        if(i==0) hstart=147;
#endif
        spriteData[i]=(UWORD *)AllocMem(SPRITE_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
        if(spriteData[i]) {
            spriteData[i][0]=(UWORD)((vstart<<8)|(hstart>>1));
            spriteData[i][1]=(UWORD)((vstop<<8)|(hstart&1)|
                ((i&1)?0x0080:0));
#ifdef AGA_SCROLL_CALIBRATION
            if(i==0) {
                WORD sy;
                /* Sprite 0 is a fixed 16-pixel colour-17 bar at screen x=32;
                   it exposes the absolute display origin independently of the
                   scrolling bitplane pointers. */
                for(sy=0;sy<SPRITE_H;sy++) {
                    spriteData[i][2+sy*2]=0xffff;
                    spriteData[i][3+sy*2]=0x0000;
                }
            }
#endif
        }
    } }
    workMask=(UWORD *)AllocMem(WORK_H*WORK_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    workBits=(UWORD *)AllocMem(FRONT_PLANES*WORK_H*WORK_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    if(!frontBM||!frontCleanBM||!rearBM||!hudBM||!cop||!workMask||!workBits||
       !spriteData[7]) { restore(); return 10; }
#ifdef AGA64_FETCH_MODE
    {
        WORD plane;
        if((frontBM->BytesPerRow&7)||(rearBM->BytesPerRow&7)) {
            restore(); return 13;
        }
        for(plane=0;plane<FRONT_PLANES;plane++)
            if((ULONG)frontBM->Planes[plane]&7) { restore(); return 13; }
        for(plane=0;plane<REAR_PLANES;plane++)
            if((ULONG)rearBM->Planes[plane]&7) { restore(); return 13; }
    }
#endif
#ifdef AGA_SCROLL_CALIBRATION
    paintCalibration(frontBM,FALSE);
    paintCalibration(frontCleanBM,FALSE);
    paintCalibration(rearBM,FALSE);
    paintCalibration(hudBM,TRUE);
    rear16Loaded=TRUE; striderLoaded=TRUE;
#else
    paint(frontBM,FALSE);
    paint(frontCleanBM,FALSE);
    rear16Loaded=loadRear16();
    if(!rear16Loaded) { restore(); return 11; }
    striderLoaded=placeStriderIdle();
    if(!striderLoaded) { restore(); return 12; }
#endif
    buildCopper();
    if(copperOverflow) { restore(); return 15; }
    setScroll(0,0);
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
    hw->dmacon=DMAF_SETCLR|DMAF_MASTER|DMAF_RASTER|DMAF_COPPER|DMAF_SPRITE|
               DMAF_BLITTER;
    while(frames<3000 && !leftButton()) {
        waitRasterFrame();
        while(rasterLine()<100) { }
        /*
         * rb08 deliberately holds at the far edge.  Earlier builds bounced
         * here and the reported one-frame fault follows that cadence.  This
         * separates a turnaround bug from an unstable steady-state fetch.
         */
#ifdef AGA_SCROLL_CALIBRATION
        /* Hold the identical authored coordinate at phase zero for three
           seconds. Afterwards sweep slowly across the coarse boundary. */
        camera=frames<150?0:((frames-150)>>2)&63;
#else
        if(camera<300) camera++;
#endif
        setScroll(camera,camera>>2);
#ifndef AGA_SCROLL_CALIBRATION
        while(rasterLine()<253) { }
        runMatchedWorkload();
        while(rasterLine()>=253) { }
#endif
        frames++;
    }
    exitedByMouse=leftButton(); frameCount=frames; lastCamera=camera;
    /* Do not pass the exit click through to Workbench after restoring it. */
    while(leftButton()) waitRasterFrame();
    waitRasterFrame();
    restore(); return 0;
}
