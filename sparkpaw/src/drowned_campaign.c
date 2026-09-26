#include "whd_load_trace.h"
/* Separate, namespaced native engine. Same gameplay pipeline as full Drowned;
   the only public boundary is drownedCampaignRun and its immutable entry. */
#include "drowned_campaign.h"
#include "game.h"
#include "player.h"
#include "renderer.h"
#include "collision.h"
#include "audio.h"
#include "music.h"
#include "platform_amiga.h"
#include "title.h"
#if defined(SPARKPAW_FOUR_ADF)||defined(SPARKPAW_DROWNED_THREE_ADF)
#include "disk_media.h"
#endif
#include <proto/graphics.h>
#ifdef SPARKPAW_WHDLOAD_DIAG
#include <dos/dos.h>
#include <exec/memory.h>
#include <proto/dos.h>
#include <proto/exec.h>
static void whdDrownedTrace(const char *stage)
{
    BPTR file=Open("PROGDIR:whd-drowned-diag.log",MODE_READWRITE);
    if(!file) file=Open("PROGDIR:whd-drowned-diag.log",MODE_NEWFILE);
    if(!file) return;
    Seek(file,0,OFFSET_END);
    FPrintf(file,"stage=%s chip=%ld chip_largest=%ld fast=%ld fast_largest=%ld\n",
            (STRPTR)stage,(LONG)AvailMem(MEMF_CHIP),
            (LONG)AvailMem(MEMF_CHIP|MEMF_LARGEST),
            (LONG)AvailMem(MEMF_FAST),
            (LONG)AvailMem(MEMF_FAST|MEMF_LARGEST));
    Flush(file); Close(file);
}
#endif

static void retireDisplay(void)
{
    platformReleaseForLoading(FALSE);
    platformBeginTakeover(); /* Also retires a DOS-live result/loading display. */
    WaitTOF();
}
static void closeSection(void)
{
    retireDisplay();
    audioUnload(); rendererCleanup(); titleRelease(); platformClose();
}
#if defined(SPARKPAW_FOUR_ADF)||defined(SPARKPAW_DROWNED_THREE_ADF)
/* Transfer media back to the parent while this module still owns a visible
   DOS-live loading screen. Parent then revalidates disk 1 in its resolver. */
static int closeToParent(int outcome)
{
    retireDisplay(); audioUnload(); rendererCleanup(); titleRelease();
    if(!titleShowSectionLoading()||!diskMediaRequire(1)) {
        closeSection(); return DROWNED_CAMPAIGN_ERROR;
    }
    closeSection(); return outcome;
}
#else
#define closeToParent(outcome) (closeSection(),(outcome))
#endif
static void resetSection(const struct DrownedCampaignEntry *entry)
{
    gameInit(entry->seed);
    gameRestoreDrownedVitals(entry->lives,entry->health,entry->diamonds);
    playerSetControlMode((enum ControlMode)entry->controlMode);
}
static void startSection(void)
{
    platformResetGameInput();
    platformFinishTakeover(rendererCopperList());
    rendererUpdateGameplay();
    while(platformRasterLine()<300) { }
    while(platformRasterLine()>=300) { }
    platformSwitchCopper(rendererCopperList());
    titleRelease(); platformStartGameplayAudio();
}
int drownedCampaignRun(const struct DrownedCampaignEntry *entry)
{
    BOOL paused=FALSE,defeated;
    enum ResultDecision decision;
    const struct GameState *result;
#ifdef SPARKPAW_WHDLOAD_DIAG
    whdDrownedTrace("entry");
    if(!entry) { whdDrownedTrace("invalid_entry"); return DROWNED_CAMPAIGN_ERROR; }
    if(!platformOpen()) { whdDrownedTrace("platform_open_failed"); return DROWNED_CAMPAIGN_ERROR; }
    whdDrownedTrace("platform_opened");
#else
    if(!entry||!platformOpen()) return DROWNED_CAMPAIGN_ERROR;
#endif
#if defined(SPARKPAW_FOUR_ADF)||defined(SPARKPAW_DROWNED_THREE_ADF)
    if(!diskMediaRequire(
#ifdef SPARKPAW_DROWNED_THREE_ADF
        3
#else
        4
#endif
        )) { platformClose(); return DROWNED_CAMPAIGN_ERROR; }
#endif
    audioSetMode((enum AudioMode)entry->audioMode);
    resetSection(entry);
#ifdef SPARKPAW_WHDLOAD_DIAG
    if(!titleShowSectionLoading()) { whdDrownedTrace("loading_image_failed");
        closeSection(); return DROWNED_CAMPAIGN_ERROR; }
    whdDrownedTrace("loading_image_ready");
    if(!WLT_CALL(WLT_FILES,rendererLoadGameplay())) { whdDrownedTrace("gameplay_assets_failed");
        closeSection(); return DROWNED_CAMPAIGN_ERROR; }
    whdDrownedTrace("gameplay_assets_ready");
    if(!WLT_CALL(WLT_COLLISION,collisionLoad())) { whdDrownedTrace("collision_failed");
        closeSection(); return DROWNED_CAMPAIGN_ERROR; }
    whdDrownedTrace("collision_ready");
    if(!WLT_CALL(WLT_AUDIO,audioLoad())) { whdDrownedTrace("audio_failed");
        closeSection(); return DROWNED_CAMPAIGN_ERROR; }
    whdDrownedTrace("audio_ready");
    if(!WLT_CALL(WLT_RENDERER,rendererPrepareGameplay())) { whdDrownedTrace("renderer_prepare_failed");
        closeSection(); return DROWNED_CAMPAIGN_ERROR; }
    whdDrownedTrace("renderer_ready");
#else
    if(!titleShowSectionLoading()||!WLT_CALL(WLT_FILES,rendererLoadGameplay())||
       !WLT_CALL(WLT_COLLISION,collisionLoad())||!WLT_CALL(WLT_AUDIO,audioLoad())||!WLT_CALL(WLT_RENDERER,rendererPrepareGameplay())) {
        closeSection(); return DROWNED_CAMPAIGN_ERROR;
    }
#endif
    titleFadeOut(); startSection();
    for(;;) {
        while(!gameOver()&&!gameLevelComplete()) {
            BOOL left,right,down,jump,fire;
            platformReadGameKeys(&left,&right,&down,&jump,&fire);
#ifdef SPARKPAW_WHDLOAD
            if(platformWHDLoadQuitRequested()) {
                closeSection(); return DROWNED_CAMPAIGN_QUIT;
            }
#endif
            if(platformGameEscapeRequested()) {
                rendererFadeOut(); return closeToParent(DROWNED_CAMPAIGN_READY);
            }
            if(platformGamePauseToggleRequested()) paused=!paused;
            if(!paused) {
                gameUpdate();
                if(gameOver()||gameLevelComplete()) break;
                rendererUpdateGameplay(); rendererDrawGameplayBobs();
            }
            do {
                while(platformRasterLine()<300) { }
                while(platformRasterLine()>=300) { }
            } while(!paused&&!rendererPublishGameplay(platformRasterLine()));
        }
        result=gameState(); defeated=gameOver();
        if(defeated) rendererFadeOut();
        retireDisplay();
        if(defeated) { rendererCleanup(); audioUnload(); }
        if(!(defeated?titleShowGameOver(entry->bankedScore+result->score):
                     titleShowLevelComplete())) {
            closeSection(); return DROWNED_CAMPAIGN_ERROR;
        }
        platformResetGameInput(); platformFinishTakeover(titleCopperList());
        if(defeated) { titleRunGameOver(); decision=RESULT_DECISION_BACK_TO_TITLE; }
        else {
            /* Use the existing Level1 result policy (120s par), own run data,
               unchanged tally art, and Replay/Back to title final menu. */
            decision=titleRunLevelCompleteMenu(result->enemiesDefeated,
                result->diamondsCollected,result->elapsedFields,result->score,TRUE);
        }
        titleFadeOut(); platformReleaseForLoading(TRUE);
        if(decision!=RESULT_DECISION_REPLAY_CURRENT) {
            return closeToParent(DROWNED_CAMPAIGN_TITLE);
        }
        /* Resident replay: restore immutable entry, not completion vitals. */
        resetSection(entry); retireDisplay(); titleRelease(); rendererResetGameplay();
        paused=FALSE; startSection();
    }
}
