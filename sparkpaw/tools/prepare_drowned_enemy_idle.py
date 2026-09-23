"""Offline native idle proof only: approved source -> indexed cells and scene.
No runtime/dist writes; no animation frames manufactured from this proof.
"""
from pathlib import Path
import json
from collections import Counter
from PIL import Image
from preview_drowned_native import decode
from preview_drowned_polish import PAL
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'assets/enemies/drowned-native-idle-v1'
SOURCE=ROOT/'assets/enemies/drowned-enemies-idle-source-v1.png'

def mask(im):return Image.frombytes('L',im.size,bytes(255 if p else 0 for p in im.tobytes()))
def paste(dst,im,at):dst.paste(im.convert('RGB'),at,mask(im))
def main():
 OUT.mkdir(exist_ok=True)
 src=Image.open(SOURCE).convert('RGBA')
 assert src.getchannel('A').getextrema()[0]==0,'requires real alpha, no inferred dark-background removal'
 pal=[tuple((v>>4)*17 for v in c) for c in PAL]
 metadata={};sprites=[]
 for name,box,size,envelope in [('crab',(0,0,src.width//2,src.height),(32,24),(30,22)),('walker',(src.width//2,0,src.width,src.height),(64,64),(54,58))]:
  part=src.crop(box);bounds=part.getchannel('A').point(lambda a:255 if a>=128 else 0).getbbox();assert bounds
  part=part.crop(bounds);scale=min(envelope[0]/part.width,envelope[1]/part.height)
  small=part.resize((max(1,round(part.width*scale)),max(1,round(part.height*scale))),Image.Resampling.BOX)
  cell=Image.new('P',size,0);cell.putpalette([v for c in pal for v in c]+[0]*720)
  at=((size[0]-small.width)//2,size[1]-1-small.height)
  for y in range(small.height):
   for x in range(small.width):
    r,g,b,a=small.getpixel((x,y))
    if a<128:continue
    # Material-aware roles: no bright orange protagonist pens2/3.
    if r>b*1.18 and r>g*1.08:allowed=[1,12,13,14,15]
    elif g>r*1.2 and b>r*1.2:allowed=[1,5,6,8,9,10,11]
    else:allowed=[1,8,9,10,11]
    pen=min(allowed,key=lambda p:sum((c-pal[p][i])**2 for i,c in enumerate((r,g,b))))
    cell.putpixel((at[0]+x,at[1]+y),pen)
  cell.save(OUT/(name+'-indexed.png'),transparency=0)
  rgba=cell.convert('RGBA');rgba.putalpha(mask(cell));rgba.save(OUT/(name+'-rgba.png'))
  metadata[name]={'cell':size,'opaque_bbox':mask(cell).getbbox(),'source_bbox_in_half':bounds,'scale':scale,'pen_counts':dict(Counter(cell.tobytes()))}
  sprites.append(cell)
 player=decode(ROOT/'assets/runtime/sparkpaw-sprites4.spbm').crop((0,0,48,48))
 strip=Image.new('RGB',(192,80),(17,24,32));paste(strip,sprites[0],(12,64-24));paste(strip,player,(66,64-48));paste(strip,sprites[1],(120,0))
 strip.save(OUT/'scale-1x.png');strip.resize((1152,480),Image.Resampling.NEAREST).save(OUT/'scale-6x.png')
 srcdir=ROOT/'build/drowned-slice/assets'
 rear=decode(srcdir/'drowned-rear.spbm').crop((80,0,400,208)).convert('RGB')
 front=decode(srcdir/'drowned-front.spbm').crop((320,0,640,208))
 paste(rear,front,(0,0));paste(rear,sprites[1],(24,136));paste(rear,player,(160,152));paste(rear,sprites[0],(232,176))
 scene=Image.new('RGB',(320,256));scene.paste(rear,(0,0))
 hud=Image.open(ROOT/'assets/concept/drowned-polish-native-v1/preview-1x.png').convert('RGB').crop((0,208,320,256));scene.paste(hud,(0,208))
 scene.save(OUT/'scene-1x.png');scene.resize((1280,1024),Image.Resampling.NEAREST).save(OUT/'scene-4x.png')
 (OUT/'manifest.json').write_text(json.dumps({'status':'native idle review only, no runtime or animation acceptance','palette':pal,'sprites':metadata},indent=2)+'\n')
 print(json.dumps(metadata))
if __name__=='__main__':main()
