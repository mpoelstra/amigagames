"""Execute the terminal-life and enemy-hit functions from the production source."""
from pathlib import Path
import subprocess,tempfile
ROOT=Path(__file__).resolve().parents[1]
s=(ROOT/'src/game.c').read_text()
def fn(sig):
 start=s.index(sig);a=s.index('{',start);depth=1;b=a+1
 while depth:
  depth+=(s[b]=='{')-(s[b]=='}');b+=1
 return s[start:b]
code='''
#include <assert.h>
typedef int BOOL; typedef short WORD;
#define TRUE 1
#define FALSE 0
static struct { unsigned char lives,waterSplashTimer; unsigned long score; } game;
static struct { int health; } player;
static int resets,hits=1;
static void resetLevelRuntime(void){resets++;}
static int playerTakeEnemyHit(WORD x){(void)x;return hits;}
static void audioPlayPlayerHurt(void){}
static void audioUpdate(void){}
#define playerState() (&player)
'''+fn('static BOOL loseLife(void)')+fn('BOOL gameOver(void)')+fn('static BOOL applyEnemyDamage(WORD sourceCenterX)')+'''
int main(void){
 game.lives=3;game.score=12450;player.health=0;
 assert(applyEnemyDamage(10)&&game.lives==2&&resets==1&&!gameOver());
 assert(applyEnemyDamage(10)&&game.lives==1&&resets==2&&!gameOver());
 assert(applyEnemyDamage(10)&&game.lives==0&&resets==2&&gameOver());
 assert(!loseLife()&&game.lives==0&&game.score==12450);
 game.waterSplashTimer=16;assert(!gameOver());
 while(game.waterSplashTimer) game.waterSplashTimer--;
 assert(gameOver());
 game.lives=3;player.health=1;assert(!applyEnemyDamage(10)&&game.lives==3);
 hits=0;assert(!applyEnemyDamage(10)&&game.lives==3);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'test.c').write_text(code)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
assert s.count('if(!loseLife()) return;')==2 # flight and finale
assert 'if(gameOver()) return;' in s
assert 'if(!--game.waterSplashTimer&&game.lives)' in s
main=(ROOT/'src/main.c').read_text()
assert main.index('if(gameOver()) {')<main.index('if(gameLevelComplete())')
assert 'campaign.bankedScore+' in main
print('PASS: terminal life, repeated zero, score preservation, enemy damage and delayed splash; flight/finale route checks')

# Run the actual hidden-Copper fade: every instruction/address survives;
# colours (both AGA nibble banks) reach zero and original lists are not reused.
s=(ROOT/'src/renderer.c').read_text()
fade=fn('void rendererFadeOut(void)')
code='''
#include <assert.h>
#include <string.h>
typedef unsigned short UWORD; typedef unsigned char UBYTE;
#define SPARKPAW_ROLLING_PROTOTYPE
#define COP_WORDS 768
#define CopyMem(a,b,n) memcpy(b,a,n)
static UWORD a[COP_WORDS],b[COP_WORDS];
static UWORD *prototypeCopper[2]={a,b};
static UBYTE prototypeActiveCopper;
static int switches,raster;
static int platformRasterLine(void){raster^=1;return raster?300:0;}
static void platformSwitchCopper(UWORD *p){
 assert(p!=prototypeCopper[prototypeActiveCopper]);
 assert(p[0]==0x106&&p[1]==0x200); /* BPLCON3 survives */
 assert(p[2]==0x180&&p[4]==0x1be&&p[6]==0x120);
 assert(p[7]==0xabcd&&p[8]==0xffff&&p[9]==0xfffe);
 switches++;
}
'''+fade+'''
int main(void){
 a[0]=0x106;a[1]=0x200;a[2]=0x180;a[3]=0xfed;
 a[4]=0x1be;a[5]=0x321;a[6]=0x120;a[7]=0xabcd;
 a[8]=0xffff;a[9]=0xfffe;
 rendererFadeOut();
 assert(switches==24&&prototypeActiveCopper==0&&a[3]==0&&a[5]==0);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'fade.c').write_text(code)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',str(p/'fade.c'),'-o',str(p/'fade')],check=True)
 subprocess.run([str(p/'fade')],check=True)
print('PASS: actual Copper fade preserves control/pointers, alternates hidden lists and blacks all palette writes')
