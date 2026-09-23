#include "drowned_busy.h"
#include "platform_amiga.h"
#include "game.h"
#include "player.h"
#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/dos.h>
#define DB_GROUPS 9
#define DB_PER_GROUP 16
#define DB_CAPACITY (DB_GROUPS*DB_PER_GROUP)
struct BusyStamp { ULONG field; UWORD line, valid; };
struct BusySample {
 ULONG step, stepAfter, camera, seen;
 WORD x,y;
 UWORD group;
 ULONG count[DB_COUNTS];
 struct BusyStamp stamp[DB_MARKS];
};
static struct BusySample *samples,*current;
static UWORD used, groupUsed[DB_GROUPS];
static ULONG sequence,previousStep;
static UBYTE afterReset, countdown;
UBYTE drownedBusyActive;
void drownedBusyInit(void)
{
 samples=AllocMem(sizeof(*samples)*DB_CAPACITY,MEMF_FAST|MEMF_CLEAR);
}
void drownedBusyMark(UBYTE mark)
{
 ULONG before,after;
 UWORD a,b;
 struct BusyStamp *s;
 if(!drownedBusyActive||mark>=DB_MARKS)return;
 s=&current->stamp[mark];
 before=platformFieldCounter();
 a=platformRasterLine(); b=platformRasterLine();
 after=platformFieldCounter();
 s->field=before; s->line=b;
 /* Reject torn VPOS/VHPOS reads and TOD changes during the snapshot.
    Analysis guards frame-edge alignment for intervals crossing TOD ticks. */
 s->valid=(UWORD)(before==after && b>=a && b-a<=1 && b<312);
 current->seen|=1UL<<mark;
 if(mark==DB_GAME_END)current->stepAfter=gameState()->frameCounter;
 if(mark==DB_REAR_END)drownedBusyActive=0;
}
void drownedBusyBegin(void)
{
 const struct PlayerState *p;
 ULONG step;
 UBYTE group;
 WORD x;
 drownedBusyActive=0;
 if(!samples)return;
 step=gameState()->frameCounter;
 if(sequence && step<previousStep)afterReset=1;
 previousStep=step;
 sequence++;
 /* Prime interval avoids locking samples to 2/4/6/16-tick animation phases. */
 if(countdown){countdown--;return;}
 countdown=30;
 p=playerState(); x=(WORD)(p->x>>8);
 group=x<1376?0:x<2128?1:x<2400?2:x<2800?3:x<3200?4:
       x<3888?5:x<4656?6:7;
 if(group==4 && afterReset)group=8;
 if(groupUsed[group]>=DB_PER_GROUP)return;
 groupUsed[group]++;
 current=&samples[used++];
 current->step=step;current->stepAfter=step;
 current->camera=gameState()->cameraX;
 current->x=x;current->y=(WORD)(p->y>>8);current->group=group;
 drownedBusyActive=1;
 drownedBusyMark(DB_GAME_BEGIN);
 drownedBusyMark(DB_EMPTY);
}
void drownedBusyCount(UBYTE count,ULONG amount)
{
 if(drownedBusyActive && count<DB_COUNTS)current->count[count]+=amount;
}
void drownedBusyWrite(LONG file)
{
 UWORD i,j;
 drownedBusyActive=0;
 FPrintf(file,"busy_v1 stride=31 capacity=%ld samples=%ld allocated=%ld bytes=%ld\n",
  (ULONG)DB_CAPACITY,(ULONG)used,(ULONG)(samples!=0),
  samples?(ULONG)(sizeof(*samples)*DB_CAPACITY):0UL);
 FPrintf(file,"busy_clock=raw_tod_and_beam cross_field_edge_guard=8..300 resolution=scanline observer_cost=unmeasured no_extra_waits=1 pure_cpu=0\n");
 FPrintf(file,"busy_marks=game_begin,ai_begin,ai_end,game_end,copper_end,enemy_restore_begin,enemy_restore_end,restore_end,water_end,patch_build_end,compact_end,patch_sync_end,other_draw_end,enemy_draw_end,projectile_draw_end,bob_end,rear_begin,rear_end,empty_probe\n");
 FPrintf(file,"busy_counts=enemy0,enemy1,enemy2,projectiles,masked_draw_word_cells,restore_word_cells\n");
 for(i=0;i<used;i++) {
  struct BusySample *s=&samples[i];
  FPrintf(file,"busy_sample=%ld group=%ld step=%ld after=%ld camera=%ld x=%ld y=%ld seen=%ld\n",
    (ULONG)i,(ULONG)s->group,s->step,s->stepAfter,s->camera,(LONG)s->x,(LONG)s->y,s->seen);
  FPrintf(file,"busy_count=%ld e0=%ld e1=%ld e2=%ld shots=%ld draw_words=%ld restore_words=%ld\n",
    (ULONG)i,s->count[0],s->count[1],s->count[2],s->count[3],s->count[4],s->count[5]);
  for(j=0;j<DB_MARKS;j++)if(s->seen&(1UL<<j))
   FPrintf(file,"busy_stamp=%ld mark=%ld field=%ld line=%ld valid=%ld\n",
    (ULONG)i,(ULONG)j,s->stamp[j].field,(ULONG)s->stamp[j].line,(ULONG)s->stamp[j].valid);
 }
}
