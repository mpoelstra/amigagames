"""Actual raw-key pause latch plus gameplay-loop ordering contract."""
from pathlib import Path
import re
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parents[1]
platform=(ROOT/'src/platform_amiga.c').read_text()
main_source=(ROOT/'src/main.c').read_text()

def function(source,signature,next_signature=None):
    start=source.index(signature)
    if next_signature:
        end=source.index(next_signature,start)
        return source[start:end]
    depth=0
    opened=False
    for at in range(start,len(source)):
        if source[at]=='{': depth+=1;opened=True
        elif source[at]=='}':
            depth-=1
            if opened and depth==0: return source[start:at+1]
    raise AssertionError(signature)

defines='\n'.join(re.findall(r'^#define GAMEKEY_.*$',platform,re.M))
code=function(platform,'static void handleGameRawKey')+'\n'
code+=function(platform,'BOOL platformGamePauseToggleRequested')+'\n'
code+=function(platform,'void platformResetGameInput')+'\n'
harness=r'''
#include <assert.h>
typedef unsigned char UBYTE;
typedef int BOOL;
#define TRUE 1
#define FALSE 0
static UBYTE gameKeys;
static BOOL pauseToggleRequested;
'''
main=r'''
int main(void)
{
    platformResetGameInput();
    handleGameRawKey(0x19); /* P down */
    assert((gameKeys&GAMEKEY_P)!=0);
    assert(platformGamePauseToggleRequested());
    assert(!platformGamePauseToggleRequested());
    handleGameRawKey(0x19); /* key repeat while still held */
    assert(!platformGamePauseToggleRequested());
    handleGameRawKey(0x99); /* P up */
    assert((gameKeys&GAMEKEY_P)==0);
    handleGameRawKey(0x19);
    assert(platformGamePauseToggleRequested());
    platformResetGameInput();
    assert(!platformGamePauseToggleRequested()&&gameKeys==0);
    return 0;
}
'''
with tempfile.TemporaryDirectory() as directory:
    path=Path(directory)
    (path/'test.c').write_text(harness+defines+'\n'+code+main)
    subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',
                    str(path/'test.c'),'-o',str(path/'test')],check=True)
    subprocess.run([str(path/'test')],check=True)

loop=main_source[main_source.index('while(state==APP_PLAYING)'):
                 main_source.index('if(state==APP_RETURN_READY)')]
poll=loop.index('platformReadGameKeys(')
toggle=loop.index('platformGamePauseToggleRequested()')
pause=loop.index('if(paused)')
update=loop.index('gameUpdate();')
assert poll<toggle<pause<update
paused_branch=loop[pause:update]
assert 'continue;' in paused_branch
assert 'platformGameEscapeRequested()' in loop[:pause]
assert loop.rstrip().endswith('paused=FALSE;')
start_audio=function(platform,'void platformStartGameplayAudio')
assert 'pauseToggleRequested=FALSE;' in start_audio
print('PASS: P edge latch, repeat suppression, paused simulation skip and live Escape/F10 polling')
