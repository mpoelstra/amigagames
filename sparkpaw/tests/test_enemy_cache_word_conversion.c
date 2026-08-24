#include <stdio.h>
#include <string.h>

typedef unsigned short UWORD;
typedef unsigned char UBYTE;

#define PLANES 4
#define VISIBLE_WORDS 4
#define WORDS (VISIBLE_WORDS+1)

static void pixelReference(const UWORD sourceMask[2][WORDS],
                           const UWORD sourceBits[2][PLANES][WORDS],
                           UBYTE sourceFacing,UWORD targetMask[WORDS],
                           UWORD targetBits[PLANES][WORDS])
{
    int x,plane;
    memset(targetMask,0,WORDS*sizeof(*targetMask));
    memset(targetBits,0,PLANES*WORDS*sizeof(**targetBits));
    for(x=0;x<VISIBLE_WORDS*16;x++) {
        int word=x>>4;
        UWORD bit=(UWORD)(0x8000U>>(x&15));
        UBYTE pen=0;
        if(!(sourceMask[sourceFacing][word]&bit)) continue;
        for(plane=0;plane<PLANES;plane++)
            if(sourceBits[sourceFacing][plane][word]&bit)
                pen|=(UBYTE)(1U<<plane);
        if(!pen) continue;
        targetMask[word]|=bit;
        for(plane=0;plane<PLANES;plane++)
            if(pen&(1U<<plane)) targetBits[plane][word]|=bit;
    }
}

static void wordCandidate(const UWORD sourceMask[2][WORDS],
                          const UWORD sourceBits[2][PLANES][WORDS],
                          UBYTE sourceFacing,UWORD targetMask[WORDS],
                          UWORD targetBits[PLANES][WORDS])
{
    int word,plane;
    memset(targetMask,0,WORDS*sizeof(*targetMask));
    memset(targetBits,0,PLANES*WORDS*sizeof(**targetBits));
    for(word=0;word<VISIBLE_WORDS;word++) {
        UWORD opaque=0;
        for(plane=0;plane<PLANES;plane++) {
            UWORD bits=(UWORD)(sourceBits[sourceFacing][plane][word]&
                               sourceMask[sourceFacing][word]);
            targetBits[plane][word]=bits;
            opaque|=bits;
        }
        targetMask[word]=opaque;
    }
}

int main(void)
{
    UWORD sourceMask[2][WORDS]={{0xffff,0x55aa,0x0000,0xf00f,0xdead},
                                {0x7ffe,0xa55a,0xffff,0x0ff0,0xbeef}};
    UWORD sourceBits[2][PLANES][WORDS];
    UWORD referenceMask[WORDS],candidateMask[WORDS];
    UWORD referenceBits[PLANES][WORDS],candidateBits[PLANES][WORDS];
    int facing,plane,word;
    for(facing=0;facing<2;facing++)
        for(plane=0;plane<PLANES;plane++)
            for(word=0;word<WORDS;word++)
                sourceBits[facing][plane][word]=(UWORD)(
                    0x9e37U*(1+facing*19+plane*7+word*3));
    for(facing=0;facing<2;facing++) {
        pixelReference(sourceMask,sourceBits,(UBYTE)facing,
                       referenceMask,referenceBits);
        wordCandidate(sourceMask,sourceBits,(UBYTE)facing,
                      candidateMask,candidateBits);
        if(memcmp(referenceMask,candidateMask,sizeof(referenceMask))||
           memcmp(referenceBits,candidateBits,sizeof(referenceBits))) {
            fprintf(stderr,"FAIL: facing %d differs\n",facing);
            return 1;
        }
    }
    puts("PASS: word conversion matches pixel reference");
    return 0;
}
