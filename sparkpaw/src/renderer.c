/* Sparkpaw: The Stormstone Quest -- AGA dual-playfield milestone. */
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
static const struct PlanarAsset *diamondSprite;
static struct BitMap *frontDisplay;
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
static struct BitMap *rearDisplay;
#endif
static UWORD *cop,copPos,ptrValue[WORLD_PLANES],scrollValue;
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
    }
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
    WORD i; copPos=0;
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
    for(i=0;i<32;i++) cmove((UWORD)(0x180+i*2),colors[i]);
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
        cop[copPos++]=(UWORD)(((44+64+i*4-1)<<8)|0xd9);
        cop[copPos++]=0xfffe;
        copperRearPalette(0,1,(UBYTE)i,4);
    }
    for(i=1;i<=8;i++) {
        cop[copPos++]=(UWORD)(((44+136+i*3-1)<<8)|0xd9);
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
    setPtr(4,frontDisplay->Planes[2],fo); setPtr(1,rearDisplay->Planes[0],ro);
    setPtr(3,rearDisplay->Planes[1],ro); setPtr(5,rearDisplay->Planes[2],ro);
    setPtr(6,frontDisplay->Planes[3],fo);
#else
    setPtr(0,frontDisplay->Planes[0],fo); setPtr(2,frontDisplay->Planes[1],fo);
    setPtr(4,frontDisplay->Planes[2],fo); setPtr(1,rearWorld->bitmap->Planes[0],ro);
    setPtr(3,rearWorld->bitmap->Planes[1],ro); setPtr(5,rearWorld->bitmap->Planes[2],ro);
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
            prototypeCopyCanonicalRect(target,
                (WORD)(oldOrigin+PROTOTYPE_RING_W),0,pixels,WORLD_H);
        } else {
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
    const struct BitMap *display;
    WORD plane;
    hudSetState(health,game->lives,game->diamonds);
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
    WORD stopY=screenY+SPRITE_H;
    UWORD facing=player->facingLeft?1:0;
    /* Blink the complete attached-sprite actor during accepted invulnerability.
       Pointer substitution preserves every cached 48-row stream and its real
       terminator; never shorten VSTOP to clip an attached pair. */
    if(game->waterSplashTimer||
       (player->invulnTimer&&!(player->invulnTimer&4))) {
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
            (UBYTE)player->animFrame);
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
            CopyMem(hwSprites[facing][player->animFrame][channel],data,
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
                (UBYTE)player->animFrame);
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
    UBYTE pen=playerPlasmaPatternPen(
        hostile?(UBYTE)(pattern-PLAYER_PLASMA_PATTERNS):pattern,left,x,y);
    if(!hostile) return pen;
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
#endif
    {
    UWORD words=(UWORD)(((localX&15)+width+15)>>4);
    LONG at=(LONG)y*frontDisplay->BytesPerRow+(localX>>4)*2;
    LONG sourceAt=(LONG)y*clean->BytesPerRow+(sourceX>>4)*2;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        platformWaitBlit();
        hw->bltcon0=0x09f0; hw->bltcon1=0;
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=(UWORD)(clean->BytesPerRow-words*2);
        hw->bltdmod=(UWORD)(frontDisplay->BytesPerRow-words*2);
        hw->bltapt=clean->Planes[plane]+sourceAt;
        hw->bltdpt=frontDisplay->Planes[plane]+at;
        hw->bltsize=(UWORD)((height<<6)|words);
    }
    }
}

static void blitMaskedBobTarget(struct BitMap *target,UWORD *mask,UWORD *bits,
                                WORD sourceWords,WORD width,WORD height,
                                WORD x,WORD y)
{
    UBYTE plane; UWORD shift=(UWORD)(x&15);
    UWORD words=(UWORD)((shift+width+15)>>4);
    LONG at=(LONG)y*target->BytesPerRow+(x>>4)*2;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        platformWaitBlit();
        hw->bltcon0=(UWORD)((shift<<12)|0x0fca);
        hw->bltcon1=(UWORD)(shift<<12);
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=(UWORD)((sourceWords-words)*2);
        hw->bltbmod=(UWORD)((sourceWords-words)*2);
        hw->bltcmod=(UWORD)(target->BytesPerRow-words*2);
        hw->bltdmod=(UWORD)(target->BytesPerRow-words*2);
        hw->bltapt=mask;
        hw->bltbpt=bits+(LONG)plane*height*sourceWords;
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
}

static void drawSplashBob(void)
{
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
    sprites=assetsPlayerSprites(); configureEnemyCaches();
    hudBase=assetsHudBase();
    diamondSprite=assetsCollectibleDiamond();
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
    if(!rearDisplay||(rearDisplay->BytesPerRow&3)) return FALSE;
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
    rearDisplay=AllocBitMap((UWORD)(rearWorld->width+PLAYFIELD_GUARD_BYTES*8),
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
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticLoadingFrames[DIAG_LOAD_COPPER_ALLOC]=
        GfxBase->VBCounter-diagnosticLoadStart;
    if(!frontDisplay||!cop) return FALSE;
    DIAG_LOAD(DIAG_LOAD_HUD,hudPrepare());
    DIAG_LOAD(DIAG_LOAD_PLAYER_SPRITES,buildHardwareSprites());
#else
    if(!frontDisplay||!cop||!hudPrepare()||!buildHardwareSprites())
        return FALSE;
#endif
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    DIAG_LOAD(DIAG_LOAD_REAR_GUARD,prepareRearGuardedDisplay());
#else
    if(!prepareRearGuardedDisplay()) return FALSE;
#endif
#endif
#ifdef SPARKPAW_AGA32_FETCH_CANDIDATE
    /* Never hand an invalid wide-fetch layout to Alice. Graphics.library may
       pad displayable rows, so validate its actual pointers and stride. */
    if(!aga32DisplayLayoutValid()) return FALSE;
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    if(!prototypeCopper[1]||!prototypeTarget[1].display
#ifndef SPARKPAW_CANONICAL_BOB_RESTORE
       ||!prototypeTarget[0].clean||!prototypeTarget[1].clean
#endif
       ) return FALSE;
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
    DIAG_LOAD(DIAG_LOAD_DIAMOND,buildDiamondPattern());
    DIAG_LOAD(DIAG_LOAD_STATIC_COLLECTIBLES,prepareStaticCollectibles());
    DIAG_LOAD(DIAG_LOAD_WATER,buildWaterPatterns());
    DIAG_LOAD(DIAG_LOAD_SPLASH,buildSplashPatterns());
#else
    if(!buildEnemyPatterns(&enemyCaches[ENEMY_TYPE_CLOCKWORK_BEETLE],FALSE)||
       !buildEnemyPatterns(&enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER],TRUE)||
       !prepareStriderStages()||
       !buildPlasmaPatterns()||!buildDiamondPattern()||
       !prepareStaticCollectibles()||!buildWaterPatterns()||
       !buildSplashPatterns())
        return FALSE;
#endif
#ifdef SPARKPAW_ROLLING_PROTOTYPE
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticLoadStart=GfxBase->VBCounter;
#endif
    prototypeCopyInitial(&prototypeTarget[0]);
    prototypeCopyInitial(&prototypeTarget[1]);
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
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
    diagnosticLoadingFrames[DIAG_LOAD_COPPER]=
        GfxBase->VBCounter-diagnosticLoadStart;
#endif
    assetsUnloadGameplayConversionSources();
    sprites=NULL; diamondSprite=NULL;
    enemyCaches[ENEMY_TYPE_CLOCKWORK_BEETLE].source=NULL;
    enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER].source=NULL;
    setScroll(0,0);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
#undef DIAG_LOAD
#endif
    return TRUE;
}

void rendererCleanup(void)
{
    WORD facing,frame,channel,type;
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
#else
    if(frontDisplay) FreeBitMap(frontDisplay);
#endif
#if defined(SPARKPAW_AGA32_LEFT_GUARD)||defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD)
    if(rearDisplay) FreeBitMap(rearDisplay);
#endif
    assetsUnloadGameplay();
}

UWORD *rendererCopperList(void)
{
#ifdef SPARKPAW_ROLLING_PROTOTYPE
    return prototypeCopper[prototypeActiveCopper];
#else
    return cop;
#endif
}

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
    setHardwareSprite();
    performanceProfileEnd(PERF_SPRITE_STAGE,profileStart);
    profileStart=performanceProfileBegin();
    setHudPointers();
    performanceProfileEnd(PERF_HUD_UPDATE,profileStart);
    profileStart=performanceProfileBegin();
    setScroll(game->cameraX,game->cameraX>>2);
    performanceProfileEnd(PERF_SCROLL_PATCH,profileStart);
#else
    setHardwareSprite(); setHudPointers();
    setScroll(game->cameraX,game->cameraX>>2);
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
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
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
        DIAG_CALL(0,PERF_BOB_PROJECTILE_RESTORE,eraseProjectileBobs());
        DIAG_CALL(1,PERF_BOB_ENEMY_RESTORE,restoreEnemyBob());
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_RESTORE,restoreCollectibleBobs());
        DIAG_CALL(4,PERF_BOB_SPLASH_RESTORE,restoreSplashBob());
        DIAG_CALL(3,PERF_BOB_WATER,animateWater());
#ifndef SPARKPAW_COLLECTIBLE_CANONICAL_SYNC_REFERENCE
        DIAG_CALL(3,PERF_BOB_COMPACT_TARGET,prototypePrepareCompactTarget(target));
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawCollectibleBobs());
#else
        DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawCollectibleBobs());
        DIAG_CALL(3,PERF_BOB_COMPACT_TARGET,prototypePrepareCompactTarget(target));
#endif
        DIAG_CALL(4,PERF_BOB_SPLASH_DRAW,drawSplashBob());
        DIAG_CALL(1,PERF_BOB_ENEMY_DRAW,drawEnemyBob());
        DIAG_CALL(0,PERF_BOB_PROJECTILE_DRAW,drawProjectileBobs());
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        profileStart=performanceProfileBegin();
#endif
        platformWaitBlit();
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
        performanceProfileEnd(PERF_BOB_FINAL_WAIT,profileStart);
#endif
#ifdef SPARKPAW_BLITTER_PRIORITY_CANDIDATE
        platformSetBlitterPriority(FALSE);
#endif
        prototypeSaveHistory(prototypePreparedCopper);
        prototypeExposeHistoryUnion();
        prototypeCopperReady=TRUE;
    }
#else
    DIAG_CALL(0,PERF_BOB_PROJECTILE_RESTORE,eraseProjectileBobs());
    DIAG_CALL(1,PERF_BOB_ENEMY_RESTORE,restoreEnemyBob());
    DIAG_CALL(2,PERF_BOB_COLLECTIBLE_RESTORE,restoreCollectibleBobs());
    DIAG_CALL(4,PERF_BOB_SPLASH_RESTORE,restoreSplashBob());
    DIAG_CALL(3,PERF_BOB_WATER,animateWater());
    DIAG_CALL(4,PERF_BOB_SPLASH_DRAW,drawSplashBob());
    DIAG_CALL(2,PERF_BOB_COLLECTIBLE_DRAW,drawCollectibleBobs());
    DIAG_CALL(1,PERF_BOB_ENEMY_DRAW,drawEnemyBob());
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
    ULONG updateField;
    memset(&diagnosticCurrent,0,sizeof(diagnosticCurrent));
    memset(&diagnosticFrame,0,sizeof(diagnosticFrame));
    diagnosticCurrent.gameFrame=game->frameCounter;
    diagnosticCurrent.updateStamp=diagnosticSample(line);
    updateField=diagnosticCurrent.updateStamp/SPARKPAW_PAL_LINES;
    if(diagnosticHasPreviousUpdate) {
        ULONG fields=updateField-diagnosticPreviousUpdateField;
        if(fields<1) fields=1;
        diagnosticCadenceIntervals++;
        diagnosticCadenceFields+=fields;
        if(fields==1) diagnosticCadenceOne++;
        else if(fields==2) diagnosticCadenceTwo++;
        else diagnosticCadenceThreePlus++;
        if(fields>diagnosticCadenceMax) diagnosticCadenceMax=fields;
    }
    diagnosticPreviousUpdateField=updateField;
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
