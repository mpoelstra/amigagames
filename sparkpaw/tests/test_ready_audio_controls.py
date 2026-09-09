"""Run the actual title menu/control functions with scripted joystick and OS mocks.

Tests public behaviour/owner boundaries, not native interrupt timing.
"""
from pathlib import Path
import subprocess,tempfile
ROOT=Path(__file__).resolve().parents[1]
source=(ROOT/'src/title.c').read_text()
a=source.index('static void refreshReadyUI(void)')
b=source.index('\n#else\n#ifdef SPARKPAW_CAMPAIGN\nstatic UBYTE readyCampaignOptionsState',a)
source=source[a:b]
shim=r'''
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "ready_ui.h"
typedef unsigned char UBYTE;typedef unsigned short UWORD;typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define READY_HAS_SOUNDTEST 1
#define SPARKPAW_WHDLOAD
enum SecondaryButtonAction { SECONDARY_BUTTON_JUMP,SECONDARY_BUTTON_FIRE };
enum CampaignStartSection { CAMPAIGN_START_STORM_RUINS,CAMPAIGN_START_STORMRAIL };
enum AudioMode { AUDIO_FX_ONLY,AUDIO_MUSIC_ONLY,AUDIO_FX_MUSIC };
static struct ReadyUI storage,*readyUI=&storage;
static struct ReadySelection readySelection;
static BOOL readyUiDirty[2];static UBYTE readyDustState;
static struct { struct {unsigned char *Planes[6];} *bitmap;} asset;
static int input[1024],length,cursor,owned=1,cia,lsp=1,title=1,mode=2;
static int fxStarts,fxTime,heroLoads,neonLoads,stormLoads,restarts,copperLoads,railLoads,failLoad;
static int tempTrack,frames,quit,mutations;
static void *assetsLevelReadyMenu(void){return &asset;}
/* Avoid an untyped mock cast at the real -> access. */
#define assetsLevelReadyMenu() (&asset)
void readyUiCompose(struct ReadyUI *u,unsigned char **a,const struct ReadySelection *s,int hd)
{(void)u;(void)a;(void)s;assert(hd);mutations++;}
void readyDustSetMenuMask(const unsigned char *p){(void)p;}
static void renderReadyDustFrame(void){assert(owned);frames++;}
static void readReadyMenuInput(BOOL *u,BOOL *d,BOOL *l,BOOL *r,BOOL *f)
{
 int n;assert(cursor<length);n=input[cursor++];
 *u=(n&1)!=0;*d=(n&2)!=0;*l=(n&4)!=0;*r=(n&8)!=0;*f=(n&16)!=0;
 if(n&64)quit=1;
}
static BOOL platformWHDLoadQuitRequested(void){return quit;}
static void platformStopMenuPreview(void){assert(owned);cia=0;fxTime=0;}
static void musicSuspend(void){lsp=0;}
static void audioPreviewEffect(unsigned id){assert(owned&&!cia&&!lsp&&id<16);fxStarts++;fxTime=4;}
static void audioUpdate(void){if(fxTime)fxTime--;}
static BOOL audioPreviewEffectPlaying(void){return fxTime!=0;}
static BOOL musicAudible(void){return lsp;}
static void platformReleaseForLoading(BOOL keep){assert(owned&&!cia&&!lsp&&keep);owned=0;}
static void level1AudioPreviewClear(void){assert(!cia);tempTrack=0;}
static BOOL musicPlayIntro(void){assert(!owned&&!cia);heroLoads++;title=0;lsp=!failLoad;return !failLoad;}
static BOOL musicPlayGameOver(void){assert(!owned&&!cia);stormLoads++;title=0;lsp=!failLoad;return !failLoad;}
static BOOL musicRestartTitle(void){assert(!cia);if(!title)return FALSE;restarts++;lsp=1;return TRUE;}
static BOOL musicPlayTitle(void){assert(!owned&&!cia);neonLoads++;title=1;lsp=!failLoad;return !failLoad;}
static BOOL level1AudioPreviewPrepare(BOOL rail){assert(!owned&&!cia&&!lsp);if(rail)railLoads++;else copperLoads++;tempTrack=rail&&!failLoad;return !failLoad;}
static void platformResetGameInput(void){}
static void platformResumeMenuAfterLoading(void){assert(!owned&&!cia);owned=1;}
static BOOL platformStartMenuMusic(void){assert(owned&&!lsp&&!cia);cia=1;return TRUE;}
static enum AudioMode audioGetMode(void){return (enum AudioMode)mode;}
static void audioSetMode(enum AudioMode m){mode=m;}
'''
# Remove unused mock function (macro provides the typed expression).
shim=shim.replace('static void *assetsLevelReadyMenu(void){return &asset;}','')
checks=r'''
static void edge(int bits){input[length++]=bits;input[length++]=0;}
static void hold(int bits){int i;for(i=0;i<12;i++)input[length++]=bits;input[length++]=0;}
static void enter(void){input[length++]=0;edge(2);hold(16);edge(2);edge(2);edge(2);hold(16);}
static void leave(void){edge(2);edge(16);edge(2);edge(16);edge(16);}
int main(void){
 enum SecondaryButtonAction second=SECONDARY_BUTTON_JUMP;
 enum CampaignStartSection section=CAMPAIGN_START_STORM_RUINS;
 struct {unsigned char *Planes[6];} bitmap;
 int i;
 asset.bitmap=(void *)&bitmap;
 input[length++]=0;edge(2);hold(16); /* OPTIONS */
 edge(2);edge(2);edge(8);edge(8); /* mode MUSIC ONLY */
 edge(2);hold(16); /* SOUNDTEST; held opening Fire must not play */
 edge(16);edge(16);edge(8);edge(16); /* immediate retrigger + next effect */
 edge(2);edge(16);edge(16); /* Hero play/stop */
 edge(8);edge(16); /* Neon */
 edge(8);edge(16); /* Copper */
 edge(8);edge(16); /* Iron */
 edge(8);edge(16); /* Storm Light uses LSP, not CIA */
 edge(16);edge(16); /* stop and restart Storm Light */
 edge(8);edge(16); /* wrap to Hero */
 edge(4);edge(16); /* wrap backwards to Storm Light */
 leave();
 titleRunLevelReadyMenu(&second,&section);
 assert(fxStarts==3&&heroLoads==2&&neonLoads==2&&restarts==0&&stormLoads==3);
 assert(copperLoads==1&&railLoads==1&&mode==AUDIO_MUSIC_ONLY);
 assert(!cia&&!tempTrack&&owned&&lsp&&!quit);
 assert(second==SECONDARY_BUTTON_JUMP&&section==CAMPAIGN_START_STORM_RUINS);
 /* Failure paths still permit joystick return and clean gameplay entry. */
 for(i=0;i<5;i++){
  length=cursor=0;readySelection.track=i;failLoad=1;title=0;lsp=0;
  enter();edge(2);edge(16);leave();
  titleRunLevelReadyMenu(&second,&section);
  assert(owned&&!cia&&!tempTrack);
 }
 /* F10 while CIA preview runs: stop before returning to caller cleanup. */
 failLoad=0;title=1;lsp=1;length=cursor=0;readySelection.track=3;
 enter();edge(2);edge(16);edge(64);
 titleRunLevelReadyMenu(&second,&section);
 assert(quit&&owned&&!cia&&!lsp&&!tempTrack);
 assert(frames>100&&mutations>20);
 puts("PASS: actual menu, held Fire, 16-entry selection, all tracks, failure return, mode persistence and F10 quiescence");
 return 0;
}
'''
with tempfile.TemporaryDirectory() as td:
    p=Path(td);(p/'test.c').write_text(shim+source+checks)
    subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',
                    '-fsanitize=address,undefined','-I'+str(ROOT/'src'),str(p/'test.c'),
                    '-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],check=True)
