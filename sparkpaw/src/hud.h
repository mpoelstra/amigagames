#ifndef SPARKPAW_HUD_H
#define SPARKPAW_HUD_H

#include <exec/types.h>
#include <graphics/gfx.h>

BOOL hudPrepare(void);
void hudRelease(void);
void hudSetState(UBYTE health,UBYTE lives,UBYTE diamonds,ULONG score);
const struct BitMap *hudDisplayBitmap(void);
const UBYTE *hudBlankPlane(void);
UWORD hudBytesPerRow(void);
#ifdef SPARKPAW_RENDER_DIAGNOSTIC
UBYTE hudDisplayIndex(void);
#endif

#endif
