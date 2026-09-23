"""Combine accepted pontoon pixels/physics with the approved flying family."""
from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[1];O=R/'build/drowned-ferry';A=O/'assets';A.mkdir(parents=True,exist_ok=True)
for p in (R/'build/drowned-pontoon/assets').iterdir():
 if p.is_file():shutil.copy2(p,A/p.name)
shutil.copy2(R/'build/drowned-spillwing/assets/spillwing.spbm',A/'spillwing.spbm')
for name in ['drowned_pontoon_art.h','drowned_route_coins.h']:
 shutil.copy2(R/'build/drowned-pontoon'/name,O/name)
(O/'drowned_route_layout.h').write_text('''static const struct EnemyPatrolSurface drownedRouteSurfaces[]={{200,432,200},{384,672,200},{552,808,200},{704,992,200},{800,1056,200}};
static const struct EnemySpawnCandidate drownedRouteSpawns[]={{328,328,-1,0,0,1,1},{520,520,-1,1,0,1,1},{704,704,-1,2,0,1,1},{848,848,-1,3,0,1,1},{952,952,-1,4,0,1,1}};
static const WORD drownedRouteWater[]={160,240,320,400,480,560,640,720,800,880};
''')
print('Ferry encounter:800px basin,5 spatially bounded Spillwings,low/high/low/high/low.')

# Solid 96x16 maintenance deck; the pontoon passes beneath its underside.
from PIL import Image
from preview_drowned_native import decode
from generate_runtime_assets import save_spbm
front=decode(A/'drowned-route.spbm');material=Image.open(R/'assets/concept/drowned-polish-native-v1/front-indexed.png')
for x in range(528,624,16):front.paste(material.crop((16,160,32,176)),(x,160))
pal=[tuple(front.getpalette()[i:i+3]) for i in range(0,48,3)]
save_spbm(A/'drowned-route.spbm',front,pal,4)
collision=bytearray((A/'drowned-route.bin').read_bytes())
for col in range(528//16,624//16):collision[10*150+col]=1
(A/'drowned-route.bin').write_bytes(collision)
(O/'drowned_route_coins.h').write_text('static const WORD routeCoins[][2]={{280,140},{408,132},{544,130},{592,130},{744,132},{856,140},{864,98}};\n')

# Precision upper path: one16px tile per landing; middle deck stays a branch.
upper=[(672,128),(768,96),(864,128)]
for x,y in upper:
 front.paste(material.crop((16,160,32,176)),(x,y))
 collision[(y//16)*150+x//16]=1
save_spbm(A/'drowned-route.spbm',front,pal,4)
(A/'drowned-route.bin').write_bytes(collision)
(O/'drowned_route_upper.h').write_text('static const WORD upper[][2]={'+','.join('{%d,%d}'%q for q in upper)+'};\n')

import subprocess,sys
subprocess.run([sys.executable,str(R/"tools/build_spillwing_clearance.py")],check=True)
