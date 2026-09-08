/* OS-live proof only. ReadEClock is interrupt-safe (V36 autodoc).
   Never program a CIA register: ptplayer owns both CIA-B timers. */
#include <exec/types.h>
#include <devices/timer.h>
#include <proto/exec.h>
#include <proto/timer.h>
#include <proto/dos.h>
#include "timing.h"
struct Device *TimerBase;
static struct MsgPort *port;
static struct timerequest *request;
static ULONG frequency,beginTick,endTick,calMin=0xffffffffUL,calMax,calTotal;
static volatile BOOL active;
/* Level 6 music can interrupt level 4 mixer. Same-level handlers do not nest.
   Disable/Enable makes only stack publication and accounting atomic; it does
   not mask interrupts across the measured audio service. */
typedef struct { ULONG start,children; UWORD source; } Scope;
typedef struct { ULONG calls,inclusive,exclusive,max,gapMax,previous; } Metric;
static Scope scopes[4];
static Metric metrics[3];
static UWORD depth;
static ULONG overflow,underflow;
static ULONG tick(void) { struct EClockVal t; ReadEClock(&t); return t.ev_lo; }
BOOL proofTimingOpen(void)
{
    UWORD i;ULONG a,d;struct EClockVal t;
    port=CreateMsgPort();if(!port)return FALSE;
    request=(struct timerequest *)CreateIORequest(port,sizeof(*request));
    if(!request){proofTimingClose();return FALSE;}
    if(OpenDevice(TIMERNAME,UNIT_ECLOCK,(struct IORequest *)request,0)) {
        DeleteIORequest((struct IORequest *)request);request=0;
        proofTimingClose();return FALSE;
    }
    TimerBase=request->tr_node.io_Device;frequency=ReadEClock(&t);
    for(i=0;i<256;i++) {
        a=tick();d=tick()-a;
        if(d<calMin)calMin=d;if(d>calMax)calMax=d;calTotal+=d;
    }
    if(!frequency){proofTimingClose();return FALSE;}
    return TRUE;
}
void proofTimingClose(void)
{
    if(request){CloseDevice((struct IORequest *)request);DeleteIORequest((struct IORequest *)request);request=0;}
    if(port){DeleteMsgPort(port);port=0;}TimerBase=0;
}
void proofTimingStart(void){beginTick=tick();active=TRUE;}
void proofTimingStop(void){active=FALSE;endTick=tick();}
void proofTimingEnter(UWORD source)
{
    ULONG now;Metric *m;
    if(!active)return;
    Disable();now=tick();
    if(depth>=4){overflow++;Enable();return;}
    scopes[depth].start=now;scopes[depth].children=0;
    scopes[depth++].source=source;
    m=&metrics[source];
    if(m->calls&&now-m->previous>m->gapMax)m->gapMax=now-m->previous;
    m->previous=now;Enable();
}
void proofTimingLeave(void)
{
    ULONG duration;Scope *s;Metric *m;
    if(!active)return;
    Disable();
    if(!depth){underflow++;Enable();return;}
    s=&scopes[--depth];duration=tick()-s->start;m=&metrics[s->source];
    m->calls++;m->inclusive+=duration;m->exclusive+=duration-s->children;
    if(duration>m->max)m->max=duration;
    if(depth)scopes[depth-1].children+=duration;
    Enable();
}
void proofTimerABegin(void){proofTimingEnter(1);}
void proofTimerBBegin(void){proofTimingEnter(2);}
void proofTimerEnd(void){proofTimingLeave();}
void proofTimingWrite(BPTR file)
{
    UWORD i;static const char *names[]={"sfx_buffer","music_timer_a","music_dma_timer_b"};
    FPrintf(file,"timing=ReadEClock_v1 eclock_hz=%ld elapsed_ticks=%ld low32_wrap_safe=1\n",frequency,endTick-beginTick);
    FPrintf(file,"clock_pair_calibration calls=256 min_ticks=%ld max_ticks=%ld total_ticks=%ld\n",calMin,calMax,calTotal);
    for(i=0;i<3;i++)FPrintf(file,"scope=%s calls=%ld inclusive_ticks=%ld exclusive_ticks=%ld max_inclusive_ticks=%ld max_start_gap_ticks=%ld\n",(STRPTR)names[i],metrics[i].calls,metrics[i].inclusive,metrics[i].exclusive,metrics[i].max,metrics[i].gapMax);
    FPrintf(file,"timing_stack depth=%ld overflow=%ld underflow=%ld\n",(LONG)depth,overflow,underflow);
    FPuts(file,"timing_limits=instrumented service bodies; includes clock/accounting overhead and OS preemption; nested measured music subtracted from mixer exclusive; excludes Exec dispatch and outer register saves; gaps are not dropout counts; calibration is not subtracted\n");
}
