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


#include "drowned_route_upper.h"
static int jumpTo(int ax,int ay,int width,int bx,int by,int targetWidth){
 int offset,hold,run,t;
 for(run=0;run<2;run++)for(offset=-20;offset<width;offset+=2)for(hold=8;hold<=64;hold++){
  resetAt(ax+offset,ay-39,run?650:0);
  if(!collisionSolidAt(ax+offset+16,ay))continue;
  for(t=0;t<100;t++){
   playerUpdatePhysics(0,t<hold,0,t==0);
   if(t>2&&playerState()->grounded){
    int x=playerState()->x>>8,y=playerState()->y>>8;
    if(y==by-39&&x+27>=bx+3&&x+4<=bx+targetWidth-4){printf("jump %d,%d -> %d,%d offset%d hold%d run%d ticks%d\n",ax,ay,bx,by,offset,hold,run,t);return 1;}
    break;
   }
   if((playerState()->y>>8)>208)break;
  }
 }
 return 0;
}
int main(void){
 int i;assert(collisionLoad());drownedReset();
 /* First half contains no alternate solid footholds above the basin. */
 for(i=160;i<528;i+=8){int y;for(y=64;y<189;y+=8)assert(!collisionSolidAt(i,y));}
 for(i=0;i<2;i++)assert(jumpTo(upper[i][0],upper[i][1],16,upper[i+1][0],upper[i+1][1],16));
 assert(jumpTo(864,128,16,960,200,120));

 assert(jumpTo(528,160,96,672,128,16));
 return 0;
}

''')
    subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_FERRY','-DSPARKPAW_DROWNED_SPILLWING','-DSPARKPAW_SPILLWING_PAIR','-DSPARKPAW_DROWNED_SLICE','-DSPARKPAW_DROWNED_PONTOON','-DSPARKPAW_DROWNED_ROUTE','-DSPARKPAW_DROWNED_ENCOUNTER','-DSPARKPAW_DROWNED_ENEMY_ART','-DSPARKPAW_WORLD_W=2400',
        '-I'+str(p),'-I'+str(ROOT/'src'),'-I'+str(ROOT/'build/drowned-ferry'),str(p/'test.c'),
        *[str(ROOT/'src'/name) for name in ('drowned_slice.c','drowned_pontoon.c','collision.c','player.c','projectiles.c','enemies.c','spillwing.c','collectibles.c')],
        '-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],cwd=ROOT,check=True)
