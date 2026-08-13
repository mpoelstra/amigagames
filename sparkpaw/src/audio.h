#ifndef SPARKPAW_AUDIO_H
#define SPARKPAW_AUDIO_H

#include <exec/types.h>

BOOL audioLoad(void);
void audioUnload(void);
void audioSetHardwareActive(BOOL active);
void audioPlayShot(void);
void audioPlayPlayerHurt(void);
void audioPlayEnemyHit(void);
void audioPlayStriderShot(void);
void audioPlayJump(void);
void audioPlayCollect(void);
void audioUpdate(void);

#endif
