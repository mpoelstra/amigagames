#ifndef DROWNED_GOVERNOR_H
#define DROWNED_GOVERNOR_H
#include <exec/types.h>
#ifndef DROWNED_FINALE_OFFSET
#define DROWNED_FINALE_OFFSET 0
#endif
UBYTE governorExitFrame(void);
BOOL governorComplete(void);
void governorReset(void);
void governorTick(void);
void governorView(WORD x);
UBYTE governorArtFrame(UBYTE item);
UBYTE governorFrame(UBYTE item);
UBYTE governorHit(WORD x,WORD y);
BOOL governorSweep(WORD start,WORD end,WORD y,WORD *hit);
BOOL governorHazard(WORD left,WORD top,WORD right,WORD bottom);
BOOL governorSolid(WORD x,WORD y);
#endif
