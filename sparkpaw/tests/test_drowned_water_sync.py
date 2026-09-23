"""Actual batched water helper + DMA copier against independent pixel oracle."""
from pathlib import Path
import subprocess, tempfile
ROOT=Path(__file__).resolve().parents[1]
pre=r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
typedef int16_t WORD; typedef uint16_t UWORD;
typedef int32_t LONG; typedef uint8_t UBYTE;
#define PROTOTYPE_RING_W 512
#define FRONT_PLANES 4
#define WORLD_H 208
#define WATER_Y 197
#define WATER_H 11
#define WATER_W 80
#define LEVEL_WATER_COUNT 10
struct Bitmap { int BytesPerRow; UBYTE *Planes[4]; };
struct Asset { struct Bitmap *bitmap; } asset,*frontClean=&asset;
struct PrototypeTarget { WORD origin; struct Bitmap *display; UBYTE waterFrame[10]; };
static UWORD src[4][150*208],dst[4][96*208],ref[4][96*208];
static UBYTE waterDrawnFrame[10]; static WORD starts[10];
static WORD levelWaterLeft(WORD i) {return starts[i];}
static int blits;
struct {UWORD bltcon0,bltcon1,bltafwm,bltalwm,bltamod,bltdmod,bltsize;
 UBYTE *bltapt,*bltdpt;} custom,*hw=&custom;
static void platformWaitBlit(void) {
 int y,bytes=2*(hw->bltsize&63),height=hw->bltsize>>6;
 if(!hw->bltsize)return;
 assert(bytes>0&&hw->bltcon0==0x09f0&&!hw->bltcon1);
 assert(hw->bltafwm==65535&&hw->bltalwm==65535);
 assert(hw->bltamod+bytes==300&&hw->bltdmod+bytes==192);
 for(y=0;y<height;y++)memcpy(hw->bltdpt+y*192,hw->bltapt+y*300,bytes);
 hw->bltsize=0;blits++;
}
'''
main=r'''
int main(void) {
 struct Bitmap s={300,{0}},d={192,{0}};
 struct PrototypeTarget t={0,&d,{0}};
 int p,i,mode,origin,dirty,row,x,c,n=0,old;
 asset.bitmap=&s;
 for(p=0;p<4;p++) {s.Planes[p]=(UBYTE*)src[p];d.Planes[p]=(UBYTE*)dst[p];
  for(i=0;i<150*208;i++)src[p][i]=(UWORD)(i*17+p*751);}
 for(mode=0;mode<2;mode++)for(origin=0;origin<=1024;origin+=16)
 for(dirty=0;dirty<1024;dirty++) {
  t.origin=origin;memset(dst,0xa5,sizeof(dst));memcpy(ref,dst,sizeof(dst));
  for(i=0;i<10;i++) {
   starts[i]=160+(mode?96:80)*i;waterDrawnFrame[i]=(i*3+dirty)%16;
   t.waterFrame[i]=(dirty>>i&1)?255:waterDrawnFrame[i];
   if(!(dirty>>i&1))continue;
   for(x=starts[i];x<starts[i]+80;x+=16) {
    if(x<origin||x>=origin+512)continue;
    for(p=0;p<4;p++)for(row=197;row<208;row++)for(c=0;c<3;c++)
     ref[p][row*96+(x%512)/16+c*32]=src[p][row*150+x/16];
   }
  }
  blits=0;
  for(i=0;i<10;i++)if(dirty>>i&1)drownedCopyPatchRect(&t,starts[i],197,80,11);
  old=blits;assert(!memcmp(dst,ref,sizeof(dst)));
  memset(dst,0xa5,sizeof(dst));blits=0;
  drownedSynchronizeWater(&t);
  assert(!memcmp(dst,ref,sizeof(dst)));assert(!hw->bltsize);
  assert(!memcmp(t.waterFrame,waterDrawnFrame,10));assert(blits<=old);
  if(!mode&&origin==160&&dirty==63){assert(old==84&&blits==24);}
  blits=0;drownedSynchronizeWater(&t);assert(blits==0);n++;
 }
 assert(n==133120);printf("PASS: %d actual-C water cases, pixel/phase parity, four planes/three copies, DMA waits, repeat no-op; example84->24 blits\n",n);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d)
 (p/'test.c').write_text(pre+(ROOT/'src/drowned_patch_copy.h').read_text()+(ROOT/'src/drowned_water_sync.h').read_text()+main)
 subprocess.run(['cc','-O2','-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_PATCH_BLIT','-I'+str(ROOT/'src'),str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
