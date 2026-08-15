#include "collision.h"
#include "level_data.h"
#include "world_config.h"

#include <dos/dos.h>
#include <proto/dos.h>

#define GAME_H 224
#define TILE_SIZE 16
#define MAP_COLS (WORLD_W/TILE_SIZE)
#define MAP_ROWS 14

static UBYTE collision[MAP_COLS*MAP_ROWS];

BOOL collisionLoad(void)
{
    BPTR file=Open("PROGDIR:assets/runtime/storm-collision.bin",MODE_OLDFILE);
    if(!file) return FALSE;
    if(Read(file,collision,sizeof(collision))!=sizeof(collision)) {
        Close(file); return FALSE;
    }
    Close(file); return TRUE;
}

BOOL collisionSolidAt(WORD x,WORD y)
{
    WORD tileX,tileY;
    if(x<0||x>=WORLD_W||y<0) return TRUE;
    /* Phase 6B.3A moves the continuous floor top to the visible cap while the
       accepted water columns remain open through the bottom death region. */
    if(y>=LEVEL_FLOOR_Y&&!levelHazardColumnAt(x)) return TRUE;
    if(y>=GAME_H) return !levelHazardColumnAt(x);
    tileX=x/TILE_SIZE; tileY=y/TILE_SIZE;
    return collision[tileY*MAP_COLS+tileX]!=0;
}

BOOL collisionSolidHorizontal(WORD left,WORD right,WORD y)
{
    WORD x;
    for(x=left;x<=right;x++) if(collisionSolidAt(x,y)) return TRUE;
    return FALSE;
}

BOOL collisionSolidVertical(WORD x,WORD top,WORD bottom)
{
    WORD y;
    for(y=top;y<=bottom;y++) if(collisionSolidAt(x,y)) return TRUE;
    return FALSE;
}
