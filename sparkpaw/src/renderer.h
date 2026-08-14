#ifndef SPARKPAW_RENDERER_H
#define SPARKPAW_RENDERER_H

#include <exec/types.h>

BOOL rendererLoadGameplay(void);
BOOL rendererPrepareGameplay(void);
void rendererCleanup(void);
UWORD *rendererCopperList(void);
void rendererUpdateGameplay(void);
void rendererDrawGameplayBobs(void);

#ifdef PHASE6_MEMORY_TEST
ULONG rendererPhase6PeakChipFree(void);
ULONG rendererPhase6PeakChipLargest(void);
ULONG rendererPhase6PeakFastFree(void);
ULONG rendererPhase6PeakFastLargest(void);
#endif

#endif
