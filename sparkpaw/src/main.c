#include <dos/dos.h>
#include <proto/dos.h>

#include "audio.h"
#include "collision.h"
#include "game.h"
#include "platform_amiga.h"
#include "renderer.h"
#include "title.h"

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

int main(void)
{
    enum AppState state=APP_BOOT;
    BOOL loadingShown;
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
    gameInit();
    state=APP_LEVEL_LOADING;
    loadingShown=titleShowLevelLoading();
    if(!loadingShown||!loadLevelFiles()||!titleShowLevelCharging()||
       !rendererPrepareGameplay()) {
        PutStr("Sparkpaw: runtime assets or Chip RAM unavailable.\n");
        if(!loadingShown) {
            PutStr((STRPTR)titleFailureReason()); PutStr("\n");
        }
        cleanup(); return 10;
    }
    /* Keep the second status readable even when preparation finishes quickly
       on Fast RAM systems or accelerated/emulated CPUs. */
    titleWaitLevelCharging(100);
    titleFadeOut();
    rendererUpdateGameplay();
    platformBeginTakeover();
    platformFinishTakeover(rendererCopperList());
    titleRelease();
    state=APP_PLAYING;
    while(state==APP_PLAYING) {
        while(platformRasterLine()<100) { }
        gameUpdate();
        /* The Copper consumes these list entries at frame start. Update them
           well after that read and well before the next wrap, independent of
           how long the post-display Bob pass takes. */
        rendererUpdateGameplay();
        while(platformRasterLine()<300) { }
        rendererDrawGameplayBobs();
        while(platformRasterLine()>=300) { }
    }
    return 0;
}
