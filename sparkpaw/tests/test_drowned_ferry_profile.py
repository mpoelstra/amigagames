"""Actual scope selector/macro: one timer pair per late-ferry frame, no nesting."""
from pathlib import Path
import tempfile,subprocess
r=Path(__file__).resolve().parents[1];s=(r/'src/performance_profile.c').read_text();a=s.index('static WORD ferrySelected');b=s.index('\n#endif',a);selector=s[a:b]
code=r'''
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
typedef int16_t WORD;typedef uint8_t UBYTE;typedef uint32_t ULONG;typedef int BOOL;typedef void *BPTR;
#define SPARKPAW_RENDER_DIAGNOSTIC
#define SPARKPAW_MINIMAL_CADENCE_DIAGNOSTIC
#define SPARKPAW_DROWNED_FERRY_PROFILE
#include "performance_profile.h"
static int begins,ends,work;
static int seen[PERF_SLOT_COUNT];
ULONG (performanceProfileBegin)(void){begins++;return 0;}
void (performanceProfileEnd)(enum PerformanceProfileSlot slot,ULONG start){ends++;seen[slot]++;}
'''+selector+r'''
int main(void){
 for(int i=0;i<3200;i++){
  int x=i%4==0?2799:i%4==1?2800:i%4==2?3199:3200;
  performanceFerryFrame(x,100);int before=begins;
  DROWNED_MEASURE(PERF_GAME_UPDATE,{
   DROWNED_MEASURE(PERF_ENEMIES,{work++;});
  });
  DROWNED_MEASURE(PERF_COPPER_PATCH,{work++;});
  DROWNED_MEASURE(PERF_BOB_PASS,{
   DROWNED_MEASURE(PERF_BOB_ENEMY_RESTORE,{work++;});
   DROWNED_MEASURE(PERF_BOB_WATER,{work++;});
   DROWNED_MEASURE(PERF_BOB_COMPACT_TARGET,{
    DROWNED_MEASURE(PERF_BLITTER_WAIT,{work++;});
    DROWNED_MEASURE(PERF_RING_ROLL,{work++;});
    DROWNED_MEASURE(PERF_RING_DYNAMIC,{work++;});
   });
   DROWNED_MEASURE(PERF_BOB_ENEMY_DRAW,{work++;});
  });
  assert(begins-before==(x>=2800&&x<3200));assert(begins==ends);
 }
 assert(ferryFrames==1600&&ferryUpperFrames==1600&&work==3200*8);
 assert(seen[PERF_RING_ROLL]>0&&seen[PERF_RING_DYNAMIC]>0&&seen[PERF_BLITTER_WAIT]>0);
 assert(!seen[PERF_COPPER_PATCH]&&!seen[PERF_ENEMIES]&&!seen[PERF_BOB_ENEMY_RESTORE]);
 puts("PASS: actual selector/macro,3200frames, boundaries and nesting, exactly one timer pair per lateferry frame");
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'exec').mkdir();(p/'dos').mkdir();(p/'exec/types.h').write_text('');(p/'dos/dos.h').write_text('');(p/'t.c').write_text(code)
 subprocess.run(['cc','-O2','-fsanitize=address,undefined','-I'+str(p),'-I'+str(r/'src'),str(p/'t.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t')],check=True)
