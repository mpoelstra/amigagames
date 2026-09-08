"""Host smoke of actual adapter lifecycle with fake Exec/Paula; not IRQ emulation."""
from pathlib import Path
import re,tempfile,subprocess
here=Path(__file__).resolve().parent;root=here.parents[1]
src=re.sub(r'^#include[^\n]*\n','',(here/'audio_gameplay.c').read_text(),flags=re.M)
src=src.replace('(volatile struct Custom *)0xdff000','&fakeCustom').replace('(volatile UBYTE *)0xbfe001','&filter')
shim=r'''
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#define __reg(x)
typedef uint8_t UBYTE;typedef uint16_t UWORD;typedef uint32_t ULONG;
typedef int32_t LONG;typedef int BOOL;typedef long BPTR;typedef char *STRPTR;
#define TRUE 1
#define FALSE 0
#define MEMF_FAST 1
#define MEMF_CHIP 2
#define MEMF_CLEAR 4
#define MODE_OLDFILE 1
#define OFFSET_END 1
#define OFFSET_BEGINNING -1
#define NT_INTERRUPT 2
#define INTF_AUD3 1024
#define INTF_SETCLR 32768
#define INTB_AUD3 10
struct Library {int dummy;};
struct Interrupt {struct {int ln_Type;char *ln_Name;}is_Node;void (*is_Code)(void);};
struct Custom {UWORD intena,intreq,dmacon;struct {UWORD *ac_ptr;UWORD ac_len,ac_per,ac_vol;}aud[4];};
static struct Custom fakeCustom;static UBYTE filter=2;
static int allocations,opens,failOpen,installOK=1,locks,resourcesOwned,quiesced,vector;
static ULONG filePos;static struct Library resource;
static void *OpenResource(const char *s){(void)s;return &resource;}
static BPTR Open(STRPTR n,int mode){(void)n;(void)mode;return ++opens==failOpen?0:1;}
static LONG Seek(BPTR f,int n,int mode){ULONG old=filePos;(void)f;(void)n;filePos=mode==OFFSET_END?128:0;return old;}
static LONG Read(BPTR f,void *p,LONG n){(void)f;memset(p,32,n);return n;}
static void Close(BPTR f){(void)f;}
static void *AllocMem(ULONG n,int flags){void *p=malloc(n);if(p){allocations++;if(flags&MEMF_CLEAR)memset(p,0,n);}return p;}
static void FreeMem(void *p,ULONG n){(void)n;allocations--;free(p);}
static void Disable(void){locks++;}
static void Enable(void){assert(locks>0);locks--;}
static struct Interrupt *SetIntVector(int n,struct Interrupt *p){(void)n;vector=p!=NULL;return NULL;}
static void AbleICR(struct Library *r,int mask){(void)r;assert(resourcesOwned&&mask==3);quiesced=1;}
static void SetICR(struct Library *r,int mask){(void)r;assert(resourcesOwned&&mask==3);}
static void FPrintf(BPTR f,const char *fmt,...){(void)f;(void)fmt;}
void audioUnload(void);
void proof_audio_irq(void){}
UBYTE mt_Enable;
int mt_install(void){if(installOK)resourcesOwned=1;return installOK;}
void mt_remove(void){assert(resourcesOwned&&quiesced);resourcesOwned=0;}
void mt_init(void *h,void *s,void *b,UBYTE x){(void)h;(void)s;(void)b;(void)x;assert(resourcesOwned&&locks);}
void mt_end(void *h){(void)h;assert(quiesced&&locks);}
void mt_channelmask(void *h,UBYTE x){(void)h;assert(x==7);}
void mt_mastervol(void *h,UWORD x){(void)h;assert(x==48);}
'''
checks=r'''
int main(void){
 unsigned i;void (*fn[16])(void)={audioPlayShot,audioPlayPlayerHurt,audioPlayEnemyHit,audioPlayEnemyDeath,audioPlayStriderShot,audioPlayJump,audioPlayCollect,audioPlayWaterSplash,audioPlayStormstoneCore,audioPlayTallyTick,audioPlayExtraLife,audioPlayHarrierFanCharge,audioPlayHarrierFanFire,audioPlayHarrierHunterCharge,audioPlayHarrierHunterFire,audioPlayHealthCollect};
 failOpen=5;assert(!audioLoad());assert(!allocations&&!resourcesOwned&&!vector&&!locks);
 failOpen=0;opens=0;installOK=0;assert(!audioLoad());assert(!allocations&&!resourcesOwned&&!vector&&!locks);
 installOK=1;assert(audioLoad());assert(vector&&resourcesOwned&&!locks);
 audioPlayShot();assert(!mixer.requests[0]);
 Disable();audioSetHardwareActive(TRUE);Enable();assert(running&&mt_Enable);
 for(i=0;i<16;i++)fn[i]();
 for(i=0;i<16;i++)assert(mixer.requests[i]==1);
 audioUpdate();proofAudioIRQ();assert(irqCount==1&&mixer.rendered==112);
 Disable();audioSetHardwareActive(FALSE);Enable();assert(!running&&!mt_Enable&&quiesced);
 proofAudioIRQ();audioPlayShot();assert(irqCount==1&&mixer.requests[0]==1);
 audioGameWrite(1);audioUnload();assert(!allocations&&!resourcesOwned&&!vector&&!locks&&filter==2);
 audioUnload();assert(!allocations&&!locks);
 puts("adapter: load/install failures, all 16 event mappings, IRQ buffer service, quiesce, balanced cleanup PASS");return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'test.c').write_text(shim+'\n#include "mix.h"\n#include "catalog.h"\n'+src+checks)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsanitize=address,undefined','-I'+str(root/'experiments/audio-level1'),'-I'+str(root/'build/audio-gameplay'),str(p/'test.c'),str(root/'experiments/audio-level1/mix.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
