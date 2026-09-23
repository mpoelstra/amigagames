#ifndef DROWNED_CHECKPOINT_H
#define DROWNED_CHECKPOINT_H
#include <exec/types.h>
/* Joined-level checkpoint, retained across life loss only. */
#define DROWNED_CHECKPOINT_X 2320
#define DROWNED_CHECKPOINT_Y 152
#define DROWNED_CHECKPOINT_W 32
#define DROWNED_CHECKPOINT_RESPAWN_X 2320
#define DROWNED_CHECKPOINT_RESPAWN_Y 161
void drownedCheckpointNewAttempt(void);
BOOL drownedCheckpointTouch(WORD left,WORD right,WORD foot,BOOL grounded);
void drownedCheckpointTick(void);
void drownedCheckpointAfterRespawn(void);
BOOL drownedCheckpointActive(void);
UBYTE drownedCheckpointActivationTick(void);
#endif
