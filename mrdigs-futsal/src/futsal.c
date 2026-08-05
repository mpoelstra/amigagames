/*
 * MrDig's Futsal -- first playable Amiga prototype.
 * One-screen 3-vs-3 indoor football with wall rebounds.
 */
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <devices/inputevent.h>
#include <dos/dos.h>
#include <intuition/intuition.h>
#include <intuition/screens.h>
#include <graphics/gfxbase.h>
#include <graphics/displayinfo.h>
#include <graphics/rastport.h>
#include <utility/tagitem.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <stdio.h>
#include <string.h>
#include "modplayer.h"
#include "sfxplayer.h"

#define W 320
#define H 256
#define SCALE 16
#define LEFT 12
#define RIGHT 307
#define TOP 34
#define BOTTOM 243
#define GOAL_TOP 105
#define GOAL_BOTTOM 165
#define MODE_TITLE 0
#define MODE_GAME 1
#define MODE_WIN 2
#define MODE_OPTIONS 3
#define TEAM_BLUE 0
#define TEAM_RED 1
#define FIELD_PLAYERS 6
#define MAX_PLAYERS 8
#define BLUE_KEEPER 6
#define RED_KEEPER 7
#define RAW_ESC 0x45
#define RAW_SPACE 0x40
#define RAW_TAB 0x42
#define RAW_P 0x19
#define RAW_M 0x37
#define RAW_W 0x11
#define RAW_A 0x20
#define RAW_S 0x21
#define RAW_D 0x22
#define RAW_UP 0x4c
#define RAW_DOWN 0x4d
#define RAW_RIGHT 0x4e
#define RAW_LEFT 0x4f
#define RAW_MINUS 0x0b
#define RAW_PLUS 0x0c
#define RAW_NP_MINUS 0x4a
#define RAW_NP_PLUS 0x5e
#define RAW_1 0x01
#define RAW_2 0x02
#define RAW_3 0x03
#define RAW_4 0x04
#define RAW_7 0x07
#define RAW_8 0x08
#define RAW_9 0x09
#define PASS_TAP_FRAMES 4
#define MAX_SHOT_CHARGE 16
#define MAX_POSSESSION_AGE 100
#define GAME_SPEED_BASE 360
#define TITLE_MARQUEE_LEFT 36
#define TITLE_MARQUEE_RIGHT 283
#define TITLE_MARQUEE_TOP 197
#define TITLE_MARQUEE_BOTTOM 243
#define TITLE_MARQUEE_BASELINE 224
#define DIR_UP 0
#define DIR_UP_RIGHT 1
#define DIR_RIGHT 2
#define DIR_DOWN_RIGHT 3
#define DIR_DOWN 4
#define DIR_DOWN_LEFT 5
#define DIR_LEFT 6
#define DIR_UP_LEFT 7

struct Body { LONG x,y,vx,vy; UBYTE team,facing; };
struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;
static struct Screen *screen;
static struct Window *window;
static struct RastPort *display, frameRp, arenaRp;
static struct BitMap frameBuffers[2], *frame, *systemBitmap, title, arena;
static BOOL frameReady, titleReady, arenaReady, titleLoaded, optionsLoaded;
static WORD frameIndex;
static UWORD *hiddenPointer;
static struct Body player[MAX_PLAYERS], ball;
static UBYTE keys[128];
static WORD mode, selected, blueScore, redScore, goalPause;
static WORD redSelected=3;
static WORD ballTouchCooldown;
static WORD ballOwner=-1;
static WORD possessionProtection;
static WORD possessionAge;
static WORD autoSwitchCooldown;
static WORD redAutoSwitchCooldown;
static UBYTE possessionFacing;
static WORD spaceCharge;
static WORD redSpaceCharge;
static WORD gameSpeed=100;
static WORD optionGameSpeed=100;
static WORD speedMessageFrames;
static WORD musicMessageFrames;
static WORD floorMessageFrames;
static WORD floorStyle;
static WORD aiMessageFrames;
static WORD aiDifficulty=1;
static WORD playerCount=1;
static WORD teamSize=3;
static WORD keeperHoldFrames;
static WORD keeperReleaseFrames;
static LONG keeperOutletX,keeperOutletY;
static WORD keeperKickFrames;
static WORD keeperReleaseImmunity;
static WORD keeperLastKicker=-1;
static WORD wallShotFrames;
static LONG keeperAimY[2];
static WORD goalLimit=5;
static WORD timeLimitMinutes=3;
static WORD optionRow;
static BOOL optionMusic;
static BOOL paused;
static BOOL spaceCharging;
static BOOL redSpaceCharging;
static BOOL ingameMusicMuted;
static BOOL joystickLeft,joystickRight,joystickUp,joystickDown;
static BOOL joystickFire;
static BOOL redJoystickLeft,redJoystickRight,redJoystickUp,redJoystickDown;
static BOOL redJoystickFire;
static BOOL menuJoystickUp,menuJoystickDown,menuJoystickLeft,menuJoystickRight;
static ULONG oldCacheState;
static BOOL cacheStateSaved;
static ULONG gameFrames, randomState=0x68020120;
static ULONG matchElapsedMicros,matchClockSeconds,matchClockMicros;
static BOOL quit;

static const char titleMarquee[] =
    "*** 3 VS 3 OR 4 VS 4 *** 1 OR 2 PLAYERS *** JOYSTICKS IN PORTS 1 AND 2 *** "
    "P1 FIRE OR SPACE FOR MATCH SETUP *** "
    "ESC TO QUIT *** "
    "1 2 3 4 CHANGE COURT *** 7 8 9 AI EASY MEDIUM HARD *** "
    "+ AND - CHANGE GAME SPEED *** M TOGGLE IN-GAME MUSIC *** "
    "P PAUSE *** TAB SWITCH PLAYER *** "
    "TAP SPACE TO PASS - HOLD SPACE TO SHOOT *** "
    "MRDIG PRODUCTIONS *** COPYRIGHT 2026 *** "
    "THIS GAME IS MADE BY 100% AI ***";

static LONG absolute(LONG v) { return v < 0 ? -v : v; }
static LONG clamp(LONG v,LONG lo,LONG hi) { return v<lo?lo:(v>hi?hi:v); }
static LONG rnd(void) { randomState=randomState*1103515245UL+12345; return randomState; }
static BOOL isKeeper(WORD index)
{ return index==BLUE_KEEPER||index==RED_KEEPER; }
static BOOL playerActive(WORD index)
{ return index>=0&&index<FIELD_PLAYERS||
         (teamSize==4&&index>=FIELD_PLAYERS&&index<MAX_PLAYERS); }
static UBYTE directionFromDelta(LONG x,LONG y)
{
    LONG ax=absolute(x),ay=absolute(y);
    if(ax>ay*2)return x>0?DIR_RIGHT:DIR_LEFT;
    if(ay>ax*2)return y>0?DIR_DOWN:DIR_UP;
    if(x>0)return y>0?DIR_DOWN_RIGHT:DIR_UP_RIGHT;
    return y>0?DIR_DOWN_LEFT:DIR_UP_LEFT;
}
static void limitVelocity(LONG *x,LONG *y,LONG maximum)
{
    LONG ax=absolute(*x),ay=absolute(*y),length;
    /* Fast integer approximation of sqrt(x*x+y*y), within a few percent. */
    length=(ax>ay)?ax+ay*3/8:ay+ax*3/8;
    if(length>maximum) {
        *x=*x*maximum/length;
        *y=*y*maximum/length;
    }
}

static BOOL allocBitmap(struct BitMap *bm)
{
    WORD p;
    InitBitMap(bm,4,W,H);
    for (p=0;p<4;++p) {
        bm->Planes[p]=AllocRaster(W,H);
        if (!bm->Planes[p]) {
            while (--p>=0) FreeRaster(bm->Planes[p],W,H);
            return FALSE;
        }
    }
    return TRUE;
}
static void freeBitmap(struct BitMap *bm)
{
    WORD p;
    for(p=0;p<4;++p) if(bm->Planes[p]) {
        FreeRaster(bm->Planes[p],W,H); bm->Planes[p]=NULL;
    }
}
static ULONG be32(UBYTE *p) {
    return ((ULONG)p[0]<<24)|((ULONG)p[1]<<16)|((ULONG)p[2]<<8)|p[3];
}
static BOOL exact(BPTR f,APTR p,LONG n) { return Read(f,p,n)==n; }

static BOOL loadILBM(const char *name,struct BitMap *bm)
{
    BPTR f; UBYTE id[4],head[20],colors[48]; ULONG size,used;
    WORD row,plane,pen; BOOL header=FALSE,body=FALSE; LONG skip;
    f=Open((STRPTR)name,MODE_OLDFILE); if(!f) return FALSE;
    if(!exact(f,id,4)||memcmp(id,"FORM",4)||!exact(f,id,4)||
       !exact(f,id,4)||memcmp(id,"ILBM",4)){Close(f);return FALSE;}
    while(exact(f,id,4)&&exact(f,&size,4)) {
        used=0;
        if(!memcmp(id,"BMHD",4)&&size>=20) {
            if(!exact(f,head,20)) break; used=20;
            header=((((UWORD)head[0]<<8)|head[1])==W &&
                    (((UWORD)head[2]<<8)|head[3])==H &&
                    head[8]==4 && head[10]==0);
        } else if(!memcmp(id,"CMAP",4)&&size>=48) {
            if(!exact(f,colors,48)) break; used=48;
            for(pen=0;pen<16;++pen) SetRGB4(&screen->ViewPort,pen,
                colors[pen*3]>>4,colors[pen*3+1]>>4,colors[pen*3+2]>>4);
        } else if(!memcmp(id,"BODY",4)&&header&&size>=40960) {
            for(row=0;row<H;++row) for(plane=0;plane<4;++plane) {
                UBYTE *dest=(UBYTE *)bm->Planes[plane]+row*bm->BytesPerRow;
                if(!exact(f,dest,W/8)){Close(f);return FALSE;} used+=W/8;
            }
            body=TRUE;
        }
        skip=(LONG)(size-used)+(size&1); if(skip>0) Seek(f,skip,OFFSET_CURRENT);
    }
    Close(f); return header&&body;
}

static void paletteGame(void)
{
    static UBYTE c[16][3]={
        {0,0,1},{0,3,2},{0,6,3},{1,9,5},{2,12,7},{5,2,1},{1,7,13},{5,10,15},
        {15,13,2},{9,1,1},{13,2,1},{15,6,2},{4,4,5},{8,8,9},{13,13,13},{15,15,15}};
    WORD i; for(i=0;i<16;++i) SetRGB4(&screen->ViewPort,i,c[i][0],c[i][1],c[i][2]);
}
static void paletteFloor(void)
{
    static const UBYTE floorColors[4][3][3]={
        {{5,2,0},{10,5,1},{14,9,3}},
        {{0,6,2},{0,9,3},{1,11,4}},
        {{2,2,3},{4,5,6},{7,8,9}},
        {{7,2,2},{3,4,6},{10,10,11}}
    };
    WORD pen;
    for(pen=0;pen<3;++pen)
        SetRGB4(&screen->ViewPort,pen+2,
            floorColors[floorStyle][pen][0],
            floorColors[floorStyle][pen][1],
            floorColors[floorStyle][pen][2]);
}
static void textCenter(struct RastPort *r,const char *s,WORD y,WORD pen)
{
    WORD width=(WORD)TextLength(r,(STRPTR)s,strlen(s));
    SetAPen(r,pen); Move(r,(W-width)/2,y); Text(r,(STRPTR)s,strlen(s));
}
static void present(void)
{
    /*
     * Synchronise the completed off-screen frame, not the start of drawing.
     * This keeps the full-screen blit ahead of the display beam even when
     * the machine was booted without Workbench/SetPatch.
     */
    WaitTOF();
    /*
     * Flip complete bitmaps instead of copying a full frame into the bitmap
     * currently being scanned out. The old copy could race the display beam
     * and made moving players appear to blink on slower/minimal boot setups.
     */
    screen->ViewPort.RasInfo->BitMap=frame;
    screen->RastPort.BitMap=frame;
    display->BitMap=frame;
    MakeScreen(screen);
    RethinkDisplay();
    frameIndex^=1;
    frame=&frameBuffers[frameIndex];
    frameRp.BitMap=frame;
}
static void drawGoalNet(struct RastPort *r,BOOL right)
{
    WORD front=right?RIGHT:LEFT;
    WORD back=right?317:2;
    WORD x,y,top,bottom;

    /* Recessed goal floor and its team-coloured back padding. */
    SetAPen(r,0);
    RectFill(r,right?RIGHT+1:0,GOAL_TOP,
        right?W-1:LEFT-1,GOAL_BOTTOM);
    SetAPen(r,right?9:6);
    RectFill(r,right?315:2,GOAL_TOP+7,
        right?318:4,GOAL_BOTTOM-7);

    /* Mesh: narrowing roof/floor and a slightly bowed horizontal weave. */
    SetAPen(r,12);
    for(x=right?310:5;right?x<=316:x<=11;x+=3) {
        top=GOAL_TOP+(absolute(x-front)*6/10);
        bottom=GOAL_BOTTOM-(absolute(x-front)*6/10);
        Move(r,x,top);Draw(r,x,bottom);
    }
    SetAPen(r,13);
    for(y=GOAL_TOP+8;y<GOAL_BOTTOM;y+=8) {
        Move(r,front,y);
        Draw(r,back,y+(139-y)/10);
    }
    Move(r,front,GOAL_TOP);Draw(r,back,GOAL_TOP+6);
    Move(r,front,GOAL_BOTTOM);Draw(r,back,GOAL_BOTTOM-6);

    /* Heavy shadow followed by bright metal posts and rear frame. */
    SetAPen(r,5);
    RectFill(r,front+(right?-1:1),GOAL_TOP+1,
        front+(right?0:2),GOAL_BOTTOM+1);
    SetAPen(r,15);
    RectFill(r,front+(right?-1:0),GOAL_TOP,
        front+(right?0:1),GOAL_BOTTOM);
    Move(r,front,GOAL_TOP);Draw(r,back,GOAL_TOP+6);
    Move(r,front,GOAL_BOTTOM);Draw(r,back,GOAL_BOTTOM-6);
    SetAPen(r,14);
    Move(r,back,GOAL_TOP+6);Draw(r,back,GOAL_BOTTOM-6);
}
static void drawGoalArc(struct RastPort *r,WORD cx,WORD cy,
                        WORD radius,BOOL opensRight,WORD pen,
                        WORD xOffset,WORD yOffset)
{
    WORD x=radius,y=0,error=1-radius;
    WORD side=opensRight?1:-1;
    SetAPen(r,pen);
    while(x>=y) {
        WritePixel(r,cx+side*x+xOffset,cy+y+yOffset);
        WritePixel(r,cx+side*x+xOffset,cy-y+yOffset);
        WritePixel(r,cx+side*y+xOffset,cy+x+yOffset);
        WritePixel(r,cx+side*y+xOffset,cy-x+yOffset);
        ++y;
        if(error<0)error+=2*y+1;
        else {--x;error+=2*(y-x)+1;}
    }
}
static void drawBoingCourtBall(struct RastPort *r,WORD cx,WORD cy,
                               WORD radius,WORD side)
{
    WORD dx,dy,patternX,cell,pen,lastPen;
    LONG distanceSquared;
    WORD inner=(WORD)(radius-2);

    /* Mirrored red/white checks evoke the original Amiga Boing Ball.  A side
       of 1 or -1 clips the sphere to the inward-facing goal semicircle. */
    for(dy=(WORD)-inner;dy<=inner;++dy) {
        lastPen=-1;
        for(dx=(WORD)-inner;dx<=inner;++dx) {
            if((side>0&&dx<0)||(side<0&&dx>0))continue;
            distanceSquared=(LONG)dx*dx+(LONG)dy*dy;
            if(distanceSquared>(LONG)inner*inner)continue;
            /* Mirror the pattern around the vertical axis.  Using signed
               division here previously made the centre and right spheres
               turn into asymmetric chevrons on 68k C. */
            patternX=absolute(dx);
            cell=(WORD)(((patternX/8)+((dy+radius)/8))&1);
            pen=cell?2:4;
            if(pen!=lastPen){SetAPen(r,pen);lastPen=pen;}
            WritePixel(r,cx+dx,cy+dy);
        }
    }
}
static void drawPitch(struct RastPort *r)
{
    WORD i,x,y;

    SetAPen(r,0); RectFill(r,0,0,W-1,H-1);

    if(floorStyle==0) {
        /* Varnished parquet with staggered board joints. */
        SetAPen(r,3);RectFill(r,LEFT,TOP,RIGHT,BOTTOM);
        for(y=TOP+4,i=0;y<BOTTOM-4;y+=8,++i) {
            SetAPen(r,2);RectFill(r,LEFT,y,RIGHT,y);
            for(x=LEFT+(i&1?20:60);x<RIGHT;x+=80)
                RectFill(r,x,y,x,y+7);
        }
    } else if(floorStyle==1) {
        /* Kick Off 2-inspired soggy turf: solid green with restrained,
           irregular fibres instead of boards or horizontal plank seams. */
        SetAPen(r,3);RectFill(r,LEFT,TOP,RIGHT,BOTTOM);
        for(y=TOP+3;y<BOTTOM-3;y+=4) {
            for(x=LEFT+3;x<RIGHT-3;x+=7) {
                if(((x*3+y*5)&15)<3) {
                    SetAPen(r,((x+y)&8)?2:4);
                    WritePixel(r,x,y);
                    if((x+y)&1)WritePixel(r,x+1,y);
                }
            }
        }
    } else if(floorStyle==2) {
        /* Interlocking dark sports-hall tiles. */
        SetAPen(r,3);RectFill(r,LEFT,TOP,RIGHT,BOTTOM);
        for(y=TOP+4,i=0;y<BOTTOM-4;y+=20,++i) {
            for(x=LEFT;x<RIGHT;x+=24) {
                SetAPen(r,((x/24+i)&1)?2:3);
                RectFill(r,x,y,x+23<RIGHT?x+23:RIGHT,y+19);
            }
        }
        SetAPen(r,4);
        for(y=TOP+4;y<BOTTOM-4;y+=20)
            RectFill(r,LEFT,y,RIGHT,y);
        for(x=LEFT;x<RIGHT;x+=24)
            RectFill(r,x,TOP+4,x,BOTTOM-4);
    } else {
        /* A subdued neutral floor keeps the players and live ball readable;
           the court markings below become the actual Boing Ball motif. */
        SetAPen(r,3);RectFill(r,LEFT,TOP,RIGHT,BOTTOM);
    }

    /* Extra lengthwise seams suit the wood and turf, not the tile court. */
    if(floorStyle!=2) {
        SetAPen(r,4);
        RectFill(r,85,TOP+4,85,BOTTOM-4);
        RectFill(r,159,TOP+4,159,BOTTOM-4);
        RectFill(r,234,TOP+4,234,BOTTOM-4);
    }

    if(floorStyle==3) {
        drawBoingCourtBall(r,160,139,40,0);
        drawBoingCourtBall(r,LEFT,135,40,1);
        drawBoingCourtBall(r,RIGHT,135,40,-1);
    }

    /* Painted markings first receive a dark one-pixel floor shadow. */
    SetAPen(r,1);
    RectFill(r,LEFT+1,TOP+5,RIGHT+1,TOP+6);
    RectFill(r,LEFT+1,BOTTOM-3,RIGHT+1,BOTTOM-2);
    RectFill(r,160,TOP+5,161,BOTTOM-3);
    DrawEllipse(r,161,139,40,40);
    drawGoalArc(r,LEFT,135,40,TRUE,1,1,1);
    drawGoalArc(r,RIGHT,135,40,FALSE,1,1,1);
    SetAPen(r,15);
    RectFill(r,LEFT,TOP+4,RIGHT,TOP+5);
    RectFill(r,LEFT,BOTTOM-4,RIGHT,BOTTOM-3);
    RectFill(r,159,TOP+4,160,BOTTOM-4);
    DrawEllipse(r,160,139,40,40);
    drawGoalArc(r,LEFT,135,40,TRUE,15,0,0);
    drawGoalArc(r,RIGHT,135,40,FALSE,15,0,0);
    RectFill(r,157,136,162,141);

    /* Far wall is slim; the near wall is deeper to imply elevation. */
    SetAPen(r,5); RectFill(r,0,TOP,W-1,TOP+3);
    SetAPen(r,13); RectFill(r,0,TOP,W-1,TOP);
    SetAPen(r,12); RectFill(r,0,TOP+1,W-1,TOP+2);
    SetAPen(r,5); RectFill(r,0,BOTTOM-2,W-1,H-1);
    SetAPen(r,15); RectFill(r,0,BOTTOM-3,W-1,BOTTOM-2);
    SetAPen(r,12); RectFill(r,0,BOTTOM,W-1,BOTTOM+4);
    SetAPen(r,0); RectFill(r,0,BOTTOM+5,W-1,H-1);
    SetAPen(r,5);
    for(x=16;x<W;x+=32)RectFill(r,x,BOTTOM,x+1,H-1);
    SetAPen(r,13);
    for(x=1;x<W;x+=32)RectFill(r,x,BOTTOM+1,x+13,BOTTOM+1);

    /* Raised side boards, broken at the goal mouths. */
    SetAPen(r,5);
    RectFill(r,0,TOP+4,LEFT-1,GOAL_TOP-1);
    RectFill(r,0,GOAL_BOTTOM+1,LEFT-1,BOTTOM-4);
    RectFill(r,RIGHT+1,TOP+4,W-1,GOAL_TOP-1);
    RectFill(r,RIGHT+1,GOAL_BOTTOM+1,W-1,BOTTOM-4);
    SetAPen(r,12);
    RectFill(r,2,TOP+4,LEFT-2,GOAL_TOP-2);
    RectFill(r,2,GOAL_BOTTOM+2,LEFT-2,BOTTOM-5);
    RectFill(r,RIGHT+2,TOP+4,W-3,GOAL_TOP-2);
    RectFill(r,RIGHT+2,GOAL_BOTTOM+2,W-3,BOTTOM-5);
    SetAPen(r,13);
    RectFill(r,LEFT-2,TOP+4,LEFT-1,GOAL_TOP-1);
    RectFill(r,LEFT-2,GOAL_BOTTOM+1,LEFT-1,BOTTOM-4);
    RectFill(r,RIGHT+1,TOP+4,RIGHT+2,GOAL_TOP-1);
    RectFill(r,RIGHT+1,GOAL_BOTTOM+1,RIGHT+2,BOTTOM-4);
    SetAPen(r,5);
    for(y=TOP+18;y<GOAL_TOP;y+=18) {
        RectFill(r,2,y,LEFT-2,y+1);
        RectFill(r,RIGHT+2,y,W-3,y+1);
    }
    for(y=GOAL_BOTTOM+18;y<BOTTOM-4;y+=18) {
        RectFill(r,2,y,LEFT-2,y+1);
        RectFill(r,RIGHT+2,y,W-3,y+1);
    }

    drawGoalNet(r,FALSE);
    drawGoalNet(r,TRUE);
}
static void drawScoreboard(void)
{
    struct RastPort *r=&frameRp;
    static const UBYTE digits[10][7]={
        {14,17,19,21,25,17,14},{4,12,4,4,4,4,14},
        {14,17,1,2,4,8,31},{30,1,1,14,1,1,30},
        {2,6,10,18,31,2,2},{31,16,16,30,1,1,30},
        {14,16,16,30,17,17,14},{31,1,2,4,8,8,8},
        {14,17,17,14,17,17,14},{14,17,17,15,1,1,14}};
    WORD row,column;
    UBYTE bits;

    SetAPen(r,0); RectFill(r,0,0,W-1,33);

    /* Metal cabinet and bevel. */
    SetAPen(r,12); RectFill(r,47,1,272,22);
    SetAPen(r,13); RectFill(r,49,2,270,3);
    RectFill(r,49,3,50,20);
    SetAPen(r,5); RectFill(r,49,20,270,21);
    RectFill(r,269,3,270,20);
    SetAPen(r,0); RectFill(r,53,5,266,18);

    /* Team windows with darker lower lips. */
    SetAPen(r,0); RectFill(r,55,6,127,17);
    SetAPen(r,6); RectFill(r,55,6,127,14);
    SetAPen(r,5); RectFill(r,129,5,190,18);
    SetAPen(r,9); RectFill(r,192,6,264,17);
    SetAPen(r,10); RectFill(r,192,6,264,14);
    SetAPen(r,0); RectFill(r,106,3,125,18);
    RectFill(r,194,3,213,18);

    SetAPen(r,15); Move(r,61,15); Text(r,"BLUE",4);
    Move(r,238,15); Text(r,"RED",3);
    SetAPen(r,13); Move(r,151,15); Text(r,"VS",2);

    /* Large 5x7 score digits. */
    bits=0;
    for(row=0;row<7;++row) {
        bits=digits[blueScore%10][row];
        SetAPen(r,15);
        for(column=0;column<5;++column) if(bits&(1<<(4-column)))
            RectFill(r,111+column*2,4+row*2,112+column*2,5+row*2);
        bits=digits[redScore%10][row];
        for(column=0;column<5;++column) if(bits&(1<<(4-column)))
            RectFill(r,199+column*2,4+row*2,200+column*2,5+row*2);
    }

    /* Status lamps and timer tray. */
    SetAPen(r,8); RectFill(r,136,9,139,12);
    SetAPen(r,4); RectFill(r,180,9,183,12);
    SetAPen(r,12); RectFill(r,111,22,208,33);
    SetAPen(r,5); RectFill(r,113,24,206,33);
    SetAPen(r,0); RectFill(r,115,24,204,31);
}
static void drawPlayer(struct Body *b,WORD index)
{
    WORD x=(WORD)(b->x/SCALE),y=(WORD)(b->y/SCALE);
    struct RastPort *r=&frameRp;
    WORD moving=(absolute(b->vx)+absolute(b->vy)>8);
    WORD step=moving?(WORD)((gameFrames/5+index)&1):0;
    WORD keeper=isKeeper(index);
    WORD teamPen=keeper?8:(b->team==TEAM_BLUE?6:10);
    WORD trimPen=b->team==TEAM_BLUE?6:10;
    WORD diagonal=(b->facing&1);
    WORD lookRight=(b->facing==DIR_UP_RIGHT||
                    b->facing==DIR_RIGHT||
                    b->facing==DIR_DOWN_RIGHT);
    WORD lookLeft=(b->facing==DIR_UP_LEFT||
                   b->facing==DIR_LEFT||
                   b->facing==DIR_DOWN_LEFT);
    WORD lean=diagonal?(lookRight?2:-2):0;

    SetAPen(r,1);
    RectFill(r,x-5+lean/2,y+5,x+5+lean/2,y+6);
    RectFill(r,x-3+lean/2,y+7,x+3+lean/2,y+7);
    SetAPen(r,0);
    if(diagonal) {
        RectFill(r,x-5+lean+(step?2:0),y+2,
                 x-2+lean+(step?2:0),y+6);
        RectFill(r,x+1-(step?1:0),y+4,
                 x+5-(step?1:0),y+7);
    } else {
        RectFill(r,x-5+(step?2:0),y+3,x-1+(step?2:0),y+6);
        RectFill(r,x+1-(step?2:0),y+3,x+5-(step?2:0),y+6);
    }
    SetAPen(r,15);
    RectFill(r,x-4+lean/2,y+1,x-1+lean/2,y+3);
    RectFill(r,x+1+lean/2,y+1,x+4+lean/2,y+3);

    SetAPen(r,0); RectFill(r,x-6+lean/2,y-5,x+6+lean/2,y+2);
    SetAPen(r,teamPen); RectFill(r,x-5+lean/2,y-4,x+5+lean/2,y+1);
    RectFill(r,x-6+lean/2,y-3,x-5+lean/2,y-1);
    RectFill(r,x+5+lean/2,y-3,x+6+lean/2,y-1);
    if(keeper) {
        SetAPen(r,trimPen);
        RectFill(r,x-1+lean/2,y-4,x+1+lean/2,y+1);
        SetAPen(r,15);
        WritePixel(r,x-7+lean/2,y-1);
        WritePixel(r,x+7+lean/2,y-1);
    }
    SetAPen(r,11);
    RectFill(r,x-7+lean/2,y-2,x-6+lean/2,y+1);
    RectFill(r,x+6+lean/2,y-2,x+7+lean/2,y+1);

    SetAPen(r,0); RectFill(r,x-4+lean,y-11,x+4+lean,y-5);
    SetAPen(r,11); RectFill(r,x-3+lean,y-10,x+3+lean,y-6);
    SetAPen(r,5); RectFill(r,x-3+lean,y-11,x+3+lean,y-9);
    if(b->facing==DIR_UP) {
        SetAPen(r,5); RectFill(r,x-3,y-8,x+3,y-6);
    } else if(lookRight) {
        SetAPen(r,0); WritePixel(r,x+2+lean,y-8);
        if(diagonal)WritePixel(r,x+1+lean,y-7);
    } else if(lookLeft) {
        SetAPen(r,0); WritePixel(r,x-2+lean,y-8);
        if(diagonal)WritePixel(r,x-1+lean,y-7);
    } else {
        SetAPen(r,0); WritePixel(r,x-2,y-8); WritePixel(r,x+2,y-8);
    }
    if(index==selected||(playerCount==2&&index==redSelected)) {
        BOOL charging=index<3?spaceCharging:redSpaceCharging;
        WORD charge=index<3?spaceCharge:redSpaceCharge;
        SetAPen(r,index<3?8:10);
        RectFill(r,x-1,y-16,x+1,y-14);
        RectFill(r,x-3,y-17,x+3,y-16);
        if(charging) {
            SetAPen(r,0); RectFill(r,x-8,y-22,x+8,y-19);
            SetAPen(r,13); RectFill(r,x-7,y-21,x+7,y-20);
            SetAPen(r,charge<PASS_TAP_FRAMES?4:
                (charge<12?8:10));
            RectFill(r,x-7,y-21,
                x-7+(WORD)(14*charge/MAX_SHOT_CHARGE),y-20);
        }
    }
}
static void drawBall(void)
{
    WORD x=(WORD)(ball.x/SCALE),y=(WORD)(ball.y/SCALE);
    WORD rollFrame=(WORD)(((ball.x/SCALE)+(ball.y/SCALE))&1);
    struct RastPort *r=&frameRp;

    SetAPen(r,1); RectFill(r,x-3,y+5,x+3,y+6);
    SetAPen(r,0);
    RectFill(r,x-2,y-4,x+2,y+4);
    RectFill(r,x-4,y-2,x+4,y+2);
    RectFill(r,x-3,y-3,x+3,y+3);
    SetAPen(r,15);
    RectFill(r,x-1,y-3,x+1,y-3);
    RectFill(r,x-2,y-2,x+2,y-2);
    RectFill(r,x-3,y-1,x+3,y+1);
    RectFill(r,x-2,y+2,x+2,y+2);
    RectFill(r,x-1,y+3,x+1,y+3);
    SetAPen(r,12);
    if(rollFrame==0) {
        WritePixel(r,x-2,y-2); WritePixel(r,x+2,y-2);
        WritePixel(r,x+2,y+2); WritePixel(r,x-2,y+2);
    } else {
        WritePixel(r,x,y-3); WritePixel(r,x+3,y);
        WritePixel(r,x,y+3); WritePixel(r,x-3,y);
    }
    SetAPen(r,0);
    RectFill(r,x-1,y-1,x+1,y+1);
}
static void renderGame(void)
{
    char line[80]; WORD i;LONG total,remaining;
    BltBitMap(&arena,0,0,frame,0,0,W,H,0xc0,0xff,NULL);
    WaitBlit();
    drawScoreboard();
    total=(LONG)timeLimitMinutes*60;
    remaining=clamp(total-(LONG)(matchElapsedMicros/1000000UL),0,total);
    sprintf(line,"TIME %ld:%02ld",remaining/60,remaining%60);
    textCenter(&frameRp,line,31,15);
    for(i=0;i<MAX_PLAYERS;++i)
        if(playerActive(i))drawPlayer(&player[i],i);
    if(!goalPause)drawBall();
    if(goalPause) textCenter(&frameRp,"GOAL!",133,8);
    if(paused) {
        textCenter(&frameRp,"PAUSED",125,15);
        textCenter(&frameRp,"P CONTINUE   ESC TITLE",145,15);
    } else if(mode==MODE_WIN) {
        textCenter(&frameRp,
            blueScore>redScore?"BLUE TEAM WINS!":"RED TEAM WINS!",125,15);
        textCenter(&frameRp,"FIRE OPTIONS   ESC TITLE",145,15);
    }
    if(speedMessageFrames>0) {
        WORD textWidth;
        sprintf(line,"GAME SPEED %d%%",gameSpeed);
        textWidth=(WORD)TextLength(&frameRp,(STRPTR)line,strlen(line));
        SetAPen(&frameRp,0);
        RectFill(&frameRp,(W-textWidth)/2-5,39,
            (W+textWidth)/2+5,52);
        SetAPen(&frameRp,12);
        Move(&frameRp,(W-textWidth)/2-5,39);
        Draw(&frameRp,(W+textWidth)/2+5,39);
        SetAPen(&frameRp,15);
        Move(&frameRp,(W-textWidth)/2,49);
        Text(&frameRp,line,strlen(line));
        --speedMessageFrames;
    }
    if(musicMessageFrames>0) {
        WORD textWidth;
        strcpy(line,ingameMusicMuted?"MUSIC OFF":"MUSIC ON");
        textWidth=(WORD)TextLength(&frameRp,(STRPTR)line,strlen(line));
        SetAPen(&frameRp,0);
        RectFill(&frameRp,(W-textWidth)/2-5,55,
            (W+textWidth)/2+5,68);
        SetAPen(&frameRp,12);
        Move(&frameRp,(W-textWidth)/2-5,55);
        Draw(&frameRp,(W+textWidth)/2+5,55);
        SetAPen(&frameRp,15);
        Move(&frameRp,(W-textWidth)/2,65);
        Text(&frameRp,line,strlen(line));
        --musicMessageFrames;
    }
    if(floorMessageFrames>0) {
        WORD textWidth;
        strcpy(line,floorStyle==0?"WOOD COURT":
            (floorStyle==1?"GREEN COURT":
            (floorStyle==2?"CHECKERED COURT":"BOING BALL COURT")));
        textWidth=(WORD)TextLength(&frameRp,(STRPTR)line,strlen(line));
        SetAPen(&frameRp,0);
        RectFill(&frameRp,(W-textWidth)/2-5,71,
            (W+textWidth)/2+5,84);
        SetAPen(&frameRp,12);
        Move(&frameRp,(W-textWidth)/2-5,71);
        Draw(&frameRp,(W+textWidth)/2+5,71);
        SetAPen(&frameRp,15);
        Move(&frameRp,(W-textWidth)/2,81);
        Text(&frameRp,line,strlen(line));
        --floorMessageFrames;
    }
    if(aiMessageFrames>0) {
        WORD textWidth;
        strcpy(line,aiDifficulty==0?"AI EASY":
            (aiDifficulty==1?"AI MEDIUM":"AI HARD"));
        textWidth=(WORD)TextLength(&frameRp,(STRPTR)line,strlen(line));
        SetAPen(&frameRp,0);
        RectFill(&frameRp,(W-textWidth)/2-5,87,
            (W+textWidth)/2+5,100);
        SetAPen(&frameRp,12);
        Move(&frameRp,(W-textWidth)/2-5,87);
        Draw(&frameRp,(W+textWidth)/2+5,87);
        SetAPen(&frameRp,15);
        Move(&frameRp,(W-textWidth)/2,97);
        Text(&frameRp,line,strlen(line));
        --aiMessageFrames;
    }
    present();
}
static void resetPositions(void)
{
    static WORD pos[MAX_PLAYERS][2]={
        {80,139},{115,80},{115,198},{240,139},{205,80},{205,198},
        {27,135},{293,135}
    };
    WORD i;
    for(i=0;i<MAX_PLAYERS;++i) {
        player[i].x=pos[i][0]*SCALE;player[i].y=pos[i][1]*SCALE;
        player[i].vx=player[i].vy=0;
        player[i].team=(i<3||i==BLUE_KEEPER)?TEAM_BLUE:TEAM_RED;
        player[i].facing=player[i].team==TEAM_BLUE?DIR_RIGHT:DIR_LEFT;
    }
    ball.x=160*SCALE;ball.y=139*SCALE;ball.vx=ball.vy=0;
    ballTouchCooldown=0;ballOwner=-1;possessionProtection=0;
    possessionAge=0;possessionFacing=0;
    autoSwitchCooldown=redAutoSwitchCooldown=0;
    spaceCharge=redSpaceCharge=0;
    keeperHoldFrames=keeperReleaseFrames=keeperKickFrames=0;
    keeperReleaseImmunity=0;keeperLastKicker=-1;
    wallShotFrames=0;
    keeperAimY[0]=keeperAimY[1]=135*SCALE;
    keeperOutletX=keeperOutletY=0;
    spaceCharging=redSpaceCharging=FALSE;
    selected=0;redSelected=3;
}
static void resetMatchClock(void)
{
    matchElapsedMicros=0;
    CurrentTime(&matchClockSeconds,&matchClockMicros);
}
static void updateMatchClock(BOOL running)
{
    ULONG seconds,micros,deltaSeconds,deltaMicros;
    CurrentTime(&seconds,&micros);
    if(running) {
        deltaSeconds=seconds-matchClockSeconds;
        if(micros>=matchClockMicros)deltaMicros=micros-matchClockMicros;
        else {--deltaSeconds;deltaMicros=1000000UL+micros-matchClockMicros;}
        /* Preserve normal slow frames, but ignore an implausible system
           clock jump rather than ending the match immediately. */
        if(deltaSeconds<60)
            matchElapsedMicros+=deltaSeconds*1000000UL+deltaMicros;
    }
    matchClockSeconds=seconds;matchClockMicros=micros;
}
static void startGame(void)
{
    blueScore=redScore=0; gameFrames=0; goalPause=0; resetPositions();
    resetMatchClock();
    paused=FALSE;gameSpeed=optionGameSpeed;
    ingameMusicMuted=!optionMusic; musicMessageFrames=0;
    mode=MODE_GAME; paletteGame();paletteFloor();
    drawPitch(&arenaRp);
    musicStop();
    sfxLoad("sfx/futsal-sfx.bank"); sfxPlay("ref-whistle",0);
    if(optionMusic)musicPlay("music/mrdigs-futsal-ingame.mod");
}
static LONG distance(struct Body *a,struct Body *b)
{ return absolute(a->x-b->x)+absolute(a->y-b->y); }
static void facingVector(UBYTE facing,LONG *x,LONG *y)
{
    static const BYTE vectors[8][2]={
        {0,-16},{11,-11},{16,0},{11,11},
        {0,16},{-11,11},{-16,0},{-11,-11}
    };
    *x=vectors[facing&7][0];
    *y=vectors[facing&7][1];
}
static void kickBall(WORD i)
{
    LONG dx=ball.x-player[i].x,dy=ball.y-player[i].y,div;
    if(absolute(dx)>13*SCALE||absolute(dy)>13*SCALE) return;
    div=absolute(dx)+absolute(dy); if(div<1) div=1;
    ball.vx=dx*115/div; ball.vy=dy*115/div;
    ballOwner=-1;
    sfxPlay("kick",player[i].x<W*SCALE/2?-1:1);
    ballTouchCooldown=7;
}
static void shootBall(WORD i,WORD charge)
{
    LONG fx,fy,power;
    if(ballOwner!=i)return;
    facingVector(player[i].facing,&fx,&fy);
    power=82+(LONG)charge*5;
    if(power>162)power=162;
    ball.x=player[i].x+fx*11*SCALE/16;
    ball.y=player[i].y+fy*11*SCALE/16;
    ball.vx=fx*power/16+player[i].vx/2;
    ball.vy=fy*power/16+player[i].vy/2;
    ballOwner=-1;
    sfxPlay(charge>=12?"power-shot":"kick",
        player[i].x<W*SCALE/2?-1:1);
    ballTouchCooldown=8;
}
static void passBall(WORD team)
{
    WORD i,target=-1,first=team==TEAM_BLUE?0:3;
    WORD last=team==TEAM_BLUE?3:6;
    WORD owner=team==TEAM_BLUE?selected:redSelected;
    LONG fx,fy,dx,dy,dot,side,score,best=-32767;
    LONG tx,ty,div;
    if(ballOwner!=owner)return;
    facingVector(player[owner].facing,&fx,&fy);
    for(i=first;i<last;++i)if(i!=owner) {
        dx=player[i].x-player[owner].x;
        dy=player[i].y-player[owner].y;
        dot=(dx*fx+dy*fy)/(SCALE*16);
        side=absolute(dx*fy-dy*fx)/(SCALE*16);
        score=dot*2-side;
        if(dot<0)score+=dot*2;
        if(score>best){best=score;target=i;}
    }
    if(target<0)return;
    tx=player[target].x+player[target].vx*6;
    ty=player[target].y+player[target].vy*6;
    dx=tx-ball.x;dy=ty-ball.y;
    div=absolute(dx)+absolute(dy);if(div<1)div=1;
    ball.vx=dx*86/div;ball.vy=dy*86/div;
    ballOwner=-1;
    sfxPlay("kick",ball.x<W*SCALE/2?-1:1);
    ballTouchCooldown=8;
    if(team==TEAM_BLUE)selected=target;else redSelected=target;
}
static void aiPassBall(WORD owner)
{
    WORD first,second,target;
    LONG tx,ty,dx,dy,div;
    first=3+(WORD)(((UWORD)(rnd()>>8))%3);
    if(first==owner)first=3+((first-3+1)%3);
    second=3+((first-3+1)%3);
    if(second==owner)second=3+((second-3+1)%3);

    /*
     * Usually prefer the teammate farther toward the attacking (left)
     * goal, but retain a random alternate to avoid scripted-looking play.
     */
    target=(player[first].x<player[second].x)?first:second;
    if(((UWORD)(rnd()>>8)&3)==0)
        target=target==first?second:first;
    tx=player[target].x+player[target].vx*6;
    ty=player[target].y+player[target].vy*6;
    dx=tx-ball.x;dy=ty-ball.y;
    div=absolute(dx)+absolute(dy);if(div<1)div=1;
    ball.vx=dx*92/div;
    ball.vy=dy*92/div;
    ballOwner=-1;
    ballTouchCooldown=8;
    sfxPlay("kick",ball.x<W*SCALE/2?-1:1);
}
static void keeperDistribute(WORD owner)
{
    UWORD random=(UWORD)(rnd()>>8);
    LONG power=48+(random&15);
    LONG outletX,outletY,dx,dy,div,bestDistance,testDistance;
    WORD i,first,target;

    first=player[owner].team==TEAM_BLUE?0:3;
    if((random%10)<7) {
        /* Usually roll or throw toward one of the three field players. */
        target=first+(random%3);
        outletX=player[target].x+player[target].vx*5;
        outletY=player[target].y+player[target].vy*5;
        dx=outletX-ball.x;dy=outletY-ball.y;
        div=absolute(dx)+absolute(dy);if(div<1)div=1;
        ball.vx=dx*power/div;ball.vy=dy*power/div;
    } else {
        /* Otherwise use a safe diagonal lane. Never aim straight at the
           opposing goalkeeper. */
        ball.vx=player[owner].team==TEAM_BLUE?power:-power;
        ball.vy=(random&16)?(22+(random&15)):-(22+(random&15));
    }

    /* Select the teammate best placed to receive the clearance. Looking a
       few updates along the flight path avoids always choosing the player
       who merely happens to stand closest to the goalkeeper. */
    outletX=ball.x+ball.vx*8;
    outletY=ball.y+ball.vy*8;
    target=first;
    dx=player[first].x-outletX;dy=player[first].y-outletY;
    bestDistance=absolute(dx)+absolute(dy);
    for(i=first+1;i<first+3;++i) {
        dx=player[i].x-outletX;dy=player[i].y-outletY;
        testDistance=absolute(dx)+absolute(dy);
        if(testDistance<bestDistance) {
            bestDistance=testDistance;target=i;
        }
    }
    if(player[owner].team==TEAM_BLUE)selected=target;
    else if(playerCount==2)redSelected=target;

    ballOwner=-1;ballTouchCooldown=10;possessionProtection=0;
    keeperKickFrames=45;
    keeperReleaseImmunity=6;keeperLastKicker=owner;
    keeperHoldFrames=keeperReleaseFrames=0;
    sfxPlay("kick",player[owner].team==TEAM_BLUE?-1:1);
}
static LONG predictedBallYAtX(LONG targetX)
{
    LONG projected,travel,minY=(TOP+4)*SCALE,maxY=(BOTTOM-4)*SCALE;
    LONG span=maxY-minY,cycle,relative;
    if(ball.vx==0)return ball.y;
    travel=absolute(targetX-ball.x);
    projected=ball.y+ball.vy*travel/absolute(ball.vx);
    cycle=span*2;
    relative=(projected-minY)%cycle;
    if(relative<0)relative+=cycle;
    if(relative>span)relative=cycle-relative;
    return minY+relative;
}
static void controlKeepers(void)
{
    WORD n,index,i,above,below;LONG homeX,targetX,targetY,dx,dy,div;
    WORD reaction,errorPixels,keeperAcceleration,keeperMaximum;
    BOOL threatened,incoming;
    static const WORD reactions[3]={5,3,2};
    static const WORD errors[3]={11,6,3};
    static const WORD accelerations[3]={7,8,9};
    static const WORD maximums[3]={31,35,39};

    if(teamSize!=4)return;
    reaction=reactions[aiDifficulty];
    errorPixels=errors[aiDifficulty];
    keeperAcceleration=accelerations[aiDifficulty];
    keeperMaximum=maximums[aiDifficulty];
    for(n=0;n<2;++n) {
        index=n==0?BLUE_KEEPER:RED_KEEPER;
        homeX=(n==0?LEFT+15:RIGHT-15)*SCALE;
        threatened=n==0?(ball.x<110*SCALE):(ball.x>210*SCALE);
        incoming=ballOwner<0&&
            (n==0?(ball.x<175*SCALE&&ball.vx<0):
                  (ball.x>145*SCALE&&ball.vx>0));
        targetX=homeX;
        targetY=135*SCALE;
        if(ballOwner==index) {
            if(keeperHoldFrames==0) {
                above=below=0;
                for(i=0;i<MAX_PLAYERS;++i)
                    if(playerActive(i)&&i!=index&&
                       absolute(player[i].x-player[index].x)<80*SCALE) {
                        if(player[i].y<player[index].y)++above;
                        else ++below;
                    }
                keeperReleaseFrames=6+((UWORD)(rnd()>>8)%5);
                keeperOutletX=homeX+(n==0?1:-1)*
                    (4+((UWORD)(rnd()>>8)&3))*SCALE;
                keeperOutletY=player[index].y+
                    (above<=below?-10:10)*SCALE+
                    (((LONG)((UWORD)(rnd()>>8)&7)-3)*SCALE);
                keeperOutletY=clamp(keeperOutletY,
                    (GOAL_TOP+9)*SCALE,(GOAL_BOTTOM-9)*SCALE);
            }
            targetX=keeperOutletX;
            targetY=keeperOutletY;
            if(++keeperHoldFrames>=keeperReleaseFrames)
                keeperDistribute(index);
        } else if(incoming) {
            /* Aim for the future goal-line crossing, including any number
               of top/bottom wall reflections. Reaction time and a small
               accuracy error keep the prediction human rather than exact. */
            if((gameFrames%reaction)==0) {
                LONG error=errorPixels+(wallShotFrames>0?
                    (aiDifficulty==0?7:(aiDifficulty==1?5:3)):0);
                keeperAimY[n]=predictedBallYAtX(homeX)+
                    (((LONG)((UWORD)(rnd()>>8)%(error*2+1))-error)*SCALE);
            }
            targetY=clamp(keeperAimY[n],
                (GOAL_TOP+5)*SCALE,(GOAL_BOTTOM-5)*SCALE);
            if(threatened)targetX=(n==0?LEFT+21:RIGHT-21)*SCALE;
        } else if(threatened) {
            targetY=clamp(ball.y,(GOAL_TOP-10)*SCALE,
                (GOAL_BOTTOM+10)*SCALE);
            /* Step out modestly for a loose ball, without abandoning the
               goal mouth as aggressively as the first implementation. */
            if(n==0&&ball.x<85*SCALE)
                targetX=(LEFT+24)*SCALE;
            else if(n==1&&ball.x>235*SCALE)
                targetX=(RIGHT-24)*SCALE;
        }
        dx=targetX-player[index].x;
        dy=targetY-player[index].y;
        /* At home, remain completely still instead of nervously correcting
           every tiny position difference while play is far away. */
        if(ballOwner!=index&&!threatened&&!incoming&&
           absolute(dx)<=3*SCALE&&absolute(dy)<=3*SCALE) {
            player[index].vx=player[index].vy=0;
            player[index].facing=n==0?DIR_RIGHT:DIR_LEFT;
            continue;
        }
        div=absolute(dx)+absolute(dy);if(div<1)div=1;
        player[index].vx+=dx*keeperAcceleration/div;
        player[index].vy+=dy*keeperAcceleration/div;
        limitVelocity(&player[index].vx,&player[index].vy,keeperMaximum);
        if(dx||dy)player[index].facing=directionFromDelta(dx,dy);
    }
}
static void controlPlayers(void)
{
    LONG ax=0,ay=0,rax=0,ray=0,dx,dy,div,fx,fy,px,py,targetX,targetY;
    LONG bestDistance,candidate;
    WORD i,attacker,redChaser=3,redSlot;
    WORD aiAcceleration,aiMaximum,aiReactionMask,aiKickMask;
    LONG acceleration=spaceCharging?5:8;
    LONG redAcceleration=redSpaceCharging?5:8;
    /*
     * Easy retains the former Normal tuning. Normal now matches the human
     * player's acceleration and top speed; Hard has a small physical edge.
     */
    static const WORD aiAccelerations[3]={5,8,9};
    static const WORD aiMaximums[3]={29,38,42};
    static const WORD aiReactionMasks[3]={1,0,0};
    static const WORD aiKickMasks[3]={15,11,7};

    aiAcceleration=aiAccelerations[aiDifficulty];
    aiMaximum=aiMaximums[aiDifficulty];
    aiReactionMask=aiReactionMasks[aiDifficulty];
    aiKickMask=aiKickMasks[aiDifficulty];
    if(ballOwner>=3&&ballOwner<6)redChaser=ballOwner;
    else {
        bestDistance=distance(&player[3],&ball);
        for(i=4;i<6;++i) {
            candidate=distance(&player[i],&ball);
            if(candidate<bestDistance) {
                bestDistance=candidate;
                redChaser=i;
            }
        }
    }
    if(keys[RAW_LEFT]||keys[RAW_A]||joystickLeft) ax=-acceleration;
    if(keys[RAW_RIGHT]||keys[RAW_D]||joystickRight) ax=acceleration;
    if(keys[RAW_UP]||keys[RAW_W]||joystickUp) ay=-acceleration;
    if(keys[RAW_DOWN]||keys[RAW_S]||joystickDown) ay=acceleration;
    if(ax&&ay) {
        ax=ax>0?(ax*11+8)/16:-((-ax*11+8)/16);
        ay=ay>0?(ay*11+8)/16:-((-ay*11+8)/16);
    }
    player[selected].vx+=ax;
    player[selected].vy+=ay;
    limitVelocity(&player[selected].vx,&player[selected].vy,38);
    if(ax||ay)player[selected].facing=directionFromDelta(ax,ay);
    if(playerCount==2) {
        if(redJoystickLeft)rax=-redAcceleration;
        if(redJoystickRight)rax=redAcceleration;
        if(redJoystickUp)ray=-redAcceleration;
        if(redJoystickDown)ray=redAcceleration;
        if(rax&&ray) {
            rax=rax>0?(rax*11+8)/16:-((-rax*11+8)/16);
            ray=ray>0?(ray*11+8)/16:-((-ray*11+8)/16);
        }
        player[redSelected].vx+=rax;
        player[redSelected].vy+=ray;
        limitVelocity(&player[redSelected].vx,&player[redSelected].vy,38);
        if(rax||ray)
            player[redSelected].facing=directionFromDelta(rax,ray);
    }
    controlKeepers();
    facingVector(player[selected].facing,&fx,&fy);
    px=-fy;py=fx;
    for(i=0;i<MAX_PLAYERS;++i)
      if(playerActive(i)&&!isKeeper(i)&&i!=selected&&
         !(playerCount==2&&i==redSelected)) {
        if(i<3) {
            attacker=((selected+1)%3);
            if(i==attacker) {
                targetX=player[selected].x+fx*66*SCALE/16+
                    px*((i&1)?34:-34)*SCALE/16;
                targetY=player[selected].y+fy*66*SCALE/16+
                    py*((i&1)?34:-34)*SCALE/16;
            } else {
                targetX=player[selected].x-fx*48*SCALE/16-
                    px*((i&1)?28:-28)*SCALE/16;
                targetY=player[selected].y-fy*48*SCALE/16-
                    py*((i&1)?28:-28)*SCALE/16;
            }
            targetX=clamp(targetX,(LEFT+12)*SCALE,(RIGHT-12)*SCALE);
            targetY=clamp(targetY,(TOP+14)*SCALE,(BOTTOM-12)*SCALE);
            if(distance(&player[i],&ball)<42*SCALE) {
                targetX=(targetX*2+ball.x)/3;
                targetY=(targetY*2+ball.y)/3;
            }
            dx=targetX-player[i].x;
            dy=targetY-player[i].y;
        } else if(playerCount==2) {
            facingVector(player[redSelected].facing,&fx,&fy);
            px=-fy;py=fx;
            attacker=3+((redSelected-3+1)%3);
            if(i==attacker) {
                targetX=player[redSelected].x+fx*66*SCALE/16+
                    px*((i&1)?34:-34)*SCALE/16;
                targetY=player[redSelected].y+fy*66*SCALE/16+
                    py*((i&1)?34:-34)*SCALE/16;
            } else {
                targetX=player[redSelected].x-fx*48*SCALE/16-
                    px*((i&1)?28:-28)*SCALE/16;
                targetY=player[redSelected].y-fy*48*SCALE/16-
                    py*((i&1)?28:-28)*SCALE/16;
            }
            targetX=clamp(targetX,(LEFT+12)*SCALE,(RIGHT-12)*SCALE);
            targetY=clamp(targetY,(TOP+14)*SCALE,(BOTTOM-12)*SCALE);
            if(distance(&player[i],&ball)<42*SCALE) {
                targetX=(targetX*2+ball.x)/3;
                targetY=(targetY*2+ball.y)/3;
            }
            dx=targetX-player[i].x;
            dy=targetY-player[i].y;
        } else {
            if(i==redChaser) {
                if(ballOwner==i) {
                    targetX=(LEFT+22)*SCALE;
                    targetY=(ball.y*2+135*SCALE)/3;
                } else {
                    targetX=ball.x;
                    targetY=ball.y;
                }
            } else {
                redSlot=((i-3)-(redChaser-3)+3)%3;
                if(redSlot==1) {
                    /* Forward passing option, offset toward the attack. */
                    targetX=ball.x-35*SCALE;
                    targetY=ball.y+((i&1)?48:-48)*SCALE;
                } else {
                    /* Covering player remains behind and on the far side. */
                    targetX=ball.x+58*SCALE;
                    targetY=ball.y+((i&1)?-44:44)*SCALE;
                }
            }
            targetX=clamp(targetX,(LEFT+12)*SCALE,(RIGHT-12)*SCALE);
            targetY=clamp(targetY,(TOP+14)*SCALE,(BOTTOM-12)*SCALE);
            dx=targetX-player[i].x;
            dy=targetY-player[i].y;
        }
        div=absolute(dx)+absolute(dy); if(div<1) div=1;
        if(i<3) {
            player[i].vx+=dx*5/div;
            player[i].vy+=dy*5/div;
            limitVelocity(&player[i].vx,&player[i].vy,29);
            if(dx||dy)player[i].facing=directionFromDelta(dx,dy);
        } else if(playerCount==2) {
            player[i].vx+=dx*5/div;
            player[i].vy+=dy*5/div;
            limitVelocity(&player[i].vx,&player[i].vy,29);
            if(dx||dy)player[i].facing=directionFromDelta(dx,dy);
        } else if((gameFrames&aiReactionMask)==0) {
            player[i].vx+=dx*aiAcceleration/div;
            player[i].vy+=dy*aiAcceleration/div;
            limitVelocity(&player[i].vx,&player[i].vy,aiMaximum);
            if(dx||dy)player[i].facing=directionFromDelta(dx,dy);
        }
        if(playerCount==1&&i>=3&&ballOwner==i&&possessionAge>10&&
           (((UWORD)(rnd()>>8))&aiKickMask)==0) {
            UWORD choice=(UWORD)(rnd()>>8)&7;
            BOOL shouldPass=FALSE;
            if(aiDifficulty==2) {
                if(player[i].x>190*SCALE)shouldPass=choice<6;
                else if(player[i].x>105*SCALE)shouldPass=choice<4;
                else shouldPass=choice<2;
            } else if(aiDifficulty==1) {
                shouldPass=(player[i].x>140*SCALE&&choice<3);
            } else {
                shouldPass=(player[i].x>180*SCALE&&choice==0);
            }
            if(shouldPass)aiPassBall(i);
            else {
                player[i].facing=directionFromDelta(
                    LEFT*SCALE-player[i].x,135*SCALE-player[i].y);
                shootBall(i,aiDifficulty==0?7:
                    (aiDifficulty==1?11:16));
            }
        } else if(playerCount==1&&i>=3&&ballOwner<0&&
                  distance(&player[i],&ball)<14*SCALE&&
                  ((rnd()>>8)&aiKickMask)==0) {
            kickBall(i);
        }
    }
}
static void scoreGoal(WORD team)
{
    if(team==TEAM_BLUE) ++blueScore; else ++redScore;
    /*
     * A fast shot may overshoot far beyond the display in its scoring
     * frame. Keep the stopped ball attached to the goal that was scored
     * in; it stays hidden during the celebration and resets to centre.
     */
    ball.x=(team==TEAM_BLUE?RIGHT:LEFT)*SCALE;
    ball.y=clamp(ball.y,(GOAL_TOP+4)*SCALE,(GOAL_BOTTOM-4)*SCALE);
    ball.vx=ball.vy=0;ballOwner=-1;goalPause=55;
    sfxPlay("goal",team==TEAM_BLUE?1:-1);
}
static BOOL tryKeeperSave(void)
{
    WORD n,index,missChance;LONG dx,dy,speed;
    /* ballTouchCooldown only prevents the shooter from immediately taking
       the ball again. It must never make a close-range shot intangible to
       the goalkeeper. */
    if(teamSize!=4||ballOwner>=0)return FALSE;
    for(n=0;n<2;++n) {
        index=n==0?BLUE_KEEPER:RED_KEEPER;
        /* The ball initially remains inside the keeper's large save box
           after a release. Do not let him instantly catch his own kick. */
        if(index==keeperLastKicker&&keeperReleaseImmunity>0)continue;
        dx=ball.x-player[index].x;dy=ball.y-player[index].y;
        if(absolute(dx)<15*SCALE&&absolute(dy)<16*SCALE) {
            speed=absolute(ball.vx)+absolute(ball.vy);
            missChance=wallShotFrames>0?
                (aiDifficulty==0?3:(aiDifficulty==1?5:10)):
                (aiDifficulty==0?4:(aiDifficulty==1?10:20));
            if(speed>100&&((UWORD)(rnd()>>8)%missChance)==0)
                continue;
            if(speed>120) {
                /* Strong powershots are parried rather than swallowed. */
                ball.x=player[index].x+(n==0?12:-12)*SCALE;
                ball.vx=n==0?absolute(ball.vx)*3/4:
                    -absolute(ball.vx)*3/4;
                ball.vy+=dy/(2*SCALE);
                ballTouchCooldown=6;
                sfxPlay("wall-rebound",n==0?-1:1);
            } else {
                ballOwner=index;ball.vx=ball.vy=0;
                possessionProtection=18;possessionAge=0;
                possessionFacing=player[index].facing;
                keeperHoldFrames=0;
                sfxPlay("player-bump",n==0?-1:1);
            }
            return TRUE;
        }
    }
    return FALSE;
}
static BOOL ballInsideAssistedGoal(void)
{
    LONG speed=absolute(ball.vx)+absolute(ball.vy);

    /* The visible opening remains authoritative. Just outside either post
       is a two-pixel finishing-assist zone: almost any moving near miss is
       awarded two times out of three, but a nearly stationary ball is not. */
    if(ball.y>GOAL_TOP*SCALE&&ball.y<GOAL_BOTTOM*SCALE)return TRUE;
    if(speed<25)return FALSE;
    if((ball.y>=(GOAL_TOP-2)*SCALE&&ball.y<=GOAL_TOP*SCALE)||
       (ball.y>=GOAL_BOTTOM*SCALE&&ball.y<=(GOAL_BOTTOM+2)*SCALE))
        return ((UWORD)(rnd()>>8)%3)!=0;
    return FALSE;
}
static void physics(void)
{
    WORD i,j,closest; LONG dx,dy,fx,fy,closestDistance,currentDistance,testDistance;
    if(ballTouchCooldown>0) --ballTouchCooldown;
    if(keeperReleaseImmunity>0) --keeperReleaseImmunity;
    if(wallShotFrames>0) --wallShotFrames;
    if(possessionProtection>0) --possessionProtection;
    if(autoSwitchCooldown>0)--autoSwitchCooldown;
    if(redAutoSwitchCooldown>0)--redAutoSwitchCooldown;
    if(ballOwner>=0&&player[ballOwner].team==TEAM_RED&&
       autoSwitchCooldown==0) {
        closest=selected;
        closestDistance=distance(&player[selected],&player[ballOwner]);
        currentDistance=closestDistance;
        for(i=0;i<3;++i) {
            testDistance=distance(&player[i],&player[ballOwner]);
            if(testDistance<closestDistance) {
                closestDistance=testDistance;
                closest=i;
            }
        }
        if(closest!=selected&&closestDistance+4*SCALE<currentDistance) {
            selected=closest;
            autoSwitchCooldown=10;
        }
    }
    if(playerCount==2&&ballOwner>=0&&
       player[ballOwner].team==TEAM_BLUE&&
       redAutoSwitchCooldown==0) {
        closest=redSelected;
        closestDistance=distance(&player[redSelected],&player[ballOwner]);
        currentDistance=closestDistance;
        for(i=3;i<6;++i) {
            testDistance=distance(&player[i],&player[ballOwner]);
            if(testDistance<closestDistance) {
                closestDistance=testDistance;closest=i;
            }
        }
        if(closest!=redSelected&&closestDistance+4*SCALE<currentDistance) {
            redSelected=closest;redAutoSwitchCooldown=10;
        }
    }
    controlPlayers();
    for(i=0;i<MAX_PLAYERS;++i) if(playerActive(i)) {
        player[i].x+=player[i].vx*gameSpeed*GAME_SPEED_BASE/10000;
        player[i].y+=player[i].vy*gameSpeed*GAME_SPEED_BASE/10000;
        player[i].vx=player[i].vx*13/16;player[i].vy=player[i].vy*13/16;
        player[i].x=clamp(player[i].x,(LEFT+7)*SCALE,(RIGHT-7)*SCALE);
        player[i].y=clamp(player[i].y,(TOP+7)*SCALE,(BOTTOM-7)*SCALE);
    }
    if(ballOwner>=0) {
        facingVector(player[ballOwner].facing,&fx,&fy);
        ball.x=player[ballOwner].x+fx*7*SCALE/16;
        ball.y=player[ballOwner].y+fy*7*SCALE/16;
        ball.vx=player[ballOwner].vx;
        ball.vy=player[ballOwner].vy;
        ++possessionAge;
        if(absolute(player[ballOwner].vx)+
           absolute(player[ballOwner].vy)>30) ++possessionAge;
        if(player[ballOwner].facing!=possessionFacing) {
            possessionAge+=12;
            possessionFacing=player[ballOwner].facing;
        }
        if(possessionAge>10&&(gameFrames%12)==0&&
           absolute(player[ballOwner].vx)+
           absolute(player[ballOwner].vy)>10)
            sfxPlay("dribble-tap",
                player[ballOwner].x<W*SCALE/2?-1:1);
        if(possessionAge>=MAX_POSSESSION_AGE) {
            ball.vx=player[ballOwner].vx+fx*38/16;
            ball.vy=player[ballOwner].vy+fy*38/16;
            ballOwner=-1;
            ballTouchCooldown=8;
        }
    } else {
        ball.x+=ball.vx*gameSpeed*GAME_SPEED_BASE/10000;
        ball.y+=ball.vy*gameSpeed*GAME_SPEED_BASE/10000;
        if(keeperKickFrames>0) {
            /* Keeper clearances are deliberately medium range, leaving a
               realistic interception window instead of travelling from
               goalkeeper to goalkeeper. */
            --keeperKickFrames;
            ball.vx=ball.vx*118/128;ball.vy=ball.vy*118/128;
        } else {
            ball.vx=ball.vx*127/128;ball.vy=ball.vy*127/128;
        }
    }
    tryKeeperSave();
    if(ball.y<(TOP+4)*SCALE){
        ball.y=(TOP+4)*SCALE;ball.vy=absolute(ball.vy);
        ball.vx=clamp(ball.vx+(((UWORD)(rnd()>>8)&7)-3),-180,180);
        wallShotFrames=25;
        sfxPlay("wall-rebound",0);
    }
    if(ball.y>(BOTTOM-4)*SCALE){
        ball.y=(BOTTOM-4)*SCALE;ball.vy=-absolute(ball.vy);
        ball.vx=clamp(ball.vx+(((UWORD)(rnd()>>8)&7)-3),-180,180);
        wallShotFrames=25;
        sfxPlay("wall-rebound",0);
    }
    if(ball.x<LEFT*SCALE) {
        if(ballInsideAssistedGoal()) {
            scoreGoal(TEAM_RED);
        }
        else {ball.x=LEFT*SCALE;ball.vx=absolute(ball.vx);sfxPlay("wall-rebound",-1);}
    }
    if(ball.x>RIGHT*SCALE) {
        if(ballInsideAssistedGoal()) {
            scoreGoal(TEAM_BLUE);
        }
        else {ball.x=RIGHT*SCALE;ball.vx=-absolute(ball.vx);sfxPlay("wall-rebound",1);}
    }
    if(ballOwner<0&&ballTouchCooldown==0) {
        for(i=0;i<MAX_PLAYERS;++i) if(playerActive(i)) {
            dx=ball.x-player[i].x;dy=ball.y-player[i].y;
            if(absolute(dx)<11*SCALE&&absolute(dy)<11*SCALE) {
                ballOwner=i;
                ball.vx=ball.vy=0;
                keeperKickFrames=0;
                possessionProtection=12;
                possessionAge=0;
                possessionFacing=player[i].facing;
                if(i<3)selected=i;
                else if(i<6&&playerCount==2)redSelected=i;
                if(!isKeeper(i))keeperHoldFrames=0;
                sfxPlay("kick",player[i].x<W*SCALE/2?-1:1);
                break;
            }
        }
    } else if(ballOwner>=0&&possessionProtection==0) {
        for(i=0;i<MAX_PLAYERS;++i) if(playerActive(i)&&i!=ballOwner&&
            player[i].team!=player[ballOwner].team&&
            distance(&player[i],&player[ballOwner])<12*SCALE) {
            ballOwner=i;
            keeperKickFrames=0;
            possessionProtection=12;
            possessionAge=0;
            possessionFacing=player[i].facing;
            if(i<3)selected=i;
            else if(i<6&&playerCount==2)redSelected=i;
            keeperHoldFrames=isKeeper(i)?0:keeperHoldFrames;
            sfxPlay("player-bump",player[i].x<W*SCALE/2?-1:1);
            break;
        }
    }
    for(i=0;i<MAX_PLAYERS;++i) if(playerActive(i)) {
        for(j=i+1;j<MAX_PLAYERS;++j) if(playerActive(j)&&
           distance(&player[i],&player[j])<13*SCALE) {
            LONG t=player[i].vx;player[i].vx=player[j].vx;player[j].vx=t;
        }
    }
}
static void returnTitle(void)
{
    sfxFree(); mode=MODE_TITLE;
    spaceCharging=redSpaceCharging=FALSE;
    spaceCharge=redSpaceCharge=0;
    ingameMusicMuted=FALSE; musicMessageFrames=0;
    musicPlay("music/mrdigs-futsal-title.mod");
    titleLoaded=loadILBM("assets/title/mrdigs-futsal-title.iff",&title);
    optionsLoaded=FALSE;
}
static void drawTitleMarquee(void)
{
    WORD i,characterWidth,textWidth;
    LONG x,characterX,travel;
    UBYTE oldDrawMode;

    /*
     * Erase the three baked-in lines and compose the marquee in the
     * off-screen frame. Only complete glyphs inside the panel are drawn:
     * a RastPort without a Layer does not clip negative text coordinates.
     */
    /*
     * Pen 12 is the title artwork's almost-black panel colour. Using it
     * makes the cleaned area merge into the original panel; pen 0 in this
     * particular IFF is light grey.
     */
    SetAPen(&frameRp,12);
    RectFill(&frameRp,TITLE_MARQUEE_LEFT,TITLE_MARQUEE_TOP,
        TITLE_MARQUEE_RIGHT,TITLE_MARQUEE_BOTTOM);
    characterWidth=(WORD)TextLength(&frameRp,(STRPTR)"M",1);
    textWidth=(WORD)TextLength(&frameRp,(STRPTR)titleMarquee,
        (ULONG)strlen(titleMarquee));
    travel=textWidth+(TITLE_MARQUEE_RIGHT-TITLE_MARQUEE_LEFT+1);
    x=TITLE_MARQUEE_RIGHT-
        (LONG)((gameFrames*3)%(ULONG)travel);
    oldDrawMode=frameRp.DrawMode;
    SetDrMd(&frameRp,JAM1);
    /* Pen 0 is the brightest near-white colour in the title IFF palette. */
    SetAPen(&frameRp,0);
    for(i=0;titleMarquee[i]!='\0';++i) {
        characterX=x+(LONG)i*characterWidth;
        if(characterX>=TITLE_MARQUEE_LEFT&&
           characterX+characterWidth<=TITLE_MARQUEE_RIGHT+1) {
            Move(&frameRp,(WORD)characterX,TITLE_MARQUEE_BASELINE);
            Text(&frameRp,(STRPTR)&titleMarquee[i],1);
        }
    }
    SetDrMd(&frameRp,oldDrawMode);
}
static void renderTitle(void)
{
    if(titleLoaded) BltBitMap(&title,0,0,frame,0,0,W,H,0xc0,0xff,NULL);
    else {SetAPen(&frameRp,0);RectFill(&frameRp,0,0,W-1,H-1);
          textCenter(&frameRp,"MRDIG'S FUTSAL",100,15);}
    drawTitleMarquee();
    present(); ++gameFrames;
}
static void enterOptions(void)
{
    if(mode!=MODE_TITLE) {
        sfxFree();
        musicPlay("music/mrdigs-futsal-title.mod");
    }
    optionsLoaded=loadILBM("assets/options/mrdigs-futsal-options.iff",&title);
    mode=MODE_OPTIONS;
    optionRow=0;
    spaceCharging=redSpaceCharging=FALSE;
    spaceCharge=redSpaceCharge=0;
    menuJoystickUp=menuJoystickDown=FALSE;
    menuJoystickLeft=menuJoystickRight=FALSE;
}
static void optionValue(char *line,WORD row)
{
    static const char *courts[4]={"WOOD","GREEN","CHECKERED","BOING BALL"};
    static const char *levels[3]={"EASY","MEDIUM","HARD"};
    switch(row) {
    case 0: sprintf(line,"< %d PLAYER%s >",playerCount,
                    playerCount==1?"":"S");break;
    case 1: sprintf(line,"< %d VS %d >",teamSize,teamSize);break;
    case 2: sprintf(line,"< %s >",courts[floorStyle]);break;
    case 3:
        if(playerCount==2)strcpy(line,"< NOT USED >");
        else sprintf(line,"< %s >",levels[aiDifficulty]);
        break;
    case 4: sprintf(line,"< %s >",optionMusic?"YES":"NO");break;
    case 5: sprintf(line,"< %d >",goalLimit);break;
    case 6: sprintf(line,"< %d MIN >",timeLimitMinutes);break;
    default:sprintf(line,"< %d%% >",optionGameSpeed);break;
    }
}
static void renderOptions(void)
{
    static const char *labels[8]={
        "PLAYERS","TEAM SIZE","COURT","CPU LEVEL","MUSIC",
        "GOAL LIMIT","TIME LIMIT","GAME SPEED"
    };
    char value[32]; WORD row,y;
    UBYTE oldDrawMode=frameRp.DrawMode;
    if(optionsLoaded) {
        BltBitMap(&title,0,0,frame,0,0,W,H,0xc0,0xff,NULL);
        WaitBlit();
    } else {
        SetAPen(&frameRp,0);RectFill(&frameRp,0,0,W-1,H-1);
    }
    /* JAM1 is essential here: JAM2 paints a white rectangle behind every
       Topaz glyph, which made all menu labels look like solid bars. */
    SetDrMd(&frameRp,JAM1);
    for(row=0;row<8;++row) {
        y=(WORD)(86+row*18);
        if(row==optionRow) {
            SetAPen(&frameRp,6);RectFill(&frameRp,47,y-9,273,y+3);
            SetAPen(&frameRp,7);RectFill(&frameRp,49,y-8,271,y-7);
        }
        SetAPen(&frameRp,15);Move(&frameRp,58,y);Text(&frameRp,
            (STRPTR)labels[row],strlen(labels[row]));
        optionValue(value,row);
        /* Leave enough room for the longest value, "< 2 PLAYERS >". */
        Move(&frameRp,158,y);Text(&frameRp,value,strlen(value));
    }
    /* Ground the controls in two small console buttons instead of leaving
       the captions floating in the empty footer. */
    SetAPen(&frameRp,2);RectFill(&frameRp,43,225,143,246);
    RectFill(&frameRp,181,225,277,246);
    SetAPen(&frameRp,7);RectFill(&frameRp,45,227,141,244);
    RectFill(&frameRp,183,227,275,244);
    SetAPen(&frameRp,5);RectFill(&frameRp,47,229,139,243);
    RectFill(&frameRp,185,229,273,243);
    SetAPen(&frameRp,9);Move(&frameRp,54,240);Text(&frameRp,"FIRE START",10);
    SetAPen(&frameRp,15);Move(&frameRp,197,240);Text(&frameRp,"ESC BACK",8);
    SetDrMd(&frameRp,oldDrawMode);
    present();++gameFrames;
}
static void adjustOption(WORD direction)
{
    static const WORD goals[3]={5,10,15};
    static const WORD minutes[3]={3,5,10};
    static const WORD speeds[4]={80,100,120,150};
    WORD i;
    switch(optionRow) {
    case 0: playerCount=playerCount==1?2:1;break;
    case 1: teamSize=teamSize==3?4:3;break;
    case 2: floorStyle=(WORD)((floorStyle+direction+4)%4);break;
    case 3:
        if(playerCount==1)
            aiDifficulty=(WORD)((aiDifficulty+direction+3)%3);
        break;
    case 4: optionMusic=!optionMusic;break;
    case 5:
        for(i=0;i<3&&goals[i]!=goalLimit;++i);if(i==3)i=0;
        goalLimit=goals[(i+direction+3)%3];break;
    case 6:
        for(i=0;i<3&&minutes[i]!=timeLimitMinutes;++i);if(i==3)i=0;
        timeLimitMinutes=minutes[(i+direction+3)%3];break;
    case 7:
        for(i=0;i<4&&speeds[i]!=optionGameSpeed;++i);if(i==4)i=1;
        optionGameSpeed=speeds[(i+direction+4)%4];break;
    }
    sfxPlay("menu-select",0);
}
static void moveOption(WORD direction)
{
    optionRow=(WORD)((optionRow+direction+8)%8);
    sfxPlay("menu-select",0);
}
static void actionPressed(WORD team)
{
    WORD owner=team==TEAM_BLUE?selected:redSelected;
    if(mode==MODE_TITLE||mode==MODE_WIN) {
        enterOptions();
    } else if(mode==MODE_OPTIONS) {
        startGame();
    } else if(mode==MODE_GAME&&!paused&&!goalPause&&!spaceCharging&&
              ballOwner==owner&&(team==TEAM_BLUE||playerCount==2)) {
        if(team==TEAM_BLUE) {spaceCharging=TRUE;spaceCharge=0;}
        else if(!redSpaceCharging) {
            redSpaceCharging=TRUE;redSpaceCharge=0;
        }
    }
}
static void actionReleased(WORD team)
{
    BOOL charging=team==TEAM_BLUE?spaceCharging:redSpaceCharging;
    WORD charge=team==TEAM_BLUE?spaceCharge:redSpaceCharge;
    WORD owner=team==TEAM_BLUE?selected:redSelected;
    if(mode==MODE_GAME&&charging) {
        if(!paused&&!goalPause) {
            if(charge<PASS_TAP_FRAMES)passBall(team);
            else shootBall(owner,charge);
        }
        if(team==TEAM_BLUE){spaceCharging=FALSE;spaceCharge=0;}
        else {redSpaceCharging=FALSE;redSpaceCharge=0;}
    }
}
static void pollJoystick(void)
{
    volatile UWORD *joy0dat=(volatile UWORD *)0xdff00a;
    volatile UWORD *joy1dat=(volatile UWORD *)0xdff00c;
    volatile UBYTE *ciaaPra=(volatile UBYTE *)0xbfe001;
    UWORD value=*joy1dat,redValue=*joy0dat;
    UBYTE buttons=*ciaaPra;
    BOOL fire=((buttons&0x80)==0),redFire=((buttons&0x40)==0);
    BOOL left=(value&0x0200)!=0;
    BOOL right=(value&0x0002)!=0;
    BOOL redLeft=(redValue&0x0200)!=0;
    BOOL redRight=(redValue&0x0002)!=0;
    BOOL menuUp,menuDown,menuLeft,menuRight;

    /*
     * JOY1DAT stores quadrature counter bits, so vertical directions are
     * decoded by XORing each vertical bit with its horizontal partner.
     */
    joystickLeft=left;
    joystickRight=right;
    joystickUp=((value^(value>>1))&0x0100)!=0;
    joystickDown=((value^(value>>1))&0x0001)!=0;
    redJoystickLeft=redLeft;
    redJoystickRight=redRight;
    redJoystickUp=((redValue^(redValue>>1))&0x0100)!=0;
    redJoystickDown=((redValue^(redValue>>1))&0x0001)!=0;
    if(mode==MODE_OPTIONS) {
        /* Only player one's joystick (port 2 / JOY1DAT) navigates menus.
           JOY0DAT is normally the mouse in port 1; combining its quadrature
           counters with menu input caused apparently random directions. */
        menuUp=joystickUp;
        menuDown=joystickDown;
        menuLeft=joystickLeft;
        menuRight=joystickRight;
        if(menuUp&&!menuJoystickUp)moveOption(-1);
        if(menuDown&&!menuJoystickDown)moveOption(1);
        if(menuLeft&&!menuJoystickLeft)adjustOption(-1);
        if(menuRight&&!menuJoystickRight)adjustOption(1);
        menuJoystickUp=menuUp;menuJoystickDown=menuDown;
        menuJoystickLeft=menuLeft;menuJoystickRight=menuRight;
    }
    if(fire&&!joystickFire)actionPressed(TEAM_BLUE);
    else if(!fire&&joystickFire)actionReleased(TEAM_BLUE);
    /* Port 1 belongs to player 2 only during an active two-player match.
       It must never advance the title, options, or result screens. */
    if(mode==MODE_GAME&&playerCount==2) {
        if(redFire&&!redJoystickFire)actionPressed(TEAM_RED);
        else if(!redFire&&redJoystickFire)actionReleased(TEAM_RED);
    }
    joystickFire=fire;
    redJoystickFire=redFire;
}
static void key(UWORD code)
{
    BOOL up=(code&IECODE_UP_PREFIX)!=0; code&=~IECODE_UP_PREFIX;
    if(code<128) keys[code]=!up;
    if(mode==MODE_GAME&&code==RAW_SPACE) {
        if(up)actionReleased(TEAM_BLUE);else actionPressed(TEAM_BLUE);
        return;
    }
    if(up) return;
    if(mode==MODE_TITLE) {
        if(code==RAW_ESC) quit=TRUE;
        else if(code==RAW_SPACE)actionPressed(TEAM_BLUE);
    } else if(mode==MODE_OPTIONS) {
        if(code==RAW_ESC)returnTitle();
        else if(code==RAW_UP||code==RAW_W)moveOption(-1);
        else if(code==RAW_DOWN||code==RAW_S)moveOption(1);
        else if(code==RAW_LEFT||code==RAW_A)adjustOption(-1);
        else if(code==RAW_RIGHT||code==RAW_D)adjustOption(1);
        else if(code==RAW_SPACE)startGame();
    } else if(mode==MODE_GAME) {
        if(code==RAW_ESC) returnTitle();
        else if(code==RAW_PLUS||code==RAW_NP_PLUS) {
            if(gameSpeed<240)gameSpeed+=10;
            speedMessageFrames=75;
        } else if(code==RAW_MINUS||code==RAW_NP_MINUS) {
            if(gameSpeed>60)gameSpeed-=10;
            speedMessageFrames=75;
        }
        else if(code==RAW_M) {
            ingameMusicMuted=!ingameMusicMuted;
            if(ingameMusicMuted)musicStop();
            else musicPlay("music/mrdigs-futsal-ingame.mod");
            musicMessageFrames=75;
        }
        else if(code==RAW_1||code==RAW_2||code==RAW_3||code==RAW_4) {
            floorStyle=(WORD)(code-RAW_1);
            paletteFloor();
            drawPitch(&arenaRp);
            floorMessageFrames=75;
        }
        else if(code==RAW_7||code==RAW_8||code==RAW_9) {
            aiDifficulty=(WORD)(code-RAW_7);
            aiMessageFrames=75;
        }
        else if(code==RAW_TAB) {
            do {selected=(selected+1)%3;} while(selected>=3);
            autoSwitchCooldown=20;
            sfxPlay("menu-select",0);
        }
        else if(code==RAW_P) paused=!paused;
    } else {
        if(code==RAW_ESC) returnTitle();
        else if(code==RAW_SPACE)actionPressed(TEAM_BLUE);
    }
}
static void input(void)
{
    struct IntuiMessage *m; ULONG cls; UWORD code;
    while((m=(struct IntuiMessage *)GetMsg(window->UserPort))) {
        cls=m->Class;code=m->Code;ReplyMsg((struct Message *)m);
        if(cls==IDCMP_RAWKEY) key(code);
    }
}
static BOOL openAll(void)
{
    IntuitionBase=(struct IntuitionBase *)OpenLibrary("intuition.library",37);
    GfxBase=(struct GfxBase *)OpenLibrary("graphics.library",37);
    if(!IntuitionBase||!GfxBase)return FALSE;
    screen=OpenScreenTags(NULL,SA_Width,W,SA_Height,H,SA_Depth,4,
        SA_DisplayID,PAL_MONITOR_ID|LORES_KEY,
        SA_Title,(ULONG)"MrDig's Futsal",
        SA_Quiet,TRUE,SA_ShowTitle,FALSE,TAG_DONE);
    if(!screen)return FALSE;
    window=OpenWindowTags(NULL,WA_CustomScreen,(ULONG)screen,WA_Left,0,WA_Top,0,
        WA_Width,W,WA_Height,H,WA_Borderless,TRUE,WA_Backdrop,TRUE,
        WA_Activate,TRUE,WA_RMBTrap,TRUE,WA_IDCMP,IDCMP_RAWKEY,TAG_DONE);
    if(!window)return FALSE;
    hiddenPointer=(UWORD *)AllocMem(4,MEMF_CHIP|MEMF_CLEAR);
    if(hiddenPointer)SetPointer(window,hiddenPointer,1,16,0,0);
    display=window->RPort;
    systemBitmap=screen->RastPort.BitMap;
    if(!allocBitmap(&frameBuffers[0]))return FALSE;
    if(!allocBitmap(&frameBuffers[1])) {
        freeBitmap(&frameBuffers[0]);return FALSE;
    }
    frameReady=TRUE;frameIndex=0;frame=&frameBuffers[0];
    if(!allocBitmap(&title))return FALSE;titleReady=TRUE;
    if(!allocBitmap(&arena))return FALSE;arenaReady=TRUE;
    InitRastPort(&frameRp);frameRp.BitMap=frame;
    InitRastPort(&arenaRp);arenaRp.BitMap=&arena;
    SetFont(&frameRp,display->Font);return TRUE;
}
static void closeAll(void)
{
    sfxFree();musicShutdown();
    if(screen&&systemBitmap&&screen->ViewPort.RasInfo) {
        WaitTOF();
        screen->ViewPort.RasInfo->BitMap=systemBitmap;
        screen->RastPort.BitMap=systemBitmap;
        if(display)display->BitMap=systemBitmap;
        MakeScreen(screen);RethinkDisplay();WaitTOF();
    }
    if(frameReady) {
        freeBitmap(&frameBuffers[0]);freeBitmap(&frameBuffers[1]);
    }
    if(titleReady)freeBitmap(&title);
    if(arenaReady)freeBitmap(&arena);
    if(window){if(hiddenPointer){ClearPointer(window);FreeMem(hiddenPointer,4);}CloseWindow(window);}
    if(screen)CloseScreen(screen);
    if(GfxBase)CloseLibrary((struct Library *)GfxBase);
    if(IntuitionBase)CloseLibrary((struct Library *)IntuitionBase);
    if(cacheStateSaved)
        CacheControl(oldCacheState,CACRF_EnableI);
}
int main(void)
{
    /*
     * Workbench normally enables the 68020 instruction cache through
     * SetPatch. A minimal game floppy does not, so enable it here and restore
     * the caller's original state when the game exits.
     */
    oldCacheState=CacheControl(CACRF_EnableI|CACRF_ClearI,
        CACRF_EnableI|CACRF_ClearI);
    cacheStateSaved=TRUE;
    if(!openAll()){closeAll();printf("MrDig's Futsal: screen failed.\\n");return 20;}
    musicInitialize(50);returnTitle();quit=FALSE;gameFrames=0;
    while(!quit) {
        input();pollJoystick();sfxTick();
        if(mode==MODE_TITLE)renderTitle();
        else if(mode==MODE_OPTIONS)renderOptions();
        else if(mode==MODE_GAME) {
            updateMatchClock(!goalPause&&!paused);
            if(goalPause) {
                if(!--goalPause) {
                    resetPositions();
                    sfxPlay("ref-whistle",0);
                }
            }
            else if(!paused) {
                if(spaceCharging&&spaceCharge<MAX_SHOT_CHARGE)
                    ++spaceCharge;
                if(redSpaceCharging&&redSpaceCharge<MAX_SHOT_CHARGE)
                    ++redSpaceCharge;
                physics();++gameFrames;
            }
            if(blueScore>=goalLimit||redScore>=goalLimit||
               (matchElapsedMicros>=(ULONG)timeLimitMinutes*60UL*1000000UL&&
                blueScore!=redScore))mode=MODE_WIN;
            renderGame();
        } else {
            renderGame();
        }
    }
    closeAll();return 0;
}
