"""Precompute exact static ferry flight predicates; 8 flags per world pixel."""
from pathlib import Path
import re,sys
R=Path(__file__).resolve().parents[1];O=R/'build/drowned-ferry'
if len(sys.argv)>1: O=Path(sys.argv[1])
tiles=(O/'assets/drowned-route.bin').read_bytes();world=len(tiles)//14*16;cols=world//16
arc=list(map(int,re.search(r'arc\[32\]=\{([^}]+)',(R/'src/spillwing.c').read_text()).group(1).replace('\n','').split(',')));assert len(arc)==32
def row(x,y):
 if x<0 or x+23>=world or y<0:return True
 return any(tiles[(y//16)*cols+xx//16] for xx in range(x,x+24))
def box(x,y):return row(x,y) or row(x,y+12) or row(x,y+23)
out=[]
for x in range(world):
 bits=0
 for high in range(2):
  base=96 if high else 120;speed=4 if high else 3
  for left in range(2):
   clear=all(not box(x+(-speed if left else speed)*(i+1),base+a+(a//2 if high else 0)) for i,a in enumerate(arc))
   if clear:bits|=1<<(high*2+(0 if left else 1))
  for bob in range(2):
   if not box(x,base+bob):bits|=1<<(4+high*2+bob)
 out.append(bits)
(O/'spillwing_clearance.h').write_text('/* Generated from final ferry collision map; bits: lowL/R,highL/R,low0/1,high0/1. */\nstatic const UBYTE spillwingClearance[]={'+','.join(map(str,out))+'};\n')
print(f'Flight clearance:{world} bytes, static normal/Fast data; no Chip allocation.')
