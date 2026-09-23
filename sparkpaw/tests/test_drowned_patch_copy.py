"""Actual Drowned transfer vs pixel-address oracle, including ring seams/clips."""
from pathlib import Path
import subprocess, tempfile
ROOT=Path(__file__).resolve().parents[1]
pre=r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
typedef int16_t WORD; typedef uint16_t UWORD;
typedef int32_t LONG; typedef uint8_t UBYTE;
#define PROTOTYPE_RING_W 512
#define FRONT_PLANES 4
#define WORLD_H 208
struct Bitmap { int BytesPerRow; UBYTE *Planes[4]; };
struct Asset { struct Bitmap *bitmap; } asset,*frontClean=&asset;
struct PrototypeTarget { WORD origin; struct Bitmap *display; };
static UWORD src[4][60*208],dst[4][96*208],ref[4][96*208];
'''
dma=r'''
struct {UWORD bltcon0,bltcon1,bltafwm,bltalwm,bltamod,bltdmod,bltsize;
 UBYTE *bltapt,*bltdpt;} custom,*hw=&custom;
static void platformWaitBlit(void) {
 int y,bytes=2*(hw->bltsize&63),height=hw->bltsize>>6;
 if(!hw->bltsize)return;
 assert(hw->bltcon0==0x09f0&&!hw->bltcon1);
 assert(hw->bltafwm==65535&&hw->bltalwm==65535);
 assert(hw->bltamod+bytes==120&&hw->bltdmod+bytes==192);
 for(y=0;y<height;y++)memcpy(hw->bltdpt+y*192,hw->bltapt+y*120,bytes);
 hw->bltsize=0;
}
'''
main=r'''
int main(void) {
 struct Bitmap s={120,{0}},d={192,{0}};
 struct PrototypeTarget t={0,&d};
 int p,i,origin,x,y,w,h,c,row,col,left,right,top,bottom,n=0;
 int xs[]={0,240,448,496,504,512,608,768,832,944};
 int ys[]={-3,0,131,136,197,208};
 int ws[]={16,32,80,96,160,320,512}; int hs[]={1,11,61,64};
 asset.bitmap=&s;
 for(p=0;p<4;p++) {s.Planes[p]=(UBYTE*)src[p];d.Planes[p]=(UBYTE*)dst[p];
  for(i=0;i<60*208;i++)src[p][i]=(UWORD)(i*17+p*751);}
 for(origin=0;origin<=448;origin+=16)
 for(x=0;x<10;x++)for(y=0;y<6;y++)for(w=0;w<7;w++)for(h=0;h<4;h++) {
  t.origin=origin;memset(dst,0xa5,sizeof(dst));memcpy(ref,dst,sizeof(dst));
  left=xs[x]&~15;right=(xs[x]+ws[w]+15)&~15;
  if(left<origin)left=origin;if(right>origin+512)right=origin+512;
  top=ys[y]<0?0:ys[y];bottom=ys[y]+hs[h];if(bottom>208)bottom=208;
  for(p=0;p<4;p++)for(row=top;row<bottom;row++)for(col=left;col<right;col+=16)
   for(c=0;c<3;c++)ref[p][row*96+(col%512)/16+c*32]=src[p][row*60+col/16];
  drownedCopyPatchRect(&t,xs[x],ys[y],ws[w],hs[h]);
  assert(!memcmp(dst,ref,sizeof(dst)));n++;
 }
 assert(n==48720);return 0;
}
'''
for mode in ('cpu','dma-reference','dma'):
 with tempfile.TemporaryDirectory() as d:
  p=Path(d);(p/'test.c').write_text(pre+dma+(ROOT/'src/drowned_patch_copy.h').read_text()+main)
  flags=['-DSPARKPAW_DROWNED_PATCH_BLIT'] if mode!='cpu' else []
  if mode=='dma-reference':flags+=['-DSPARKPAW_DROWNED_PATCH_SETUP_REFERENCE']
  subprocess.run(['cc','-I'+str(ROOT/'src'),'-O2','-std=c99','-fsanitize=address,undefined',*flags,str(p/'test.c'),'-o',str(p/'test')],check=True)
  subprocess.run([str(p/'test')],check=True)
print('PASS: actual CPU and DMA transfers, 48720 rectangles each, including wide water batches, four planes/three ring copies, wrapping and clipping')
