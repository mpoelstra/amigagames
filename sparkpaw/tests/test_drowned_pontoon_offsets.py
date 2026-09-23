"""Exhaustively compare actual prepared offsets with the existing formula."""
from pathlib import Path
import subprocess,tempfile
R=Path(__file__).resolve().parents[1]
pre='''#include <stdint.h>
#include <assert.h>
#include <stdio.h>
typedef int BOOL;typedef int16_t WORD;typedef uint16_t UWORD;typedef int32_t LONG;typedef uint8_t UBYTE;
'''
main=r'''
int main(void) {
 int reset,x,y,f,n=0;LONG expected,at;
 for(reset=0;reset<3;reset++) {
  preparePontoonOffsets();
  for(x=PONTOON_LEFT;x<=PONTOON_RIGHT;x++)for(y=189;y<=190;y++)for(f=0;f<16;f++) {
   expected=(((LONG)(y-189)*16+f)*80+x%80)*56;
   at=pontoonMaskOffset(x,y,f);assert(at==expected);
   assert(at>=0&&at+56<=2L*16*80*56);n++;
  }
 }
 printf("PASS: %d actual offset comparisons, every legal x/bob/frame and repeated preparation; %lu bytes BSS\n",n,(unsigned long)(sizeof(pontoonPositionOffset)+sizeof(pontoonPhaseOffset)));
 return 0;
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'exec').mkdir();(p/'exec/types.h').write_text('')
 (p/'test.c').write_text(pre+'#include "drowned_pontoon.h"\n#include "drowned_pontoon_offsets.h"\n'+main)
 for joined in (False,True):
  subprocess.run(['cc','-O2','-fsanitize=address,undefined',*(['-DSPARKPAW_DROWNED_JOINED'] if joined else []),'-I'+str(p),'-I'+str(R/'src'),str(p/'test.c'),'-o',str(p/'test')],check=True)
  subprocess.run([str(p/'test')],check=True)
