#include "performance_profile.h"

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#include <proto/dos.h>

#include "platform_amiga.h"

struct ProfileTotal {
    ULONG samples;
    ULONG total;
    ULONG maximum;
};

static struct ProfileTotal totals[PERF_SLOT_COUNT];
static const char *const names[PERF_SLOT_COUNT]={
    "game_update","copper_patch","bob_pass","publish_wait",
    "player","enemies","collectibles","projectiles"
};

ULONG performanceProfileBegin(void)
{
    return platformProfileTimerTicks();
}

void performanceProfileEnd(enum PerformanceProfileSlot slot,ULONG start)
{
    ULONG elapsed=platformProfileTimerTicks()-start;
    struct ProfileTotal *total=&totals[slot];
    total->samples++;
    total->total+=elapsed;
    if(elapsed>total->maximum) total->maximum=elapsed;
}

void performanceProfileWrite(BPTR file)
{
    UWORD slot;
    FPrintf(file,"cia_profile clock=ciab_timer_b_eclock pal_ticks_per_frame_approx=14188\n");
    for(slot=0;slot<PERF_SLOT_COUNT;slot++) {
        const struct ProfileTotal *total=&totals[slot];
        ULONG average=total->samples?total->total/total->samples:0;
        FPrintf(file,"cia section=%s samples=%ld avg_ticks=%ld max_ticks=%ld avg_frame_x1000=%ld max_frame_x1000=%ld\n",
                names[slot],total->samples,average,total->maximum,
                (average*1000UL)/14188UL,
                (total->maximum*1000UL)/14188UL);
    }
}
#endif
