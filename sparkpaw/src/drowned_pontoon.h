#ifndef DROWNED_PONTOON_H
#define DROWNED_PONTOON_H
#include <exec/types.h>
struct PlayerState;
#define PONTOON_W 96
#ifdef SPARKPAW_DROWNED_JOINED
#define PONTOON_LEFT 2400
#define PONTOON_RIGHT (3200-PONTOON_W)
#else
#define PONTOON_LEFT 160
#define PONTOON_RIGHT (960-PONTOON_W)
#endif
void drownedPontoonReset(void);
void drownedPontoonStep(struct PlayerState *p);
WORD drownedPontoonX(void);
WORD drownedPontoonDeck(void);
BOOL drownedPontoonSupport(WORD left,WORD right,WORD foot);
#endif
