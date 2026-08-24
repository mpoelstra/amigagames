#include <stdio.h>
#include <string.h>

typedef unsigned short UWORD;

#define PLANES 4
#define VISIBLE_WORDS 3
#define DMA_WORDS 4

static void pixelReference(const UWORD mask[VISIBLE_WORDS],
                           const UWORD planes[PLANES][VISIBLE_WORDS],
                           int pair,UWORD data[2][DMA_WORDS])
{
    int x;
    memset(data,0,2*DMA_WORDS*sizeof(**data));
    for(x=0;x<VISIBLE_WORDS*16;x++) {
        int word=x>>4;
        UWORD bit=(UWORD)(0x8000U>>(x&15));
        unsigned pen=0;
        int plane;
        if(!(mask[word]&bit)) continue;
        for(plane=0;plane<PLANES;plane++)
            if(planes[plane][word]&bit) pen|=1U<<plane;
        if(pen&(1U<<(pair*2))) data[0][word]|=bit;
        if(pen&(2U<<(pair*2))) data[1][word]|=bit;
    }
}

static void wordCandidate(const UWORD mask[VISIBLE_WORDS],
                          const UWORD planes[PLANES][VISIBLE_WORDS],
                          int pair,UWORD data[2][DMA_WORDS])
{
    int word;
    memset(data,0,2*DMA_WORDS*sizeof(**data));
    for(word=0;word<VISIBLE_WORDS;word++) {
        data[0][word]=(UWORD)(planes[pair*2][word]&mask[word]);
        data[1][word]=(UWORD)(planes[pair*2+1][word]&mask[word]);
    }
}

int main(void)
{
    const UWORD mask[VISIBLE_WORDS]={0xffff,0x55aa,0x0ff0};
    UWORD planes[PLANES][VISIBLE_WORDS];
    UWORD reference[2][DMA_WORDS],candidate[2][DMA_WORDS];
    int pair,plane,word;
    for(plane=0;plane<PLANES;plane++)
        for(word=0;word<VISIBLE_WORDS;word++)
            planes[plane][word]=(UWORD)(0x7f4aU*(1+plane*5+word*3));
    for(pair=0;pair<2;pair++) {
        pixelReference(mask,planes,pair,reference);
        wordCandidate(mask,planes,pair,candidate);
        if(memcmp(reference,candidate,sizeof(reference))) {
            fprintf(stderr,"FAIL: attached pair %d differs\n",pair);
            return 1;
        }
        if(candidate[0][3]||candidate[1][3]) {
            fprintf(stderr,"FAIL: transparent fourth chunk is not zero\n");
            return 1;
        }
    }
    puts("PASS: player sprite word conversion matches pixel reference");
    return 0;
}
