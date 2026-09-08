/* Standalone OS-live audio proof. Not linked into Sparkpaw production. */
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/interrupts.h>
#include <exec/execbase.h>
#include <hardware/custom.h>
#include <hardware/intbits.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <string.h>
#include "mix.h"
#ifdef AUDIO_TIMING
#include "timing.h"
#endif
extern struct ExecBase *SysBase;
int mt_install(void);
void mt_remove(void);
void mt_init(__reg("a6") void *,__reg("a0") void *,__reg("a1") void *,__reg("d0") UBYTE);
void mt_end(__reg("a6") void *);
void mt_channelmask(__reg("a6") void *,__reg("d0") UBYTE);
void mt_mastervol(__reg("a6") void *,__reg("d0") UWORD);
extern UBYTE mt_Enable;
void proof_audio_irq(void);void proof_vblank_irq(void);
static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static Mixer mixer;static Effect effects[FX_COUNT];
static UBYTE *fxData[FX_COUNT],*score,*bank,*buffers;
static ULONG scoreSize,bankSize;
volatile ULONG proofMusicTicks;
static volatile ULONG fields,irqCount,irqMaxLines,irqTotalLines,lateGaps;
static volatile UWORD latest=0xffff;
static ULONG previousStamp;
static UWORD nextBuffer=1;
static volatile BOOL running;
static const char *names[FX_COUNT]={
 "plasma","hurt","enemy hit / debris hit","enemy death / debris break",
 "Strider fire","jump","diamond pickup","water splash","Stormstone Core",
 "tally","1UP","Harrier fan charge","Harrier fan fire",
 "Harrier hunter charge","Harrier hunter fire","health pickup"};
static const char *paths[FX_COUNT]={
 "PROGDIR:assets/runtime/energy-shot.raw","PROGDIR:assets/runtime/player-hurt.raw",
 "PROGDIR:assets/runtime/enemy-hit.raw","PROGDIR:assets/runtime/enemy-death.raw",
 "PROGDIR:assets/runtime/strider-shot.raw","PROGDIR:assets/runtime/jump.raw",
 "PROGDIR:assets/runtime/collect-spark.raw","PROGDIR:assets/runtime/water-splash.raw",
 "PROGDIR:assets/runtime/stormstone-core.raw","PROGDIR:assets/runtime/tally-tick.raw",
 "PROGDIR:assets/runtime/extra-life.raw","PROGDIR:assets/runtime/harrier-fan-charge.raw",
 "PROGDIR:assets/runtime/harrier-fan-fire.raw","PROGDIR:assets/runtime/harrier-hunter-charge.raw",
 "PROGDIR:assets/runtime/harrier-hunter-fire.raw","PROGDIR:assets/runtime/collect-spark.raw"};
static const UBYTE priorities[FX_COUNT]={127,9,6,8,7,4,5,10,11,3,10,7,7,7,8,5};
static const UBYTE volumes[FX_COUNT]={60,64,60,64,64,58,58,64,64,54,62,64,64,64,64,64};
static const UBYTE cooldowns[FX_COUNT]={0,16,4,6,12,4,3,20,55,1,20,16,8,16,6,3};
static UBYTE *load(const char *name,ULONG flags,ULONG *length)
{
 BPTR f=Open((STRPTR)name,MODE_OLDFILE);LONG size;UBYTE *p;
 if(!f)return NULL;Seek(f,0,OFFSET_END);size=Seek(f,0,OFFSET_BEGINNING);
 if(size<=0){Close(f);return NULL;}
 p=AllocMem(size,flags);if(p&&Read(f,p,size)!=size){FreeMem(p,size);p=NULL;}
 Close(f);*length=size;return p;
}
static UWORD line(void){return (UWORD)(((hw->vposr&7)<<8)|(hw->vhposr>>8));}
void proofAudioIRQ(void)
{
#if !defined(AUDIO_TIMING) && !defined(AUDIO_CONTROL)
 UWORD start=line(),finish,used;ULONG stamp;
#endif
 hw->intreq=INTF_AUD3;hw->intreq=INTF_AUD3;
 if(!running)return;
#ifdef AUDIO_TIMING
 proofTimingEnter(0);
#endif
 /* AUD3 interrupt marks start of current buffer. Refill the other buffer,
    publish it as next reload; never overwrite the currently DMA-read block. */
 mixRender(&mixer,(int8_t *)(buffers+nextBuffer*MIX_BYTES),MIX_BYTES);
 hw->aud[3].ac_ptr=(UWORD *)(buffers+nextBuffer*MIX_BYTES);
 hw->aud[3].ac_len=MIX_BYTES/2;nextBuffer^=1;
#if !defined(AUDIO_TIMING) && !defined(AUDIO_CONTROL)
 finish=line();used=finish>=start?finish-start:312-start+finish;
 irqTotalLines+=used;if(used>irqMaxLines)irqMaxLines=used;
 /* Raster-only duration estimate; no CIA timer is stolen for profiling. */
 stamp=fields*312+finish;
 if(irqCount&&stamp>previousStamp+400)lateGaps++;
 previousStamp=stamp;
#elif defined(AUDIO_TIMING)
 proofTimingLeave();
#endif
 irqCount++;
}
static void eventField(void)
{
 unsigned id;
 if(!running)return;
 fields++;
 id=mixSchedule(&mixer,effects,fields);
 if(id<FX_COUNT)latest=id;
}
void proofVBlank(void)
{
 /* Level-4 audio may preempt VBlank: protect shared voice publication only.
    CIA music (level 6) remains serviceable; no global interrupt masking. */
 hw->intena=INTF_AUD3;
 eventField();
 if(running)hw->intena=INTF_SETCLR|INTF_AUD3;
}
static void logResult(void)
{
 UWORD i;BPTR f=Open("PROGDIR:renderdiag.log",MODE_NEWFILE);
 if(!f)return;
 FPrintf(f,"Copper Sprint / audio-only / ptplayer 6.4 CIA / two SFX voices\n");
#ifdef AUDIO_TIMING
 FPuts(f,"build=audio-block-v1-measured\n");
#elif defined(AUDIO_CONTROL)
 FPuts(f,"build=audio-block-v1-control\n");
#endif
#if !defined(AUDIO_TIMING) && !defined(AUDIO_CONTROL)
 FPrintf(f,"fields=%ld irq_count=%ld irq_max_raster_lines=%ld irq_total_raster_lines=%ld coarse_late_gaps=%ld\n",fields,irqCount,irqMaxLines,irqTotalLines,lateGaps);
#else
 FPrintf(f,"fields=%ld irq_count=%ld\n",fields,irqCount);
 #ifdef AUDIO_TIMING
 proofTimingWrite(f);
 #endif
#endif
 FPrintf(f,"music_ticks=%ld\n",proofMusicTicks);
 FPrintf(f,"music_chip=%ld music_fast=%ld output_chip=%ld mixed_samples=%ld overlap_buffers=%ld\n",bankSize,scoreSize,(LONG)(2*MIX_BYTES),mixer.rendered,mixer.overlaps);
 for(i=0;i<FX_COUNT;i++)FPrintf(f,"effect=%s requested=%ld started=%ld suppressed=%ld\n",(STRPTR)names[i],mixer.requests[i],mixer.starts[i],mixer.rejected[i]);
 FPrintf(f,"post_run=complete; no integrated-game performance claim\n");
 Close(f);
}
int main(void)
{
 struct Interrupt audioInt,vblankInt,*oldAudio=NULL;
 BOOL installed=FALSE,handlers=FALSE;UWORD i,last=0xffff;ULONG n,j,stallField=0;
 BPTR console=0;int result=20;UBYTE oldFilter;
#ifdef AUDIO_TIMING
 BOOL timingReady=FALSE;
#endif
 if(SysBase->VBlankFrequency!=50){PutStr("PAL 50 Hz required for this proof.\n");return 20;}
 oldFilter=*(volatile UBYTE *)0xbfe001&2;
 score=load("PROGDIR:assets/runtime/pulse-score.bin",MEMF_FAST,&scoreSize);
 bank=load("PROGDIR:assets/runtime/pulse-bank.bin",MEMF_CHIP,&bankSize);
 buffers=AllocMem(2*MIX_BYTES,MEMF_CHIP|MEMF_CLEAR);
 if(!score||!bank||!buffers)goto done;
 for(i=0;i<FX_COUNT;i++) {
   fxData[i]=load(paths[i],MEMF_FAST,&n);if(!fxData[i])goto done;
   effects[i].data=(int8_t *)fxData[i];effects[i].length=n;
   effects[i].priority=priorities[i];effects[i].cooldown=cooldowns[i];
   /* Pre-scale fixed event gain and two-voice headroom once, not per sample IRQ.
      Division rounds toward zero, avoiding DC bias from negative right shifts. */
   for(j=0;j<n;j++)((int8_t *)fxData[i])[j]=(int8_t)(((int)((int8_t *)fxData[i])[j]*volumes[i])/128);
 }
#ifdef AUDIO_TIMING
 if(!proofTimingOpen())goto done;timingReady=TRUE;
#endif
 if(!mt_install())goto done;installed=TRUE;
 mt_init((void *)hw,score,bank,0);mt_channelmask((void *)hw,7);
 mt_mastervol((void *)hw,48);
 memset(&audioInt,0,sizeof(audioInt));memset(&vblankInt,0,sizeof(vblankInt));
 audioInt.is_Node.ln_Type=NT_INTERRUPT;audioInt.is_Node.ln_Name="Sparkpaw SFX proof";
 audioInt.is_Code=proof_audio_irq;
 vblankInt.is_Node.ln_Type=NT_INTERRUPT;vblankInt.is_Node.ln_Pri=-20;
 vblankInt.is_Node.ln_Name="Sparkpaw proof events";vblankInt.is_Code=proof_vblank_irq;
 console=Open("CON:0/0/640/160/Sparkpaw Copper Sprint/AUTO/CLOSE",MODE_NEWFILE);
 if(console)FPuts(console,"COPPER SPRINT - standalone audio proof\n164 BPM / 3 music channels + 2 SFX on channel 4\nFirst 47 seconds: music alone. Then all effects and overlaps.\nLMB stops and saves renderdiag.log; automatic finish after 94 seconds.\nReturns to Workbench/Shell. No game graphics or game code.\n");
 Disable();hw->dmacon=8;hw->intena=INTF_AUD3;hw->intreq=INTF_AUD3;
 oldAudio=SetIntVector(INTB_AUD3,&audioInt);AddIntServer(INTB_VERTB,&vblankInt);handlers=TRUE;
 hw->aud[3].ac_ptr=(UWORD *)buffers;hw->aud[3].ac_len=MIX_BYTES/2;
 hw->aud[3].ac_per=322;hw->aud[3].ac_vol=64;
#ifdef AUDIO_TIMING
 proofTimingStart();
#endif
 running=TRUE;mt_Enable=1;
 hw->intena=INTF_SETCLR|INTF_INTEN|INTF_AUD3;hw->dmacon=0x8208;Enable();
 /* Require release first, so launching by a mouse press cannot stop the proof. */
 while(!(*(volatile UBYTE *)0xbfe001&64))Delay(1);
 while(fields<4684&&(*(volatile UBYTE *)0xbfe001&64)) {
   if(latest!=last){last=latest;if(console&&last<FX_COUNT)FPrintf(console,"SFX: %s\n",(STRPTR)names[last]);}
   if(fields>=2342&&fields-stallField>150) {
     ULONG begin=fields;stallField=fields;
     /* Deliberately busy foreground for three fields; all audio IRQs stay live. */
     while(fields-begin<3) { }
   }
   Delay(1);
 }
 result=0;
done:
 if(handlers) {
   Disable();
#ifdef AUDIO_TIMING
   proofTimingStop();
#endif
   running=FALSE;hw->intena=INTF_AUD3;hw->dmacon=8;
   hw->intreq=INTF_AUD3;hw->intreq=INTF_AUD3;
   RemIntServer(INTB_VERTB,&vblankInt);SetIntVector(INTB_AUD3,oldAudio);Enable();
 }
 if(installed){mt_Enable=0;mt_end((void *)hw);mt_remove();}
 if(result==0)logResult();else PutStr("Audio proof failed to load or reserve audio.\n");
#ifdef AUDIO_TIMING
 if(timingReady)proofTimingClose();
#endif
 if(console)Close(console);
 if(buffers)FreeMem(buffers,2*MIX_BYTES);
 for(i=0;i<FX_COUNT;i++)if(fxData[i])FreeMem(fxData[i],effects[i].length);
 if(score)FreeMem(score,scoreSize);if(bank)FreeMem(bank,bankSize);
 if(oldFilter)*(volatile UBYTE *)0xbfe001|=2;else *(volatile UBYTE *)0xbfe001&=~2;
 return result;
}
