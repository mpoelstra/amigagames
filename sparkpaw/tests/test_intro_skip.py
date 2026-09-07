"""Actual intro input C plus display-lifetime ordering guards."""
from pathlib import Path
import re,subprocess,tempfile
root=Path(__file__).resolve().parents[1];s=(root/'src/title.c').read_text()
def function(name):
 a=s.index('static BOOL '+name);b=s.index('\n}',a)+2;return s[a:b]
code=function('introImmediateSkip').replace('(*(volatile UBYTE *)0xbfe001)','mouseValue()')+'\n'+function('waitIntroPassage')
defines='\n'.join(re.findall(r'^#define INTRO_(?:PASSAGE_HOLD_FRAMES|TEXT_SCROLL_ROWS|SKIP_HOLD_FRAMES) .*$',s,re.M))
harness=r'''
#include <assert.h>
typedef int BOOL;typedef unsigned short UWORD;
#define TRUE 1
#define FALSE 0
static BOOL introSkipRequested;
static int tick,press,fire;
static unsigned char mouseValue(void){return tick==press?0xbf:0xff;}
static BOOL introFireHeld(void){return fire;}
static void WaitTOF(void){assert(tick<1000);tick++;}
static void stageIntroText(UWORD p,UWORD y){(void)p;(void)y;}
'''
main=r'''
int main(void){int i;
 for(i=0;i<INTRO_PASSAGE_HOLD_FRAMES;i++){
  tick=0;press=i;fire=0;introSkipRequested=0;
  assert(waitIntroPassage(0));assert(tick==i);
  press=-1;assert(introImmediateSkip());
 }
 tick=0;press=5;fire=1;introSkipRequested=0;assert(waitIntroPassage(0));assert(tick==5);
 tick=0;press=-1;fire=0;introSkipRequested=0;assert(!waitIntroPassage(0));
 return 0;
}
'''
with tempfile.TemporaryDirectory() as td:
 p=Path(td);(p/'test.c').write_text(harness+defines+'\n'+code+main)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',str(p/'test.c'),'-o',str(p/'test')],check=True);subprocess.run([str(p/'test')],check=True)
a=s.index('        fadeTo(assetsStoryIntro(),FALSE);');b=s.index('    musicStop(); /* Release intro bank',a);retire=s[a:b]
assert retire.index('hardware->dmacon=DMAF_RASTER|DMAF_COPPER|DMAF_SPRITE;') < retire.index('WaitTOF();') < retire.index('assetsUnloadStoryIntro();')
assert 'DMAF_ALL' not in retire and 'introInputActive=FALSE' in retire
assert 'if(introInputActive) (void)introImmediateSkip();' in s
assert 'introInputActive=playStory; introSkipRequested=FALSE;' in s
print('PASS: actual intro skip at 240 passage timings, held fire, latch and display retirement guards')
