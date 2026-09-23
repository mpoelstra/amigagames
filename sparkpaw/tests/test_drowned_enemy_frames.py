"""Precomputed addresses vs original layout, all families/facings/frames, rebuild."""
from pathlib import Path
import tempfile,subprocess
r=Path(__file__).resolve().parents[1]
s=r'''
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
typedef int16_t WORD;typedef uint16_t UWORD;typedef uint8_t UBYTE;typedef int32_t LONG;typedef int BOOL;
#define FALSE 0
#define TRUE 1
#define ENEMY_TYPE_COUNT 3
#define FRONT_PLANES 4
struct EnemyBobCache {UWORD *mask,*bits;UWORD height,sourceWords,frames;};
static struct EnemyBobCache enemyCaches[3];
#include "drowned_enemy_frames.h"
int main(void) {
 const int heights[]={24,64,24},words[]={3,5,3},frames[]={22,32,16};
 for(int pass=0;pass<2;pass++)for(int type=0;type<3;type++) {
  struct EnemyBobCache *c=&enemyCaches[type];
  c->height=heights[type];c->sourceWords=words[type];c->frames=frames[type];
  long n=2L*c->frames*c->height*c->sourceWords;
  c->mask=calloc(n,sizeof(UWORD));c->bits=calloc(n*4,sizeof(UWORD));
  assert(prepareDrownedEnemyFrames(c));
  for(int f=0;f<2;f++)for(int fr=0;fr<c->frames;fr++) {
   long pattern=f*c->frames+fr;
   assert(drownedEnemyFrames[type][f][fr].mask==c->mask+pattern*c->height*c->sourceWords);
   assert(drownedEnemyFrames[type][f][fr].bits==c->bits+pattern*4*c->height*c->sourceWords);
   drownedEnemyFrames[type][f][fr].mask[c->height*c->sourceWords-1]=123;
   drownedEnemyFrames[type][f][fr].bits[4*c->height*c->sourceWords-1]=456;
  }
  c->frames=33;assert(!prepareDrownedEnemyFrames(c));
  free(c->mask);free(c->bits);
 }
 return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'t.c').write_text(s)
 subprocess.run(['cc','-O2','-fsanitize=address,undefined','-I'+str(r/'src'),str(p/'t.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t')],check=True)
print('PASS: all140 native frame addresses, both facings/3families, rebuild and frame-limit guard')
