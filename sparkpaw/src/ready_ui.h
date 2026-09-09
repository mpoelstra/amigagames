#ifndef READY_UI_H
#define READY_UI_H
#define READY_UI_PLANE 2912
#define READY_UI_BYTES (6*READY_UI_PLANE)
#ifdef SPARKPAW_MULTI_ADF
#include "ready_ui_cache_adf_size.h"
#else
#include "ready_ui_cache_size.h"
#endif
/* CPU-only Fast RAM. Rasterization/difference discovery are OFFLINE ONLY.
   See docs/READY_UI_PERFORMANCE_CONTRACT.md before extending this UI. */
struct ReadyUI {
    unsigned char cache[READY_UI_CACHE_BYTES];
    unsigned char mask[READY_UI_PLANE];
    unsigned char selected[5];
    unsigned char previous[2][5];
};
struct ReadySelection {
    unsigned char page,row,secondary,section,mode,sfx,track,status;
};
enum { READY_PAGE_MAIN, READY_PAGE_OPTIONS, READY_PAGE_SOUND };
enum { READY_PREVIEW_IDLE, READY_PREVIEW_PLAYING, READY_PREVIEW_LOADING,
       READY_PREVIEW_ERROR };
void readyUiInit(struct ReadyUI *ui,unsigned char **atlas);
void readyUiCompose(struct ReadyUI *ui,unsigned char **atlas,
                    const struct ReadySelection *selection,int soundtest);
void readyUiApply(struct ReadyUI *ui,unsigned char **target,unsigned buffer);
#endif
