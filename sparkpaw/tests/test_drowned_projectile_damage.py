"""Actual player damage/bounds + actual projectile movement, both directions."""
from pathlib import Path
import tempfile,subprocess,re
R=Path(__file__).resolve().parents[1]
s=(R/'src/player.c').read_text()
def function(name):
 m=re.search(r'(?:static )?(?:void|BOOL) '+name+r'\([^)]*\)\s*\{',s);assert m
 end=m.end();depth=1
 while depth:
  depth+=(s[end]=='{')-(s[end]=='}');end+=1
 return s[m.start():end]
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);(p/'exec').mkdir()
 (p/'exec/types.h').write_text('''#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
typedef int BOOL;typedef int8_t BYTE;typedef uint8_t UBYTE;typedef int16_t WORD;typedef uint16_t UWORD;typedef int32_t LONG;typedef uint32_t ULONG;
#define TRUE 1
#define FALSE 0
#endif
''')
 defs='\n'.join(re.findall(r'^#define (?:FIX_SHIFT|CONTACT_\w+) .+$',s,re.M))
 code='''#include <assert.h>
#include <string.h>
#include "player.h"
#include "projectiles.h"
static struct PlayerState player;
static BOOL crouchInputHeld;
static BOOL canStand(WORD x,WORD y){return TRUE;}
'''+defs+'\n'+'\n'.join(function(n) for n in ['playerContactBounds','playerProjectileBounds','playerShowsLowPose','playerTakeEnemyHit'])+r'''
static BOOL solid(WORD x,WORD y){return FALSE;}
static BOOL sweep(WORD a,WORD b,WORD y,WORD *x){return FALSE;}
static UBYTE hit(WORD x,WORD y){return 0;}
static void sound(void){}
static int shoot(BOOL left,WORD height,BOOL headBounds){
 WORD l,t,r,b,c;int i;
 projectilesInit();
 assert(projectilesSpawnEnemy(left?280:120,height,left));
 for(i=0;i<50;i++){
  projectilesUpdate(0,solid,sweep,hit,sweep,sound,sound);
  if(headBounds)playerProjectileBounds(&l,&t,&r,&b);
  else playerContactBounds(&l,&t,&r,&b);
  if(projectilesContactPlayer(l,t,r,b,&c))return playerTakeEnemyHit(c)?2:1;
 }
 return 0;
}
int main(void){
 int left;WORD l,t,r,b;
 for(left=0;left<2;left++){
  memset(&player,0,sizeof(player));player.x=200L<<8;player.y=161L<<8;player.health=6;
  playerContactBounds(&l,&t,&r,&b);assert(t==168);
  assert(shoot(left,159,FALSE)==0); /* reproduce visible head shot miss */
  assert(shoot(left,159,TRUE)==2&&player.health==5&&player.invulnTimer);
  assert(shoot(left,159,TRUE)==1&&player.health==5); /* no double damage */
  player.invulnTimer=0;player.crouching=TRUE;
  assert(shoot(left,159,TRUE)==0&&player.health==5); /* duck underneath */
  player.crouching=FALSE;player.y=140L<<8;
  assert(shoot(left,159,TRUE)==2&&player.health==4); /* airborne torso */
  player.invulnTimer=0;player.y=161L<<8;
  assert(shoot(left,205,TRUE)==0&&player.health==4); /* below feet */
 }
 return 0;
}
'''
 (p/'t.c').write_text(code)
 subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_ENEMY_ART','-I'+str(p),'-I'+str(R/'src'),str(p/'t.c'),str(R/'src/projectiles.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t')],check=True)
assert 'playerProjectileBounds(&playerLeft,&playerTop,&playerRight,&playerBottom)' in (R/'src/game.c').read_text()
print('PASS: reproduced torso-box miss, head hits both directions, health/iframes, crouch evasion, airborne hit, below-feet miss')
