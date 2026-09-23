#include "drowned_checkpoint.h"
static BOOL active;
static UBYTE activationTick;
void drownedCheckpointNewAttempt(void) { active=FALSE; activationTick=0; }
BOOL drownedCheckpointTouch(WORD left,WORD right,WORD foot,BOOL grounded)
{
    if(active||!grounded||foot<198||foot>200||
       right<DROWNED_CHECKPOINT_X||left>=DROWNED_CHECKPOINT_X+DROWNED_CHECKPOINT_W)
        return FALSE;
    active=TRUE; activationTick=1;
    return TRUE; /* One event: animation + sound, no score/health/life reward. */
}
void drownedCheckpointTick(void)
{ if(activationTick&&activationTick<32) activationTick++; }
void drownedCheckpointAfterRespawn(void)
{ if(active) activationTick=32; }
BOOL drownedCheckpointActive(void) { return active; }
UBYTE drownedCheckpointActivationTick(void) { return activationTick; }
