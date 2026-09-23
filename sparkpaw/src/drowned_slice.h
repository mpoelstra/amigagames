#ifndef SPARKPAW_DROWNED_SLICE_H
#define SPARKPAW_DROWNED_SLICE_H
#include <exec/types.h>
#define DROWNED_HEADER_LEFT 800
#define DROWNED_HEADER_RIGHT 832
#if defined(SPARKPAW_DROWNED_GOVERNOR) && !defined(SPARKPAW_DROWNED_FULL)
#undef DROWNED_HEADER_LEFT
#undef DROWNED_HEADER_RIGHT
#define DROWNED_HEADER_LEFT 1312
#define DROWNED_HEADER_RIGHT 1344
#endif
#define DROWNED_HEADER_TOP 120
#define DROWNED_HEADER_BOTTOM 136
BOOL drownedHeaderOverlaps(WORD left,WORD top,WORD right,WORD bottom);
/* Focused candidate only; no production section selection in hot paths. */
void drownedReset(void);
#ifdef SPARKPAW_DROWNED_JOINED
void drownedRespawn(BOOL saved);
#endif
void drownedSetView(WORD cameraX);
void drownedTick(void);
UBYTE drownedJetFrame(void);
UBYTE drownedGateFrame(void);
BOOL drownedGateSolid(WORD x,WORD y);
BOOL drownedJetTouches(WORD left,WORD top,WORD right,WORD bottom);
UBYTE drownedPanelHit(WORD x,WORD y);
BOOL drownedPanelSweep(WORD start,WORD end,WORD y,WORD *hitX);
#ifdef SPARKPAW_DROWNED_ENCOUNTER
UBYTE drownedEncounterHit(WORD x,WORD y);
BOOL drownedEncounterSweep(WORD start,WORD end,WORD y,WORD *hitX);
#endif
#endif
