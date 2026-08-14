#ifndef SPARKPAW_PROJECTILES_H
#define SPARKPAW_PROJECTILES_H

#include <exec/types.h>

#define PROJECTILE_W 16
#define PROJECTILE_H 9
#define MAX_PLAYER_PROJECTILES 6
#define MAX_ENEMY_PROJECTILES 2
#define MAX_PROJECTILES (MAX_PLAYER_PROJECTILES+MAX_ENEMY_PROJECTILES)

struct Projectile {
    LONG x,y,vx;
    UBYTE life,impactTimer;
    BOOL active,drawn,lowShot,hostile;
    WORD drawnX,drawnY;
};

typedef BOOL (*ProjectileSolidAt)(WORD x,WORD y);
#define PROJECTILE_ENEMY_MISS 0
#define PROJECTILE_ENEMY_HIT 1
#define PROJECTILE_ENEMY_KILL 2
typedef UBYTE (*ProjectileEnemyHit)(WORD x,WORD y,BOOL lowShot);
typedef void (*ProjectilePlaySound)(void);

void projectilesInit(void);
void projectilesResetPreservingDrawn(void);
void projectilesSpawn(WORD playerX,WORD playerY,BOOL facingLeft,BOOL crouching,
                      BOOL airborne,ProjectilePlaySound playSound);
BOOL projectilesSpawnEnemy(WORD x,WORD y,BOOL facingLeft);
void projectilesUpdate(WORD cameraX,ProjectileSolidAt solidAt,
                       ProjectileEnemyHit hitEnemy,
                       ProjectilePlaySound playEnemyHitSound,
                       ProjectilePlaySound playEnemyDeathSound);
BOOL projectilesContactPlayer(WORD left,WORD top,WORD right,WORD bottom,
                              WORD *projectileCenterX);
struct Projectile *projectileAt(WORD index);

#endif
