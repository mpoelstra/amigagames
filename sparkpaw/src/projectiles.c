#include "projectiles.h"

#include <string.h>

#define SCREEN_W 320
#define PROJECTILE_SPEED 2300

static struct Projectile projectiles[MAX_PROJECTILES];

void projectilesInit(void)
{
    memset(projectiles,0,sizeof(projectiles));
}

void projectilesResetPreservingDrawn(void)
{
    BOOL wasDrawn[MAX_PROJECTILES];
    WORD oldX[MAX_PROJECTILES],oldY[MAX_PROJECTILES],index;
    for(index=0;index<MAX_PROJECTILES;index++) {
        wasDrawn[index]=projectiles[index].drawn;
        oldX[index]=projectiles[index].drawnX;
        oldY[index]=projectiles[index].drawnY;
    }
    projectilesInit();
    for(index=0;index<MAX_PROJECTILES;index++) {
        projectiles[index].drawn=wasDrawn[index];
        projectiles[index].drawnX=oldX[index];
        projectiles[index].drawnY=oldY[index];
    }
}

void projectilesSpawn(WORD playerX,WORD playerY,BOOL facingLeft,BOOL crouching,
                      ProjectilePlaySound playSound)
{
    WORD index;
    for(index=0;index<MAX_PROJECTILES;index++) {
        struct Projectile *projectile=&projectiles[index];
        if(projectile->active||projectile->drawn) continue;
        /* Frame 40's cyan muzzle is at cell (45,28); crouch-fire frame 48's
           is at (45,41). The actor cell begins eight pixels left/above the
           physics box, and projectile Y is its top rather than its centre. */
        projectile->x=(LONG)(facingLeft?playerX-22:playerX+37)<<8;
        projectile->y=(LONG)(playerY+(crouching?29:15))<<8;
        projectile->vx=facingLeft?-PROJECTILE_SPEED:PROJECTILE_SPEED;
        projectile->life=80; projectile->impactTimer=0;
        projectile->lowShot=crouching; projectile->active=TRUE;
        playSound(); return;
    }
}

void projectilesUpdate(WORD cameraX,ProjectileSolidAt solidAt,
                       ProjectileEnemyHit hitEnemy,
                       ProjectilePlaySound playEnemyHitSound)
{
    WORD index;
    for(index=0;index<MAX_PROJECTILES;index++) {
        struct Projectile *projectile=&projectiles[index]; WORD x,y,screenHit;
        BOOL enemyWasHit;
        if(!projectile->active) continue;
        if(projectile->impactTimer) {
            if(!--projectile->impactTimer) projectile->active=FALSE;
            continue;
        }
        projectile->x+=projectile->vx;
        x=(WORD)(projectile->x>>8)+(projectile->vx>0?PROJECTILE_W-1:0);
        y=(WORD)(projectile->y>>8)+(PROJECTILE_H>>1);
        enemyWasHit=hitEnemy(x,y,projectile->lowShot);
        if(enemyWasHit) playEnemyHitSound();
        if(enemyWasHit||solidAt(x,y)||!--projectile->life) {
            screenHit=x-cameraX;
            /* Never show a clipped impact on invisible off-screen geometry. */
            if(screenHit<2||screenHit>SCREEN_W-3) projectile->active=FALSE;
            else {
                projectile->x=(LONG)(x-(PROJECTILE_W>>1))<<8;
                projectile->y=(LONG)(y-(PROJECTILE_H>>1))<<8;
                projectile->vx=0; projectile->impactTimer=5;
            }
        }
    }
}

struct Projectile *projectileAt(WORD index)
{
    return &projectiles[index];
}
