#include <assert.h>
#include <stdio.h>

#include "../src/camera_contract.h"

int main(void)
{
    const long playerX=1000;
    const long cameraX=cameraCenteredTarget(playerX);
    const long logicalPlayerWidth=32;

    assert(playerX-cameraX==144);
    assert(playerX-cameraX+logicalPlayerWidth/2==160);
    puts("PASS: gameplay camera centres Sparkpaw at screen x=160");
    return 0;
}
