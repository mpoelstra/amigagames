"""Generate only the input boundary of player.c; physics/animation untouched."""
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1]
def player_source():
 source=(ROOT/'src/player.c').read_text()
 start=source.index('void playerReadInput(')
 end=source.index('\n#ifdef SPARKPAW_STORMRAIL_PROOF',start)
 replacement='''extern unsigned long fixedStep;
void playerReadInput(BOOL *left,BOOL *right,BOOL *down,BOOL *jump,BOOL *fire)
{
    unsigned char bits=scriptInput(fixedStep);
    BOOL up=(bits&SCRIPT_JUMP)!=0,held=(bits&SCRIPT_FIRE)!=0;
    *left=(bits&SCRIPT_LEFT)!=0;*right=(bits&SCRIPT_RIGHT)!=0;*down=FALSE;
    *jump=up&&!jumpInputHeld;jumpInputHeld=up;
    *fire=held&&!joystickFireHeld;joystickFireHeld=held;
}
'''
 return '#include "script.h"\n'+source[:start]+replacement+source[end:]
if __name__=='__main__':
 dest=ROOT/'build/audio-gameplay-fixed';dest.mkdir(parents=True,exist_ok=True)
 (dest/'player_fixed.c').write_text(player_source())
