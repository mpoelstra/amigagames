/* Direct Level-1 audition: production update/render sequence, no campaign flow. */
#include <exec/types.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include "audio.h"
#include "collision.h"
#include "game.h"
#include "player.h"
#include "renderer.h"
#include "platform_amiga.h"
#ifdef AUDIO_GAME_TRACE
#include "trace.h"
#endif
#ifdef AUDIO_GAME_CANDIDATE
void audioGameWrite(BPTR f);
#else
void audioDiagnosticWrite(BPTR f);
#endif
int main(void)
{
 BOOL opened=FALSE,owned=FALSE,published,armed=FALSE;
 ULONG attempts=0;
 ULONG last=0,now,delta,intervals=0,fields=0,one=0,two=0,three=0,over=0,maximum=0;
 ULONG chip=0,fast=0,largest=0;BPTR f;int result=20;
 if(!platformOpen())return 20;opened=TRUE;
 PutStr("Level-1 audio candidate: LMB or Escape stops, saves and returns.\n");
 gameInit(0x53504157UL);
 if(!rendererLoadGameplay()||!collisionLoad()||!audioLoad()||!rendererPrepareGameplay())goto done;
 chip=AvailMem(MEMF_CHIP);fast=AvailMem(MEMF_FAST);largest=AvailMem(MEMF_CHIP|MEMF_LARGEST);
 playerSetSecondaryButtonAction(SECONDARY_BUTTON_JUMP);
 LoadView(NULL);WaitTOF();WaitTOF();
 platformBeginTakeover();platformFinishTakeover(rendererCopperList());owned=TRUE;
 rendererUpdateGameplay();platformSwitchCopper(rendererCopperList());
 last=platformFieldCounter();
 for(;;) {
  /* Same non-diagnostic rolling-renderer sequence as production main.c. */
#ifdef AUDIO_GAME_TRACE
  traceBegin();
#endif
  gameUpdate();
#ifdef AUDIO_GAME_TRACE
  tracePhase(1);
#endif
  rendererUpdateGameplay();
  rendererDrawGameplayBobs();
#ifdef AUDIO_GAME_TRACE
  tracePhase(2);
#endif
  attempts=0;
  do {
   while(platformRasterLine()<300) { }
   while(platformRasterLine()>=300) { }
   attempts++;
   published=rendererPublishGameplay(platformRasterLine());
  }while(!published);
#ifdef AUDIO_GAME_TRACE
  tracePhase(3);
#endif
  now=platformFieldCounter();delta=(now-last)&0x00ffffffUL;last=now;
#ifdef AUDIO_GAME_TRACE
  traceEnd(delta,attempts);
#endif
  intervals++;fields+=delta;if(delta>maximum)maximum=delta;
  if(delta==1)one++;else if(delta==2)two++;else if(delta==3)three++;else over++;
  if(!platformLeftMouse())armed=TRUE;
  if((armed&&platformLeftMouse())||platformGameEscapeRequested())break;
  /* Full campaign/results are deliberately outside this first ownership gate.
     Keep playing the Core scene until user saves, allowing its cue to finish. */
 }
 result=0;
done:
 if(owned)platformRestore();
 if(result==0) {
  f=Open("PROGDIR:renderdiag.log",MODE_NEWFILE);
  if(f) {
#ifdef AUDIO_GAME_CANDIDATE
   FPuts(f,"build=level1-audio-v1-music-mixer\n");
#else
   FPuts(f,"build=level1-audio-v1-original-sfx\n");
#endif
   FPrintf(f,"intervals=%ld fields=%ld one=%ld two=%ld three=%ld other=%ld max_fields=%ld\n",intervals,fields,one,two,three,over,maximum);
   FPrintf(f,"prepared_chip_free=%ld prepared_chip_largest=%ld prepared_fast_free=%ld camera_x=%ld\n",chip,largest,fast,gameState()->cameraX);
#ifdef AUDIO_GAME_CANDIDATE
   audioGameWrite(f);
#else
   audioDiagnosticWrite(f);
#endif
#ifdef AUDIO_GAME_TRACE
   FPuts(f,"instrumentation=phase-trace-v1\n");
   traceWrite(f);
#endif
   FPuts(f,"post_run=complete; CIA-A TOD cadence only; manual workload; no CIA-B profiler; results/campaign transitions not tested\n");
   Close(f);
  }
 } else PutStr("Level-1 audio proof failed to load/prepare; no gameplay started.\n");
 audioUnload();rendererCleanup();if(opened)platformClose();
 return result;
}
