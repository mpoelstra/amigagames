#include "collectibles.h"

#include <string.h>

static struct Collectible collectibles[MAX_COLLECTIBLES];

void collectiblesInit(void)
{
    /* Original arcs and short trails follow the authored collision surfaces.
       Keep screen-local concurrency bounded for later larger-enemy headroom. */
    static const WORD spawnX[MAX_COLLECTIBLES]={
        144,160,192,208, 256,304, 336,368,400, 512,
        560,592, 704,736, 864,896,944, 1008,1104,1168,
        1296,1328,1360,1408, 1488,1520,1552, 1664,1704,1744,1840,1920,
        2048,2080,2208,2240,2320,2368, 2512,2560,2624,2688,
        2880,2912,2960,3008,3040,3056
    };
    static const WORD spawnY[MAX_COLLECTIBLES]={
        132,122,122,132, 148,174, 100,90,100, 148,
        138,148, 116,116, 84,74,84, 148,132,132,
        148,132,122,132, 100,90,100, 148,132,148,116,84,
        116,100,148,132,86,86, 118,116,110,132,
        100,84,116,100,92,108
    };
    UBYTE index;
    memset(collectibles,0,sizeof(collectibles));
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        collectibles[index].x=spawnX[index];
        collectibles[index].y=spawnY[index];
        collectibles[index].active=TRUE;
    }
}

void collectiblesResetPreservingProgress(void)
{
    BOOL wasActive[MAX_COLLECTIBLES],wasDrawn[MAX_COLLECTIBLES];
    WORD oldX[MAX_COLLECTIBLES],oldY[MAX_COLLECTIBLES],index;
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        wasActive[index]=collectibles[index].active;
        wasDrawn[index]=collectibles[index].drawn;
        oldX[index]=collectibles[index].drawnX;
        oldY[index]=collectibles[index].drawnY;
    }
    collectiblesInit();
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        /* A life-loss restart belongs to the same level attempt. Keep every
           collected diamond inactive so its HUD count and score cannot be
           earned repeatedly by deliberately entering a hazard. */
        collectibles[index].active=wasActive[index];
        collectibles[index].drawn=wasDrawn[index];
        collectibles[index].drawnX=oldX[index];
        collectibles[index].drawnY=oldY[index];
    }
}

UBYTE collectiblesCollect(WORD left,WORD top,WORD right,WORD bottom)
{
    UBYTE count=0,index;
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        struct Collectible *item=&collectibles[index];
        if(item->active&&right>=item->x&&left<item->x+COLLECTIBLE_W&&
           bottom>=item->y&&top<item->y+COLLECTIBLE_H) {
            item->active=FALSE; count++;
        }
    }
    return count;
}

struct Collectible *collectibleAt(WORD index)
{
    return &collectibles[index];
}
