"""Execute the real renderer patch transfer with a bounded host Blitter model."""
from pathlib import Path
import subprocess,tempfile
ROOT=Path(__file__).resolve().parents[1]
s=(ROOT/'src/renderer.c').read_text()
# Exercise the two-patch fallback. The preceding FULL/GOVERNOR/JOINED branches
# contain nested #endif directives, so splitting at the first one truncates it.
patch=s.split('static UBYTE *drownedStage;',1)[1].split('\n#endif\n\n#ifdef SPARKPAW_DROWNED_PONTOON',1)[0]
fallback=patch.split('\n#else\n#define DROWNED_PATCH_COUNT 2',1)[1]
fallback=fallback.replace('\n#endif\n\nstatic void drownedAnimatePatches', '\n\nstatic void drownedAnimatePatches', 1)
body='static UBYTE *drownedStage;\n#define DROWNED_PATCH_COUNT 2'+fallback
pre=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
typedef uint8_t UBYTE; typedef int8_t BYTE; typedef int16_t WORD;
typedef uint16_t UWORD; typedef int32_t LONG;
#define FRONT_PLANES 4
#define SCREEN_W 320
struct BitMap { UWORD BytesPerRow; UBYTE *Planes[4]; };
struct PlanarAsset { struct BitMap *bitmap; };
struct PrototypeTarget { int tag; } prototypeTarget[2];
struct { WORD cameraX; } gs,*game=&gs;
struct {UWORD bltcon0,bltcon1,bltafwm,bltalwm,bltamod,bltdmod,bltsize;
 UBYTE *bltapt,*bltdpt;} custom,*hw=&custom;
static struct PlanarAsset source,clean,*frontClean=&clean;
static UBYTE jf,gf,guard[612]; static int blits,copies;
static const struct PlanarAsset *assetsDrownedPatches(void){return &source;}
static UBYTE drownedJetFrame(void){return jf;}
static UBYTE drownedGateFrame(void){return gf;}
static void platformWaitBlit(void){
 int rows=hw->bltsize>>6,bytes=(hw->bltsize&63)*2,y;
 if(!hw->bltsize)return;
 assert(hw->bltapt==guard+1);assert((bytes==4&&rows==64)||(bytes==10&&rows==61));
 for(y=0;y<rows;y++)memcpy(hw->bltdpt+y*(bytes+hw->bltdmod),hw->bltapt+y*(bytes+hw->bltamod),bytes);
 hw->bltsize=0;blits++;
}
static void CopyMem(void *src,void *dst,LONG bytes){
 assert(!hw->bltsize);assert(dst==guard+1&&(bytes==256||bytes==610));memcpy(dst,src,bytes);
}
static void drownedCopyPatchRect(struct PrototypeTarget*t,WORD x,WORD y,WORD w,WORD h){
 assert(t==prototypeTarget||t==prototypeTarget+1);assert(x==448||x==768);
 assert((x==448&&y==131&&w==32&&h==64)||(x==768&&y==136&&w==80&&h==61));copies++;
}
'''
main=r'''
int main(void){
 struct BitMap src,cl;int p,f,y,x,before;
 source.bitmap=&src;clean.bitmap=&cl;src.BytesPerRow=2;cl.BytesPerRow=120;
 for(p=0;p<4;p++){src.Planes[p]=malloc(10844);cl.Planes[p]=malloc(120*208);
 for(f=0;f<23;f++)memset(src.Planes[p]+(f<9?f*256:2304+(f-9)*610),f*7+p,f<9?256:610);
 memset(cl.Planes[p],0xcd,120*208);}
 memset(guard,0xa5,sizeof(guard));drownedStage=guard+1;
 game->cameraX=0;jf=0;gf=9;drownedAnimatePatches();assert(blits==0);
 game->cameraX=448;
 for(f=0;f<23;f++){
 jf=f%9;gf=9+f%14;drownedAnimatePatches();assert(!hw->bltsize);
 for(p=0;p<4;p++)for(y=0;y<208;y++)for(x=0;x<120;x++){
 int expected=0xcd;
 if(y>=131&&y<195&&x>=56&&x<60)expected=jf*7+p;
 if(y>=136&&y<197&&x>=96&&x<106)expected=gf*7+p;
 assert(cl.Planes[p][y*120+x]==expected);
 }
 drownedSynchronizePatches(prototypeTarget);drownedSynchronizePatches(prototypeTarget+1);
 before=copies;drownedSynchronizePatches(prototypeTarget);assert(copies==before);
 before=blits;drownedAnimatePatches();assert(blits==before);
 assert(guard[0]==0xa5&&guard[611]==0xa5);
 }
 return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'t.c').write_text(pre+body+main)
 subprocess.run(['cc','-std=c99','-fsanitize=address,undefined',str(p/'t.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t')],check=True)
print('PASS: actual Fast-to-Chip transfer, DMA stage address, guards, all 23 frames, both target sync and offscreen culling')
