#ifndef SPARKPAW_PLAYER_H
#define SPARKPAW_PLAYER_H

#include <exec/types.h>
#include "control_options.h"

#define PLAYER_W 32
#define PLAYER_H 40
#define PLAYER_ANIM_FRAMES 63
#define PLAYER_STORMRAIL_PILOT_FRAME 62
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
#ifdef SPARKPAW_DROWNED_CAMPAIGN_MODULE
void playerRestoreDrownedHealth(UBYTE health);
#endif
#ifdef SPARKPAW_DROWNED_JOINED
void playerRespawnAt(WORD x,WORD y);
#endif
void playerSetControlMode(enum ControlMode action);
void playerReadInput(BOOL *left,BOOL *right,BOOL *down,BOOL *jump,BOOL *fire);
#ifdef SPARKPAW_STORMRAIL_PROOF
void playerReadFlightInput(BOOL *left,BOOL *right,BOOL *up,BOOL *down,
                           BOOL *fire);
#endif
void playerStartShot(BOOL pressed,PlayerPlayShot playShot);
BOOL playerUpdatePhysics(BOOL left,BOOL right,BOOL down,BOOL jump);
void playerUpdateShot(void);
void playerAnimate(BOOL landed,LONG frameCounter);
void playerContactBounds(WORD *left,WORD *top,WORD *right,WORD *bottom);
#ifdef SPARKPAW_DROWNED_ENEMY_ART
void playerProjectileBounds(WORD *left,WORD *top,WORD *right,WORD *bottom);
#endif
BOOL playerTakeEnemyHit(WORD enemyCenterX);
const struct PlayerState *playerState(void);

#ifdef SPARKPAW_DROWNED_PONTOON
void playerCarryHorizontal(LONG delta);
#endif
#endif
