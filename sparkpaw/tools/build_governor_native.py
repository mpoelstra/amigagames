"""Integrate reviewed tower into proof; all animation stays in32x64 patches."""
from pathlib import Path
from PIL import Image,ImageDraw
from preview_drowned_native import decode
from generate_runtime_assets import save_spbm
R=Path(__file__).resolve().parents[1];O=R/'build/drowned-governor';A=O/'assets'
f=decode(A/'drowned-route.spbm');pal=[tuple(f.getpalette()[i:i+3]) for i in range(0,48,3)]
n=Image.open(R/'assets/concept/drowned-governor-native-v1/tower-indexed.png')
# Lower the complete port/fuse assembly12px to match standing projectile centre
# (floor200,body40,spawn+15,projectile centre+2 =>177). Keep tower feet/scale.
block=n.crop((25,76,55,115));d=ImageDraw.Draw(n);d.rectangle((25,76,54,114),fill=8)
d.line((26,76,26,114),fill=9);d.line((53,76,53,114),fill=1);n.paste(block,(25,88))
frames=[]
# Preserve semantic states0..5; append7opening and21closing frames.
spec=[(0,3,False),(0,3,True),(8,3,False),(8,2,False),(8,1,False),(8,0,False)]
spec +=[(o,3,True) for o in range(1,8)]
spec +=[(o,3-damage,True) for damage in range(3) for o in range(7,0,-1)]
mask=Image.new('L',(80,128));ImageDraw.Draw(mask).ellipse((32,96,47,112),fill=255)
for opening,hits,warning in spec:
 im=n.copy();d=ImageDraw.Draw(im)
 for y in range(96,113):
  for x in range(32,48):
   if mask.getpixel((x,y)) and opening<8 and abs(y-104)>=opening:
    yy=y+opening if y<104 else y-opening
    im.putpixel((x,y),[9,10,9,8,1][(yy-96)%5])
 d.rectangle((38,69,41,72),fill=3 if warning else 9)
 for j in range(3):
  d.rectangle((31+j*5,119,36+j*5,125),fill=8,outline=9)
  d.rectangle((33+j*5,121,34+j*5,123),fill=15 if j<hits else 8)
 if hits==0:
  for y in range(96,113):
   for x in range(32,48):
    if mask.getpixel((x,y)):im.putpixel((x,y),9)
  d.line((35,105,38,108,45,100),fill=6)
 frames.append(im)
for x,y in [(232,72),(488,24),(744,72)]:
 maski=Image.frombytes('L',n.size,bytes(255 if p else 0 for p in n.tobytes()))
 f.paste(n,(x,y),maski)
# Reuse the same deck and copper-bearing supports as the precision route.
material=Image.open(R/'assets/concept/drowned-polish-native-v1/front-indexed.png')
f.paste(0,(432,152,576,200))
for x in range(432,576,16):
 f.paste(material.crop((16,160,32,168)),(x,152))
 # Maintain the existing solid16px cap with approved structural texture.
 f.paste(material.crop((16,168,32,176)),(x,160))
for x in (448,552):
 f.paste(material.crop((0,168,16,200)),(x,168))
gate=decode(R/'build/drowned-slice/assets/drowned-front.spbm')
f.paste(gate.crop((768,112,864,200)),(832,112))
# Prepend448px authored Walker approach, retaining accepted arena geometry.
old=f.crop((0,0,960,200));f.paste(0,(0,0,1408,200));f.paste(old,(448,0))
for x in range(192,384,16):
 f.paste(material.crop((16,160,32,176)),(x,144))
for x in (208,352):
 for y in range(160,200,16):f.paste(material.crop((0,168,16,168+min(16,200-y))),(x,y))
# The animated jet patch ends at194; retain the static nozzle beneath it.
nozzle=decode(R/'assets/concept/drowned-geyser-native-v2/lip.spbm')
nozzle_mask=Image.frombytes('L',nozzle.size,bytes(255 if p else 0 for p in nozzle.tobytes()))
for x in (832,1088):f.paste(nozzle,(x,194),nozzle_mask)
save_spbm(A/'drowned-route.spbm',f,pal,4)
assert decode(A/'drowned-route.spbm').tobytes()==f.tobytes()
# Preserve exit prototype art for now; give it its own six cells after tower34.
for state in range(6):
 im=Image.new('P',(32,64));im.putpalette(n.getpalette());d=ImageDraw.Draw(im)
 d.rectangle((0,0,31,63),fill=8,outline=10)
 if state!=5:
  for y in range(3,60,8):d.rectangle((3,y,28,y+5),fill=9,outline=1)
 else:d.line((7,32,24,32,19,27,24,32,19,37),fill=6,width=2)
 frames.append(im)
packed=[]
for i,im in enumerate(frames):
 patch=im.crop((24,64,56,128)) if i<34 else im
 packed.append([[sum(((patch.getpixel((x+k,y))>>p)&1)<<(7-k) for k in range(8)) for y in range(64) for x in range(0,32,8)] for p in range(4)])
def c(v):return '{'+','.join(c(x) if isinstance(x,list) else str(x) for x in v)+'}'
(O/'governor_art.h').write_text('static const unsigned char governorArt[40][4][256]='+c(packed)+';\n')
# Display an actual native-sized composition and state loop before packaging.
rear=decode(A/'drowned-rear.spbm').convert('RGB');bg=rear.crop((32,0,352,208));fg=f.crop((128,0,448,208));bg.paste(fg.convert('RGB'),(0,0),Image.frombytes('L',fg.size,bytes(255 if p else 0 for p in fg.tobytes())));bg.resize((1280,832),Image.Resampling.NEAREST).save(O/'native-scene.png')
print('Reviewed tower integrated;40x1024 Fast art bytes, same610Chipstage; port y168..184 includes standing shot177.')
