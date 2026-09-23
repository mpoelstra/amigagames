/* Frame addresses prepared once after allocation. Pixel caches stay in Chip;
   this small pointer table lives in ordinary program data/Fast RAM. */
#define DROWNED_FRAME_LIMIT 32
struct DrownedEnemyFrame { UWORD *mask,*bits; };
static struct DrownedEnemyFrame drownedEnemyFrames[ENEMY_TYPE_COUNT][2][DROWNED_FRAME_LIMIT];
#ifdef SPARKPAW_DROWNED_ENEMY_BOUNDS
#if !defined(SPARKPAW_DROWNED_FULL) || !defined(SPARKPAW_DROWNED_RESIDENT_WALKER) || defined(SPARKPAW_DROWNED_FRAME_ADDRESS_REFERENCE)
#error Enemy_bounds_requires_full_resident_frame_table
#endif
/* Top in high byte, height in low byte. Preserve original cache plane stride. */
static UWORD drownedEnemyBounds[ENEMY_TYPE_COUNT][2][DROWNED_FRAME_LIMIT];
static UWORD drownedEnemyDrawnBounds[2][MAX_ENEMIES];
static UWORD drownedMaskBounds(const UWORD *mask,UWORD height,UWORD stride)
{
    UWORD top=height,bottom=0,y,x;
    for(y=0;y<height;y++) for(x=0;x<stride;x++) if(mask[y*stride+x]) {
        if(y<top)top=y;
        bottom=y+1;
        break;
    }
    /* Retain drawn/lifetime behavior even for an entirely transparent pose. */
    return bottom?(UWORD)((top<<8)|(bottom-top)):height;
}
#endif
static BOOL prepareDrownedEnemyFrames(struct EnemyBobCache *cache)
{
    WORD type=(WORD)(cache-enemyCaches);
    UBYTE facing,frame;
    LONG words=(LONG)cache->height*cache->sourceWords;
    if(type<0||type>=ENEMY_TYPE_COUNT||cache->frames>DROWNED_FRAME_LIMIT) return FALSE;
    for(facing=0;facing<2;facing++) for(frame=0;frame<cache->frames;frame++) {
        LONG pattern=(LONG)facing*cache->frames+frame;
        drownedEnemyFrames[type][facing][frame].mask=cache->mask+pattern*words;
        drownedEnemyFrames[type][facing][frame].bits=cache->bits+pattern*FRONT_PLANES*words;
#ifdef SPARKPAW_DROWNED_ENEMY_BOUNDS
        {
            UWORD bounds=drownedMaskBounds(drownedEnemyFrames[type][facing][frame].mask,
                                           cache->height,cache->sourceWords);
            LONG offset=(LONG)(bounds>>8)*cache->sourceWords;
            drownedEnemyBounds[type][facing][frame]=bounds;
            drownedEnemyFrames[type][facing][frame].mask+=offset;
            drownedEnemyFrames[type][facing][frame].bits+=offset;
        }
#endif
    }
    return TRUE;
}
