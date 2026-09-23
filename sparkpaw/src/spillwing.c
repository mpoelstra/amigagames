#include "spillwing.h"
#ifdef SPARKPAW_DROWNED_FERRY
#include "collision.h"
#include "world_config.h"
#if !defined(SPARKPAW_SPILLWING_CLEARANCE_REFERENCE) && (!defined(SPARKPAW_DROWNED_GOVERNOR) || defined(SPARKPAW_DROWNED_FULL))
#include "spillwing_clearance.h"
#endif
#endif
static const UBYTE arc[32]={0,4,8,12,16,20,24,28,32,35,38,40,42,43,44,44,
                                44,44,43,42,40,38,35,32,28,24,20,16,12,8,4,0};
/* Isolated audition aliases the small cache; JOINED owns appended type2.
   Neither mode grows the four-slot pool or Enemy structure. Flight owns
   traversalState/timer and shootCooldown for its re-arm delay. */
#define HOVER_Y 120
#define HOVER 0
#define WARNING 1
#define SWOOP 2
static UBYTE highFlight(const struct Enemy *e)
{
#ifdef SPARKPAW_SPILLWING_PAIR
    return e->spawnIndex&1;
#else
    return 0;
#endif
}
static WORD patrolSpeed(const struct Enemy *e)
{ return highFlight(e)?384:256; }
static WORD hoverY(const struct Enemy *e)
{ return highFlight(e)?96:HOVER_Y; }
static UBYTE nextPause(struct Enemy *e)
{
#ifdef SPARKPAW_SPILLWING_PAIR
    /* Authored irregular sequence, repeatable per encounter, no random hot loop. */
    static const UBYTE pauses[8]={55,93,68,114,76,49,101,63};
    e->turnTimer=(UBYTE)((e->turnTimer+3)&7);
    return pauses[e->turnTimer];
#else
    return 70;
#endif
}
#ifdef SPARKPAW_DROWNED_FERRY
static BOOL clearSwoop(const struct Enemy *e,BOOL left)
{
#if !defined(SPARKPAW_SPILLWING_CLEARANCE_REFERENCE) && !defined(SPARKPAW_DROWNED_GOVERNOR)
    WORD x=(WORD)(e->x>>8);
    UBYTE bit=(UBYTE)(highFlight(e)*2+(left?0:1));
    return x>=0&&x<WORLD_W&&(spillwingClearance[x]&(1U<<bit));
#else
    WORD x=(WORD)(e->x>>8),y,step=highFlight(e)?4:3;
    UBYTE phase;
#if defined(SPARKPAW_DROWNED_FULL) && !defined(SPARKPAW_SPILLWING_CLEARANCE_REFERENCE)
    if(x>=0&&x<DROWNED_FINALE_OFFSET-152) {
        UBYTE bit=(UBYTE)(highFlight(e)*2+(left?0:1));
        return (spillwingClearance[x]&(1U<<bit))!=0;
    }
#endif
    if(left)step=-step;
    /* Once when arming: reject an arc through solid art. Test three rows
       so a 16px tile fully inside the 24px cell cannot be skipped. */
    for(phase=0;phase<32;phase++) {
        x+=step;y=hoverY(e)+arc[phase]+(highFlight(e)?(arc[phase]>>1):0);
        if(collisionSolidHorizontal(x,x+23,y)||
           collisionSolidHorizontal(x,x+23,y+12)||
           collisionSolidHorizontal(x,x+23,y+23))return FALSE;
    }
    return TRUE;
#endif
}
#endif
void spillwingInit(struct Enemy *e)
{
    e->health=1; e->y=hoverY(e); e->vx=e->facingLeft?-patrolSpeed(e):patrolSpeed(e);
    e->shootCooldown=highFlight(e)?103:60; e->traversalState=HOVER;
    e->turnTimer=highFlight(e)?5:0;
}
WORD spillwingLeft(const struct Enemy *e)
{ return (WORD)(e->x>>8)+(e->facingLeft?0:6); }
WORD spillwingRight(const struct Enemy *e)
{ return (WORD)(e->x>>8)+(e->facingLeft?17:23); }
BOOL spillwingHit(const struct Enemy *e,WORD x,WORD y)
{ return x>=spillwingLeft(e)&&x<=spillwingRight(e)&&y>=e->y+9&&y<=e->y+20; }
void spillwingUpdate(struct Enemy *e,WORD cameraX,WORD playerCenterX)
{
    /* Fixed 32-tick arc: no homing, division, trig or transforms on the 020. */

    WORD x=(WORD)(e->x>>8),distance=playerCenterX-(x+12);
    UBYTE phase;
    if(e->dying) {
        phase=(UBYTE)(24-e->deathTimer);
        e->animFrame=phase<4?(UBYTE)(10+(phase>>1)):(UBYTE)(12+(phase-4)/5);
        if(phase>=4&&e->y<176) e->y++;
        if(!--e->deathTimer) e->active=FALSE;
        return;
    }
    e->walkTick++;
    if(e->traversalState==WARNING) {
        e->animFrame=(UBYTE)(4+((e->traversalTimer>>1)&1));
        if(!--e->traversalTimer) {e->traversalState=SWOOP;e->traversalTimer=0;}
        return;
    }
    if(e->traversalState==SWOOP) {
        phase=e->traversalTimer;
        e->x+=e->attackVX; e->y=hoverY(e)+arc[phase]+(highFlight(e)?(arc[phase]>>1):0);
        e->animFrame=(UBYTE)((phase<16?6:8)+((phase>>1)&1));
        if(++e->traversalTimer==32) {
            e->traversalState=HOVER;e->shootCooldown=nextPause(e);
            e->vx=e->facingLeft?-patrolSpeed(e):patrolSpeed(e);
        }
    } else {
        if(e->shootCooldown) e->shootCooldown--;
        e->y=hoverY(e)+(WORD)((e->walkTick>>4)&1);
        e->animFrame=(UBYTE)((e->walkTick>>2)&3);
#ifdef SPARKPAW_DROWNED_FERRY
        {
            WORD next=(WORD)((e->x+e->vx)>>8);
#if !defined(SPARKPAW_SPILLWING_CLEARANCE_REFERENCE) && !defined(SPARKPAW_DROWNED_GOVERNOR)
            UBYTE bit=(UBYTE)(4+highFlight(e)*2+((e->walkTick>>4)&1));
            if(next<0||next>=WORLD_W||!(spillwingClearance[next]&(1U<<bit)))e->vx=-e->vx;
#else
#if defined(SPARKPAW_DROWNED_FULL) && !defined(SPARKPAW_SPILLWING_CLEARANCE_REFERENCE)
            if(next>=0&&next<DROWNED_FINALE_OFFSET-24) {
                UBYTE bit=(UBYTE)(4+highFlight(e)*2+((e->walkTick>>4)&1));
                if(!(spillwingClearance[next]&(1U<<bit)))e->vx=-e->vx;
                else e->x+=e->vx;
            } else
#endif
            if(collisionSolidHorizontal(next,next+23,e->y)||
               collisionSolidHorizontal(next,next+23,e->y+12)||
               collisionSolidHorizontal(next,next+23,e->y+23))e->vx=-e->vx;
#endif
            else e->x+=e->vx;
        }
#else
        e->x+=e->vx;
#endif
        e->facingLeft=e->vx<0;
        /* Readable approach: full cell visible, then a committed direction.
           Require room for the entire 96px swoop before arming. */
        if(!e->shootCooldown&&distance>=-112&&distance<=112&&
           x>=cameraX+8&&x+24<=cameraX+312&&
           (distance<0?x-(highFlight(e)?128:96)>=e->patrolLeft:x+(highFlight(e)?152:120)<=e->patrolRight)) {
#ifdef SPARKPAW_DROWNED_FERRY
            if(!clearSwoop(e,distance<0)){e->shootCooldown=8;return;}
#endif
            e->facingLeft=distance<0;e->attackVX=highFlight(e)?1024:768;
            if(e->facingLeft) e->attackVX=-e->attackVX;
            e->traversalState=WARNING;e->traversalTimer=12;e->animFrame=4;
        }
    }
    if(e->x<(LONG)e->patrolLeft*256) {e->x=(LONG)e->patrolLeft*256;e->vx=patrolSpeed(e);}
    if(e->x>(LONG)(e->patrolRight-24)*256) {e->x=(LONG)(e->patrolRight-24)*256;e->vx=-patrolSpeed(e);}
}
