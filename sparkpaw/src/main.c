#include <dos/dos.h>
#include <proto/dos.h>

#include "audio.h"
#include "collision.h"
#include "game.h"
#include "platform_amiga.h"
#include "renderer.h"

static void cleanup(void)
{
    audioUnload();
    rendererCleanup();
    platformClose();
}

int main(void)
{
    BOOL platformReady=platformOpen();
    gameInit();
    if(!platformReady||!rendererLoadGameplay()||!collisionLoad()||
       !audioLoad()||!rendererPrepareGameplay()) {
        PutStr("Sparkpaw: runtime assets or Chip RAM unavailable.\n");
        cleanup(); return 10;
    }
    rendererUpdateGameplay();
    platformTakeover(rendererCopperList());
    for(;;) {
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