"""Execute real candidate state, collision, player physics and projectile sweep."""
from pathlib import Path
import subprocess, tempfile, sys
ROOT=Path(__file__).resolve().parents[1]
subprocess.run([sys.executable,str(ROOT/'tools/build_drowned_ferry.py')],check=True)
with tempfile.TemporaryDirectory() as directory:
    p=Path(directory)
    for folder in ('exec','dos','proto'): (p/folder).mkdir()
    (p/'exec/types.h').write_text('''#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
#include <stddef.h>
typedef int BOOL; typedef int8_t BYTE; typedef uint8_t UBYTE;
typedef int16_t WORD; typedef uint16_t UWORD; typedef int32_t LONG;
typedef uint32_t ULONG; typedef void *BPTR;
#define TRUE 1
#define FALSE 0
#endif
''')
    (p/'dos/dos.h').write_text('#define MODE_OLDFILE 1005\n')
    (p/'proto/dos.h').write_text('#include <exec/types.h>\nBPTR Open(const char*,LONG); LONG Read(BPTR,void*,LONG); LONG Close(BPTR);\n')
    (p/'test.c').write_text(r'''
#include <assert.h>
#include <stdio.h>
#include "drowned_slice.h"
#include "drowned_pontoon.h"
#include "collision.h"
#include "player.h"
#include "projectiles.h"
#include "level_data.h"
#include "enemies.h"
#include "collectibles.h"
BPTR Open(const char *name,LONG mode) { return fopen("build/drowned-ferry/assets/drowned-route.bin","rb"); }
LONG Read(BPTR f,void *p,LONG n) { return fread(p,1,n,f); }
LONG Close(BPTR f) { return fclose(f); }
void platformReadGameKeys(BOOL*a,BOOL*b,BOOL*c,BOOL*d,BOOL*e) {*a=*b=*c=*d=*e=0;}
BOOL platformSecondaryButtonHeld(void) { return 0; }
static void sound(void) {}
static void resetAt(int x,int y,int speed) {
    struct PlayerState *s; playerInit(); s=(struct PlayerState *)playerState();
    s->x=x*256; s->y=y*256; s->vx=speed; s->grounded=1;
}

int main(void){
 int t,launch,hold,backHold,success=0;struct PlayerState*p;WORD hit;
 setbuf(stdout,NULL);assert(collisionLoad());
 collectiblesInit();
 for(t=0;t<48;t++)assert(collectibleAt(t)->active==(t<7));
 assert(collectiblesCollect(544,121,575,159)==1);
 assert(collectiblesCollect(592,121,623,159)==1);
 assert(collectiblesCollect(544,121,623,159)==0);
 collectiblesResetPreservingProgress();
 assert(!collectibleAt(2)->active&&!collectibleAt(3)->active);
 assert(collisionFirstSolidOnSweep(500,650,168,&hit)&&hit==528);
 assert(collisionFirstSolidOnSweep(650,500,168,&hit)&&hit==623);
 /* Standing and crouching may not be carried through the deck's side. */
 for(hold=0;hold<2;hold++){
  drownedReset();resetAt(180,150,0);
  for(t=0;t<170;t++)playerUpdatePhysics(0,0,hold,0);
  assert((playerState()->x>>8)+27<528);
 }
 /* Search bounded human input timings; require a complete boat/deck/boat route. */
 for(backHold=12;backHold<=40&&!success;backHold+=2)for(launch=400;launch<=490&&!success;launch+=10)for(hold=20;hold<=60&&!success;hold+=2){
  int landed=0,jumped=0,back=0;drownedReset();resetAt(180,150,0);
  while(drownedPontoonX()<launch)playerUpdatePhysics(0,0,0,0);
  for(t=0;t<90;t++){
   playerUpdatePhysics(0,t<hold,0,t==0);
   if(t>2&&playerState()->grounded&&(playerState()->y>>8)==121){landed=1;break;}
   if((playerState()->y>>8)>208)break;
  }
  if(!landed)continue;

  /* Hold position until the boat has emerged, then jump towards its deck. */
  while((playerState()->x>>8)<590)playerUpdatePhysics(0,1,0,0);
  for(t=0;t<80;t++){
   playerUpdatePhysics(0,t<backHold,0,t==0);
   if(t>2&&playerState()->grounded&&(playerState()->y>>8)==drownedPontoonDeck()-39){back=1;break;}
   if((playerState()->y>>8)>208)break;
  }
  if(back){success=1;printf("boat/platform/boat launch=%d hold=%d returnTicks=%d\n",launch,hold,t);}
 }
 assert(success);
 enemiesInit(123);
 for(t=0;t<3000;t++){
  int i;enemiesUpdate(400,collisionSolidAt,580,140,0);
  for(i=0;i<MAX_ENEMIES;i++){
   struct Enemy *e=enemyAt(i);int y;
   if(!e->active)continue;
   for(y=e->y;y<e->y+24;y++)assert(!collisionSolidHorizontal(e->x>>8,(e->x>>8)+23,y));
  }
 }
 printf("PASS: real platform geometry, shot blocking, standing/crouched carry obstruction, boat/platform/boat route\n");
 return 0;
}

''')
    subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_FERRY','-DSPARKPAW_DROWNED_SPILLWING','-DSPARKPAW_SPILLWING_PAIR','-DSPARKPAW_DROWNED_SLICE','-DSPARKPAW_DROWNED_PONTOON','-DSPARKPAW_DROWNED_ROUTE','-DSPARKPAW_DROWNED_ENCOUNTER','-DSPARKPAW_DROWNED_ENEMY_ART','-DSPARKPAW_WORLD_W=2400',
        '-I'+str(p),'-I'+str(ROOT/'src'),'-I'+str(ROOT/'build/drowned-ferry'),str(p/'test.c'),
        *[str(ROOT/'src'/name) for name in ('drowned_slice.c','drowned_pontoon.c','collision.c','player.c','projectiles.c','enemies.c','spillwing.c','collectibles.c')],
        '-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],cwd=ROOT,check=True)
