#include <stdio.h>

typedef short WORD;
typedef int BOOL;

#define ENEMY_W 32
#define ENEMY_H 24
#include "../src/beetle_hitbox.h"

static int expect(const char *name,BOOL actual,BOOL wanted)
{
    if(actual==wanted) {
        printf("PASS: %s\n",name);
        return 0;
    }
    fprintf(stderr,"FAIL: %s (got %d, wanted %d)\n",name,actual,wanted);
    return 1;
}

int main(void)
{
    const WORD shotX=116;
    const WORD standingShotY=171;
    const WORD crouchShotY=185;
    const WORD beetleX=100;
    int failures=0;

    failures+=expect("standing shot hits elevated beetle",
        beetleHitboxContains(shotX,standingShotY,beetleX,152),1);
    failures+=expect("same standing shot misses floor beetle by Y",
        beetleHitboxContains(shotX,standingShotY,beetleX,176),0);
    failures+=expect("crouch shot still hits floor beetle",
        beetleHitboxContains(shotX,crouchShotY,beetleX,176),1);
    return failures?1:0;
}
