#define SPARKPAW_RENDERER_DISPATCH_UNIT
#include "renderer.h"
#include "game.h"

enum RendererSection { RENDERER_SECTION_NONE,RENDERER_SECTION_LEVEL1,
                       RENDERER_SECTION_STORMRAIL };
static enum RendererSection activeRenderer=RENDERER_SECTION_NONE;
BOOL rendererStormrailSelected=FALSE;

#define DECLARE_VARIANT(prefix) \
BOOL prefix##LoadGameplay(void); \
BOOL prefix##PrepareGameplay(void); \
void prefix##ResetGameplay(void); \
void prefix##Cleanup(void); \
UWORD *prefix##CopperList(void); \
void prefix##UpdateGameplay(void); \
BOOL prefix##PublishGameplay(UWORD line); \
void prefix##DrawGameplayBobs(void)
DECLARE_VARIANT(rendererLevel1);
DECLARE_VARIANT(rendererStormrail);

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#define DECLARE_DIAGNOSTIC(prefix) \
void prefix##DiagnosticUpdateEntry(UWORD line); \
void prefix##DiagnosticPublicationEntry(UWORD line); \
void prefix##DiagnosticPublicationExit(UWORD line); \
void prefix##DiagnosticBobEntry(UWORD line); \
void prefix##DiagnosticBobExit(UWORD line); \
void prefix##DiagnosticBoundary(UWORD line,BOOL published); \
void prefix##WriteDiagnosticLog(void)
DECLARE_DIAGNOSTIC(rendererLevel1);
DECLARE_DIAGNOSTIC(rendererStormrail);
#endif

#ifdef SPARKPAW_STORMRAIL_PROOF
BOOL rendererStormrailCaptureStormrailProof(UBYTE index);
BOOL rendererStormrailWriteStormrailProofFrames(void);
ULONG rendererStormrailUnsafeBlitsInternal(void);
#endif

static BOOL useStormrail(void)
{
#ifdef SPARKPAW_STORMRAIL_PROOF
    return gameStormrailActive();
#else
    return FALSE;
#endif
}

BOOL rendererLoadGameplay(void)
{
    activeRenderer=useStormrail()?RENDERER_SECTION_STORMRAIL:
                                  RENDERER_SECTION_LEVEL1;
    rendererStormrailSelected=(activeRenderer==RENDERER_SECTION_STORMRAIL);
    return activeRenderer==RENDERER_SECTION_STORMRAIL?
        rendererStormrailLoadGameplay():rendererLevel1LoadGameplay();
}

BOOL rendererPrepareGameplay(void)
{
    return activeRenderer==RENDERER_SECTION_STORMRAIL?
        rendererStormrailPrepareGameplay():rendererLevel1PrepareGameplay();
}

void rendererResetGameplay(void)
{
    if(activeRenderer==RENDERER_SECTION_STORMRAIL)
        rendererStormrailResetGameplay();
    else rendererLevel1ResetGameplay();
}

void rendererCleanup(void)
{
    if(activeRenderer==RENDERER_SECTION_STORMRAIL)
        rendererStormrailCleanup();
    else if(activeRenderer==RENDERER_SECTION_LEVEL1) rendererLevel1Cleanup();
    activeRenderer=RENDERER_SECTION_NONE;
    rendererStormrailSelected=FALSE;
}

UWORD *rendererCopperList(void)
{
    return activeRenderer==RENDERER_SECTION_STORMRAIL?
        rendererStormrailCopperList():rendererLevel1CopperList();
}

#ifdef SPARKPAW_STORMRAIL_PROOF
BOOL rendererCaptureStormrailProof(UBYTE index)
{ return rendererStormrailCaptureStormrailProof(index); }
BOOL rendererWriteStormrailProofFrames(void)
{ return rendererStormrailWriteStormrailProofFrames(); }
ULONG rendererStormrailUnsafeBlits(void)
{ return rendererStormrailUnsafeBlitsInternal(); }
#endif

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#define DISPATCH_DIAGNOSTIC_1(name,arg) do { \
    if(activeRenderer==RENDERER_SECTION_STORMRAIL) \
        rendererStormrail##name(arg); \
    else rendererLevel1##name(arg); \
} while(0)
void rendererDiagnosticUpdateEntry(UWORD line)
{ DISPATCH_DIAGNOSTIC_1(DiagnosticUpdateEntry,line); }
void rendererDiagnosticPublicationEntry(UWORD line)
{ DISPATCH_DIAGNOSTIC_1(DiagnosticPublicationEntry,line); }
void rendererDiagnosticPublicationExit(UWORD line)
{ DISPATCH_DIAGNOSTIC_1(DiagnosticPublicationExit,line); }
void rendererDiagnosticBobEntry(UWORD line)
{ DISPATCH_DIAGNOSTIC_1(DiagnosticBobEntry,line); }
void rendererDiagnosticBobExit(UWORD line)
{ DISPATCH_DIAGNOSTIC_1(DiagnosticBobExit,line); }
void rendererDiagnosticBoundary(UWORD line,BOOL published)
{
    if(activeRenderer==RENDERER_SECTION_STORMRAIL)
        rendererStormrailDiagnosticBoundary(line,published);
    else rendererLevel1DiagnosticBoundary(line,published);
}
void rendererWriteDiagnosticLog(void)
{
    if(activeRenderer==RENDERER_SECTION_STORMRAIL)
        rendererStormrailWriteDiagnosticLog();
    else rendererLevel1WriteDiagnosticLog();
}
#endif
