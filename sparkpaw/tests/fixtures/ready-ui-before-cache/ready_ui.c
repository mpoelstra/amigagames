#include "ready_ui.h"
#include "ready_ui_data.h"
#include <string.h>

static void dot(struct ReadyUI *u,int x,int y,unsigned pen)
{
    unsigned p,offset,bit; int xx,yy;
    x-=48; y-=118;
    if(x<0||x>=224||y<0||y>=104) return;
    offset=y*28+x/8;bit=128>>(x&7);
    for(p=0;p<6;p++) {
        unsigned char *v=&u->patch[p*READY_UI_PLANE+offset];
        *v=(unsigned char)((*v&~bit)|((pen&(1<<p))?bit:0));
    }
    /* Actual visible glyph + 1px margin, not a rectangle/union of old labels. */
    for(yy=y-1;yy<=y+1;yy++) for(xx=x-1;xx<=x+1;xx++)
        if(xx>=0&&xx<224&&yy>=0&&yy<104)
            u->mask[yy*28+xx/8]&=(unsigned char)~(128>>(xx&7));
}
static void text(struct ReadyUI *u,const char *s,int x,int y,unsigned pen,int scale)
{
    unsigned c,gx,gy;int a,b;
    while((c=(unsigned char)*s++)!=0) {
        if(c>=32&&c<=90) for(gy=0;gy<7;gy++) for(gx=0;gx<5;gx++)
            if(readyUiGlyphs[(c-32)*7+gy]&(16>>gx))
                for(a=0;a<scale;a++) for(b=0;b<scale;b++)
                    dot(u,x+(int)gx*scale+a,y+(int)gy*scale+b,pen);
        x+=6*scale;
    }
}
static void centre(struct ReadyUI *u,const char *s,int y,unsigned pen,int scale)
{ text(u,s,160-((int)strlen(s)*6-1)*scale/2,y,pen,scale); }
static void medium(struct ReadyUI *u,const char *s,int y,unsigned pen)
{
    int x=160-(int)strlen(s)*9/2; unsigned c,gx,gy; int xx,yy;
    while((c=(unsigned char)*s++)!=0) {
        for(gy=0;gy<7;gy++) for(gx=0;gx<5;gx++)
            if(readyUiGlyphs[(c-32)*7+gy]&(16>>gx))
                for(yy=gy*3/2;yy<(int)(gy+1)*3/2;yy++)
                    for(xx=gx*3/2;xx<(int)(gx+1)*3/2;xx++)
                        dot(u,x+xx,y+yy,pen);
        x+=9;
    }
}
static void row(struct ReadyUI *u,const char *label,const char *value,int y,int selected,int valueX)
{
    unsigned pen=selected?READY_UI_CYAN:READY_UI_GREY;
    text(u,label,valueX-16-((int)strlen(label)*6-1),y,READY_UI_CREAM,1);
    text(u,value,valueX,y,pen,1);
    if(selected) {
        text(u,"<",valueX-9,y,pen,1);
        text(u,">",valueX+(int)strlen(value)*6+4,y,pen,1);
    }
}
/* One screen-centred gutter on both pages; arrows do not affect alignment. */
#define READY_VALUE_X 167
static void action(struct ReadyUI *u,const char *s,int y,int selected)
{
    int x,width=(int)strlen(s)*6-1;
    centre(u,s,y,selected?READY_UI_CYAN:READY_UI_GREY,1);
    if(selected) for(x=0;x<12;x++) {
        dot(u,160-width/2-20+x,y+3,READY_UI_CYAN);
        dot(u,160+(width+1)/2+8+x,y+3,READY_UI_CYAN);
    }
}
static void atlasPatch(struct ReadyUI *u,unsigned char **atlas,unsigned state)
{
    unsigned p,y;
    memcpy(u->patch,u->background,READY_UI_BYTES);
    for(p=0;p<6;p++) for(y=0;y<104;y++)
        memcpy(u->patch+p*READY_UI_PLANE+y*28+2,atlas[p]+state*2496+y*24,24);
}
void readyUiInit(struct ReadyUI *u,unsigned char **atlas)
{
    unsigned i,n=0,k;
    for(i=0;i<sizeof(readyUiBackgroundRle);i+=2)
        for(k=0;k<readyUiBackgroundRle[i];k++) u->background[n++]=readyUiBackgroundRle[i+1];
    atlasPatch(u,atlas,0);
    memcpy(u->previous[0],u->patch,READY_UI_BYTES);
    memcpy(u->previous[1],u->patch,READY_UI_BYTES);
}
void readyUiCompose(struct ReadyUI *u,unsigned char **atlas,
                    const struct ReadySelection *s,int soundtest)
{
    static const char *const modes[]={"SFX ONLY","MUSIC ONLY","SFX + MUSIC"};
    if(s->page==READY_PAGE_MAIN) { atlasPatch(u,atlas,s->row?1:0); return; }
    memcpy(u->patch,u->background,READY_UI_BYTES);
    memcpy(u->mask,readyUiBaseMask,READY_UI_PLANE);
    if(s->page==READY_PAGE_OPTIONS) {
        medium(u,"OPTIONS",124,READY_UI_CYAN);
        row(u,"SECOND BUTTON",s->secondary?"FIRE":"JUMP",146,s->row==0,READY_VALUE_X);
        row(u,"START AT",s->section?"STORMRAIL":"STORM RUINS",162,s->row==1,READY_VALUE_X);
        row(u,"AUDIO MODE",modes[s->mode],178,s->row==2,READY_VALUE_X);
        if(soundtest) action(u,"SOUNDTEST",198,s->row==3);
        action(u,"BACK",211,s->row==(soundtest?4:3));
    }
#ifndef SPARKPAW_MULTI_ADF
    else {
        static const char *const fx[]={"PLASMA SHOT","PLAYER HURT","ENEMY HIT","ENEMY DEATH",
            "STRIDER SHOT","JUMP","DIAMOND PICKUP","WATER SPLASH","STORMSTONE CORE",
            "TALLY TICK","EXTRA LIFE","FAN CHARGE","FAN FIRE",
            "HUNTER CHARGE","HUNTER FIRE","HEALTH PICKUP"};
        static const char *const tracks[]={"HERO DRIVE","NEON SKY","COPPER SPRINT","IRON HORIZON","STORM LIGHT"};
        const char *status=s->status==READY_PREVIEW_LOADING?"LOADING AUDIO":
            s->status==READY_PREVIEW_ERROR?"TRACK UNAVAILABLE":
            s->row==0?"PRESS FIRE TO PLAY SFX":
            s->status==READY_PREVIEW_PLAYING?"PRESS FIRE TO STOP":
            "PRESS FIRE TO PLAY MUSIC";
        medium(u,"SOUNDTEST",124,READY_UI_CYAN);
        row(u,"SFX TEST",fx[s->sfx],146,s->row==0,READY_VALUE_X);
        row(u,"MUSIC TEST",tracks[s->track],162,s->row==1,READY_VALUE_X);
        /* Top at 185 centres the 7px hint between MUSIC TEST and BACK. */
        if(s->row<2) centre(u,status,185,READY_UI_GREY,1);
        action(u,"BACK",207,s->row==2);
    }
#else
    (void)soundtest;
#endif
}
void readyUiApply(struct ReadyUI *u,unsigned char **target,unsigned buffer)
{
    unsigned y,x,p,first,last,n;
    unsigned char *old=u->previous[buffer];
    for(y=0;y<104;y++) {
        first=28;last=0;
        for(x=0;x<28;x++) for(p=0;p<6;p++) {
            unsigned at=p*READY_UI_PLANE+y*28+x;
            if(old[at]!=u->patch[at]) { if(first>x)first=x;last=x+1;break; }
        }
        first&=~1U;last=(last+1)&~1U;
        if(first>=last) continue;
        for(p=0;p<6;p++) {
            unsigned at=p*READY_UI_PLANE+y*28+first;
            unsigned short *dst=(unsigned short *)(target[p]+(y+118)*40+6+first);
            unsigned short *history=(unsigned short *)(old+at);
            const unsigned short *src=(const unsigned short *)(u->patch+at);
            for(n=(last-first)/2;n;n--) { *history++=*src; *dst++=*src++; }
        }
    }
}
