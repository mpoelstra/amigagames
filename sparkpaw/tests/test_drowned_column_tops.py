"""Actual shortened column DMA vs full-height oracle, both directions/dynamic rows."""
from pathlib import Path
import subprocess,tempfile,sys,json
r=Path(__file__).resolve().parents[1];sys.path.insert(0,str(r/'tools'))
from preview_drowned_native import decode
front=decode(r/'build/drowned-joined/assets/drowned-route.spbm')
text=(r/'build/drowned-joined/drowned_column_tops.h').read_text();tops=list(map(int,text.split('{')[1].split('}')[0].split(',')))
assert len(tops)==220
for col,top in enumerate(tops):assert not any(front.crop((col*16,0,col*16+16,top)).tobytes())
manifest=json.loads((r/'build/drowned-joined/manifest.json').read_text())
for x,y,w in [(448,131,32),(768,136,80),(1792,107,32),(2320,152,48)]+[(x,197,80) for x in manifest['water']]+[(x-16,y-2,48) for x,y in manifest['coins']]:
 for col in range(220):
  if col*16<x+w and col*16+16>x:assert tops[col]<=y
renderer=(r/'src/renderer.c').read_text();a=renderer.index('static void prototypeRollTarget(');start=renderer.index('{',a);end=start+1;depth=1
while depth:
 depth+=(renderer[end]=='{')-(renderer[end]=='}');end+=1
roll=renderer[a:end]
pre=r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
typedef int16_t WORD;typedef uint16_t UWORD;typedef uint8_t UBYTE;typedef int32_t LONG;
#define WORLD_W 3520
#define WORLD_H 208
#define PROTOTYPE_RING_W 512
#define PROTOTYPE_RING_COPIES 3
#define FRONT_PLANES 4
struct Bitmap{UWORD BytesPerRow;UBYTE *Planes[4];} src,dst;
struct Asset{struct Bitmap *bitmap;} asset={&src},*frontClean=&asset;
struct PrototypeTarget{struct Bitmap *display;WORD origin;};
static UBYTE source[4][440*208],dest[4][192*208+4],expected[4][192*208+4];
struct{UWORD bltcon0,bltcon1,bltafwm,bltalwm,bltamod,bltdmod,bltsize;UBYTE *bltapt,*bltdpt;} custom,*hw=&custom;
static long copied;
static void platformWaitBlit(void){
 if(!hw->bltsize)return;
 int h=hw->bltsize>>6;assert((hw->bltsize&63)==1&&h>0&&h<=208);
 assert(hw->bltcon0==0x09f0&&!hw->bltcon1&&hw->bltafwm==65535&&hw->bltalwm==65535);
 assert(hw->bltamod==438&&hw->bltdmod==190);
 for(int y=0;y<h;y++)memcpy(hw->bltdpt+y*192,hw->bltapt+y*440,2);
 copied+=h*2;hw->bltsize=0;
}
#include "drowned_column_blit.h"
#define performanceProfileEnd(slot,start) ((void)0)
static void prototypeCopyInitial(struct PrototypeTarget *t){assert(0);}
static void prototypeCopyCanonicalRect(struct PrototypeTarget *t,WORD x,WORD y,WORD w,WORD h){assert(0);}
/* ACTUAL_ROLL */
int main(int argc,char **argv){
 struct PrototypeTarget t={&dst};src.BytesPerRow=440;dst.BytesPerRow=192;
 FILE*f=fopen(argv[1],"rb");assert(f);fseek(f,60,SEEK_SET);assert(fread(source,1,sizeof(source),f)==sizeof(source));fclose(f);
 for(int p=0;p<4;p++){src.Planes[p]=source[p];dst.Planes[p]=dest[p]+2;}
 long baseline=0,total=0,cases=0;
 for(int phase=0;phase<3;phase++){
  if(phase)for(int p=0;p<4;p++)for(int col=0;col<220;col++)for(int y=0;y<208;y++)for(int b=0;b<2;b++)
   source[p][y*440+col*2+b]=y<drownedColumnTops[col]?0:(UBYTE)(y*17+col*31+p+phase*43+b);
  for(int x=0;x<3520;x+=16)for(int dir=-1;dir<=1;dir++){
   int old=dir?x+dir*512:-1;if(dir&&(old<0||old>=3520))continue;
   memset(dest,0xa5,sizeof(dest));
   if(old>=0)for(int p=0;p<4;p++)for(int y=0;y<208;y++)for(int c=0;c<3;c++)for(int b=0;b<2;b++)
    dest[p][2+y*192+(x%512)/8+c*64+b]=y<drownedColumnTops[old/16]?0:(UBYTE)(source[p][y*440+old/8+b]^0x55);
   memcpy(expected,dest,sizeof(dest));
   for(int p=0;p<4;p++)for(int y=0;y<208;y++)for(int c=0;c<3;c++)
    memcpy(expected[p]+2+y*192+(x%512)/8+c*64,source[p]+y*440+x/8,2);
   copied=0;
   if(dir==-1){t.origin=old;prototypeRollTarget(&t,old+16);assert(t.origin==old+16);}
   else if(dir==1){t.origin=x+16;prototypeRollTarget(&t,x);assert(t.origin==x);}
   else drownedBlitColumn(&t,x,old);
   assert(!hw->bltsize);
   assert(!memcmp(dest,expected,sizeof(dest)));assert(copied<=208*24);
   if(old>=0){baseline+=208*24;total+=copied;}cases++;
  }
 }
 printf("PASS: %ld actual DMA cases, old/new bounds, both scroll directions, dynamic rows, full fallback, guard pixels; bytes %ld vs %ld\n",cases,total,baseline);
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'t.c').write_text(pre.replace('/* ACTUAL_ROLL */',roll))
 subprocess.run(['cc','-O2','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_JOINED','-DSPARKPAW_DROWNED_COLUMN_BLIT','-I'+str(r/'src'),'-I'+str(r/'build/drowned-joined'),str(p/'t.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t'),str(r/'build/drowned-joined/assets/drowned-route.spbm')],check=True)
# Profile route camera approximately follows player-144; report every possible
# aligned single-column exchange whose evicted x lies in the late-ferry window.
late=[min(tops[x//16],tops[(x+512)//16]) for x in range(2528,3008,16)]
print('Late-route paired blank rows:',min(late),max(late),'mean',sum(late)/len(late),'of208')
