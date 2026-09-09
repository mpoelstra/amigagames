#ifndef READY_UI_H
#define READY_UI_H
#define READY_UI_PLANE 2912
#define READY_UI_BYTES (6*READY_UI_PLANE)
/* Allocated explicitly in Fast RAM by title.c, never a Blitter source. */
struct ReadyUI {
    unsigned char background[READY_UI_BYTES];
    unsigned char patch[READY_UI_BYTES];
    unsigned char previous[2][READY_UI_BYTES];
    unsigned char mask[READY_UI_PLANE];
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
