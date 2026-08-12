#include "level_data.h"

static const struct EnemySpawnCandidate enemySpawns[]={
    /* Four required encounters plus two optional floor-safe variations.
       Ranges stay clear of the authored columns at x=256, 640 and 1008. */
    { 300, 344,{ 286, 392,208},  1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    { 420, 452,{ 416, 492,208}, -1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,0 },
    { 535, 592,{ 500, 640,208}, -1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    { 790, 856,{ 752, 900,208},  1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    { 920, 960,{ 912,1000,208},  1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,0 },
    {1080,1152,{1035,1195,208}, -1, ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },

    /* Phase 5 authored Strider surfaces. These two required and one optional
       encounter are persistent level data, but remain runtime-disabled until
       the type-specific packed Bob consumer is introduced in Phase 5C.2. */
    { 336, 344,{ 328, 408,128},  1, ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 },
    { 870, 884,{ 864, 952,208}, -1, ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 },
    {1090,1136,{1072,1200,160},  1, ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,0 }
};

const struct EnemySpawnCandidate *levelEnemySpawnCandidates(UWORD *count)
{
    *count=(UWORD)(sizeof(enemySpawns)/sizeof(enemySpawns[0]));
    return enemySpawns;
}
