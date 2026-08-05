#ifndef CHIPSNAKE_MODPLAYER_H
#define CHIPSNAKE_MODPLAYER_H

#include <exec/types.h>

BOOL musicInitialize(UWORD displayRate);
BOOL musicPlay(const char *filename);
void musicFrame(void);
void musicStop(void);
void musicShutdown(void);
BOOL musicIsReady(void);

#endif
