#include <assert.h>

#include "../src/projectile_render_visibility.h"

int main(void)
{
    assert(projectileRenderVisible(90,16,100,320,16));
    assert(projectileRenderVisible(420,16,100,320,16));
    assert(!projectileRenderVisible(67,16,100,320,16));
    assert(!projectileRenderVisible(453,16,100,320,16));
    return 0;
}
