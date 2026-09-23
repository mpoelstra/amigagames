"""Checkpoint contract, regional counters and actual joined camera regression."""
from pathlib import Path
import tempfile,subprocess
R=Path(__file__).resolve().parents[1]
s=(R/'src/game.c').read_text();start=s.index('static void updateCamera(void)');end=s.index('\n#ifdef SPARKPAW_STORMRAIL_PROOF',start)
camera=s[start:end]
pre=r'''
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
typedef int16_t WORD;typedef uint16_t UWORD;typedef uint8_t UBYTE;typedef int32_t LONG;typedef uint32_t ULONG;typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define SCREEN_W 320
#ifdef SPARKPAW_DROWNED_JOINED
#define WORLD_W 3520
#else
#define WORLD_W 3392
#endif
struct PlayerState {LONG x;}; static struct PlayerState player;
static const struct PlayerState *playerState(void){return &player;}
static struct {WORD cameraX; UBYTE coreCollectTimer;} game;
#include "camera_contract.h"
#include "drowned_checkpoint.h"
#include "drowned_cadence_regions.h"
'''
main=r'''
int main(void){
 int x,y,g,t,n=0;
 for(x=2200;x<2400;x++)for(y=176;y<225;y++)for(g=0;g<2;g++){
  BOOL expected=g&&y>=198&&y<=200&&x+23>=2320&&x<2352;
  drownedCheckpointNewAttempt();assert(!drownedCheckpointActive());
  assert(drownedCheckpointTouch(x,x+23,y,g)==expected);
  if(expected){
   assert(drownedCheckpointActive()&&drownedCheckpointActivationTick()==1);
   assert(!drownedCheckpointTouch(x,x+23,y,g));
   for(t=0;t<200;t++)drownedCheckpointTick();assert(drownedCheckpointActivationTick()==32);
   drownedCheckpointAfterRespawn();assert(drownedCheckpointActive());
  }
  n++;
 }
 drownedCheckpointNewAttempt();assert(!drownedCheckpointActive());
 int bounds[]={1375,1376,2127,2128,2399,2400,2799,2800,3199,3200};
 int zones[]={0,1,1,2,2,3,3,4,4,5};
 for(t=0;t<10;t++)assert(drownedCadenceRegion(bounds[t])==zones[t]);
 for(x=0;x<3520;x++)drownedCadenceRecord(x,1+x%4);
 ULONG count=0,total=0;for(t=0;t<6;t++){count+=drownedCadenceRegions[t].intervals;total+=drownedCadenceRegions[t].fields;}
 assert(count==3520&&total==8800);
 player.x=3072L*256;game.cameraX=2800;
 for(t=0;t<200;t++)updateCamera();
#ifdef SPARKPAW_DROWNED_JOINED
 assert(game.cameraX==2928);assert((player.x>>8)-game.cameraX==144);
#else
 assert(game.cameraX==3072);
#endif
 player.x=(WORLD_W-32L)*256;for(t=0;t<200;t++)updateCamera();assert(game.cameraX==WORLD_W-320);
 player.x=36L*256;for(t=0;t<1000;t++)updateCamera();assert(game.cameraX==0);
 printf("PASS: %d checkpoint activation/no-repeat/life-persistence cases, region boundaries/totals, actual camera end/return\n",n);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as td:
 p=Path(td);(p/'exec').mkdir();(p/'exec/types.h').write_text('')
 (p/'test.c').write_text(pre+(R/'src/drowned_checkpoint.c').read_text()+camera+main)
 for joined in (False,True):
  subprocess.run(['cc','-O2','-fsanitize=address,undefined',*(['-DSPARKPAW_DROWNED_JOINED'] if joined else []),'-I'+str(p),'-I'+str(R/'src'),str(p/'test.c'),'-o',str(p/'test')],check=True)
  subprocess.run([str(p/'test')],check=True)
