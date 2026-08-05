#include "title.h"

#include <graphics/copper.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <proto/graphics.h>
#include <string.h>

#include "assets.h"

static struct View view;
static struct ViewPort viewport;
static struct RasInfo rasInfo;
static struct View *previousView;
static struct ColorMap *colorMap;
static ULONG colors[1+64*3+1];
static BOOL displayed;

BOOL titleShowLoading(void)
{
    const struct PlanarAsset *title;
    UWORD index;
    if(!assetsLoadTitle()) return FALSE;
    title=assetsTitle();
    memset(&view,0,sizeof(view)); memset(&viewport,0,sizeof(viewport));
    memset(&rasInfo,0,sizeof(rasInfo));
    InitView(&view); InitVPort(&viewport);
    previousView=GfxBase->ActiView;
    view.ViewPort=&viewport;
    viewport.DWidth=320; viewport.DHeight=256;
    viewport.RasInfo=&rasInfo; rasInfo.BitMap=title->bitmap;
    colorMap=GetColorMap(64); viewport.ColorMap=colorMap;
    if(!colorMap) { titleRelease(); return FALSE; }
    colors[0]=(64UL<<16);
    for(index=0;index<64;index++) {
        colors[1+index*3]=(ULONG)title->palette[index][0]*0x01010101UL;
        colors[2+index*3]=(ULONG)title->palette[index][1]*0x01010101UL;
        colors[3+index*3]=(ULONG)title->palette[index][2]*0x01010101UL;
    }
    colors[1+64*3]=0;
    LoadRGB32(&viewport,colors);
    if(MakeVPort(&view,&viewport)!=MVP_OK||MrgCop(&view)!=MCOP_OK) {
        titleRelease(); return FALSE;
    }
    LoadView(&view); WaitTOF(); WaitTOF(); displayed=TRUE;
    return TRUE;
}

void titleRestoreSystemView(void)
{
    if(displayed) {
        LoadView(previousView); WaitTOF(); WaitTOF(); displayed=FALSE;
    }
}

void titleRelease(void)
{
    if(viewport.DspIns) FreeVPortCopLists(&viewport);
    if(view.LOFCprList) FreeCprList(view.LOFCprList);
    if(view.SHFCprList) FreeCprList(view.SHFCprList);
    if(colorMap) { FreeColorMap(colorMap); colorMap=NULL; }
    assetsUnloadTitle();
    memset(&view,0,sizeof(view)); memset(&viewport,0,sizeof(viewport));
    displayed=FALSE;
}