#include "hud.h"

#include <exec/memory.h>
#include <hardware/custom.h>
#include <proto/exec.h>
#include <proto/graphics.h>

#include "assets.h"
#include "game.h"
#include "platform_amiga.h"
#include "player.h"

#define HUD_W 336
#define HUD_H 48
#define HUD_PLANES 3
#define HUD_BUFFERS 2
#define HEALTH_X 48
#define HEALTH_Y 12
#define HEALTH_W 80
#define HEALTH_H 32
#define HEALTH_STATES (PLAYER_MAX_HEALTH+1)
#define LIVES_X 160
#define LIVES_Y 12
#define LIVES_W 32
#define LIVES_H 24
#define DIAMONDS_X 224
#define DIAMONDS_Y 12
#define DIAMONDS_W 32
#define DIAMONDS_H 24
#define DIAMOND_STATES 50

static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static const struct PlanarAsset *base,*healthAtlas,*livesAtlas,*diamondsAtlas;
static struct BitMap *buffers[HUD_BUFFERS];
static UBYTE *blankPlane;
static UBYTE bufferHealth[HUD_BUFFERS],bufferLives[HUD_BUFFERS];
static UBYTE bufferDiamonds[HUD_BUFFERS],current;

static void copyPatchCpu(const struct PlanarAsset *source,UWORD sourceY,
                         struct BitMap *dest,UWORD x,UWORD y,
                         UWORD width,UWORD height)
{
    UBYTE plane; UWORD row,bytes=width>>3;
    for(plane=0;plane<HUD_PLANES;plane++) for(row=0;row<height;row++)
        CopyMem(source->bitmap->Planes[plane]+(LONG)(sourceY+row)*
                source->bitmap->BytesPerRow,
                dest->Planes[plane]+(LONG)(y+row)*dest->BytesPerRow+(x>>3),
                bytes);
}

static void copyPatchBlitter(const struct PlanarAsset *source,UWORD sourceY,
                             struct BitMap *dest,UWORD x,UWORD y,
                             UWORD width,UWORD height)
{
    UBYTE plane; UWORD words=width>>4;
    LONG sourceAt=(LONG)sourceY*source->bitmap->BytesPerRow;
    LONG destAt=(LONG)y*dest->BytesPerRow+(x>>3);
    for(plane=0;plane<HUD_PLANES;plane++) {
        platformWaitBlit();
        hw->bltcon0=0x09f0; hw->bltcon1=0;
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=(UWORD)(source->bitmap->BytesPerRow-width/8);
        hw->bltdmod=(UWORD)(dest->BytesPerRow-width/8);
        hw->bltapt=source->bitmap->Planes[plane]+sourceAt;
        hw->bltdpt=dest->Planes[plane]+destAt;
        hw->bltsize=(UWORD)((height<<6)|words);
    }
}

static void composeCpu(UBYTE index,UBYTE health,UBYTE lives,UBYTE diamonds)
{
    UBYTE plane;
    for(plane=0;plane<HUD_PLANES;plane++)
        CopyMem(base->bitmap->Planes[plane],buffers[index]->Planes[plane],
                (LONG)base->bitmap->BytesPerRow*HUD_H);
    copyPatchCpu(healthAtlas,(UWORD)(health*HEALTH_H),buffers[index],
                 HEALTH_X,HEALTH_Y,HEALTH_W,HEALTH_H);
    copyPatchCpu(livesAtlas,(UWORD)((lives-1)*LIVES_H),buffers[index],
                 LIVES_X,LIVES_Y,LIVES_W,LIVES_H);
    copyPatchCpu(diamondsAtlas,(UWORD)(diamonds*DIAMONDS_H),buffers[index],
                 DIAMONDS_X,DIAMONDS_Y,DIAMONDS_W,DIAMONDS_H);
    bufferHealth[index]=health; bufferLives[index]=lives;
    bufferDiamonds[index]=diamonds;
}

BOOL hudPrepare(void)
{
    UBYTE index;
    base=assetsHudBase(); healthAtlas=assetsHudHealth();
    livesAtlas=assetsHudLives(); diamondsAtlas=assetsHudDiamonds();
    if(base->depth!=HUD_PLANES||healthAtlas->depth!=HUD_PLANES||
       livesAtlas->depth!=HUD_PLANES||diamondsAtlas->depth!=HUD_PLANES||
       base->width!=HUD_W||base->height!=HUD_H||
       healthAtlas->width!=HEALTH_W||healthAtlas->height!=HEALTH_H*HEALTH_STATES||
       livesAtlas->width!=LIVES_W||livesAtlas->height!=LIVES_H*GAME_START_LIVES||
       diamondsAtlas->width!=DIAMONDS_W||
       diamondsAtlas->height!=DIAMONDS_H*DIAMOND_STATES)
        return FALSE;
    blankPlane=(UBYTE *)AllocMem((LONG)base->bitmap->BytesPerRow*HUD_H,
                                 MEMF_CHIP|MEMF_CLEAR);
    if(!blankPlane) return FALSE;
    for(index=0;index<HUD_BUFFERS;index++) {
        buffers[index]=AllocBitMap(HUD_W,HUD_H,HUD_PLANES,
                                  BMF_CLEAR|BMF_DISPLAYABLE,NULL);
        if(!buffers[index]) return FALSE;
        if(buffers[index]->BytesPerRow!=base->bitmap->BytesPerRow)
            return FALSE;
        composeCpu(index,PLAYER_MAX_HEALTH,GAME_START_LIVES,0);
    }
    current=0;
    return TRUE;
}

void hudRelease(void)
{
    UBYTE index;
    for(index=0;index<HUD_BUFFERS;index++) {
        if(buffers[index]) { FreeBitMap(buffers[index]); buffers[index]=NULL; }
    }
    if(blankPlane) {
        FreeMem(blankPlane,(LONG)base->bitmap->BytesPerRow*HUD_H);
        blankPlane=NULL;
    }
}

void hudSetState(UBYTE health,UBYTE lives,UBYTE diamonds)
{
    UBYTE next=(UBYTE)(current^1);
    if(health>PLAYER_MAX_HEALTH) health=PLAYER_MAX_HEALTH;
    if(lives<1||lives>GAME_START_LIVES) lives=GAME_START_LIVES;
    if(diamonds>=DIAMOND_STATES) diamonds=DIAMOND_STATES-1;
    if(bufferHealth[current]==health&&bufferLives[current]==lives&&
       bufferDiamonds[current]==diamonds) return;
    if(bufferHealth[next]!=health) {
        copyPatchBlitter(healthAtlas,(UWORD)(health*HEALTH_H),buffers[next],
                         HEALTH_X,HEALTH_Y,HEALTH_W,HEALTH_H);
        bufferHealth[next]=health;
    }
    if(bufferLives[next]!=lives) {
        copyPatchBlitter(livesAtlas,(UWORD)((lives-1)*LIVES_H),buffers[next],
                         LIVES_X,LIVES_Y,LIVES_W,LIVES_H);
        bufferLives[next]=lives;
    }
    if(bufferDiamonds[next]!=diamonds) {
        copyPatchBlitter(diamondsAtlas,(UWORD)(diamonds*DIAMONDS_H),
                         buffers[next],DIAMONDS_X,DIAMONDS_Y,
                         DIAMONDS_W,DIAMONDS_H);
        bufferDiamonds[next]=diamonds;
    }
    platformWaitBlit(); current=next;
}

const struct BitMap *hudDisplayBitmap(void)
{
    return buffers[current];
}

const UBYTE *hudBlankPlane(void)
{
    return blankPlane;
}
