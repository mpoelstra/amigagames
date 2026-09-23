"""Native palette conversion and deterministic shutter motion review only."""
from pathlib import Path
from PIL import Image,ImageDraw
from preview_drowned_native import decode
from generate_runtime_assets import save_spbm
R=Path(__file__).resolve().parents[1];O=R/'assets/concept/drowned-governor-native-v1';O.mkdir(exist_ok=True)
f=decode(R/'build/drowned-joined/assets/drowned-route.spbm');pal=[tuple(f.getpalette()[i:i+3]) for i in range(0,48,3)]
s=Image.open(R/'assets/concept/drowned-governor-tower-source-v1.png').convert('RGBA')
box=s.getchannel('A').point(lambda x:255 if x>200 else 0).getbbox();s=s.crop(box);s=s.resize((round(s.width*128/s.height),128),Image.Resampling.BOX)
n=Image.new('P',(80,128));n.putpalette(f.getpalette());off=(80-s.width)//2
for y in range(128):
 for x in range(s.width):
  r,g,b,a=s.getpixel((x,y))
  if a<128:continue
  candidates=[1,8,9,10,11,12,13,14,15,5,6,3,4]
  idx=min(candidates,key=lambda p:sum((v-w)**2 for v,w in zip((r,g,b),pal[p])))
  n.putpixel((x+off,y),idx)
n.save(O/'tower-indexed.png');n.resize((480,768),Image.Resampling.NEAREST).save(O/'tower-6x.png')
save_spbm(O/'tower.spbm',n,pal,4)
assert decode(O/'tower.spbm').tobytes()==n.tobytes()
print('native80x128,16pen palette; source crop',box,'scaled',s.size)
# Source port footprint fits the already tested32x64 dynamic patch.
# Two solid shutter halves retract vertically within the recessed steel ring.
mask=Image.new('L',n.size);md=ImageDraw.Draw(mask);md.ellipse((32,84,47,100),fill=255)
frames=[]
for opening,hits in [(0,3)]*8+[(i,3) for i in range(1,9)]+[(8,3)]*8+[(8,2)]*5+[(8,1)]*5+[(8,0)]*8:
 im=n.copy();d=ImageDraw.Draw(im)
 for y in range(84,101):
  for x in range(32,48):
   if not mask.getpixel((x,y)):continue
   if opening<8 and abs(y-92)>=opening:
    yy=y+opening if y<92 else y-opening
    im.putpixel((x,y),[9,10,9,8,1][(yy-84)%5])
 for j in range(3):
  d.rectangle((31+j*5,107,36+j*5,113),fill=8,outline=9)
  d.rectangle((33+j*5,109,34+j*5,111),fill=15 if j<hits else 8)
 if hits==0:
  for y in range(84,101):
   for x in range(32,48):
    if mask.getpixel((x,y)) and im.getpixel((x,y)) in (5,6,11):im.putpixel((x,y),9)
  d.line((35,93,38,96,45,88),fill=6,width=1)
 frames.append(im)
# Only the fixed patch changes across the whole family; no body jitter.
for im in frames:
 for y in range(128):
  for x in range(80):
   if not (24<=x<56 and 64<=y<128):assert im.getpixel((x,y))==n.getpixel((x,y))
scaled=[im.resize((320,512),Image.Resampling.NEAREST).convert('RGB') for im in frames]
scaled[0].save(O/'shutters-preview.gif',save_all=True,append_images=scaled[1:],duration=80,loop=0)
review=Image.new('RGB',(80*5,128))
for i,k in enumerate([0,11,16,25,36]):review.paste(frames[k].convert('RGB'),(i*80,0))
review.resize((1600,512),Image.Resampling.NEAREST).save(O/'states-4x.png')
# Native gameplay-sized composition, actual rear art and existing floor strip.
rear=decode(R/'build/drowned-joined/assets/drowned-rear.spbm').convert('RGB')
scene=rear.crop((32,0,352,208));fg=Image.new('P',(320,208));fg.putpalette(f.getpalette())
for x in range(0,320,16):fg.paste(f.crop((0,200,16,208)),(x,200))
fg.paste(n,(104,72));alpha=fg.point(lambda v:255 if v else 0).convert('L')
# Explicit index mask; palette luminance must not affect transparency.
alpha=Image.frombytes('L',fg.size,bytes(255 if v else 0 for v in fg.tobytes()))
scene.paste(fg.convert('RGB'),(0,0),alpha);scene.resize((1280,832),Image.Resampling.NEAREST).save(O/'scene-4x.png')
print('Native shutter frames: fixed32x64 patch bounds verified, all body pixels stable.')
