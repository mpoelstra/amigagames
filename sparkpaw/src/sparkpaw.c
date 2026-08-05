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
#include "audio.h"
#include "collision.h"
#include "enemies.h"
#include "game.h"
#include "player.h"
#include "projectiles.h"

#define SCREEN_W 320
#define SCREEN_H 256
#define WORLD_W 1280
#define WORLD_H 256
#define FETCH_BYTES 42
#define COP_WORDS 320
#define SPRITE_W 48
#define SPRITE_H 48
#define ANIM_FRAMES PLAYER_ANIM_FRAMES
#define SPRITE_CHANNELS 6
#define TOTAL_SPRITE_CHANNELS 8
#define SPRITE_WORDS (2+SPRITE_H*2+2)
#define PLASMA_PATTERNS 5
#define PLASMA_SOURCE_WORDS 2

struct GfxBase *GfxBase;
static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static const struct PlanarAsset *frontClean,*rearWorld,*sprites,*enemySprites;
static struct BitMap *frontDisplay;
static struct View *oldView;
static UWORD *cop,copPos,ptrValue[6],scrollValue,oldDma,oldIntena;
static UWORD *hwSprites[2][ANIM_FRAMES][SPRITE_CHANNELS];
static UWORD *nullSprite,spritePtrValue[TOTAL_SPRITE_CHANNELS];
static BOOL systemLocked,interruptsDisabled;
static const struct GameState *game;
static UWORD *plasmaMask,*plasmaBits;
static UWORD *enemyMask,*enemyBits;

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
    static const UWORD colors[16]={
        0x001,0x111,0xd41,0xf92,0xfea,0x26c,0x3ce,0x94c,
        0x001,0x013,0x125,0x247,0x449,0x65a,0x97b,0xcbd
    };
    WORD i; copPos=0;
    cmove(0x08e,0x2c81); cmove(0x090,0x2cc1);
    cmove(0x092,0x0030); cmove(0x094,0x00d0);
    cmove(0x100,0x6600); cmove(0x102,0); scrollValue=copPos-1;
    cmove(0x104,0x0024); cmove(0x106,0x0c00);
    cmove(0x108,frontDisplay->BytesPerRow-FETCH_BYTES);
    cmove(0x10a,rearWorld->bitmap->BytesPerRow-FETCH_BYTES);
    /* AGA defaults: both even and odd sprites use palette bank 1 (16..31). */
    cmove(0x10c,0x0011); cmove(0x1fc,0);
    cptr(0x0e0,frontDisplay->Planes[0],0); cptr(0x0e4,rearWorld->bitmap->Planes[0],1);
    cptr(0x0e8,frontDisplay->Planes[1],2); cptr(0x0ec,rearWorld->bitmap->Planes[1],3);
    cptr(0x0f0,frontDisplay->Planes[2],4); cptr(0x0f4,rearWorld->bitmap->Planes[2],5);
    for(i=0;i<16;i++) cmove((UWORD)(0x180+i*2),colors[i]);
    /* Three attached pairs provide one 48x48 actor; the final attached pair
       is reserved for the energy bolt and its compact impact flash. */
    for(i=0;i<SPRITE_CHANNELS;i++)
        spriteCptr((UWORD)(0x120+i*4),hwSprites[0][0][i],(UWORD)i);
    for(i=SPRITE_CHANNELS;i<TOTAL_SPRITE_CHANNELS;i++)
        spriteCptr((UWORD)(0x120+i*4),nullSprite,(UWORD)i);
    for(i=0;i<16;i++) {
        const UBYTE *rgb=sprites->palette[i];
        UWORD amiga=(UWORD)(((rgb[0]>>4)<<8)|((rgb[1]>>4)<<4)|(rgb[2]>>4));
        cmove((UWORD)(0x1a0+i*2),amiga);
    }
    /* AGA low-nibble palette pass: playfields stay exact 12-bit while the
       character uses the complete 24-bit source palette. */
    cmove(0x106,0x0e00);
    for(i=0;i<16;i++) cmove((UWORD)(0x180+i*2),0);
    for(i=0;i<16;i++) {
        const UBYTE *rgb=sprites->palette[i];
        UWORD amiga=(UWORD)(((rgb[0]&15)<<8)|((rgb[1]&15)<<4)|(rgb[2]&15));
        cmove((UWORD)(0x1a0+i*2),amiga);
    }
    cmove(0x106,0x0c00);
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
    cop[scrollValue]=(rf<<4)|ff;
}

static UWORD rasterLine(void)
{
    UWORD h=hw->vposr,l=hw->vhposr; return (UWORD)(((h&7)<<8)|(l>>8));
}

static void waitFrame(void)
{
    while(rasterLine()<300) { }
    while(rasterLine()>=300) { }
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

static UBYTE enemyPen(WORD x,WORD y)
{
    UBYTE mask=(UBYTE)(0x80>>(x&7));
    if(!(enemySprites->mask[(LONG)y*enemySprites->rowBytes+(x>>3)]&mask)) return 0;
    return pixel(enemySprites->bitmap,x,y,3);
}

static UWORD *enemyMaskRow(UBYTE facing,UBYTE frame,WORD row)
{
    LONG index=((LONG)facing*ENEMY_FRAMES+frame)*ENEMY_H+row;
    return enemyMask+index*ENEMY_SOURCE_WORDS;
}

static UWORD *enemyBitsRow(UBYTE facing,UBYTE frame,UBYTE plane,WORD row)
{
    LONG pattern=(LONG)facing*ENEMY_FRAMES+frame;
    LONG index=(pattern*3+plane)*ENEMY_H+row;
    return enemyBits+index*ENEMY_SOURCE_WORDS;
}

static BOOL buildEnemyPatterns(void)
{
    LONG maskWords=2L*ENEMY_FRAMES*ENEMY_H*ENEMY_SOURCE_WORDS;
    LONG bitsWords=maskWords*3;
    UBYTE facing,frame,plane; WORD x,y;
    enemyMask=(UWORD *)AllocMem(maskWords*2,MEMF_CHIP|MEMF_CLEAR);
    enemyBits=(UWORD *)AllocMem(bitsWords*2,MEMF_CHIP|MEMF_CLEAR);
    if(!enemyMask||!enemyBits) return FALSE;
    for(facing=0;facing<2;facing++) for(frame=0;frame<ENEMY_FRAMES;frame++)
        for(y=0;y<ENEMY_H;y++) for(x=0;x<ENEMY_W;x++) {
            UBYTE pen=enemyPen(facing*ENEMY_W+x,frame*ENEMY_H+y);
            UBYTE at=(UBYTE)(x>>4); UWORD bit=(UWORD)(0x8000U>>(x&15));
            if(!pen) continue;
            enemyMaskRow(facing,frame,y)[at]|=bit;
            for(plane=0;plane<3;plane++)
                if(pen&(1<<plane)) enemyBitsRow(facing,frame,plane,y)[at]|=bit;
        }
    return TRUE;
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

static void setHardwareSprite(void)
{
    const struct PlayerState *player=playerState();
    WORD channel,screenX=(WORD)(player->x>>8)-(WORD)game->cameraX+128-
                         (SPRITE_W-PLAYER_W)/2;
    WORD screenY=(WORD)(player->y>>8)+44-(SPRITE_H-PLAYER_H);
    WORD stopY=screenY+SPRITE_H;
    UWORD facing=player->facingLeft?1:0;
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

static UBYTE plasmaPatternPen(UBYTE pattern,BOOL left,WORD x,WORD y)
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

static UWORD *plasmaMaskRow(UBYTE pattern,BOOL left,WORD row)
{
    LONG index=((LONG)pattern*2+left)*PROJECTILE_H+row;
    return plasmaMask+index*PLASMA_SOURCE_WORDS;
}

static UWORD *plasmaBitsRow(UBYTE pattern,BOOL left,UBYTE plane,WORD row)
{
    LONG source=(LONG)pattern*2+left;
    LONG index=(source*3+plane)*PROJECTILE_H+row;
    return plasmaBits+index*PLASMA_SOURCE_WORDS;
}

static BOOL buildPlasmaPatterns(void)
{
    LONG maskWords=PLASMA_PATTERNS*2L*PROJECTILE_H*PLASMA_SOURCE_WORDS;
    LONG bitsWords=maskWords*3;
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
            for(plane=0;plane<3;plane++)
                if(pen&(1<<plane)) plasmaBitsRow(pattern,left,plane,y)[0]|=bit;
        }
    return TRUE;
}

static void waitPrivateBlit(void)
{
    (void)hw->dmaconr;
    while(hw->dmaconr&DMAF_BLTDONE) { }
}

static void blitRestoreRect(WORD x,WORD y,WORD width,WORD height)
{
    UBYTE plane; UWORD words=(UWORD)(((x&15)+width+15)>>4);
    LONG at=(LONG)y*frontDisplay->BytesPerRow+(x>>4)*2;
    for(plane=0;plane<3;plane++) {
        waitPrivateBlit();
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
    for(plane=0;plane<3;plane++) {
        waitPrivateBlit();
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
        if(!enemy->drawn) continue;
        blitRestoreRect(enemy->drawnX,enemy->drawnY,ENEMY_W,ENEMY_H);
        enemy->drawn=FALSE;
    }
}

static void drawEnemyBob(void)
{
    WORD i;
    for(i=0;i<MAX_ENEMIES;i++) {
        struct Enemy *enemy=enemyAt(i); UBYTE facing;
        if(!enemy->active) continue;
        enemy->drawnX=(WORD)(enemy->x>>8); enemy->drawnY=enemy->y;
          if(enemy->drawnX+ENEMY_W<(WORD)game->cameraX-32||
              enemy->drawnX>(WORD)game->cameraX+SCREEN_W+32||
           enemy->drawnX<0||enemy->drawnX+ENEMY_W>WORLD_W||
           enemy->drawnY<0||enemy->drawnY+ENEMY_H>WORLD_H) continue;
        /* The authored column faces left; the second is its exact mirror. */
        facing=enemy->facingLeft?0:1;
        blitMaskedBob(enemyMaskRow(facing,enemy->animFrame,0),
                      enemyBitsRow(facing,enemy->animFrame,0,0),
                      ENEMY_SOURCE_WORDS,ENEMY_W,ENEMY_H,
                      enemy->drawnX,enemy->drawnY);
        enemy->drawn=TRUE;
    }
}

static BOOL loadData(void)
{
    if(!assetsLoadGameplay()) return FALSE;
    frontClean=assetsFrontClean(); rearWorld=assetsRearWorld();
    sprites=assetsPlayerSprites(); enemySprites=assetsEnemySprites();
    return collisionLoad()&&audioLoad();
}

static BOOL prepare(void)
{
    UBYTE p;
    frontDisplay=AllocBitMap(WORLD_W,WORLD_H,3,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    cop=(UWORD *)AllocMem(COP_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    if(!frontDisplay||!cop||!buildHardwareSprites()) return FALSE;
    for(p=0;p<3;p++) CopyMem(frontClean->bitmap->Planes[p],frontDisplay->Planes[p],
                             (LONG)frontDisplay->BytesPerRow*WORLD_H);
    if(!buildEnemyPatterns()||!buildPlasmaPatterns()) return FALSE;
    buildCopper(); setScroll(0,0); return TRUE;
}

static void takeover(void)
{
    oldView=GfxBase->ActiView; oldDma=hw->dmaconr&DMAF_ALL; oldIntena=hw->intenar&0x7fff;
    LoadView(NULL); WaitTOF(); WaitTOF(); OwnBlitter(); WaitBlit(); Forbid(); systemLocked=TRUE;
    Disable(); interruptsDisabled=TRUE; hw->intena=0x7fff; hw->dmacon=DMAF_ALL;
    hw->cop1lc=(ULONG)cop; hw->copjmp1=0;
    hw->dmacon=DMAF_SETCLR|DMAF_MASTER|DMAF_RASTER|DMAF_COPPER|DMAF_SPRITE|
               DMAF_BLITTER;
    audioSetHardwareActive(TRUE);
}

static void restoreSystem(void)
{
    if(interruptsDisabled) {
        audioSetHardwareActive(FALSE);
        hw->dmacon=DMAF_ALL; hw->dmacon=DMAF_SETCLR|DMAF_MASTER|oldDma;
        if(oldView) {
            LoadView(oldView);
            if(oldView->LOFCprList&&oldView->LOFCprList->start) {
                hw->cop1lc=(ULONG)oldView->LOFCprList->start;
                hw->cop2lc=(ULONG)((oldView->SHFCprList&&oldView->SHFCprList->start)?
                                  oldView->SHFCprList->start:oldView->LOFCprList->start);
                hw->copjmp1=0;
            }
        }
        hw->intena=0x7fff; hw->intena=0x8000|oldIntena; Enable(); interruptsDisabled=FALSE;
    }
    if(oldView) { WaitTOF(); WaitTOF(); }
    if(systemLocked) { DisownBlitter(); Permit(); systemLocked=FALSE; }
}

static void cleanup(void)
{
    WORD facing,frame,channel;
    if(cop) FreeMem(cop,COP_WORDS*2);
    for(facing=0;facing<2;facing++) for(frame=0;frame<ANIM_FRAMES;frame++)
        for(channel=0;channel<SPRITE_CHANNELS;channel++)
            if(hwSprites[facing][frame][channel])
                FreeMem(hwSprites[facing][frame][channel],SPRITE_WORDS*2);
    if(nullSprite) FreeMem(nullSprite,4);
    if(plasmaBits) FreeMem(plasmaBits,PLASMA_PATTERNS*2L*PROJECTILE_H*
                           PLASMA_SOURCE_WORDS*3*2);
    if(plasmaMask) FreeMem(plasmaMask,PLASMA_PATTERNS*2L*PROJECTILE_H*
                           PLASMA_SOURCE_WORDS*2);
    if(enemyBits) FreeMem(enemyBits,2L*ENEMY_FRAMES*ENEMY_H*
                          ENEMY_SOURCE_WORDS*3*2);
    if(enemyMask) FreeMem(enemyMask,2L*ENEMY_FRAMES*ENEMY_H*
                          ENEMY_SOURCE_WORDS*2);
    audioUnload();
    if(frontDisplay) FreeBitMap(frontDisplay);
    assetsUnloadGameplay();
    if(GfxBase) CloseLibrary((struct Library *)GfxBase);
}

int main(void)
{
    GfxBase=(struct GfxBase *)OpenLibrary("graphics.library",39);
    gameInit(); game=gameState();
    if(!GfxBase||!loadData()||!prepare()) { PutStr("Sparkpaw: runtime assets or Chip RAM unavailable.\n"); cleanup(); return 10; }
    setHardwareSprite();
    takeover();
    for(;;) {
        while(rasterLine()<100) { }
        gameUpdate();
        /* The Copper consumes these list entries at frame start. Update them
           well after that read and well before the next wrap, independent of
           how long the post-display Bob pass takes. */
        setHardwareSprite(); setScroll(game->cameraX,game->cameraX>>2);
        while(rasterLine()<300) { }
        eraseProjectileBobs(); restoreEnemyBob();
        drawEnemyBob(); drawProjectileBobs();
        waitPrivateBlit();
        while(rasterLine()>=300) { }
    }
    return 0;
}
