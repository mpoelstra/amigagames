#include "level_data.h"

static const struct EnemyPatrolSurface enemySurfaces[ENEMY_SURFACE_COUNT]={
    { 286, 392,208}, { 416, 492,208}, { 500, 640,208},
    { 752, 900,208}, { 912,1000,208}, {1035,1195,208},
    { 320, 416,128}, { 672,1008,208}, {1072,1200,160},
    { 300, 496,208}
};

static const struct EnemySpawnCandidate enemySpawns[]={
    /* Four required encounters plus two optional floor-safe variations.
       Ranges stay clear of the authored columns at x=256, 640 and 1008. */
    { 300, 344, 1,SURFACE_BEETLE_FLOOR_A,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    { 420, 452,-1,SURFACE_BEETLE_FLOOR_B,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,0 },
    { 535, 592,-1,SURFACE_BEETLE_FLOOR_C,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    { 790, 856, 1,SURFACE_BEETLE_FLOOR_D,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    { 920, 960, 1,SURFACE_BEETLE_FLOOR_E,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,0 },
    {1080,1152,-1,SURFACE_BEETLE_FLOOR_F,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },

    /* Two required Striders are runtime-enabled. The optional third remains
       authored here but gated until its simultaneous-load behaviour is tested. */
    { 336, 344, 1,SURFACE_STRIDER_RAISED_START,
                         ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 },
    { 870, 884,-1,SURFACE_STRIDER_LONG_FLOOR,
                         ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 },
    {1090,1136, 1,SURFACE_STRIDER_OPTIONAL_RAISED,
                         ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,0 }
};

/* Phase 5D/5E traversal proof. This is deliberately authored test-level data,
   not permanent geometry: a later level layout may replace these coordinates
   without changing the Strider state-machine contract. */
static const struct EnemyTraversalLink enemyTraversalLinks[]={
    /* The floor patrol runs beneath the clear x=320..415 start platform and
       stays between the x=288 column face and x=496 low-platform face. Patrol
       bounds describe the inset leading foot, so left=300 keeps the 64px body
       at or right of the column. Broader body-aware navigation remains 5E. */
    { SURFACE_STRIDER_RAISED_START,SURFACE_STRIDER_JUMP_FLOOR,
      1,360,364,416,428,256,-1408,64 },
    /* Return only while travelling left after the safe x=496 floor turn. The
       steeper arc clears the raised platform's right face before moving above
       it, then lands well inside the original authored surface. */
    { SURFACE_STRIDER_JUMP_FLOOR,SURFACE_STRIDER_RAISED_START,
     -1,440,444,376,392,-512,-2656,128 }
};

const struct EnemySpawnCandidate *levelEnemySpawnCandidates(UWORD *count)
{
    *count=(UWORD)(sizeof(enemySpawns)/sizeof(enemySpawns[0]));
    return enemySpawns;
}

const struct EnemyPatrolSurface *levelEnemyPatrolSurface(UBYTE surfaceId)
{
    return surfaceId<ENEMY_SURFACE_COUNT?&enemySurfaces[surfaceId]:NULL;
}


const struct EnemyTraversalLink *levelEnemyTraversalLinks(UWORD *count)
{
    *count=(UWORD)(sizeof(enemyTraversalLinks)/sizeof(enemyTraversalLinks[0]));
    return enemyTraversalLinks;
}
