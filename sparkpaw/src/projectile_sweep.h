#ifndef SPARKPAW_PROJECTILE_SWEEP_H
#define SPARKPAW_PROJECTILE_SWEEP_H

static short projectileSweepNext(short current,short end)
{
    if(current<end) return (short)(current+1);
    if(current>end) return (short)(current-1);
    return current;
}

static short projectileSweepNextTileProbe(short current,short end,
                                          short tileSize)
{
    short next;
    if(current<end) {
        next=(short)((current&~(tileSize-1))+tileSize);
        return next>end?end:next;
    }
    if(current>end) {
        next=(short)((current&~(tileSize-1))-1);
        return next<end?end:next;
    }
    return current;
}

static int projectileSweepGeometryWins(short start,short end,
                                       int solidFound,short solidX,
                                       int enemyFound,short enemyX)
{
    if(!solidFound) return 0;
    if(!enemyFound) return 1;
    return start<=end?solidX<=enemyX:solidX>=enemyX;
}

#endif
