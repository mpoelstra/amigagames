#ifndef SPARKPAW_MUSIC_H
#define SPARKPAW_MUSIC_H
#include <exec/types.h>
BOOL musicInitialize(UWORD displayRate);
BOOL musicPlayTitle(void);
BOOL musicPlayGameOver(void);
#ifdef SPARKPAW_THREE_ADF
BOOL musicPreloadGameOver(void);
#endif
BOOL musicPlayIntro(void);
void musicOwnedFrame(void);
void musicSuspend(void);
BOOL musicRestartTitle(void);
BOOL musicAudible(void);
void musicStop(void);
void musicShutdown(void);
BOOL musicIsReady(void);
BOOL musicIsPlaying(void);
#endif
