#ifndef WHD_LOAD_TRACE_H
#define WHD_LOAD_TRACE_H
enum { WLT_FILES, WLT_COLLISION, WLT_AUDIO, WLT_RENDERER, WLT_READY,
       WLT_MENU_CACHE, WLT_MIN_WAIT, WLT_RENDER_SETUP, WLT_RENDER_ENEMIES,
       WLT_RENDER_EFFECTS, WLT_RENDER_TARGETS, WLT_RENDER_FINAL,
       WLT_BOOT_BANKS, WLT_RETURN_RELEASE, WLT_RETURN_CLEANUP, WLT_RETURN_TITLE,
       WLT_PHASE_COUNT };
#ifdef SPARKPAW_WHD_LOAD_TRACE
#include <exec/types.h>
void whdLoadTraceSection(UWORD section);
void whdLoadTraceBegin(UWORD phase);
BOOL whdLoadTraceEnd(UWORD phase,BOOL success);
void whdLoadTraceWrite(void);
#define WLT_CALL(phase,expression) \
    (whdLoadTraceBegin(phase),whdLoadTraceEnd(phase,(expression)))
#else
#define WLT_CALL(phase,expression) (expression)
#endif
#endif
