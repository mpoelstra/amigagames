#include "assets.h"

#include <exec/memory.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <string.h>

static struct PlanarAsset title,levelLoading,levelCharging,frontClean,rearWorld;
static struct PlanarAsset playerSprites,enemySprites,striderSprites;
static struct PlanarAsset hudBase,hudHealth,hudLives,hudDiamonds;
static struct PlanarAsset collectibleDiamond;

static UWORD readBigEndian16(const UBYTE *value)
{
    return (UWORD)(((UWORD)value[0]<<8)|value[1]);
}

static void freeAsset(struct PlanarAsset *asset)
{
    if(asset->mask) {
        FreeMem(asset->mask,(LONG)asset->rowBytes*asset->height);
        asset->mask=NULL;
    }
    if(asset->bitmap) {
        FreeBitMap(asset->bitmap); asset->bitmap=NULL;
    }
}

static BOOL readRows(BPTR file,PLANEPTR plane,UWORD fileRow,UWORD memoryRow,
                     UWORD height)
{
    UWORD row;
    if(fileRow==memoryRow)
        return Read(file,plane,(LONG)fileRow*height)==(LONG)fileRow*height;
    for(row=0;row<height;row++)
        if(Read(file,plane+(LONG)row*memoryRow,fileRow)!=fileRow) return FALSE;
    return TRUE;
}

static BOOL loadAsset(const char *name,struct PlanarAsset *asset,
                      UBYTE wantedDepth)
{
    BPTR file; UBYTE header[12],plane; LONG size;
    memset(asset,0,sizeof(*asset));
    file=Open((STRPTR)name,MODE_OLDFILE); if(!file) return FALSE;
    if(Read(file,header,12)!=12||memcmp(header,"SPBM",4)!=0) {
        Close(file); return FALSE;
    }
    asset->width=readBigEndian16(header+4);
    asset->height=readBigEndian16(header+6);
    asset->depth=header[8]; asset->hasMask=header[9];
    asset->rowBytes=readBigEndian16(header+10);
    if(asset->depth!=wantedDepth||
       Read(file,asset->palette,(LONG)(1<<asset->depth)*3)!=
       (LONG)(1<<asset->depth)*3) {
        Close(file); return FALSE;
    }
    asset->bitmap=AllocBitMap(asset->width,asset->height,asset->depth,
                              BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    if(!asset->bitmap) { Close(file); return FALSE; }
    for(plane=0;plane<asset->depth;plane++)
        if(!readRows(file,asset->bitmap->Planes[plane],asset->rowBytes,
                     asset->bitmap->BytesPerRow,asset->height)) {
            Close(file); freeAsset(asset); return FALSE;
        }
    if(asset->hasMask) {
        size=(LONG)asset->rowBytes*asset->height;
        asset->mask=(UBYTE *)AllocMem(size,MEMF_CHIP);
        if(!asset->mask||Read(file,asset->mask,size)!=size) {
            Close(file); freeAsset(asset); return FALSE;
        }
    }
    Close(file); return TRUE;
}

BOOL assetsLoadGameplay(void)
{
    return loadAsset("PROGDIR:assets/runtime/storm-front.spbm",&frontClean,4)&&
           loadAsset("PROGDIR:assets/runtime/storm-rear.spbm",&rearWorld,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-sprites4.spbm",
                     &playerSprites,4)&&
           loadAsset("PROGDIR:assets/runtime/clockwork-beetle.spbm",
                     &enemySprites,4)&&
           loadAsset("PROGDIR:assets/runtime/clockwork-storm-strider.spbm",
                     &striderSprites,4)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-base.spbm",
                     &hudBase,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-health.spbm",
                     &hudHealth,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-lives.spbm",
                     &hudLives,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-diamonds.spbm",
                     &hudDiamonds,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-diamond.spbm",
                     &collectibleDiamond,4);
}

BOOL assetsLoadTitle(void)
{
    return loadAsset("PROGDIR:assets/runtime/sparkpaw-title.spbm",&title,6);
}

void assetsUnloadTitle(void)
{
    freeAsset(&title);
}

BOOL assetsLoadLevelLoading(void)
{
    return loadAsset("PROGDIR:assets/runtime/sparkpaw-level-loading.spbm",
                     &levelLoading,6);
}

void assetsUnloadLevelLoading(void)
{
    freeAsset(&levelLoading);
}

BOOL assetsLoadLevelCharging(void)
{
    return loadAsset("PROGDIR:assets/runtime/sparkpaw-level-charging.spbm",
                     &levelCharging,6);
}

void assetsUnloadLevelCharging(void)
{
    freeAsset(&levelCharging);
}

void assetsUnloadGameplay(void)
{
    freeAsset(&collectibleDiamond); freeAsset(&hudDiamonds);
    freeAsset(&hudLives); freeAsset(&hudHealth); freeAsset(&hudBase);
    freeAsset(&striderSprites); freeAsset(&enemySprites);
    freeAsset(&playerSprites);
    freeAsset(&rearWorld); freeAsset(&frontClean);
}

const struct PlanarAsset *assetsTitle(void) { return &title; }
const struct PlanarAsset *assetsLevelLoading(void) { return &levelLoading; }
const struct PlanarAsset *assetsLevelCharging(void) { return &levelCharging; }
const struct PlanarAsset *assetsFrontClean(void) { return &frontClean; }
const struct PlanarAsset *assetsRearWorld(void) { return &rearWorld; }
const struct PlanarAsset *assetsPlayerSprites(void) { return &playerSprites; }
const struct PlanarAsset *assetsEnemySprites(void) { return &enemySprites; }
const struct PlanarAsset *assetsStriderSprites(void) { return &striderSprites; }
const struct PlanarAsset *assetsHudBase(void) { return &hudBase; }
const struct PlanarAsset *assetsHudHealth(void) { return &hudHealth; }
const struct PlanarAsset *assetsHudLives(void) { return &hudLives; }
const struct PlanarAsset *assetsHudDiamonds(void) { return &hudDiamonds; }
const struct PlanarAsset *assetsCollectibleDiamond(void)
{
    return &collectibleDiamond;
}
