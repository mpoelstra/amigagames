#ifndef SPARKPAW_TITLE_H
#define SPARKPAW_TITLE_H

#include <exec/types.h>
#include "campaign_contract.h"
#include "control_options.h"

BOOL titleShow(void);
BOOL titleShowMain(void);
BOOL titleShowMainFromResults(void);
BOOL titlePrepareLevelLoading(void);
BOOL titleShowLevelLoading(void);
BOOL titleShowReplayLoading(void);
#ifdef SPARKPAW_MULTI_ADF
BOOL titleShowInsertDisk(UBYTE disk);
#endif
BOOL titleShowLevelCharging(void);
BOOL titleShowLevelReady(void);
BOOL titleShowLevelComplete(void);
void titleRunLevelComplete(UWORD enemies,UWORD diamonds,
                           ULONG elapsedFields,ULONG liveScore);
void titleRunLevelCompleteWithBonus(UWORD enemies,UWORD diamonds,
                                    UWORD timeBonusSeconds,ULONG liveScore);
enum ResultDecision titleRunLevelCompleteMenu(UWORD enemies,UWORD diamonds,
    ULONG elapsedFields,ULONG liveScore,BOOL stormrail);
enum ResultDecision titleRunLevelCompleteWithBonusMenu(UWORD enemies,
    UWORD diamonds,UWORD timeBonusSeconds,ULONG liveScore,BOOL stormrail);
void titleRunLevelReadyMenu(enum SecondaryButtonAction *secondaryAction,
    enum CampaignStartSection *startSection);
UWORD *titleCopperList(void);
void titleWaitLevelCharging(UWORD frames);
void titleFadeOut(void);
const char *titleFailureReason(void);
ULONG titleChipFree(void);
ULONG titleChipLargest(void);
void titleWaitFrames(UWORD frames);
void titleRestoreSystemView(void);
void titleRelease(void);

#endif
