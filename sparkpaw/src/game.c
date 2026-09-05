#include "game.h"

#include "audio.h"
#include "camera_contract.h"
#include "collision.h"
#include "collectibles.h"
#include "enemies.h"
#include "level_data.h"
#include "performance_profile.h"
#include "platform_amiga.h"
#include "player.h"
#include "projectiles.h"
#include "stormrail_contract.h"
#include "world_config.h"

#define SCREEN_W 320
static struct GameState game;

static void accountElapsedFields(void)
{
    ULONG now=platformFieldCounter();
    game.elapsedFields+=(now-game.lastFieldCounter)&0x00ffffffUL;
    game.lastFieldCounter=now;
}

static void resetLevelRuntime(void)
{
    /* Assets and packed caches stay resident. Preserve only prior Bob restore
       rectangles so the next line-300 pass erases the old runtime state. */
    game.enemySeed=game.enemySeed*1664525UL+
                   (ULONG)game.frameCounter+1013904223UL;
    enemiesResetPreservingDrawn(game.enemySeed);
    collectiblesResetPreservingProgress();
    projectilesResetPreservingDrawn();
    playerInit();
    game.cameraX=0; game.frameCounter=0;
    game.waterSplashTimer=0;
    game.coreCollectTimer=0;
    game.lastFieldCounter=platformFieldCounter();
}

static BOOL applyEnemyDamage(WORD sourceCenterX)
{
    if(!playerTakeEnemyHit(sourceCenterX)) return FALSE;
    audioPlayPlayerHurt();
    if(playerState()->health) return FALSE;
    if(game.lives>1) game.lives--;
    else game.lives=GAME_START_LIVES;
    resetLevelRuntime();
    audioUpdate();
    return TRUE;
}

static BOOL spawnEnemyProjectile(WORD x,WORD y,BOOL facingLeft)
{
    if(!projectilesSpawnEnemy(x,y,facingLeft)) return FALSE;
    audioPlayStriderShot();
    return TRUE;
}

static void updateCamera(void)
{
#ifdef SPARKPAW_EXTRA_LIFE_VISUAL_PROOF
    game.cameraX=WORLD_W-SCREEN_W;
    return;
#else
    const struct PlayerState *player=playerState();
    LONG playerX=player->x>>8,wanted=game.cameraX;
    if(playerX>=3072||game.coreCollectTimer) wanted=WORLD_W-SCREEN_W;
    else {
#ifdef SPARKPAW_CAMERA_DEADZONE_REFERENCE
        if(playerX-game.cameraX>202) wanted=playerX-202;
        if(playerX-game.cameraX<105) wanted=playerX-105;
#else
        wanted=cameraCenteredTarget(playerX);
#endif
    }
    if(wanted<0) wanted=0;
    if(wanted>WORLD_W-SCREEN_W) wanted=WORLD_W-SCREEN_W;
    if(wanted>game.cameraX+5) game.cameraX+=5;
    else if(wanted<game.cameraX-5) game.cameraX-=5;
    else game.cameraX=wanted;
#endif
}

#ifdef SPARKPAW_STORMRAIL_PROOF
#define STORMRAIL_PARK_X 180
#define STORMRAIL_PARK_Y 132

static LONG stormrailTriangle(ULONG phase,ULONG period,LONG amplitude)
{
    ULONG half=period>>1,at=phase%period;
    LONG value=(LONG)(at<half?at:period-at);
    return (value*amplitude)/(LONG)half-amplitude/2;
}

static void stormrailSpawnFormation(UBYTE id)
{
    static const UBYTE counts[STORMRAIL_FORMATION_COUNT]={5,4,5,5,5,5,5,5};
    UBYTE i,count=counts[id];
    game.stormrailFormationSpawned[id]=1;
    game.stormrailFormationAlive[id]=count;
    for(i=0;i<count;i++) {
        game.stormrailEnemyActive[i]=TRUE;
        game.stormrailEnemyKind[i]=(UBYTE)(id==1||(id==3&&i>=3)||
            (id==4&&i==4)||(id==6&&i>=3));
        game.stormrailEnemyFormation[i]=id;
        game.stormrailEnemyHp[i]=(UBYTE)(game.stormrailEnemyKind[i]?2:1);
        game.stormrailEnemyAge[i]=(UWORD)(id>=4?0:i*10);
        game.stormrailEnemyX[i]=(WORD)(328+i*24);
        game.stormrailEnemyY[i]=(WORD)(id==1?(24+i*36):
            id==2?(i<3?28+i*24:150+(i-3)*22):
            id==3?(34+i*30):id==4?(i<4?42+i*30:88):
            id==5?(30+i*28):id==6?(i<3?34+i*54:62+(i-3)*72):
            id==7?88:(40+i*24));
    }
}

static BOOL stormrailRewardPoolEmpty(void)
{
    UBYTE i;
    for(i=0;i<STORMRAIL_MAX_REWARDS;i++)
        if(game.stormrailRewardActive[i]) return FALSE;
    return TRUE;
}

static BOOL stormrailPickupWasConsumed(UBYTE pickupId)
{
    ULONG bit=1UL<<(pickupId&31);
    return (BOOL)(pickupId<32?
        (game.stormrailPickupConsumed&bit):
        (game.stormrailPickupConsumedHigh&bit));
}

static void stormrailMarkPickupConsumed(UBYTE pickupId)
{
    ULONG bit=1UL<<(pickupId&31);
    if(pickupId<32) game.stormrailPickupConsumed|=bit;
    else game.stormrailPickupConsumedHigh|=bit;
}

static void stormrailSpawnDiamondLine(UBYTE baseId,UBYTE pattern,WORD x,WORD y)
{
    UBYTE i;
    for(i=0;i<STORMRAIL_MAX_REWARDS;i++) {
        UBYTE pickupId=(UBYTE)(baseId+i);
        if(stormrailPickupWasConsumed(pickupId)) continue;
        game.stormrailRewardActive[i]=TRUE;
        game.stormrailRewardId[i]=pickupId;
        game.stormrailRewardKind[i]=STORMRAIL_REWARD_DIAMOND;
        game.stormrailRewardX[i]=(WORD)(x+i*18);
        game.stormrailRewardY[i]=(WORD)(pattern==0?y:
            pattern==1?y+i*14:
            pattern==2?y+(i<2?i*28:(3-i)*28):
            y+(i&1?34:0));
    }
}

static void stormrailQueueFormationReward(UBYTE id)
{
    if(!game.stormrailFormationEscaped[id])
        game.stormrailFormationRewardPending[id]=1;
}

static BOOL stormrailSpawnObstacle(UBYTE id,UBYTE type,UBYTE frame,
                                   WORD x,WORD y,WORD dx,WORD dy,
                                   BOOL destructible,UBYTE dropId)
{
    UBYTE i;
    for(i=0;i<STORMRAIL_MAX_OBSTACLES;i++)
        if(!game.stormrailObstacleActive[i]) {
            game.stormrailObstacleActive[i]=TRUE;
            game.stormrailObstacleType[i]=type;
            frame=stormrailObstacleSafeBaseFrame(type,frame);
            game.stormrailObstacleFrame[i]=frame;
            game.stormrailObstacleBaseFrame[i]=frame;
            game.stormrailObstacleAnimFrames[i]=2;
            game.stormrailObstacleHp[i]=(UBYTE)(destructible?
                (type==STORMRAIL_OBSTACLE_BIG?STORMRAIL_OBSTACLE_BIG_HP:
                 type==STORMRAIL_OBSTACLE_PILLAR?
                 STORMRAIL_OBSTACLE_PILLAR_HP:
                 STORMRAIL_OBSTACLE_SMALL_HP):255);
            game.stormrailObstacleAge[i]=0;
            game.stormrailObstacleId[i]=id;
            game.stormrailObstacleDestructible[i]=(UBYTE)destructible;
            game.stormrailObstacleDropId[i]=dropId;
            game.stormrailObstacleFlash[i]=0;
            game.stormrailObstacleX[i]=x;
            game.stormrailObstacleY[i]=y;
            game.stormrailObstacleDx[i]=dx;
            game.stormrailObstacleDy[i]=dy;
            return TRUE;
        }
    return FALSE;
}

static void stormrailDropObstacleReward(UBYTE pickupId,UBYTE kind,WORD x,WORD y)
{
    UBYTE i;
    if(stormrailPickupWasConsumed(pickupId)) return;
    for(i=0;i<STORMRAIL_MAX_REWARDS;i++) if(!game.stormrailRewardActive[i]) {
        game.stormrailRewardActive[i]=TRUE;
        game.stormrailRewardId[i]=pickupId;
        game.stormrailRewardKind[i]=kind;
        game.stormrailRewardX[i]=x;
        game.stormrailRewardY[i]=y;
        game.stormrailFreeLineSpawned[3]=1;
        return;
    }
}

static BOOL stormrailMarkObstacleScoreAwarded(UBYTE id)
{
    ULONG bit=1UL<<(id&31);
    if(id<32) {
        if(game.stormrailObstacleScoreAwarded&bit) return FALSE;
        game.stormrailObstacleScoreAwarded|=bit;
    } else {
        if(game.stormrailObstacleScoreAwardedHigh&bit) return FALSE;
        game.stormrailObstacleScoreAwardedHigh|=bit;
    }
    return TRUE;
}

static void stormrailMarkDebrisEventHandled(UBYTE id)
{
    ULONG bit=1UL<<(id&31);
    if(id<32) game.stormrailObstacleSpawnMask|=bit;
    else game.stormrailObstacleSpawnMaskHigh|=bit;
}

static void stormrailUpdateObstacles(void)
{
    static const ULONG distance[STORMRAIL_DEBRIS_EVENT_COUNT]={
        5800UL,5880UL,5970UL,6070UL,6200UL,6300UL,
        6450UL,6520UL,6600UL,6740UL,6850UL,7010UL,
        7160UL,7240UL,7340UL,7500UL,7590UL,7700UL,
        7860UL,7930UL,8020UL,8140UL,8270UL,8420UL,
        8580UL,8650UL,8730UL,8840UL,8930UL,9060UL,9180UL,9340UL,
        9500UL,9580UL,9680UL,9800UL,9940UL,
        10100UL,10170UL,10260UL,10380UL,10510UL,10670UL,
        10830UL,10920UL,11040UL,11210UL,11330UL};
    static const UBYTE type[STORMRAIL_DEBRIS_EVENT_COUNT]={
        1,2,1,2,0,1, 1,2,2,1,1,0,
        2,1,1,0,2,1, 1,2,1,1,2,0,
        1,1,2,1,2,1,1,0,
        2,2,1,1,0, 1,2,1,2,1,0,
        2,1,1,0,2};
    static const UBYTE frame[STORMRAIL_DEBRIS_EVENT_COUNT]={
        0,0,2,0,0,4, 0,0,0,2,4,2,
        0,0,2,0,0,4, 0,0,2,4,0,2,
        0,2,0,4,0,0,2,0,
        0,0,4,0,2, 2,0,4,0,0,0,
        0,2,4,2,0};
    static const WORD x[STORMRAIL_DEBRIS_EVENT_COUNT]={
        336,352,328,344,336,360,328,344, 352,336,360,328,344,336,352,328,
        360,336,344,328,352,360,336,344, 328,352,336,360,344,328,352,336,
        360,344,328,336,352,328,360,344, 336,352,328,360,344,336,352,328};
    static const WORD y[STORMRAIL_DEBRIS_EVENT_COUNT]={
        20,150,96,0,118,176, 8,20,168,130,44,18,
        0,180,62,150,20,12, 168,30,80,16,168,66,
        184,42,0,0,160,122,40,12,
        168,8,36,180,126, 12,168,188,0,176,36,
        160,22,174,132,150};
    static const WORD dx[STORMRAIL_DEBRIS_EVENT_COUNT]={
        -3,-3,-4,-2,-4,-2, -4,-3,-3,-4,-2,-4,
        -2,-3,-4,-4,-3,-2, -3,-3,-4,-4,-2,-4,
        -4,-3,-2,-2,-3,-4,-3,-3,
        -4,-3,-2,-4,-4, -3,-2,-3,-4,-2,-4,
        -3,-4,-2,-4,-3};
    static const WORD dy[STORMRAIL_DEBRIS_EVENT_COUNT]={
        1,-1,1,1,0,-1, 1,0,-1,-1,1,1,
        1,-1,1,-1,1,1, -1,1,1,1,-1,0,
        -1,1,1,1,-1,-1,1,1,
        -1,1,0,-1,-1, 1,-1,-1,1,-1,1,
        -1,1,-1,-1,0};
    static const UBYTE destructible[STORMRAIL_DEBRIS_EVENT_COUNT]={
        0,0,1,0,0,1, 0,1,0,0,1,1,
        0,1,0,0,1,0, 0,1,0,1,0,0,
        1,0,1,0,0,1,0,1,
        0,1,0,1,0, 0,1,0,1,0,0,
        1,0,1,0,0};
    static const UBYTE dropId[STORMRAIL_DEBRIS_EVENT_COUNT]={
        255,255,255,255,255,255,255,255,255,255,255,32,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,33,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255};
    UBYTE i,j,activeBefore=0,bigBefore=0;
    for(i=0;i<STORMRAIL_MAX_OBSTACLES;i++)
        if(game.stormrailObstacleActive[i]) {
            activeBefore++;
            if(game.stormrailObstacleType[i]==STORMRAIL_OBSTACLE_BIG)
                bigBefore++;
        }
    while(game.stormrailDebrisNextEvent<STORMRAIL_DEBRIS_EVENT_COUNT&&
          game.stormrailDistance>=distance[game.stormrailDebrisNextEvent]) {
            i=game.stormrailDebrisNextEvent;
            {
            BOOL lootCarrier=(BOOL)(dropId[i]<64);
            BOOL typeSlot=(BOOL)(type[i]!=STORMRAIL_OBSTACLE_BIG||bigBefore<1);
            if(lootCarrier&&!typeSlot)
                for(j=0;j<STORMRAIL_MAX_OBSTACLES;j++)
                    if(game.stormrailObstacleActive[j]&&
                       game.stormrailObstacleType[j]==STORMRAIL_OBSTACLE_BIG) {
                        game.stormrailLootBlockerId=
                            game.stormrailObstacleId[j];
                        game.stormrailLootBlockerAge=
                            game.stormrailObstacleAge[j];
                        game.stormrailLootBlockerX=
                            game.stormrailObstacleX[j];
                        game.stormrailLootBlockedDistance=
                            game.stormrailDistance;
                        break;
                    }
            if(lootCarrier&&typeSlot&&
               activeBefore>=STORMRAIL_MAX_OBSTACLES) {
                UBYTE victim=STORMRAIL_MAX_OBSTACLES;
                UBYTE oldest=0;
                /* Loot beats own an authored slot. Retire only the oldest
                 * ordinary small/medium cue, which is already nearest the
                 * left exit; never evict another reward carrier. */
                for(j=0;j<STORMRAIL_MAX_OBSTACLES;j++)
                    if(game.stormrailObstacleActive[j]&&
                       game.stormrailObstacleType[j]!=STORMRAIL_OBSTACLE_BIG&&
                       game.stormrailObstacleDropId[j]>=32&&
                       (victim==STORMRAIL_MAX_OBSTACLES||
                        game.stormrailObstacleAge[j]>oldest)) {
                        victim=j;
                        oldest=game.stormrailObstacleAge[j];
                    }
                if(victim<STORMRAIL_MAX_OBSTACLES) {
                    game.stormrailObstacleActive[victim]=FALSE;
                    activeBefore--;
                }
            }
            if(activeBefore<STORMRAIL_MAX_OBSTACLES&&typeSlot&&
               stormrailSpawnObstacle(i,type[i],frame[i],x[i],y[i],dx[i],dy[i],
                                      destructible[i],dropId[i])) {
                stormrailMarkDebrisEventHandled(i);
                game.stormrailDebrisNextEvent++;
                game.stormrailLastDebrisSpawnTick=game.stormrailLaunchTimer;
                game.stormrailLastDebrisSpawnDistance=game.stormrailDistance;
                game.stormrailLastDebrisSpawnId=i;
                activeBefore++;
                if(type[i]==STORMRAIL_OBSTACLE_BIG) bigBefore++;
            } else if(!lootCarrier) {
                /* A normal density cue is consumed at its authored distance.
                 * Waiting for a later pool vacancy creates a rhythmic tail. */
                stormrailMarkDebrisEventHandled(i);
                game.stormrailDebrisNextEvent++;
            } else {
                /* A large loot carrier may wait only for the preceding large
                 * silhouette to exit. Do not let later cues overtake it. */
                break;
            }
        }
    }
    {
        UBYTE active=0;
        for(i=0;i<STORMRAIL_MAX_OBSTACLES;i++)
            if(game.stormrailObstacleActive[i]) active++;
        if(active>game.stormrailObstaclePeak)
            game.stormrailObstaclePeak=active;
        if(bigBefore>game.stormrailObstacleBigPeak)
            game.stormrailObstacleBigPeak=bigBefore;
    }
    for(i=0;i<STORMRAIL_MAX_OBSTACLES;i++)
        if(game.stormrailObstacleActive[i]) {
            UBYTE type=game.stormrailObstacleType[i];
            WORD width=(WORD)stormrailObstacleWidth(type);
            WORD height=(WORD)stormrailObstacleHeight(type);
            game.stormrailObstacleX[i]+=game.stormrailObstacleDx[i];
            game.stormrailObstacleY[i]+=game.stormrailObstacleDy[i];
            if(game.stormrailObstacleAge[i]<255)
                game.stormrailObstacleAge[i]++;
            if(game.stormrailObstacleAnimFrames[i]>1)
                game.stormrailObstacleFrame[i]=(UBYTE)(
                    game.stormrailObstacleBaseFrame[i]+
                    ((game.stormrailObstacleAge[i]+
                      game.stormrailObstacleId[i]*5)>>3&1));
            for(j=0;j<STORMRAIL_MAX_SHOTS;j++)
                if(stormrailObstacleHittable(game.stormrailObstacleX[i],type)&&
                   game.stormrailShotActive[j]&&
                   game.stormrailShotX[j]+16>=game.stormrailObstacleX[i]&&
                   game.stormrailShotX[j]<=game.stormrailObstacleX[i]+width&&
                   game.stormrailShotY[j]+8>=game.stormrailObstacleY[i]&&
                   game.stormrailShotY[j]<=game.stormrailObstacleY[i]+height) {
                    game.stormrailShotActive[j]=FALSE;
                    if(!game.stormrailObstacleDestructible[i]) {
                        audioPlayEnemyHit();
                        break;
                    }
                    game.stormrailObstacleFlash[i]=STORMRAIL_OBSTACLE_HIT_FLASH;
                    if(--game.stormrailObstacleHp[i]==0) {
                        WORD dropX=game.stormrailObstacleX[i];
                        WORD dropY=game.stormrailObstacleY[i];
                        UBYTE dropId=game.stormrailObstacleDropId[i];
                        game.stormrailObstacleActive[i]=FALSE;
                        if(stormrailMarkObstacleScoreAwarded(
                                game.stormrailObstacleId[i])) {
                            game.score+=type==STORMRAIL_OBSTACLE_BIG?20:
                                        type==STORMRAIL_OBSTACLE_PILLAR?10:5;
                        }
                        audioPlayEnemyDeath();
                        if(dropId<64)
                            stormrailDropObstacleReward(dropId,
                                dropId==33?STORMRAIL_REWARD_HEART:
                                           STORMRAIL_REWARD_DIAMOND,
                                dropX,dropY);
                    } else {
                        if(type==STORMRAIL_OBSTACLE_BIG&&
                           game.stormrailObstacleHp[i]<=3) {
                            game.stormrailObstacleBaseFrame[i]=4;
                            game.stormrailObstacleAnimFrames[i]=1;
                            game.stormrailObstacleFrame[i]=4;
                        }
                        audioPlayEnemyHit();
                    }
                    break;
                }
            if(game.stormrailObstacleFlash[i])
                game.stormrailObstacleFlash[i]--;
#ifndef SPARKPAW_STORMRAIL_AUTO_BOARD
            if(game.stormrailObstacleActive[i]&&
               game.stormrailObstacleAge[i]>=STORMRAIL_OBSTACLE_CONTACT_GRACE&&
               !game.stormrailInvuln&&
               game.stormrailObstacleX[i]+width>=game.stormrailX+10&&
               game.stormrailObstacleX[i]<=game.stormrailX+70&&
               game.stormrailObstacleY[i]+height>=game.stormrailY+4&&
               game.stormrailObstacleY[i]<=game.stormrailY+28) {
                game.stormrailObstacleActive[i]=FALSE;
                if(game.stormrailHealth) game.stormrailHealth--;
                game.stormrailInvuln=STORMRAIL_HIT_INVULN;
                game.stormrailCollisions++;
                audioPlayPlayerHurt();
            }
#endif
            if(game.stormrailObstacleActive[i]&&
               (game.stormrailObstacleX[i]<-width||
                (game.stormrailObstacleX[i]>=STORMRAIL_PLAYFIELD_W&&
                 game.stormrailObstacleDx[i]>=0)||
                game.stormrailObstacleY[i]<0||
                game.stormrailObstacleY[i]>
                    STORMRAIL_PLAYFIELD_H-height))
                game.stormrailObstacleActive[i]=FALSE;
            if(game.stormrailObstacleActive[i]&&
               game.stormrailObstacleAge[i]>=STORMRAIL_OBSTACLE_MAX_AGE)
                game.stormrailObstacleActive[i]=FALSE;
        }
    for(i=0;i<STORMRAIL_MAX_OBSTACLES;i++)
        if(game.stormrailObstacleActive[i]) {
            game.stormrailLastDebrisActiveTick=game.stormrailLaunchTimer;
            game.stormrailLastDebrisActiveDistance=game.stormrailDistance;
            break;
        }
}

static void stormrailUpdateEncounters(void)
{
    static const ULONG starts[STORMRAIL_FORMATION_COUNT]={
        STORMRAIL_FORMATION0_DISTANCE,STORMRAIL_FORMATION1_DISTANCE,
        STORMRAIL_FORMATION2_DISTANCE,STORMRAIL_FORMATION3_DISTANCE,
        STORMRAIL_FORMATION4_DISTANCE,STORMRAIL_FORMATION5_DISTANCE,
        STORMRAIL_FORMATION6_DISTANCE,STORMRAIL_FORMATION7_DISTANCE};
    UBYTE i,j,active=0;
    for(i=0;i<STORMRAIL_MAX_ENEMIES;i++) if(game.stormrailEnemyActive[i]) active++;
    if(!active) for(i=0;i<STORMRAIL_FORMATION_COUNT;i++)
        if(!game.stormrailFormationSpawned[i]&&game.stormrailDistance>=starts[i]&&
           (i!=6||(game.stormrailFreeLineSpawned[4]&&stormrailRewardPoolEmpty()))) {
            stormrailSpawnFormation(i); break;
        }
    for(i=0;i<STORMRAIL_MAX_ENEMIES;i++) if(game.stormrailEnemyActive[i]) {
        UBYTE formation=game.stormrailEnemyFormation[i];
        game.stormrailEnemyAge[i]++;
        if(formation==4) {
            game.stormrailEnemyX[i]-=(WORD)(i<4?5:2);
            game.stormrailEnemyY[i]=(WORD)(i<4?
                44+i*30+stormrailTriangle(game.stormrailEnemyAge[i]+i*18,96,52):
                88+stormrailTriangle(game.stormrailEnemyAge[i],112,24));
            game.stormrailEnemyY[i]=(WORD)stormrailClamp(
                game.stormrailEnemyY[i],0,
                STORMRAIL_PLAYFIELD_H-STORMRAIL_ENEMY_H);
        } else if(formation==5) {
            if(game.stormrailEnemyAge[i]<44+i*3) game.stormrailEnemyX[i]-=4;
            else if(game.stormrailEnemyAge[i]<78+i*2) game.stormrailEnemyX[i]+=2;
            else game.stormrailEnemyX[i]-=5;
            game.stormrailEnemyY[i]=(WORD)(38+i*27+
                stormrailTriangle(game.stormrailEnemyAge[i]+i*9,96,70));
            game.stormrailEnemyY[i]=(WORD)stormrailClamp(
                game.stormrailEnemyY[i],0,
                STORMRAIL_PLAYFIELD_H-STORMRAIL_ENEMY_H);
        } else if(formation==6) {
            game.stormrailEnemyX[i]-=(WORD)(i<3?5:2);
            game.stormrailEnemyY[i]=(WORD)(i<3?
                36+i*52+stormrailTriangle(game.stormrailEnemyAge[i]+i*24,112,72):
                58+(i-3)*76+stormrailTriangle(game.stormrailEnemyAge[i]+i*13,96,28));
            game.stormrailEnemyY[i]=(WORD)stormrailClamp(
                game.stormrailEnemyY[i],0,
                STORMRAIL_PLAYFIELD_H-STORMRAIL_ENEMY_H);
        } else if(formation==7) {
            LONG spread=game.stormrailEnemyAge[i]<45?
                game.stormrailEnemyAge[i]/3:
                game.stormrailEnemyAge[i]<90?(90-game.stormrailEnemyAge[i])/3:0;
            game.stormrailEnemyX[i]-=5;
            game.stormrailEnemyY[i]=(WORD)(88+(LONG)(i-2)*spread);
            game.stormrailEnemyY[i]=(WORD)stormrailClamp(
                game.stormrailEnemyY[i],0,
                STORMRAIL_PLAYFIELD_H-STORMRAIL_ENEMY_H);
        } else {
        game.stormrailEnemyX[i]-=(WORD)(game.stormrailEnemyKind[i]?2:4);
        if(formation==2)
            /* Keep both low split lanes wholly inside the 208-line
               playfield. A 32px Bob may start no lower than y=176; the old
               y=176 base plus triangle motion made slot 4 pop in and out. */
            game.stormrailEnemyY[i]=(WORD)((i<3?30+i*28:140+(i-3)*24)+stormrailTriangle(game.stormrailEnemyAge[i]+i*7,72,24));
        else if(formation==3)
            game.stormrailEnemyY[i]=(WORD)(34+i*29+stormrailTriangle(game.stormrailEnemyAge[i]+i*11,88,36));
        else if(game.stormrailEnemyKind[i])
            game.stormrailEnemyY[i]=(WORD)(46+i*30+stormrailTriangle(game.stormrailEnemyAge[i]+i*9,80,46));
        else
            game.stormrailEnemyY[i]=(WORD)(38+i*25+stormrailTriangle(game.stormrailEnemyAge[i]+i*12,64,30));
        }
        if(game.stormrailEnemyKind[i]&&game.stormrailEnemyAge[i]==70+i*12) {
            for(j=0;j<STORMRAIL_MAX_HOSTILE_SHOTS;j++) if(!game.stormrailHostileActive[j]) {
                game.stormrailHostileActive[j]=TRUE;
                game.stormrailHostileKind[j]=0;
                game.stormrailHostileX[j]=game.stormrailEnemyX[i];
                game.stormrailHostileY[j]=(WORD)(game.stormrailEnemyY[i]+12);
                game.stormrailHostileDx[j]=-4;
                game.stormrailHostileDy[j]=0;
                break;
            }
        }
        for(j=0;j<STORMRAIL_MAX_SHOTS;j++) if(
            stormrailEnemyHittable(game.stormrailEnemyX[i])&&
            game.stormrailShotActive[j]&&
            game.stormrailShotX[j]+16>=game.stormrailEnemyX[i]&&
            game.stormrailShotX[j]<=game.stormrailEnemyX[i]+STORMRAIL_ENEMY_W&&
            game.stormrailShotY[j]+8>=game.stormrailEnemyY[i]&&
            game.stormrailShotY[j]<=game.stormrailEnemyY[i]+STORMRAIL_ENEMY_H) {
            game.stormrailShotActive[j]=FALSE;
            if(--game.stormrailEnemyHp[i]==0) {
                ULONG awardBit=1UL<<((UWORD)formation*STORMRAIL_MAX_ENEMIES+i);
                game.stormrailEnemyActive[i]=FALSE;
                /* Match Level 1's compact four-digit score economy: the
                   tougher firing Orb is worth two regular enemy defeats.
                   Like Level 1's spawnState.scoreAwarded, this bit survives
                   a life-loss restart so the same encounter cannot score
                   twice. */
                if(!(game.stormrailEnemyScoreAwarded&awardBit)) {
                    game.stormrailEnemyScoreAwarded|=awardBit;
                    game.score+=game.stormrailEnemyKind[i]?40:20;
                    game.enemiesDefeated++;
                }
                audioPlayEnemyDeath();
                if(--game.stormrailFormationAlive[formation]==0)
                    stormrailQueueFormationReward(formation);
            } else audioPlayEnemyHit();
            break;
        }
        if(game.stormrailEnemyActive[i]&&!game.stormrailInvuln&&
           game.stormrailEnemyX[i]+40>=game.stormrailX+10&&
           game.stormrailEnemyX[i]+8<=game.stormrailX+70&&
           game.stormrailEnemyY[i]+26>=game.stormrailY+4&&
           game.stormrailEnemyY[i]+6<=game.stormrailY+28) {
            if(game.stormrailHealth) game.stormrailHealth--;
            game.stormrailInvuln=STORMRAIL_HIT_INVULN;
            game.stormrailCollisions++;
            game.stormrailX=(WORD)stormrailClamp(game.stormrailX-12,
                STORMRAIL_FLIGHT_MIN_X,STORMRAIL_FLIGHT_MAX_X);
            audioPlayPlayerHurt();
        }
        if(game.stormrailEnemyActive[i]&&game.stormrailEnemyX[i]<-STORMRAIL_ENEMY_W) {
            game.stormrailEnemyActive[i]=FALSE;
            game.stormrailFormationEscaped[formation]=1;
            if(game.stormrailFormationAlive[formation]) game.stormrailFormationAlive[formation]--;
        }
    }
    for(i=0;i<STORMRAIL_MAX_HOSTILE_SHOTS;i++) if(game.stormrailHostileActive[i]) {
        game.stormrailHostileX[i]+=game.stormrailHostileDx[i];
        game.stormrailHostileY[i]+=game.stormrailHostileDy[i];
        if(game.stormrailHostileX[i]<-STORMRAIL_HOSTILE_W||
           game.stormrailHostileY[i]<-STORMRAIL_HOSTILE_H||
           game.stormrailHostileY[i]>=STORMRAIL_PLAYFIELD_H)
            game.stormrailHostileActive[i]=FALSE;
        else if(!game.stormrailInvuln&&game.stormrailHostileX[i]+STORMRAIL_HOSTILE_W>=game.stormrailX+10&&
                game.stormrailHostileX[i]<=game.stormrailX+70&&
                game.stormrailHostileY[i]+STORMRAIL_HOSTILE_H>=game.stormrailY+4&&
                game.stormrailHostileY[i]<=game.stormrailY+28) {
            game.stormrailHostileActive[i]=FALSE;
            if(game.stormrailHealth) game.stormrailHealth--;
            game.stormrailInvuln=STORMRAIL_HIT_INVULN; game.stormrailCollisions++;
            audioPlayPlayerHurt();
        }
    }
    for(i=0;i<STORMRAIL_MAX_REWARDS;i++) if(game.stormrailRewardActive[i]) {
        game.stormrailRewardX[i]-=3;
        if(game.stormrailRewardX[i]<-16) game.stormrailRewardActive[i]=FALSE;
        else if(game.stormrailRewardX[i]+16>=game.stormrailX&&game.stormrailRewardX[i]<=game.stormrailX+80&&
                game.stormrailRewardY[i]+21>=game.stormrailY&&game.stormrailRewardY[i]<=game.stormrailY+32) {
            UBYTE pickupId=game.stormrailRewardId[i];
            game.stormrailRewardActive[i]=FALSE;
            if(!stormrailPickupWasConsumed(pickupId)) {
                stormrailMarkPickupConsumed(pickupId);
                if(game.stormrailRewardKind[i]==STORMRAIL_REWARD_HEART) {
                    game.stormrailHealth=stormrailRestoreHeart(
                        game.stormrailHealth,PLAYER_MAX_HEALTH);
                    audioPlayHealthCollect();
                } else {
                    game.diamonds++; game.diamondsCollected++; game.score+=5;
                    if(game.diamonds>=GAME_DIAMONDS_PER_LIFE&&
                       game.stormrailLifeAwards<2) {
                        game.diamonds-=GAME_DIAMONDS_PER_LIFE;
                        if(game.lives<GAME_MAX_LIVES) game.lives++;
                        game.stormrailLifeAwards++;
                        audioPlayExtraLife();
                    }
                    audioPlayCollect();
                }
            }
        }
    }
    if(stormrailRewardPoolEmpty()) {
        static const ULONG lineDistance[STORMRAIL_FREE_LINE_COUNT]={
            STORMRAIL_FREE_LINE0_DISTANCE,STORMRAIL_FREE_LINE1_DISTANCE,
            STORMRAIL_FREE_LINE2_DISTANCE,STORMRAIL_FREE_LINE3_DISTANCE,
            STORMRAIL_FREE_LINE4_DISTANCE};
        static const UBYTE lineY[STORMRAIL_FREE_LINE_COUNT]={92,42,64,112,68};
        if(!game.stormrailHeartPickupSpawned&&
           game.stormrailDistance>=STORMRAIL_HEART_PICKUP_DISTANCE) {
            static const WORD heartRingX[STORMRAIL_MAX_REWARDS]={346,346,322,370};
            static const WORD heartRingY[STORMRAIL_MAX_REWARDS]={88,58,112,112};
            game.stormrailHeartPickupSpawned=1;
            for(i=0;i<STORMRAIL_MAX_REWARDS;i++) {
                UBYTE pickupId=(UBYTE)(i?STORMRAIL_HEART_RING_FIRST_DIAMOND_ID+
                                           i-1:STORMRAIL_HEART_PICKUP_ID);
                if(stormrailPickupWasConsumed(pickupId)) continue;
                game.stormrailRewardActive[i]=TRUE;
                game.stormrailRewardId[i]=pickupId;
                game.stormrailRewardKind[i]=(UBYTE)(i?
                    STORMRAIL_REWARD_DIAMOND:STORMRAIL_REWARD_HEART);
                game.stormrailRewardX[i]=heartRingX[i];
                game.stormrailRewardY[i]=heartRingY[i];
            }
            i=0;
        } else for(i=0;i<STORMRAIL_FORMATION_COUNT;i++)
            if(game.stormrailFormationRewardPending[i]) {
                WORD rewardY=game.stormrailY;
                WORD maximum=(WORD)(i==0?187:i==1?145:i==2?159:153);
                game.stormrailFormationRewardPending[i]=0;
                rewardY=(WORD)stormrailClamp(rewardY,0,maximum);
                UBYTE rewardId=(UBYTE)(i<4?16+i*4:
                    STORMRAIL_PATTERN_REWARD_FIRST_ID+(i-4)*4);
                stormrailSpawnDiamondLine(rewardId,(UBYTE)(i&3),316,rewardY);
                break;
            }
        if(i==STORMRAIL_FORMATION_COUNT)
            for(i=0;i<STORMRAIL_FREE_LINE_COUNT;i++)
                if(!game.stormrailFreeLineSpawned[i]&&
                   game.stormrailDistance>=lineDistance[i]) {
                    game.stormrailFreeLineSpawned[i]=1;
                    stormrailSpawnDiamondLine((UBYTE)(i<4?i*4:
                        STORMRAIL_GATE5_FREE_FIRST_ID),(UBYTE)(i&3),330,lineY[i]);
                    break;
                }
    }
    if(game.stormrailInvuln) game.stormrailInvuln--;
    if(!game.stormrailHealth) {
        if(game.lives>1) game.lives--;
        else game.lives=GAME_START_LIVES;
        game.stormrailHealth=PLAYER_MAX_HEALTH;
        game.stormrailInvuln=STORMRAIL_HIT_INVULN;
        game.stormrailDistance=0; game.cameraX=0;
        game.stormrailX=STORMRAIL_PLAY_X; game.stormrailY=80;
        for(i=0;i<STORMRAIL_MAX_ENEMIES;i++) game.stormrailEnemyActive[i]=FALSE;
        for(i=0;i<STORMRAIL_MAX_HOSTILE_SHOTS;i++) game.stormrailHostileActive[i]=FALSE;
        for(i=0;i<STORMRAIL_MAX_REWARDS;i++) game.stormrailRewardActive[i]=FALSE;
        for(i=0;i<STORMRAIL_MAX_OBSTACLES;i++)
            game.stormrailObstacleActive[i]=FALSE;
        game.stormrailObstacleSpawnMask=0;
        game.stormrailObstacleSpawnMaskHigh=0;
        game.stormrailDebrisNextEvent=0;
        game.stormrailObstacleBigPeak=0;
        for(i=0;i<STORMRAIL_FORMATION_COUNT;i++) {
            game.stormrailFormationSpawned[i]=0;
            game.stormrailFormationAlive[i]=0;
            game.stormrailFormationEscaped[i]=0;
            game.stormrailFormationRewardPending[i]=0;
        }
        for(i=0;i<STORMRAIL_FREE_LINE_COUNT;i++)
            game.stormrailFreeLineSpawned[i]=0;
        game.stormrailHeartPickupSpawned=0;
    }
}

static void stormrailRetireFinaleFire(void)
{
    UBYTE i;
    for(i=0;i<STORMRAIL_MAX_HOSTILE_SHOTS;i++)
        game.stormrailHostileActive[i]=FALSE;
    for(i=0;i<STORMRAIL_MAX_SHOTS;i++)
        game.stormrailShotActive[i]=FALSE;
}

static void stormrailBeginFinale(void)
{
    UBYTE i;
    game.stormrailFinaleActive=1;
    game.stormrailFinalePhase=STORMRAIL_FINALE_PHASE_ARRIVAL;
    game.stormrailFinaleTick=0;
    game.stormrailFinaleVisualTick=0;
    game.stormrailFinaleGateOpen=0;
    game.stormrailFinaleExitCentered=0;
    game.stormrailFinaleHasFired=0;
    game.stormrailFinaleLastFireTick=0;
    game.stormrailFinaleHostileLaunches=0;
    game.stormrailFinaleFanLaunches=0;
    game.stormrailFinaleHunterLaunches=0;
    game.stormrailFinaleFanCharges=0;
    game.stormrailFinaleHunterCharges=0;
    game.stormrailFinalePhaseMask|=1<<STORMRAIL_FINALE_PHASE_ARRIVAL;
    for(i=0;i<STORMRAIL_FINALE_ACTOR_COUNT;i++) {
        game.stormrailFinaleHp[i]=stormrailFinaleActors[i].hp;
        game.stormrailFinaleFlash[i]=0;
    }
    for(i=0;i<STORMRAIL_MAX_ENEMIES;i++) game.stormrailEnemyActive[i]=FALSE;
    for(i=0;i<STORMRAIL_MAX_REWARDS;i++) game.stormrailRewardActive[i]=FALSE;
    for(i=0;i<STORMRAIL_MAX_OBSTACLES;i++) game.stormrailObstacleActive[i]=FALSE;
    stormrailRetireFinaleFire();
}

static void stormrailSpawnFinaleShot(UBYTE attack)
{
    const struct StormrailFinaleAttackContract *cue=&stormrailFinaleAttacks[attack];
    const struct StormrailFinaleActorContract *actor=&stormrailFinaleActors[cue->actor];
    WORD actorX=stormrailFinaleActorX(game.stormrailFinaleVisualTick,cue->actor);
    WORD actorY=stormrailFinaleActorY(game.stormrailFinaleVisualTick,cue->actor);
    UBYTE i,freeCount=0,volley=0,needed;
    for(i=0;i<STORMRAIL_MAX_HOSTILE_SHOTS;i++)
        if(!game.stormrailHostileActive[i]) freeCount++;
    needed=(UBYTE)(cue->kind==STORMRAIL_FINALE_ATTACK_FAN?
                   STORMRAIL_FINALE_VOLLEY_SHOTS:1);
    /* A partial fan would silently remove its escape geometry. Skip the whole
       authored beat unless all required existing hostile slots are available. */
    if(freeCount<needed) return;
    for(i=0;i<STORMRAIL_MAX_HOSTILE_SHOTS&&
                volley<needed;i++)
        if(!game.stormrailHostileActive[i]) {
            WORD dy=0;
            game.stormrailHostileActive[i]=TRUE;
            game.stormrailHostileKind[i]=(UBYTE)(cue->kind==
                STORMRAIL_FINALE_ATTACK_HUNTER?2:1);
            game.stormrailHostileX[i]=(WORD)(actorX-8);
            game.stormrailHostileY[i]=(WORD)(actorY+actor->height/2-4);
            game.stormrailHostileDx[i]=cue->dx;
            if(cue->kind==STORMRAIL_FINALE_ATTACK_FAN) {
                game.stormrailHostileY[i]+=(WORD)(((WORD)volley-1)*4);
                dy=stormrailFinaleVolleyDy[volley];
            } else {
                WORD target=(WORD)(game.stormrailY+16);
                WORD source=(WORD)(actorY+actor->height/2);
                if(target<source-24) dy=-2;
                else if(target<source-8) dy=-1;
                else if(target>source+24) dy=2;
                else if(target>source+8) dy=1;
            }
            game.stormrailHostileDy[i]=dy;
            volley++;
        }
    game.stormrailFinaleLastFireTick=game.stormrailFinaleTick;
    game.stormrailFinaleHasFired=1;
    game.stormrailFinaleHostileLaunches+=needed;
    if(cue->kind==STORMRAIL_FINALE_ATTACK_HUNTER) {
        game.stormrailFinaleHunterLaunches+=needed;
        audioPlayHarrierHunterFire();
    } else {
        game.stormrailFinaleFanLaunches+=needed;
        audioPlayHarrierFanFire();
    }
}

static void stormrailUpdateFinale(void)
{
    UBYTE i,j;
    if(game.stormrailDistance!=STORMRAIL_SLICE_END_DISTANCE)
        game.stormrailFinaleDistanceViolation=1;
    if(game.stormrailFinaleVisualTick<65535) game.stormrailFinaleVisualTick++;
    if(game.stormrailFinalePhase==STORMRAIL_FINALE_PHASE_ARRIVAL) {
        stormrailRetireFinaleFire();
        game.stormrailFinalePhaseMask|=1<<STORMRAIL_FINALE_PHASE_ARRIVAL;
        if(game.stormrailFinaleTick>=STORMRAIL_FINALE_ARRIVAL_TICKS) {
            game.stormrailFinalePhase=STORMRAIL_FINALE_PHASE_COMBAT;
            game.stormrailFinalePhaseMask|=1<<STORMRAIL_FINALE_PHASE_COMBAT;
            game.stormrailFinaleTick=0;
            return;
        }
    } else if(game.stormrailFinalePhase==STORMRAIL_FINALE_PHASE_COMBAT) {
        for(i=0;i<STORMRAIL_FINALE_ACTOR_COUNT;i++) {
            const struct StormrailFinaleActorContract *actor=&stormrailFinaleActors[i];
            WORD actorX=stormrailFinaleActorX(game.stormrailFinaleVisualTick,i);
            WORD actorY=stormrailFinaleActorY(game.stormrailFinaleVisualTick,i);
            if(game.stormrailFinaleFlash[i]) game.stormrailFinaleFlash[i]--;
            if(!game.stormrailFinaleHp[i]) continue;
            for(j=0;j<STORMRAIL_MAX_SHOTS;j++)
                if(game.stormrailShotActive[j]&&
                   game.stormrailShotX[j]+16>=actorX&&
                   game.stormrailShotX[j]<=actorX+actor->width&&
                   game.stormrailShotY[j]+8>=actorY&&
                   game.stormrailShotY[j]<=actorY+actor->height) {
                    game.stormrailShotActive[j]=FALSE;
                    game.stormrailFinaleFlash[i]=4;
                    if(--game.stormrailFinaleHp[i]==0) {
                        if(stormrailMarkObstacleScoreAwarded(actor->awardId)) {
                            game.score+=actor->score;
                            game.enemiesDefeated++;
                        }
                        audioPlayEnemyDeath();
                    } else audioPlayEnemyHit();
                    break;
                }
            if(stormrailFinalePlayerContact(game.stormrailX,
               game.stormrailY,actorX,actorY,i)) {
                game.stormrailX=(WORD)stormrailClamp(
                    stormrailFinaleContactResolveX(actorX),
                    STORMRAIL_FLIGHT_MIN_X,STORMRAIL_FLIGHT_MAX_X);
                if(!game.stormrailInvuln) {
                    if(game.stormrailHealth>=STORMRAIL_FINALE_CONTACT_DAMAGE)
                        game.stormrailHealth-=STORMRAIL_FINALE_CONTACT_DAMAGE;
                    else game.stormrailHealth=0;
                    game.stormrailInvuln=STORMRAIL_HIT_INVULN;
                    game.stormrailCollisions++;
                    audioPlayPlayerHurt();
                }
            }
        }
        if(stormrailFinaleAllDestroyed(game.stormrailFinaleHp)) {
            game.stormrailFinalePhase=STORMRAIL_FINALE_PHASE_OPENING;
            game.stormrailFinaleTick=0;
            stormrailRetireFinaleFire();
            return;
        }
        for(i=0;i<STORMRAIL_FINALE_ATTACK_COUNT;i++) {
            if(game.stormrailFinaleHp[stormrailFinaleAttacks[i].actor]&&
               stormrailFinaleAttackEnabled(
                   game.stormrailFinaleHp[STORMRAIL_FINALE_HARRIER],i)&&
               stormrailFinaleAttackChargeDue(game.stormrailFinaleTick,i)) {
                if(stormrailFinaleAttacks[i].kind==STORMRAIL_FINALE_ATTACK_HUNTER) {
                    game.stormrailFinaleHunterCharges++;
                    audioPlayHarrierHunterCharge();
                } else {
                    game.stormrailFinaleFanCharges++;
                    audioPlayHarrierFanCharge();
                }
            }
            if(game.stormrailFinaleHp[stormrailFinaleAttacks[i].actor]&&
               stormrailFinaleAttackEnabled(
                   game.stormrailFinaleHp[STORMRAIL_FINALE_HARRIER],i)&&
               stormrailFinaleAttackDue(game.stormrailFinaleTick,i)&&
               stormrailFinaleMayFire(game.stormrailFinaleTick,
                   game.stormrailFinaleLastFireTick,
                   game.stormrailFinaleHasFired))
                stormrailSpawnFinaleShot(i);
        }
        for(i=0;i<STORMRAIL_MAX_HOSTILE_SHOTS;i++)
            if(game.stormrailHostileActive[i]) {
                game.stormrailHostileX[i]+=game.stormrailHostileDx[i];
                game.stormrailHostileY[i]+=game.stormrailHostileDy[i];
                if(game.stormrailHostileX[i]<-STORMRAIL_HOSTILE_W||
                   game.stormrailHostileY[i]<-STORMRAIL_HOSTILE_H||
                   game.stormrailHostileY[i]>=STORMRAIL_PLAYFIELD_H)
                    game.stormrailHostileActive[i]=FALSE;
                else if(!game.stormrailInvuln&&
                   game.stormrailHostileX[i]+STORMRAIL_HOSTILE_W>=game.stormrailX+10&&
                   game.stormrailHostileX[i]<=game.stormrailX+70&&
                   game.stormrailHostileY[i]+STORMRAIL_HOSTILE_H>=game.stormrailY+4&&
                   game.stormrailHostileY[i]<=game.stormrailY+28) {
                    game.stormrailHostileActive[i]=FALSE;
                    if(game.stormrailHealth) game.stormrailHealth--;
                    game.stormrailInvuln=STORMRAIL_HIT_INVULN;
                    game.stormrailCollisions++;
                    audioPlayPlayerHurt();
                }
            }
        if(game.stormrailInvuln) game.stormrailInvuln--;
        if(!game.stormrailHealth) {
            if(game.lives>1) game.lives--;
            else game.lives=GAME_START_LIVES;
            game.stormrailHealth=PLAYER_MAX_HEALTH;
            game.stormrailInvuln=STORMRAIL_HIT_INVULN;
            game.stormrailX=STORMRAIL_FINALE_RESPAWN_X;
            game.stormrailY=STORMRAIL_FINALE_RESPAWN_Y;
            /* This remains one encounter: retain earned boss damage and do
               not replay the first-arrival composition after losing a life.
               Restart the authored fire cycle so respawn cannot coincide
               with an immediate launch. */
            game.stormrailFinalePhase=STORMRAIL_FINALE_PHASE_COMBAT;
            game.stormrailFinaleTick=0;
            game.stormrailFinaleHasFired=0;
            game.stormrailFinaleLastFireTick=0;
            for(i=0;i<STORMRAIL_FINALE_ACTOR_COUNT;i++)
                game.stormrailFinaleFlash[i]=0;
            stormrailRetireFinaleFire();
            return;
        }
    } else if(game.stormrailFinalePhase==STORMRAIL_FINALE_PHASE_OPENING) {
        stormrailRetireFinaleFire();
        game.stormrailFinalePhaseMask|=1<<STORMRAIL_FINALE_PHASE_OPENING;
        if(game.stormrailFinaleTick<STORMRAIL_FINALE_OPEN_TICKS)
            game.stormrailFinaleGateOpen=(UBYTE)game.stormrailFinaleTick;
        else {
            game.stormrailFinaleGateOpen=STORMRAIL_FINALE_OPEN_TICKS;
            game.stormrailFinalePhase=STORMRAIL_FINALE_PHASE_EXIT;
            game.stormrailFinaleTick=0;
            return;
        }
    } else if(game.stormrailFinalePhase==STORMRAIL_FINALE_PHASE_EXIT) {
        stormrailRetireFinaleFire();
        game.stormrailFinalePhaseMask|=1<<STORMRAIL_FINALE_PHASE_EXIT;
        if(!game.stormrailFinaleExitCentered) {
            game.stormrailX=stormrailFinaleApproach(game.stormrailX,
                STORMRAIL_FINALE_CENTER_X,2);
            game.stormrailY=stormrailFinaleApproach(game.stormrailY,
                STORMRAIL_FINALE_CENTER_Y,2);
            if(game.stormrailX==STORMRAIL_FINALE_CENTER_X&&
               game.stormrailY==STORMRAIL_FINALE_CENTER_Y)
                game.stormrailFinaleExitCentered=1;
        } else game.stormrailX+=4;
        if(game.stormrailX>STORMRAIL_FINALE_EXIT_X) {
            game.stormrailFinalePhase=STORMRAIL_FINALE_PHASE_COMPLETE;
            game.stormrailFinalePhaseMask|=
                1<<STORMRAIL_FINALE_PHASE_COMPLETE;
            game.stormrailFinaleTick=0;
            stormrailResultsFinalize(&game.stormrailResults,
                game.enemiesDefeated,game.diamondsCollected,
                game.elapsedFields,game.score);
            return;
        }
    } else {
        stormrailRetireFinaleFire();
        game.stormrailFinalePhaseMask|=1<<STORMRAIL_FINALE_PHASE_COMPLETE;
    }
    if(game.stormrailFinalePhase!=STORMRAIL_FINALE_PHASE_COMBAT)
        for(i=0;i<STORMRAIL_MAX_HOSTILE_SHOTS;i++)
            if(game.stormrailHostileActive[i])
                game.stormrailFinaleHostileAfterCombat=1;
    if(game.stormrailFinaleTick<65535) game.stormrailFinaleTick++;
}
#endif

void gameInit(ULONG enemySeed)
{
    UBYTE stormShot;
    game.cameraX=0; game.frameCounter=0;
    game.lives=GAME_START_LIVES;
    game.diamonds=0;
    game.score=0; game.elapsedFields=0;
    game.enemiesDefeated=0; game.diamondsCollected=0;
    game.coreCollectTimer=0;
    game.enemySeed=enemySeed?enemySeed:0x53504157UL;
    game.extraLifeState=EXTRA_LIFE_HIDDEN;
    game.extraLifeY=EXTRA_LIFE_START_Y;
    game.lastFieldCounter=platformFieldCounter();
#ifdef SPARKPAW_STORMRAIL_PROOF
    game.stormrailActive=
#ifdef SPARKPAW_CAMPAIGN
        game.stormrailActive;
#else
        TRUE;
#endif
    game.stormrailMode=STORMRAIL_MODE_APPROACH;
    game.stormrailX=STORMRAIL_PARK_X;
    game.stormrailY=STORMRAIL_PARK_Y;
    game.stormrailHealth=PLAYER_MAX_HEALTH;
    game.stormrailInvuln=0; game.stormrailCollisions=0;
    game.stormrailBoardTimer=0;
    game.stormrailLaunchTimer=0;
    game.stormrailFade=0;
    game.stormrailShotCooldown=0;
    game.stormrailFireHeld=0;
    game.stormrailFireHoldTicks=0;
    game.stormrailDistance=0;
    game.stormrailPickupConsumed=0;
    game.stormrailPickupConsumedHigh=0;
    game.stormrailHeartPickupSpawned=0;
    game.stormrailEnemyScoreAwarded=0;
    game.stormrailObstacleScoreAwarded=0;
    game.stormrailObstacleScoreAwardedHigh=0;
    game.stormrailObstacleSpawnMask=0;
    game.stormrailObstacleSpawnMaskHigh=0;
    game.stormrailDebrisNextEvent=0;
    game.stormrailLastDebrisSpawnTick=0;
    game.stormrailLastDebrisActiveTick=0;
    game.stormrailLastDebrisSpawnDistance=0;
    game.stormrailLastDebrisActiveDistance=0;
    game.stormrailLastDebrisSpawnId=255;
    game.stormrailLootBlockerId=255;
    game.stormrailLootBlockerAge=0;
    game.stormrailLootBlockerX=0;
    game.stormrailLootBlockedDistance=0;
    game.stormrailObstaclePeak=0;
    game.stormrailObstacleBigPeak=0;
    game.stormrailLifeAwards=0;
    game.stormrailFinaleActive=0;
    game.stormrailFinalePhase=STORMRAIL_FINALE_PHASE_COMBAT;
    game.stormrailFinaleTick=0;
    game.stormrailFinaleVisualTick=0;
    game.stormrailFinaleGateOpen=0;
    game.stormrailFinaleExitCentered=0;
    game.stormrailFinaleHasFired=0;
    game.stormrailFinaleLastFireTick=0;
    game.stormrailFinaleHostileLaunches=0;
    game.stormrailFinaleFanLaunches=0;
    game.stormrailFinaleHunterLaunches=0;
    game.stormrailFinaleFanCharges=0;
    game.stormrailFinaleHunterCharges=0;
    game.stormrailFinalePhaseMask=0;
    game.stormrailFinaleHostileAfterCombat=0;
    game.stormrailFinaleDistanceViolation=0;
    stormrailResultsReset(&game.stormrailResults);
    game.stormrailMinControlledX=32767;
    game.stormrailMaxControlledX=-32768;
    for(stormShot=0;stormShot<STORMRAIL_MAX_SHOTS;stormShot++)
        game.stormrailShotActive[stormShot]=FALSE;
    for(stormShot=0;stormShot<STORMRAIL_MAX_ENEMIES;stormShot++)
        game.stormrailEnemyActive[stormShot]=FALSE;
    for(stormShot=0;stormShot<STORMRAIL_MAX_HOSTILE_SHOTS;stormShot++)
    {
        game.stormrailHostileActive[stormShot]=FALSE;
        game.stormrailHostileKind[stormShot]=0;
        game.stormrailHostileDy[stormShot]=0;
    }
    for(stormShot=0;stormShot<STORMRAIL_MAX_REWARDS;stormShot++)
        game.stormrailRewardActive[stormShot]=FALSE;
    for(stormShot=0;stormShot<STORMRAIL_MAX_OBSTACLES;stormShot++)
        game.stormrailObstacleActive[stormShot]=FALSE;
    for(stormShot=0;stormShot<STORMRAIL_FINALE_ACTOR_COUNT;stormShot++) {
        game.stormrailFinaleHp[stormShot]=stormrailFinaleActors[stormShot].hp;
        game.stormrailFinaleFlash[stormShot]=0;
    }
    for(stormShot=0;stormShot<STORMRAIL_FREE_LINE_COUNT;stormShot++)
        game.stormrailFreeLineSpawned[stormShot]=0;
    for(stormShot=0;stormShot<STORMRAIL_FORMATION_COUNT;stormShot++) {
        game.stormrailFormationSpawned[stormShot]=0;
        game.stormrailFormationAlive[stormShot]=0;
        game.stormrailFormationEscaped[stormShot]=0;
        game.stormrailFormationRewardPending[stormShot]=0;
    }
#ifdef SPARKPAW_STORMRAIL_FINALE_DIRECT_START
    game.stormrailMode=STORMRAIL_MODE_FLIGHT;
    game.stormrailLaunchTimer=STORMRAIL_CONTROL_DELAY;
    game.stormrailDistance=15480UL;
    game.cameraX=stormrailPresentationX(game.stormrailDistance);
    game.stormrailX=STORMRAIL_PLAY_X;
    game.stormrailY=STORMRAIL_FINALE_CENTER_Y;
    game.stormrailDebrisNextEvent=STORMRAIL_DEBRIS_EVENT_COUNT;
    for(stormShot=0;stormShot<STORMRAIL_FORMATION_COUNT;stormShot++)
        game.stormrailFormationSpawned[stormShot]=1;
    for(stormShot=0;stormShot<STORMRAIL_FREE_LINE_COUNT;stormShot++)
        game.stormrailFreeLineSpawned[stormShot]=1;
    game.stormrailHeartPickupSpawned=1;
#endif
#endif
    playerInit(); enemiesInit(game.enemySeed); collectiblesInit(); projectilesInit();
#ifdef SPARKPAW_EXTRA_LIFE_VISUAL_PROOF
    game.cameraX=WORLD_W-SCREEN_W;
    game.extraLifeState=EXTRA_LIFE_DROPPING;
#endif
}

void gameUpdate(void)
{
    ULONG profileStart;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    ULONG detailProfileStart;
#endif
    BOOL left,right,down,jump,fire,wasGrounded;
    WORD playerLeft,playerTop,playerRight,playerBottom,enemyCenterX;
    const struct PlayerState *player=playerState();
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game.stormrailActive) {
    /* One monotone clock covers departure, boarding, the complete route and
       finale. Life-loss route/finale retries deliberately never reset it. */
    accountElapsedFields();
    if(game.stormrailMode==STORMRAIL_MODE_BOARDING) {
        /* Land first, visibly settle into the powered craft, then lift. */
        game.stormrailBoardTimer++;
        if(game.stormrailBoardTimer>=40) {
            if(game.stormrailY>116) game.stormrailY-=2;
        }
        if(game.stormrailBoardTimer>=64) {
            game.stormrailMode=STORMRAIL_MODE_LAUNCH_OUT;
            game.stormrailLaunchTimer=0;
            game.stormrailFade=0;
            /* Renderer stores flight X screen-relative and adds cameraX when
               forming world coordinates. The final boarding frame is already
               screen x=180, so preserve that exact coordinate at handoff. */
            game.stormrailX=STORMRAIL_BOARDING_SCREEN_X;
        }
        audioUpdate(); game.frameCounter++; return;
    }
    if(game.stormrailMode==STORMRAIL_MODE_LAUNCH_OUT) {
        UWORD timer=game.stormrailLaunchTimer;
        if(timer<STORMRAIL_LAUNCH_OUT_FRAMES) {
            WORD step=(WORD)(4+(timer>>2));
            if(step>10) step=10;
            game.stormrailX+=step;
            if(game.cameraX<WORLD_W-SCREEN_W-3) game.cameraX+=3;
        } else {
            UBYTE fade=(UBYTE)(timer-STORMRAIL_LAUNCH_OUT_FRAMES+1);
            if(fade>STORMRAIL_FADE_STEPS) fade=STORMRAIL_FADE_STEPS;
            game.stormrailFade=fade;
        }
        game.stormrailLaunchTimer++;
        if(game.stormrailFade>=STORMRAIL_FADE_STEPS) {
            game.stormrailMode=STORMRAIL_MODE_FLIGHT;
            game.stormrailLaunchTimer=0;
            game.stormrailX=STORMRAIL_REENTRY_X;
            game.stormrailY=80;
            game.stormrailDistance=0;
#ifdef SPARKPAW_STORMRAIL_FINALE_DIRECT_START
            game.stormrailDistance=15480UL;
#endif
            game.stormrailMinControlledX=32767;
            game.stormrailMaxControlledX=-32768;
#ifdef SPARKPAW_STORMRAIL_WRAP_PROOF
            game.stormrailDistance=32720UL;
#endif
            game.cameraX=stormrailPresentationX(game.stormrailDistance);
        }
        audioUpdate(); game.frameCounter++; return;
    }
    if(game.stormrailMode==STORMRAIL_MODE_FLIGHT) {
        BOOL sLeft,sRight,sDown,sUp,sFire;
        UBYTE shot;
        playerReadFlightInput(&sLeft,&sRight,&sUp,&sDown,&sFire);
#ifdef SPARKPAW_STORMRAIL_AUTO_BOARD
        /* The bounded native proof exercises both axes and the real rapid-fire
           pool after the same control handoff used by the user build. */
        sUp=(game.stormrailLaunchTimer>=32&&game.stormrailLaunchTimer<80)||
            (game.stormrailLaunchTimer>=1600&&game.stormrailLaunchTimer<1625);
        sLeft=(game.stormrailLaunchTimer>=80&&game.stormrailLaunchTimer<130)||
              (game.stormrailLaunchTimer>=1500&&game.stormrailLaunchTimer<1540);
        sDown=game.stormrailLaunchTimer>=130&&game.stormrailLaunchTimer<180;
        sRight=game.stormrailLaunchTimer>=180&&game.stormrailLaunchTimer<270;
        /* Leave deterministic quiet windows so whole-display proof captures
           can inspect each formation before the aim sweep destroys it. */
        sFire=(game.stormrailLaunchTimer>=48&&game.stormrailLaunchTimer<120)||
              (game.stormrailLaunchTimer>=210&&game.stormrailLaunchTimer<300)||
              (game.stormrailLaunchTimer>=440&&game.stormrailLaunchTimer<540)||
              (game.stormrailLaunchTimer>=1680&&game.stormrailLaunchTimer<1850);
#ifdef SPARKPAW_STORMRAIL_FINALE_PROOF
        if(game.stormrailFinaleActive&&
           game.stormrailFinalePhase==STORMRAIL_FINALE_PHASE_COMBAT) {
            UBYTE target=STORMRAIL_FINALE_HARRIER;
            WORD wanted=(WORD)(stormrailFinaleActorY(
                               game.stormrailFinaleVisualTick,target)+8-
                               STORMRAIL_FLIGHT_MUZZLE_Y);
            sUp=game.stormrailY>wanted;
            sDown=game.stormrailY<wanted;
            sLeft=FALSE; sRight=FALSE; sFire=TRUE;
        }
#endif
#endif
        /* Saturate only at the storage limit. Cadence gates and later route
           beats must remain able to address Flight ticks beyond 1,000. */
        if(game.stormrailLaunchTimer<65535) game.stormrailLaunchTimer++;
        if(game.stormrailLaunchTimer<=STORMRAIL_FADE_IN_FRAMES)
            game.stormrailFade=(UBYTE)(STORMRAIL_FADE_STEPS-
                game.stormrailLaunchTimer);
        else game.stormrailFade=0;
        if(stormrailReentryCorrectionActive(game.stormrailLaunchTimer,
                                             game.stormrailX)) {
            WORD launchStep=(WORD)(5+(game.stormrailLaunchTimer>>2));
            if(launchStep>10) launchStep=10;
            game.stormrailX+=launchStep;
            if(game.stormrailX>STORMRAIL_PLAY_X)
                game.stormrailX=STORMRAIL_PLAY_X;
        }
        if(game.stormrailLaunchTimer>=STORMRAIL_CONTROL_DELAY&&
           (!game.stormrailFinaleActive||
            stormrailFinalePlayerMayMove(game.stormrailFinalePhase))) {
            if(sLeft) game.stormrailX-=3;
            if(sRight) game.stormrailX+=3;
            if(sUp) game.stormrailY-=3;
            if(sDown) game.stormrailY+=3;
            game.stormrailX=(WORD)stormrailClamp(game.stormrailX,
                STORMRAIL_FLIGHT_MIN_X,STORMRAIL_FLIGHT_MAX_X);
            game.stormrailY=(WORD)stormrailClamp(game.stormrailY,
                STORMRAIL_FLIGHT_MIN_Y,STORMRAIL_FLIGHT_MAX_Y);
            if(game.stormrailX<game.stormrailMinControlledX)
                game.stormrailMinControlledX=game.stormrailX;
            if(game.stormrailX>game.stormrailMaxControlledX)
                game.stormrailMaxControlledX=game.stormrailX;
        }
        {
            LONG scrollStep=1+(game.stormrailLaunchTimer>>4);
            if(scrollStep>4) scrollStep=4;
            /* Flight distance is an encounter clock, not a Level-1 camera.
               The rolling compositor consumes this monotonically increasing
               coordinate while its canonical foreground and rear panorama
               remain bounded resident spans. */
            if(game.stormrailDistance<STORMRAIL_SLICE_END_DISTANCE) {
                game.stormrailDistance+=(ULONG)scrollStep;
                if(game.stormrailDistance>STORMRAIL_SLICE_END_DISTANCE)
                    game.stormrailDistance=STORMRAIL_SLICE_END_DISTANCE;
            }
            game.cameraX=stormrailPresentationX(game.stormrailDistance);
        }
        /* Advance the existing stream before spawning. A fresh pulse must be
           visible on the cyan muzzle for one complete composed frame instead
           of teleporting ten pixels forward on its birth tick. */
        for(shot=0;shot<STORMRAIL_MAX_SHOTS;shot++) {
            if(!game.stormrailShotActive[shot]) continue;
            game.stormrailShotX[shot]+=10;
            if(game.stormrailShotX[shot]>336)
                game.stormrailShotActive[shot]=FALSE;
        }
        if(game.stormrailShotCooldown) game.stormrailShotCooldown--;
        if(!sFire) game.stormrailFireHoldTicks=0;
        else if(game.stormrailFireHeld&&game.stormrailFireHoldTicks<65535)
            game.stormrailFireHoldTicks++;
        if(game.stormrailLaunchTimer>=STORMRAIL_CONTROL_DELAY&&sFire&&
           (!game.stormrailFinaleActive||
            game.stormrailFinalePhase==STORMRAIL_FINALE_PHASE_COMBAT)&&
           !game.stormrailShotCooldown&&
           stormrailFireDue(game.stormrailFireHoldTicks,
                            game.stormrailFireHeld)) {
            for(shot=0;shot<STORMRAIL_MAX_SHOTS;shot++)
                if(!game.stormrailShotActive[shot]) {
                    game.stormrailShotActive[shot]=TRUE;
                    game.stormrailShotX[shot]=(WORD)(game.stormrailX+
                        STORMRAIL_FLIGHT_MUZZLE_X);
                    game.stormrailShotY[shot]=(WORD)(game.stormrailY+
                        STORMRAIL_FLIGHT_MUZZLE_Y);
                    game.stormrailShotCooldown=STORMRAIL_TAP_COOLDOWN;
                    audioPlayShot();
                    break;
                }
        }
        game.stormrailFireHeld=sFire;
#ifdef SPARKPAW_STORMRAIL_AUTO_BOARD
        /* Keep the bounded route proof on one monotone attempt. User-test and
           cadence builds retain the complete health/life restart contract. */
        game.stormrailInvuln=2;
#endif
        if(game.stormrailDistance<STORMRAIL_SLICE_END_DISTANCE) {
            stormrailUpdateEncounters();
            stormrailUpdateObstacles();
        } else {
            if(stormrailFinaleShouldBegin(game.stormrailDistance,
                                          game.stormrailFinaleActive))
                stormrailBeginFinale();
            stormrailUpdateFinale();
        }
        audioUpdate();
        game.frameCounter++;
        return;
    }
    /* The departure reuses accepted Level-1 movement and collision only.
       Level-1 enemies, diamonds, Core and projectiles do not belong to this
       short bridge into the airborne encounter. */
    playerReadInput(&left,&right,&down,&jump,&fire);
    wasGrounded=player->grounded;
    if(playerUpdatePhysics(left,right,down,jump)) audioPlayJump();
    playerUpdateShot();
    updateCamera();
    playerAnimate(!wasGrounded&&player->grounded,game.frameCounter);
    audioUpdate();
    /* Boarding is a physical action: jump onto the open cockpit and descend
       into it. Merely walking nearby or pressing Fire never teleports Paw. */
    if(!player->grounded&&player->vy>0&&
       (player->x>>8)>=184&&(player->x>>8)<=236&&
       (player->y>>8)+38>=136&&(player->y>>8)+38<=172) {
        game.stormrailMode=STORMRAIL_MODE_BOARDING;
        game.stormrailBoardTimer=0;
        game.stormrailX=STORMRAIL_PARK_X;
        game.stormrailY=STORMRAIL_PARK_Y;
    }
#ifdef SPARKPAW_STORMRAIL_AUTO_BOARD
    else if(game.frameCounter>=300) {
        game.stormrailMode=STORMRAIL_MODE_BOARDING;
        game.stormrailBoardTimer=0;
        game.stormrailX=STORMRAIL_PARK_X;
        game.stormrailY=STORMRAIL_PARK_Y;
    }
#endif
    if((player->y>>8)>174) {
        /* Recover before the actor disappears behind the HUD. */
        playerInit();
        audioPlayPlayerHurt();
    }
    game.frameCounter++;
    return;
    }
#endif
    if(!game.coreCollectTimer) accountElapsedFields();
    if(game.coreCollectTimer) {
        audioUpdate();
        updateCamera();
        game.frameCounter++;
        if(game.coreCollectTimer>1) game.coreCollectTimer--;
        return;
    }
    if(game.waterSplashTimer) {
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        detailProfileStart=performanceProfileBegin();
        audioUpdate();
        performanceProfileEnd(PERF_AUDIO_UPDATE,detailProfileStart);
#else
        audioUpdate();
#endif
        game.frameCounter++;
        if(!--game.waterSplashTimer) resetLevelRuntime();
        return;
    }
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    detailProfileStart=performanceProfileBegin();
    playerReadInput(&left,&right,&down,&jump,&fire);
    performanceProfileEnd(PERF_INPUT,detailProfileStart);
#else
    playerReadInput(&left,&right,&down,&jump,&fire);
#endif
    wasGrounded=player->grounded;
    playerStartShot(fire,audioPlayShot);
    profileStart=performanceProfileBegin();
    if(playerUpdatePhysics(left,right,down,jump)) audioPlayJump();
    performanceProfileEnd(PERF_PLAYER,profileStart);
    playerUpdateShot();
    playerContactBounds(&playerLeft,&playerTop,&playerRight,&playerBottom);
    /* Reaching the chamber beyond the Core reveals the secret. Crouching is
       merely how the level geometry admits Sparkpaw, not the trigger itself. */
    if(extraLifeShouldReveal(game.extraLifeState,playerLeft,playerRight)) {
        game.extraLifeState=EXTRA_LIFE_DROPPING;
        game.extraLifeY=EXTRA_LIFE_START_Y;
    }
    if(game.extraLifeState==EXTRA_LIFE_DROPPING) {
        game.extraLifeY=extraLifeDropY(game.extraLifeY);
        if(game.extraLifeY>=EXTRA_LIFE_GROUND_Y) {
            game.extraLifeY=EXTRA_LIFE_GROUND_Y;
            game.extraLifeState=EXTRA_LIFE_READY;
        }
    }
    if(game.extraLifeState==EXTRA_LIFE_READY&&
       extraLifeTouches(playerLeft,playerTop,playerRight,playerBottom,
                        game.extraLifeY)) {
        if(game.lives<GAME_MAX_LIVES) game.lives++;
        game.extraLifeState=EXTRA_LIFE_COLLECTED;
        audioPlayExtraLife();
    }
    if(levelPlayerTouchesWater(playerLeft,playerRight,playerBottom)) {
        if(game.lives>1) game.lives--;
        else game.lives=GAME_START_LIVES;
        game.waterSplashX=(WORD)((playerLeft+playerRight)>>1);
        game.waterSplashTimer=16;
        audioPlayWaterSplash();
        return;
    }
    if(levelPlayerFallsInDryGap(playerLeft,playerRight,playerBottom)) {
        if(game.lives>1) game.lives--;
        else game.lives=GAME_START_LIVES;
        resetLevelRuntime();
        audioUpdate();
        return;
    }
    /* Latch a short readable Core response before the resident replay path.
       A later checkpoint can replace this timer with LEVEL_COMPLETE. */
    if(levelPlayerTouchesStormstoneCore(playerLeft,playerTop,
                                        playerRight,playerBottom)) {
        game.coreCollectTimer=50;
        audioPlayStormstoneCore();
        audioUpdate();
        return;
    }
    profileStart=performanceProfileBegin();
    enemiesUpdate((WORD)game.cameraX,collisionSolidAt,
                  (WORD)((playerLeft+playerRight)>>1),
                  (WORD)((playerTop+playerBottom)>>1),
                  spawnEnemyProjectile);
    performanceProfileEnd(PERF_ENEMIES,profileStart);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    detailProfileStart=performanceProfileBegin();
    left=enemiesContactPlayer(playerLeft,playerTop,playerRight,playerBottom,
                              &enemyCenterX);
    performanceProfileEnd(PERF_ENEMY_CONTACT,detailProfileStart);
    if(left) {
#else
    if(enemiesContactPlayer(playerLeft,playerTop,playerRight,playerBottom,
                            &enemyCenterX)) {
#endif
        if(applyEnemyDamage(enemyCenterX)) return;
    }
    {
        UBYTE picked;
        profileStart=performanceProfileBegin();
        picked=collectiblesCollect(playerLeft,playerTop,
                                         playerRight,playerBottom);
        performanceProfileEnd(PERF_COLLECTIBLES,profileStart);
        if(picked) {
            UWORD total=(UWORD)game.diamonds+picked;
            while(total>=GAME_DIAMONDS_PER_LIFE&&game.lives<GAME_MAX_LIVES) {
                total-=GAME_DIAMONDS_PER_LIFE;
                game.lives++;
            }
            game.diamonds=(UBYTE)(total<GAME_DIAMONDS_PER_LIFE?
                                  total:GAME_DIAMONDS_PER_LIFE-1);
            game.diamondsCollected+=picked;
            game.score+=(ULONG)picked*5UL;
            audioPlayCollect();
        }
    }
    profileStart=performanceProfileBegin();
    projectilesUpdate((WORD)game.cameraX,collisionSolidAt,
                      collisionFirstSolidOnSweep,enemiesHitProjectile,
                      enemiesFirstProjectileHitOnSweep,
                      audioPlayEnemyHit,audioPlayEnemyDeath);
    {
        UWORD award=enemiesConsumeScoreAward();
        if(award) {
            game.score+=award;
            game.enemiesDefeated+=(UWORD)(award/20);
        }
    }
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    detailProfileStart=performanceProfileBegin();
    left=projectilesContactPlayer(playerLeft,playerTop,playerRight,playerBottom,
                                  &enemyCenterX);
    performanceProfileEnd(PERF_PROJECTILE_CONTACT,detailProfileStart);
    if(left&&applyEnemyDamage(enemyCenterX)) {
#else
    if(projectilesContactPlayer(playerLeft,playerTop,playerRight,playerBottom,
                                &enemyCenterX)&&
       applyEnemyDamage(enemyCenterX)) {
#endif
        performanceProfileEnd(PERF_PROJECTILES,profileStart);
        return;
    }
    performanceProfileEnd(PERF_PROJECTILES,profileStart);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    detailProfileStart=performanceProfileBegin();
    audioUpdate();
    performanceProfileEnd(PERF_AUDIO_UPDATE,detailProfileStart);
#else
    audioUpdate();
#endif
    updateCamera();
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    detailProfileStart=performanceProfileBegin();
    playerAnimate(!wasGrounded&&player->grounded,game.frameCounter);
    performanceProfileEnd(PERF_PLAYER_ANIMATE,detailProfileStart);
#else
    playerAnimate(!wasGrounded&&player->grounded,game.frameCounter);
#endif
    game.frameCounter++;
}

const struct GameState *gameState(void)
{
    return &game;
}

BOOL gameLevelComplete(void)
{
#ifdef SPARKPAW_CAMPAIGN
    if(game.stormrailActive)
        return game.stormrailFinaleActive&&
               game.stormrailFinalePhase==STORMRAIL_FINALE_PHASE_COMPLETE&&
               game.stormrailResults.finalized;
    return game.coreCollectTimer==1;
#else
#ifdef SPARKPAW_STORMRAIL_RESULTS_TEST
    return game.stormrailFinaleActive&&
           game.stormrailFinalePhase==STORMRAIL_FINALE_PHASE_COMPLETE&&
           game.stormrailResults.finalized;
#else
    return game.coreCollectTimer==1;
#endif
#endif
}

#ifdef SPARKPAW_STORMRAIL_PROOF
void gameSetStormrailActive(BOOL active) { game.stormrailActive=active; }
BOOL gameStormrailActive(void) { return game.stormrailActive; }
void gameRestoreCampaignVitals(UBYTE lives,UBYTE health,UBYTE diamonds)
{
    game.lives=lives>=1&&lives<=GAME_MAX_LIVES?lives:GAME_START_LIVES;
    game.diamonds=diamonds<GAME_DIAMONDS_PER_LIFE?diamonds:0;
    game.stormrailHealth=health>=1&&health<=PLAYER_MAX_HEALTH?
                         health:PLAYER_MAX_HEALTH;
}
#endif
