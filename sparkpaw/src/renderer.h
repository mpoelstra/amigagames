#ifndef SPARKPAW_RENDERER_H
#define SPARKPAW_RENDERER_H

#include <exec/types.h>

BOOL rendererLoadGameplay(void);
BOOL rendererPrepareGameplay(void);
void rendererCleanup(void);
UWORD *rendererCopperList(void);
void rendererUpdateGameplay(void);
void rendererDrawGameplayBobs(void);

#endif