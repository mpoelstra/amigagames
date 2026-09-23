"""Actual ending camera and pickup predicates, with bounded state shims."""
from pathlib import Path
import subprocess,tempfile,re
R=Path(__file__).resolve().parents[1]
def fn(path,name):
 s=(R/path).read_text();a=s.index(name);a=s.rfind('\n',0,a)+1;b=s.index('{',a);depth=1;e=b+1
 while depth:depth+=(s[e]=='{')-(s[e]=='}');e+=1
 return s[a:e]
s='''#include <assert.h>
typedef int BOOL;typedef long LONG;typedef short WORD;
#define TRUE 1
#define FALSE 0
#define SPARKPAW_DROWNED_GOVERNOR
#define SPARKPAW_DROWNED_JOINED
#define WORLD_W 3520
#define SCREEN_W 320
struct PlayerState {LONG x;};static struct PlayerState player;
static struct PlayerState *playerState(void){return &player;}
static struct {LONG cameraX;int coreCollectTimer;} game;
static LONG cameraCenteredTarget(LONG x){return x-136;}
static BOOL finished;static BOOL governorComplete(void){return finished;}
'''+fn('src/game.c','static void updateCamera(void)')+fn('src/drowned_slice.c','BOOL levelPlayerTouchesStormstoneCore(')+'''
int main(void){
 /* Stop immediately after trigger: player must remain visible indefinitely. */
 player.x=1380L<<8;game.cameraX=1200;
 for(int i=0;i<300;i++){LONG old=game.cameraX;updateCamera();assert(game.cameraX>=old&&game.cameraX-old<=5&&1380-game.cameraX>=40);}
 assert(game.cameraX==1340);
 /* Walk, pause at every position, then reverse through the trigger. */
 game.cameraX=1240;
 for(int x=1376;x<=1680;x+=3){
  player.x=(LONG)x<<8;
  for(int pause=0;pause<40;pause++){updateCamera();assert(x-game.cameraX>=40&&x-game.cameraX<320);}
  if(x+27>=1624)assert(game.cameraX==1552);
 }
 for(int x=1680;x>=1300;x-=3){player.x=(LONG)x<<8;updateCamera();assert(x-game.cameraX>=40&&x-game.cameraX<320);}
 /* Uninterrupted fast approach still shows full station before pickup. */
 game.cameraX=1240;
 for(int x=1376;x<1624;x+=3){player.x=(LONG)x<<8;updateCamera();assert(x-game.cameraX>=40);if(x+27>=1624)assert(game.cameraX==1552);}
 assert(!levelPlayerTouchesStormstoneCore(1624,100,1656,148));finished=1;
 assert(levelPlayerTouchesStormstoneCore(1624,100,1656,148));
 assert(!levelPlayerTouchesStormstoneCore(1592,100,1623,148));
 assert(!levelPlayerTouchesStormstoneCore(1657,100,1682,148));
 assert(!levelPlayerTouchesStormstoneCore(1624,160,1656,200));
 return 0;}
'''
for offset in (0,3248):
 variant=s
 if offset:
  variant=variant.replace('#define WORLD_W 3520','#define WORLD_W 5120')
  head,main=variant.split('int main(void)',1)
  variant=head+'int main(void)'+re.sub(r'\b(1[2-6][0-9][0-9])(L?)\b',lambda m:str(int(m[1])+offset)+m[2],main)
 with tempfile.TemporaryDirectory() as d:
  p=Path(d);(p/'test.c').write_text(variant)
  subprocess.run(['cc','-DDROWNED_FINALE_OFFSET='+str(offset),'-fsanitize=address,undefined',str(p/'test.c'),'-o',str(p/'test')],check=True)
  subprocess.run([str(p/'test')],check=True)
print('PASS actual isolated/full end cameras and Rain pickup predicates')
