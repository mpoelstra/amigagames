#include "enemies.h"

#include <string.h>

#define ENEMY_SPEED 96

static struct Enemy enemies[MAX_ENEMIES];

void enemiesInit(void)
{
    static const WORD spawnX[MAX_ENEMIES]={300,535,790,1080};
    static const WORD patrolLeft[MAX_ENEMIES]={286,500,752,1035};
    static const WORD patrolRight[MAX_ENEMIES]={392,640,900,1195};
    UBYTE index;
    memset(enemies,0,sizeof(enemies));
    for(index=0;index<MAX_ENEMIES;index++) {
        struct Enemy *enemy=&enemies[index];
        enemy->x=(LONG)spawnX[index]<<8; enemy->y=184;
        enemy->vx=(index&1)?-ENEMY_SPEED:ENEMY_SPEED;
        enemy->patrolLeft=patrolLeft[index];
        enemy->patrolRight=patrolRight[index];
        enemy->health=2; enemy->active=TRUE;
        enemy->facingLeft=enemy->vx<0;
    }
}

void enemiesUpdate(LONG frameCounter,EnemySolidAt solidAt)
{
    WORD index;
    for(index=0;index<MAX_ENEMIES;index++) {
        struct Enemy *enemy=&enemies[index]; WORD nextX,front;
        if(!enemy->active) continue;
        if(enemy->dying) {
            enemy->animFrame=(UBYTE)(5+(20-enemy->deathTimer)/5);
            if(!--enemy->deathTimer) enemy->active=FALSE;
            continue;
        }
        if(enemy->hitTimer) {
            enemy->hitTimer--; enemy->animFrame=4; continue;
        }
        nextX=(WORD)((enemy->x+enemy->vx)>>8);
        front=nextX+(enemy->vx<0?1:ENEMY_W-2);
        if(nextX<enemy->patrolLeft||nextX>enemy->patrolRight-ENEMY_W||
           solidAt(front,enemy->y+ENEMY_H-8)||
           !solidAt(front,enemy->y+ENEMY_H)) {
            enemy->vx=-enemy->vx;
        } else enemy->x+=enemy->vx;
        enemy->facingLeft=enemy->vx<0;
        enemy->animFrame=(UBYTE)((frameCounter>>2)&3);
    }
}

BOOL enemiesHitProjectile(WORD x,WORD y,BOOL lowShot)
{
    WORD index;
    for(index=0;index<MAX_ENEMIES;index++) {
        struct Enemy *enemy=&enemies[index];
        if(lowShot&&enemy->active&&!enemy->dying&&
           x>=(WORD)(enemy->x>>8)+2&&x<=(WORD)(enemy->x>>8)+ENEMY_W-3&&
           y>=enemy->y+7&&y<=enemy->y+ENEMY_H-1) {
            if(!--enemy->health) {
                enemy->dying=TRUE; enemy->deathTimer=20;
                enemy->vx=0; enemy->animFrame=5;
            } else {
                enemy->hitTimer=8; enemy->animFrame=4;
            }
            return TRUE;
        }
    }
    return FALSE;
}

struct Enemy *enemyAt(WORD index)
{
    return &enemies[index];
}