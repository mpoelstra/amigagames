"""Review existing station/Core art in Drowned palette; does not stage runtime."""
from pathlib import Path
import sys
from PIL import Image
from preview_drowned_native import decode,indexed
from generate_runtime_assets import WAYSTATION_SOURCE
R=Path(__file__).resolve().parents[1];O=R/'assets/concept/drowned-rain-station-v1';O.mkdir(exist_ok=True)
front=decode(R/'build/drowned-governor/assets/drowned-route.spbm');pal=[tuple(front.getpalette()[i:i+3]) for i in range(0,48,3)]
source=Image.open(WAYSTATION_SOURCE).convert('RGB');mask=Image.new('L',source.size)
mask.putdata([0 if min(p)>=210 and max(p)-min(p)<=14 else 255 for p in source.getdata()])
b=mask.getbbox();art=source.crop(b).convert('RGBA');art.putalpha(mask.crop(b));art.thumbnail((200,145),Image.Resampling.LANCZOS)
station=indexed(art.convert('RGB'),pal,first=1);alpha=art.getchannel('A').point(lambda a:255 if a>=96 else 0)
station.putdata([p if a else 0 for p,a in zip(station.getdata(),alpha.getdata())]);station.save(O/'station-indexed.png')
core=decode(R/'assets/runtime/stormstone-core.spbm');core=core.point([0,1,5,6,11,5,6,5,8,9,10,11,8,5,5,6]+list(range(16,256)));core.putpalette(front.getpalette());core.save(O/'rain-core-indexed.png')
rear=decode(R/'build/drowned-governor/assets/drowned-rear.spbm').convert('RGB');scene=rear.crop((400,0,720,208));floor=front.crop((0,200,320,208));scene.paste(floor.convert('RGB'),(0,200))
scene.paste(station.convert('RGB'),((320-art.width)//2,200-art.height),alpha)
scene.resize((1280,832),Image.Resampling.NEAREST).save(O/'station-scene.png')
frames=[]
for i in range(48):
 cell=core.crop((0,(i//8%6)*48,64,(i//8%6+1)*48));a=Image.frombytes('L',cell.size,bytes(255 if p else 0 for p in cell.tobytes()))
 im=scene.copy();im.paste(cell.convert('RGB'),(128,112+[-1,-2,-1,0,1,2][i//8]),a);frames.append(im.resize((960,624),Image.Resampling.NEAREST))
frames[0].save(O/'rain-station-preview.png');frames[0].save(O/'rain-station-preview.gif',save_all=True,append_images=frames[1:],duration=80,loop=0,disposal=2)
(O/'README.txt').write_text('Host-only native review. Existing Level1 station source and Core family, Drowned16pen palette. Core hue remap preserves silhouette and18existing idle/pickup frames. No emulator, no HUD in crop, no runtime integration. Station shares200pxfloor; Core hover112. Proposed final camera holds station centred.\n')
print(O)
