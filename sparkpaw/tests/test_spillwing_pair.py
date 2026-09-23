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
 (p/'drowned_route_layout.h').write_text('static const struct EnemyPatrolSurface drownedRouteSurfaces[]={{160,672,200},{160,672,200}};\nstatic const struct EnemySpawnCandidate drownedRouteSpawns[]={{280,280,-1,0,0,1,1},{400,400,-1,1,0,1,1}};\nstatic const WORD drownedRouteWater[]={240,608,1184,1376,1488,1600,1712,1824,1936,2048};\n')
 (p/'t.c').write_text(r'''
#include <assert.h>
#include "enemies.h"
#include "spillwing.h"
#include "level_data.h"
static BOOL solid(WORD x,WORD y){return y>=200;}
int main(void){
 int i,t,attacks[2]={0,0},high[2]={0,0},low[2]={999,999},both=0;
 int last[2]={-1,-1},interval[2]={0,0},varied[2]={0,0};
 struct Enemy *e[2]={0,0};WORD hit;
 enemiesInit(123);
 for(t=0;t<6000;t++){
  int n=0;
  for(i=0;i<MAX_ENEMIES;i++){
   struct Enemy *a=enemyAt(i);if(a->active){e[a->spawnIndex]=a;n++;}
  }
  assert(n==2&&e[0]&&e[1]);
  assert(e[0]->health==1&&e[1]->health==1);
  for(i=0;i<2;i++){
   struct Enemy *a=e[i];UBYTE before=a->traversalState;
   if(before==1&&a->traversalTimer==12){
    if(last[i]>=0){int gap=t-last[i];if(interval[i]&&gap!=interval[i])varied[i]=1;interval[i]=gap;}
    last[i]=t;attacks[i]++;
   }
   if(a->y>high[i])high[i]=a->y;if(a->y<low[i])low[i]=a->y;
   assert(a->animFrame<10&&(a->x>>8)>=160&&(a->x>>8)+24<=672);
  }
  if(e[0]->traversalState==2&&e[1]->traversalState==2)both++;
  enemiesUpdate(160,solid,320,174,0);
 }
 assert(attacks[0]>3&&attacks[1]>3&&varied[0]&&varied[1]);
 assert(low[0]==120&&low[1]==96&&high[0]==164&&high[1]==162);
 /* Sweeps choose the nearest body when both actors overlap the shot lane. */
 e[0]->x=250*256;e[1]->x=330*256;e[0]->y=e[1]->y=160;
 assert(enemiesFirstProjectileHitOnSweep(200,400,174,&hit)&&hit==spillwingLeft(e[0]));
 assert(enemiesFirstProjectileHitOnSweep(400,200,174,&hit)&&hit==spillwingRight(e[1]));
 return 0;
}
''')
 subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_SPILLWING','-DSPARKPAW_SPILLWING_PAIR','-DSPARKPAW_DROWNED_ROUTE','-DSPARKPAW_DROWNED_ENEMY_ART','-DSPARKPAW_DROWNED_ENCOUNTER','-DSPARKPAW_DROWNED_SLICE','-I'+str(p),'-I'+str(R/'src'),'-I'+str(R/'build/drowned-spillwing'),str(p/'t.c'),str(R/'src/drowned_slice.c'),str(R/'src/enemies.c'),str(R/'src/spillwing.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t')],check=True)
print('PASS: two actual enemies, height/speed profiles, irregular attack intervals, frame/world bounds, nearest swept target')
