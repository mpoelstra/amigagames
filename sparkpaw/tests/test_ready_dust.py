"""Actual particle renderer: mask coverage, restore parity, all menu states."""
from pathlib import Path
import ctypes,subprocess,sys,tempfile
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'tools'))
from generate_ready_dust_mask import mask,decode
masks=[mask(i) for i in range(12)];allowed=masks[0];source=(ROOT/'src/ready_dust_mask.h').read_text()
assert bytes(int(v) for v in source.split('{',1)[1].split('}',1)[0].replace('\n','').split(',') if v.strip())==allowed
class History(ctypes.Structure):
 _fields_=[('count',ctypes.c_ubyte),('offset',ctypes.c_ushort*48),('original',(ctypes.c_ubyte*6)*48)]
with tempfile.TemporaryDirectory() as td:
 lib=Path(td)/'dust.dylib';subprocess.run(['cc','-shared','-fPIC','-O2','-I'+str(ROOT/'src'),str(ROOT/'src/ready_dust.c'),'-o',str(lib)],check=True)
 lib=ctypes.CDLL(str(lib));u8=ctypes.c_ubyte;ptr=ctypes.POINTER(u8)
 lib.readyDustDraw.argtypes=[ctypes.POINTER(ptr),ctypes.POINTER(History),ctypes.c_ulong,ptr,ctypes.c_ubyte]
 lib.readyDustRestore.argtypes=[ctypes.POINTER(ptr),ctypes.POINTER(History)]
 raw=(ROOT/'assets/runtime/sparkpaw-ready-screen.spbm').read_bytes();base=raw[204:];w,h,palette,pixels=decode('sparkpaw-ready-screen.spbm')
 pens=(u8*6)(*[min(range(1,64),key=lambda p:sum((palette[p][c]-color[c])**2 for c in range(3))) for color in [(60,90,105),(88,132,148),(125,173,185),(174,211,216),(220,238,232),(254,158,2)]])
 buffers=[(u8*10240).from_buffer_copy(base[p*10240:(p+1)*10240]) for p in range(6)];planes=(ptr*6)(*[ctypes.cast(p,ptr) for p in buffers]);history=History();seen=0
 for frame in range(1600):
  lib.readyDustRestore(planes,ctypes.byref(history));assert b''.join(bytes(p) for p in buffers)==base
  lib.readyDustDraw(planes,ctypes.byref(history),frame,pens,(frame//137)%12);allowed=masks[(frame//137)%12];assert history.count<=48
  if frame%37==0:
   for p in range(6):
    for i,b in enumerate(bytes(buffers[p])):
     assert not ((b^base[p*10240+i])&~allowed[i])
     if b!=base[p*10240+i]:assert i//40 in (108,122,145,168,182,210,228,115,136,157,196,218)
  seen+=history.count
 lib.readyDustRestore(planes,ctypes.byref(history));assert b''.join(bytes(p) for p in buffers)==base
 assert seen>1000
 # Preview built from the actual C-rendered planes, not a mock effect.
 lib.readyDustDraw(planes,ctypes.byref(history),600,pens,0)
 from PIL import Image
 im=Image.new('RGB',(320,256));im.putdata([palette[sum(((buffers[p][y*40+x//8]>>(7-x%8))&1)<<p for p in range(6))] for y in range(256) for x in range(320)])
 im.resize((960,768),Image.Resampling.NEAREST).save(ROOT/'build/ready-dust-preview.png')
print('PASS: 1600 particle frames, exact restoration, masked writes and generated mask parity')
