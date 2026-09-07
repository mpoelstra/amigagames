"""Actual C differential patch must match full patch for all 144 transitions."""
from pathlib import Path
import ctypes as c, subprocess,tempfile
root=Path(__file__).resolve().parents[1]
raw=(root/'assets/runtime/readymenu.spbm').read_bytes()[204:]
assert len(raw)==6*24*104*12
u8=c.c_ubyte;ptr=c.POINTER(u8)
sources=[(u8*(24*104*12)).from_buffer_copy(raw[p*24*104*12:(p+1)*24*104*12]) for p in range(6)]
src=(ptr*6)(*[c.cast(p,ptr) for p in sources])
def full(state):
 planes=[bytearray([0xA5])*10240 for _ in range(6)]
 for p in range(6):
  for y in range(104):planes[p][(118+y)*40+8:(118+y)*40+32]=bytes(sources[p][(state*104+y)*24:(state*104+y+1)*24])
 return planes
with tempfile.TemporaryDirectory() as td:
 lib=Path(td)/'patch.dylib'
 subprocess.run(['cc','-shared','-fPIC','-O2','-Wall','-Wextra','-Werror',str(root/'src/ready_patch.c'),'-o',str(lib)],check=True)
 dll=c.CDLL(str(lib));dll.readyPatchPrepare.argtypes=[c.POINTER(ptr)];dll.readyPatchApply.argtypes=[c.POINTER(ptr),c.POINTER(ptr),u8,u8]
 dll.readyPatchPrepare(src)
 for a in range(12):
  for b in range(12):
   buf=[(u8*10240).from_buffer_copy(p) for p in full(a)];dst=(ptr*6)(*[c.cast(p,ptr) for p in buf])
   dll.readyPatchApply(dst,src,a,b)
   assert [bytes(p) for p in buf]==[bytes(p) for p in full(b)],(a,b)
 for a,b in [(0,1),(1,0)]:
  copied=0
  for y in range(104):
   xs=[x for x in range(24) if any(sources[p][(a*104+y)*24+x]!=sources[p][(b*104+y)*24+x] for p in range(6))]
   if xs:copied+=(((max(xs)+2)&~1)-(min(xs)&~1))*6
  print(f'Menu {a}->{b}: {copied} bytes versus 14976 full-patch bytes')
print('PASS: actual C patch parity for all 144 transitions, including surrounding bytes')
