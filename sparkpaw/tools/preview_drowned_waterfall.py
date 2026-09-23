"""Four-frame waterfall study combined with the frozen approved water v1 GIF."""
from pathlib import Path
import json, hashlib
from PIL import Image
from preview_drowned_native import decode
from generate_runtime_assets import save_spbm

R=Path(__file__).resolve().parents[1]
B=R/'assets/concept/drowned-water-animation-v1'
O=R/'assets/concept/drowned-waterfall-preview-v1';O.mkdir(parents=True,exist_ok=True)
protected={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in B.iterdir()}
rear=decode(R/'assets/concept/drowned-panorama-v2/drowned-rear.spbm')
front=decode(R/'build/drowned-full/assets/drowned-route.spbm')
palette=[tuple(rear.getpalette()[i:i+3]) for i in range(0,24,3)]
box=(1224,140,1256,184);original=rear.crop(box)
patches=[]
for phase in range(4):
    patch=original.copy()
    # Preserve authored cliffs and water contour. Four phases move the broken
    # stream streaks three pixels downward, wrapping a12px spatial pattern.
    for y in range(5,40):
        for x,offset in ((8,0),(9,0),(13,5),(17,8)):
            pen=original.getpixel((x,y))
            if pen<5:continue
            position=(y-phase*3+offset)%12
            if position<3:patch.putpixel((x,y),6)
            elif position==3:patch.putpixel((x,y),7)
    # Low broken foam lip at the foot; no tall particle splash in the distance.
    for j,(x,y) in enumerate(((7,40),(13,41),(20,40))):
        width=(3,5,4,2)[(phase+j)%4]
        for dx in range(width):
            px=x+dx
            if original.getpixel((px,y))>=4:patch.putpixel((px,y),7 if (phase+j)%4<2 else 6)
    patches.append(patch)
    patch.save(O/f'frame-{phase}.png')
    save_spbm(O/f'fall-{phase}.spbm',patch,palette,3)
    assert max(patch.tobytes())<8
    assert decode(O/f'fall-{phase}.spbm').tobytes()==patch.tobytes()
base=Image.open(B/'scene-3x.gif');cam=3808;sx=box[0]-cam//4;sy=box[1]
fg=front.crop((cam,0,cam+320,208))
frames=[]
for i in range(base.n_frames):
    base.seek(i);still=base.convert('RGB').resize((320,208),Image.Resampling.NEAREST)
    im=still.copy();patch=patches[i%4]
    for y in range(patch.height):
        for x in range(patch.width):
            if patch.getpixel((x,y))!=original.getpixel((x,y)) and not fg.getpixel((sx+x,sy+y)):
                im.putpixel((sx+x,sy+y),palette[patch.getpixel((x,y))])
    assert im.crop((0,184,320,208)).tobytes()==still.crop((0,184,320,208)).tobytes()
    for y in range(208):
        for x in range(320):
            if not(sx<=x<sx+32 and sy<=y<184) or fg.getpixel((x,y)):
                assert im.getpixel((x,y))==still.getpixel((x,y))
    frames.append(im)
colors=sorted(set(rgb for im in frames for rgb in im.getdata()));assert len(colors)<=256
lookup={rgb:i for i,rgb in enumerate(colors)}
pal=[c for rgb in colors for c in rgb]+[0]*(768-len(colors)*3)
def gif(name,images):
    indexed=[]
    for im in images:
        p=Image.frombytes('P',im.size,bytes(lookup[rgb] for rgb in im.getdata()));p.putpalette(pal)
        assert p.convert('RGB').tobytes()==im.tobytes();indexed.append(p)
    indexed[0].save(O/name,save_all=True,append_images=indexed[1:],duration=120,loop=0,disposal=2,optimize=False)
gif('combined-3x.gif',[im.resize((960,624),Image.Resampling.NEAREST) for im in frames])
gif('waterfall-detail-6x.gif',[im.crop((sx-8,sy-4,sx+40,200)).resize((288,384),Image.Resampling.NEAREST) for im in frames])
assert protected=={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in B.iterdir()}
(O/'manifest.json').write_text(json.dumps(dict(status='preview only; waterfall not approved',
    approved_water='drowned-water-animation-v1, byte-preserved',rear_box=box,
    waterfall_frames=4,frame_ms=120,combined_loop_ms=2880,
    raw_planar_frame_bytes=32//8*44*3,total_four_frame_bytes=4*32//8*44*3,
    checks=['SPBM exact roundtrip','fixed eight waterfall pens','exact GIF RGB',
            'water v1 frame pixels unchanged below184','outside waterfall and foreground unchanged',
            'frozen approved files hash-identical'],
    runtime='not integrated; CPU/Chip cost unmeasured'),indent=2)+'\n')
print('PASS: approved water v1 preserved; four native waterfall frames; combined preview only')
