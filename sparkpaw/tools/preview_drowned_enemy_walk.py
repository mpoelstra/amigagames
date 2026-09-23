"""Offline fixed-part gait audition; no runtime/dist writes or production claim."""
from pathlib import Path
import math,json
from PIL import Image,ImageDraw
from prepare_drowned_enemy_idle import ROOT,decode,paste
IN=ROOT/'assets/enemies/drowned-native-idle-v1'
OUT=ROOT/'assets/enemies/drowned-walk-review-v1'

def cut(src,box):
 out=Image.new('RGBA',src.size);out.paste(src.crop(box),box[:2]);return out

def transform(part,a,b,c,d):
 # Rigid mapping source segment a->b to destination c->d; lengths preserved
 # by IK. Only rotation/translation, no independent body or pose scaling.
 angle=math.atan2(d[1]-c[1],d[0]-c[0])-math.atan2(b[1]-a[1],b[0]-a[0])
 co,si=math.cos(angle),math.sin(angle)
 return part.transform(part.size,Image.Transform.AFFINE,(co,si,a[0]-co*c[0]-si*c[1],-si,co,a[1]+si*c[0]-co*c[1]),Image.Resampling.NEAREST)

def knee(hip,foot,l1,l2):
 dx,dy=foot[0]-hip[0],foot[1]-hip[1];dist=math.hypot(dx,dy)
 assert abs(l1-l2)<dist<l1+l2
 along=(l1*l1-l2*l2+dist*dist)/(2*dist);height=math.sqrt(max(0,l1*l1-along*along))
 return (hip[0]+along*dx/dist-height*dy/dist,hip[1]+along*dy/dist+height*dx/dist)

def walker_frames(src):
 specs=[((34,39),(35,49),(39,59),(28,37,44,51),(29,47,44,60),(28,58,50,63)),
        ((23,39),(18,47),(20,59),(11,37,29,50),(11,46,29,60),(8,58,29,63))]
 body=cut(src,(0,0,64,40));frames=[]
 for phase in range(8):
  out=Image.new('RGBA',(64,64))
  for i,(h,k,f,u,l,feet) in enumerate(specs):
   t=((phase+4*i)%8)/8
   # planted half moves backward; returning foot lifts, then plants again.
   x=3-12*t if t<.5 else -3+12*(t-.5)
   lift=0 if t<.5 else 3*math.sin((t-.5)*2*math.pi)
   hip=(h[0],h[1]+1);foot=(f[0]+x,f[1]-lift)
   joint=knee(hip,foot,math.dist(h,k),math.dist(k,f))
   out.alpha_composite(transform(cut(src,l),k,f,joint,foot))
   out.alpha_composite(transform(cut(src,u),h,k,hip,joint))
   shoe=cut(src,feet);out.alpha_composite(shoe,(round(x),-round(lift)))
  out.alpha_composite(body,(0,1));frames.append(out)
 return frames

def crab_frames(src):
 body=cut(src,(0,0,32,19));frames=[]
 legs=[((9,18,15,24),(12,18),0),((23,18,29,24),(25,18),4),
       ((0,16,9,24),(7,17),4),((16,17,24,24),(20,18),0)]
 for phase in range(8):
  out=Image.new('RGBA',(32,24))
  for box,p,offset in legs:
   angle=math.radians(15*math.sin((phase+offset)*math.pi/4))
   out.alpha_composite(transform(cut(src,box),p,(p[0],p[1]+5),p,(p[0]-5*math.sin(angle),p[1]+5*math.cos(angle))))
  out.alpha_composite(body);frames.append(out)
 return frames

def main():
 OUT.mkdir(exist_ok=True)
 walker=Image.open(IN/'walker-rgba.png').convert('RGBA');crab=Image.open(IN/'crab-rgba.png').convert('RGBA')
 families={'walker':walker_frames(walker),'crab':crab_frames(crab)}
 for name,frames in families.items():
  w,h=frames[0].size;sheet=Image.new('RGBA',(w*8,h))
  for i,f in enumerate(frames):sheet.alpha_composite(f,(i*w,0))
  sheet.save(OUT/(name+'-walk.png'))
  sheet.resize((w*8*4,h*4),Image.Resampling.NEAREST).save(OUT/(name+'-contact-4x.png'))
  large=[]
  for f in frames:
   bg=Image.new('RGBA',f.size,(17,24,32,255));bg.alpha_composite(f);large.append(bg.convert('RGB').resize((w*6,h*6),Image.Resampling.NEAREST))
  large[0].save(OUT/(name+'-walk-6x.gif'),save_all=True,append_images=large[1:],duration=120,loop=0)
 srcdir=ROOT/'build/drowned-slice/assets'
 bg=decode(srcdir/'drowned-rear.spbm').crop((80,0,400,208)).convert('RGB');paste(bg,decode(srcdir/'drowned-front.spbm').crop((320,0,640,208)),(0,0))
 player=decode(ROOT/'assets/runtime/sparkpaw-sprites4.spbm').crop((0,0,48,48));paste(bg,player,(160,152))
 hud=Image.open(IN/'scene-1x.png').crop((0,208,320,256))
 scene=Image.new('RGB',(320,256));scene.paste(bg);scene.paste(hud,(0,208))
 animation=[]
 # Four walk cycles per direction; planted-foot displacement matches world travel.
 for tick in range(80):
  direction=1 if tick<40 else -1;local=tick%40;moving=local<32
  step=min(local,31);phase=step%8
  x=24+step*1.5 if direction==1 else 24+(31-step)*1.5
  f=scene.convert('RGBA')
  for name,base,pos in [('walker',walker,(round(x),136)),('crab',crab,(232+round((x-24)*.3),176))]:
   art=families[name][phase] if moving else base
   if direction<0:art=art.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
   f.alpha_composite(art,pos)
  animation.append(f.convert('RGB'))
 animation[0].save(OUT/'scene-1x.gif',save_all=True,append_images=animation[1:],duration=120,loop=0)
 big=[f.resize((960,768),Image.Resampling.NEAREST) for f in animation]
 big[0].save(OUT/'scene-3x.gif',save_all=True,append_images=big[1:],duration=120,loop=0)
 # Only palette-preserving nearest-neighbour transforms of approved master.
 for name,frames in families.items():
  original=Image.open(IN/(name+'-rgba.png')).convert('RGBA');allowed=set(original.getdata())
  for f in frames:assert all(p in allowed or p[3]==0 for p in f.getdata())
 (OUT/'README.txt').write_text('Offline fixed-part walk audition,8 poses per enemy. Exact native cells, nearest-neighbour parts from idle master. Not final gait/turn approval. Stop-and-mirror is direction-test staging, not final planted turn animation. No shots/death or runtime integration. Walking speed here illustrates cycle, not engine speed. Source:tools/preview_drowned_enemy_walk.py\n')
 print(OUT/'scene-3x.gif')
if __name__=='__main__':main()
