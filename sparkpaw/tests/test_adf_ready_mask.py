"""Disk-loaded mask must render identically to embedded HD tables."""
from pathlib import Path
import re,subprocess,tempfile
ROOT=Path(__file__).resolve().parents[1]
s=(ROOT/'src/ready_dust.c').read_text()
s=re.sub(r'^#include[^\n]*\n','',s,flags=re.M)
# The inactive embedded branch is harmless after preprocessing.
shim=r'''
#define SPARKPAW_MULTI_ADF
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "ready_dust.h"
#include "ready_dust_mask.h"
/* Rename embedded reference arrays; loaded arrays follow below. */
'''
shim=shim.replace('#include "ready_dust_mask.h"','''#define readyDustMask referenceMask
#define readyDustMenuMask referenceMenuMask
#include "ready_dust_mask.h"
#undef readyDustMask
#undef readyDustMenuMask''')
shim+=r'''
typedef unsigned long ULONG;
#define MEMF_FAST 4
static int bad,loads,live;
static unsigned char *assetsLoadDiskData(const char *n,ULONG flags,ULONG *size){
 unsigned char *p;(void)n;assert(flags==MEMF_FAST);loads++;
 *size=bad?12:40192;p=malloc(*size);live++;
 if(!bad){memcpy(p,referenceMask,10240);memcpy(p+10240,referenceMenuMask,29952);}
 return p;
}
static void FreeMem(void *p,ULONG n){(void)n;live--;free(p);}
void referenceDraw(unsigned char **,struct ReadyDustHistory *,unsigned long,const unsigned char *,unsigned char);
void referenceRestore(unsigned char **,struct ReadyDustHistory *);
'''
main=r'''
int main(void){
 unsigned char a[6][10240],b[6][10240],*ap[6],*bp[6],pens[6]={1,2,3,4,5,6};
 struct ReadyDustHistory ah={0},bh={0};unsigned long frame;int p;
 bad=1;assert(!readyDustLoad()&&!live);bad=0;
 assert(readyDustLoad()&&live==1);assert(readyDustLoad()&&loads==2);
 assert(!memcmp(readyDustMask,referenceMask,10240));
 assert(!memcmp(readyDustMenuMask,referenceMenuMask,29952));
 for(p=0;p<6;p++){memset(a[p],p*31,10240);memcpy(b[p],a[p],10240);ap[p]=a[p];bp[p]=b[p];}
 for(frame=0;frame<1600;frame++){
  readyDustRestore(ap,&ah);referenceRestore(bp,&bh);
  readyDustDraw(ap,&ah,frame,pens,frame%12);referenceDraw(bp,&bh,frame,pens,frame%12);
  assert(!memcmp(a,b,sizeof(a)));assert(!memcmp(&ah,&bh,sizeof(ah)));
 }
 readyDustUnload();readyDustUnload();assert(!live);
 assert(readyDustLoad());readyDustUnload();assert(!live);
 puts("PASS: loaded/embedded mask, 1600 frames/all states, wrong size, load-once and cleanup");
 return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'disk.c').write_text(shim+s+main)
 subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-I'+str(ROOT/'src'),'-DreadyDustDraw=referenceDraw','-DreadyDustSetMenuMask=referenceSetMenuMask','-DreadyDustRestore=referenceRestore','-c',str(ROOT/'src/ready_dust.c'),'-o',str(p/'reference.o')],check=True)
 subprocess.run(['cc','-std=c99','-fsanitize=address,undefined','-I'+str(ROOT/'src'),str(p/'disk.c'),str(p/'reference.o'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
