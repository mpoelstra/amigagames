#ifndef SPARKPAW_AUDIO_H
#define SPARKPAW_AUDIO_H

#include <exec/types.h>
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#include <dos/dos.h>
#endif

BOOL audioLoad(void);
void audioUnload(void);
void audioSetHardwareActive(BOOL active);
void audioPlayShot(void);
void audioPlayPlayerHurt(void);
void audioPlayEnemyHit(void);
void audioPlayEnemyDeath(void);
void audioPlayStriderShot(void);
void audioPlayJump(void);
void audioPlayCollect(void);
void audioPlayWaterSplash(void);
void audioPlayStormstoneCore(void);
void audioPlayTallyTick(void);
void audioUpdate(void);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
void audioDiagnosticWrite(BPTR file);
#endif

#endif
