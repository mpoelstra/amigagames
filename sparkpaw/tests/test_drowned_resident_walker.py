"""Compare actual staged/resident renderer frame selection for every slot/pose."""
from pathlib import Path
import re,tempfile,subprocess
R=Path(__file__).resolve().parents[1];s=(R/'src/renderer.c').read_text()
def func(name):
 m=re.search(r'static (?:BOOL|UWORD \*)\s*'+name+r'\([^)]*\)\s*\{',s);assert m,name
 end=m.end();depth=1
 while depth:
  depth+=(s[end]=='{')-(s[end]=='}');end+=1
 return s[m.start():end]
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);src='''#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
typedef int BOOL;typedef int16_t WORD;typedef uint16_t UWORD;typedef uint8_t UBYTE;typedef int32_t LONG;
#define TRUE 1
#define FALSE 0
#define MAX_ENEMIES 4
#define FRONT_PLANES 4
#define ENEMY_TYPE_CLOCKWORK_STORM_STRIDER 1
#define MEMF_CHIP 2
#define MEMF_CLEAR 0x10000
struct EnemyBobCache { WORD height,sourceWords;UBYTE frames;UWORD *mask,*bits; };
static struct EnemyBobCache enemyCaches[2];
static UWORD *striderStageMask,*striderStageBits;
static UBYTE striderStageFacing[4],striderStageFrame[4];
static BOOL striderStageValid[4];
static long allocated,copied;
static void *AllocMem(long bytes,int flags){allocated+=bytes;return calloc(1,bytes);}
static void CopyMem(void *a,void *b,long n){copied+=n;memcpy(b,a,n);}
'''
 src+='\n'.join(func(n) for n in ['enemyMaskRow','enemyBitsRow','prepareStriderStages','stageStriderFrame'])
 src+='''
int main(void){
 struct EnemyBobCache*c=&enemyCaches[1];int slot,facing,frame,i,repeat;UWORD*m,*b;
 c->height=64;c->sourceWords=5;c->frames=32;
 c->mask=malloc(64*320*2);c->bits=malloc(64*1280*2);
 for(i=0;i<64*320;i++)c->mask[i]=(UWORD)(i*31+7);
 for(i=0;i<64*1280;i++)c->bits[i]=(UWORD)(i*17+11);
 assert(prepareStriderStages());
 for(repeat=0;repeat<2;repeat++)for(frame=0;frame<32;frame++)for(facing=0;facing<2;facing++)for(slot=0;slot<4;slot++){
  assert(stageStriderFrame(slot,facing,frame,&m,&b));
  assert(!memcmp(m,c->mask+(facing*32+frame)*320,640));
  assert(!memcmp(b,c->bits+(facing*32+frame)*1280,2560));
 }
 assert(!stageStriderFrame(4,0,0,&m,&b));
 assert(!stageStriderFrame(0,0,32,&m,&b));
#ifdef SPARKPAW_DROWNED_RESIDENT_WALKER
 assert(allocated==0&&copied==0);
#else
 assert(allocated==12800&&copied>0);
#endif
 free(c->mask);free(c->bits);free(striderStageMask);free(striderStageBits);return 0;
}
'''
 (p/'t.c').write_text(src)
 for flags in [[],['-DSPARKPAW_DROWNED_RESIDENT_WALKER']]:
  subprocess.run(['cc','-std=c99','-fsanitize=address,undefined']+flags+[str(p/'t.c'),'-o',str(p/'t')],check=True)
  subprocess.run([str(p/'t')],check=True)
print('PASS: actual staged/resident selectors, all32 frames/both facings/4slots; pixel-word parity; resident0 copies/0 stage allocation')
