from pathlib import Path
import shutil
from PIL import Image
from preview_drowned_native import decode
from generate_runtime_assets import save_spbm
R=Path(__file__).resolve().parents[1];O=R/'build/drowned-pontoon';A=O/'assets';A.mkdir(parents=True,exist_ok=True)
for p in (R/'build/drowned-route/assets').iterdir():
 if p.is_file():shutil.copy2(p,A/p.name)
f=decode(A/'drowned-route.spbm');pal=[tuple(f.getpalette()[i:i+3]) for i in range(0,48,3)]
f.paste(0,(0,0,2400,208))
material=Image.open(R/'assets/concept/drowned-polish-native-v1/front-indexed.png')
for x in list(range(0,160,32))+list(range(960,2400,32)):
 f.paste(material.crop((160,200,192,208)),(x,200))
save_spbm(A/'drowned-route.spbm',f,pal,4)
(A/'drowned-route.bin').write_bytes(bytes(2400//16*14))
# No collectibles in isolated basin proof; inactive sentinel beyond world.
(O/'drowned_route_coins.h').write_text('static const WORD routeCoins[][2]={{32,32}};\n')
shutil.copy2(R/'build/drowned-route/drowned_route_layout.h',O/'drowned_route_layout.h')
im=Image.open(R/'assets/concept/drowned-pontoon-native-v1/pontoon-indexed.png');words=[]
for plane in range(5):
 for y in range(16):
  for word in range(7):
   bits=0
   for bit in range(16):
    x=word*16+bit;p=im.getpixel((x,y)) if x<96 else 0
    if (p!=0 if plane==0 else p&(1<<(plane-1))):bits|=1<<(15-bit)
   words.append(bits)
(O/'drowned_pontoon_art.h').write_text('static const UWORD pontoonArt[560]={'+','.join(map(str,words))+'};\n')
print('Pontoon basin assets and1120-byte native mask/planes generated.')
# Compile immutable water masks offline; no multi-million-pixel Amiga startup loop.
import ctypes,subprocess,tempfile,struct
s=(R/'src/renderer.c').read_text();start=s.index('static UBYTE waterPatternPen');end=s.index('static BOOL buildWaterPatterns',start)
c=s[start:end].replace('static UBYTE waterPatternPen','UBYTE waterPatternPen')
with tempfile.TemporaryDirectory() as td:
 td=Path(td);(td/'water.c').write_text('typedef unsigned char UBYTE;typedef short WORD;\n'+c)
 subprocess.run(['cc','-shared','-fPIC',str(td/'water.c'),'-o',str(td/'water.so')],check=True)
 lib=ctypes.CDLL(str(td/'water.so'));fn=lib.waterPatternPen;fn.argtypes=[ctypes.c_ubyte,ctypes.c_short,ctypes.c_short];fn.restype=ctypes.c_ubyte
 packed=[]
 for bob in range(2):
  for frame in range(16):
   for offset in range(80):
    out=words[56:112].copy()
    for y in range(8,16):
     for x in range(96):
      if fn(frame,(offset+x)%80,189+bob+y-197):out[(y-8)*7+x//16]&=~(0x8000>>(x%16))
    packed.extend(out)
 (A/'pontoon-clip.bin').write_bytes(struct.pack('>'+str(len(packed))+'H',*packed))
 assert (A/'pontoon-clip.bin').stat().st_size==286720
 print('Precomputed286720-byte big-endian waterline mask asset.')
