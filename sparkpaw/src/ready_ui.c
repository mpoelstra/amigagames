#include "ready_ui.h"
#ifdef SPARKPAW_MULTI_ADF
#include "ready_ui_cache_adf_data.h"
#else
#include "ready_ui_cache_data.h"
#endif
#include <string.h>

/* No glyph renderer or image-difference scan may be linked here. Both belong
   to the offline generator; tests enforce the owned-frame work contract. */
void readyUiInit(struct ReadyUI *u,unsigned char **atlas)
{
    unsigned long i,n=0;
    unsigned k,b,id,control,count;
    (void)atlas;
    for(i=0;i<sizeof(readyUiRle);) {
        control=readyUiRle[i++];count=(control&127)+1;
        for(k=0;k<count;k++) u->cache[n++]=(control&128)?readyUiRle[i++]:0;
    }
    /* Delta-RLE saves disk space; reconstruct once while CHARGING is live. */
    for(b=0;b<5;b++) for(id=1;id<readyUiCounts[b];id++) {
        unsigned long at=readyUiOffsets[readyUiBases[b]+id];
        unsigned long previous=readyUiOffsets[readyUiBases[b]+id-1];
        for(k=0;k<readyUiHeight[b]*28*7;k++) u->cache[at+k]^=u->cache[previous+k];
    }
    for(b=0;b<5;b++) {
        u->selected[b]=readyUiMap[b];
        u->previous[0][b]=u->previous[1][b]=readyUiMap[b];
    }
    memset(u->mask,0,sizeof(u->mask));
}

void readyUiCompose(struct ReadyUI *u,unsigned char **atlas,
                    const struct ReadySelection *s,int soundtest)
{
    unsigned state,b,id;
    (void)atlas;
#ifdef SPARKPAW_MULTI_ADF
    (void)soundtest;
    if(s->page==READY_PAGE_MAIN) state=s->row;
    else if(s->page==READY_PAGE_OPTIONS)
        state=2+((s->secondary*2+s->section)*3+s->mode)*4+s->row;
    else return; /* Soundtest has no ADF states or assets. */
#else
    if(s->page==READY_PAGE_MAIN) state=s->row;
    else if(s->page==READY_PAGE_OPTIONS)
        state=2+(soundtest?0:60)+((s->secondary*2+s->section)*3+s->mode)*
              (soundtest?5:4)+s->row;
    else state=110+(s->sfx*5+s->track)*12+s->status*3+s->row;
#endif
    for(b=0;b<5;b++) {
        id=readyUiMap[state*5+b];
        if(id!=u->selected[b]) {
            const unsigned char *mask=u->cache+readyUiOffsets[readyUiBases[b]+id]+
                                      readyUiHeight[b]*28*6;
            memcpy(u->mask+readyUiY[b]*28,mask,readyUiHeight[b]*28);
            u->selected[b]=(unsigned char)id;
        }
    }
}

void readyUiApply(struct ReadyUI *u,unsigned char **target,unsigned buffer)
{
    unsigned b,p,y,n,first,end,old,id,h;
    for(b=0;b<5;b++) {
        const unsigned char *rect,*pixels;
        old=u->previous[buffer][b];id=u->selected[b];
        if(old==id) continue;
        rect=readyUiBounds+readyUiTransitions[b]+(old*readyUiCounts[b]+id)*4;
        first=rect[2];end=rect[3];h=readyUiHeight[b];
        pixels=u->cache+readyUiOffsets[readyUiBases[b]+id];
        for(p=0;p<6;p++) {
            const unsigned short *src=(const unsigned short *)(pixels+p*h*28+rect[0]*28+first);
            unsigned short *dst=(unsigned short *)(target[p]+(118+readyUiY[b]+rect[0])*40+6+first);
            for(y=rect[0];y<rect[1];y++) {
                /* Bounded word-aligned CPU copies; Fast source is not DMA.
                   Four words per branch, then at most three tail words. */
                n=(end-first)/2;
                while(n>=4) {
                    *dst++=*src++;*dst++=*src++;
                    *dst++=*src++;*dst++=*src++;
                    n-=4;
                }
                while(n--) *dst++=*src++;
                src+=14-(end-first)/2;dst+=20-(end-first)/2;
            }
        }
        u->previous[buffer][b]=(unsigned char)id;
    }
}
