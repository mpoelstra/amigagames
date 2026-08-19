#include <assert.h>
#include "../src/water_update_visibility.h"

int main(void)
{
    assert(!waterUpdateVisible(1584,810,320,80,16));
    assert(!waterUpdateVisible(2432,810,320,80,16));
    assert(waterUpdateVisible(1584,1248,320,80,16));
    assert(waterUpdateVisible(1584,1680,320,80,16));
    assert(!waterUpdateVisible(1584,1681,320,80,16));
    return 0;
}
