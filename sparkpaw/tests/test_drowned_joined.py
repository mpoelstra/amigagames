"""Execute real candidate state, collision, player physics and projectile sweep."""
from pathlib import Path
import subprocess, tempfile, sys
ROOT=Path(__file__).resolve().parents[1]
subprocess.run([sys.executable,str(ROOT/'tools/build_drowned_joined.py')],check=True)
sys.path.insert(0,str(ROOT/'tools'))
from preview_drowned_native import decode
game_source=(ROOT/'src/game.c').read_text()
reset_code=game_source[game_source.index('static void resetLevelRuntime(void)'):game_source.index('/* One attempt includes')]
reset_prelude='''
#include "drowned_checkpoint.h"
#include "camera_contract.h"
#define SCREEN_W 320
#define WORLD_W 3520
static struct { ULONG enemySeed,lastFieldCounter,score; WORD cameraX; UWORD frameCounter; UBYTE waterSplashTimer,coreCollectTimer,lives,diamonds; } game;
static ULONG platformFieldCounter(void) {return 1234;}
'''

joined=ROOT/'build/drowned-joined/assets';land=ROOT/'build/drowned-route/assets';ferry=ROOT/'build/drowned-ferry/assets'
front=decode(joined/'drowned-route.spbm')
assert front.crop((0,0,2400,208)).tobytes()==decode(land/'drowned-route.spbm').tobytes()
assert front.crop((2400,0,3520,208)).tobytes()==decode(ferry/'drowned-route.spbm').crop((160,0,1280,208)).tobytes()
for name in ('pump-walker.spbm','turbine-crab.spbm','drowned-patches.spbm','drowned-rear.spbm'):
 assert (joined/name).read_bytes()==(land/name).read_bytes()
for name in ('spillwing.spbm','pontoon-clip.bin'):
 assert (joined/name).read_bytes()==(ferry/name).read_bytes()

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
BPTR Open(const char *name,LONG mode) { return fopen("build/drowned-joined/assets/drowned-route.bin","rb"); }
LONG Read(BPTR f,void *p,LONG n) { return fread(p,1,n,f); }
LONG Close(BPTR f) { return fclose(f); }
void platformReadGameKeys(BOOL*a,BOOL*b,BOOL*c,BOOL*d,BOOL*e) {*a=*b=*c=*d=*e=0;}
BOOL platformSecondaryButtonHeld(void) { return 0; }
static void sound(void) {}
static void resetAt(int x,int y,int speed) {
    struct PlayerState *s; playerInit(); s=(struct PlayerState *)playerState();
    s->x=x*256; s->y=y*256; s->vx=speed; s->grounded=1;
}

static int jumpTo(int ax,int ay,int width,int bx,int by,int targetWidth){
 int offset,hold,run,t;
 for(run=0;run<2;run++)for(offset=-20;offset<width;offset+=2)for(hold=8;hold<=64;hold++){
  resetAt(ax+offset,ay-39,run?650:0);
  if(!collisionSolidAt(ax+offset+16,ay))continue;
  for(t=0;t<100;t++){
   playerUpdatePhysics(0,t<hold,0,t==0);
   if(t>2&&playerState()->grounded){
    int x=playerState()->x>>8,y=playerState()->y>>8;
    if(y==by-39&&x+27>=bx+3&&x+4<=bx+targetWidth-4){printf("jump %d,%d -> %d,%d offset%d hold%d run%d ticks%d\n",ax,ay,bx,by,offset,hold,run,t);return 1;}
    break;
   }
   if((playerState()->y>>8)>208)break;
  }
 }
 return 0;
}
/* CHECKPOINT_RESET_FUNCTION */
int main(void){
 int t,cam,i,seen=0,spawnMask=0;WORD hit;UWORD count;const struct EnemySpawnCandidate *spawns;
 setbuf(stdout,NULL);assert(collisionLoad());collectiblesInit();
 assert(ENEMY_TYPE_COUNT==3&&ENEMY_W==32&&ENEMY_FRAMES==22);
 for(t=0;t<48;t++)assert(collectibleAt(t)->active==(t<33));
 assert(levelWaterColumnAt(2400)&&levelWaterColumnAt(3199)&&!levelWaterColumnAt(3200));
 assert(!levelWaterColumnAt(2399));
 assert(collisionFirstSolidOnSweep(2740,2890,168,&hit)&&hit==2768);
 assert(collisionFirstSolidOnSweep(2890,2740,168,&hit)&&hit==2863);
 /* Existing gate still needs a visible panel, opens, resets. */
 drownedReset();drownedSetView(0);assert(drownedPanelHit(775,170)==PROJECTILE_ENEMY_MISS);
 drownedSetView(640);assert(drownedPanelHit(775,170)==PROJECTILE_ENEMY_HIT);
 for(t=0;t<50;t++)drownedTick();assert(!drownedGateSolid(800,180));
 drownedReset();assert(drownedGateSolid(800,180));
 assert(jumpTo(2768,160,96,2912,128,16));
 assert(jumpTo(2912,128,16,3008,96,16));
 assert(jumpTo(3008,96,16,3104,128,16));
 assert(jumpTo(3104,128,16,3200,200,120));
 /* Old geyser and precision geyser remain hazardous. */
 for(t=0;t<140;t++)drownedTick();assert(drownedJetTouches(460,160,467,190));
 assert(drownedJetTouches(1804,136,1811,166));
 /* Mandatory first ferry half, unchanged relative middle transfer timing. */
 drownedReset();resetAt(2420,150,0);
 for(t=0;t<1200&&drownedPontoonX()<2640;t++)playerUpdatePhysics(0,0,0,0);
 assert(t<1200);int landed=0;
 for(t=0;t<90;t++) {playerUpdatePhysics(0,t<40,0,t==0);
  if(t>2&&playerState()->grounded&&(playerState()->y>>8)==121){landed=1;break;}}
 assert(landed);
 for(t=0;t<200&&(playerState()->x>>8)<2830;t++)playerUpdatePhysics(0,1,0,0);
 assert(t<200);int back=0;
 for(t=0;t<80;t++){playerUpdatePhysics(0,t<12,0,t==0);
  if(t>2&&playerState()->grounded&&(playerState()->y>>8)==drownedPontoonDeck()-39){back=1;break;}}
 assert(back);drownedReset();assert(drownedPontoonX()==2400);
 /* No overhead bypass before middle deck. */
 for(i=2400;i<2768;i+=16)for(t=0;t<176;t+=16)assert(!collisionSolidAt(i,t));
 spawns=levelEnemySpawnCandidates(&count);assert(count==13);
 assert(spawns[6].type==2&&spawns[7].type==2);
 /* Sweep the full camera range and return; all three families activate,
    frames stay inside their own caches, flyers avoid static platforms. */
 enemiesInit(123);
 for(int pass=0;pass<2;pass++)for(int c=0;c<=3200;c+=32){
  cam=pass?3200-c:c;
  for(t=0;t<80;t++) {
   enemiesUpdate(cam,collisionSolidAt,cam+160,140,0);
   for(i=0;i<MAX_ENEMIES;i++){
    struct Enemy *e=enemyAt(i);if(!e->active)continue;
    assert(e->type<3);seen|=1<<e->type;spawnMask|=1<<e->spawnIndex;
    assert(e->animFrame<(e->type==1?32:e->type==2?16:22));
    if(e->type==2)for(int y=e->y;y<e->y+24;y++)assert(!collisionSolidHorizontal(e->x>>8,(e->x>>8)+23,y));
   }
  }
 }
 assert(seen==7&&spawnMask==8191);
 /* Exercise own hit/death rules and directional sweep for each family. */
 for(int type=0;type<3;type++){
  enemiesInit(123);cam=type==2?2500:200;
  for(t=0;t<10;t++)enemiesUpdate(cam,collisionSolidAt,cam+160,170,0);
  struct Enemy *e=0;for(i=0;i<MAX_ENEMIES;i++)if(enemyAt(i)->active&&enemyAt(i)->type==type){e=enemyAt(i);break;}
  assert(e);WORD x=(e->x>>8)+(type==1?25:10),y=e->y+(type==1?30:12);
  int hp=e->health;for(i=0;i<hp;i++){e->hitTimer=0;assert(enemiesHitProjectile(x,y)!=PROJECTILE_ENEMY_MISS);}
  assert(e->dying); if(type!=1)assert(e->deathTimer==(type==2?24:20));
 }
 /* Actual game's life-reset function with real resident subsystem resets. */
 drownedCheckpointNewAttempt();drownedReset();enemiesInit(123);projectilesInit();
 assert(!drownedCheckpointTouch(2320,2339,199,FALSE));
 assert(drownedCheckpointTouch(2320,2339,199,TRUE));
 assert(!drownedCheckpointTouch(2320,2339,199,TRUE));
 drownedSetView(640);assert(drownedPanelHit(775,170)==PROJECTILE_ENEMY_HIT);
 /* Death during activation and gate opening must retain both earned states. */
 collectibleAt(0)->active=FALSE;collectibleAt(0)->drawn=TRUE;
 projectileAt(0)->active=TRUE;projectileAt(0)->drawn=TRUE;projectileAt(0)->drawnX=3000;
 enemyAt(0)->drawn=TRUE;enemyAt(0)->drawnX=3100;
 game.score=555;game.diamonds=19;game.lives=2;game.waterSplashTimer=5;game.coreCollectTimer=2;
 resetLevelRuntime();
 assert((playerState()->x>>8)==2320&&(playerState()->y>>8)==161);
 assert(playerState()->health==PLAYER_MAX_HEALTH&&playerState()->invulnTimer==75);
 assert(game.cameraX==2176&&game.score==555&&game.diamonds==19&&game.lives==2);
 assert(game.waterSplashTimer==0&&game.coreCollectTimer==0&&game.lastFieldCounter==1234);
 assert(!collectibleAt(0)->active&&collectibleAt(0)->drawn);
 assert(!projectileAt(0)->active&&projectileAt(0)->drawn&&projectileAt(0)->drawnX==3000);
 assert(enemyAt(0)->drawn&&enemyAt(0)->drawnX==3100);
 assert(drownedCheckpointActive()&&drownedCheckpointActivationTick()==32);
 assert(!drownedGateSolid(800,180)&&drownedPontoonX()==2400);
 for(t=0;t<100;t++)playerUpdatePhysics(0,0,0,0);
 assert(drownedPontoonX()==2400&&(playerState()->y>>8)==161);
 resetLevelRuntime();assert(game.cameraX==2176&&drownedCheckpointActive());
 drownedCheckpointNewAttempt();resetLevelRuntime();
 assert(!drownedCheckpointActive()&&game.cameraX==0&&drownedGateSolid(800,180));
 assert((playerState()->x>>8)==36);
 puts("PASS: actual checkpoint life-reset, camera, gate, boat, progress and Bob restore history");
 puts("PASS: joined water/gate/geysers, boat-middle-boat/reset, all13spawns/3families, bounded frames, flyer collision and per-family death");
 return 0;
}

'''.replace('/* CHECKPOINT_RESET_FUNCTION */',reset_prelude+reset_code))
    subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_FERRY','-DSPARKPAW_DROWNED_SPILLWING','-DSPARKPAW_SPILLWING_PAIR','-DSPARKPAW_DROWNED_SLICE','-DSPARKPAW_DROWNED_PONTOON','-DSPARKPAW_DROWNED_ROUTE','-DSPARKPAW_DROWNED_ENCOUNTER','-DSPARKPAW_DROWNED_ENEMY_ART','-DSPARKPAW_WORLD_W=3520','-DSPARKPAW_DROWNED_JOINED',
        '-I'+str(p),'-I'+str(ROOT/'src'),'-I'+str(ROOT/'build/drowned-joined'),str(p/'test.c'),
        *[str(ROOT/'src'/name) for name in ('drowned_checkpoint.c','drowned_slice.c','drowned_pontoon.c','collision.c','player.c','projectiles.c','enemies.c','spillwing.c','collectibles.c')],
        '-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],cwd=ROOT,check=True)
