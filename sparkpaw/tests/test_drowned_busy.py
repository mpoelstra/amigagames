"""Actual sparse collector: capacity, reset attribution, clock rejection and gating."""
from pathlib import Path
import subprocess,tempfile,sys,importlib.util
R=Path(__file__).resolve().parents[1]
header=(R/'src/drowned_busy.h').read_text().replace('#include <exec/types.h>','')
body=(R/'src/drowned_busy.c').read_text()
body='\n'.join(l for l in body.splitlines() if not l.startswith('#include'))
body=body[:body.index('void drownedBusyWrite')]
c=r'''
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
typedef uint8_t UBYTE;typedef uint16_t UWORD;typedef int16_t WORD;
typedef uint32_t ULONG;typedef int32_t LONG;
#define SPARKPAW_DROWNED_BUSY
#define SPARKPAW_DROWNED_FPS
#define SPARKPAW_DROWNED_FULL
#define MEMF_FAST 1
#define MEMF_CLEAR 2
struct G {ULONG frameCounter,cameraX;} g;
struct PlayerState {LONG x,y;} p;
static struct G *gameState(void){return &g;}
static const struct PlayerState *playerState(void){return &p;}
static int fail;
static void *AllocMem(ULONG n,ULONG flags){assert(flags==3);return fail?0:calloc(1,n);}
static ULONG fields[2]={100,100};static UWORD lines[2]={40,40};
static unsigned fi,li;
static ULONG platformFieldCounter(void){return fields[fi++&1];}
static UWORD platformRasterLine(void){return lines[li++&1];}
'''+header+'\n'+body+r'''
int main(void){
 fail=1;drownedBusyInit();drownedBusyBegin();assert(!drownedBusyActive && !used);
 fail=0;drownedBusyInit();p.x=2850L<<8;p.y=100L<<8;g.frameCounter=100;
 drownedBusyBegin();assert(used==1 && drownedBusyActive && current->group==4);
 assert(current->stamp[0].valid && current->stamp[18].valid);
 drownedBusyCount(2,1);drownedBusyCount(4,192);assert(current->count[2]==1 && current->count[4]==192);
 fields[1]=101;drownedBusyMark(1);assert(!current->stamp[1].valid);
 fields[1]=100;lines[0]=255;lines[1]=0;drownedBusyMark(2);assert(!current->stamp[2].valid);
 lines[0]=4;lines[1]=4;drownedBusyMark(3);assert(current->stamp[3].valid);
 lines[0]=100;lines[1]=101;drownedBusyMark(4);assert(current->stamp[4].valid);
 drownedBusyMark(DB_REAR_END);assert(!drownedBusyActive);
 drownedBusyCount(2,1);assert(current->count[2]==1);
 for(int i=0;i<2000;i++){g.frameCounter++;drownedBusyBegin();}
 assert(groupUsed[4]==16 && used==16);
 g.frameCounter=0;drownedBusyBegin();assert(afterReset);
 for(int i=0;i<2000;i++){g.frameCounter++;drownedBusyBegin();}
 assert(groupUsed[8]==16 && used==32);
 for(int group=0;group<8;group++)if(group!=4){
  const int xs[]={0,1500,2200,2500,2900,3400,4000,4800};p.x=(LONG)xs[group]<<8;
  for(int i=0;i<2000;i++){g.frameCounter++;drownedBusyBegin();}
 }
 assert(used==DB_CAPACITY);
 for(int i=0;i<100;i++)drownedBusyBegin();assert(used==DB_CAPACITY);
 free(samples);return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'t.c').write_text(c)
 subprocess.run(['cc','-O2','-fsanitize=address,undefined',str(p/'t.c'),'-o',str(p/'t')],check=True)
 subprocess.run([str(p/'t')],check=True)
spec=importlib.util.spec_from_file_location('busy',R/'tools/analyze_drowned_busy.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
def stamp(field,line,valid=1):return dict(field=field,line=line,valid=valid)
assert m.elapsed(stamp(0xffffff,290),stamp(0,40))==62
assert m.elapsed(stamp(100,200),stamp(100,40)) is None
assert m.elapsed(stamp(100,2),stamp(100,40))==38
assert m.elapsed(stamp(100,2),stamp(101,40)) is None
assert m.elapsed(stamp(100,30,0),stamp(100,40)) is None
assert m.elapsed(stamp(100,30),stamp(100,30))==0
for text in ('','variant=busy_sparse_discovery\n'):
 try:m.analyze(text);raise AssertionError('accepted incomplete log')
 except ValueError:pass
print('PASS: actual sparse collector, 144-slot bounds, region/reset quotas, allocation failure, torn/edge stamps, disabled counters, raw wrap/rejection')

synthetic="variant=busy_sparse_discovery\nbusy_v1 stride=31 capacity=144 samples=1 allocated=1 bytes=28512\nbusy_sample=0 group=4 step=100 after=101 camera=2700 x=2850 y=100 seen=393225\nbusy_count=0 e0=0 e1=0 e2=2 shots=3 draw_words=1000 restore_words=900\nbusy_stamp=0 mark=0 field=100 line=30 valid=1\nbusy_stamp=0 mark=18 field=100 line=31 valid=1\nbusy_stamp=0 mark=3 field=100 line=110 valid=1\nbusy_stamp=0 mark=17 field=101 line=20 valid=1\npost_run=complete\n"
r=m.analyze(synthetic)
assert r['groups']['4']['scopes']['game']['median_lines']==79
assert r['groups']['4_enemies_2']['shots']==[3]
assert m.analyze(synthetic.replace('after=101','after=0'))['reset_samples']==1
print('PASS: full synthetic log parsing, enemy grouping, scope attribution and reset exclusion')
