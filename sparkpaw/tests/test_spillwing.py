"""Real enemy AI, authored floor bounds, projectile ordering and respawn."""
from pathlib import Path
import subprocess,tempfile
R=Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'exec').mkdir();(p/'dos').mkdir()
 (p/'exec/types.h').write_text('''#ifndef T_H
#define T_H
#include <stdint.h>
#include <stddef.h>
typedef int BOOL;typedef int8_t BYTE;typedef uint8_t UBYTE;typedef int16_t WORD;typedef uint16_t UWORD;typedef int32_t LONG;typedef uint32_t ULONG;typedef void* BPTR;
#define TRUE 1
#define FALSE 0
#endif
''')
 (p/'dos/dos.h').write_text('#include <exec/types.h>\n')
 (p/'drowned_route_layout.h').write_text('static const struct EnemyPatrolSurface drownedRouteSurfaces[]={{160,576,200}};\nstatic const struct EnemySpawnCandidate drownedRouteSpawns[]={{280,280,-1,0,0,1,1}};\nstatic const WORD drownedRouteWater[]={240,608,1184,1376,1488,1600,1712,1824,1936,2048};\n')
 (p/'t.c').write_text(r'''
#include <assert.h>
#include "enemies.h"
#include "spillwing.h"
#include "level_data.h"
static BOOL solid(WORD x,WORD y){return y>=200;}
static struct Enemy *live(void){int i;for(i=0;i<MAX_ENEMIES;i++)if(enemyAt(i)->active)return enemyAt(i);return 0;}
int main(void){
 int t,x,y,f,seen=0,warnings=0,wasWarn=0;WORD hit,center;struct Enemy *e;
 enemiesInit(123);e=live();assert(e&&e->health==1&&ENEMY_W==24&&ENEMY_FRAMES==16);
 /* Both facings: point hit and swept projectile agree for every cell pixel.
    Transparent rotor/margins are not damaging; contact lies inside shot box. */
 for(f=0;f<2;f++){
  e->facingLeft=f;
  for(y=e->y-1;y<e->y+25;y++)for(x=(e->x>>8)-1;x<(e->x>>8)+25;x++){
   assert(enemiesFirstProjectileHitOnSweep(x,x,y,&hit)==spillwingHit(e,x,y));
   if(enemiesContactPlayer(x,y,x,y,&center))assert(spillwingHit(e,x,y));
  }
  assert(enemiesFirstProjectileHitOnSweep(0,600,e->y+14,&hit)&&hit==spillwingLeft(e));
  assert(enemiesFirstProjectileHitOnSweep(600,0,e->y+14,&hit)&&hit==spillwingRight(e));
 }
 /* Exercise many cycles, varying player's side during each committed dive. */
 for(t=0;t<2000;t++){
  LONG vx=e->attackVX;UBYTE state=e->traversalState;
  enemiesUpdate(100,solid,220,174,0);e=live();assert(e);
  assert(e->y>=120&&e->y<=164&&e->animFrame<10);
  assert((e->x>>8)>=160&&(e->x>>8)+24<=576);
  seen|=1<<e->traversalState;
  if(e->traversalState==1){warnings++;wasWarn=1;}
  if(state==2)assert(e->attackVX==vx);
 }
 assert(seen==7&&wasWarn&&warnings>=12);
 /* Copy one committed enemy: wildly different player positions give same arc. */
 e->traversalState=2;e->traversalTimer=0;e->attackVX=768;e->x=200*256;
 {struct Enemy a=*e,b=*e;for(t=0;t<32;t++){
  spillwingUpdate(&a,0,0);spillwingUpdate(&b,0,1000);
  assert(a.x==b.x&&a.y==b.y&&a.animFrame==b.animFrame);
 }}
 /* 1HP, hit flash then all four collapse frames, no post-death contact/score. */
 e->traversalState=0;
 assert(enemiesHitProjectile(spillwingLeft(e)+5,e->y+14)==PROJECTILE_ENEMY_KILL);
 assert(e->dying&&e->health==0&&enemiesConsumeScoreAward()==20);
 assert(!enemiesContactPlayer(0,0,1000,200,&center));seen=0;
 for(t=0;t<24;t++){
  enemiesUpdate(100,solid,220,174,0);
  if(e->active){assert(e->animFrame>=10&&e->animFrame<16);seen|=1<<e->animFrame;}
 }
 assert((seen&0xF000)==0xF000&&!e->active);
 for(t=0;t<800;t++)enemiesUpdate(1000,solid,1100,174,0);
 enemiesUpdate(100,solid,220,174,0);e=live();assert(e&&e->health==1&&!e->dying);
 assert(enemiesHitProjectile(spillwingLeft(e)+5,e->y+14)==PROJECTILE_ENEMY_KILL);
 assert(enemiesConsumeScoreAward()==0);
 return 0;
}
''')
 subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_SPILLWING','-DSPARKPAW_DROWNED_ROUTE','-DSPARKPAW_DROWNED_ENEMY_ART','-DSPARKPAW_DROWNED_ENCOUNTER','-DSPARKPAW_DROWNED_SLICE','-I'+str(p),'-I'+str(R/'src'),'-I'+str(R/'build/drowned-spillwing'),str(p/'t.c'),str(R/'src/drowned_slice.c'),str(R/'src/enemies.c'),str(R/'src/spillwing.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t')],check=True)
print('PASS: Spillwing fixed arc, mirrored point/sweep/contact,1HP,16-frame bounds, death/score/offscreen respawn')
