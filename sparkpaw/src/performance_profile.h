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
    PERF_SLOT_COUNT
};

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
ULONG performanceProfileBegin(void);
void performanceProfileEnd(enum PerformanceProfileSlot slot,ULONG start);
void performanceProfileWrite(BPTR file);
#else
#define performanceProfileBegin() 0UL
#define performanceProfileEnd(slot,start) do { } while(0)
#endif

#endif
