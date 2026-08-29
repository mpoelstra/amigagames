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
    assert(projectileSweepNextTileProbe(3,35,16)==16);
    assert(projectileSweepNextTileProbe(16,35,16)==32);
    assert(projectileSweepNextTileProbe(35,3,16)==31);
    assert(projectileSweepNextTileProbe(16,3,16)==15);
    assert(projectileSweepNextTileProbe(8,12,16)==12);
    assert(projectileSweepNextTileProbe(8,8,16)==8);
    assert(projectileSweepGeometryWins(10,20,1,15,1,15));
    assert(!projectileSweepGeometryWins(10,20,1,16,1,15));
    assert(projectileSweepGeometryWins(20,10,1,15,1,15));
    assert(!projectileSweepGeometryWins(20,10,1,14,1,15));
    assert(projectileSweepGeometryWins(10,20,1,18,0,0));
    assert(!projectileSweepGeometryWins(10,20,0,0,1,18));
    return 0;
}
