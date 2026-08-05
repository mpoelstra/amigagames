#include "player.h"

#include "collision.h"
#include "projectiles.h"

#include <string.h>

#define FIX_SHIFT 8
#define HIT_LEFT 4
#define HIT_RIGHT 27
#define HIT_TOP 5
#define HIT_CROUCH_TOP 19
#define HIT_BOTTOM 38
#define IDLE_ACT_DELAY 100
#define LAND_TICKS 10
#define TURN_TICKS 28
#define CROUCH_SHOT_TICKS 11

static struct PlayerState player;
static BOOL joystickUpHeld,joystickFireHeld;

void playerInit(void)
{
    memset(&player,0,sizeof(player));
    player.x=36L<<8; player.y=164L<<8;
}

void playerReadInput(BOOL *left,BOOL *right,BOOL *down,BOOL *jump,BOOL *fire)
{
    UWORD value=*(volatile UWORD *)0xdff00c; BOOL up,held;
    *left=(value&0x0200)!=0; *right=(value&0x0002)!=0;
    *down=((value^(value>>1))&0x0001)!=0;
    up=((value^(value>>1))&0x0100)!=0;
    *jump=up&&!joystickUpHeld; joystickUpHeld=up;
    held=(*(volatile UBYTE *)0xbfe001&0x80)==0;
    *fire=held&&!joystickFireHeld; joystickFireHeld=held;
}

void playerStartShot(BOOL pressed,PlayerPlayShot playShot)
{
    if(pressed&&!player.shootCooldown&&!player.turnTimer) {
        player.shootTimer=player.crouching?CROUCH_SHOT_TICKS:
                          (player.grounded?7:10);
        player.shootCooldown=3; player.shotPending=FALSE;
        player.idleTicks=0;
        projectilesSpawn((WORD)(player.x>>8),(WORD)(player.y>>8),
                         player.facingLeft,player.crouching,playShot);
    }
}

void playerUpdateShot(void)
{
    if(player.shootCooldown) player.shootCooldown--;
    if(player.shootTimer) player.shootTimer--;
}

void playerAnimate(BOOL landed,LONG frameCounter)
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
    for(positionY=y+HIT_TOP;positionY<y+HIT_CROUCH_TOP;positionY++)
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
            if(direction>0) player.grounded=TRUE;
            player.vy=0; player.y=(LONG)y<<8; return;
        }
        y=next;
    }
    player.y=target;
    if(collisionSolidHorizontal(x+HIT_LEFT,x+HIT_RIGHT,y+HIT_BOTTOM+1))
        player.grounded=TRUE;
}

void playerUpdatePhysics(BOOL left,BOOL right,BOOL down,BOOL jump)
{
    WORD playerX=(WORD)(player.x>>FIX_SHIFT),playerY=(WORD)(player.y>>FIX_SHIFT);
    LONG acceleration,maxSpeed;
    if(player.turnFinishing) {
        player.facingLeft=player.turnTargetLeft;
        player.turnFinishing=FALSE;
    }
    if(player.grounded&&down) { player.crouching=TRUE; player.turnTimer=0; }
    else if(player.crouching&&!down&&canStand(playerX,playerY))
        player.crouching=FALSE;
    if(!player.grounded&&canStand(playerX,playerY)) player.crouching=FALSE;
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
    }
    player.vy+=55; if(player.vy>1050) player.vy=1050;
    moveX(player.vx); moveY(player.vy);
    if(player.grounded&&!player.crouching&&!player.turnTimer&&
       player.vx>-20&&player.vx<20) {
        if(player.idleTicks<65535) player.idleTicks++;
    } else player.idleTicks=0;
}

const struct PlayerState *playerState(void)
{
    return &player;
}