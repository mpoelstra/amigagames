#ifndef SPARKPAW_TITLE_H
#define SPARKPAW_TITLE_H

#include <exec/types.h>

BOOL titleShow(void);
BOOL titleShowLevelLoading(void);
const char *titleFailureReason(void);
LONG titleScreenError(void);
ULONG titleChipFree(void);
ULONG titleChipLargest(void);
void titleWaitFrames(UWORD frames);
void titleRestoreSystemView(void);
void titleRelease(void);

#endif