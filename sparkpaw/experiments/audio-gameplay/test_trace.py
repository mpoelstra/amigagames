"""Actual trace C: selection, bounded storage, events and raw phase capture."""
from pathlib import Path
import re,tempfile,subprocess
here=Path(__file__).resolve().parent
source=re.sub(r'^#include[^\n]*\n','',(here/'trace.c').read_text(),flags=re.M).replace('(volatile struct Custom *)0xdff000','&fakeCustom')
shim=r'''
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
typedef uint32_t ULONG;typedef int32_t LONG;typedef uint16_t UWORD;
typedef void *BPTR;typedef char *STRPTR;
struct Custom {UWORD vposr,vhposr;};static struct Custom fakeCustom;
struct GameState {ULONG frameCounter;LONG cameraX;UWORD waterSplashTimer;};
struct PlayerState {UWORD health;};static struct GameState game;static struct PlayerState player;
static ULONG tod;
const struct GameState *gameState(void){return &game;}
const struct PlayerState *playerState(void){return &player;}
ULONG platformFieldCounter(void){return tod;}
static void FPrintf(BPTR f,const char *s,...){(void)f;(void)s;}
static void FPuts(BPTR f,const char *s){(void)f;(void)s;}
'''
checks=r'''
int main(void){
 unsigned i;player.health=6;fakeCustom.vposr=1;fakeCustom.vhposr=10<<8;
 traceBegin();assert(current.line[0]==266);
 tod=12;tracePhase(1);assert(current.field[1]==12);
 tracePhase(2);tracePhase(3);traceEnd(1,1);assert(!importantCount&&!shortCount);
 for(i=0;i<200;i++){traceBegin();traceEnd(0,1);}
 assert(shortCount==16&&shortDropped==184&&!importantCount);
 traceBegin();game.waterSplashTimer=40;traceEnd(1,1);assert(importantCount==1);
 traceBegin();game.waterSplashTimer=39;traceEnd(1,1);assert(importantCount==1);
 game.frameCounter=90;traceBegin();game.frameCounter=0;game.waterSplashTimer=0;traceEnd(7,3);
 assert(importantCount==2&&important[1].before==90&&important[1].after==0&&important[1].retries==3);
 traceBegin();player.health=5;traceEnd(1,1);assert(importantCount==3);
 for(i=0;i<100;i++){traceBegin();traceEnd(3,2);}
 assert(importantCount==96&&importantDropped==7);
 traceWrite(0);puts("trace: raw snapshots, event selection, independent capacities and overflow accounting PASS");return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'test.c').write_text(shim+source+checks)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-fsanitize=address,undefined',str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
