#include "drowned_ring_layout.h"
/* Canonical and destination bitplanes are Chip RAM. Caller owns inactive
   target. Each one-word-wide copy remains inside its physical ring copy. */
#if defined(SPARKPAW_DROWNED_JOINED) && !defined(SPARKPAW_DROWNED_FULL_COLUMN_REFERENCE)
#include "drowned_column_tops.h"
#endif
static void drownedBlitColumn(struct PrototypeTarget *target,WORD worldX,WORD oldWorldX)
{
    WORD slot=(WORD)DROWNED_RING_SLOT(worldX);
    UBYTE plane,copy;
    WORD top=0,height;
    LONG sourceRow,displayRow;
#if defined(SPARKPAW_DROWNED_JOINED) && !defined(SPARKPAW_DROWNED_FULL_COLUMN_REFERENCE)
    /* All actor restores precede rolling. Skip only rows guaranteed blank in
       BOTH the evicted canonical column and its replacement. */
    if(oldWorldX>=0&&oldWorldX<WORLD_W&&worldX>=0&&worldX<WORLD_W) {
        WORD oldTop=drownedColumnTops[oldWorldX>>4];
        top=drownedColumnTops[worldX>>4];
        if(oldTop<top) top=oldTop;
    }
#endif
    height=WORLD_H-top;
    if(!height) {platformWaitBlit();return;}
    sourceRow=(LONG)top*frontClean->bitmap->BytesPerRow;
    displayRow=(LONG)top*target->display->BytesPerRow;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        for(copy=0;copy<PROTOTYPE_RING_COPIES;copy++) {
            platformWaitBlit();
            hw->bltcon0=0x09f0; hw->bltcon1=0;
            hw->bltafwm=0xffff; hw->bltalwm=0xffff;
            hw->bltamod=frontClean->bitmap->BytesPerRow-2;
            hw->bltdmod=target->display->BytesPerRow-2;
            hw->bltapt=frontClean->bitmap->Planes[plane]+
                sourceRow+(worldX>>3);
            hw->bltdpt=target->display->Planes[plane]+
                       displayRow+(slot>>3)+
                       (WORD)copy*(PROTOTYPE_RING_W/8);
            hw->bltsize=(height<<6)|1;
        }
    }
    /* CPU dynamic-patch writes may immediately follow this function. */
    platformWaitBlit();
}
