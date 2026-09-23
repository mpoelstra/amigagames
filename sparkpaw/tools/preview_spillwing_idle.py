"""Concept-scale audition, not an approved runtime frame or animation."""
from pathlib import Path
import json
from PIL import Image
from preview_drowned_native import decode
R=Path(__file__).resolve().parents[1];O=R/'assets/enemies/spillwing-idle-review-v1';O.mkdir(exist_ok=True)
s=Image.open(R/'assets/enemies/spillwing-concept-v1.png').convert('RGBA')
b=s.getchannel('A').point(lambda a:255 if a>=128 else 0).getbbox();assert b
s=s.crop(b);scale=min(24/s.width,22/s.height);s=s.resize((round(s.width*scale),round(s.height*scale)),Image.Resampling.BOX)
f=decode(R/'build/drowned-route/assets/drowned-route.spbm');pal=[tuple(f.getpalette()[i:i+3]) for i in range(0,48,3)]
cell=Image.new('P',(24,24));cell.putpalette(f.getpalette());at=((24-s.width)//2,(24-s.height)//2)
for y in range(s.height):
 for x in range(s.width):
  rr,g,bb,a=s.getpixel((x,y))
  if a<128:continue
  choices=[1,12,13,14,15] if rr>bb*1.18 and rr>g*1.08 else [1,8,9,10,11]
  pen=min(choices,key=lambda p:sum((c-pal[p][i])**2 for i,c in enumerate((rr,g,bb))))
  cell.putpixel((at[0]+x,at[1]+y),pen)
cell.save(O/'spillwing-indexed.png',transparency=0)
def mask(im):return Image.frombytes('L',im.size,bytes(255 if p else 0 for p in im.tobytes()))
def paste(im,part,xy):im.paste(part.convert('RGB'),xy,mask(part))
scene=Image.open(R/'assets/concept/drowned-pontoon-native-v1/scene-1x.png').convert('RGB');paste(scene,cell,(204,149))
scene.save(O/'scale-scene-1x.png');scene.resize((1280,1024),Image.Resampling.NEAREST).save(O/'scale-scene-4x.png')
strip=Image.new('RGB',(168,64),(17,17,34));paste(strip,cell,(120,15));boat=Image.open(R/'assets/concept/drowned-pontoon-native-v1/pontoon-indexed.png');paste(strip,boat,(8,48));player=decode(R/'assets/runtime/sparkpaw-sprites4.spbm').crop((0,0,48,48));paste(strip,player,(30,48-mask(player).getbbox()[3]));strip.resize((1008,384),Image.Resampling.NEAREST).save(O/'scale-6x.png')
(O/'manifest.json').write_text(json.dumps({'status':'provisional native size audition, no animation or runtime integration','source_bbox':b,'cell':[24,24],'opaque_bbox':mask(cell).getbbox(),'palette':pal,'next':'concept approval then refine native clusters and fixed-pivot rotor/swoop/death animation'},indent=2)+'\n')
print('24x24 size audition saved.')
