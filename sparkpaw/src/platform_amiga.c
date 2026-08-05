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
static struct View *oldView;
static UWORD oldDma,oldIntena;
static BOOL systemLocked,interruptsDisabled;

BOOL platformOpen(void)
{
    GfxBase=(struct GfxBase *)OpenLibrary("graphics.library",39);
    return GfxBase!=NULL;
}

void platformClose(void)
{
    if(GfxBase) {
        CloseLibrary((struct Library *)GfxBase);
        GfxBase=NULL;
    }
}

void platformTakeover(UWORD *copper)
{
    oldView=GfxBase->ActiView;
    oldDma=hardware->dmaconr&DMAF_ALL;
    oldIntena=hardware->intenar&0x7fff;
    LoadView(NULL); WaitTOF(); WaitTOF();
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
        if(oldView) {
            LoadView(oldView);
            if(oldView->LOFCprList&&oldView->LOFCprList->start) {
                hardware->cop1lc=(ULONG)oldView->LOFCprList->start;
                hardware->cop2lc=(ULONG)((oldView->SHFCprList&&
                                          oldView->SHFCprList->start)?
                                         oldView->SHFCprList->start:
                                         oldView->LOFCprList->start);
                hardware->copjmp1=0;
            }
        }
        hardware->intena=0x7fff;
        hardware->intena=0x8000|oldIntena;
        Enable(); interruptsDisabled=FALSE;
    }
    if(oldView) { WaitTOF(); WaitTOF(); }
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