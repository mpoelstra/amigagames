/* Runtime-only large camera jump. Startup still uses prototypeCopyInitial
   under OS ownership. Caller owns the inactive target and has retired actor
   restores; the canonical copier waits before setup and before returning. */
static void drownedCopyResetTarget(struct PrototypeTarget *target)
{
    WORD i;
    drownedCopyPatchRect(target,target->origin,0,PROTOTYPE_RING_W,WORLD_H);
    /* Match prototypeCopyInitial's history exactly. Pixel/state parity is
       tested against that actual function, including collected diamonds. */
    for(i=0;i<LEVEL_WATER_COUNT;i++) target->waterFrame[i]=waterDrawnFrame[i];
    for(i=0;i<MAX_COLLECTIBLES;i++) {
        struct Collectible *item=collectibleAt(i);
        target->collectibleDrawn[i]=FALSE;
        target->collectibleX[i]=0;
        target->collectibleY[i]=item->drawnY;
    }
}
