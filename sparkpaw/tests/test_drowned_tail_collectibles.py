"""Actual collectible init/pickup/respawn, using the generated full-route table."""
from pathlib import Path
import json,subprocess,tempfile
R=Path(__file__).resolve().parents[1];O=R/'build/drowned-full'
m=json.loads((O/'manifest.json').read_text())
assert len(m['coins'])==45 and len(m['tail_coins'])==12
assert max(sum(x<=c[0]<x+320 for c in m['tail_coins']) for x in range(3200,5120))<=4
for x,y in m['tail_coins']:
 assert x>=3200 and x+16<4888
# Original station composition stays unchanged; both plants stand on dry ground.
for x,w,h,species in m['flowers']:
 assert not any(x<wx+80 and x+w>wx for wx in m['water'])
assert {p[3] for p in m['flowers']}=={0,1}
c=r"""
#include <assert.h>
#include "collectibles.h"
#include "drowned_route_coins.h"
int main(void){
 int i,n=0;collectiblesInit();
 for(i=0;i<48;i++)n+=collectibleAt(i)->active;
 assert(n==45);
 for(i=33;i<45;i++){
  struct Collectible *c=collectibleAt(i);int x=c->x,y=c->y;
  assert(x==routeCoins[i][0]&&y==routeCoins[i][1]);
  assert(collectiblesCollect(x,y,x+15,y+20)==1);
  assert(collectiblesCollect(x,y,x+15,y+20)==0);
 }
 collectiblesResetPreservingProgress();
 for(i=0;i<48;i++)assert(!!collectibleAt(i)->active==(i<33));
 return 0;
}
"""
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'exec').mkdir()
 (p/'exec/types.h').write_text('typedef short WORD;typedef unsigned char UBYTE;typedef int BOOL;\n#define TRUE 1\n#define FALSE 0\n')
 (p/'proof.c').write_text(c)
 subprocess.run(['cc','-O2','-fsanitize=address,undefined','-DSPARKPAW_DROWNED_FULL','-DSPARKPAW_DROWNED_GOVERNOR','-DSPARKPAW_DROWNED_ROUTE','-I',str(p),'-I',str(R/'src'),'-I',str(O),str(p/'proof.c'),str(R/'src/collectibles.c'),'-o',str(p/'proof')],check=True)
 subprocess.run([str(p/'proof')],check=True)
print('PASS:45active/12new, max4tail diamonds per screen, collectible pickup/no-repeat/respawn, dry flower roots.')
