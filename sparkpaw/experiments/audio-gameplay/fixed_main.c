/* Reproducible direct Level-1 comparison; no presentation or timing fixes. */
#include <exec/types.h>
#include <exec/memory.h>
#include <hardware/custom.h>
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
#include "script.h"
#ifdef AUDIO_GAME_CANDIDATE
void audioGameWrite(BPTR f);
#else
void audioDiagnosticWrite(BPTR f);
#endif
unsigned long fixedStep;
#define MEASURE_BEGIN 520UL
static ULONG hash=0x53504157UL,frames,fields,zero,one,two,three,more,maximum;
static ULONG readiness[4][4],retryFrames,resets,bandSteps;
static LONG minimumCamera=0x7fffffffL,maximumCamera;
static volatile struct Custom *hw=(volatile struct Custom *)0xdff000;
static UWORD readLine(void)
{
 UWORD hi,lo,again;
 do{hi=hw->vposr;lo=hw->vhposr;again=hw->vposr;}while((hi&7)!=(again&7));
 return (UWORD)(((hi&7)<<8)|(lo>>8));
}
int main(void)
{
 BOOL owned=FALSE,published,armed=FALSE,complete=FALSE;
 ULONG last,now,delta,readyField,workDelta,attempts;
 ULONG chip=0,largest=0,fast=0;LONG previousFrame=0;UWORD line,bucket;
 BPTR f;int result=20;
 if(!platformOpen())return 20;
 PutStr("Fixed Level-1 audio comparison: automatic run, no controls needed.\n");
 gameInit(0x53504157UL);
 if(!rendererLoadGameplay()||!collisionLoad()||!audioLoad()||!rendererPrepareGameplay())goto done;
 chip=AvailMem(MEMF_CHIP);largest=AvailMem(MEMF_CHIP|MEMF_LARGEST);fast=AvailMem(MEMF_FAST);
 LoadView(NULL);WaitTOF();WaitTOF();platformBeginTakeover();
 platformFinishTakeover(rendererCopperList());owned=TRUE;
 rendererUpdateGameplay();platformSwitchCopper(rendererCopperList());last=platformFieldCounter();
 for(fixedStep=0;fixedStep<SCRIPT_STEPS;fixedStep++) {
  gameUpdate();
  rendererUpdateGameplay();rendererDrawGameplayBobs();
  /* Two raw observations only in measured window. They cannot themselves
     establish an exact deadline: TOD and beam phase differ. No clock changes. */
  readyField=0;line=0;
  if(fixedStep>=MEASURE_BEGIN){readyField=platformFieldCounter();line=readLine();}
  attempts=0;
  do {
   while(platformRasterLine()<300) { }
   while(platformRasterLine()>=300) { }
   attempts++;published=rendererPublishGameplay(platformRasterLine());
  }while(!published);
  now=platformFieldCounter();delta=(now-last)&0x00ffffffUL;
  if(fixedStep>=MEASURE_BEGIN) {
   frames++;fields+=delta;
   if(!delta)zero++;else if(delta==1)one++;else if(delta==2)two++;else if(delta==3)three++;else more++;
   if(delta>maximum)maximum=delta;
   if(attempts>1)retryFrames++;
   workDelta=(readyField-last)&0x00ffffffUL;if(workDelta>3)workDelta=3;
   bucket=line<=4?0:(line<=31?1:(line<300?2:3));readiness[workDelta][bucket]++;
   if(gameState()->cameraX<minimumCamera)minimumCamera=gameState()->cameraX;
   if(gameState()->cameraX>maximumCamera)maximumCamera=gameState()->cameraX;
  }
  last=now;
  /* Same lightweight observer in A/B, after publication. Hash selected
     gameplay fields, never real-time counters, pointers or renderer history. */
  hash=(hash<<5)|(hash>>27);
  hash^=(ULONG)playerState()->x^(ULONG)playerState()->y^(ULONG)gameState()->cameraX;
  hash^=gameState()->score^((ULONG)playerState()->health<<24)^(ULONG)gameState()->frameCounter;
  if(gameState()->frameCounter<previousFrame)resets++;
  previousFrame=gameState()->frameCounter;
  if(gameState()->cameraX>=750&&gameState()->cameraX<=1000)bandSteps++;
  if(!platformLeftMouse())armed=TRUE;
  if(armed&&platformLeftMouse())break;
 }
 complete=fixedStep==SCRIPT_STEPS;result=0;
done:
 if(owned)platformRestore();
 if(result==0) {
  f=Open("PROGDIR:renderdiag.log",MODE_NEWFILE);
  if(f) {
#ifdef AUDIO_GAME_CANDIDATE
   FPuts(f,"build=level1-fixed-v1-music-mixer\n");
#else
   FPuts(f,"build=level1-fixed-v1-original-sfx\n");
#endif
   FPrintf(f,"script=patrol-v1 complete=%ld planned_steps=1000 measure_begin=520 selected_state_hash=%ld resets=%ld band_750_1000_steps=%ld\n",(LONG)complete,hash,resets,bandSteps);
   FPrintf(f,"intervals=%ld fields=%ld zero=%ld one=%ld two=%ld three=%ld more=%ld max_fields=%ld publication_retry_frames=%ld\n",frames,fields,zero,one,two,three,more,maximum,retryFrames);
   FPrintf(f,"camera_min=%ld camera_max=%ld final_camera=%ld health=%ld prepared_chip_free=%ld prepared_chip_largest=%ld prepared_fast_free=%ld\n",minimumCamera,maximumCamera,gameState()->cameraX,(LONG)playerState()->health,chip,largest,fast);
   for(bucket=0;bucket<4;bucket++)FPrintf(f,"ready_tod_delta=%ld line_0_4=%ld line_5_31=%ld line_32_299=%ld line_300_plus=%ld\n",(LONG)bucket,readiness[bucket][0],readiness[bucket][1],readiness[bucket][2],readiness[bucket][3]);
#ifdef AUDIO_GAME_CANDIDATE
   audioGameWrite(f);
#else
   audioDiagnosticWrite(f);
#endif
   FPuts(f,"post_run=complete; script_complete is separate; raw TOD/beam readiness, delta3 bucket includes larger values; compare matched selected-state hashes and requests; no exact visible-deadline claim or CIA-B profiler\n");Close(f);
  }
 }else PutStr("Fixed audio comparison failed to prepare.\n");
 audioUnload();rendererCleanup();platformClose();return result;
}
