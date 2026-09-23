"""Execute real candidate state, collision, player physics and projectile sweep."""
from pathlib import Path
import subprocess, tempfile, sys
ROOT=Path(__file__).resolve().parents[1]
subprocess.run([sys.executable,str(ROOT/'tools/build_drowned_ferry.py')],check=True)
with tempfile.TemporaryDirectory() as directory:
    p=Path(directory)
    for folder in ('exec','dos','proto'): (p/folder).mkdir()
    (p/'exec/types.h').write_text('''#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
#include <stddef.h>
typedef int BOOL; typedef int8_t BYTE; typedef uint8_t UBYTE;
typedef int16_t WORD; typedef uint16_t UWORD; typedef int32_t LONG;
typedef uint32_t ULONG; typedef void *BPTR;
#define TRUE 1
#define FALSE 0
#endif
''')
    (p/'dos/dos.h').write_text('#define MODE_OLDFILE 1005\n')
    (p/'proto/dos.h').write_text('#include <exec/types.h>\nBPTR Open(const char*,LONG); LONG Read(BPTR,void*,LONG); LONG Close(BPTR);\n')
    (p/'test.c').write_text(r'''
#include <assert.h>
#include <stdio.h>
#include "drowned_slice.h"
#include "drowned_pontoon.h"
#include "collision.h"
#include "player.h"
#include "projectiles.h"
#include "level_data.h"
#include "enemies.h"
#include "collectibles.h"
BPTR Open(const char *name,LONG mode) { return fopen("build/drowned-ferry/assets/drowned-route.bin","rb"); }
LONG Read(BPTR f,void *p,LONG n) { return fread(p,1,n,f); }
LONG Close(BPTR f) { return fclose(f); }
void platformReadGameKeys(BOOL*a,BOOL*b,BOOL*c,BOOL*d,BOOL*e) {*a=*b=*c=*d=*e=0;}
BOOL platformSecondaryButtonHeld(void) { return 0; }
static void sound(void) {}
static void resetAt(int x,int y,int speed) {
    struct PlayerState *s; playerInit(); s=(struct PlayerState *)playerState();
    s->x=x*256; s->y=y*256; s->vx=speed; s->grounded=1;
}


#include "spillwing.h"
#include <string.h>
void referenceUpdate(struct Enemy*,WORD,WORD);
BOOL currentClear(const struct Enemy*,BOOL);
BOOL referenceClear(const struct Enemy*,BOOL);
static long probes;
BOOL countedSolid(WORD left,WORD right,WORD y){probes++;return collisionSolidHorizontal(left,right,y);}
int main(void){
 struct Enemy a,b;int x,high,left,bob;long ref=0,fast=0;assert(collisionLoad());
 for(x=0;x<2400;x++)for(high=0;high<2;high++)for(left=0;left<2;left++){
  BOOL aa,bb;memset(&a,0,sizeof(a));a.spawnIndex=high;a.x=x*256;
  probes=0;aa=currentClear(&a,left);fast+=probes;
  probes=0;bb=referenceClear(&a,left);ref+=probes;assert(aa==bb);
  for(bob=0;bob<2;bob++){
   WORD cam=x<160?0:x-160;
   memset(&a,0,sizeof(a));a.spawnIndex=high;a.x=x*256;a.patrolLeft=0;a.patrolRight=2400;
   a.vx=left?-256:256;a.walkTick=bob?15:0;a.health=1;a.active=TRUE;
   b=a;probes=0;spillwingUpdate(&a,cam,x+(left?-80:80));fast+=probes;
   probes=0;referenceUpdate(&b,cam,x+(left?-80:80));ref+=probes;
   assert(!memcmp(&a,&b,sizeof(a)));
  }
 }
 assert(fast==0&&ref>100000);
 printf("PASS:9600 exact swoop predicates +19200 full updates; reference probes=%ld cached probes=%ld\n",ref,fast);
 return 0;
}

''')

    flags=['-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_FERRY','-DSPARKPAW_DROWNED_SPILLWING','-DSPARKPAW_SPILLWING_PAIR','-DSPARKPAW_DROWNED_SLICE','-DSPARKPAW_DROWNED_PONTOON','-DSPARKPAW_DROWNED_ROUTE','-DSPARKPAW_DROWNED_ENCOUNTER','-DSPARKPAW_DROWNED_ENEMY_ART','-DSPARKPAW_WORLD_W=2400','-I'+str(p),'-I'+str(ROOT/'src'),'-I'+str(ROOT/'build/drowned-ferry')]
    source=(ROOT/'src/spillwing.c').read_text()
    for reference in [False,True]:
        label='reference' if reference else 'current'
        f=p/(label+'.c');f.write_text(source+'\nBOOL '+label+'Clear(const struct Enemy*e,BOOL left){return clearSwoop(e,left); }\n')
        rename=['-DcollisionSolidHorizontal=countedSolid']
        if reference:
            rename+=['-DSPARKPAW_SPILLWING_CLEARANCE_REFERENCE','-DspillwingUpdate=referenceUpdate']+['-Dspillwing'+n+'=reference'+n for n in ['Init','Left','Right','Hit']]
        subprocess.run(['cc',*flags,*rename,'-c',str(f),'-o',str(p/(label+'.o'))],check=True)
    subprocess.run(['cc',*flags,str(p/'test.c'),str(p/'current.o'),str(p/'reference.o'),*[str(ROOT/'src'/name) for name in ('drowned_slice.c','drowned_pontoon.c','collision.c','player.c','projectiles.c','enemies.c','collectibles.c')],'-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],cwd=ROOT,check=True)
