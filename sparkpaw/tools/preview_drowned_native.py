#!/usr/bin/env python3
"""Offline art study only. Never writes runtime assets or dist."""
from pathlib import Path
import hashlib
import json
import struct
from PIL import Image, ImageDraw
from generate_runtime_assets import FRONT16, save_spbm

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'assets/concept/drowned-native-v1'
SOURCE = ROOT / 'assets/concept/drowned-turbines-slice-source-v1.png'


def decode(path):
    data = path.read_bytes()
    assert data[:4] == b'SPBM'
    w, h, depth, masked, stride = struct.unpack('>HHBBH', data[4:12])
    palette = list(data[12:12 + (1 << depth)*3])
    image = Image.new('P', (w, h))
    image.putpalette(palette + [0]*(768-len(palette)))
    start = 12 + len(palette)
    for y in range(h):
        for x in range(w):
            pen = 0
            for plane in range(depth):
                pen |= ((data[start + plane*stride*h + y*stride + x//8]
                         >> (7-x%8)) & 1) << plane
            image.putpixel((x, y), pen)
    return image


def indexed(source, palette, first=0):
    out = Image.new('P', source.size)
    out.putpalette([c for rgb in palette for c in rgb] + [0]*(768-3*len(palette)))
    lookup = {}
    for y in range(source.height):
        for x in range(source.width):
            rgb = source.getpixel((x, y))
            if rgb not in lookup:
                lookup[rgb] = min(range(first, len(palette)), key=lambda p:
                    sum((rgb[c]-palette[p][c])**2 for c in range(3)))
            out.putpixel((x, y), lookup[rgb])
    return out


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    # Author-reviewed source is underpainted to the native height. Crop away
    # the reflected foreground below the ground lip; no scrolling proof implied.
    source = Image.open(SOURCE).convert('RGB').crop((0, 0, 1536, 852))
    source = source.resize((320, 208), Image.Resampling.LANCZOS)
    # One constant eight-pen rear bank, quantized to representable 12-bit RGB.
    # No scanline palette additions or 4+4 assumption in this first study.
    q = source.quantize(colors=7, method=Image.Quantize.MEDIANCUT)
    colors = [tuple(q.getpalette()[i:i+3]) for i in range(0, 21, 3)]
    colors.sort(key=lambda c: sum(c))
    rear_palette = [(0, 0, 17)] + [tuple(round(c/17)*17 for c in rgb) for rgb in colors]
    rear = indexed(source, rear_palette)
    mask = Image.new('L', (320, 208))
    draw = ImageDraw.Draw(mask)
    draw.rectangle((0, 200, 319, 207), fill=255)
    # Source-aligned silhouette of right raised platform and its two supports.
    draw.polygon([(216,140),(319,140),(319,152),(304,152),
                  (304,200),(295,200),(295,157),(281,157),(272,153),
                  (261,153),(254,160),(254,200),(242,200),(242,160),
                  (230,157),(230,151),(216,151)], fill=255)
    foreground = indexed(source, FRONT16, first=1)
    foreground.paste(0, (0, 0, 320, 208), Image.eval(mask, lambda v:255-v))
    foreground.save(OUT/'front-indexed.png')
    rear.save(OUT/'rear-indexed.png')
    mask.save(OUT/'foreground-mask.png')
    save_spbm(OUT/'front.spbm', foreground, FRONT16, depth=4)
    save_spbm(OUT/'rear.spbm', rear, rear_palette, depth=3)
    for name, original, limit in [('front',foreground,16),('rear',rear,8)]:
        decoded = decode(OUT/(name+'.spbm'))
        assert decoded.tobytes() == original.tobytes()
        assert original.getextrema()[1] < limit
        assert decoded.getpalette() == original.getpalette()
    frame = rear.convert('RGB')
    frame.paste(foreground.convert('RGB'), (0,0), mask)
    # Exact existing player frame 0 at 48x48: no re-paint, rescale or recolour.
    player = decode(ROOT/'assets/runtime/sparkpaw-sprites4.spbm').crop((0,0,48,48))
    # Form the transparency mask from indices, not palette luminance.
    alpha = Image.frombytes('L', player.size, bytes(255 if p else 0 for p in player.tobytes()))
    bbox = alpha.getbbox()
    player_pos = (62,200-bbox[3])
    frame.paste(player.convert('RGB'),player_pos,alpha)
    full = Image.new('RGB',(320,256))
    full.paste(frame,(0,0))
    hud = decode(ROOT/'assets/runtime/sparkpaw-hud-base.spbm').convert('RGB')
    # Copy authored HUD panel atlas cells, no invented HUD typography.
    for name, cell, at in [('health',(0,6*32,80,7*32),(48,12)),
                           ('lives',(0,2*24,32,3*24),(160,12)),
                           ('diamonds',(0,0,32,24),(224,12))]:
        atlas = decode(ROOT/f'assets/runtime/sparkpaw-hud-{name}.spbm').convert('RGB')
        hud.paste(atlas.crop(cell),at)
    digits = decode(ROOT/'assets/runtime/sparkpaw-hud-score.spbm').convert('RGB')
    for i in range(4):
        hud.paste(digits.crop((0,i*190,8,i*190+19)),(272+i*8,16))
    full.paste(hud.crop((2,0,322,48)),(0,208))
    full.save(OUT/'preview-1x.png')
    full.resize((1280,1024),Image.Resampling.NEAREST).save(OUT/'preview-4x.png')
    source.save(OUT/'source-native.png')
    report = dict(status='offline indexed art study; user review pending; no runtime integration',
        size=[320,256], playfield=[320,208], front_palette=FRONT16,
        rear_palette=rear_palette, rear_palette_mode='one fixed 8-pen 12-bit bank',
        planar_bytes={'front':33280,'rear':24960,'total':58240},
        player=dict(frame=0,cell=[48,48],position=player_pos,opaque_bounds=bbox,
                    unchanged=True),
        limitations=['single static screen; foreground mask is source-aligned study',
            'rear still contains hidden foreground underpaint: not a clean scrolling layer',
            'no collision, parallax seams, native timing or allocator proof',
            'no enemy, jet, water animation or Copper band morph added'],
        hashes={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()
                for p in [SOURCE, ROOT/'assets/runtime/sparkpaw-sprites4.spbm']})
    (OUT/'manifest.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))


if __name__ == '__main__':
    main()
