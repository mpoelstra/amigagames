#include "audio.h"
#include "audio_contract.h"

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
static UBYTE *enemyDeathSample;
static LONG enemyDeathSampleBytes;
static UBYTE *striderShotSample;
static LONG striderShotSampleBytes;
static UBYTE *jumpSample;
static LONG jumpSampleBytes;
static UBYTE *collectSample;
static LONG collectSampleBytes;
static UBYTE *waterSplashSample;
static LONG waterSplashSampleBytes;
static UBYTE *stormstoneCoreSample;
static LONG stormstoneCoreSampleBytes;
static UBYTE *tallyTickSample;
static LONG tallyTickSampleBytes;
static UBYTE *extraLifeSample;
static LONG extraLifeSampleBytes;
static UWORD *silenceSample;
static UBYTE shotDmaTicks;
static UBYTE gameplayDmaTicks;
static UBYTE gameplayPriority;
static UBYTE hurtCooldown;
static UBYTE enemyHitCooldown;
static UBYTE enemyDeathCooldown;
static UBYTE striderShotCooldown;
static UBYTE jumpCooldown;
static UBYTE collectCooldown;
static UBYTE waterSplashCooldown;
static UBYTE stormstoneCoreCooldown;
static UBYTE tallyTickCooldown;
static UBYTE extraLifeCooldown;
static BOOL hardwareActive;

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
enum AudioDiagnosticEvent {
    AUDIO_DIAG_SHOT,
    AUDIO_DIAG_HURT,
    AUDIO_DIAG_ENEMY_HIT,
    AUDIO_DIAG_ENEMY_DEATH,
    AUDIO_DIAG_STRIDER_SHOT,
    AUDIO_DIAG_JUMP,
    AUDIO_DIAG_COLLECT,
    AUDIO_DIAG_WATER,
    AUDIO_DIAG_STORMSTONE_CORE,
    AUDIO_DIAG_TALLY_TICK,
    AUDIO_DIAG_EXTRA_LIFE,
    AUDIO_DIAG_COUNT
};
static ULONG diagnosticRequests[AUDIO_DIAG_COUNT];
static ULONG diagnosticStarts[AUDIO_DIAG_COUNT];
#define AUDIO_REQUEST(event) diagnosticRequests[event]++
#define AUDIO_START(event) diagnosticStarts[event]++
#else
#define AUDIO_REQUEST(event) do { } while(0)
#define AUDIO_START(event) do { } while(0)
#endif

#define GAMEPLAY_CHANNEL 1
#define PLAYER_HURT_PRIORITY 9
#define PLAYER_HURT_COOLDOWN 16
#define ENEMY_HIT_PRIORITY 6
#define ENEMY_HIT_COOLDOWN 4
#define ENEMY_DEATH_PRIORITY 8
#define ENEMY_DEATH_COOLDOWN 6
#define STRIDER_SHOT_PRIORITY 7
#define STRIDER_SHOT_COOLDOWN 12
#define JUMP_PRIORITY 4
#define JUMP_COOLDOWN 4
#define COLLECT_PRIORITY 5
#define COLLECT_COOLDOWN 3
#define WATER_SPLASH_PRIORITY 10
#define WATER_SPLASH_COOLDOWN 20
#define STORMSTONE_CORE_PRIORITY 11
#define STORMSTONE_CORE_COOLDOWN 55
#define TALLY_TICK_PRIORITY 3
#define TALLY_TICK_COOLDOWN 1
#define EXTRA_LIFE_PRIORITY 10
#define EXTRA_LIFE_COOLDOWN 20

static void waitAudioLatch(void)
{
    UWORD line=(UWORD)(((hardware->vposr&7)<<8)|(hardware->vhposr>>8));
    UWORD changes=0;
    /* Paula needs a deterministic DMA-off/on settling interval. Two PAL
       raster-line changes are independent of CPU speed and match the existing
       CIA keyboard acknowledgement convention. */
    while(changes<2) {
        UWORD next=(UWORD)(((hardware->vposr&7)<<8)|(hardware->vhposr>>8));
        if(next!=line) { line=next; changes++; }
    }
}

static void startOneShot(UBYTE channel,UWORD dmaMask,UBYTE *sample,
                         LONG sampleBytes,UBYTE volume)
{
    hardware->dmacon=dmaMask;
    waitAudioLatch();
    hardware->aud[channel].ac_ptr=(UWORD *)sample;
    hardware->aud[channel].ac_len=(UWORD)(sampleBytes>>1);
    hardware->aud[channel].ac_per=(UWORD)AUDIO_EFFECT_PERIOD;
    hardware->aud[channel].ac_vol=volume;
    hardware->dmacon=DMAF_SETCLR|dmaMask;
    waitAudioLatch();
    /* Paula has latched the real one-shot. Its next reload must be silence,
       never the beginning of the effect. */
    hardware->aud[channel].ac_ptr=silenceSample;
    hardware->aud[channel].ac_len=1;
}

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
    silenceSample=(UWORD *)AllocMem(sizeof(UWORD),MEMF_CHIP|MEMF_CLEAR);
    if(!silenceSample) return FALSE;
    if(!loadSample("PROGDIR:assets/runtime/energy-shot.raw",
                   &shotSample,&shotSampleBytes)) {
        audioUnload(); return FALSE;
    }
    if(!loadSample("PROGDIR:assets/runtime/player-hurt.raw",
                   &hurtSample,&hurtSampleBytes)) {
        audioUnload(); return FALSE;
    }
    if(!loadSample("PROGDIR:assets/runtime/enemy-hit.raw",
                   &enemyHitSample,&enemyHitSampleBytes)) {
        audioUnload(); return FALSE;
    }
    if(!loadSample("PROGDIR:assets/runtime/enemy-death.raw",
                   &enemyDeathSample,&enemyDeathSampleBytes)) {
        audioUnload(); return FALSE;
    }
    if(!loadSample("PROGDIR:assets/runtime/strider-shot.raw",
                   &striderShotSample,&striderShotSampleBytes)) {
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
    if(!loadSample("PROGDIR:assets/runtime/water-splash.raw",
                   &waterSplashSample,&waterSplashSampleBytes)) {
        audioUnload(); return FALSE;
    }
    if(!loadSample("PROGDIR:assets/runtime/stormstone-core.raw",
                   &stormstoneCoreSample,&stormstoneCoreSampleBytes)) {
        audioUnload(); return FALSE;
    }
    if(!loadSample("PROGDIR:assets/runtime/tally-tick.raw",
                   &tallyTickSample,&tallyTickSampleBytes)) {
        audioUnload(); return FALSE;
    }
    if(!loadSample("PROGDIR:assets/runtime/extra-life.raw",
                   &extraLifeSample,&extraLifeSampleBytes)) {
        audioUnload(); return FALSE;
    }
    return TRUE;
}

void audioUnload(void)
{
    if(silenceSample) {
        FreeMem(silenceSample,sizeof(UWORD));
        silenceSample=NULL;
    }
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
    if(enemyDeathSample) {
        FreeMem(enemyDeathSample,enemyDeathSampleBytes);
        enemyDeathSample=NULL; enemyDeathSampleBytes=0;
    }
    if(striderShotSample) {
        FreeMem(striderShotSample,striderShotSampleBytes);
        striderShotSample=NULL; striderShotSampleBytes=0;
    }
    if(jumpSample) {
        FreeMem(jumpSample,jumpSampleBytes);
        jumpSample=NULL; jumpSampleBytes=0;
    }
    if(collectSample) {
        FreeMem(collectSample,collectSampleBytes);
        collectSample=NULL; collectSampleBytes=0;
    }
    if(waterSplashSample) {
        FreeMem(waterSplashSample,waterSplashSampleBytes);
        waterSplashSample=NULL; waterSplashSampleBytes=0;
    }
    if(stormstoneCoreSample) {
        FreeMem(stormstoneCoreSample,stormstoneCoreSampleBytes);
        stormstoneCoreSample=NULL; stormstoneCoreSampleBytes=0;
    }
    if(tallyTickSample) {
        FreeMem(tallyTickSample,tallyTickSampleBytes);
        tallyTickSample=NULL; tallyTickSampleBytes=0;
    }
    if(extraLifeSample) {
        FreeMem(extraLifeSample,extraLifeSampleBytes);
        extraLifeSample=NULL; extraLifeSampleBytes=0;
    }
}

void audioSetHardwareActive(BOOL active)
{
    hardwareActive=active;
    if(!active) {
        hardware->dmacon=DMAF_AUD0|DMAF_AUD1;
        shotDmaTicks=0; gameplayDmaTicks=0;
        gameplayPriority=0; hurtCooldown=0; enemyHitCooldown=0;
        enemyDeathCooldown=0;
        striderShotCooldown=0;
        jumpCooldown=0; collectCooldown=0; waterSplashCooldown=0;
        stormstoneCoreCooldown=0;
        tallyTickCooldown=0;
        extraLifeCooldown=0;
    }
}

void audioPlayShot(void)
{
    AUDIO_REQUEST(AUDIO_DIAG_SHOT);
    if(!shotSample||!hardwareActive) return;
    startOneShot(0,DMAF_AUD0,shotSample,shotSampleBytes,60);
    AUDIO_START(AUDIO_DIAG_SHOT);
    shotDmaTicks=(UBYTE)audioSampleFields((ULONG)shotSampleBytes);
}

static void playGameplaySample(UBYTE *sample,LONG sampleBytes,UBYTE priority,
                               UBYTE *cooldown,UBYTE cooldownTicks,
                               UBYTE volume
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                               ,UBYTE diagnosticEvent
#endif
                               )
{
    /* Paula 0 remains dedicated to rapid plasma. Paula 1 is the prioritized
       gameplay-effect voice; channels 2-3 stay free for future music. */
    if(!sample||!hardwareActive||*cooldown) return;
    if(gameplayDmaTicks&&gameplayPriority>priority) return;
    startOneShot(GAMEPLAY_CHANNEL,DMAF_AUD1,sample,sampleBytes,volume);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    AUDIO_START(diagnosticEvent);
#endif
    gameplayDmaTicks=(UBYTE)audioSampleFields((ULONG)sampleBytes);
    gameplayPriority=priority;
    *cooldown=cooldownTicks;
}

void audioPlayPlayerHurt(void)
{
    AUDIO_REQUEST(AUDIO_DIAG_HURT);
    playGameplaySample(hurtSample,hurtSampleBytes,PLAYER_HURT_PRIORITY,
                       &hurtCooldown,
                       PLAYER_HURT_COOLDOWN,64
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                       ,AUDIO_DIAG_HURT
#endif
                       );
}

void audioPlayEnemyHit(void)
{
    AUDIO_REQUEST(AUDIO_DIAG_ENEMY_HIT);
    playGameplaySample(enemyHitSample,enemyHitSampleBytes,ENEMY_HIT_PRIORITY,
                       &enemyHitCooldown,
                       ENEMY_HIT_COOLDOWN,60
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                       ,AUDIO_DIAG_ENEMY_HIT
#endif
                       );
}

void audioPlayEnemyDeath(void)
{
    AUDIO_REQUEST(AUDIO_DIAG_ENEMY_DEATH);
    playGameplaySample(enemyDeathSample,enemyDeathSampleBytes,
                       ENEMY_DEATH_PRIORITY,&enemyDeathCooldown,
                       ENEMY_DEATH_COOLDOWN,64
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                       ,AUDIO_DIAG_ENEMY_DEATH
#endif
                       );
}

void audioPlayStriderShot(void)
{
    AUDIO_REQUEST(AUDIO_DIAG_STRIDER_SHOT);
    playGameplaySample(striderShotSample,striderShotSampleBytes,
                       STRIDER_SHOT_PRIORITY,&striderShotCooldown,
                       STRIDER_SHOT_COOLDOWN,64
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                       ,AUDIO_DIAG_STRIDER_SHOT
#endif
                       );
}

void audioPlayJump(void)
{
    AUDIO_REQUEST(AUDIO_DIAG_JUMP);
    playGameplaySample(jumpSample,jumpSampleBytes,JUMP_PRIORITY,
                       &jumpCooldown,JUMP_COOLDOWN,58
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                       ,AUDIO_DIAG_JUMP
#endif
                       );
}

void audioPlayCollect(void)
{
    AUDIO_REQUEST(AUDIO_DIAG_COLLECT);
    playGameplaySample(collectSample,collectSampleBytes,COLLECT_PRIORITY,
                       &collectCooldown,COLLECT_COOLDOWN,58
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                       ,AUDIO_DIAG_COLLECT
#endif
                       );
}

void audioPlayWaterSplash(void)
{
    AUDIO_REQUEST(AUDIO_DIAG_WATER);
    playGameplaySample(waterSplashSample,waterSplashSampleBytes,
                       WATER_SPLASH_PRIORITY,&waterSplashCooldown,
                       WATER_SPLASH_COOLDOWN,64
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                       ,AUDIO_DIAG_WATER
#endif
                       );
}

void audioPlayStormstoneCore(void)
{
    AUDIO_REQUEST(AUDIO_DIAG_STORMSTONE_CORE);
    playGameplaySample(stormstoneCoreSample,stormstoneCoreSampleBytes,
                       STORMSTONE_CORE_PRIORITY,&stormstoneCoreCooldown,
                       STORMSTONE_CORE_COOLDOWN,64
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                       ,AUDIO_DIAG_STORMSTONE_CORE
#endif
                       );
}

void audioPlayTallyTick(void)
{
    AUDIO_REQUEST(AUDIO_DIAG_TALLY_TICK);
    playGameplaySample(tallyTickSample,tallyTickSampleBytes,
                       TALLY_TICK_PRIORITY,&tallyTickCooldown,
                       TALLY_TICK_COOLDOWN,54
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                       ,AUDIO_DIAG_TALLY_TICK
#endif
                       );
}

void audioPlayExtraLife(void)
{
    AUDIO_REQUEST(AUDIO_DIAG_EXTRA_LIFE);
    playGameplaySample(extraLifeSample,extraLifeSampleBytes,
                       EXTRA_LIFE_PRIORITY,&extraLifeCooldown,
                       EXTRA_LIFE_COOLDOWN,62
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                       ,AUDIO_DIAG_EXTRA_LIFE
#endif
                       );
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
    if(enemyDeathCooldown) enemyDeathCooldown--;
    if(striderShotCooldown) striderShotCooldown--;
    if(jumpCooldown) jumpCooldown--;
    if(collectCooldown) collectCooldown--;
    if(waterSplashCooldown) waterSplashCooldown--;
    if(stormstoneCoreCooldown) stormstoneCoreCooldown--;
    if(tallyTickCooldown) tallyTickCooldown--;
    if(extraLifeCooldown) extraLifeCooldown--;
}

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
void audioDiagnosticWrite(BPTR file)
{
    static const char *const names[AUDIO_DIAG_COUNT]={
        "shot","hurt","enemy_hit","enemy_death",
        "strider_shot","jump","collect","water","stormstone_core",
        "tally_tick","extra_life"
    };
    UWORD event;
    for(event=0;event<AUDIO_DIAG_COUNT;event++)
        FPrintf(file,"audio event=%s requests=%ld paula_starts=%ld suppressed=%ld\n",
                names[event],diagnosticRequests[event],diagnosticStarts[event],
                diagnosticRequests[event]-diagnosticStarts[event]);
}
#endif
