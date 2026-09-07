#include "ready_dust.h"
#include "ready_dust_mask.h"
void readyDustRestore(unsigned char **planes,struct ReadyDustHistory *h)
{
    unsigned char i,p;
    for(i=0;i<h->count;i++) for(p=0;p<6;p++)
        planes[p][h->offset[i]]=h->original[i][p];
    h->count=0;
}
static void point(unsigned char **planes,struct ReadyDustHistory *h,
                  int x,int y,unsigned char pen,unsigned char state)
{
    unsigned short offset;
    unsigned char bit,i,p;
    if(x<0||x>=320||y<0||y>=256) return;
    offset=(unsigned short)(y*40+x/8);bit=(unsigned char)(128>>(x&7));
    if(x>=64&&x<256&&y>=118&&y<222) {
        if(!(readyDustMenuMask[(unsigned long)state*2496+(y-118)*24+(x-64)/8]&bit)) return;
    } else if(!(readyDustMask[offset]&bit)) return;
    for(i=0;i<h->count;i++) if(h->offset[i]==offset) break;
    if(i==h->count) {
        if(i==READY_DUST_DIRTY) return;
        h->offset[i]=offset;
        for(p=0;p<6;p++) h->original[i][p]=planes[p][offset];
        h->count++;
    }
    for(p=0;p<6;p++) {
        if(pen&(1<<p)) planes[p][offset]|=bit;
        else planes[p][offset]&=(unsigned char)~bit;
    }
}
void readyDustDraw(unsigned char **planes,struct ReadyDustHistory *h,
                   unsigned long frame,const unsigned char *pens,unsigned char state)
{
    static const unsigned short phase[12]={7,47,91,139,181,223,263,307,29,113,201,281};
    static const unsigned char speed[12]={2,3,4,5,6,8,3,7,4,6,3,5};
    static const unsigned char y[12]={108,122,145,168,182,210,228,115,136,157,196,218};
    static const unsigned char shape[12]={0,1,0,2,1,0,1,2,1,2,0,1};
    unsigned char i,j,pen;int x;
    for(i=0;i<12;i++) {
        x=323-(int)((phase[i]+((frame*speed[i])>>3))%328);
        {
            static const unsigned char glow[8]={0,1,2,3,4,3,2,1};
            pen=pens[glow[((frame>>3)+i*3)&7]];
            /* Stagger warm accents: 0.4 seconds per particle every 3.84 seconds. */
            if((frame+(unsigned long)i*37)%192<20) pen=pens[5];
        }
        for(j=0;j<(shape[i]==0?1:shape[i]==1?2:4);j++) point(planes,h,x+j,y[i],pen,state);
    }
}
