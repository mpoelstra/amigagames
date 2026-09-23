"""Exercise the actual section driver with ownership and failure-injection mocks."""
from pathlib import Path
import subprocess,tempfile
R=Path(__file__).resolve().parents[1]
source='\n'.join(l for l in (R/'src/drowned_campaign.c').read_text().splitlines() if not l.startswith('#include'))
shim=r'''
#include <assert.h>
#include <stdio.h>
#include "drowned_campaign.h"
#include "campaign_contract.h"
typedef int BOOL;
#define TRUE 1
#define FALSE 0
enum SecondaryButtonAction { JUMP,FIRE };
enum AudioMode { FX,MUSIC,BOTH };
struct GameState {unsigned long score,elapsedFields; unsigned enemiesDefeated,diamondsCollected;};
static struct GameState game;
static int fail,stage,locked,display,open,updates,reads,rounds,resets,prepared,loads,menus;
static int scenario,raster,secondary,mode,life,health,diamonds;
static int step(void){assert(!locked);return ++stage!=fail;}
static BOOL platformOpen(void){open=step();return open;}
static void platformReleaseForLoading(BOOL keep){locked=0;if(!keep)display=0;}
static void platformBeginTakeover(void){assert(!locked);display=0;}
static void WaitTOF(void){}
static void audioUnload(void){assert(!locked&&!display);}
static void rendererCleanup(void){assert(!locked&&!display);prepared=0;}
static void titleRelease(void){assert(display!=2);}
static void platformClose(void){assert(!locked&&!display);open=0;}
static void audioSetMode(enum AudioMode m){mode=m;}
static void gameInit(unsigned long seed){assert(seed==123);game=(struct GameState){0};updates=0;rounds++;}
static void gameRestoreDrownedVitals(unsigned char l,unsigned char h,unsigned char d){life=l;health=h;diamonds=d;}
static void playerSetSecondaryButtonAction(enum SecondaryButtonAction s){secondary=s;}
static BOOL titleShowSectionLoading(void){int ok=step();if(ok)display=2;return ok;}
static BOOL rendererLoadGameplay(void){loads++;return step();}
static BOOL collisionLoad(void){return step();}
static BOOL audioLoad(void){return step();}
static BOOL rendererPrepareGameplay(void){assert(life==2&&health==4&&diamonds==37);prepared=step();return prepared;}
static void titleFadeOut(void){}
static void platformResetGameInput(void){}
static int *rendererCopperList(void){static int c;return &c;}
static int *titleCopperList(void){static int c;return &c;}
static void platformFinishTakeover(int *c){assert(!locked);locked=1;display=c==rendererCopperList()?1:2;}
static void rendererUpdateGameplay(void){assert(locked&&prepared);}
static int platformRasterLine(void){assert(++raster<1000);return raster%3==1?310:0;}
static void platformSwitchCopper(int *c){assert(locked&&c==rendererCopperList());display=1;}
static void platformStartGameplayAudio(void){assert(locked&&prepared&&mode==2&&secondary==1);}
static BOOL gameOver(void){return scenario==2&&updates==2;}
static BOOL gameLevelComplete(void){return scenario!=2&&updates==2;}
static void platformReadGameKeys(BOOL *l,BOOL *r,BOOL *d,BOOL *j,BOOL *f){*l=*r=*d=*j=*f=0;assert(++reads<50);}
static BOOL platformWHDLoadQuitRequested(void){return scenario==5&&reads==1;}
static BOOL platformGameEscapeRequested(void){return scenario==1&&reads==1;}
static BOOL platformGamePauseToggleRequested(void){return scenario==3&&(reads==1||reads==3);}
static void rendererFadeOut(void){}
static void gameUpdate(void){assert(locked&&prepared);updates++;game.score=100;game.elapsedFields=65;game.enemiesDefeated=3;game.diamondsCollected=5;life=1;health=2;diamonds=42;}
static void rendererDrawGameplayBobs(void){assert(locked&&prepared);}
static BOOL rendererPublishGameplay(int line){(void)line;assert(locked);return TRUE;}
static const struct GameState *gameState(void){return &game;}
static BOOL titleShowGameOver(unsigned long score){assert(score==2100);assert(!prepared);if(!step())return FALSE;display=2;return TRUE;}
static BOOL titleShowLevelComplete(void){assert(prepared);if(!step())return FALSE;display=2;return TRUE;}
static void titleRunGameOver(void){assert(locked);menus++;}
static enum ResultDecision titleRunLevelCompleteMenu(unsigned e,unsigned d,unsigned long t,unsigned long s,BOOL last){assert(locked&&e==3&&d==5&&t==65&&s==100&&last);menus++;return scenario==4&&menus==1?RESULT_DECISION_REPLAY_CURRENT:RESULT_DECISION_BACK_TO_TITLE;}
static void rendererResetGameplay(void){assert(!locked&&!display);assert(life==2&&health==4&&diamonds==37&&!game.score);resets++;}
'''
checks=r'''
int main(void){
 const struct DrownedCampaignEntry e={2000,123,2,4,37,1,2};int s,f,result;
 assert(drownedCampaignRun(NULL)==DROWNED_CAMPAIGN_ERROR);
 for(s=0;s<6;s++)for(f=0;f<=7;f++){
  fail=f;stage=locked=display=open=updates=reads=rounds=resets=prepared=loads=menus=raster=0;scenario=s;
  result=drownedCampaignRun(&e);
  assert(!open&&!locked&&!display&&!prepared);
  if(f&&f<=stage)assert(result==DROWNED_CAMPAIGN_ERROR);
  else if(s==5)assert(result==DROWNED_CAMPAIGN_QUIT&&menus==0);
  else if(s==1)assert(result==DROWNED_CAMPAIGN_READY&&menus==0);
  else {assert(result==DROWNED_CAMPAIGN_TITLE);assert(menus==(s==4?2:1));assert(resets==(s==4));}
  assert(e.bankedScore==2000&&e.lives==2&&e.health==4&&e.diamonds==37);
 }
 puts("PASS: actual Drowned lifecycle, entry carry, replay, results, pause, Escape, WHDLoad F10, game over and seven failure boundaries");return 0;
}
'''
with tempfile.TemporaryDirectory() as td:
 p=Path(td);(p/'test.c').write_text(shim+source+checks)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Wno-unused-function','-Werror','-fsanitize=address,undefined','-DSPARKPAW_WHDLOAD','-I'+str(R/'src'),str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
