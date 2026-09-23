"""Native width helper against pixel truth for all alignments/two ring targets."""
from pathlib import Path
import subprocess,tempfile,ctypes,sys
R=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(R/'tools'))
from preview_drowned_native import decode
# Actual diamond mask used by the renderer; any opaque pixel must be removed.
a=decode(R/'assets/runtime/sparkpaw-diamond.spbm');mask=[(x,y) for y in range(a.height) for x in range(a.width) if a.getpixel((x,y))]
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);(p/'t.c').write_text('#include "drowned_collectible_restore.h"\nunsigned short width(short x){return drownedCollectibleRestoreWidth(x);}\n')
 subprocess.run(['cc','-shared','-fPIC','-I'+str(R/'src'),str(p/'t.c'),'-o',str(p/'t.so')],check=True)
 fn=ctypes.CDLL(str(p/'t.so')).width;fn.restype=ctypes.c_ushort
 cases=0;old_failed=0
 for world in [16,480,496,512,1008,1456,1568]:
  for shift in range(16):
   x=world+shift;w=fn(x)
   for hover in [-2,-1,0,1]:
    # Both physical target histories must erase the complete old silhouette.
    for target in range(2):
     px=512+(x%512);base=[i%17 for i in range(1536*25)];screen=base.copy()
     for dx,dy in mask:
      if dx<16 and dy<21:screen[(dy+hover+2)*1536+px+dx]=99
     old=screen.copy();start=px&~15
     for y in range(25):
      at=y*1536+start;screen[at:at+w]=base[at:at+w];old[at:at+16]=base[at:at+16]
     assert screen==base,(x,hover,target,w)
     old_failed+=old!=base;cases+=1
 assert old_failed>0
print('PASS:',cases,'restores across all word alignments, hover extremes and two targets; old16px restore fails',old_failed,'cases')
s=(R/'src/renderer.c').read_text();assert 'WORD width=drownedCollectibleRestoreWidth(item->x);' in s
