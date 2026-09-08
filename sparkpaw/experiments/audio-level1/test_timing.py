"""Exercise the actual timing C accounting with a deterministic E-clock/Exec shim."""
from pathlib import Path
import re, subprocess, tempfile
here=Path(__file__).resolve().parent
source=re.sub(r'^#include[^\n]*\n','',(here/'timing.c').read_text(),flags=re.M)
shim=r'''
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
typedef int32_t LONG; typedef uint32_t ULONG; typedef uint16_t UWORD; typedef int BOOL;
typedef void *BPTR; typedef char *STRPTR;
#define TRUE 1
#define FALSE 0
#define TIMERNAME "timer.device"
#define UNIT_ECLOCK 2
struct Device {int unused;}; struct MsgPort {int unused;};
struct IORequest {struct Device *io_Device;};
struct timerequest {struct IORequest tr_node;};
struct EClockVal {ULONG ev_hi,ev_lo;};
static ULONG now;static int lock;
static struct Device device;
void proofTimingClose(void);
static void Disable(void){lock++;}
static void Enable(void){assert(lock>0);lock--;}
static ULONG ReadEClock(struct EClockVal *t){t->ev_hi=0;t->ev_lo=now;return 709379;}
static struct MsgPort *CreateMsgPort(void){return calloc(1,sizeof(struct MsgPort));}
static void DeleteMsgPort(struct MsgPort *p){free(p);}
static struct IORequest *CreateIORequest(struct MsgPort *p,unsigned n){(void)p;return calloc(1,n);}
static void DeleteIORequest(struct IORequest *p){free(p);}
static int OpenDevice(const char *n,int u,struct IORequest *p,int f){(void)n;(void)u;(void)f;p->io_Device=&device;return 0;}
static void CloseDevice(struct IORequest *p){(void)p;}
static void FPrintf(BPTR f,const char *s,...){(void)f;(void)s;}
static void FPuts(BPTR f,const char *s){(void)f;(void)s;}
'''
checks=r'''
int main(void){
 assert(proofTimingOpen()); now=0xfffffff0U;proofTimingStart();
 proofTimingEnter(0);now+=5;proofTimerABegin();now+=11;proofTimerEnd();
 now+=9;proofTimingLeave();
 assert(metrics[0].inclusive==25 && metrics[0].exclusive==14);
 assert(metrics[1].inclusive==11 && metrics[1].exclusive==11);
 assert(metrics[0].inclusive==metrics[0].exclusive+metrics[1].exclusive);
 now+=10;proofTimerBBegin();now+=4;proofTimerEnd();
 now+=100;proofTimingEnter(0);now+=2;proofTimingLeave();
 assert(metrics[0].calls==2 && metrics[0].gapMax==139);
 assert(metrics[0].max==25 && metrics[2].calls==1);
 proofTimingStop();assert(endTick-beginTick==141);
 assert(!depth&&!overflow&&!underflow&&!lock);
 proofTimerABegin();proofTimerEnd();assert(metrics[1].calls==1);
 proofTimingWrite(0);proofTimingClose();assert(!TimerBase&&!request&&!port);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'test.c').write_text(shim+source+checks)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsanitize=address,undefined',str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
print('Actual timing C: nested accounting, low32 rollover, gaps, lifecycle and lock balance PASS')
