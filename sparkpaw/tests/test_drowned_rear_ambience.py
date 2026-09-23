"""Actual C stage/blits, publication gate, Copper writes and native frame parity."""
from pathlib import Path
import json, subprocess, tempfile, sys
R=Path(__file__).resolve().parents[1];sys.path.insert(0,str(R/'tools'))
from preview_drowned_native import decode
from PIL import Image
M=json.loads((R/'build/drowned-full/rear-ambience.json').read_text())
blob=(R/'build/drowned-full/assets/drowned-amb.bin').read_bytes()
rear=decode(R/'build/drowned-full/assets/drowned-rear.spbm')
assert blob[:4]==b'DRA1' and len(blob)==M['fast_bytes']
approved=Image.open(R/'assets/concept/drowned-water-animation-v1/scene-3x.gif')
front=decode(R/'build/drowned-full/assets/drowned-route.spbm')
spec=json.loads((R/'assets/concept/drowned-water-animation-v1/manifest.json').read_text())
pal=[tuple(rear.getpalette()[i:i+3]) for i in range(0,24,3)]
def pixel(offset,stride,h,x,y):
    return sum(((blob[offset+p*stride*h+y*stride+x//8]>>(7-x%8))&1)<<p for p in range(3))
for phase in range(24):
    approved.seek(phase);proof=approved.convert('RGB')
    offset=4+phase*M['water_frame_bytes']
    for y in range(184,200):
        for x in range(320):
            pen=pixel(offset,M['water_stride'],14,952+x,y-185) if 185<=y<199 else rear.getpixel((952+x,y))
            rgb=pal[pen]
            for top,bottom,shift,strength in spec['shore_bands']:
                if top<=y<bottom and pen in (4,6):
                    delta=spec['shore_wave'][(phase+shift)%24]*strength//3
                    rgb=tuple(min(255,c+delta) for c in rgb)
            fp=front.getpixel((3808+x,y))
            if fp:rgb=tuple(front.getpalette()[fp*3:fp*3+3])
            assert rgb==proof.getpixel((x*3,y*3)),(phase,x,y,rgb)
fall=M['falls'][3]
for phase in range(4):
    original=Image.open(R/f'assets/concept/drowned-waterfall-preview-v1/frame-{phase}.png')
    for y in range(44):
        for x in range(32):assert pixel(fall['offset']+phase*fall['frame_bytes'],6,44,x+8,y)==original.getpixel((x,y))
c=r'''
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef uint8_t UBYTE;typedef uint16_t UWORD;typedef int16_t WORD;
typedef uint32_t ULONG;typedef int32_t LONG;typedef int BOOL;typedef void *APTR;typedef FILE *BPTR;
#define TRUE 1
#define FALSE 0
#define MEMF_FAST 1
#define MEMF_CHIP 2
#define MODE_OLDFILE 0
#define PLAYFIELD_GUARD_BYTES 4
#define SPARKPAW_DROWNED_FULL
#define SPARKPAW_AGA32_LEFT_GUARD
struct BitMap {UWORD BytesPerRow;UBYTE *Planes[3];};
struct PlanarAsset {struct BitMap *bitmap;};
static struct BitMap clean,display,*rearDisplay=&display;
static struct PlanarAsset asset,*rearWorld=&asset;
static struct {ULONG frameCounter;LONG cameraX;} state,*game=&state;
static struct {UWORD bltcon0,bltcon1,bltafwm,bltalwm,bltamod,bltdmod,bltsize;UBYTE *bltapt,*bltdpt;} custom,*hw=&custom;
static UWORD lists[2][896],*cop=lists[0],copPos,line;
static UBYTE prototypePreparedCopper;
static int blits,copies,copyBytes,dmaBytes;
static void cmove(UWORD reg,UWORD val){assert(copPos+2<896);cop[copPos++]=reg;cop[copPos++]=val;}
static void *AllocMem(LONG n,int flags){(void)flags;return calloc(1,n);}
static void FreeMem(void *p,LONG n){(void)n;free(p);}
static void CopyMem(const void *a,void *b,LONG n){assert(!hw->bltsize);memcpy(b,a,n);copies++;copyBytes+=n;}
static BPTR Open(const char *name,int mode){(void)mode;assert(strstr(name,"drowned-amb.bin"));return fopen("build/drowned-full/assets/drowned-amb.bin","rb");}
static LONG Read(BPTR f,void *p,LONG n){return fread(p,1,n,f);}
static void Close(BPTR f){fclose(f);}
static UWORD platformRasterLine(void){return line;}
static void platformWaitBlit(void){
 int row,bytes,height;UBYTE *src,*dst;
 if(!hw->bltsize)return;
 bytes=(hw->bltsize&63)*2;height=hw->bltsize>>6;
 assert(bytes>0&&height>0&&hw->bltcon0==0x09f0&&!hw->bltcon1);
 assert(hw->bltafwm==0xffff && hw->bltalwm==0xffff);
 src=hw->bltapt;dst=hw->bltdpt;
 {
 int allowed=(uintptr_t)src>=(uintptr_t)stage_begin &&
   (uintptr_t)src+(height-1)*(bytes+hw->bltamod)+bytes<=(uintptr_t)stage_end;
#ifdef SPARKPAW_DROWNED_REAR_DIRECT_WATER
 int p;
 for(p=0;p<3;p++)if((uintptr_t)src>=(uintptr_t)clean.Planes[p] &&
   (uintptr_t)src+(height-1)*(bytes+hw->bltamod)+bytes<=(uintptr_t)clean.Planes[p]+208*196)allowed=1;
#endif
 assert(allowed);
 }
 for(row=0;row<height;row++){memcpy(dst,src,bytes);src+=bytes+hw->bltamod;dst+=bytes+hw->bltdmod;}
 dmaBytes+=2*bytes*height;hw->bltsize=0;blits++;
}
'''
c=c.replace('static void platformWaitBlit','static UBYTE *stage_begin,*stage_end;\nstatic void platformWaitBlit')
c+='#include "drowned_rear_ambience.h"\n'
c+=r'''
int main(void){
 int plane,x,y,left,bytes,frame,k;UBYTE *expected;LONG off;
 asset.bitmap=&clean;clean.BytesPerRow=196;display.BytesPerRow=200;
 for(plane=0;plane<3;plane++){clean.Planes[plane]=calloc(208,196);display.Planes[plane]=calloc(208,200);}
 assert(prepareDrownedRearAmbience());stage_begin=drownedRearStage;stage_end=stage_begin+DRA_STAGE_BYTES;
 for(k=0;k<2;k++){cop=lists[k];copPos=0;buildDrownedShoreCopper(k);assert(copPos==64);}
 for(frame=0;frame<144;frame++){
   prototypePreparedCopper=frame&1;cop=lists[prototypePreparedCopper];game->frameCounter++;
   updateDrownedShoreCopper();
   for(k=0;k<12;k++) assert(cop[drownedShoreSlot[prototypePreparedCopper][k]]==drownedShoreValues[drownedRearPhase][k]);
 }
 for(frame=0;frame<24;frame++) for(left=0;left<=1200;left+=16){
   bytes=DRA_WIDTH/8-left/8;if(bytes>44)bytes=44;
   for(plane=0;plane<3;plane++){memset(clean.Planes[plane],0xa5,208*196);memset(display.Planes[plane],0xa5,208*200);}
   off=4+(LONG)frame*DRA_WATER_FRAME_BYTES;
   blits=copies=copyBytes=dmaBytes=0;copyDrownedRearRect(drownedRearFrames+off+left/8,DRA_WATER_STRIDE,left,185,bytes,14);
#ifdef SPARKPAW_DROWNED_REAR_DIRECT_WATER
   assert(blits==3 && dmaBytes==bytes*14*3*2);
#else
   assert(blits==6 && dmaBytes==bytes*14*3*4);
#endif
   assert(copies==42 && copyBytes==bytes*14*3);
   for(plane=0;plane<3;plane++) for(y=0;y<208;y++) for(x=0;x<196;x++){
     UBYTE value=0xa5;
     if(y>=185&&y<199&&x>=left/8&&x<left/8+bytes)
       value=drownedRearFrames[off+plane*DRA_WATER_STRIDE*14+(y-185)*DRA_WATER_STRIDE+x];
     assert(clean.Planes[plane][y*196+x]==value);
     assert(display.Planes[plane][y*200+4+x]==value);
     if(x<4) assert(display.Planes[plane][y*200+x]==0xa5);
   }
 }
 for(k=0;k<DRA_FALL_COUNT;k++) for(frame=0;frame<4;frame++){
   const struct DrownedRearFall *fall=&drownedRearFalls[k];
   for(plane=0;plane<3;plane++){memset(clean.Planes[plane],0xa5,208*196);memset(display.Planes[plane],0xa5,208*200);}
   off=fall->offset+frame*fall->frameBytes;
   blits=copies=copyBytes=dmaBytes=0;copyDrownedRearRect(drownedRearFrames+off,fall->stride,fall->x,fall->y,fall->stride,fall->height);assert(blits==6);
   assert(copies==1 && copyBytes==fall->stride*fall->height*3);
   for(plane=0;plane<3;plane++) for(y=0;y<208;y++) for(x=0;x<196;x++){
     UBYTE value=0xa5;
     if(y>=fall->y&&y<fall->y+fall->height&&x>=fall->x/8&&x<fall->x/8+fall->stride)
       value=drownedRearFrames[off+plane*fall->stride*fall->height+(y-fall->y)*fall->stride+x-fall->x/8];
     assert(clean.Planes[plane][y*196+x]==value);
     assert(display.Planes[plane][y*200+4+x]==value);
     if(x<4)assert(display.Planes[plane][y*200+x]==0xa5);
   }
 }
 for(frame=0;frame<1000;frame++){
   game->cameraX=(frame*37)%4801;game->frameCounter++;
   cop=lists[prototypePreparedCopper];updateDrownedShoreCopper();
   line=100;blits=0;publishDrownedRearAmbience();assert(!blits);
   line=0;publishDrownedRearAmbience();assert(blits<=6);
 }
 for(plane=0;plane<3;plane++){free(clean.Planes[plane]);free(display.Planes[plane]);}
 freeDrownedRearAmbience();assert(!drownedRearFrames&&!drownedRearStage);
 puts("PASS actual C: staged blits/guards, bounded scheduler, Copper phase and allocation lifecycle");return 0;
}
'''
with tempfile.TemporaryDirectory() as tmp:
    src=Path(tmp)/'proof.c';exe=Path(tmp)/'proof';src.write_text(c)
    for flags in ([],['-DSPARKPAW_DROWNED_REAR_STAGE_REFERENCE'],['-DSPARKPAW_DROWNED_REAR_DIRECT_WATER']):
        subprocess.run(['cc','-O1','-fsanitize=address,undefined',*flags,'-I'+str(R/'src'),'-I'+str(R/'build/drowned-full'),str(src),'-o',str(exe)],check=True)
        subprocess.run([str(exe)],cwd=R,check=True)
print('PASS approved water24frames and original waterfall4frames exact; native data parity')
