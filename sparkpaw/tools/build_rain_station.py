"""Approved unique station/tree -> native static foreground and Rain Core family."""
from pathlib import Path
from PIL import Image
from preview_drowned_native import decode,indexed
from generate_runtime_assets import save_spbm
R=Path(__file__).resolve().parents[1];A=R/'build/drowned-governor/assets';O=R/'build/drowned-governor'
f=decode(A/'drowned-route.spbm');pal=[tuple(f.getpalette()[i:i+3]) for i in range(0,48,3)]
s=Image.open(R/'assets/concept/drowned-rain-station-source-v3.png').convert('RGBA');s=s.crop(s.getchannel('A').point(lambda a:255 if a>128 else 0).getbbox());s.thumbnail((224,176),Image.Resampling.LANCZOS)
a=s.getchannel('A').point(lambda a:255 if a>=128 else 0);n=indexed(s.convert('RGB'),pal,first=1);n.putdata([p if m else 0 for p,m in zip(n.getdata(),a.getdata())]);f.paste(n,(1624,200-n.height),a)
save_spbm(A/'drowned-route.spbm',f,pal,4);assert decode(A/'drowned-route.spbm').tobytes()==f.tobytes()
# Preserve original mask, dimensions and all18 animation frames; hue remap only.
source=R/'assets/runtime/stormstone-core.spbm';raw=source.read_bytes();core=decode(source);core=core.point([0,1,5,6,11,5,6,5,8,9,10,11,8,5,5,6]+list(range(16,256)));core.putpalette(f.getpalette());mask=raw[60+4*(core.width//8)*core.height:];assert len(mask)==core.width//8*core.height
save_spbm(A/'rain-core.spbm',core,pal,4,mask)
rear=decode(A/'drowned-rear.spbm').convert('RGB');bg=rear.crop((388,0,708,208));fg=f.crop((1552,0,1872,208));bg.paste(fg.convert('RGB'),(0,0),Image.frombytes('L',fg.size,bytes(255 if p else 0 for p in fg.tobytes())))
cell=core.crop((0,0,64,48));bg.paste(cell.convert('RGB'),(56,112),Image.frombytes('L',cell.size,bytes(255 if p else 0 for p in cell.tobytes())));bg.resize((1280,832),Image.Resampling.NEAREST).save(O/'rain-end-scene.png')
print('Unique station/tree',n.size,'static; RainCore existing18frame footprint/mask retained, no additional Chip allocations')
