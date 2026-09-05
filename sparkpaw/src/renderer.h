#ifndef SPARKPAW_RENDERER_H
#define SPARKPAW_RENDERER_H

#include <exec/types.h>

BOOL rendererLoadGameplay(void);
BOOL rendererPrepareGameplay(void);
void rendererResetGameplay(void);
void rendererCleanup(void);
UWORD *rendererCopperList(void);
#if defined(SPARKPAW_SPLIT_RENDERER_CLIENT) && \
    !defined(SPARKPAW_RENDERER_IMPLEMENTATION_UNIT) && \
    !defined(SPARKPAW_RENDERER_DISPATCH_UNIT)
extern BOOL rendererStormrailSelected;
void rendererLevel1UpdateGameplay(void);
BOOL rendererLevel1PublishGameplay(UWORD rasterLine);
void rendererLevel1DrawGameplayBobs(void);
void rendererStormrailUpdateGameplay(void);
BOOL rendererStormrailPublishGameplay(UWORD rasterLine);
void rendererStormrailDrawGameplayBobs(void);
#define rendererUpdateGameplay() do { \
    if(rendererStormrailSelected) rendererStormrailUpdateGameplay(); \
    else rendererLevel1UpdateGameplay(); \
} while(0)
#define rendererPublishGameplay(rasterLine) \
    (rendererStormrailSelected? \
        rendererStormrailPublishGameplay(rasterLine): \
        rendererLevel1PublishGameplay(rasterLine))
#define rendererDrawGameplayBobs() do { \
    if(rendererStormrailSelected) rendererStormrailDrawGameplayBobs(); \
    else rendererLevel1DrawGameplayBobs(); \
} while(0)
#else
void rendererUpdateGameplay(void);
BOOL rendererPublishGameplay(UWORD rasterLine);
void rendererDrawGameplayBobs(void);
#endif

#ifdef SPARKPAW_EXTRA_LIFE_VISUAL_PROOF
BOOL rendererWriteExtraLifeProof(void);
#endif

#ifdef SPARKPAW_REPLAY_PROOF
BOOL rendererReplayPresentationValid(void);
#endif

#ifdef SPARKPAW_STORMRAIL_PROOF
BOOL rendererCaptureStormrailProof(UBYTE index);
BOOL rendererWriteStormrailProofFrames(void);
ULONG rendererStormrailUnsafeBlits(void);
#endif

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
void rendererDiagnosticUpdateEntry(UWORD line);
void rendererDiagnosticPublicationEntry(UWORD line);
void rendererDiagnosticPublicationExit(UWORD line);
void rendererDiagnosticBobEntry(UWORD line);
void rendererDiagnosticBobExit(UWORD line);
void rendererDiagnosticBoundary(UWORD line,BOOL published);
void rendererWriteDiagnosticLog(void);

/* Optional observer-cost experiment. Keep the trace contents and section
   selection intact, but bypass the extra diagnostic wrapper stack frame.
   Normal production and the retained cadence target do not enable this. */
#if defined(SPARKPAW_DIRECT_DIAGNOSTIC_CLIENT) && \
    defined(SPARKPAW_SPLIT_RENDERER_CLIENT) && \
    !defined(SPARKPAW_RENDERER_IMPLEMENTATION_UNIT) && \
    !defined(SPARKPAW_RENDERER_DISPATCH_UNIT)
#define DECLARE_DIRECT_DIAGNOSTIC(prefix) \
void prefix##DiagnosticUpdateEntry(UWORD line); \
void prefix##DiagnosticPublicationEntry(UWORD line); \
void prefix##DiagnosticPublicationExit(UWORD line); \
void prefix##DiagnosticBobEntry(UWORD line); \
void prefix##DiagnosticBobExit(UWORD line); \
void prefix##DiagnosticBoundary(UWORD line,BOOL published)
DECLARE_DIRECT_DIAGNOSTIC(rendererLevel1);
DECLARE_DIRECT_DIAGNOSTIC(rendererStormrail);
#undef DECLARE_DIRECT_DIAGNOSTIC
#define DIRECT_DIAGNOSTIC_CALL(name,line) do { \
    if(rendererStormrailSelected) rendererStormrail##name(line); \
    else rendererLevel1##name(line); \
} while(0)
#define rendererDiagnosticUpdateEntry(line) \
    DIRECT_DIAGNOSTIC_CALL(DiagnosticUpdateEntry,line)
#define rendererDiagnosticPublicationEntry(line) \
    DIRECT_DIAGNOSTIC_CALL(DiagnosticPublicationEntry,line)
#define rendererDiagnosticPublicationExit(line) \
    DIRECT_DIAGNOSTIC_CALL(DiagnosticPublicationExit,line)
#define rendererDiagnosticBobEntry(line) \
    DIRECT_DIAGNOSTIC_CALL(DiagnosticBobEntry,line)
#define rendererDiagnosticBobExit(line) \
    DIRECT_DIAGNOSTIC_CALL(DiagnosticBobExit,line)
#define rendererDiagnosticBoundary(line,published) do { \
    if(rendererStormrailSelected) \
        rendererStormrailDiagnosticBoundary(line,published); \
    else rendererLevel1DiagnosticBoundary(line,published); \
} while(0)
#endif
#endif

#ifdef PHASE6_MEMORY_TEST
ULONG rendererPhase6PeakChipFree(void);
ULONG rendererPhase6PeakChipLargest(void);
ULONG rendererPhase6PeakFastFree(void);
ULONG rendererPhase6PeakFastLargest(void);
#endif

#endif
