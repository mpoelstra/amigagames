"""Actual projectile update: camera clipping matches pixel reference."""
from pathlib import Path
import subprocess,tempfile
R=Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory() as td:
 p=Path(td);(p/'exec').mkdir()
 (p/'exec/types.h').write_text('''#include <stdint.h>
typedef int BOOL;typedef int16_t WORD;typedef uint16_t UWORD;typedef int32_t LONG;typedef uint32_t ULONG;typedef uint8_t UBYTE;
#define TRUE 1
#define FALSE 0
''')
 (p/'t.c').write_text(r'''
#include <assert.h>
#include "projectiles.h"
static int target,hits,cam;
static BOOL solid(WORD x,WORD y){return FALSE;}
static BOOL firstSolid(WORD a,WORD b,WORD y,WORD *out){return FALSE;}
static UBYTE hit(WORD x,WORD y){assert(x>=cam&&x<cam+320);if(x==target){hits++;return PROJECTILE_ENEMY_KILL;}return 0;}
static BOOL sweep(WORD a,WORD b,WORD y,WORD *out){
 assert(a>=cam&&a<cam+320&&b>=cam&&b<cam+320);
 if((a<=target&&target<=b)||(b<=target&&target<=a)){*out=target;return TRUE;}return FALSE;
}
static void sound(void){}
static void run(int start,int facing,int at,int expected){
 struct Projectile *p;projectilesInit();hits=0;target=at;p=projectileAt(0);
 p->active=TRUE;p->drawn=TRUE;p->drawnX=42;p->drawnY=80;
 p->collisionX=start;p->x=(start-(facing?0:15))*256;p->y=100*256;
 p->vx=facing?-2300:2300;p->life=80;
 projectilesUpdate(cam,solid,firstSolid,hit,sweep,sound,sound);
 assert(hits==expected);assert(p->drawn&&p->drawnX==42&&p->drawnY==80);
 if(!expected)assert(!p->active);
}
int main(void){
 int c;for(c=0;c<3;c++){
 cam=c*400;
 run(cam+315,0,cam+320,0);run(cam+315,0,cam+319,1);
 run(cam+4,1,cam-1,0);run(cam+4,1,cam,1);
 run(cam+340,0,cam+342,0);run(cam-20,1,cam-23,0);
 }
 /* A hostile shot retains its previous world-space lifetime. */
 projectilesInit();{struct Projectile*p=projectileAt(6);p->active=TRUE;p->hostile=TRUE;p->x=1200*256;p->vx=1150;p->life=80;
 projectilesUpdate(0,solid,firstSolid,hit,sweep,sound,sound);assert(p->active&&p->life==79);}
 return 0;
}
''')
 for flags in [[],['-DSPARKPAW_PROJECTILE_PIXEL_SWEEP_REFERENCE']]:
  subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_SLICE',*flags,'-I'+str(p),'-I'+str(R/'src'),str(p/'t.c'),str(R/'src/projectiles.c'),'-o',str(p/'t')],check=True)
  subprocess.run([str(p/'t')],check=True)
print('PASS: actual optimized/pixel projectile updates, both viewport edges, scrolling, offscreen rejection, visible edge hits, Bob history, hostile lifetime')
