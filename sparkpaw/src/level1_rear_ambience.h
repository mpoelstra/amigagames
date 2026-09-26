/* Level1 ambience: immutable source + two complete guarded rear targets.
   The inactive rear is finished BEFORE its Copper list can be published. */
#ifdef SPARKPAW_LEVEL1_REAR_AMBIENCE_RELEASE
#include "level1_rear_ambience_release_data.h"
#else
#include "level1_rear_ambience_data.h"
#endif
#if !defined(SPARKPAW_LEVEL1_RENDERER_TU_ISOLATION) || !defined(SPARKPAW_ROLLING_PROTOTYPE) || !defined(SPARKPAW_AGA32_LEFT_GUARD)
#error Level1_rear_ambience_requires_isolated_guarded_rolling_renderer
#endif
static struct BitMap *l1RearBuffers[2];
static UBYTE *l1RearFrames,*l1RearStage;
static UBYTE l1RearDrawn[2][L1_REAR_PATCHES];
#if defined(SPARKPAW_DROWNED_FPS)||defined(SPARKPAW_LEVEL1_REAR_HOST_TEST)
#define L1_REAR_COUNTERS
static ULONG l1RearUploads,l1RearBytes,l1RearMaxBytes,l1RearUnsafe;
static UWORD l1RearMaxPatches;
#endif

static void freeLevel1RearAmbience(void)
{
    platformWaitBlit();
    if(l1RearBuffers[1]) FreeBitMap(l1RearBuffers[1]);
    /* rendererCleanup still owns the original guarded allocation. */
    if(l1RearBuffers[0]) rearDisplay=l1RearBuffers[0];
    l1RearBuffers[0]=l1RearBuffers[1]=NULL;
    if(l1RearFrames) FreeMem(l1RearFrames,L1_REAR_BYTES);
    if(l1RearStage) FreeMem(l1RearStage,L1_REAR_STAGE_BYTES);
    l1RearFrames=l1RearStage=NULL;
}

static BOOL prepareLevel1RearAmbience(void)
{
    UBYTE p; UWORD row;
#if defined(SPARKPAW_MULTI_ADF)||defined(SPARKPAW_WHD_PACKED)
    ULONG size;
#else
    BPTR file; UBYTE extra;
#endif
    const struct BitMap *source=rearWorld->bitmap;
    l1RearBuffers[0]=rearDisplay;
    l1RearBuffers[1]=AllocBitMap((UWORD)(rearDisplay->BytesPerRow*8),
        rearWorld->height,REAR_PLANES,BMF_CLEAR|BMF_DISPLAYABLE,NULL);
    if(!l1RearBuffers[1]||l1RearBuffers[1]->BytesPerRow!=rearDisplay->BytesPerRow)
        return FALSE;
    for(p=0;p<REAR_PLANES;p++) {
        if((ULONG)l1RearBuffers[1]->Planes[p]&3) return FALSE;
        for(row=0;row<rearWorld->height;row++)
            CopyMem(source->Planes[p]+(LONG)row*source->BytesPerRow,
                l1RearBuffers[1]->Planes[p]+(LONG)row*rearDisplay->BytesPerRow+
                PLAYFIELD_GUARD_BYTES,source->BytesPerRow);
    }
#if defined(SPARKPAW_MULTI_ADF)||defined(SPARKPAW_WHD_PACKED)
    /* The established reader owns ADF decompression and WHD bank access. */
    l1RearFrames=assetsLoadDiskData("PROGDIR:assets/runtime/l1-electric.bin",MEMF_FAST,&size);
    if(!l1RearFrames) return FALSE;
    if(size!=L1_REAR_BYTES) {
        FreeMem(l1RearFrames,size); l1RearFrames=NULL; return FALSE;
    }
    if(memcmp(l1RearFrames,"L1A3",4)) return FALSE;
    l1RearStage=AllocMem(L1_REAR_STAGE_BYTES,MEMF_CHIP);
    if(!l1RearStage) return FALSE;
#else
    l1RearFrames=AllocMem(L1_REAR_BYTES,MEMF_FAST);
    l1RearStage=AllocMem(L1_REAR_STAGE_BYTES,MEMF_CHIP);
    if(!l1RearFrames||!l1RearStage) return FALSE;
    file=Open("PROGDIR:assets/runtime/l1-electric.bin",MODE_OLDFILE);
    if(!file) return FALSE;
    if(Read(file,l1RearFrames,L1_REAR_BYTES)!=L1_REAR_BYTES||
       Read(file,&extra,1)!=0||memcmp(l1RearFrames,"L1A3",4)) {
        Close(file); return FALSE;
    }
    Close(file);
#endif
    memset(l1RearDrawn,255,sizeof(l1RearDrawn));
#ifdef L1_REAR_COUNTERS
    l1RearUploads=l1RearBytes=l1RearMaxBytes=l1RearUnsafe=0;
    l1RearMaxPatches=0;
#endif
    return TRUE;
}

static void updateLevel1RearAmbience(void)
{
    UBYTE i,p,state,phase=(UBYTE)((game->frameCounter/6UL)%L1_REAR_PHASES);
    UBYTE target=prototypePreparedCopper;
    WORD left=(WORD)(game->cameraX>>2)-32;
    WORD right=(WORD)(game->cameraX>>2)+384;
#ifdef L1_REAR_COUNTERS
    ULONG copied=0; UWORD patches=0;
#endif
    struct BitMap *dest=l1RearBuffers[target];
    if(target==prototypeActiveCopper||dest==l1RearBuffers[prototypeActiveCopper]||
       rearDisplay!=dest) {
#ifdef L1_REAR_COUNTERS
        l1RearUnsafe++;
#endif
        return;
    }
    for(i=0;i<L1_REAR_PATCHES;i++) {
        const struct L1RearPatch *patch=&l1RearPatches[i];
        LONG at,planeBytes;
        if((WORD)(patch->x+patch->stride*8)<=left||(WORD)patch->x>=right) continue;
        state=patch->states[phase];
        if(l1RearDrawn[target][i]==state) continue;
        /* Stage is reused only after its previous reader has completed. */
        platformWaitBlit();
        CopyMem(l1RearFrames+patch->offset+(ULONG)state*patch->frameBytes,
                l1RearStage,patch->frameBytes);
        hw->bltcon0=0x09f0; hw->bltcon1=0;
        hw->bltafwm=0xffff; hw->bltalwm=0xffff;
        hw->bltamod=0; hw->bltdmod=dest->BytesPerRow-patch->stride;
        at=(LONG)patch->y*dest->BytesPerRow+patch->x/8+PLAYFIELD_GUARD_BYTES;
        planeBytes=(LONG)patch->stride*patch->height;
        for(p=0;p<REAR_PLANES;p++) {
            platformWaitBlit();
            hw->bltapt=l1RearStage+p*planeBytes;
            hw->bltdpt=dest->Planes[p]+at;
            hw->bltsize=(UWORD)((patch->height<<6)|(patch->stride>>1));
        }
        platformWaitBlit();
        l1RearDrawn[target][i]=state;
#ifdef L1_REAR_COUNTERS
        copied+=patch->frameBytes; patches++; l1RearUploads++;
#endif
    }
#ifdef L1_REAR_COUNTERS
    l1RearBytes+=copied;
    if(copied>l1RearMaxBytes) l1RearMaxBytes=copied;
    if(patches>l1RearMaxPatches) l1RearMaxPatches=patches;
#endif
}

#ifdef SPARKPAW_DROWNED_FPS
void level1RearWriteDiagnostic(BPTR file)
{
    FPrintf(file,"rear_ambience=v3_double_buffer unsafe=%ld uploads=%ld copied_bytes=%ld max_bytes_frame=%ld max_patches_frame=%ld\n",
        l1RearUnsafe,l1RearUploads,l1RearBytes,l1RearMaxBytes,(ULONG)l1RearMaxPatches);
    FPrintf(file,"rear_extra_chip_payload=%ld rear_fast_frames=%ld stage_bytes=%ld\n",
        (ULONG)l1RearBuffers[1]->BytesPerRow*rearWorld->height*3+L1_REAR_STAGE_BYTES,
        (ULONG)L1_REAR_BYTES,(ULONG)L1_REAR_STAGE_BYTES);
}
#endif
