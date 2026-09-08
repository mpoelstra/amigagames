#if defined(SPARKPAW_RENDER_DIAGNOSTIC)
#error Level1_audio_owns_CIA_B_do_not_link_the_renderer_CIA_profiler
#endif
/* Shared gameplay backend (historical Level-1 API name). music.c retains the session audio.device reservation. */
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
#include "level1_audio.h"
#include "music.h"
#ifdef SPARKPAW_MULTI_ADF
#include "assets.h"
#endif
#include "audio_mix.h"
#include "audio_catalog.h"
int mt_install(void);void mt_remove(void);
void mt_pause_timer_b(void);
void mt_init(__reg("a6") void *,__reg("a0") void *,__reg("a1") void *,__reg("d0") UBYTE);
void mt_end(__reg("a6") void *);
void mt_channelmask(__reg("a6") void *,__reg("d0") UBYTE);
void mt_mastervol(__reg("a6") void *,__reg("d0") UWORD);
extern UBYTE mt_Enable;
void level1_audio_irq(void);
static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static Mixer mixer;static Effect effects[FX_COUNT];
static UBYTE *fxData[FX_COUNT],*score,*bank,*buffers;
static ULONG scoreSize,bankSize;
static UWORD nextBuffer=1;
static volatile BOOL running;
static BOOL installed,vectorInstalled;
static struct Interrupt audioInt,*oldAudio;
static UBYTE oldFilter;
static struct Library *ciaResource;
static UBYTE *load(const char *name,ULONG flags,ULONG *length)
{
#ifdef SPARKPAW_MULTI_ADF
 return assetsLoadDiskData(name,flags,length);
#else
 BPTR f=Open((STRPTR)name,MODE_OLDFILE);LONG n;UBYTE *p;
 *length=0;if(!f)return NULL;
 Seek(f,0,OFFSET_END);n=Seek(f,0,OFFSET_BEGINNING);
 if(n<=0){Close(f);return NULL;}
 p=AllocMem(n,flags);
 if(p&&Read(f,p,n)!=n){FreeMem(p,n);p=NULL;}
 Close(f);if(p)*length=n;return p;
#endif
}
BOOL level1AudioLoad(BOOL stormrail)
{
 UWORD i;ULONG n,j;
 ciaResource=(struct Library *)OpenResource("ciab.resource");
 if(!ciaResource)return FALSE;
 if(!musicInitialize(50))return FALSE;
 score=load(stormrail?"PROGDIR:assets/runtime/rail-score.bin":
                     "PROGDIR:assets/runtime/pulse-score.bin",MEMF_FAST,&scoreSize);
 bank=load(stormrail?"PROGDIR:assets/runtime/rail-bank.bin":
                    "PROGDIR:assets/runtime/pulse-bank.bin",MEMF_CHIP,&bankSize);
 buffers=AllocMem(2*MIX_BYTES,MEMF_CHIP|MEMF_CLEAR);
 if(!score||!bank||!buffers)goto fail;
 /* This backend ships two certified scores, not arbitrary MOD input. */
 if(scoreSize!=(stormrail?17468UL:9276UL)||bankSize!=11552||memcmp(score+1080,"M.K.",4))goto fail;
 for(i=0;i<FX_COUNT;i++) {
  fxData[i]=load(paths[i],MEMF_FAST,&n);if(!fxData[i])goto fail;
  effects[i].data=(int8_t *)fxData[i];effects[i].length=n;
  effects[i].priority=priorities[i];effects[i].cooldown=cooldowns[i];
  for(j=0;j<n;j++)((int8_t *)fxData[i])[j]=(int8_t)(((int)((int8_t *)fxData[i])[j]*volumes[i])/128);
 }
 /* Timer-only install cannot allocate/steal Paula or touch its registers.
    Keep new CIA callbacks masked until gameplay; title VBlank remains live. */
 Disable();
 if(!mt_install()){Enable();goto fail;}
 installed=TRUE;AbleICR(ciaResource,3);SetICR(ciaResource,3);
 Enable();
 return TRUE;
fail:
 level1AudioUnload();return FALSE;
}
void level1AudioIRQ(void)
{
 hw->intreq=INTF_AUD3;hw->intreq=INTF_AUD3;
 if(!running)return;
 mixRender(&mixer,(int8_t *)(buffers+nextBuffer*MIX_BYTES),MIX_BYTES);
 hw->aud[3].ac_ptr=(UWORD *)(buffers+nextBuffer*MIX_BYTES);
 hw->aud[3].ac_len=MIX_BYTES/2;nextBuffer^=1;
}
BOOL level1AudioStart(void)
{
 UWORD mask;
 if(!installed||running)return FALSE;
 /* titleRelease/musicStop must precede this call. */
 if(musicIsPlaying())return FALSE;
 oldFilter=*(volatile UBYTE *)0xbfe001&2;
 mt_pause_timer_b();
 memset(&mixer,0,sizeof(mixer));memset(buffers,0,2*MIX_BYTES);nextBuffer=1;
 mask=hw->intenar;
 mt_init((void *)hw,score,bank,0);mt_channelmask((void *)hw,7);
 mt_mastervol((void *)hw,48);
 /* Preserve the platform mask across player initialization. */
 hw->intena=0x7fff;hw->intena=0x8000|mask;
 memset(&audioInt,0,sizeof(audioInt));
 audioInt.is_Node.ln_Type=NT_INTERRUPT;
 audioInt.is_Node.ln_Name="Sparkpaw game mixer";
 audioInt.is_Code=level1_audio_irq;
 hw->intena=INTF_AUD3;hw->intreq=INTF_AUD3;
 oldAudio=SetIntVector(INTB_AUD3,&audioInt);vectorInstalled=TRUE;
 hw->aud[3].ac_ptr=(UWORD *)buffers;hw->aud[3].ac_len=MIX_BYTES/2;
 hw->aud[3].ac_per=322;hw->aud[3].ac_vol=64;
 hw->intreq=INTF_AUD3;hw->intreq=INTF_AUD3;
 running=TRUE;mt_Enable=1;
 SetICR(ciaResource,3);AbleICR(ciaResource,0x83);
 hw->dmacon=0x8208;
 return TRUE;
}
void level1AudioStop(void)
{
 UWORD mask;
 if(!running)return;
 running=FALSE;mt_Enable=0;
 AbleICR(ciaResource,3);mt_pause_timer_b();SetICR(ciaResource,3);
 hw->intena=INTF_AUD3;hw->dmacon=15;
 hw->intreq=INTF_AUD3;hw->intreq=INTF_AUD3;
 mask=hw->intenar;mt_end((void *)hw);
 hw->intena=0x7fff;hw->intena=0x8000|mask;
 if(vectorInstalled){SetIntVector(INTB_AUD3,oldAudio);vectorInstalled=FALSE;}
 if(oldFilter)*(volatile UBYTE *)0xbfe001|=2;
 else *(volatile UBYTE *)0xbfe001&=~2;
}
BOOL level1AudioRunning(void){return running;}
void level1AudioRequest(unsigned id)
{
 if(!running)return;
 /* Serialize only the two-voice state; level-6 music stays live. */
 hw->intena=INTF_AUD3;mixRequest(&mixer,effects,id);
 hw->intena=INTF_SETCLR|INTF_AUD3;
}
void level1AudioUpdate(void)
{
 if(!running)return;
 hw->intena=INTF_AUD3;mixField(&mixer);hw->intena=INTF_SETCLR|INTF_AUD3;
}
void level1AudioUnload(void)
{
 UWORD i;
 if(installed) {
  Disable();level1AudioStop();
  AbleICR(ciaResource,3);SetICR(ciaResource,3);
  mt_remove();installed=FALSE;Enable();
 }
 if(buffers){FreeMem(buffers,2*MIX_BYTES);buffers=NULL;}
 for(i=0;i<FX_COUNT;i++)if(fxData[i]){FreeMem(fxData[i],effects[i].length);fxData[i]=NULL;}
 if(score){FreeMem(score,scoreSize);score=NULL;}
 if(bank){FreeMem(bank,bankSize);bank=NULL;}
}
