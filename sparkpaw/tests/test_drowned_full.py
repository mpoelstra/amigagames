"""Actual full-world governor coordinates, occlusion pixels and asset bounds."""
from pathlib import Path
import json,sys,tempfile,subprocess
R=Path(__file__).resolve().parents[1];sys.path.insert(0,str(R/'tools'))
from preview_drowned_native import decode
O=R/'build/drowned-full';m=json.loads((O/'manifest.json').read_text())
f=decode(O/'assets/drowned-route.spbm');g=decode(R/'build/drowned-governor/assets/drowned-route.spbm')
assert f.size==(5120,208)
assert f.crop((3248+1408,0,5120,208)).tobytes()==g.crop((1408,0,1872,208)).tobytes()
old=(R/'build/drowned-joined/assets/drowned-route.bin').read_bytes();new=(O/'assets/drowned-route.bin').read_bytes()
for y in range(14):assert old[y*220:y*220+203]==new[y*320:y*320+203]
assert len(m['spawns'])==19 and len(m['surfaces'])==20
for x,w,h,species,flip in m['trees']:
 assert not any(x<wx+80 and x+w>wx for wx in m['water'])
rear=decode(O/'assets/drowned-rear.spbm');assert rear.width>=1520
source=(R/'src/drowned_governor.c').read_text().replace('#include "drowned_governor.h"','').replace('#include "projectiles.h"','')
route=(R/'src/drowned_slice.c').read_text()
def fn(name):
 a=route.index(name);a=route.rfind('\n',0,a)+1;b=route.index('{',a);d=1;e=b+1
 while d:d+=(route[e]=='{')-(route[e]=='}');e+=1
 return route[a:e]
mechanisms='static UWORD pressureTick,gateTick;static BOOL panelVisible;\n'
for name in ['void drownedReset(', 'void drownedRespawn(', 'void drownedSetView(', 'void drownedTick(', 'UBYTE drownedJetFrame(', 'UBYTE drownedGateFrame(', 'BOOL drownedGateSolid(', 'BOOL drownedHeaderOverlaps(', 'BOOL drownedJetTouches(', 'UBYTE drownedPanelHit(']:
 mechanisms+=fn(name)+'\n'
c=r"""
#include <stdint.h>
#include <assert.h>
#include <string.h>
typedef int16_t WORD;typedef uint16_t UWORD;typedef uint32_t ULONG;typedef uint8_t UBYTE;typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define DROWNED_FINALE_OFFSET 3248
#define SPARKPAW_DROWNED_FULL
#define SPARKPAW_DROWNED_GOVERNOR
#define SPARKPAW_DROWNED_JOINED
#define SPARKPAW_DROWNED_ROUTE
#define DROWNED_HEADER_LEFT 800
#define DROWNED_HEADER_RIGHT 832
#define DROWNED_HEADER_TOP 120
#define DROWNED_HEADER_BOTTOM 136
#define PROJECTILE_ENEMY_MISS 0
#define PROJECTILE_ENEMY_HIT 1
#include "drowned_full_masks.h"
#include "drowned_full_occlusion.h"
"""+source+mechanisms+r"""
int main(void){
 int j,i,x,y,row,chunk,bit,k;UWORD a[400],b[400];
 drownedReset();drownedSetView(640);
 assert(drownedGateSolid(808,160)&&governorSolid(4560,160));
 assert(drownedPanelHit(772,170)==PROJECTILE_ENEMY_HIT);
 for(i=0;i<43;i++)drownedTick();
 assert(!drownedGateSolid(808,160)&&governorSolid(4560,160));
 assert(drownedHeaderOverlaps(800,120,815,130)&&drownedHeaderOverlaps(4560,120,4575,130));
 drownedRespawn(TRUE);assert(!drownedGateSolid(808,160)&&!governorComplete());
 drownedRespawn(FALSE);assert(drownedGateSolid(808,160));
 pressureTick=140;assert(drownedJetTouches(460,160,467,190)&&drownedJetTouches(1804,128,1811,168));
 governorReset();
 assert(governorSolid(3440,144)&&!governorSolid(3440,143));
 assert(governorSolid(4560,160));
 for(j=0;j<3;j++){
  governorView(3248+448+xs[j]-120);
  for(i=0;i<100;i++)governorTick();
  for(i=0;i<3;i++)governorHit(3248+448+xs[j]+16,ys[j]+32);
 }
 for(i=0;i<43;i++)governorTick();
 assert(governorComplete()&&!governorSolid(4560,160)&&governorSolid(4560,128));
 for(k=0;k<sizeof(drownedOccluders)/sizeof(drownedOccluders[0]);k++){
  for(x=drownedOccluders[k].x-49;x<drownedOccluders[k].x+35;x++)
   for(y=80;y<208;y+=3){
    memset(a,255,sizeof(a));memset(b,255,sizeof(b));
    drownedFullMask(a,b,x,y);
    for(row=0;row<48;row++)for(chunk=0;chunk<4;chunk++){
     UWORD want=65535;
     for(bit=0;bit<16;bit++)for(i=0;i<sizeof(drownedOccluders)/sizeof(drownedOccluders[0]);i++){
      const struct DrownedOccluder *o=&drownedOccluders[i];
      int px=x+chunk*16+bit-o->x,py=y+row-o->y;
      if(chunk<3&&px>=0&&px<o->w&&py>=0&&py<o->h&&(o->bits[py]&(0x80000000u>>px)))want&=~(0x8000u>>bit);
     }
     assert(a[8+row*8+chunk]==want&&a[12+row*8+chunk]==want);
    }
    assert(!memcmp(a,b,sizeof(a)));for(i=0;i<8;i++)assert(a[i]==65535);
    for(i=392;i<400;i++)assert(a[i]==65535);
   }
 }
}
"""
with tempfile.TemporaryDirectory() as t:
 p=Path(t);(p/'test.c').write_text(c)
 subprocess.run(['cc','-O2','-fsanitize=address,undefined','-I',str(O),'-I',str(R/'src'),str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
print('PASS full5120: prior collision bytes, preserved station group, dry rooted trees, shifted finale and foreground pixel oracle.')
