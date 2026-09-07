"""Actual direct-start helper: visible loading for HD/WHDLoad/ADF and failures."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
s=(root/'src/main.c').read_text();a=s.index('static BOOL switchPreparedLevel1ToStormrail');b=s.index('\n}\n',a)+2
helper=s[a:b]
harness=r'''
#include <assert.h>
#include <string.h>
typedef int BOOL; typedef unsigned long ULONG;
#define TRUE 1
#define FALSE 0
#define GAME_START_LIVES 3
#define PLAYER_MAX_HEALTH 5
struct CampaignState {int postLevel1Lives,postLevel1Health,postLevel1Diamonds;};
static char events[64],fail;static int n;
static int event(char c){events[n++]=c;events[n]=0;return c!=fail;}
static int titleShowReplayLoading(void){return event('L');}
static void rendererCleanup(void){event('R');}
static void audioUnload(void){event('A');}
static void gameSetStormrailActive(int v){assert(v);event('G');}
static void assetsSetStormrailGameplay(int v){assert(v);event('S');}
static void campaignStartAtStormrail(struct CampaignState*c,int l,int h,int d){c->postLevel1Lives=l;c->postLevel1Health=h;c->postLevel1Diamonds=d;event('C');}
static void gameInit(ULONG seed){assert(seed==123);event('I');}
static void gameRestoreCampaignVitals(int l,int h,int d){assert(l==3&&h==5&&d==0);event('V');}
static int loadLevelFiles(void){return event('D');}
#ifdef SPARKPAW_MULTI_ADF
static int titleShowLevelCharging(void){return event('H');}
#endif
static int rendererPrepareGameplay(void){return event('P');}
static void titleFadeOut(void){event('F');}
'''
main=r'''
int main(void){struct CampaignState c;unsigned i;
#ifdef SPARKPAW_MULTI_ADF
 const char *expected="LRAGSCIVDHPF",*failures="LDHP";
#else
 const char *expected="LRAGSCIVDPF",*failures="LDP";
#endif
 assert(switchPreparedLevel1ToStormrail(&c,123));assert(!strcmp(events,expected));
 for(i=0;failures[i];i++){n=0;fail=failures[i];assert(!switchPreparedLevel1ToStormrail(&c,123));assert(!strchr(events,'F'));assert(events[n-1]==fail);}
 return 0;}
'''
with tempfile.TemporaryDirectory() as td:
 p=Path(td);(p/'test.c').write_text(harness+helper+main)
 for flag in (None,'SPARKPAW_WHDLOAD','SPARKPAW_MULTI_ADF'):
  subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',*(['-D'+flag] if flag else []),str(p/'test.c'),'-o',str(p/'test')],check=True)
  subprocess.run([str(p/'test')],check=True)
print('PASS: actual Stormrail OPTIONS helper loading/order/failure paths in HD, WHDLoad and ADF')
