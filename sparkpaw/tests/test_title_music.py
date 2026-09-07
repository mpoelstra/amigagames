#!/usr/bin/env python3
"""Actual music lifecycle under host OS stubs and ASan/UBSan."""
from pathlib import Path
import re,subprocess,tempfile,hashlib
ROOT=Path(__file__).resolve().parents[1]
s=re.sub(r'^#include.*\n','',(ROOT/'src/music.c').read_text(),flags=re.M)
s=re.sub(r'__reg\("[^\"]+"\)\s*','',s)
stubs=r'''
#define SPARKPAW_STORY_INTRO
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
typedef int BOOL; typedef uint8_t UBYTE; typedef uint16_t UWORD;
typedef unsigned long ULONG; typedef long LONG; typedef int BPTR; typedef char *STRPTR;
#define TRUE 1
#define FALSE 0
#define MEMF_FAST 1
#define MEMF_CHIP 2
#define MODE_OLDFILE 0
#define OFFSET_END 1
#define OFFSET_CURRENT 0
#define OFFSET_BEGINNING -1
#define ADALLOC_MAXPREC 127
#define ADCMD_ALLOCATE 0
#define ADIOF_NOWAIT 0
#define AUDIONAME "audio.device"
#define NT_INTERRUPT 0
#define INTB_VERTB 5
struct Node {int ln_Pri,ln_Type;char *ln_Name;};
struct Msg {struct Node mn_Node;};
struct IORequest {struct Msg io_Message;int io_Command,io_Flags;};
struct IOAudio {struct IORequest ioa_Request;void *ioa_Data;ULONG ioa_Length;};
struct MsgPort {int value;};
struct Interrupt {struct Node is_Node;void (*is_Code)(void);};
static int memcount,failalloc,alloccall,failopen,failread,devicefail;
static int devcount,servercount,enabled,framecount,lockdepth;
struct MsgPort *CreateMsgPort(void){return calloc(1,sizeof(struct MsgPort));}
void DeleteMsgPort(struct MsgPort *p){free(p);}
void *CreateIORequest(struct MsgPort *p,ULONG n){(void)p;return calloc(1,n);}
void DeleteIORequest(void *p){free(p);}
int OpenDevice(char *n,int u,struct IORequest *p,int f){(void)n;(void)u;(void)p;(void)f;if(devicefail)return 1;devcount++;return 0;}
void CloseDevice(struct IORequest *p){(void)p;devcount--;}
void AddIntServer(int n,struct Interrupt *p){(void)n;(void)p;servercount++;}
void RemIntServer(int n,struct Interrupt *p){(void)n;(void)p;servercount--;}
void Disable(void){lockdepth++;}void Enable(void){assert(lockdepth>0);lockdepth--;}
void *AllocMem(ULONG n,ULONG flags){assert(flags==MEMF_FAST||flags==MEMF_CHIP);if(++alloccall==failalloc)return NULL;memcount++;return calloc(1,n);}
void FreeMem(void *p,ULONG n){(void)n;assert(!enabled);memcount--;free(p);}
BPTR Open(char *n,int m){(void)m;if(failopen)return 0;return strstr(n,"lsmusic")?1:2;}
void Close(BPTR f){(void)f;}
LONG Seek(BPTR f,LONG p,int m){(void)f;(void)p;return m==OFFSET_CURRENT?32:0;}
LONG Read(BPTR f,void *p,LONG n){if(failread)return 0;memset(p,0,n);if(f==1)memcpy(p,"LSP1ABCD",8);else memcpy(p,"ABCD",4);return n;}
void lsp_init(void *a,void *b){assert(a&&b&&lockdepth);}
void lsp_set_once(UBYTE n){assert(n<=1);}
void lsp_stop(void){assert(!enabled);}
void lsp_enable(UBYTE n){enabled=n;}
void lsp_set_rate(UWORD n){assert(n==50);}
void lsp_vblank_interrupt(void){}
void lsp_frame(void){if(enabled)framecount++;}
BOOL musicInitialize(UWORD);BOOL musicPlayTitle(void);void musicStop(void);void musicShutdown(void);
'''
main=r'''
int main(void){
 int i,j;
 devicefail=1;assert(!musicPlayTitle());assert(!memcount&&!devcount&&!servercount);
 devicefail=0;
 for(i=0;i<12;i++){
  assert(musicPlayIntro());assert(memcount==2);
  assert(musicPlayTitle());assert(memcount==2&&enabled&&devcount==1&&servercount==1);
  musicOwnedFrame();assert(framecount==i+1);
  Disable();musicStop();assert(lockdepth==1&&!enabled&&!memcount);Enable();
  musicOwnedFrame();assert(framecount==i+1);
 }
 for(j=1;j<=2;j++){
  alloccall=0;failalloc=j;assert(!musicPlayTitle());assert(!memcount&&!enabled);
 }
 failalloc=0;failread=1;assert(!musicPlayTitle());assert(!memcount);
 failread=0;failopen=1;assert(!musicPlayTitle());assert(!memcount);
 failopen=0;assert(musicPlayTitle());musicShutdown();musicShutdown();
 assert(!memcount&&!devcount&&!servercount&&!lockdepth&&!enabled);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as tmp:
 c=Path(tmp)/'music.c';exe=Path(tmp)/'music';c.write_text(stubs+s+main)
 subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-g',str(c),'-o',str(exe)],check=True)
 subprocess.run([str(exe)],check=True)
assert hashlib.sha256((ROOT/'music/neon-sky.mod').read_bytes()).hexdigest()=='4a48e8e24415485ad949204f55ae4fbed4ae84fa0fcfd15ac31d948f5ad221c3'
score=(ROOT/'assets/runtime/neon-sky.lsmusic').read_bytes();bank=(ROOT/'assets/runtime/neon-sky.lsbank').read_bytes()
assert score[:4]==b'LSP1' and score[4:8]==bank[:4] and len(bank)==142796
print('PASS: actual music lifecycle, failures, nested interrupt ownership and approved MOD identity')
