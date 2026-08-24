#include "performance_profile.h"

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#include <exec/memory.h>
#include <proto/dos.h>
#include <proto/exec.h>

#include "platform_amiga.h"

struct ProfileTotal {
    ULONG samples;
    ULONG total;
    ULONG maximum;
    UWORD retained;
};

#define PROFILE_RETAINED_SAMPLES 1024
static struct ProfileTotal totals[PERF_SLOT_COUNT];
static ULONG retainedSamples[PERF_SLOT_COUNT][PROFILE_RETAINED_SAMPLES];
static const char *const names[PERF_SLOT_COUNT]={
    "game_update","copper_patch","bob_pass","publish_wait",
    "player","enemies","collectibles","projectiles",
    "input","player_animate","enemy_contact","projectile_contact",
    "audio_update","copper_copy","sprite_stage","hud_update",
    "scroll_patch","ring_roll","ring_dynamic",
    "bob_projectile_restore","bob_enemy_restore",
    "bob_collectible_restore","bob_splash_restore","bob_water",
    "bob_compact_target","bob_splash_draw","bob_collectible_draw",
    "bob_enemy_draw","bob_projectile_draw","bob_final_wait",
    "enemy_parked","enemy_active","enemy_respawn","enemy_activate"
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
    retainedSamples[slot][(total->samples-1)&
                          (PROFILE_RETAINED_SAMPLES-1)]=elapsed;
    if(total->retained<PROFILE_RETAINED_SAMPLES) total->retained++;
}

static void sortSamples(ULONG *values,WORD left,WORD right)
{
    WORD i=left,j=right;
    ULONG pivot=values[(left+right)>>1];
    while(i<=j) {
        ULONG swap;
        while(values[i]<pivot) i++;
        while(values[j]>pivot) j--;
        if(i>j) break;
        swap=values[i]; values[i]=values[j]; values[j]=swap;
        i++; j--;
    }
    if(left<j) sortSamples(values,left,j);
    if(i<right) sortSamples(values,i,right);
}

void performanceProfileWrite(BPTR file)
{
    UWORD slot;
    FPrintf(file,"cia_profile clock=ciab_timer_b_eclock pal_ticks_per_frame_approx=14188\n");
    for(slot=0;slot<PERF_SLOT_COUNT;slot++) {
        const struct ProfileTotal *total=&totals[slot];
        ULONG average=total->samples?total->total/total->samples:0;
        ULONG median=0,p95=0;
        ULONG *sorted=(ULONG *)AllocMem(
            (ULONG)total->retained*sizeof(*sorted),MEMF_FAST);
        if(sorted&&total->retained) {
            UWORD p95Index=(UWORD)(((ULONG)total->retained*95UL+99UL)/100UL-1);
            CopyMem(retainedSamples[slot],sorted,
                    (ULONG)total->retained*sizeof(*sorted));
            sortSamples(sorted,0,(WORD)(total->retained-1));
            median=sorted[total->retained>>1];
            p95=sorted[p95Index];
        }
        FPrintf(file,"cia section=%s calls=%ld retained=%ld median_ticks=%ld p95_ticks=%ld avg_ticks=%ld max_ticks=%ld avg_frame_x1000=%ld max_frame_x1000=%ld\n",
                names[slot],total->samples,(LONG)total->retained,median,p95,
                average,total->maximum,
                (average*1000UL)/14188UL,
                (total->maximum*1000UL)/14188UL);
        if(sorted) FreeMem(sorted,(ULONG)total->retained*sizeof(*sorted));
    }
}
#endif
