#include <assert.h>
#include <stdio.h>

#include "../src/collision_hazard_cache.h"

#define TEST_WORLD_W 3392

static int referenceHazard(short x)
{
    return (x>=1584&&x<1664)||(x>=2432&&x<2512)||
           (x>=2112&&x<=2175)||(x>=2784&&x<=2863);
}

int main(void)
{
    unsigned char cache[TEST_WORLD_W];
    short x;
    collisionBuildHazardCache(cache,TEST_WORLD_W,referenceHazard);
    for(x=0;x<TEST_WORLD_W;x++)
        assert(collisionHazardCached(cache,x)==referenceHazard(x));
    puts("PASS: cached hazard columns match every reference column");
    return 0;
}
