"""Offline attack/hit/death motion study, derived from native idle parts.
No runtime generation, frame-slot changes or gameplay timing claims.
"""
import math,json
from PIL import Image,ImageDraw
from preview_drowned_enemy_walk import IN,ROOT,cut,transform,knee
OUT=ROOT/'assets/enemies/drowned-actions-review-v2'
CYAN=(51,204,238,255);WHITE=(221,238,221,255)

def rotate_part(part,pivot,degrees,shift=(0,0)):
 a=math.radians(degrees)
 return transform(part,pivot,(pivot[0]+10,pivot[1]),
                  (pivot[0]+shift[0],pivot[1]+shift[1]),
                  (pivot[0]+shift[0]+10*math.cos(a),pivot[1]+shift[1]+10*math.sin(a)))

def folded(src,drop,tilt=0,shift_x=0):
 specs=[((34,39),(35,49),(39,59),(28,37,44,51),(29,47,44,58),(28,58,50,63)),
        ((23,39),(18,47),(20,59),(11,37,29,50),(11,46,29,58),(8,58,29,63))]
 out=Image.new('RGBA',src.size)
 for h,k,f,u,l,feet in specs:
  hip=(h[0]+shift_x,h[1]+drop);j=knee(hip,f,math.dist(h,k),math.dist(k,f))
  out.alpha_composite(transform(cut(src,l),k,f,j,f))
  out.alpha_composite(transform(cut(src,u),h,k,hip,j))
  out.alpha_composite(cut(src,feet))
 body=cut(src,(0,0,64,40));out.alpha_composite(rotate_part(body,(32,39),tilt,(shift_x,drop)))
 return out

def walker_actions(src):
 charge=folded(src,2);d=ImageDraw.Draw(charge)
 # Native gauge pressure and unchanged tiny sensor: additions use current bank.
 d.line((20,12,23,9),fill=CYAN,width=1)
 # Recoil travels through hips and knees while the same feet stay planted.
 fire=folded(src,1,shift_x=-1)
 hit=[]
 for dx,drop in [(-1,1),(-2,1),(-2,1),(-1,1),(0,0),(0,0),(0,0)]:
  hit.append(folded(src,drop,shift_x=dx) if dx or drop else src.copy())
 death=[]
 for i,drop in enumerate([1,5,9,12]):
  f=folded(src,drop,i*5);d=ImageDraw.Draw(f)
  # Thin seam across tank, then sensor/gauge energy goes dark.
  d.line((32,22+drop,34,25+drop,33,28+drop),fill=(17,17,17,255),width=1)
  if i<2:d.point((34,24+drop),fill=CYAN)
  if i>=2:
   for y in range(f.height):
    for x in range(f.width):
     r,g,b,a=f.getpixel((x,y))
     if a and b>r*1.5 and g>r*1.5:f.putpixel((x,y),(68,85,85,255))
  # Small local pressure leak, not a replacement explosion.
  if i>0:
   for x,y in [(45,28),(46,27),(46,26),(47,25)][:i+1]:d.point((x,y),fill=WHITE)
  death.append(f)
 return {'idle':[src],'charge':[charge],'fire':[fire],'hit':hit,'death':death}

def crab_actions(src):
 body=cut(src,(0,0,32,19));legs=cut(src,(0,19,32,24));hit=[]
 for dx in [-1,-1,0,0]:
  f=legs.copy();f.alpha_composite(body,(dx,0));hit.append(f)
 death=[]
 for i in range(4):
  f=Image.new('RGBA',src.size)
  # Fold existing detailed legs around their original attachment points.
  for box,p,a in [((0,16,9,24),(7,17),-i*9),((16,17,24,24),(20,18),i*9),((9,18,15,24),(12,18),-i*6),((23,18,29,24),(25,18),i*6)]:
   f.alpha_composite(rotate_part(cut(src,box),p,a))
  shell=body.copy();d=ImageDraw.Draw(shell)
  d.line((14,7,13,10,15,12),fill=(17,17,17,255),width=1)
  if i<2:d.point((14,10),fill=CYAN)
  f.alpha_composite(shell,(0,i))
  if i>=2:
   for y in range(24):
    for x in range(32):
     r,g,b,a=f.getpixel((x,y))
     if a and b>r*1.5 and g>r*1.5:f.putpixel((x,y),(68,85,85,255))
  death.append(f)
 return {'idle':[src],'hit':hit,'death':death}

def timeline(family,walker):
 seq=[]
 def add(state,ticks):
  frames=family[state]
  for t in range(ticks):seq.append((state,frames[min(len(frames)-1,t*len(frames)//ticks)]))
 add('idle',35)
 if walker:add('charge',24);add('fire',6);add('idle',25)
 add('hit',14 if walker else 8);add('idle',30)
 add('death',20);add('wreck',40) if 'wreck' in family else None
 return seq

def main():
 OUT.mkdir(exist_ok=True)
 w=walker_actions(Image.open(IN/'walker-rgba.png').convert('RGBA'));c=crab_actions(Image.open(IN/'crab-rgba.png').convert('RGBA'))
 w['wreck']=[w['death'][-1]];c['wreck']=[c['death'][-1]]
 sequences={}
 for name,fam in [('walker',w),('crab',c)]:
  seq=timeline(fam,name=='walker');sequences[name]=seq
  imgs=[]
  for state,im in seq:
   bg=Image.new('RGBA',im.size,(17,24,32,255));bg.alpha_composite(im);imgs.append(bg.convert('RGB').resize((im.width*6,im.height*6),Image.Resampling.NEAREST))
  imgs[0].save(OUT/(name+'-actions-6x.gif'),save_all=True,append_images=imgs[1:],duration=20,loop=0)
  poses=[im for state,frames in fam.items() if state!='wreck' for im in frames]
  sheet=Image.new('RGBA',(im.width*len(poses),im.height))
  for i,pose in enumerate(poses):sheet.alpha_composite(pose,(i*im.width,0))
  sheet.save(OUT/(name+'-actions.png'))
  sheet.resize((sheet.width*3,sheet.height*3),Image.Resampling.NEAREST).save(OUT/(name+'-contact-3x.png'))
 # Static approved composition with new actors removed, keeping original HUD.
 from preview_drowned_native import decode
 from prepare_drowned_enemy_idle import paste
 srcdir=ROOT/'build/drowned-slice/assets'
 bg=decode(srcdir/'drowned-rear.spbm').crop((80,0,400,208)).convert('RGB');paste(bg,decode(srcdir/'drowned-front.spbm').crop((320,0,640,208)),(0,0))
 paste(bg,decode(ROOT/'assets/runtime/sparkpaw-sprites4.spbm').crop((0,0,48,48)),(160,152))
 scene=Image.new('RGBA',(320,256));scene.paste(bg);scene.paste(Image.open(IN/'scene-1x.png').crop((0,208,320,256)),(0,208))
 seq=sequences['walker'];frames=[]
 for i,(state,art) in enumerate(seq):
  f=scene.copy();f.alpha_composite(art,(24,136));cr=sequences['crab'][min(i,len(sequences['crab'])-1)][1];f.alpha_composite(cr,(232,176))
  # No fake projectile: muzzle animation is reviewed separately from real shot.
  frames.append(f.convert('RGB').resize((960,768),Image.Resampling.NEAREST))
 frames[0].save(OUT/'scene-3x.gif',save_all=True,append_images=frames[1:],duration=20,loop=0)
 (OUT/'README.txt').write_text('Offline action audition v2: Pump Walker recoil includes hips/knees with planted feet; Turbine Crab and both deaths unchanged. Idle,24tick charge,6tick fire recoil,hit,4x5tick collapse,40tick wreck hold. Hit/charge durations mirror current selectors; wreck hold and loop reset are review staging. No projectile, actual damage, respawn or runtime integration. Native body parts retained, no resizing. Death darkening uses existing steel role; small pressure leak. User review pending.\n')
 print(OUT/'scene-3x.gif')
if __name__=='__main__':main()
