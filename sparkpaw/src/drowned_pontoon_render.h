/* Included after masked Bob helpers and waterPatternPen, isolated proof only. */
#include "drowned_pontoon.h"
#include "drowned_pontoon_art.h"
#if defined(SPARKPAW_DROWNED_FERRY) && !defined(SPARKPAW_PONTOON_OFFSET_REFERENCE)
#include "drowned_pontoon_offsets.h"
#endif
#define PONTOON_CLIP_BYTES (2L*16*80*56*2)
static UWORD *pontoonChip,*pontoonClip;
static BOOL pontoonDrawn[2];
static WORD pontoonOldWorld[2],pontoonOldPhysical[2],pontoonOldY[2];
static BOOL preparePontoon(void)
{
 BPTR file;LONG count;
#if defined(SPARKPAW_FOUR_ADF)||defined(SPARKPAW_DROWNED_THREE_ADF)||defined(SPARKPAW_WHD_PACKED)
 ULONG packedSize=0;
#endif
#if defined(SPARKPAW_DROWNED_FERRY) && !defined(SPARKPAW_PONTOON_OFFSET_REFERENCE)
 preparePontoonOffsets();
#endif
 pontoonChip=AllocMem(1120,MEMF_CHIP|MEMF_CLEAR);
#if defined(SPARKPAW_FOUR_ADF)||defined(SPARKPAW_DROWNED_THREE_ADF)||defined(SPARKPAW_WHD_PACKED)
 pontoonClip=(UWORD *)assetsLoadDiskData(
     "PROGDIR:assets/runtime/pontoon-clip.bin",MEMF_FAST,&packedSize);
#else
 pontoonClip=AllocMem(PONTOON_CLIP_BYTES,MEMF_FAST);
#endif
 if(!pontoonChip||!pontoonClip)return FALSE;
#if defined(SPARKPAW_FOUR_ADF)||defined(SPARKPAW_DROWNED_THREE_ADF)||defined(SPARKPAW_WHD_PACKED)
 if(packedSize!=PONTOON_CLIP_BYTES)return FALSE;
#endif
 CopyMem((APTR)pontoonArt,pontoonChip,1120);
 memset(pontoonDrawn,0,sizeof(pontoonDrawn));
#if !defined(SPARKPAW_FOUR_ADF)&&!defined(SPARKPAW_DROWNED_THREE_ADF)&&!defined(SPARKPAW_WHD_PACKED)
 file=Open("PROGDIR:assets/runtime/pontoon-clip.bin",MODE_OLDFILE);
 if(!file)return FALSE;
 count=Read(file,pontoonClip,PONTOON_CLIP_BYTES);Close(file);
 if(count!=PONTOON_CLIP_BYTES)return FALSE;
#endif
 return TRUE;
}
static void restorePontoon(void)
{
 UBYTE t=prototypePreparedCopper;
 if(pontoonDrawn[t]){
  blitRestoreRect(pontoonOldWorld[t],pontoonOldPhysical[t],pontoonOldY[t],96,16);
  pontoonDrawn[t]=FALSE;
 }
}
static void drawPontoon(void)
{
 WORD x=drownedPontoonX(),y=drownedPontoonDeck(),physical;
 UBYTE t=prototypePreparedCopper,frame=(game->frameCounter>>1)&15;
 LONG at;
 if(x+96<(WORD)game->cameraX-16||x>(WORD)game->cameraX+SCREEN_W+16||!prototypeRectFits(x,96))return;
#if defined(SPARKPAW_DROWNED_FERRY) && !defined(SPARKPAW_PONTOON_OFFSET_REFERENCE)
 at=pontoonMaskOffset(x,y,frame);
#else
 at=(((LONG)(y-189)*16+frame)*80+x%80)*56;
#endif
 platformWaitBlit();
 CopyMem(pontoonClip+at,pontoonChip+56,112);
 physical=prototypePhysicalX(x);
 blitMaskedBob(pontoonChip,pontoonChip+112,7,96,16,physical,y);
 pontoonDrawn[t]=TRUE;pontoonOldWorld[t]=x;pontoonOldPhysical[t]=physical;pontoonOldY[t]=y;
}
