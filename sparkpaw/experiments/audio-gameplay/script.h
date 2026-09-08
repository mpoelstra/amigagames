#ifndef AUDIO_FIXED_SCRIPT_H
#define AUDIO_FIXED_SCRIPT_H
#define SCRIPT_STEPS 1000UL
#define SCRIPT_RIGHT 1
#define SCRIPT_LEFT 2
#define SCRIPT_JUMP 4
#define SCRIPT_FIRE 8
static unsigned char scriptInput(unsigned long step)
{
 unsigned char bits=0;
 if(step<520)bits|=SCRIPT_RIGHT;
 /* Fixed combat patrol; no dependence on wall clock or CPU speed. */
 else if(step<610)bits|=SCRIPT_LEFT;
 else if(step<700)bits|=SCRIPT_RIGHT;
 else if(step<790)bits|=SCRIPT_LEFT;
 else if(step<880)bits|=SCRIPT_RIGHT;
 else if(step<970)bits|=SCRIPT_LEFT;
 if(step%48==0)bits|=SCRIPT_JUMP;
 if(step%8==0)bits|=SCRIPT_FIRE;
 return bits;
}
#endif
