"""Procedural glint previews: V3 localized pixels; V4 adds gentle shore colour swell.

Offline only. Both need bitmap animation; V4 also needs safe palette modulation.
"""
from pathlib import Path
import json, random, argparse
from PIL import Image
from preview_drowned_native import decode

R=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser()
parser.add_argument('--version',type=int,choices=(3,4),default=4)
version=parser.parse_args().version
O=R/f'assets/concept/drowned-water-shimmer-v{version}';O.mkdir(parents=True,exist_ok=True)
rear=decode(R/'assets/concept/drowned-panorama-v2/drowned-rear.spbm')
front=decode(R/'build/drowned-full/assets/drowned-route.spbm')
N=24;DURATION=120
# Gentle shore swell, maximum18/255 versus the rejected V2 peak64/255.
# Phase-shifted two-line bands give a small approach/recede impression.
swell=(0,0,0,1,3,5,8,11,14,16,18,18,16,14,11,8,5,3,1,0,0,0,0,0)
shore_bands=((184,186,0,3),(186,188,2,2),(188,190,4,1))
base_palette=[tuple(rear.getpalette()[i:i+3]) for i in range(0,24,3)]
rng=random.Random(73021)
# Uneven spacing and staggered lifetimes. Positions belong to the rear world,
# so scrolling uses exactly the same quarter-speed as the existing lake.
glints=[]
for cell in range(4,rear.width-10,19):
    x=cell+rng.randrange(9);y=rng.randrange(185,199)
    glints.append((x,y,rng.randrange(N),rng.randrange(4,9)))

def animate(frame):
    im=rear.copy()
    for x,y,phase,length in glints:
        age=(frame+phase)%N
        if age>=10:continue
        widths=(1,2,3,4,5,5,4,3,2,1)
        width=max(1,(widths[age]*length+4)//5)
        pen=(5,6,6,7,7,7,6,6,5,5)[age]
        # Very slight sideways drift, no descending rain/sparkle motion.
        left=x+age//5-width//2
        for px in range(left,left+width):
            if 0<=px<im.width:im.putpixel((px,y),pen)
    assert im.crop((0,0,im.width,185)).tobytes()==rear.crop((0,0,rear.width,185)).tobytes()
    assert im.crop((0,199,im.width,208)).tobytes()==rear.crop((0,199,rear.width,208)).tobytes()
    assert im.getpalette()==rear.getpalette() and max(im.tobytes())<8
    return im

def scene(cam,frame):
    indices=animate(frame).crop((cam//4,0,cam//4+320,208))
    im=indices.convert('RGB')
    if version==4:
        for top,bottom,phase,strength in shore_bands:
            delta=swell[(frame+phase)%N]*strength//3
            for y in range(top,bottom):
                for x in range(320):
                    pen=indices.getpixel((x,y))
                    if pen in (4,6):
                        im.putpixel((x,y),tuple(min(255,c+delta) for c in base_palette[pen]))
    unchanged=indices.convert('RGB')
    assert im.crop((0,0,320,184)).tobytes()==unchanged.crop((0,0,320,184)).tobytes()
    assert im.crop((0,190,320,208)).tobytes()==unchanged.crop((0,190,320,208)).tobytes()
    fg=front.crop((cam,0,cam+320,208));mask=Image.frombytes('L',fg.size,bytes(255 if p else 0 for p in fg.tobytes()))
    im.paste(fg.convert('RGB'),(0,0),mask)
    return im

frames=[scene(3808,i) for i in range(N)]
colors=sorted(set(rgb for im in frames for rgb in im.getdata()))
lookup={rgb:i for i,rgb in enumerate(colors)}
assert len(colors)<=256
palette=[c for rgb in colors for c in rgb]+[0]*(768-3*len(colors))
def gif(name,images):
    indexed=[]
    for im in images:
        p=Image.frombytes('P',im.size,bytes(lookup[rgb] for rgb in im.getdata()));p.putpalette(palette)
        assert p.convert('RGB').tobytes()==im.tobytes();indexed.append(p)
    indexed[0].save(O/name,save_all=True,append_images=indexed[1:],loop=0,duration=DURATION,disposal=2,optimize=False)
gif('scene-3x.gif',[im.resize((960,624),Image.Resampling.NEAREST) for im in frames])
gif('water-detail-4x.gif',[im.crop((0,180,320,200)).resize((1280,80),Image.Resampling.NEAREST) for im in frames])
assert animate(0).tobytes()==animate(N).tobytes()
assert scene(3808,0).tobytes()==scene(3808,N).tobytes()
for cam in (0,1800,2500,3000,3808,4352,4800):
    fg=front.crop((cam,184,cam+320,199));ref=fg.convert('RGB')
    im=scene(cam,7)
    for y in range(15):
        for x in range(320):
            if fg.getpixel((x,y)):assert im.getpixel((x,y+184))==ref.getpixel((x,y))
(O/'manifest.json').write_text(json.dumps(dict(status='procedural preview only; user review pending',
    frames=N,frame_ms=DURATION,loop_ms=N*DURATION,positions=glints,
    palette='eight rear pens; V4 adds two-pen AGA RGB8 modulation only in shore bands' if version==4 else 'unchanged eight rear pens',
    shore_bands=shore_bands if version==4 else [],shore_wave=swell if version==4 else [],
    effect='short staggered horizontal dashes; grow, brighten, shorten, disappear',
    coordinates='rear-world, quarter-speed scroll',
    runtime='prebuilt localized bitmap updates plus V4 safe Copper palette updates/restores; not implemented or timed',
    checks=['exact GIF RGB','unchanged outside water band','native eight-pen palette',
            'loop periodicity','foreground occlusion at seven cameras']),indent=2)+'\n')
print(f'PASS V{version}: exact GIF RGB, loop and foreground parity; no runtime changes')
