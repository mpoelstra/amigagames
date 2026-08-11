#include "platform_amiga.h"

#include <graphics/copper.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <proto/exec.h>
#include <proto/graphics.h>

#include "audio.h"

struct GfxBase *GfxBase;

static volatile struct Custom *hardware=(volatile struct Custom *)0xdff000;
static struct View *systemView;
static UWORD oldDma,oldIntena;
static BOOL systemLocked,interruptsDisabled;
static UBYTE gameKeys;

#define CIAA_SDR (*(volatile UBYTE *)0xbfec01)
#define CIAA_ICR (*(volatile UBYTE *)0xbfed01)
#define CIAA_CRA (*(volatile UBYTE *)0xbfee01)
#define CIAICRF_SP 0x08
#define CIACRAF_SPMODE 0x40

#define GAMEKEY_W 0x01
#define GAMEKEY_A 0x02
#define GAMEKEY_S 0x04
#define GAMEKEY_D 0x08
#define GAMEKEY_SPACE 0x10

BOOL platformOpen(void)
{
    GfxBase=(struct GfxBase *)OpenLibrary("graphics.library",39);
    if(GfxBase) {
        systemView=GfxBase->ActiView;
        oldDma=hardware->dmaconr&DMAF_ALL;
        oldIntena=hardware->intenar&0x7fff;
    }
    return GfxBase!=NULL;
}

void platformClose(void)
{
    if(GfxBase) {
        CloseLibrary((struct Library *)GfxBase);
        GfxBase=NULL;
    }
}

void platformBeginTakeover(void)
{
    gameKeys=0;
    WaitTOF(); hardware->color[0]=0;
    hardware->dmacon=DMAF_RASTER|DMAF_COPPER|DMAF_SPRITE;
}

void platformFinishTakeover(UWORD *copper)
{
    OwnBlitter(); WaitBlit(); Forbid(); systemLocked=TRUE;
    Disable(); interruptsDisabled=TRUE;
    hardware->intena=0x7fff; hardware->dmacon=DMAF_ALL;
    hardware->cop1lc=(ULONG)copper; hardware->copjmp1=0;
    hardware->dmacon=DMAF_SETCLR|DMAF_MASTER|DMAF_RASTER|DMAF_COPPER|
                     DMAF_SPRITE|DMAF_BLITTER;
    audioSetHardwareActive(TRUE);
}

void platformRestore(void)
{
    if(interruptsDisabled) {
        audioSetHardwareActive(FALSE);
        hardware->dmacon=DMAF_ALL;
        hardware->dmacon=DMAF_SETCLR|DMAF_MASTER|oldDma;
        if(systemView) {
            LoadView(systemView);
            if(systemView->LOFCprList&&systemView->LOFCprList->start) {
                hardware->cop1lc=(ULONG)systemView->LOFCprList->start;
                hardware->cop2lc=(ULONG)((systemView->SHFCprList&&
                                          systemView->SHFCprList->start)?
                                         systemView->SHFCprList->start:
                                         systemView->LOFCprList->start);
                hardware->copjmp1=0;
            }
        }
        hardware->intena=0x7fff;
        hardware->intena=0x8000|oldIntena;
        Enable(); interruptsDisabled=FALSE;
    }
    if(systemView) { WaitTOF(); WaitTOF(); }
    if(systemLocked) {
        DisownBlitter(); Permit(); systemLocked=FALSE;
    }
}

UWORD platformRasterLine(void)
{
    UWORD high=hardware->vposr,low=hardware->vhposr;
    return (UWORD)(((high&7)<<8)|(low>>8));
}

void platformWaitBlit(void)
{
    (void)hardware->dmaconr;
    while(hardware->dmaconr&DMAF_BLTDONE) { }
}

static void acknowledgeKeyboard(void)
{
    UWORD line=platformRasterLine(),changes=0;
    CIAA_CRA|=CIACRAF_SPMODE;
    /* The keyboard requires an acknowledge pulse of at least 85 us. Two PAL
       raster lines are deterministic even on accelerated CPUs. */
    while(changes<2) {
        UWORD next=platformRasterLine();
        if(next!=line) { line=next; changes++; }
    }
    CIAA_CRA&=(UBYTE)~CIACRAF_SPMODE;
}

void platformReadGameKeys(BOOL *left,BOOL *right,BOOL *down,
                          BOOL *jump,BOOL *fire)
{
    UBYTE code,flag=0;
    if(CIAA_ICR&CIAICRF_SP) {
        code=(UBYTE)~CIAA_SDR;
        code=(UBYTE)((code>>1)|(code<<7));
        acknowledgeKeyboard();
        switch(code&0x7f) {
            case 0x11: flag=GAMEKEY_W; break;
            case 0x20: flag=GAMEKEY_A; break;
            case 0x21: flag=GAMEKEY_S; break;
            case 0x22: flag=GAMEKEY_D; break;
            case 0x40: flag=GAMEKEY_SPACE; break;
        }
        if(flag) {
            if(code&0x80) gameKeys&=(UBYTE)~flag;
            else gameKeys|=flag;
        }
    }
    *left=(gameKeys&GAMEKEY_A)!=0;
    *right=(gameKeys&GAMEKEY_D)!=0;
    *down=(gameKeys&GAMEKEY_S)!=0;
    *jump=(gameKeys&GAMEKEY_W)!=0;
    *fire=(gameKeys&GAMEKEY_SPACE)!=0;
}
