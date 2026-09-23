"""Exercise the three-disk resolver across DF0-DF3 and DF0 swaps."""
from pathlib import Path
from test_multidisk_probe import compile_run

ROOT = Path(__file__).resolve().parents[1]
media = '\n'.join(line for line in (ROOT / 'src/disk_media.c').read_text().splitlines()
                  if not line.startswith('#include'))
stubs = r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#define SPARKPAW_MULTI_ADF
#define SPARKPAW_DROWNED_THREE_ADF
#define TRUE 1
#define FALSE 0
#define MODE_OLDFILE 0
typedef unsigned char UBYTE;typedef int BOOL;typedef int LONG;typedef int BPTR;
typedef void *APTR;typedef char *STRPTR;
struct Process {APTR pr_WindowPtr;} proc;
static char disks[4];static int selected,prompts,reloads,swap,requested;
static char lastpath[96];
static void *FindTask(void *unused){(void)unused;return &proc;}
static BPTR Open(STRPTR path,LONG mode){(void)mode;strcpy(lastpath,path);
 selected=path[2]-'0';assert(selected>=0&&selected<4);
 return disks[selected]?selected+1:0;}
static LONG Read(BPTR f,void *out,LONG size){char marker[8];
 assert(size>=7);snprintf(marker,sizeof(marker),"SP09D%d\n",disks[f-1]);
 memcpy(out,marker,7);return 7;}
static void Close(BPTR f){(void)f;}
static int titleShowInsertDisk(UBYTE d){requested=d;prompts++;swap=1;return 1;}
static int titleShowReplayLoading(void){reloads++;return 1;}
static void WaitTOF(void){if(swap){disks[0]=requested;swap=0;}}
'''
driver = r'''
int main(void){int i;proc.pr_WindowPtr=(void*)123;disks[0]=1;disks[1]=3;
 assert(diskMediaSelectIfPresent(3)&&activeDrive==1&&!prompts);
 assert(diskMediaOpen("PROGDIR:assets/runtime/drowned-route.spbm",0));
 assert(!strcmp(lastpath,"DF1:assets/runtime/drowned-route.spr1"));
 disks[1]=0;disks[2]=3;
 assert(diskMediaSelectIfPresent(3)&&activeDrive==2&&!prompts);
 assert(diskMediaOpen("PROGDIR:assets/runtime/drowned-route.spbm",0));
 assert(!strcmp(lastpath,"DF2:assets/runtime/drowned-route.spr1"));
 disks[2]=0;disks[3]=3;
 assert(diskMediaSelectIfPresent(3)&&activeDrive==3&&!prompts);
 assert(diskMediaOpen("PROGDIR:assets/runtime/drowned-route.spbm",0));
 assert(!strcmp(lastpath,"DF3:assets/runtime/drowned-route.spr1"));
 disks[3]=0;
 assert(!diskMediaSelectIfPresent(4));
 for(i=1;i<=3;i++){
  disks[0]=i==1?2:1;disks[1]=0;
  assert(diskMediaRequire(i));assert(activeDrive==0);
  assert(disks[0]==i);assert(proc.pr_WindowPtr==(void*)123);
 }
 assert(prompts==3&&reloads==3);
 assert(!diskMediaRequire(0)&&!diskMediaRequire(4));
 disks[0]=2;disks[1]=3;
 assert(diskMediaRequire(3)&&activeDrive==1&&prompts==3);
 puts("Three-disk media: DF0 swaps, DF1-DF3 selection, markers, path and requester pass");
 return 0;}
'''
compile_run('campaign-drowned-media', stubs + media + '\n' + driver)
