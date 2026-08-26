#include "assets.h"

#include <exec/memory.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <string.h>

#include "packed_crc32.h"

static struct PlanarAsset title,levelLoading,levelCharging,frontClean,rearWorld;
static struct PlanarAsset playerSprites,enemySprites,striderSprites;
static struct PlanarAsset hudBase,hudHealth,hudLives,hudDiamonds;
static struct PlanarAsset collectibleDiamond;
static struct PlanarAsset stormstoneCore;

static UWORD readBigEndian16(const UBYTE *value)
{
    return (UWORD)(((UWORD)value[0]<<8)|value[1]);
}

/* Match the real-hardware-proven ADF reader: DOS only sees a small ordinary
   buffer, never a complete 98 KiB plane or an arbitrary final DMA address.
   This makes raw HD loading independent of fragile MaxTransfer/Mask setups. */
#define RAW_READ_CHUNK 512L

static BOOL readExact(BPTR file,UBYTE *target,LONG size)
{
    UBYTE input[RAW_READ_CHUNK];
    while(size>0) {
        LONG wanted=size>RAW_READ_CHUNK?RAW_READ_CHUNK:size;
        LONG got=Read(file,input,wanted);
        if(got!=wanted) return FALSE;
        CopyMem(input,target,got);
        target+=got; size-=got;
    }
    return TRUE;
}

#ifdef ADF_PACKED_ASSETS
struct PackedReader {
    BPTR file;
    UBYTE input[512],value;
    UWORD inputAt,inputCount,tokenRemaining;
    ULONG packedRemaining,expectedSize,produced,expectedCRC,crc;
    BOOL run;
};

static ULONG readBigEndian32(const UBYTE *value)
{
    return ((ULONG)value[0]<<24)|((ULONG)value[1]<<16)|
           ((ULONG)value[2]<<8)|value[3];
}

static BOOL packedByte(struct PackedReader *reader,UBYTE *value)
{
    LONG wanted;
    if(reader->inputAt==reader->inputCount) {
        if(!reader->packedRemaining) return FALSE;
        wanted=reader->packedRemaining>sizeof(reader->input)?
               sizeof(reader->input):(LONG)reader->packedRemaining;
        if(Read(reader->file,reader->input,wanted)!=wanted) return FALSE;
        reader->inputAt=0; reader->inputCount=(UWORD)wanted;
        reader->packedRemaining-=wanted;
    }
    *value=reader->input[reader->inputAt++];
    return TRUE;
}

static BOOL packedOpen(const char *name,struct PackedReader *reader)
{
    UBYTE header[16];
    memset(reader,0,sizeof(*reader));
    reader->file=Open((STRPTR)name,MODE_OLDFILE);
    if(!reader->file) return FALSE;
    if(Read(reader->file,header,sizeof(header))!=sizeof(header)||
       memcmp(header,"SPR1",4)!=0) {
        Close(reader->file); reader->file=0; return FALSE;
    }
    reader->expectedSize=readBigEndian32(header+4);
    reader->expectedCRC=readBigEndian32(header+8);
    reader->packedRemaining=readBigEndian32(header+12);
    reader->crc=0xffffffffUL;
    return TRUE;
}

static BOOL packedRead(struct PackedReader *reader,UBYTE *target,LONG size)
{
    UBYTE token,value;
    while(size--) {
        if(!reader->tokenRemaining) {
            if(!packedByte(reader,&token)) return FALSE;
            reader->tokenRemaining=(UWORD)((token&0x7f)+1);
            reader->run=(token&0x80)!=0;
            if(reader->run&&!packedByte(reader,&reader->value)) return FALSE;
        }
        if(reader->run) value=reader->value;
        else if(!packedByte(reader,&value)) return FALSE;
        if(reader->produced>=reader->expectedSize) return FALSE;
        *target++=value;
        reader->crc=packedCRC32Byte(reader->crc,value);
        reader->produced++; reader->tokenRemaining--;
    }
    return TRUE;
}

static BOOL packedClose(struct PackedReader *reader,BOOL complete)
{
    BOOL valid=complete&&reader->produced==reader->expectedSize&&
        !reader->tokenRemaining&&!reader->packedRemaining&&
        reader->inputAt==reader->inputCount&&
        (reader->crc^0xffffffffUL)==reader->expectedCRC;
    if(reader->file) Close(reader->file);
    reader->file=0;
    return valid;
}
#endif

static void freeAsset(struct PlanarAsset *asset)
{
    UBYTE plane;
    if(asset->mask) {
        FreeMem(asset->mask,(LONG)asset->rowBytes*asset->height);
        asset->mask=NULL;
    }
    if(asset->bitmap) {
        if(asset->cpuOnlyBitmap) {
            LONG planeBytes=(LONG)asset->bitmap->BytesPerRow*asset->height;
            for(plane=0;plane<asset->depth;plane++)
                if(asset->bitmap->Planes[plane])
                    FreeMem(asset->bitmap->Planes[plane],planeBytes);
            FreeMem(asset->bitmap,sizeof(*asset->bitmap));
        } else FreeBitMap(asset->bitmap);
        asset->bitmap=NULL; asset->cpuOnlyBitmap=FALSE;
    }
}

static BOOL allocateAssetBitmap(struct PlanarAsset *asset,BOOL dmaSource)
{
    UBYTE plane;
    LONG planeBytes;
    if(dmaSource) {
        asset->bitmap=AllocBitMap(asset->width,asset->height,asset->depth,
                                  BMF_CLEAR|BMF_DISPLAYABLE,NULL);
        return asset->bitmap!=NULL;
    }
    asset->bitmap=(struct BitMap *)AllocMem(sizeof(*asset->bitmap),
                                            MEMF_FAST|MEMF_CLEAR);
    if(!asset->bitmap) return FALSE;
    asset->cpuOnlyBitmap=TRUE;
    InitBitMap(asset->bitmap,asset->depth,asset->width,asset->height);
    planeBytes=(LONG)asset->bitmap->BytesPerRow*asset->height;
    for(plane=0;plane<asset->depth;plane++) {
        asset->bitmap->Planes[plane]=(PLANEPTR)AllocMem(planeBytes,
                                                       MEMF_FAST|MEMF_CLEAR);
        if(!asset->bitmap->Planes[plane]) {
            freeAsset(asset); return FALSE;
        }
    }
    return TRUE;
}

static BOOL readRows(BPTR file,PLANEPTR plane,UWORD fileRow,UWORD memoryRow,
                     UWORD height)
{
    UWORD row;
    if(fileRow==memoryRow)
        return readExact(file,plane,(LONG)fileRow*height);
    for(row=0;row<height;row++)
        if(!readExact(file,plane+(LONG)row*memoryRow,fileRow)) return FALSE;
    return TRUE;
}

static BOOL loadAsset(const char *name,struct PlanarAsset *asset,
                      UBYTE wantedDepth,BOOL dmaSource)
{
    BPTR file; UBYTE header[12],plane; LONG size;
    memset(asset,0,sizeof(*asset));
    file=Open((STRPTR)name,MODE_OLDFILE); if(!file) return FALSE;
    if(!readExact(file,header,12)||memcmp(header,"SPBM",4)!=0) {
        Close(file); return FALSE;
    }
    asset->width=readBigEndian16(header+4);
    asset->height=readBigEndian16(header+6);
    asset->depth=header[8]; asset->hasMask=header[9];
    asset->rowBytes=readBigEndian16(header+10);
    if(asset->depth!=wantedDepth||
       !readExact(file,(UBYTE *)asset->palette,
                  (LONG)(1<<asset->depth)*3)) {
        Close(file); return FALSE;
    }
    if(!allocateAssetBitmap(asset,dmaSource)) { Close(file); return FALSE; }
    for(plane=0;plane<asset->depth;plane++)
        if(!readRows(file,asset->bitmap->Planes[plane],asset->rowBytes,
                     asset->bitmap->BytesPerRow,asset->height)) {
            Close(file); freeAsset(asset); return FALSE;
        }
    if(asset->hasMask) {
        size=(LONG)asset->rowBytes*asset->height;
        asset->mask=(UBYTE *)AllocMem(size,dmaSource?MEMF_CHIP:MEMF_FAST);
        if(!asset->mask||!readExact(file,asset->mask,size)) {
            Close(file); freeAsset(asset); return FALSE;
        }
    }
    Close(file); return TRUE;
}

#ifdef ADF_PACKED_ASSETS
static BOOL readPackedRows(struct PackedReader *reader,PLANEPTR plane,
                           UWORD fileRow,UWORD memoryRow,UWORD height)
{
    UWORD row;
    if(fileRow==memoryRow)
        return packedRead(reader,plane,(LONG)fileRow*height);
    for(row=0;row<height;row++)
        if(!packedRead(reader,plane+(LONG)row*memoryRow,fileRow)) return FALSE;
    return TRUE;
}

static BOOL loadPackedAsset(const char *name,struct PlanarAsset *asset,
                            UBYTE wantedDepth,BOOL dmaSource)
{
    struct PackedReader reader;
    UBYTE header[12],plane; LONG size; BOOL complete=FALSE;
    memset(asset,0,sizeof(*asset));
    if(!packedOpen(name,&reader)) return FALSE;
    if(!packedRead(&reader,header,12)||memcmp(header,"SPBM",4)!=0) goto done;
    asset->width=readBigEndian16(header+4);
    asset->height=readBigEndian16(header+6);
    asset->depth=header[8]; asset->hasMask=header[9];
    asset->rowBytes=readBigEndian16(header+10);
    if(asset->depth!=wantedDepth||
       !packedRead(&reader,(UBYTE *)asset->palette,(LONG)(1<<asset->depth)*3))
        goto done;
    if(!allocateAssetBitmap(asset,dmaSource)) goto done;
    for(plane=0;plane<asset->depth;plane++)
        if(!readPackedRows(&reader,asset->bitmap->Planes[plane],asset->rowBytes,
                           asset->bitmap->BytesPerRow,asset->height)) goto done;
    if(asset->hasMask) {
        size=(LONG)asset->rowBytes*asset->height;
        asset->mask=(UBYTE *)AllocMem(size,dmaSource?MEMF_CHIP:MEMF_FAST);
        if(!asset->mask||!packedRead(&reader,asset->mask,size)) goto done;
    }
    complete=TRUE;
done:
    if(!packedClose(&reader,complete)) complete=FALSE;
    if(!complete) freeAsset(asset);
    return complete;
}
#endif

BOOL assetsLoadGameplay(void)
{
#ifdef ADF_PACKED_ASSETS
    return loadPackedAsset("PROGDIR:assets/runtime/storm-front.spr1",
                           &frontClean,4,TRUE)&&
           loadPackedAsset("PROGDIR:assets/runtime/storm-rear.spr1",
                           &rearWorld,3,TRUE)&&
           loadPackedAsset("PROGDIR:assets/runtime/sparkpaw-sprites4.spr1",
                           &playerSprites,4,FALSE)&&
           loadAsset("PROGDIR:assets/runtime/clockwork-beetle.spbm",
                     &enemySprites,4,FALSE)&&
           loadPackedAsset(
               "PROGDIR:assets/runtime/clockwork-storm-strider.spr1",
               &striderSprites,4,FALSE)&&
#else
    return loadAsset("PROGDIR:assets/runtime/storm-front.spbm",&frontClean,4,TRUE)&&
           loadAsset("PROGDIR:assets/runtime/storm-rear.spbm",&rearWorld,3,TRUE)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-sprites4.spbm",
                     &playerSprites,4,FALSE)&&
           loadAsset("PROGDIR:assets/runtime/clockwork-beetle.spbm",
                     &enemySprites,4,FALSE)&&
           loadAsset("PROGDIR:assets/runtime/clockwork-storm-strider.spbm",
                     &striderSprites,4,FALSE)&&
#endif
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-base.spbm",
                     &hudBase,3,TRUE)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-health.spbm",
                     &hudHealth,3,TRUE)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-lives.spbm",
                     &hudLives,3,TRUE)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-diamonds.spbm",
                     &hudDiamonds,3,TRUE)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-diamond.spbm",
                     &collectibleDiamond,4,FALSE)&&
           loadAsset("PROGDIR:assets/runtime/stormstone-core.spbm",
                     &stormstoneCore,4,FALSE);
}

BOOL assetsLoadTitle(void)
{
    return loadAsset("PROGDIR:assets/runtime/sparkpaw-title.spbm",&title,6,TRUE);
}

void assetsUnloadGameplayConversionSources(void)
{
    /* These sheets are CPU-read only while rendererPrepareGameplay() converts
       them into the final hardware-sprite and Blitter cache layouts.  Keeping
       both representations in Chip RAM after conversion wastes 325,220 bytes. */
    freeAsset(&collectibleDiamond);
    freeAsset(&stormstoneCore);
    freeAsset(&striderSprites);
    freeAsset(&enemySprites);
    freeAsset(&playerSprites);
    freeAsset(&stormstoneCore);
}

void assetsUnloadTitle(void)
{
    freeAsset(&title);
}

BOOL assetsLoadLevelLoading(void)
{
    return loadAsset("PROGDIR:assets/runtime/sparkpaw-level-loading.spbm",
                     &levelLoading,6,TRUE);
}

void assetsUnloadLevelLoading(void)
{
    freeAsset(&levelLoading);
}

BOOL assetsLoadLevelCharging(void)
{
    return loadAsset(
        "PROGDIR:assets/runtime/level-charge-patch.spbm",
        &levelCharging,6,FALSE);
}

void assetsUnloadLevelCharging(void)
{
    freeAsset(&levelCharging);
}

void assetsUnloadGameplay(void)
{
    assetsUnloadGameplayConversionSources(); freeAsset(&hudDiamonds);
    freeAsset(&hudLives); freeAsset(&hudHealth); freeAsset(&hudBase);
    freeAsset(&striderSprites); freeAsset(&enemySprites);
    freeAsset(&playerSprites);
    freeAsset(&rearWorld); freeAsset(&frontClean);
}

const struct PlanarAsset *assetsTitle(void) { return &title; }
const struct PlanarAsset *assetsLevelLoading(void) { return &levelLoading; }
const struct PlanarAsset *assetsLevelCharging(void) { return &levelCharging; }
const struct PlanarAsset *assetsFrontClean(void) { return &frontClean; }
const struct PlanarAsset *assetsRearWorld(void) { return &rearWorld; }
const struct PlanarAsset *assetsPlayerSprites(void) { return &playerSprites; }
const struct PlanarAsset *assetsEnemySprites(void) { return &enemySprites; }
const struct PlanarAsset *assetsStriderSprites(void) { return &striderSprites; }
const struct PlanarAsset *assetsHudBase(void) { return &hudBase; }
const struct PlanarAsset *assetsHudHealth(void) { return &hudHealth; }
const struct PlanarAsset *assetsHudLives(void) { return &hudLives; }
const struct PlanarAsset *assetsHudDiamonds(void) { return &hudDiamonds; }
const struct PlanarAsset *assetsCollectibleDiamond(void)
{
    return &collectibleDiamond;
}
const struct PlanarAsset *assetsStormstoneCore(void) { return &stormstoneCore; }
