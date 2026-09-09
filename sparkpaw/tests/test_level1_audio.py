"""Actual integrated backend lifecycle with fake Exec/Paula, ASan/UBSan.
No claim about physical interrupt latency or audible transition quality.
"""
from pathlib import Path
import re,subprocess,tempfile,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from prepare_game_audio import player_source
s=re.sub(r'^#include[^\n]*\n','',(ROOT/'src/level1_audio.c').read_text(),flags=re.M)
s=s.replace('(volatile struct Custom *)0xdff000','&fakeCustom').replace('(volatile UBYTE *)0xbfe001','&filter')
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
struct Custom {UWORD intena,intenar,intreq,dmacon;struct {UWORD *ac_ptr;UWORD ac_len,ac_per,ac_vol;}aud[4];};
static struct Custom fakeCustom;static UBYTE filter=2;
static int allocations,opens,failOpen,installOK=1,locks,resourcesOwned,quiesced,vector,titlePlaying=1;
static int initCount,pauseCount,allocCalls,failAlloc;
static void *lastScore,*lastBank;
static ULONG filePos,fileLength;static struct Library resource;
static void *OpenResource(const char *s){(void)s;return &resource;}
static int musicInitialize(int hz){assert(hz==50);return 1;}
static int musicIsPlaying(void){return titlePlaying;}
static BPTR Open(STRPTR n,int mode){(void)mode;fileLength=strstr(n,"score")?(strstr(n,"rail-score")?17468:9276):strstr(n,"bank")?11552:128;return ++opens==failOpen?0:1;}
static LONG Seek(BPTR f,int n,int mode){ULONG old=filePos;(void)f;(void)n;filePos=mode==OFFSET_END?fileLength:0;return old;}
static LONG Read(BPTR f,void *p,LONG n){(void)f;memset(p,32,n);if(n==9276||n==17468)memcpy((char *)p+1080,"M.K.",4);return n;}
static void Close(BPTR f){(void)f;}
static void *AllocMem(ULONG n,int flags){void *p;if(++allocCalls==failAlloc)return NULL;p=malloc(n);if(p){allocations++;if(flags&MEMF_CLEAR)memset(p,0,n);}return p;}
static void FreeMem(void *p,ULONG n){(void)n;allocations--;free(p);}
static void Disable(void){locks++;}
static void Enable(void){assert(locks>0);locks--;}
static struct Interrupt *SetIntVector(int n,struct Interrupt *p){(void)n;assert(locks);vector=p!=NULL;return NULL;}
static void AbleICR(struct Library *r,int mask){(void)r;assert(resourcesOwned&&locks);assert(mask==3||mask==0x83);quiesced=mask==3;}
static void SetICR(struct Library *r,int mask){(void)r;assert(resourcesOwned&&locks&&mask==3);}
void level1AudioUnload(void);
void level1_audio_irq(void){}
UBYTE mt_Enable;
int mt_install(void){assert(locks);if(installOK)resourcesOwned=1;return installOK;}
void mt_remove(void){assert(resourcesOwned&&quiesced&&locks&&!vector);resourcesOwned=0;}
void mt_pause_timer_b(void){assert(locks);pauseCount++;}
void mt_init(void *h,void *s,void *b,UBYTE x){(void)h;(void)x;lastScore=s;lastBank=b;assert(resourcesOwned&&locks&&!titlePlaying);initCount++;}
void mt_end(void *h){(void)h;assert(quiesced&&locks);}
void mt_channelmask(void *h,UBYTE x){(void)h;assert(x==7);}
void mt_mastervol(void *h,UWORD x){(void)h;assert(x==48);}
'''
checks=r'''
int main(void){
 unsigned i;int cycle;
 for(i=1;i<=19;i++){
  allocCalls=0;failAlloc=i;assert(!level1AudioLoad(FALSE));
  assert(!allocations&&!resourcesOwned&&!vector&&!locks);
 }
 failAlloc=0;
 failOpen=opens+5;assert(!level1AudioLoad(FALSE));assert(!allocations&&!resourcesOwned&&!vector&&!locks);
 failOpen=0;installOK=0;assert(!level1AudioLoad(FALSE));assert(!allocations&&!resourcesOwned&&!vector&&!locks);
 installOK=1;
 for(cycle=0;cycle<8;cycle++){
  titlePlaying=1;assert(level1AudioLoad(cycle%2));
  assert(scoreSize==(cycle%2?17468UL:9276UL));assert(!vector&&resourcesOwned&&quiesced&&!locks);
  assert(!initCount);Disable();assert(!level1AudioStart());Enable();assert(!running&&!vector);
  titlePlaying=0;
  for(i=0;i<3;i++){
   Disable();assert(level1AudioStart());Enable();assert(running&&mt_Enable&&vector);
   level1AudioRequest(0);level1AudioRequest(1);assert(mixer.voice[0].remaining&&mixer.voice[1].remaining);
   level1AudioIRQ();level1AudioUpdate();assert(nextBuffer==0);
   Disable();level1AudioStop();Enable();assert(!running&&!mt_Enable&&quiesced&&!vector&&filter==2);
   level1AudioIRQ();assert(nextBuffer==0);
  }
  level1AudioUnload();level1AudioUnload();assert(!allocations&&!resourcesOwned&&!vector&&!locks);
  initCount=0;
 }
 /* Music-only preview never installs AUD3, renders buffers, or admits SFX.
    Temporary Iron data cannot replace the prepared Copper allocations. */
 titlePlaying=0;assert(level1AudioLoad(FALSE));
 { UBYTE *originalScore=score,*originalBank=bank;int resident=allocations;
   int n;
   for(n=0;n<12;n++) {
    assert(level1AudioPreviewPrepare(TRUE));assert(allocations==resident+2);
    assert(score==originalScore&&bank==originalBank);
    Disable();assert(level1AudioStartMusic());Enable();
    assert(running&&!mixing&&!vector&&lastScore==previewScore&&lastBank==previewBank);
    level1AudioRequest(1);level1AudioUpdate();level1AudioIRQ();
    assert(!mixer.voice[1].remaining&&nextBuffer==1);
    level1AudioPreviewClear();assert(previewScore); /* refuse unsafe free */
    Disable();level1AudioStop();Enable();level1AudioPreviewClear();
    assert(!previewScore&&!previewBank&&allocations==resident);
    assert(level1AudioPreviewPrepare(FALSE));
    Disable();assert(level1AudioStartMusic());Enable();
    assert(lastScore==originalScore&&lastBank==originalBank&&!vector);
    Disable();level1AudioStop();Enable();
   }
   for(n=1;n<=2;n++) {
    allocCalls=0;failAlloc=n;assert(!level1AudioPreviewPrepare(TRUE));
    assert(allocations==resident&&!previewScore&&!previewBank&&score==originalScore);
   }
   failAlloc=0;failOpen=opens+2;assert(!level1AudioPreviewPrepare(TRUE));failOpen=0;
   assert(allocations==resident&&!previewScore&&!previewBank);
   Disable();assert(level1AudioStart());Enable();assert(mixing&&vector);
   Disable();level1AudioStop();Enable();
 }
 level1AudioUnload();assert(!allocations&&!locks&&!vector&&!resourcesOwned);
 assert(pauseCount>=48);
 puts("PASS: preload with title live, every allocation failure, install failure, 24 starts/stops, IRQ quiesce and balanced cleanup");return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'test.c').write_text(shim+'\n#include "audio_mix.h"\n#include "audio_catalog.h"\n'+s+checks)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsanitize=address,undefined','-I'+str(ROOT/'src'),str(p/'test.c'),str(ROOT/'src/audio_mix.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
# Inspect the actual generated OS-compatible install/remove boundaries.
a=player_source();install=a.split('_mt_install:',1)[1].split('mt_ioport:',1)[0]
remove=a.split('_mt_remove:',1)[1].split('mt_cia_timer_a_code:',1)[0]
assert '_LVOOpenDevice(a6)' not in install and 'bra\tmt_reset' not in install
assert '_LVOCloseDevice(a6)' not in remove and '_LVOFreeSignal(a6)' not in remove
assert '\tENABLE\n' not in a and '\tDISABLE\n' not in a
assert a.index('_mt_pause_timer_b:')<a.index('\n\tend\n')
print('PASS: generated timer-only resource owner, no title/Paula reset during install')
# Reuse the independently written sample-at-a-time oracle from the proof.
ref=(ROOT/'experiments/audio-level1/mix_reference.h').read_text()
ref=re.sub(r'^.*m->overlaps\+\+;\n','',ref,flags=re.M)
ref=ref.replace('m->completed[j]++;','').replace('m->rendered+=count;','')
t=(ROOT/'experiments/audio-level1/test_mix.c').read_text()
t=t.replace('"mix.h"','"audio_mix.h"')
t=re.sub(r'assert\(m.completed\[[^;]+;','',t)
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'mix_reference.h').write_text(ref);(p/'test.c').write_text(t)
 subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-I'+str(ROOT/'src'),str(p/'test.c'),str(ROOT/'src/audio_mix.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
