"""Readable mechanical gameplay mockup, not approved final art."""
from pathlib import Path
import shutil
from PIL import Image,ImageDraw
from preview_drowned_native import decode
from generate_runtime_assets import save_spbm
R=Path(__file__).resolve().parents[1];O=R/'build/drowned-governor';A=O/'assets';A.mkdir(parents=True,exist_ok=True)
S=R/'build/drowned-joined'
for p in (S/'assets').iterdir():shutil.copy2(p,A/p.name)
for n in ['drowned_route_layout.h','drowned_route_coins.h','drowned_pontoon_art.h','spillwing_clearance.h']:
 if (S/n).exists():shutil.copy2(S/n,O/n)
f=decode(S/'assets/drowned-route.spbm');pal=[tuple(f.getpalette()[i:i+3]) for i in range(0,48,3)]
out=Image.new('P',f.size);out.putpalette(f.getpalette());draw=ImageDraw.Draw(out)
for x in range(0,f.width,16):out.paste(f.crop((0,200,16,208)),(x,200))
draw.rectangle((432,152,575,167),fill=8);draw.line((432,152,575,152),fill=11,width=2)
draw.rectangle((448,168,455,199),fill=8);draw.rectangle((552,168,559,199),fill=8)
save_spbm(A/'drowned-route.spbm',out,pal,4)
# Force full-height column fallback in this unaccepted prototype.
(O/'drowned_column_tops.h').write_text('static const UBYTE drownedColumnTops[220]={0};\n')
frames=[];sheet=Image.new('P',(6*32,4*64));sheet.putpalette(f.getpalette())
for item in range(4):
 row=[]
 for state in range(6):
  im=Image.new('P',(32,64));im.putpalette(f.getpalette());d=ImageDraw.Draw(im)
  d.rectangle((1,0,30,63),fill=8,outline=10);d.rectangle((4,16,27,47),fill=1)
  d.text((11,1),str(item+1) if item<3 else 'E',fill=11)
  if state in (0,1):
   for y in range(16,48,6):d.rectangle((4,y,27,y+4),fill=10,outline=8)
   d.rectangle((5,54,26,58),fill=3 if state==1 else 9)
  elif state<5:
   d.ellipse((7,20,24,43),fill=6,outline=11);d.line((10,31,21,31),fill=1);d.line((15,24,15,39),fill=1)
   # Shutters retracted to top/bottom, exposed bullseye and remaining-hit lamps.
   for j in range(3):d.rectangle((5+j*8,54,10+j*8,58),fill=11 if j<5-state else 9)
  else:
   d.line((7,31,13,38,25,23),fill=6,width=3)
  sheet.paste(im,(state*32,item*64))
  planes=[]
  for p in range(4):
   planes.append([sum(((im.getpixel((x+k,y))>>p)&1)<<(7-k) for k in range(8)) for y in range(64) for x in range(0,32,8)])
  row.append(planes)
 frames.append(row)
def c(v):return '{'+','.join(c(x) if isinstance(x,list) else str(x) for x in v)+'}'
(O/'governor_art.h').write_text('static const unsigned char governorArt[4][6][4][256]='+c(frames)+';\n')
sheet.resize((768,1024),Image.Resampling.NEAREST).save(O/'targets-review.png')
out.crop((0,0,960,208)).resize((1920,416),Image.Resampling.NEAREST).save(O/'layout-review.png')
print('Governor proof:6 patches, numbered shutter targets, elevated firing perch; art is placeholder.')
