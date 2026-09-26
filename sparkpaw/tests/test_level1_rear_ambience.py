"""Exercise actual candidate C loader/Blitter kernel with two guarded targets."""
from pathlib import Path
import subprocess,tempfile,json,sys
from PIL import Image
R=Path(__file__).resolve().parents[1];O=R/'build/level1-electric-v3-20260926'
sys.path.insert(0,str(R/'tools'))
from preview_level1_rear_ambience import decode,indexed_image
blob=(O/'assets/l1-electric.bin').read_bytes()
manifest=json.loads((O/'asset-manifest.json').read_text())
base=indexed_image(decode(R/'assets/runtime/storm-rear.spbm')[0])
decoded={}
for patch_id,patch in enumerate(manifest['patches']):
    w,h,stride=patch['width'],patch['height'],patch['stride']
    for state in range(patch['count']):
        off=patch['offset']+state*patch['frame_bytes']
        image=Image.new('L',(w,h))
        image.putdata([sum(((blob[off+p*stride*h+y*stride+x//8]>>(7-x%8))&1)<<p
                           for p in range(3)) for y in range(h) for x in range(w)])
        decoded[patch_id,state]=image
for frame in range(48):
    rebuilt=base.copy()
    for i,patch in enumerate(manifest['patches']):
        rebuilt.paste(decoded[i,patch['states'][frame]],(patch['x'],patch['y']))
    reviewed=Image.open(R/f'assets/concept/level1-rear-ambience-study-v3/frame-{frame:02d}-indices.png')
    assert rebuilt.tobytes()==reviewed.tobytes(),frame
print('PASS independent planar decode reconstructs all 48 approved rear frames exactly')
C=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef uint8_t UBYTE; typedef uint16_t UWORD; typedef int16_t WORD;
typedef uintptr_t ULONG; typedef int32_t LONG; typedef int BOOL;
typedef void *APTR; typedef FILE *BPTR;
#define TRUE 1
#define FALSE 0
#define MEMF_FAST 1
#define MEMF_CHIP 2
#define MODE_OLDFILE 0
#define BMF_CLEAR 1
#define BMF_DISPLAYABLE 2
#define REAR_PLANES 3
#define PLAYFIELD_GUARD_BYTES 4
#define SPARKPAW_LEVEL1_REAR_HOST_TEST
#define SPARKPAW_LEVEL1_RENDERER_TU_ISOLATION
#define SPARKPAW_ROLLING_PROTOTYPE
#define SPARKPAW_AGA32_LEFT_GUARD
struct BitMap { UWORD BytesPerRow; UBYTE *Planes[3]; };
struct Asset { struct BitMap *bitmap; UWORD height; };
static struct Asset asset,*rearWorld=&asset;
static struct BitMap *rearDisplay;
static struct {ULONG frameCounter; LONG cameraX;} state,*game=&state;
static UBYTE prototypePreparedCopper,prototypeActiveCopper;
static struct {UWORD bltcon0,bltcon1,bltafwm,bltalwm,bltamod,bltdmod,bltsize; UBYTE *bltapt,*bltdpt;} custom,*hw=&custom;
static int allocAt,failAt,live,playing,fileMode,blits;
static UBYTE *stageLow,*stageHigh;
static struct BitMap *visible,*allowed;
static void *AllocMem(ULONG n,int flags){(void)flags; if(++allocAt==failAt)return NULL;live++;return calloc(1,n);}
static void FreeMem(void *p,ULONG n){(void)n;assert(p);live--;free(p);}
static struct BitMap *AllocBitMap(UWORD w,UWORD h,UWORD depth,int flags,void *friend){
 int p;(void)flags;(void)friend;assert(depth==3&&h==208);
 if(++allocAt==failAt)return NULL;
 struct BitMap *b=calloc(1,sizeof(*b));b->BytesPerRow=(w+15)/16*2;live++;
 for(p=0;p<3;p++){UBYTE *raw=malloc(b->BytesPerRow*h+32);memset(raw,0xa5,b->BytesPerRow*h+32);b->Planes[p]=raw+16;memset(b->Planes[p],0,b->BytesPerRow*h);}
 return b;
}
static void FreeBitMap(struct BitMap *b){
 int p,k;for(p=0;p<3;p++){for(k=0;k<16;k++){assert(b->Planes[p][-16+k]==0xa5);assert(b->Planes[p][b->BytesPerRow*208+k]==0xa5);}free(b->Planes[p]-16);}free(b);live--;
}
static void CopyMem(const void *src,void *dst,ULONG n){
 assert(!hw->bltsize);
 if(playing)assert((uintptr_t)dst>=(uintptr_t)stageLow&&(uintptr_t)dst+n<=(uintptr_t)stageHigh);
 memcpy(dst,src,n);
}
static BPTR Open(const char *name,int mode){(void)mode;assert(strstr(name,"l1-electric.bin"));return fileMode==1?NULL:fopen("build/level1-electric-v3-20260926/assets/l1-electric.bin","rb");}
static LONG Read(BPTR f,void *p,ULONG n){
 LONG got=fread(p,1,n,f);
 if(fileMode==2&&n>1)return got-1;
 if(fileMode==3&&n>1)((UBYTE*)p)[0]='X';
 if(fileMode==4&&n==1)return 1;
 return got;
}
static void Close(BPTR f){fclose(f);}
static void platformWaitBlit(void){
 int p,y,ok=0;UWORD bytes,height;UBYTE *src,*dst;
 if(!hw->bltsize)return;
 bytes=(hw->bltsize&63)*2;height=hw->bltsize>>6;
 assert(hw->bltcon0==0x09f0&&hw->bltcon1==0&&hw->bltafwm==0xffff&&hw->bltalwm==0xffff);
 assert(playing&&allowed!=visible&&bytes>0&&height>0);
 src=hw->bltapt;dst=hw->bltdpt;
 assert((uintptr_t)src>=(uintptr_t)stageLow&&(uintptr_t)src+bytes*height<=(uintptr_t)stageHigh);
 for(p=0;p<3;p++)if((uintptr_t)dst>=(uintptr_t)allowed->Planes[p]&&
  (uintptr_t)dst+(height-1)*(bytes+hw->bltdmod)+bytes<=(uintptr_t)allowed->Planes[p]+208*allowed->BytesPerRow)ok=1;
 assert(ok);
 for(y=0;y<height;y++){memcpy(dst,src,bytes);src+=bytes+hw->bltamod;dst+=bytes+hw->bltdmod;}
 hw->bltsize=0;blits++;
}
#include "level1_rear_ambience.h"
static void initial(void){
 int p,y;FILE *f;
 playing=0;failAt=0;allocAt=0;fileMode=0;asset.height=208;
 asset.bitmap=AllocBitMap(1120,208,3,0,NULL);rearDisplay=AllocBitMap(1152,208,3,0,NULL);
 f=fopen("assets/runtime/storm-rear.spbm","rb");assert(f);fseek(f,36,SEEK_SET);
 for(p=0;p<3;p++){assert(fread(asset.bitmap->Planes[p],1,140*208,f)==140*208);for(y=0;y<208;y++)memcpy(rearDisplay->Planes[p]+y*144+4,asset.bitmap->Planes[p]+y*140,140);}
 fclose(f);allocAt=0;
}
static void finish(void){playing=0;freeLevel1RearAmbience();FreeBitMap(rearDisplay);FreeBitMap(asset.bitmap);rearDisplay=NULL;assert(live==0);}
static void checkPatch(struct BitMap *b,int target,int i){
 int p,y;const struct L1RearPatch *r=&l1RearPatches[i];UBYTE s=l1RearDrawn[target][i];
 if(s==255)return;
 for(p=0;p<3;p++)for(y=0;y<r->height;y++)assert(!memcmp(b->Planes[p]+(r->y+y)*144+r->x/8+4,l1RearFrames+r->offset+s*r->frameBytes+p*r->stride*r->height+y*r->stride,r->stride));
}
int main(void){
 int failure,p,k,i,t;UBYTE saved[3][144*208],source[3][140*208];
 for(failure=1;failure<=3;failure++){initial();failAt=failure;assert(!prepareLevel1RearAmbience());finish();}
 for(failure=1;failure<=4;failure++){initial();fileMode=failure;assert(!prepareLevel1RearAmbience());finish();}
 initial();assert(prepareLevel1RearAmbience());assert(live==5);
 stageLow=l1RearStage;stageHigh=stageLow+L1_REAR_STAGE_BYTES;
 for(p=0;p<3;p++)memcpy(source[p],asset.bitmap->Planes[p],140*208);
 prototypeActiveCopper=0;playing=1;
 for(t=0;t<2048;t++){
  prototypePreparedCopper=prototypeActiveCopper^1;visible=l1RearBuffers[prototypeActiveCopper];allowed=l1RearBuffers[prototypePreparedCopper];rearDisplay=allowed;
  for(p=0;p<3;p++)memcpy(saved[p],visible->Planes[p],144*208);
  game->cameraX=t%3==0?3072:(t%3==1?0:(t*37)%3073);
  game->frameCounter=(t%301)*6;
  if(t%301==0)memset(l1RearDrawn,255,sizeof(l1RearDrawn));
  updateLevel1RearAmbience();assert(!hw->bltsize&&!l1RearUnsafe);
  for(p=0;p<3;p++){
   assert(!memcmp(saved[p],visible->Planes[p],144*208));
   assert(!memcmp(source[p],asset.bitmap->Planes[p],140*208));
   for(k=0;k<208;k++)for(i=0;i<4;i++)assert(allowed->Planes[p][k*144+i]==0);
  }
  for(i=0;i<9;i++){
   const struct L1RearPatch *r=&l1RearPatches[i];int left=game->cameraX/4-32,right=game->cameraX/4+384;
   if(r->x+r->stride*8>left&&r->x<right)assert(l1RearDrawn[prototypePreparedCopper][i]==r->states[(game->frameCounter/6)%48]);
   checkPatch(allowed,prototypePreparedCopper,i);
  }
  prototypeActiveCopper=prototypePreparedCopper;
 }
 /* Deliberately violate ownership: reject before touching live pixels. */
 updateLevel1RearAmbience();assert(l1RearUnsafe==1);
 printf("PASS actual C: allocation/file failures; 2048 alternating camera/phase/reset cases; immutable active/source; guards; %d blits; max %lu bytes / %u patches\n",blits,(unsigned long)l1RearMaxBytes,l1RearMaxPatches);
 finish();return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='l1-rear-host-') as tmp:
    c=Path(tmp)/'test.c';exe=Path(tmp)/'test';c.write_text(C)
    subprocess.run(['cc','-std=c99','-O1','-g','-fsanitize=address,undefined','-I'+str(R/'src'),'-I'+str(O),str(c),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],cwd=R,check=True)
