#include <dos/dos.h>
#include <exec/memory.h>
#include <proto/dos.h>
#include <proto/exec.h>

#include "audio.h"
#include "collision.h"
#include "game.h"
#include "performance_profile.h"
#include "platform_amiga.h"
#include "renderer.h"
#include "title.h"
#include "world_config.h"

enum AppState {
    APP_BOOT,
    APP_TITLE_LOADING,
    APP_TITLE_READY,
    APP_LEVEL_LOADING,
    APP_PLAYING
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

#ifdef PHASE6_MEMORY_TEST
static void writePhase6MemoryLog(ULONG beforeFree,ULONG beforeLargest)
{
    BPTR file=Open("PROGDIR:phase6-memory.log",MODE_NEWFILE);
    if(!file) return;
    FPrintf(file,"phase=6C.1 world_width=%ld\n",(LONG)SPARKPAW_WORLD_W);
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
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    BOOL prototypePublished;
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
    titleWaitFrames(225);
    DateStamp(&levelTime);
    enemySeed=(ULONG)levelTime.ds_Days*86400UL+
              (ULONG)levelTime.ds_Minute*60UL+(ULONG)levelTime.ds_Tick;
    gameInit(enemySeed^0x53504157UL);
    state=APP_LEVEL_LOADING;
    loadingShown=titleShowLevelLoading();
#ifdef PHASE6_MEMORY_TEST
    phase6BeforeFree=AvailMem(MEMF_CHIP);
    phase6BeforeLargest=AvailMem(MEMF_CHIP|MEMF_LARGEST);
#endif
    if(!loadingShown||!loadLevelFiles()||!titleShowLevelCharging()||
       !rendererPrepareGameplay()) {
        PutStr("Sparkpaw: runtime assets or Chip RAM unavailable.\n");
        if(!loadingShown) {
            PutStr((STRPTR)titleFailureReason()); PutStr("\n");
        }
        cleanup(); return 10;
    }
#ifdef PHASE6_MEMORY_TEST
    writePhase6MemoryLog(phase6BeforeFree,phase6BeforeLargest);
#endif
    /* Keep the second status readable even when preparation finishes quickly
       on Fast RAM systems or accelerated/emulated CPUs. */
    titleWaitLevelCharging(100);
    titleFadeOut();
    rendererUpdateGameplay();
    platformBeginTakeover();
    platformFinishTakeover(rendererCopperList());
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    platformProfileTimerStart();
#endif
    titleRelease();
    state=APP_PLAYING;
    while(state==APP_PLAYING) {
        ULONG profileStart;
        while(platformRasterLine()<100) { }
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        rendererDiagnosticUpdateEntry(platformRasterLine());
#endif
        profileStart=performanceProfileBegin();
        gameUpdate();
        performanceProfileEnd(PERF_GAME_UPDATE,profileStart);
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
    }
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    while(platformLeftMouse()) { }
    platformPrepareDebugFlush();
    rendererWriteDiagnosticLog();
    for(;;) { }
#endif
    return 0;
}
