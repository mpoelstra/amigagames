#ifndef SPARKPAW_GAME_H
#define SPARKPAW_GAME_H

#include <exec/types.h>
#include "extra_life_contract.h"
#ifdef SPARKPAW_STORMRAIL_PROOF
#include "stormrail_contract.h"
#include "stormrail_results_contract.h"
#endif

struct GameState {
    LONG cameraX;
    LONG frameCounter;
    ULONG enemySeed;
    ULONG score;
    ULONG elapsedFields;
    ULONG lastFieldCounter;
    UWORD enemiesDefeated;
    UWORD diamondsCollected;
    UBYTE lives;
    UBYTE diamonds;
    UBYTE waterSplashTimer;
    UBYTE coreCollectTimer;
    UBYTE extraLifeState;
    WORD extraLifeY;
    WORD waterSplashX;
#ifdef SPARKPAW_STORMRAIL_PROOF
#define STORMRAIL_MAX_SHOTS STORMRAIL_MAX_PLAYER_SHOTS
    UBYTE stormrailActive;
    UBYTE stormrailMode;
    UBYTE stormrailEnemyType;
    UBYTE stormrailHealth;
    UBYTE stormrailInvuln;
    UBYTE stormrailCollisions;
    UBYTE stormrailBoardTimer;
    UWORD stormrailLaunchTimer;
    UBYTE stormrailFade;
    UBYTE stormrailShotCooldown;
    UBYTE stormrailFireHeld;
    UWORD stormrailFireHoldTicks;
    ULONG stormrailDistance;
    BOOL stormrailShotActive[STORMRAIL_MAX_SHOTS];
    BOOL stormrailEnemyActive[STORMRAIL_MAX_ENEMIES];
    BOOL stormrailHostileActive[STORMRAIL_MAX_HOSTILE_SHOTS];
    BOOL stormrailRewardActive[STORMRAIL_MAX_REWARDS];
    UBYTE stormrailRewardId[STORMRAIL_MAX_REWARDS];
    UBYTE stormrailRewardKind[STORMRAIL_MAX_REWARDS];
    UBYTE stormrailHeartPickupSpawned;
    UBYTE stormrailFreeLineSpawned[STORMRAIL_FREE_LINE_COUNT];
    UBYTE stormrailFormationRewardPending[STORMRAIL_FORMATION_COUNT];
    UBYTE stormrailLifeAwards;
    ULONG stormrailPickupConsumed;
    ULONG stormrailPickupConsumedHigh;
    ULONG stormrailEnemyScoreAwarded;
    ULONG stormrailObstacleScoreAwarded;
    ULONG stormrailObstacleScoreAwardedHigh;
    BOOL stormrailObstacleActive[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailObstacleType[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailObstacleFrame[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailObstacleBaseFrame[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailObstacleAnimFrames[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailObstacleHp[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailObstacleAge[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailObstacleId[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailObstacleDestructible[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailObstacleDropId[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailObstacleFlash[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailObstaclePeak;
    UBYTE stormrailObstacleBigPeak;
    ULONG stormrailObstacleSpawnMask;
    ULONG stormrailObstacleSpawnMaskHigh;
    UBYTE stormrailDebrisNextEvent;
    UWORD stormrailLastDebrisSpawnTick;
    UWORD stormrailLastDebrisActiveTick;
    ULONG stormrailLastDebrisSpawnDistance;
    ULONG stormrailLastDebrisActiveDistance;
    UBYTE stormrailLastDebrisSpawnId;
    UBYTE stormrailLootBlockerId;
    UBYTE stormrailLootBlockerAge;
    WORD stormrailLootBlockerX;
    ULONG stormrailLootBlockedDistance;
    WORD stormrailObstacleX[STORMRAIL_MAX_OBSTACLES];
    WORD stormrailObstacleY[STORMRAIL_MAX_OBSTACLES];
    WORD stormrailObstacleDx[STORMRAIL_MAX_OBSTACLES];
    WORD stormrailObstacleDy[STORMRAIL_MAX_OBSTACLES];
    UBYTE stormrailEnemyKind[STORMRAIL_MAX_ENEMIES];
    UBYTE stormrailEnemyFormation[STORMRAIL_MAX_ENEMIES];
    UBYTE stormrailEnemyHp[STORMRAIL_MAX_ENEMIES];
    UBYTE stormrailFormationSpawned[STORMRAIL_FORMATION_COUNT];
    UBYTE stormrailFormationAlive[STORMRAIL_FORMATION_COUNT];
    UBYTE stormrailFormationEscaped[STORMRAIL_FORMATION_COUNT];
    UWORD stormrailEnemyAge[STORMRAIL_MAX_ENEMIES];
    WORD stormrailX,stormrailY;
    WORD stormrailMinControlledX,stormrailMaxControlledX;
    WORD stormrailShotX[STORMRAIL_MAX_SHOTS];
    WORD stormrailShotY[STORMRAIL_MAX_SHOTS];
    WORD stormrailEnemyX[STORMRAIL_MAX_ENEMIES],stormrailEnemyY[STORMRAIL_MAX_ENEMIES];
    WORD stormrailHostileX[STORMRAIL_MAX_HOSTILE_SHOTS],stormrailHostileY[STORMRAIL_MAX_HOSTILE_SHOTS];
    WORD stormrailHostileDx[STORMRAIL_MAX_HOSTILE_SHOTS];
    WORD stormrailHostileDy[STORMRAIL_MAX_HOSTILE_SHOTS];
    UBYTE stormrailHostileKind[STORMRAIL_MAX_HOSTILE_SHOTS];
    WORD stormrailRewardX[STORMRAIL_MAX_REWARDS],stormrailRewardY[STORMRAIL_MAX_REWARDS];
    UBYTE stormrailFinaleActive;
    UBYTE stormrailFinalePhase;
    UWORD stormrailFinaleTick;
    UWORD stormrailFinaleVisualTick;
    UBYTE stormrailFinaleGateOpen;
    UBYTE stormrailFinaleExitCentered;
    UBYTE stormrailFinaleHp[STORMRAIL_FINALE_ACTOR_COUNT];
    UBYTE stormrailFinaleFlash[STORMRAIL_FINALE_ACTOR_COUNT];
    ULONG stormrailFinaleLastFireTick;
    UBYTE stormrailFinaleHasFired;
    UWORD stormrailFinaleHostileLaunches;
    UWORD stormrailFinaleFanLaunches;
    UWORD stormrailFinaleHunterLaunches;
    UWORD stormrailFinaleFanCharges;
    UWORD stormrailFinaleHunterCharges;
    UBYTE stormrailFinalePhaseMask;
    UBYTE stormrailFinaleHostileAfterCombat;
    UBYTE stormrailFinaleDistanceViolation;
    struct StormrailResultsSnapshot stormrailResults;
#endif
};

#define GAME_START_LIVES 3
#define GAME_MAX_LIVES 9
#define GAME_DIAMONDS_PER_LIFE 50

void gameInit(ULONG enemySeed);
#ifdef SPARKPAW_STORMRAIL_PROOF
void gameSetStormrailActive(BOOL active);
BOOL gameStormrailActive(void);
void gameRestoreCampaignVitals(UBYTE lives,UBYTE health,UBYTE diamonds);
#endif
void gameUpdate(void);
BOOL gameLevelComplete(void);
const struct GameState *gameState(void);

#endif
