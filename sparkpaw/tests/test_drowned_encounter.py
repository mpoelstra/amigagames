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
 (p/'t.c').write_text(r'''
#include <assert.h>
#include "drowned_slice.h"
#include "level_data.h"
#include "enemies.h"
static int shots;
static BOOL fire(WORD x,WORD y,BOOL left){
#ifdef SPARKPAW_DROWNED_ENEMY_ART
 assert(y==159);
#endif
 shots++;return TRUE;}
static BOOL solid(WORD x,WORD y){return y>=200&&!levelWaterColumnAt(x);}
int main(void){
 int t,i,seen=0,jumpDirections=0,landDirections=0;WORD hit;UWORD n;struct Enemy *strider=0;
 const struct EnemySpawnCandidate *sp=levelEnemySpawnCandidates(&n);
 assert(n==2&&sp[0].required&&sp[1].required);
 assert(sp[0].policy==ENEMY_POLICY_RESPAWN&&sp[1].policy==ENEMY_POLICY_RESPAWN);
 drownedReset();drownedSetView(600);enemiesInit(123);
 for(t=0;t<6000;t++){
  enemiesUpdate(300,solid,470,174,fire);
  for(i=0;i<MAX_ENEMIES;i++){
   struct Enemy *e=enemyAt(i);int x=e->x>>8,w=e->type?64:32;
   if(!e->active)continue;
#ifdef SPARKPAW_DROWNED_ENEMY_ART
   assert(e->animFrame<(e->type?STRIDER_FRAMES:ENEMY_FRAMES));
#endif
   seen|=1<<e->type;
   assert(x+(e->type?12:1)>=e->patrolLeft&&x+w-1-(e->type?12:1)<e->patrolRight);
#ifdef SPARKPAW_DROWNED_JUMP_PROOF
   if(e->type&&e->traversalState){
    assert(!e->traversalFailed&&e->y>=108&&e->y<=136);
    assert(e->animFrame>=18&&e->animFrame<=23);
    if(e->animFrame==20)jumpDirections|=e->facingLeft?1:2;
    if(e->animFrame==22)landDirections|=e->facingLeft?1:2;
   } else
#endif
   assert(e->y==(e->type?136:176));
   if(e->type)strider=e;
  }
 }
 assert(seen==3&&shots>0&&strider);
#ifdef SPARKPAW_DROWNED_JUMP_PROOF
 assert(jumpDirections==3&&landDirections==3);
#endif
 /* Unseen/partially visible panel cannot be shot in either direction. */
 drownedSetView(0);
 assert(!drownedPanelSweep(730,810,174,&hit));
 assert(!drownedPanelSweep(810,730,174,&hit));
 assert(drownedPanelHit(768,174)==PROJECTILE_ENEMY_MISS);
 assert(drownedGateFrame()==9);
 drownedSetView(471);assert(!drownedPanelSweep(730,810,174,&hit));
 drownedSetView(472);assert(drownedPanelSweep(730,810,174,&hit));
 drownedSetView(761);assert(!drownedPanelSweep(810,730,174,&hit));
 drownedSetView(600);
 /* Both travel directions choose nearer obstruction; panel still opens. */
 assert(drownedEncounterSweep(300,810,174,&hit)&&hit<768);
 assert(drownedEncounterSweep(810,300,174,&hit)&&hit==783);
 assert(drownedEncounterHit(783,174)==PROJECTILE_ENEMY_HIT);
 assert(drownedGateFrame()==10);
 assert(drownedEncounterSweep(810,300,174,&hit)&&hit<768);
 /* Kill actual Strider, leave camera region and allow existing respawn policy. */
 for(t=0;t<3;t++){
  assert(drownedEncounterHit((strider->x>>8)+32,strider->y+38)!=PROJECTILE_ENEMY_MISS);
  for(i=0;i<30;i++)enemiesUpdate(300,solid,470,174,fire);
 }
 assert(enemiesConsumeScoreAward()==20);
 for(t=0;t<800;t++)enemiesUpdate(640,solid,800,174,fire);
 for(t=0;t<10;t++)enemiesUpdate(300,solid,470,174,fire);
 seen=0;for(i=0;i<MAX_ENEMIES;i++){struct Enemy*e=enemyAt(i);if(e->active&&e->type==1&&e->health==3&&!e->dying)seen=1;}
 assert(seen);
#ifdef SPARKPAW_DROWNED_ENEMY_ART
 /* Crab death stays inside its actual four-frame cache family. */
 for(i=0;i<MAX_ENEMIES;i++){
  struct Enemy *e=enemyAt(i);
  if(e->active&&e->type==0){
   int tries=0;
   while(!e->dying&&tries++<4)enemiesHitProjectile((e->x>>8)+16,e->y+12);
   assert(e->dying&&e->animFrame==10);
   for(t=0;t<20;t++){
    enemiesUpdate(300,solid,470,174,fire);
    if(e->active)assert(e->animFrame>=10&&e->animFrame<=13);
   }
  }
 }
#endif
 return 0;
}
''')
 for extra in [[],['-DSPARKPAW_DROWNED_ENEMY_ART'],['-DSPARKPAW_DROWNED_ENEMY_ART','-DSPARKPAW_DROWNED_JUMP_PROOF']]:
  subprocess.run(['cc','-std=c99','-fsanitize=address,undefined']+extra+['-DSPARKPAW_DROWNED_ENCOUNTER','-DSPARKPAW_DROWNED_SLICE','-I'+str(p),'-I'+str(R/'src'),str(p/'t.c'),str(R/'src/drowned_slice.c'),str(R/'src/enemies.c'),'-o',str(p/'t')],check=True)
  subprocess.run([str(p/'t')],check=True)
print('PASS: two real AI types, dry patrols, Strider shots, bidirectional target ordering, death/score/offscreen respawn')
