#include "assets.h"

#include <exec/memory.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <string.h>

static struct PlanarAsset title,levelLoading,levelCharging,frontClean,rearWorld;
static struct PlanarAsset playerSprites,enemySprites,striderSprites;
static struct PlanarAsset hudBase,hudHealth,hudLives,hudDiamonds;
static struct PlanarAsset collectibleDiamond;

static UWORD readBigEndian16(const UBYTE *value)
{
    return (UWORD)(((UWORD)value[0]<<8)|value[1]);
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

static ULONG updateCRC32(ULONG crc,UBYTE value)
{
    UBYTE bit;
    crc^=value;
    for(bit=0;bit<8;bit++) crc=(crc>>1)^((crc&1)?0xedb88320UL:0);
    return crc;
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
        reader->crc=updateCRC32(reader->crc,value);
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
    if(asset->mask) {
        FreeMem(asset->mask,(LONG)asset->rowBytes*asset->height);
        asset->mask=NULL;
    }
    if(asset->bitmap) {
        FreeBitMap(asset->bitmap); asset->bitmap=NULL;
    }
}

static BOOL readRows(BPTR file,PLANEPTR plane,UWORD fileRow,UWORD memoryRow,
                     UWORD height)
{
    UWORD row;
    if(fileRow==memoryRow)
        return Read(file,plane,(LONG)fileRow*height)==(LONG)fileRow*height;
    for(row=0;row<height;row++)
        if(Read(file,plane+(LONG)row*memoryRow,fileRow)!=fileRow) return FALSE;
    return TRUE;
}

static BOOL loadAsset(const char *name,struct PlanarAsset *asset,
                      UBYTE wantedDepth)
{
    BPTR file; UBYTE header[12],plane; LONG size;
    memset(asset,0,sizeof(*asset));
    file=Open((STRPTR)name,MODE_OLDFILE); if(!file) return FALSE;
    if(Read(file,header,12)!=12||memcmp(header,"SPBM",4)!=0) {
        Close(file); return FALSE;
    }
    asset->width=readBigEndian16(header+4);
    asset->height=readBigEndian16(header+6);
    asset->depth=header[8]; asset->hasMask=header[9];
    asset->rowBytes=readBigEndian16(header+10);
    if(asset->depth!=wantedDepth||
       Read(file,asset->palette,(LONG)(1<<asset->depth)*3)!=
       (LONG)(1<<asset->depth)*3) {
        Close(file); return FALSE;
    }
    asset->bitmap=AllocBitMap(asset->width,asset->height,asset->depth,
                              BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    if(!asset->bitmap) { Close(file); return FALSE; }
    for(plane=0;plane<asset->depth;plane++)
        if(!readRows(file,asset->bitmap->Planes[plane],asset->rowBytes,
                     asset->bitmap->BytesPerRow,asset->height)) {
            Close(file); freeAsset(asset); return FALSE;
        }
    if(asset->hasMask) {
        size=(LONG)asset->rowBytes*asset->height;
        asset->mask=(UBYTE *)AllocMem(size,MEMF_CHIP);
        if(!asset->mask||Read(file,asset->mask,size)!=size) {
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
                            UBYTE wantedDepth)
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
    asset->bitmap=AllocBitMap(asset->width,asset->height,asset->depth,
                              BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    if(!asset->bitmap) goto done;
    for(plane=0;plane<asset->depth;plane++)
        if(!readPackedRows(&reader,asset->bitmap->Planes[plane],asset->rowBytes,
                           asset->bitmap->BytesPerRow,asset->height)) goto done;
    if(asset->hasMask) {
        size=(LONG)asset->rowBytes*asset->height;
        asset->mask=(UBYTE *)AllocMem(size,MEMF_CHIP);
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
                           &frontClean,4)&&
           loadPackedAsset("PROGDIR:assets/runtime/storm-rear.spr1",
                           &rearWorld,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-sprites4.spbm",
                     &playerSprites,4)&&
           loadAsset("PROGDIR:assets/runtime/clockwork-beetle.spbm",
                     &enemySprites,4)&&
           loadPackedAsset(
               "PROGDIR:assets/runtime/clockwork-storm-strider.spr1",
               &striderSprites,4)&&
#else
    return loadAsset("PROGDIR:assets/runtime/storm-front.spbm",&frontClean,4)&&
           loadAsset("PROGDIR:assets/runtime/storm-rear.spbm",&rearWorld,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-sprites4.spbm",
                     &playerSprites,4)&&
           loadAsset("PROGDIR:assets/runtime/clockwork-beetle.spbm",
                     &enemySprites,4)&&
           loadAsset("PROGDIR:assets/runtime/clockwork-storm-strider.spbm",
                     &striderSprites,4)&&
#endif
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-base.spbm",
                     &hudBase,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-health.spbm",
                     &hudHealth,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-lives.spbm",
                     &hudLives,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-hud-diamonds.spbm",
                     &hudDiamonds,3)&&
           loadAsset("PROGDIR:assets/runtime/sparkpaw-diamond.spbm",
                     &collectibleDiamond,4);
}

BOOL assetsLoadTitle(void)
{
    return loadAsset("PROGDIR:assets/runtime/sparkpaw-title.spbm",&title,6);
}

void assetsUnloadTitle(void)
{
    freeAsset(&title);
}

BOOL assetsLoadLevelLoading(void)
{
    return loadAsset("PROGDIR:assets/runtime/sparkpaw-level-loading.spbm",
                     &levelLoading,6);
}

void assetsUnloadLevelLoading(void)
{
    freeAsset(&levelLoading);
}

BOOL assetsLoadLevelCharging(void)
{
    return loadAsset("PROGDIR:assets/runtime/sparkpaw-level-charging.spbm",
                     &levelCharging,6);
}

void assetsUnloadLevelCharging(void)
{
    freeAsset(&levelCharging);
}

void assetsUnloadGameplay(void)
{
    freeAsset(&collectibleDiamond); freeAsset(&hudDiamonds);
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
