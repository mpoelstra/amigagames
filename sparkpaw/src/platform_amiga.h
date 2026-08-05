#ifndef SPARKPAW_PLATFORM_AMIGA_H
#define SPARKPAW_PLATFORM_AMIGA_H

#include <exec/types.h>

BOOL platformOpen(void);
void platformClose(void);
void platformTakeover(UWORD *copper);
void platformRestore(void);
UWORD platformRasterLine(void);
void platformWaitBlit(void);

#endif