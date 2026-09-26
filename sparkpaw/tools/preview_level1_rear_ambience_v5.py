#!/usr/bin/env python3
"""Render a non-runtime Level 1 rear-animation study from exact SPBM indices."""
from __future__ import annotations

import json
import math
import hashlib
from pathlib import Path

from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "assets/concept/level1-rear-ambience-study-v5"
REAR = ROOT / "assets/runtime/storm-rear.spbm"
FRONT = ROOT / "assets/runtime/storm-front.spbm"
CAMERA = 0

BANDS = (
    ((0, 0, 1), (0, 0, 4), (1, 1, 6), (2, 1, 8),
     (4, 2, 9), (6, 4, 11), (9, 6, 12), (13, 11, 14)),
    ((0, 0, 1), (1, 1, 4), (2, 10, 13), (3, 3, 7),
     (4, 3, 9), (6, 5, 10), (9, 7, 12), (12, 10, 13)),
    ((0, 0, 1), (0, 1, 2), (0, 2, 3), (1, 3, 4),
     (1, 4, 4), (2, 5, 5), (4, 6, 6), (6, 8, 9)),
)


def decode(path: Path):
    data = path.read_bytes()
    assert data[:4] == b"SPBM"
    width = int.from_bytes(data[4:6], "big")
    height = int.from_bytes(data[6:8], "big")
    depth = data[8]
    assert not data[9]
    stride = int.from_bytes(data[10:12], "big")
    palette = [tuple(data[12 + i * 3:15 + i * 3]) for i in range(1 << depth)]
    offset = 12 + (1 << depth) * 3
    plane_bytes = stride * height
    assert len(data) == offset + plane_bytes * depth
    pixels = []
    for y in range(height):
        row = []
        for x in range(width):
            at = y * stride + x // 8
            mask = 0x80 >> (x & 7)
            row.append(sum(1 << plane for plane in range(depth)
                           if data[offset + plane * plane_bytes + at] & mask))
        pixels.append(row)
    return pixels, palette


def rear_palette(y):
    if y < 68:
        source, target, step, steps = 0, 0, 0, 1
    elif y < 84:
        source, target, step, steps = 0, 1, min(4, (y - 68) // 4 + 1), 4
    elif y < 139:
        source, target, step, steps = 1, 1, 0, 1
    elif y < 163:
        source, target, step, steps = 1, 2, min(8, (y - 139) // 3 + 1), 8
    else:
        source, target, step, steps = 2, 2, 0, 1
    return [tuple((BANDS[source][pen][component] * (steps - step)
                   + BANDS[target][pen][component] * step) // steps * 17
                  for component in range(3)) for pen in range(8)]


# Authored centreline follows the original white bolt, then the tower tip
# towards the existing crystal. The moving pulse restores from the source.
BOLT = ((214,28),(217,30),(215,34),(214,36),(212,39),(210,42),
        (213,45),(216,49),(219,52),(221,55),(219,58),(216,61),
        (213,64),(210,68),(208,71),(209,74),(212,78),(214,82))
SITES = ((24,35),(105,16),(275,35),(410,27),(554,20),(711,35),(868,24),(1030,25))
CLOUD_PATH = ((0,8),(7,7),(11,3),(18,6),(24,4),(28,10),(35,8),(41,13),(48,10))
SITES += ((60,60),(130,60),(325,60),(482,60),(630,60),(780,60),(935,60),(1052,60))
N = 48


def line_points(points):
    result=[]
    for (x0,y0),(x1,y1) in zip(points,points[1:]):
        steps=max(abs(x1-x0),abs(y1-y0))
        for i in range(steps):
            xy=(round(x0+(x1-x0)*i/steps),round(y0+(y1-y0)*i/steps))
            if not result or result[-1]!=xy: result.append(xy)
    result.append(points[-1])
    return result


def indexed_image(pixels):
    im=Image.new('L',(len(pixels[0]),len(pixels)))
    im.putdata([p for row in pixels for p in row])
    return im


def render(pixels, front=None, front_palette=None, camera=0):
    width=320 if front is not None else 1120
    im=Image.new('RGB',(width,208)); out=im.load()
    for y in range(208):
        pal=rear_palette(y)
        for x in range(width):
            fp=front[y][camera+x] if front is not None else 0
            out[x,y]=front_palette[fp] if fp else pal[pixels[y][x+(camera//4 if front is not None else 0)]]
    return im


def animate(original,t):
    p=[row[:] for row in original]
    # A complete downward transit every 24 frames (2.88 seconds).
    phase=t%24
    path=line_points(BOLT)
    if 3<=phase<=14:
        progress=(phase-3)/11
        center=round(progress*(len(path)-1))
        for x,y in path:
            if y>=72: continue
            for dx in (-2,-1,1,2):
                base=original[y][x+dx]
                if base in (5,6): p[y][x+dx]=max(p[y][x+dx],6)
        # Low-amplitude local glow is attached to the original bolt silhouette.
        for i,(x,y) in enumerate(path):
            distance=abs(i-center)
            if distance>10: continue
            radius=3 if distance<5 else 2
            for dy in range(-1,2):
                for dx in range(-radius,radius+1):
                    xx,yy=x+dx,y+dy
                    if yy>=78: continue
                    base=original[yy][xx]
                    if abs(dx)<=1 and distance<4:
                        p[yy][xx]=max(base,7)
                    elif base>=3:
                        p[yy][xx]=max(p[yy][xx],min(6,base+1))
    crystal=[(x,y) for y in range(81,85) for x in range(211,217) if original[y][x]==2]
    if 14<=phase<=18:
        for x,y in crystal:
            p[y][x]=7 if phase<=15 else (6 if phase==16 else 2)
        if phase<=16:
            for x,y in crystal:
                for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)):
                    xx,yy=x+dx,y+dy
                    if original[yy][xx] in (1,3,4,5): p[yy][xx]=2
    # Local discharges distributed across the entire finite rear panorama.
    for site,(sx,sy) in enumerate(SITES):
        age=(t-site*5)%N
        if age>=9: continue
        points=line_points([(sx+x,sy+(16-y if site%2 else y)+(i%3 if site%3==1 else 0))
                            for i,(x,y) in enumerate(CLOUD_PATH)])
        head=round(min(1,age/5)*(len(points)-1))
        for i,(x,y) in enumerate(points):
            behind=head-i
            if not 0<=behind<=16: continue
            pen=7 if behind<4 and age<6 else (6 if behind<10 else 5)
            if age>=6: pen=max(4,pen-(age-5))
            p[y][x]=max(original[y][x],pen)
            # A few existing cloud-edge pixels catch the local discharge.
            for dx,dy in ((0,-1),(0,1),(-1,0),(1,0)):
                xx,yy=x+dx,y+dy
                base=original[yy][xx]
                if age<6 and base in (4,5): p[yy][xx]=max(p[yy][xx],base+1)
    # Existing cyan slit in the user-indicated building at rear x771.
    age=(t-7)%24
    core=[(771,y) for y in range(117,122)]
    # Only the five original cyan pixels blink; no halo or brighter outline.
    for x,y in core:
        assert original[y][x]==2
        p[y][x]=1 if age in (2,3,16) else 2
    return p


def save_gif(path,frames,durations):
    frames[0].save(path,save_all=True,append_images=frames[1:],duration=durations,
                   loop=0,disposal=1,optimize=False)


def main():
    OUT.mkdir(parents=True,exist_ok=True)
    rear,_=decode(REAR); front,front_palette=decode(FRONT)
    scene=[]; sky=[]; details=[]; route=[]; records=[]; building=[]
    baseline=render(rear,front,front_palette)
    baseline.save(OUT/'original-native.png')
    for t in range(N):
        pixels=animate(rear,t)
        assert all(0<=v<8 for row in pixels for v in row)
        frame=render(pixels,front,front_palette)
        frame.save(OUT/f'frame-{t:02d}-native.png')
        indexed_image(pixels).save(OUT/f'frame-{t:02d}-indices.png')
        building.append(render(pixels).crop((750,104,790,144)).resize((320,320),Image.Resampling.NEAREST))
        scene.append(frame.resize((960,624),Image.Resampling.NEAREST))
        sky.append(render(pixels).resize((1120,208),Image.Resampling.NEAREST))
        details.append(frame.crop((192,20,240,96)).resize((384,608),Image.Resampling.NEAREST))
        # Four fixed cameras show the early, middle and final gameplay skies.
        sheet=Image.new('RGB',(640,416))
        for k,camera in enumerate((0,1024,2048,3072)):
            sheet.paste(render(pixels,front,front_palette,camera),((k%2)*320,(k//2)*208))
        route.append(sheet)
        changes=[(x,y) for y in range(208) for x in range(1120) if pixels[y][x]!=rear[y][x]]
        assert all(y<90 or (768<=x<784 and 112<=y<128) for x,y in changes)
        records.append({'frame':t,'tower_phase':t%24,'changed_pixels':len(changes)})
    save_gif(OUT/'building-detail-8x.gif',building,[120]*N)
    save_gif(OUT/'sequence-3x.gif',scene,[120]*N)
    save_gif(OUT/'sky-panorama.gif',sky,[120]*N)
    save_gif(OUT/'tower-detail-8x.gif',details,[120]*N)
    save_gif(OUT/'four-cameras.gif',route,[120]*N)
    # Labelled storyboard keeps the downward sequence visible in still form.
    picks=(3,6,9,12,14,16)
    board=Image.new('RGB',(960,464),(8,8,24)); draw=ImageDraw.Draw(board)
    for k,t in enumerate(picks):
        x=(k%3)*320;y=(k//3)*232
        panel=Image.open(OUT/f'frame-{t:02d}-native.png')
        board.paste(panel,(x,y+24))
        draw.text((x+8,y+6),('Upper','Descending','Middle','Tower tip','Crystal flash','Afterglow')[k],fill='white')
    board.save(OUT/'storyboard.png')
    manifest={'status':'v5 candidate: denser sky and user-indicated building pulse; v3 retained',
              'source_sha256':hashlib.sha256(REAR.read_bytes()).hexdigest(),
              'pulse_path':BOLT,'sky_sites':SITES,'step_ms':120,'loop_ms':N*120,
              'tower_cycle_ms':24*120,'frames':records,
              'note':'3-plane indexed frames; no native beam/cadence proof; existing inactive-rear scheduling; native v5 cadence pending'}
    (OUT/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print(OUT)


if __name__=='__main__': main()
