"""Approved 24px frames, exact mirror pairs; isolated dry encounter assets."""
from pathlib import Path
import shutil,json,sys
from PIL import Image
from preview_drowned_native import decode
from generate_runtime_assets import save_spbm,bitmap_mask
R=Path(__file__).resolve().parents[1];O=R/'build/drowned-spillwing';A=O/'assets';A.mkdir(parents=True,exist_ok=True)
for p in (R/'build/drowned-route/assets').iterdir():
 if p.is_file():shutil.copy2(p,A/p.name)
f=decode(A/'drowned-route.spbm');pal=[tuple(f.getpalette()[i:i+3]) for i in range(0,48,3)]
f.paste(0,(0,0,2400,208))
material=Image.open(R/'assets/concept/drowned-polish-native-v1/front-indexed.png')
for x in range(0,2400,32):f.paste(material.crop((160,200,192,208)),(x,200))
save_spbm(A/'drowned-route.spbm',f,pal,4)
(A/'drowned-route.bin').write_bytes(bytes(2400//16*14))
(O/'drowned_route_coins.h').write_text('static const WORD routeCoins[][2]={{32,32}};\n')
(O/'drowned_route_layout.h').write_text('''static const struct EnemyPatrolSurface drownedRouteSurfaces[]={{160,576,200}};
static const struct EnemySpawnCandidate drownedRouteSpawns[]={{280,280,-1,0,0,1,1}};
static const WORD drownedRouteWater[]={240,608,1184,1376,1488,1600,1712,1824,1936,2048};
''')
if '--pair' in sys.argv:
 (O/'drowned_route_layout.h').write_text('static const struct EnemyPatrolSurface drownedRouteSurfaces[]={{160,672,200},{160,672,200}};\nstatic const struct EnemySpawnCandidate drownedRouteSpawns[]={{280,280,-1,0,0,1,1},{400,400,-1,1,0,1,1}};\nstatic const WORD drownedRouteWater[]={240,608,1184,1376,1488,1600,1712,1824,1936,2048};\n')
source=R/'assets/enemies/spillwing-actions-review-v1'
frames=[Image.open(source/f'{name}-{n}.png') for name,count in [('fly',4),('warn',2),('dive',2),('recover',2),('hit',2),('death',4)] for n in range(count)]
sheet=Image.new('P',(48,384));sheet.putpalette(frames[0].getpalette())
for i,im in enumerate(frames):
 assert im.size==(24,24) and max(im.getdata())<16
 assert im.getpalette()[:48]==f.getpalette()[:48], 'native palette mismatch'
 sheet.paste(im.transpose(Image.Transpose.FLIP_LEFT_RIGHT),(0,i*24));sheet.paste(im,(24,i*24))
save_spbm(A/'spillwing.spbm',sheet,pal,4,bitmap_mask(sheet))
assert list(decode(A/'spillwing.spbm').getdata())==list(sheet.getdata())
(O/'art-manifest.json').write_text(json.dumps({'frames':16,'cell':[24,24],'layout':'left/right','chip_cache_bytes':2*16*24*3*5*2,'source_bytes':(A/'spillwing.spbm').stat().st_size},indent=2))
print('Spillwing:16 approved frames, native decoder parity,24x24,23040-byte cache.')
