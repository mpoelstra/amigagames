#ifndef SPARKPAW_FRAME_PHASE_H
#define SPARKPAW_FRAME_PHASE_H

#ifdef SPARKPAW_HOST_TEST
#include <stdint.h>
typedef uint32_t ULONG;
typedef uint16_t UWORD;
typedef uint8_t BOOL;
#define TRUE 1
#else
#include <exec/types.h>
#endif

#define SPARKPAW_PAL_LINES 312

struct FramePhaseClock {
    ULONG epoch;
    UWORD line;
    BOOL initialized;
};

/* skippedFields comes from an independent slower clock when sampling may have
   skipped complete fields. The raster comparison accounts for the final
   partial wrap, so a multi-field gap is represented explicitly. */
static void framePhaseSample(struct FramePhaseClock *clock,UWORD line,
                             ULONG skippedFields)
{
    if(!clock->initialized) {
        clock->line=line; clock->initialized=TRUE; return;
    }
    clock->epoch+=skippedFields;
    if(line<clock->line) clock->epoch++;
    clock->line=line;
}

static ULONG framePhaseStamp(const struct FramePhaseClock *clock)
{
    return clock->epoch*SPARKPAW_PAL_LINES+clock->line;
}

#endif
