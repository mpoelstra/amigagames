"""Actual ring kernels + DMA model over full-level scroll/reset/actor lifetimes.
Reference and candidate each checked against inverse world-coordinate oracle.
Not a raster-timing or emulator proof.
"""
from pathlib import Path
import re,subprocess,tempfile,json
R=Path(__file__).resolve().parents[1];s=(R/'src/renderer.c').read_text()
def func(name):
 m=re.search(r'static (?:void|BOOL|WORD)\s+'+name+r'\([^)]*\)\s*\{',s);assert m,name
 end=m.end();depth=1
 while depth:depth+=(s[end]=='{')-(s[end]=='}');end+=1
 return s[m.start():end]+'\n'
pre=r'''
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef int16_t WORD;typedef uint16_t UWORD;typedef uint8_t UBYTE;typedef int32_t LONG;typedef uint32_t ULONG;typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define WORLD_W 5120
#define WORLD_H 208
#define FRONT_PLANES 4
#define PROTOTYPE_RING_W 512
#include "drowned_ring_layout.h"
#define PROTOTYPE_RING_COPIES DROWNED_RING_COPIES
#define PROTOTYPE_RING_BASE DROWNED_RING_BASE
#define WIDTH (512*DROWNED_RING_COPIES)
#define STRIDE (WIDTH/8)
#define BYTES (STRIDE*208)
#define MAX_COLLECTIBLES 48
#define LEVEL_WATER_COUNT 20
#define SPARKPAW_CANONICAL_BOB_RESTORE
#define SPARKPAW_ROLLING_PROTOTYPE
#define SPARKPAW_DROWNED_JOINED
#define SPARKPAW_DROWNED_COLUMN_BLIT
#define SPARKPAW_DROWNED_PATCH_BLIT
#define SPARKPAW_DROWNED_ENEMY_BOUNDS
#define BUSY_COUNT(a,b) ((void)0)
#define performanceProfileEnd(a,b) ((void)0)
#define GUARD 64
struct BitMap{WORD BytesPerRow;UBYTE *Planes[4];} sourceBitmap,targetBitmap[2],*frontDisplay;
static struct {struct BitMap *bitmap;} asset={&sourceBitmap},*frontClean=&asset;
struct PrototypeTarget{WORD origin;struct BitMap *display;UBYTE waterFrame[20];BOOL collectibleDrawn[48];WORD collectibleX[48],collectibleY[48];} target[2];
struct Collectible{BOOL drawn;WORD drawnY;} items[48];
static struct Collectible*collectibleAt(int n){return &items[n];}
static UBYTE waterDrawnFrame[20];
static struct{WORD cameraX;} gameState,*game=&gameState;
static WORD prototypeBuildOrigin;
static int prepared,displayed;
static UBYTE source[4][640*208],memory[2][4][BYTES+2*GUARD],expected[4][BYTES],activeSaved[4][BYTES];
static UWORD mask[4][64*7],bits[4][4*64*7];
static struct{UWORD bltcon0,bltcon1,bltafwm,bltalwm,bltamod,bltbmod,bltcmod,bltdmod,bltsize;void *bltapt,*bltbpt,*bltcpt,*bltdpt;} regs,*hw=&regs;
static unsigned long copiedWords,maskedWords,blits;
static void platformWaitBlit(void){
 if(!hw->bltsize)return;
 int w=hw->bltsize&63,h=hw->bltsize>>6,copy=hw->bltcon0==0x09f0,shift=hw->bltcon0>>12;
 assert(w>0&&h>0&&h<=208);assert(copy||(hw->bltcon0&4095)==0xfca);
 assert(hw->bltafwm==65535&&hw->bltalwm==65535);
 int found=0;
 for(int p=0;p<4;p++){
  uintptr_t d=(uintptr_t)hw->bltdpt,base=(uintptr_t)targetBitmap[prepared].Planes[p];
  if(d>=base&&d<base+BYTES){assert(d+(h-1)*(w*2+hw->bltdmod)+w*2<=base+BYTES);found++;}
 }
 assert(found==1&&prepared!=displayed);
 if(copy){
  found=0;
  for(int p=0;p<4;p++){
   uintptr_t a=(uintptr_t)hw->bltapt,base=(uintptr_t)source[p];
   if(a>=base&&a<base+sizeof source[p]){assert(a+(h-1)*(w*2+hw->bltamod)+w*2<=base+sizeof source[p]);found++;}
  }
  assert(found==1);copiedWords+=w*h;
 }else maskedWords+=w*h;
 for(int y=0;y<h;y++){
  UWORD *a=(UWORD*)((UBYTE*)hw->bltapt+y*(w*2+hw->bltamod));
  UWORD *d=(UWORD*)((UBYTE*)hw->bltdpt+y*(w*2+hw->bltdmod));
  if(copy){memcpy(d,a,w*2);continue;}
  UWORD *b=(UWORD*)((UBYTE*)hw->bltbpt+y*(w*2+hw->bltbmod));
  UWORD *c=(UWORD*)((UBYTE*)hw->bltcpt+y*(w*2+hw->bltcmod));unsigned pa=0,pb=0;
  for(int x=0;x<w;x++){
   unsigned aa=a[x],bb=b[x],ma=(aa>>shift)|(shift?pa<<(16-shift):0),mb=(bb>>shift)|(shift?pb<<(16-shift):0);
   d[x]=(UWORD)((ma&mb)|(~ma&c[x]));pa=aa;pb=bb;
  }
 }
 hw->bltsize=0;blits++;
}
#include "rolling_renderer_contract.h"
static WORD stormrailRestoreWordCount(WORD x,WORD w){return ((x&15)+w+15)>>4;}
'''
body=pre+func('prototypeOriginForCamera')+func('prototypeRectFits')+func('prototypePhysicalX')
body+=func('prototypeCopyCanonicalSpan')+func('prototypeCopyCanonicalRect')
body+='#include "drowned_column_blit.h"\n'
# Fallback's #else is outside the extracted C function; select actual non-Stormrail body.
body+=func('prototypeCopyCanonicalColumn')+func('prototypeCopyDynamicSpan')+func('prototypeCopyDynamicRect')+func('prototypeCopyInitial')
body+='#include "drowned_patch_copy.h"\n#include "drowned_reset_copy.h"\n'
body+=func('prototypeRollTarget')+func('blitRestoreRect')
a=s.index('#if defined(SPARKPAW_STORMRAIL_PROOF) || defined(SPARKPAW_DROWNED_ENEMY_BOUNDS)',s.index('static void blitRestoreRect'))
body+=s[a:s.index('static BOOL buildDiamondPattern')]
body+=r'''
struct Actor{int valid;WORD world,x,y,w,h,sw,id;};static struct Actor history[2][4];
static void compareCanonical(void){
 for(int p=0;p<4;p++)for(int y=0;y<208;y++)for(int x=0;x<WIDTH;x+=16){
  int world=target[prepared].origin+((x-PROTOTYPE_RING_BASE-target[prepared].origin)&511);
  assert(!memcmp(targetBitmap[prepared].Planes[p]+y*STRIDE+x/8,source[p]+y*640+world/8,2));
 }
}
static void guardsAndActive(void){
 assert(!hw->bltsize);
 for(int p=0;p<4;p++){
  assert(!memcmp(activeSaved[p],targetBitmap[displayed].Planes[p],BYTES));
  for(int b=0;b<2;b++)for(int g=0;g<GUARD;g++){
   assert(memory[b][p][g]==0xa5);assert(memory[b][p][GUARD+BYTES+g]==0xa5);
  }
 }
}
static void drawOracle(struct Actor *a){
 for(int y=0;y<a->h;y++)for(int x=0;x<a->w;x++){
  UWORD bit=32768>>(x&15);if(!(mask[a->id][y*a->sw+x/16]&bit))continue;
  int dx=a->x+x,dy=a->y+y;
  for(int p=0;p<4;p++){
   UWORD *d=(UWORD*)(expected[p]+dy*STRIDE+(dx/16)*2),db=32768>>(dx&15);
   *d=(*d&~db)|((bits[a->id][(p*a->h+y)*a->sw+x/16]&bit)?db:0);
  }
 }
}
int main(int argc,char **argv){
 assert(argc==2);FILE*f=fopen(argv[1],"rb");assert(f);fseek(f,60,SEEK_SET);assert(fread(source,1,sizeof source,f)==sizeof source);fclose(f);
 sourceBitmap.BytesPerRow=640;memset(memory,0xa5,sizeof memory);
 for(int p=0;p<4;p++)sourceBitmap.Planes[p]=source[p];
 for(int b=0;b<2;b++){
  targetBitmap[b].BytesPerRow=STRIDE;target[b].display=&targetBitmap[b];
  for(int p=0;p<4;p++)targetBitmap[b].Planes[p]=memory[b][p]+GUARD;
  prepared=b;displayed=1-b;target[b].origin=0;prototypeCopyInitial(&target[b]);compareCanonical();
 }
 /* Explicit parity for less-common CPU copy variants, clipping and reset DMA. */
 for(int camera=0;camera<=4800;camera+=16){
  prepared=(camera/16)&1;displayed=1-prepared;frontDisplay=&targetBitmap[prepared];game->cameraX=camera;
  for(int p=0;p<4;p++)memcpy(activeSaved[p],targetBitmap[displayed].Planes[p],BYTES);
  target[prepared].origin=prototypeOriginForCamera(camera);prototypeCopyInitial(&target[prepared]);compareCanonical();
  prototypeCopyDynamicRect(&target[prepared],camera-20,-3,512,214);compareCanonical();
  prototypeCopyCanonicalColumn(&target[prepared],target[prepared].origin);compareCanonical();
  drownedCopyResetTarget(&target[prepared]);compareCanonical();guardsAndActive();
 }
 /* Keep source heights compatible with generated shortened-column contract. */
 unsigned long ringWords=0,restoreWords=0;int frames=0;
 for(int tick=0;tick<10048;tick++){
  prepared=tick&1;displayed=1-prepared;frontDisplay=&targetBitmap[prepared];
  for(int p=0;p<4;p++)memcpy(activeSaved[p],targetBitmap[displayed].Planes[p],BYTES);
  unsigned long start=copiedWords;
  for(int i=0;i<4;i++)if(history[prepared][i].valid){
   struct Actor*a=&history[prepared][i];blitRestoreRect(a->world,a->x,a->y,a->w,a->h);a->valid=0;
  }
  platformWaitBlit();restoreWords+=copiedWords-start;start=copiedWords;
  int camera=tick<4801?tick:(tick<9602?9601-tick:(tick*137)%4801);
  /* Deliberate large camera jumps on both target histories. */
  if(tick>=9602){if(tick%127==0)camera=2688;if(tick%131==0)camera=0;}
  game->cameraX=camera;
  prototypeRollTarget(&target[prepared],prototypeOriginForCamera(camera));
  prototypeBuildOrigin=target[prepared].origin;
  /* Animate same canonical bottom strip and sync current target. This also
     exercises updates missed while the other target was displayed. */
  for(int p=0;p<4;p++)for(int y=197;y<208;y++)for(int x=0;x<5120;x+=16)
   if(y>=drownedColumnTops[x/16])((UWORD*)source[p])[y*320+x/16]=(UWORD)(tick*23+p*731+y*11+x*13);
  drownedCopyPatchRect(&target[prepared],0,197,5120,11);
  ringWords+=copiedWords-start;compareCanonical();
  for(int p=0;p<4;p++)memcpy(expected[p],targetBitmap[prepared].Planes[p],BYTES);
  for(int i=0;i<4;i++){
   struct Actor*a=&history[prepared][i];int widths[]={24,32,64,96},heights[]={24,24,64,16};
   a->id=i;a->w=widths[(tick/17+i)&3];a->h=heights[(tick/17+i)&3];a->sw=(a->w+15)/16+1;
   a->world=(WORD)(target[prepared].origin+((tick*7+i*31)%(513-a->w)));a->y=30+i*12;a->x=prototypePhysicalX(a->world);
   assert(prototypeRectFits(a->world,a->w)&&a->x>=0&&a->x+a->w<=WIDTH);
   if(tick%19==i){a->valid=0;continue;}
   memset(mask[i],0,sizeof mask[i]);memset(bits[i],0,sizeof bits[i]);
   for(int y=0;y<a->h;y++)for(int x=0;x<a->w;x++)if((x+y+tick+i)%5){
    UWORD m=32768>>(x&15);mask[i][y*a->sw+x/16]|=m;
    int pen=1+(x+y+tick)%15;
    for(int p=0;p<4;p++)if(pen&(1<<p))bits[i][(p*a->h+y)*a->sw+x/16]|=m;
   }
   platformWaitBlit();blitMaskedBob(mask[i],bits[i],a->sw,a->w,a->h,a->x,a->y);platformWaitBlit();drawOracle(a);a->valid=1;
  }
  for(int p=0;p<4;p++)assert(!memcmp(expected[p],targetBitmap[prepared].Planes[p],BYTES));
  /* Fetch pointer addresses and pixel correspondence must stay identical
     to the legacy layout after rebasing. */
  int fetch=rollingAga32CorrectedByteOffset(PROTOTYPE_RING_BASE+(camera&511))*8-32;
  int legacy=rollingAga32CorrectedByteOffset(512+(camera&511))*8-32;
  assert(fetch>=0&&fetch+384<=WIDTH&&fetch-PROTOTYPE_RING_BASE==legacy-512);
  guardsAndActive();frames++;
 }
 printf("{\"copies\":%d,\"frames\":%d,\"ring_dma_words\":%lu,\"restore_words\":%lu,\"masked_words\":%lu,\"blits\":%lu}\n",DROWNED_RING_COPIES,frames,ringWords,restoreWords,maskedWords,blits);
}
'''
if __name__ == '__main__':
 results=[]
 with tempfile.TemporaryDirectory() as td:
  p=Path(td);(p/'test.c').write_text(body)
  for candidate in [False,True]:
   flags=['-DSPARKPAW_DROWNED_TWO_COPY_RING'] if candidate else []
   subprocess.run(['cc','-O2','-std=c99','-fsanitize=address,undefined',*flags,'-I'+str(R/'src'),'-I'+str(R/'build/drowned-full'),str(p/'test.c'),'-o',str(p/'test')],check=True)
   result=subprocess.check_output([str(p/'test'),str(R/'build/drowned-full/assets/drowned-route.spbm')],text=True)
   results.append(json.loads(result))
 a,b=results
 assert b['ring_dma_words']*3==a['ring_dma_words']*2
 assert b['restore_words']==a['restore_words'] and b['masked_words']==a['masked_words']
 print('PASS ASan/UBSan: actual ring/copy/column/restore/Blitter setup;10048frames each; all cameras, both directions, wraps, teleports, clipping, overlapping24/32/64/96pxactors, inactive ownership/guards; exact1/3less ring DMA words')
 print(json.dumps(results))
