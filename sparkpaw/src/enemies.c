#include "enemies.h"

#include "level_data.h"

#include <string.h>

#define INVALID_SPAWN 255
#define SCREEN_W 320
#define ACTIVATE_MARGIN 96
#define UNLOAD_MARGIN 32
#define WALK_PHASE_DISTANCE 384
#define STRIDER_WALK_PHASE_DISTANCE 768
#define STRIDER_WALK_FRAMES 8
#define STRIDER_TURN_FRAMES 6
#define STRIDER_COMPRESS_START_FRAME 18
#define STRIDER_COMPRESS_CHARGED_FRAME 19
#define STRIDER_FLIGHT_FRAME 20
#define STRIDER_DESCENT_FRAME 21
#define STRIDER_LANDING_FRAME 22
#define STRIDER_RECOVERY_FRAME 23
#define STRIDER_TELEGRAPH_STAGE_FRAMES 6
#define STRIDER_LANDING_HOLD_FRAMES 5
#define STRIDER_RECOVERY_HOLD_FRAMES 7
#define STRIDER_MAX_FLIGHT_FRAMES 96
#define STRIDER_CONTACT_LEFT 11
#define STRIDER_CONTACT_RIGHT 52
#define STRIDER_CONTACT_TOP 7
#define STRIDER_CONTACT_BOTTOM 61
#define STRIDER_SHOOT_TELEGRAPH_FRAMES 24
#define STRIDER_SHOOT_FIRE_FRAMES 6
#define STRIDER_SHOOT_COOLDOWN_FRAMES 150
#define STRIDER_SHOOT_MIN_DISTANCE 48
#define STRIDER_SHOOT_MAX_DISTANCE 208
#define STRIDER_HIT_FIRST_FRAME 11
#define STRIDER_HIT_LAST_FRAME 17
#define STRIDER_HIT_FRAME_HOLD 2
#define STRIDER_HIT_FRAMES ((STRIDER_HIT_LAST_FRAME-STRIDER_HIT_FIRST_FRAME)+1)
#define STRIDER_HIT_TOTAL_FRAMES (STRIDER_HIT_FRAMES*STRIDER_HIT_FRAME_HOLD)
#define STRIDER_DEATH_FIRST_FRAME 24
#define STRIDER_DEATH_FRAME_HOLD 5
#define STRIDER_DEATH_FRAMES 4
#define STRIDER_DEATH_TOTAL_FRAMES (STRIDER_DEATH_FRAMES*STRIDER_DEATH_FRAME_HOLD)
#define TRAVERSAL_NONE 0
#define TRAVERSAL_COMPRESS_START 1
#define TRAVERSAL_COMPRESS_CHARGED 2
#define TRAVERSAL_FLIGHT 3
#define TRAVERSAL_LANDING 4
#define TRAVERSAL_RECOVERY 5
#define INVALID_TRAVERSAL_LINK 255
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

static BOOL spawnRuntimeReady(UBYTE type)
{
    if(type==ENEMY_TYPE_CLOCKWORK_BEETLE) return TRUE;
    /* Phase 6C admits every authored Strider candidate while the existing
       camera-managed pool still limits simultaneous enemy Bobs to four. */
    return type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER;
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
    const struct EnemyPatrolSurface *surface=
        levelEnemyPatrolSurface(spawn->surfaceId);
    WORD speed=speeds[randomBelow(3)];
    BOOL selected=state->selected;
    if(!surface) return;
    memset(state,0,sizeof(*state));
    enemy->x=(LONG)(spawn->minX+randomBelow(
        (UWORD)(spawn->maxX-spawn->minX+1)))<<8;
    enemy->y=(WORD)(surface->groundY-enemyHeightForType(spawn->type));
    enemy->vx=spawn->initialDirection<0?-speed:speed;
    enemy->patrolLeft=surface->left;
    enemy->patrolRight=surface->right;
    enemy->health=spawn->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER?3:2;
    enemy->active=TRUE;
    enemy->facingLeft=enemy->vx<0;
    enemy->spawnIndex=spawnIndex; enemy->type=spawn->type;
    enemy->surfaceId=spawn->surfaceId;
    enemy->traversalLink=INVALID_TRAVERSAL_LINK;
    if(spawn->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER)
        enemy->shootCooldown=(UBYTE)(75+randomBelow(100));
    state->selected=preserveSelection?selected:TRUE;
    state->loadedSlot=INVALID_SPAWN;
}

static const struct EnemyTraversalLink *readyTraversalLinkForEnemy(
    const struct Enemy *enemy,UBYTE *linkIndex)
{
    const struct EnemyTraversalLink *links;
    UWORD count,index;
    WORD x=(WORD)(enemy->x>>8);
    links=levelEnemyTraversalLinks(&count);
    for(index=0;index<count;index++)
        if(links[index].sourceSurfaceId==enemy->surfaceId&&
           (links[index].launchDirection<0)==(enemy->vx<0)&&
           x>=links[index].launchLeft&&x<=links[index].launchRight) {
            *linkIndex=(UBYTE)index;
            return &links[index];
        }
    return NULL;
}

static BOOL traversalLandingAvailable(const struct EnemyTraversalLink *link,
                                      EnemySolidAt solidAt)
{
    const struct EnemyPatrolSurface *destination=
        levelEnemyPatrolSurface(link->destinationSurfaceId);
    WORD inset=link->launchDirection<0?12:STRIDER_W-1-12;
    WORD first=(WORD)(link->landingLeft+inset);
    WORD last=(WORD)(link->landingRight+inset);
    if(!destination) return FALSE;
    return solidAt(first,destination->groundY)&&
           solidAt(last,destination->groundY)&&
           !solidAt(first,destination->groundY-8)&&
           !solidAt(last,destination->groundY-8);
}

static BOOL startTraversalIfReady(struct Enemy *enemy,EnemySolidAt solidAt)
{
    const struct EnemyTraversalLink *link;
    UBYTE linkIndex;
    link=readyTraversalLinkForEnemy(enemy,&linkIndex);
    if(!link||enemy->traversalLink!=INVALID_TRAVERSAL_LINK) return FALSE;
    if(!traversalLandingAvailable(link,solidAt)) {
        /* A blocked authored destination is not inferred or forced. Turn away
           on the source surface and reconsider only after a later approach. */
        enemy->vx=-enemy->vx;
        enemy->facingLeft=enemy->vx<0;
        return TRUE;
    }
    enemy->resumeVX=enemy->vx;
    enemy->vx=0; enemy->vy=0;
    enemy->traversalLink=linkIndex;
    enemy->traversalState=TRAVERSAL_COMPRESS_START;
    enemy->traversalTimer=STRIDER_TELEGRAPH_STAGE_FRAMES;
    enemy->traversalStartX=(WORD)(enemy->x>>8);
    enemy->traversalFailed=FALSE;
    enemy->animFrame=STRIDER_COMPRESS_START_FRAME;
    return TRUE;
}

static void failTraversal(struct Enemy *enemy)
{
    const struct EnemyPatrolSurface *source=
        levelEnemyPatrolSurface(enemy->surfaceId);
    if(source) {
        enemy->x=((LONG)enemy->traversalStartX)<<8;
        enemy->y=(WORD)(source->groundY-STRIDER_H);
        enemy->patrolLeft=source->left;
        enemy->patrolRight=source->right;
    }
    enemy->jumpY=((LONG)enemy->y)<<8;
    enemy->vx=0; enemy->vy=0;
    enemy->traversalFailed=TRUE;
    enemy->traversalState=TRAVERSAL_RECOVERY;
    enemy->traversalTimer=STRIDER_RECOVERY_HOLD_FRAMES;
    enemy->animFrame=STRIDER_RECOVERY_FRAME;
}

static void updateTraversal(struct Enemy *enemy)
{
    const struct EnemyTraversalLink *links,*link;
    const struct EnemyPatrolSurface *destination;
    UWORD count;
    WORD nextY,x;
    links=levelEnemyTraversalLinks(&count);
    if(enemy->traversalLink>=count) {
        enemy->traversalState=TRAVERSAL_NONE;
        enemy->traversalLink=INVALID_TRAVERSAL_LINK;
        return;
    }
    link=&links[enemy->traversalLink];
    destination=levelEnemyPatrolSurface(link->destinationSurfaceId);
    if(!destination) {
        enemy->traversalState=TRAVERSAL_NONE;
        enemy->traversalLink=INVALID_TRAVERSAL_LINK;
        return;
    }
    if(enemy->traversalState==TRAVERSAL_COMPRESS_START) {
        enemy->animFrame=STRIDER_COMPRESS_START_FRAME;
        if(!--enemy->traversalTimer) {
            enemy->traversalState=TRAVERSAL_COMPRESS_CHARGED;
            enemy->traversalTimer=STRIDER_TELEGRAPH_STAGE_FRAMES;
            enemy->animFrame=STRIDER_COMPRESS_CHARGED_FRAME;
        }
        return;
    }
    if(enemy->traversalState==TRAVERSAL_COMPRESS_CHARGED) {
        enemy->animFrame=STRIDER_COMPRESS_CHARGED_FRAME;
        if(!--enemy->traversalTimer) {
            enemy->traversalState=TRAVERSAL_FLIGHT;
            enemy->vx=link->launchVX; enemy->vy=link->launchVY;
            enemy->traversalTimer=0;
            enemy->jumpY=((LONG)enemy->y)<<8;
            enemy->facingLeft=enemy->vx<0;
            /* Keep the complete authored route eligible for camera parking
               while the actor is between its source and destination. */
            if(destination->left<enemy->patrolLeft)
                enemy->patrolLeft=destination->left;
            if(destination->right>enemy->patrolRight)
                enemy->patrolRight=destination->right;
        }
        return;
    }
    if(enemy->traversalState==TRAVERSAL_FLIGHT) {
        enemy->traversalTimer++;
        enemy->x+=enemy->vx;
        enemy->vy+=link->gravity;
        enemy->jumpY+=enemy->vy;
        nextY=(WORD)(enemy->jumpY>>8);
        enemy->y=nextY;
        enemy->animFrame=enemy->vy<0?STRIDER_FLIGHT_FRAME:
                                      STRIDER_DESCENT_FRAME;
        x=(WORD)(enemy->x>>8);
        if(enemy->vy>=0&&x>=link->landingLeft&&x<=link->landingRight&&
           enemy->y>=destination->groundY-STRIDER_H) {
            enemy->y=(WORD)(destination->groundY-STRIDER_H);
            enemy->jumpY=((LONG)enemy->y)<<8;
            enemy->vx=0; enemy->vy=0;
            enemy->traversalState=TRAVERSAL_LANDING;
            enemy->traversalTimer=STRIDER_LANDING_HOLD_FRAMES;
            enemy->animFrame=STRIDER_LANDING_FRAME;
        } else if(enemy->traversalTimer>=STRIDER_MAX_FLIGHT_FRAMES||
                  (enemy->vy>=0&&
                   enemy->y>=destination->groundY-STRIDER_H&&
                   ((enemy->vx>=0&&x>link->landingRight)||
                    (enemy->vx<0&&x<link->landingLeft)))) {
            failTraversal(enemy);
        }
        return;
    }
    if(enemy->traversalState==TRAVERSAL_LANDING) {
        enemy->animFrame=STRIDER_LANDING_FRAME;
        if(!--enemy->traversalTimer) {
            enemy->traversalState=TRAVERSAL_RECOVERY;
            enemy->traversalTimer=STRIDER_RECOVERY_HOLD_FRAMES;
            enemy->animFrame=STRIDER_RECOVERY_FRAME;
        }
        return;
    }
    enemy->animFrame=STRIDER_RECOVERY_FRAME;
    if(!--enemy->traversalTimer) {
        if(!enemy->traversalFailed) {
            enemy->surfaceId=link->destinationSurfaceId;
            enemy->patrolLeft=destination->left;
            enemy->patrolRight=destination->right;
            enemy->vx=enemy->resumeVX;
        } else {
            enemy->vx=-enemy->resumeVX;
            enemy->traversalFailed=FALSE;
        }
        enemy->facingLeft=enemy->vx<0;
        enemy->walkTick=0; enemy->animFrame=0;
        enemy->traversalState=TRAVERSAL_NONE;
        enemy->traversalLink=INVALID_TRAVERSAL_LINK;
    }
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
        UBYTE oldType=enemies[slot].drawnType;
        if(enemies[slot].active||(!allowRestoreSlots&&wasDrawn)||
           enemies[slot].spawnIndex!=INVALID_SPAWN) continue;
        for(spawnIndex=0;spawnIndex<spawnCount;spawnIndex++) {
            struct EnemySpawnState *state=&spawnStates[spawnIndex];
            LONG priority;
            if(!state->selected||state->respawnPending||state->exhausted||
               state->loadedSlot!=INVALID_SPAWN||
               !spawnRuntimeReady(state->enemy.type)||
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
        enemies[slot].drawnType=oldType;
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
    WORD footInset=enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER?12:1;
    BOOL patrolEnd,blocked;
    UWORD phaseDistance=enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER?
        STRIDER_WALK_PHASE_DISTANCE:WALK_PHASE_DISTANCE;
    if(enemy->shootCooldown) enemy->shootCooldown--;
    if(enemy->traversalState) {
        updateTraversal(enemy);
        return;
    }
    if(enemy->shootTimer) {
        enemy->shootTimer--;
        enemy->animFrame=enemy->shootTimer>=STRIDER_SHOOT_FIRE_FRAMES?9:10;
        if(enemy->shootTimer==STRIDER_SHOOT_FIRE_FRAMES)
            enemy->shotPending=TRUE;
        if(!enemy->shootTimer) {
            enemy->vx=enemy->attackVX;
            enemy->facingLeft=enemy->vx<0;
            enemy->walkTick=0; enemy->animFrame=0;
            enemy->shootCooldown=STRIDER_SHOOT_COOLDOWN_FRAMES;
        }
        return;
    }
    if(enemy->dying) {
        enemy->animFrame=enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER?
            (UBYTE)(STRIDER_DEATH_FIRST_FRAME+
                (STRIDER_DEATH_TOTAL_FRAMES-enemy->deathTimer)/
                STRIDER_DEATH_FRAME_HOLD):
            (UBYTE)(10+(20-enemy->deathTimer)/5);
        if(!--enemy->deathTimer) enemy->active=FALSE;
        return;
    }
    if(enemy->hitTimer) {
        if(enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER) {
            enemy->animFrame=(UBYTE)(STRIDER_HIT_FIRST_FRAME+
                (STRIDER_HIT_TOTAL_FRAMES-enemy->hitTimer)/
                STRIDER_HIT_FRAME_HOLD);
            enemy->hitTimer--;
            if(!enemy->hitTimer) {
                enemy->vx=enemy->attackVX;
                enemy->facingLeft=enemy->vx<0;
                enemy->walkTick=0;
            }
        } else {
            enemy->hitTimer--; enemy->animFrame=4;
        }
        return;
    }
    if(enemy->turnTimer) {
        enemy->animFrame=8;
        if(!--enemy->turnTimer) {
            enemy->facingLeft=enemy->vx<0;
            enemy->animFrame=0;
            enemy->walkTick=0;
        }
        return;
    }
    if(enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER&&
       startTraversalIfReady(enemy,solidAt)) return;
    nextX=(WORD)((enemy->x+enemy->vx)>>8);
    front=nextX+(enemy->vx<0?footInset:width-1-footInset);
    patrolEnd=front<enemy->patrolLeft||front>=enemy->patrolRight;
    blocked=solidAt(front,enemy->y+height-8)||
            !solidAt(front,enemy->y+height);
    if(enemy->animFrame>=(enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER?
                          STRIDER_WALK_FRAMES:4))
        enemy->animFrame=0;
    if(patrolEnd||blocked) {
        enemy->vx=-enemy->vx;
        /* Only authored patrol extrema own the visible planted turn. A solid
           or missing-support safety probe may still reverse movement, but it
           must not flash slot 8 at an incidental point inside the route. */
        if(patrolEnd&&
           enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER) {
            enemy->turnTimer=STRIDER_TURN_FRAMES;
            enemy->animFrame=8;
        }
    } else {
        enemy->x+=enemy->vx;
        speed=(WORD)(enemy->vx<0?-enemy->vx:enemy->vx);
        enemy->walkTick=(UWORD)(enemy->walkTick+speed);
        if(enemy->walkTick>=phaseDistance) {
            enemy->walkTick=(UWORD)(enemy->walkTick-phaseDistance);
            if(enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER)
                /* Wrap inside the walk family immediately. Letting 7 become
                   8 exposed the reserved front-turn pose for one game frame
                   before the next update corrected it back to zero. */
                enemy->animFrame=(UBYTE)((enemy->animFrame+1)&7);
            else
                enemy->animFrame=(UBYTE)((enemy->animFrame+1)&3);
        }
    }
    if(!enemy->turnTimer) enemy->facingLeft=enemy->vx<0;
}

static BOOL enemyFullyVisible(const struct Enemy *enemy,WORD cameraX)
{
    WORD x=(WORD)(enemy->x>>8);
    return x>=cameraX+8&&x+STRIDER_W<=cameraX+SCREEN_W-8;
}

static void tryStartStriderShot(struct Enemy *enemy,WORD cameraX,
                                WORD playerCenterX,WORD playerCenterY)
{
    WORD centerX,centerY,dx,dy;
    if(enemy->type!=ENEMY_TYPE_CLOCKWORK_STORM_STRIDER||
       enemy->shootCooldown||enemy->shootTimer||enemy->traversalState||
       enemy->turnTimer||enemy->dying||enemy->hitTimer||
       !enemyFullyVisible(enemy,cameraX)) return;
    centerX=(WORD)(enemy->x>>8)+(STRIDER_W>>1);
    centerY=(WORD)(enemy->y+(STRIDER_H>>1));
    dx=(WORD)(playerCenterX-centerX);
    dy=(WORD)(playerCenterY-centerY); if(dy<0) dy=-dy;
    if(dy>44||
       (dx<0?-dx:dx)<STRIDER_SHOOT_MIN_DISTANCE||
       (dx<0?-dx:dx)>STRIDER_SHOOT_MAX_DISTANCE||
       (enemy->facingLeft?dx>=0:dx<=0)) return;
    enemy->attackVX=enemy->vx; enemy->vx=0;
    enemy->shootTimer=STRIDER_SHOOT_TELEGRAPH_FRAMES;
    enemy->animFrame=9; enemy->walkTick=0;
}

void enemiesUpdate(WORD cameraX,EnemySolidAt solidAt,WORD playerCenterX,
                   WORD playerCenterY,EnemySpawnProjectile spawnProjectile)
{
    const struct EnemySpawnCandidate *spawns;
    UWORD candidateCount;
    UBYTE slot,spawnIndex;
    spawns=levelEnemySpawnCandidates(&candidateCount);
    /* Camera parking removes only the bounded runtime/Bob slot. Keep each
       persistent encounter's world-space route alive exactly once per frame so
       it may approach and re-enter from either side without a camera-edge
       reset. No rendering or Chip-RAM work occurs on this parked path. */
    for(spawnIndex=0;spawnIndex<spawnCount;spawnIndex++) {
        struct EnemySpawnState *state=&spawnStates[spawnIndex];
        if(state->selected&&
           state->enemy.type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER&&
           state->loadedSlot==INVALID_SPAWN&&
           state->enemy.active&&!state->respawnPending&&!state->exhausted)
            updateEnemy(&state->enemy,solidAt);
        /* A telegraph may finish after its runtime slot was parked. Preserve
           cooldown/recovery, but never materialize that unseen shot later. */
        if(state->loadedSlot==INVALID_SPAWN) state->enemy.shotPending=FALSE;
    }
    for(slot=0;slot<MAX_ENEMIES;slot++) {
        struct Enemy *enemy=&enemies[slot];
        struct EnemySpawnState *state;
        if(enemy->spawnIndex==INVALID_SPAWN) continue;
        state=&spawnStates[enemy->spawnIndex];
        if(enemy->active) {
            updateEnemy(enemy,solidAt);
            if(enemy->shotPending) {
                if(enemyFullyVisible(enemy,cameraX)&&spawnProjectile)
                    spawnProjectile((WORD)((enemy->x>>8)+
                        (enemy->facingLeft?-6:STRIDER_W-10)),
                        (WORD)(enemy->y+32),enemy->facingLeft);
                enemy->shotPending=FALSE;
            }
            tryStartStriderShot(enemy,cameraX,playerCenterX,playerCenterY);
        }
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
        const struct EnemyPatrolSurface *surface=
            levelEnemyPatrolSurface(spawns[spawnIndex].surfaceId);
        if(!state->selected||!state->respawnPending) continue;
        if(state->respawnTimer) state->respawnTimer--;
        if(surface&&!state->respawnTimer&&
           (surface->right<cameraX-ACTIVATE_MARGIN||
            surface->left>
                cameraX+SCREEN_W+ACTIVATE_MARGIN)) {
            initializeSpawnState(state,&spawns[spawnIndex],spawnIndex,TRUE);
        }
    }
    activateVisibleSpawns(cameraX,FALSE);
}

UBYTE enemiesHitProjectile(WORD x,WORD y,BOOL lowShot)
{
    WORD index;
    for(index=0;index<MAX_ENEMIES;index++) {
        struct Enemy *enemy=&enemies[index];
        if(enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER&&
           enemy->active&&!enemy->dying&&
           x>=(WORD)(enemy->x>>8)+STRIDER_CONTACT_LEFT&&
           x<=(WORD)(enemy->x>>8)+STRIDER_CONTACT_RIGHT&&
           y>=enemy->y+STRIDER_CONTACT_TOP&&
           y<=enemy->y+STRIDER_CONTACT_BOTTOM) {
            /* Phase 5F.3 owns non-lethal recoil; Phase 5F.4 appends death and
               reuses the generic safe off-camera respawn lifecycle. */
            if(!enemy->hitTimer) {
                if(enemy->health==1) {
                    enemy->health=0;
                    enemy->dying=TRUE;
                    enemy->deathTimer=STRIDER_DEATH_TOTAL_FRAMES;
                    enemy->shootTimer=0; enemy->shotPending=FALSE;
                    enemy->turnTimer=0;
                    enemy->traversalState=TRAVERSAL_NONE;
                    enemy->traversalLink=INVALID_TRAVERSAL_LINK;
                    enemy->traversalFailed=FALSE;
                    enemy->vx=0; enemy->vy=0;
                    enemy->animFrame=STRIDER_DEATH_FIRST_FRAME;
                    enemy->walkTick=0;
                } else if(enemy->health>1) enemy->health--;
                if(enemy->traversalState) {
                    /* Route frames 18..23 remain authoritative. The plasma
                       impact and sound provide immediate feedback; never play
                       a delayed ground recoil after landing. */
                } else if(!enemy->dying) {
                    if(enemy->shootTimer) {
                        enemy->shootTimer=0; enemy->shotPending=FALSE;
                        enemy->vx=enemy->attackVX;
                        enemy->shootCooldown=STRIDER_SHOOT_COOLDOWN_FRAMES;
                    }
                    enemy->attackVX=enemy->vx;
                    enemy->vx=0;
                    enemy->hitTimer=STRIDER_HIT_TOTAL_FRAMES;
                    enemy->animFrame=STRIDER_HIT_FIRST_FRAME;
                    enemy->walkTick=0;
                }
            }
            return enemy->dying?PROJECTILE_ENEMY_KILL:PROJECTILE_ENEMY_HIT;
        }
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
            return enemy->dying?PROJECTILE_ENEMY_KILL:PROJECTILE_ENEMY_HIT;
        }
    }
    return PROJECTILE_ENEMY_MISS;
}

BOOL enemiesContactPlayer(WORD left,WORD top,WORD right,WORD bottom,
                          WORD *enemyCenterX)
{
    WORD index;
    for(index=0;index<MAX_ENEMIES;index++) {
        struct Enemy *enemy=&enemies[index];
        WORD enemyLeft,enemyRight,enemyTop,enemyBottom;
        WORD x=(WORD)(enemy->x>>8),width;
        if(!enemy->active||enemy->dying) continue;
        if(enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER) {
            /* Keep contact inside the accepted mechanical body across walk,
               turn and traversal poses. Transparent cell margins and source
               rows 62-63 never deal invisible damage. */
            enemyLeft=x+STRIDER_CONTACT_LEFT;
            enemyRight=x+STRIDER_CONTACT_RIGHT;
            enemyTop=enemy->y+STRIDER_CONTACT_TOP;
            enemyBottom=enemy->y+STRIDER_CONTACT_BOTTOM;
            width=STRIDER_W;
        } else if(enemy->type==ENEMY_TYPE_CLOCKWORK_BEETLE) {
            enemyLeft=x+2; enemyRight=x+ENEMY_W-3;
            enemyTop=enemy->y+7; enemyBottom=enemy->y+ENEMY_H-1;
            width=ENEMY_W;
        } else continue;
        if(right>=enemyLeft&&left<=enemyRight&&
           bottom>=enemyTop&&top<=enemyBottom) {
            *enemyCenterX=x+(width>>1);
            return TRUE;
        }
    }
    return FALSE;
}

struct Enemy *enemyAt(WORD index)
{
    return &enemies[index];
}
