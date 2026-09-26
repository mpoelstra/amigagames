#!/usr/bin/env python3
"""Generate the Harrier defeat preview, audio audition and native planar art."""

import math
import wave
from pathlib import Path

from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "build/harrier-defeat-preview-v6"
RATE = 11025
PALETTE = [(229, 225, 219), (255, 238, 170), (255, 153, 34),
           (221, 68, 17), (34, 102, 204), (153, 68, 204)]


def make_visual():
    source = Image.open(ROOT / "assets/concept/sparkpaw-stormrail-gate6-native-v4-aga16.png")
    hull = source.crop((0, 0, 80, 46)).convert("RGBA")
    key = source.crop((0, 0, 80, 46))
    hull.putalpha(Image.frombytes("L", key.size,
                                  bytes(255 if p else 0 for p in key.tobytes())))
    frames = []
    native_frames = []
    # 50 Hz storyboard: localized ruptures, expanding fire, flying armour, embers.
    for tick in range(64):
        frame = Image.new("RGBA", (128, 80), (12, 16, 32, 255))
        d = ImageDraw.Draw(frame)
        d.rectangle((116, 0, 127, 79), fill=(36, 43, 69))
        d.line((118, 0, 118, 79), fill=(92, 105, 135), width=2)
        if tick < 25:
            frame.alpha_composite(hull, (19, 17))
        d = ImageDraw.Draw(frame)
        for start, cx, cy in ((0, 42, 30), (7, 76, 44), (14, 57, 37)):
            age = tick - start
            if 0 <= age < 9:
                radius = (2, 4, 5, 6, 5, 4, 3, 2, 1)[age]
                d.ellipse((cx-radius, cy-radius, cx+radius, cy+radius), fill=PALETTE[3])
                d.rectangle((cx-1, cy-1, cx+1, cy+1), fill=PALETTE[1])
        if 24 <= tick < 44:
            age = tick - 24
            # Raster flame cells, not smooth concentric polygons. Offset plumes
            # expand at different rates; deterministic grit tears their edges.
            growth = min(1, (age+2)/12)
            fade = 1 if age < 16 else max(.08, (28-age)/12)
            plumes = ((59, 39, 20, 14, 0), (43, 34, 16, 10, -1),
                      (76, 34, 19, 11, 1), (52, 49, 14, 10, 1),
                      (69, 48, 15, 9, -1), (63, 26, 12, 8, 0),
                      (36, 43, 10, 7, -1), (84, 43, 10, 7, 1))
            for y in range(7, 71):
                for x in range(4, 112):
                    value = -1.0
                    for cx, cy, rx, ry, drift in plumes:
                        cx += drift * min(age, 9)
                        ox = (x-cx)/(rx*growth*fade+1)
                        oy = (y-cy)/(ry*growth*fade+1)
                        value = max(value, 1-ox*ox-oy*oy)
                    grit = ((x*37+y*53+age//3*67) ^ (x*y*3)) & 31
                    curl = math.sin(x*.47+y*.31+age*.37)*.12 + \
                           math.sin(x*.19-y*.43-age*.29)*.13
                    value += curl + (grit-15)*.012
                    if value < .08:
                        continue
                    if age < 13 and value > .68 and grit > 8:
                        color = PALETTE[1]
                    elif value > .48 and age < 21:
                        color = PALETTE[2]
                    else:
                        color = PALETTE[3]
                    d.point((x, y), fill=color)
            if age < 13:
                for dx, dy in ((-9, -4), (6, -7), (12, 3), (-3, 6)):
                    if (age+dx)%3:
                        d.rectangle((59+dx, 39+dy, 60+dx, 40+dy), fill=PALETTE[0])
        if 25 <= tick < 62:
            age = tick - 25
            fragments = ((35, 30, -2, -1, 14, 15, 8, 5),
                         (72, 27, 1, -2, 48, 8, 7, 5),
                         (80, 44, 2, 0, 60, 25, 8, 6),
                         (44, 49, -1, 1, 25, 34, 9, 5),
                         (59, 20, 0, -2, 34, 2, 6, 4),
                         (66, 52, 1, 1, 42, 29, 7, 5))
            for sx, sy, vx, vy, cropx, cropy, width, height in fragments:
                x = sx + vx * age
                y = sy + vy * age + age*age//30
                if x < 6 or x > 106 or y < 2 or y > 73:
                    continue
                if age < 20:
                    shard = hull.crop((cropx, cropy, cropx+width, cropy+height))
                    frame.alpha_composite(shard, (x, y))
                elif age % 3:
                    d.rectangle((x, y, x+1, y+1), fill=PALETTE[2])
            for k in range(11):
                x = 59 + round((k-5)*age*.24)
                y = 39 + round(math.sin(k*7)*age*.7) + age*age//90
                if 19 < x < 106 and 2 < y < 74 and (k+age)%4:
                    d.point((x, y), fill=PALETTE[2 if age > 17 else 1])
        native_frames.append(frame.copy())
        frames.append(frame.resize((512, 320), Image.Resampling.NEAREST))
    frames[0].save(OUT / "harrier-defeat-preview.gif", save_all=True,
                   append_images=frames[1:], duration=20, loop=0, optimize=False)
    frames[34].save(OUT / "harrier-defeat-keyframe.png")
    make_native_header(native_frames, source)


def make_native_header(frames, source):
    """The review's exact native pixels, packed into blitter-ready planes."""
    rgb_palette = [tuple(source.getpalette()[i*3:i*3+3]) for i in range(16)]
    color_to_pen = {color: i for i, color in enumerate(rgb_palette)}
    words = []
    for tick in range(0, 65, 5):
        pixels = frames[min(tick, 63)].crop((0, 8, 112, 72)).convert("RGB")
        planes = [[[0 for _ in range(8)] for _ in range(64)] for _ in range(5)]
        for y in range(64):
            for x in range(112):
                color = pixels.getpixel((x, y))
                if color == (12, 16, 32):
                    continue
                pen = color_to_pen.get(color)
                if pen is None:
                    pen = min(range(16), key=lambda n: sum(
                        (color[c]-rgb_palette[n][c])**2 for c in range(3)))
                bit = 0x8000 >> (x & 15)
                planes[0][y][x >> 4] |= bit
                for plane in range(4):
                    if pen & (1 << plane):
                        planes[plane+1][y][x >> 4] |= bit
        for plane in planes:
            for row in plane:
                words.extend(row)
    header = ROOT / "src/stormrail_harrier_death_art.h"
    with header.open("w") as out:
        out.write("/* Generated from approved native preview; mask then 4 planes. */\n")
        out.write("#ifndef STORMRAIL_HARRIER_DEATH_ART_H\n")
        out.write("#define STORMRAIL_HARRIER_DEATH_ART_H\n")
        out.write("#define STORM_DEATH_FRAMES 13\n#define STORM_DEATH_W 112\n")
        out.write("#define STORM_DEATH_H 64\n#define STORM_DEATH_WORDS 8\n")
        out.write("#define STORM_DEATH_FRAME_WORDS (5*STORM_DEATH_H*STORM_DEATH_WORDS)\n")
        out.write("static const unsigned short stormHarrierDeathPlanar[STORM_DEATH_FRAMES*STORM_DEATH_FRAME_WORDS]={\n")
        for at in range(0, len(words), 12):
            out.write(" " + ",".join(f"0x{v:04x}" for v in words[at:at+12]) + ",\n")
        out.write("};\n#endif\n")


def make_audio():
    from generate_sparkpaw_sfx import harrier_defeat, pcm
    samples = pcm(harrier_defeat())
    count = len(samples)
    with wave.open(str(OUT / "harrier-defeat-preview.wav"), "wb") as wav:
        wav.setparams((1, 1, RATE, count, "NONE", "not compressed"))
        wav.writeframes(bytes(sample + 128 for sample in samples))
    (OUT / "harrier-defeat-preview.raw").write_bytes(
        bytes((sample + 256) % 256 for sample in samples))
    print(f"SFX: {count} bytes, unclipped peak {max(abs(x) for x in samples)}")


if __name__ == "__main__":
    OUT.mkdir(parents=True, exist_ok=True)
    make_visual()
    make_audio()
    print(OUT)
