#ifndef SPARKPAW_LEVEL1_AUDIO_H
#define SPARKPAW_LEVEL1_AUDIO_H
#include <exec/types.h>
BOOL level1AudioLoad(BOOL stormrail);
void level1AudioUnload(void);
/* Start/stop require the platform's disabled interrupt ownership. */
BOOL level1AudioStart(void);
BOOL level1AudioStartMusic(void);
#if !defined(SPARKPAW_MULTI_ADF)||defined(SPARKPAW_FOUR_ADF)
BOOL level1AudioPreviewPrepare(BOOL stormrail);
#ifdef SPARKPAW_CAMPAIGN_DROWNED
BOOL level1AudioPreviewPrepareDrowned(void);
#endif
void level1AudioPreviewClear(void);
#endif
void level1AudioStop(void);
BOOL level1AudioRunning(void);
void level1AudioRequest(unsigned id);
void level1AudioUpdate(void);
#endif
