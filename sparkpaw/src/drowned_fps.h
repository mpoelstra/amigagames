#ifndef DROWNED_FPS_H
#define DROWNED_FPS_H
#include <exec/types.h>
#ifdef SPARKPAW_DROWNED_FPS
void drownedFpsPublished(ULONG field, UWORD line);
void drownedFpsPause(void);
void drownedFpsMemory(void);
void drownedFpsWrite(void);
#endif
#endif
