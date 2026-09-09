"""Execute actual HUD initialization/health selection with carried campaign vitals."""
from pathlib import Path
import subprocess,tempfile
R=Path(__file__).resolve().parents[1]
s=(R/'src/hud.c').read_text();start=s.index('BOOL hudPrepare(void)');end=s.index('\nvoid hudRelease',start);prepare=s[start:end]
r=(R/'src/renderer.c').read_text();start=r.index('static void setHudPointers(void)');sel=r[start:r.index('    const struct BitMap *display;',start)].replace('static void setHudPointers(void)','static UBYTE displayedHealth(void)')+'    return health;\n}\n'
shim=r'''
#include <assert.h>
#include <stdlib.h>
typedef unsigned char UBYTE; typedef unsigned short UWORD; typedef unsigned long ULONG; typedef long LONG; typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define SPARKPAW_STORMRAIL_PROOF
#define PLAYER_MAX_HEALTH 6
#define HUD_BUFFERS 2
#define HUD_PLANES 3
#define HUD_W 352
#define HUD_ASSET_W 336
#define HUD_H 48
#define HEALTH_W 48
#define HEALTH_H 16
#define HEALTH_STATES 7
#define LIVES_W 32
#define LIVES_H 16
#define GAME_MAX_LIVES 9
#define DIAMONDS_W 16
#define DIAMONDS_H 16
#define DIAMOND_STATES 10
#define SCORE_DIGIT_W 8
#define SCORE_DIGIT_H 8
#define SCORE_DIGITS 4
#define BMF_CLEAR 0
#define BMF_DISPLAYABLE 0
#define MEMF_CHIP 0
#define MEMF_CLEAR 0
struct GameState {UBYTE lives,diamonds,stormrailActive,stormrailHealth,stormrailMode;ULONG score;};
struct PlayerState {UBYTE health;};
static struct GameState state,*game=&state;static struct PlayerState player;
static const struct GameState *gameState(void){return &state;}
static const struct PlayerState *playerState(void){return &player;}
struct Asset {int depth,width,height;};
static struct Asset a[5]={{3,336,48},{3,48,112},{3,32,144},{3,16,160},{3,8,320}};
#define assetsHudBase() (&a[0])
#define assetsHudHealth() (&a[1])
#define assetsHudLives() (&a[2])
#define assetsHudDiamonds() (&a[3])
#define assetsHudScore() (&a[4])
static struct Asset *base,*healthAtlas,*livesAtlas,*diamondsAtlas,*scoreAtlas;
struct BitMap {UWORD BytesPerRow;};static struct BitMap bm[2],*buffers[2];
static int allocated,calls;static UWORD hudStride;static UBYTE *blankPlane,current;
static struct BitMap *AllocBitMap(int w,int h,int d,int f,void *p){(void)h;(void)d;(void)f;(void)p;bm[allocated].BytesPerRow=w/8;return &bm[allocated++];}
#define AllocMem(n,f) malloc(n)
static void composeCpu(UBYTE i,UBYTE h,UBYTE l,UBYTE d,ULONG score){assert(i==calls++);assert(h==(state.stormrailActive?state.stormrailHealth:player.health));assert(l==state.lives&&d==state.diamonds&&score==state.score);}
'''
main=r'''
int main(void){int mode,h;
 for(mode=0;mode<8;mode++)for(h=1;h<=6;h++){
  state.stormrailActive=1;state.stormrailMode=mode;state.stormrailHealth=h;
  state.lives=2;state.diamonds=7;state.score=450;player.health=6;
  allocated=calls=0;assert(hudPrepare());assert(calls==2&&current==0);
  assert(displayedHealth()==h);free(blankPlane);
 }
 /* OPTIONS -> Stormrail: fresh run, not an old carried snapshot. */
 state.stormrailActive=1;state.stormrailMode=0;state.lives=3;
 state.stormrailHealth=6;state.diamonds=0;state.score=0;player.health=6;
 allocated=calls=0;assert(hudPrepare());assert(displayedHealth()==6);free(blankPlane);
 /* Replaying a carried section starts with its saved vitals again. */
 state.lives=2;state.stormrailHealth=3;state.stormrailMode=0;
 allocated=calls=0;assert(hudPrepare());assert(displayedHealth()==3);free(blankPlane);
 state.stormrailActive=0;state.lives=3;player.health=4;
 allocated=calls=0;assert(hudPrepare());assert(displayedHealth()==4);free(blankPlane);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'test.c').write_text(shim+prepare+sel+main)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-fsanitize=address,undefined',str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
print('PASS: both initial HUD buffers carry lives/health/diamonds/score; approach/boarding/fades/flight select campaign health; Level 1 keeps player health')
