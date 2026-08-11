#include "game.h"

#include "audio.h"
#include "collision.h"
#include "enemies.h"
#include "player.h"
#include "projectiles.h"

#define SCREEN_W 320
#define WORLD_W 1280

static struct GameState game;

static void resetLevelRuntime(void)
{
    /* Assets and packed caches stay resident. Preserve only prior Bob restore
       rectangles so the next line-300 pass erases the old runtime state. */
    enemiesResetPreservingDrawn();
    projectilesResetPreservingDrawn();
    playerInit();
    game.cameraX=0; game.frameCounter=0;
}

static void updateCamera(void)
{
    const struct PlayerState *player=playerState();
    LONG playerX=player->x>>8,wanted=game.cameraX;
    if(playerX-game.cameraX>202) wanted=playerX-202;
    if(playerX-game.cameraX<105) wanted=playerX-105;
    if(wanted<0) wanted=0;
    if(wanted>WORLD_W-SCREEN_W) wanted=WORLD_W-SCREEN_W;
    if(wanted>game.cameraX+5) game.cameraX+=5;
    else if(wanted<game.cameraX-5) game.cameraX-=5;
    else game.cameraX=wanted;
}

void gameInit(void)
{
    game.cameraX=0; game.frameCounter=0;
    playerInit(); enemiesInit(); projectilesInit();
}

void gameUpdate(void)
{
    BOOL left,right,down,jump,fire,wasGrounded;
    WORD playerLeft,playerTop,playerRight,playerBottom,enemyCenterX;
    const struct PlayerState *player=playerState();
    playerReadInput(&left,&right,&down,&jump,&fire);
    wasGrounded=player->grounded;
    playerStartShot(fire,audioPlayShot);
    playerUpdatePhysics(left,right,down,jump); playerUpdateShot();
    enemiesUpdate(game.frameCounter,collisionSolidAt);
    playerContactBounds(&playerLeft,&playerTop,&playerRight,&playerBottom);
    if(enemiesContactPlayer(playerLeft,playerTop,playerRight,playerBottom,
                            &enemyCenterX))
        if(playerTakeEnemyHit(enemyCenterX)&&!playerState()->health) {
            resetLevelRuntime();
            audioUpdate();
            return;
        }
    projectilesUpdate((WORD)game.cameraX,collisionSolidAt,enemiesHitProjectile);
    audioUpdate(); updateCamera();
    playerAnimate(!wasGrounded&&player->grounded,game.frameCounter);
    game.frameCounter++;
}

const struct GameState *gameState(void)
{
    return &game;
}
