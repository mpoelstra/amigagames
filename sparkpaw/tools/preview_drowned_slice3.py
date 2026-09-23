"""Offline view of exact candidate patches; not emulator or FPS evidence."""
from pathlib import Path
from PIL import Image
from preview_drowned_native import decode
from preview_drowned_jet import paste

ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'assets/concept/drowned-geyser-native-v2'
src=ROOT/'build/drowned-slice/assets'
front=decode(src/'drowned-front.spbm')
rear=decode(src/'drowned-rear.spbm')
patches=Image.open(src/'drowned-patches.png')
hud=Image.open(ROOT/'assets/concept/drowned-polish-native-v1/preview-1x.png').convert('RGB').crop((0,208,320,256))
player=decode(ROOT/'assets/runtime/sparkpaw-sprites4.spbm').crop((0,0,48,48))
reference=Image.open(ROOT/'assets/concept/drowned-polish-native-v1/preview-1x.png').convert('RGB')
frames=[];durations=[];last=None
for tick in range(200):
    f=(0 if tick<100 else (8 if (tick//5)&1 else 1) if tick<130 else
       2 if tick<135 else 3+((tick-135)//4)%4 if tick<185 else 7 if tick<191 else 8)
    if f==last:
        durations[-1]+=20
        continue
    last=f
    local=front.crop((320,0,640,208))
    local.paste(patches.crop((0,f*96,96,(f+1)*96)),(128,112))
    scene=Image.new('RGB',(320,256));scene.paste(rear.crop((80,0,400,208)).convert('RGB'),(0,0))
    paste(scene,local,(0,0))
    # Static accepted water sample in the visible right-hand gap.
    scene.paste(reference.crop((80,197,112,208)),(288,197))
    paste(scene,player,(200,152));scene.paste(hud,(0,208))
    assert scene.crop((0,208,320,256)).tobytes()==hud.tobytes()
    frames.append(scene);durations.append(20)
frames[0].save(OUT/'scene-1x.gif',save_all=True,append_images=frames[1:],duration=durations,loop=0,disposal=2)
large=[f.resize((960,768),Image.Resampling.NEAREST) for f in frames]
large[0].save(OUT/'scene-3x.gif',save_all=True,append_images=large[1:],duration=durations,loop=0,disposal=2)
large[8].save(OUT/'scene-3x.png')
print('Exact candidate patch preview, 4-second illustrative cycle, HUD unchanged')
