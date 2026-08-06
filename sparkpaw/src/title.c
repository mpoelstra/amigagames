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

static volatile struct Custom *hardware=(volatile struct Custom *)0xdff000;
static struct View *previousView;
static UWORD *copper[2];
static UWORD copperPos,savedDma;
static UBYTE buildCopperIndex,currentCopper;
static BOOL displayed;
static ULONG titleStartFrame;
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

static void writePalette(const struct PlanarAsset *asset)
{
    UWORD bank,index;
    for(bank=0;bank<2;bank++) {
        cmove(0x106,(UWORD)(0x0020|(bank<<13)));
        for(index=0;index<32;index++) {
            const UBYTE *rgb=asset->palette[bank*32+index];
            cmove((UWORD)(0x180+index*2),
                  (UWORD)(((rgb[0]>>4)<<8)|((rgb[1]>>4)<<4)|(rgb[2]>>4)));
        }
        cmove(0x106,(UWORD)(0x0220|(bank<<13)));
        for(index=0;index<32;index++) {
            const UBYTE *rgb=asset->palette[bank*32+index];
            cmove((UWORD)(0x180+index*2),
                  (UWORD)(((rgb[0]&15)<<8)|((rgb[1]&15)<<4)|(rgb[2]&15)));
        }
    }
    cmove(0x106,0x0020);
}

static void buildCopper(const struct PlanarAsset *asset,UBYTE index)
{
    UBYTE plane;
    buildCopperIndex=index; copperPos=0;
    cmove(0x08e,0x2c81); cmove(0x090,0x2cc1);
    cmove(0x092,0x0038); cmove(0x094,0x00d0);
    cmove(0x100,0x6200); cmove(0x102,0x0000);
    cmove(0x104,0x0000); cmove(0x106,0x0020);
    cmove(0x108,(UWORD)(asset->bitmap->BytesPerRow-SCREEN_ROW_BYTES));
    cmove(0x10a,(UWORD)(asset->bitmap->BytesPerRow-SCREEN_ROW_BYTES));
    cmove(0x10c,0x0000); cmove(0x1fc,0x0000);
    for(plane=0;plane<6;plane++)
        cptr((UWORD)(0x0e0+plane*4),asset->bitmap->Planes[plane]);
    writePalette(asset);
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
    buildCopper(assetsTitle(),0);
    installCopper(0);
    titleStartFrame=GfxBase->VBCounter;
    return TRUE;
}

BOOL titlePrepareLevelLoading(void)
{
    UBYTE next;
    if(!displayed) {
        failureReason="title display unavailable for loading image";
        return FALSE;
    }
    if(!assetsLoadLevelLoading()) {
        failureReason="six-plane loading image asset load failed"; return FALSE;
    }
    next=currentCopper^1;
    buildCopper(assetsLevelLoading(),next);
    return TRUE;
}

BOOL titleShowLevelLoading(void)
{
    UBYTE next=currentCopper^1;
    if(!assetsLevelLoading()->bitmap) {
        failureReason="loading image was not prepared"; return FALSE;
    }
    installCopper(next);
    assetsUnloadTitle();
    return TRUE;
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
    assetsUnloadTitle(); assetsUnloadLevelLoading();
    for(index=0;index<2;index++) {
        if(copper[index]) {
            FreeMem(copper[index],COPPER_WORDS*sizeof(UWORD));
            copper[index]=NULL;
        }
    }
}