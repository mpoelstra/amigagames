#ifndef SPARKPAW_COLLECTIBLES_H
#define SPARKPAW_COLLECTIBLES_H

#include <exec/types.h>

#define COLLECTIBLE_W 16
#define COLLECTIBLE_H 24
#define COLLECTIBLE_FRAMES 4
#define MAX_COLLECTIBLES 20

struct Collectible {
    WORD x,y,drawnX,drawnY;
    BOOL active,drawn;
};

void collectiblesInit(void);
void collectiblesResetPreservingDrawn(void);
UBYTE collectiblesCollect(WORD left,WORD top,WORD right,WORD bottom);
struct Collectible *collectibleAt(WORD index);

#endif
