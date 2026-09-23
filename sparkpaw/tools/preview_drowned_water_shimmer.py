"""Procedural Copper-palette effect preview; no runtime or dist writes.

The table is the proposed hardware contract: two PF2 pens in three raster
bands, full AGA RGB8 values. Runtime needs safe high/low-nibble Copper writes,
not bitmap copies. Each band still has eight colours.
"""
from pathlib import Path
import json, argparse
from PIL import Image
from preview_drowned_native import decode

R=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser()
parser.add_argument('--version',type=int,choices=(1,2),default=2)
version=parser.parse_args().version
O=R/f'assets/concept/drowned-water-shimmer-v{version}'
O.mkdir(parents=True,exist_ok=True)
rear=decode(R/'assets/concept/drowned-panorama-v2/drowned-rear.spbm')
front=decode(R/'build/drowned-full/assets/drowned-route.spbm')
pal=[tuple(rear.getpalette()[i:i+3]) for i in range(0,24,3)]
# Slow 3.2-second loop at6.25palette updates/sec, never all bands in phase.
wave=(0,2,4,6,8,10,8,6,4,2,0,-2,-4,-6,-8,-10,-8,-6,-4,-2)
bands=((188,192,0),(192,196,7),(196,200,13))
if version==2:
    # Brief stronger glints with a longer quiet trough; stagger the bands.
    wave=(-8,-8,-6,-2,8,24,44,64,48,28,12,0,-6,-8,-8,-8,-8,-8,-8,-8)
    bands=((184,190,0),(190,195,7),(195,200,13))
start_y=bands[0][0]
update_ms=120 if version==2 else 160
table=[]
for frame in range(len(wave)):
    rows=[]
    for top,bottom,phase in bands:
        colors={}
        for pen,offset in ((6,0),(7,5)):
            delta=wave[(frame+phase+offset)%len(wave)]
            colors[pen]=tuple(max(0,min(255,c+delta)) for c in pal[pen])
        rows.append(dict(top=top,bottom=bottom,colors=colors))
    table.append(rows)

def scene(cam,frame):
    indices=rear.crop((cam//4,0,cam//4+320,208))
    image=indices.convert('RGB')
    for band in table[frame]:
        for y in range(band['top'],band['bottom']):
            for x in range(320):
                pen=indices.getpixel((x,y))
                if pen in band['colors']:image.putpixel((x,y),band['colors'][pen])
    baseline=indices.convert('RGB')
    assert image.crop((0,0,320,start_y)).tobytes()==baseline.crop((0,0,320,start_y)).tobytes()
    assert image.crop((0,200,320,208)).tobytes()==baseline.crop((0,200,320,208)).tobytes()
    fg=front.crop((cam,0,cam+320,208))
    mask=Image.frombytes('L',fg.size,bytes(255 if p else 0 for p in fg.tobytes()))
    image.paste(fg.convert('RGB'),(0,0),mask)
    return image

# A single fixed GIF palette prevents per-frame adaptive quantization flicker.
frames=[scene(3808,i) for i in range(20)]
colors=set()
for f in frames:colors.update(f.getdata())
assert len(colors)<=256
gifpal=Image.new('P',(1,1)); flat=[c for rgb in sorted(colors) for c in rgb]
gifpal.putpalette(flat+[0]*(768-len(flat)))
def save_gif(name,images):
    lookup={rgb:i for i,rgb in enumerate(sorted(colors))}
    indexed=[]
    for im in images:
        frame=Image.frombytes('P',im.size,bytes(lookup[rgb] for rgb in im.getdata()))
        frame.putpalette(gifpal.getpalette());indexed.append(frame)
    for original,index in zip(images,indexed): assert index.convert('RGB').tobytes()==original.tobytes()
    indexed[0].save(O/name,save_all=True,append_images=indexed[1:],duration=update_ms,loop=0,optimize=False,disposal=2)
save_gif('scene-3x.gif',[f.resize((960,624),Image.Resampling.NEAREST) for f in frames])
save_gif('water-detail-6x.gif',[f.crop((0,180,320,200)).resize((1920,120),Image.Resampling.NEAREST) for f in frames])
# Prove foreground is invariant and every frame matches the same palette law.
for cam in (0,1800,2500,3000,3808,4352,4800):
    fg=front.crop((cam,0,cam+320,208)); reference=fg.convert('RGB')
    for frame in (0,5,10,15):
        im=scene(cam,frame)
        for y in range(start_y,200):
            for x in range(320):
                if fg.getpixel((x,y)):assert im.getpixel((x,y))==reference.getpixel((x,y))
(O/'palette-sequence.json').write_text(json.dumps(dict(status='concept preview, not runtime integrated',
    loop_ms=20*update_ms,update_ms=update_ms,world_scroll='cameraX/4',bands=bands,frames=table,
    runtime_requirements=['safe inactive Copper list updates','full AGA high/low nibble writes',
                          'restore original PF2 palette at y200 and protect HUD','no bitmap animation'],
    caveat='global within each horizontal band: shore pixels sharing pens6/7 also vary',
    checks=['GIF exact RGB roundtrip','outside-band pixels unchanged','foreground invariant at seven cameras'],
    additional_bitmap_bytes=0,performance='unmeasured; proposed colour changes only'),indent=2)+'\n')
print('PASS: fixed-palette GIF exact RGB, outside-band and foreground parity; preview only')
