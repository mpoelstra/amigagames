#include <dos/dos.h>
#include <exec/memory.h>
#include <proto/dos.h>
#include <proto/exec.h>

#include "audio.h"
#include "collision.h"
#include "game.h"
#include "performance_profile.h"
#include "platform_amiga.h"
#include "player.h"
#include "renderer.h"
#include "title.h"
#include "world_config.h"

enum AppState {
    APP_BOOT,
    APP_TITLE_LOADING,
    APP_TITLE_READY,
    APP_LEVEL_LOADING,
    APP_PLAYING,
    APP_LEVEL_COMPLETE
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
    return rendererLoadGameplay()&&collisionLoad()&&audioLoad();
}

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
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    BOOL prototypePublished;
#endif
#ifdef SPARKPAW_REPLAY_PROOF
    UBYTE replayProofCompletions=0;
#endif
    BOOL platformReady=platformOpen();
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
#if defined(SPARKPAW_EXTRA_LIFE_VISUAL_PROOF)||defined(SPARKPAW_REPLAY_PROOF)
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
#if defined(SPARKPAW_EXTRA_LIFE_VISUAL_PROOF)||defined(SPARKPAW_REPLAY_PROOF)
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
#if !defined(SPARKPAW_EXTRA_LIFE_VISUAL_PROOF)&&!defined(SPARKPAW_REPLAY_PROOF)
    titleRunLevelReadyMenu(&secondaryButtonAction);
#endif
#ifdef SPARKPAW_WHDLOAD
    if(platformWHDLoadQuitRequested()) {
        platformRestore(); cleanup(); return 0;
    }
#endif
    playerSetSecondaryButtonAction(secondaryButtonAction);
    titleFadeOut();
    rendererUpdateGameplay();
    platformSwitchCopper(rendererCopperList());
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    platformProfileTimerStart();
#endif
    titleRelease();
    state=APP_PLAYING;
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
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        rendererDiagnosticUpdateEntry(platformRasterLine());
#endif
        profileStart=performanceProfileBegin();
        gameUpdate();
        performanceProfileEnd(PERF_GAME_UPDATE,profileStart);
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
    }
    if(state==APP_LEVEL_COMPLETE) {
        const struct GameState *result=gameState();
        UWORD enemies=result->enemiesDefeated;
        UWORD diamonds=result->diamondsCollected;
        ULONG elapsed=result->elapsedFields;
        ULONG score=result->score;
        platformReleaseForLoading(FALSE);
        if(!titleShowLevelComplete()) {
            PutStr("Sparkpaw: level-complete screen unavailable.\n");
            PutStr((STRPTR)titleFailureReason()); PutStr("\n");
            platformRestore(); titleRelease(); audioUnload();
            platformClose(); return 10;
        }
        platformFinishTakeover(titleCopperList());
        titleRunLevelComplete(enemies,diamonds,elapsed,score);
        /* Fade the custom score display completely before restoring the
           resident gameplay targets. The rejected visible-score reload proved
           that renderer mutation must never overlap a bright score frame. */
        titleFadeOut();
        platformReleaseForLoading(FALSE);
        platformResetGameInput();
        DateStamp(&levelTime);
        enemySeed=(ULONG)levelTime.ds_Days*86400UL+
                  (ULONG)levelTime.ds_Minute*60UL+(ULONG)levelTime.ds_Tick;
        gameInit(enemySeed^0x53504157UL);
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
