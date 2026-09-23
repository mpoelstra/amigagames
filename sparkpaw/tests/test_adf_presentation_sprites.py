"""Execute the production Copper builder with dirty sprite state and bounds checks."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
s=(root/'src/title.c').read_text()
body=s[s.index('static void buildCopper('):s.index('static BOOL allocateCopper(')]
shim=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
typedef unsigned char UBYTE; typedef unsigned short UWORD; typedef uintptr_t ULONG;
#define SPARKPAW_MULTI_ADF 1
#define DMAF_SPRITE 32
#define SCREEN_ROW_BYTES 40
#define DIWHIGH_PAL_320X256 0
#define BPLCON0_SIX_PLANES_AGA 0
#define BPLCON2_KILLEHB 0
#define BPLCON3_BORDER_BLANK 0
static UWORD lists[2][480],*copper[2]={lists[0],lists[1]},empty[8],*presentationNullSprite=empty;
static unsigned copperPos,buildCopperIndex;
struct Bitmap { UWORD BytesPerRow; UBYTE *Planes[6]; };
struct PlanarAsset { struct Bitmap *bitmap; };
static void cmove(UWORD reg,UWORD value){assert(copperPos+2<480);copper[buildCopperIndex][copperPos++]=reg;copper[buildCopperIndex][copperPos++]=value;}
static void cptr(UWORD reg,void *p){cmove(reg,(ULONG)p>>16);cmove(reg+2,(ULONG)p);}
static void writePalette(const struct PlanarAsset *a,UWORD level){unsigned i;(void)a;(void)level;for(i=0;i<132;i++)cmove(0x180,0);}
'''
checks=r'''
int main(void){struct Bitmap b={40,{0}};struct PlanarAsset a={&b};unsigned list,sp,i;UWORD regs[256];
for(list=0;list<2;list++) {for(i=0;i<256;i++)regs[i]=65535;
 buildCopper(&a,list,256);assert(copperPos<=480);
 assert(copper[list][0]==0x096&&copper[list][1]==32);
 for(i=0;i<copperPos-2;i+=2)regs[copper[list][i]/2]=copper[list][i+1];
 for(sp=0;sp<8;sp++) {assert(regs[(0x120+sp*4)/2]==(UWORD)((ULONG)empty>>16));assert(regs[(0x122+sp*4)/2]==(UWORD)(ULONG)empty);assert(!regs[(0x142+sp*8)/2]);assert(!regs[(0x144+sp*8)/2]);assert(!regs[(0x146+sp*8)/2]);}
}
printf("PASS: both Copper lists clear all eight sprite channels; %u/480 words\n",copperPos);}
'''
with tempfile.TemporaryDirectory() as td:
 p=Path(td);(p/'test.c').write_text(shim+body+checks)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-fsanitize=address,undefined',str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
