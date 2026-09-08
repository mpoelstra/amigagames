#ifndef AUDIO_PROOF_TIMING_H
#define AUDIO_PROOF_TIMING_H
#include <exec/types.h>
#include <dos/dos.h>
BOOL proofTimingOpen(void);
void proofTimingClose(void);
void proofTimingStart(void);
void proofTimingStop(void);
void proofTimingWrite(BPTR file);
void proofTimingEnter(UWORD source);
void proofTimingLeave(void);
void proofTimerABegin(void);
void proofTimerBBegin(void);
void proofTimerEnd(void);
#endif
