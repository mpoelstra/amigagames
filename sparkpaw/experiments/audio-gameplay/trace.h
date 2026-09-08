#ifndef AUDIO_GAME_TRACE_H
#define AUDIO_GAME_TRACE_H
#include <exec/types.h>
#include <dos/dos.h>
void traceBegin(void);
void tracePhase(UWORD phase);
void traceEnd(ULONG delta,ULONG retries);
void traceWrite(BPTR file);
#endif
