"""Actual renderer visibility predicate: unfinished Drowned has no Level-1 Core."""
from pathlib import Path
import subprocess,tempfile
R=Path(__file__).resolve().parents[1]
s=(R/'src/renderer.c').read_text();a=s.index('static BOOL coreRenderVisible(void)');b=s.index('\nstatic void restoreCoreBob',a)
fn=s[a:b]
a=s.index('static BOOL extraLifeRenderVisible(void)');b=s.index('\nstatic void ',a);fn+=s[a:b]
with tempfile.TemporaryDirectory() as d:
 p=Path(d);src=p/'test.c'
 src.write_text('''#include <assert.h>
typedef int BOOL;typedef short WORD;
#define FALSE 0
#define CORE_SPRITE_W 64
#define LEVEL_STORMSTONE_CORE_CENTER_X 3232
#define SCREEN_W 320
#define EXTRA_LIFE_DROPPING 1
#define EXTRA_LIFE_READY 2
static struct {int cameraX,extraLifeState;} state,*game=&state;
'''+'\n#ifdef SPARKPAW_DROWNED_GOVERNOR\nstatic BOOL complete;static BOOL governorComplete(void){return complete;}\n#endif\n'+fn+'''
int main(void) {
 for(int x=0;x<=3520;x++) {game->cameraX=x;
#ifdef SPARKPAW_DROWNED_GOVERNOR
 complete=0;assert(!coreRenderVisible());complete=1;assert(coreRenderVisible()==(3200+64>=x-16&&3200<=x+336));
#elif defined(SPARKPAW_DROWNED_SLICE)
 assert(!coreRenderVisible());
#else
 assert(coreRenderVisible()==(3200+64>=x-16&&3200<=x+336));
#endif
 }
 for(int state=0;state<4;state++) {game->extraLifeState=state;
#ifdef SPARKPAW_DROWNED_SLICE
 assert(!extraLifeRenderVisible());
#else
 assert(extraLifeRenderVisible()==(state==1||state==2));
#endif
 }
 return 0;
}
''')
 for flags in ([],['-DSPARKPAW_DROWNED_SLICE'],['-DSPARKPAW_DROWNED_SLICE','-DSPARKPAW_DROWNED_GOVERNOR']):
  subprocess.run(['cc', '-DDROWNED_FINALE_OFFSET=0','-std=c99','-Wall','-Werror',*flags,str(src),'-o',str(p/'test')],check=True)
  subprocess.run([str(p/'test')],check=True)
print('PASS: actual visibility function over3521camera positions; Drowned Core/1up hidden, Level1 unchanged')
