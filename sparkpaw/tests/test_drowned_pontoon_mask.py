"""Execute actual native mask builder against per-pixel water occlusion oracle."""
from pathlib import Path
import subprocess,tempfile
R=Path(__file__).resolve().parents[1]
s=(R/'src/renderer.c').read_text();a=s.index('static UBYTE waterPatternPen');b=s.index('static BOOL buildWaterPatterns',a)
h=(R/'src/drowned_pontoon_render.h').read_text();h=h[:h.index('static void restorePontoon')]
pre='''#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
typedef int16_t WORD;typedef uint16_t UWORD;typedef int32_t LONG;typedef uint8_t UBYTE;typedef int BOOL;typedef void *APTR;
typedef FILE *BPTR;
#define MODE_OLDFILE 0
static BPTR Open(const char *name,int mode){return fopen("build/drowned-pontoon/assets/pontoon-clip.bin","rb");}
static LONG Read(BPTR f,void *p,LONG n){unsigned char *b=malloc(n);LONG got=fread(b,1,n,f),i;for(i=0;i+1<got;i+=2)((UWORD*)p)[i/2]=(b[i]<<8)|b[i+1];free(b);return got;}
static void Close(BPTR f){fclose(f);}
#define SPARKPAW_DROWNED_FERRY 1
#define TRUE 1
#define FALSE 0
#define MEMF_CHIP 1
#define MEMF_CLEAR 2
#define MEMF_FAST 4
static void *AllocMem(long n,int flags){return calloc(1,n);}
#define CopyMem(a,b,n) memcpy(b,a,n)
'''
main='''int main(void){int bob,f,off,y,x;long cases=0;assert(preparePontoon());
for(bob=0;bob<2;bob++)for(f=0;f<16;f++)for(off=0;off<80;off++){
UWORD *m=pontoonClip+pontoonMaskOffset(PONTOON_LEFT+off,189+bob,f);
for(y=8;y<16;y++)for(x=0;x<112;x++){
int art=(pontoonArt[y*7+x/16]>>(15-x%16))&1;
int expected=art&&(x>=96||!waterPatternPen(f,(off+x)%80,189+bob+y-197));
assert(((m[(y-8)*7+x/16]>>(15-x%16))&1)==expected);cases++;
}}
assert(!memcmp(pontoonChip,pontoonArt,1120));free(pontoonChip);free(pontoonClip);
printf("PASS: %ld actual mask pixels, both deck heights,16 water phases,80 offsets, padding and immutable art\\n",cases);return 0;}
'''
with tempfile.TemporaryDirectory() as td:
 p=Path(td);(p/'exec').mkdir();(p/'exec/types.h').write_text('')
 (p/'t.c').write_text(pre+s[a:b]+h+main)
 subprocess.run(['cc','-O2','-fsanitize=address,undefined','-I'+str(p),'-I'+str(R/'src'),'-I'+str(R/'build/drowned-pontoon'),str(p/'t.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t')],cwd=R,check=True)
