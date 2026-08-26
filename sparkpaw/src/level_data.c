#include "level_data.h"

static const struct EnemyPatrolSurface enemySurfaces[ENEMY_SURFACE_COUNT]={
    { 286, 392,200}, { 416, 492,200}, { 500, 640,200},
    { 752, 900,200}, { 912,1000,200}, {1035,1195,200},
    { 320, 416,128}, { 672,1008,200}, {1084,1188,160},
    { 300, 496,200}, { 860, 980,112},
    {1300,1420,200}, {1472,1568,128}, {1668,1760,176}, {1840,1936,144},
    {2048,2112,144}, {2176,2288,176}, {2512,2640,144}, {2928,3072,160},
    {2048,2112,144}, {2176,2288,176},
    {2320,2416,160}, {2512,2640,144},
    {2656,2784,176}, {2928,3072,160}
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
    { 870, 884, 1,SURFACE_STRIDER_GAP_HIGH,
                         ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 },
    {1090,1136, 1,SURFACE_STRIDER_OPTIONAL_RAISED,
                         ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 },
    {1320,1380, 1,SURFACE_BEETLE_EXTENDED_A,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    {1490,1536,-1,SURFACE_BEETLE_EXTENDED_B,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    {1688,1736, 1,SURFACE_BEETLE_EXTENDED_C,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    {1860,1904,-1,SURFACE_BEETLE_EXTENDED_D,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,0 },
    {2052,2080, 1,SURFACE_BEETLE_EXTENDED_E,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    {2208,2240,-1,SURFACE_BEETLE_EXTENDED_F,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    {2544,2592, 1,SURFACE_BEETLE_EXTENDED_G,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,1 },
    {2992,3040,-1,SURFACE_BEETLE_EXTENDED_H,ENEMY_TYPE_CLOCKWORK_BEETLE,
                                            ENEMY_POLICY_RESPAWN,0 },

    /* Phase 6C adds four required persistent Striders. Their authored routes
       cross both water openings and both dry chasms in both directions. */
    {1500,1520, 1,SURFACE_BEETLE_EXTENDED_B,
                         ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 },
    {2232,2252,-1,SURFACE_STRIDER_DRY0_RIGHT,
                         ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 },
    {2352,2372, 1,SURFACE_STRIDER_WATER1_LEFT,
                         ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 },
    {2960,2980,-1,SURFACE_STRIDER_DRY1_RIGHT,
                         ENEMY_TYPE_CLOCKWORK_STORM_STRIDER,
                                            ENEMY_POLICY_RESPAWN,1 }
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
     -1,440,444,376,392,-512,-2656,128 },
    /* Phase 5E.5 gap proof for Strider 2. The high platform ends at x=992 and
       the lower platform starts at x=1072, leaving 80px of visible open air.
       This right-edge route stays in the same camera view as Sparkpaw's normal
       approach, unlike the rejected remote-left-edge proof. */
    { SURFACE_STRIDER_GAP_HIGH,SURFACE_STRIDER_OPTIONAL_RAISED,
      1,928,932,1072,1080,1024,-832,64 },
    /* Repeat across the same gap so offscreen persistence cannot consume the
       only observable proof before the camera arrives. */
    { SURFACE_STRIDER_OPTIONAL_RAISED,SURFACE_STRIDER_GAP_HIGH,
     -1,1072,1076,928,936,-1024,-1472,64 }
    ,
    /* Water 0: high left ruin to lower right bank and back. */
    { SURFACE_BEETLE_EXTENDED_B,SURFACE_BEETLE_EXTENDED_C,
      1,1520,1524,1664,1672,1024,-832,64 },
    { SURFACE_BEETLE_EXTENDED_C,SURFACE_BEETLE_EXTENDED_B,
     -1,1668,1672,1520,1528,-1024,-1472,64 },
    /* Dry chasm 0: broken bridge with a lower destination court. */
    { SURFACE_STRIDER_DRY0_LEFT,SURFACE_STRIDER_DRY0_RIGHT,
      1,2056,2060,2176,2184,1024,-720,64 },
    { SURFACE_STRIDER_DRY0_RIGHT,SURFACE_STRIDER_DRY0_LEFT,
     -1,2180,2184,2052,2060,-1024,-1280,64 },
    /* Water 1: high approach to the broad right-hand ruin. */
    { SURFACE_STRIDER_WATER1_LEFT,SURFACE_STRIDER_WATER1_RIGHT,
      1,2368,2372,2512,2520,1024,-1312,64 },
    { SURFACE_STRIDER_WATER1_RIGHT,SURFACE_STRIDER_WATER1_LEFT,
     -1,2516,2520,2368,2376,-1024,-1088,64 },
    /* Dry chasm 1: lower left court to the final portal and return. */
    { SURFACE_STRIDER_DRY1_LEFT,SURFACE_STRIDER_DRY1_RIGHT,
      1,2720,2724,2928,2936,1024,-1792,64 },
    { SURFACE_STRIDER_DRY1_RIGHT,SURFACE_STRIDER_DRY1_LEFT,
     -1,2932,2936,2720,2728,-1024,-1600,64 }
};

static const WORD waterLeft[LEVEL_WATER_COUNT]={
    LEVEL_WATER0_LEFT,LEVEL_WATER1_LEFT
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

BOOL levelWaterColumnAt(WORD x)
{
    UBYTE index;
    for(index=0;index<LEVEL_WATER_COUNT;index++)
        if(x>=waterLeft[index]&&x<waterLeft[index]+LEVEL_WATER_W) return TRUE;
    return FALSE;
}

BOOL levelPlayerTouchesStormstoneCore(WORD left,WORD top,WORD right,WORD bottom)
{
    WORD center=(WORD)((left+right)>>1);
    return center>=LEVEL_STORMSTONE_CORE_LEFT&&
           center<=LEVEL_STORMSTONE_CORE_RIGHT&&
           bottom>=LEVEL_STORMSTONE_CORE_TOP&&
           top<=LEVEL_STORMSTONE_CORE_BOTTOM;
}

WORD levelWaterLeft(UBYTE index)
{
    return index<LEVEL_WATER_COUNT?waterLeft[index]:-1;
}

BOOL levelHazardColumnAt(WORD x)
{
    return levelWaterColumnAt(x)||
           (x>=LEVEL_DRY_GAP0_LEFT&&x<=LEVEL_DRY_GAP0_RIGHT)||
           (x>=LEVEL_DRY_GAP1_LEFT&&x<=LEVEL_DRY_GAP1_RIGHT);
}

BOOL levelPlayerInWater(WORD left,WORD right,WORD bottom)
{
    WORD center=(WORD)((left+right)>>1);
    return levelWaterColumnAt(center)&&bottom>=LEVEL_WATER_DEATH_Y;
}

BOOL levelPlayerTouchesWater(WORD left,WORD right,WORD bottom)
{
    WORD center=(WORD)((left+right)>>1);
    return levelWaterColumnAt(center)&&bottom>=LEVEL_WATER_SPLASH_Y;
}

BOOL levelPlayerFallsInDryGap(WORD left,WORD right,WORD bottom)
{
    WORD center=(WORD)((left+right)>>1);
    return bottom>=LEVEL_WATER_DEATH_Y&&
           ((center>=LEVEL_DRY_GAP0_LEFT&&center<=LEVEL_DRY_GAP0_RIGHT)||
            (center>=LEVEL_DRY_GAP1_LEFT&&center<=LEVEL_DRY_GAP1_RIGHT));
}
