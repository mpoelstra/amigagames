#include "game.h"

#include "audio.h"
#include "collision.h"
#include "collectibles.h"
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
    game.enemySeed=game.enemySeed*1664525UL+
                   (ULONG)game.frameCounter+1013904223UL;
    enemiesResetPreservingDrawn(game.enemySeed);
    collectiblesResetPreservingDrawn();
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

void gameInit(ULONG enemySeed)
{
    game.cameraX=0; game.frameCounter=0;
    game.lives=GAME_START_LIVES;
    game.diamonds=0;
    game.enemySeed=enemySeed?enemySeed:0x53504157UL;
    playerInit(); enemiesInit(game.enemySeed); collectiblesInit(); projectilesInit();
}

void gameUpdate(void)
{
    BOOL left,right,down,jump,fire,wasGrounded;
    WORD playerLeft,playerTop,playerRight,playerBottom,enemyCenterX;
    const struct PlayerState *player=playerState();
    playerReadInput(&left,&right,&down,&jump,&fire);
    wasGrounded=player->grounded;
    playerStartShot(fire,audioPlayShot);
    if(playerUpdatePhysics(left,right,down,jump)) audioPlayJump();
    playerUpdateShot();
    enemiesUpdate((WORD)game.cameraX,collisionSolidAt);
    playerContactBounds(&playerLeft,&playerTop,&playerRight,&playerBottom);
    /* Temporary level exit: reaching the authored right edge reuses the
       in-memory replay path until LEVEL_COMPLETE can select a next level. */
    if(playerReachedWorldRight(WORLD_W-1)) {
        resetLevelRuntime();
        audioUpdate();
        return;
    }
    if(enemiesContactPlayer(playerLeft,playerTop,playerRight,playerBottom,
                            &enemyCenterX)) {
        if(playerTakeEnemyHit(enemyCenterX)) {
            audioPlayPlayerHurt();
            if(!playerState()->health) {
                if(game.lives>1) game.lives--;
                else game.lives=GAME_START_LIVES;
                resetLevelRuntime();
                audioUpdate();
                return;
            }
        }
    }
    {
        UBYTE picked=collectiblesCollect(playerLeft,playerTop,
                                         playerRight,playerBottom);
        if(picked) {
            UWORD total=(UWORD)game.diamonds+picked;
            while(total>=GAME_DIAMONDS_PER_LIFE&&game.lives<GAME_MAX_LIVES) {
                total-=GAME_DIAMONDS_PER_LIFE;
                game.lives++;
            }
            game.diamonds=(UBYTE)(total<GAME_DIAMONDS_PER_LIFE?
                                  total:GAME_DIAMONDS_PER_LIFE-1);
            audioPlayCollect();
        }
    }
    projectilesUpdate((WORD)game.cameraX,collisionSolidAt,enemiesHitProjectile,
                      audioPlayEnemyHit);
    audioUpdate(); updateCamera();
    playerAnimate(!wasGrounded&&player->grounded,game.frameCounter);
    game.frameCounter++;
}

const struct GameState *gameState(void)
{
    return &game;
}
