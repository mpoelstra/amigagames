"""Actual bounded prototype state: phase, visibility, hit counts, hazard, reset."""
from pathlib import Path
import tempfile,subprocess
R=Path(__file__).resolve().parents[1]
s=(R/'src/drowned_governor.c').read_text().replace('#include "drowned_governor.h"','').replace('#include "projectiles.h"','')
route=(R/'src/drowned_slice.c').read_text()
def function(name):
 a=route.index(name);start=route.rfind('\n',0,a)+1;opening=route.index('{',a);depth=1;end=opening+1
 while depth:
  depth+=(route[end]=='{')-(route[end]=='}');end+=1
 return route[start:end]
s+='\n#define SPARKPAW_DROWNED_GOVERNOR\n#define PROJECTILE_ENEMY_MISS 0\nstatic BOOL enemiesFirstProjectileHitOnSweep(WORD a,WORD b,WORD y,WORD *x){\n if(y!=168|| (a<b?(a>1148||b<1148):(a<1148||b>1148)))return FALSE;*x=1148;return TRUE;}\nstatic UBYTE enemiesHitProjectile(WORD x,WORD y){return x==1148&&y==168?2:0;}\n'
s+=function('UBYTE drownedEncounterHit(')+function('BOOL drownedEncounterSweep(')
prefix='#include <assert.h>\n#include <stdio.h>\ntypedef short WORD;typedef unsigned short UWORD;typedef unsigned char UBYTE;typedef int BOOL;\n#define TRUE 1\n#define FALSE 0\n#define PROJECTILE_ENEMY_HIT 1\n'
s+='''
static void testView(WORD x){governorView(x+448);}
static UBYTE testHit(WORD x,WORD y){return governorHit(x+448,y);}
static BOOL testSolid(WORD x,WORD y){return governorSolid(x+448,y);}
static BOOL testHazard(WORD l,WORD t,WORD r,WORD b){return governorHazard(l+448,t,r+448,b);}
static BOOL testSweep(WORD a,WORD b,WORD y,WORD *h){BOOL yes=governorSweep(a+448,b+448,y,h);if(yes)*h-=448;return yes;}
static BOOL testEncounter(WORD a,WORD b,WORD y,WORD *h){BOOL yes=drownedEncounterSweep(a+448,b+448,y,h);if(yes)*h-=448;return yes;}
#define governorView testView
#define governorHit testHit
#define governorSolid testSolid
#define governorHazard testHazard
#define governorSweep testSweep
#define drownedEncounterSweep testEncounter
'''
main='''
int main(void){WORD hit;int i,j;
 governorReset();
 for(j=0;j<3;j++)for(int d=0;d<3;d++)for(i=0;i<200;i++){
  lock=j;damage=d;tick=i;closing=TRUE;
  UBYTE frame=governorArtFrame(j);
  assert(frame<34);
  if(i<14)assert(frame==13+d*7+(i>>1));
  else if(i>=86&&i<100)assert(frame==6+((i-86)>>1));
  else assert(frame==governorFrame(j));
  assert(governorArtFrame(5)==34);
 }
 lock=3;assert(governorArtFrame(5)==39);
 governorReset();assert(governorArtFrame(0)==1&&!governorComplete());
 governorReset();assert(governorFrame(0)==1&&governorFrame(1)==0);
 assert(governorSolid(865,170));assert(governorSolid(450,152));assert(!governorSolid(450,151));
 for(j=0;j<3;j++){
  governorView(j==0?0:j==1?352:608);
  governorHit(xs[j]+16,ys[j]+32);assert(damage==0);
  for(i=0;i<45;i++)governorTick();
  assert(governorHazard(j<2?396:652,160,j<2?399:655,190));
  for(i=45;i<100;i++)governorTick();
  assert(!governorHazard(j<2?396:652,160,j<2?399:655,190));
  assert(governorFrame(j)==2);
  for(i=0;i<(j==0?100:j==1?75:55)-1;i++)governorTick();
  assert(governorFrame(j)==2);governorTick();assert(governorFrame(j)==1);
  for(i=0;i<100;i++)governorTick();assert(governorFrame(j)==2);
  assert(governorSweep(xs[j]-30,xs[j]+40,ys[j]+32,&hit)&&hit==xs[j]+4);
  assert(governorSweep(xs[j]+40,xs[j]-30,ys[j]+32,&hit)&&hit==xs[j]+27);
  governorView(1200);assert(!governorSweep(xs[j]-30,xs[j]+40,ys[j]+32,&hit));
  governorHit(xs[j]+16,ys[j]+32);assert(!damage);
  governorView(j==0?0:j==1?352:608);
  governorHit(xs[j]+16,ys[j]+32);assert(governorFrame(j)==3);
  governorHit(xs[j]+16,ys[j]+32);assert(governorFrame(j)==4);
  governorHit(xs[j]+16,ys[j]+32);assert(governorFrame(j)==5);
 }
 assert(lock==3&&governorSolid(865,170));
 for(i=0;i<43;i++)governorTick();
 assert(governorExitFrame()==22&&!governorSolid(865,170)&&governorComplete());
 assert(governorSolid(865,125));
 for(i=0;i<1000;i++)governorTick();assert(lock==3&&!governorHazard(640,150,670,195));
 governorReset();governorView(600);
 assert(drownedEncounterSweep(650,810,168,&hit)&&hit==700);
 assert(drownedEncounterSweep(810,650,168,&hit)&&hit==795);
 assert(drownedEncounterHit(1148,168)==2&&damage==0);
 governorReset();assert(lock==0&&damage==0&&tick==0&&governorSolid(865,170));
 puts("PASS actual governor: three sequential locks, closed/open hits, both sweep directions, offscreen, hazard timing, exit and reset");
 return 0;}
'''
with tempfile.TemporaryDirectory() as t:
 p=Path(t);(p/'test.c').write_text(prefix+s+main)
 subprocess.run(['cc', '-DDROWNED_FINALE_OFFSET=0','-fsanitize=address,undefined',str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
