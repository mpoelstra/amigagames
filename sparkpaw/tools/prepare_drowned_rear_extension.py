"""Review-only assembly of generated continuation; never writes runtime/dist."""
from pathlib import Path
import json
from PIL import Image, ImageDraw
from preview_drowned_native import indexed, decode
from generate_runtime_assets import save_spbm

R = Path(__file__).resolve().parents[1]
O = R / 'assets/concept/drowned-panorama-v2'
old = Image.open(R / 'assets/concept/drowned-panorama-v1/rear-indexed.png')
palette = [tuple(old.getpalette()[i:i+3]) for i in range(0,24,3)]
# Generated continuation includes surplus lake at bottom. Align its waterline
# with the authored rear before palette conversion; no runtime blending.
source = Image.open(O / 'extension-source.png').convert('RGB')
extension = indexed(source.crop((0,0,2172,672)).resize((752,208), Image.Resampling.LANCZOS), palette)
# Choose a connected low-contrast join inside the overlapping landscape.
# Each output pixel comes from one source: no crossfade/ghosting/dither seam.
lo, hi = 32, 280
cost = []
parents = []
for y in range(208):
    row, parent = [], []
    for x in range(lo,hi):
        a = palette[old.getpixel((800+x,y))]
        b = palette[extension.getpixel((x,y))]
        error = sum((a[c]-b[c])**2 for c in range(3))
        if y:
            candidates = range(max(0,x-lo-1), min(hi-lo,x-lo+2))
            best = min(candidates, key=lambda k: cost[-1][k]+abs(k-(x-lo))*12)
            error += cost[-1][best]+abs(best-(x-lo))*12
            parent.append(best)
        row.append(error)
    cost.append(row)
    parents.append(parent)
x = min(range(hi-lo), key=lambda k: cost[-1][k])
seam = [0]*208
for y in range(207,-1,-1):
    seam[y] = lo+x
    if y: x = parents[y][x]
rear = Image.new('P',(1552,208)); rear.putpalette(old.getpalette())
rear.paste(old,(0,0)); rear.paste(extension,(800,0))
for y, x in enumerate(seam):
    rear.paste(old.crop((0,y,800+x,y+1)),(0,y))
rear.save(O/'rear-indexed.png')
rear.resize((3104,416),Image.Resampling.NEAREST).save(O/'panorama-2x.png')
save_spbm(O/'drowned-rear.spbm',rear,palette,3)
assert decode(O/'drowned-rear.spbm').tobytes() == rear.tobytes()
assert rear.crop((0,0,832,208)).tobytes() == old.crop((0,0,832,208)).tobytes()
assert max(rear.tobytes()) < 8
for camera in range(4801): assert camera//4+352 <= rear.width
front = decode(R/'build/drowned-full/assets/drowned-route.spbm')
sheet = Image.new('RGB',(960,456)); draw = ImageDraw.Draw(sheet)
for i,cam in enumerate((3504,3808,4096,4352,4608,4800)):
    frame = rear.crop((cam//4,0,cam//4+320,208)).convert('RGB')
    fg = front.crop((cam,0,cam+320,208))
    mask = Image.frombytes('L',fg.size,bytes(255 if p else 0 for p in fg.tobytes()))
    frame.paste(fg.convert('RGB'),(0,0),mask)
    x,y=(i%3)*320,(i//3)*228
    sheet.paste(frame,(x,y+20)); draw.text((x+5,y+4),f'Camera {cam}',fill='white')
sheet.save(O/'finale-review.png')
(O/'manifest.json').write_text(json.dumps(dict(status='review only, not integrated',
    dimensions=list(rear.size),palette=palette,unchanged_prefix=832,
    join_range=[800+min(seam),800+max(seam)],world_width=5120,
    maximum_camera=4800,conservative_fetch_pixels=352,
    extra_planar_bytes_vs_current=32//8*208*3,
    checks=['exact SPBM roundtrip','same eight pens','all camera fetch windows fit',
            'first832 columns unchanged'],limitations=['offline composition, no native FPS measurement']),indent=2)+'\n')
print(O/'finale-review.png')
