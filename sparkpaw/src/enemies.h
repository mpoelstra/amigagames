#ifndef SPARKPAW_ENEMIES_H
#define SPARKPAW_ENEMIES_H

#include <exec/types.h>
#include "projectiles.h"

#define ENEMY_W 32
#define ENEMY_H 24
#define ENEMY_FRAMES 9
#define ENEMY_SOURCE_WORDS 3
#define STRIDER_W 64
#define STRIDER_H 64
#define STRIDER_FRAMES 28
#define STRIDER_SOURCE_WORDS 5
#define MAX_ENEMIES 4

struct Enemy {
    LONG x,vx,vy,jumpY,resumeVX,attackVX;
    WORD y,drawnX,drawnY,patrolLeft,patrolRight,traversalStartX;
    UWORD walkTick;
    UBYTE animFrame,health,hitTimer,deathTimer,turnTimer,spawnIndex,type,drawnType;
    UBYTE traversalState,traversalTimer,traversalLink,surfaceId,traversalFailed;
    UBYTE shootTimer,shootCooldown;
    BOOL active,drawn,facingLeft,dying,shotPending;
};

typedef BOOL (*EnemySolidAt)(WORD x,WORD y);
typedef BOOL (*EnemySpawnProjectile)(WORD x,WORD y,BOOL facingLeft);

void enemiesInit(ULONG seed);
void enemiesResetPreservingDrawn(ULONG seed);
void enemiesUpdate(WORD cameraX,EnemySolidAt solidAt,WORD playerCenterX,
                   WORD playerCenterY,EnemySpawnProjectile spawnProjectile);
UBYTE enemiesHitProjectile(WORD x,WORD y,BOOL lowShot);
BOOL enemiesContactPlayer(WORD left,WORD top,WORD right,WORD bottom,
                          WORD *enemyCenterX);
struct Enemy *enemyAt(WORD index);

#endif
