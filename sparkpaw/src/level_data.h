#ifndef SPARKPAW_LEVEL_DATA_H
#define SPARKPAW_LEVEL_DATA_H

#include <exec/types.h>

#define ENEMY_TYPE_CLOCKWORK_BEETLE 0
#define ENEMY_TYPE_CLOCKWORK_STORM_STRIDER 1
#define ENEMY_TYPE_COUNT 2

#define ENEMY_POLICY_PERMANENT 0
#define ENEMY_POLICY_RESPAWN 1

struct EnemyPatrolSurface {
    WORD left,right,groundY;
};

struct EnemySpawnCandidate {
    WORD minX,maxX;
    struct EnemyPatrolSurface surface;
    BYTE initialDirection;
    UBYTE type,policy,required;
};

struct EnemyTraversalLink {
    UBYTE spawnIndex;
    BYTE launchDirection;
    WORD launchLeft,launchRight;
    struct EnemyPatrolSurface destination;
    WORD landingLeft,landingRight;
    LONG launchVX,launchVY;
    WORD gravity;
};

#define MAX_LEVEL_ENEMY_SPAWNS 9

const struct EnemySpawnCandidate *levelEnemySpawnCandidates(UWORD *count);
const struct EnemyTraversalLink *levelEnemyTraversalLinks(UWORD *count);

#endif
