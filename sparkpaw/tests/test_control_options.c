#include <assert.h>
#include <stdio.h>

#include "../src/control_options.h"

int main(void)
{
    assert(secondaryButtonAddsJump(SECONDARY_BUTTON_JUMP,1));
    assert(!secondaryButtonAddsFire(SECONDARY_BUTTON_JUMP,1));
    assert(!secondaryButtonAddsJump(SECONDARY_BUTTON_FIRE,1));
    assert(secondaryButtonAddsFire(SECONDARY_BUTTON_FIRE,1));
    assert(!secondaryButtonAddsJump(SECONDARY_BUTTON_JUMP,0));
    assert(!secondaryButtonAddsFire(SECONDARY_BUTTON_FIRE,0));
    puts("PASS: secondary button mapping is exclusive and defaults safely");
    return 0;
}
