#include "collision.h"
#ifndef SPARKPAW_COLLISION_HAZARD_SCAN_REFERENCE
#define SPARKPAW_COLLISION_HAZARD_CACHE
#endif
#ifdef SPARKPAW_COLLISION_HAZARD_CACHE
#include "collision_hazard_cache.h"
#endif
#include "level_data.h"
#include "game.h"
#include "projectile_sweep.h"
#include "world_config.h"

#include <dos/dos.h>
#include <proto/dos.h>
#ifdef SPARKPAW_MULTI_ADF
#include "disk_media.h"
#undef Open
#define Open diskMediaOpen
#endif

#define GAME_H 224
#define TILE_SIZE 16
#define MAP_COLS (WORLD_W/TILE_SIZE)
#define MAP_ROWS 14

static UBYTE collision[MAP_COLS*MAP_ROWS];
#ifdef SPARKPAW_STORMRAIL_PROOF
/* Section ownership changes only while gameplay is unloaded. Cache it beside
 * the collision data so Level-1's hottest query does not cross into game.c. */
static BOOL stormrailCollision;
#endif
#ifdef SPARKPAW_COLLISION_HAZARD_CACHE
static UBYTE hazardColumns[WORLD_W];

static int collisionHazardPredicate(short x)
{
    return levelHazardColumnAt((WORD)x);
}
#endif

BOOL collisionLoad(void)
{
    BPTR file=Open("PROGDIR:assets/runtime/storm-collision.bin",MODE_OLDFILE);
    if(!file) return FALSE;
    if(Read(file,collision,sizeof(collision))!=sizeof(collision)) {
        Close(file); return FALSE;
    }
    Close(file);
#ifdef SPARKPAW_STORMRAIL_PROOF
    stormrailCollision=gameStormrailActive();
#endif
#ifdef SPARKPAW_COLLISION_HAZARD_CACHE
    collisionBuildHazardCache(hazardColumns,WORLD_W,collisionHazardPredicate);
#endif
    return TRUE;
}

BOOL collisionSolidAt(WORD x,WORD y)
{
    WORD tileX,tileY;
#ifdef SPARKPAW_STORMRAIL_PROOF
    /* One raised visible cliff is the entire on-foot route. Beyond its broken
       edge is real empty space, so the Skimmer is the only continuation. */
    if(stormrailCollision) {
        if(x>=0&&x<176) return y>=168;
        if(x>=176&&x<430) return FALSE;
    }
#endif
    if(x<0||x>=WORLD_W||y<0) return TRUE;
    /* Phase 6B.3A moves the continuous floor top to the visible cap while the
       accepted water columns remain open through the bottom death region. */
    if(y>=LEVEL_FLOOR_Y&&!
#ifdef SPARKPAW_COLLISION_HAZARD_CACHE
       collisionHazardCached(hazardColumns,x)
#else
       levelHazardColumnAt(x)
#endif
       ) return TRUE;
    if(y>=GAME_H) return !
#ifdef SPARKPAW_COLLISION_HAZARD_CACHE
        collisionHazardCached(hazardColumns,x);
#else
        levelHazardColumnAt(x);
#endif
    tileX=x/TILE_SIZE; tileY=y/TILE_SIZE;
    return collision[tileY*MAP_COLS+tileX]!=0;
}

BOOL collisionFirstSolidOnSweep(WORD start,WORD end,WORD y,WORD *hitX)
{
    WORD x=start;
    /* Above the floor, collision is constant inside each authored 16px tile.
       Probe the current pixel and then only the first pixel of every crossed
       tile.  The floor/hazard region may change at authored pixel boundaries,
       so retain the exact reference walk there. */
    if(y<LEVEL_FLOOR_Y&&y>=0) {
        for(;;) {
            WORD next;
            if(collisionSolidAt(x,y)) { *hitX=x; return TRUE; }
            if(x==end) return FALSE;
            next=projectileSweepNextTileProbe(x,end,TILE_SIZE);
            x=next;
        }
    }
    for(;;) {
        if(collisionSolidAt(x,y)) { *hitX=x; return TRUE; }
        if(x==end) return FALSE;
        x=(WORD)(x<end?x+1:x-1);
    }
}

BOOL collisionSolidHorizontal(WORD left,WORD right,WORD y)
{
    WORD x;
#ifndef SPARKPAW_COLLISION_PIXEL_SPAN_REFERENCE
    for(x=left;x<=right;x=(WORD)((x&~(TILE_SIZE-1))+TILE_SIZE))
        if(collisionSolidAt(x,y)) return TRUE;
#else
    for(x=left;x<=right;x++) if(collisionSolidAt(x,y)) return TRUE;
#endif
    return FALSE;
}

BOOL collisionSolidVertical(WORD x,WORD top,WORD bottom)
{
    WORD y;
#ifndef SPARKPAW_COLLISION_PIXEL_SPAN_REFERENCE
    for(y=top;y<=bottom;) {
        WORD next;
        if(collisionSolidAt(x,y)) return TRUE;
        next=(WORD)((y&~(TILE_SIZE-1))+TILE_SIZE);
        if(y<LEVEL_FLOOR_Y&&next>LEVEL_FLOOR_Y) next=LEVEL_FLOOR_Y;
        y=next;
    }
#else
    for(y=top;y<=bottom;y++) if(collisionSolidAt(x,y)) return TRUE;
#endif
    return FALSE;
}
