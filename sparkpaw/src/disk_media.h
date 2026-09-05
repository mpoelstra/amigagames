#ifndef SPARKPAW_DISK_MEDIA_H
#define SPARKPAW_DISK_MEDIA_H
#ifdef SPARKPAW_MULTI_ADF
#include <exec/types.h>
#include <dos/dos.h>
BOOL diskMediaRequire(UBYTE disk);
BPTR diskMediaOpen(const char *name,LONG mode);
#endif
#endif
