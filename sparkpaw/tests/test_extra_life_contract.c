#include <assert.h>
#include <stdio.h>

#include "../src/extra_life_contract.h"

int main(void)
{
    int plane,row;
    int y=EXTRA_LIFE_START_Y;
    for(plane=0;plane<4;plane++) for(row=0;row<EXTRA_LIFE_H;row++) {
        long offset=extraLifePlaneWordOffset((unsigned char)plane,row,2);
        assert(offset==(long)plane*EXTRA_LIFE_H*2+(long)row*2);
        assert(offset>=0&&offset+1<4L*EXTRA_LIFE_H*2);
    }
    assert(!extraLifeShouldReveal(EXTRA_LIFE_HIDDEN,3220,3240));
    assert(extraLifeShouldReveal(EXTRA_LIFE_HIDDEN,3240,3256));
    assert(!extraLifeShouldReveal(EXTRA_LIFE_READY,3240,3256));
    while(y<EXTRA_LIFE_GROUND_Y) y=extraLifeDropY(y);
    assert(y==EXTRA_LIFE_GROUND_Y);
    assert(extraLifeDropY(y)==EXTRA_LIFE_GROUND_Y);
    assert(extraLifeTouches(3320,178,3340,200,y));
    assert(!extraLifeTouches(3300,178,3327,200,y));
    puts("extra-life secret contract passed");
    return 0;
}
