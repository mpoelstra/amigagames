"""Join accepted land and ferry art/geometry; no new raster art or finale."""
from pathlib import Path
import json,shutil,subprocess,sys,re
from PIL import Image
from preview_drowned_native import decode
from generate_runtime_assets import save_spbm
R=Path(__file__).resolve().parents[1];O=R/'build/drowned-joined';A=O/'assets';A.mkdir(parents=True,exist_ok=True)
width=3520;shift=2240;cols=width//16
land=R/'build/drowned-route';ferry=R/'build/drowned-ferry'
for p in (land/'assets').iterdir():
 if p.is_file():shutil.copy2(p,A/p.name)
for name in ('spillwing.spbm','pontoon-clip.bin'):
 shutil.copy2(ferry/'assets'/name,A/name)
shutil.copy2(ferry/'drowned_pontoon_art.h',O/'drowned_pontoon_art.h')
f=decode(land/'assets/drowned-route.spbm');boat=decode(ferry/'assets/drowned-route.spbm')
front=Image.new('P',(width,208));front.putpalette(f.getpalette());front.paste(f,(0,0))
# Preserve land through2399. Begin water at2400 using ferry local x160.
front.paste(boat.crop((160,0,1280,208)),(2400,0))
pal=[tuple(front.getpalette()[i:i+3]) for i in range(0,48,3)]
save_spbm(A/'drowned-route.spbm',front,pal,4)
l=(land/'assets/drowned-route.bin').read_bytes();b=(ferry/'assets/drowned-route.bin').read_bytes();collision=bytearray(cols*14)
for row in range(14):
 collision[row*cols:row*cols+150]=l[row*150:(row+1)*150]
 collision[row*cols+150:(row+1)*cols]=b[row*150+10:row*150+80]
(A/'drowned-route.bin').write_bytes(collision)
d=json.loads((R/'assets/levels/drowned-route-v1.json').read_text());assert len(d['spawns'])==6
# Safe dry checkpoint approach; preserve isolated route authoring.
d['surfaces'][6]=[2192,2288,200]
d['spawns'][5][:2]=[2240,2256]
surfaces=d['surfaces']+[[a+shift,z+shift,y] for a,z,y in [(200,432,200),(384,672,200),(552,808,200),(704,992,200),(800,1056,200)]]
spawns=d['spawns']+[[x+shift,x+shift,-1,len(d['surfaces'])+i,2,1,1] for i,x in enumerate([328,520,704,848,952])]
# Append precision flyer, preserving all existing spawn IDs/flight profiles.
surfaces.append([1728,2144,200])
spawns.append([1840,1840,-1,len(surfaces)-1,2,1,1])
# Opening precision encounter, appended so all existing profiles retain IDs.
surfaces.append([1328,1552,200])
spawns.append([1488,1488,-1,len(surfaces)-1,2,1,1])
water=d['water']+[2400+i*80 for i in range(10)]
coins=d['coins']+[[x+shift,y] for x,y in [(280,140),(408,132),(544,130),(592,130),(744,132),(856,140),(864,98)]]
assert len(coins)<=48 and len(spawns)<=24
# Conservative first possibly nonzero row of every16px canonical column.
# Include all changing canonical patches and collectible hover footprints,
# even where the normal Bob-only path never bakes those collectibles.
tops=[]
for x in range(0,width,16):
 cell=front.crop((x,0,x+16,208))
 top=next((y for y in range(208) if any(cell.crop((0,y,16,y+1)).tobytes())),208)
 for px,py,pw in [(448,131,32),(768,136,80),(1792,107,32),(2320,152,48)]+[(wx,197,80) for wx in water]+[(cx-16,cy-2,48) for cx,cy in coins]:
  if x<px+pw and x+16>px:top=min(top,py)
 tops.append(max(0,top))
(O/'drowned_column_tops.h').write_text('static const UBYTE drownedColumnTops['+str(cols)+']={'+','.join(map(str,tops))+'};\n')

header=[]
for typ,name,data in [('struct EnemyPatrolSurface','drownedRouteSurfaces',surfaces),('struct EnemySpawnCandidate','drownedRouteSpawns',spawns),('struct EnemyTraversalLink','drownedRouteLinks',d['links'])]:
 header.append('static const '+typ+' '+name+'[]={'+','.join('{'+','.join(map(str,v))+'}' for v in data)+'};')
header.append('static const WORD drownedRouteWater[]={'+','.join(map(str,water))+'};')
(O/'drowned_route_layout.h').write_text('\n'.join(header)+'\n')
(O/'drowned_route_coins.h').write_text('static const WORD routeCoins[][2]={'+','.join('{%d,%d}'%tuple(v) for v in coins)+'};\n')
manifest=dict(world_width=width,ferry_shift=shift,water=water,coins=coins,surfaces=surfaces,spawns=spawns,links=d['links'],middle=[2768,160,96],upper=[[2912,128],[3008,96],[3104,128]],finale=False)
(O/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
subprocess.run([sys.executable,str(R/'tools/build_spillwing_clearance.py'),str(O)],check=True)
assert decode(A/'drowned-route.spbm').tobytes()==front.tobytes()
rear=decode(A/'drowned-rear.spbm').convert('RGB');assert rear.width>=(width-320)//4+320
views=Image.new('RGB',(1280,624))
for i,cam in enumerate(range(0,width-319,320)):
 bg=rear.crop((cam//4,0,cam//4+320,208));fg=front.crop((cam,0,cam+320,208));mask=Image.frombytes('L',fg.size,bytes(255 if v else 0 for v in fg.tobytes()));bg.paste(fg.convert('RGB'),(0,0),mask);views.paste(bg,((i%4)*320,(i//4)*208))
views.save(O/'joined-layout.png')
print(f'Joined:{width}px,20water strips,{len(coins)}diamonds,13spawn sites,3families; finale not yet implemented.')
