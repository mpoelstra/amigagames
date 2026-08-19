#include <assert.h>
#include "../src/projectile_sweep.h"

int main(void)
{
    short x=132,visited=0;
    while(x!=156) { x=projectileSweepNext(x,156); visited++; }
    assert(visited==24&&x==156);
    x=99; visited=0;
    while(x!=76) { x=projectileSweepNext(x,76); visited++; }
    assert(visited==23&&x==76);
    assert(projectileSweepNext(42,42)==42);
    return 0;
}
