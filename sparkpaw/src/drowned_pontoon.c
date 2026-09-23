#include "drowned_pontoon.h"
#include "player.h"
static WORD px=PONTOON_LEFT,deck=189,direction=1;
static UWORD phase;
static BOOL started;
void drownedPontoonReset(void){px=PONTOON_LEFT;deck=189;direction=1;phase=0;started=FALSE;}
WORD drownedPontoonX(void){return px;}
WORD drownedPontoonDeck(void){return deck;}
BOOL drownedPontoonSupport(WORD left,WORD right,WORD foot)
{
 WORD a=left>px+3?left:px+3,b=right<px+92?right:px+92;
 return foot==deck&&b-a+1>=4;
}
void drownedPontoonStep(struct PlayerState *p)
{
 WORD oldX=px,oldDeck=deck;
 BOOL riding=p->grounded&&drownedPontoonSupport((WORD)(p->x>>8)+4,
                        (WORD)(p->x>>8)+27,(WORD)(p->y>>8)+39);
 if(riding)started=TRUE;
 if(started){
  px+=direction*2;
  if(px>=PONTOON_RIGHT){px=PONTOON_RIGHT;direction=-1;}
  if(px<=PONTOON_LEFT){px=PONTOON_LEFT;direction=1;}
 }
 phase=(phase+1)&63;deck=189+((phase>>4)&1);
 if(riding){playerCarryHorizontal((LONG)(px-oldX)*256);p->y+=(LONG)(deck-oldDeck)*256;}
}
