#include "audio.h"

#include <exec/memory.h>
#include <dos/dos.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <proto/dos.h>
#include <proto/exec.h>

static volatile struct Custom *hardware=(volatile struct Custom *)0xdff000;
static UBYTE *shotSample;
static LONG shotSampleBytes;
static UBYTE *hurtSample;
static LONG hurtSampleBytes;
static UBYTE *enemyHitSample;
static LONG enemyHitSampleBytes;
static UBYTE *jumpSample;
static LONG jumpSampleBytes;
static UBYTE *collectSample;
static LONG collectSampleBytes;
static UBYTE shotDmaTicks;
static UBYTE gameplayDmaTicks;
static UBYTE gameplayPriority;
static UBYTE hurtCooldown;
static UBYTE enemyHitCooldown;
static UBYTE jumpCooldown;
static UBYTE collectCooldown;
static BOOL hardwareActive;

#define GAMEPLAY_CHANNEL 1
#define PLAYER_HURT_PRIORITY 9
#define PLAYER_HURT_TICKS 12
#define PLAYER_HURT_COOLDOWN 16
#define ENEMY_HIT_PRIORITY 6
#define ENEMY_HIT_TICKS 8
#define ENEMY_HIT_COOLDOWN 4
#define JUMP_PRIORITY 4
#define JUMP_TICKS 12
#define JUMP_COOLDOWN 4
#define COLLECT_PRIORITY 5
#define COLLECT_TICKS 11
#define COLLECT_COOLDOWN 3

static BOOL loadSample(CONST_STRPTR name,UBYTE **sample,LONG *sampleBytes)
{
    BPTR file=Open(name,MODE_OLDFILE);
    LONG size;
    if(!file) return FALSE;
    Seek(file,0,OFFSET_END); size=Seek(file,0,OFFSET_BEGINNING);
    if(size<=0) { Close(file); return FALSE; }
    *sample=(UBYTE *)AllocMem(size,MEMF_CHIP);
    if(!*sample||Read(file,*sample,size)!=size) {
        Close(file);
        if(*sample) FreeMem(*sample,size);
        *sample=NULL; return FALSE;
    }
    Close(file); *sampleBytes=size; return TRUE;
}

BOOL audioLoad(void)
{
    if(!loadSample("PROGDIR:assets/runtime/energy-shot.raw",
                   &shotSample,&shotSampleBytes)) return FALSE;
    if(!loadSample("PROGDIR:assets/runtime/player-hurt.raw",
                   &hurtSample,&hurtSampleBytes)) {
        audioUnload(); return FALSE;
    }
    if(!loadSample("PROGDIR:assets/runtime/enemy-hit.raw",
                   &enemyHitSample,&enemyHitSampleBytes)) {
        audioUnload(); return FALSE;
    }
    if(!loadSample("PROGDIR:assets/runtime/jump.raw",
                   &jumpSample,&jumpSampleBytes)) {
        audioUnload(); return FALSE;
    }
    if(!loadSample("PROGDIR:assets/runtime/collect-spark.raw",
                   &collectSample,&collectSampleBytes)) {
        audioUnload(); return FALSE;
    }
    return TRUE;
}

void audioUnload(void)
{
    if(shotSample) {
        FreeMem(shotSample,shotSampleBytes);
        shotSample=NULL; shotSampleBytes=0;
    }
    if(hurtSample) {
        FreeMem(hurtSample,hurtSampleBytes);
        hurtSample=NULL; hurtSampleBytes=0;
    }
    if(enemyHitSample) {
        FreeMem(enemyHitSample,enemyHitSampleBytes);
        enemyHitSample=NULL; enemyHitSampleBytes=0;
    }
    if(jumpSample) {
        FreeMem(jumpSample,jumpSampleBytes);
        jumpSample=NULL; jumpSampleBytes=0;
    }
    if(collectSample) {
        FreeMem(collectSample,collectSampleBytes);
        collectSample=NULL; collectSampleBytes=0;
    }
}

void audioSetHardwareActive(BOOL active)
{
    hardwareActive=active;
    if(!active) {
        hardware->dmacon=DMAF_AUD0|DMAF_AUD1;
        shotDmaTicks=0; gameplayDmaTicks=0;
        gameplayPriority=0; hurtCooldown=0; enemyHitCooldown=0;
        jumpCooldown=0; collectCooldown=0;
    }
}

void audioPlayShot(void)
{
    if(!shotSample||!hardwareActive) return;
    hardware->dmacon=DMAF_AUD0;
    hardware->aud[0].ac_ptr=(UWORD *)shotSample;
    hardware->aud[0].ac_len=(UWORD)(shotSampleBytes>>1);
    hardware->aud[0].ac_per=322;
    hardware->aud[0].ac_vol=60;
    hardware->dmacon=DMAF_SETCLR|DMAF_AUD0;
    shotDmaTicks=9;
}

static void playGameplaySample(UBYTE *sample,LONG sampleBytes,UBYTE priority,
                               UBYTE ticks,UBYTE *cooldown,UBYTE cooldownTicks,
                               UBYTE volume)
{
    /* Paula 0 remains dedicated to rapid plasma. Paula 1 is the prioritized
       gameplay-effect voice; channels 2-3 stay free for future music. */
    if(!sample||!hardwareActive||*cooldown) return;
    if(gameplayDmaTicks&&gameplayPriority>priority) return;
    hardware->dmacon=DMAF_AUD1;
    hardware->aud[GAMEPLAY_CHANNEL].ac_ptr=(UWORD *)sample;
    hardware->aud[GAMEPLAY_CHANNEL].ac_len=(UWORD)(sampleBytes>>1);
    hardware->aud[GAMEPLAY_CHANNEL].ac_per=322;
    hardware->aud[GAMEPLAY_CHANNEL].ac_vol=volume;
    hardware->dmacon=DMAF_SETCLR|DMAF_AUD1;
    gameplayDmaTicks=ticks; gameplayPriority=priority;
    *cooldown=cooldownTicks;
}

void audioPlayPlayerHurt(void)
{
    playGameplaySample(hurtSample,hurtSampleBytes,PLAYER_HURT_PRIORITY,
                       PLAYER_HURT_TICKS,&hurtCooldown,
                       PLAYER_HURT_COOLDOWN,64);
}

void audioPlayEnemyHit(void)
{
    playGameplaySample(enemyHitSample,enemyHitSampleBytes,ENEMY_HIT_PRIORITY,
                       ENEMY_HIT_TICKS,&enemyHitCooldown,
                       ENEMY_HIT_COOLDOWN,60);
}

void audioPlayJump(void)
{
    playGameplaySample(jumpSample,jumpSampleBytes,JUMP_PRIORITY,JUMP_TICKS,
                       &jumpCooldown,JUMP_COOLDOWN,58);
}

void audioPlayCollect(void)
{
    playGameplaySample(collectSample,collectSampleBytes,COLLECT_PRIORITY,
                       COLLECT_TICKS,&collectCooldown,COLLECT_COOLDOWN,58);
}

void audioUpdate(void)
{
    if(hardwareActive&&shotDmaTicks&&!--shotDmaTicks)
        hardware->dmacon=DMAF_AUD0;
    if(hardwareActive&&gameplayDmaTicks&&!--gameplayDmaTicks) {
        hardware->dmacon=DMAF_AUD1;
        gameplayPriority=0;
    }
    if(hurtCooldown) hurtCooldown--;
    if(enemyHitCooldown) enemyHitCooldown--;
    if(jumpCooldown) jumpCooldown--;
    if(collectCooldown) collectCooldown--;
}
