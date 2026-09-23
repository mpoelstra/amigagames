#include "drowned_busy.h"
/* Isolated music-compatible manual-test observer. No CIA timer ownership. */
#if !defined(SPARKPAW_DROWNED_FPS) || (!defined(SPARKPAW_DROWNED_FULL) && !defined(SPARKPAW_LEVEL1_RING_TEST))
#error Drowned_FPS_is_only_for_the_full_level_manual_test
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#error Do_not_combine_Drowned_FPS_with_the_CIA_B_profiler
#endif
#include "drowned_fps.h"
#include "game.h"
#include "player.h"
#include <exec/memory.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include "drowned_fps_counters.h"
#ifndef SPARKPAW_DROWNED_FPS_BUILD_ID
#define SPARKPAW_DROWNED_FPS_BUILD_ID unbound
#endif
#define FPS_STRING_VALUE_(value) #value
#define FPS_STRING_VALUE(value) FPS_STRING_VALUE_(value)
static ULONG preparedChip, preparedLargest, preparedFast, preparedFastLargest;
void drownedFpsMemory(void)
{
#ifdef SPARKPAW_DROWNED_BUSY
    drownedBusyInit();
#endif
    preparedChip=AvailMem(MEMF_CHIP);
    preparedLargest=AvailMem(MEMF_CHIP|MEMF_LARGEST);
    preparedFast=AvailMem(MEMF_FAST);
    preparedFastLargest=AvailMem(MEMF_FAST|MEMF_LARGEST);
}
void drownedFpsPublished(ULONG field, UWORD line)
{
    const struct PlayerState *p=playerState();
    fpsRecord(field,line,(WORD)(p->x>>8),(WORD)(p->y>>8),
              (ULONG)gameState()->frameCounter);
}
void drownedFpsPause(void)
{
    if(fpsPrimed) fpsPauses++;
    fpsPrimed=0;
    fpsResetTail=0;
}
static void writeTotals(BPTR file, const char *name, struct DrownedFpsTotals *s)
{
    FPrintf(file,"sample=%s intervals=%ld fields=%ld zero=%ld one=%ld two=%ld three_plus=%ld max=%ld\n",
            (STRPTR)name,s->intervals,s->fields,s->zero,s->one,s->two,
            s->threePlus,s->maximum);
}
void drownedFpsWrite(void)
{
#ifdef SPARKPAW_LEVEL1_RING_TEST
    static const char *names[8]={"x0_511","x512_1023","x1024_1535","x1536_2047",
        "x2048_2559","x2560_3071","x3072_3327","x3328_end"};
#else
    static const char *names[8]={"land","precision","approach","ferry_first",
        "ferry_last","finale_approach","governors","station"};
#endif
    BPTR file=Open("PROGDIR:renderdiag.log",MODE_NEWFILE);
    UBYTE i;
    if(!file) return;
#ifdef SPARKPAW_LEVEL1_RING_TEST
    FPrintf(file,"level1_ring_fps_v1 clock=ciaa_tod_read_only music=level1 seed=0x53504157\n");
#else
    FPrintf(file,"drowned_fps_v1 clock=ciaa_tod_read_only music=v5 seed=0x53504157\n");
#endif
    FPrintf(file,"build_id=%s\n",FPS_STRING_VALUE(SPARKPAW_DROWNED_FPS_BUILD_ID));
#ifdef SPARKPAW_LEVEL1_RING_TEST
#ifdef SPARKPAW_LEVEL1_TWO_COPY_RING
    FPrintf(file,"variant=B_level1_two_copies_base96\n");
#else
    FPrintf(file,"variant=A_level1_three_copies_base512\n");
#endif
#elif defined(SPARKPAW_DROWNED_BUSY)
    FPrintf(file,"variant=busy_sparse_discovery\n");
#elif defined(SPARKPAW_DROWNED_TWO_COPY_TEST)
#ifdef SPARKPAW_DROWNED_TWO_COPY_RING
    FPrintf(file,"variant=B_ring_two_copies_base96\n");
#else
    FPrintf(file,"variant=A_ring_three_copies_base512\n");
#endif
#elif defined(SPARKPAW_DROWNED_ENEMY_BOUNDS_TEST)
#ifdef SPARKPAW_DROWNED_ENEMY_BOUNDS
    FPrintf(file,"variant=B_enemy_mask_bounds\n");
#else
    FPrintf(file,"variant=A_enemy_full_cells\n");
#endif
#elif defined(SPARKPAW_DROWNED_REAR_DIRECT_TEST)
#ifdef SPARKPAW_DROWNED_REAR_DIRECT_WATER
    FPrintf(file,"variant=B_rear_water_direct\n");
#else
    FPrintf(file,"variant=A_rear_water_staged\n");
#endif
#elif defined(SPARKPAW_DROWNED_RESET_TEST)
#ifdef SPARKPAW_DROWNED_RESET_BLIT
    FPrintf(file,"variant=B_reset_blitter\n");
#else
    FPrintf(file,"variant=A_reset_cpu\n");
#endif
#elif defined(SPARKPAW_DROWNED_REAR_STAGE_REFERENCE)
    FPrintf(file,"variant=A_rear_stage_reference\n");
#else
    FPrintf(file,"variant=B_rear_stage_pointers\n");
#endif
#ifdef SPARKPAW_DROWNED_BUSY
    FPrintf(file,"raw_tod=1 exact_deadlines=0 ownership_counters=unavailable cpu_scopes=raw_checkpoints observer_cost=nonzero\n");
#else
    FPrintf(file,"raw_tod=1 exact_deadlines=0 ownership_counters=unavailable cpu_scopes=none observer_cost=nonzero\n");
#endif
    FPrintf(file,"sample_point=after_copjmp_before_bookkeeping_and_rear attribution=previous_published_player\n");
#ifdef SPARKPAW_LEVEL1_RING_TEST
    FPrintf(file,"regions_x=512,1024,1536,2048,2560,3072,3328 reset_split=simulation_counter_decrease\n");
#else
    FPrintf(file,"regions_x=1376,2128,2400,2800,3200,3888,4656 upper_y_lt=140 reset_split=simulation_counter_decrease\n");
#endif
    writeTotals(file,"all",&fpsTotal);
    for(i=0;i<8;i++) writeTotals(file,names[i],&fpsRegion[i]);
#ifndef SPARKPAW_LEVEL1_RING_TEST
    writeTotals(file,"late_first_lower",&fpsFerry[0][0]);
    writeTotals(file,"late_first_upper",&fpsFerry[0][1]);
    writeTotals(file,"late_after_reset_lower",&fpsFerry[1][0]);
    writeTotals(file,"late_after_reset_upper",&fpsFerry[1][1]);
#endif
    writeTotals(file,"reset_and_next",&fpsReset);
    FPrintf(file,"resets=%ld pauses=%ld max_publish_argument_line=%ld\n",fpsResets,fpsPauses,(ULONG)fpsMaxPublishLine);
    FPrintf(file,"prepared_chip=%ld largest_chip=%ld prepared_fast=%ld largest_fast=%ld\n",
            preparedChip,preparedLargest,preparedFast,preparedFastLargest);
#ifdef SPARKPAW_DROWNED_BUSY
    drownedBusyWrite(file);
#endif
    FPrintf(file,"post_run=complete\n");
    Close(file);
}
