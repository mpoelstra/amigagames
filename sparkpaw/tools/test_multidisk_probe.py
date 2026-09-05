"""Exercise actual disk-only reader and media source with host DOS stubs."""
from pathlib import Path
import subprocess,random,struct,zlib
from pack_disk_asset import pack
from pack_adf_asset import pack as spr1
ROOT=Path(__file__).resolve().parents[1];OUT=ROOT/'build/multidisk-probe/tests';OUT.mkdir(parents=True,exist_ok=True)

def compile_run(name,source,args=()):
 p=OUT/(name+'.c');p.write_text(source)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-g','-I'+str(ROOT/'src'),str(p),'-o',str(OUT/name)],check=True)
 subprocess.run([str(OUT/name),*map(str,args)],check=True)

def main():
 s=(ROOT/'src/assets.c').read_text();a=s.index('struct PackedReader {');b=s.index('\n#endif\n\nstatic void freeAsset',a)
 body=s[a:b]
 header='''#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef uint8_t UBYTE;typedef uint16_t UWORD;typedef uint32_t ULONG;
typedef int32_t LONG;typedef int BOOL;typedef char *STRPTR;typedef FILE *BPTR;
#define SPARKPAW_MULTI_ADF
#define TRUE 1
#define FALSE 0
#define MODE_OLDFILE 0
#define Open(name,mode) fopen(name,"rb")
#define Close(file) fclose(file)
#define Read(file,data,n) ((LONG)fread(data,1,n,file))
#include "packed_crc32.h"
static UBYTE diskDecodeWindow[4096];
'''
 driver='''
int main(int argc,char **argv){struct PackedReader r;UBYTE bytes[333],expected[333];FILE *raw;int chunk,n;BOOL ok=TRUE;assert(argc==4);raw=fopen(argv[2],"rb");assert(raw);
if(!packedOpen(argv[1],&r)){assert(atoi(argv[3])==0);fclose(raw);return 0;}
for(chunk=0;(n=fread(expected,1,(chunk%3==0?12:chunk%3==1?97:333),raw))>0;chunk++){
if(!packedRead(&r,bytes,n)){ok=FALSE;break;}if(memcmp(bytes,expected,n)){ok=FALSE;break;}}
ok=packedClose(&r,ok);fclose(raw);assert(ok==atoi(argv[3]));return 0;}
'''
 # Compile once; run independent reference payloads with split reads and corruption.
 compile_run('reader',header+body+driver,prepare_first())
 rng=random.Random(68020)
 payloads=[b'',b'A',b'A'*9000,bytes(range(256))*40,bytes(rng.randrange(256) for _ in range(8000))]
 payloads += [(ROOT/'assets/runtime'/n).read_bytes() for n in ['readymenu.spbm','sparkpaw-sprites4.spbm','stormrail-family.spbm']]
 count=0
 for i,raw in enumerate(payloads):
  rp=OUT/'raw';rp.write_bytes(raw)
  for encoder in [spr1,pack]:
   cp=OUT/'packed';data=encoder(raw);cp.write_bytes(data)
   subprocess.run([str(OUT/'reader'),str(cp),str(rp),'1'],check=True);count+=1
   if len(data)>16:
    bad=bytearray(data);bad[8]^=1;cp.write_bytes(bad)
    subprocess.run([str(OUT/'reader'),str(cp),str(rp),'0'],check=True);count+=1
    cp.write_bytes(data[:-1]);subprocess.run([str(OUT/'reader'),str(cp),str(rp),'0'],check=True);count+=1
 # Invalid backreference before any output.
 raw=b'abc';rp.write_bytes(raw);cp.write_bytes(struct.pack('>4sIII',b'SPL1',3,zlib.crc32(raw),3)+b'\x80\x00\x00')
 subprocess.run([str(OUT/'reader'),str(cp),str(rp),'0'],check=True);count+=1
 media=(ROOT/'src/disk_media.c').read_text();media='\n'.join(l for l in media.splitlines() if not l.startswith('#include'))
 stubs='''#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#define SPARKPAW_MULTI_ADF
#define TRUE 1
#define FALSE 0
#define MODE_OLDFILE 0
typedef unsigned char UBYTE;typedef int BOOL;typedef int LONG;typedef int BPTR;typedef void *APTR;typedef char *STRPTR;
struct Process {APTR pr_WindowPtr;} proc;static char disks[2];static int selected,prompts,reloads,scanphase;static char lastpath[96];
static void *FindTask(void *unused){(void)unused;return &proc;}
static BPTR Open(STRPTR path,LONG mode){(void)mode;strcpy(lastpath,path);selected=path[2]-'0';return disks[selected]?selected+1:0;}
static LONG Read(BPTR f,void *out,LONG size){const char *m=disks[f-1]==1?"SP07D1\\n":disks[f-1]==2?"SP07D2\\n":"BADVER\\n";assert(size>=7);memcpy(out,m,7);return 7;}
static void Close(BPTR f){(void)f;}
static int titleShowInsertDisk(UBYTE d){(void)d;prompts++;scanphase=1;return 1;}
static int titleShowReplayLoading(void){reloads++;return 1;}
static void WaitTOF(void){if(scanphase==1){disks[0]=2;scanphase=2;}}
'''
 driver='''int main(void){proc.pr_WindowPtr=(void*)123;disks[0]=1;disks[1]=2;
assert(diskMediaRequire(2));assert(activeDrive==1&&!prompts);assert(proc.pr_WindowPtr==(void*)123);
assert(diskMediaOpen("PROGDIR:assets/runtime/readymenu.spbm",0));assert(!strcmp(lastpath,"DF1:assets/runtime/readymenu.spr1"));
assert(diskMediaRequire(1));assert(activeDrive==0&&!prompts);disks[1]=3;
assert(diskMediaRequire(2));assert(prompts==1&&reloads==1&&activeDrive==0);assert(proc.pr_WindowPtr==(void*)123);
assert(!diskMediaRequire(0)&&!diskMediaRequire(3));puts("Media: DF1 automatic, wrong version, DF0 swap, path rewrite and requester restoration pass");return 0;}
'''
 compile_run('media',stubs+media+'\n'+driver)
 print('Native reader source cases passed:',count)
def prepare_first():
 r=OUT/'first.raw';r.write_bytes(b'test');p=OUT/'first.packed';p.write_bytes(pack(b'test'));return p,r,'1'
if __name__=='__main__':main()
