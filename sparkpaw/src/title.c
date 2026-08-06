#include "title.h"

#include <exec/memory.h>
#include <graphics/gfxbase.h>
#include <graphics/modeid.h>
#include <intuition/intuition.h>
#include <intuition/screens.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>

#include "assets.h"

struct IntuitionBase *IntuitionBase;

static struct Screen *screen[2];
static struct Window *window[2];
static UWORD *hiddenPointer[2];
static UBYTE currentScreen;
static ULONG colors[1+16*3+1];
static const char *failureReason="unknown title failure";
static LONG screenError;
static ULONG chipFree,chipLargest;

static void prepareColors(const struct PlanarAsset *asset)
{
    UWORD index;
    colors[0]=(16UL<<16);
    for(index=0;index<16;index++) {
        colors[1+index*3]=(ULONG)asset->palette[index][0]*0x01010101UL;
        colors[2+index*3]=(ULONG)asset->palette[index][1]*0x01010101UL;
        colors[3+index*3]=(ULONG)asset->palette[index][2]*0x01010101UL;
    }
    colors[1+16*3]=0;
}

static void closeDisplay(UBYTE index)
{
    if(window[index]) {
        CloseWindow(window[index]); window[index]=NULL;
    }
    if(screen[index]) {
        CloseScreen(screen[index]); screen[index]=NULL;
    }
    if(hiddenPointer[index]) {
        FreeMem(hiddenPointer[index],4); hiddenPointer[index]=NULL;
    }
}

static BOOL openDisplay(UBYTE index,const struct PlanarAsset *asset)
{
    screenError=0;
    screen[index]=OpenScreenTags(NULL,
        SA_Width,320,SA_Height,256,SA_Depth,4,
        SA_DisplayID,LORES_KEY,SA_ErrorCode,(ULONG)&screenError,
        SA_Title,(ULONG)"Sparkpaw",SA_Quiet,TRUE,SA_ShowTitle,FALSE,
        SA_Behind,TRUE,TAG_DONE);
    if(!screen[index]) return FALSE;
    window[index]=OpenWindowTags(NULL,
        WA_CustomScreen,(ULONG)screen[index],WA_Left,0,WA_Top,0,
        WA_Width,320,WA_Height,256,WA_Borderless,TRUE,WA_Backdrop,TRUE,
        WA_Activate,TRUE,WA_RMBTrap,TRUE,WA_IDCMP,0,TAG_DONE);
    if(!window[index]) { closeDisplay(index); return FALSE; }
    hiddenPointer[index]=(UWORD *)AllocMem(4,MEMF_CHIP|MEMF_CLEAR);
    if(hiddenPointer[index])
        SetPointer(window[index],hiddenPointer[index],1,16,0,0);
    BltBitMap(asset->bitmap,0,0,screen[index]->RastPort.BitMap,0,0,320,256,
              0xc0,0x3f,NULL);
    WaitBlit();
    prepareColors(asset); LoadRGB32(&screen[index]->ViewPort,colors);
    return TRUE;
}

BOOL titleShow(void)
{
    if(!assetsLoadTitle()) {
        failureReason="title asset load failed"; return FALSE;
    }
    IntuitionBase=(struct IntuitionBase *)OpenLibrary("intuition.library",39);
    if(!IntuitionBase) {
        failureReason="intuition.library unavailable";
        titleRelease(); return FALSE;
    }
    screenError=0;
    chipFree=AvailMem(MEMF_CHIP);
    chipLargest=AvailMem(MEMF_CHIP|MEMF_LARGEST);
    currentScreen=0;
    if(!openDisplay(currentScreen,assetsTitle())) {
        failureReason="16-colour title screen open failed";
        titleRelease(); return FALSE;
    }
    assetsUnloadTitle();
    ScreenToFront(screen[currentScreen]); WaitTOF(); WaitTOF();
    return TRUE;
}

BOOL titleShowLevelLoading(void)
{
    UBYTE next;
    if(!screen[currentScreen]) {
        failureReason="title screen unavailable for loading image";
        return FALSE;
    }
    if(!assetsLoadLevelLoading()) {
        failureReason="loading image asset load failed"; return FALSE;
    }
    next=currentScreen^1;
    if(!openDisplay(next,assetsLevelLoading())) {
        failureReason="16-colour loading screen open failed";
        assetsUnloadLevelLoading(); return FALSE;
    }
    assetsUnloadLevelLoading();
    ScreenToFront(screen[next]); WaitTOF(); WaitTOF();
    closeDisplay(currentScreen); currentScreen=next;
    return TRUE;
}

const char *titleFailureReason(void)
{
    return failureReason;
}

LONG titleScreenError(void) { return screenError; }
ULONG titleChipFree(void) { return chipFree; }
ULONG titleChipLargest(void) { return chipLargest; }

void titleWaitFrames(UWORD frames)
{
    while(frames--) WaitTOF();
}

void titleRestoreSystemView(void)
{
    titleRelease();
}

void titleRelease(void)
{
    closeDisplay(0); closeDisplay(1);
    assetsUnloadTitle(); assetsUnloadLevelLoading();
    if(IntuitionBase) {
        CloseLibrary((struct Library *)IntuitionBase);
        IntuitionBase=NULL;
    }
}