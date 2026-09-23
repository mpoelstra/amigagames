#ifndef DROWNED_BUSY_H
#define DROWNED_BUSY_H
/* Sparse read-only checkpoints; never enable in ordinary gameplay builds. */
#ifdef SPARKPAW_DROWNED_BUSY
#if !defined(SPARKPAW_DROWNED_FPS) || !defined(SPARKPAW_DROWNED_FULL)
#error Busy_observer_requires_full_Drowned_FPS
#endif
#include <exec/types.h>
enum {
 DB_GAME_BEGIN, DB_AI_BEGIN, DB_AI_END, DB_GAME_END, DB_COPPER_END,
 DB_ENEMY_RESTORE_BEGIN, DB_ENEMY_RESTORE_END, DB_RESTORE_END,
 DB_WATER_END, DB_PATCH_BUILD_END, DB_COMPACT_END, DB_PATCH_SYNC_END,
 DB_OTHER_DRAW_END, DB_ENEMY_DRAW_END, DB_PROJECTILE_DRAW_END,
 DB_BOB_END, DB_REAR_BEGIN, DB_REAR_END, DB_EMPTY, DB_MARKS
};
enum { DB_ENEMY0, DB_ENEMY1, DB_ENEMY2, DB_PROJECTILES,
 DB_DRAW_WORDS, DB_RESTORE_WORDS, DB_COUNTS };
extern UBYTE drownedBusyActive;
void drownedBusyInit(void);
void drownedBusyBegin(void);
void drownedBusyMark(UBYTE mark);
void drownedBusyCount(UBYTE count, ULONG amount);
void drownedBusyWrite(LONG file);
#define BUSY_MARK(mark) do { if(drownedBusyActive) drownedBusyMark(mark); } while(0)
#define BUSY_COUNT(count,amount) do { if(drownedBusyActive) drownedBusyCount(count,amount); } while(0)
#define BUSY_BEGIN() drownedBusyBegin()
#else
#define BUSY_MARK(mark)
#define BUSY_COUNT(count,amount)
#define BUSY_BEGIN()
#endif
#endif
