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
#include "collectibles.h"
#include "enemies.h"
#include "game.h"
#include "hud.h"
#include "level_data.h"
#include "platform_amiga.h"
#include "player.h"
#include "projectiles.h"
#include "renderer.h"
#include "world_config.h"

#define SCREEN_W 320
#define SCREEN_H 256
#define WORLD_H 256
#define FETCH_BYTES 42
#define COP_WORDS 512
#define SPRITE_W 48
#define SPRITE_H 48
#define ANIM_FRAMES PLAYER_ANIM_FRAMES
#define SPRITE_CHANNELS 6
#define TOTAL_SPRITE_CHANNELS 8
#define SPRITE_WORDS (2+SPRITE_H*2+2)
#define HUD_TOP 208
#define HUD_H (SCREEN_H-HUD_TOP)
#define PLAYER_PLASMA_PATTERNS 5
#define PLASMA_PATTERNS (PLAYER_PLASMA_PATTERNS*2)
#define PLASMA_SOURCE_WORDS 2
#define DIAMOND_SOURCE_WORDS 2
#define WATER_X LEVEL_WATER_LEFT
#define WATER_Y 197
#define WATER_W (LEVEL_WATER_RIGHT-LEVEL_WATER_LEFT+1)
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

static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static const struct PlanarAsset *frontClean,*rearWorld,*sprites;
static const struct PlanarAsset *hudBase;
static const struct PlanarAsset *diamondSprite;
static struct BitMap *frontDisplay;
static UWORD *cop,copPos,ptrValue[WORLD_PLANES],scrollValue;
static UWORD *hwSprites[2][ANIM_FRAMES][SPRITE_CHANNELS];
static UWORD hudPtrValue[WORLD_PLANES];
static UWORD *nullSprite,spritePtrValue[TOTAL_SPRITE_CHANNELS];
static const struct GameState *game;
static UWORD *plasmaMask,*plasmaBits;
static UWORD *diamondMask,*diamondBits;
static UWORD *waterBits;
static UBYTE waterDrawnFrame=255;
static UWORD *splashMask,*splashBits;
static BOOL splashDrawn;
static WORD splashDrawnX,splashDrawnY;

#ifdef PHASE6_MEMORY_TEST
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

static void cmove(UWORD reg,UWORD value) { cop[copPos++]=reg; cop[copPos++]=value; }

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
    cmove(0x092,0x0030); cmove(0x094,0x00d0);
    cmove(0x100,0x7600); cmove(0x102,0); scrollValue=copPos-1;
    cmove(0x104,0x0024); cmove(0x106,0x1020);
    cmove(0x108,frontDisplay->BytesPerRow-FETCH_BYTES);
    cmove(0x10a,rearWorld->bitmap->BytesPerRow-FETCH_BYTES);
    /* PF2 now owns entries 16..23. Move both even/odd hardware-sprite groups
       to entries 32..47 so Sparkpaw no longer aliases the rear palette. */
    cmove(0x10c,0x0022); cmove(0x1fc,0);
    cptr(0x0e0,frontDisplay->Planes[0],0); cptr(0x0e4,rearWorld->bitmap->Planes[0],1);
    cptr(0x0e8,frontDisplay->Planes[1],2); cptr(0x0ec,rearWorld->bitmap->Planes[1],3);
    cptr(0x0f0,frontDisplay->Planes[2],4); cptr(0x0f4,rearWorld->bitmap->Planes[2],5);
    cptr(0x0f8,frontDisplay->Planes[3],6);
    /* PF1 reads entries 0..15 and PF2OF=16 reads entries 16..23, all within
       AGA palette bank zero (COLOR00..31). */
    cmove(0x106,0x1020);
    for(i=0;i<32;i++) cmove((UWORD)(0x180+i*2),colors[i]);
    /* Three attached pairs provide one 48x48 actor. */
    for(i=0;i<SPRITE_CHANNELS;i++)
        spriteCptr((UWORD)(0x120+i*4),hwSprites[0][0][i],(UWORD)i);
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
    /* Switch before the PAL line-255 boundary to a fixed bottom HUD bitmap.
       The gameplay list restarts normally next frame, restoring the scrolling
       world pointers above it without a vertical-counter wrap sequence. */
    cop[copPos++]=(UWORD)(((44+HUD_TOP)<<8)|1); cop[copPos++]=0xfffe;
    /* Match the proven zero-world-offset fetch alignment. A zero fine-scroll
       value shifts this 42-byte fetch roughly 15 visible pixels left. */
    cmove(0x102,0x000f);
    for(i=0;i<WORLD_PLANES;i++) {
        const struct BitMap *display=hudDisplayBitmap();
        APTR value=(i&1)||(i==6)?(APTR)hudBlankPlane():
                    (APTR)display->Planes[i>>1];
        ULONG p=(ULONG)value;
        UWORD reg=(UWORD)(0x0e0+i*4);
        cmove(reg,(UWORD)(p>>16)); hudPtrValue[i]=copPos-1;
        cmove(reg+2,(UWORD)p);
    }
    cmove(0x108,hudBase->bitmap->BytesPerRow-FETCH_BYTES);
    cmove(0x10a,hudBase->bitmap->BytesPerRow-FETCH_BYTES);
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
    cop[copPos++]=0xffff; cop[copPos++]=0xfffe;
}

static void setPtr(UWORD plane,APTR base,LONG offset)
{
    ULONG p=(ULONG)base+offset; UWORD hi=ptrValue[plane];
    cop[hi]=(UWORD)(p>>16); cop[hi+2]=(UWORD)p;
}

static void setScroll(LONG front,LONG rear)
{
    UWORD ff=(UWORD)(15-(front&15)),rf=(UWORD)(15-(rear&15));
    LONG fo=(front>>4)<<1,ro=(rear>>4)<<1;
    setPtr(0,frontDisplay->Planes[0],fo); setPtr(2,frontDisplay->Planes[1],fo);
    setPtr(4,frontDisplay->Planes[2],fo); setPtr(1,rearWorld->bitmap->Planes[0],ro);
    setPtr(3,rearWorld->bitmap->Planes[1],ro); setPtr(5,rearWorld->bitmap->Planes[2],ro);
    setPtr(6,frontDisplay->Planes[3],fo);
    cop[scrollValue]=(rf<<4)|ff;
}

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

static BOOL buildEnemyPatterns(struct EnemyBobCache *cache)
{
    LONG maskWords=2L*cache->frames*cache->height*cache->sourceWords;
    LONG bitsWords=maskWords*FRONT_PLANES;
    UBYTE facing,frame,plane; WORD x,y;
    if(!cache->source||!cache->source->mask||
       cache->source->width!=cache->width*2||
       cache->source->height!=cache->height*cache->frames) return FALSE;
    cache->mask=(UWORD *)AllocMem(maskWords*2,MEMF_CHIP|MEMF_CLEAR);
    cache->bits=(UWORD *)AllocMem(bitsWords*2,MEMF_CHIP|MEMF_CLEAR);
    if(!cache->mask||!cache->bits) return FALSE;
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
    WORD facing,frame,half,pairPlane,x,y;
    nullSprite=(UWORD *)AllocMem(4,MEMF_CHIP|MEMF_CLEAR);
    if(!nullSprite) return FALSE;
    for(facing=0;facing<2;facing++) for(frame=0;frame<ANIM_FRAMES;frame++)
    for(half=0;half<3;half++) for(pairPlane=0;pairPlane<2;pairPlane++) {
        UWORD *data=(UWORD *)AllocMem(SPRITE_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
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
    for(plane=0;plane<WORLD_PLANES;plane++) {
        APTR value=(plane&1)||(plane==6)?(APTR)hudBlankPlane():
                    (APTR)display->Planes[plane>>1];
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
    for(channel=0;channel<SPRITE_CHANNELS;channel++) {
        UWORD *data=hwSprites[facing][player->animFrame][channel];
        WORD x=screenX+(channel>>1)*16;
        ULONG p=(ULONG)data; UWORD hi=spritePtrValue[channel];
        data[0]=(UWORD)((screenY<<8)|((x>>1)&0xff));
        data[1]=(UWORD)((stopY<<8)|((screenY&0x100)>>6)|
                        ((stopY&0x100)>>7)|(x&1)|((channel&1)?0x0080:0));
        cop[hi]=(UWORD)(p>>16); cop[hi+2]=(UWORD)p;
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

static void blitRestoreRect(WORD x,WORD y,WORD width,WORD height)
{
    UBYTE plane; UWORD words=(UWORD)(((x&15)+width+15)>>4);
    LONG at=(LONG)y*frontDisplay->BytesPerRow+(x>>4)*2;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        platformWaitBlit();
        hw->bltcon0=0x09f0; hw->bltcon1=0;
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=(UWORD)(frontClean->bitmap->BytesPerRow-words*2);
        hw->bltdmod=(UWORD)(frontDisplay->BytesPerRow-words*2);
        hw->bltapt=frontClean->bitmap->Planes[plane]+at;
        hw->bltdpt=frontDisplay->Planes[plane]+at;
        hw->bltsize=(UWORD)((height<<6)|words);
    }
}

static void blitMaskedBob(UWORD *mask,UWORD *bits,WORD sourceWords,
                          WORD width,WORD height,WORD x,WORD y)
{
    UBYTE plane; UWORD shift=(UWORD)(x&15);
    UWORD words=(UWORD)((width>>4)+(shift?1:0));
    LONG at=(LONG)y*frontDisplay->BytesPerRow+(x>>4)*2;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        platformWaitBlit();
        hw->bltcon0=(UWORD)((shift<<12)|0x0fca);
        hw->bltcon1=(UWORD)(shift<<12);
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=(UWORD)((sourceWords-words)*2);
        hw->bltbmod=(UWORD)((sourceWords-words)*2);
        hw->bltcmod=(UWORD)(frontDisplay->BytesPerRow-words*2);
        hw->bltdmod=(UWORD)(frontDisplay->BytesPerRow-words*2);
        hw->bltapt=mask;
        hw->bltbpt=bits+(LONG)plane*height*sourceWords;
        hw->bltcpt=frontDisplay->Planes[plane]+at;
        hw->bltdpt=frontDisplay->Planes[plane]+at;
        hw->bltsize=(UWORD)((height<<6)|words);
    }
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
    blitRestoreRect(splashDrawnX,splashDrawnY,SPLASH_W,SPLASH_H);
    splashDrawn=FALSE;
}

static void drawSplashBob(void)
{
    UBYTE frame;
    if(!game->waterSplashTimer) return;
    frame=(UBYTE)((16-game->waterSplashTimer)>>2);
    if(frame>=SPLASH_FRAMES) frame=SPLASH_FRAMES-1;
    splashDrawnX=(WORD)(game->waterSplashX-(SPLASH_W>>1));
    splashDrawnY=184;
    blitMaskedBob(splashMaskRow(frame,0),splashBitsRow(frame,0,0),
                  SPLASH_SOURCE_WORDS,SPLASH_W,SPLASH_H,
                  splashDrawnX,splashDrawnY);
    splashDrawn=TRUE;
}

static void blitWaterFrame(struct BitMap *target,UBYTE frame)
{
    UBYTE plane;
    LONG at=(LONG)WATER_Y*target->BytesPerRow+(WATER_X>>4)*2;
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
    UBYTE frame=(UBYTE)((game->frameCounter>>1)&(WATER_FRAMES-1));
    if(frame==waterDrawnFrame) return;
    /* Restore passes have finished. Update clean first and display second so
       later Bob draws and next-frame restores see the identical background. */
    blitWaterFrame(frontClean->bitmap,frame);
    blitWaterFrame(frontDisplay,frame);
    waterDrawnFrame=frame;
}

static void restoreCollectibleBobs(void)
{
    WORD index;
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        struct Collectible *item=collectibleAt(index);
        if(!item->drawn) continue;
        blitRestoreRect(item->drawnX,item->drawnY,
                        COLLECTIBLE_W,COLLECTIBLE_H);
        item->drawn=FALSE;
    }
}

static void drawCollectibleBobs(void)
{
    static const BYTE hover[8]={0,-1,-2,-1,0,1,2,1};
    WORD index;
    for(index=0;index<MAX_COLLECTIBLES;index++) {
        struct Collectible *item=collectibleAt(index);
        if(!item->active||item->x+COLLECTIBLE_W<(WORD)game->cameraX-16||
           item->x>(WORD)game->cameraX+SCREEN_W+16) continue;
        item->drawnX=item->x;
        item->drawnY=(WORD)(item->y+
            hover[((game->frameCounter>>2)+index)&7]);
        blitMaskedBob(diamondMask,diamondBits,DIAMOND_SOURCE_WORDS,
                      COLLECTIBLE_W,COLLECTIBLE_H,
                      item->drawnX,item->drawnY);
        item->drawn=TRUE;
    }
}

static void eraseProjectileBobs(void)
{
    WORD i;
    for(i=0;i<MAX_PROJECTILES;i++) {
        struct Projectile *projectile=projectileAt(i);
        if(!projectile->drawn) continue;
        blitRestoreRect(projectile->drawnX,projectile->drawnY,
                        PROJECTILE_W,PROJECTILE_H);
        projectile->drawn=FALSE;
    }
}

static void drawProjectileBobs(void)
{
    WORD i;
    for(i=0;i<MAX_PROJECTILES;i++) {
        struct Projectile *p=projectileAt(i); UBYTE pattern,left;
        if(!p->active) continue;
        p->drawnX=(WORD)(p->x>>8); p->drawnY=(WORD)(p->y>>8);
        if(p->drawnX<0||p->drawnX+PROJECTILE_W>WORLD_W||
           p->drawnY<0||p->drawnY+PROJECTILE_H>WORLD_H) continue;
        left=p->vx<0;
        pattern=p->impactTimer?(p->impactTimer>=4?2:(p->impactTimer>=2?3:4)):
                                (UBYTE)((game->frameCounter>>1)&1);
        if(p->hostile) pattern+=PLAYER_PLASMA_PATTERNS;
        blitMaskedBob(plasmaMaskRow(pattern,left,0),
                      plasmaBitsRow(pattern,left,0,0),PLASMA_SOURCE_WORDS,
                      PROJECTILE_W,PROJECTILE_H,p->drawnX,p->drawnY);
        p->drawn=TRUE;
    }
}

static void restoreEnemyBob(void)
{
    WORD i;
    for(i=0;i<MAX_ENEMIES;i++) {
        struct Enemy *enemy=enemyAt(i);
        struct EnemyBobCache *cache;
        if(!enemy->drawn) continue;
        cache=&enemyCaches[enemy->drawnType<ENEMY_TYPE_COUNT?
                           enemy->drawnType:ENEMY_TYPE_CLOCKWORK_BEETLE];
        blitRestoreRect(enemy->drawnX,enemy->drawnY,
                        cache->width,cache->height);
        enemy->drawn=FALSE;
    }
}

static void drawEnemyBob(void)
{
    WORD i;
    for(i=0;i<MAX_ENEMIES;i++) {
        struct Enemy *enemy=enemyAt(i); UBYTE facing;
        struct EnemyBobCache *cache;
        if(!enemy->active||enemy->type>=ENEMY_TYPE_COUNT) continue;
        cache=&enemyCaches[enemy->type];
        enemy->drawnX=(WORD)(enemy->x>>8);
        /* The accepted rb18 Strider has transparent source rows 62-63. Its
           logical 64px collision cell remains grounded at enemy->y+64, while
           the Bob needs a two-pixel visual offset for row 61 to meet the last
           free row above that surface. Beetle geometry remains unchanged. */
        enemy->drawnY=(WORD)(enemy->y+
            (enemy->type==ENEMY_TYPE_CLOCKWORK_STORM_STRIDER?2:0));
          if(enemy->drawnX+cache->width<(WORD)game->cameraX-32||
              enemy->drawnX>(WORD)game->cameraX+SCREEN_W+32||
           enemy->drawnX<0||enemy->drawnX+cache->width>WORLD_W||
           enemy->drawnY<0||enemy->drawnY+cache->height>WORLD_H||
           enemy->animFrame>=cache->frames) continue;
        /* Caches normalize each source sheet to logical left/right columns. */
        facing=enemy->facingLeft?0:1;
        blitMaskedBob(enemyMaskRow(cache,facing,enemy->animFrame,0),
                      enemyBitsRow(cache,facing,enemy->animFrame,0,0),
                      cache->sourceWords,cache->width,cache->height,
                      enemy->drawnX,enemy->drawnY);
        enemy->drawnType=enemy->type;
        enemy->drawn=TRUE;
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

BOOL rendererPrepareGameplay(void)
{
    UBYTE p;
    game=gameState();
    frontDisplay=AllocBitMap(WORLD_W,WORLD_H,FRONT_PLANES,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    cop=(UWORD *)AllocMem(COP_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    if(!frontDisplay||!cop||!hudPrepare()||!buildHardwareSprites())
        return FALSE;
    for(p=0;p<FRONT_PLANES;p++) CopyMem(frontClean->bitmap->Planes[p],frontDisplay->Planes[p],
                             (LONG)frontDisplay->BytesPerRow*WORLD_H);
    if(!buildEnemyPatterns(&enemyCaches[ENEMY_TYPE_CLOCKWORK_BEETLE])||
       !buildEnemyPatterns(&enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER])||
       !buildPlasmaPatterns()||!buildDiamondPattern()||!buildWaterPatterns()||
       !buildSplashPatterns())
        return FALSE;
#ifdef PHASE6_MEMORY_TEST
    phase6PeakChipFree=AvailMem(MEMF_CHIP);
    phase6PeakChipLargest=AvailMem(MEMF_CHIP|MEMF_LARGEST);
    phase6PeakFastFree=AvailMem(MEMF_FAST);
    phase6PeakFastLargest=AvailMem(MEMF_FAST|MEMF_LARGEST);
#endif
    /* buildCopper still consumes the player palette from the conversion
       source; finish that final read before releasing the source sheets. */
    buildCopper();
    assetsUnloadGameplayConversionSources();
    sprites=NULL; diamondSprite=NULL;
    enemyCaches[ENEMY_TYPE_CLOCKWORK_BEETLE].source=NULL;
    enemyCaches[ENEMY_TYPE_CLOCKWORK_STORM_STRIDER].source=NULL;
    setScroll(0,0); return TRUE;
}

void rendererCleanup(void)
{
    WORD facing,frame,channel,type;
    if(cop) FreeMem(cop,COP_WORDS*2);
    for(facing=0;facing<2;facing++) for(frame=0;frame<ANIM_FRAMES;frame++)
        for(channel=0;channel<SPRITE_CHANNELS;channel++)
            if(hwSprites[facing][frame][channel])
                FreeMem(hwSprites[facing][frame][channel],SPRITE_WORDS*2);
    if(nullSprite) FreeMem(nullSprite,4);
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
    if(diamondBits) FreeMem(diamondBits,COLLECTIBLE_H*DIAMOND_SOURCE_WORDS*
                            FRONT_PLANES*2);
    if(diamondMask) FreeMem(diamondMask,COLLECTIBLE_H*
                            DIAMOND_SOURCE_WORDS*2);
    if(waterBits) FreeMem(waterBits,WATER_FRAMES*FRONT_PLANES*WATER_H*
                          WATER_WORDS*2);
    if(splashBits) FreeMem(splashBits,SPLASH_FRAMES*FRONT_PLANES*SPLASH_H*
                           SPLASH_SOURCE_WORDS*2);
    if(splashMask) FreeMem(splashMask,SPLASH_FRAMES*SPLASH_H*
                           SPLASH_SOURCE_WORDS*2);
    if(frontDisplay) FreeBitMap(frontDisplay);
    assetsUnloadGameplay();
}

UWORD *rendererCopperList(void)
{
    return cop;
}

void rendererUpdateGameplay(void)
{
    setHardwareSprite(); setHudPointers();
    setScroll(game->cameraX,game->cameraX>>2);
}

void rendererDrawGameplayBobs(void)
{
    eraseProjectileBobs(); restoreEnemyBob(); restoreCollectibleBobs();
    restoreSplashBob();
    animateWater();
    drawSplashBob(); drawCollectibleBobs(); drawEnemyBob(); drawProjectileBobs();
    platformWaitBlit();
}

#ifdef PHASE6_MEMORY_TEST
ULONG rendererPhase6PeakChipFree(void) { return phase6PeakChipFree; }
ULONG rendererPhase6PeakChipLargest(void) { return phase6PeakChipLargest; }
ULONG rendererPhase6PeakFastFree(void) { return phase6PeakFastFree; }
ULONG rendererPhase6PeakFastLargest(void) { return phase6PeakFastLargest; }
#endif
