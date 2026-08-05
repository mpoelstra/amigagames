#ifndef SPARKPAW_ENEMIES_H
#define SPARKPAW_ENEMIES_H

#include <exec/types.h>

#define ENEMY_W 32
#define ENEMY_H 24
#define ENEMY_FRAMES 9
#define ENEMY_SOURCE_WORDS 3
#define MAX_ENEMIES 4

struct Enemy {
    LONG x,vx;
    WORD y,drawnX,drawnY,patrolLeft,patrolRight;
    UBYTE animFrame,health,hitTimer,deathTimer;
    BOOL active,drawn,facingLeft,dying;
};

typedef BOOL (*EnemySolidAt)(WORD x,WORD y);

void enemiesInit(void);
void enemiesUpdate(LONG frameCounter,EnemySolidAt solidAt);
BOOL enemiesHitProjectile(WORD x,WORD y,BOOL lowShot);
struct Enemy *enemyAt(WORD index);

#endif