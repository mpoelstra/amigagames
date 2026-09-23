#ifndef SPARKPAW_PERFORMANCE_PROFILE_H
#define SPARKPAW_PERFORMANCE_PROFILE_H

#include <dos/dos.h>
#include <exec/types.h>

enum PerformanceProfileSlot {
    PERF_GAME_UPDATE,
    PERF_COPPER_PATCH,
    PERF_BOB_PASS,
    PERF_PUBLISH_WAIT,
    PERF_PLAYER,
    PERF_ENEMIES,
    PERF_COLLECTIBLES,
    PERF_PROJECTILES,
    PERF_INPUT,
    PERF_PLAYER_ANIMATE,
    PERF_ENEMY_CONTACT,
    PERF_PROJECTILE_CONTACT,
    PERF_AUDIO_UPDATE,
    PERF_COPPER_COPY,
    PERF_SPRITE_STAGE,
    PERF_HUD_UPDATE,
    PERF_SCROLL_PATCH,
    PERF_RING_ROLL,
    PERF_RING_DYNAMIC,
    PERF_BOB_PROJECTILE_RESTORE,
    PERF_BOB_ENEMY_RESTORE,
    PERF_BOB_COLLECTIBLE_RESTORE,
    PERF_BOB_SPLASH_RESTORE,
    PERF_BOB_WATER,
    PERF_BOB_COMPACT_TARGET,
    PERF_BOB_SPLASH_DRAW,
    PERF_BOB_COLLECTIBLE_DRAW,
    PERF_BOB_ENEMY_DRAW,
    PERF_BOB_PROJECTILE_DRAW,
    PERF_BOB_FINAL_WAIT,
    PERF_ENEMY_PARKED,
    PERF_ENEMY_ACTIVE,
    PERF_ENEMY_RESPAWN,
    PERF_ENEMY_ACTIVATE,
    PERF_BLITTER_WAIT,
    PERF_STORMRAIL_RESTORE,
    PERF_STORMRAIL_DRAW,
#if defined(SPARKPAW_DROWNED_TARGETED_PROFILE) || defined(SPARKPAW_DROWNED_FERRY_PROFILE)
    PERF_DROWNED_PATCH_BUILD,
    PERF_DROWNED_PATCH_SYNC,
#endif
    PERF_SLOT_COUNT
};

#if defined(SPARKPAW_RENDER_DIAGNOSTIC) && \
    !defined(SPARKPAW_MINIMAL_CADENCE_DIAGNOSTIC)
ULONG performanceProfileBegin(void);
void performanceProfileEnd(enum PerformanceProfileSlot slot,ULONG start);
void performanceProfileWrite(BPTR file);
#elif defined(SPARKPAW_RENDER_DIAGNOSTIC) && \
      !defined(SPARKPAW_PERFORMANCE_PROFILE_IMPLEMENTATION)
#define performanceProfileBegin() 0UL
#define performanceProfileEnd(slot,start) do { } while(0)
void performanceProfileWrite(BPTR file);
#elif defined(SPARKPAW_RENDER_DIAGNOSTIC)
ULONG performanceProfileBegin(void);
void performanceProfileEnd(enum PerformanceProfileSlot slot,ULONG start);
void performanceProfileWrite(BPTR file);
#else
#define performanceProfileBegin() 0UL
#define performanceProfileEnd(slot,start) do { } while(0)
#endif

/* Explicit, disjoint scopes only; broad and nested profiling stays disabled.
   Parenthesized declarations/calls bypass the minimal function-like macros. */
#ifdef SPARKPAW_DROWNED_FERRY_PROFILE
ULONG (performanceProfileBegin)(void);
void (performanceProfileEnd)(enum PerformanceProfileSlot slot,ULONG start);
void performanceFerryFrame(WORD playerX,WORD playerY);
BOOL performanceFerrySelected(enum PerformanceProfileSlot slot);
#define DROWNED_MEASURE(slot,call) do { \
    if(performanceFerrySelected(slot)) { \
        ULONG ferryStart=(performanceProfileBegin)(); \
        call; (performanceProfileEnd)(slot,ferryStart); \
    } else { call; } \
} while(0)
#elif defined(SPARKPAW_DROWNED_TARGETED_PROFILE)
#if !defined(SPARKPAW_DROWNED_SLICE) || !defined(SPARKPAW_MINIMAL_CADENCE_DIAGNOSTIC)
#error Drowned targeted profiling requires the isolated minimal diagnostic
#endif
ULONG (performanceProfileBegin)(void);
void (performanceProfileEnd)(enum PerformanceProfileSlot slot,ULONG start);
#define DROWNED_MEASURE(slot,call) do { \
    ULONG drownedMeasureStart=(performanceProfileBegin)(); \
    call; \
    (performanceProfileEnd)(slot,drownedMeasureStart); \
} while(0)
#else
#define DROWNED_MEASURE(slot,call) do { call; } while(0)
#endif

#endif
