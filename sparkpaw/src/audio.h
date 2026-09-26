#ifndef SPARKPAW_AUDIO_H
#define SPARKPAW_AUDIO_H

#include <exec/types.h>
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#include <dos/dos.h>
#endif

enum AudioMode { AUDIO_FX_ONLY, AUDIO_MUSIC_ONLY, AUDIO_FX_MUSIC };
void audioSetMode(enum AudioMode mode);
enum AudioMode audioGetMode(void);
void audioBeginGameplay(void);
#ifndef SPARKPAW_MULTI_ADF
void audioPreviewEffect(unsigned id);
BOOL audioPreviewEffectPlaying(void);
#ifdef SPARKPAW_CAMPAIGN_DROWNED
BOOL audioPreviewPrepareHarrierDefeat(void);
void audioPreviewReleaseHarrierDefeat(void);
#endif
#endif
BOOL audioLoad(void);
void audioUnload(void);
void audioSetHardwareActive(BOOL active);
void audioPlayShot(void);
void audioPlayPlayerHurt(void);
void audioPlayEnemyHit(void);
void audioPlayEnemyDeath(void);
void audioPlayStriderShot(void);
void audioPlayHarrierFanCharge(void);
void audioPlayHarrierFanFire(void);
void audioPlayHarrierHunterCharge(void);
void audioPlayHarrierHunterFire(void);
void audioPlayHarrierDefeat(void);
void audioPlayJump(void);
void audioPlayCollect(void);
void audioPlayHealthCollect(void);
void audioPlayExtraLife(void);
#ifdef SPARKPAW_DROWNED_JOINED
void audioPlayCheckpoint(void);
#endif
void audioPlayWaterSplash(void);
void audioPlayStormstoneCore(void);
void audioPlayTallyTick(void);
void audioUpdate(void);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
void audioDiagnosticWrite(BPTR file);
#endif

#endif
