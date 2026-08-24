#include <assert.h>
#include <stdio.h>

#include "../src/sprite_stage_cache.h"

static unsigned int present(struct SpriteStageCacheState stages[2],
                            unsigned int *stage,unsigned char facing,
                            unsigned char frame)
{
    int copy;
    *stage^=1;
    copy=SPRITE_STAGE_CACHE_NEEDS_COPY(&stages[*stage],facing,frame);
    if(copy) SPRITE_STAGE_CACHE_COMMIT(&stages[*stage],facing,frame);
    return (unsigned int)copy;
}

int main(void)
{
    struct SpriteStageCacheState stages[2]={{0,0,0},{0,0,0}};
    unsigned int stage=0;

    /* Both alternating buffers must receive the initial pose exactly once. */
    assert(present(stages,&stage,0,3)==1);
    assert(present(stages,&stage,0,3)==1);
    assert(present(stages,&stage,0,3)==0);
    assert(present(stages,&stage,0,3)==0);

    /* Facing and frame transitions independently invalidate both buffers. */
    assert(present(stages,&stage,1,3)==1);
    assert(present(stages,&stage,1,3)==1);
    assert(present(stages,&stage,1,4)==1);
    assert(present(stages,&stage,1,4)==1);
    assert(present(stages,&stage,1,4)==0);

    /* A hidden interval does not advance the stage; resume stays coherent. */
    assert(SPRITE_STAGE_CACHE_NEEDS_COPY(&stages[stage],1,4)==0);
    assert(present(stages,&stage,0,4)==1);
    assert(present(stages,&stage,0,4)==1);

    puts("sprite stage cache tests passed");
    return 0;
}
