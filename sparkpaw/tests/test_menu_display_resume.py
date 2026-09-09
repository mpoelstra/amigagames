"""Actual retained-display reacquisition: no Copper/display DMA restart.

Mock MMIO records every DMA write, rather than checking only the last value.
This proves ownership/register policy, not the absence of a native glitch.
"""
from pathlib import Path
import subprocess,tempfile,re
ROOT=Path(__file__).resolve().parents[1]
source=(ROOT/'src/platform_amiga.c').read_text()
a=source.index('void platformResumeMenuAfterLoading(void)')
b=source.index('void platformResetGameInput',a)
body=source[a:b]
assert 'cop1lc' not in body and 'copjmp' not in body
# Only instrument MMIO; run the production function unchanged otherwise.
body=re.sub(r'hardware->dmacon=([^;]+);',r'writeDma(\1);',body)
title=(ROOT/'src/title.c').read_text()
a=title.index('static void startReadyPreview')
b=title.index('void titleRunLevelReadyMenu',a)
preview=title[a:b]
assert preview.count('platformReleaseForLoading(TRUE)')==2
assert preview.count('platformResumeMenuAfterLoading()')==2
assert 'platformFinishTakeover(' not in preview
for function in preview.split('static void ')[1:]:
    assert function.index('platformReleaseForLoading(TRUE)')<function.index('platformResumeMenuAfterLoading()')
shim=r'''
#include <assert.h>
#include <stdio.h>
typedef unsigned short UWORD;typedef int BOOL;
#define TRUE 1
#define PORT2_CD32_RESET_HIGH 0x3000
#define DMAF_AUDIO 15
#define DMAF_DISK 16
#define DMAF_SPRITE 32
#define DMAF_BLITTER 64
#define DMAF_COPPER 128
#define DMAF_RASTER 256
#define DMAF_MASTER 512
#define DMAF_SETCLR 32768
/* Deliberately no Copper/bitplane registers: resume cannot write them. */
static struct {UWORD potgo,dmaconr,intena;} registers,*hardware=&registers;
static int systemLocked,interruptsDisabled,step,playing,active,writes;
static UWORD dma,retainedMusic;
static void OwnBlitter(void){assert(step++==0);}
static void WaitBlit(void){assert(step++==1);}
static void Forbid(void){assert(step++==2);}
static void Disable(void){assert(step++==3&&systemLocked);}
static BOOL musicIsPlaying(void){assert(step==4&&interruptsDisabled);return playing;}
static void audioSetHardwareActive(BOOL on){assert(step==4&&on);active=1;}
static void writeDma(UWORD value){
 assert(step==4&&systemLocked&&interruptsDisabled);
 assert(!(value&(DMAF_COPPER|DMAF_RASTER|DMAF_MASTER)));
 if(value&DMAF_SETCLR)dma|=value&32767;
 else {assert(!(value&retainedMusic));dma&=~value;}
 assert((dma&(DMAF_COPPER|DMAF_RASTER|DMAF_MASTER))==(DMAF_COPPER|DMAF_RASTER|DMAF_MASTER));
 writes++;
}
'''
checks=r'''
int main(void){int music,mask;
 for(music=0;music<2;music++)for(mask=0;mask<16;mask++){
  systemLocked=interruptsDisabled=step=active=writes=0;playing=music;
  dma=DMAF_MASTER|DMAF_COPPER|DMAF_RASTER|DMAF_DISK|DMAF_SPRITE|mask;
  hardware->dmaconr=dma;retainedMusic=music?mask:0;
  platformResumeMenuAfterLoading();
  assert(systemLocked&&interruptsDisabled&&active&&writes==2);
  assert(hardware->potgo==PORT2_CD32_RESET_HIGH&&hardware->intena==32767);
  assert(dma==(DMAF_MASTER|DMAF_COPPER|DMAF_RASTER|DMAF_BLITTER|retainedMusic));
 }
 puts("PASS: retained display DMA/Copper, 32 LSP DMA cases, reacquired ownership and both soundtest reload call sites");
}
'''
with tempfile.TemporaryDirectory() as td:
    path=Path(td);(path/'test.c').write_text(shim+body+checks)
    subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-fsanitize=address,undefined',str(path/'test.c'),'-o',str(path/'test')],check=True)
    subprocess.run([str(path/'test')],check=True)
