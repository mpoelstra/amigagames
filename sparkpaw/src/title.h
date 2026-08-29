#ifndef SPARKPAW_TITLE_H
#define SPARKPAW_TITLE_H

#include <exec/types.h>
#include "control_options.h"

BOOL titleShow(void);
BOOL titlePrepareLevelLoading(void);
BOOL titleShowLevelLoading(void);
BOOL titleShowReplayLoading(void);
BOOL titleShowLevelCharging(void);
BOOL titleShowLevelReady(void);
BOOL titleShowLevelComplete(void);
void titleRunLevelComplete(UWORD enemies,UWORD diamonds,
                           ULONG elapsedFields,ULONG liveScore);
void titleRunLevelReadyMenu(enum SecondaryButtonAction *secondaryAction);
UWORD *titleCopperList(void);
void titleWaitLevelCharging(UWORD frames);
void titleFadeOut(void);
const char *titleFailureReason(void);
ULONG titleChipFree(void);
ULONG titleChipLargest(void);
void titleWaitFrames(UWORD frames);
void titleRestoreSystemView(void);
void titleRelease(void);

#endif
