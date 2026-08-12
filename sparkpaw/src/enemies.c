#include "enemies.h"

#include "level_data.h"

#include <string.h>

#define INVALID_SPAWN 255
#define SCREEN_W 320
#define ACTIVATE_MARGIN 96
#define UNLOAD_MARGIN 32
#define WALK_PHASE_DISTANCE 384
#define RESPAWN_MIN_FRAMES 250
#define RESPAWN_FRAME_RANGE 251
struct EnemySpawnState {
    struct Enemy enemy;
    UWORD respawnTimer;
    BOOL selected,respawnPending,exhausted;
    UBYTE loadedSlot;
};

static struct Enemy enemies[MAX_ENEMIES];
static struct EnemySpawnState spawnStates[MAX_LEVEL_ENEMY_SPAWNS];
static UBYTE spawnCount;
static ULONG randomState;

static ULONG nextRandom(void)
{
    randomState^=randomState<<13;
    randomState^=randomState>>17;
    randomState^=randomState<<5;
    return randomState;
}

static UWORD randomBelow(UWORD limit)
{
    return limit?(UWORD)(nextRandom()%limit):0;
}

static BOOL spawnRuntimeReady(UBYTE spawnIndex,UBYTE type)
{
    if(type==ENEMY_TYPE_CLOCKWORK_BEETLE) return TRUE;
    /* Phase 5C.2 renders only the two required static proof placements. */
    return type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER&&spawnIndex<8;
}

static WORD enemyWidthForType(UBYTE type)
{
    return type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER?STRIDER_W:ENEMY_W;
}

static WORD enemyHeightForType(UBYTE type)
{
    return type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER?STRIDER_H:ENEMY_H;
}

static void initializeSpawnState(struct EnemySpawnState *state,
                                 const struct EnemySpawnCandidate *spawn,
                                 UBYTE spawnIndex,BOOL preserveSelection)
{
    static const WORD speeds[3]={48,96,192};
    struct Enemy *enemy=&state->enemy;
    WORD speed=speeds[randomBelow(3)];
    BOOL selected=state->selected;
    memset(state,0,sizeof(*state));
    enemy->x=(LONG)(spawn->minX+randomBelow(
        (UWORD)(spawn->maxX-spawn->minX+1)))<<8;
    enemy->y=(WORD)(spawn->surface.groundY-enemyHeightForType(spawn->type));
    enemy->vx=spawn->initialDirection<0?-speed:speed;
    enemy->patrolLeft=spawn->surface.left;
    enemy->patrolRight=spawn->surface.right;
    enemy->health=spawn->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER?3:2;
    enemy->active=TRUE;
    enemy->facingLeft=enemy->vx<0;
    enemy->spawnIndex=spawnIndex; enemy->type=spawn->type;
    state->selected=preserveSelection?selected:TRUE;
    state->loadedSlot=INVALID_SPAWN;
}

static void generateLevelEnemies(ULONG seed)
{
    const struct EnemySpawnCandidate *spawns;
    UWORD candidateCount,index;
    UBYTE type;
    randomState=seed?seed:0x53504157UL;
    memset(spawnStates,0,sizeof(spawnStates));
    spawns=levelEnemySpawnCandidates(&candidateCount);
    if(candidateCount>MAX_LEVEL_ENEMY_SPAWNS)
        candidateCount=MAX_LEVEL_ENEMY_SPAWNS;
    spawnCount=(UBYTE)candidateCount;
    for(index=0;index<candidateCount;index++)
        spawnStates[index].loadedSlot=INVALID_SPAWN;
    for(type=0;type<ENEMY_TYPE_COUNT;type++) {
        UWORD optionalCount=0,optionalWanted,optionalPick=0,optionalIndex=0;
        for(index=0;index<candidateCount;index++)
            if(spawns[index].type==type&&!spawns[index].required)
                optionalCount++;
        optionalWanted=randomBelow((UWORD)(optionalCount+1));
        if(optionalWanted&&optionalWanted<optionalCount)
            optionalPick=randomBelow(optionalCount);
        for(index=0;index<candidateCount;index++) {
            BOOL selected;
            if(spawns[index].type!=type) continue;
            selected=spawns[index].required;
            if(!spawns[index].required) {
                if(optionalWanted==optionalCount||optionalIndex==optionalPick)
                    selected=optionalWanted!=0;
                optionalIndex++;
            }
            if(selected)
                initializeSpawnState(&spawnStates[index],&spawns[index],
                                     (UBYTE)index,FALSE);
        }
    }
}

static BOOL spawnNearCamera(const struct Enemy *enemy,WORD cameraX)
{
    return enemy->patrolRight>=cameraX-ACTIVATE_MARGIN&&
           enemy->patrolLeft<=cameraX+SCREEN_W+ACTIVATE_MARGIN;
}

static LONG spawnPriority(const struct Enemy *enemy,WORD cameraX)
{
    WORD center=(WORD)((enemy->patrolLeft+enemy->patrolRight)>>1);
    WORD screenRight=(WORD)(cameraX+SCREEN_W);
    LONG distance;
    if(enemy->patrolRight>=cameraX&&enemy->patrolLeft<=screenRight)
        return (LONG)(center-cameraX);
    if(enemy->patrolLeft>screenRight) {
        distance=(LONG)enemy->patrolLeft-screenRight;
        return 0x10000L+distance;
    }
    distance=(LONG)cameraX-enemy->patrolRight;
    return 0x20000L+distance;
}

static void activateVisibleSpawns(WORD cameraX,BOOL allowRestoreSlots)
{
    UBYTE slot;
    for(slot=0;slot<MAX_ENEMIES;slot++) {
        UBYTE spawnIndex,bestSpawn=INVALID_SPAWN;
        LONG bestPriority=0x7fffffffL;
        BOOL wasDrawn=enemies[slot].drawn;
        WORD oldX=enemies[slot].drawnX,oldY=enemies[slot].drawnY;
        if(enemies[slot].active||(!allowRestoreSlots&&wasDrawn)||
           enemies[slot].spawnIndex!=INVALID_SPAWN) continue;
        for(spawnIndex=0;spawnIndex<spawnCount;spawnIndex++) {
            struct EnemySpawnState *state=&spawnStates[spawnIndex];
            LONG priority;
            if(!state->selected||state->respawnPending||state->exhausted||
               state->loadedSlot!=INVALID_SPAWN||
               !spawnRuntimeReady(spawnIndex,state->enemy.type)||
               !spawnNearCamera(&state->enemy,cameraX)) continue;
            priority=spawnPriority(&state->enemy,cameraX);
            if(priority<bestPriority) {
                bestPriority=priority; bestSpawn=spawnIndex;
            }
        }
        if(bestSpawn==INVALID_SPAWN) continue;
        enemies[slot]=spawnStates[bestSpawn].enemy;
        enemies[slot].drawn=wasDrawn;
        enemies[slot].drawnX=oldX; enemies[slot].drawnY=oldY;
        spawnStates[bestSpawn].loadedSlot=slot;
    }
}

static void clearRuntimeSlots(void)
{
    UBYTE slot;
    memset(enemies,0,sizeof(enemies));
    for(slot=0;slot<MAX_ENEMIES;slot++) enemies[slot].spawnIndex=INVALID_SPAWN;
}

void enemiesInit(ULONG seed)
{
    clearRuntimeSlots();
    generateLevelEnemies(seed);
    activateVisibleSpawns(0,FALSE);
}

void enemiesResetPreservingDrawn(ULONG seed)
{
    BOOL wasDrawn[MAX_ENEMIES];
    WORD oldX[MAX_ENEMIES],oldY[MAX_ENEMIES];
    UBYTE oldType[MAX_ENEMIES];
    UBYTE slot;
    for(slot=0;slot<MAX_ENEMIES;slot++) {
        wasDrawn[slot]=enemies[slot].drawn;
        oldX[slot]=enemies[slot].drawnX;
        oldY[slot]=enemies[slot].drawnY;
        oldType[slot]=enemies[slot].drawnType;
    }
    clearRuntimeSlots();
    for(slot=0;slot<MAX_ENEMIES;slot++) {
        enemies[slot].drawn=wasDrawn[slot];
        enemies[slot].drawnX=oldX[slot]; enemies[slot].drawnY=oldY[slot];
        enemies[slot].drawnType=oldType[slot];
    }
    generateLevelEnemies(seed);
    activateVisibleSpawns(0,TRUE);
}

static void updateEnemy(struct Enemy *enemy,EnemySolidAt solidAt)
{
    WORD nextX,front,speed,width=enemyWidthForType(enemy->type);
    WORD height=enemyHeightForType(enemy->type);
    if(enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER) {
        enemy->vx=0; enemy->animFrame=0; enemy->facingLeft=FALSE;
        return;
    }
    if(enemy->dying) {
        enemy->animFrame=(UBYTE)(5+(20-enemy->deathTimer)/5);
        if(!--enemy->deathTimer) enemy->active=FALSE;
        return;
    }
    if(enemy->hitTimer) {
        enemy->hitTimer--; enemy->animFrame=4; return;
    }
    nextX=(WORD)((enemy->x+enemy->vx)>>8);
    front=nextX+(enemy->vx<0?1:width-2);
    if(enemy->animFrame>3) enemy->animFrame=0;
    if(nextX<enemy->patrolLeft||nextX>enemy->patrolRight-width||
       solidAt(front,enemy->y+height-8)||
       !solidAt(front,enemy->y+height)) {
        enemy->vx=-enemy->vx;
    } else {
        enemy->x+=enemy->vx;
        speed=(WORD)(enemy->vx<0?-enemy->vx:enemy->vx);
        enemy->walkTick=(UWORD)(enemy->walkTick+speed);
        if(enemy->walkTick>=WALK_PHASE_DISTANCE) {
            enemy->walkTick=(UWORD)(enemy->walkTick-WALK_PHASE_DISTANCE);
            enemy->animFrame=(UBYTE)((enemy->animFrame+1)&3);
        }
    }
    enemy->facingLeft=enemy->vx<0;
}

void enemiesUpdate(WORD cameraX,EnemySolidAt solidAt)
{
    const struct EnemySpawnCandidate *spawns;
    UWORD candidateCount;
    UBYTE slot,spawnIndex;
    spawns=levelEnemySpawnCandidates(&candidateCount);
    for(slot=0;slot<MAX_ENEMIES;slot++) {
        struct Enemy *enemy=&enemies[slot];
        struct EnemySpawnState *state;
        if(enemy->spawnIndex==INVALID_SPAWN) continue;
        state=&spawnStates[enemy->spawnIndex];
        if(enemy->active) updateEnemy(enemy,solidAt);
        state->enemy=*enemy; state->enemy.drawn=FALSE;
        if(!enemy->active&&!state->respawnPending&&!state->exhausted) {
            if(spawns[enemy->spawnIndex].policy==ENEMY_POLICY_RESPAWN) {
                state->respawnPending=TRUE;
                state->respawnTimer=(UWORD)(RESPAWN_MIN_FRAMES+
                                            randomBelow(RESPAWN_FRAME_RANGE));
            } else state->exhausted=TRUE;
        } else if(enemy->active&&
                  (enemy->patrolRight<cameraX-UNLOAD_MARGIN||
                   enemy->patrolLeft>cameraX+SCREEN_W+UNLOAD_MARGIN)) {
            state->loadedSlot=INVALID_SPAWN;
            enemy->active=FALSE;
            enemy->spawnIndex=INVALID_SPAWN;
            continue;
        }
        if(!enemy->active&&!enemy->drawn) {
            state->loadedSlot=INVALID_SPAWN;
            memset(enemy,0,sizeof(*enemy));
            enemy->spawnIndex=INVALID_SPAWN;
        }
    }
    for(spawnIndex=0;spawnIndex<spawnCount;spawnIndex++) {
        struct EnemySpawnState *state=&spawnStates[spawnIndex];
        if(!state->selected||!state->respawnPending) continue;
        if(state->respawnTimer) state->respawnTimer--;
        if(!state->respawnTimer&&
           (spawns[spawnIndex].surface.right<cameraX-ACTIVATE_MARGIN||
            spawns[spawnIndex].surface.left>
                cameraX+SCREEN_W+ACTIVATE_MARGIN)) {
            initializeSpawnState(state,&spawns[spawnIndex],spawnIndex,TRUE);
        }
    }
    activateVisibleSpawns(cameraX,FALSE);
}

BOOL enemiesHitProjectile(WORD x,WORD y,BOOL lowShot)
{
    WORD index;
    for(index=0;index<MAX_ENEMIES;index++) {
        struct Enemy *enemy=&enemies[index];
        if(enemy->type==ENEMY_TYPE_CLOCKWORK_BEETLE&&lowShot&&
           enemy->active&&!enemy->dying&&
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

BOOL enemiesContactPlayer(WORD left,WORD top,WORD right,WORD bottom,
                          WORD *enemyCenterX)
{
    WORD index;
    for(index=0;index<MAX_ENEMIES;index++) {
        struct Enemy *enemy=&enemies[index];
        WORD enemyLeft,enemyRight,enemyTop,enemyBottom;
        if(enemy->type!=ENEMY_TYPE_CLOCKWORK_BEETLE||
           !enemy->active||enemy->dying) continue;
        enemyLeft=(WORD)(enemy->x>>8)+2;
        enemyRight=(WORD)(enemy->x>>8)+ENEMY_W-3;
        enemyTop=enemy->y+7; enemyBottom=enemy->y+ENEMY_H-1;
        if(right>=enemyLeft&&left<=enemyRight&&
           bottom>=enemyTop&&top<=enemyBottom) {
            *enemyCenterX=(WORD)(enemy->x>>8)+(ENEMY_W>>1);
            return TRUE;
        }
    }
    return FALSE;
}

struct Enemy *enemyAt(WORD index)
{
    return &enemies[index];
}
