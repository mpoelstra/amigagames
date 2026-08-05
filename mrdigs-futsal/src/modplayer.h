#ifndef FUTSAL_MODPLAYER_H
#define FUTSAL_MODPLAYER_H
#include <exec/types.h>
BOOL musicInitialize(UWORD displayRate);
BOOL musicPlay(const char *filename);
void musicStop(void);
void musicShutdown(void);
BOOL musicIsReady(void);
#endif
