#include <assert.h>
#include <stdio.h>

#include "../src/level_data.h"

int main(void)
{
    assert(!levelPlayerTouchesStormstoneCore(3180,120,3210,170));
    assert(levelPlayerTouchesStormstoneCore(3183,120,3213,170));
    assert(levelPlayerTouchesStormstoneCore(3193,120,3223,170));
    assert(!levelPlayerTouchesStormstoneCore(3194,120,3224,170));
    assert(!levelPlayerTouchesStormstoneCore(3183,60,3213,111));
    assert(levelPlayerTouchesStormstoneCore(3183,174,3213,200));
    assert(!levelPlayerTouchesStormstoneCore(3183,175,3213,200));
    puts("PASS: Stormstone Core trigger bounds");
    return 0;
}
