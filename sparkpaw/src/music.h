#ifndef SPARKPAW_MUSIC_H
#define SPARKPAW_MUSIC_H
#include <exec/types.h>
BOOL musicInitialize(UWORD displayRate);
BOOL musicPlayTitle(void);
BOOL musicPlayIntro(void);
void musicOwnedFrame(void);
void musicStop(void);
void musicShutdown(void);
BOOL musicIsReady(void);
BOOL musicIsPlaying(void);
#endif
