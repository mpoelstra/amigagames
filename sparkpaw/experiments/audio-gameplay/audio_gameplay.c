/* Candidate-only adapter. The production audio.c is selected for reference A. */
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/interrupts.h>
#include <hardware/custom.h>
#include <hardware/intbits.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/cia.h>
#include <string.h>
#include "audio.h"
#include "mix.h"
#include "catalog.h"
int mt_install(void);void mt_remove(void);
void mt_init(__reg("a6") void *,__reg("a0") void *,__reg("a1") void *,__reg("d0") UBYTE);
void mt_end(__reg("a6") void *);
void mt_channelmask(__reg("a6") void *,__reg("d0") UBYTE);
void mt_mastervol(__reg("a6") void *,__reg("d0") UWORD);
extern UBYTE mt_Enable;
void proof_audio_irq(void);
static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static Mixer mixer;static Effect effects[FX_COUNT];
static UBYTE *fxData[FX_COUNT],*score,*bank,*buffers;
static ULONG scoreSize,bankSize;
volatile ULONG proofMusicTicks;
static ULONG irqCount;
static UWORD nextBuffer=1;
static volatile BOOL running;
static BOOL installed,vectorInstalled;
static struct Interrupt audioInt,*oldAudio;
static UBYTE oldFilter;
static struct Library *ciaResource;
static UBYTE *load(const char *name,ULONG flags,ULONG *length)
{
 BPTR f=Open((STRPTR)name,MODE_OLDFILE);LONG n;UBYTE *p;
 *length=0;if(!f)return NULL;
 Seek(f,0,OFFSET_END);n=Seek(f,0,OFFSET_BEGINNING);
 if(n<=0){Close(f);return NULL;}
 p=AllocMem(n,flags);
 if(p&&Read(f,p,n)!=n){FreeMem(p,n);p=NULL;}
 Close(f);if(p)*length=n;return p;
}
BOOL audioLoad(void)
{
 UWORD i;ULONG n,j;
 ciaResource=(struct Library *)OpenResource("ciab.resource");
 if(!ciaResource)return FALSE;
 oldFilter=*(volatile UBYTE *)0xbfe001&2;
 score=load("PROGDIR:assets/runtime/pulse-score.bin",MEMF_FAST,&scoreSize);
 bank=load("PROGDIR:assets/runtime/pulse-bank.bin",MEMF_CHIP,&bankSize);
 buffers=AllocMem(2*MIX_BYTES,MEMF_CHIP|MEMF_CLEAR);
 if(!score||!bank||!buffers)goto fail;
 for(i=0;i<FX_COUNT;i++) {
  fxData[i]=load(paths[i],MEMF_FAST,&n);if(!fxData[i])goto fail;
  effects[i].data=(int8_t *)fxData[i];effects[i].length=n;
  effects[i].priority=priorities[i];effects[i].cooldown=cooldowns[i];
  for(j=0;j<n;j++)((int8_t *)fxData[i])[j]=(int8_t)(((int)((int8_t *)fxData[i])[j]*volumes[i])/128);
 }
 if(!mt_install())goto fail;installed=TRUE;
 Disable();mt_init((void *)hw,score,bank,0);mt_channelmask((void *)hw,7);
 mt_mastervol((void *)hw,48);
 memset(&audioInt,0,sizeof(audioInt));
 audioInt.is_Node.ln_Type=NT_INTERRUPT;audioInt.is_Node.ln_Name="Sparkpaw game mixer";
 audioInt.is_Code=proof_audio_irq;
 hw->intena=INTF_AUD3;hw->intreq=INTF_AUD3;
 oldAudio=SetIntVector(INTB_AUD3,&audioInt);vectorInstalled=TRUE;Enable();
 return TRUE;
fail:
 audioUnload();return FALSE;
}
void proofAudioIRQ(void)
{
 hw->intreq=INTF_AUD3;hw->intreq=INTF_AUD3;
 if(!running)return;
 mixRender(&mixer,(int8_t *)(buffers+nextBuffer*MIX_BYTES),MIX_BYTES);
 hw->aud[3].ac_ptr=(UWORD *)(buffers+nextBuffer*MIX_BYTES);
 hw->aud[3].ac_len=MIX_BYTES/2;nextBuffer^=1;irqCount++;
}
void audioSetHardwareActive(BOOL active)
{
 /* Called inside platform's disabled takeover/restore section. */
 if(active) {
  if(!installed)return;
  hw->dmacon=8;hw->intena=INTF_AUD3;
  hw->aud[3].ac_ptr=(UWORD *)buffers;hw->aud[3].ac_len=MIX_BYTES/2;
  hw->aud[3].ac_per=322;hw->aud[3].ac_vol=64;
  hw->intreq=INTF_AUD3;hw->intreq=INTF_AUD3;
  running=TRUE;mt_Enable=1;hw->dmacon=0x8208;
 } else {
  running=FALSE;mt_Enable=0;
  if(installed){AbleICR(ciaResource,3);SetICR(ciaResource,3);}
  hw->intena=INTF_AUD3;hw->dmacon=15;
  hw->intreq=INTF_AUD3;hw->intreq=INTF_AUD3;
 }
}
static void request(unsigned id)
{
 if(!running)return;
 /* Serialize only the two-voice state; level-6 music stays live. */
 hw->intena=INTF_AUD3;mixRequest(&mixer,effects,id);
 hw->intena=INTF_SETCLR|INTF_AUD3;
}
void audioUpdate(void)
{
 if(!running)return;
 hw->intena=INTF_AUD3;mixField(&mixer);hw->intena=INTF_SETCLR|INTF_AUD3;
}
#define FX(fn,id) void fn(void){request(id);}
FX(audioPlayShot,0) FX(audioPlayPlayerHurt,1) FX(audioPlayEnemyHit,2)
FX(audioPlayEnemyDeath,3) FX(audioPlayStriderShot,4) FX(audioPlayJump,5)
FX(audioPlayCollect,6) FX(audioPlayWaterSplash,7) FX(audioPlayStormstoneCore,8)
FX(audioPlayTallyTick,9) FX(audioPlayExtraLife,10)
FX(audioPlayHarrierFanCharge,11) FX(audioPlayHarrierFanFire,12)
FX(audioPlayHarrierHunterCharge,13) FX(audioPlayHarrierHunterFire,14)
FX(audioPlayHealthCollect,15)
void audioGameWrite(BPTR f)
{
 UWORD i;
 FPrintf(f,"music_ticks=%ld mixer_services=%ld mixed_samples=%ld overlap_buffers=%ld\n",proofMusicTicks,irqCount,mixer.rendered,mixer.overlaps);
 FPrintf(f,"music_chip=%ld music_fast=%ld output_chip=224 fx_fast=48130\n",bankSize,scoreSize);
 for(i=0;i<FX_COUNT;i++)FPrintf(f,"effect=%s requested=%ld started=%ld suppressed=%ld\n",(STRPTR)names[i],mixer.requests[i],mixer.starts[i],mixer.rejected[i]);
}
void audioUnload(void)
{
 UWORD i;
 if(installed) {
  Disable();audioSetHardwareActive(FALSE);mt_end((void *)hw);
  if(vectorInstalled){SetIntVector(INTB_AUD3,oldAudio);vectorInstalled=FALSE;}
  Enable();mt_remove();installed=FALSE;
 }
 if(buffers){FreeMem(buffers,2*MIX_BYTES);buffers=NULL;}
 for(i=0;i<FX_COUNT;i++)if(fxData[i]){FreeMem(fxData[i],effects[i].length);fxData[i]=NULL;}
 if(score){FreeMem(score,scoreSize);score=NULL;}
 if(bank){FreeMem(bank,bankSize);bank=NULL;}
 if(oldFilter)*(volatile UBYTE *)0xbfe001|=2;else *(volatile UBYTE *)0xbfe001&=~2;
}
