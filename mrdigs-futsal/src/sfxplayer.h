#ifndef FUTSAL_SFXPLAYER_H
#define FUTSAL_SFXPLAYER_H
#include <exec/types.h>
BOOL sfxLoad(const char *filename);
void sfxPlay(const char *name, WORD pan);
void sfxTick(void);
void sfxFree(void);
#endif
