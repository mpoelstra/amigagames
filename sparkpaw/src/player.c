#include "player.h"

#include "collision.h"
#include "platform_amiga.h"
#include "projectiles.h"

#include <string.h>

#define FIX_SHIFT 8
#define HIT_LEFT 4
#define HIT_RIGHT 27
#define HIT_TOP 5
#define STAND_VISUAL_TOP 0
#define HIT_CROUCH_TOP 19
#define HIT_BOTTOM 38
#define IDLE_ACT_DELAY 100
#define LEDGE_ACT_DELAY 10
#define LEDGE_MAX_SUPPORT 10
#define MIN_GROUND_SUPPORT 4
#define LAND_TICKS 10
#define TURN_TICKS 28
#define CROUCH_SHOT_TICKS 11
#define CONTACT_INVULN_TICKS 60
#define CONTACT_HURT_TICKS 12
#define CONTACT_KNOCKBACK_X 700
#define CONTACT_KNOCKBACK_Y -500

static struct PlayerState player;
static BOOL jumpInputHeld,joystickFireHeld,crouchInputHeld;

static UBYTE groundSupportCount(WORD x,WORD y,UBYTE *leftCount)
{
    WORD probe; UBYTE total=0,left=0;
    for(probe=HIT_LEFT;probe<=HIT_RIGHT;probe++)
        if(collisionSolidAt(x+probe,y)) {
            total++;
            if(probe<(HIT_LEFT+HIT_RIGHT+1)/2) left++;
        }
    if(leftCount) *leftCount=left;
    return total;
}

static BOOL ledgeSideClear(WORD x,WORD y,BOOL missingLeft)
{
    WORD offset,side;
    for(offset=1;offset<=3;offset++) {
        side=x+(missingLeft?HIT_LEFT-offset:HIT_RIGHT+offset);
        if(collisionSolidVertical(side,y+HIT_TOP,y+HIT_BOTTOM)) return FALSE;
    }
    return TRUE;
}

void playerInit(void)
{
    memset(&player,0,sizeof(player));
    crouchInputHeld=FALSE;
    player.x=36L<<8; player.y=156L<<8;
    player.health=PLAYER_MAX_HEALTH;
}

void playerReadInput(BOOL *left,BOOL *right,BOOL *down,BOOL *jump,BOOL *fire)
{
    UWORD value=*(volatile UWORD *)0xdff00c; BOOL up,held;
    BOOL keyLeft,keyRight,keyDown,keyJump,keyFire;
    platformReadGameKeys(&keyLeft,&keyRight,&keyDown,&keyJump,&keyFire);
    *left=((value&0x0200)!=0)||keyLeft;
    *right=((value&0x0002)!=0)||keyRight;
    *down=(((value^(value>>1))&0x0001)!=0)||keyDown;
    up=((value^(value>>1))&0x0100)!=0;
    up=up||keyJump||platformSecondaryButtonHeld();
    *jump=up&&!jumpInputHeld; jumpInputHeld=up;
    held=(*(volatile UBYTE *)0xbfe001&0x80)==0;
    held=held||keyFire;
    *fire=held&&!joystickFireHeld; joystickFireHeld=held;
}

void playerStartShot(BOOL pressed,PlayerPlayShot playShot)
{
    if(pressed&&!player.shootCooldown&&!player.turnTimer&&!player.hurtTimer) {
        player.shootTimer=player.crouching?CROUCH_SHOT_TICKS:
                          (player.grounded?7:10);
        player.shootCooldown=3; player.shotPending=FALSE;
        player.idleTicks=0;
        projectilesSpawn((WORD)(player.x>>8),(WORD)(player.y>>8),
                         player.facingLeft,player.crouching,!player.grounded,
                         playShot);
    }
}

void playerUpdateShot(void)
{
    if(player.shootCooldown) player.shootCooldown--;
    if(player.shootTimer) player.shootTimer--;
}

void playerAnimate(BOOL landed,LONG frameCounter)
{
    /* A crawl-height recoil lands directly into its low recovery. Queuing the
       ordinary landing family here would reveal frames 14-16 only after hurt
       ends, producing a brief impossible standing pose under platforms. */
    if(landed&&!player.crouching&&!player.hurtCrouched)
        player.landTimer=LAND_TICKS;
    if(player.hurtTimer) {
        UBYTE progress=(UBYTE)(CONTACT_HURT_TICKS-player.hurtTimer);
        UBYTE base=player.hurtCrouched?54:50;
        if(player.hurtCrouched) player.landTimer=0;
        if(progress<2) player.animFrame=base;
        else if(progress<5) player.animFrame=(UBYTE)(base+1);
        else if(progress<8) player.animFrame=(UBYTE)(base+2);
        else player.animFrame=(UBYTE)(base+3);
        return;
    }
    /* A low contact recoil stays compact until physics has registered its
       landing; do not pop directly into a standing-height airborne pose. */
    if(player.hurtCrouched) { player.animFrame=57; return; }
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
        /* Do not cycle stretched run poses while pressing into a wall. */
        player.runTick=0; player.runFrame=0; player.animFrame=5; return;
    }
    if(player.vx>70||player.vx< -70) {
        LONG speed=player.vx<0?-player.vx:player.vx;
        /* About 12-13 animation changes/sec keeps all eight phases readable. */
        player.runTick+=(UWORD)(speed/10);
        while(player.runTick>=256) {
            player.runTick-=256; player.runFrame=(UBYTE)((player.runFrame+1)&7);
        }
        player.animFrame=(UBYTE)(2+player.runFrame); return;
    }
    player.runTick=0; player.runFrame=0;
    if(player.idleTicks>=LEDGE_ACT_DELAY&&!player.shootTimer) {
        WORD x=(WORD)(player.x>>FIX_SHIFT);
        WORD y=(WORD)(player.y>>FIX_SHIFT)+HIT_BOTTOM+1;
        UBYTE leftSupport,totalSupport=groundSupportCount(x,y,&leftSupport);
        BOOL missingLeft=leftSupport<totalSupport-leftSupport;
        if(totalSupport>=MIN_GROUND_SUPPORT&&
           totalSupport<=LEDGE_MAX_SUPPORT&&
           ledgeSideClear(x,(WORD)(player.y>>FIX_SHIFT),missingLeft)) {
            UWORD phase=(UWORD)((player.idleTicks-LEDGE_ACT_DELAY)&31);
            static const UBYTE frames[4]={58,59,60,61};
            /* Face the unsupported side. The source family is authored right
               and the existing hardware cache supplies its exact mirror. */
            player.facingLeft=missingLeft;
            player.animFrame=frames[phase>>3];
            return;
        }
    }
    if(player.idleTicks>=IDLE_ACT_DELAY) {
        /* Keep the accepted acknowledgement sequence; the removed tail-sway
           shifted the silhouette sideways and read as a moonwalk. */
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
    WORD positionY;
    /* Standing art reaches five pixels above its deliberately forgiving
       gameplay hitbox. Clearance must cover that visible silhouette too. */
    for(positionY=y+STAND_VISUAL_TOP;
        positionY<y+HIT_CROUCH_TOP;positionY++)
        if(collisionSolidHorizontal(x+HIT_LEFT,x+HIT_RIGHT,positionY))
            return FALSE;
    return TRUE;
}

static void moveX(LONG delta)
{
    LONG target=player.x+delta; WORD x=(WORD)(player.x>>FIX_SHIFT);
    WORD end=(WORD)(target>>FIX_SHIFT),y=(WORD)(player.y>>FIX_SHIFT);
    WORD direction=delta<0?-1:1;
    if(delta&&collisionSolidVertical(x+(direction<0?HIT_LEFT-1:HIT_RIGHT+1),
                                     y+playerHitTop(),y+HIT_BOTTOM)) {
        player.wallBlocked=TRUE; player.vx=0; player.x=(LONG)x<<8; return;
    }
    while(x!=end) {
        WORD next=x+direction;
        WORD side=next+(direction<0?HIT_LEFT:HIT_RIGHT);
        if(collisionSolidVertical(side,y+playerHitTop(),y+HIT_BOTTOM)) {
            player.wallBlocked=TRUE; player.vx=0; player.x=(LONG)x<<8; return;
        }
        x=next;
    }
    player.x=target;
}

static void moveY(LONG delta)
{
    LONG target=player.y+delta; WORD x=(WORD)(player.x>>FIX_SHIFT);
    WORD y=(WORD)(player.y>>FIX_SHIFT),end=(WORD)(target>>FIX_SHIFT);
    WORD direction=delta<0?-1:1;
    player.grounded=FALSE;
    while(y!=end) {
        WORD next=y+direction;
        WORD edge=next+(direction<0?playerHitTop():HIT_BOTTOM);
        if(collisionSolidHorizontal(x+HIT_LEFT,x+HIT_RIGHT,edge)) {
            UBYTE leftSupport=0;
            UBYTE totalSupport=direction>0?
                groundSupportCount(x,edge,&leftSupport):MIN_GROUND_SUPPORT;
            /* A last 1..3 pixels of horizontal overlap are not a landing.
               Let downward movement pass the ledge; upward head collision
               retains the established any-pixel blocking contract. */
            if(direction<0||totalSupport>=MIN_GROUND_SUPPORT) {
                if(direction>0) player.grounded=TRUE;
                player.vy=0; player.y=(LONG)y<<8; return;
            }
            /* Clear exactly that residual overlap before descending. Without
               this, the actor lands below while embedded in the ledge's side
               and canStand() correctly—but confusingly—rejects the next jump. */
            if(totalSupport&&leftSupport==totalSupport) x+=totalSupport;
            else if(totalSupport&&!leftSupport) x-=totalSupport;
            player.x=(LONG)x<<FIX_SHIFT;
        }
        y=next;
    }
    player.y=target;
    if(groundSupportCount(x,y+HIT_BOTTOM+1,NULL)>=MIN_GROUND_SUPPORT)
        player.grounded=TRUE;
}

BOOL playerUpdatePhysics(BOOL left,BOOL right,BOOL down,BOOL jump)
{
    WORD playerX=(WORD)(player.x>>FIX_SHIFT),playerY=(WORD)(player.y>>FIX_SHIFT);
    LONG acceleration,maxSpeed;
    BOOL jumped=FALSE;
    /* Preserve current crouch intent even when hurt physics returns before
       the ordinary grounded-input branch updates player.crouching. */
    crouchInputHeld=down;
    if(player.invulnTimer) player.invulnTimer--;
    if(player.hurtTimer) {
        player.hurtTimer--;
        /* A contact hit must not force the standing collision box under a low
           ceiling. Recovery will leave crouch only through canStand(). */
        player.turnTimer=0; player.turnFinishing=FALSE;
        player.wallBlocked=FALSE;
        player.vx=(player.vx*235)>>8;
        if(player.vx>-18&&player.vx<18) player.vx=0;
        player.vy+=55; if(player.vy>1050) player.vy=1050;
        moveX(player.vx); moveY(player.vy);
        player.idleTicks=0;
        return FALSE;
    }
    if(player.hurtCrouched&&player.grounded) player.hurtCrouched=FALSE;
    if(player.turnFinishing) {
        player.facingLeft=player.turnTargetLeft;
        player.turnFinishing=FALSE;
    }
    if(player.grounded&&down) { player.crouching=TRUE; player.turnTimer=0; }
    else if(player.crouching&&!down&&!player.hurtCrouched&&
            canStand(playerX,playerY))
        player.crouching=FALSE;
    if(!player.grounded&&!player.hurtCrouched&&canStand(playerX,playerY))
        player.crouching=FALSE;
    acceleration=player.crouching?38:64;
    maxSpeed=player.crouching?280:650;
    player.wallBlocked=FALSE;
    if(player.turnTimer) {
        UBYTE turnProgress=(UBYTE)(TURN_TICKS-player.turnTimer);
        static const UBYTE brakePercent[12]={100,100,96,90,82,72,60,48,36,24,12,0};
        static const UWORD launchSpeed[8]={25,60,110,170,240,320,410,500};
        /* Deterministic run-out, planted pivot, then new-direction launch. */
        if(turnProgress<12)
            player.vx=(player.turnStartVx*brakePercent[turnProgress])/100;
        else if(turnProgress<20) {
            static const UWORD skidSpeed[8]={280,250,220,190,155,110,60,0};
            LONG skid=(LONG)skidSpeed[turnProgress-12];
            player.vx=player.turnStartVx<0?-skid:skid;
        } else {
            LONG launch=(LONG)launchSpeed[turnProgress-20];
            player.vx=player.turnTargetLeft?-launch:launch;
        }
        if(player.vx>-16&&player.vx<16) player.vx=0;
    } else if(left&&!right) {
        if(player.grounded&&!player.crouching&&player.vx>70) {
            player.turnTimer=TURN_TICKS; player.turnTargetLeft=TRUE;
            player.turnStartVx=player.vx;
            player.runTick=0; player.runFrame=0;
        } else {
            player.vx-=acceleration; if(player.vx< -maxSpeed) player.vx=-maxSpeed;
            player.facingLeft=TRUE;
        }
    } else if(right&&!left) {
        if(player.grounded&&!player.crouching&&player.vx< -70) {
            player.turnTimer=TURN_TICKS; player.turnTargetLeft=FALSE;
            player.turnStartVx=player.vx;
            player.runTick=0; player.runFrame=0;
        } else {
            player.vx+=acceleration; if(player.vx>maxSpeed) player.vx=maxSpeed;
            player.facingLeft=FALSE;
        }
    } else {
        player.vx=(player.vx*205)>>8;
        if(player.vx>-18&&player.vx<18) player.vx=0;
    }
    if(jump&&player.grounded&&canStand(playerX,playerY)) {
        player.crouching=FALSE; player.turnTimer=0;
        player.vy=-1300; player.grounded=FALSE;
        jumped=TRUE;
    }
    player.vy+=55; if(player.vy>1050) player.vy=1050;
    moveX(player.vx); moveY(player.vy);
    if(player.grounded&&!player.crouching&&!player.turnTimer&&
       player.vx>-20&&player.vx<20) {
        if(player.idleTicks<65535) player.idleTicks++;
    } else player.idleTicks=0;
    return jumped;
}

void playerContactBounds(WORD *left,WORD *top,WORD *right,WORD *bottom)
{
    WORD x=(WORD)(player.x>>FIX_SHIFT),y=(WORD)(player.y>>FIX_SHIFT);
    /* Contact damage owns a deliberately narrower torso/leg box than solid
       tile collision.  Neither box is derived from transparent sprite art. */
    *left=x+6; *right=x+25;
    *top=y+(player.crouching?20:7); *bottom=y+38;
}

BOOL playerReachedWorldRight(WORD worldRight)
{
    WORD x=(WORD)(player.x>>FIX_SHIFT);
    /* Use the proven solid collision edge. The narrower contact-damage box
       stops two pixels short and can therefore never reach the world wall. */
    return x+HIT_RIGHT>=worldRight;
}

static BOOL playerShowsLowPose(void)
{
    UBYTE frame=player.animFrame;
    return (frame>=17&&frame<=19)||
           (frame>=46&&frame<=49)||
           (frame>=54&&frame<=57);
}

BOOL playerTakeEnemyHit(WORD enemyCenterX)
{
    WORD centerX=(WORD)(player.x>>FIX_SHIFT)+(PLAYER_W>>1);
    WORD x=(WORD)(player.x>>FIX_SHIFT),y=(WORD)(player.y>>FIX_SHIFT);
    BOOL keepCrouched;
    if(!player.health) return FALSE;
    if(player.invulnTimer) return FALSE;
    /* Contact is resolved after physics but before this frame's animation.
       Preserve the visible low posture as well as the logical flag, otherwise
       a one-frame landing/input transition can select the tall hurt family. */
    keepCrouched=crouchInputHeld||player.crouching||player.hurtCrouched||
                  playerShowsLowPose()||!canStand(x,y);
    player.health--;
    player.invulnTimer=CONTACT_INVULN_TICKS;
    player.hurtTimer=CONTACT_HURT_TICKS;
    player.shootTimer=0; player.shotPending=FALSE;
    player.crouching=keepCrouched; player.landTimer=0;
    player.hurtCrouched=keepCrouched;
    player.turnTimer=0; player.turnFinishing=FALSE;
    player.vx=centerX<enemyCenterX?-CONTACT_KNOCKBACK_X:CONTACT_KNOCKBACK_X;
    player.vy=keepCrouched?(CONTACT_KNOCKBACK_Y/2):CONTACT_KNOCKBACK_Y;
    player.grounded=FALSE;
    player.idleTicks=0;
    return TRUE;
}

const struct PlayerState *playerState(void)
{
    return &player;
}
