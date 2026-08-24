#ifndef SPARKPAW_COLLISION_HAZARD_CACHE_H
#define SPARKPAW_COLLISION_HAZARD_CACHE_H

typedef int (*CollisionHazardPredicate)(short x);

static void collisionBuildHazardCache(unsigned char *cache,short width,
                                      CollisionHazardPredicate predicate)
{
    short x;
    for(x=0;x<width;x++) cache[x]=(unsigned char)(predicate(x)!=0);
}

static int collisionHazardCached(const unsigned char *cache,short x)
{
    return cache[x]!=0;
}

#endif
