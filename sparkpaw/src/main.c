#include <dos/dos.h>
#include <proto/dos.h>

#include "audio.h"
#include "collision.h"
#include "game.h"
#include "platform_amiga.h"
#include "renderer.h"

enum AppState {
    APP_BOOT,
    APP_TITLE_LOADING,
    APP_TITLE_READY,
    APP_LEVEL_LOADING,
    APP_PLAYING
};

static void cleanup(void)
{
    audioUnload();
    rendererCleanup();
    platformClose();
}

static BOOL loadLevel(void)
{
    return rendererLoadGameplay()&&collisionLoad()&&audioLoad()&&
           rendererPrepareGameplay();
}

int main(void)
{
    enum AppState state=APP_BOOT;
    BOOL platformReady=platformOpen();
    gameInit();
    state=APP_LEVEL_LOADING;
    if(!platformReady||!loadLevel()) {
        PutStr("Sparkpaw: runtime assets or Chip RAM unavailable.\n");
        cleanup(); return 10;
    }
    rendererUpdateGameplay();
    platformTakeover(rendererCopperList());
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