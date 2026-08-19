#ifndef SPARKPAW_PROJECTILE_SWEEP_H
#define SPARKPAW_PROJECTILE_SWEEP_H

static short projectileSweepNext(short current,short end)
{
    if(current<end) return (short)(current+1);
    if(current>end) return (short)(current-1);
    return current;
}

#endif
