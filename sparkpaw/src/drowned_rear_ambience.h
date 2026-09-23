/* Full Drowned-only, bounded rear animation. Included after Copper helpers. */
#include "drowned_rear_ambience_data.h"
static UBYTE *drownedRearFrames,*drownedRearStage;
static UBYTE drownedRearPhase,drownedRearTick,drownedRearWaterFrame;
static UBYTE drownedRearFallFrame[DRA_FALL_COUNT],drownedRearNextFall;
static WORD drownedRearWaterLeft;
static ULONG drownedRearLastTick;
static UWORD drownedShoreSlot[2][12];
static UBYTE drownedShoreDrawn[2];

static void freeDrownedRearAmbience(void)
{
    if(drownedRearFrames) FreeMem(drownedRearFrames,DRA_BYTES);
    if(drownedRearStage) FreeMem(drownedRearStage,DRA_STAGE_BYTES);
    drownedRearFrames=drownedRearStage=NULL;
}
static BOOL prepareDrownedRearAmbience(void)
{
    BPTR file; LONG size;
#if defined(SPARKPAW_FOUR_ADF)||defined(SPARKPAW_DROWNED_THREE_ADF)||defined(SPARKPAW_WHD_PACKED)
    ULONG packedSize=0;
    drownedRearFrames=assetsLoadDiskData(
        "PROGDIR:assets/runtime/drowned-amb.bin",MEMF_FAST,&packedSize);
    if(!drownedRearFrames||packedSize!=DRA_BYTES) return FALSE;
#else
    drownedRearFrames=AllocMem(DRA_BYTES,MEMF_FAST);
#endif
    drownedRearStage=AllocMem(DRA_STAGE_BYTES,MEMF_CHIP);
    if(!drownedRearFrames||!drownedRearStage) return FALSE;
#if !defined(SPARKPAW_FOUR_ADF)&&!defined(SPARKPAW_DROWNED_THREE_ADF)&&!defined(SPARKPAW_WHD_PACKED)
    file=Open("PROGDIR:assets/runtime/drowned-amb.bin",MODE_OLDFILE);
    if(!file) return FALSE;
    size=Read(file,drownedRearFrames,DRA_BYTES); Close(file);
    if(size!=DRA_BYTES||memcmp(drownedRearFrames,"DRA1",4)) return FALSE;
#else
    if(memcmp(drownedRearFrames,"DRA1",4)) return FALSE;
#endif
    drownedRearPhase=drownedRearTick=drownedRearNextFall=0;
    drownedRearLastTick=game->frameCounter;
    drownedRearWaterFrame=255;drownedRearWaterLeft=-1;
    memset(drownedRearFallFrame,255,sizeof(drownedRearFallFrame));
    memset(drownedShoreDrawn,255,sizeof(drownedShoreDrawn));
    return TRUE;
}
/* Two pens, full RGB8, with explicit restoration of both nibbles. */
static void drownedShoreColors(UBYTE list,UBYTE band,BOOL remember)
{
    UBYTE nibble,pen; UWORD value,index;
    for(nibble=0;nibble<2;nibble++) {
        cmove(0x106,nibble?0x1220:0x1020);
        for(pen=0;pen<2;pen++) {
            index=(UWORD)(band*4+pen*2+nibble);
            value=remember?drownedShoreValues[0][index]:(pen?0x368:0x247);
            cmove((UWORD)(pen?0x1ac:0x1a8),value);
            if(remember) drownedShoreSlot[list][index]=copPos-1;
        }
    }
    cmove(0x106,0x1020);
}
static void buildDrownedShoreCopper(UBYTE list)
{
    UBYTE band;
    for(band=0;band<3;band++) {
        cop[copPos++]=(UWORD)(((44+184+band*2-1)<<8)|0xd9);
        cop[copPos++]=0xfffe;
        drownedShoreColors(list,band,TRUE);
    }
    cop[copPos++]=(UWORD)(((44+190-1)<<8)|0xd9);cop[copPos++]=0xfffe;
    drownedShoreColors(list,0,FALSE);
}
static void updateDrownedShoreCopper(void)
{
    UBYTE index;
    if(drownedRearLastTick!=game->frameCounter) {
        drownedRearLastTick=game->frameCounter;
        if(++drownedRearTick==6) {
            drownedRearTick=0;
            if(++drownedRearPhase==24) drownedRearPhase=0;
        }
    }
    if(drownedShoreDrawn[prototypePreparedCopper]==drownedRearPhase) return;
    for(index=0;index<12;index++)
        cop[drownedShoreSlot[prototypePreparedCopper][index]]=
            drownedShoreValues[drownedRearPhase][index];
    drownedShoreDrawn[prototypePreparedCopper]=drownedRearPhase;
}
/* Source is precomputed Fast data. Only the private DMA stage gets CPU writes.
   Updates run immediately AFTER publication at PAL line0, long before the
   first animated rear row112 is fetched. A late caller defers rather than
   touching the displayed rows near the beam. */
#ifdef SPARKPAW_DROWNED_REAR_DIRECT_WATER
#if !defined(SPARKPAW_DROWNED_FULL) || (!defined(SPARKPAW_AGA32_LEFT_GUARD) && !defined(SPARKPAW_FMODE0_EARLY_WORD_GUARD))
#error Direct_rear_water_requires_separate_guarded_display
#endif
/* The Copper fetches rearDisplay exclusively in this configuration. Retire
   DMA first, then write the private canonical bitmap directly from Fast.
   It becomes the source for three display blits instead of routing the same
   bytes through the small stage and three additional canonical blits.
   Both bitmaps still receive exactly the original rectangle. */
static void copyDrownedRearWater(const UBYTE *source,WORD sourceStride,
                                 WORD x,WORD y,WORD bytes,WORD height)
{
    struct BitMap *canonical=rearWorld->bitmap;
    LONG at=(LONG)y*canonical->BytesPerRow+(x>>3);
    LONG displayAt=(LONG)y*rearDisplay->BytesPerRow+(x>>3)+PLAYFIELD_GUARD_BYTES;
    UBYTE plane; WORD row;
    /* Caller has already retired every use of the canonical source. */
    for(plane=0;plane<3;plane++) {
        UBYTE *dest=canonical->Planes[plane]+at;
        for(row=0;row<height;row++) {
            CopyMem((APTR)source,dest,bytes);
            source+=sourceStride;
            dest+=canonical->BytesPerRow;
        }
    }
    hw->bltcon0=0x09f0; hw->bltcon1=0;
    hw->bltafwm=0xffff;
    hw->bltalwm=0xffff;
    hw->bltamod=canonical->BytesPerRow-bytes;
    hw->bltdmod=rearDisplay->BytesPerRow-bytes;
    for(plane=0;plane<3;plane++) {
        platformWaitBlit();
        hw->bltapt=canonical->Planes[plane]+at;
        hw->bltdpt=rearDisplay->Planes[plane]+displayAt;
        hw->bltsize=(UWORD)((height<<6)|(bytes>>1));
    }
    platformWaitBlit();
}
#endif
static void copyDrownedRearRect(const UBYTE *source,WORD sourceStride,
                               WORD x,WORD y,WORD bytes,WORD height)
{
    UBYTE plane,copy;WORD row;
    LONG sourcePlane=(LONG)sourceStride*height;
    LONG stagePlane=(LONG)bytes*height;
    platformWaitBlit();
#ifdef SPARKPAW_DROWNED_REAR_DIRECT_WATER
    if(sourceStride!=bytes) {
        copyDrownedRearWater(source,sourceStride,x,y,bytes,height);
        return;
    }
#endif
    if(sourceStride==bytes) CopyMem((APTR)source,drownedRearStage,stagePlane*3);
#ifdef SPARKPAW_DROWNED_REAR_STAGE_REFERENCE
    else for(plane=0;plane<3;plane++)
            for(row=0;row<height;row++)
                CopyMem((APTR)(source+plane*sourcePlane+(LONG)row*sourceStride),
                        drownedRearStage+plane*stagePlane+(LONG)row*bytes,bytes);
#else
    else {
        /* Planes follow consecutively in both layouts. Advance the row
           pointers instead of recomputing two products for all 42 water rows.
           Same CopyMem calls/bytes, private Chip stage and DMA barriers. */
        UBYTE *dest=drownedRearStage;
        WORD rows=(WORD)(height*3);
        for(row=0;row<rows;row++) {
            CopyMem((APTR)source,dest,bytes);
            source+=sourceStride;
            dest+=bytes;
        }
    }
#endif
    hw->bltcon0=0x09f0;hw->bltcon1=0;
    /* Separate writes: vbcc reads the volatile RHS back for chained
       assignments, but BLTALWM is a write-only custom register. */
    hw->bltafwm=0xffff;
    hw->bltalwm=0xffff;
    hw->bltamod=0;
    for(copy=0;copy<2;copy++) {
        struct BitMap *dest=copy?rearDisplay:rearWorld->bitmap;
        LONG at=(LONG)y*dest->BytesPerRow+(x>>3)+(copy?PLAYFIELD_GUARD_BYTES:0);
        platformWaitBlit();
        hw->bltdmod=dest->BytesPerRow-bytes;
        for(plane=0;plane<3;plane++) {
            platformWaitBlit();
            hw->bltapt=drownedRearStage+plane*stagePlane;
            hw->bltdpt=dest->Planes[plane]+at;
            hw->bltsize=(UWORD)((height<<6)|(bytes>>1));
        }
    }
    platformWaitBlit();
}
static void publishDrownedRearAmbience(void)
{
    WORD left,bytes;UBYTE attempt,index,frame;
    if(!drownedRearFrames||platformRasterLine()>64) return;
    left=(WORD)((game->cameraX>>2)&~15);
    bytes=(WORD)((DRA_WIDTH-left)>>3);
    if(bytes>DRA_STAGE_STRIDE) bytes=DRA_STAGE_STRIDE;
    if(left!=drownedRearWaterLeft||drownedRearWaterFrame!=drownedRearPhase) {
        copyDrownedRearRect(drownedRearFrames+DRA_WATER_OFFSET+
            (LONG)drownedRearPhase*DRA_WATER_FRAME_BYTES+(left>>3),
            DRA_WATER_STRIDE,left,185,bytes,14);
        drownedRearWaterLeft=left;drownedRearWaterFrame=drownedRearPhase;
        return; /* Never stack waterfall DMA on the water update. */
    }
    for(attempt=0;attempt<DRA_FALL_COUNT;attempt++) {
        const struct DrownedRearFall *fall;
        index=drownedRearNextFall++;
        if(drownedRearNextFall==DRA_FALL_COUNT) drownedRearNextFall=0;
        fall=&drownedRearFalls[index];
        if(fall->x+fall->width<=left||fall->x>=left+352) continue;
        frame=(UBYTE)((drownedRearPhase+index)&3);
        if(drownedRearFallFrame[index]==frame) continue;
        copyDrownedRearRect(drownedRearFrames+fall->offset+
            (LONG)frame*fall->frameBytes,fall->stride,fall->x,fall->y,
            fall->stride,fall->height);
        drownedRearFallFrame[index]=frame;
        return; /* At most one small cascade per published frame. */
    }
}
