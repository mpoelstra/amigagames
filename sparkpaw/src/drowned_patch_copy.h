#include "drowned_ring_layout.h"
/* Drowned-only canonical-to-inactive-ring transfer. Caller retires Blitter
   work first. Keep all three physical copies; no fetch/culling shortcut. */
static void drownedCopyPatchRect(struct PrototypeTarget *target,
                                WORD worldX,WORD y,WORD width,WORD height)
{
    WORD left=(WORD)(worldX&~15);
    WORD right=(WORD)((worldX+width+15)&~15);
    WORD windowRight=(WORD)(target->origin+PROTOTYPE_RING_W);
    WORD sourceStep=(WORD)(frontClean->bitmap->BytesPerRow>>1);
    WORD displayStep=(WORD)(target->display->BytesPerRow>>1);
    LONG sourceRow,displayRow;
    if(y<0) { height=(WORD)(height+y); y=0; }
    if(y+height>WORLD_H) height=(WORD)(WORLD_H-y);
    if(left<target->origin) left=target->origin;
    if(right>windowRight) right=windowRight;
    if(right<=left||height<=0) return;
    sourceRow=(LONG)y*frontClean->bitmap->BytesPerRow;
    displayRow=(LONG)y*target->display->BytesPerRow;
    while(left<right) {
        WORD slot=(WORD)DROWNED_RING_SLOT(left);
        WORD chunk=(WORD)(PROTOTYPE_RING_W-slot);
        WORD words; UBYTE plane;
#ifdef SPARKPAW_DROWNED_PATCH_BLIT
        UWORD blitSize;
#endif
        if(chunk>right-left) chunk=(WORD)(right-left);
        words=(WORD)(chunk>>4);
#ifdef SPARKPAW_DROWNED_PATCH_BLIT
        blitSize=(UWORD)((height<<6)|words);
#ifndef SPARKPAW_DROWNED_PATCH_SETUP_REFERENCE
        platformWaitBlit();
        hw->bltcon0=0x09f0; hw->bltcon1=0;
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=frontClean->bitmap->BytesPerRow-words*2;
        hw->bltdmod=target->display->BytesPerRow-words*2;
#endif
#endif
        for(plane=0;plane<FRONT_PLANES;plane++) {
            const UWORD *source=(const UWORD *)(frontClean->bitmap->Planes[plane]+
                sourceRow+(left>>3));
            UWORD *display=(UWORD *)(target->display->Planes[plane]+
                displayRow+(slot>>3));
#ifdef SPARKPAW_DROWNED_PATCH_BLIT
            UBYTE copy;
            for(copy=0;copy<DROWNED_RING_COPIES;copy++) {
                platformWaitBlit();
#ifdef SPARKPAW_DROWNED_PATCH_SETUP_REFERENCE
                hw->bltcon0=0x09f0; hw->bltcon1=0;
                hw->bltafwm=0xffff; hw->bltalwm=0xffff;
                hw->bltamod=frontClean->bitmap->BytesPerRow-words*2;
                hw->bltdmod=target->display->BytesPerRow-words*2;
#endif
                hw->bltapt=(UBYTE *)source;
                hw->bltdpt=(UBYTE *)(display+copy*(PROTOTYPE_RING_W/16));
                hw->bltsize=blitSize;
            }
#else
            WORD row;
            /* Row bases advance by fixed strides. The generic span rebuilds
               both addresses with multiplication on every row. */
            for(row=0;row<height;row++) {
                WORD word;
                for(word=0;word<words;word++) {
                    UWORD value=source[word];
                    display[word]=value;
                    display[word+PROTOTYPE_RING_W/16]=value;
#ifndef SPARKPAW_RING_TWO_COPY
                    display[word+2*(PROTOTYPE_RING_W/16)]=value;
#endif
                }
                source+=sourceStep;
                display+=displayStep;
            }
#endif
        }
        left=(WORD)(left+chunk);
    }
#ifdef SPARKPAW_DROWNED_PATCH_BLIT
    /* Finish before subsequent CPU writes or target publication. */
    platformWaitBlit();
#endif
}
