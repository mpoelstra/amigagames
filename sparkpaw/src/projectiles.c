#include "projectiles.h"
#include "projectile_sweep.h"

#include <string.h>

#define SCREEN_W 320
#define PLAYER_COLLISION_W 32
#define PROJECTILE_SPEED 2300
#define ENEMY_PROJECTILE_SPEED 1150

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
                      BOOL airborne,ProjectilePlaySound playSound)
{
    WORD index;
    for(index=0;index<MAX_PLAYER_PROJECTILES;index++) {
        struct Projectile *projectile=&projectiles[index];
        if(projectile->active||projectile->drawn) continue;
        /* Frame 40's cyan muzzle is at cell (45,28); crouch-fire frame 48's
           is at (45,41). The actor cell begins eight pixels left/above the
           physics box, and projectile Y is its top rather than its centre. */
        projectile->x=(LONG)(facingLeft?playerX-22:playerX+37)<<8;
        projectile->y=(LONG)(playerY+(crouching?29:15))<<8;
        projectile->vx=facingLeft?-PROJECTILE_SPEED:PROJECTILE_SPEED;
        /* Collision starts at Sparkpaw's physical front edge, not at the
           authored muzzle/Bob origin, which may visually enter a nearby wall. */
        projectile->collisionX=(WORD)(facingLeft?playerX-1:
                                      playerX+PLAYER_COLLISION_W);
        projectile->life=80; projectile->impactTimer=0;
        /* Floor beetles retain the crouch-shot contract, but an airborne shot
           may hit an elevated beetle when the existing geometry overlaps. */
        projectile->lowShot=crouching||airborne;
        projectile->hostile=FALSE;
        projectile->active=TRUE;
        playSound(); return;
    }
}

BOOL projectilesSpawnEnemy(WORD x,WORD y,BOOL facingLeft)
{
    WORD index;
    for(index=MAX_PLAYER_PROJECTILES;index<MAX_PROJECTILES;index++) {
        struct Projectile *projectile=&projectiles[index];
        if(projectile->active||projectile->drawn) continue;
        projectile->x=(LONG)x<<8; projectile->y=(LONG)y<<8;
        projectile->vx=facingLeft?-ENEMY_PROJECTILE_SPEED:
                                  ENEMY_PROJECTILE_SPEED;
        projectile->collisionX=(WORD)(facingLeft?x:x+PROJECTILE_W-1);
        projectile->life=100; projectile->impactTimer=0;
        projectile->lowShot=FALSE; projectile->hostile=TRUE;
        projectile->active=TRUE;
        return TRUE;
    }
    return FALSE;
}

void projectilesUpdate(WORD cameraX,ProjectileSolidAt solidAt,
                       ProjectileEnemyHit hitEnemy,
                       ProjectilePlaySound playEnemyHitSound,
                       ProjectilePlaySound playEnemyDeathSound)
{
    WORD index;
    for(index=0;index<MAX_PROJECTILES;index++) {
        struct Projectile *projectile=&projectiles[index]; WORD x,y,screenHit;
        BOOL contacted=FALSE;
        UBYTE enemyHitResult;
        if(!projectile->active) continue;
        if(projectile->impactTimer) {
            if(!--projectile->impactTimer) projectile->active=FALSE;
            continue;
        }
        projectile->x+=projectile->vx;
        x=(WORD)(projectile->x>>8)+(projectile->vx>0?PROJECTILE_W-1:0);
        y=(WORD)(projectile->y>>8)+(PROJECTILE_H>>1);
        enemyHitResult=PROJECTILE_ENEMY_MISS;
        /* Sweep every crossed pixel. Solid geometry wins at each position,
           including the physical muzzle edge, before enemy dispatch. */
        for(;;) {
            WORD sampleX=projectile->collisionX;
            if(solidAt(sampleX,y)) { contacted=TRUE; x=sampleX; break; }
            if(!projectile->hostile) enemyHitResult=hitEnemy(sampleX,y);
            if(enemyHitResult) { contacted=TRUE; x=sampleX; break; }
            if(sampleX==x) break;
            projectile->collisionX=projectileSweepNext(sampleX,x);
        }
        projectile->collisionX=x;
        if(enemyHitResult==PROJECTILE_ENEMY_KILL) playEnemyDeathSound();
        else if(enemyHitResult==PROJECTILE_ENEMY_HIT) playEnemyHitSound();
        if(contacted||!--projectile->life) {
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

BOOL projectilesContactPlayer(WORD left,WORD top,WORD right,WORD bottom,
                              WORD *projectileCenterX)
{
    WORD index;
    for(index=MAX_PLAYER_PROJECTILES;index<MAX_PROJECTILES;index++) {
        struct Projectile *projectile=&projectiles[index];
        WORD x,y;
        if(!projectile->active||projectile->impactTimer||!projectile->hostile)
            continue;
        x=(WORD)(projectile->x>>8); y=(WORD)(projectile->y>>8);
        if(right>=x+2&&left<=x+PROJECTILE_W-3&&
           bottom>=y+1&&top<=y+PROJECTILE_H-2) {
            projectile->vx=0; projectile->impactTimer=5;
            *projectileCenterX=(WORD)(x+(PROJECTILE_W>>1));
            return TRUE;
        }
    }
    return FALSE;
}

struct Projectile *projectileAt(WORD index)
{
    return &projectiles[index];
}
