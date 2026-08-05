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

#include "collision.h"
#include "enemies.h"
#include "projectiles.h"

#define SCREEN_W 320
#define SCREEN_H 256
#define WORLD_W 1280
#define WORLD_H 256
#define FETCH_BYTES 42
#define COP_WORDS 320
#define FIX_SHIFT 8
#define PLAYER_W 32
#define PLAYER_H 40
#define SPRITE_W 48
#define SPRITE_H 48
#define ANIM_FRAMES 50
#define SPRITE_CHANNELS 6
#define TOTAL_SPRITE_CHANNELS 8
#define SPRITE_WORDS (2+SPRITE_H*2+2)
#define PLASMA_PATTERNS 5
#define PLASMA_SOURCE_WORDS 2
#define HIT_LEFT 4
#define HIT_RIGHT 27
#define HIT_TOP 5
#define HIT_CROUCH_TOP 19
#define HIT_BOTTOM 38
#define IDLE_ACT_DELAY 100
#define LAND_TICKS 10
#define TURN_TICKS 28
#define CROUCH_SHOT_TICKS 11

struct PlanarAsset {
    struct BitMap *bitmap;
    UWORD width,height,rowBytes;
    UBYTE depth,hasMask;
    UBYTE palette[32][3];
    UBYTE *mask;
};

struct Player {
    LONG x,y,vx,vy,turnStartVx;
    BOOL grounded,facingLeft,crouching,wallBlocked,turnTargetLeft,turnFinishing;
    UBYTE animFrame,runFrame,landTimer,turnTimer,shootTimer,shootCooldown;
    BOOL shotPending;
    UWORD runTick,idleTicks;
};

struct GfxBase *GfxBase;
static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static struct PlanarAsset frontClean,rearWorld,sprites,enemySprites;
static struct BitMap *frontDisplay;
static struct Player player;
static struct View *oldView;
static UWORD *cop,copPos,ptrValue[6],scrollValue,oldDma,oldIntena;
static UWORD *hwSprites[2][ANIM_FRAMES][SPRITE_CHANNELS];
static UWORD *nullSprite,spritePtrValue[TOTAL_SPRITE_CHANNELS];
static UBYTE *shotSample;
static LONG shotSampleBytes;
static BOOL systemLocked,interruptsDisabled;
static LONG cameraX,frameCounter;
static BOOL joystickUpHeld,joystickFireHeld;
static UBYTE shotDmaTicks;
static UWORD *plasmaMask,*plasmaBits;
static UWORD *enemyMask,*enemyBits;

static UWORD be16(const UBYTE *p) { return (UWORD)(((UWORD)p[0]<<8)|p[1]); }

static void freeAsset(struct PlanarAsset *a)
{
    if(a->mask) { FreeMem(a->mask,(LONG)a->rowBytes*a->height); a->mask=NULL; }
    if(a->bitmap) { FreeBitMap(a->bitmap); a->bitmap=NULL; }
}

static BOOL readRows(BPTR f,PLANEPTR p,UWORD fileRow,UWORD memRow,UWORD h)
{
    UWORD y;
    if(fileRow==memRow) return Read(f,p,(LONG)fileRow*h)==(LONG)fileRow*h;
    for(y=0;y<h;y++) if(Read(f,p+(LONG)y*memRow,fileRow)!=fileRow) return FALSE;
    return TRUE;
}

static BOOL loadAsset(const char *name,struct PlanarAsset *a,UBYTE wantedDepth)
{
    BPTR f; UBYTE header[12],plane; LONG size;
    memset(a,0,sizeof(*a)); f=Open((STRPTR)name,MODE_OLDFILE); if(!f) return FALSE;
    if(Read(f,header,12)!=12||memcmp(header,"SPBM",4)!=0) { Close(f); return FALSE; }
    a->width=be16(header+4); a->height=be16(header+6); a->depth=header[8];
    a->hasMask=header[9]; a->rowBytes=be16(header+10);
    if(a->depth!=wantedDepth||Read(f,a->palette,(LONG)(1<<a->depth)*3)!=(LONG)(1<<a->depth)*3) {
        Close(f); return FALSE;
    }
    a->bitmap=AllocBitMap(a->width,a->height,a->depth,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    if(!a->bitmap) { Close(f); return FALSE; }
    for(plane=0;plane<a->depth;plane++)
        if(!readRows(f,a->bitmap->Planes[plane],a->rowBytes,a->bitmap->BytesPerRow,a->height)) {
            Close(f); freeAsset(a); return FALSE;
        }
    if(a->hasMask) {
        size=(LONG)a->rowBytes*a->height; a->mask=(UBYTE *)AllocMem(size,MEMF_CHIP);
        if(!a->mask||Read(f,a->mask,size)!=size) { Close(f); freeAsset(a); return FALSE; }
    }
    Close(f); return TRUE;
}

static BOOL loadShotSample(void)
{
    BPTR f=Open("PROGDIR:assets/runtime/energy-shot.raw",MODE_OLDFILE);
    LONG size;
    if(!f) return FALSE;
    Seek(f,0,OFFSET_END); size=Seek(f,0,OFFSET_BEGINNING);
    if(size<=0) { Close(f); return FALSE; }
    shotSample=(UBYTE *)AllocMem(size,MEMF_CHIP);
    if(!shotSample||Read(f,shotSample,size)!=size) {
        Close(f); if(shotSample) FreeMem(shotSample,size); shotSample=NULL; return FALSE;
    }
    Close(f); shotSampleBytes=size; return TRUE;
}

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
    cmove(0x10a,rearWorld.bitmap->BytesPerRow-FETCH_BYTES);
    /* AGA defaults: both even and odd sprites use palette bank 1 (16..31). */
    cmove(0x10c,0x0011); cmove(0x1fc,0);
    cptr(0x0e0,frontDisplay->Planes[0],0); cptr(0x0e4,rearWorld.bitmap->Planes[0],1);
    cptr(0x0e8,frontDisplay->Planes[1],2); cptr(0x0ec,rearWorld.bitmap->Planes[1],3);
    cptr(0x0f0,frontDisplay->Planes[2],4); cptr(0x0f4,rearWorld.bitmap->Planes[2],5);
    for(i=0;i<16;i++) cmove((UWORD)(0x180+i*2),colors[i]);
    /* Three attached pairs provide one 48x48 actor; the final attached pair
       is reserved for the energy bolt and its compact impact flash. */
    for(i=0;i<SPRITE_CHANNELS;i++)
        spriteCptr((UWORD)(0x120+i*4),hwSprites[0][0][i],(UWORD)i);
    for(i=SPRITE_CHANNELS;i<TOTAL_SPRITE_CHANNELS;i++)
        spriteCptr((UWORD)(0x120+i*4),nullSprite,(UWORD)i);
    for(i=0;i<16;i++) {
        UBYTE *rgb=sprites.palette[i];
        UWORD amiga=(UWORD)(((rgb[0]>>4)<<8)|((rgb[1]>>4)<<4)|(rgb[2]>>4));
        cmove((UWORD)(0x1a0+i*2),amiga);
    }
    /* AGA low-nibble palette pass: playfields stay exact 12-bit while the
       character uses the complete 24-bit source palette. */
    cmove(0x106,0x0e00);
    for(i=0;i<16;i++) cmove((UWORD)(0x180+i*2),0);
    for(i=0;i<16;i++) {
        UBYTE *rgb=sprites.palette[i];
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
    setPtr(4,frontDisplay->Planes[2],fo); setPtr(1,rearWorld.bitmap->Planes[0],ro);
    setPtr(3,rearWorld.bitmap->Planes[1],ro); setPtr(5,rearWorld.bitmap->Planes[2],ro);
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
    if(!(sprites.mask[(LONG)y*sprites.rowBytes+(x>>3)]&mask)) return 0;
    return pixel(sprites.bitmap,x,y,4);
}

static UBYTE enemyPen(WORD x,WORD y)
{
    UBYTE mask=(UBYTE)(0x80>>(x&7));
    if(!(enemySprites.mask[(LONG)y*enemySprites.rowBytes+(x>>3)]&mask)) return 0;
    return pixel(enemySprites.bitmap,x,y,3);
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
    WORD channel,screenX=(WORD)(player.x>>8)-(WORD)cameraX+128-
                         (SPRITE_W-PLAYER_W)/2;
    WORD screenY=(WORD)(player.y>>8)+44-(SPRITE_H-PLAYER_H);
    WORD stopY=screenY+SPRITE_H;
    UWORD facing=player.facingLeft?1:0;
    for(channel=0;channel<SPRITE_CHANNELS;channel++) {
        UWORD *data=hwSprites[facing][player.animFrame][channel];
        WORD x=screenX+(channel>>1)*16;
        ULONG p=(ULONG)data; UWORD hi=spritePtrValue[channel];
        data[0]=(UWORD)((screenY<<8)|((x>>1)&0xff));
        data[1]=(UWORD)((stopY<<8)|((screenY&0x100)>>6)|
                        ((stopY&0x100)>>7)|(x&1)|((channel&1)?0x0080:0));
        cop[hi]=(UWORD)(p>>16); cop[hi+2]=(UWORD)p;
    }
}

static void playShot(void)
{
    if(!shotSample||!systemLocked) return;
    hw->dmacon=DMAF_AUD0;
    hw->aud[0].ac_ptr=(UWORD *)shotSample;
    hw->aud[0].ac_len=(UWORD)(shotSampleBytes>>1);
    hw->aud[0].ac_per=322;
    hw->aud[0].ac_vol=60;
    hw->dmacon=DMAF_SETCLR|DMAF_AUD0;
    shotDmaTicks=9;
}

static void updateAudio(void)
{
    if(shotDmaTicks&&!--shotDmaTicks) hw->dmacon=DMAF_AUD0;
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
        hw->bltamod=(UWORD)(frontClean.bitmap->BytesPerRow-words*2);
        hw->bltdmod=(UWORD)(frontDisplay->BytesPerRow-words*2);
        hw->bltapt=frontClean.bitmap->Planes[plane]+at;
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
                                (UBYTE)((frameCounter>>1)&1);
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
        if(enemy->drawnX+ENEMY_W<(WORD)cameraX-32||
           enemy->drawnX>(WORD)cameraX+SCREEN_W+32||
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

static void startShot(BOOL pressed)
{
    if(pressed&&!player.shootCooldown&&!player.turnTimer) {
        player.shootTimer=player.crouching?CROUCH_SHOT_TICKS:(player.grounded?7:10);
        player.shootCooldown=3; player.shotPending=FALSE;
        player.idleTicks=0;
        projectilesSpawn((WORD)(player.x>>8),(WORD)(player.y>>8),
                 player.facingLeft,player.crouching,playShot);
    }
}

static void updateShot(void)
{
    if(player.shootCooldown) player.shootCooldown--;
    if(player.shootTimer) player.shootTimer--;
}

static void animatePlayer(BOOL landed)
{
    if(landed) player.landTimer=LAND_TICKS;
    if(!player.grounded) {
        UBYTE base=player.shootTimer?42:10;
        if(player.vy<-220) player.animFrame=base;
        else if(player.vy<180) player.animFrame=(UBYTE)(base+1);
        else if(player.vy<700) player.animFrame=(UBYTE)(base+2);
        else player.animFrame=(UBYTE)(base+3);
        return;
    }
    if(player.landTimer) {
        if(player.landTimer>7) player.animFrame=14;
        else if(player.landTimer>4) player.animFrame=15;
        else player.animFrame=16;
        player.landTimer--; return;
    }
    if(player.crouching&&player.shootTimer) {
        if(player.shootTimer>9) player.animFrame=46;
        else if(player.shootTimer>7) player.animFrame=47;
        else if(player.shootTimer>3) player.animFrame=48;
        else player.animFrame=49;
        return;
    }
    if(player.shootTimer) {
        if(player.shootTimer>9) player.animFrame=38;
        else if(player.shootTimer>7) player.animFrame=39;
        else if(player.shootTimer>3) player.animFrame=40;
        else player.animFrame=41;
        return;
    }
    if(player.crouching) {
        if(player.vx>45||player.vx< -45)
            player.animFrame=(UBYTE)(((frameCounter>>2)&1)?18:19);
        else player.animFrame=17;
        return;
    }
    if(player.turnTimer) {
        UBYTE progress=(UBYTE)(TURN_TICKS-player.turnTimer);
        /* The two poses after the visual pivot share a longer inertia beat:
           Sparkpaw is already facing back while still sliding forward. */
        if(progress<16) player.animFrame=(UBYTE)(20+(progress/4));
        else if(progress<24) player.animFrame=24;
        else player.animFrame=25;
        player.turnTimer--;
        if(!player.turnTimer) player.turnFinishing=TRUE;
        return;
    }
    if(player.wallBlocked) {
        /* Do not keep cycling stretched run poses while pressing into a wall. */
        player.runTick=0; player.runFrame=0; player.animFrame=5; return;
    }
    if(player.vx>70||player.vx< -70) {
        LONG speed=player.vx<0?-player.vx:player.vx;
        /* Eight authored phases need a calmer cadence than the old four-pose
           cycle.  Roughly 12-13 animation changes/sec at full running speed
           keeps the paws readable without making locomotion feel sluggish. */
        player.runTick+=(UWORD)(speed/10);
        while(player.runTick>=256) {
            player.runTick-=256; player.runFrame=(UBYTE)((player.runFrame+1)&7);
        }
        player.animFrame=(UBYTE)(2+player.runFrame); return;
    }
    player.runTick=0; player.runFrame=0;
    if(player.idleTicks>=IDLE_ACT_DELAY) {
        /* A single deliberate side-to-front acknowledgement and return.  The
           old extended tail-sway sequence shifted the silhouette sideways and
           read as a moonwalk, so it is deliberately not part of playback. */
        UWORD phase=(UWORD)((player.idleTicks-IDLE_ACT_DELAY)%250);
        if(phase<32) player.animFrame=(UBYTE)(26+(phase/8));
        else if(phase<82) player.animFrame=30;
        else if(phase<114) player.animFrame=(UBYTE)(29-((phase-82)/8));
        else player.animFrame=0;
    } else player.animFrame=((frameCounter%180)>=176)?1:0;
}

static WORD playerHitTop(void)
{
    return player.crouching?HIT_CROUCH_TOP:HIT_TOP;
}

static BOOL canStand(WORD x,WORD y)
{
    WORD yy;
    for(yy=y+HIT_TOP;yy<y+HIT_CROUCH_TOP;yy++)
        if(collisionSolidHorizontal(x+HIT_LEFT,x+HIT_RIGHT,yy)) return FALSE;
    return TRUE;
}

static void moveX(LONG delta)
{
    LONG target=player.x+delta; WORD x=(WORD)(player.x>>FIX_SHIFT);
    WORD end=(WORD)(target>>FIX_SHIFT),y=(WORD)(player.y>>FIX_SHIFT),dir=delta<0?-1:1;
    if(delta&&collisionSolidVertical(x+(dir<0?HIT_LEFT-1:HIT_RIGHT+1),
                                     y+playerHitTop(),y+HIT_BOTTOM)) {
        player.wallBlocked=TRUE; player.vx=0; player.x=(LONG)x<<8; return;
    }
    while(x!=end) {
        WORD n=x+dir;
        WORD side=n+(dir<0?HIT_LEFT:HIT_RIGHT);
        if(collisionSolidVertical(side,y+playerHitTop(),y+HIT_BOTTOM)) {
            player.wallBlocked=TRUE; player.vx=0; player.x=(LONG)x<<8; return;
        }
        x=n;
    }
    player.x=target;
}

static void moveY(LONG delta)
{
    LONG target=player.y+delta; WORD x=(WORD)(player.x>>FIX_SHIFT);
    WORD y=(WORD)(player.y>>FIX_SHIFT),end=(WORD)(target>>FIX_SHIFT),dir=delta<0?-1:1;
    player.grounded=FALSE;
    while(y!=end) {
        WORD n=y+dir;
        WORD edge=n+(dir<0?playerHitTop():HIT_BOTTOM);
        if(collisionSolidHorizontal(x+HIT_LEFT,x+HIT_RIGHT,edge)) {
            if(dir>0) player.grounded=TRUE;
            player.vy=0; player.y=(LONG)y<<8; return;
        }
        y=n;
    }
    player.y=target;
    if(collisionSolidHorizontal(x+HIT_LEFT,x+HIT_RIGHT,y+HIT_BOTTOM+1))
        player.grounded=TRUE;
}

static void joystick(BOOL *left,BOOL *right,BOOL *down,BOOL *jump,BOOL *fire)
{
    UWORD v=*(volatile UWORD *)0xdff00c; BOOL up,held;
    *left=(v&0x0200)!=0; *right=(v&0x0002)!=0;
    *down=((v^(v>>1))&0x0001)!=0;
    up=((v^(v>>1))&0x0100)!=0; *jump=up&&!joystickUpHeld; joystickUpHeld=up;
    held=(*(volatile UBYTE *)0xbfe001&0x80)==0;
    *fire=held&&!joystickFireHeld; joystickFireHeld=held;
}

static void physics(BOOL left,BOOL right,BOOL down,BOOL jump)
{
    WORD px=(WORD)(player.x>>FIX_SHIFT),py=(WORD)(player.y>>FIX_SHIFT);
    LONG acceleration,maxSpeed;
    if(player.turnFinishing) {
        player.facingLeft=player.turnTargetLeft;
        player.turnFinishing=FALSE;
    }
    if(player.grounded&&down) { player.crouching=TRUE; player.turnTimer=0; }
    else if(player.crouching&&!down&&canStand(px,py)) player.crouching=FALSE;
    if(!player.grounded&&canStand(px,py)) player.crouching=FALSE;
    acceleration=player.crouching?38:64;
    maxSpeed=player.crouching?280:650;
    player.wallBlocked=FALSE;
    if(player.turnTimer) {
        UBYTE turnProgress=(UBYTE)(TURN_TICKS-player.turnTimer);
        static const UBYTE brakePercent[12]={100,100,96,90,82,72,60,48,36,24,12,0};
        static const UWORD launchSpeed[8]={25,60,110,170,240,320,410,500};
        if(turnProgress<12) {
            /* A deterministic Flashback-style run-out: visible old-direction
               travel, then a clean planted zero at the centre of the pivot. */
            player.vx=(player.turnStartVx*brakePercent[turnProgress])/100;
        } else if(turnProgress<20) {
            /* The authored pose has already rotated toward the new direction,
               but inertia still carries Sparkpaw about five pixels along the
               old run direction.  The final tick plants before launch. */
            static const UWORD skidSpeed[8]={280,250,220,190,155,110,60,0};
            LONG skid=(LONG)skidSpeed[turnProgress-12];
            player.vx=player.turnStartVx<0?-skid:skid;
        } else {
            /* Then accelerate visibly in the new direction over two poses. */
            LONG launch=(LONG)launchSpeed[turnProgress-20];
            player.vx=player.turnTargetLeft?-launch:launch;
        }
        if(player.vx>-16&&player.vx<16) player.vx=0;
    }
    else if(left&&!right) {
        if(player.grounded&&!player.crouching&&player.vx>70) {
            player.turnTimer=TURN_TICKS; player.turnTargetLeft=TRUE;
            player.turnStartVx=player.vx;
            player.runTick=0; player.runFrame=0;
        } else {
            player.vx-=acceleration; if(player.vx< -maxSpeed) player.vx=-maxSpeed;
            player.facingLeft=TRUE;
        }
    }
    else if(right&&!left) {
        if(player.grounded&&!player.crouching&&player.vx< -70) {
            player.turnTimer=TURN_TICKS; player.turnTargetLeft=FALSE;
            player.turnStartVx=player.vx;
            player.runTick=0; player.runFrame=0;
        } else {
            player.vx+=acceleration; if(player.vx>maxSpeed) player.vx=maxSpeed;
            player.facingLeft=FALSE;
        }
    }
    else { player.vx=(player.vx*205)>>8; if(player.vx>-18&&player.vx<18) player.vx=0; }
    if(jump&&player.grounded&&canStand(px,py)) {
        player.crouching=FALSE; player.turnTimer=0;
        player.vy=-1300; player.grounded=FALSE;
    }
    player.vy+=55; if(player.vy>1050) player.vy=1050;
    moveX(player.vx); moveY(player.vy);
    if(player.grounded&&!player.crouching&&!player.turnTimer&&
       player.vx>-20&&player.vx<20) {
        if(player.idleTicks<65535) player.idleTicks++;
    } else player.idleTicks=0;
}

static void camera(void)
{
    LONG px=player.x>>8,wanted=cameraX;
    if(px-cameraX>202) wanted=px-202; if(px-cameraX<105) wanted=px-105;
    if(wanted<0) wanted=0; if(wanted>WORLD_W-SCREEN_W) wanted=WORLD_W-SCREEN_W;
    if(wanted>cameraX+5) cameraX+=5; else if(wanted<cameraX-5) cameraX-=5; else cameraX=wanted;
}

static BOOL loadData(void)
{
    return loadAsset("PROGDIR:assets/runtime/storm-front.spbm",&frontClean,3)&&
           loadAsset("PROGDIR:assets/runtime/storm-rear.spbm",&rearWorld,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-sprites4.spbm",&sprites,4)&&
           loadAsset("PROGDIR:assets/runtime/clockwork-beetle.spbm",&enemySprites,3)&&
           collisionLoad()&&loadShotSample();
}

static BOOL prepare(void)
{
    UBYTE p;
    frontDisplay=AllocBitMap(WORLD_W,WORLD_H,3,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    cop=(UWORD *)AllocMem(COP_WORDS*2,MEMF_CHIP|MEMF_CLEAR);
    if(!frontDisplay||!cop||!buildHardwareSprites()) return FALSE;
    for(p=0;p<3;p++) CopyMem(frontClean.bitmap->Planes[p],frontDisplay->Planes[p],
                             (LONG)frontDisplay->BytesPerRow*WORLD_H);
    if(!buildEnemyPatterns()||!buildPlasmaPatterns()) return FALSE;
    enemiesInit(); projectilesInit();
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
}

static void restoreSystem(void)
{
    if(interruptsDisabled) {
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
    if(shotSample) FreeMem(shotSample,shotSampleBytes);
    if(frontDisplay) FreeBitMap(frontDisplay);
    freeAsset(&sprites); freeAsset(&rearWorld); freeAsset(&frontClean);
    freeAsset(&enemySprites);
    if(GfxBase) CloseLibrary((struct Library *)GfxBase);
}

int main(void)
{
    BOOL left,right,down,jump,fire,wasGrounded;
    GfxBase=(struct GfxBase *)OpenLibrary("graphics.library",39);
    memset(&player,0,sizeof(player)); player.x=36L<<8; player.y=164L<<8;
    if(!GfxBase||!loadData()||!prepare()) { PutStr("Sparkpaw: runtime assets or Chip RAM unavailable.\n"); cleanup(); return 10; }
    setHardwareSprite();
    takeover();
    for(;;) {
        while(rasterLine()<100) { }
        joystick(&left,&right,&down,&jump,&fire); wasGrounded=player.grounded;
        startShot(fire); physics(left,right,down,jump); updateShot();
        enemiesUpdate(frameCounter,collisionSolidAt);
        projectilesUpdate((WORD)cameraX,collisionSolidAt,enemiesHitProjectile);
        updateAudio(); camera(); animatePlayer(!wasGrounded&&player.grounded);
        frameCounter++;
          /* The Copper consumes these list entries at frame start. Update them
              well after that read and well before the next wrap, independent of
              how long the post-display Bob pass takes. */
          setHardwareSprite(); setScroll(cameraX,cameraX>>2);
        while(rasterLine()<300) { }
        eraseProjectileBobs(); restoreEnemyBob();
        drawEnemyBob(); drawProjectileBobs();
        waitPrivateBlit();
        while(rasterLine()>=300) { }
    }
    return 0;
}
