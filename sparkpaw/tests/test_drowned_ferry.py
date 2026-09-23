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
BPTR Open(const char *name,LONG mode) { return fopen("build/drowned-pontoon/assets/drowned-route.bin","rb"); }
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
 int t,boarded=0,ends=0,seen=0,attack=0,shotlane=0,maxVisible=0;WORD l,top,rr,bottom;struct PlayerState *p;
 drownedReset();assert(collisionLoad());resetAt(110,161,650);
 for(t=0;t<90;t++){
  playerUpdatePhysics(0,(playerState()->x>>8)<174,0,t==0);
  if(t>2&&playerState()->grounded){boarded=1;break;}
 }
 assert(boarded);assert((playerState()->y>>8)==drownedPontoonDeck()-39);
 enemiesInit(123);
 /* Long ride, bob and reversals: no slipping, water damage or accumulated drift. */
 for(t=0;t<1200;t++){
  playerUpdatePhysics(0,0,0,0);
  {
   int i,visible=0,camera=(playerState()->x>>8)-144;WORD hit;
   if(camera<0)camera=0;
   enemiesUpdate(camera,collisionSolidAt,(playerState()->x>>8)+16,(playerState()->y>>8)+20,0);
   for(i=0;i<MAX_ENEMIES;i++){
    struct Enemy *e=enemyAt(i);int x=e->x>>8;
    if(!e->active)continue;
    seen|=1<<e->spawnIndex;
    if(e->traversalState==2)attack|=1<<e->spawnIndex;
    if(x+24>=camera&&x<camera+320)visible++;
    if(enemiesFirstProjectileHitOnSweep(x,x+24,(playerState()->y>>8)+17,&hit))shotlane|=1<<e->spawnIndex;
    assert(e->animFrame<16&&e->y>=96&&e->y<=164);
   }
   if(visible>maxVisible)maxVisible=visible;
  }
  assert(playerState()->grounded);
  assert(drownedPontoonX()>=160&&drownedPontoonX()+PONTOON_W<=960);
  if(drownedPontoonX()==160)ends|=1;
  if(drownedPontoonX()==864)ends|=2;
  assert((playerState()->y>>8)==drownedPontoonDeck()-39);
  assert(drownedPontoonSupport((playerState()->x>>8)+4,(playerState()->x>>8)+27,(playerState()->y>>8)+39));
  playerContactBounds(&l,&top,&rr,&bottom);assert(!levelPlayerTouchesWater(l,rr,bottom));
 }
 assert(ends==3);
 printf("coverage seen=%d attack=%d shotlane=%d peakVisible=%d\n",seen,attack,shotlane,maxVisible);
 assert(seen==31&&attack==31&&shotlane==31&&maxVisible<=4);
 /* Jump detaches: airborne player does not receive platform displacement. */
 p=(struct PlayerState*)playerState();p->vx=0;
 {LONG x=p->x;assert(playerUpdatePhysics(0,0,0,1));assert(!p->grounded);
  x=p->x;playerUpdatePhysics(0,0,0,0);assert(p->x==x);}
 /* Landing at the far bank from aboard a moving pontoon. */
 drownedReset();resetAt(180,150,0);
 for(t=0;t<352;t++)playerUpdatePhysics(0,0,0,0);
 assert(drownedPontoonX()==864);
 for(t=0;t<90;t++){
  playerUpdatePhysics(0,1,0,t==0);
  if(t>2&&playerState()->grounded&&((playerState()->y>>8)==161))break;
 }
 assert(t<90);assert((playerState()->x>>8)+27>=960);
 /* Return to left bank and jump ashore. */
 drownedReset();resetAt(180,150,0);
 for(t=0;t<704;t++)playerUpdatePhysics(0,0,0,0);
 assert(drownedPontoonX()==160);
 for(t=0;t<90;t++){
  playerUpdatePhysics(1,0,0,t==0);
  if(t>2&&playerState()->grounded&&((playerState()->y>>8)==161))break;
 }
 assert(t<90);assert((playerState()->x>>8)+4<160);
 /* Missing the float falls into water; it is not a solid water-wide platform. */
 drownedReset();resetAt(500,150,0);p=(struct PlayerState*)playerState();p->grounded=0;
 for(t=0;t<60;t++)playerUpdatePhysics(0,0,0,0);
 playerContactBounds(&l,&top,&rr,&bottom);assert(levelPlayerTouchesWater(l,rr,bottom));
 drownedReset();assert(drownedPontoonX()==160&&drownedPontoonDeck()==189);
 assert(!drownedPontoonSupport(251,253,189));
 printf("PASS: real boarding,1200ticks carry/bob/reversal,jump detachment,far-bank exit,missed landing and reset\n");
 return 0;
}

''')
    subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_FERRY','-DSPARKPAW_DROWNED_SPILLWING','-DSPARKPAW_SPILLWING_PAIR','-DSPARKPAW_DROWNED_SLICE','-DSPARKPAW_DROWNED_PONTOON','-DSPARKPAW_DROWNED_ROUTE','-DSPARKPAW_DROWNED_ENCOUNTER','-DSPARKPAW_DROWNED_ENEMY_ART','-DSPARKPAW_WORLD_W=2400',
        '-I'+str(p),'-I'+str(ROOT/'src'),'-I'+str(ROOT/'build/drowned-ferry'),str(p/'test.c'),
        *[str(ROOT/'src'/name) for name in ('drowned_slice.c','drowned_pontoon.c','collision.c','player.c','projectiles.c','enemies.c','spillwing.c')],
        '-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],cwd=ROOT,check=True)
