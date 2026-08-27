#ifndef SPARKPAW_PLATFORM_AMIGA_H
#define SPARKPAW_PLATFORM_AMIGA_H

#include <exec/types.h>

BOOL platformOpen(void);
void platformClose(void);
void platformBeginTakeover(void);
void platformFinishTakeover(UWORD *copper);
void platformSwitchCopper(UWORD *copper);
void platformRestore(void);
UWORD platformRasterLine(void);
void platformWaitBlit(void);
void platformSetBlitterPriority(BOOL enabled);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
BOOL platformBlitterBusy(void);
BOOL platformLeftMouse(void);
void platformPrepareDebugFlush(void);
void platformProfileTimerStart(void);
ULONG platformProfileTimerTicks(void);
#endif
void platformReadGameKeys(BOOL *left,BOOL *right,BOOL *down,
                          BOOL *jump,BOOL *fire);

#endif
