#ifndef SPARKPAW_RENDERER_H
#define SPARKPAW_RENDERER_H

#include <exec/types.h>

BOOL rendererLoadGameplay(void);
BOOL rendererPrepareGameplay(void);
void rendererCleanup(void);
UWORD *rendererCopperList(void);
void rendererUpdateGameplay(void);
BOOL rendererPublishGameplay(UWORD rasterLine);
void rendererDrawGameplayBobs(void);

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
void rendererDiagnosticUpdateEntry(UWORD line);
void rendererDiagnosticPublicationEntry(UWORD line);
void rendererDiagnosticPublicationExit(UWORD line);
void rendererDiagnosticBobEntry(UWORD line);
void rendererDiagnosticBobExit(UWORD line);
void rendererDiagnosticBoundary(UWORD line,BOOL published);
void rendererWriteDiagnosticLog(void);
#endif

#ifdef PHASE6_MEMORY_TEST
ULONG rendererPhase6PeakChipFree(void);
ULONG rendererPhase6PeakChipLargest(void);
ULONG rendererPhase6PeakFastFree(void);
ULONG rendererPhase6PeakFastLargest(void);
#endif

#endif
