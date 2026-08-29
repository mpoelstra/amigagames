#include "platform_amiga.h"

#include <graphics/copper.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <hardware/custom.h>
#include <hardware/cia.h>
#include <hardware/dmabits.h>
#include <proto/exec.h>
#include <proto/graphics.h>

#include "audio.h"
#include "performance_profile.h"

struct GfxBase *GfxBase;

static volatile struct Custom *hardware=(volatile struct Custom *)0xdff000;
static volatile struct CIA *ciaa=(volatile struct CIA *)0xbfe001;
static struct View *systemView;
static UWORD oldDma,oldIntena;
static BOOL systemLocked,interruptsDisabled;
static UBYTE gameKeys;
#ifdef SPARKPAW_WHDLOAD
static BOOL whdloadQuitRequested;
#endif

#define CIAA_SDR (*(volatile UBYTE *)0xbfec01)
#define CIAA_ICR (*(volatile UBYTE *)0xbfed01)
#define CIAA_CRA (*(volatile UBYTE *)0xbfee01)
#ifndef CIAICRF_SP
#define CIAICRF_SP 0x08
#endif
#ifndef CIACRAF_SPMODE
#define CIACRAF_SPMODE 0x40
#endif

/* Controller port 2 pin 5 holds a CD32 pad's shift register in its reset
   state, where pin 9 exposes Blue as the ordinary Amiga second button.
   Keep pin 9 configured as an input so native two-button joysticks use the
   same active-low POTINP bit. */
#define PORT2_CD32_RESET_HIGH 0x3000
#define PORT2_SECOND_BUTTON 0x4000

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#define CIAB_TBLO (*(volatile UBYTE *)0xbfd600)
#define CIAB_TBHI (*(volatile UBYTE *)0xbfd700)
#define CIAB_CRB (*(volatile UBYTE *)0xbfdf00)
static UBYTE profileOldCrb,profileOldTbLo,profileOldTbHi;
static UWORD profileLastCounter;
static ULONG profileTicks;
static BOOL profileTimerActive;
#endif

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

ULONG platformFieldCounter(void)
{
    UBYTE high=ciaa->ciatodhi;
    UBYTE middle=ciaa->ciatodmid;
    UBYTE low=ciaa->ciatodlow;
    return ((ULONG)high<<16)|((ULONG)middle<<8)|low;
}

void platformBeginTakeover(void)
{
    gameKeys=0;
#ifdef SPARKPAW_WHDLOAD
    whdloadQuitRequested=FALSE;
#endif
    WaitTOF(); hardware->color[0]=0;
    hardware->dmacon=DMAF_RASTER|DMAF_COPPER|DMAF_SPRITE;
}

void platformFinishTakeover(UWORD *copper)
{
    hardware->potgo=PORT2_CD32_RESET_HIGH;
    OwnBlitter(); WaitBlit(); Forbid(); systemLocked=TRUE;
    Disable(); interruptsDisabled=TRUE;
    hardware->intena=0x7fff; hardware->dmacon=DMAF_ALL;
    hardware->cop1lc=(ULONG)copper; hardware->copjmp1=0;
    /* Title/loading/ready Copper lists do not own sprite pointers. Keep
       sprite DMA disabled until the gameplay list that initializes all
       attached-player pointers is installed. Otherwise stale OS sprite
       pointers can fetch intermittent fragments over an idle ready screen. */
    hardware->dmacon=DMAF_SETCLR|DMAF_MASTER|DMAF_RASTER|DMAF_COPPER|
                     DMAF_BLITTER;
    audioSetHardwareActive(TRUE);
}

void platformSwitchCopper(UWORD *copper)
{
    hardware->cop1lc=(ULONG)copper;
    hardware->copjmp1=0;
    /* The sole production caller installs rendererCopperList(), whose early
       moves initialize every gameplay sprite pointer before visible fetch. */
    hardware->dmacon=DMAF_SETCLR|DMAF_SPRITE;
}

void platformReleaseForLoading(BOOL keepDisplay)
{
    if(interruptsDisabled) {
        audioSetHardwareActive(FALSE);
        if(!keepDisplay) {
            hardware->dmacon=DMAF_RASTER|DMAF_COPPER|DMAF_SPRITE;
            hardware->color[0]=0;
        }
        /* Re-enable only the saved disk channel in addition to the current
           presentation DMA. DOS may now load the next resident state while
           Workbench remains detached and therefore cannot flash on screen. */
        hardware->dmacon=DMAF_SETCLR|DMAF_MASTER|(oldDma&DMAF_DISK);
        hardware->intena=0x7fff;
        hardware->intena=0x8000|oldIntena;
        Enable(); interruptsDisabled=FALSE;
    }
    hardware->potgo=0;
    if(systemLocked) {
        DisownBlitter(); Permit(); systemLocked=FALSE;
    }
}

void platformResetGameInput(void)
{
    gameKeys=0;
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
    hardware->potgo=0;
    if(systemLocked) {
        DisownBlitter(); Permit(); systemLocked=FALSE;
    }
    if(!interruptsDisabled&&systemView&&GfxBase->ActiView!=systemView) {
        LoadView(systemView); WaitTOF(); WaitTOF();
    }
}

BOOL platformSecondaryButtonHeld(void)
{
    return (hardware->potinp&PORT2_SECOND_BUTTON)==0;
}

#ifdef SPARKPAW_WHDLOAD
BOOL platformWHDLoadQuitRequested(void)
{
    return whdloadQuitRequested;
}
#endif

UWORD platformRasterLine(void)
{
    UWORD high=hardware->vposr,low=hardware->vhposr;
    return (UWORD)(((high&7)<<8)|(low>>8));
}

void platformWaitBlit(void)
{
#if defined(SPARKPAW_RENDER_DIAGNOSTIC) && \
    !defined(SPARKPAW_DISABLE_WAIT_PROFILE)
    ULONG profileStart=performanceProfileBegin();
#endif
    (void)hardware->dmaconr;
    while(hardware->dmaconr&DMAF_BLTDONE) { }
#if defined(SPARKPAW_RENDER_DIAGNOSTIC) && \
    !defined(SPARKPAW_DISABLE_WAIT_PROFILE)
    performanceProfileEnd(PERF_BLITTER_WAIT,profileStart);
#endif
}

void platformSetBlitterPriority(BOOL enabled)
{
    hardware->dmacon=enabled?(DMAF_SETCLR|DMAF_BLITHOG):DMAF_BLITHOG;
}

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
static UWORD readProfileCounter(void)
{
    UBYTE high0,high1,low;
    do {
        high0=CIAB_TBHI; low=CIAB_TBLO; high1=CIAB_TBHI;
    } while(high0!=high1);
    return (UWORD)(((UWORD)high1<<8)|low);
}

void platformProfileTimerStart(void)
{
    profileOldCrb=CIAB_CRB;
    profileOldTbLo=CIAB_TBLO;
    profileOldTbHi=CIAB_TBHI;
    CIAB_CRB=0;
    CIAB_TBLO=0xff; CIAB_TBHI=0xff;
    /* Continuous E-clock countdown. Diagnostics sample often enough that one
       65536-tick period (about 92 ms PAL) cannot pass unseen. */
    CIAB_CRB=0x11;
    profileLastCounter=readProfileCounter();
    profileTicks=0; profileTimerActive=TRUE;
}

ULONG platformProfileTimerTicks(void)
{
    UWORD current=readProfileCounter();
    profileTicks+=(UWORD)(profileLastCounter-current);
    profileLastCounter=current;
    return profileTicks;
}

static void stopProfileTimer(void)
{
    if(!profileTimerActive) return;
    CIAB_CRB=0;
    CIAB_TBLO=profileOldTbLo; CIAB_TBHI=profileOldTbHi;
    CIAB_CRB=(UBYTE)(profileOldCrb|0x10);
    profileTimerActive=FALSE;
}

BOOL platformBlitterBusy(void)
{
    return (hardware->dmaconr&DMAF_BLTDONE)!=0;
}

BOOL platformLeftMouse(void)
{
    return ((*(volatile UBYTE *)0xbfe001)&0x40)==0;
}

void platformPrepareDebugFlush(void)
{
    /* A debug flush needs DOS scheduling and interrupts, not Workbench.  Stop
       every custom-chip user of gameplay memory, restore the saved interrupt
       mask and release the OS locks.  The caller writes/closes its log and
       then waits for reset without freeing or redisplaying any bitmap. */
    if(interruptsDisabled) {
        stopProfileTimer();
        audioSetHardwareActive(FALSE);
        hardware->dmacon=DMAF_ALL;
        /* DOS can flush an HD log without custom-chip disk DMA, which hid
           this omission in the first Stage 1 run. DF0 requires the saved
           system DMA channels again before Open()/Write()/Close(). */
        hardware->dmacon=DMAF_SETCLR|DMAF_MASTER|oldDma;
        hardware->intena=0x7fff;
        hardware->intena=0x8000|oldIntena;
        Enable(); interruptsDisabled=FALSE;
    }
    if(systemLocked) {
        DisownBlitter(); Permit(); systemLocked=FALSE;
    }
}
#endif

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
#ifdef SPARKPAW_WHDLOAD
        /* Once Sparkpaw owns the custom chips, Exec's keyboard interrupt no
           longer reaches the KickEmu quit-key patch. Preserve F10 explicitly
           and return through the program/slave boundary instead. */
        if(code==0x59) whdloadQuitRequested=TRUE;
#endif
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
