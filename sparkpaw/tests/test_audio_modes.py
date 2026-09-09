"""Actual effect dispatch: gameplay mode only, every effect, result/preview bypass."""
from pathlib import Path
import re, subprocess, tempfile
ROOT=Path(__file__).resolve().parents[1]
s=re.sub(r'^#include[^\n]*\n','',(ROOT/'src/audio.c').read_text(),flags=re.M)
s=s.replace('(volatile struct Custom *)0xdff000','&fakeCustom')
a=s.index('static void waitAudioLatch(void)');b=s.index('static void startOneShot',a)
s=s[:a]+'static void waitAudioLatch(void) { latchCalls++; }\n'+s[b:]
shim=r'''
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "audio_contract.h"
#define SPARKPAW_LEVEL1_MUSIC
typedef uint8_t UBYTE;typedef uint16_t UWORD;typedef uint32_t ULONG;
typedef long LONG;typedef int BOOL;typedef int BPTR;typedef const char *CONST_STRPTR;
#define TRUE 1
#define FALSE 0
#define MEMF_CHIP 1
#define MEMF_CLEAR 2
#define MODE_OLDFILE 0
#define OFFSET_END 1
#define OFFSET_BEGINNING -1
#define DMAF_AUD0 1
#define DMAF_AUD1 2
#define DMAF_SETCLR 32768
enum AudioMode { AUDIO_FX_ONLY,AUDIO_MUSIC_ONLY,AUDIO_FX_MUSIC };
static struct Custom {UWORD dmacon;struct {UWORD *ac_ptr;UWORD ac_len,ac_per,ac_vol;}aud[4];}fakeCustom;
static int running,requests,lastId,latchCalls,allocations;
static BOOL level1AudioRunning(void){return running;}
static void level1AudioRequest(unsigned id){requests++;lastId=id;}
static void level1AudioUpdate(void){}
static void level1AudioStop(void){running=0;}
static void level1AudioUnload(void){running=0;}
static BOOL level1AudioLoad(BOOL rail){(void)rail;return TRUE;}
static BOOL gameStormrailActive(void){return FALSE;}
static BPTR Open(const char *n,int m){(void)n;(void)m;return 1;}
static LONG Seek(BPTR f,int n,int m){(void)f;(void)n;return m==OFFSET_BEGINNING?128:0;}
static LONG Read(BPTR f,void *p,LONG n){(void)f;memset(p,1,n);return n;}
static void Close(BPTR f){(void)f;}
static void *AllocMem(ULONG n,int flags){(void)flags;allocations++;return calloc(1,n);}
static void FreeMem(void *p,ULONG n){(void)n;allocations--;free(p);}
void audioUnload(void);
'''
checks=r'''
int main(void){
 void (*const play[])(void)={audioPlayShot,audioPlayPlayerHurt,audioPlayEnemyHit,
  audioPlayEnemyDeath,audioPlayStriderShot,audioPlayJump,audioPlayCollect,
  audioPlayWaterSplash,audioPlayStormstoneCore,audioPlayTallyTick,audioPlayExtraLife,
  audioPlayHarrierFanCharge,audioPlayHarrierFanFire,audioPlayHarrierHunterCharge,
  audioPlayHarrierHunterFire,audioPlayHealthCollect};
 int mode,id;
 assert(audioGetMode()==AUDIO_FX_MUSIC);assert(audioLoad());
 for(mode=0;mode<3;mode++) for(id=0;id<16;id++) {
  audioSetHardwareActive(FALSE);audioSetHardwareActive(TRUE);
  audioSetMode((enum AudioMode)mode);audioBeginGameplay();running=mode!=0;
  latchCalls=requests=0;play[id]();
  if(mode==AUDIO_FX_ONLY)assert(latchCalls==2&&!requests);
  if(mode==AUDIO_MUSIC_ONLY)assert(!latchCalls&&!requests);
  if(mode==AUDIO_FX_MUSIC)assert(!latchCalls&&requests==1&&lastId==id);
  /* Results and solo previews ignore session mode, after owner retirement. */
  audioSetHardwareActive(FALSE);audioSetHardwareActive(TRUE);
  latchCalls=requests=0;play[id]();assert(latchCalls==2&&!requests);
  latchCalls=0;audioPreviewEffect(id);assert(latchCalls==2&&!requests);
  assert(audioGetMode()==mode&&audioPreviewEffectPlaying());
  audioUpdate();audioUpdate();assert(!audioPreviewEffectPlaying());
 }
 audioSetHardwareActive(FALSE);audioSetMode(AUDIO_MUSIC_ONLY);audioUnload();
 assert(audioGetMode()==AUDIO_MUSIC_ONLY&&!allocations);
 puts("PASS: all 16 effects in three gameplay modes; result/preview bypass and session persistence");
 return 0;
}
'''
with tempfile.TemporaryDirectory() as td:
    p=Path(td);(p/'test.c').write_text(shim+s+checks)
    subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-Wno-sign-compare',
                    '-fsanitize=address,undefined','-I'+str(ROOT/'src'),str(p/'test.c'),
                    '-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],check=True)

# Exercise the actual platform start/stop helpers, including Exec nesting.
platform=(ROOT/'src/platform_amiga.c').read_text()
a=platform.index('void platformStartGameplayAudio(void)')
b=platform.index('void platformSwitchCopper',a)
platform=platform[a:b]
shim=r'''
#include <assert.h>
#include <stdio.h>
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define SPARKPAW_LEVEL1_MUSIC
enum { AUDIO_FX_ONLY,AUDIO_MUSIC_ONLY,AUDIO_FX_MUSIC };
static struct {unsigned short intena;} hw,*hardware=&hw;
static BOOL interruptsDisabled=TRUE,audioInterruptsEnabled,pauseToggleRequested;
static int depth=1,mode,gameplay,running,mixing;
static void Disable(void){depth++;}
static void Enable(void){assert(depth==1);depth--;}
static void audioBeginGameplay(void){gameplay=1;}
static int audioGetMode(void){return mode;}
static int level1AudioStart(void){assert(depth==1);if(running)return 0;running=mixing=1;return 1;}
static int level1AudioStartMusic(void){assert(depth==1);if(running)return 0;running=1;mixing=0;return 1;}
static void audioSetHardwareActive(int active){assert(depth==1);if(!active){running=gameplay=mixing=0;}}
'''
checks=r'''
int main(void){int i;
 for(i=0;i<30;i++) {
  mode=i%3;platformStartGameplayAudio();assert(gameplay);
  if(mode==AUDIO_FX_ONLY)assert(depth==1&&!running&&!audioInterruptsEnabled);
  else {
   assert(depth==0&&audioInterruptsEnabled&&running);
   assert(mixing==(mode==AUDIO_FX_MUSIC));
   assert(hw.intena==(mode==AUDIO_MUSIC_ONLY?0xa000:0xa400));
  }
  platformStartGameplayAudio(); /* repeat cannot unbalance Enable */
  platformStopMenuPreview();assert(depth==1&&!running&&!audioInterruptsEnabled&&!gameplay);
  assert(platformStartMenuMusic());assert(depth==0&&running&&!mixing&&hw.intena==0xa000);
  assert(!platformStartMenuMusic());
  platformStopMenuPreview();platformStopMenuPreview();
  assert(depth==1&&!running&&!audioInterruptsEnabled);
 }
 puts("PASS: actual platform helpers, all modes, preview IRQ masks and balanced Disable/Enable");
 return 0;
}
'''
with tempfile.TemporaryDirectory() as td:
    p=Path(td);(p/'test.c').write_text(shim+platform+checks)
    subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',
                    '-fsanitize=address,undefined',str(p/'test.c'),'-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],check=True)
