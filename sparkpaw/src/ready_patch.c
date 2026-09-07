#include "ready_patch.h"
#include <stdint.h>
/* CPU-only row spans: no additional DMA/Chip buffer. Word aligned copies. */
static unsigned char spans[12][12][104][2];
void readyPatchPrepare(unsigned char **source)
{
    unsigned int a,b,y,x,p,first,last;
    for(a=0;a<12;a++) for(b=0;b<12;b++) for(y=0;y<104;y++) {
        first=24;last=0;
        if(a!=b) for(x=0;x<24;x++) for(p=0;p<6;p++) {
            if(source[p][(a*104+y)*24+x]!=source[p][(b*104+y)*24+x]) {
                if(x<first) first=x;
                if(x+1>last) last=x+1;
                break;
            }
        }
        spans[a][b][y][0]=(unsigned char)(first&~1U);
        spans[a][b][y][1]=(unsigned char)((last+1)&~1U);
    }
}
void readyPatchApply(unsigned char **target,unsigned char **source,
                     unsigned char oldState,unsigned char newState)
{
    unsigned int y,p,first,end,n;
    const unsigned char (*rows)[2]=spans[oldState][newState];
    if((oldState<2)!=(newState<2)) {
        /* Full page transitions already span all 24 bytes on every row.
           Both offsets/strides are multiples of four. Keep Fast->Chip CPU
           access, but remove the per-word loop and repeated address maths. */
        for(p=0;p<6;p++) {
            const uint32_t *src=(const uint32_t *)(source[p]+newState*104*24);
            uint32_t *dst=(uint32_t *)(target[p]+118*40+8);
            for(y=0;y<104;y++) {
                dst[0]=src[0];dst[1]=src[1];dst[2]=src[2];
                dst[3]=src[3];dst[4]=src[4];dst[5]=src[5];
                src+=6;dst+=10;
            }
        }
        return;
    }
    for(y=0;y<104;y++) {
        first=rows[y][0];end=rows[y][1];
        if(first>=end) continue;
        for(p=0;p<6;p++) {
            const unsigned short *src=(const unsigned short *)(source[p]+(newState*104+y)*24+first);
            unsigned short *dst=(unsigned short *)(target[p]+(118+y)*40+8+first);
            for(n=(end-first)/2;n;n--) *dst++=*src++;
        }
    }
}
