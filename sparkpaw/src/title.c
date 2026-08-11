#include "title.h"

#include <exec/memory.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <proto/exec.h>
#include <proto/graphics.h>

#include "assets.h"

#define COPPER_WORDS 340
#define SCREEN_ROW_BYTES 40
#define BPLCON0_SIX_PLANES_AGA 0x6201
#define BPLCON2_KILLEHB 0x0200
#define BPLCON3_BORDER_BLANK 0x0020
#define DIWHIGH_PAL_320X256 0x2100
#define FADE_FRAMES 24
#define DISPLAY_LOCK_FRAMES 35
#define PALETTE_SAFE_LINE 100

static volatile struct Custom *hardware=(volatile struct Custom *)0xdff000;
static struct View *previousView;
static UWORD *copper[2];
static UWORD paletteValuePos[2][2][64];
static UWORD copperPos,savedDma;
static UBYTE buildCopperIndex,currentCopper;
static BOOL displayed;
static ULONG titleStartFrame;
static ULONG chargingStartFrame;
static const char *failureReason="unknown title failure";
static ULONG chipFree,chipLargest;

static void cmove(UWORD reg,UWORD value)
{
    copper[buildCopperIndex][copperPos++]=reg;
    copper[buildCopperIndex][copperPos++]=value;
}

static void cptr(UWORD reg,APTR value)
{
    ULONG pointer=(ULONG)value;
    cmove(reg,(UWORD)(pointer>>16));
    cmove((UWORD)(reg+2),(UWORD)pointer);
}

static UBYTE fadeComponent(UBYTE component,UWORD level)
{
    return (UBYTE)(((ULONG)component*level)>>8);
}

static UWORD paletteWord(const UBYTE *rgb,UWORD level,BOOL lowNibble)
{
    UBYTE red=fadeComponent(rgb[0],level);
    UBYTE green=fadeComponent(rgb[1],level);
    UBYTE blue=fadeComponent(rgb[2],level);
    if(lowNibble)
        return (UWORD)(((red&15)<<8)|((green&15)<<4)|(blue&15));
    return (UWORD)(((red>>4)<<8)|((green>>4)<<4)|(blue>>4));
}

static void writePalette(const struct PlanarAsset *asset,UWORD level)
{
    UWORD bank,index;
    for(bank=0;bank<2;bank++) {
        cmove(0x106,(UWORD)(0x0020|(bank<<13)));
        for(index=0;index<32;index++) {
            const UBYTE *rgb=asset->palette[bank*32+index];
            UWORD pen=(UWORD)(bank*32+index);
            paletteValuePos[buildCopperIndex][0][pen]=copperPos+1;
            cmove((UWORD)(0x180+index*2),
                  paletteWord(rgb,level,FALSE));
        }
        cmove(0x106,(UWORD)(0x0220|(bank<<13)));
        for(index=0;index<32;index++) {
            const UBYTE *rgb=asset->palette[bank*32+index];
            UWORD pen=(UWORD)(bank*32+index);
            paletteValuePos[buildCopperIndex][1][pen]=copperPos+1;
            cmove((UWORD)(0x180+index*2),
                  paletteWord(rgb,level,TRUE));
        }
    }
    cmove(0x106,0x0020);
}

static void buildCopper(const struct PlanarAsset *asset,UBYTE index,UWORD level)
{
    UBYTE plane;
    buildCopperIndex=index; copperPos=0;
    cmove(0x08e,0x2c81); cmove(0x090,0x2cc1);
    /* DIWSTRT/DIWSTOP reset DIWHIGH for legacy compatibility. Write the
       complete PAL 320x256 stop high bits afterwards so an AGA Workbench
       mode cannot leak its extended display-window state into this View. */
    cmove(0x1e4,DIWHIGH_PAL_320X256);
    cmove(0x092,0x0038); cmove(0x094,0x00d0);
    /* The OS-managed reference View selected ordinary 64-colour AGA mode.
       KILLEHB is required here as well; without it pens 32..63 are interpreted
       as half-bright copies of pens 0..31. ECSENA makes the AGA controls,
       including border blanking and palette banking, explicit rather than
       inheriting their availability from the preceding Workbench display. */
    cmove(0x100,BPLCON0_SIX_PLANES_AGA); cmove(0x102,0x0000);
    cmove(0x104,BPLCON2_KILLEHB); cmove(0x106,BPLCON3_BORDER_BLANK);
    cmove(0x108,(UWORD)(asset->bitmap->BytesPerRow-SCREEN_ROW_BYTES));
    cmove(0x10a,(UWORD)(asset->bitmap->BytesPerRow-SCREEN_ROW_BYTES));
    cmove(0x10c,0x0000); cmove(0x1fc,0x0000);
    for(plane=0;plane<6;plane++)
        cptr((UWORD)(0x0e0+plane*4),asset->bitmap->Planes[plane]);
    writePalette(asset,level);
    copper[index][copperPos++]=0xffff;
    copper[index][copperPos++]=0xfffe;
}

static BOOL allocateCopper(void)
{
    UBYTE index;
    for(index=0;index<2;index++) {
        copper[index]=(UWORD *)AllocMem(COPPER_WORDS*sizeof(UWORD),
                                        MEMF_CHIP|MEMF_CLEAR);
        if(!copper[index]) return FALSE;
    }
    return TRUE;
}

static void installCopper(UBYTE index)
{
    WaitTOF();
    hardware->dmacon=DMAF_RASTER|DMAF_COPPER|DMAF_SPRITE;
    hardware->cop1lc=(ULONG)copper[index]; hardware->copjmp1=0;
    hardware->dmacon=DMAF_SETCLR|DMAF_MASTER|DMAF_RASTER|DMAF_COPPER;
    currentCopper=index; displayed=TRUE;
}

static UWORD rasterLine(void)
{
    UWORD high=hardware->vposr,low=hardware->vhposr;
    return (UWORD)(((high&7)<<8)|(low>>8));
}

static void stagePalette(const struct PlanarAsset *asset,UWORD level)
{
    UWORD pen;
    while(rasterLine()<PALETTE_SAFE_LINE) { }
    for(pen=0;pen<64;pen++) {
        const UBYTE *rgb=asset->palette[pen];
        copper[currentCopper][paletteValuePos[currentCopper][0][pen]]=
            paletteWord(rgb,level,FALSE);
        copper[currentCopper][paletteValuePos[currentCopper][1][pen]]=
            paletteWord(rgb,level,TRUE);
    }
}

static void fadeTo(const struct PlanarAsset *asset,BOOL fadeIn)
{
    UWORD frame;
    for(frame=1;frame<=FADE_FRAMES;frame++) {
        UWORD level=fadeIn?(UWORD)(((ULONG)frame*256)/FADE_FRAMES):
                           (UWORD)(((ULONG)(FADE_FRAMES-frame)*256)/FADE_FRAMES);
        stagePalette(asset,level);
        WaitTOF();
    }
}

BOOL titleShow(void)
{
    chipFree=AvailMem(MEMF_CHIP);
    chipLargest=AvailMem(MEMF_CHIP|MEMF_LARGEST);
    if(!assetsLoadTitle()) {
        failureReason="six-plane title asset load failed"; return FALSE;
    }
    if(!allocateCopper()) {
        failureReason="title Copper allocation failed";
        titleRelease(); return FALSE;
    }
    previousView=GfxBase->ActiView;
    savedDma=hardware->dmaconr&DMAF_ALL;
    /* Detach the active OS View before owning COP1LC directly. DOS and
       interrupts remain available for loading, but graphics.library no
       longer has a Workbench Copper to reinstall at VBlank. A minimal ADF
       boot happened to leave an inert View; Workbench does not. */
    LoadView(NULL);
    WaitTOF(); WaitTOF();
    buildCopper(assetsTitle(),0,0);
    installCopper(0);
    /* Give scandoublers time to lock to the PAL low-resolution display while
       it is deliberately black, then begin the authored fade. */
    {
        UWORD frame;
        for(frame=0;frame<DISPLAY_LOCK_FRAMES;frame++) WaitTOF();
    }
    fadeTo(assetsTitle(),TRUE);
    titleStartFrame=GfxBase->VBCounter;
    return TRUE;
}

BOOL titlePrepareLevelLoading(void)
{
    if(!displayed) {
        failureReason="title display unavailable for loading image";
        return FALSE;
    }
    if(!assetsLoadLevelLoading()) {
        failureReason="six-plane loading image asset load failed"; return FALSE;
    }
    if(!assetsLoadLevelCharging()) {
        failureReason="six-plane charging image asset load failed";
        assetsUnloadLevelLoading();
        return FALSE;
    }
    buildCopper(assetsLevelLoading(),currentCopper^1,0);
    return TRUE;
}

BOOL titleShowLevelCharging(void)
{
    UBYTE next=currentCopper^1;
    if(!displayed||!assetsLevelCharging()->bitmap) {
        failureReason="charging image was not prepared"; return FALSE;
    }
    /* Both status images share one palette. Rebuild only the inactive list,
       switch it at VBlank, then release the no-longer-visible bitmap before
       the CPU-heavy gameplay preparation begins. */
    buildCopper(assetsLevelCharging(),next,256);
    installCopper(next);
    chargingStartFrame=GfxBase->VBCounter;
    assetsUnloadLevelLoading();
    return TRUE;
}

void titleWaitLevelCharging(UWORD frames)
{
    while((ULONG)(GfxBase->VBCounter-chargingStartFrame)<frames) WaitTOF();
}

BOOL titleShowLevelLoading(void)
{
    UBYTE next=currentCopper^1;
    if(!assetsLevelLoading()->bitmap) {
        failureReason="loading image was not prepared"; return FALSE;
    }
    fadeTo(assetsTitle(),FALSE);
    next=currentCopper^1;
    installCopper(next);
    assetsUnloadTitle();
    fadeTo(assetsLevelLoading(),TRUE);
    return TRUE;
}

void titleFadeOut(void)
{
    if(displayed&&assetsLevelCharging()->bitmap)
        fadeTo(assetsLevelCharging(),FALSE);
    else if(displayed&&assetsLevelLoading()->bitmap)
        fadeTo(assetsLevelLoading(),FALSE);
}

const char *titleFailureReason(void) { return failureReason; }
ULONG titleChipFree(void) { return chipFree; }
ULONG titleChipLargest(void) { return chipLargest; }

void titleWaitFrames(UWORD frames)
{
    while((ULONG)(GfxBase->VBCounter-titleStartFrame)<frames) WaitTOF();
}

void titleRestoreSystemView(void)
{
    if(!displayed) return;
    WaitTOF();
    hardware->dmacon=DMAF_RASTER|DMAF_COPPER|DMAF_SPRITE;
    if(previousView) LoadView(previousView);
    hardware->dmacon=DMAF_SETCLR|DMAF_MASTER|savedDma;
    WaitTOF(); WaitTOF();
    displayed=FALSE;
}

void titleRelease(void)
{
    UBYTE index;
    displayed=FALSE;
    assetsUnloadTitle(); assetsUnloadLevelLoading(); assetsUnloadLevelCharging();
    for(index=0;index<2;index++) {
        if(copper[index]) {
            FreeMem(copper[index],COPPER_WORDS*sizeof(UWORD));
            copper[index]=NULL;
        }
    }
}
