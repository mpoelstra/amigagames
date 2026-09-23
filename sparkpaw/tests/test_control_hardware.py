"""Actual C input/handshake regression checks; MMIO is modeled, not hardware proof.

The passive switch model deliberately retains a discharged POT input without
OUTRY+DATRY. This reproduces the reported latch mechanism, not a user's machine.
"""
from pathlib import Path
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
platform = (ROOT/'src/platform_amiga.c').read_text()
player = (ROOT/'src/player.c').read_text()


def function(source, signature):
    start = source.index(signature)
    opening = source.index('{', start)
    depth = 1
    for end in range(opening+1, len(source)):
        depth += (source[end] == '{') - (source[end] == '}')
        if not depth:
            return source[start:end+1]
    raise AssertionError(signature)


# Both ownership paths establish pull-ups AFTER OS interrupts are excluded.
for signature in ['void platformFinishTakeover', 'void platformResumeMenuAfterLoading']:
    body = function(platform, signature)
    assert body.index('Disable();') < body.index('hardware->potgo=PORT2_BUTTONS_PULLUP;')
# No per-poll reconfiguration or capacitor discharge; allow settling through the
# existing presentation-frame waits before gameplay/menu input is sampled.
assert platform.count('hardware->potgo=PORT2_BUTTONS_PULLUP;') == 2
assert 'potgo' not in function(platform, 'BOOL platformSecondaryButtonHeld')

shim = r'''
#include <assert.h>
#include <stdio.h>
#include "control_options.h"
typedef unsigned char UBYTE;
typedef unsigned short UWORD;
typedef int BOOL;
#define TRUE 1
#define FALSE 0
static struct { UWORD potinp; } registers, *hardware=&registers;
static UWORD joy;
static UBYTE primary=0x80, cra;
#define CIAA_CRA cra
#define CIACRAF_SPMODE 0x40
static UBYTE gameKeys;
static BOOL pauseToggleRequested,whdloadQuitRequested;
static BOOL controlJumpInputHeld,keyJumpInputHeld,joystickFireHeld,keyFireHeld;
static enum ControlMode controlMode;
static int tick,lineTicks,readTicks;
static UWORD platformRasterLine(void) {
    assert(CIAA_CRA&CIACRAF_SPMODE);
    tick+=readTicks;
    return (UWORD)((tick/lineTicks)%312);
}
static void platformReadGameKeys(BOOL *l,BOOL *r,BOOL *d,BOOL *j,BOOL *f);
'''
defines = '\n'.join(re.findall(r'^#define (?:PORT2_|GAMEKEY_).*$', platform, re.M))
code = '\n'.join(function(platform, s) for s in [
    'BOOL platformSecondaryButtonHeld', 'static void acknowledgeKeyboard',
    'static void handleGameRawKey'])
code += r'''
static void platformReadGameKeys(BOOL *l,BOOL *r,BOOL *d,BOOL *j,BOOL *f) {
    *l=!!(gameKeys&GAMEKEY_A); *r=!!(gameKeys&GAMEKEY_D);
    *d=!!(gameKeys&GAMEKEY_S); *j=!!(gameKeys&GAMEKEY_W);
    *f=!!(gameKeys&GAMEKEY_SPACE);
}
'''
code += '\n'+function(player, 'void playerReadInput(')
code += '\n'+function(player, 'void playerReadFlightInput(')
code = code.replace('*(volatile UWORD *)0xdff00c', 'joy')
code = code.replace('*(volatile UBYTE *)0xbfe001', 'primary')
checks = r'''
static void sample(int *j,int *f) {
    BOOL l,r,d; playerReadInput(&l,&r,&d,j,f);
}
static void reset(void) {
    gameKeys=0; joy=0; primary=0x80;
    hardware->potinp=0x4000; controlJumpInputHeld=keyJumpInputHeld=joystickFireHeld=keyFireHeld=0;
}
/* Passive switch + retained capacitor model, after >=300us settling.
   No external pull-up: input-only mode cannot guarantee release goes high. */
static void pin(int setup,int pressed) {
    if(pressed) hardware->potinp=0;
    else if((setup&0xc000)==0xc000) hardware->potinp=0x4000;
}
int main(void) {
    int action,j,f,i,phase,speed,standard,start;
    assert((PORT2_BUTTONS_PULLUP&0xc000)==0xc000);
    assert((PORT2_BUTTONS_PULLUP&0x3000)==0x3000);
    assert(!(PORT2_BUTTONS_PULLUP&0x0fff));
    for(action=0;action<2;action++) {
        controlMode=(enum ControlMode)action;
        reset();
        for(i=0;i<100;i++) {
            pin(PORT2_BUTTONS_PULLUP,1); sample(&j,&f);
            assert((action==CONTROL_JOYPAD)==!!j && !f);
            sample(&j,&f); assert(!j&&!f);
            pin(PORT2_BUTTONS_PULLUP,0); sample(&j,&f); assert(!j&&!f);
            joy=0x100; primary=0; sample(&j,&f);
            assert((action==CONTROL_JOYSTICK)==!!j && f);
            sample(&j,&f); assert(!j&&!f);
            joy=0; primary=0x80; sample(&j,&f);
            handleGameRawKey(0x11); handleGameRawKey(0x40);
            sample(&j,&f); assert(j&&f);
            handleGameRawKey(0x91); handleGameRawKey(0xc0);
            sample(&j,&f); assert(!j&&!f);
        }
        /* Keyboard edges survive a held controller input, including a
           falsely held second button in the inactive joystick mode. */
        reset(); pin(PORT2_BUTTONS_PULLUP,1); sample(&j,&f);
        handleGameRawKey(0x11); handleGameRawKey(0x40);
        sample(&j,&f); assert(j&&f);
        reset(); joy=0x100; primary=0; sample(&j,&f);
        handleGameRawKey(0x11); handleGameRawKey(0x40);
        sample(&j,&f); assert(j&&f);
        reset(); {BOOL l,r,u,d; joy=0x100; primary=0;
          playerReadFlightInput(&l,&r,&u,&d,&f);
          assert(u==(action==CONTROL_JOYSTICK) && f);
          pin(PORT2_BUTTONS_PULLUP,1);
          playerReadFlightInput(&l,&r,&u,&d,&f);
          assert(u&&f);
        }
    }
    /* 0.1us model ticks, PAL and NTSC, every starting phase, including wrap.
       Fast and slow polling: lower bound must hold independently of CPU. */
    for(standard=0;standard<2;standard++) {
        lineTicks=standard?635:640;
        for(speed=1;speed<=100;speed+=9) {
            readTicks=speed;
            for(phase=0;phase<lineTicks;phase++) {
                tick=311*lineTicks+phase; start=tick; cra=0x05;
                acknowledgeKeyboard();
                assert(tick-start>=850); assert(cra==0x05);
            }
        }
    }
    handleGameRawKey(0x59); assert(whdloadQuitRequested);
    handleGameRawKey(0x19); assert(pauseToggleRequested);
    puts("PASS: control modes, independent keyboard edges, pull-up, flight, PAL/NTSC ACK phases, F10/P");
    return 0;
}
'''
with tempfile.TemporaryDirectory() as td:
    path=Path(td)
    (path/'test.c').write_text(shim+defines+'\n'+code+checks)
    subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',
                    '-Wno-unused-function','-DSPARKPAW_WHDLOAD',
                    '-fsanitize=address,undefined','-I'+str(ROOT/'src'),
                    str(path/'test.c'),'-o',str(path/'test')],check=True)
    subprocess.run([str(path/'test')],check=True)
