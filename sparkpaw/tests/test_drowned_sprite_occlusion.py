"""Check near-post clipping against pixels and execute real sprite staging."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
renderer = (ROOT / 'src/renderer.c').read_text()
body = renderer.split('static void setHardwareSprite(void)', 1)[1].split(
    'static UBYTE playerPlasmaPatternPen', 1)[0]
source = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
typedef uint8_t UBYTE; typedef uint16_t UWORD; typedef int16_t WORD;
typedef uint32_t ULONG; typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define SPARKPAW_DROWNED_SLICE
#define SPARKPAW_AGA64_PLAYER_SPRITE
#define SPARKPAW_SPRITE_STAGE_CACHE
#define SPRITE_W 48
#define SPRITE_H 48
#define PLAYER_W 32
#define PLAYER_H 40
#define SPRITE_CHANNELS 2
#define SPRITE_WORDS 400
#define CopyMem(s,d,n) memcpy(d,s,n)
#include "drowned_sprite_occlusion.h"
struct PlayerState { int x,y,facingLeft,animFrame,invulnTimer; } player;
static const struct PlayerState *playerState(void){return &player;}
struct {int cameraX,waterSplashTimer;} gs,*game=&gs;
struct Cache {int facing,frame;} hwSpriteStageCache[2];
#define SPRITE_STAGE_CACHE_NEEDS_COPY(c,f,a) ((c)->facing!=(f)||(c)->frame!=(a))
#define SPRITE_STAGE_CACHE_COMMIT(c,f,a) ((c)->facing=(f),(c)->frame=(a))
UWORD masters[2][2][2][400],stage[2][2][400],nullData[16];
UWORD *hwSprites[2][2][2],*hwSpriteStage[2][2],*nullSprite=nullData;
UWORD cop[16],spritePtrValue[2]={0,4};
UBYTE hwSpriteStageIndex;
ULONG drownedPostMask[88]; BOOL drownedSpriteMasked[2];
static void setHardwareSprite(void)
''' + body + r'''
static void checkPixels(UWORD *out,const UWORD *master,int x,int y,int masked){
 int row,chunk,plane,bit;
 for(row=0;row<48;row++)for(chunk=0;chunk<4;chunk++)for(plane=0;plane<2;plane++){
  UWORD want=master[8+row*8+chunk+plane*4];
  if(masked)for(bit=0;bit<16;bit++){
   int px=x+chunk*16+bit-834,py=y+row-112;
   if(chunk<3&&px>=0&&px<18&&py>=0&&py<88&&
      (drownedPostMask[py]&(0x80000000u>>px)))want&=~(0x8000u>>bit);
  }
  assert(out[8+row*8+chunk+plane*4]==want);
 }
 assert(!memcmp(out+392,master+392,16));
}
int main(void){
 int x,y,s,c,f,a,i,pass; UWORD out[2][400],before[400];
 for(i=0;i<88;i++)drownedPostMask[i]=(i%3==0?0x93ab8000u:0xffffc000u);
 for(i=0;i<400;i++)before[i]=(UWORD)(i*391+17);
 for(x=760;x<900;x++)for(y=60;y<210;y++){
  memcpy(out[0],before,800);memcpy(out[1],before,800);
  drownedMaskSprite(out[0],out[1],drownedPostMask,x,y);
  for(c=0;c<2;c++){
   checkPixels(out[c],before,x,y,1);assert(!memcmp(out[c],before,16));
  }
 }
 for(f=0;f<2;f++)for(a=0;a<2;a++)for(c=0;c<2;c++){
  hwSprites[f][a][c]=masters[f][a][c];
  for(i=0;i<400;i++)masters[f][a][c][i]=(UWORD)(i*391+17+f*11+a*7+c*3);
 }
 for(s=0;s<2;s++)for(c=0;c<2;c++){
  hwSpriteStage[s][c]=stage[s][c];memcpy(stage[s][c],masters[0][0][c],800);
 }
 /* Repeated poses, both directions, scrolling, blink/splash and leaving a
    clipped stage: real cache and inactive-stage code must restore all bits. */
 for(pass=0;pass<4;pass++)for(i=0;i<180;i++){
  UWORD previous[2][400];int old=hwSpriteStageIndex;
  memcpy(previous,stage[old],1600);
  x=pass&1?900-i:730+i; y=pass<2?152:100+i%49;
  player.x=(x+8)*256;player.y=(y+8)*256;
  player.facingLeft=pass&1;player.animFrame=(i/31)&1;
  player.invulnTimer=i%11==0?1:0;gs.waterSplashTimer=i%17==0;
  gs.cameraX=i*3;
  setHardwareSprite();
  assert(!memcmp(previous,stage[old],1600));
  if(player.invulnTimer||gs.waterSplashTimer){assert(old==hwSpriteStageIndex);continue;}
  for(c=0;c<2;c++)checkPixels(stage[hwSpriteStageIndex][c],
   masters[player.facingLeft][player.animFrame][c],x,y,1);
 }
 for(f=0;f<2;f++)for(a=0;a<2;a++)for(c=0;c<2;c++)for(i=0;i<400;i++)
  assert(masters[f][a][c][i]==(UWORD)(i*391+17+f*11+a*7+c*3));
 return 0;
}
'''
gov=source.replace('#define SPARKPAW_DROWNED_SLICE','#define SPARKPAW_DROWNED_GOVERNOR\n#define SPARKPAW_DROWNED_SLICE')
gov=gov.replace('ULONG drownedPostMask[88];','#include "drowned_shrub_mask.h"\nBOOL stagingChecks; ULONG drownedPostMask[88];')
gov=gov.replace('-834','-1346').replace('x=760;x<900','x=1272;x<1530').replace('900-i:730+i','1530-i:1290+i').replace('gs.cameraX=i*3','gs.cameraX=x-140')
gov=gov.replace('for(pass=0;pass<4;pass++)for(i=0;i<180;i++)','stagingChecks=1; for(pass=0;pass<4;pass++)for(i=0;i<280;i++)')
gov=gov.replace('1530-i:1290+i','1570-i:1290+i')
gov=gov.replace('assert(out[8+row*8+chunk+plane*4]==want);', '''
  if(masked&&stagingChecks&&chunk<3)for(bit=0;bit<16;bit++){
   int px=x+chunk*16+bit-1504,py=y+row-181;
   if(px>=0&&px<32&&py>=0&&py<20&&
      (drownedShrubMask[py]&(0x80000000u>>px)))want&=~(0x8000u>>bit);
  }
  assert(out[8+row*8+chunk+plane*4]==want);''')
full=source.replace('#define SPARKPAW_DROWNED_SLICE','#define SPARKPAW_DROWNED_FULL\n#define SPARKPAW_DROWNED_GOVERNOR\n#define SPARKPAW_DROWNED_SLICE')
full=full.replace('#include "drowned_sprite_occlusion.h"','#include "drowned_sprite_occlusion.h"\n#include "drowned_full_masks.h"\n#include "drowned_full_occlusion.h"')
a=full.index('   int px=x+chunk*16+bit-834')
b=full.index('  }\n  assert(out',a)
full=full[:a]+'''
   int oi;
   for(oi=0;oi<sizeof(drownedOccluders)/sizeof(drownedOccluders[0]);oi++){
    const struct DrownedOccluder *o=&drownedOccluders[oi];
    int px=x+chunk*16+bit-o->x,py=y+row-o->y;
    if(chunk<3&&px>=0&&px<o->w&&py>=0&&py<o->h&&
       (o->bits[py]&(0x80000000u>>px)))want&=~(0x8000u>>bit);
   }
'''+full[b:]
a=full.index(' for(x=760;x<900')
b=full.index(' for(f=0;f<2;',a)
full=full[:a]+full[b:]
full=full.replace('i<180','i<5120').replace('900-i:730+i','5119-i:i').replace('gs.cameraX=i*3','gs.cameraX=x-140')
with tempfile.TemporaryDirectory() as tmp:
    c = Path(tmp) / 'proof.c'
    exe = Path(tmp) / 'proof'
    for variant in (source,gov,full):
        c.write_text(variant)
        subprocess.run(['cc', '-O2', '-std=c99', '-fsanitize=address,undefined',
                        '-Wno-pointer-to-int-cast', '-I', str(ROOT / 'src'), '-I', str(ROOT / 'build/drowned-governor'), '-I', str(ROOT / 'build/drowned-full'),
                        str(c), '-o', str(exe)], check=True)
        subprocess.run([str(exe)], check=True)
print('Drowned sprite occlusion: both post configurations, pixel reference, inactive stage, cache, blink and masters PASS')
