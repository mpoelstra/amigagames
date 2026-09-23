"""Actual counters: raw TOD phase pairs, wrap, regions, reset and pause gaps."""
from pathlib import Path
import subprocess
import tempfile

R = Path(__file__).resolve().parents[1]
C = r'''
#include <stdint.h>
#include <assert.h>
typedef uint8_t UBYTE; typedef uint16_t UWORD; typedef int16_t WORD;
typedef uint32_t ULONG;
#include "drowned_fps_counters.h"
int main(void) {
    int i; ULONG sum=0, fields=0;
    const int edges[]={1376,2128,2400,2800,3200,3888,4656};
    for(i=0;i<7;i++) {
        assert(fpsRegionFor(edges[i]-1)==i);
        assert(fpsRegionFor(edges[i])==i+1);
    }
    fpsRecord(0xfffffe,0,2810,100,100);
    fpsRecord(0xffffff,1,2811,100,101);
    fpsRecord(1,2,2812,100,102); /* 24-bit wrap, two */
    fpsRecord(1,1,2813,100,103); /* preserve zero, never clamp */
    fpsRecord(4,0,2814,100,104);
    assert(fpsTotal.intervals==4 && fpsTotal.fields==6);
    assert(fpsTotal.one==1 && fpsTotal.two==1 && fpsTotal.zero==1);
    assert(fpsTotal.threePlus==1 && fpsTotal.maximum==3);
    assert(fpsFerry[0][1].intervals==4);
    fpsRecord(11,0,2320,160,0); /* reset rebuild excluded from late bucket */
    fpsRecord(18,0,2320,160,1); /* second target rebuild */
    assert(fpsReset.intervals==2 && fpsReset.fields==14 && fpsResets==1);
    assert(fpsFerry[0][1].intervals==4);
    fpsRecord(19,0,2810,100,2);
    fpsRecord(20,0,2811,100,3);
    assert(fpsFerry[1][1].intervals==1);
    fpsRecord(21,0,2812,160,4);
    fpsRecord(22,0,2813,160,5);
    assert(fpsFerry[1][0].intervals==1);
    for(i=0;i<8;i++) {sum+=fpsRegion[i].intervals;fields+=fpsRegion[i].fields;}
    assert(sum==fpsTotal.intervals && fields==fpsTotal.fields);
    fpsPrimed=0;fpsResetTail=0; /* same operation as the pause hook */
    fpsRecord(1000,0,4800,150,6);
    assert(fpsTotal.intervals==sum); /* no pause/loading time */
    fpsRecord(1001,0,4800,150,7);
    assert(fpsRegion[7].one==1 && fpsTotal.intervals==sum+1);
    assert(fpsMaxPublishLine==2);
}
'''
with tempfile.TemporaryDirectory() as tmp:
    p = Path(tmp)
    for level1 in (False,True):
        source=C
        if level1:
            source='#define SPARKPAW_LEVEL1_RING_TEST\n'+source.replace(
                '1376,2128,2400,2800,3200,3888,4656',
                '512,1024,1536,2048,2560,3072,3328')
            for a,b in [('2810','2200'),('2811','2201'),('2812','2202'),
                        ('2813','2203'),('2814','2204'),('2320','1800'),('4800','3350')]:
                source=source.replace(a,b)
        (p / 'test.c').write_text(source)
        subprocess.run(['cc', '-O1', '-fsanitize=address,undefined', '-I'+str(R/'src'),
                        str(p/'test.c'), '-o', str(p/'test')], check=True)
        subprocess.run([str(p/'test')], check=True)

# Execute the real save quiescence path with music enabled, without defining
# the old profiler. A call to stopProfileTimer would fail this compilation.
platform = (R/'src/platform_amiga.c').read_text()
start = platform.index('void platformPrepareDebugFlush(void)')
end = platform.index('\n}\n', start) + 3
flush = r'''
#include <assert.h>
#define SPARKPAW_LEVEL1_MUSIC
#define FALSE 0
#define DMAF_ALL 0x7ff
#define DMAF_SETCLR 0x8000
#define DMAF_MASTER 0x200
static int audioInterruptsEnabled,interruptsDisabled,systemLocked;
static int nesting,stopped,disowned,permitted;
static unsigned oldDma=0x123,oldIntena=0x234;
static struct {unsigned dmacon,intena;} hw,*hardware=&hw;
static void Disable(void) {nesting++;}
static void Enable(void) {assert(nesting==1);nesting--;}
static void audioSetHardwareActive(int active) {assert(!active&&nesting==1);stopped++;}
static void DisownBlitter(void) {assert(!nesting&&stopped);disowned++;}
static void Permit(void) {assert(disowned);permitted++;}
''' + platform[start:end] + r'''
int main(void) {
 int music;
 for(music=0;music<2;music++) {
  audioInterruptsEnabled=music;interruptsDisabled=systemLocked=1;
  nesting=!music;stopped=disowned=permitted=0;
  platformPrepareDebugFlush();
  assert(!nesting&&!interruptsDisabled&&!systemLocked&&!audioInterruptsEnabled);
  assert(stopped==1&&disowned==1&&permitted==1);
  assert(hw.dmacon==(DMAF_SETCLR|DMAF_MASTER|oldDma));
  assert(hw.intena==(0x8000|oldIntena));
  platformPrepareDebugFlush();
  assert(stopped==1&&disowned==1&&permitted==1);
 }
}
'''
with tempfile.TemporaryDirectory() as tmp:
    p=Path(tmp); (p/'flush.c').write_text(flush)
    subprocess.run(['cc','-Werror=implicit-function-declaration','-fsanitize=address,undefined',
                    str(p/'flush.c'),'-o',str(p/'flush')],check=True)
    subprocess.run([str(p/'flush')],check=True)

renderer = (R/'src/renderer.c').read_text()
publish = renderer[renderer.index('BOOL rendererPublishGameplay('):]
assert publish.index('hw->copjmp1=0;') < publish.index('drownedFpsPublished(')
assert publish.index('drownedFpsPublished(') < publish.index('prototypeExposeHistoryUnion();')
assert publish.index('drownedFpsPublished(') < publish.index('publishDrownedRearAmbience();')
module = (R/'src/drowned_fps.c').read_text()
assert 'platformProfileTimer' not in module
assert 'exact_deadlines=0' in module and 'ownership_counters=unavailable' in module
assert 'post_run=complete' in module
print('PASS actual TOD counters: bins, wrap, previous-region attribution, reset pair and pause exclusion; actual flush balances music/OS ownership without profiler')
