"""2400px route: approved foreground materials, one layout source for C/assets."""
from pathlib import Path
import json,shutil
from PIL import Image
from preview_drowned_native import decode
from generate_runtime_assets import save_spbm
R=Path(__file__).resolve().parents[1];OUT=R/'build/drowned-route';A=OUT/'assets'
def main():
 d=json.loads((R/'assets/levels/drowned-route-v1.json').read_text());A.mkdir(parents=True,exist_ok=True)
 width=d['world_width'];assert width==2400 and len(d['water'])==10
 old=R/'build/drowned-slice/assets';base=decode(old/'drowned-front.spbm');pal=[tuple(base.getpalette()[i:i+3]) for i in range(0,48,3)]
 front=Image.new('P',(width,208));front.putpalette(base.getpalette());front.paste(base,(0,0))
 material=Image.open(R/'assets/concept/drowned-polish-native-v1/front-indexed.png')
 for x in range(960,width,32):front.paste(material.crop((160,200,192,208)),(x,200))
 collision=bytearray(width//16*14)
 for x,y,w in d['platforms']:
  assert x%16==y%16==w%16==0
  if x>=960:
   for offset in range(0,w,16):
    # Existing industrial deck and support texture, no new raster identity.
    front.paste(material.crop((16,160,32,168)),(x+offset,y))
   # Extend approved support material without blank lower sections on tall posts.
   for offset in ([0] if w==32 else [0,w-16]):
    sw=32 if w==32 else 16
    for yy in range(y+8,200,32):
     hh=min(32,200-yy)
     front.paste(material.crop((0,168,sw,168+hh)),(x+offset,yy))
  for col in range(x//16,(x+w)//16):collision[(y//16)*(width//16)+col]=1
 for x in d['water']:front.paste(0,(x,197,x+80,208))
 # The compact animation contains only nozzle row0 at y170. Keep the
 # remaining static housing below it, as at the original ground geyser.
 nozzle=decode(R/'assets/concept/drowned-geyser-native-v2/lip.spbm')
 mask=Image.frombytes('L',nozzle.size,bytes(255 if p else 0 for p in nozzle.tobytes()))
 front.paste(nozzle,(1792,170),mask)
 save_spbm(A/'drowned-route.spbm',front,pal,4);front.save(A/'route-front.png')
 (A/'drowned-route.bin').write_bytes(collision)
 for name in ['drowned-rear.spbm','drowned-patches.spbm','pump-walker.spbm','turbine-crab.spbm','pump-shot.raw']:
  shutil.copyfile(old/name,A/name)
 header=['/* Generated from assets/levels/drowned-route-v1.json. */']
 for typ,key,name in [('struct EnemyPatrolSurface','surfaces','drownedRouteSurfaces'),('struct EnemySpawnCandidate','spawns','drownedRouteSpawns'),('struct EnemyTraversalLink','links','drownedRouteLinks')]:
  header.append('static const '+typ+' '+name+'[]={'+','.join('{'+','.join(map(str,row))+'}' for row in d[key])+'};')
 header.append('static const WORD drownedRouteWater[]={'+','.join(map(str,d['water']))+'};')
 (OUT/'drowned_route_layout.h').write_text('\n'.join(header)+'\n')
 (OUT/'drowned_route_coins.h').write_text('static const WORD routeCoins[][2]={'+','.join('{'+','.join(map(str,row))+'}' for row in d['coins'])+'};\n')
 assert decode(A/'drowned-route.spbm').tobytes()==front.tobytes()
 # Native-size still layout: live water, enemies and effects omitted.
 rear=decode(A/'drowned-rear.spbm').convert('RGB');views=Image.new('RGB',(960,624))
 for i,cam in enumerate([0,320,640,960,1280,1600,1920,2080]):
  bg=rear.crop((cam//4,0,cam//4+320,208));fg=front.crop((cam,0,cam+320,208));mask=Image.frombytes('L',fg.size,bytes(255 if p else 0 for p in fg.tobytes()));bg.paste(fg.convert('RGB'),(0,0),mask)
  views.paste(bg,((i%3)*320,(i//3)*208))
 views.save(OUT/'route-layout.png')
 (OUT/'manifest.json').write_text(json.dumps(d,indent=2)+'\n');print(f'{width}px,{len(d["water"])} water strips,6 enemy spawns,4 traversal links,{len(d["coins"])} diamonds')
if __name__=='__main__':main()
