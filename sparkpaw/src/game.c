#include "game.h"

#include "audio.h"
#include "camera_contract.h"
#include "collision.h"
#include "collectibles.h"
#include "enemies.h"
#include "level_data.h"
#include "performance_profile.h"
#include "platform_amiga.h"
#include "player.h"
#include "projectiles.h"
#include "world_config.h"

#define SCREEN_W 320
static struct GameState game;

static void accountElapsedFields(void)
{
    ULONG now=platformFieldCounter();
    game.elapsedFields+=(now-game.lastFieldCounter)&0x00ffffffUL;
    game.lastFieldCounter=now;
}

static void resetLevelRuntime(void)
{
    /* Assets and packed caches stay resident. Preserve only prior Bob restore
       rectangles so the next line-300 pass erases the old runtime state. */
    game.enemySeed=game.enemySeed*1664525UL+
                   (ULONG)game.frameCounter+1013904223UL;
    enemiesResetPreservingDrawn(game.enemySeed);
    collectiblesResetPreservingProgress();
    projectilesResetPreservingDrawn();
    playerInit();
    game.cameraX=0; game.frameCounter=0;
    game.waterSplashTimer=0;
    game.coreCollectTimer=0;
    game.lastFieldCounter=platformFieldCounter();
}

static BOOL applyEnemyDamage(WORD sourceCenterX)
{
    if(!playerTakeEnemyHit(sourceCenterX)) return FALSE;
    audioPlayPlayerHurt();
    if(playerState()->health) return FALSE;
    if(game.lives>1) game.lives--;
    else game.lives=GAME_START_LIVES;
    resetLevelRuntime();
    audioUpdate();
    return TRUE;
}

static BOOL spawnEnemyProjectile(WORD x,WORD y,BOOL facingLeft)
{
    if(!projectilesSpawnEnemy(x,y,facingLeft)) return FALSE;
    audioPlayStriderShot();
    return TRUE;
}

static void updateCamera(void)
{
    const struct PlayerState *player=playerState();
    LONG playerX=player->x>>8,wanted=game.cameraX;
    if(playerX>=3072||game.coreCollectTimer) wanted=WORLD_W-SCREEN_W;
    else {
#ifdef SPARKPAW_CAMERA_DEADZONE_REFERENCE
        if(playerX-game.cameraX>202) wanted=playerX-202;
        if(playerX-game.cameraX<105) wanted=playerX-105;
#else
        wanted=cameraCenteredTarget(playerX);
#endif
    }
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
    game.score=0; game.elapsedFields=0;
    game.enemiesDefeated=0; game.diamondsCollected=0;
    game.coreCollectTimer=0;
    game.enemySeed=enemySeed?enemySeed:0x53504157UL;
    game.lastFieldCounter=platformFieldCounter();
    playerInit(); enemiesInit(game.enemySeed); collectiblesInit(); projectilesInit();
}

void gameUpdate(void)
{
    ULONG profileStart;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    ULONG detailProfileStart;
#endif
    BOOL left,right,down,jump,fire,wasGrounded;
    WORD playerLeft,playerTop,playerRight,playerBottom,enemyCenterX;
    const struct PlayerState *player=playerState();
    if(!game.coreCollectTimer) accountElapsedFields();
    if(game.coreCollectTimer) {
        audioUpdate();
        updateCamera();
        game.frameCounter++;
        if(game.coreCollectTimer>1) game.coreCollectTimer--;
        return;
    }
    if(game.waterSplashTimer) {
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        detailProfileStart=performanceProfileBegin();
        audioUpdate();
        performanceProfileEnd(PERF_AUDIO_UPDATE,detailProfileStart);
#else
        audioUpdate();
#endif
        game.frameCounter++;
        if(!--game.waterSplashTimer) resetLevelRuntime();
        return;
    }
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    detailProfileStart=performanceProfileBegin();
    playerReadInput(&left,&right,&down,&jump,&fire);
    performanceProfileEnd(PERF_INPUT,detailProfileStart);
#else
    playerReadInput(&left,&right,&down,&jump,&fire);
#endif
    wasGrounded=player->grounded;
    playerStartShot(fire,audioPlayShot);
    profileStart=performanceProfileBegin();
    if(playerUpdatePhysics(left,right,down,jump)) audioPlayJump();
    performanceProfileEnd(PERF_PLAYER,profileStart);
    playerUpdateShot();
    playerContactBounds(&playerLeft,&playerTop,&playerRight,&playerBottom);
    if(levelPlayerTouchesWater(playerLeft,playerRight,playerBottom)) {
        if(game.lives>1) game.lives--;
        else game.lives=GAME_START_LIVES;
        game.waterSplashX=(WORD)((playerLeft+playerRight)>>1);
        game.waterSplashTimer=16;
        audioPlayWaterSplash();
        return;
    }
    if(levelPlayerFallsInDryGap(playerLeft,playerRight,playerBottom)) {
        if(game.lives>1) game.lives--;
        else game.lives=GAME_START_LIVES;
        resetLevelRuntime();
        audioUpdate();
        return;
    }
    /* Latch a short readable Core response before the resident replay path.
       A later checkpoint can replace this timer with LEVEL_COMPLETE. */
    if(levelPlayerTouchesStormstoneCore(playerLeft,playerTop,
                                        playerRight,playerBottom)) {
        game.coreCollectTimer=50;
        audioPlayStormstoneCore();
        audioUpdate();
        return;
    }
    profileStart=performanceProfileBegin();
    enemiesUpdate((WORD)game.cameraX,collisionSolidAt,
                  (WORD)((playerLeft+playerRight)>>1),
                  (WORD)((playerTop+playerBottom)>>1),
                  spawnEnemyProjectile);
    performanceProfileEnd(PERF_ENEMIES,profileStart);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    detailProfileStart=performanceProfileBegin();
    left=enemiesContactPlayer(playerLeft,playerTop,playerRight,playerBottom,
                              &enemyCenterX);
    performanceProfileEnd(PERF_ENEMY_CONTACT,detailProfileStart);
    if(left) {
#else
    if(enemiesContactPlayer(playerLeft,playerTop,playerRight,playerBottom,
                            &enemyCenterX)) {
#endif
        if(applyEnemyDamage(enemyCenterX)) return;
    }
    {
        UBYTE picked;
        profileStart=performanceProfileBegin();
        picked=collectiblesCollect(playerLeft,playerTop,
                                         playerRight,playerBottom);
        performanceProfileEnd(PERF_COLLECTIBLES,profileStart);
        if(picked) {
            UWORD total=(UWORD)game.diamonds+picked;
            while(total>=GAME_DIAMONDS_PER_LIFE&&game.lives<GAME_MAX_LIVES) {
                total-=GAME_DIAMONDS_PER_LIFE;
                game.lives++;
            }
            game.diamonds=(UBYTE)(total<GAME_DIAMONDS_PER_LIFE?
                                  total:GAME_DIAMONDS_PER_LIFE-1);
            game.diamondsCollected+=picked;
            game.score+=(ULONG)picked*5UL;
            audioPlayCollect();
        }
    }
    profileStart=performanceProfileBegin();
    projectilesUpdate((WORD)game.cameraX,collisionSolidAt,
                      collisionFirstSolidOnSweep,enemiesHitProjectile,
                      enemiesFirstProjectileHitOnSweep,
                      audioPlayEnemyHit,audioPlayEnemyDeath);
    {
        UWORD award=enemiesConsumeScoreAward();
        if(award) {
            game.score+=award;
            game.enemiesDefeated+=(UWORD)(award/20);
        }
    }
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    detailProfileStart=performanceProfileBegin();
    left=projectilesContactPlayer(playerLeft,playerTop,playerRight,playerBottom,
                                  &enemyCenterX);
    performanceProfileEnd(PERF_PROJECTILE_CONTACT,detailProfileStart);
    if(left&&applyEnemyDamage(enemyCenterX)) {
#else
    if(projectilesContactPlayer(playerLeft,playerTop,playerRight,playerBottom,
                                &enemyCenterX)&&
       applyEnemyDamage(enemyCenterX)) {
#endif
        performanceProfileEnd(PERF_PROJECTILES,profileStart);
        return;
    }
    performanceProfileEnd(PERF_PROJECTILES,profileStart);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    detailProfileStart=performanceProfileBegin();
    audioUpdate();
    performanceProfileEnd(PERF_AUDIO_UPDATE,detailProfileStart);
#else
    audioUpdate();
#endif
    updateCamera();
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    detailProfileStart=performanceProfileBegin();
    playerAnimate(!wasGrounded&&player->grounded,game.frameCounter);
    performanceProfileEnd(PERF_PLAYER_ANIMATE,detailProfileStart);
#else
    playerAnimate(!wasGrounded&&player->grounded,game.frameCounter);
#endif
    game.frameCounter++;
}

const struct GameState *gameState(void)
{
    return &game;
}

BOOL gameLevelComplete(void)
{
    return game.coreCollectTimer==1;
}
