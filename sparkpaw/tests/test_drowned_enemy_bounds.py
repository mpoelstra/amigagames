"""Actual C pose selection/draw/restore against full-cell pixel oracle, two buffers.
Host DMA model checks masked source plane stride; does not prove raster timing.
"""
from pathlib import Path
import re, struct, subprocess, tempfile
R=Path(__file__).resolve().parents[1]
s=(R/'src/renderer.c').read_text()
def func(name):
    m=re.search(r'static (?:void|BOOL)\s+'+name+r'\([^)]*\)\s*\{',s); assert m,name
    end=m.end(); depth=1
    while depth:
        depth+=(s[end]=='{')-(s[end]=='}');end+=1
    return s[m.start():end]
# Normalize original SPBM sheets to the resident cache word layout.
fixtures=[]
for name,w,h,n,left in [('turbine-crab',32,24,22,True),('pump-walker',64,64,32,False),('spillwing',24,24,16,True)]:
    data=(R/f'build/drowned-full/assets/{name}.spbm').read_bytes()
    sw,sh,depth,masked,stride=struct.unpack_from('>HHBBH',data,4)
    assert depth==4 and masked and sw==2*w and sh==n*h
    start=12+3*(1<<depth); size=stride*sh; words=5 if w==64 else 3
    masks=[];bits=[]
    for facing in range(2):
        sx=(facing if left else 1-facing)*w
        for frame in range(n):
            m=[0]*(h*words);b=[0]*(4*h*words)
            for y in range(h):
                for x in range(w):
                    at=(frame*h+y)*stride+(sx+x)//8; bit=128>>((sx+x)&7)
                    pen=sum((1<<p) for p in range(4) if data[start+p*size+at]&bit)
                    if pen and data[start+4*size+at]&bit:
                        m[y*words+x//16]|=32768>>(x&15)
                        for p in range(4):
                            if pen&(1<<p):b[(p*h+y)*words+x//16]|=32768>>(x&15)
            masks+=m;bits+=b
    fixtures.append((w,h,n,words,masks,bits))
pre=r'''
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
typedef int16_t WORD;typedef uint16_t UWORD;typedef uint8_t UBYTE;typedef int32_t LONG;typedef uint32_t ULONG;typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define MAX_ENEMIES 4
#define ENEMY_TYPE_COUNT 3
#define ENEMY_TYPE_CLOCKWORK_BEETLE 0
#define ENEMY_TYPE_CLOCKWORK_STORM_STRIDER 1
#define FRONT_PLANES 4
#define WORLD_W 5120
#define WORLD_H 208
#define SCREEN_W 320
#define SPARKPAW_DROWNED_ENEMY_BOUNDS
#define SPARKPAW_DROWNED_FULL
#define SPARKPAW_DROWNED_JOINED
#define SPARKPAW_DROWNED_RESIDENT_WALKER
#define SPARKPAW_DROWNED_ENEMY_ART
#define SPARKPAW_ROLLING_PROTOTYPE
#define SPARKPAW_CANONICAL_BOB_RESTORE
#define BUSY_COUNT(a,b) ((void)0)
struct EnemyBobCache {UWORD *mask,*bits;WORD width,height,sourceWords;UBYTE frames;};
static struct EnemyBobCache enemyCaches[3];
struct Enemy {LONG x;WORD y,drawnX,drawnY;UBYTE type,drawnType,animFrame;BOOL active,drawn,facingLeft;};
static struct Enemy enemies[4];
static struct Enemy *enemyAt(int i){return &enemies[i];}
static struct {WORD cameraX;} gameData,*game=&gameData;
static UBYTE prototypePreparedCopper;
static WORD prototypeEnemyWorldX[4];
struct Hist {WORD x,y,worldX;UBYTE type;BOOL drawn;};
static struct PrototypeTarget {struct Hist enemy[4];} prototypeTarget[2];
#define PROTOTYPE_RING_W 512
#include "drowned_ring_layout.h"
#define WIDTH (512*DROWNED_RING_COPIES)
static UWORD screen[2][4][208][WIDTH/16],clean[4][208][WIDTH/16],oracle[4][208][WIDTH/16];
struct BitMap {WORD BytesPerRow;UBYTE *Planes[4];};
static struct BitMap bitmap[2],*frontDisplay;
static struct {UWORD bltcon0,bltcon1,bltafwm,bltalwm,bltamod,bltbmod,bltcmod,bltdmod,bltsize;void *bltapt,*bltbpt,*bltcpt,*bltdpt;} regs,*hw=&regs;
static void platformWaitBlit(void){
 unsigned words=hw->bltsize&63,rows=hw->bltsize>>6,shift=hw->bltcon0>>12;
 if(!hw->bltsize)return;
 assert((hw->bltcon0&4095)==0xfca);assert(hw->bltafwm==65535&&hw->bltalwm==65535);
 for(unsigned y=0;y<rows;y++){
  UWORD *a=(UWORD*)((UBYTE*)hw->bltapt+y*(words*2+hw->bltamod));
  UWORD *b=(UWORD*)((UBYTE*)hw->bltbpt+y*(words*2+hw->bltbmod));
  UWORD *c=(UWORD*)((UBYTE*)hw->bltcpt+y*(words*2+hw->bltcmod));
  UWORD *d=(UWORD*)((UBYTE*)hw->bltdpt+y*(words*2+hw->bltdmod));
  unsigned pa=0,pb=0;
  for(unsigned x=0;x<words;x++){
   unsigned aa=a[x],bb=b[x],ma=(aa>>shift)|(shift?pa<<(16-shift):0),mb=(bb>>shift)|(shift?pb<<(16-shift):0);
   d[x]=(UWORD)((ma&mb)|(~ma&c[x]));pa=aa;pb=bb;
  }
 }
 hw->bltsize=0;
}
static WORD prototypePhysicalX(WORD x){return DROWNED_RING_BASE+(game->cameraX&511)+x-game->cameraX;}
static BOOL prototypeRectFits(WORD x,WORD w){WORD p=prototypePhysicalX(x);return p>=0&&p+w+16<WIDTH;}
static void blitRestoreRect(WORD worldX,WORD x,WORD y,WORD w,WORD h){
 (void)worldX;platformWaitBlit();assert(h>0&&y>=0&&y+h<=208&&x>=0&&x+w<=WIDTH);
 for(int p=0;p<4;p++)for(int yy=y;yy<y+h;yy++)for(int xx=x;xx<x+w;xx++){
  UWORD bit=32768>>(xx&15),*d=&screen[prototypePreparedCopper][p][yy][xx/16];
  *d=(*d&~bit)|(clean[p][yy][xx/16]&bit);
 }
}
#include "drowned_enemy_frames.h"
#include "enemy_vertical_order.h"
#include "strider_restore_union.h"
'''
# Extract actual helpers including preprocessor branches; truncate unrelated history.
chunk=s[s.index('#if defined(SPARKPAW_STORMRAIL_PROOF) || defined(SPARKPAW_DROWNED_ENEMY_BOUNDS)',s.index('static void blitRestoreRect')):s.index('static BOOL buildDiamondPattern')]
body=pre+chunk+'\n'+func('stageStriderFrame')+'\n'+func('restoreEnemyBob')+'\n'+func('drawEnemyBob')
for name in ['prototypeLoadHistory','prototypeSaveHistory']:
    body+='\n'+func(name).split('    for(i=0;i<MAX_PROJECTILES;i++)')[0]+'}\n'
body+=r'''
static void check(void){
 platformWaitBlit();memcpy(oracle,clean,sizeof clean);
 WORD y[4];UBYTE order[4];for(int i=0;i<4;i++)y[i]=enemies[i].active?enemies[i].y+(enemies[i].type==1?2:0):32767;
 enemyVerticalOrder(y,order,4);
 for(int k=0;k<4;k++){
  struct Enemy *e=&enemies[order[k]];if(!e->drawn)continue;
  struct EnemyBobCache *c=&enemyCaches[e->type];int pattern=(!e->facingLeft)*c->frames+e->animFrame;
  UWORD *m=c->mask+pattern*c->height*c->sourceWords,*b=c->bits+pattern*4*c->height*c->sourceWords;
  for(int yy=0;yy<c->height;yy++)for(int xx=0;xx<c->width;xx++)if(m[yy*c->sourceWords+xx/16]&(32768>>(xx&15))){
   int dy=e->drawnY+yy,dx=e->drawnX+xx;
   for(int p=0;p<4;p++){
    UWORD *d=&oracle[p][dy][dx/16],bit=32768>>(dx&15);
    *d=(*d&~bit)|((b[(p*c->height+yy)*c->sourceWords+xx/16]&(32768>>(xx&15)))?bit:0);
   }
  }
 }
 assert(!memcmp(oracle,screen[prototypePreparedCopper],sizeof oracle));
}
int main(int argc,char **argv){
 assert(argc==2);FILE *f=fopen(argv[1],"rb");assert(f);
 int widths[]={32,64,24},heights[]={24,64,24},frames[]={22,32,16},words[]={3,5,3};
 for(int t=0;t<3;t++){
  struct EnemyBobCache *c=&enemyCaches[t];c->width=widths[t];c->height=heights[t];c->frames=frames[t];c->sourceWords=words[t];
  int n=2*c->frames*c->height*c->sourceWords;c->mask=calloc(n,2);c->bits=calloc(n*4,2);
  for(int kind=0;kind<2;kind++)for(int j=0;j<n*(kind?4:1);j++){
   int hi=fgetc(f),lo=fgetc(f);assert(hi>=0&&lo>=0);(kind?c->bits:c->mask)[j]=(hi<<8)|lo;
  }
  assert(prepareDrownedEnemyFrames(c));
 }
 fclose(f);
 for(int p=0;p<4;p++)for(int y=0;y<208;y++)for(int x=0;x<WIDTH/16;x++)clean[p][y][x]=(UWORD)(p*173+y*137+x*113);
 for(int b=0;b<2;b++){
  memcpy(screen[b],clean,sizeof clean);bitmap[b].BytesPerRow=WIDTH/8;
  for(int p=0;p<4;p++)bitmap[b].Planes[p]=(UBYTE*)screen[b][p];
 }
 /* Every real pose/facing at every pixel alignment, plus alternating-buffer
    overlap, reused slots/types, despawn, offscreen and checkpoint-style clears. */
 for(int tick=0;tick<4096;tick++){
  prototypePreparedCopper=tick&1;frontDisplay=&bitmap[tick&1];prototypeLoadHistory(tick&1);restoreEnemyBob();
  assert(!memcmp(screen[tick&1],clean,sizeof clean));
  if(tick%97==0){memset(prototypeTarget,0,sizeof prototypeTarget);for(int b=0;b<2;b++)memcpy(screen[b],clean,sizeof clean);}
  game->cameraX=(tick/128%8)*512;
  for(int i=0;i<4;i++){
   struct Enemy *e=&enemies[i];e->type=(tick/1024+i)%3;
   e->animFrame=(tick/16+i)%enemyCaches[e->type].frames;e->facingLeft=(tick/512+i)&1;
   e->x=(LONG)(game->cameraX+80+i*12+(tick&15))*256;e->y=20+i*5+(tick/64%40);
   e->active=(tick%19!=i);e->drawn=0;
   if(tick%37==0)e->y=-1;
   if(tick%41==0)e->y=207;
   if(tick%43==0)e->x=(LONG)(game->cameraX+400)*256;
  }
  drawEnemyBob();check();prototypeSaveHistory(tick&1);
 }
 /* Entirely transparent frame still has a valid full-height restore. */
 UWORD blank[64*5]={0};assert(drownedMaskBounds(blank,64,5)==64);
 for(int t=0;t<3;t++){free(enemyCaches[t].mask);free(enemyCaches[t].bits);}
 puts("PASS: actual C draw/restore/history, 4096 two-buffer scenes, real 140 poses, 16 shifts, overlaps/reuse/culling/reset; ASan/UBSan");
}
'''
with tempfile.TemporaryDirectory() as td:
    p=Path(td);(p/'t.c').write_text(body)
    with (p/'cache.bin').open('wb') as f:
        for w,h,n,words,m,b in fixtures:
            for a in [m,b]:f.write(struct.pack('>'+str(len(a))+'H',*a))
    for flags in [[], ['-DSPARKPAW_DROWNED_TWO_COPY_RING']]:
        subprocess.run(['cc','-O2','-std=c99','-fsanitize=address,undefined',*flags,'-I'+str(R/'src'),str(p/'t.c'),'-o',str(p/'t')],check=True)
        subprocess.run([str(p/'t'),str(p/'cache.bin')],check=True)
