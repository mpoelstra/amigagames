#define SPARKPAW_PERFORMANCE_PROFILE_IMPLEMENTATION
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
    "enemy_parked","enemy_active","enemy_respawn","enemy_activate",
    "blitter_wait","stormrail_restore","stormrail_draw"
#if defined(SPARKPAW_DROWNED_TARGETED_PROFILE) || defined(SPARKPAW_DROWNED_FERRY_PROFILE)
    ,"drowned_patch_build","drowned_patch_sync"
#endif
};

#ifdef SPARKPAW_DROWNED_FERRY_PROFILE
static WORD ferrySelected=-1;
static ULONG ferryFrames,ferryUpperFrames;
static UBYTE ferryPhase;
void performanceFerryFrame(WORD playerX,WORD playerY)
{
    static const UBYTE slots[8]={PERF_GAME_UPDATE,PERF_BOB_PASS,
        PERF_BOB_COMPACT_TARGET,PERF_BOB_WATER,PERF_RING_ROLL,
        PERF_RING_DYNAMIC,PERF_BLITTER_WAIT,PERF_BOB_ENEMY_DRAW};
    ferrySelected=-1;
    if(playerX>=2800&&playerX<3200) {
        /* Prime-length sampling cycle avoids always sampling one water or
           sprite animation phase (their periods are powers of two). */
        ferrySelected=slots[ferryPhase<8?ferryPhase:ferryPhase<16?ferryPhase-8:0];
        if(++ferryPhase==17) ferryPhase=0;
        ferryFrames++;
        if(playerY<140) ferryUpperFrames++;
    }
}
BOOL performanceFerrySelected(enum PerformanceProfileSlot slot)
{ return ferrySelected==(WORD)slot; }
#endif

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
#ifdef SPARKPAW_DROWNED_FERRY_PROFILE
    FPrintf(file,"ferry_profile=1 buffer_split=1 range=2800..3199 rotating_scopes=8 sampling_cycle=17 timer_pairs_per_frame=1 frames=%ld upper_y_lt140=%ld observer_cost=nonzero\n",ferryFrames,ferryUpperFrames);
#endif
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
        FPrintf(file,"cia section=%s calls=%ld retained=%ld total_ticks=%ld median_ticks=%ld p95_ticks=%ld avg_ticks=%ld max_ticks=%ld avg_frame_x1000=%ld max_frame_x1000=%ld\n",
                names[slot],total->samples,(LONG)total->retained,total->total,
                median,p95,average,total->maximum,
                (average*1000UL)/14188UL,
                (total->maximum*1000UL)/14188UL);
        if(sorted) FreeMem(sorted,(ULONG)total->retained*sizeof(*sorted));
    }
}
#endif
