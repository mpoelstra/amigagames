"""Compile approved concept to native review assets; no runtime/dist writes.
Water frames execute the current renderer's actual waterPatternPen C function.
"""
from pathlib import Path
import ctypes, hashlib, json, subprocess, tempfile
from PIL import Image
from preview_drowned_native import decode
R=Path(__file__).resolve().parents[1]
O=R/'assets/concept/drowned-pontoon-native-v1'
S=R/'assets/concept/drowned-pontoon-concept-v1.png'
def mask(im): return Image.frombytes('L',im.size,bytes(255 if p else 0 for p in im.tobytes()))
def paste(dst,im,at): dst.paste(im.convert('RGB'),at,mask(im))
def main():
 O.mkdir(exist_ok=True)
 front=decode(R/'build/drowned-route/assets/drowned-route.spbm')
 pal=[tuple(front.getpalette()[i:i+3]) for i in range(0,48,3)]
 src=Image.open(S).convert('RGBA').crop((96,188,1784,393))
 # Retain only largest alpha-connected component: exclude concept labels.
 alpha=src.getchannel('A'); pix=alpha.load();seen=set();largest=[]
 for y in range(src.height):
  for x in range(src.width):
   if (x,y) in seen or pix[x,y]<128: continue
   todo=[(x,y)];seen.add((x,y));component=[]
   while todo:
    xx,yy=todo.pop();component.append((xx,yy))
    for nx,ny in ((xx-1,yy),(xx+1,yy),(xx,yy-1),(xx,yy+1)):
     if 0<=nx<src.width and 0<=ny<src.height and (nx,ny) not in seen and pix[nx,ny]>=128:
      seen.add((nx,ny));todo.append((nx,ny))
   if len(component)>len(largest):largest=component
 clean=Image.new('L',src.size)
 for xy in largest:clean.putpixel(xy,255)
 src.putalpha(clean);bbox=clean.getbbox();src=src.crop(bbox)
 small=src.resize((96,round(src.height*96/src.width)),Image.Resampling.BOX)
 hull=Image.new('P',(96,16));hull.putpalette(front.getpalette())
 for y in range(small.height):
  for x in range(96):
   rr,gg,bb,aa=small.getpixel((x,y))
   if aa<128:continue
   allowed=[1,12,13,14,15] if rr>bb*1.18 and rr>gg*1.08 else [1,8,9,10,11]
   pen=min(allowed,key=lambda p:sum((c-pal[p][i])**2 for i,c in enumerate((rr,gg,bb))))
   hull.putpixel((x,y),pen)
 hull.save(O/'pontoon-indexed.png',transparency=0)
 rgba=hull.convert('RGBA');rgba.putalpha(mask(hull));rgba.save(O/'pontoon-rgba.png')
 text=(R/'src/renderer.c').read_text();a=text.index('static UBYTE waterPatternPen(');b=text.index('\nstatic BOOL buildWaterPatterns',a)
 c=text[a:b].replace('static UBYTE waterPatternPen','unsigned char waterPatternPen')
 with tempfile.TemporaryDirectory() as td:
  td=Path(td);(td/'water.c').write_text('typedef unsigned char UBYTE; typedef short WORD;\n'+c)
  libpath=td/'water.so';subprocess.run(['cc','-shared','-fPIC',str(td/'water.c'),'-o',str(libpath)],check=True)
  lib=ctypes.CDLL(str(libpath));fn=lib.waterPatternPen;fn.argtypes=[ctypes.c_ubyte,ctypes.c_short,ctypes.c_short];fn.restype=ctypes.c_ubyte
  waters=[]
  for f in range(16):
   w=Image.new('P',(320,11));w.putpalette(front.getpalette())
   for y in range(11):
    for x in range(320):w.putpixel((x,y),fn(f,x%80,y))
   waters.append(w)
 rear=decode(R/'build/drowned-route/assets/drowned-rear.spbm').crop((320,0,640,208)).convert('RGB')
 player=decode(R/'assets/runtime/sparkpaw-sprites4.spbm').crop((0,0,48,48));feet=mask(player).getbbox()[3]
 hud=Image.open(R/'assets/concept/drowned-polish-native-v1/preview-1x.png').convert('RGB').crop((0,208,320,256))
 frames=[];detail=[]
 for t in range(128):
  im=rear.copy();x=64+(t if t<64 else 127-t);bob=1 if (t//16)%2 else 0;deck=189+bob
  paste(im,hull,(x,deck));paste(im,player,(x+24,deck-feet))
  # Foreground wave covers submerged hull, unchanged native water pixels.
  paste(im,waters[t%16],(0,197))
  full=Image.new('RGB',(320,256));full.paste(im);full.paste(hud,(0,208));frames.append(full)
  detail.append(im.crop((40,132,256,208)).resize((1080,380),Image.Resampling.NEAREST))
 frames[16].save(O/'scene-1x.png');frames[16].resize((1280,1024),Image.Resampling.NEAREST).save(O/'scene-4x.png')
 frames[0].save(O/'motion-1x.gif',save_all=True,append_images=frames[1:],duration=40,loop=0,disposal=2)
 detail[0].save(O/'waterline-5x.gif',save_all=True,append_images=detail[1:],duration=40,loop=0,disposal=2)
 # Review proof: mask reaches below surface; water overwrite exactly source pens.
 assert mask(hull).getbbox()[3]>10
 assert all(v%17==0 for rgb in pal for v in rgb)
 (O/'manifest.json').write_text(json.dumps({'status':'offline native art/motion review only; no runtime integration or physics proof','source':str(S.relative_to(R)),'source_sha256':hashlib.sha256(S.read_bytes()).hexdigest(),'source_crop':[96,188,1784,393],'component_bbox':bbox,'cell':[96,16],'hull_bounds':mask(hull).getbbox(),'palette':pal,'water':'actual renderer waterPatternPen compiled and executed;80px repeated strips,16frames at25Hz','motion':'one-pixel bob with player following deck; scripted lateral motion, no collision simulation','hud':'unchanged existing offline reference HUD','cache_estimate':'single96x16 mask+4planes with7 sourcewords:1120bytes; excludes wake/restore storage','pending':'native visual approval, optional wake, moving-platform physics, DMA layering and020 cadence'},indent=2)+'\n')
 print('Native96x16 pontoon, exact12-bit front palette, actual C water16frames, 128-frame offline preview.')
if __name__=='__main__':main()
