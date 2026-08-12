#ifndef SPARKPAW_LEVEL_DATA_H
#define SPARKPAW_LEVEL_DATA_H

#include <exec/types.h>

#define ENEMY_TYPE_CLOCKWORK_BEETLE 0

#define ENEMY_POLICY_PERMANENT 0
#define ENEMY_POLICY_RESPAWN 1

struct EnemySpawnCandidate {
    WORD minX,maxX,y;
    WORD patrolLeft,patrolRight;
    BYTE initialDirection;
    UBYTE type,policy,required;
};

#define MAX_LEVEL_ENEMY_SPAWNS 6

const struct EnemySpawnCandidate *levelEnemySpawnCandidates(UWORD *count);

#endif
