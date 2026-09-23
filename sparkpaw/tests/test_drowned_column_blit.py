"""Execute real column DMA setup against CPU pixel oracle and guarded buffers."""
from pathlib import Path
import subprocess,tempfile
R=Path(__file__).resolve().parents[1]
pre=r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
typedef int32_t LONG;typedef int16_t WORD;typedef uint16_t UWORD;typedef uint8_t UBYTE;
#define PROTOTYPE_RING_W 512
#define PROTOTYPE_RING_COPIES 3
#define FRONT_PLANES 4
#define WORLD_H 208
struct Bitmap { UWORD BytesPerRow; UBYTE *Planes[4]; } src,dst;
struct Asset { struct Bitmap *bitmap; } asset={&src},*frontClean=&asset;
struct PrototypeTarget { struct Bitmap *display; };
struct {UWORD bltcon0,bltcon1,bltafwm,bltalwm,bltamod,bltdmod,bltsize;
 UBYTE *bltapt,*bltdpt;} custom,*hw=&custom;
static UBYTE source[4][120*208],dest[4][192*208+4],expected[4][192*208+4];
static int blits;
static void platformWaitBlit(void) {
 int y,p,sourceOK=0,destOK=0;
 if(!hw->bltsize)return;
 assert(hw->bltcon0==0x09f0&&!hw->bltcon1);
 assert(hw->bltafwm==65535&&hw->bltalwm==65535);
 assert(hw->bltsize==((208<<6)|1));assert(hw->bltamod==118&&hw->bltdmod==190);
 for(p=0;p<4;p++) {
  uintptr_t a=(uintptr_t)hw->bltapt,d=(uintptr_t)hw->bltdpt;
  if(a>=(uintptr_t)source[p]&&a+207*120+2<=(uintptr_t)(source[p]+sizeof(source[p])))sourceOK++;
  if(d>=(uintptr_t)(dest[p]+2)&&d+207*192+2<=(uintptr_t)(dest[p]+sizeof(dest[p])-2))destOK++;
 }
 assert(sourceOK==1&&destOK==1);
 for(y=0;y<208;y++)memcpy(hw->bltdpt+y*192,hw->bltapt+y*120,2);
 hw->bltsize=0;blits++;
}
'''
main=r'''
int main(void) {
 int p,x,y,i,c;struct PrototypeTarget t={&dst};src.BytesPerRow=120;dst.BytesPerRow=192;
 for(p=0;p<4;p++) {src.Planes[p]=source[p];dst.Planes[p]=dest[p]+2;
 for(i=0;i<120*208;i++)source[p][i]=(UBYTE)(i*17+p*31);}
 for(x=0;x<960;x+=16) {
  memset(dest,0xa5,sizeof(dest));memcpy(expected,dest,sizeof(dest));blits=0;
  for(p=0;p<4;p++)for(y=0;y<208;y++)for(c=0;c<3;c++)
   memcpy(expected[p]+2+y*192+(x%512)/8+c*64,source[p]+y*120+x/8,2);
  drownedBlitColumn(&t,x,-1);assert(blits==12&&!hw->bltsize);
  assert(!memcmp(expected,dest,sizeof(dest)));
 }
 return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'t.c').write_text(pre+(R/'src/drowned_column_blit.h').read_text()+main)
 subprocess.run(['cc','-I'+str(R/'src'),'-std=c99','-fsanitize=address,undefined',str(p/'t.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t')],check=True)
print('PASS: real DMA register setup, all60 columns,4 planes,3 copies, row strides, guards and final wait')
