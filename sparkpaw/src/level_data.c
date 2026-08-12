#include "level_data.h"

static const struct EnemySpawnCandidate enemySpawns[]={
    /* Four required encounters plus two optional floor-safe variations.
       Ranges stay clear of the authored columns at x=256, 640 and 1008. */
    { 300, 344,184, 286, 392,  1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                      ENEMY_POLICY_RESPAWN,1 },
    { 420, 452,184, 416, 492, -1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                      ENEMY_POLICY_RESPAWN,0 },
    { 535, 592,184, 500, 640, -1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                      ENEMY_POLICY_RESPAWN,1 },
    { 790, 856,184, 752, 900,  1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                      ENEMY_POLICY_RESPAWN,1 },
    { 920, 960,184, 912,1000,  1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                      ENEMY_POLICY_RESPAWN,0 },
    {1080,1152,184,1035,1195, -1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                      ENEMY_POLICY_RESPAWN,1 }
};

const struct EnemySpawnCandidate *levelEnemySpawnCandidates(UWORD *count)
{
    *count=(UWORD)(sizeof(enemySpawns)/sizeof(enemySpawns[0]));
    return enemySpawns;
}
