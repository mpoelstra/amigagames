"""Actual 24px cache builder versus independent per-pixel source oracle."""
from pathlib import Path
import re,tempfile,subprocess
R=Path(__file__).resolve().parents[1];s=(R/'src/renderer.c').read_text()
def func(name):
 m=re.search(r'static (?:BOOL|UWORD \*)\s*'+name+r'\([^)]*\)\s*\{',s);assert m,name
 end=m.end();depth=1
 while depth:
  depth+=(s[end]=='{')-(s[end]=='}');end+=1
 return s[m.start():end]
src='''#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
typedef int BOOL;typedef int16_t WORD;typedef uint16_t UWORD;typedef uint8_t UBYTE;typedef int32_t LONG;typedef uint32_t ULONG;
#define TRUE 1
#define FALSE 0
#define FRONT_PLANES 4
#define MEMF_FAST 4
#define MEMF_CHIP 2
#define MEMF_CLEAR 0x10000
struct Bitmap{int BytesPerRow;UBYTE *Planes[4];};
struct Asset{int width,height,rowBytes;UBYTE *mask;struct Bitmap *bitmap;};
struct EnemyBobCache{int width,height,frames,sourceWords,sourceLeftFirst;struct Asset *source;UWORD *mask,*bits;};
static void *AllocMem(long n,int flags){return calloc(1,n);}
'''
src+='\n'.join(func(n) for n in ['enemyMaskRow','enemyBitsRow','buildEnemyPatterns'])
src+='''
int main(void){
 UBYTE raw[5][384*6];struct Bitmap bm={6,{raw[1],raw[2],raw[3],raw[4]}};
 struct Asset a={48,384,6,raw[0],&bm};
 struct EnemyBobCache c={24,24,16,3,1,&a,0,0};int i,p,f,fr,y,x;
 for(p=0;p<5;p++)for(i=0;i<384*6;i++)raw[p][i]=(UBYTE)(i*37+p*19+(i>>4));
 assert(buildEnemyPatterns(&c,FALSE));
 for(f=0;f<2;f++)for(fr=0;fr<16;fr++)for(y=0;y<24;y++)for(x=0;x<48;x++){
  int sx=f*24+x,at=(fr*24+y)*6+(sx>>3),bit=0x80>>(sx&7),opaque=0;
  for(p=0;p<4;p++){
   int expected=x<24&&(raw[0][at]&bit)&&(raw[p+1][at]&bit);
   int actual=!!(enemyBitsRow(&c,f,fr,p,y)[x>>4]&(0x8000>>(x&15)));
   assert(actual==!!expected);opaque|=expected;
  }
  assert(!!(enemyMaskRow(&c,f,fr,y)[x>>4]&(0x8000>>(x&15)))==!!opaque);
 }
 free(c.mask);free(c.bits);return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'t.c').write_text(src)
 subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_SPILLWING',str(p/'t.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t')],check=True)
print('PASS: actual24px cache, both byte-aligned facings,16frames, masks/4planes/zero guards')
