/* Exact repeated80px authored pattern; no animation-rate or pixel reduction. */
#define DROWNED_WATER_BATCH_TILES 6
#define DROWNED_WATER_BATCH_WORDS (WATER_WORDS*DROWNED_WATER_BATCH_TILES)
#define DROWNED_WATER_BATCH_BYTES (WATER_FRAMES*FRONT_PLANES*WATER_H*DROWNED_WATER_BATCH_WORDS*2L)
static UWORD *drownedWaterBatchBits;
static BOOL prepareDrownedWaterBatch(void)
{
    LONG row; UBYTE tile;
    drownedWaterBatchBits=AllocMem(DROWNED_WATER_BATCH_BYTES,MEMF_CHIP);
    if(!drownedWaterBatchBits) return FALSE;
    for(row=0;row<WATER_FRAMES*FRONT_PLANES*WATER_H;row++)
        for(tile=0;tile<DROWNED_WATER_BATCH_TILES;tile++)
            CopyMem(waterBits+row*WATER_WORDS,
                    drownedWaterBatchBits+row*DROWNED_WATER_BATCH_WORDS+tile*WATER_WORDS,
                    WATER_WORDS*2);
    return TRUE;
}
static void blitDrownedWaterRun(UBYTE frame,WORD x,UBYTE count)
{
    struct BitMap *target=frontClean->bitmap;
    UBYTE plane;
    WORD words=count*WATER_WORDS;
    LONG at=(LONG)WATER_Y*target->BytesPerRow+(x>>3);
    LONG source=(LONG)frame*FRONT_PLANES*WATER_H*DROWNED_WATER_BATCH_WORDS;
    UWORD size=(WATER_H<<6)|words;
    for(plane=0;plane<FRONT_PLANES;plane++) {
        platformWaitBlit();
        if(!plane) {
            hw->bltcon0=0x09f0;hw->bltcon1=0;
            hw->bltafwm=0xffff;hw->bltalwm=0xffff;
            hw->bltamod=(DROWNED_WATER_BATCH_WORDS-words)*2;
            hw->bltdmod=target->BytesPerRow-words*2;
        }
        hw->bltapt=drownedWaterBatchBits+source;
        hw->bltdpt=target->Planes[plane]+at;
        hw->bltsize=size;
        source+=WATER_H*DROWNED_WATER_BATCH_WORDS;
    }
}
static void animateDrownedWaterBatch(void)
{
    UBYTE i=0,frame=(UBYTE)((game->frameCounter>>1)&(WATER_FRAMES-1));
    while(i<LEVEL_WATER_COUNT) {
        UBYTE first,count;
        WORD left=levelWaterLeft(i);
        if(!waterUpdateVisible(left,game->cameraX,SCREEN_W,WATER_W,16)||
           waterDrawnFrame[i]==frame) {i++;continue;}
        first=i++; count=1;
        while(i<LEVEL_WATER_COUNT&&count<DROWNED_WATER_BATCH_TILES&&
              levelWaterLeft(i)==left+count*WATER_W&&
              waterDrawnFrame[i]!=frame&&
              waterUpdateVisible(levelWaterLeft(i),game->cameraX,SCREEN_W,WATER_W,16)) {
            i++;count++;
        }
        blitDrownedWaterRun(frame,left,count);
        while(first<i) {
            waterDrawnFrame[first++]=frame;
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
            diagnosticFrame.waterUpdates++;
#endif
        }
    }
}
