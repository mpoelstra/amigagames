"""Execute real candidate state, collision, player physics and projectile sweep."""
from pathlib import Path
import subprocess, tempfile, sys
ROOT=Path(__file__).resolve().parents[1]
subprocess.run([sys.executable,str(ROOT/'tools/build_drowned_route.py')],check=True)
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
#include "collision.h"
#include "player.h"
#include "projectiles.h"
#include "level_data.h"
#include "enemies.h"
BPTR Open(const char *name,LONG mode) { return fopen("build/drowned-route/assets/drowned-route.bin","rb"); }
LONG Read(BPTR f,void *p,LONG n) { return fread(p,1,n,f); }
LONG Close(BPTR f) { return fclose(f); }
void platformReadGameKeys(BOOL*a,BOOL*b,BOOL*c,BOOL*d,BOOL*e) {*a=*b=*c=*d=*e=0;}
BOOL platformSecondaryButtonHeld(void) { return 0; }
static void sound(void) {}
static void resetAt(int x,int y,int speed) {
    struct PlayerState *s; playerInit(); s=(struct PlayerState *)playerState();
    s->x=x*256; s->y=y*256; s->vx=speed; s->grounded=1;
}
static int jumpAcross(int start,int y,int destination) {
    int t; resetAt(start,y,650);
    for(t=0;t<80;t++) {
        playerUpdatePhysics(0,1,0,t==0);
        if((playerState()->y>>8)>165) return 0;
        if((playerState()->x>>8)>=destination&&playerState()->grounded) return 1;
    }
    return 0;
}

static BOOL fire(WORD x,WORD y,BOOL left){return TRUE;}
static void landingTest(int start,int y,int targetX,int targetY){
 int t,landed=0;resetAt(start,y,650);
 for(t=0;t<85;t++){
  playerUpdatePhysics(0,(playerState()->x>>8)<targetX-12,0,t==0);
  if(t>1&&playerState()->grounded){
   int x=playerState()->x>>8;
   if(x+27>=targetX&&x+4<targetX+(targetX==2128?240:32)&&(playerState()->y>>8)==targetY)landed=1;
   break;
  }
 }
 if(!landed)fprintf(stderr,"landing start%d final%d,%d target%d,%d\n",start,(int)(playerState()->x>>8),(int)(playerState()->y>>8),targetX,targetY);
 assert(landed); printf("landing %d -> %d: %d ticks\n",start,targetX,t+1);
}
int main(void){
 int seed,t,i,mask=0,peak=0;UWORD count;WORD cams[]={920,1170};
 drownedReset();drownedSetView(600);assert(collisionLoad());
 assert(levelEnemySpawnCandidates(&count)&&count==6);
 assert(levelEnemyTraversalLinks(&count)&&count==4);
 assert(LEVEL_WATER_COUNT==10);
 /* Precision landing on two narrow supports and then the far bank. */
 landingTest(1340,161,1456,137);
 landingTest(1452,137,1568,105);
 landingTest(1564,105,1680,73);
 landingTest(1676,73,1792,137);
 landingTest(1788,137,1904,105);
 landingTest(1900,105,2016,137);
 landingTest(2012,137,2128,161);
 for(t=0;t<135;t++)drownedTick();
 assert(drownedJetTouches(1804,128,1811,168));
 assert(!drownedJetTouches(1760,137,1787,175));
 for(t=0;t<50;t++)drownedTick();
 assert(!drownedJetTouches(1804,128,1811,168));
 /* New high platform is reachable using real player physics. */
 resetAt(900,161,650);
 for(t=0;t<70;t++){playerUpdatePhysics(0,1,0,t==0);if(t>1&&playerState()->grounded)break;}
 assert((playerState()->y>>8)==121);
 for(seed=1;seed<=8;seed++)for(i=0;i<2;i++){
  int slot;enemiesInit(seed);
  for(t=0;t<12000;t++){
   int active=0;enemiesUpdate(cams[i],collisionSolidAt,cams[i]+160,174,fire);
   for(slot=0;slot<MAX_ENEMIES;slot++){
    struct Enemy *e=enemyAt(slot);if(!e->active)continue;active++;
    assert(!e->traversalFailed);
    assert(e->animFrame<(e->type?STRIDER_FRAMES:ENEMY_FRAMES));
    if(e->type&&e->traversalState){
     assert(e->y>=72&&e->y<=140);
     if(e->animFrame==22){mask|=1<<e->traversalLink;
      assert(collisionSolidAt((e->x>>8)+12,e->y+64));
      assert(collisionSolidAt((e->x>>8)+51,e->y+64));
     }
     /* Inner load-bearing body must not pass through a platform side. */
     if(e->animFrame==20||e->animFrame==21){
      assert(!collisionSolidAt((e->x>>8)+12,e->y+48));
      assert(!collisionSolidAt((e->x>>8)+51,e->y+48));
     }
    }
   }
   if(active>peak)peak=active;
  }
 }
 assert(mask==15);printf("PASS: precision ledges, player high platform, all4 Walker links across8seeds, peak active%d\n",peak);
 return 0;
}
''')
    subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_SLICE','-DSPARKPAW_DROWNED_ROUTE','-DSPARKPAW_DROWNED_ENCOUNTER','-DSPARKPAW_DROWNED_ENEMY_ART','-DSPARKPAW_WORLD_W=2400',
        '-I'+str(p),'-I'+str(ROOT/'src'),'-I'+str(ROOT/'build/drowned-route'),str(p/'test.c'),
        *[str(ROOT/'src'/name) for name in ('drowned_slice.c','collision.c','player.c','projectiles.c','enemies.c')],
        '-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],cwd=ROOT,check=True)
