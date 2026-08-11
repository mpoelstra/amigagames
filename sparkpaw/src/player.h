#ifndef SPARKPAW_PLAYER_H
#define SPARKPAW_PLAYER_H

#include <exec/types.h>

#define PLAYER_W 32
#define PLAYER_H 40
#define PLAYER_ANIM_FRAMES 58
#define PLAYER_MAX_HEALTH 6

struct PlayerState {
    LONG x,y,vx,vy,turnStartVx;
    BOOL grounded,facingLeft,crouching,wallBlocked,turnTargetLeft,turnFinishing;
    BOOL hurtCrouched;
    UBYTE animFrame,runFrame,landTimer,turnTimer,shootTimer,shootCooldown;
    UBYTE health,invulnTimer,hurtTimer;
    BOOL shotPending;
    UWORD runTick,idleTicks;
};

typedef void (*PlayerPlayShot)(void);

void playerInit(void);
void playerReadInput(BOOL *left,BOOL *right,BOOL *down,BOOL *jump,BOOL *fire);
void playerStartShot(BOOL pressed,PlayerPlayShot playShot);
BOOL playerUpdatePhysics(BOOL left,BOOL right,BOOL down,BOOL jump);
void playerUpdateShot(void);
void playerAnimate(BOOL landed,LONG frameCounter);
void playerContactBounds(WORD *left,WORD *top,WORD *right,WORD *bottom);
BOOL playerTakeEnemyHit(WORD enemyCenterX);
const struct PlayerState *playerState(void);

#endif
