/* Sparkpaw: The Stormstone Quest -- AGA dual-playfield milestone. */
#define SPARKPAW_RENDERER_IMPLEMENTATION_UNIT
#include <exec/types.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <graphics/gfxbase.h>
#include <graphics/copper.h>
#include <graphics/rastport.h>
#include <graphics/view.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <stdio.h>
#include <string.h>

#include "assets.h"
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#include "audio.h"
#endif
#include "collectibles.h"
#include "enemies.h"
#include "frame_phase.h"
#include "game.h"
#include "hud.h"
#include "level_data.h"
#include "platform_amiga.h"
#include "performance_profile.h"
#include "player.h"
#include "projectiles.h"
#include "projectile_render_visibility.h"
#ifdef SPARKPAW_ROLLING_PROTOTYPE
#include "rolling_renderer_contract.h"
#endif
#include "renderer.h"
#include "world_config.h"
#include "stormrail_contract.h"
#ifdef SPARKPAW_LEVEL1_RENDERER_TU_ISOLATION
/* Diagnostic only: retain the integrated campaign/game binary while asking
   vbcc to emit the proven Level-1 renderer translation unit. Public
   Stormrail proof hooks receive cold stubs below so this build can link, but
   it must never be used to enter Stormrail. */
#undef SPARKPAW_STORMRAIL_PROOF
#undef SPARKPAW_STORMRAIL_DUST
#undef SPARKPAW_STORMRAIL_FINALE_PROOF
#undef SPARKPAW_STORMRAIL_FINALE_GATE_OVERLAY_CACHE
#endif
#ifdef SPARKPAW_STORMRAIL_PROOF
#include "stormrail_palette_table.h"
#ifdef SPARKPAW_STORMRAIL_PROOF
#include "stormrail_gate6_art.h"
#endif
#endif

/* Production restores inactive-target Bobs directly from the canonical clean
   world and therefore needs only the two display rings. Keep the former full
   clean-target architecture available solely for matched regression builds. */
#if defined(SPARKPAW_ROLLING_PROTOTYPE) && \
    !defined(SPARKPAW_TARGET_CLEAN_REFERENCE)
#define SPARKPAW_CANONICAL_BOB_RESTORE
#endif

#define SCREEN_W 320
#define SCREEN_H 256
#define WORLD_H HUD_TOP
#ifdef SPARKPAW_FMODE0_EARLY_WORD_GUARD
#define PLAYFIELD_FETCH_BYTES 44
#define PLAYFIELD_GUARD_BYTES 2
#define HUD_FETCH_BYTES 42
#elif defined(SPARKPAW_AGA32_FETCH_CANDIDATE)
#ifdef SPARKPAW_AGA64_FETCH_CANDIDATE
#define PLAYFIELD_FETCH_BYTES 48
#define PLAYFIELD_GUARD_BYTES 0
#elif defined(SPARKPAW_AGA32_LEFT_GUARD)
#define PLAYFIELD_FETCH_BYTES 48
#define PLAYFIELD_GUARD_BYTES 4
#else
#define PLAYFIELD_FETCH_BYTES 44
#define PLAYFIELD_GUARD_BYTES 0
#endif
#ifdef SPARKPAW_HUD_RIGHT_FETCH_GUARD
#define HUD_FETCH_BYTES 48
#else
#define HUD_FETCH_BYTES 44
#endif
#else
#define PLAYFIELD_FETCH_BYTES 42
#define PLAYFIELD_GUARD_BYTES 0
#define HUD_FETCH_BYTES 42
#endif
#define COP_WORDS 768
#define SPRITE_W 48
#define SPRITE_H 48
#define ANIM_FRAMES PLAYER_ANIM_FRAMES
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
static void writeStartupStage(const char *stage)
{
    BPTR file=Open("PROGDIR:startupdiag.log",MODE_READWRITE);
    if(!file) file=Open("PROGDIR:startupdiag.log",MODE_NEWFILE);
    if(!file) return;
    Seek(file,0,OFFSET_END);
    FPrintf(file,"stage=%s world_width=%ld\n",(STRPTR)stage,(LONG)WORLD_W);
    FPrintf(file,"chip_free=%ld chip_largest=%ld\n",
            (LONG)AvailMem(MEMF_CHIP),
            (LONG)AvailMem(MEMF_CHIP|MEMF_LARGEST));
    FPrintf(file,"fast_free=%ld fast_largest=%ld\n",
            (LONG)AvailMem(MEMF_FAST),
            (LONG)AvailMem(MEMF_FAST|MEMF_LARGEST));
    Flush(file);
    Close(file);
}
#define STARTUP_REQUIRE(stage,expression) do { \
    writeStartupStage("before_" stage); \
    if(!(expression)) { writeStartupStage("failed_" stage); return FALSE; } \
    writeStartupStage("after_" stage); \
} while(0)
#endif
#ifdef SPARKPAW_AGA64_PLAYER_SPRITE
#define SPRITE_CHANNELS 2
#define SPRITE_DMA_W 64
/* POS and CTL each occupy one 64-bit fetch block, followed by four DATA
   words and four DATB words per row, then two zero fetch blocks. */
#define SPRITE_WORDS (8+SPRITE_H*8+8)
#define SPRITE_ALLOC_BYTES (SPRITE_WORDS*2+7)
#else
#define SPRITE_CHANNELS 6
#define SPRITE_DMA_W 16
#define SPRITE_ALLOC_BYTES (SPRITE_WORDS*2)
#define SPRITE_WORDS (2+SPRITE_H*2+2)
#endif
#define TOTAL_SPRITE_CHANNELS 8
#define CORE_SPRITE_W 64
#define CORE_SPRITE_H 48
#define CORE_IDLE_FRAMES 6
#define CORE_PICKUP_FRAMES 12
#define CORE_SPRITE_FRAMES (CORE_IDLE_FRAMES+CORE_PICKUP_FRAMES)
#define CORE_SOURCE_WORDS 4
#include "enemy_vertical_order.h"
#include "strider_restore_union.h"
#include "sprite_stage_cache.h"
#include "water_update_visibility.h"
#define HUD_TOP 208
#define HUD_H (SCREEN_H-HUD_TOP)
#define PLAYER_PLASMA_PATTERNS 5
#define PLASMA_PATTERNS (PLAYER_PLASMA_PATTERNS*2)
#define PLASMA_SOURCE_WORDS 2
#define DIAMOND_SOURCE_WORDS 2
#define DIAMOND_PATCH_H (COLLECTIBLE_H+4)
#define DIAMOND_WIDE_INDEX 28
#define DIAMOND_WIDE_WORDS 2
/* Match the established diamond Bob: one visible word and one zero shift
   guard. The transfer width selects one or two words from this source. */
#define EXTRA_LIFE_SOURCE_WORDS 2
#define WATER_Y 197
#define WATER_W LEVEL_WATER_W
#define WATER_H 11
#define WATER_WORDS (WATER_W/16)
#define WATER_FRAMES 16
#define SPLASH_W 32
#define SPLASH_H 16
#define SPLASH_FRAMES 4
#define SPLASH_SOURCE_WORDS 3
#define FRONT_PLANES 4
#define REAR_PLANES 3
#define WORLD_PLANES (FRONT_PLANES+REAR_PLANES)
#ifdef SPARKPAW_ROLLING_PROTOTYPE
/* A logical 512-pixel tile ring is repeated three times in each physical
   target. The Copper reads from the middle copy, so its unchanged 336-pixel
   fetch and complete edge Bobs cross either ring seam without a bitmap shift
   or a per-scanline Copper split. Only entering 16-pixel world columns replace
   their fixed ring slots. */
#define PROTOTYPE_RING_W 512
#define PROTOTYPE_RING_COPIES 3
#define PROTOTYPE_RING_BASE PROTOTYPE_RING_W
#define PROTOTYPE_TARGET_W (PROTOTYPE_RING_W*PROTOTYPE_RING_COPIES)
#define PROTOTYPE_TARGET_BYTES (PROTOTYPE_TARGET_W/8)
#endif

static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static const struct PlanarAsset *frontClean,*rearWorld,*sprites;
static const struct PlanarAsset *hudBase;
static const struct PlanarAsset *diamondSprite,*heartSprite,*coreSprite,*extraLifeSprite;
#ifdef SPARKPAW_STORMRAIL_PROOF
static const struct PlanarAsset *stormrailSprite,*stormrailObstacleSprite;
static const struct PlanarAsset *stormrailFlightRear;
#define STORM_VEHICLE_W 112
#define STORM_VEHICLE_H 88
#define STORM_VEHICLE_FRAMES 5
#define STORM_DRONE_W 48
#define STORM_DRONE_H 32
#define STORM_VEHICLE_WORDS 8
/* Five visible words plus the blank source guard consumed by shifted Bobs. */
#define STORM_COMPACT_WORDS 6
#define STORM_DRONE_WORDS 4
#define STORM_ROCK_BIG_WORDS 4
#define STORM_ROCK_SHARD_WORDS 2
#define STORM_ROCK_PILLAR_WORDS 3
#define STORM_ROCK_BIG_FRAMES 5
#define STORM_ROCK_SHARD_FRAMES 6
#define STORM_ROCK_PILLAR_FRAMES 2
#define STORM_FINALE_ACTOR_W STORMRAIL_FINALE_RENDER_W
#define STORM_FINALE_ACTOR_H STORMRAIL_FINALE_RENDER_H
/* Four visible words plus the blank shifted-Bob source guard. */
#define STORM_FINALE_ACTOR_WORDS 5
#define STORM_FINALE_GATE_W 32
#define STORM_FINALE_GATE_H 104
#define STORM_FINALE_GATE_WORDS 3
#define STORM_FINALE_GATE_HALVES 2
#ifdef SPARKPAW_STORMRAIL_DUST
#define STORM_DUST_SLOTS 8
#define STORM_DUST_SHAPES 3
#define STORM_DUST_COLORS 3
#define STORM_DUST_STYLES (STORM_DUST_SHAPES*STORM_DUST_COLORS)
#define STORM_DUST_W 16
#define STORM_DUST_H 3
#define STORM_DUST_WORDS 1
static UWORD *stormDustMask,*stormDustBits;
#endif
static UWORD *stormVehicleMask,*stormVehicleBits;
static UWORD *stormFlightMask,*stormFlightBits;
static UWORD *stormCompactMask,*stormCompactBits;
static UWORD *stormDroneMask,*stormDroneBits;
static UWORD *stormRockBigMask,*stormRockBigBits;
static UWORD *stormRockShardMask,*stormRockShardBits;
static UWORD *stormRockPillarMask,*stormRockPillarBits;
static UWORD *stormFinaleActorMask,*stormFinaleActorBits;
static UWORD *stormFinaleGateMask,*stormFinaleGateBits;
#define STORM_FINALE_GATE_DIRTY_WORDS ((STORMRAIL_PLAYFIELD_H+15)/16)
static UWORD stormFinaleGateDirty[STORM_FINALE_GATE_DIRTY_WORDS];
static struct BitMap *stormFlightBlank;
static BOOL stormFlightTargetClean[2];
static BOOL stormApproachReleased;
#define STORM_PROOF_FRAMES 8
static ULONG stormrailUnsafeBlits;
#ifndef SPARKPAW_CAMPAIGN_PLAY
static UBYTE *stormProofPlanes[STORM_PROOF_FRAMES];
static UWORD stormProofRowBytes[STORM_PROOF_FRAMES];
static UWORD stormProofTargetX[STORM_PROOF_FRAMES];
static UWORD stormProofTargetY[STORM_PROOF_FRAMES];
#endif
struct StormrailBobHistory {
    BOOL vehicleDrawn,enemyDrawn[STORMRAIL_MAX_ENEMIES];
    BOOL shotDrawn[STORMRAIL_MAX_SHOTS];
    BOOL hostileDrawn[STORMRAIL_MAX_HOSTILE_SHOTS];
    BOOL rewardDrawn[STORMRAIL_MAX_REWARDS];
    BOOL obstacleDrawn[STORMRAIL_MAX_OBSTACLES];
    BOOL finaleActorDrawn[STORMRAIL_FINALE_ACTOR_COUNT];
    BOOL finaleGateDrawn[2];
#ifdef SPARKPAW_STORMRAIL_FINALE_GATE_OVERLAY_CACHE
    BOOL finaleGateResident;
#endif
    BOOL finaleWarningDrawn[STORMRAIL_FINALE_ATTACK_COUNT];
#ifdef SPARKPAW_STORMRAIL_DUST
    BOOL dustDrawn[STORM_DUST_SLOTS];
    WORD dustX[STORM_DUST_SLOTS],dustWorldX[STORM_DUST_SLOTS];
    WORD dustY[STORM_DUST_SLOTS];
#endif
    WORD vehicleX,vehicleWorldX,vehicleY;
    WORD enemyX[STORMRAIL_MAX_ENEMIES],enemyWorldX[STORMRAIL_MAX_ENEMIES],enemyY[STORMRAIL_MAX_ENEMIES];
    WORD hostileX[STORMRAIL_MAX_HOSTILE_SHOTS],hostileWorldX[STORMRAIL_MAX_HOSTILE_SHOTS],hostileY[STORMRAIL_MAX_HOSTILE_SHOTS];
    WORD rewardX[STORMRAIL_MAX_REWARDS],rewardWorldX[STORMRAIL_MAX_REWARDS],rewardY[STORMRAIL_MAX_REWARDS];
    WORD obstacleX[STORMRAIL_MAX_OBSTACLES];
    WORD obstacleWorldX[STORMRAIL_MAX_OBSTACLES];
    WORD obstacleY[STORMRAIL_MAX_OBSTACLES];
    WORD shotX[STORMRAIL_MAX_SHOTS];
    WORD shotWorldX[STORMRAIL_MAX_SHOTS];
    WORD shotY[STORMRAIL_MAX_SHOTS];
    WORD finaleActorX[STORMRAIL_FINALE_ACTOR_COUNT];
    WORD finaleActorWorldX[STORMRAIL_FINALE_ACTOR_COUNT];
    WORD finaleActorY[STORMRAIL_FINALE_ACTOR_COUNT];
    WORD finaleGateX[2],finaleGateWorldX[2],finaleGateY[2];
    WORD finaleWarningX[STORMRAIL_FINALE_ATTACK_COUNT];
    WORD finaleWarningWorldX[STORMRAIL_FINALE_ATTACK_COUNT];
    WORD finaleWarningY[STORMRAIL_FINALE_ATTACK_COUNT];
    UBYTE vehicleFrame,vehicleWidth,vehicleHeight,enemyFrame[STORMRAIL_MAX_ENEMIES];
    UBYTE obstacleType[STORMRAIL_MAX_OBSTACLES];
    UBYTE obstacleFrame[STORMRAIL_MAX_OBSTACLES];
};
static struct StormrailBobHistory stormrailHistory;
#endif
static struct BitMap *frontDisplay;
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
static struct BitMap *rearDisplay;
#ifdef SPARKPAW_STORMRAIL_PROOF
static struct BitMap *stormFlightRearDisplay;
#endif
#endif
static UWORD *cop,copPos,ptrValue[WORLD_PLANES],scrollValue;
static UWORD frontColorValue[2][16];
#ifdef SPARKPAW_STORMRAIL_PROOF
#define STORM_REAR_PALETTE_STAGES 13
static UWORD stormRearColorValue[2][STORM_REAR_PALETTE_STAGES][8];
static UBYTE stormRearPaletteBuildList,stormRearPaletteBuildStage;
#endif
static const UWORD frontColors[16]={
    0x001,0x111,0xd41,0xf92,0xfea,0x26c,0x3ce,0x94c,
    0x444,0x666,0xa9a,0xedc,0x426,0x72a,0xa5d,0xe26
};
#ifdef SPARKPAW_ROLLING_PROTOTYPE
struct PrototypeEnemyHistory {
    WORD x,worldX,y;
    UBYTE type;
    BOOL drawn;
};
struct PrototypeProjectileHistory {
    WORD x,worldX,y;
    BOOL drawn;
};
struct PrototypeTarget {
    struct BitMap *clean,*display;
    WORD origin;
    struct PrototypeEnemyHistory enemy[MAX_ENEMIES];
    struct PrototypeProjectileHistory projectile[MAX_PROJECTILES];
    BOOL splashDrawn;
    WORD splashX,splashWorldX,splashY;
    UBYTE waterFrame[LEVEL_WATER_COUNT];
    BOOL collectibleDrawn[MAX_COLLECTIBLES];
    WORD collectibleX[MAX_COLLECTIBLES];
    WORD collectibleY[MAX_COLLECTIBLES];
    BOOL coreDrawn;
    WORD coreX,coreY;
    UBYTE coreFrame;
    BOOL extraLifeDrawn;
    WORD extraLifeX,extraLifeY;
#ifdef SPARKPAW_STORMRAIL_PROOF
    struct StormrailBobHistory stormrail;
#endif
};
static UWORD *prototypeCopper[2];
static UBYTE prototypeActiveCopper,prototypePreparedCopper;
static BOOL prototypeCopperReady;
static struct PrototypeTarget prototypeTarget[2];
static WORD prototypeBuildOrigin,prototypeDesiredOrigin;
static ULONG prototypeOwnershipViolations;
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
static WORD prototypeEnemyWorldX[MAX_ENEMIES];
static WORD prototypeProjectileWorldX[MAX_PROJECTILES];
static WORD prototypeSplashWorldX;
#endif
#endif
static UWORD *hwSprites[2][ANIM_FRAMES][SPRITE_CHANNELS];
static UWORD *hwSpriteStage[2][SPRITE_CHANNELS];
#ifdef SPARKPAW_AGA64_PLAYER_SPRITE
static UBYTE *hwSpriteStageAllocation[2][SPRITE_CHANNELS];
static UBYTE *nullSpriteAllocation;
#endif
static UBYTE hwSpriteStageIndex;
static struct SpriteStageCacheState hwSpriteStageCache[2];
static UWORD hudPtrValue[WORLD_PLANES];
static UWORD *nullSprite,spritePtrValue[TOTAL_SPRITE_CHANNELS];
static const struct GameState *game;
static UWORD *plasmaMask,*plasmaBits;
static UWORD *diamondMask,*diamondBits;
#ifdef SPARKPAW_STORMRAIL_PROOF
static UWORD *heartMask,*heartBits;
#endif
static UWORD *coreMask,*coreBits;
static UWORD *extraLifeMask,*extraLifeBits;
#ifndef SPARKPAW_ROLLING_PROTOTYPE
static BOOL coreDrawn;
static WORD coreDrawnX,coreDrawnY;
static UBYTE coreDrawnFrame;
static BOOL extraLifeDrawn;
static WORD extraLifeDrawnX,extraLifeDrawnY;
#endif
static UWORD *diamondBackground;
static UWORD *diamondWideBackground;
static UWORD *waterBits;
static UBYTE waterDrawnFrame[LEVEL_WATER_COUNT];
static UWORD *splashMask,*splashBits;
static BOOL splashDrawn;
static WORD splashDrawnX,splashDrawnY;

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#ifndef DIAGNOSTIC_TRACE_FRAMES
#define DIAGNOSTIC_TRACE_FRAMES 1024
#endif
#define DIAG_MISSED_100 1
#define DIAG_MISSED_252 2
#define DIAG_MISSED_253 4
#define DIAG_MISSED_FIELD 8
#define DIAG_RING_COLUMN 16
struct RenderDiagnosticFrame {
    UWORD projectileRestore,projectileDraw;
    UWORD beetleRestore,beetleDraw,striderRestore,striderDraw;
    UWORD collectibleRestore,collectibleDraw;
    UWORD splashRestore,splashDraw,waterUpdates;
};
struct RenderDiagnosticEnemy {
    LONG x;
    WORD y;
    UBYTE active,drawn,spawn,type,frame,facing;
};
struct RenderDiagnosticTrace {
    ULONG gameFrame,updateStamp,publishEntryStamp,publishExitStamp;
    ULONG bobEntryStamp,bobCompleteStamp,boundaryStamp;
    ULONG copperAddress,generation;
    ULONG worldPointers[WORLD_PLANES],hudPointers[WORLD_PLANES];
    LONG cameraX,playerX;
    LONG frontLogical,rearLogical,frontCoarse,rearCoarse;
    UWORD scrollRegister;
    UBYTE frontPhase,rearPhase,fetchMode;
    UWORD familyLines[5];
    UWORD flags;
    UBYTE hudIndex,blitterAtPublish,blitterAtBobEntry,blitterAtBobComplete;
    UBYTE publicationSucceeded;
    struct RenderDiagnosticFrame counts;
    struct RenderDiagnosticEnemy enemy[MAX_ENEMIES];
};
static struct RenderDiagnosticFrame diagnosticFrame,diagnosticWorst;
static struct RenderDiagnosticTrace *diagnosticTrace,diagnosticCurrent;
static struct FramePhaseClock diagnosticClock;
static UWORD diagnosticTraceCount,diagnosticTraceNext;
static ULONG diagnosticGeneration;
static ULONG diagnosticCadenceIntervals,diagnosticCadenceFields;
static ULONG diagnosticCadenceOne,diagnosticCadenceTwo;
static ULONG diagnosticCadenceThreePlus,diagnosticCadenceMax;
static ULONG diagnosticPreviousUpdateField;
static BOOL diagnosticHasPreviousUpdate;
static LONG diagnosticFrames,diagnosticWraps,diagnosticWorstElapsed;
static LONG diagnosticWorstFrame,diagnosticWorstCamera;
static UWORD diagnosticWorstStart,diagnosticWorstEnd;
enum {
    DIAG_LOAD_SETUP,
    DIAG_LOAD_TARGET_ALLOC,
    DIAG_LOAD_COPPER_ALLOC,
    DIAG_LOAD_HUD,
    DIAG_LOAD_PLAYER_SPRITES,
    DIAG_LOAD_REAR_GUARD,
    DIAG_LOAD_TRACE_ALLOC,
    DIAG_LOAD_BEETLE,
    DIAG_LOAD_STRIDER,
    DIAG_LOAD_STRIDER_STAGE,
    DIAG_LOAD_PLASMA,
    DIAG_LOAD_DIAMOND,
    DIAG_LOAD_STATIC_COLLECTIBLES,
    DIAG_LOAD_WATER,
    DIAG_LOAD_SPLASH,
    DIAG_LOAD_RING_TARGETS,
    DIAG_LOAD_COPPER,
    DIAG_LOAD_COUNT
};
static ULONG diagnosticLoadingFrames[DIAG_LOAD_COUNT];
#endif

#if defined(PHASE6_MEMORY_TEST)||defined(SPARKPAW_RENDER_DIAGNOSTIC)
static ULONG phase6PeakChipFree,phase6PeakChipLargest;
static ULONG phase6PeakFastFree,phase6PeakFastLargest;
#endif

struct EnemyBobCache {
    const struct PlanarAsset *source;
    UWORD *mask,*bits;
    UWORD width,height,frames,sourceWords;
    BOOL sourceLeftFirst;
};

static struct EnemyBobCache enemyCaches[ENEMY_TYPE_COUNT];
static UWORD *striderStageMask,*striderStageBits;
static UBYTE striderStageFacing[MAX_ENEMIES],striderStageFrame[MAX_ENEMIES];
static BOOL striderStageValid[MAX_ENEMIES];

static void cmove(UWORD reg,UWORD value) { cop[copPos++]=reg; cop[copPos++]=value; }

static const UBYTE rearBandColors[3][8][3]={
    {{0,0,1},{0,0,4},{1,1,6},{2,1,8},
     {4,2,9},{6,4,11},{9,6,12},{13,11,14}},
    {{0,0,1},{1,1,4},{2,10,13},{3,3,7},
     {4,3,9},{6,5,10},{9,7,12},{12,10,13}},
    {{0,0,1},{0,1,2},{0,2,3},{1,3,4},
     {1,4,4},{2,5,5},{4,6,6},{6,8,9}}
};

#ifdef SPARKPAW_STORMRAIL_PROOF
static UWORD stormScaleColor(UWORD color,UBYTE fade)
{
    UBYTE strength=(UBYTE)(STORMRAIL_FADE_STEPS-
        (fade>STORMRAIL_FADE_STEPS?STORMRAIL_FADE_STEPS:fade));
    UWORD r=(UWORD)(((color>>8)&15)*strength/STORMRAIL_FADE_STEPS);
    UWORD g=(UWORD)(((color>>4)&15)*strength/STORMRAIL_FADE_STEPS);
    UWORD b=(UWORD)((color&15)*strength/STORMRAIL_FADE_STEPS);
    return (UWORD)((r<<8)|(g<<4)|b);
}

static UWORD stormApproachPaletteValue(UBYTE stage,UBYTE pen)
{
    UBYTE source,target,step,steps,component;
    UWORD value=0;
    if(!stage) { source=target=0; step=0; steps=1; }
    else if(stage<=4) { source=0; target=1; step=stage; steps=4; }
    else { source=1; target=2; step=(UBYTE)(stage-4); steps=8; }
    for(component=0;component<3;component++) {
        UBYTE result=(UBYTE)((rearBandColors[source][pen][component]*
            (steps-step)+rearBandColors[target][pen][component]*step)/steps);
        value|=(UWORD)(result<<(8-component*4));
    }
    return value;
}

static void setStormrailRearPalette(ULONG distance)
{
    UBYTE stage,pen;
    const UWORD *palette;
    UWORD phase=(UWORD)((stormrailRearScroll(distance)+160)%
                        STORMRAIL_REAR_LOOP_W);
    palette=stormRoutePaletteValues[stormRoutePalettePhase[phase]];
    /* Generated rows contain every Copper stage, including the thirteenth
       bottom band immediately above the fixed HUD. */
    for(stage=0;stage<STORM_REAR_PALETTE_STAGES;stage++)
        for(pen=0;pen<8;pen++)
            cop[stormRearColorValue[prototypePreparedCopper][stage][pen]]=
                game->stormrailFade?
                    stormScaleColor(palette[stage*8+pen],game->stormrailFade):
                    palette[stage*8+pen];
}

static void setStormrailTransitionPalette(void)
{
    UBYTE stage,pen;
    for(pen=0;pen<16;pen++)
        cop[frontColorValue[prototypePreparedCopper][pen]]=
            stormScaleColor(frontColors[pen],game->stormrailFade);
    if(game->stormrailActive&&game->stormrailMode==STORMRAIL_MODE_FLIGHT) {
        setStormrailRearPalette(game->stormrailDistance);
        return;
    }
    for(stage=0;stage<STORM_REAR_PALETTE_STAGES;stage++)
        for(pen=0;pen<8;pen++)
            cop[stormRearColorValue[prototypePreparedCopper][stage][pen]]=
                stormScaleColor(stormApproachPaletteValue(stage,pen),
                                game->stormrailFade);
}

static void resetStormrailApproachCopperPalettes(void)
{
    UBYTE stage,pen;
    /* Flight rewrites these words in both resident Copper lists. Replay starts
       in APPROACH, before the per-frame transition palette path is active, so
       restore both lists explicitly rather than exposing the final route's
       horizontal colour bands over the clean approach bitmap. */
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    {
        UBYTE list;
        for(list=0;list<2;list++)
            for(stage=0;stage<STORM_REAR_PALETTE_STAGES;stage++)
                for(pen=0;pen<8;pen++)
                    prototypeCopper[list]
                        [stormRearColorValue[list][stage][pen]]=
                        stormApproachPaletteValue(stage,pen);
    }
#else
    for(stage=0;stage<STORM_REAR_PALETTE_STAGES;stage++)
        for(pen=0;pen<8;pen++)
            cop[stormRearColorValue[0][stage][pen]]=
                stormApproachPaletteValue(stage,pen);
#endif
}
#endif

static void copperRearPalette(UBYTE source,UBYTE target,UBYTE step,UBYTE steps)
{
    WORD i;
    cmove(0x106,0x1020);
    for(i=0;i<8;i++) {
        UWORD r=(rearBandColors[source][i][0]*(steps-step)+
                 rearBandColors[target][i][0]*step)/steps;
        UWORD g=(rearBandColors[source][i][1]*(steps-step)+
                 rearBandColors[target][i][1]*step)/steps;
        UWORD b=(rearBandColors[source][i][2]*(steps-step)+
                 rearBandColors[target][i][2]*step)/steps;
        cmove((UWORD)(0x1a0+i*2),
              (UWORD)((r<<8)|(g<<4)|b));
#ifdef SPARKPAW_STORMRAIL_PROOF
        if(stormRearPaletteBuildStage<STORM_REAR_PALETTE_STAGES)
            stormRearColorValue[stormRearPaletteBuildList]
                               [stormRearPaletteBuildStage][i]=copPos-1;
#endif
    }
#ifdef SPARKPAW_STORMRAIL_PROOF
    stormRearPaletteBuildStage++;
#endif
}

static void cptr(UWORD reg,APTR value,UWORD plane)
{
    ULONG p=(ULONG)value;
    cmove(reg,(UWORD)(p>>16)); ptrValue[plane]=copPos-1; cmove(reg+2,(UWORD)p);
}

static void spriteCptr(UWORD reg,APTR value,UWORD sprite)
{
    ULONG p=(ULONG)value;
    cmove(reg,(UWORD)(p>>16)); spritePtrValue[sprite]=copPos-1;
    cmove(reg+2,(UWORD)p);
}

static void plainCptr(UWORD reg,APTR value)
{
    ULONG p=(ULONG)value; cmove(reg,(UWORD)(p>>16)); cmove(reg+2,(UWORD)p);
}

static void buildCopper(void)
{
    static const UWORD colors[32]={
        0x001,0x111,0xd41,0xf92,0xfea,0x26c,0x3ce,0x94c,
        0x444,0x666,0xa9a,0xedc,0x426,0x72a,0xa5d,0xe26,
        0x001,0x013,0x125,0x247,0x449,0x65a,0x97b,0xcbd,
        0x100,0x210,0x320,0x430,0x540,0x650,0x760,0x870
    };
    UBYTE listIndex=0;
    WORD i; copPos=0;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    if(cop==prototypeCopper[1]) listIndex=1;
#endif
#ifdef SPARKPAW_STORMRAIL_PROOF
    stormRearPaletteBuildList=listIndex;
    stormRearPaletteBuildStage=0;
#endif
    cmove(0x08e,0x2c81); cmove(0x090,0x2cc1);
#ifdef SPARKPAW_FMODE0_EARLY_WORD_GUARD
    cmove(0x092,0x0028); cmove(0x094,0x00d0);
#elif defined(SPARKPAW_AGA32_LEFT_GUARD)
    cmove(0x092,0x0020); cmove(0x094,0x00d0);
#else
    cmove(0x092,0x0030); cmove(0x094,0x00d0);
#endif
    cmove(0x100,0x7600); cmove(0x102,0); scrollValue=copPos-1;
    cmove(0x104,0x0024); cmove(0x106,0x1020);
    cmove(0x108,frontDisplay->BytesPerRow-PLAYFIELD_FETCH_BYTES);
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
    cmove(0x10a,rearDisplay->BytesPerRow-PLAYFIELD_FETCH_BYTES);
#else
    cmove(0x10a,rearWorld->bitmap->BytesPerRow-PLAYFIELD_FETCH_BYTES);
#endif
    /* PF2 now owns entries 16..23. Move both even/odd hardware-sprite groups
       to entries 32..47 so Sparkpaw no longer aliases the rear palette. */
    cmove(0x10c,0x0022);
#ifdef SPARKPAW_AGA32_FETCH_CANDIDATE
#ifdef SPARKPAW_AGA64_FETCH_CANDIDATE
    cmove(0x1fc,0x0003); /* AGA 64-bit bitplanes; sprites stay 16-bit. */
#elif defined(SPARKPAW_AGA64_PLAYER_SPRITE)
    cmove(0x1fc,0x000d); /* 32-bit bitplanes plus one 64-pixel sprite pair. */
#else
    cmove(0x1fc,0x0001); /* AGA 32-bit bitplane fetch; sprites stay 16-bit. */
#endif
#else
    cmove(0x1fc,0);
#endif
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
    cptr(0x0e0,frontDisplay->Planes[0]+PROTOTYPE_RING_BASE/8-PLAYFIELD_GUARD_BYTES,0);
    cptr(0x0e4,rearDisplay->Planes[0],1);
    cptr(0x0e8,frontDisplay->Planes[1]+PROTOTYPE_RING_BASE/8-PLAYFIELD_GUARD_BYTES,2);
    cptr(0x0ec,rearDisplay->Planes[1],3);
    cptr(0x0f0,frontDisplay->Planes[2]+PROTOTYPE_RING_BASE/8-PLAYFIELD_GUARD_BYTES,4);
    cptr(0x0f4,rearDisplay->Planes[2],5);
#else
    cptr(0x0e0,frontDisplay->Planes[0],0); cptr(0x0e4,rearWorld->bitmap->Planes[0],1);
    cptr(0x0e8,frontDisplay->Planes[1],2); cptr(0x0ec,rearWorld->bitmap->Planes[1],3);
    cptr(0x0f0,frontDisplay->Planes[2],4); cptr(0x0f4,rearWorld->bitmap->Planes[2],5);
#endif
#ifdef SPARKPAW_AGA32_LEFT_GUARD
    cptr(0x0f8,frontDisplay->Planes[3]+PROTOTYPE_RING_BASE/8-PLAYFIELD_GUARD_BYTES,6);
#else
    cptr(0x0f8,frontDisplay->Planes[3],6);
#endif
    /* PF1 reads entries 0..15 and PF2OF=16 reads entries 16..23, all within
       AGA palette bank zero (COLOR00..31). */
    cmove(0x106,0x1020);
    for(i=0;i<32;i++) {
        cmove((UWORD)(0x180+i*2),colors[i]);
        if(i<16) frontColorValue[listIndex][i]=copPos-1;
    }
    copperRearPalette(0,0,0,1);
    /* Three attached pairs provide one 48x48 actor. */
    for(i=0;i<SPRITE_CHANNELS;i++)
        spriteCptr((UWORD)(0x120+i*4),hwSpriteStage[0][i],(UWORD)i);
    for(i=SPRITE_CHANNELS;i<TOTAL_SPRITE_CHANNELS;i++)
        spriteCptr((UWORD)(0x120+i*4),nullSprite,(UWORD)i);
    /* BANK=1 addresses entries 32..63; Sparkpaw uses its first sixteen. */
    cmove(0x106,0x3020);
    for(i=0;i<16;i++) {
        const UBYTE *rgb=sprites->palette[i];
        UWORD amiga=(UWORD)(((rgb[0]>>4)<<8)|((rgb[1]>>4)<<4)|(rgb[2]>>4));
        cmove((UWORD)(0x180+i*2),amiga);
    }
    /* AGA low-nibble pass gives the character its complete 24-bit palette. */
    cmove(0x106,0x3220);
    for(i=0;i<16;i++) {
        const UBYTE *rgb=sprites->palette[i];
        UWORD amiga=(UWORD)(((rgb[0]&15)<<8)|((rgb[1]&15)<<4)|(rgb[2]&15));
        cmove((UWORD)(0x180+i*2),amiga);
    }
    cmove(0x106,0x1020);
    /* Morph only PF2's eight colours during preceding horizontal blanks.
       Twelve small steps hide band boundaries while yielding 24 authored
       rear colours from the existing three planes and no extra fetch DMA. */
    for(i=1;i<=4;i++) {
#ifdef SPARKPAW_STORMRAIL_PROOF
        cop[copPos++]=(UWORD)(((game->stormrailActive?
            44+60+i*12-1:44+64+i*4-1)<<8)|0xd9);
#else
        cop[copPos++]=(UWORD)(((44+64+i*4-1)<<8)|0xd9);
#endif
        cop[copPos++]=0xfffe;
        copperRearPalette(0,1,(UBYTE)i,4);
    }
    for(i=1;i<=8;i++) {
#ifdef SPARKPAW_STORMRAIL_PROOF
        cop[copPos++]=(UWORD)(((game->stormrailActive?
            44+108+i*11-1:44+136+i*3-1)<<8)|0xd9);
#else
        cop[copPos++]=(UWORD)(((44+136+i*3-1)<<8)|0xd9);
#endif
        cop[copPos++]=0xfffe;
        copperRearPalette(1,2,(UBYTE)i,8);
    }
#if defined(SPARKPAW_HUD_SEAM_FRONT_BLACK)||defined(SPARKPAW_HUD_SEAM_ISOLATE_FRONT_PALETTE)||defined(SPARKPAW_HUD_SEAM_ISOLATE_REAR_PALETTE)
    /* H6 changes colour lookup only after line 250's final fetch. Bitplane
       DMA, shifters, pointers, modulos and the alpha.43 HUD split keep running
       normally. The standard HUD palette writes below restore its colours. */
    cop[copPos++]=(UWORD)(((44+HUD_TOP-2)<<8)|0xd1); cop[copPos++]=0xfffe;
    cmove(0x106,0x1020);
#if defined(SPARKPAW_HUD_SEAM_FRONT_BLACK)||defined(SPARKPAW_HUD_SEAM_ISOLATE_FRONT_PALETTE)
    for(i=0;i<16;i++) cmove((UWORD)(0x180+i*2),0);
#else
    for(i=16;i<24;i++) cmove((UWORD)(0x180+i*2),0);
#endif
    cmove(0x106,0x1220);
#if defined(SPARKPAW_HUD_SEAM_FRONT_BLACK)||defined(SPARKPAW_HUD_SEAM_ISOLATE_FRONT_PALETTE)
    for(i=0;i<16;i++) cmove((UWORD)(0x180+i*2),0);
#else
    for(i=16;i<24;i++) cmove((UWORD)(0x180+i*2),0);
#endif
    cmove(0x106,0x1020);
#endif
#ifdef SPARKPAW_HUD_SEAM_ISOLATE_SPRITES
    /* Diagnostic A: neutralise all sprite comparators before the final
       gameplay scanline becomes visible. The next frame's normal Copper and
       sprite DMA setup restores Stage 5L without touching the HUD split. */
    cop[copPos++]=(UWORD)(((44+HUD_TOP-1)<<8)|0x01); cop[copPos++]=0xfffe;
    for(i=0;i<TOTAL_SPRITE_CHANNELS;i++) cmove((UWORD)(0x142+i*8),0);
#elif defined(SPARKPAW_HUD_SEAM_ISOLATE_BITPLANES)
    /* Diagnostic B: suppress only bitplane output across the final gameplay
       and first HUD scanlines. Sprite output remains live. Restore BPLCON0 in
       the following horizontal blank, after the unmodified alpha.43 split. */
    cop[copPos++]=(UWORD)(((44+HUD_TOP-1)<<8)|0x01); cop[copPos++]=0xfffe;
    cmove(0x100,0x0000);
#endif
    /* Switch before the PAL line-255 boundary to a fixed bottom HUD bitmap.
       The gameplay list restarts normally next frame, restoring the scrolling
       world pointers above it without a vertical-counter wrap sequence. */
    /* Begin the HUD setup in the preceding horizontal blank. Waiting until
       line 252 left too little time to replace all seven pointers, scroll and
       modulos before that line's first fetch, exposing a scrolling world strip. */
    /* DDFSTOP is 0xd0. Start immediately after that final gameplay fetch;
       Stage 4E's 0xd9 start left the last pointer change marginally late. */
    cop[copPos++]=(UWORD)(((44+HUD_TOP-1)<<8)|0xd1); cop[copPos++]=0xfffe;
#ifdef SPARKPAW_HUD_NATIVE_3PLANE
    /* The authored HUD is a single three-plane bitmap. Stop carrying four
       blank dual-playfield planes across the timing-critical split. */
    cmove(0x100,0x3200);
#endif
#if defined(SPARKPAW_HUD_SEAM_FMODE_FIRST) && defined(SPARKPAW_AGA32_LEFT_GUARD)
    /* Stage 5L's playfield-to-HUD transition has only the horizontal blank
       after the final gameplay fetch. Restore the fixed HUD fetch mode as the
       first MOVE in that window, before scroll, DDF and pointer state. */
    cmove(0x1fc,0x0001);
#endif
    /* Match the proven zero-world-offset fetch alignment. The candidate uses
       the equivalent 31-pixel AGA delay with a 44-byte longword fetch. */
#ifdef SPARKPAW_AGA32_FETCH_CANDIDATE
#ifdef SPARKPAW_AGA32_ORIGIN_CORRECTED
    cmove(0x102,0x44ff);
#else
    cmove(0x102,0x00ff);
#endif
#ifdef SPARKPAW_AGA64_FETCH_CANDIDATE
    /* Keep the independently accepted fixed HUD on FMODE1/44-byte fetch. */
    cmove(0x1fc,0x0001);
#endif
#ifdef SPARKPAW_AGA32_LEFT_GUARD
    /* The HUD has no prefetch guard and already passed its boundary proof.
       Restore its original complete 44-byte fetch before replacing pointers. */
    cmove(0x092,0x0030);
#ifdef SPARKPAW_HUD_RIGHT_FETCH_GUARD
    cmove(0x094,0x00d8);
#elif !defined(SPARKPAW_HUD_SEAM_KEEP_DDFSTOP)
    cmove(0x094,0x00d0);
#endif
#ifdef SPARKPAW_AGA64_PLAYER_SPRITE
    /* Sparkpaw stops above the fixed HUD. Restore the HUD's proven bitplane
       mode after the gameplay sprite DMA region has completed. */
#ifndef SPARKPAW_HUD_SEAM_FMODE_FIRST
    cmove(0x1fc,0x0001);
#endif
#endif
#endif
#else
    cmove(0x102,0x000f);
#endif
#ifdef SPARKPAW_FMODE0_EARLY_WORD_GUARD
    /* The fixed HUD keeps its accepted FMODE0 `$30..$d0` 42-byte fetch. */
    cmove(0x092,0x0030); cmove(0x094,0x00d0);
#endif
    for(i=0;i<
#ifdef SPARKPAW_HUD_NATIVE_3PLANE
        3
#else
        WORLD_PLANES
#endif
        ;i++) {
        const struct BitMap *display=hudDisplayBitmap();
#ifdef SPARKPAW_HUD_NATIVE_3PLANE
        APTR value=(APTR)display->Planes[i];
#else
        APTR value=(i&1)||(i==6)?(APTR)hudBlankPlane():
                    (APTR)display->Planes[i>>1];
#endif
        ULONG p=(ULONG)value;
        UWORD reg=(UWORD)(0x0e0+i*4);
        cmove(reg,(UWORD)(p>>16)); hudPtrValue[i]=copPos-1;
        cmove(reg+2,(UWORD)p);
    }
    cmove(0x108,hudBytesPerRow()-HUD_FETCH_BYTES);
    cmove(0x10a,hudBytesPerRow()-HUD_FETCH_BYTES);
    cmove(0x106,0x1020);
    for(i=0;i<8;i++) {
        const UBYTE *rgb=hudBase->palette[i];
        UWORD amiga=(UWORD)(((rgb[0]>>4)<<8)|((rgb[1]>>4)<<4)|(rgb[2]>>4));
        cmove((UWORD)(0x180+i*2),amiga);
    }
    cmove(0x106,0x1220);
    for(i=0;i<8;i++) {
        const UBYTE *rgb=hudBase->palette[i];
        UWORD amiga=(UWORD)(((rgb[0]&15)<<8)|((rgb[1]&15)<<4)|(rgb[2]&15));
        cmove((UWORD)(0x180+i*2),amiga);
    }
    cmove(0x106,0x1020);
#ifdef SPARKPAW_HUD_SEAM_ISOLATE_BITPLANES
    cop[copPos++]=(UWORD)(((44+HUD_TOP)<<8)|0xd1); cop[copPos++]=0xfffe;
    cmove(0x100,0x7600);
#endif
    cop[copPos++]=0xffff; cop[copPos++]=0xfffe;
}

static void setPtr(UWORD plane,APTR base,LONG offset)
{
    ULONG p=(ULONG)base+offset; UWORD hi=ptrValue[plane];
    cop[hi]=(UWORD)(p>>16); cop[hi+2]=(UWORD)p;
}

#ifdef SPARKPAW_FMODE0_EARLY_WORD_GUARD
static UWORD copperMoveCount(const UWORD *list,UWORD words,
                             UWORD reg,UWORD value)
{
    UWORD at,count=0;
    for(at=0;at+1<words;at+=2)
        if(list[at]==reg&&list[at+1]==value) count++;
    return count;
}

static BOOL earlyWordCopperLayoutValid(void)
{
    /* Refuse to enter gameplay if either the playfield's `$28` DDFSTRT or
       the fixed HUD's `$30` restoration is absent from the emitted list. */
    return copperMoveCount(cop,copPos,0x0092,0x0028)==1&&
           copperMoveCount(cop,copPos,0x0092,0x0030)==1;
}
#endif

static void setScroll(LONG front,LONG rear)
{
#ifdef SPARKPAW_STORMRAIL_PROOF
    const struct BitMap *rearBitmap=game->stormrailActive&&
        game->stormrailMode==STORMRAIL_MODE_FLIGHT?
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
        stormFlightRearDisplay:
#else
        stormrailFlightRear->bitmap:
#endif
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
        rearDisplay;
#else
        rearWorld->bitmap;
#endif
#endif
#ifdef SPARKPAW_AGA32_FETCH_CANDIDATE
    LONG fo,ro;
#ifdef SPARKPAW_AGA64_FETCH_CANDIDATE
    ro=rollingAga64CorrectedByteOffset(rear);
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    fo=rollingAga64CorrectedByteOffset(PROTOTYPE_RING_BASE+
       (front&(PROTOTYPE_RING_W-1)));
#else
    fo=rollingAga64CorrectedByteOffset(front);
#endif
#else
#ifdef SPARKPAW_AGA32_ORIGIN_CORRECTED
    ro=rollingAga32CorrectedByteOffset(rear);
#else
    ro=rollingAga32ByteOffset(rear);
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
#ifdef SPARKPAW_AGA32_ORIGIN_CORRECTED
    fo=rollingAga32CorrectedByteOffset(PROTOTYPE_RING_BASE+
       (front&(PROTOTYPE_RING_W-1)));
#else
    fo=rollingAga32ByteOffset(PROTOTYPE_RING_BASE+
       (front&(PROTOTYPE_RING_W-1)));
#endif
#else
#ifdef SPARKPAW_AGA32_ORIGIN_CORRECTED
    fo=rollingAga32CorrectedByteOffset(front);
#else
    fo=rollingAga32ByteOffset(front);
#endif
#endif
#endif
#else
    UWORD ff=(UWORD)(15-(front&15)),rf=(UWORD)(15-(rear&15));
    LONG fo,ro=(rear>>4)<<1;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    fo=((PROTOTYPE_RING_BASE+(front&(PROTOTYPE_RING_W-1)))>>4)<<1;
#else
    fo=(front>>4)<<1;
#endif
#endif
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
    fo-=PLAYFIELD_GUARD_BYTES;
    setPtr(0,frontDisplay->Planes[0],fo); setPtr(2,frontDisplay->Planes[1],fo);
    setPtr(4,frontDisplay->Planes[2],fo); setPtr(1,
#ifdef SPARKPAW_STORMRAIL_PROOF
        rearBitmap
#else
        rearDisplay
#endif
        ->Planes[0],ro);
    setPtr(3,
#ifdef SPARKPAW_STORMRAIL_PROOF
        rearBitmap
#else
        rearDisplay
#endif
        ->Planes[1],ro); setPtr(5,
#ifdef SPARKPAW_STORMRAIL_PROOF
        rearBitmap
#else
        rearDisplay
#endif
        ->Planes[2],ro);
    setPtr(6,frontDisplay->Planes[3],fo);
#else
    setPtr(0,frontDisplay->Planes[0],fo); setPtr(2,frontDisplay->Planes[1],fo);
    setPtr(4,frontDisplay->Planes[2],fo); setPtr(1,
#ifdef SPARKPAW_STORMRAIL_PROOF
        rearBitmap
#else
        rearWorld->bitmap
#endif
        ->Planes[0],ro);
    setPtr(3,
#ifdef SPARKPAW_STORMRAIL_PROOF
        rearBitmap
#else
        rearWorld->bitmap
#endif
        ->Planes[1],ro); setPtr(5,
#ifdef SPARKPAW_STORMRAIL_PROOF
        rearBitmap
#else
        rearWorld->bitmap
#endif
        ->Planes[2],ro);
    setPtr(6,frontDisplay->Planes[3],fo);
#endif
#ifdef SPARKPAW_AGA32_FETCH_CANDIDATE
#ifdef SPARKPAW_AGA64_FETCH_CANDIDATE
    cop[scrollValue]=rollingAga64CorrectedBplcon1(front,rear);
#else
#ifdef SPARKPAW_AGA32_ORIGIN_CORRECTED
    cop[scrollValue]=rollingAga32CorrectedBplcon1(front,rear);
#else
    cop[scrollValue]=rollingAga32Bplcon1(front,rear);
#endif
#endif
#else
    cop[scrollValue]=(rf<<4)|ff;
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticCurrent.frontLogical=front;
    diagnosticCurrent.rearLogical=rear;
    diagnosticCurrent.frontCoarse=fo;
    diagnosticCurrent.rearCoarse=ro;
#ifdef SPARKPAW_AGA64_FETCH_CANDIDATE
    diagnosticCurrent.frontPhase=(UBYTE)((front+16)&63);
    diagnosticCurrent.rearPhase=(UBYTE)((rear+16)&63);
    diagnosticCurrent.fetchMode=3;
#else
#ifdef SPARKPAW_AGA32_FETCH_CANDIDATE
    diagnosticCurrent.frontPhase=(UBYTE)((front+16)&31);
    diagnosticCurrent.rearPhase=(UBYTE)((rear+16)&31);
    diagnosticCurrent.fetchMode=1;
#else
    diagnosticCurrent.frontPhase=(UBYTE)(front&15);
    diagnosticCurrent.rearPhase=(UBYTE)(rear&15);
    diagnosticCurrent.fetchMode=0;
#endif
#endif
    diagnosticCurrent.scrollRegister=cop[scrollValue];
#endif
}

#ifdef SPARKPAW_ROLLING_PROTOTYPE
static WORD prototypeOriginForCamera(WORD cameraX)
{
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive&&game->stormrailMode==STORMRAIL_MODE_FLIGHT&&
       game->stormrailDistance>=STORMRAIL_APPROACH_RETIRE_X) {
        WORD origin=(WORD)((cameraX&~15)-96);
        return origin>0?origin:0;
    }
#endif
    return (WORD)rollingRingWindowOrigin(cameraX,WORLD_W,PROTOTYPE_RING_W);
}

static BOOL prototypeRectFits(WORD x,WORD width)
{
    return rollingRectFits(x,width,prototypeBuildOrigin,PROTOTYPE_RING_W);
}

static WORD prototypePhysicalX(WORD worldX)
{
    return (WORD)rollingRingPhysicalX(worldX,game->cameraX,PROTOTYPE_RING_W);
}

static void prototypeCopyCanonicalSpan(struct PrototypeTarget *target,
                                       WORD worldX,WORD y,WORD width,WORD height)
{
    WORD remaining=width;
    while(remaining>0) {
        WORD slot=(WORD)(worldX&(PROTOTYPE_RING_W-1));
        WORD chunk=(WORD)(PROTOTYPE_RING_W-slot);
        WORD row,copy; UBYTE plane;
        if(chunk>remaining) chunk=remaining;
#ifdef SPARKPAW_FETCH_RELEVANT_RING_COPIES
        /* Split once at the two global fetch-union boundaries. Every word in
           the resulting chunk then has the same physical-copy destinations,
           avoiding reachability comparisons in the inner Chip-RAM loop. */
        if(slot<ROLLING_COPY2_REACH_END&&
           slot+chunk>ROLLING_COPY2_REACH_END)
            chunk=(WORD)(ROLLING_COPY2_REACH_END-slot);
        else if(slot<ROLLING_COPY0_REACH_START&&
                slot+chunk>ROLLING_COPY0_REACH_START)
            chunk=(WORD)(ROLLING_COPY0_REACH_START-slot);
#endif
#ifndef SPARKPAW_CANONICAL_RING_COPYMEM_REFERENCE
        for(plane=0;plane<FRONT_PLANES;plane++)
            for(row=0;row<height;row++) {
                const UWORD *source=(const UWORD *)(
                    frontClean->bitmap->Planes[plane]+
                    (LONG)(y+row)*frontClean->bitmap->BytesPerRow+
                    (worldX>>3));
                UWORD *display=(UWORD *)(target->display->Planes[plane]+
                    (LONG)(y+row)*target->display->BytesPerRow+(slot>>3));
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
                UWORD *clean=(UWORD *)(target->clean->Planes[plane]+
                    (LONG)(y+row)*target->clean->BytesPerRow+(slot>>3));
                UWORD *clean1=clean+(PROTOTYPE_RING_W/16);
                UWORD *clean2=clean1+(PROTOTYPE_RING_W/16);
#endif
                UWORD *display1=display+(PROTOTYPE_RING_W/16);
                UWORD *display2=display1+(PROTOTYPE_RING_W/16);
                WORD words=(WORD)(chunk>>4);
                WORD word;
#ifdef SPARKPAW_FETCH_RELEVANT_RING_COPIES
                if(slot>=ROLLING_COPY0_REACH_START) {
                    for(word=0;word<words;word++) {
                        UWORD value=*source++;
                        *clean++=value; *display++=value;
                        *clean1++=value; *display1++=value;
                    }
                } else if(slot<ROLLING_COPY2_REACH_END) {
                    for(word=0;word<words;word++) {
                        UWORD value=*source++;
                        *clean1++=value; *display1++=value;
                        *clean2++=value; *display2++=value;
                    }
                } else {
                    for(word=0;word<words;word++) {
                        UWORD value=*source++;
                        *clean1++=value; *display1++=value;
                    }
                }
#else
                for(word=0;word<words;word++) {
                    UWORD value=*source++;
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
                    *clean++=value; *clean1++=value; *clean2++=value;
#endif
                    *display++=value; *display1++=value; *display2++=value;
                }
#endif
            }
#else
        for(copy=0;copy<PROTOTYPE_RING_COPIES;copy++)
            for(plane=0;plane<FRONT_PLANES;plane++)
                for(row=0;row<height;row++) {
                    const UBYTE *source=frontClean->bitmap->Planes[plane]+
                        (LONG)(y+row)*frontClean->bitmap->BytesPerRow+
                        (worldX>>3);
                    LONG at=(LONG)(y+row)*target->display->BytesPerRow+
                        (slot>>3)+(LONG)copy*(PROTOTYPE_RING_W/8);
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
                    CopyMem((APTR)source,target->clean->Planes[plane]+at,
                            chunk>>3);
#endif
                    CopyMem((APTR)source,target->display->Planes[plane]+at,
                            chunk>>3);
                }
#endif
        worldX=(WORD)(worldX+chunk);
        remaining=(WORD)(remaining-chunk);
    }
}

static void prototypeCopyCanonicalRect(struct PrototypeTarget *target,
                                       WORD worldX,WORD y,WORD width,WORD height)
{
    WORD left=(WORD)(worldX&~15);
    WORD right=(WORD)((worldX+width+15)&~15);
    WORD windowRight=(WORD)(target->origin+PROTOTYPE_RING_W);
    if(y<0) { height=(WORD)(height+y); y=0; }
    if(y+height>WORLD_H) height=(WORD)(WORLD_H-y);
    if(left<target->origin) left=target->origin;
    if(right>windowRight) right=windowRight;
    if(right<=left||height<=0) return;
    prototypeCopyCanonicalSpan(target,left,y,(WORD)(right-left),height);
}

#ifndef SPARKPAW_RING_COLUMN_GENERIC_REFERENCE
/* The ordinary camera roll admits one aligned 16px column. Avoid the generic
   one-iteration word loop and repeated row-address multiplication: each row
   reads one canonical word and publishes it to the same three physical ring
   copies as prototypeCopyCanonicalSpan(). */
#ifndef SPARKPAW_STORMRAIL_PROOF
static void prototypeCopyCanonicalColumn(struct PrototypeTarget *target,
                                         WORD worldX)
{
    WORD slot=(WORD)(worldX&(PROTOTYPE_RING_W-1));
    WORD sourceStep=(WORD)(frontClean->bitmap->BytesPerRow>>1);
    WORD displayStep=(WORD)(target->display->BytesPerRow>>1);
    UBYTE plane;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        const UWORD *source=(const UWORD *)(
            frontClean->bitmap->Planes[plane]+(worldX>>3));
        UWORD *display=(UWORD *)(target->display->Planes[plane]+(slot>>3));
        WORD row;
        for(row=0;row<WORLD_H;row++) {
            UWORD value=*source;
            display[0]=value;
            display[PROTOTYPE_RING_W/16]=value;
            display[(PROTOTYPE_RING_W/16)*2]=value;
            source+=sourceStep;
            display+=displayStep;
        }
    }
}
#else
static void prototypeCopyCanonicalColumn(struct PrototypeTarget *target,
                                         WORD worldX)
{
    WORD slot=(WORD)(worldX&(PROTOTYPE_RING_W-1));
    WORD sourceStep=(WORD)(frontClean->bitmap->BytesPerRow>>1);
    WORD displayStep=(WORD)(target->display->BytesPerRow>>1);
    UBYTE plane;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        BOOL flightBlank=FALSE;
#ifdef SPARKPAW_STORMRAIL_PROOF
        flightBlank=(BOOL)(game->stormrailActive&&
                           game->stormrailMode==STORMRAIL_MODE_FLIGHT&&
                           worldX>=176);
#endif
        const UWORD *source=flightBlank?NULL:(const UWORD *)(
            frontClean->bitmap->Planes[plane]+(worldX>>3));
        UWORD *display=(UWORD *)(target->display->Planes[plane]+(slot>>3));
        WORD row;
        for(row=0;row<WORLD_H;row++) {
            UWORD value=flightBlank?0:*source;
            display[0]=value;
            display[PROTOTYPE_RING_W/16]=value;
            display[(PROTOTYPE_RING_W/16)*2]=value;
            if(!flightBlank) source+=sourceStep;
            display+=displayStep;
        }
    }
}
#endif
#endif

#ifndef SPARKPAW_DYNAMIC_RING_COPYMEM_REFERENCE
/* Dynamic patches are narrow, word-aligned rectangles replicated into three
   physical ring copies in both clean and display. CopyMem call overhead and
   six repeated source reads dominate these patches on a 68020. Read each
   canonical word once and publish that value to the six inactive destinations.
   The target has already passed WaitBlit and is never the displayed target. */
static void prototypeCopyDynamicSpan(struct PrototypeTarget *target,
                                     WORD worldX,WORD y,WORD width,WORD height)
{
    WORD remaining=width;
    while(remaining>0) {
        WORD slot=(WORD)(worldX&(PROTOTYPE_RING_W-1));
        WORD chunk=(WORD)(PROTOTYPE_RING_W-slot);
        WORD row,word,words; UBYTE plane;
        if(chunk>remaining) chunk=remaining;
#ifdef SPARKPAW_FETCH_RELEVANT_RING_COPIES
        if(slot<ROLLING_COPY2_REACH_END&&
           slot+chunk>ROLLING_COPY2_REACH_END)
            chunk=(WORD)(ROLLING_COPY2_REACH_END-slot);
        else if(slot<ROLLING_COPY0_REACH_START&&
                slot+chunk>ROLLING_COPY0_REACH_START)
            chunk=(WORD)(ROLLING_COPY0_REACH_START-slot);
#endif
        words=(WORD)(chunk>>4);
        for(plane=0;plane<FRONT_PLANES;plane++)
            for(row=0;row<height;row++) {
                const UWORD *source=(const UWORD *)(
                    frontClean->bitmap->Planes[plane]+
                    (LONG)(y+row)*frontClean->bitmap->BytesPerRow+
                    (worldX>>3));
                UWORD *display=(UWORD *)(target->display->Planes[plane]+
                    (LONG)(y+row)*target->display->BytesPerRow+(slot>>3));
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
                UWORD *clean=(UWORD *)(target->clean->Planes[plane]+
                    (LONG)(y+row)*target->clean->BytesPerRow+(slot>>3));
#endif
#ifndef SPARKPAW_DYNAMIC_RING_INDEXED_REFERENCE
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
                UWORD *clean1=clean+(PROTOTYPE_RING_W/16);
                UWORD *clean2=clean1+(PROTOTYPE_RING_W/16);
#endif
                UWORD *display1=display+(PROTOTYPE_RING_W/16);
                UWORD *display2=display1+(PROTOTYPE_RING_W/16);
#ifdef SPARKPAW_FETCH_RELEVANT_RING_COPIES
                if(slot>=ROLLING_COPY0_REACH_START) {
                    for(word=0;word<words;word++) {
                        UWORD value=*source++;
                        *clean++=value; *display++=value;
                        *clean1++=value; *display1++=value;
                    }
                } else if(slot<ROLLING_COPY2_REACH_END) {
                    for(word=0;word<words;word++) {
                        UWORD value=*source++;
                        *clean1++=value; *display1++=value;
                        *clean2++=value; *display2++=value;
                    }
                } else {
                    for(word=0;word<words;word++) {
                        UWORD value=*source++;
                        *clean1++=value; *display1++=value;
                    }
                }
#else
                for(word=0;word<words;word++) {
                    UWORD value=*source++;
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
                    *clean++=value; *clean1++=value; *clean2++=value;
#endif
                    *display++=value; *display1++=value; *display2++=value;
                }
#endif
#else
                for(word=0;word<words;word++) {
                    UWORD value=source[word];
                    WORD copy;
                    for(copy=0;copy<PROTOTYPE_RING_COPIES;copy++) {
                        LONG at=(LONG)copy*(PROTOTYPE_RING_W/16)+word;
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
                        clean[at]=value;
#endif
                        display[at]=value;
                    }
                }
#endif
            }
        worldX=(WORD)(worldX+chunk);
        remaining=(WORD)(remaining-chunk);
    }
}

static void prototypeCopyDynamicRect(struct PrototypeTarget *target,
                                     WORD worldX,WORD y,WORD width,WORD height)
{
    WORD left=(WORD)(worldX&~15);
    WORD right=(WORD)((worldX+width+15)&~15);
    WORD windowRight=(WORD)(target->origin+PROTOTYPE_RING_W);
    if(y<0) { height=(WORD)(height+y); y=0; }
    if(y+height>WORLD_H) height=(WORD)(WORLD_H-y);
    if(left<target->origin) left=target->origin;
    if(right>windowRight) right=windowRight;
    if(right<=left||height<=0) return;
    prototypeCopyDynamicSpan(target,left,y,(WORD)(right-left),height);
}
#endif

static void prototypeCopyInitial(struct PrototypeTarget *target)
{
    WORD i;
    prototypeCopyCanonicalRect(target,target->origin,0,
                               PROTOTYPE_RING_W,WORLD_H);
    for(i=0;i<LEVEL_WATER_COUNT;i++) target->waterFrame[i]=waterDrawnFrame[i];
    for(i=0;i<MAX_COLLECTIBLES;i++) {
        struct Collectible *item=collectibleAt(i);
#ifndef SPARKPAW_COLLECTIBLE_CANONICAL_SYNC_REFERENCE
        target->collectibleDrawn[i]=FALSE;
        target->collectibleX[i]=0;
#else
        target->collectibleDrawn[i]=item->drawn;
#endif
        target->collectibleY[i]=item->drawnY;
    }
}

static void prototypeRollTarget(struct PrototypeTarget *target,WORD newOrigin)
{
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    ULONG profileStart=performanceProfileBegin();
#endif
    WORD pixels=(WORD)(newOrigin-target->origin);
    if(!pixels) {
        performanceProfileEnd(PERF_RING_ROLL,profileStart);
        return;
    }
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticCurrent.flags|=DIAG_RING_COLUMN;
#endif
    if(pixels>=PROTOTYPE_RING_W||pixels<=-PROTOTYPE_RING_W) {
        target->origin=newOrigin; prototypeCopyInitial(target);
        performanceProfileEnd(PERF_RING_ROLL,profileStart);
        return;
    }
    {
        WORD oldOrigin=target->origin;
        target->origin=newOrigin;
        if(pixels>0) {
#ifndef SPARKPAW_RING_COLUMN_GENERIC_REFERENCE
            if(pixels==16)
                prototypeCopyCanonicalColumn(target,
                    (WORD)(oldOrigin+PROTOTYPE_RING_W));
            else
#endif
            prototypeCopyCanonicalRect(target,
                (WORD)(oldOrigin+PROTOTYPE_RING_W),0,pixels,WORLD_H);
        } else {
#ifndef SPARKPAW_RING_COLUMN_GENERIC_REFERENCE
            if(pixels==-16)
                prototypeCopyCanonicalColumn(target,newOrigin);
            else
#endif
            prototypeCopyCanonicalRect(target,newOrigin,0,(WORD)-pixels,WORLD_H);
        }
    }
    performanceProfileEnd(PERF_RING_ROLL,profileStart);
}

static void prototypeSynchronizeDynamic(struct PrototypeTarget *target)
{
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    ULONG profileStart=performanceProfileBegin();
#endif
    WORD i;
    for(i=0;i<LEVEL_WATER_COUNT;i++) {
        if(target->waterFrame[i]==waterDrawnFrame[i]) continue;
#ifdef SPARKPAW_DYNAMIC_RING_COPYMEM_REFERENCE
        prototypeCopyCanonicalRect(target,levelWaterLeft(i),WATER_Y,
                                   WATER_W,WATER_H);
#else
        prototypeCopyDynamicRect(target,levelWaterLeft(i),WATER_Y,
                                 WATER_W,WATER_H);
#endif
        target->waterFrame[i]=waterDrawnFrame[i];
    }
#ifdef SPARKPAW_COLLECTIBLE_CANONICAL_SYNC_REFERENCE
    for(i=0;i<MAX_COLLECTIBLES;i++) {
        struct Collectible *item=collectibleAt(i);
        if(target->collectibleDrawn[i]==item->drawn&&
           (!item->drawn||target->collectibleY[i]==item->drawnY)) continue;
#ifdef SPARKPAW_DYNAMIC_RING_COPYMEM_REFERENCE
        prototypeCopyCanonicalRect(target,(WORD)(item->x-2),(WORD)(item->y-2),
                                   COLLECTIBLE_W+4,DIAMOND_PATCH_H);
#else
        prototypeCopyDynamicRect(target,(WORD)(item->x-2),(WORD)(item->y-2),
                                 COLLECTIBLE_W+4,DIAMOND_PATCH_H);
#endif
        target->collectibleDrawn[i]=item->drawn;
        target->collectibleY[i]=item->drawnY;
    }
#endif
    performanceProfileEnd(PERF_RING_DYNAMIC,profileStart);
}

static void prototypePrepareCompactTarget(struct PrototypeTarget *target)
{
    platformWaitBlit();
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive&&game->stormrailMode==STORMRAIL_MODE_FLIGHT) {
        UBYTE index=(UBYTE)(target-prototypeTarget);
        if(!stormFlightTargetClean[index]) {
            UBYTE plane,copy;
            /* Clear one 512px physical copy per Blit. BLTSIZE encodes at most
               64 words, so a library-sized 1536px clear is neither bounded
               nor safe during custom takeover. */
            for(plane=0;plane<FRONT_PLANES;plane++)
                for(copy=0;copy<PROTOTYPE_RING_COPIES;copy++) {
                    platformWaitBlit();
                    hw->bltcon0=0x0100; hw->bltcon1=0;
                    hw->bltafwm=0xffff; hw->bltalwm=0xffff;
                    hw->bltdmod=(UWORD)(target->display->BytesPerRow-
                                       PROTOTYPE_RING_W/8);
                    hw->bltdpt=target->display->Planes[plane]+
                               copy*(PROTOTYPE_RING_W/8);
                    hw->bltsize=(UWORD)((WORLD_H<<6)|(PROTOTYPE_RING_W/16));
                }
            platformWaitBlit();
            stormFlightTargetClean[index]=TRUE;
            if(stormFlightTargetClean[0]&&stormFlightTargetClean[1]&&
               !stormApproachReleased) {
#ifndef SPARKPAW_STORMRAIL_RESULTS_TEST
                assetsReleaseStormrailApproach();
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
                if(rearDisplay) { FreeBitMap(rearDisplay); rearDisplay=NULL; }
#endif
                stormApproachReleased=TRUE;
#endif
            }
        }
        /* The flight ring is intentionally canonical blank until sparse
           non-colliding near-parallax spans are approved. Logical origin may
           wrap; all three physical copies remain equivalent and approach art
           can never become reachable again. */
        target->origin=prototypeDesiredOrigin;
        prototypeBuildOrigin=target->origin;
        return;
    }
#endif
    prototypeRollTarget(target,prototypeDesiredOrigin);
    prototypeBuildOrigin=target->origin;
    prototypeSynchronizeDynamic(target);
}

static void prototypeLoadHistory(UBYTE index)
{
    struct PrototypeTarget *target=&prototypeTarget[index];
    WORD i;
    for(i=0;i<MAX_ENEMIES;i++) {
        struct Enemy *enemy=enemyAt(i);
        enemy->drawn=target->enemy[i].drawn;
        enemy->drawnX=target->enemy[i].x; enemy->drawnY=target->enemy[i].y;
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
        prototypeEnemyWorldX[i]=target->enemy[i].worldX;
#endif
        enemy->drawnType=target->enemy[i].type;
    }
    for(i=0;i<MAX_PROJECTILES;i++) {
        struct Projectile *projectile=projectileAt(i);
        projectile->drawn=target->projectile[i].drawn;
        projectile->drawnX=target->projectile[i].x;
        projectile->drawnY=target->projectile[i].y;
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
        prototypeProjectileWorldX[i]=target->projectile[i].worldX;
#endif
    }
    splashDrawn=target->splashDrawn;
    splashDrawnX=target->splashX; splashDrawnY=target->splashY;
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) {
        stormrailHistory=target->stormrail;
    }
#endif
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
    prototypeSplashWorldX=target->splashWorldX;
#endif
}

static void prototypeSaveHistory(UBYTE index)
{
    struct PrototypeTarget *target=&prototypeTarget[index];
    WORD i;
    for(i=0;i<MAX_ENEMIES;i++) {
        struct Enemy *enemy=enemyAt(i);
        target->enemy[i].drawn=enemy->drawn;
        target->enemy[i].x=enemy->drawnX; target->enemy[i].y=enemy->drawnY;
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
        target->enemy[i].worldX=prototypeEnemyWorldX[i];
#endif
        target->enemy[i].type=enemy->drawnType;
    }
    for(i=0;i<MAX_PROJECTILES;i++) {
        struct Projectile *projectile=projectileAt(i);
        target->projectile[i].drawn=projectile->drawn;
        target->projectile[i].x=projectile->drawnX;
        target->projectile[i].y=projectile->drawnY;
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
        target->projectile[i].worldX=prototypeProjectileWorldX[i];
#endif
    }
    target->splashDrawn=splashDrawn;
    target->splashX=splashDrawnX; target->splashY=splashDrawnY;
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) {
        target->stormrail=stormrailHistory;
    }
#endif
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
    target->splashWorldX=prototypeSplashWorldX;
#endif
}

static void prototypeExposeHistoryUnion(void)
{
    WORD i;
    for(i=0;i<MAX_ENEMIES;i++) {
        struct Enemy *enemy=enemyAt(i);
        UBYTE chosen=prototypeTarget[prototypeActiveCopper].enemy[i].drawn?
                     prototypeActiveCopper:(UBYTE)(prototypeActiveCopper^1);
        enemy->drawn=prototypeTarget[0].enemy[i].drawn||
                     prototypeTarget[1].enemy[i].drawn;
        if(enemy->drawn) {
            enemy->drawnX=prototypeTarget[chosen].enemy[i].x;
            enemy->drawnY=prototypeTarget[chosen].enemy[i].y;
            enemy->drawnType=prototypeTarget[chosen].enemy[i].type;
        }
    }
    for(i=0;i<MAX_PROJECTILES;i++) {
        struct Projectile *projectile=projectileAt(i);
        UBYTE chosen=prototypeTarget[prototypeActiveCopper].projectile[i].drawn?
                     prototypeActiveCopper:(UBYTE)(prototypeActiveCopper^1);
        projectile->drawn=prototypeTarget[0].projectile[i].drawn||
                          prototypeTarget[1].projectile[i].drawn;
        if(projectile->drawn) {
            projectile->drawnX=prototypeTarget[chosen].projectile[i].x;
            projectile->drawnY=prototypeTarget[chosen].projectile[i].y;
        }
    }
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) {
        UBYTE chosen=prototypeTarget[prototypeActiveCopper].stormrail.vehicleDrawn?
                     prototypeActiveCopper:(UBYTE)(prototypeActiveCopper^1);
        stormrailHistory=prototypeTarget[chosen].stormrail;
        stormrailHistory.vehicleDrawn=
            prototypeTarget[0].stormrail.vehicleDrawn||
            prototypeTarget[1].stormrail.vehicleDrawn;
        for(i=0;i<STORMRAIL_MAX_ENEMIES;i++)
            stormrailHistory.enemyDrawn[i]=
                prototypeTarget[0].stormrail.enemyDrawn[i]||
                prototypeTarget[1].stormrail.enemyDrawn[i];
        for(i=0;i<STORMRAIL_MAX_SHOTS;i++)
            stormrailHistory.shotDrawn[i]=
                prototypeTarget[0].stormrail.shotDrawn[i]||
                prototypeTarget[1].stormrail.shotDrawn[i];
        for(i=0;i<STORMRAIL_MAX_HOSTILE_SHOTS;i++)
            stormrailHistory.hostileDrawn[i]=
                prototypeTarget[0].stormrail.hostileDrawn[i]||
                prototypeTarget[1].stormrail.hostileDrawn[i];
        for(i=0;i<STORMRAIL_MAX_REWARDS;i++)
            stormrailHistory.rewardDrawn[i]=
                prototypeTarget[0].stormrail.rewardDrawn[i]||
                prototypeTarget[1].stormrail.rewardDrawn[i];
        for(i=0;i<STORMRAIL_MAX_OBSTACLES;i++)
            stormrailHistory.obstacleDrawn[i]=
                prototypeTarget[0].stormrail.obstacleDrawn[i]||
                prototypeTarget[1].stormrail.obstacleDrawn[i];
    }
#endif
}
#endif

static UBYTE pixel(const struct BitMap *bm,WORD x,WORD y,UBYTE depth)
{
    UBYTE p,value=0,mask=(UBYTE)(0x80>>(x&7)); LONG at=(LONG)y*bm->BytesPerRow+(x>>3);
    if(x<0||x>=bm->BytesPerRow*8||y<0||y>=bm->Rows) return 0;
    for(p=0;p<depth;p++) if(bm->Planes[p][at]&mask) value|=(1<<p);
    return value;
}

static UBYTE spritePen(WORD x,WORD y)
{
    UBYTE mask=(UBYTE)(0x80>>(x&7));
    if(!(sprites->mask[(LONG)y*sprites->rowBytes+(x>>3)]&mask)) return 0;
    return pixel(sprites->bitmap,x,y,4);
}

static UBYTE enemyPen(const struct EnemyBobCache *cache,WORD x,WORD y)
{
    UBYTE mask=(UBYTE)(0x80>>(x&7));
    if(!(cache->source->mask[(LONG)y*cache->source->rowBytes+(x>>3)]&mask))
        return 0;
    return pixel(cache->source->bitmap,x,y,FRONT_PLANES);
}

static UWORD *enemyMaskRow(struct EnemyBobCache *cache,UBYTE facing,
                           UBYTE frame,WORD row)
{
    LONG index=((LONG)facing*cache->frames+frame)*cache->height+row;
    return cache->mask+index*cache->sourceWords;
}

static UWORD *enemyBitsRow(struct EnemyBobCache *cache,UBYTE facing,
                           UBYTE frame,UBYTE plane,WORD row)
{
    LONG pattern=(LONG)facing*cache->frames+frame;
    LONG index=(pattern*FRONT_PLANES+plane)*cache->height+row;
    return cache->bits+index*cache->sourceWords;
}

static BOOL buildEnemyPatterns(struct EnemyBobCache *cache,BOOL fastMaster)
{
    LONG maskWords=2L*cache->frames*cache->height*cache->sourceWords;
    LONG bitsWords=maskWords*FRONT_PLANES;
    ULONG memory=(fastMaster?MEMF_FAST:MEMF_CHIP)|MEMF_CLEAR;
    UBYTE facing,frame,plane; WORD x,y;
    if(!cache->source||!cache->source->mask||
       cache->source->width!=cache->width*2||
       cache->source->height!=cache->height*cache->frames) return FALSE;
    cache->mask=(UWORD *)AllocMem(maskWords*2,memory);
    cache->bits=(UWORD *)AllocMem(bitsWords*2,memory);
    if(!cache->mask||!cache->bits) return FALSE;
#ifndef SPARKPAW_ENEMY_CACHE_PIXEL_REFERENCE
    for(facing=0;facing<2;facing++) for(frame=0;frame<cache->frames;frame++)
        for(y=0;y<cache->height;y++) {
            UBYTE sourceFacing=
                cache->sourceLeftFirst?facing:(UBYTE)(1-facing);
            LONG sourceY=(LONG)frame*cache->height+y;
            LONG maskAt=sourceY*cache->source->rowBytes+
                (LONG)sourceFacing*cache->width/8;
            LONG bitmapAt=sourceY*cache->source->bitmap->BytesPerRow+
                (LONG)sourceFacing*cache->width/8;
            /* sourceWords includes one zero guard word required by the shifted
               cookie-cut Blit. Only width/16 words contain authored pixels. */
            for(x=0;x<(cache->width>>4);x++) {
                UWORD sourceMask=((const UWORD *)(cache->source->mask+maskAt))[x];
                UWORD opaque=0;
                for(plane=0;plane<FRONT_PLANES;plane++) {
                    UWORD bits=((const UWORD *)(
                        cache->source->bitmap->Planes[plane]+bitmapAt))[x]&
                        sourceMask;
                    enemyBitsRow(cache,facing,frame,plane,y)[x]=bits;
                    opaque|=bits;
                }
                enemyMaskRow(cache,facing,frame,y)[x]=opaque;
            }
        }
#else
    for(facing=0;facing<2;facing++) for(frame=0;frame<cache->frames;frame++)
        for(y=0;y<cache->height;y++) for(x=0;x<cache->width;x++) {
            UBYTE sourceFacing=cache->sourceLeftFirst?facing:(UBYTE)(1-facing);
            UBYTE pen=enemyPen(cache,(WORD)(sourceFacing*cache->width+x),
                               (WORD)(frame*cache->height+y));
            UBYTE at=(UBYTE)(x>>4); UWORD bit=(UWORD)(0x8000U>>(x&15));
            if(!pen) continue;
            enemyMaskRow(cache,facing,frame,y)[at]|=bit;
            for(plane=0;plane<FRONT_PLANES;plane++)
                if(pen&(1<<plane))
                    enemyBitsRow(cache,facing,frame,plane,y)[at]|=bit;
        }
#endif
    return TRUE;
}

static BOOL prepareStriderStages(void)
{
    const struct EnemyBobCache *cache=
        &enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER];
    LONG frameWords=(LONG)cache->height*cache->sourceWords;
    striderStageMask=(UWORD *)AllocMem(
        MAX_ENEMIES*frameWords*2,MEMF_CHIP|MEMF_CLEAR);
    striderStageBits=(UWORD *)AllocMem(
        MAX_ENEMIES*frameWords*FRONT_PLANES*2,MEMF_CHIP|MEMF_CLEAR);
    memset(striderStageValid,0,sizeof(striderStageValid));
    return striderStageMask&&striderStageBits;
}

static BOOL stageStriderFrame(UBYTE slot,UBYTE facing,UBYTE frame,
                              UWORD **mask,UWORD **bits)
{
    struct EnemyBobCache *cache=
        &enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER];
    LONG frameWords=(LONG)cache->height*cache->sourceWords;
    LONG pattern=(LONG)facing*cache->frames+frame;
    UWORD *slotMask=striderStageMask+(LONG)slot*frameWords;
    UWORD *slotBits=striderStageBits+
        (LONG)slot*frameWords*FRONT_PLANES;
    if(slot>=MAX_ENEMIES||frame>=cache->frames) return FALSE;
    if(!striderStageValid[slot]||striderStageFacing[slot]!=facing||
       striderStageFrame[slot]!=frame) {
        CopyMem(cache->mask+pattern*frameWords,slotMask,frameWords*2);
        CopyMem(cache->bits+pattern*frameWords*FRONT_PLANES,slotBits,
                frameWords*FRONT_PLANES*2);
        striderStageFacing[slot]=facing;
        striderStageFrame[slot]=frame;
        striderStageValid[slot]=TRUE;
    }
    *mask=slotMask; *bits=slotBits;
    return TRUE;
}

static void configureEnemyCaches(void)
{
    struct EnemyBobCache *beetle=&enemyCaches[ENEMY_TYPE_CLOCKWORK_BEETLE];
    struct EnemyBobCache *strider=
        &enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER];
    memset(enemyCaches,0,sizeof(enemyCaches));
    beetle->source=assetsEnemySprites();
    beetle->width=ENEMY_W; beetle->height=ENEMY_H;
    beetle->frames=ENEMY_FRAMES; beetle->sourceWords=ENEMY_SOURCE_WORDS;
    beetle->sourceLeftFirst=TRUE;
    strider->source=assetsStriderSprites();
    strider->width=STRIDER_W; strider->height=STRIDER_H;
    strider->frames=STRIDER_FRAMES;
    strider->sourceWords=STRIDER_SOURCE_WORDS;
    strider->sourceLeftFirst=FALSE;
}

static BOOL buildHardwareSprites(void)
{
#ifdef SPARKPAW_AGA64_PLAYER_SPRITE
    WORD facing,frame,pairPlane,x,y,stage,chunk;
    UBYTE *nullRaw=(UBYTE *)AllocMem(16+7,MEMF_CHIP|MEMF_CLEAR);
    if(!nullRaw) return FALSE;
    nullSpriteAllocation=nullRaw;
    nullSprite=(UWORD *)(((ULONG)nullRaw+7)&~7UL);
    for(facing=0;facing<2;facing++) for(frame=0;frame<ANIM_FRAMES;frame++)
    for(pairPlane=0;pairPlane<2;pairPlane++) {
        UWORD *data=(UWORD *)AllocMem(SPRITE_WORDS*2,MEMF_FAST|MEMF_CLEAR);
        WORD cellX=(facing?SPRITE_W*4:0)+(frame&3)*SPRITE_W;
        WORD cellY=(frame>>2)*SPRITE_H;
        if(!data) return FALSE;
        hwSprites[facing][frame][pairPlane]=data;
#ifndef SPARKPAW_PLAYER_SPRITE_PIXEL_REFERENCE
        for(y=0;y<SPRITE_H;y++) for(chunk=0;chunk<3;chunk++) {
            LONG maskAt=(LONG)(cellY+y)*sprites->rowBytes+
                (cellX>>3)+chunk*2;
            LONG bitmapAt=(LONG)(cellY+y)*sprites->bitmap->BytesPerRow+
                (cellX>>3)+chunk*2;
            UWORD sourceMask=*(const UWORD *)(sprites->mask+maskAt);
            data[8+y*8+chunk]=*(const UWORD *)(
                sprites->bitmap->Planes[pairPlane*2]+bitmapAt)&sourceMask;
            data[8+y*8+4+chunk]=*(const UWORD *)(
                sprites->bitmap->Planes[pairPlane*2+1]+bitmapAt)&sourceMask;
        }
#else
        for(y=0;y<SPRITE_H;y++) for(chunk=0;chunk<4;chunk++) {
            UWORD a=0,b=0;
            for(x=0;x<16;x++) {
                WORD localX=chunk*16+x;
                UBYTE pen=localX<SPRITE_W?spritePen(cellX+localX,cellY+y):0;
                if(pen&(1<<(pairPlane*2))) a|=(UWORD)(0x8000>>x);
                if(pen&(2<<(pairPlane*2))) b|=(UWORD)(0x8000>>x);
            }
            data[8+y*8+chunk]=a;
            data[8+y*8+4+chunk]=b;
        }
#endif
    }
    for(stage=0;stage<2;stage++) for(pairPlane=0;pairPlane<2;pairPlane++) {
        UBYTE *raw=(UBYTE *)AllocMem(SPRITE_ALLOC_BYTES,MEMF_CHIP|MEMF_CLEAR);
        UWORD *aligned;
        if(!raw) return FALSE;
        aligned=(UWORD *)(((ULONG)raw+7)&~7UL);
        hwSpriteStageAllocation[stage][pairPlane]=raw;
        hwSpriteStage[stage][pairPlane]=aligned;
        CopyMem(hwSprites[0][0][pairPlane],aligned,SPRITE_WORDS*2);
    }
#else
    WORD facing,frame,half,pairPlane,x,y,stage,channel;
    nullSprite=(UWORD *)AllocMem(4,MEMF_CHIP|MEMF_CLEAR);
    if(!nullSprite) return FALSE;
    for(facing=0;facing<2;facing++) for(frame=0;frame<ANIM_FRAMES;frame++)
    for(half=0;half<3;half++) for(pairPlane=0;pairPlane<2;pairPlane++) {
        UWORD *data=(UWORD *)AllocMem(SPRITE_WORDS*2,MEMF_FAST|MEMF_CLEAR);
        WORD channel=half*2+pairPlane;
        WORD cellX=(facing?SPRITE_W*4:0)+(frame&3)*SPRITE_W+half*16;
        WORD cellY=(frame>>2)*SPRITE_H;
        if(!data) return FALSE;
        hwSprites[facing][frame][channel]=data;
        for(y=0;y<SPRITE_H;y++) {
            UWORD a=0,b=0;
            for(x=0;x<16;x++) {
                UBYTE pen=spritePen(cellX+x,cellY+y);
                if(pen&(1<<(pairPlane*2))) a|=(UWORD)(0x8000>>x);
                if(pen&(2<<(pairPlane*2))) b|=(UWORD)(0x8000>>x);
            }
            data[2+y*2]=a; data[3+y*2]=b;
        }
    }
    for(stage=0;stage<2;stage++) for(channel=0;channel<SPRITE_CHANNELS;channel++) {
        hwSpriteStage[stage][channel]=(UWORD *)AllocMem(
            SPRITE_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
        if(!hwSpriteStage[stage][channel]) return FALSE;
        CopyMem(hwSprites[0][0][channel],hwSpriteStage[stage][channel],
                SPRITE_WORDS*2);
    }
#endif
    hwSpriteStageIndex=0;
    SPRITE_STAGE_CACHE_COMMIT(&hwSpriteStageCache[0],0,0);
    SPRITE_STAGE_CACHE_COMMIT(&hwSpriteStageCache[1],0,0);
    return TRUE;
}

static void setHudPointers(void)
{
    const struct PlayerState *player=playerState();
    UBYTE health=player->health<=PLAYER_MAX_HEALTH?player->health:
                                                  PLAYER_MAX_HEALTH;
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive&&
       game->stormrailMode>=STORMRAIL_MODE_LAUNCH_OUT)
        health=game->stormrailHealth;
#endif
    const struct BitMap *display;
    WORD plane;
    hudSetState(health,game->lives,game->diamonds,game->score);
    display=hudDisplayBitmap();
    for(plane=0;plane<
#ifdef SPARKPAW_HUD_NATIVE_3PLANE
        3
#else
        WORLD_PLANES
#endif
        ;plane++) {
#ifdef SPARKPAW_HUD_NATIVE_3PLANE
        APTR value=(APTR)display->Planes[plane];
#else
        APTR value=(plane&1)||(plane==6)?(APTR)hudBlankPlane():
                    (APTR)display->Planes[plane>>1];
#endif
        ULONG p=(ULONG)value; UWORD hi=hudPtrValue[plane];
        cop[hi]=(UWORD)(p>>16); cop[hi+2]=(UWORD)p;
    }
}

static void setHardwareSprite(void)
{
    const struct PlayerState *player=playerState();
    WORD channel,screenX=(WORD)(player->x>>8)-(WORD)game->cameraX+128-
                         (SPRITE_W-PLAYER_W)/2;
    WORD screenY=(WORD)(player->y>>8)+44-(SPRITE_H-PLAYER_H);
    WORD stopY;
    UWORD facing=player->facingLeft?1:0;
    UBYTE spriteFrame=player->animFrame;
#ifdef SPARKPAW_STORMRAIL_PROOF
    BOOL stormPilot=game->stormrailActive&&
                    game->stormrailMode==STORMRAIL_MODE_FLIGHT;
    if(stormPilot) {
        screenX=(WORD)(128+game->stormrailX+STORMRAIL_PILOT_SPRITE_X);
        screenY=(WORD)(44+game->stormrailY+STORMRAIL_PILOT_SPRITE_Y);
        facing=0;
        spriteFrame=PLAYER_STORMRAIL_PILOT_FRAME;
    }
#endif
    stopY=screenY+SPRITE_H;
    /* Blink the complete attached-sprite actor during accepted invulnerability.
       Pointer substitution preserves every cached 48-row stream and its real
       terminator; never shorten VSTOP to clip an attached pair. */
    if(
#ifdef SPARKPAW_STORMRAIL_PROOF
       (!stormPilot&&game->waterSplashTimer)||
#else
       game->waterSplashTimer||
#endif
#ifdef SPARKPAW_STORMRAIL_PROOF
       (game->stormrailActive&&
        game->stormrailMode>=STORMRAIL_MODE_LAUNCH_OUT&&!stormPilot)||
       (stormPilot&&game->stormrailInvuln&&(game->stormrailInvuln&4))||
       (game->stormrailActive&&game->stormrailMode==STORMRAIL_MODE_BOARDING&&
        game->stormrailBoardTimer>=4)||
#endif
       (
#ifdef SPARKPAW_STORMRAIL_PROOF
        !stormPilot&&
#endif
        player->invulnTimer&&!(player->invulnTimer&4))) {
        ULONG p=(ULONG)nullSprite;
        for(channel=0;channel<SPRITE_CHANNELS;channel++) {
            UWORD hi=spritePtrValue[channel];
            cop[hi]=(UWORD)(p>>16); cop[hi+2]=(UWORD)p;
        }
        return;
    }
    /* Fill the stage not referenced by the current frame. Every authored DMA
       word is copied unchanged from the Fast-RAM master before its position
       control words and next-frame Copper pointer are published. */
    hwSpriteStageIndex^=1;
    {
        BOOL copyImage=TRUE;
#if defined(SPARKPAW_SPRITE_STAGE_CACHE) && \
    !defined(SPARKPAW_SPRITE_STAGE_ALWAYS_COPY_REFERENCE)
        copyImage=(BOOL)SPRITE_STAGE_CACHE_NEEDS_COPY(
            &hwSpriteStageCache[hwSpriteStageIndex],(UBYTE)facing,
            spriteFrame);
#endif
    for(channel=0;channel<SPRITE_CHANNELS;channel++) {
        UWORD *data=hwSpriteStage[hwSpriteStageIndex][channel];
#ifdef SPARKPAW_AGA64_PLAYER_SPRITE
        WORD x=screenX;
#else
        WORD x=screenX+(channel>>1)*16;
#endif
        ULONG p=(ULONG)data; UWORD hi=spritePtrValue[channel];
        if(copyImage)
            CopyMem(hwSprites[facing][spriteFrame][channel],data,
                    SPRITE_WORDS*2);
        data[0]=(UWORD)((screenY<<8)|((x>>1)&0xff));
#ifdef SPARKPAW_AGA64_PLAYER_SPRITE
        data[4]=(UWORD)((stopY<<8)|((screenY&0x100)>>6)|
                        ((stopY&0x100)>>7)|(x&1)|((channel&1)?0x0080:0));
#else
        data[1]=(UWORD)((stopY<<8)|((screenY&0x100)>>6)|
                        ((stopY&0x100)>>7)|(x&1)|((channel&1)?0x0080:0));
#endif
        cop[hi]=(UWORD)(p>>16); cop[hi+2]=(UWORD)p;
    }
#if defined(SPARKPAW_SPRITE_STAGE_CACHE) && \
    !defined(SPARKPAW_SPRITE_STAGE_ALWAYS_COPY_REFERENCE)
        if(copyImage)
            SPRITE_STAGE_CACHE_COMMIT(
                &hwSpriteStageCache[hwSpriteStageIndex],(UBYTE)facing,
                spriteFrame);
#endif
    }
}

static UBYTE playerPlasmaPatternPen(UBYTE pattern,BOOL left,WORD x,WORD y)
{
    WORD lx=left?PROJECTILE_W-1-x:x;
    if(pattern>=2) {
        WORD dx=x-8,dy=y-4,d=dx*dx+dy*dy;
        if(pattern==2) {
            if(d<=5) return 4;
            if(d>=9&&d<=20) return 6;
        } else if(pattern==3) {
            if(d<=3) return 4;
            if(d>=5&&d<=13) return ((x+y)&1)?6:5;
        } else if(d<=4) return 6;
        return 0;
    }
    {
        WORD dx=lx-10,dy=y-4,d=dx*dx+dy*dy*3;
        if(d<=8) return 4;
        if(d<=24) return 6;
        if(d<=38) return 5;
    }
    if(y>=3&&y<=5&&lx>=0&&lx<8) {
        if(((lx+pattern*2)&3)==0) return 6;
        if(((lx+pattern)&1)==0) return 5;
    }
    return 0;
}

static UBYTE plasmaPatternPen(UBYTE pattern,BOOL left,WORD x,WORD y)
{
    BOOL hostile=pattern>=PLAYER_PLASMA_PATTERNS;
    UBYTE hostileBase=hostile?(UBYTE)(pattern-PLAYER_PLASMA_PATTERNS):0;
    UBYTE pen;
    if(hostile&&hostileBase==4) {
        /* Left-flying Hunter needle: long light spine, amber shoulders and a
           one-pixel nose. It does not reuse either round fan silhouette. */
        if(y==4&&x>=1&&x<=13) return 4;
        if((y==3||y==5)&&x>=4&&x<=11) return 3;
        if((y==2||y==6)&&x>=9&&x<=12) return 3;
        return 0;
    }
    pen=playerPlasmaPatternPen(hostile?hostileBase:pattern,left,x,y);
    if(!hostile) return pen;
    /* The Harrier fan owns compact violet storm pulses (hostile bases 2/3);
       turret and route fire retain the accepted amber bolt (bases 0/1).
       Player plasma remains untouched in its cyan family. */
    if(hostileBase==2||hostileBase==3) {
        if(pen==6) return 15;
        if(pen==5) return 14;
        return pen;
    }
    /* Hunter base 4 is a narrow hot amber-white needle, deliberately unlike
       both the round violet fan and Sparkpaw's cyan plasma. */
    /* Retain the accepted compact mask but give hostile fire an unmistakable
       hot orange/red identity with the same bright neutral core. This also
       avoids aliasing the authored violet parallax storm lights. */
    if(pen==6) return 3;
    if(pen==5) return 2;
    return pen;
}

static UWORD *plasmaMaskRow(UBYTE pattern,BOOL left,WORD row)
{
    LONG index=((LONG)pattern*2+left)*PROJECTILE_H+row;
    return plasmaMask+index*PLASMA_SOURCE_WORDS;
}

static UWORD *plasmaBitsRow(UBYTE pattern,BOOL left,UBYTE plane,WORD row)
{
    LONG source=(LONG)pattern*2+left;
    LONG index=(source*FRONT_PLANES+plane)*PROJECTILE_H+row;
    return plasmaBits+index*PLASMA_SOURCE_WORDS;
}

static BOOL buildPlasmaPatterns(void)
{
    LONG maskWords=PLASMA_PATTERNS*2L*PROJECTILE_H*PLASMA_SOURCE_WORDS;
    LONG bitsWords=maskWords*FRONT_PLANES;
    UBYTE pattern,left,plane; WORD x,y;
    plasmaMask=(UWORD *)AllocMem(maskWords*2,MEMF_CHIP|MEMF_CLEAR);
    plasmaBits=(UWORD *)AllocMem(bitsWords*2,MEMF_CHIP|MEMF_CLEAR);
    if(!plasmaMask||!plasmaBits) return FALSE;
    for(pattern=0;pattern<PLASMA_PATTERNS;pattern++) for(left=0;left<2;left++)
        for(y=0;y<PROJECTILE_H;y++) for(x=0;x<PROJECTILE_W;x++) {
            UBYTE pen=plasmaPatternPen(pattern,left,x,y);
            UWORD bit=(UWORD)(0x8000U>>x);
            if(!pen) continue;
            plasmaMaskRow(pattern,left,y)[0]|=bit;
            for(plane=0;plane<FRONT_PLANES;plane++)
                if(pen&(1<<plane)) plasmaBitsRow(pattern,left,plane,y)[0]|=bit;
        }
    return TRUE;
}

#ifdef SPARKPAW_STORMRAIL_PROOF
static void blitRestoreRect(WORD sourceX,WORD x,WORD y,WORD width,WORD height);
static void blitMaskedBob(UWORD *mask,UWORD *bits,WORD sourceWords,
                          WORD width,WORD height,WORD x,WORD y);
static void blitMaskedBobTargetStride(struct BitMap *target,UWORD *mask,
                          UWORD *bits,WORD sourceWords,WORD planeRows,
                          WORD width,WORD height,WORD x,WORD y);
static void stormFinaleGateClearDirty(void)
{
    UBYTE word;
    for(word=0;word<STORM_FINALE_GATE_DIRTY_WORDS;word++)
        stormFinaleGateDirty[word]=0;
}

static void stormFinaleGateMarkDamage(WORD worldX,WORD y,
                                      WORD width,WORD height)
{
    WORD gateWorldX,top,bottom,row;
    if(!game->stormrailFinaleActive||
       !stormrailHistory.finaleGateResident||
       !stormrailFinaleGateResident(game->stormrailFinalePhase,
                                    game->stormrailFinaleGateOpen)) return;
    gateWorldX=(WORD)(game->cameraX+STORMRAIL_FINALE_GATE_X);
    if(worldX+width<=gateWorldX||worldX>=gateWorldX+STORM_FINALE_GATE_W)
        return;
    top=y<0?0:y;
    bottom=(WORD)(y+height);
    if(bottom>STORMRAIL_PLAYFIELD_H) bottom=STORMRAIL_PLAYFIELD_H;
    for(row=top;row<bottom;row++)
        stormFinaleGateDirty[row>>4]|=(UWORD)(1U<<(row&15));
}

static void stormFinaleGateRepairDirty(void)
{
    WORD row=0;
    WORD worldX=(WORD)(game->cameraX+STORMRAIL_FINALE_GATE_X);
    WORD physicalX=prototypePhysicalX(worldX);
    while(row<STORMRAIL_PLAYFIELD_H) {
        WORD first,half,local,run;
        while(row<STORMRAIL_PLAYFIELD_H&&
              !(stormFinaleGateDirty[row>>4]&(1U<<(row&15)))) row++;
        if(row>=STORMRAIL_PLAYFIELD_H) break;
        first=row;
        half=(WORD)(first/STORM_FINALE_GATE_H);
        while(row<STORMRAIL_PLAYFIELD_H&&
              row<(half+1)*STORM_FINALE_GATE_H&&
              (stormFinaleGateDirty[row>>4]&(1U<<(row&15)))) row++;
        local=(WORD)(first-half*STORM_FINALE_GATE_H);
        run=(WORD)(row-first);
        blitMaskedBobTargetStride(frontDisplay,
            stormFinaleGateMask+
                (LONG)(half*STORM_FINALE_GATE_H+local)*
                    STORM_FINALE_GATE_WORDS,
            stormFinaleGateBits+
                (LONG)half*FRONT_PLANES*STORM_FINALE_GATE_H*
                    STORM_FINALE_GATE_WORDS+
                (LONG)local*STORM_FINALE_GATE_WORDS,
            STORM_FINALE_GATE_WORDS,STORM_FINALE_GATE_H,
            STORM_FINALE_GATE_W,run,physicalX,first);
    }
}
static UBYTE stormrailSourcePen(WORD x,WORD y)
{
    UBYTE bit=(UBYTE)(0x80>>(x&7));
    LONG at=(LONG)y*stormrailSprite->rowBytes+(x>>3);
    if(!stormrailSprite->mask||!(stormrailSprite->mask[at]&bit)) return 0;
    return pixel(stormrailSprite->bitmap,x,y,FRONT_PLANES);
}

static UBYTE stormrailObstacleSourcePen(WORD x,WORD y)
{
    UBYTE bit=(UBYTE)(0x80>>(x&7));
    LONG at=(LONG)y*stormrailObstacleSprite->rowBytes+(x>>3);
    if(!stormrailObstacleSprite->mask||
       !(stormrailObstacleSprite->mask[at]&bit)) return 0;
    return pixel(stormrailObstacleSprite->bitmap,x,y,FRONT_PLANES);
}

static UBYTE stormGate6PackedPen(LONG offset,WORD width,WORD x,WORD y)
{
    LONG pixel=offset+(LONG)y*width+x;
    UBYTE packed=stormGate6ArtPacked[pixel>>1];
    return (UBYTE)((pixel&1)?packed&15:packed>>4);
}

static UBYTE stormFinaleGatePen(UBYTE half,WORD x,WORD y)
{
    LONG offset=80L*46+(LONG)half*32*104;
    return stormGate6PackedPen(offset,32,x,y);
}

static BOOL buildStormrailPatterns(void)
{
    LONG vehicleWords=STORM_VEHICLE_FRAMES*STORM_VEHICLE_H*
                      STORM_VEHICLE_WORDS;
    LONG droneWords=3L*STORM_DRONE_H*STORM_DRONE_WORDS;
    LONG bigWords=(LONG)STORM_ROCK_BIG_FRAMES*
                  STORMRAIL_OBSTACLE_BIG_H*STORM_ROCK_BIG_WORDS;
    LONG shardWords=(LONG)STORM_ROCK_SHARD_FRAMES*
                    STORMRAIL_OBSTACLE_SHARD_H*STORM_ROCK_SHARD_WORDS;
    LONG pillarWords=(LONG)STORM_ROCK_PILLAR_FRAMES*
                     STORMRAIL_OBSTACLE_PILLAR_H*STORM_ROCK_PILLAR_WORDS;
    UBYTE frame,plane; WORD x,y;
    if(!stormrailSprite||stormrailSprite->width!=640||
       stormrailSprite->height!=120||!stormrailObstacleSprite||
       stormrailObstacleSprite->width!=400||
       stormrailObstacleSprite->height!=40) return FALSE;
    stormVehicleMask=(UWORD *)AllocMem(vehicleWords*2,MEMF_CHIP|MEMF_CLEAR);
    stormVehicleBits=(UWORD *)AllocMem(vehicleWords*FRONT_PLANES*2,
                                      MEMF_CHIP|MEMF_CLEAR);
    stormDroneMask=(UWORD *)AllocMem(droneWords*2,MEMF_CHIP|MEMF_CLEAR);
    stormDroneBits=(UWORD *)AllocMem(droneWords*FRONT_PLANES*2,
                                    MEMF_CHIP|MEMF_CLEAR);
    stormRockBigMask=(UWORD *)AllocMem(bigWords*2,MEMF_CHIP|MEMF_CLEAR);
    stormRockBigBits=(UWORD *)AllocMem(bigWords*FRONT_PLANES*2,
                                      MEMF_CHIP|MEMF_CLEAR);
    stormRockShardMask=(UWORD *)AllocMem(shardWords*2,MEMF_CHIP|MEMF_CLEAR);
    stormRockShardBits=(UWORD *)AllocMem(shardWords*FRONT_PLANES*2,
                                        MEMF_CHIP|MEMF_CLEAR);
    stormRockPillarMask=(UWORD *)AllocMem(pillarWords*2,MEMF_CHIP|MEMF_CLEAR);
    stormRockPillarBits=(UWORD *)AllocMem(pillarWords*FRONT_PLANES*2,
                                         MEMF_CHIP|MEMF_CLEAR);
    stormFinaleActorMask=(UWORD *)AllocMem((LONG)STORMRAIL_FINALE_ACTOR_COUNT*
        STORM_FINALE_ACTOR_H*
        STORM_FINALE_ACTOR_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    stormFinaleActorBits=(UWORD *)AllocMem((LONG)STORMRAIL_FINALE_ACTOR_COUNT*
        FRONT_PLANES*
        STORM_FINALE_ACTOR_H*STORM_FINALE_ACTOR_WORDS*2,
        MEMF_CHIP|MEMF_CLEAR);
    stormFinaleGateMask=(UWORD *)AllocMem(STORM_FINALE_GATE_HALVES*
        STORM_FINALE_GATE_H*
        STORM_FINALE_GATE_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    stormFinaleGateBits=(UWORD *)AllocMem(STORM_FINALE_GATE_HALVES*
        FRONT_PLANES*STORM_FINALE_GATE_H*
        STORM_FINALE_GATE_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    stormFlightMask=(UWORD *)AllocMem(STORMRAIL_LAUNCH_BOB_H*
                                     STORM_VEHICLE_WORDS*2,MEMF_CHIP);
    stormFlightBits=(UWORD *)AllocMem(STORMRAIL_LAUNCH_BOB_H*
        STORM_VEHICLE_WORDS*FRONT_PLANES*2,MEMF_CHIP);
    stormCompactMask=(UWORD *)AllocMem(STORMRAIL_FLIGHT_BOB_H*
        STORM_COMPACT_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    stormCompactBits=(UWORD *)AllocMem(STORMRAIL_FLIGHT_BOB_H*
        STORM_COMPACT_WORDS*FRONT_PLANES*2,MEMF_CHIP|MEMF_CLEAR);
#ifdef SPARKPAW_STORMRAIL_DUST
    stormDustMask=(UWORD *)AllocMem(STORM_DUST_STYLES*STORM_DUST_H*
        STORM_DUST_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    stormDustBits=(UWORD *)AllocMem(STORM_DUST_STYLES*FRONT_PLANES*
        STORM_DUST_H*STORM_DUST_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
#endif
    if(!stormVehicleMask||!stormVehicleBits||!stormFlightMask||
       !stormFlightBits||!stormCompactMask||!stormCompactBits||!stormDroneMask||
       !stormDroneBits||!stormRockBigMask||!stormRockBigBits||
       !stormRockShardMask||!stormRockShardBits||
       !stormRockPillarMask||!stormRockPillarBits||
       !stormFinaleActorMask||!stormFinaleActorBits||
       !stormFinaleGateMask||!stormFinaleGateBits
#ifdef SPARKPAW_STORMRAIL_DUST
       ||!stormDustMask||!stormDustBits
#endif
       )
        return FALSE;
#ifdef SPARKPAW_STORMRAIL_DUST
    {
        static const UWORD shape[STORM_DUST_SHAPES][STORM_DUST_H]={
            {0x8000,0x0000,0x0000},
            {0xc000,0x0000,0x0000},
            {0xf000,0x4000,0x0000}};
        /* Neutral light, pale blue and rare warm sunlight on the same grit. */
        static const UBYTE colorPen[STORM_DUST_COLORS]={9,6,3};
        UBYTE shapeIndex,color,row;
        for(shapeIndex=0;shapeIndex<STORM_DUST_SHAPES;shapeIndex++)
            for(color=0;color<STORM_DUST_COLORS;color++) {
                UBYTE style=(UBYTE)(shapeIndex*STORM_DUST_COLORS+color);
                for(row=0;row<STORM_DUST_H;row++) {
                    UWORD bits=shape[shapeIndex][row];
                    stormDustMask[(LONG)style*STORM_DUST_H+row]=bits;
                    for(plane=0;plane<FRONT_PLANES;plane++)
                        if(colorPen[color]&(1<<plane))
                            stormDustBits[((LONG)style*FRONT_PLANES+plane)*
                                          STORM_DUST_H+row]=bits;
                }
            }
    }
#endif
    for(frame=0;frame<STORM_VEHICLE_FRAMES;frame++)
        for(y=0;y<STORM_VEHICLE_H;y++)
        for(x=0;x<STORM_VEHICLE_W;x++) {
            UBYTE pen=stormrailSourcePen((WORD)(frame*STORM_VEHICLE_W+x),y);
            LONG word=((LONG)frame*STORM_VEHICLE_H+y)*STORM_VEHICLE_WORDS+
                      (x>>4);
            UWORD bit=(UWORD)(0x8000U>>(x&15));
            if(!pen) continue;
            stormVehicleMask[word]|=bit;
            for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
                stormVehicleBits[((LONG)frame*FRONT_PLANES+plane)*
                    STORM_VEHICLE_H*STORM_VEHICLE_WORDS+
                    (LONG)y*STORM_VEHICLE_WORDS+(x>>4)]|=bit;
        }
    for(frame=0;frame<3;frame++) for(y=0;y<STORM_DRONE_H;y++)
        for(x=0;x<STORM_DRONE_W;x++) {
            UBYTE pen=stormrailSourcePen((WORD)(frame*STORM_DRONE_W+x),
                                         (WORD)(STORM_VEHICLE_H+y));
            LONG word=((LONG)frame*STORM_DRONE_H+y)*STORM_DRONE_WORDS+
                      (x>>4);
            UWORD bit=(UWORD)(0x8000U>>(x&15));
            if(!pen) continue;
            stormDroneMask[word]|=bit;
            for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
                stormDroneBits[((LONG)frame*FRONT_PLANES+plane)*
                    STORM_DRONE_H*STORM_DRONE_WORDS+
                    (LONG)y*STORM_DRONE_WORDS+(x>>4)]|=bit;
        }
    for(frame=0;frame<STORM_ROCK_BIG_FRAMES;frame++)
        for(y=0;y<STORMRAIL_OBSTACLE_BIG_H;y++)
        for(x=0;x<STORMRAIL_OBSTACLE_BIG_W;x++) {
            UBYTE pen=stormrailObstacleSourcePen(
                (WORD)(frame*STORMRAIL_OBSTACLE_BIG_W+x),y);
            LONG word=((LONG)frame*STORMRAIL_OBSTACLE_BIG_H+y)*
                      STORM_ROCK_BIG_WORDS+(x>>4);
            UWORD bit=(UWORD)(0x8000U>>(x&15));
            if(!pen) continue;
            stormRockBigMask[word]|=bit;
            for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
                stormRockBigBits[((LONG)frame*FRONT_PLANES+plane)*
                    STORMRAIL_OBSTACLE_BIG_H*STORM_ROCK_BIG_WORDS+
                    (LONG)y*STORM_ROCK_BIG_WORDS+(x>>4)]|=bit;
        }
    for(frame=0;frame<STORM_ROCK_SHARD_FRAMES;frame++)
        for(y=0;y<STORMRAIL_OBSTACLE_SHARD_H;y++)
        for(x=0;x<STORMRAIL_OBSTACLE_SHARD_W;x++) {
            UBYTE pen=stormrailObstacleSourcePen(
                (WORD)(240+frame*STORMRAIL_OBSTACLE_SHARD_W+x),
                (WORD)(20+y));
            LONG word=((LONG)frame*STORMRAIL_OBSTACLE_SHARD_H+y)*
                      STORM_ROCK_SHARD_WORDS+(x>>4);
            UWORD bit=(UWORD)(0x8000U>>(x&15));
            if(!pen) continue;
            stormRockShardMask[word]|=bit;
            for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
                stormRockShardBits[((LONG)frame*FRONT_PLANES+plane)*
                    STORMRAIL_OBSTACLE_SHARD_H*STORM_ROCK_SHARD_WORDS+
                    (LONG)y*STORM_ROCK_SHARD_WORDS+(x>>4)]|=bit;
        }
    for(frame=0;frame<STORM_ROCK_PILLAR_FRAMES;frame++)
    for(y=0;y<STORMRAIL_OBSTACLE_PILLAR_H;y++)
        for(x=0;x<STORMRAIL_OBSTACLE_PILLAR_W;x++) {
            UBYTE pen=stormrailObstacleSourcePen(
                (WORD)(336+frame*STORMRAIL_OBSTACLE_PILLAR_W+x),y);
            LONG word=((LONG)frame*STORMRAIL_OBSTACLE_PILLAR_H+y)*
                      STORM_ROCK_PILLAR_WORDS+(x>>4);
            UWORD bit=(UWORD)(0x8000U>>(x&15));
            if(!pen) continue;
            stormRockPillarMask[word]|=bit;
            for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
                stormRockPillarBits[((LONG)frame*FRONT_PLANES+plane)*
                    STORMRAIL_OBSTACLE_PILLAR_H*STORM_ROCK_PILLAR_WORDS+
                    (LONG)y*STORM_ROCK_PILLAR_WORDS+(x>>4)]|=bit;
        }
    for(y=0;y<STORMRAIL_LAUNCH_BOB_H;y++) {
        LONG source=(LONG)(42+y)*STORM_VEHICLE_WORDS;
        LONG target=(LONG)y*STORM_VEHICLE_WORDS;
        CopyMem(stormVehicleMask+(LONG)4*STORM_VEHICLE_H*
                    STORM_VEHICLE_WORDS+source,
                stormFlightMask+target,STORM_VEHICLE_WORDS*2);
        for(plane=0;plane<FRONT_PLANES;plane++)
            CopyMem(stormVehicleBits+((LONG)4*FRONT_PLANES+plane)*
                        STORM_VEHICLE_H*STORM_VEHICLE_WORDS+source,
                    stormFlightBits+((LONG)plane*STORMRAIL_LAUNCH_BOB_H+y)*
                        STORM_VEHICLE_WORDS,STORM_VEHICLE_WORDS*2);
    }
    for(y=0;y<STORMRAIL_FLIGHT_BOB_H;y++) for(x=0;x<STORMRAIL_FLIGHT_BOB_W;x++) {
        UBYTE pen=stormrailSourcePen((WORD)(5*STORM_VEHICLE_W+x),y);
        LONG word=(LONG)y*STORM_COMPACT_WORDS+(x>>4);
        UWORD bit=(UWORD)(0x8000U>>(x&15));
        if(!pen) continue;
        stormCompactMask[word]|=bit;
        for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
            stormCompactBits[((LONG)plane*STORMRAIL_FLIGHT_BOB_H+y)*
                            STORM_COMPACT_WORDS+(x>>4)]|=bit;
    }
    /* Gate-6 native art: one startup-built planar family at final size. */
    for(frame=0;frame<STORMRAIL_FINALE_ACTOR_COUNT;frame++)
        for(y=0;y<STORM_FINALE_ACTOR_H;y++)
        for(x=0;x<STORM_FINALE_ACTOR_W;x++) {
            BOOL inside;
            UBYTE pen;
            LONG word;
            UWORD bit;
            pen=stormGate6PackedPen(0,80,
                (WORD)(x+STORMRAIL_FINALE_RENDER_X_OFFSET),
                (WORD)(y+STORMRAIL_FINALE_RENDER_Y_OFFSET));
            inside=(BOOL)(pen!=0);
            if(!inside) continue;
            word=((LONG)frame*STORM_FINALE_ACTOR_H+y)*
                 STORM_FINALE_ACTOR_WORDS+(x>>4);
            bit=(UWORD)(0x8000U>>(x&15));
            stormFinaleActorMask[word]|=bit;
            for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
                stormFinaleActorBits[((LONG)frame*FRONT_PLANES+plane)*
                    STORM_FINALE_ACTOR_H*STORM_FINALE_ACTOR_WORDS+
                    (LONG)y*STORM_FINALE_ACTOR_WORDS+(x>>4)]|=bit;
        }
    for(frame=0;frame<STORM_FINALE_GATE_HALVES;frame++)
    for(y=0;y<STORM_FINALE_GATE_H;y++) for(x=0;x<STORM_FINALE_GATE_W;x++) {
        UBYTE pen=stormFinaleGatePen(frame,x,y);
        LONG word=((LONG)frame*STORM_FINALE_GATE_H+y)*
                  STORM_FINALE_GATE_WORDS+(x>>4);
        UWORD bit=(UWORD)(0x8000U>>(x&15));
        if(!pen) continue;
        stormFinaleGateMask[word]|=bit;
        for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
            stormFinaleGateBits[(((LONG)frame*FRONT_PLANES+plane)*
                                 STORM_FINALE_GATE_H+y)*
                                STORM_FINALE_GATE_WORDS+(x>>4)]|=bit;
    }
    return TRUE;
}

static void restoreStormrailBobs(void)
{
    if(!game->stormrailActive) return;
    UBYTE shot;
    stormFinaleGateClearDirty();
#ifdef SPARKPAW_STORMRAIL_FINALE_GATE_OVERLAY_CACHE
    if(stormrailHistory.finaleGateResident&&
       !stormrailFinaleGateResident(game->stormrailFinalePhase,
                                    game->stormrailFinaleGateOpen)) {
        WORD gateWorldX=(WORD)(game->cameraX+STORMRAIL_FINALE_GATE_X);
        WORD gatePhysicalX=prototypePhysicalX(gateWorldX);
        stormrailHistory.finaleGateResident=FALSE;
        for(shot=0;shot<2;shot++)
            blitRestoreRect(gateWorldX,gatePhysicalX,
                (WORD)(shot?STORM_FINALE_GATE_H:0),
                STORM_FINALE_GATE_W,STORM_FINALE_GATE_H);
    }
#endif
#ifdef SPARKPAW_STORMRAIL_DUST
    for(shot=0;shot<STORM_DUST_SLOTS;shot++)
        if(stormrailHistory.dustDrawn[shot]) {
            blitRestoreRect(stormrailHistory.dustWorldX[shot],
                stormrailHistory.dustX[shot],stormrailHistory.dustY[shot],
                STORM_DUST_W,STORM_DUST_H);
            stormrailHistory.dustDrawn[shot]=FALSE;
        }
#endif
    for(shot=0;shot<STORMRAIL_MAX_SHOTS;shot++)
        if(stormrailHistory.shotDrawn[shot]) {
            if(!stormrailVerticalRectSafe(stormrailHistory.shotY[shot],
                                          PROJECTILE_H)) {
                stormrailUnsafeBlits++;
                stormrailHistory.shotDrawn[shot]=FALSE;
                continue;
            }
            blitRestoreRect(stormrailHistory.shotWorldX[shot],
                            stormrailHistory.shotX[shot],
                            stormrailHistory.shotY[shot],
                            PROJECTILE_W,PROJECTILE_H);
            stormrailHistory.shotDrawn[shot]=FALSE;
        }
    for(shot=0;shot<STORMRAIL_MAX_ENEMIES;shot++) if(stormrailHistory.enemyDrawn[shot]) {
        blitRestoreRect(stormrailHistory.enemyWorldX[shot],stormrailHistory.enemyX[shot],
                        stormrailHistory.enemyY[shot],STORM_DRONE_W,STORM_DRONE_H);
        stormrailHistory.enemyDrawn[shot]=FALSE;
    }
    for(shot=0;shot<STORMRAIL_MAX_HOSTILE_SHOTS;shot++) if(stormrailHistory.hostileDrawn[shot]) {
        blitRestoreRect(stormrailHistory.hostileWorldX[shot],stormrailHistory.hostileX[shot],
                        stormrailHistory.hostileY[shot],PROJECTILE_W,PROJECTILE_H);
        stormrailHistory.hostileDrawn[shot]=FALSE;
    }
    for(shot=0;shot<STORMRAIL_MAX_REWARDS;shot++) if(stormrailHistory.rewardDrawn[shot]) {
        blitRestoreRect(stormrailHistory.rewardWorldX[shot],stormrailHistory.rewardX[shot],
                        stormrailHistory.rewardY[shot],COLLECTIBLE_W,COLLECTIBLE_H);
        stormrailHistory.rewardDrawn[shot]=FALSE;
    }
    for(shot=0;shot<STORMRAIL_MAX_OBSTACLES;shot++)
        if(stormrailHistory.obstacleDrawn[shot]) {
            UBYTE type=stormrailHistory.obstacleType[shot];
            WORD width=(WORD)stormrailObstacleWidth(type);
            WORD height=(WORD)stormrailObstacleHeight(type);
            blitRestoreRect(stormrailHistory.obstacleWorldX[shot],
                            stormrailHistory.obstacleX[shot],
                            stormrailHistory.obstacleY[shot],width,height);
            stormrailHistory.obstacleDrawn[shot]=FALSE;
        }
    for(shot=0;shot<STORMRAIL_FINALE_ACTOR_COUNT;shot++)
        if(stormrailHistory.finaleActorDrawn[shot]) {
            blitRestoreRect(stormrailHistory.finaleActorWorldX[shot],
                stormrailHistory.finaleActorX[shot],
                stormrailHistory.finaleActorY[shot],
                STORMRAIL_FINALE_RENDER_W,
                STORMRAIL_FINALE_RENDER_H);
            stormrailHistory.finaleActorDrawn[shot]=FALSE;
        }
    for(shot=0;shot<2;shot++) if(stormrailHistory.finaleGateDrawn[shot]) {
        blitRestoreRect(stormrailHistory.finaleGateWorldX[shot],
            stormrailHistory.finaleGateX[shot],stormrailHistory.finaleGateY[shot],
            STORM_FINALE_GATE_W,STORM_FINALE_GATE_H);
        stormrailHistory.finaleGateDrawn[shot]=FALSE;
    }
    for(shot=0;shot<STORMRAIL_FINALE_ATTACK_COUNT;shot++)
        if(stormrailHistory.finaleWarningDrawn[shot]) {
            blitRestoreRect(stormrailHistory.finaleWarningWorldX[shot],
                stormrailHistory.finaleWarningX[shot],
                stormrailHistory.finaleWarningY[shot],
                PROJECTILE_W,PROJECTILE_H);
            stormrailHistory.finaleWarningDrawn[shot]=FALSE;
        }
    if(stormrailHistory.vehicleDrawn) {
        if(!stormrailVerticalRectSafe(stormrailHistory.vehicleY,
                                      stormrailHistory.vehicleHeight)) {
            stormrailUnsafeBlits++;
            stormrailHistory.vehicleDrawn=FALSE;
            return;
        }
        blitRestoreRect(stormrailHistory.vehicleWorldX,
                        stormrailHistory.vehicleX,stormrailHistory.vehicleY,
                        stormrailHistory.vehicleWidth,
                        stormrailHistory.vehicleHeight);
        stormrailHistory.vehicleDrawn=FALSE;
    }
}

static void drawStormrailBobs(void)
{
    WORD worldX,physicalX,vehicleY,vehicleWidth,vehicleHeight,sourceWords;
    UBYTE shot;
    UBYTE flightPose;
    if(!game->stormrailActive) return;
    flightPose=game->stormrailMode>=STORMRAIL_MODE_LAUNCH_OUT;
    UBYTE compactPose=game->stormrailMode==STORMRAIL_MODE_FLIGHT;
    UBYTE frame=flightPose?4:
        game->stormrailMode==STORMRAIL_MODE_BOARDING&&
            game->stormrailBoardTimer>=36?4:
        game->stormrailMode==STORMRAIL_MODE_BOARDING&&
            game->stormrailBoardTimer>=20?3:
        game->stormrailMode==STORMRAIL_MODE_BOARDING&&
            game->stormrailBoardTimer>=12?2:
        game->stormrailMode==STORMRAIL_MODE_BOARDING&&
            game->stormrailBoardTimer>=4?1:0;
#ifdef SPARKPAW_STORMRAIL_DUST
    if(game->stormrailMode==STORMRAIL_MODE_FLIGHT) {
        static const UWORD phase[STORM_DUST_SLOTS]={
            7,47,91,139,181,223,263,307};
        static const UBYTE speedEighths[STORM_DUST_SLOTS]={
            2,3,4,5,6,8,3,7};
        static const UBYTE particleY[STORM_DUST_SLOTS]={
            41,67,94,121,149,178,205,18};
        static const UBYTE shape[STORM_DUST_SLOTS]={0,1,0,2,1,0,1,2};
        static const UBYTE lightCycle[4]={0,1,0,2};
        for(shot=0;shot<STORM_DUST_SLOTS;shot++) {
            ULONG dustDistance=game->stormrailDistance+
                (game->stormrailFinaleActive?
                    (ULONG)game->stormrailFinaleVisualTick*4:0);
            UBYTE color=lightCycle[((dustDistance>>8)+shot)&3];
            UBYTE style=(UBYTE)(shape[shot]*STORM_DUST_COLORS+color);
            ULONG travel=((dustDistance*speedEighths[shot])>>3)+
                         phase[shot];
            WORD screenX=(WORD)(304-(travel%320));
            if(screenX>=0) {
                worldX=(WORD)(game->cameraX+screenX);
                physicalX=prototypePhysicalX(worldX);
                blitMaskedBob(stormDustMask+(LONG)style*STORM_DUST_H*
                                  STORM_DUST_WORDS,
                    stormDustBits+(LONG)style*FRONT_PLANES*STORM_DUST_H*
                                  STORM_DUST_WORDS,
                    STORM_DUST_WORDS,STORM_DUST_W,STORM_DUST_H,
                    physicalX,particleY[shot]);
                stormFinaleGateMarkDamage(worldX,particleY[shot],
                                           STORM_DUST_W,STORM_DUST_H);
                stormrailHistory.dustDrawn[shot]=TRUE;
                stormrailHistory.dustX[shot]=physicalX;
                stormrailHistory.dustWorldX[shot]=worldX;
                stormrailHistory.dustY[shot]=particleY[shot];
            }
        }
    }
#endif
    /* Slow two-pixel triangle wave: powered, but still waiting for Paw. */
    vehicleWidth=compactPose?STORMRAIL_FLIGHT_BOB_W:STORM_VEHICLE_W;
    vehicleHeight=compactPose?STORMRAIL_FLIGHT_BOB_H:
                  flightPose?STORMRAIL_LAUNCH_BOB_H:STORM_VEHICLE_H;
    sourceWords=compactPose?STORM_COMPACT_WORDS:STORM_VEHICLE_WORDS;
    vehicleY=flightPose?
        (WORD)(game->stormrailY+STORMRAIL_FLIGHT_BOB_Y_OFFSET):
        (WORD)(game->stormrailY-42);
    if(game->stormrailMode==STORMRAIL_MODE_APPROACH||
       (flightPose&&!compactPose)) {
        UBYTE hover=(UBYTE)((game->frameCounter>>3)&3);
        WORD hoverOffset=(WORD)(hover<2?hover:4-hover);
        /* At the honest top bound the transparent cache cell already starts
           on row zero. Suspend only the cosmetic lift there; never let hover
           turn a valid control position into an out-of-bounds Blit. */
        if(vehicleY>=hoverOffset) vehicleY-=hoverOffset;
    }
    worldX=STORMRAIL_BOARDING_SCREEN_X;
    if(game->stormrailMode==STORMRAIL_MODE_BOARDING&&
       game->stormrailBoardTimer>40) {
        WORD progress=(WORD)(game->stormrailBoardTimer-40);
        if(progress>24) progress=24;
        worldX+=(WORD)((game->cameraX*progress)/24);
    } else if(flightPose)
        worldX=(WORD)stormrailFlightWorldX(game->cameraX,game->stormrailX);
    if(stormrailVerticalRectSafe(vehicleY,vehicleHeight)&&
       prototypeRectFits(worldX,vehicleWidth)&&
       !(game->stormrailMode==STORMRAIL_MODE_FLIGHT&&
         game->stormrailInvuln&&
         (game->stormrailInvuln&4))) {
        physicalX=prototypePhysicalX(worldX);
        blitMaskedBob(compactPose?stormCompactMask:flightPose?stormFlightMask:
                          stormVehicleMask+(LONG)frame*STORM_VEHICLE_H*
                              STORM_VEHICLE_WORDS,
                      compactPose?stormCompactBits:flightPose?stormFlightBits:
                          stormVehicleBits+(LONG)frame*FRONT_PLANES*
                              STORM_VEHICLE_H*STORM_VEHICLE_WORDS,
                      sourceWords,vehicleWidth,vehicleHeight,
                      physicalX,vehicleY);
        stormFinaleGateMarkDamage(worldX,vehicleY,
                                   vehicleWidth,vehicleHeight);
        stormrailHistory.vehicleDrawn=TRUE;
        stormrailHistory.vehicleX=physicalX;
        stormrailHistory.vehicleWorldX=worldX;
        stormrailHistory.vehicleY=vehicleY;
        stormrailHistory.vehicleFrame=frame;
        stormrailHistory.vehicleWidth=(UBYTE)vehicleWidth;
        stormrailHistory.vehicleHeight=(UBYTE)vehicleHeight;
    } else if(!stormrailVerticalRectSafe(vehicleY,vehicleHeight))
        stormrailUnsafeBlits++;
    if(game->stormrailMode!=STORMRAIL_MODE_FLIGHT) return;
    if(game->stormrailFinaleActive) {
        WORD gateX=(WORD)(STORMRAIL_FINALE_GATE_X+
            stormrailFinaleGateOpenOffset(game->stormrailFinaleGateOpen)+
            stormrailFinaleGateArrivalOffset(game->stormrailFinalePhase,
                                              game->stormrailFinaleTick));
        if(game->stormrailFinaleGateOpen<STORMRAIL_FINALE_OPEN_TICKS&&
           stormrailFinaleGateResident(game->stormrailFinalePhase,
                                       game->stormrailFinaleGateOpen)&&
           stormrailHistory.finaleGateResident) {
            stormFinaleGateRepairDirty();
        } else if(game->stormrailFinaleGateOpen<STORMRAIL_FINALE_OPEN_TICKS)
            for(shot=0;shot<2;shot++) {
                WORD gateY=(WORD)(shot?STORM_FINALE_GATE_H:0);
                worldX=(WORD)(game->cameraX+gateX);
                physicalX=prototypePhysicalX(worldX);
                blitMaskedBob(stormFinaleGateMask+(LONG)shot*
                        STORM_FINALE_GATE_H*STORM_FINALE_GATE_WORDS,
                    stormFinaleGateBits+(LONG)shot*FRONT_PLANES*
                        STORM_FINALE_GATE_H*STORM_FINALE_GATE_WORDS,
                    STORM_FINALE_GATE_WORDS,STORM_FINALE_GATE_W,
                    STORM_FINALE_GATE_H,physicalX,gateY);
#ifdef SPARKPAW_STORMRAIL_FINALE_GATE_OVERLAY_CACHE
                if(stormrailFinaleGateResident(
                       game->stormrailFinalePhase,
                       game->stormrailFinaleGateOpen)) {
                    stormrailHistory.finaleGateResident=TRUE;
                    continue;
                }
#endif
                stormrailHistory.finaleGateDrawn[shot]=TRUE;
                stormrailHistory.finaleGateX[shot]=physicalX;
                stormrailHistory.finaleGateWorldX[shot]=worldX;
                stormrailHistory.finaleGateY[shot]=gateY;
            }
        for(shot=0;shot<STORMRAIL_FINALE_ACTOR_COUNT;shot++)
            if(game->stormrailFinaleHp[shot]&&
               (!game->stormrailFinaleFlash[shot]||
                !(game->stormrailFinaleFlash[shot]&1))) {
                WORD actorX=stormrailFinaleActorX(
                    game->stormrailFinaleVisualTick,shot);
                WORD actorY=stormrailFinaleActorY(
                    game->stormrailFinaleVisualTick,shot);
                actorX=(WORD)(actorX+stormrailFinaleHarrierArrivalOffset(
                    game->stormrailFinalePhase,game->stormrailFinaleTick));
                actorX=(WORD)(actorX+STORMRAIL_FINALE_RENDER_X_OFFSET);
                actorY=(WORD)(actorY+STORMRAIL_FINALE_RENDER_Y_OFFSET);
                worldX=(WORD)(game->cameraX+actorX);
                physicalX=prototypePhysicalX(worldX);
                blitMaskedBobTargetStride(frontDisplay,
                    stormFinaleActorMask+(LONG)shot*
                        STORM_FINALE_ACTOR_H*STORM_FINALE_ACTOR_WORDS,
                    stormFinaleActorBits+(LONG)shot*FRONT_PLANES*
                        STORM_FINALE_ACTOR_H*STORM_FINALE_ACTOR_WORDS,
                    STORM_FINALE_ACTOR_WORDS,STORM_FINALE_ACTOR_H,
                    STORMRAIL_FINALE_RENDER_W,STORMRAIL_FINALE_RENDER_H,
                    physicalX,actorY);
                stormrailHistory.finaleActorDrawn[shot]=TRUE;
                stormrailHistory.finaleActorX[shot]=physicalX;
                stormrailHistory.finaleActorWorldX[shot]=worldX;
                stormrailHistory.finaleActorY[shot]=actorY;
            }
        if(game->stormrailFinalePhase==STORMRAIL_FINALE_PHASE_COMBAT)
            for(shot=0;shot<STORMRAIL_FINALE_ATTACK_COUNT;shot++)
                if(game->stormrailFinaleHp[stormrailFinaleAttacks[shot].actor]&&
                   stormrailFinaleAttackEnabled(
                       game->stormrailFinaleHp[STORMRAIL_FINALE_HARRIER],shot)&&
                   stormrailFinaleAttackTelegraphing(
                       game->stormrailFinaleTick,shot)&&
                   (game->frameCounter&2)) {
                    const struct StormrailFinaleActorContract *actor=
                        &stormrailFinaleActors[stormrailFinaleAttacks[shot].actor];
                    UBYTE actorId=stormrailFinaleAttacks[shot].actor;
                    UBYTE warningPattern=(UBYTE)(PLAYER_PLASMA_PATTERNS+
                        (stormrailFinaleAttacks[shot].kind==
                         STORMRAIL_FINALE_ATTACK_HUNTER?4:2));
                    WORD warningX=(WORD)(stormrailFinaleActorX(
                        game->stormrailFinaleVisualTick,actorId)-8);
                    WORD warningY=(WORD)(stormrailFinaleActorY(
                        game->stormrailFinaleVisualTick,actorId)+
                        actor->height/2-4);
                    worldX=(WORD)(game->cameraX+warningX);
                    physicalX=prototypePhysicalX(worldX);
                    blitMaskedBob(plasmaMaskRow(warningPattern,TRUE,0),
                        plasmaBitsRow(warningPattern,TRUE,0,0),
                        PLASMA_SOURCE_WORDS,PROJECTILE_W,PROJECTILE_H,
                        physicalX,warningY);
                    stormrailHistory.finaleWarningDrawn[shot]=TRUE;
                    stormrailHistory.finaleWarningX[shot]=physicalX;
                    stormrailHistory.finaleWarningWorldX[shot]=worldX;
                    stormrailHistory.finaleWarningY[shot]=warningY;
                }
    }
    for(shot=0;shot<STORMRAIL_MAX_SHOTS;shot++) {
      if(game->stormrailShotActive[shot]) {
        worldX=(WORD)(game->cameraX+game->stormrailShotX[shot]);
        if(stormrailVerticalRectSafe(game->stormrailShotY[shot],PROJECTILE_H)&&
           prototypeRectFits(worldX,PROJECTILE_W)) {
            physicalX=prototypePhysicalX(worldX);
            blitMaskedBob(plasmaMaskRow(0,FALSE,0),
                          plasmaBitsRow(0,FALSE,0,0),
                          PLASMA_SOURCE_WORDS,PROJECTILE_W,PROJECTILE_H,
                          physicalX,game->stormrailShotY[shot]);
            stormrailHistory.shotDrawn[shot]=TRUE;
            stormrailHistory.shotX[shot]=physicalX;
            stormrailHistory.shotWorldX[shot]=worldX;
            stormrailHistory.shotY[shot]=game->stormrailShotY[shot];
        } else if(!stormrailVerticalRectSafe(game->stormrailShotY[shot],
                                              PROJECTILE_H))
            stormrailUnsafeBlits++;
      }
    }
    for(shot=0;shot<STORMRAIL_MAX_ENEMIES;shot++) if(game->stormrailEnemyActive[shot]) {
        UBYTE enemyFrame=game->stormrailEnemyKind[shot];
        worldX=(WORD)(game->cameraX+game->stormrailEnemyX[shot]);
        if(stormrailVerticalRectSafe(game->stormrailEnemyY[shot],STORM_DRONE_H)&&prototypeRectFits(worldX,STORM_DRONE_W)) {
            physicalX=prototypePhysicalX(worldX);
            blitMaskedBob(stormDroneMask+(LONG)enemyFrame*STORM_DRONE_H*STORM_DRONE_WORDS,
                stormDroneBits+(LONG)enemyFrame*FRONT_PLANES*STORM_DRONE_H*STORM_DRONE_WORDS,
                STORM_DRONE_WORDS,STORM_DRONE_W,STORM_DRONE_H,physicalX,game->stormrailEnemyY[shot]);
            stormrailHistory.enemyDrawn[shot]=TRUE; stormrailHistory.enemyX[shot]=physicalX;
            stormrailHistory.enemyWorldX[shot]=worldX; stormrailHistory.enemyY[shot]=game->stormrailEnemyY[shot];
            stormrailHistory.enemyFrame[shot]=enemyFrame;
        } else if(!stormrailVerticalRectSafe(game->stormrailEnemyY[shot],
                                              STORM_DRONE_H))
            stormrailUnsafeBlits++;
    }
    for(shot=0;shot<STORMRAIL_MAX_OBSTACLES;shot++)
        if(game->stormrailObstacleActive[shot]) {
            UBYTE type=game->stormrailObstacleType[shot];
            UBYTE obstacleFrame=game->stormrailObstacleFrame[shot];
            WORD width=(WORD)stormrailObstacleWidth(type);
            WORD height=(WORD)stormrailObstacleHeight(type);
            WORD words=(WORD)(type==STORMRAIL_OBSTACLE_BIG?
                STORM_ROCK_BIG_WORDS:type==STORMRAIL_OBSTACLE_PILLAR?
                STORM_ROCK_PILLAR_WORDS:STORM_ROCK_SHARD_WORDS);
            UWORD *mask=type==STORMRAIL_OBSTACLE_BIG?
                stormRockBigMask+(LONG)obstacleFrame*height*words:
                type==STORMRAIL_OBSTACLE_PILLAR?
                stormRockPillarMask+(LONG)obstacleFrame*height*words:
                stormRockShardMask+(LONG)obstacleFrame*height*words;
            UWORD *bits=type==STORMRAIL_OBSTACLE_BIG?
                stormRockBigBits+(LONG)obstacleFrame*FRONT_PLANES*height*words:
                type==STORMRAIL_OBSTACLE_PILLAR?
                stormRockPillarBits+(LONG)obstacleFrame*FRONT_PLANES*height*words:
                stormRockShardBits+(LONG)obstacleFrame*FRONT_PLANES*height*words;
            worldX=(WORD)(game->cameraX+game->stormrailObstacleX[shot]);
            if((!game->stormrailObstacleFlash[shot]||
                !(game->stormrailObstacleFlash[shot]&1))&&
               stormrailVerticalRectSafe(game->stormrailObstacleY[shot],height)&&
               prototypeRectFits(worldX,width)) {
                physicalX=prototypePhysicalX(worldX);
                blitMaskedBob(mask,bits,words,width,height,physicalX,
                              game->stormrailObstacleY[shot]);
                stormrailHistory.obstacleDrawn[shot]=TRUE;
                stormrailHistory.obstacleX[shot]=physicalX;
                stormrailHistory.obstacleWorldX[shot]=worldX;
                stormrailHistory.obstacleY[shot]=game->stormrailObstacleY[shot];
                stormrailHistory.obstacleType[shot]=type;
                stormrailHistory.obstacleFrame[shot]=obstacleFrame;
            } else if(!stormrailVerticalRectSafe(
                          game->stormrailObstacleY[shot],height))
                stormrailUnsafeBlits++;
        }
    for(shot=0;shot<STORMRAIL_MAX_HOSTILE_SHOTS;shot++) if(game->stormrailHostileActive[shot]) {
        UBYTE hostilePattern=(UBYTE)(PLAYER_PLASMA_PATTERNS+
            (game->stormrailHostileKind[shot]==2?4:
             game->stormrailHostileKind[shot]?2+((game->frameCounter>>1)&1):
             ((game->frameCounter>>1)&1)));
        worldX=(WORD)(game->cameraX+game->stormrailHostileX[shot]);
        if(stormrailVerticalRectSafe(game->stormrailHostileY[shot],PROJECTILE_H)&&prototypeRectFits(worldX,PROJECTILE_W)) {
            physicalX=prototypePhysicalX(worldX);
            blitMaskedBob(plasmaMaskRow(hostilePattern,TRUE,0),
                plasmaBitsRow(hostilePattern,TRUE,0,0),PLASMA_SOURCE_WORDS,
                PROJECTILE_W,PROJECTILE_H,physicalX,
                game->stormrailHostileY[shot]);
            stormrailHistory.hostileDrawn[shot]=TRUE; stormrailHistory.hostileX[shot]=physicalX;
            stormrailHistory.hostileWorldX[shot]=worldX; stormrailHistory.hostileY[shot]=game->stormrailHostileY[shot];
        }
    }
    for(shot=0;shot<STORMRAIL_MAX_REWARDS;shot++) if(game->stormrailRewardActive[shot]) {
        worldX=(WORD)(game->cameraX+game->stormrailRewardX[shot]);
        if(stormrailVerticalRectSafe(game->stormrailRewardY[shot],COLLECTIBLE_H)&&prototypeRectFits(worldX,COLLECTIBLE_W)) {
            physicalX=prototypePhysicalX(worldX);
            UWORD *rewardMask=game->stormrailRewardKind[shot]==STORMRAIL_REWARD_HEART?
                heartMask:diamondMask;
            UWORD *rewardBits=game->stormrailRewardKind[shot]==STORMRAIL_REWARD_HEART?
                heartBits:diamondBits;
            blitMaskedBob(rewardMask,rewardBits,DIAMOND_SOURCE_WORDS,
                COLLECTIBLE_W,COLLECTIBLE_H,physicalX,
                game->stormrailRewardY[shot]);
            stormrailHistory.rewardDrawn[shot]=TRUE; stormrailHistory.rewardX[shot]=physicalX;
            stormrailHistory.rewardWorldX[shot]=worldX; stormrailHistory.rewardY[shot]=game->stormrailRewardY[shot];
        }
    }
}
#endif

static void blitRestoreRect(WORD sourceX,WORD x,WORD y,WORD width,WORD height)
{
    UBYTE plane; WORD localX=x;
    const struct BitMap *clean=frontClean->bitmap;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    /* Target-local histories already store physical triplicated-ring X. */
    localX=x;
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
    clean=prototypeTarget[prototypePreparedCopper].clean;
#else
    clean=frontClean->bitmap;
#endif
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive&&game->stormrailMode==STORMRAIL_MODE_FLIGHT&&
       stormFlightTargetClean[prototypePreparedCopper])
        clean=stormFlightBlank;
#endif
#endif
    {
    UWORD words=(UWORD)stormrailRestoreWordCount(localX,width);
#ifdef SPARKPAW_STORMRAIL_FINALE_GATE_OVERLAY_CACHE
    stormFinaleGateMarkDamage(
        stormrailRestoreFootprintWorldX(sourceX,localX),y,
        stormrailRestoreFootprintWidth(localX,width),height);
#endif
    LONG at=(LONG)y*frontDisplay->BytesPerRow+(localX>>4)*2;
    LONG sourceAt=(LONG)y*clean->BytesPerRow+
#ifdef SPARKPAW_STORMRAIL_PROOF
        (game->stormrailActive&&game->stormrailMode==STORMRAIL_MODE_FLIGHT&&
         stormFlightTargetClean[prototypePreparedCopper]?0:(sourceX>>4)*2);
#else
        (sourceX>>4)*2;
#endif
    for(plane=0;plane<FRONT_PLANES;plane++) {
        platformWaitBlit();
#ifndef SPARKPAW_BOB_PER_PLANE_SETUP_REFERENCE
        if(!plane) {
#endif
        hw->bltcon0=0x09f0; hw->bltcon1=0;
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=(UWORD)(clean->BytesPerRow-words*2);
        hw->bltdmod=(UWORD)(frontDisplay->BytesPerRow-words*2);
#ifndef SPARKPAW_BOB_PER_PLANE_SETUP_REFERENCE
        }
#endif
        hw->bltapt=clean->Planes[plane]+sourceAt;
        hw->bltdpt=frontDisplay->Planes[plane]+at;
        hw->bltsize=(UWORD)((height<<6)|words);
    }
    }
}

#ifdef SPARKPAW_STORMRAIL_PROOF
static void blitMaskedBobTargetStride(struct BitMap *target,UWORD *mask,
                                UWORD *bits,WORD sourceWords,WORD planeRows,
                                WORD width,WORD height,WORD x,WORD y);

static void blitMaskedBobTarget(struct BitMap *target,UWORD *mask,UWORD *bits,
                                WORD sourceWords,WORD width,WORD height,
                                WORD x,WORD y)
{
    blitMaskedBobTargetStride(target,mask,bits,sourceWords,height,width,height,x,y);
}

static void blitMaskedBobTargetStride(struct BitMap *target,UWORD *mask,
                                UWORD *bits,WORD sourceWords,WORD planeRows,
                                WORD width,WORD height,WORD x,WORD y)
{
#else
static void blitMaskedBobTarget(struct BitMap *target,UWORD *mask,UWORD *bits,
                                WORD sourceWords,WORD width,WORD height,
                                WORD x,WORD y)
{
#endif
    UBYTE plane; UWORD shift=(UWORD)(x&15);
    UWORD words=(UWORD)((shift+width+15)>>4);
    LONG at=(LONG)y*target->BytesPerRow+(x>>4)*2;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        platformWaitBlit();
#ifndef SPARKPAW_BOB_PER_PLANE_SETUP_REFERENCE
        if(!plane) {
#endif
        hw->bltcon0=(UWORD)((shift<<12)|0x0fca);
        hw->bltcon1=(UWORD)(shift<<12);
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=(UWORD)((sourceWords-words)*2);
        hw->bltbmod=(UWORD)((sourceWords-words)*2);
        hw->bltcmod=(UWORD)(target->BytesPerRow-words*2);
        hw->bltdmod=(UWORD)(target->BytesPerRow-words*2);
#ifndef SPARKPAW_BOB_PER_PLANE_SETUP_REFERENCE
        }
#endif
        hw->bltapt=mask;
#ifdef SPARKPAW_STORMRAIL_PROOF
        hw->bltbpt=bits+(LONG)plane*planeRows*sourceWords;
#else
        hw->bltbpt=bits+(LONG)plane*height*sourceWords;
#endif
        hw->bltcpt=target->Planes[plane]+at;
        hw->bltdpt=target->Planes[plane]+at;
        hw->bltsize=(UWORD)((height<<6)|words);
    }
}

static void blitMaskedBob(UWORD *mask,UWORD *bits,WORD sourceWords,
                          WORD width,WORD height,WORD x,WORD y)
{
    blitMaskedBobTarget(frontDisplay,mask,bits,sourceWords,
                        width,height,x,y);
}

static BOOL buildDiamondPattern(void)
{
    UBYTE plane; WORD x,y;
    diamondMask=(UWORD *)AllocMem(COLLECTIBLE_H*DIAMOND_SOURCE_WORDS*2,
                                  MEMF_CHIP|MEMF_CLEAR);
    diamondBits=(UWORD *)AllocMem(COLLECTIBLE_H*DIAMOND_SOURCE_WORDS*
                                  FRONT_PLANES*2,
                                  MEMF_CHIP|MEMF_CLEAR);
    if(!diamondMask||!diamondBits) return FALSE;
    for(y=0;y<COLLECTIBLE_H;y++) for(x=0;x<COLLECTIBLE_W;x++) {
        UBYTE sourceMask=(UBYTE)(0x80>>(x&7));
        LONG sourceAt=(LONG)y*diamondSprite->rowBytes+(x>>3);
        UWORD bit=(UWORD)(0x8000U>>x);
        UBYTE pen;
        if(!(diamondSprite->mask[sourceAt]&sourceMask)) continue;
        pen=pixel(diamondSprite->bitmap,x,y,FRONT_PLANES);
        diamondMask[(LONG)y*DIAMOND_SOURCE_WORDS]|=bit;
        for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
            diamondBits[((LONG)plane*COLLECTIBLE_H+y)*
                        DIAMOND_SOURCE_WORDS]|=bit;
    }
    return TRUE;
}

#ifdef SPARKPAW_STORMRAIL_PROOF
static BOOL buildHeartPattern(void)
{
    UBYTE plane; WORD x,y;
    heartMask=(UWORD *)AllocMem(COLLECTIBLE_H*DIAMOND_SOURCE_WORDS*2,
                                MEMF_CHIP|MEMF_CLEAR);
    heartBits=(UWORD *)AllocMem(COLLECTIBLE_H*DIAMOND_SOURCE_WORDS*
                                FRONT_PLANES*2,MEMF_CHIP|MEMF_CLEAR);
    if(!heartMask||!heartBits) return FALSE;
    for(y=0;y<COLLECTIBLE_H;y++) for(x=0;x<COLLECTIBLE_W;x++) {
        UBYTE sourceMask=(UBYTE)(0x80>>(x&7));
        LONG sourceAt=(LONG)y*heartSprite->rowBytes+(x>>3);
        UWORD bit=(UWORD)(0x8000U>>x);
        UBYTE pen;
        if(!(heartSprite->mask[sourceAt]&sourceMask)) continue;
        pen=pixel(heartSprite->bitmap,x,y,FRONT_PLANES);
        heartMask[(LONG)y*DIAMOND_SOURCE_WORDS]|=bit;
        for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
            heartBits[((LONG)plane*COLLECTIBLE_H+y)*
                      DIAMOND_SOURCE_WORDS]|=bit;
    }
    return TRUE;
}
#endif

static UWORD *coreMaskFrame(UBYTE frame)
{
    return coreMask+(LONG)frame*CORE_SPRITE_H*CORE_SOURCE_WORDS;
}

static UWORD *coreBitsFrame(UBYTE frame)
{
    return coreBits+(LONG)frame*FRONT_PLANES*CORE_SPRITE_H*CORE_SOURCE_WORDS;
}

static BOOL buildCorePattern(void)
{
    LONG maskWords=(LONG)CORE_SPRITE_FRAMES*CORE_SPRITE_H*CORE_SOURCE_WORDS;
    UBYTE frame,plane; WORD x,y;
    coreMask=(UWORD *)AllocMem(maskWords*2,MEMF_CHIP|MEMF_CLEAR);
    coreBits=(UWORD *)AllocMem(maskWords*FRONT_PLANES*2,
                              MEMF_CHIP|MEMF_CLEAR);
    if(!coreMask||!coreBits) return FALSE;
    for(frame=0;frame<CORE_SPRITE_FRAMES;frame++)
    for(y=0;y<CORE_SPRITE_H;y++) for(x=0;x<CORE_SPRITE_W;x++) {
        LONG sourceY=(LONG)frame*CORE_SPRITE_H+y;
        LONG sourceAt=sourceY*coreSprite->rowBytes+(x>>3);
        UBYTE sourceMask=(UBYTE)(0x80>>(x&7));
        UWORD bit=(UWORD)(0x8000U>>(x&15));
        LONG word=(LONG)y*CORE_SOURCE_WORDS+(x>>4);
        UBYTE pen;
        if(!(coreSprite->mask[sourceAt]&sourceMask)) continue;
        pen=pixel(coreSprite->bitmap,x,(WORD)sourceY,FRONT_PLANES);
        coreMaskFrame(frame)[word]|=bit;
        for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
            coreBitsFrame(frame)[(LONG)plane*CORE_SPRITE_H*
                                 CORE_SOURCE_WORDS+word]|=bit;
    }
    return TRUE;
}

static BOOL buildExtraLifePattern(void)
{
    UBYTE plane; WORD x,y;
    extraLifeMask=(UWORD *)AllocMem(EXTRA_LIFE_H*EXTRA_LIFE_SOURCE_WORDS*2,
                                    MEMF_CHIP|MEMF_CLEAR);
    extraLifeBits=(UWORD *)AllocMem(EXTRA_LIFE_H*EXTRA_LIFE_SOURCE_WORDS*
                                    FRONT_PLANES*2,MEMF_CHIP|MEMF_CLEAR);
    if(!extraLifeMask||!extraLifeBits) return FALSE;
    for(y=0;y<EXTRA_LIFE_H;y++) for(x=0;x<EXTRA_LIFE_W;x++) {
        LONG at=(LONG)y*extraLifeSprite->rowBytes+(x>>3);
        UBYTE sourceMask=(UBYTE)(0x80>>(x&7));
        UWORD bit=(UWORD)(0x8000U>>(x&15));
        LONG word=(LONG)y*EXTRA_LIFE_SOURCE_WORDS+(x>>4);
        UBYTE pen;
        if(!(extraLifeSprite->mask[at]&sourceMask)) continue;
        pen=pixel(extraLifeSprite->bitmap,x,y,FRONT_PLANES);
        extraLifeMask[word]|=bit;
        for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
            extraLifeBits[extraLifePlaneWordOffset(
                          plane,y,EXTRA_LIFE_SOURCE_WORDS)+(x>>4)]|=bit;
    }
    return TRUE;
}

static UWORD *diamondBackgroundPlane(WORD index,UBYTE plane)
{
    return diamondBackground+
        ((LONG)index*FRONT_PLANES+plane)*DIAMOND_PATCH_H;
}

static UWORD *diamondWideBackgroundPlane(UBYTE plane)
{
    return diamondWideBackground+(LONG)plane*DIAMOND_PATCH_H*
           DIAMOND_WIDE_WORDS;
}

static void restoreDiamondBackground(struct BitMap *target,WORD index)
{
    struct Collectible *item=collectibleAt(index);
    LONG at=(LONG)(item->y-2)*target->BytesPerRow+(item->x>>4)*2;
    UWORD words=index==DIAMOND_WIDE_INDEX?DIAMOND_WIDE_WORDS:1;
    UBYTE plane;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        platformWaitBlit();
        hw->bltcon0=0x09f0; hw->bltcon1=0;
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=0;
        hw->bltdmod=(UWORD)(target->BytesPerRow-words*2);
        hw->bltapt=index==DIAMOND_WIDE_INDEX?
            diamondWideBackgroundPlane(plane):
            diamondBackgroundPlane(index,plane);
        hw->bltdpt=target->Planes[plane]+at;
        hw->bltsize=(UWORD)((DIAMOND_PATCH_H<<6)|words);
    }
}

static void drawDiamondToWorld(struct BitMap *target,WORD index,WORD y)
{
    struct Collectible *item=collectibleAt(index);
    blitMaskedBobTarget(target,diamondMask,diamondBits,DIAMOND_SOURCE_WORDS,
                        COLLECTIBLE_W,COLLECTIBLE_H,item->x,y);
}

static BOOL prepareStaticCollectibles(void)
{
    static const BYTE hover[8]={0,-1,-2,-1,0,1,2,1};
    WORD index,row; UBYTE plane;
    LONG words=(LONG)MAX_COLLECTIBLES*FRONT_PLANES*DIAMOND_PATCH_H;
    diamondBackground=(UWORD *)AllocMem(words*2,MEMF_CHIP|MEMF_CLEAR);
    diamondWideBackground=(UWORD *)AllocMem(
        FRONT_PLANES*DIAMOND_PATCH_H*DIAMOND_WIDE_WORDS*2,
        MEMF_CHIP|MEMF_CLEAR);
    if(!diamondBackground||!diamondWideBackground) return FALSE;
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        struct Collectible *item=collectibleAt(index);
        LONG at=(LONG)(item->y-2)*frontClean->bitmap->BytesPerRow+
                (item->x>>4)*2;
        for(plane=0;plane<FRONT_PLANES;plane++)
            for(row=0;row<DIAMOND_PATCH_H;row++)
                CopyMem(frontClean->bitmap->Planes[plane]+at+
                        (LONG)row*frontClean->bitmap->BytesPerRow,
                        diamondBackgroundPlane(index,plane)+row,2);
        if(index==DIAMOND_WIDE_INDEX)
            for(plane=0;plane<FRONT_PLANES;plane++)
                for(row=0;row<DIAMOND_PATCH_H;row++)
                    CopyMem(frontClean->bitmap->Planes[plane]+at+
                            (LONG)row*frontClean->bitmap->BytesPerRow,
                            diamondWideBackgroundPlane(plane)+
                            row*DIAMOND_WIDE_WORDS,
                            DIAMOND_WIDE_WORDS*2);
        item->drawnX=item->x; item->drawnY=(WORD)(item->y+hover[index&7]);
#ifdef SPARKPAW_COLLECTIBLE_CANONICAL_SYNC_REFERENCE
        drawDiamondToWorld(frontClean->bitmap,index,item->drawnY);
#endif
#ifndef SPARKPAW_ROLLING_PROTOTYPE
        drawDiamondToWorld(frontDisplay,index,item->drawnY);
#endif
        item->drawn=TRUE;
    }
    platformWaitBlit(); return TRUE;
}

static UBYTE waterPatternPen(UBYTE frame,WORD x,WORD y)
{
    static const UBYTE surfaceCurve[16]={
        1,1,0,0,0,1,1,2,2,2,1,1,0,0,1,1
    };
    static const UBYTE bubbleX[6]={7,19,31,46,60,72};
    static const UBYTE bubbleStart[6]={0,11,4,15,7,13};
    static const UBYTE bubbleLife[6]={7,9,6,10,8,7};
    UBYTE bubble;
    UBYTE surface=(UBYTE)(1+surfaceCurve[((x>>1)+frame)&15]);
    /* Meet both banks at y=200. The short ramps prevent an exposed crest at
       either edge while leaving the centre free to undulate. */
    if(x<3||x>=77) surface=3;
    /* One continuous cyan surface spans the opening. Its shallow two-pixel
       curve advances sideways instead of breaking into separate crests. */
    if(y<surface) return 0;
    if(y==surface)
        return ((((x>>2)+frame*3)&7)==0||
                (((x+frame*6)&31)==17))?11:6;
    if(y==surface+1) return ((x+frame)&7)?5:6;
    /* Six deterministic bubble tracks use distinct phases and speeds. Their
       active/rest windows keep the highlights from rising in lockstep. */
    for(bubble=0;bubble<6;bubble++) {
        UBYTE age=(UBYTE)((frame+16-bubbleStart[bubble])&15);
        UBYTE life=bubbleLife[bubble];
        if(age<life) {
            WORD bx=(WORD)(bubbleX[bubble]+(((frame+bubble)&3)==0?1:0));
            WORD by=(WORD)(10-((age*7)/life));
            if(by<=surface+1) continue;
            if(x==bx&&y==by) return (bubble&1)?11:6;
            if(!(bubble&1)&&x==bx+1&&y==by) return 6;
        }
    }
    return 5;
}

static BOOL buildWaterPatterns(void)
{
    UBYTE frame,plane; WORD x,y;
    LONG words=WATER_FRAMES*FRONT_PLANES*WATER_H*WATER_WORDS;
    waterBits=(UWORD *)AllocMem(words*2,MEMF_CHIP|MEMF_CLEAR);
    if(!waterBits) return FALSE;
    for(frame=0;frame<WATER_FRAMES;frame++)
        for(y=0;y<WATER_H;y++) for(x=0;x<WATER_W;x++) {
            UBYTE pen=waterPatternPen(frame,x,y);
            LONG row=((LONG)frame*FRONT_PLANES*WATER_H+y)*WATER_WORDS;
            UWORD bit=(UWORD)(0x8000U>>(x&15));
            for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
                waterBits[row+(LONG)plane*WATER_H*WATER_WORDS+(x>>4)]|=bit;
        }
    return TRUE;
}

static UBYTE splashPatternPen(UBYTE frame,WORD x,WORD y)
{
    WORD dx=x-16;
    if(frame==0) {
        if(y>=12&&y<=14&&dx>=-7&&dx<=7) return y==12?11:6;
        if((x==10||x==22)&&y>=8&&y<=11) return 6;
    } else if(frame==1) {
        if(y>=11&&y<=14&&dx>=-12&&dx<=12)
            return (y==11&&(x&2))?11:6;
        if((x==6||x==25)&&y>=5&&y<=8) return 6;
        if((x==12||x==20)&&y>=2&&y<=5) return 11;
    } else if(frame==2) {
        if(y>=12&&y<=14&&dx>=-15&&dx<=15&&((x+y)&1)) return 6;
        if((x==4||x==27)&&y>=8&&y<=10) return 6;
        if((x==9||x==23)&&y>=4&&y<=6) return 11;
        if((x==14||x==18)&&y<=2) return 6;
    } else {
        if(y==13&&((x+2)&7)<3) return 6;
        if(y==11&&(x==5||x==16||x==27)) return 11;
    }
    return 0;
}

static UWORD *splashMaskRow(UBYTE frame,WORD row)
{
    return splashMask+((LONG)frame*SPLASH_H+row)*SPLASH_SOURCE_WORDS;
}

static UWORD *splashBitsRow(UBYTE frame,UBYTE plane,WORD row)
{
    LONG index=((LONG)frame*FRONT_PLANES+plane)*SPLASH_H+row;
    return splashBits+index*SPLASH_SOURCE_WORDS;
}

static BOOL buildSplashPatterns(void)
{
    LONG maskWords=SPLASH_FRAMES*SPLASH_H*SPLASH_SOURCE_WORDS;
    UBYTE frame,plane; WORD x,y;
    splashMask=(UWORD *)AllocMem(maskWords*2,MEMF_CHIP|MEMF_CLEAR);
    splashBits=(UWORD *)AllocMem(maskWords*FRONT_PLANES*2,MEMF_CHIP|MEMF_CLEAR);
    if(!splashMask||!splashBits) return FALSE;
    for(frame=0;frame<SPLASH_FRAMES;frame++)
        for(y=0;y<SPLASH_H;y++) for(x=0;x<SPLASH_W;x++) {
            UBYTE pen=splashPatternPen(frame,x,y);
            UWORD bit=(UWORD)(0x8000U>>(x&15));
            if(!pen) continue;
            splashMaskRow(frame,y)[x>>4]|=bit;
            for(plane=0;plane<FRONT_PLANES;plane++) if(pen&(1<<plane))
                splashBitsRow(frame,plane,y)[x>>4]|=bit;
        }
    return TRUE;
}

static void restoreSplashBob(void)
{
    if(!splashDrawn) return;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticFrame.splashRestore++;
#endif
    blitRestoreRect(
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
                    prototypeSplashWorldX,
#else
                    splashDrawnX,
#endif
                    splashDrawnX,splashDrawnY,SPLASH_W,SPLASH_H);
    splashDrawn=FALSE;
#ifdef SPARKPAW_STORMRAIL_PROOF
    memset(&stormrailHistory,0,sizeof(stormrailHistory));
#endif
}

static void drawSplashBob(void)
{
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) return;
#endif
    UBYTE frame;
    if(!game->waterSplashTimer) return;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticFrame.splashDraw++;
#endif
    frame=(UBYTE)((16-game->waterSplashTimer)>>2);
    if(frame>=SPLASH_FRAMES) frame=SPLASH_FRAMES-1;
    splashDrawnX=(WORD)(game->waterSplashX-(SPLASH_W>>1));
    splashDrawnY=184;
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
    prototypeSplashWorldX=splashDrawnX;
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    if(!prototypeRectFits(splashDrawnX,SPLASH_W)) return;
    splashDrawnX=prototypePhysicalX(splashDrawnX);
#endif
    blitMaskedBob(splashMaskRow(frame,0),splashBitsRow(frame,0,0),
                  SPLASH_SOURCE_WORDS,SPLASH_W,SPLASH_H,
                  splashDrawnX,splashDrawnY);
    splashDrawn=TRUE;
}

static void blitWaterFrame(struct BitMap *target,UBYTE frame,WORD waterX)
{
    UBYTE plane;
    LONG at=(LONG)WATER_Y*target->BytesPerRow+(waterX>>4)*2;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        LONG source=((LONG)frame*FRONT_PLANES+plane)*WATER_H*WATER_WORDS;
        platformWaitBlit();
        hw->bltcon0=0x09f0; hw->bltcon1=0;
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=0;
        hw->bltdmod=(UWORD)(target->BytesPerRow-WATER_WORDS*2);
        hw->bltapt=waterBits+source;
        hw->bltdpt=target->Planes[plane]+at;
        hw->bltsize=(UWORD)((WATER_H<<6)|WATER_WORDS);
    }
}

static void animateWater(void)
{
    UBYTE index;
    UBYTE frame=(UBYTE)((game->frameCounter>>1)&(WATER_FRAMES-1));
    /* Restore passes have finished. Update clean first and display second so
       later Bob draws and next-frame restores see the identical background.
       A resident off-screen strip does not need every animation frame; update
       it immediately when it enters the culling margin instead. This removes
       sixteen unnecessary four-plane blits from the camera-810 Strider scene. */
    for(index=0;index<LEVEL_WATER_COUNT;index++) {
        WORD waterX=levelWaterLeft(index);
        if(!waterUpdateVisible(waterX,game->cameraX,SCREEN_W,WATER_W,16)||
           waterDrawnFrame[index]==frame) continue;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        diagnosticFrame.waterUpdates++;
#endif
        blitWaterFrame(frontClean->bitmap,frame,waterX);
#ifndef SPARKPAW_ROLLING_PROTOTYPE
        blitWaterFrame(frontDisplay,frame,waterX);
#endif
        waterDrawnFrame[index]=frame;
    }
}

static void restoreCollectibleBobs(void)
{
    static const BYTE hover[8]={0,-1,-2,-1,0,1,2,1};
    WORD index;
#ifndef SPARKPAW_COLLECTIBLE_CANONICAL_SYNC_REFERENCE
    struct PrototypeTarget *target=&prototypeTarget[prototypePreparedCopper];
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        struct Collectible *item=collectibleAt(index);
        if(target->collectibleDrawn[index]) {
            WORD sourceX=(WORD)(item->x&~15);
            WORD targetX=(WORD)(target->collectibleX[index]&~15);
            WORD width=index==DIAMOND_WIDE_INDEX?32:16;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
            diagnosticFrame.collectibleRestore++;
#endif
            blitRestoreRect(sourceX,targetX,(WORD)(item->y-2),width,
                            DIAMOND_PATCH_H);
            target->collectibleDrawn[index]=FALSE;
        }
        if(!item->active) { item->drawn=FALSE; continue; }
        {
            WORD desiredY=(WORD)(item->y+
                hover[((game->frameCounter>>2)+index)&7]);
            BOOL visible=item->x+COLLECTIBLE_W>=
                         (WORD)game->cameraX-16&&
                         item->x<=(WORD)game->cameraX+SCREEN_W+16;
            if(visible&&desiredY!=item->drawnY&&
               ((game->frameCounter&3)==(index&3)))
                item->drawnY=desiredY;
            item->drawn=TRUE;
        }
    }
#else
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        struct Collectible *item=collectibleAt(index);
        WORD desiredY;
        BOOL visible;
        if(!item->drawn) continue;
        visible=item->x+COLLECTIBLE_W>=(WORD)game->cameraX-16&&
                item->x<=(WORD)game->cameraX+SCREEN_W+16;
        desiredY=(WORD)(item->y+
            hover[((game->frameCounter>>2)+index)&7]);
        if(item->active&&(!visible||desiredY==item->drawnY||
           ((game->frameCounter&3)!=(index&3)))) continue;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        diagnosticFrame.collectibleRestore++;
#endif
        restoreDiamondBackground(frontClean->bitmap,index);
#ifndef SPARKPAW_ROLLING_PROTOTYPE
        restoreDiamondBackground(frontDisplay,index);
#endif
        item->drawn=FALSE;
    }
#endif
}

static void drawCollectibleBobs(void)
{
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) return;
#endif
    static const BYTE hover[8]={0,-1,-2,-1,0,1,2,1};
    WORD index;
#ifndef SPARKPAW_COLLECTIBLE_CANONICAL_SYNC_REFERENCE
    struct PrototypeTarget *target=&prototypeTarget[prototypePreparedCopper];
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        struct Collectible *item=collectibleAt(index);
        WORD physicalX;
        if(!item->active||
           item->x+COLLECTIBLE_W<(WORD)game->cameraX-16||
           item->x>(WORD)game->cameraX+SCREEN_W+16||
           !prototypeRectFits(item->x,COLLECTIBLE_W)) continue;
        physicalX=prototypePhysicalX(item->x);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        diagnosticFrame.collectibleDraw++;
#endif
        blitMaskedBobTarget(frontDisplay,diamondMask,diamondBits,
                            DIAMOND_SOURCE_WORDS,COLLECTIBLE_W,COLLECTIBLE_H,
                            physicalX,item->drawnY);
        target->collectibleDrawn[index]=TRUE;
        target->collectibleX[index]=physicalX;
        target->collectibleY[index]=item->drawnY;
    }
#else
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        struct Collectible *item=collectibleAt(index);
        if(!item->active||item->drawn||
           item->x+COLLECTIBLE_W<(WORD)game->cameraX-16||
           item->x>(WORD)game->cameraX+SCREEN_W+16) continue;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        diagnosticFrame.collectibleDraw++;
#endif
        item->drawnX=item->x;
        item->drawnY=(WORD)(item->y+
            hover[((game->frameCounter>>2)+index)&7]);
        drawDiamondToWorld(frontClean->bitmap,index,item->drawnY);
#ifndef SPARKPAW_ROLLING_PROTOTYPE
        drawDiamondToWorld(frontDisplay,index,item->drawnY);
#endif
        item->drawn=TRUE;
    }
#endif
}

static UBYTE desiredCoreFrame(void)
{
    static const UBYTE pulse[8]={0,1,2,3,4,5,4,2};
    UBYTE elapsed,frame;
    if(!game->coreCollectTimer) return pulse[(game->frameCounter>>3)&7];
    elapsed=(UBYTE)(50-game->coreCollectTimer);
    if(elapsed<8) return CORE_IDLE_FRAMES;
    frame=(UBYTE)((elapsed-8)>>2);
    if(frame>=CORE_PICKUP_FRAMES-1) frame=CORE_PICKUP_FRAMES-2;
    if(elapsed>=48) frame=CORE_PICKUP_FRAMES-1;
    return (UBYTE)(CORE_IDLE_FRAMES+1+frame);
}

static UBYTE desiredCoreFlash(void)
{
    UBYTE elapsed;
    if(!game->coreCollectTimer) return 0;
    elapsed=(UBYTE)(50-game->coreCollectTimer);
    if(elapsed<2) return 2;
    if(elapsed<4) return 1;
    return 0;
}

static void setCoreWorldFlash(void)
{
    UBYTE index,level=desiredCoreFlash(),listIndex=0;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    listIndex=prototypePreparedCopper;
#endif
    for(index=0;index<16;index++) {
        UWORD base=frontColors[index],value=base;
        if(level&&index) {
            UWORD r=(base>>8)&15,g=(base>>4)&15,b=base&15;
            r=(UWORD)(r+((15-r)*level)/4);
            g=(UWORD)(g+((15-g)*level)/4);
            b=(UWORD)(b+((15-b)*level)/4);
            value=(UWORD)((r<<8)|(g<<4)|b);
        }
        cop[frontColorValue[listIndex][index]]=value;
    }
}

static WORD desiredCoreY(void)
{
    static const BYTE hover[16]={
        -1,-1,-2,-2,-2,-1,-1,0,0,1,1,2,2,1,1,0
    };
    if(game->coreCollectTimer) return 112;
    return (WORD)(112+hover[(game->frameCounter>>2)&15]);
}

static BOOL coreRenderVisible(void)
{
    const WORD x=LEVEL_STORMSTONE_CORE_CENTER_X-CORE_SPRITE_W/2;
    return x+CORE_SPRITE_W>=(WORD)game->cameraX-16&&
           x<=(WORD)game->cameraX+SCREEN_W+16;
}

static void restoreCoreBob(void)
{
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    struct PrototypeTarget *target=&prototypeTarget[prototypePreparedCopper];
    if(!target->coreDrawn) return;
    if(coreRenderVisible()&&target->coreFrame==desiredCoreFrame()&&
       target->coreY==desiredCoreY()) return;
    blitRestoreRect(LEVEL_STORMSTONE_CORE_CENTER_X-CORE_SPRITE_W/2,
                    target->coreX,target->coreY,
                    CORE_SPRITE_W,CORE_SPRITE_H);
    target->coreDrawn=FALSE;
#else
    if(!coreDrawn) return;
    if(coreRenderVisible()&&coreDrawnFrame==desiredCoreFrame()&&
       coreDrawnY==desiredCoreY()) return;
    blitRestoreRect(LEVEL_STORMSTONE_CORE_CENTER_X-CORE_SPRITE_W/2,
                    coreDrawnX,coreDrawnY,CORE_SPRITE_W,CORE_SPRITE_H);
    coreDrawn=FALSE;
#endif
}

static void drawCoreBob(void)
{
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) return;
#endif
    WORD worldX=LEVEL_STORMSTONE_CORE_CENTER_X-CORE_SPRITE_W/2;
    WORD y=desiredCoreY();
    UBYTE frame=desiredCoreFrame();
    if(!coreRenderVisible()) return;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    {
        struct PrototypeTarget *target=&prototypeTarget[prototypePreparedCopper];
        WORD physicalX;
        if(target->coreDrawn) return;
        if(!prototypeRectFits(worldX,CORE_SPRITE_W)) return;
        physicalX=prototypePhysicalX(worldX);
        blitMaskedBob(coreMaskFrame(frame),coreBitsFrame(frame),
                      CORE_SOURCE_WORDS,CORE_SPRITE_W,CORE_SPRITE_H,
                      physicalX,y);
        target->coreDrawn=TRUE; target->coreX=physicalX;
        target->coreY=y; target->coreFrame=frame;
    }
#else
    if(coreDrawn) return;
    blitMaskedBob(coreMaskFrame(frame),coreBitsFrame(frame),CORE_SOURCE_WORDS,
                  CORE_SPRITE_W,CORE_SPRITE_H,worldX,y);
    coreDrawn=TRUE; coreDrawnX=worldX; coreDrawnY=y; coreDrawnFrame=frame;
#endif
}

static BOOL extraLifeRenderVisible(void)
{
    return game->extraLifeState==EXTRA_LIFE_DROPPING||
           game->extraLifeState==EXTRA_LIFE_READY;
}

static void restoreExtraLifeBob(void)
{
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    struct PrototypeTarget *target=&prototypeTarget[prototypePreparedCopper];
    if(!target->extraLifeDrawn) return;
    blitRestoreRect(EXTRA_LIFE_X,target->extraLifeX,target->extraLifeY,
                    EXTRA_LIFE_W,EXTRA_LIFE_H);
    target->extraLifeDrawn=FALSE;
#else
    if(!extraLifeDrawn) return;
    blitRestoreRect(EXTRA_LIFE_X,extraLifeDrawnX,extraLifeDrawnY,
                    EXTRA_LIFE_W,EXTRA_LIFE_H);
    extraLifeDrawn=FALSE;
#endif
}

static void drawExtraLifeBob(void)
{
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) return;
#endif
    if(!extraLifeRenderVisible()) return;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    {
        struct PrototypeTarget *target=&prototypeTarget[prototypePreparedCopper];
        WORD physicalX;
        if(target->extraLifeDrawn||!prototypeRectFits(EXTRA_LIFE_X,EXTRA_LIFE_W))
            return;
        physicalX=prototypePhysicalX(EXTRA_LIFE_X);
        blitMaskedBobTarget(frontDisplay,extraLifeMask,extraLifeBits,
                            EXTRA_LIFE_SOURCE_WORDS,EXTRA_LIFE_W,EXTRA_LIFE_H,
                            physicalX,game->extraLifeY);
        target->extraLifeDrawn=TRUE; target->extraLifeX=physicalX;
        target->extraLifeY=game->extraLifeY;
    }
#else
    if(extraLifeDrawn) return;
    blitMaskedBob(extraLifeMask,extraLifeBits,EXTRA_LIFE_SOURCE_WORDS,
                  EXTRA_LIFE_W,EXTRA_LIFE_H,EXTRA_LIFE_X,game->extraLifeY);
    extraLifeDrawn=TRUE; extraLifeDrawnX=EXTRA_LIFE_X;
    extraLifeDrawnY=game->extraLifeY;
#endif
}

static void eraseProjectileBobs(void)
{
    WORD i;
    for(i=0;i<MAX_PROJECTILES;i++) {
        struct Projectile *projectile=projectileAt(i);
        if(!projectile->drawn) continue;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        diagnosticFrame.projectileRestore++;
#endif
        blitRestoreRect(
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
                        prototypeProjectileWorldX[i],
#else
                        projectile->drawnX,
#endif
                        projectile->drawnX,projectile->drawnY,
                        PROJECTILE_W,PROJECTILE_H);
        projectile->drawn=FALSE;
    }
}

static void drawProjectileBobs(void)
{
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) return;
#endif
    WORD i;
    for(i=0;i<MAX_PROJECTILES;i++) {
        struct Projectile *p=projectileAt(i); UBYTE pattern,left;
        WORD worldX;
        if(!p->active) continue;
        worldX=(WORD)(p->x>>8); p->drawnY=(WORD)(p->y>>8);
        if(worldX<0||worldX+PROJECTILE_W>WORLD_W||
           p->drawnY<0||p->drawnY+PROJECTILE_H>WORLD_H) continue;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
        if(!prototypeRectFits(worldX,PROJECTILE_W)) continue;
#endif
        if(!projectileRenderVisible(worldX,PROJECTILE_W,
                                    (WORD)game->cameraX,SCREEN_W,16)) continue;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        diagnosticFrame.projectileDraw++;
#endif
        left=p->vx<0;
        pattern=p->impactTimer?(p->impactTimer>=4?2:(p->impactTimer>=2?3:4)):
                                (UBYTE)((game->frameCounter>>1)&1);
        if(p->hostile) pattern+=PLAYER_PLASMA_PATTERNS;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
        prototypeProjectileWorldX[i]=worldX;
#endif
        p->drawnX=prototypePhysicalX(worldX);
#else
        p->drawnX=worldX;
#endif
        blitMaskedBob(plasmaMaskRow(pattern,left,0),
                      plasmaBitsRow(pattern,left,0,0),PLASMA_SOURCE_WORDS,
                      PROJECTILE_W,PROJECTILE_H,p->drawnX,p->drawnY);
        p->drawn=TRUE;
    }
}

static void restoreEnemyBob(void)
{
    WORD i,y[MAX_ENEMIES]; UBYTE order[MAX_ENEMIES];
    for(i=0;i<MAX_ENEMIES;i++) {
        struct Enemy *enemy=enemyAt(i);
        y[i]=enemy->drawn?enemy->drawnY:32767;
    }
    enemyVerticalOrder(y,order,MAX_ENEMIES);
    for(i=0;i<MAX_ENEMIES;i++) {
        struct Enemy *enemy=enemyAt(order[i]);
        struct EnemyBobCache *cache;
        WORD j,unionX,unionY,unionW,unionH;
        if(!enemy->drawn) continue;
        cache=&enemyCaches[enemy->drawnType<ENEMY_TYPE_COUNT?
                           enemy->drawnType:ENEMY_TYPE_CLOCKWORK_BEETLE];
        if(enemy->drawnType==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER)
            for(j=(WORD)(i+1);j<MAX_ENEMIES;j++) {
                struct Enemy *other=enemyAt(order[j]);
                if(!other->drawn||
                   other->drawnType!=ENEMY_TYPE_CLOCKWORK_STORM_STRIDER||
                   !striderRestoreUnion(enemy->drawnX,enemy->drawnY,
                       other->drawnX,other->drawnY,cache->width,cache->height,
                       &unionX,&unionY,&unionW,&unionH)) continue;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
                diagnosticFrame.striderRestore+=2;
#endif
                blitRestoreRect(
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
                    prototypeEnemyWorldX[order[i]]<
                    prototypeEnemyWorldX[order[j]]?
                    prototypeEnemyWorldX[order[i]]:
                    prototypeEnemyWorldX[order[j]],
#else
                    unionX,
#endif
                    unionX,unionY,unionW,unionH);
                enemy->drawn=FALSE; other->drawn=FALSE;
                break;
            }
        if(!enemy->drawn) continue;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        if(enemy->drawnType==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER)
            diagnosticFrame.striderRestore++;
        else diagnosticFrame.beetleRestore++;
#endif
        blitRestoreRect(
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
                        prototypeEnemyWorldX[order[i]],
#else
                        enemy->drawnX,
#endif
                        enemy->drawnX,enemy->drawnY,
                        cache->width,cache->height);
        enemy->drawn=FALSE;
    }
}

static void drawEnemyBob(void)
{
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) return;
#endif
    WORD i,y[MAX_ENEMIES]; UBYTE order[MAX_ENEMIES];
    for(i=0;i<MAX_ENEMIES;i++) {
        struct Enemy *enemy=enemyAt(i);
        y[i]=enemy->active?(WORD)(enemy->y+
            (enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER?2:0)):32767;
    }
    enemyVerticalOrder(y,order,MAX_ENEMIES);
    /* Complete upper Bobs first after wrap, before their earlier scanlines.
       Equal-Y enemies retain slot order, and the surrounding collectible /
       enemy / projectile family priority is unchanged. */
    for(i=0;i<MAX_ENEMIES;i++) {
        UBYTE slot=order[i];
        struct Enemy *enemy=enemyAt(slot); UBYTE facing;
        struct EnemyBobCache *cache;
        UWORD *mask,*bits;
        if(!enemy->active||enemy->type>=ENEMY_TYPE_COUNT) continue;
        cache=&enemyCaches[enemy->type];
        {
        WORD worldX=(WORD)(enemy->x>>8);
        /* The accepted rb18 Strider has transparent source rows 62-63. Its
           logical 64px collision cell remains grounded at enemy->y+64, while
           the Bob needs a two-pixel visual offset for row 61 to meet the last
           free row above that surface. Beetle geometry remains unchanged. */
        enemy->drawnY=(WORD)(enemy->y+
            (enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER?2:0));
          if(worldX+cache->width<(WORD)game->cameraX-32||
              worldX>(WORD)game->cameraX+SCREEN_W+32||
           worldX<0||worldX+cache->width>WORLD_W||
           enemy->drawnY<0||enemy->drawnY+cache->height>WORLD_H||
           enemy->animFrame>=cache->frames) continue;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
        if(!prototypeRectFits(worldX,cache->width)) continue;
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        if(enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER)
            diagnosticFrame.striderDraw++;
        else diagnosticFrame.beetleDraw++;
#endif
        /* Caches normalize each source sheet to logical left/right columns. */
        facing=enemy->facingLeft?0:1;
        if(enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER) {
            if(!stageStriderFrame(slot,facing,enemy->animFrame,&mask,&bits))
                continue;
        } else {
            mask=enemyMaskRow(cache,facing,enemy->animFrame,0);
            bits=enemyBitsRow(cache,facing,enemy->animFrame,0,0);
        }
#ifdef SPARKPAW_ROLLING_PROTOTYPE
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
        prototypeEnemyWorldX[slot]=worldX;
#endif
        enemy->drawnX=prototypePhysicalX(worldX);
#else
        enemy->drawnX=worldX;
#endif
        blitMaskedBob(mask,bits,
                      cache->sourceWords,cache->width,cache->height,
                      enemy->drawnX,enemy->drawnY);
        enemy->drawnType=enemy->type;
        enemy->drawn=TRUE;
        }
    }
}

BOOL rendererLoadGameplay(void)
{
    if(!assetsLoadGameplay()) return FALSE;
    frontClean=assetsFrontClean(); rearWorld=assetsRearWorld();
    sprites=assetsPlayerSprites(); coreSprite=assetsStormstoneCore();
    extraLifeSprite=assetsExtraLife();
#ifdef SPARKPAW_STORMRAIL_PROOF
    stormrailSprite=assetsStormrailFamily();
    stormrailObstacleSprite=assetsStormrailObstacles();
    stormrailFlightRear=assetsStormrailFlightRear();
#endif
    configureEnemyCaches();
    hudBase=assetsHudBase();
    diamondSprite=assetsCollectibleDiamond();
#ifdef SPARKPAW_STORMRAIL_PROOF
    heartSprite=assetsStormrailHeart();
#endif
    return TRUE;
}

#ifdef SPARKPAW_AGA32_FETCH_CANDIDATE
static BOOL aga32DisplayLayoutValid(void)
{
    UBYTE plane;
    const struct BitMap *hud=hudDisplayBitmap();
    if(!hud||hudBytesPerRow()<HUD_FETCH_BYTES||(hudBytesPerRow()&3)||
       ((ULONG)hudBlankPlane()&3)||
       (frontDisplay->BytesPerRow&3)) return FALSE;
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
    if(!rearDisplay||(rearDisplay->BytesPerRow&3)
#ifdef SPARKPAW_STORMRAIL_PROOF
       ||(game->stormrailActive&&
          (!stormFlightRearDisplay||(stormFlightRearDisplay->BytesPerRow&3)))
#endif
       ) return FALSE;
#else
    if(rearWorld->bitmap->BytesPerRow&3) return FALSE;
#endif
    for(plane=0;plane<FRONT_PLANES;plane++)
        if((ULONG)frontDisplay->Planes[plane]&3) return FALSE;
    for(plane=0;plane<REAR_PLANES;plane++)
        if(((ULONG)
#ifdef SPARKPAW_AGA32_LEFT_GUARD
             rearDisplay->Planes[plane]
#else
             rearWorld->bitmap->Planes[plane]
#endif
             &3)||
           ((ULONG)hud->Planes[plane]&3)) return FALSE;
#ifdef SPARKPAW_AGA64_FETCH_CANDIDATE
    if((frontDisplay->BytesPerRow&7)||(rearWorld->bitmap->BytesPerRow&7))
        return FALSE;
    for(plane=0;plane<FRONT_PLANES;plane++)
        if((ULONG)frontDisplay->Planes[plane]&7) return FALSE;
    for(plane=0;plane<REAR_PLANES;plane++)
        if((ULONG)rearWorld->bitmap->Planes[plane]&7) return FALSE;
#endif
    return TRUE;
}
#endif

#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
static BOOL prepareRearGuardedDisplay(void)
{
    UBYTE plane;
    UWORD row;
    const struct BitMap *source=rearWorld->bitmap;
    /* Base the guarded allocation on the source's physical stride, not its
       logical width. graphics.library may pad a DISPLAYABLE source row; using
       width+guard can then allocate the destination in the same stride class
       and leave no actual leading guard bytes. */
    rearDisplay=AllocBitMap(
        (UWORD)((source->BytesPerRow+PLAYFIELD_GUARD_BYTES)*8),
                           rearWorld->height,
                           REAR_PLANES,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    if(!rearDisplay||rearDisplay->BytesPerRow<
       source->BytesPerRow+PLAYFIELD_GUARD_BYTES)
        return FALSE;
    for(plane=0;plane<REAR_PLANES;plane++)
        for(row=0;row<rearWorld->height;row++)
            CopyMem(source->Planes[plane]+(LONG)row*source->BytesPerRow,
                    rearDisplay->Planes[plane]+(LONG)row*rearDisplay->BytesPerRow+
                    PLAYFIELD_GUARD_BYTES,source->BytesPerRow);
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) {
    source=stormrailFlightRear->bitmap;
    stormFlightRearDisplay=AllocBitMap(
        (UWORD)((source->BytesPerRow+PLAYFIELD_GUARD_BYTES)*8),
        stormrailFlightRear->height,REAR_PLANES,
        BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    if(!stormFlightRearDisplay||stormFlightRearDisplay->BytesPerRow<
       source->BytesPerRow+PLAYFIELD_GUARD_BYTES)
        return FALSE;
    for(plane=0;plane<REAR_PLANES;plane++)
        for(row=0;row<stormrailFlightRear->height;row++)
            CopyMem(source->Planes[plane]+(LONG)row*source->BytesPerRow,
                    stormFlightRearDisplay->Planes[plane]+
                    (LONG)row*stormFlightRearDisplay->BytesPerRow+
                    PLAYFIELD_GUARD_BYTES,source->BytesPerRow);
    }
#endif
    return TRUE;
}
#endif

BOOL rendererPrepareGameplay(void)
{
    UBYTE p;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    ULONG diagnosticLoadStart=GfxBase->VBCounter;
#define DIAG_LOAD(index,expression) do { \
    ULONG before=GfxBase->VBCounter; \
    if(!(expression)) return FALSE; \
    diagnosticLoadingFrames[index]=GfxBase->VBCounter-before; \
} while(0)
#endif
    game=gameState();
#ifdef SPARKPAW_STORMRAIL_PROOF
    memset(stormFlightTargetClean,0,sizeof(stormFlightTargetClean));
    stormApproachReleased=FALSE;
    if(game->stormrailActive) {
        stormFlightBlank=AllocBitMap(128,WORLD_H,FRONT_PLANES,
                                      BMF_CLEAR|BMF_DISPLAYABLE,NULL);
        if(!stormFlightBlank) return FALSE;
    }
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    /* A resident Level-1 replay allocates fresh bitmaps but reuses this
       module's static storage. Never let Bob restore history from the
       completed Core scene describe those new, blank targets. */
    memset(prototypeTarget,0,sizeof(prototypeTarget));
    memset(prototypeCopper,0,sizeof(prototypeCopper));
    prototypeActiveCopper=0;
    prototypePreparedCopper=0;
    prototypeCopperReady=FALSE;
    prototypeOwnershipViolations=0;
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
    memset(prototypeEnemyWorldX,0,sizeof(prototypeEnemyWorldX));
    memset(prototypeProjectileWorldX,0,sizeof(prototypeProjectileWorldX));
    prototypeSplashWorldX=0;
#endif
#endif
    splashDrawn=FALSE;
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    writeStartupStage("renderer_prepare_entry");
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
    prototypeTarget[0].clean=AllocBitMap(PROTOTYPE_TARGET_W,WORLD_H,
        FRONT_PLANES,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
#endif
    prototypeTarget[0].display=AllocBitMap(PROTOTYPE_TARGET_W,WORLD_H,
        FRONT_PLANES,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
    prototypeTarget[1].clean=AllocBitMap(PROTOTYPE_TARGET_W,WORLD_H,
        FRONT_PLANES,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
#endif
    prototypeTarget[1].display=AllocBitMap(PROTOTYPE_TARGET_W,WORLD_H,
        FRONT_PLANES,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    prototypeTarget[0].origin=prototypeTarget[1].origin=0;
    prototypeBuildOrigin=prototypeDesiredOrigin=0;
    frontDisplay=prototypeTarget[0].display;
#else
    frontDisplay=AllocBitMap(WORLD_W,WORLD_H,FRONT_PLANES,
                             BMF_CLEAR|BMF_DISPLAYABLE,NULL);
#endif
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    writeStartupStage("after_target_alloc");
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticLoadingFrames[DIAG_LOAD_TARGET_ALLOC]=
        GfxBase->VBCounter-diagnosticLoadStart;
    diagnosticLoadStart=GfxBase->VBCounter;
#endif
    cop=(UWORD *)AllocMem(COP_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    prototypeCopper[0]=cop;
    prototypeCopper[1]=(UWORD *)AllocMem(COP_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
#endif
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    writeStartupStage("after_copper_alloc");
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticLoadingFrames[DIAG_LOAD_COPPER_ALLOC]=
        GfxBase->VBCounter-diagnosticLoadStart;
    if(!frontDisplay||!cop) return FALSE;
    DIAG_LOAD(DIAG_LOAD_HUD,hudPrepare());
    DIAG_LOAD(DIAG_LOAD_PLAYER_SPRITES,buildHardwareSprites());
#else
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    if(!frontDisplay||!cop) {
        writeStartupStage("failed_target_or_copper_alloc"); return FALSE;
    }
    STARTUP_REQUIRE("hud_prepare",hudPrepare());
    STARTUP_REQUIRE("player_sprite_prepare",buildHardwareSprites());
#else
    if(!frontDisplay||!cop||!hudPrepare()||!buildHardwareSprites())
        return FALSE;
#endif
#endif
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    DIAG_LOAD(DIAG_LOAD_REAR_GUARD,prepareRearGuardedDisplay());
#else
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    STARTUP_REQUIRE("rear_guard_prepare",prepareRearGuardedDisplay());
#else
    if(!prepareRearGuardedDisplay()) return FALSE;
#endif
#endif
#endif
#ifdef SPARKPAW_AGA32_FETCH_CANDIDATE
    /* Never hand an invalid wide-fetch layout to Alice. Graphics.library may
       pad displayable rows, so validate its actual pointers and stride. */
    if(!aga32DisplayLayoutValid()) {
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
        writeStartupStage("failed_aga32_layout_validation");
#endif
        return FALSE;
    }
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    writeStartupStage("after_aga32_layout_validation");
#endif
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    if(!prototypeCopper[1]||!prototypeTarget[1].display
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
       ||!prototypeTarget[0].clean||!prototypeTarget[1].clean
#endif
       ) {
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
        writeStartupStage("failed_rolling_runtime_validation");
#endif
        return FALSE;
    }
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticLoadStart=GfxBase->VBCounter;
    diagnosticTrace=(struct RenderDiagnosticTrace *)AllocMem(
        sizeof(*diagnosticTrace)*DIAGNOSTIC_TRACE_FRAMES,MEMF_FAST|MEMF_CLEAR);
    if(!diagnosticTrace) return FALSE;
    diagnosticLoadingFrames[DIAG_LOAD_TRACE_ALLOC]=
        GfxBase->VBCounter-diagnosticLoadStart;
    diagnosticLoadingFrames[DIAG_LOAD_SETUP]=
        diagnosticLoadingFrames[DIAG_LOAD_TARGET_ALLOC]+
        diagnosticLoadingFrames[DIAG_LOAD_COPPER_ALLOC]+
        diagnosticLoadingFrames[DIAG_LOAD_HUD]+
        diagnosticLoadingFrames[DIAG_LOAD_PLAYER_SPRITES]+
        diagnosticLoadingFrames[DIAG_LOAD_REAR_GUARD]+
        diagnosticLoadingFrames[DIAG_LOAD_TRACE_ALLOC];
#endif
#ifndef SPARKPAW_ROLLING_PROTOTYPE
    for(p=0;p<FRONT_PLANES;p++)
        CopyMem(frontClean->bitmap->Planes[p],frontDisplay->Planes[p],
                (LONG)frontDisplay->BytesPerRow*WORLD_H);
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    DIAG_LOAD(DIAG_LOAD_BEETLE,
        buildEnemyPatterns(&enemyCaches[ENEMY_TYPE_CLOCKWORK_BEETLE],FALSE));
    DIAG_LOAD(DIAG_LOAD_STRIDER,
        buildEnemyPatterns(&enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER],TRUE));
    DIAG_LOAD(DIAG_LOAD_STRIDER_STAGE,prepareStriderStages());
    DIAG_LOAD(DIAG_LOAD_PLASMA,buildPlasmaPatterns());
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive&&!buildStormrailPatterns()) return FALSE;
#endif
    DIAG_LOAD(DIAG_LOAD_DIAMOND,buildDiamondPattern());
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive&&!buildHeartPattern()) return FALSE;
#endif
    DIAG_LOAD(DIAG_LOAD_DIAMOND,buildCorePattern());
    DIAG_LOAD(DIAG_LOAD_DIAMOND,buildExtraLifePattern());
    DIAG_LOAD(DIAG_LOAD_STATIC_COLLECTIBLES,prepareStaticCollectibles());
    DIAG_LOAD(DIAG_LOAD_WATER,buildWaterPatterns());
    DIAG_LOAD(DIAG_LOAD_SPLASH,buildSplashPatterns());
#else
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    STARTUP_REQUIRE("beetle_patterns",
        buildEnemyPatterns(&enemyCaches[ENEMY_TYPE_CLOCKWORK_BEETLE],FALSE));
    STARTUP_REQUIRE("strider_patterns",
        buildEnemyPatterns(&enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER],TRUE));
    STARTUP_REQUIRE("strider_stages",prepareStriderStages());
    STARTUP_REQUIRE("plasma_patterns",buildPlasmaPatterns());
    STARTUP_REQUIRE("diamond_pattern",buildDiamondPattern());
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive)
        STARTUP_REQUIRE("heart_pattern",buildHeartPattern());
#endif
    STARTUP_REQUIRE("core_pattern",buildCorePattern());
    STARTUP_REQUIRE("extra_life_pattern",buildExtraLifePattern());
    STARTUP_REQUIRE("static_collectibles",prepareStaticCollectibles());
    STARTUP_REQUIRE("water_patterns",buildWaterPatterns());
    STARTUP_REQUIRE("splash_patterns",buildSplashPatterns());
#else
    if(!buildEnemyPatterns(&enemyCaches[ENEMY_TYPE_CLOCKWORK_BEETLE],FALSE)||
       !buildEnemyPatterns(&enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER],TRUE)||
       !prepareStriderStages()||
       !buildPlasmaPatterns()||
#ifdef SPARKPAW_STORMRAIL_PROOF
       (game->stormrailActive&&!buildStormrailPatterns())||
#endif
       !buildDiamondPattern()||
#ifdef SPARKPAW_STORMRAIL_PROOF
       (game->stormrailActive&&!buildHeartPattern())||
#endif
       !buildCorePattern()||
       !buildExtraLifePattern()||
       !prepareStaticCollectibles()||!buildWaterPatterns()||
       !buildSplashPatterns())
        return FALSE;
#endif
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticLoadStart=GfxBase->VBCounter;
#endif
    prototypeCopyInitial(&prototypeTarget[0]);
    prototypeCopyInitial(&prototypeTarget[1]);
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    writeStartupStage("after_initial_ring_copy");
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticLoadingFrames[DIAG_LOAD_RING_TARGETS]=
        GfxBase->VBCounter-diagnosticLoadStart;
#endif
#endif
    for(p=0;p<LEVEL_WATER_COUNT;p++) waterDrawnFrame[p]=255;
#if defined(PHASE6_MEMORY_TEST)||defined(SPARKPAW_RENDER_DIAGNOSTIC)
    phase6PeakChipFree=AvailMem(MEMF_CHIP);
    phase6PeakChipLargest=AvailMem(MEMF_CHIP|MEMF_LARGEST);
    phase6PeakFastFree=AvailMem(MEMF_FAST);
    phase6PeakFastLargest=AvailMem(MEMF_FAST|MEMF_LARGEST);
#endif
    /* buildCopper still consumes the player palette from the conversion
       source; finish that final read before releasing the source sheets. */
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticLoadStart=GfxBase->VBCounter;
#endif
    buildCopper();
#ifdef SPARKPAW_FMODE0_EARLY_WORD_GUARD
    if(!earlyWordCopperLayoutValid()) return FALSE;
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    cop=prototypeCopper[1]; frontDisplay=prototypeTarget[1].display;
    buildCopper();
#ifdef SPARKPAW_FMODE0_EARLY_WORD_GUARD
    if(!earlyWordCopperLayoutValid()) return FALSE;
#endif
    cop=prototypeCopper[0]; frontDisplay=prototypeTarget[0].display;
    prototypeActiveCopper=0; prototypePreparedCopper=1;
    prototypeCopperReady=FALSE;
#endif
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    writeStartupStage("after_copper_build");
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticLoadingFrames[DIAG_LOAD_COPPER]=
        GfxBase->VBCounter-diagnosticLoadStart;
#endif
    assetsUnloadGameplayConversionSources();
    sprites=NULL; diamondSprite=NULL; heartSprite=NULL;
    coreSprite=NULL; extraLifeSprite=NULL;
#ifdef SPARKPAW_STORMRAIL_PROOF
    stormrailSprite=NULL;
    stormrailObstacleSprite=NULL;
#endif
    enemyCaches[ENEMY_TYPE_CLOCKWORK_BEETLE].source=NULL;
    enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER].source=NULL;
    setScroll(0,0);
#ifdef SPARKPAW_STARTUP_DIAGNOSTIC
    writeStartupStage("renderer_prepare_complete");
#undef STARTUP_REQUIRE
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#undef DIAG_LOAD
#endif
    return TRUE;
}

void rendererResetGameplay(void)
{
    UBYTE p;
    game=gameState();
    platformWaitBlit();
    splashDrawn=FALSE;
#ifdef SPARKPAW_STORMRAIL_PROOF
    memset(stormFlightTargetClean,0,sizeof(stormFlightTargetClean));
    if(game->stormrailActive) resetStormrailApproachCopperPalettes();
#endif
#ifndef SPARKPAW_ROLLING_PROTOTYPE
    coreDrawn=FALSE;
    extraLifeDrawn=FALSE;
#endif
    for(p=0;p<LEVEL_WATER_COUNT;p++) waterDrawnFrame[p]=255;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    {
        UBYTE index;
        for(index=0;index<2;index++) {
            struct BitMap *display=prototypeTarget[index].display;
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
            struct BitMap *clean=prototypeTarget[index].clean;
#endif
            memset(&prototypeTarget[index],0,sizeof(prototypeTarget[index]));
            prototypeTarget[index].display=display;
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
            prototypeTarget[index].clean=clean;
#endif
            prototypeTarget[index].origin=0;
            prototypeCopyInitial(&prototypeTarget[index]);
        }
    }
    prototypeActiveCopper=0;
    prototypePreparedCopper=1;
    prototypeCopperReady=FALSE;
    prototypeBuildOrigin=prototypeDesiredOrigin=0;
#ifdef SPARKPAW_CANONICAL_BOB_RESTORE
    memset(prototypeEnemyWorldX,0,sizeof(prototypeEnemyWorldX));
    memset(prototypeProjectileWorldX,0,sizeof(prototypeProjectileWorldX));
    prototypeSplashWorldX=0;
#endif
    cop=prototypeCopper[0];
    frontDisplay=prototypeTarget[0].display;
#else
    for(p=0;p<FRONT_PLANES;p++)
        CopyMem(frontClean->bitmap->Planes[p],frontDisplay->Planes[p],
                (LONG)frontDisplay->BytesPerRow*WORLD_H);
#endif
    hwSpriteStageIndex=0;
    SPRITE_STAGE_CACHE_COMMIT(&hwSpriteStageCache[0],0,0);
    SPRITE_STAGE_CACHE_COMMIT(&hwSpriteStageCache[1],0,0);
    setScroll(0,0);
}

void rendererCleanup(void)
{
    WORD facing,frame,channel,type;
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(stormFlightBlank) {
        FreeBitMap(stormFlightBlank); stormFlightBlank=NULL;
    }
#ifndef SPARKPAW_CAMPAIGN_PLAY
    for(frame=0;frame<STORM_PROOF_FRAMES;frame++) {
        if(stormProofPlanes[frame]) FreeMem(stormProofPlanes[frame],
            (LONG)stormProofRowBytes[frame]*WORLD_H*FRONT_PLANES);
        /* Campaign cleanup runs between Level 1 and Stormrail and again on
           BACK TO TITLE. Retire optional proof captures completely so that
           the second cleanup cannot free a stale Level-1 address. */
        stormProofPlanes[frame]=NULL;
        stormProofRowBytes[frame]=0;
    }
#endif
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    if(prototypeCopper[0]) FreeMem(prototypeCopper[0],COP_WORDS*2);
    if(prototypeCopper[1]) FreeMem(prototypeCopper[1],COP_WORDS*2);
#else
    if(cop) FreeMem(cop,COP_WORDS*2);
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    if(diagnosticTrace) FreeMem(diagnosticTrace,
        sizeof(*diagnosticTrace)*DIAGNOSTIC_TRACE_FRAMES);
#endif
    for(facing=0;facing<2;facing++) for(frame=0;frame<ANIM_FRAMES;frame++)
        for(channel=0;channel<SPRITE_CHANNELS;channel++)
            if(hwSprites[facing][frame][channel])
                FreeMem(hwSprites[facing][frame][channel],SPRITE_WORDS*2);
    for(facing=0;facing<2;facing++) for(channel=0;channel<SPRITE_CHANNELS;channel++)
#ifdef SPARKPAW_AGA64_PLAYER_SPRITE
        if(hwSpriteStageAllocation[facing][channel])
            FreeMem(hwSpriteStageAllocation[facing][channel],SPRITE_ALLOC_BYTES);
    if(nullSpriteAllocation) FreeMem(nullSpriteAllocation,16+7);
#else
        if(hwSpriteStage[facing][channel])
            FreeMem(hwSpriteStage[facing][channel],SPRITE_WORDS*2);
    if(nullSprite) FreeMem(nullSprite,4);
#endif
    hudRelease();
    if(plasmaBits) FreeMem(plasmaBits,PLASMA_PATTERNS*2L*PROJECTILE_H*
                           PLASMA_SOURCE_WORDS*FRONT_PLANES*2);
    if(plasmaMask) FreeMem(plasmaMask,PLASMA_PATTERNS*2L*PROJECTILE_H*
                           PLASMA_SOURCE_WORDS*2);
    for(type=0;type<ENEMY_TYPE_COUNT;type++) {
        struct EnemyBobCache *cache=&enemyCaches[type];
        LONG maskWords=2L*cache->frames*cache->height*cache->sourceWords;
        if(cache->bits) FreeMem(cache->bits,maskWords*FRONT_PLANES*2);
        if(cache->mask) FreeMem(cache->mask,maskWords*2);
    }
    {
        const struct EnemyBobCache *cache=
            &enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER];
        LONG frameWords=(LONG)cache->height*cache->sourceWords;
        if(striderStageBits) FreeMem(striderStageBits,
            MAX_ENEMIES*frameWords*FRONT_PLANES*2);
        if(striderStageMask) FreeMem(striderStageMask,
            MAX_ENEMIES*frameWords*2);
    }
    if(diamondBits) FreeMem(diamondBits,COLLECTIBLE_H*DIAMOND_SOURCE_WORDS*
                            FRONT_PLANES*2);
    if(diamondMask) FreeMem(diamondMask,COLLECTIBLE_H*
                            DIAMOND_SOURCE_WORDS*2);
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(heartBits) FreeMem(heartBits,COLLECTIBLE_H*DIAMOND_SOURCE_WORDS*
                          FRONT_PLANES*2);
    if(heartMask) FreeMem(heartMask,COLLECTIBLE_H*
                          DIAMOND_SOURCE_WORDS*2);
#endif
    if(coreBits) FreeMem(coreBits,(LONG)CORE_SPRITE_FRAMES*FRONT_PLANES*
                        CORE_SPRITE_H*CORE_SOURCE_WORDS*2);
    if(coreMask) FreeMem(coreMask,(LONG)CORE_SPRITE_FRAMES*CORE_SPRITE_H*
                        CORE_SOURCE_WORDS*2);
    if(extraLifeBits) FreeMem(extraLifeBits,EXTRA_LIFE_H*
        EXTRA_LIFE_SOURCE_WORDS*FRONT_PLANES*2);
    if(extraLifeMask) FreeMem(extraLifeMask,EXTRA_LIFE_H*
        EXTRA_LIFE_SOURCE_WORDS*2);
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(stormVehicleMask) FreeMem(stormVehicleMask,
        (LONG)STORM_VEHICLE_FRAMES*STORM_VEHICLE_H*
        STORM_VEHICLE_WORDS*2);
    if(stormVehicleBits) FreeMem(stormVehicleBits,
        (LONG)STORM_VEHICLE_FRAMES*FRONT_PLANES*STORM_VEHICLE_H*
        STORM_VEHICLE_WORDS*2);
    if(stormFlightMask) FreeMem(stormFlightMask,
        STORMRAIL_LAUNCH_BOB_H*STORM_VEHICLE_WORDS*2);
    if(stormFlightBits) FreeMem(stormFlightBits,
        (LONG)FRONT_PLANES*STORMRAIL_LAUNCH_BOB_H*STORM_VEHICLE_WORDS*2);
    if(stormCompactMask) FreeMem(stormCompactMask,
        STORMRAIL_FLIGHT_BOB_H*STORM_COMPACT_WORDS*2);
    if(stormCompactBits) FreeMem(stormCompactBits,
        (LONG)FRONT_PLANES*STORMRAIL_FLIGHT_BOB_H*STORM_COMPACT_WORDS*2);
    if(stormDroneMask) FreeMem(stormDroneMask,
        3L*STORM_DRONE_H*STORM_DRONE_WORDS*2);
    if(stormDroneBits) FreeMem(stormDroneBits,
        3L*FRONT_PLANES*STORM_DRONE_H*STORM_DRONE_WORDS*2);
    if(stormRockBigMask) FreeMem(stormRockBigMask,
        (LONG)STORM_ROCK_BIG_FRAMES*STORMRAIL_OBSTACLE_BIG_H*
        STORM_ROCK_BIG_WORDS*2);
    if(stormRockBigBits) FreeMem(stormRockBigBits,
        (LONG)STORM_ROCK_BIG_FRAMES*FRONT_PLANES*STORMRAIL_OBSTACLE_BIG_H*
        STORM_ROCK_BIG_WORDS*2);
    if(stormRockShardMask) FreeMem(stormRockShardMask,
        (LONG)STORM_ROCK_SHARD_FRAMES*STORMRAIL_OBSTACLE_SHARD_H*
        STORM_ROCK_SHARD_WORDS*2);
    if(stormRockShardBits) FreeMem(stormRockShardBits,
        (LONG)STORM_ROCK_SHARD_FRAMES*FRONT_PLANES*
        STORMRAIL_OBSTACLE_SHARD_H*
        STORM_ROCK_SHARD_WORDS*2);
    if(stormRockPillarMask) FreeMem(stormRockPillarMask,
        (LONG)STORM_ROCK_PILLAR_FRAMES*STORMRAIL_OBSTACLE_PILLAR_H*
        STORM_ROCK_PILLAR_WORDS*2);
    if(stormRockPillarBits) FreeMem(stormRockPillarBits,
        (LONG)STORM_ROCK_PILLAR_FRAMES*FRONT_PLANES*
        STORMRAIL_OBSTACLE_PILLAR_H*STORM_ROCK_PILLAR_WORDS*2);
    if(stormFinaleActorMask) FreeMem(stormFinaleActorMask,
        (LONG)STORMRAIL_FINALE_ACTOR_COUNT*STORM_FINALE_ACTOR_H*
        STORM_FINALE_ACTOR_WORDS*2);
    if(stormFinaleActorBits) FreeMem(stormFinaleActorBits,
        (LONG)STORMRAIL_FINALE_ACTOR_COUNT*FRONT_PLANES*STORM_FINALE_ACTOR_H*
        STORM_FINALE_ACTOR_WORDS*2);
    if(stormFinaleGateMask) FreeMem(stormFinaleGateMask,
        STORM_FINALE_GATE_HALVES*STORM_FINALE_GATE_H*
        STORM_FINALE_GATE_WORDS*2);
    if(stormFinaleGateBits) FreeMem(stormFinaleGateBits,
        (LONG)STORM_FINALE_GATE_HALVES*FRONT_PLANES*STORM_FINALE_GATE_H*
        STORM_FINALE_GATE_WORDS*2);
#ifdef SPARKPAW_STORMRAIL_DUST
    if(stormDustMask) FreeMem(stormDustMask,
        STORM_DUST_STYLES*STORM_DUST_H*STORM_DUST_WORDS*2);
    if(stormDustBits) FreeMem(stormDustBits,
        STORM_DUST_STYLES*FRONT_PLANES*STORM_DUST_H*STORM_DUST_WORDS*2);
#endif
#endif
    if(diamondBackground) FreeMem(diamondBackground,
        MAX_COLLECTIBLES*FRONT_PLANES*DIAMOND_PATCH_H*2);
    if(diamondWideBackground) FreeMem(diamondWideBackground,
        FRONT_PLANES*DIAMOND_PATCH_H*DIAMOND_WIDE_WORDS*2);
    if(waterBits) FreeMem(waterBits,WATER_FRAMES*FRONT_PLANES*WATER_H*
                          WATER_WORDS*2);
    if(splashBits) FreeMem(splashBits,SPLASH_FRAMES*FRONT_PLANES*SPLASH_H*
                           SPLASH_SOURCE_WORDS*2);
    if(splashMask) FreeMem(splashMask,SPLASH_FRAMES*SPLASH_H*
                           SPLASH_SOURCE_WORDS*2);
#ifdef SPARKPAW_ROLLING_PROTOTYPE
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
    if(prototypeTarget[0].clean) FreeBitMap(prototypeTarget[0].clean);
#endif
    if(prototypeTarget[0].display) FreeBitMap(prototypeTarget[0].display);
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
    if(prototypeTarget[1].clean) FreeBitMap(prototypeTarget[1].clean);
#endif
    if(prototypeTarget[1].display) FreeBitMap(prototypeTarget[1].display);
    memset(prototypeTarget,0,sizeof(prototypeTarget));
    memset(prototypeCopper,0,sizeof(prototypeCopper));
    prototypeActiveCopper=prototypePreparedCopper=0;
    prototypeCopperReady=FALSE;
#else
    if(frontDisplay) FreeBitMap(frontDisplay);
#endif
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
    if(rearDisplay) FreeBitMap(rearDisplay);
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(stormFlightRearDisplay) FreeBitMap(stormFlightRearDisplay);
#endif
#endif
    assetsUnloadGameplay();
    frontClean=rearWorld=sprites=hudBase=NULL;
    diamondSprite=heartSprite=coreSprite=extraLifeSprite=NULL;
    frontDisplay=NULL; cop=NULL;
    plasmaMask=plasmaBits=diamondMask=diamondBits=NULL;
    coreMask=coreBits=extraLifeMask=extraLifeBits=NULL;
    diamondBackground=diamondWideBackground=NULL;
    waterBits=splashMask=splashBits=NULL;
    striderStageMask=striderStageBits=NULL;
    memset(hwSprites,0,sizeof(hwSprites));
    memset(hwSpriteStage,0,sizeof(hwSpriteStage));
#ifdef SPARKPAW_AGA64_PLAYER_SPRITE
    memset(hwSpriteStageAllocation,0,sizeof(hwSpriteStageAllocation));
    nullSpriteAllocation=NULL;
#endif
    nullSprite=NULL;
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
    rearDisplay=NULL;
#endif
#ifdef SPARKPAW_STORMRAIL_PROOF
    stormrailSprite=stormrailObstacleSprite=stormrailFlightRear=NULL;
    heartMask=heartBits=NULL;
    stormVehicleMask=stormVehicleBits=NULL;
    stormFlightMask=stormFlightBits=NULL;
    stormCompactMask=stormCompactBits=NULL;
    stormDroneMask=stormDroneBits=NULL;
    stormRockBigMask=stormRockBigBits=NULL;
    stormRockShardMask=stormRockShardBits=NULL;
    stormRockPillarMask=stormRockPillarBits=NULL;
    stormFinaleActorMask=stormFinaleActorBits=NULL;
    stormFinaleGateMask=stormFinaleGateBits=NULL;
#ifdef SPARKPAW_STORMRAIL_DUST
    stormDustMask=stormDustBits=NULL;
#endif
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
    stormFlightRearDisplay=NULL;
#endif
    memset(&stormrailHistory,0,sizeof(stormrailHistory));
#endif
    memset(enemyCaches,0,sizeof(enemyCaches));
}

UWORD *rendererCopperList(void)
{
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    return prototypeCopper[prototypeActiveCopper];
#else
    return cop;
#endif
}

#ifdef SPARKPAW_STORMRAIL_PROOF
#ifndef SPARKPAW_CAMPAIGN_PLAY
BOOL rendererCaptureStormrailProof(UBYTE index)
{
    struct BitMap *bitmap;
    LONG planeBytes; UBYTE plane;
    if(index>=STORM_PROOF_FRAMES||stormProofPlanes[index]) return FALSE;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    bitmap=prototypeTarget[prototypeActiveCopper].display;
#else
    bitmap=frontDisplay;
#endif
    if(!bitmap) return FALSE;
    platformWaitBlit();
    planeBytes=(LONG)bitmap->BytesPerRow*WORLD_H;
    stormProofPlanes[index]=(UBYTE *)AllocMem(
        planeBytes*FRONT_PLANES,MEMF_FAST);
    if(!stormProofPlanes[index]) return FALSE;
    for(plane=0;plane<FRONT_PLANES;plane++)
        CopyMem(bitmap->Planes[plane],stormProofPlanes[index]+plane*planeBytes,
                planeBytes);
    stormProofRowBytes[index]=bitmap->BytesPerRow;
    stormProofTargetX[index]=(UWORD)prototypePhysicalX(
        game->stormrailMode?(WORD)(game->cameraX+game->stormrailX):180);
    stormProofTargetY[index]=(UWORD)game->stormrailY;
    return TRUE;
}

BOOL rendererWriteStormrailProofFrames(void)
{
    static const UBYTE magic[4]={'E','L','P','2'};
    static const char *names[STORM_PROOF_FRAMES]={
        "PROGDIR:stormrail-approach.raw",
        "PROGDIR:stormrail-boarding.raw",
        "PROGDIR:stormrail-flight-a.raw",
        "PROGDIR:stormrail-flight-b.raw",
        "PROGDIR:stormrail-loop-a.raw",
        "PROGDIR:stormrail-rock-intact.raw",
        "PROGDIR:stormrail-rock-cracked.raw",
        "PROGDIR:stormrail-rock-shards.raw"
    };
    UBYTE index; BOOL valid=TRUE;
    for(index=0;index<STORM_PROOF_FRAMES;index++) {
        BPTR file; UWORD header[6]; LONG bytes;
        if(!stormProofPlanes[index]) { valid=FALSE; continue; }
        file=Open((STRPTR)names[index],MODE_NEWFILE);
        if(!file) { valid=FALSE; continue; }
        header[0]=(UWORD)(stormProofRowBytes[index]*8);
        header[1]=WORLD_H;
        header[2]=stormProofRowBytes[index];
        header[3]=(UWORD)game->cameraX;
        header[4]=stormProofTargetX[index];
        header[5]=stormProofTargetY[index];
        bytes=(LONG)stormProofRowBytes[index]*WORLD_H*FRONT_PLANES;
        if(Write(file,(APTR)magic,4)!=4||
           Write(file,(APTR)header,sizeof(header))!=sizeof(header)||
           Write(file,stormProofPlanes[index],bytes)!=bytes) valid=FALSE;
        Close(file);
    }
    return valid;
}

#else
BOOL rendererCaptureStormrailProof(UBYTE index) { return FALSE; }
BOOL rendererWriteStormrailProofFrames(void) { return FALSE; }
#endif

ULONG rendererStormrailUnsafeBlits(void)
{
    return stormrailUnsafeBlits;
}
#endif

#ifdef SPARKPAW_REPLAY_PROOF
BOOL rendererReplayPresentationValid(void)
{
    WORD index;
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        const struct Collectible *item=collectibleAt(index);
        if(item->active&&(item->drawnY<item->y-2||item->drawnY>item->y+2))
            return FALSE;
    }
    return TRUE;
}
#endif

#ifdef SPARKPAW_EXTRA_LIFE_VISUAL_PROOF
BOOL rendererWriteExtraLifeProof(void)
{
    static const UBYTE magic[4]={'E','L','P','2'};
    UWORD header[6];
    BPTR file;
    UBYTE plane;
    struct BitMap *bitmap;
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    bitmap=prototypeTarget[prototypeActiveCopper].display;
#else
    bitmap=frontDisplay;
#endif
    if(!bitmap) return FALSE;
    platformWaitBlit();
    file=Open("PROGDIR:extra-life-proof.raw",MODE_NEWFILE);
    if(!file) return FALSE;
    header[0]=(UWORD)(bitmap->BytesPerRow*8);
    header[1]=WORLD_H;
    header[2]=bitmap->BytesPerRow;
    header[3]=(UWORD)game->cameraX;
    header[4]=(UWORD)prototypePhysicalX(EXTRA_LIFE_X);
    header[5]=(UWORD)game->extraLifeY;
    if(Write(file,(APTR)magic,4)!=4||
       Write(file,(APTR)header,sizeof(header))!=sizeof(header)) {
        Close(file); return FALSE;
    }
    for(plane=0;plane<FRONT_PLANES;plane++)
        if(Write(file,bitmap->Planes[plane],
                 (LONG)bitmap->BytesPerRow*WORLD_H)!=
           (LONG)bitmap->BytesPerRow*WORLD_H) {
            Close(file); return FALSE;
        }
    if(Write(file,extraLifeMask,
             EXTRA_LIFE_H*EXTRA_LIFE_SOURCE_WORDS*2)!=
             EXTRA_LIFE_H*EXTRA_LIFE_SOURCE_WORDS*2||
       Write(file,extraLifeBits,
             EXTRA_LIFE_H*EXTRA_LIFE_SOURCE_WORDS*FRONT_PLANES*2)!=
             EXTRA_LIFE_H*EXTRA_LIFE_SOURCE_WORDS*FRONT_PLANES*2) {
        Close(file); return FALSE;
    }
    Close(file);
    return TRUE;
}
#endif

void rendererUpdateGameplay(void)
{
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    ULONG profileStart;
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    prototypePreparedCopper=(UBYTE)(prototypeActiveCopper^1);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    profileStart=performanceProfileBegin();
#endif
#ifdef SPARKPAW_COPPER_FULL_COPY_REFERENCE
    CopyMem(prototypeCopper[prototypeActiveCopper],
            prototypeCopper[prototypePreparedCopper],COP_WORDS*2);
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    performanceProfileEnd(PERF_COPPER_COPY,profileStart);
#endif
    cop=prototypeCopper[prototypePreparedCopper];
    frontDisplay=prototypeTarget[prototypePreparedCopper].display;
    prototypeDesiredOrigin=prototypeOriginForCamera((WORD)game->cameraX);
#endif
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    profileStart=performanceProfileBegin();
    setCoreWorldFlash();
    setHardwareSprite();
    performanceProfileEnd(PERF_SPRITE_STAGE,profileStart);
    profileStart=performanceProfileBegin();
    setHudPointers();
    performanceProfileEnd(PERF_HUD_UPDATE,profileStart);
    profileStart=performanceProfileBegin();
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive&&
       game->stormrailMode>=STORMRAIL_MODE_LAUNCH_OUT)
        setStormrailTransitionPalette();
#endif
    setScroll(game->cameraX,
#ifdef SPARKPAW_STORMRAIL_PROOF
              game->stormrailActive&&
              game->stormrailMode==STORMRAIL_MODE_FLIGHT?
                  stormrailRearScroll(game->stormrailDistance):
#endif
              game->cameraX>>2);
    performanceProfileEnd(PERF_SCROLL_PATCH,profileStart);
#else
    setCoreWorldFlash(); setHardwareSprite(); setHudPointers();
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive&&
       game->stormrailMode>=STORMRAIL_MODE_LAUNCH_OUT)
        setStormrailTransitionPalette();
#endif
    setScroll(game->cameraX,
#ifdef SPARKPAW_STORMRAIL_PROOF
              game->stormrailActive&&
              game->stormrailMode==STORMRAIL_MODE_FLIGHT?
                  stormrailRearScroll(game->stormrailDistance):
#endif
              game->cameraX>>2);
#endif
}

BOOL rendererPublishGameplay(UWORD rasterLine)
{
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    /* The list has a substantial palette/pointer preamble before DIWSTRT.
       Line 44 is the display start, not a valid restart deadline. */
    if(!prototypeCopperReady||!rollingFixedPublishBoundary(rasterLine))
        return FALSE;
    hw->cop1lc=(ULONG)prototypeCopper[prototypePreparedCopper];
    hw->copjmp1=0;
    prototypeActiveCopper=prototypePreparedCopper;
    prototypeCopperReady=FALSE;
    cop=prototypeCopper[prototypeActiveCopper];
    frontDisplay=prototypeTarget[prototypeActiveCopper].display;
    prototypeExposeHistoryUnion();
    return TRUE;
#else
    return TRUE;
#endif
}

void rendererDrawGameplayBobs(void)
{
#if defined(SPARKPAW_RENDER_DIAGNOSTIC) && \
    !defined(SPARKPAW_MINIMAL_CADENCE_DIAGNOSTIC)
    UWORD before,after; ULONG profileStart;
#define DIAG_CALL(slot,perf,call) do { \
    before=platformRasterLine(); profileStart=performanceProfileBegin(); \
    call; performanceProfileEnd(perf,profileStart); \
    after=platformRasterLine(); \
    diagnosticCurrent.familyLines[slot]+=(UWORD)( \
        (after<before?SPARKPAW_PAL_LINES:0)+after-before); \
} while(0)
#else
#define DIAG_CALL(slot,perf,call) do { call; } while(0)
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    {
        struct PrototypeTarget *target=&prototypeTarget[prototypePreparedCopper];
        if(prototypePreparedCopper==prototypeActiveCopper) {
            prototypeOwnershipViolations++;
            return;
        }
        prototypeBuildOrigin=target->origin;
        prototypeLoadHistory(prototypePreparedCopper);
#ifdef SPARKPAW_BLITTER_PRIORITY_CANDIDATE
        /* Composition serializes plane operations through WaitBlit, so the
           CPU has no useful Chip-RAM work while those blits are active. Give
           the Blitter all otherwise-free memory cycles for this bounded pass;
           display, sprite and audio DMA keep their higher hardware priority. */
        platformSetBlitterPriority(TRUE);
#endif
#ifdef SPARKPAW_STORMRAIL_PROOF
        if(game->stormrailActive&&
           game->stormrailMode==STORMRAIL_MODE_FLIGHT) {
            /* Flight is a derived compositor over the same rolling AGA
               targets. It owns only vehicle/shot history; no platform Bob
               family may restore or draw into a flight target. */
            DIAG_CALL(0,PERF_STORMRAIL_RESTORE,restoreStormrailBobs());
            DIAG_CALL(3,PERF_BOB_COMPACT_TARGET,
                      prototypePrepareCompactTarget(target));
            DIAG_CALL(1,PERF_STORMRAIL_DRAW,drawStormrailBobs());
        } else {
#endif
        DIAG_CALL(0,PERF_BOB_PROJECTILE_RESTORE,eraseProjectileBobs());
        DIAG_CALL(1,PERF_BOB_ENEMY_RESTORE,restoreEnemyBob());
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_RESTORE,restoreCollectibleBobs());
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_RESTORE,restoreCoreBob());
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_RESTORE,restoreExtraLifeBob());
        DIAG_CALL(4,PERF_BOB_SPLASH_RESTORE,restoreSplashBob());
#ifdef SPARKPAW_STORMRAIL_PROOF
        if(game->stormrailActive) restoreStormrailBobs();
#endif
        DIAG_CALL(3,PERF_BOB_WATER,animateWater());
#ifndef SPARKPAW_COLLECTIBLE_CANONICAL_SYNC_REFERENCE
        DIAG_CALL(3,PERF_BOB_COMPACT_TARGET,prototypePrepareCompactTarget(target));
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawCollectibleBobs());
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawCoreBob());
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawExtraLifeBob());
#else
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawCollectibleBobs());
        DIAG_CALL(3,PERF_BOB_COMPACT_TARGET,prototypePrepareCompactTarget(target));
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawCoreBob());
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawExtraLifeBob());
#endif
        DIAG_CALL(4,PERF_BOB_SPLASH_DRAW,drawSplashBob());
        DIAG_CALL(1,PERF_BOB_ENEMY_DRAW,drawEnemyBob());
#ifdef SPARKPAW_STORMRAIL_PROOF
        if(game->stormrailActive) drawStormrailBobs();
#endif
        DIAG_CALL(0,PERF_BOB_PROJECTILE_DRAW,drawProjectileBobs());
#ifdef SPARKPAW_STORMRAIL_PROOF
        }
#endif
#if defined(SPARKPAW_RENDER_DIAGNOSTIC) && \
    !defined(SPARKPAW_MINIMAL_CADENCE_DIAGNOSTIC)
        profileStart=performanceProfileBegin();
#endif
        platformWaitBlit();
#if defined(SPARKPAW_RENDER_DIAGNOSTIC) && \
    !defined(SPARKPAW_MINIMAL_CADENCE_DIAGNOSTIC)
        performanceProfileEnd(PERF_BOB_FINAL_WAIT,profileStart);
#endif
#ifdef SPARKPAW_BLITTER_PRIORITY_CANDIDATE
        platformSetBlitterPriority(FALSE);
#endif
        prototypeSaveHistory(prototypePreparedCopper);
        /* Normal Level 1 has no actor-history consumer before publication.
           Publish exposes the union using the newly active target. Keep the
           pre-publication view for diagnostics and the Stormrail renderer. */
#if defined(SPARKPAW_STORMRAIL_PROOF) || defined(SPARKPAW_RENDER_DIAGNOSTIC)
        prototypeExposeHistoryUnion();
#endif
        prototypeCopperReady=TRUE;
    }
#else
    DIAG_CALL(0,PERF_BOB_PROJECTILE_RESTORE,eraseProjectileBobs());
    DIAG_CALL(1,PERF_BOB_ENEMY_RESTORE,restoreEnemyBob());
    DIAG_CALL(2,PERF_BOB_COLLECTIBLE_RESTORE,restoreCollectibleBobs());
    DIAG_CALL(2,PERF_BOB_COLLECTIBLE_RESTORE,restoreCoreBob());
    DIAG_CALL(2,PERF_BOB_COLLECTIBLE_RESTORE,restoreExtraLifeBob());
    DIAG_CALL(4,PERF_BOB_SPLASH_RESTORE,restoreSplashBob());
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) restoreStormrailBobs();
#endif
    DIAG_CALL(3,PERF_BOB_WATER,animateWater());
    DIAG_CALL(4,PERF_BOB_SPLASH_DRAW,drawSplashBob());
    DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawCollectibleBobs());
    DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawCoreBob());
    DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawExtraLifeBob());
    DIAG_CALL(1,PERF_BOB_ENEMY_DRAW,drawEnemyBob());
#ifdef SPARKPAW_STORMRAIL_PROOF
    if(game->stormrailActive) drawStormrailBobs();
#endif
    DIAG_CALL(0,PERF_BOB_PROJECTILE_DRAW,drawProjectileBobs());
#endif
#undef DIAG_CALL
#ifndef SPARKPAW_ROLLING_PROTOTYPE
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    profileStart=performanceProfileBegin();
#endif
    platformWaitBlit();
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    performanceProfileEnd(PERF_BOB_FINAL_WAIT,profileStart);
#endif
#endif
}

#ifdef SPARKPAW_RENDER_DIAGNOSTIC
static ULONG diagnosticSample(UWORD line)
{
    framePhaseSample(&diagnosticClock,line,0);
    return framePhaseStamp(&diagnosticClock);
}

static ULONG diagnosticPointer(UWORD highWord)
{
    return ((ULONG)cop[highWord]<<16)|cop[highWord+2];
}

void rendererDiagnosticUpdateEntry(UWORD line)
{
    ULONG updateTick;
    memset(&diagnosticCurrent,0,sizeof(diagnosticCurrent));
    memset(&diagnosticFrame,0,sizeof(diagnosticFrame));
    diagnosticCurrent.gameFrame=game->frameCounter;
    diagnosticCurrent.updateStamp=diagnosticSample(line);
    /* Raster lines and graphics.library's VBCounter can both miss complete
       fields while the custom display owns the machine. CIAB timer B is the
       independent E-clock; adjacent updates remain below its 92 ms wrap. */
    updateTick=platformProfileTimerTicks();
#ifdef SPARKPAW_LEVEL1_CADENCE_TEST
    /* Stop the bounded Level-1 sample at Core completion. The final image may
       remain resident while the user reaches for LMB without diluting FPS. */
    if(gameLevelComplete()) {
        diagnosticHasPreviousUpdate=FALSE;
        diagnosticPreviousUpdateField=updateTick;
    } else
#endif
#ifdef SPARKPAW_STORMRAIL_USER_CADENCE_TEST
#ifdef SPARKPAW_STORMRAIL_INTERLUDE_CADENCE_TEST
    /* Measure the integrated flight, transition and finale, but never include
       arbitrary user dwell after the automatic passage has completed. */
    if(game->stormrailMode!=STORMRAIL_MODE_FLIGHT||
       game->stormrailLaunchTimer<STORMRAIL_CADENCE_START||
       (game->stormrailFinaleActive&&
        game->stormrailFinalePhase==STORMRAIL_FINALE_PHASE_COMPLETE)) {
        diagnosticHasPreviousUpdate=FALSE;
        diagnosticPreviousUpdateField=updateTick;
    } else
#elif defined(SPARKPAW_STORMRAIL_FINALE_PROOF)
    /* A Gate-6 cadence drawer starts directly before the latch. Include only
       stationary combat intervals; exclude setup, opening and automatic exit. */
    if(!game->stormrailFinaleActive||
       game->stormrailFinalePhase!=STORMRAIL_FINALE_PHASE_COMBAT) {
        diagnosticHasPreviousUpdate=FALSE;
        diagnosticPreviousUpdateField=updateTick;
    } else
#else
    /* The player may take an arbitrary time to board. Measure exactly 1,000
       adjacent Flight intervals, never an attractive but irrelevant sample
       dominated by the idle cliff approach. */
    if(game->stormrailMode!=STORMRAIL_MODE_FLIGHT||
       game->stormrailLaunchTimer<STORMRAIL_CADENCE_START) {
        diagnosticHasPreviousUpdate=FALSE;
        diagnosticPreviousUpdateField=updateTick;
    } else
#endif
#endif
    if(diagnosticHasPreviousUpdate) {
        ULONG elapsed=updateTick-diagnosticPreviousUpdateField;
        ULONG fields=(elapsed+7094UL)/14188UL;
        if(fields<1) fields=1;
        diagnosticCadenceIntervals++;
        diagnosticCadenceFields+=fields;
        if(fields==1) diagnosticCadenceOne++;
        else if(fields==2) diagnosticCadenceTwo++;
        else diagnosticCadenceThreePlus++;
        if(fields>diagnosticCadenceMax) diagnosticCadenceMax=fields;
    }
    diagnosticPreviousUpdateField=updateTick;
    diagnosticHasPreviousUpdate=TRUE;
    diagnosticCurrent.cameraX=game->cameraX;
    diagnosticCurrent.playerX=playerState()->x>>8;
    if(line>100) diagnosticCurrent.flags|=DIAG_MISSED_100;
}

void rendererDiagnosticPublicationEntry(UWORD line)
{
    diagnosticCurrent.publishEntryStamp=diagnosticSample(line);
    diagnosticCurrent.blitterAtPublish=(UBYTE)platformBlitterBusy();
    if(line>=252||diagnosticCurrent.publishEntryStamp/SPARKPAW_PAL_LINES!=
       diagnosticCurrent.updateStamp/SPARKPAW_PAL_LINES)
        diagnosticCurrent.flags|=DIAG_MISSED_252;
}

void rendererDiagnosticPublicationExit(UWORD line)
{
    WORD i;
    diagnosticCurrent.publishExitStamp=diagnosticSample(line);
    diagnosticCurrent.generation=++diagnosticGeneration;
    diagnosticCurrent.copperAddress=(ULONG)cop;
    diagnosticCurrent.hudIndex=hudDisplayIndex();
    for(i=0;i<WORLD_PLANES;i++) {
        diagnosticCurrent.worldPointers[i]=diagnosticPointer(ptrValue[i]);
#ifdef SPARKPAW_HUD_NATIVE_3PLANE
        diagnosticCurrent.hudPointers[i]=i<3?
            diagnosticPointer(hudPtrValue[i]):0;
#else
        diagnosticCurrent.hudPointers[i]=diagnosticPointer(hudPtrValue[i]);
#endif
    }
    for(i=0;i<MAX_ENEMIES;i++) {
        const struct Enemy *enemy=enemyAt(i);
        struct RenderDiagnosticEnemy *out=&diagnosticCurrent.enemy[i];
        out->x=enemy->x>>8; out->y=enemy->y;
        out->active=(UBYTE)enemy->active; out->drawn=(UBYTE)enemy->drawn;
        out->spawn=enemy->spawnIndex; out->type=enemy->type;
        out->frame=enemy->animFrame; out->facing=(UBYTE)enemy->facingLeft;
    }
}

void rendererDiagnosticBobEntry(UWORD line)
{
    diagnosticCurrent.bobEntryStamp=diagnosticSample(line);
    diagnosticCurrent.blitterAtBobEntry=(UBYTE)platformBlitterBusy();
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    if(diagnosticCurrent.bobEntryStamp/SPARKPAW_PAL_LINES!=
       diagnosticCurrent.updateStamp/SPARKPAW_PAL_LINES)
#else
    if(line>253||diagnosticCurrent.bobEntryStamp/SPARKPAW_PAL_LINES!=
       diagnosticCurrent.updateStamp/SPARKPAW_PAL_LINES)
#endif
        diagnosticCurrent.flags|=DIAG_MISSED_253;
}

void rendererDiagnosticBobExit(UWORD line)
{
    LONG elapsed;
    diagnosticCurrent.bobCompleteStamp=diagnosticSample(line);
    diagnosticCurrent.blitterAtBobComplete=(UBYTE)platformBlitterBusy();
    diagnosticCurrent.counts=diagnosticFrame;
    elapsed=(LONG)(diagnosticCurrent.bobCompleteStamp-
                   diagnosticCurrent.bobEntryStamp);
    diagnosticFrames++;
    if(diagnosticCurrent.bobCompleteStamp/SPARKPAW_PAL_LINES!=
       diagnosticCurrent.bobEntryStamp/SPARKPAW_PAL_LINES) {
        diagnosticWraps++; diagnosticCurrent.flags|=DIAG_MISSED_FIELD;
    }
    if(elapsed>diagnosticWorstElapsed) {
        diagnosticWorstElapsed=elapsed;
        diagnosticWorstFrame=game->frameCounter;
        diagnosticWorstCamera=game->cameraX;
        diagnosticWorstStart=(UWORD)(diagnosticCurrent.bobEntryStamp%SPARKPAW_PAL_LINES);
        diagnosticWorstEnd=(UWORD)(diagnosticCurrent.bobCompleteStamp%SPARKPAW_PAL_LINES);
        diagnosticWorst=diagnosticFrame;
    }
}

void rendererDiagnosticBoundary(UWORD line,BOOL published)
{
    diagnosticCurrent.boundaryStamp=diagnosticSample(line);
    diagnosticCurrent.publicationSucceeded=(UBYTE)published;
    diagnosticTrace[diagnosticTraceNext]=diagnosticCurrent;
    diagnosticTraceNext=(UWORD)((diagnosticTraceNext+1)%DIAGNOSTIC_TRACE_FRAMES);
    if(diagnosticTraceCount<DIAGNOSTIC_TRACE_FRAMES) diagnosticTraceCount++;
}

void rendererWriteDiagnosticLog(void)
{
    BPTR file=Open("PROGDIR:renderdiag.log",MODE_NEWFILE);
    if(!file) return;
    FPrintf(file,"Sparkpaw render diagnostic 2026-08-20-alpha41-phase6c1\n");
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    FPrintf(file,"prototype=stage4g-earlier-hud-setup-no-copy-ring-front512x3\n");
#ifdef SPARKPAW_BLITTER_PRIORITY_CANDIDATE
    FPrintf(file,"performance_candidate=stage5b-bounded-blitter-priority\n");
#endif
#ifdef SPARKPAW_AGA32_FETCH_CANDIDATE
#ifdef SPARKPAW_AGA32_ORIGIN_CORRECTED
#ifdef SPARKPAW_AGA64_FETCH_CANDIDATE
    FPrintf(file,"aga_fetch_candidate=stage5i-bpl64-aligned-phase-plus16 fmode=3 ddf=$30..$d0 playfield_fetch_bytes=48 hud_fmode=1 hud_fetch_bytes=44\n");
#elif defined(SPARKPAW_AGA32_LEFT_GUARD)
#ifdef SPARKPAW_AGA64_PLAYER_SPRITE
    FPrintf(file,"aga_fetch_candidate=stage5l-bpl32-left-guard-wide-player fmode=$0d ddf=$20..$d0 playfield_fetch_bytes=48 guard_bytes=4 sprite_width=64 sprite_channels=2 hud_fmode=1 hud_fetch_bytes=44\n");
#ifdef SPARKPAW_HUD_SEAM_FMODE_FIRST
#ifdef SPARKPAW_HUD_SEAM_KEEP_DDFSTOP
#ifdef SPARKPAW_HUD_NATIVE_3PLANE
#ifdef SPARKPAW_HUD_RIGHT_FETCH_GUARD
    FPrintf(file,"hud_seam_candidate=stage5l-h4-native-three-plane-right-fetch-guard wait=$fbd1 bplcon0=$3200 sequence=bplcon0,fmode1,bplcon1,ddfstart30,ddfstopd8,three-hud-pointers hud_fetch_bytes=48 hud_line=252 bob_line=253\n");
#else
    FPrintf(file,"hud_seam_candidate=stage5l-h3-native-three-plane-hud wait=$fbd1 bplcon0=$3200 sequence=bplcon0,fmode1,bplcon1,ddfstart30,three-hud-pointers inherited_ddfstop=d0 hud_line=252 bob_line=253\n");
#endif
#else
    FPrintf(file,"hud_seam_candidate=stage5l-h2-fmode-first-retain-ddfstop wait=$fbd1 sequence=fmode1,bplcon1,ddfstart30,hud-pointers inherited_ddfstop=d0 hud_line=252 bob_line=253\n");
#endif
#else
    FPrintf(file,"hud_seam_candidate=fmode-first wait=$fbd1 sequence=fmode1,bplcon1,ddfstart30,ddfstopd0,hud-pointers hud_line=252 bob_line=253\n");
#endif
#endif
#ifdef SPARKPAW_HUD_SEAM_ISOLATE_SPRITES
    FPrintf(file,"hud_seam_isolation=h5-a-sprites-off final_playfield_line=251 hud_line=252 alpha43_split_unchanged\n");
#elif defined(SPARKPAW_HUD_SEAM_ISOLATE_BITPLANES)
    FPrintf(file,"hud_seam_isolation=h5-b-bitplanes-off blank_lines=251/252 restore_after_252 sprites_live alpha43_split_unchanged\n");
#elif defined(SPARKPAW_HUD_SEAM_ISOLATE_FRONT_PALETTE)
    FPrintf(file,"hud_seam_isolation=h6-a-front16-palette-black final_playfield_line=251 dma_and_pointers_unchanged\n");
#elif defined(SPARKPAW_HUD_SEAM_ISOLATE_REAR_PALETTE)
    FPrintf(file,"hud_seam_isolation=h6-b-rear8-palette-black final_playfield_line=251 dma_and_pointers_unchanged\n");
#endif
#else
    FPrintf(file,"aga_fetch_candidate=stage5g-bpl32-left-guard phase_plus16 fmode=1 playfield_fetch_bytes=48 guard_bytes=4 hud_fetch_bytes=44\n");
#endif
#else
    FPrintf(file,"aga_fetch_candidate=stage5e-bpl32-aligned-phase-plus16 fmode=1 fetch_bytes=44 hud_stride=44\n");
#endif
#else
    FPrintf(file,"aga_fetch_candidate=stage5c-bpl32 fmode=1 fetch_bytes=44 hud_stride=44\n");
#endif
#else
#ifdef SPARKPAW_FMODE0_EARLY_WORD_GUARD
    FPrintf(file,"aga_fetch_candidate=stage5k-fmode0-half-step-left-guard fmode=0 ddf=$28..$d0 playfield_fetch_bytes=44 guard_bytes=2 hud_fetch_bytes=42\n");
#else
    FPrintf(file,"aga_fetch_candidate=off fmode=0 fetch_bytes=42\n");
#endif
#endif
    FPrintf(file,"ring_width=512 physical_width=1536 ownership_violations=%ld active_target=%ld\n",
            prototypeOwnershipViolations,(LONG)prototypeActiveCopper);
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    FPrintf(file,"renderer=no-copy-ring512x3-4+3 hud_line=252 compose=inactive-after-update frames=%ld wraps=%ld\n",
            diagnosticFrames,diagnosticWraps);
#else
    FPrintf(file,"renderer=production-4+3 hud_line=252 bobs_line=253 passive_profile frames=%ld wraps=%ld\n",
            diagnosticFrames,diagnosticWraps);
#endif
    if(diagnosticCadenceIntervals&&diagnosticCadenceFields) {
        ULONG fps100=(diagnosticCadenceIntervals*5000UL)/diagnosticCadenceFields;
        ULONG fields1000=(diagnosticCadenceFields*1000UL)/diagnosticCadenceIntervals;
        FPrintf(file,"cadence intervals=%ld one_field=%ld two_field=%ld three_plus=%ld max_fields=%ld avg_fields_x1000=%ld effective_fps=%ld.%02ld\n",
                diagnosticCadenceIntervals,diagnosticCadenceOne,
                diagnosticCadenceTwo,diagnosticCadenceThreePlus,
                diagnosticCadenceMax,fields1000,fps100/100,fps100%100);
    }
    FPrintf(file,"worst_frame=%ld camera=%ld start_line=%ld end_line=%ld elapsed_lines=%ld margin_lines=%ld\n",
            diagnosticWorstFrame,diagnosticWorstCamera,
            (LONG)diagnosticWorstStart,(LONG)diagnosticWorstEnd,
            diagnosticWorstElapsed,59-diagnosticWorstElapsed);
    FPrintf(file,"worst_restore projectiles=%ld beetles=%ld striders=%ld collectibles=%ld splash=%ld\n",
            (LONG)diagnosticWorst.projectileRestore,
            (LONG)diagnosticWorst.beetleRestore,
            (LONG)diagnosticWorst.striderRestore,
            (LONG)diagnosticWorst.collectibleRestore,
            (LONG)diagnosticWorst.splashRestore);
    FPrintf(file,"worst_draw projectiles=%ld beetles=%ld striders=%ld collectibles=%ld splash=%ld water_updates=%ld\n",
            (LONG)diagnosticWorst.projectileDraw,
            (LONG)diagnosticWorst.beetleDraw,
            (LONG)diagnosticWorst.striderDraw,
            (LONG)diagnosticWorst.collectibleDraw,
            (LONG)diagnosticWorst.splashDraw,
            (LONG)diagnosticWorst.waterUpdates);
    FPrintf(file,"family_profile=disabled_in_production_to_preserve_cpu_blitter_overlap\n");
    FPrintf(file,"loading_frames setup=%ld target_alloc=%ld copper_alloc=%ld hud=%ld player_sprites=%ld rear_guard=%ld trace_alloc=%ld beetle=%ld strider=%ld strider_stage=%ld plasma=%ld diamond=%ld static_collectibles=%ld water=%ld splash=%ld ring_targets=%ld copper=%ld\n",
            diagnosticLoadingFrames[DIAG_LOAD_SETUP],
            diagnosticLoadingFrames[DIAG_LOAD_TARGET_ALLOC],
            diagnosticLoadingFrames[DIAG_LOAD_COPPER_ALLOC],
            diagnosticLoadingFrames[DIAG_LOAD_HUD],
            diagnosticLoadingFrames[DIAG_LOAD_PLAYER_SPRITES],
            diagnosticLoadingFrames[DIAG_LOAD_REAR_GUARD],
            diagnosticLoadingFrames[DIAG_LOAD_TRACE_ALLOC],
            diagnosticLoadingFrames[DIAG_LOAD_BEETLE],
            diagnosticLoadingFrames[DIAG_LOAD_STRIDER],
            diagnosticLoadingFrames[DIAG_LOAD_STRIDER_STAGE],
            diagnosticLoadingFrames[DIAG_LOAD_PLASMA],
            diagnosticLoadingFrames[DIAG_LOAD_DIAMOND],
            diagnosticLoadingFrames[DIAG_LOAD_STATIC_COLLECTIBLES],
            diagnosticLoadingFrames[DIAG_LOAD_WATER],
            diagnosticLoadingFrames[DIAG_LOAD_SPLASH],
            diagnosticLoadingFrames[DIAG_LOAD_RING_TARGETS],
            diagnosticLoadingFrames[DIAG_LOAD_COPPER]);
    performanceProfileWrite(file);
    audioDiagnosticWrite(file);
    FPrintf(file,"trace_fields=game epoch_line(update,patch_in,patch_out,bob_in,bob_done,boundary) flags camera player hud copper generation published busy(patch,bob_in,bob_done) fetch(mode,front_phase,rear_phase,bplcon1,front_logical,rear_logical,front_coarse,rear_coarse) world_ptrs hud_ptrs enemies counts\n");
    {
        UWORD n;
        for(n=0;n<diagnosticTraceCount;n++) {
            UWORD index=(UWORD)((diagnosticTraceNext+DIAGNOSTIC_TRACE_FRAMES-
                                diagnosticTraceCount+n)%DIAGNOSTIC_TRACE_FRAMES);
            const struct RenderDiagnosticTrace *trace=&diagnosticTrace[index];
            WORD i;
            FPrintf(file,"trace game=%ld stamps=%ld,%ld,%ld,%ld,%ld,%ld flags=%ld camera=%ld player=%ld hud=%ld copper=%08lx generation=%ld published=%ld busy=%ld,%ld,%ld family_lines=%ld/%ld/%ld/%ld/%ld fetch=%ld,%ld,%ld,%04lx,%ld,%ld,%ld,%ld world=",
                trace->gameFrame,trace->updateStamp,trace->publishEntryStamp,
                trace->publishExitStamp,trace->bobEntryStamp,
                trace->bobCompleteStamp,trace->boundaryStamp,
                (LONG)trace->flags,trace->cameraX,
                trace->playerX,(LONG)trace->hudIndex,trace->copperAddress,
                trace->generation,(LONG)trace->publicationSucceeded,
                (LONG)trace->blitterAtPublish,
                (LONG)trace->blitterAtBobEntry,
                (LONG)trace->blitterAtBobComplete,
                (LONG)trace->familyLines[0],(LONG)trace->familyLines[1],
                (LONG)trace->familyLines[2],(LONG)trace->familyLines[3],
                (LONG)trace->familyLines[4],(LONG)trace->fetchMode,
                (LONG)trace->frontPhase,(LONG)trace->rearPhase,
                (LONG)trace->scrollRegister,trace->frontLogical,
                trace->rearLogical,trace->frontCoarse,trace->rearCoarse);
            for(i=0;i<WORLD_PLANES;i++) FPrintf(file,"%08lx%s",
                trace->worldPointers[i],i==WORLD_PLANES-1?"":"/");
            FPrintf(file," hudptr=");
            for(i=0;i<WORLD_PLANES;i++) FPrintf(file,"%08lx%s",
                trace->hudPointers[i],i==WORLD_PLANES-1?"":"/");
            for(i=0;i<MAX_ENEMIES;i++) {
                const struct RenderDiagnosticEnemy *enemy=&trace->enemy[i];
                FPrintf(file," e%ld=%ld/%ld/%ld/%ld/%ld/%ld/%ld/%ld",
                    (LONG)i,enemy->x,(LONG)enemy->y,(LONG)enemy->active,
                    (LONG)enemy->drawn,(LONG)enemy->spawn,(LONG)enemy->type,
                    (LONG)enemy->frame,(LONG)enemy->facing);
            }
            FPrintf(file," counts=%ld/%ld/%ld/%ld/%ld/%ld/%ld/%ld/%ld/%ld/%ld\n",
                (LONG)trace->counts.projectileRestore,
                (LONG)trace->counts.projectileDraw,
                (LONG)trace->counts.beetleRestore,
                (LONG)trace->counts.beetleDraw,
                (LONG)trace->counts.striderRestore,
                (LONG)trace->counts.striderDraw,
                (LONG)trace->counts.collectibleRestore,
                (LONG)trace->counts.collectibleDraw,
                (LONG)trace->counts.splashRestore,
                (LONG)trace->counts.splashDraw,
                (LONG)trace->counts.waterUpdates);
        }
    }
    FPrintf(file,"prepared_peak chip_free=%ld chip_largest=%ld fast_free=%ld fast_largest=%ld\n",
            (LONG)phase6PeakChipFree,(LONG)phase6PeakChipLargest,
            (LONG)phase6PeakFastFree,(LONG)phase6PeakFastLargest);
    FPrintf(file,"post_run chip_free=%ld chip_largest=%ld fast_free=%ld fast_largest=%ld\n",
            (LONG)AvailMem(MEMF_CHIP),(LONG)AvailMem(MEMF_CHIP|MEMF_LARGEST),
            (LONG)AvailMem(MEMF_FAST),(LONG)AvailMem(MEMF_FAST|MEMF_LARGEST));
    Close(file);
}
#endif

#ifdef PHASE6_MEMORY_TEST
ULONG rendererPhase6PeakChipFree(void) { return phase6PeakChipFree; }
ULONG rendererPhase6PeakChipLargest(void) { return phase6PeakChipLargest; }
ULONG rendererPhase6PeakFastFree(void) { return phase6PeakFastFree; }
ULONG rendererPhase6PeakFastLargest(void) { return phase6PeakFastLargest; }
#endif

#ifdef SPARKPAW_LEVEL1_RENDERER_TU_ISOLATION
BOOL rendererCaptureStormrailProof(UBYTE index) { (void)index; return FALSE; }
BOOL rendererWriteStormrailProofFrames(void) { return FALSE; }
ULONG rendererStormrailUnsafeBlits(void) { return 0; }
#endif
