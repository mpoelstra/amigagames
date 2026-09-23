"""Actual full-ring CPU/reset-DMA parity, both targets, every full-world origin."""
from pathlib import Path
import subprocess
import tempfile

R=Path(__file__).resolve().parents[1]
renderer=(R/'src/renderer.c').read_text()
def function(name):
    start=renderer.index('static void '+name+'(')
    end=renderer.index('{',start)+1;depth=1
    while depth:
        depth+=(renderer[end]=='{')-(renderer[end]=='}');end+=1
    return renderer[start:end]+'\n'

pre=r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
typedef uint8_t UBYTE;typedef uint16_t UWORD;typedef int16_t WORD;
typedef int32_t LONG;typedef int BOOL;
#define FALSE 0
#define WORLD_H 208
#define WORLD_W 5120
#define PROTOTYPE_RING_W 512
#include "drowned_ring_layout.h"
#define PROTOTYPE_RING_COPIES 3
#define FRONT_PLANES 4
#define MAX_COLLECTIBLES 48
#define LEVEL_WATER_COUNT 20
#define SPARKPAW_CANONICAL_BOB_RESTORE
#define SPARKPAW_DROWNED_PATCH_BLIT
#define SPARKPAW_DROWNED_RESET_BLIT
#define performanceProfileEnd(a,b) ((void)0)
struct Bitmap{UWORD BytesPerRow;UBYTE *Planes[4];};
struct Asset{struct Bitmap *bitmap;} asset,*frontClean=&asset;
struct PrototypeTarget{WORD origin;struct Bitmap *display;
 UBYTE waterFrame[20];BOOL collectibleDrawn[48];WORD collectibleX[48],collectibleY[48];};
struct Collectible{BOOL drawn;WORD drawnY;} items[48];
static struct Collectible *collectibleAt(WORD i){return &items[i];}
static UBYTE waterDrawnFrame[20];
static UBYTE source[4][640*208],dest[2][4][192*208+16],expected[4][192*208+16],other[4][192*208+16];
struct{UWORD bltcon0,bltcon1,bltafwm,bltalwm,bltamod,bltdmod,bltsize;
 UBYTE *bltapt,*bltdpt;} custom,*hw=&custom;
static int blits;static long copied;
static void platformWaitBlit(void){
 if(!hw->bltsize)return;
 int bytes=(hw->bltsize&63)*2,h=hw->bltsize>>6;
 assert(bytes>0&&bytes<=64&&h==208);
 assert(hw->bltcon0==0x09f0&&!hw->bltcon1&&hw->bltafwm==65535&&hw->bltalwm==65535);
 assert(hw->bltamod+bytes==640&&hw->bltdmod+bytes==192);
 for(int y=0;y<h;y++)memcpy(hw->bltdpt+y*192,hw->bltapt+y*640,bytes);
 copied+=bytes*h;blits++;hw->bltsize=0;
}
#include "drowned_patch_copy.h"
'''
code=pre+function('prototypeCopyCanonicalSpan')+function('prototypeCopyCanonicalRect')+function('prototypeCopyInitial')
code+='#include "drowned_reset_copy.h"\n'
code+='static void prototypeCopyCanonicalColumn(struct PrototypeTarget*t,WORD x){assert(0);}\n'
code+=function('prototypeRollTarget')+r'''
int main(void){
 struct Bitmap src={640},d[2]={{192},{192}},ref={192};
 struct PrototypeTarget t[2]={{0,&d[0]},{0,&d[1]}},want={0,&ref};
 int cases=0;
 asset.bitmap=&src;
 for(int p=0;p<4;p++){
  src.Planes[p]=source[p];ref.Planes[p]=expected[p]+8;
  for(int i=0;i<640*208;i++)source[p][i]=(UBYTE)(i*17+p*73+i/640);
  for(int b=0;b<2;b++)d[b].Planes[p]=dest[b][p]+8;
 }
 for(int phase=0;phase<3;phase++){
  for(int i=0;i<20;i++)waterDrawnFrame[i]=(UBYTE)(i*7+phase*93);
  for(int i=0;i<48;i++){items[i].drawn=(i+phase)&1;items[i].drawnY=(WORD)(i*3-2+phase);}
  for(int origin=0;origin<=4608;origin+=16)for(int b=0;b<2;b++){
   memset(dest,0xa5,sizeof(dest));memset(expected,0xa5,sizeof(expected));
   memcpy(other,dest[b^1],sizeof(other));
   memset(t[b].waterFrame,255,sizeof(t[b].waterFrame));
   memset(t[b].collectibleDrawn,255,sizeof(t[b].collectibleDrawn));
   memset(t[b].collectibleX,255,sizeof(t[b].collectibleX));
   memset(t[b].collectibleY,255,sizeof(t[b].collectibleY));
   t[b].origin=origin<512?4608:0;want.origin=origin;
   /* Actual old CPU function, unchanged startup path. */
   prototypeCopyInitial(&want);
   blits=0;copied=0;
   /* Actual large-jump dispatch, not a direct helper-only test. */
   prototypeRollTarget(&t[b],origin);
   assert(t[b].origin==origin&&!hw->bltsize);
   assert(blits==(origin%512?24:12));
   assert(copied==512/8*208*4*3);
   assert(!memcmp(dest[b],expected,sizeof(expected)));
   assert(!memcmp(dest[b^1],other,sizeof(other)));
   assert(!memcmp(t[b].waterFrame,want.waterFrame,sizeof(want.waterFrame)));
   assert(!memcmp(t[b].collectibleDrawn,want.collectibleDrawn,sizeof(want.collectibleDrawn)));
   assert(!memcmp(t[b].collectibleX,want.collectibleX,sizeof(want.collectibleX)));
   assert(!memcmp(t[b].collectibleY,want.collectibleY,sizeof(want.collectibleY)));
   /* Independent physical-address oracle: every row/plane/all three copies. */
   for(int p=0;p<4;p++)for(int y=0;y<208;y++)for(int x=origin;x<origin+512;x+=16)
    for(int c=0;c<3;c++)assert(!memcmp(dest[b][p]+8+y*192+(x%512)/8+c*64,source[p]+y*640+x/8,2));
   cases++;
  }
 }
 printf("PASS %d actual large-jump resets: CPU/DMA/independent pixel parity, both targets, guards and history;159744destination bytes,12/24blits\n",cases);
}
'''
with tempfile.TemporaryDirectory() as tmp:
    p=Path(tmp);(p/'test.c').write_text(code)
    subprocess.run(['cc','-O2','-fsanitize=address,undefined','-I'+str(R/'src'),
                    str(p/'test.c'),'-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],check=True)
