"""Offline fixed-pivot Spillwing animation audition; no runtime changes."""
from pathlib import Path
import json,math,hashlib
from PIL import Image,ImageDraw
R=Path(__file__).resolve().parents[1];O=R/'assets/enemies/spillwing-actions-review-v1';O.mkdir(exist_ok=True)
source=R/'assets/enemies/spillwing-idle-review-v1/spillwing-indexed.png';idle=Image.open(source);pal=idle.getpalette()
def blank(size=(24,24)):
 im=Image.new('P',size);im.putpalette(pal);return im
# Keep all approved body pixels except one clearer sensor highlight.
body=idle.copy();body.paste(0,(0,0,24,7));body.putpixel((22,14),15)
hub=blank();hub.paste(idle.crop((8,3,14,7)),(8,3))
rotors=[]
for n in (0,1,2,3):
 im=hub.copy()
 if n in (0,3):
  im.paste(idle.crop((0,4,8,7)),(0,4));im.paste(idle.crop((14,4,24,7)),(14,4))
  if n==3:
   for x in (2,3,18,19):im.putpixel((x,4),10)
 else:
  # Fixed hub, projected blade lengths change as rotor turns; no actor scaling.
  l=6 if n==1 else 2
  d=ImageDraw.Draw(im);d.line((11-l,5,11+l,5),fill=9,width=1)
  if n==1:d.point((11-l,4),fill=10);d.point((11+l,6),fill=10)
 rotors.append(im)
def mask(im):return Image.frombytes('L',im.size,bytes(255 if p else 0 for p in im.tobytes()))
def over(dst,im,xy=(0,0)):dst.paste(im,xy,mask(im))
def pose(rot,angle=0,warning=False):
 # Pad before nearest-neighbour fixed-pivot rotation; assert no crop loss.
 big=blank((72,72));over(big,body,(24,24));big=big.rotate(-angle,Image.Resampling.NEAREST,center=(35,31))
 bb=mask(big).getbbox();assert bb[0]>=24 and bb[1]>=24 and bb[2]<=48 and bb[3]<=48,(angle,bb)
 im=big.crop((24,24,48,48));over(im,rotors[rot])
 if warning:
  # Accent remains on the unrotated warning sensor; no flashing whole silhouette.
  im.putpixel((22,14),11)
 return im
families={
 'fly':[pose(i) for i in range(4)],
 'warn':[pose(1,0,True),pose(2,0,False)],
 'dive':[pose(0,12),pose(2,12)],
 'recover':[pose(1,8),pose(3,0)],
 'hit':[pose(0),pose(2)]}
for i,im in enumerate(families['hit']):
 d=ImageDraw.Draw(im);d.line([(6,9),(8,8),(7,11)],fill=6 if i else 11)
 d.point((20,9),fill=11)
# Four compact machine failures: rotor breaks, sparse electric arcs, falling parts.
death=[]
for f in range(4):
 im=blank();d=ImageDraw.Draw(im)
 if f<2:
  over(im,body,(0,f));over(im,rotors[0],(0,-f))
  d.line([(8,10),(10,8),(9,12),(12,11)],fill=11 if f==0 else 6)
  d.line([(18,16),(20,13),(19,18)],fill=6)
 else:
  for box,xy in [((9,9,17,17),(7,13+f-2)),((18,11,24,18),(17,13+f)),((1,10,8,16),(2,12+f))]:
   part=body.crop(box);over(im,part,xy)
  d.point((12,8+f),fill=6);d.point((5,10+f),fill=11 if f==2 else 6)
  d.line((15,11+f,17,10+f),fill=6)
 death.append(im)
families['death']=death
sheet=blank((24*4,24*6))
for row,(name,frames) in enumerate(families.items()):
 for i,im in enumerate(frames):
  assert im.size==(24,24) and im.getextrema()[1]<16
  im.save(O/f'{name}-{i}.png',transparency=0);sheet.paste(im,(i*24,row*24))
sheet.save(O/'sheet-indexed.png',transparency=0)
# 25Hz preview samples of proposed timing; no emulator or physics integration.
scene=[]
for t in range(100):
 im=Image.new('RGB',(240,76),(17,17,34));d=ImageDraw.Draw(im)
 for x,label in [(6,'FLIGHT'),(86,'DIVE'),(170,'FAILURE')]:d.text((x,3),label,fill=(153,170,187))
 over(im,families['fly'][(t//2)%4],(24,28+(t//8)%2))
 if t<20:art=families['fly'][(t//2)%4];x,y=88,24
 elif t<26:art=families['warn'][t%2];x,y=88,24
 elif t<50:
  art=families['dive'][t%2];u=(t-26)/24;x=88+round(u*40);y=24+round(math.sin(u*math.pi)*19)
 elif t<58:art=families['recover'][(t-50)//4];x,y=128,24
 else:art=families['fly'][(t//2)%4];x,y=128,24
 over(im,art,(x,y))
 if t<25:art=families['fly'][(t//2)%4]
 elif t<31:art=families['hit'][(t-25)//3]
 elif t<55:art=families['death'][(t-31)//6]
 else:art=None
 if art:over(im,art,(192,28))
 scene.append(im.resize((1200,380),Image.Resampling.NEAREST))
scene[0].save(O/'actions-5x.gif',save_all=True,append_images=scene[1:],duration=40,loop=0,disposal=2)
(O/'manifest.json').write_text(json.dumps({'status':'offline animation review; no runtime/AI changes','source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cell':[24,24],'families':{k:len(v) for k,v in families.items()},'body_pivot':[11,7],'rotor_hub':[11,5],'warning_preview_ms':240,'native_pixel_scale_fixed':True,'damage_proposal':'one hit kills; hit preview is optional electrical transition, not multiple HP','notes':'nearest-neighbour offline part rotation; no runtime transforms; schematic attack path only; native020 not tested'},indent=2)+'\n')
print('16 native frames in fixed24x24 cells and motion/death preview.')
