/* Bounded diagnostic only. Observe existing sequence without changing clocks. */
#include <exec/types.h>
#include <hardware/custom.h>
#include <proto/dos.h>
#include <string.h>
#include "platform_amiga.h"
#include "game.h"
#include "player.h"
#include "trace.h"
#define IMPORTANT 96
#define SHORT_SAMPLES 16
typedef struct {
 ULONG before,after,sequence,delta,retries,field[4];
 UWORD line[4];LONG cameraBefore,cameraAfter;
 UWORD waterBefore,waterAfter,healthBefore,healthAfter;
} Record;
static Record current,important[IMPORTANT],shortSamples[SHORT_SAMPLES];
static ULONG sequence,importantCount,importantDropped,shortCount,shortDropped;
static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static UWORD coherentLine(void)
{
 UWORD hi,lo,check;
 do {hi=hw->vposr;lo=hw->vhposr;check=hw->vposr;}while((hi&7)!=(check&7));
 return (UWORD)(((hi&7)<<8)|(lo>>8));
}
void tracePhase(UWORD phase)
{
 /* Separate raw clocks, not a falsely precise combined timestamp.
    No reprogramming CIA, and no disabling IRQs to manufacture consistency. */
 current.field[phase]=platformFieldCounter();current.line[phase]=coherentLine();
}
void traceBegin(void)
{
 const struct GameState *g=gameState();
 current.sequence=++sequence;current.before=g->frameCounter;
 current.cameraBefore=g->cameraX;current.waterBefore=g->waterSplashTimer;
 current.healthBefore=playerState()->health;tracePhase(0);
}
void traceEnd(ULONG delta,ULONG retries)
{
 const struct GameState *g=gameState();
 current.delta=delta;current.retries=retries;current.after=g->frameCounter;
 current.cameraAfter=g->cameraX;current.waterAfter=g->waterSplashTimer;
 current.healthAfter=playerState()->health;
 if(delta>=3||current.after<current.before||current.waterAfter!=current.waterBefore||
    current.healthAfter!=current.healthBefore) {
  /* Save water onset/end, not every countdown field. */
  if(delta>=3||current.after<current.before||!current.waterBefore||!current.waterAfter||
     current.healthBefore!=current.healthAfter) {
   if(importantCount<IMPORTANT)important[importantCount++]=current;else importantDropped++;
  }
 } else if(delta!=1) {
  if(shortCount<SHORT_SAMPLES)shortSamples[shortCount++]=current;else shortDropped++;
 }
}
static void writeRecords(BPTR f,const char *kind,Record *rows,ULONG count)
{
 ULONG i;UWORD p;
 for(i=0;i<count;i++) {
  Record *r=&rows[i];
  FPrintf(f,"trace=%s seq=%ld delta=%ld attempts=%ld frame_before=%ld frame_after=%ld camera_before=%ld camera_after=%ld water_before=%ld water_after=%ld health_before=%ld health_after=%ld\n",(STRPTR)kind,r->sequence,r->delta,r->retries,r->before,r->after,r->cameraBefore,r->cameraAfter,(LONG)r->waterBefore,(LONG)r->waterAfter,(LONG)r->healthBefore,(LONG)r->healthAfter);
  for(p=0;p<4;p++)FPrintf(f,"point=%ld tod=%ld raster=%ld\n",(LONG)p,r->field[p],(LONG)r->line[p]);
 }
}
void traceWrite(BPTR f)
{
 FPrintf(f,"trace_capacity important=96 short=16 important_saved=%ld important_dropped=%ld short_saved=%ld short_dropped=%ld bytes=%ld\n",importantCount,importantDropped,shortCount,shortDropped,(LONG)(sizeof(current)+sizeof(important)+sizeof(shortSamples)));
 FPuts(f,"trace_points=0_before_update,1_after_update,2_after_render_and_bobs,3_after_publish; TOD/raster separate; IRQ preemption possible; diagnostic observer cost; not exact visible timestamps\n");
 writeRecords(f,"event_or_long",important,importantCount);writeRecords(f,"short_delta",shortSamples,shortCount);
}
