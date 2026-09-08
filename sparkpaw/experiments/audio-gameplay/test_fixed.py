"""Run real gameplay/physics/collision on host to validate the fixed input route."""
from pathlib import Path
import subprocess,tempfile
from prepare_fixed import HERE,ROOT,player_source
with tempfile.TemporaryDirectory() as d:
 p=Path(d)
 for name in ['exec','dos','proto']:(p/name).mkdir()
 (p/'exec/types.h').write_text('''#ifndef HOST_AMIGA_TYPES
#define HOST_AMIGA_TYPES
#include <stdint.h>
typedef int32_t LONG;typedef uint32_t ULONG;typedef int16_t WORD;typedef uint16_t UWORD;typedef int8_t BYTE;typedef uint8_t UBYTE;typedef int16_t BOOL;typedef void *APTR;typedef const void *CONST_APTR;typedef char *STRPTR;typedef const char *CONST_STRPTR;
#define TRUE 1
#define FALSE 0
#ifndef NULL
#define NULL ((void *)0)
#endif
#endif
''')
 (p/'dos/dos.h').write_text('#pragma once\n#include <stdio.h>\ntypedef FILE *BPTR;\n#define MODE_OLDFILE 1\n')
 (p/'proto/dos.h').write_text('#include <dos/dos.h>\nBPTR Open(const char *,int);long Read(BPTR,void *,long);void Close(BPTR);\n')
 (p/'player.c').write_text(player_source())
 audio=(ROOT/'src/audio.h').read_text()
 import re
 functions=re.findall(r'void (audio\w+)\(void\);',audio)
 stubs='\n'.join('void '+f+'(void){}' for f in functions)
 (p/'main.c').write_text('''#include <stdio.h>
#include <assert.h>
#include "game.h"
#include "player.h"
#include "collision.h"
#include "script.h"
#include "dos/dos.h"
unsigned long fixedStep;
ULONG platformFieldCounter(void){return fixedStep;}
BPTR Open(const char *name,int mode){(void)name;(void)mode;return fopen("assets/runtime/storm-collision.bin","rb");}
long Read(BPTR f,void *b,long n){return fread(b,1,n,f);}
void Close(BPTR f){fclose(f);}
'''+stubs+'''
int main(void){
 unsigned resets=0,band=0;LONG previous=0,min=99999,max=0;
 gameInit(0x53504157UL);assert(collisionLoad());
 for(fixedStep=0;fixedStep<SCRIPT_STEPS;fixedStep++){
  gameUpdate();
  if(gameState()->frameCounter<previous)resets++;
  previous=gameState()->frameCounter;
  if(gameState()->cameraX>=750&&gameState()->cameraX<=1000)band++;
  if(fixedStep>=520){if(gameState()->cameraX<min)min=gameState()->cameraX;if(gameState()->cameraX>max)max=gameState()->cameraX;}
  if(fixedStep%60==0)printf("step=%lu camera=%d x=%d health=%u\\n",fixedStep,(int)gameState()->cameraX,(int)(playerState()->x>>8),playerState()->health);
 }
 assert(resets==0&&band>=250&&min>=750&&max<=1150);
 printf("route resets=%u band_steps=%u patrol_camera=%d..%d final_camera=%d\\n",resets,band,(int)min,(int)max,(int)gameState()->cameraX);
 return 0;
}
''')
 names=['game','collision','level_data','enemies','collectibles','projectiles']
 subprocess.run(['cc','-std=c99','-O1','-g','-fsanitize=address,undefined','-I'+str(p),'-I'+str(ROOT/'src'),'-I'+str(HERE),str(p/'main.c'),str(p/'player.c'),*[str(ROOT/f'src/{n}.c') for n in names],'-o',str(p/'test')],check=True)
 first=subprocess.check_output([str(p/'test')],cwd=ROOT,text=True)
 second=subprocess.check_output([str(p/'test')],cwd=ROOT,text=True)
 assert first==second
 print(first+'Repeated actual-C route is identical; bounds/no-reset assertions PASS')
