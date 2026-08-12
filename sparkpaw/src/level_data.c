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
    { 336, 344,{ 320, 416,128},  1, ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 },
    { 870, 884,{ 672,1008,208}, -1, ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 },
    {1090,1136,{1072,1200,160},  1, ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,0 }
};

/* Phase 5D traversal proof. This is deliberately authored test-level data,
   not permanent geometry: a later level layout may replace these coordinates
   without changing the Strider state-machine contract. */
static const struct EnemyTraversalLink enemyTraversalLinks[]={
    /* The floor patrol runs beneath the clear x=320..415 start platform and
       stays between the x=288 column face and x=496 low-platform face. Patrol
       bounds describe the inset leading foot, so left=300 keeps the 64px body
       at or right of the column. Broader body-aware navigation remains 5E. */
    { 6,1,360,364,{300,496,208},416,428,256,-1408,64 }
};

const struct EnemySpawnCandidate *levelEnemySpawnCandidates(UWORD *count)
{
    *count=(UWORD)(sizeof(enemySpawns)/sizeof(enemySpawns[0]));
    return enemySpawns;
}


const struct EnemyTraversalLink *levelEnemyTraversalLinks(UWORD *count)
{
    *count=(UWORD)(sizeof(enemyTraversalLinks)/sizeof(enemyTraversalLinks[0]));
    return enemyTraversalLinks;
}
