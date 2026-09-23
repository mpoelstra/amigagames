"""Actual repeated-pattern canonical DMA vs independent original strip oracle."""
from pathlib import Path
import subprocess,tempfile
r=Path(__file__).resolve().parents[1]
s=r'''
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
typedef int16_t WORD;typedef uint16_t UWORD;typedef uint8_t UBYTE;typedef int32_t LONG;typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define MEMF_CHIP 2
#define WATER_WORDS 5
#define WATER_W 80
#define WATER_H 11
#define WATER_Y 197
#define WATER_FRAMES 16
#define FRONT_PLANES 4
#define LEVEL_WATER_COUNT 20
#define SCREEN_W 320
static UWORD source[16*4*11*5],*waterBits=source;
static UBYTE waterDrawnFrame[20];
static WORD starts[20];
static WORD levelWaterLeft(UBYTE i){return starts[i];}
static void *AllocMem(long n,int f){return malloc(n);}
#define CopyMem(a,b,n) memcpy(b,a,n)
struct BitMap {WORD BytesPerRow;UBYTE *Planes[4];};
struct {struct BitMap *bitmap;} asset,*frontClean=&asset;
struct {WORD cameraX;unsigned frameCounter;} state,*game=&state;
static UBYTE actual[4][440*208],expected[4][440*208];
struct {UWORD bltcon0,bltcon1,bltafwm,bltalwm,bltamod,bltdmod,bltsize;void *bltapt,*bltdpt;} custom,*hw=&custom;
static int blits;
static void platformWaitBlit(void){
 if(!hw->bltsize)return;
 int bytes=(hw->bltsize&63)*2,h=hw->bltsize>>6;
 assert(hw->bltcon0==0x09f0&&!hw->bltcon1&&hw->bltafwm==65535&&hw->bltalwm==65535);
 assert(h==11&&bytes>0&&bytes<=60&&bytes+hw->bltamod==60&&bytes+hw->bltdmod==440);
 for(int y=0;y<h;y++)memcpy((UBYTE*)hw->bltdpt+y*440,(UBYTE*)hw->bltapt+y*60,bytes);
 hw->bltsize=0;blits++;
}
#include "water_update_visibility.h"
#include "drowned_water_batch.h"
int main(void){
 struct BitMap bm={440,{actual[0],actual[1],actual[2],actual[3]}};asset.bitmap=&bm;
 for(int i=0;i<16*4*11*5;i++)source[i]=i*73+(i>>2);
 assert(prepareDrownedWaterBatch());int cases=0;
 for(int layout=0;layout<2;layout++)for(int frame=0;frame<16;frame++)for(int cam=0;cam<=3200;cam+=16)for(int dirty=0;dirty<4;dirty++){
  game->cameraX=cam;game->frameCounter=frame*2;int strips=0;
  memset(actual,0xa5,sizeof(actual));memcpy(expected,actual,sizeof(actual));
  for(int i=0;i<20;i++){
   starts[i]=layout?i*160:160+i*80;
   waterDrawnFrame[i]=(dirty==0||dirty==1&&(i&1)||dirty==2&&(i%3))?255:frame;
   if(waterDrawnFrame[i]==frame||!waterUpdateVisible(starts[i],cam,320,80,16))continue;
   strips++;
   for(int p=0;p<4;p++)for(int y=0;y<11;y++)
    memcpy(expected[p]+(197+y)*440+starts[i]/8,source+((frame*4+p)*11+y)*5,10);
  }
  blits=0;animateDrownedWaterBatch();platformWaitBlit();
  assert(!memcmp(actual,expected,sizeof(actual)));assert(blits<=strips*4);
  if(!layout&&cam==240&&dirty==0&&frame==0){assert(strips>=4&&blits==4);}
  blits=0;animateDrownedWaterBatch();platformWaitBlit();assert(blits==0);cases++;
 }
 free(drownedWaterBatchBits);printf("PASS: %d actual canonical DMA cases,16frames,gaps/partial dirtiness,culling,repeat no-op,4blits per visible run\n",cases);
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'t.c').write_text(s)
 subprocess.run(['cc','-O2','-fsanitize=address,undefined','-I'+str(r/'src'),str(p/'t.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t')],check=True)
