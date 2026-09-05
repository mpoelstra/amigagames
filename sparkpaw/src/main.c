#include <dos/dos.h>
#include <exec/memory.h>
#include <proto/dos.h>
#include <proto/exec.h>

#include "audio.h"
#include "assets.h"
#include "campaign_contract.h"
#include "collision.h"
#include "game.h"
#include "performance_profile.h"
#include "platform_amiga.h"
#include "player.h"
#include "renderer.h"
#include "stormrail_contract.h"
#include "title.h"
#include "world_config.h"
#include "disk_media.h"

enum AppState {
    APP_BOOT,
    APP_TITLE_LOADING,
    APP_TITLE_READY,
    APP_LEVEL_LOADING,
    APP_PLAYING,
    APP_LEVEL_COMPLETE,
    APP_RETURN_READY
};

static void cleanup(void)
{
    titleRestoreSystemView(); titleRelease();
    audioUnload();
    rendererCleanup();
    platformClose();
}

static BOOL loadLevelFiles(void)
{
#ifdef SPARKPAW_MULTI_ADF
    if(!diskMediaRequire(gameStormrailActive()?2:1)) return FALSE;
#endif
    return rendererLoadGameplay()&&collisionLoad()&&audioLoad();
}

#ifdef SPARKPAW_CAMPAIGN
static BOOL switchPreparedLevel1ToStormrail(struct CampaignState *campaign,
                                            ULONG enemySeed)
{
#ifdef SPARKPAW_MULTI_ADF
    if(!titleShowReplayLoading()) return FALSE;
#endif
    rendererCleanup(); audioUnload();
    gameSetStormrailActive(TRUE);
    assetsSetStormrailGameplay(TRUE);
    campaignStartAtStormrail(campaign,GAME_START_LIVES,PLAYER_MAX_HEALTH,0);
    gameInit(enemySeed);
    gameRestoreCampaignVitals(campaign->postLevel1Lives,
        campaign->postLevel1Health,campaign->postLevel1Diamonds);
    return loadLevelFiles()&&
#ifdef SPARKPAW_MULTI_ADF
           titleShowLevelCharging()&&
#endif
           rendererPrepareGameplay();
}
#endif

#if defined(SPARKPAW_BACK_TITLE_INTERACTIVE_PROOF)||\
    defined(SPARKPAW_CAMPAIGN_TRANSITION_TRACE)
static void writeBackTitleStage(const char *stage,BOOL final)
{
    BPTR file=Open(final?"PROGDIR:back-title-complete.log":
                         "PROGDIR:back-title-trace.log",MODE_READWRITE);
    if(!file) file=Open(final?"PROGDIR:back-title-complete.log":
                             "PROGDIR:back-title-trace.log",MODE_NEWFILE);
    if(!file) return;
    Seek(file,0,OFFSET_END);
    FPrintf(file,"stage=%s chip=%ld largest=%ld\n",(STRPTR)stage,
            (LONG)AvailMem(MEMF_CHIP),
            (LONG)AvailMem(MEMF_CHIP|MEMF_LARGEST));
    Flush(file); Close(file);
}
#endif

#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
static void writeStartupStage(const char *stage)
{
    BPTR file=Open("PROGDIR:startupdiag.log",MODE_READWRITE);
    if(!file) file=Open("PROGDIR:startupdiag.log",MODE_NEWFILE);
    if(!file) return;
    Seek(file,0,OFFSET_END);
    FPrintf(file,"stage=%s world_width=%ld\n",(STRPTR)stage,
            (LONG)SPARKPAW_WORLD_W);
    FPrintf(file,"chip_free=%ld chip_largest=%ld\n",
            (LONG)AvailMem(MEMF_CHIP),
            (LONG)AvailMem(MEMF_CHIP|MEMF_LARGEST));
    FPrintf(file,"fast_free=%ld fast_largest=%ld\n",
            (LONG)AvailMem(MEMF_FAST),
            (LONG)AvailMem(MEMF_FAST|MEMF_LARGEST));
    Flush(file);
    Close(file);
}
#endif

#ifdef PHASE6_MEMORY_TEST
static void writePhase6MemoryLog(ULONG beforeFree,ULONG beforeLargest)
{
    BPTR file=Open("PROGDIR:phase6-memory.log",MODE_NEWFILE);
    if(!file) return;
    FPrintf(file,"phase=6C.2-core-clearing world_width=%ld\n",
            (LONG)SPARKPAW_WORLD_W);
    FPrintf(file,"before_gameplay_free=%ld before_gameplay_largest=%ld\n",
            (LONG)beforeFree,(LONG)beforeLargest);
    FPrintf(file,"peak_chip_free=%ld peak_chip_largest=%ld\n",
            (LONG)rendererPhase6PeakChipFree(),
            (LONG)rendererPhase6PeakChipLargest());
    FPrintf(file,"peak_fast_free=%ld peak_fast_largest=%ld\n",
            (LONG)rendererPhase6PeakFastFree(),
            (LONG)rendererPhase6PeakFastLargest());
    FPrintf(file,"prepared_chip_free=%ld prepared_chip_largest=%ld\n",
            (LONG)AvailMem(MEMF_CHIP),
            (LONG)AvailMem(MEMF_CHIP|MEMF_LARGEST));
    FPrintf(file,"prepared_fast_free=%ld prepared_fast_largest=%ld\n",
            (LONG)AvailMem(MEMF_FAST),
            (LONG)AvailMem(MEMF_FAST|MEMF_LARGEST));
    Close(file);
}
#endif

int main(void)
{
    enum AppState state=APP_BOOT;
    struct DateStamp levelTime;
    ULONG enemySeed;
#ifdef PHASE6_MEMORY_TEST
    ULONG phase6BeforeFree,phase6BeforeLargest;
#endif
    BOOL loadingShown;
    enum SecondaryButtonAction secondaryButtonAction=SECONDARY_BUTTON_JUMP;
    enum CampaignStartSection startSection=CAMPAIGN_START_STORM_RUINS;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    BOOL prototypePublished;
#endif
#ifdef SPARKPAW_REPLAY_PROOF
    UBYTE replayProofCompletions=0;
#endif
    BOOL platformReady=platformOpen();
#ifdef SPARKPAW_CAMPAIGN
    struct CampaignState campaign;
    campaignReset(&campaign);
#ifdef SPARKPAW_MULTI_ADF
    if(!platformReady||!diskMediaRequire(1)) { cleanup(); return 10; }
#endif
    gameSetStormrailActive(FALSE);
    assetsSetStormrailGameplay(FALSE);
#endif
#ifdef SPARKPAW_BACK_TITLE_INTERACTIVE_PROOF
    gameSetStormrailActive(TRUE);
    assetsSetStormrailGameplay(TRUE);
    gameInit(0x53504157UL);
    if(!platformReady||!loadLevelFiles()||!rendererPrepareGameplay()||
       !titleShowLevelComplete()) { cleanup(); return 10; }
    platformFinishTakeover(titleCopperList());
    if(titleRunLevelCompleteWithBonusMenu(0,0,0,0,TRUE)!=
       RESULT_DECISION_BACK_TO_TITLE) { cleanup(); return 10; }
    titleFadeOut(); platformReleaseForLoading(TRUE);
    writeBackTitleStage("decision_back",FALSE);
    rendererCleanup(); audioUnload();
    writeBackTitleStage("renderer_clean",FALSE);
    gameSetStormrailActive(FALSE); assetsSetStormrailGameplay(FALSE);
    campaignReset(&campaign);
    if(!titleShowMainFromResults()) { cleanup(); return 10; }
    writeBackTitleStage("title_visible",FALSE);
    if(!titlePrepareLevelLoading()) { cleanup(); return 10; }
    titleWaitFrames(225); gameInit(0x53504157UL);
    if(!titleShowLevelLoading()) { cleanup(); return 10; }
    writeBackTitleStage("loading_visible",FALSE);
    if(!loadLevelFiles()) { cleanup(); return 10; }
    writeBackTitleStage("level1_files_loaded",FALSE);
    if(!titleShowLevelCharging()||!rendererPrepareGameplay()) {
        cleanup(); return 10;
    }
    writeBackTitleStage("level1_renderer_ready",FALSE);
    titleWaitLevelCharging(100);
    if(!titleShowLevelReady()) { cleanup(); return 10; }
    writeBackTitleStage("ready_visible",FALSE);
    platformFinishTakeover(titleCopperList());
    titleRunLevelReadyMenu(&secondaryButtonAction,&startSection);
    platformReleaseForLoading(TRUE);
    writeBackTitleStage("start_game_pressed",TRUE);
    for(;;) { }
#endif
#if defined(SPARKPAW_STORMRAIL_FINALE_PROOF)&&!defined(SPARKPAW_CAMPAIGN)
    if(!platformReady) {
        PutStr("Sparkpaw: platform unavailable.\n");
        cleanup(); return 10;
    }
    DateStamp(&levelTime);
    enemySeed=(ULONG)levelTime.ds_Days*86400UL+
              (ULONG)levelTime.ds_Minute*60UL+(ULONG)levelTime.ds_Tick;
    gameInit(enemySeed^0x53504157UL);
    if(!loadLevelFiles()||!rendererPrepareGameplay()) {
        PutStr("Sparkpaw: Gate-6 runtime unavailable.\n");
        cleanup(); return 10;
    }
    platformFinishTakeover(rendererCopperList());
    rendererUpdateGameplay();
    platformSwitchCopper(rendererCopperList());
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    platformProfileTimerStart();
#endif
    state=APP_PLAYING;
#else
    state=APP_TITLE_LOADING;
    if(!platformReady||!titleShow()) {
        PutStr("Sparkpaw: title assets or display unavailable.\n");
        if(platformReady) {
            PutStr((STRPTR)titleFailureReason()); PutStr("\n");
            Printf("Chip %ld, largest %ld\n",
                   titleChipFree(),titleChipLargest());
        }
        cleanup(); return 10;
    }
    state=APP_TITLE_READY;
    if(!titlePrepareLevelLoading()) {
        PutStr("Sparkpaw: loading image unavailable.\n");
        PutStr((STRPTR)titleFailureReason()); PutStr("\n");
        cleanup(); return 10;
    }
#if defined(SPARKPAW_EXTRA_LIFE_VISUAL_PROOF)||defined(SPARKPAW_REPLAY_PROOF)||\
    (defined(SPARKPAW_STORMRAIL_PROOF)&&!defined(SPARKPAW_CAMPAIGN))
    titleWaitFrames(1);
#else
    titleWaitFrames(225);
#endif
    DateStamp(&levelTime);
    enemySeed=(ULONG)levelTime.ds_Days*86400UL+
              (ULONG)levelTime.ds_Minute*60UL+(ULONG)levelTime.ds_Tick;
    gameInit(enemySeed^0x53504157UL);
    state=APP_LEVEL_LOADING;
    loadingShown=titleShowLevelLoading();
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    writeStartupStage("level_loading_visible");
#endif
#ifdef PHASE6_MEMORY_TEST
    phase6BeforeFree=AvailMem(MEMF_CHIP);
    phase6BeforeLargest=AvailMem(MEMF_CHIP|MEMF_LARGEST);
#endif
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    if(!loadingShown) {
        writeStartupStage("failed_level_loading_display");
    } else if(!loadLevelFiles()) {
        writeStartupStage("failed_gameplay_assets_collision_or_audio");
        loadingShown=FALSE;
    } else if(!titleShowLevelCharging()) {
        writeStartupStage("failed_charging_display");
        loadingShown=FALSE;
    } else {
        writeStartupStage("charging_visible_before_renderer_prepare");
        if(!rendererPrepareGameplay()) loadingShown=FALSE;
    }
    if(!loadingShown) {
        PutStr("Sparkpaw: runtime assets or Chip RAM unavailable.\n");
        cleanup(); return 10;
    }
#else
    if(!loadingShown||!loadLevelFiles()||!titleShowLevelCharging()||
       !rendererPrepareGameplay()) {
        PutStr("Sparkpaw: runtime assets or Chip RAM unavailable.\n");
        if(!loadingShown) {
            PutStr((STRPTR)titleFailureReason()); PutStr("\n");
        }
        cleanup(); return 10;
    }
#endif
#ifdef PHASE6_MEMORY_TEST
    writePhase6MemoryLog(phase6BeforeFree,phase6BeforeLargest);
#endif
    /* Keep the second status readable even when preparation finishes quickly
       on Fast RAM systems or accelerated/emulated CPUs. */
#if defined(SPARKPAW_EXTRA_LIFE_VISUAL_PROOF)||defined(SPARKPAW_REPLAY_PROOF)||\
    (defined(SPARKPAW_STORMRAIL_PROOF)&&!defined(SPARKPAW_CAMPAIGN))
    titleWaitLevelCharging(1);
#else
    titleWaitLevelCharging(100);
#endif
    if(!titleShowLevelReady()) {
        PutStr("Sparkpaw: ready screen unavailable.\n");
        PutStr((STRPTR)titleFailureReason()); PutStr("\n");
        cleanup(); return 10;
    }
    /* All DOS work and gameplay preparation are complete. Own the machine
       while the ready screen remains displayed so Space shares the gameplay
       raw-key path with joystick Fire. */
    platformFinishTakeover(titleCopperList());
#if !defined(SPARKPAW_EXTRA_LIFE_VISUAL_PROOF)&&!defined(SPARKPAW_REPLAY_PROOF)&&\
    (!defined(SPARKPAW_STORMRAIL_PROOF)||defined(SPARKPAW_CAMPAIGN))
    titleRunLevelReadyMenu(&secondaryButtonAction,&startSection);
#endif
#ifdef SPARKPAW_WHDLOAD
    if(platformWHDLoadQuitRequested()) {
        platformRestore(); cleanup(); return 0;
    }
#endif
    playerSetSecondaryButtonAction(secondaryButtonAction);
    titleFadeOut();
#ifdef SPARKPAW_CAMPAIGN
    if(startSection==CAMPAIGN_START_STORMRAIL) {
        platformReleaseForLoading(TRUE);
        DateStamp(&levelTime);
        enemySeed=(ULONG)levelTime.ds_Days*86400UL+
                  (ULONG)levelTime.ds_Minute*60UL+(ULONG)levelTime.ds_Tick;
        if(!switchPreparedLevel1ToStormrail(&campaign,
                enemySeed^0x53504157UL)) {
            PutStr("Sparkpaw: selected Stormrail start failed.\n");
            cleanup(); return 10;
        }
        platformResetGameInput();
        platformFinishTakeover(rendererCopperList());
    }
#endif
    rendererUpdateGameplay();
    platformSwitchCopper(rendererCopperList());
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    platformProfileTimerStart();
#endif
    titleRelease();
    state=APP_PLAYING;
#endif
    for(;;) {
    while(state==APP_PLAYING) {
        ULONG profileStart;
#ifdef SPARKPAW_WHDLOAD
        BOOL ignoredLeft,ignoredRight,ignoredDown,ignoredJump,ignoredFire;
        /* Poll once even while a gameplay state temporarily skips player
           input. The later player poll reads the same cached key state. */
        platformReadGameKeys(&ignoredLeft,&ignoredRight,&ignoredDown,
                             &ignoredJump,&ignoredFire);
        if(platformWHDLoadQuitRequested()) {
            state=APP_BOOT;
            break;
        }
#endif
#ifdef SPARKPAW_UPDATE_LINE100_REFERENCE
        while(platformRasterLine()<100) { }
#endif
/* Whole-display captures belong only to focused Stormrail proof executables.
 * A campaign already has the real playable route and must not make Level 1
 * traverse proof-only finale/timer branches every frame. */
#if defined(SPARKPAW_STORMRAIL_PROOF) && !defined(SPARKPAW_CAMPAIGN)
#ifdef SPARKPAW_STORMRAIL_AUTO_BOARD
#ifdef SPARKPAW_STORMRAIL_FINALE_PROOF
        if(gameState()->stormrailFinaleActive&&
           gameState()->stormrailFinalePhase==STORMRAIL_FINALE_PHASE_COMBAT) {
            if(gameState()->stormrailFinaleTick==13)
                rendererCaptureStormrailProof(0);
            else if(gameState()->stormrailFinaleTick==33)
                rendererCaptureStormrailProof(1);
            else if(gameState()->stormrailFinaleTick==77)
                rendererCaptureStormrailProof(2);
            else if(gameState()->stormrailFinaleTick==97)
                rendererCaptureStormrailProof(3);
            else if(gameState()->stormrailFinaleTick==109)
                rendererCaptureStormrailProof(4);
            else if(gameState()->stormrailFinaleTick==121)
                rendererCaptureStormrailProof(5);
        }
#else
        if(gameState()->frameCounter==305)
            rendererCaptureStormrailProof(0);
        else if(gameState()->frameCounter==360)
            rendererCaptureStormrailProof(1);
        else if(gameState()->frameCounter==385)
            rendererCaptureStormrailProof(2);
        else if(gameState()->frameCounter==430)
            rendererCaptureStormrailProof(3);
        else if(gameState()->frameCounter==500)
            rendererCaptureStormrailProof(4);
        else if(gameState()->frameCounter==2075)
            rendererCaptureStormrailProof(5);
        else if(gameState()->frameCounter==2125)
            rendererCaptureStormrailProof(6);
        else if(gameState()->frameCounter==2160)
            rendererCaptureStormrailProof(7);
#endif
#else
        if(gameState()->frameCounter==150)
            rendererCaptureStormrailProof(0);
        else if(gameState()->frameCounter==325)
            rendererCaptureStormrailProof(1);
        else if(gameState()->frameCounter==700)
            rendererCaptureStormrailProof(2);
#endif
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        rendererDiagnosticUpdateEntry(platformRasterLine());
#endif
        profileStart=performanceProfileBegin();
        gameUpdate();
        performanceProfileEnd(PERF_GAME_UPDATE,profileStart);
#ifdef SPARKPAW_CAMPAIGN
        if(platformGameEscapeRequested()) {
            state=APP_RETURN_READY;
            break;
        }
#endif
#ifdef SPARKPAW_CAMPAIGN_CADENCE_TEST
        /* Keep the completed Stormrail frame resident until LMB ends the
           sample below. Production campaign builds still enter results. */
        if(gameLevelComplete()&&gameStormrailActive()) {
        } else
#elif defined(SPARKPAW_LEVEL1_CADENCE_TEST)
        /* Keep the completed Level-1 frame resident until LMB ends the
           bounded sample. Production campaign builds still enter results. */
        if(gameLevelComplete()) {
        } else
#endif
        if(gameLevelComplete()
#ifdef SPARKPAW_REPLAY_PROOF
           ||(!replayProofCompletions&&gameState()->frameCounter>5)
#endif
           ) {
#ifdef SPARKPAW_REPLAY_PROOF
            replayProofCompletions++;
#endif
            state=APP_LEVEL_COMPLETE;
            break;
        }
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        rendererDiagnosticPublicationEntry(platformRasterLine());
#endif
        profileStart=performanceProfileBegin();
        rendererUpdateGameplay();
        performanceProfileEnd(PERF_COPPER_PATCH,profileStart);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        rendererDiagnosticPublicationExit(platformRasterLine());
#endif
#ifndef SPARKPAW_ROLLING_PROTOTYPE
        while(platformRasterLine()<253) { }
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        rendererDiagnosticBobEntry(platformRasterLine());
#endif
        profileStart=performanceProfileBegin();
        rendererDrawGameplayBobs();
        performanceProfileEnd(PERF_BOB_PASS,profileStart);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        rendererDiagnosticBobExit(platformRasterLine());
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
        /* A Bob pass may have crossed one or several fields. Never interpret
           its final line number as belonging to the original frame. Wait for
           one explicit new PAL boundary and restart the complete inactive
           Copper list at line zero, leaving its full pre-display setup time. */
        profileStart=performanceProfileBegin();
        do {
            while(platformRasterLine()<300) { }
            while(platformRasterLine()>=300) { }
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
            prototypePublished=rendererPublishGameplay(platformRasterLine());
#else
            prototypePublished=rendererPublishGameplay(platformRasterLine());
#endif
        } while(!prototypePublished);
        performanceProfileEnd(PERF_PUBLISH_WAIT,profileStart);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        rendererDiagnosticBoundary(platformRasterLine(),prototypePublished);
#endif
#else
        while(platformRasterLine()>=253) { }
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        rendererDiagnosticBoundary(platformRasterLine(),TRUE);
#endif
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        if(platformLeftMouse()) state=APP_BOOT;
#endif
#ifdef SPARKPAW_EXTRA_LIFE_VISUAL_PROOF
        if(gameState()->frameCounter>90) {
            state=APP_BOOT;
            break;
        }
#endif
#ifdef SPARKPAW_REPLAY_PROOF
        if(replayProofCompletions&&gameState()->frameCounter>10) {
            BPTR proof;
            platformRestore();
            proof=Open("PROGDIR:replay-proof.log",MODE_NEWFILE);
            if(proof) {
                FPrintf(proof,"replay=resident frame=%ld camera=%ld collectibles=%s\n",
                        (LONG)gameState()->frameCounter,
                        (LONG)gameState()->cameraX,
                        rendererReplayPresentationValid()?"valid":"invalid");
                FPrintf(proof,"chip_free=%ld chip_largest=%ld\n",
                        (LONG)AvailMem(MEMF_CHIP),
                        (LONG)AvailMem(MEMF_CHIP|MEMF_LARGEST));
                Close(proof);
            }
            cleanup();
            return proof&&rendererReplayPresentationValid()?0:10;
        }
#endif
#if defined(SPARKPAW_STORMRAIL_PROOF) && !defined(SPARKPAW_CAMPAIGN_PLAY)
#if defined(SPARKPAW_STORMRAIL_USER_TEST) && \
    !defined(SPARKPAW_STORMRAIL_USER_CADENCE_TEST)
        /* This focused visual build is reset/stopped from FS-UAE. Returning
           to Workbench while LMB is still held can turn the release click
           into an accidental icon drag, moving the executable into assets/. */
        (void)platformLeftMouse();
#else
#ifdef SPARKPAW_STORMRAIL_AUTO_BOARD
#ifdef SPARKPAW_STORMRAIL_WRAP_PROOF
        if(gameState()->frameCounter>900) {
#elif defined(SPARKPAW_STORMRAIL_FINALE_PROOF)
        if(gameState()->stormrailFinalePhase==
           STORMRAIL_FINALE_PHASE_COMPLETE) {
#elif defined(SPARKPAW_STORMRAIL_CADENCE_PROOF)
        if(gameState()->frameCounter>1200) {
#else
        if(gameState()->frameCounter>4300) {
#endif
            state=APP_BOOT;
            break;
        }
#else
#ifndef SPARKPAW_STORMRAIL_USER_CADENCE_TEST
        if(gameState()->stormrailMode==STORMRAIL_MODE_FLIGHT&&
           gameState()->stormrailLaunchTimer>STORMRAIL_CADENCE_END) {
            state=APP_BOOT;
            break;
        }
#endif
#endif
#endif
#endif
    }
    if(state==APP_RETURN_READY) {
#ifdef SPARKPAW_CAMPAIGN
        /* Escape abandons the active run. Re-enter through the existing
           presentation/load lifecycle without replaying the story intro, and
           wait at START GAME with a fresh Level-1 preload. */
        platformReleaseForLoading(FALSE);
        rendererCleanup(); audioUnload();
        gameSetStormrailActive(FALSE);
        assetsSetStormrailGameplay(FALSE);
        campaignReset(&campaign);
        startSection=CAMPAIGN_START_STORM_RUINS;
        if(!titleShowMain()||!titlePrepareLevelLoading()) {
            PutStr("Sparkpaw: Escape ready restart unavailable.\n");
            cleanup(); return 10;
        }
        DateStamp(&levelTime);
        enemySeed=(ULONG)levelTime.ds_Days*86400UL+
                  (ULONG)levelTime.ds_Minute*60UL+(ULONG)levelTime.ds_Tick;
        gameInit(enemySeed^0x53504157UL);
        if(!titleShowLevelLoading()||!loadLevelFiles()||
           !titleShowLevelCharging()||!rendererPrepareGameplay()) {
            PutStr("Sparkpaw: Escape Level-1 preload failed.\n");
            cleanup(); return 10;
        }
        titleWaitLevelCharging(100);
        if(!titleShowLevelReady()) {
            PutStr("Sparkpaw: Escape ready screen unavailable.\n");
            cleanup(); return 10;
        }
        platformResetGameInput();
        platformFinishTakeover(titleCopperList());
        titleRunLevelReadyMenu(&secondaryButtonAction,&startSection);
        playerSetSecondaryButtonAction(secondaryButtonAction);
        titleFadeOut();
        if(startSection==CAMPAIGN_START_STORMRAIL) {
            platformReleaseForLoading(TRUE);
            DateStamp(&levelTime);
            enemySeed=(ULONG)levelTime.ds_Days*86400UL+
                      (ULONG)levelTime.ds_Minute*60UL+
                      (ULONG)levelTime.ds_Tick;
            if(!switchPreparedLevel1ToStormrail(&campaign,
                    enemySeed^0x53504157UL)) {
                PutStr("Sparkpaw: selected Stormrail start failed.\n");
                cleanup(); return 10;
            }
            platformResetGameInput();
            platformFinishTakeover(rendererCopperList());
        }
        rendererUpdateGameplay();
        platformSwitchCopper(rendererCopperList());
        titleRelease(); state=APP_PLAYING;
        continue;
#else
        state=APP_BOOT;
#endif
    }
    if(state==APP_LEVEL_COMPLETE) {
        const struct GameState *result=gameState();
#ifdef SPARKPAW_CAMPAIGN
        BOOL stormrail=gameStormrailActive();
        enum ResultDecision decision;
#endif
#ifdef SPARKPAW_STORMRAIL_RESULTS_TEST
        const struct StormrailResultsSnapshot *stormResult=
            &result->stormrailResults;
        UWORD enemies=stormResult->enemies;
        UWORD diamonds=stormResult->diamonds;
        ULONG score=stormResult->liveSectionScore;
#elif !defined(SPARKPAW_CAMPAIGN)
        UWORD enemies=result->enemiesDefeated;
        UWORD diamonds=result->diamondsCollected;
        ULONG elapsed=result->elapsedFields;
        ULONG score=result->score;
#endif
        platformReleaseForLoading(FALSE);
        if(!titleShowLevelComplete()) {
            PutStr("Sparkpaw: level-complete screen unavailable.\n");
            PutStr((STRPTR)titleFailureReason()); PutStr("\n");
            platformRestore(); titleRelease(); audioUnload();
            platformClose(); return 10;
        }
        /* Results own a fresh input context. In particular, a W/jump used to
           collect the Level-1 Core must not arrive as an Up menu edge. */
        platformResetGameInput();
        platformFinishTakeover(titleCopperList());
#ifdef SPARKPAW_CAMPAIGN
        campaignBeginResults(&campaign);
        if(stormrail) {
            const struct StormrailResultsSnapshot *stormResult=
                &result->stormrailResults;
            decision=titleRunLevelCompleteWithBonusMenu(
                stormResult->enemies,stormResult->diamonds,
                stormResult->timeBonusSeconds,stormResult->liveSectionScore,
                TRUE);
            decision=campaignAcceptDecision(&campaign,decision,
                stormResult->totalScore,result->lives,
                result->stormrailHealth,result->diamonds);
        } else {
            ULONG elapsedSeconds=result->elapsedFields/50UL;
            ULONG level1Total=result->score+
                (elapsedSeconds<120UL?(120UL-elapsedSeconds)*10UL:0UL);
            decision=titleRunLevelCompleteMenu(result->enemiesDefeated,
                result->diamondsCollected,result->elapsedFields,result->score,
                FALSE);
            decision=campaignAcceptDecision(&campaign,decision,level1Total,
                result->lives,playerState()->health,result->diamonds);
        }
#elif defined(SPARKPAW_STORMRAIL_RESULTS_TEST)
        titleRunLevelCompleteWithBonus(enemies,diamonds,
            stormResult->timeBonusSeconds,score);
#else
        titleRunLevelComplete(enemies,diamonds,elapsed,score);
#endif
        /* Fade the custom score display completely before restoring the
           resident gameplay targets. The rejected visible-score reload proved
           that renderer mutation must never overlap a bright score frame. */
        titleFadeOut();
        /* Keep the now-black Raster/Copper display alive while releasing DOS
           and Blitter ownership. Both the resident loading presentation and
           BACK TO TITLE replace this Copper directly; disabling display DMA
           here made the loading disk glitch and left the restarted title
           black even though its list had been installed successfully. */
        platformReleaseForLoading(TRUE);
        platformResetGameInput();
#ifdef SPARKPAW_CAMPAIGN
        if(decision==RESULT_DECISION_CONTINUE) {
            if(!titleShowReplayLoading()) {
                PutStr("Sparkpaw: Stormrail loading screen unavailable.\n");
                cleanup(); return 10;
            }
            rendererCleanup(); audioUnload();
            gameSetStormrailActive(TRUE);
            assetsSetStormrailGameplay(TRUE);
            DateStamp(&levelTime);
            enemySeed=(ULONG)levelTime.ds_Days*86400UL+
                      (ULONG)levelTime.ds_Minute*60UL+(ULONG)levelTime.ds_Tick;
            gameInit(enemySeed^0x53504157UL);
            gameRestoreCampaignVitals(campaign.postLevel1Lives,
                campaign.postLevel1Health,campaign.postLevel1Diamonds);
            if(!loadLevelFiles()||
#ifdef SPARKPAW_MULTI_ADF
               !titleShowLevelCharging()||
#endif
               !rendererPrepareGameplay()) {
                PutStr("Sparkpaw: Stormrail transition load failed.\n");
                cleanup(); return 10;
            }
            titleFadeOut(); titleRelease();
            platformResetGameInput();
            platformFinishTakeover(rendererCopperList());
            rendererUpdateGameplay();
            while(platformRasterLine()<300) { }
            while(platformRasterLine()>=300) { }
            platformSwitchCopper(rendererCopperList());
            state=APP_PLAYING;
            continue;
        }
        if(decision==RESULT_DECISION_BACK_TO_TITLE) {
            /* Retire the completed renderer while its Blitter/DMA lifecycle
               is still in the same released state as the proven native flow.
               Publishing the title first disables Blitter DMA and can leave
               this cleanup waiting forever on real Amiga timing. */
#ifdef SPARKPAW_CAMPAIGN_TRANSITION_TRACE
            writeBackTitleStage("decision_back",FALSE);
#endif
            rendererCleanup(); audioUnload();
#ifdef SPARKPAW_CAMPAIGN_TRANSITION_TRACE
            writeBackTitleStage("renderer_clean",FALSE);
#endif
            gameSetStormrailActive(FALSE);
            assetsSetStormrailGameplay(FALSE);
            campaignReset(&campaign);
            if(!titleShowMainFromResults()||!titlePrepareLevelLoading()) {
                PutStr("Sparkpaw: title restart unavailable.\n");
                cleanup(); return 10;
            }
#ifdef SPARKPAW_CAMPAIGN_TRANSITION_TRACE
            writeBackTitleStage("title_visible",FALSE);
#endif
            titleWaitFrames(225);
            DateStamp(&levelTime);
            enemySeed=(ULONG)levelTime.ds_Days*86400UL+
                      (ULONG)levelTime.ds_Minute*60UL+(ULONG)levelTime.ds_Tick;
            gameInit(enemySeed^0x53504157UL);
            if(!titleShowLevelLoading()) {
                PutStr("Sparkpaw: Level 1 loading display failed.\n");
                cleanup(); return 10;
            }
#ifdef SPARKPAW_CAMPAIGN_TRANSITION_TRACE
            writeBackTitleStage("loading_visible",FALSE);
#endif
            if(!loadLevelFiles()) {
                PutStr("Sparkpaw: Level 1 files failed.\n");
                cleanup(); return 10;
            }
#ifdef SPARKPAW_CAMPAIGN_TRANSITION_TRACE
            writeBackTitleStage("level1_files_loaded",FALSE);
#endif
            if(!titleShowLevelCharging()||!rendererPrepareGameplay()) {
                PutStr("Sparkpaw: Level 1 restart load failed.\n");
                cleanup(); return 10;
            }
#ifdef SPARKPAW_CAMPAIGN_TRANSITION_TRACE
            writeBackTitleStage("level1_renderer_ready",FALSE);
#endif
            titleWaitLevelCharging(100);
            if(!titleShowLevelReady()) { cleanup(); return 10; }
#ifdef SPARKPAW_CAMPAIGN_TRANSITION_TRACE
            writeBackTitleStage("ready_visible",FALSE);
#endif
            platformFinishTakeover(titleCopperList());
            startSection=CAMPAIGN_START_STORM_RUINS;
            titleRunLevelReadyMenu(&secondaryButtonAction,&startSection);
            playerSetSecondaryButtonAction(secondaryButtonAction);
            titleFadeOut();
            if(startSection==CAMPAIGN_START_STORMRAIL) {
                platformReleaseForLoading(TRUE);
                DateStamp(&levelTime);
                enemySeed=(ULONG)levelTime.ds_Days*86400UL+
                          (ULONG)levelTime.ds_Minute*60UL+
                          (ULONG)levelTime.ds_Tick;
                if(!switchPreparedLevel1ToStormrail(&campaign,
                        enemySeed^0x53504157UL)) {
                    PutStr("Sparkpaw: selected Stormrail start failed.\n");
                    cleanup(); return 10;
                }
                platformResetGameInput();
                platformFinishTakeover(rendererCopperList());
            }
            rendererUpdateGameplay();
            platformSwitchCopper(rendererCopperList());
            titleRelease(); state=APP_PLAYING;
            continue;
        }
#endif
        DateStamp(&levelTime);
        enemySeed=(ULONG)levelTime.ds_Days*86400UL+
                  (ULONG)levelTime.ds_Minute*60UL+(ULONG)levelTime.ds_Tick;
        gameInit(enemySeed^0x53504157UL);
#ifdef SPARKPAW_CAMPAIGN
        if(stormrail) gameRestoreCampaignVitals(campaign.postLevel1Lives,
            campaign.postLevel1Health,campaign.postLevel1Diamonds);
#endif
        titleRelease();
        rendererResetGameplay();
        /* Drop keyboard state accumulated while DOS and the loader owned the
           machine. playerInit() separately clears joystick/action edges. */
        platformResetGameInput();
        platformFinishTakeover(rendererCopperList());
        rendererUpdateGameplay();
        /* COPJMP1 restarts a complete gameplay list. Arm it immediately after
           PAL wrap so no lower-screen wait from a partial list can briefly
           affect the outgoing score frame. */
        while(platformRasterLine()<300) { }
        while(platformRasterLine()>=300) { }
        platformSwitchCopper(rendererCopperList());
        state=APP_PLAYING;
        continue;
    }
    break;
    }
#ifdef SPARKPAW_EXTRA_LIFE_VISUAL_PROOF
    platformRestore();
    rendererWriteExtraLifeProof();
    cleanup();
    return 0;
#endif
#if defined(SPARKPAW_STORMRAIL_PROOF) && !defined(SPARKPAW_CAMPAIGN_PLAY)
    {
        const struct GameState *proofState=gameState();
        BPTR proof;
#ifdef SPARKPAW_STORMRAIL_USER_CADENCE_TEST
        while(platformLeftMouse()) { }
        platformPrepareDebugFlush();
        rendererWriteDiagnosticLog();
        for(;;) { }
#endif
#ifdef SPARKPAW_LEVEL1_CADENCE_TEST
        while(platformLeftMouse()) { }
        platformPrepareDebugFlush();
        rendererWriteDiagnosticLog();
        for(;;) { }
#endif
        platformRestore();
        rendererWriteStormrailProofFrames();
#ifdef SPARKPAW_STORMRAIL_CADENCE_PROOF
        rendererWriteDiagnosticLog();
#endif
        proof=Open("PROGDIR:stormrail-proof.log",MODE_NEWFILE);
        if(proof) {
            FPrintf(proof,"proof=stormrail-production-renderer-v2 mode=%ld frame=%ld camera=%ld score=%ld\n",
                    (LONG)proofState->stormrailMode,
                    (LONG)proofState->frameCounter,
                    (LONG)proofState->cameraX,(LONG)proofState->score);
            FPrintf(proof,"gate=6 enemies=%ld hostile=%ld rewards=%ld hazards=0 corridors=0 player_shots=%ld\n",
                    (LONG)STORMRAIL_MAX_ENEMIES,
                    (LONG)STORMRAIL_MAX_HOSTILE_SHOTS,
                    (LONG)STORMRAIL_MAX_REWARDS,
                    (LONG)STORMRAIL_MAX_SHOTS);
            FPrintf(proof,"open_bounds=%ld..%ld,%ld..%ld health=%ld impacts=%ld\n",
                    (LONG)STORMRAIL_FLIGHT_MIN_X,
                    (LONG)STORMRAIL_FLIGHT_MAX_X,
                    (LONG)STORMRAIL_FLIGHT_MIN_Y,
                    (LONG)STORMRAIL_FLIGHT_MAX_Y,
                    (LONG)proofState->stormrailHealth,
                    (LONG)proofState->stormrailCollisions);
            FPrintf(proof,"flight_x=%ld flight_y=%ld unsafe_blits=%ld\n",
                    (LONG)proofState->stormrailX,(LONG)proofState->stormrailY,
                    (LONG)rendererStormrailUnsafeBlits());
            FPrintf(proof,"controlled_x=%ld..%ld\n",
                    (LONG)proofState->stormrailMinControlledX,
                    (LONG)proofState->stormrailMaxControlledX);
            FPrintf(proof,"flight_distance=%ld rear_scroll=%ld rear_loops=%ld\n",
                    (LONG)proofState->stormrailDistance,
                    (LONG)stormrailRearScroll(proofState->stormrailDistance),
                    (LONG)((proofState->stormrailDistance>>
                        STORMRAIL_REAR_PARALLAX_SHIFT)/STORMRAIL_REAR_LOOP_W));
            FPrintf(proof,"formations=%ld/%ld/%ld/%ld/%ld/%ld/%ld/%ld alive=%ld/%ld/%ld/%ld/%ld/%ld/%ld/%ld escaped=%ld/%ld/%ld/%ld/%ld/%ld/%ld/%ld diamonds=%ld\n",
                    (LONG)proofState->stormrailFormationSpawned[0],
                    (LONG)proofState->stormrailFormationSpawned[1],
                    (LONG)proofState->stormrailFormationSpawned[2],
                    (LONG)proofState->stormrailFormationSpawned[3],
                    (LONG)proofState->stormrailFormationSpawned[4],
                    (LONG)proofState->stormrailFormationSpawned[5],
                    (LONG)proofState->stormrailFormationSpawned[6],
                    (LONG)proofState->stormrailFormationSpawned[7],
                    (LONG)proofState->stormrailFormationAlive[0],
                    (LONG)proofState->stormrailFormationAlive[1],
                    (LONG)proofState->stormrailFormationAlive[2],
                    (LONG)proofState->stormrailFormationAlive[3],
                    (LONG)proofState->stormrailFormationAlive[4],
                    (LONG)proofState->stormrailFormationAlive[5],
                    (LONG)proofState->stormrailFormationAlive[6],
                    (LONG)proofState->stormrailFormationAlive[7],
                    (LONG)proofState->stormrailFormationEscaped[0],
                    (LONG)proofState->stormrailFormationEscaped[1],
                    (LONG)proofState->stormrailFormationEscaped[2],
                    (LONG)proofState->stormrailFormationEscaped[3],
                    (LONG)proofState->stormrailFormationEscaped[4],
                    (LONG)proofState->stormrailFormationEscaped[5],
                    (LONG)proofState->stormrailFormationEscaped[6],
                    (LONG)proofState->stormrailFormationEscaped[7],
                    (LONG)proofState->diamonds);
            FPrintf(proof,"obstacle_spawn_mask=%ld/%ld peak=%ld big_peak=%ld active=%ld/%ld/%ld/%ld/%ld/%ld frames=%ld/%ld/%ld/%ld/%ld/%ld awards=%ld/%ld\n",
                    (LONG)proofState->stormrailObstacleSpawnMask,
                    (LONG)proofState->stormrailObstacleSpawnMaskHigh,
                    (LONG)proofState->stormrailObstaclePeak,
                    (LONG)proofState->stormrailObstacleBigPeak,
                    (LONG)proofState->stormrailObstacleActive[0],
                    (LONG)proofState->stormrailObstacleActive[1],
                    (LONG)proofState->stormrailObstacleActive[2],
                    (LONG)proofState->stormrailObstacleActive[3],
                    (LONG)proofState->stormrailObstacleActive[4],
                    (LONG)proofState->stormrailObstacleActive[5],
                    (LONG)proofState->stormrailObstacleFrame[0],
                    (LONG)proofState->stormrailObstacleFrame[1],
                    (LONG)proofState->stormrailObstacleFrame[2],
                    (LONG)proofState->stormrailObstacleFrame[3],
                    (LONG)proofState->stormrailObstacleFrame[4],
                    (LONG)proofState->stormrailObstacleFrame[5],
                    (LONG)proofState->stormrailObstacleScoreAwarded,
                    (LONG)proofState->stormrailObstacleScoreAwardedHigh);
            FPrintf(proof,"debris_timing=spawn_tick:%ld spawn_distance:%ld active_tick:%ld active_distance:%ld end_tick:%ld end_distance:%ld\n",
                    (LONG)proofState->stormrailLastDebrisSpawnTick,
                    (LONG)proofState->stormrailLastDebrisSpawnDistance,
                    (LONG)proofState->stormrailLastDebrisActiveTick,
                    (LONG)proofState->stormrailLastDebrisActiveDistance,
                    (LONG)proofState->stormrailLaunchTimer,
                    (LONG)proofState->stormrailDistance);
            FPrintf(proof,"debris_next=%ld debris_last_spawn_id=%ld loot_blocker=id:%ld age:%ld x:%ld distance:%ld\n",
                    (LONG)proofState->stormrailDebrisNextEvent,
                    (LONG)proofState->stormrailLastDebrisSpawnId,
                    (LONG)proofState->stormrailLootBlockerId,
                    (LONG)proofState->stormrailLootBlockerAge,
                    (LONG)proofState->stormrailLootBlockerX,
                    (LONG)proofState->stormrailLootBlockedDistance);
            FPrintf(proof,"free_lines=%ld/%ld/%ld/%ld/%ld pickup_bits=%ld/%ld lives=%ld life_awards=%ld\n",
                    (LONG)proofState->stormrailFreeLineSpawned[0],
                    (LONG)proofState->stormrailFreeLineSpawned[1],
                    (LONG)proofState->stormrailFreeLineSpawned[2],
                    (LONG)proofState->stormrailFreeLineSpawned[3],
                    (LONG)proofState->stormrailFreeLineSpawned[4],
                    (LONG)proofState->stormrailPickupConsumed,
                    (LONG)proofState->stormrailPickupConsumedHigh,
                    (LONG)proofState->lives,
                    (LONG)proofState->stormrailLifeAwards);
            FPrintf(proof,"chip_free=%ld chip_largest=%ld\n",
                    (LONG)AvailMem(MEMF_CHIP),
                    (LONG)AvailMem(MEMF_CHIP|MEMF_LARGEST));
#ifdef SPARKPAW_STORMRAIL_FINALE_PROOF
            FPrintf(proof,"finale=active:%ld phase:%ld phase_mask:%ld tick:%ld distance:%ld distance_violation:%ld gate:%ld hp:%ld hostile_launches:%ld hostile_after_combat:%ld\n",
                    (LONG)proofState->stormrailFinaleActive,
                    (LONG)proofState->stormrailFinalePhase,
                    (LONG)proofState->stormrailFinalePhaseMask,
                    (LONG)proofState->stormrailFinaleTick,
                    (LONG)proofState->stormrailDistance,
                    (LONG)proofState->stormrailFinaleDistanceViolation,
                    (LONG)proofState->stormrailFinaleGateOpen,
                    (LONG)proofState->stormrailFinaleHp[0],
                    (LONG)proofState->stormrailFinaleHostileLaunches,
                    (LONG)proofState->stormrailFinaleHostileAfterCombat);
            FPrintf(proof,"finale_attacks=fan_charge:%ld fan_shots:%ld hunter_charge:%ld hunter_shots:%ld\n",
                    (LONG)proofState->stormrailFinaleFanCharges,
                    (LONG)proofState->stormrailFinaleFanLaunches,
                    (LONG)proofState->stormrailFinaleHunterCharges,
                    (LONG)proofState->stormrailFinaleHunterLaunches);
#endif
            Close(proof);
        }
        cleanup();
        return proof?0:10;
    }
#endif
#ifdef SPARKPAW_WHDLOAD
    platformRestore(); cleanup(); return 0;
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    while(platformLeftMouse()) { }
    platformPrepareDebugFlush();
    rendererWriteDiagnosticLog();
    for(;;) { }
#endif
    return 0;
}
