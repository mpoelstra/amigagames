#ifndef SPARKPAW_LEVEL_DATA_H
#define SPARKPAW_LEVEL_DATA_H

#include <exec/types.h>

#define ENEMY_TYPE_CLOCKWORK_BEETLE 0
#define ENEMY_TYPE_CLOCKWORK_STORM_STRIDER 1
#define ENEMY_TYPE_COUNT 2

#define ENEMY_POLICY_PERMANENT 0
#define ENEMY_POLICY_RESPAWN 1

#define SURFACE_BEETLE_FLOOR_A 0
#define SURFACE_BEETLE_FLOOR_B 1
#define SURFACE_BEETLE_FLOOR_C 2
#define SURFACE_BEETLE_FLOOR_D 3
#define SURFACE_BEETLE_FLOOR_E 4
#define SURFACE_BEETLE_FLOOR_F 5
#define SURFACE_STRIDER_RAISED_START 6
#define SURFACE_STRIDER_LONG_FLOOR 7
#define SURFACE_STRIDER_OPTIONAL_RAISED 8
#define SURFACE_STRIDER_JUMP_FLOOR 9
#define SURFACE_STRIDER_GAP_HIGH 10
#define SURFACE_BEETLE_EXTENDED_A 11
#define SURFACE_BEETLE_EXTENDED_B 12
#define SURFACE_BEETLE_EXTENDED_C 13
#define SURFACE_BEETLE_EXTENDED_D 14
#define SURFACE_BEETLE_EXTENDED_E 15
#define SURFACE_BEETLE_EXTENDED_F 16
#define SURFACE_BEETLE_EXTENDED_G 17
#define SURFACE_BEETLE_EXTENDED_H 18
#define SURFACE_STRIDER_DRY0_LEFT 19
#define SURFACE_STRIDER_DRY0_RIGHT 20
#define SURFACE_STRIDER_WATER1_LEFT 21
#define SURFACE_STRIDER_WATER1_RIGHT 22
#define SURFACE_STRIDER_DRY1_LEFT 23
#define SURFACE_STRIDER_DRY1_RIGHT 24
#define ENEMY_SURFACE_COUNT 25
#define INVALID_SURFACE_ID 255

struct EnemyPatrolSurface {
    WORD left,right,groundY;
};

struct EnemySpawnCandidate {
    WORD minX,maxX;
    BYTE initialDirection;
    UBYTE surfaceId,type,policy,required;
};

struct EnemyTraversalLink {
    UBYTE sourceSurfaceId,destinationSurfaceId;
    BYTE launchDirection;
    WORD launchLeft,launchRight;
    WORD landingLeft,landingRight;
    LONG launchVX,launchVY;
    WORD gravity;
};

#define MAX_LEVEL_ENEMY_SPAWNS 24

#define LEVEL_WATER_COUNT 2
#define LEVEL_WATER_W 80
#define LEVEL_WATER0_LEFT 1584
#define LEVEL_WATER1_LEFT 2432
#define LEVEL_DRY_GAP0_LEFT 2112
#define LEVEL_DRY_GAP0_RIGHT 2175
#define LEVEL_DRY_GAP1_LEFT 2784
#define LEVEL_DRY_GAP1_RIGHT 2863
#define LEVEL_WATER_DEATH_Y 224
#define LEVEL_WATER_SPLASH_Y 204
#define LEVEL_FLOOR_Y 200

/* Phase 6C.2 quiet reward field. The animated Core Bob is centred in
   the final 320px camera composition. */
#define LEVEL_STORMSTONE_CORE_CENTER_X 3232
#define LEVEL_STORMSTONE_CORE_LEFT 3198
#define LEVEL_STORMSTONE_CORE_TOP 112
#define LEVEL_STORMSTONE_CORE_RIGHT 3208
#define LEVEL_STORMSTONE_CORE_BOTTOM 174

const struct EnemySpawnCandidate *levelEnemySpawnCandidates(UWORD *count);
const struct EnemyPatrolSurface *levelEnemyPatrolSurface(UBYTE surfaceId);
const struct EnemyTraversalLink *levelEnemyTraversalLinks(UWORD *count);
BOOL levelWaterColumnAt(WORD x);
WORD levelWaterLeft(UBYTE index);
BOOL levelHazardColumnAt(WORD x);
BOOL levelPlayerInWater(WORD left,WORD right,WORD bottom);
BOOL levelPlayerTouchesWater(WORD left,WORD right,WORD bottom);
BOOL levelPlayerFallsInDryGap(WORD left,WORD right,WORD bottom);
BOOL levelPlayerTouchesStormstoneCore(WORD left,WORD top,WORD right,WORD bottom);

#endif
