"""Exercise actual diagnostic entry, including snapshot clear and prior position."""
from pathlib import Path
import subprocess,tempfile
R=Path(__file__).resolve().parents[1]
s=(R/'src/renderer.c').read_text();a=s.index('void rendererDiagnosticUpdateEntry(UWORD line)');b=s.index('\nvoid rendererDiagnosticPublicationEntry',a);fn=s[a:b]
pre=r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
typedef uint32_t ULONG;typedef int32_t LONG;typedef uint16_t UWORD;typedef int16_t WORD;typedef uint8_t UBYTE;typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define DIAG_MISSED_100 1
struct Snapshot {ULONG gameFrame,updateStamp,flags;LONG playerX,cameraX;};
static struct Snapshot diagnosticCurrent,diagnosticFrame;
static struct {ULONG frameCounter;LONG cameraX;} gs,*game=&gs;
static struct {LONG x;} player;
#define playerState() (&player)
static ULONG tick;
static ULONG diagnosticSample(UWORD line) {return line;}
static ULONG platformProfileTimerTicks(void) {return tick;}
static ULONG diagnosticPreviousUpdateField,diagnosticCadenceIntervals,diagnosticCadenceFields,diagnosticCadenceOne,diagnosticCadenceTwo,diagnosticCadenceThreePlus,diagnosticCadenceMax;
static BOOL diagnosticHasPreviousUpdate;
#include "drowned_cadence_regions.h"
'''
main=r'''
int main(void) {
 /* Every boundary both directions, then a ferry->checkpoint death teleport. */
 WORD x[]={36,1375,1376,2127,2128,2399,2400,2799,2800,3199,3200,3488,3200,3199,2800,2799,2400,2399,2128,2127,1376,1375,36,3000,2320,2320,2500};
 ULONG expected[6][5]={{0}},sum=0,fields=0,miss=0,longs=0;
 for(unsigned i=0;i<sizeof(x)/sizeof(x[0]);i++) {
  ULONG n=1+i%3;tick+=n*14188;player.x=(LONG)x[i]*256;
  game->cameraX=x[i]-144;game->frameCounter=i;
  if(i) {
   unsigned r=drownedCadenceRegion(x[i-1]);
   expected[r][0]++;expected[r][1]+=n;expected[r][2]+=n>1;expected[r][3]+=n>2;
   if(n>expected[r][4])expected[r][4]=n;
  }
  rendererDiagnosticUpdateEntry(70);
  assert(diagnosticCurrent.playerX==x[i]);
  assert(diagnosticCurrent.gameFrame==i);
 }
 for(int r=0;r<6;r++) {
  struct DrownedCadenceRegion *v=&drownedCadenceRegions[r];
  assert(v->intervals==expected[r][0]&&v->fields==expected[r][1]);
  assert(v->missed==expected[r][2]&&v->longFrames==expected[r][3]&&v->maxFields==expected[r][4]);
  sum+=v->intervals;fields+=v->fields;miss+=v->missed;longs+=v->longFrames;
 }
 assert(sum==diagnosticCadenceIntervals&&fields==diagnosticCadenceFields);
 assert(miss==diagnosticCadenceTwo+diagnosticCadenceThreePlus&&longs==diagnosticCadenceThreePlus);
 assert(diagnosticCadenceMax==3);
 puts("PASS: actual entry clears snapshots, retains prior region through boundaries/respawn; regional/global totals agree");
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);src=p/'test.c';src.write_text(pre+fn+main)
 subprocess.run(['cc','-O2','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_JOINED','-I'+str(R/'src'),str(src),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
 # Mutation proof: this lifecycle test must reject the original zero-position bug.
 src.write_text(pre+fn.replace('drownedCadenceRecord(priorPlayerX,fields);','drownedCadenceRecord((WORD)diagnosticCurrent.playerX,fields);')+main)
 subprocess.run(['cc','-O2','-DSPARKPAW_DROWNED_JOINED','-I'+str(R/'src'),str(src),'-o',str(p/'broken')],check=True)
 result=subprocess.run([str(p/'broken')],capture_output=True)
 assert result.returncode!=0,'test failed to detect original regression'
 print('PASS: original broken attribution rejected by lifecycle test')
