#ifndef SPARKPAW_PROJECTILES_H
#define SPARKPAW_PROJECTILES_H

#include <exec/types.h>

#define PROJECTILE_W 16
#define PROJECTILE_H 9
#define MAX_PROJECTILES 6

struct Projectile {
    LONG x,y,vx;
    UBYTE life,impactTimer;
    BOOL active,drawn,lowShot;
    WORD drawnX,drawnY;
};

typedef BOOL (*ProjectileSolidAt)(WORD x,WORD y);
typedef BOOL (*ProjectileEnemyHit)(WORD x,WORD y,BOOL lowShot);
typedef void (*ProjectilePlaySound)(void);

void projectilesInit(void);
void projectilesResetPreservingDrawn(void);
void projectilesSpawn(WORD playerX,WORD playerY,BOOL facingLeft,BOOL crouching,
                      ProjectilePlaySound playSound);
void projectilesUpdate(WORD cameraX,ProjectileSolidAt solidAt,
                       ProjectileEnemyHit hitEnemy);
struct Projectile *projectileAt(WORD index);

#endif
