#include "collectibles.h"

#include <string.h>

static struct Collectible collectibles[MAX_COLLECTIBLES];

void collectiblesInit(void)
{
    /* Original arcs and short trails follow the authored collision surfaces.
       Keep screen-local concurrency bounded for later larger-enemy headroom. */
    static const WORD spawnX[MAX_COLLECTIBLES]={
        144,160,192,208, 256,304, 336,368,400, 512,
        560,592, 704,736, 864,896,944, 1008,1104,1168
    };
    static const WORD spawnY[MAX_COLLECTIBLES]={
        132,122,122,132, 148,178, 100,90,100, 148,
        138,148, 116,116, 84,74,84, 148,132,132
    };
    UBYTE index;
    memset(collectibles,0,sizeof(collectibles));
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        collectibles[index].x=spawnX[index];
        collectibles[index].y=spawnY[index];
        collectibles[index].active=TRUE;
    }
}

void collectiblesResetPreservingDrawn(void)
{
    BOOL wasDrawn[MAX_COLLECTIBLES];
    WORD oldX[MAX_COLLECTIBLES],oldY[MAX_COLLECTIBLES],index;
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        wasDrawn[index]=collectibles[index].drawn;
        oldX[index]=collectibles[index].drawnX;
        oldY[index]=collectibles[index].drawnY;
    }
    collectiblesInit();
    for(index=0;index<MAX_COLLECTIBLES;index++) {
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
