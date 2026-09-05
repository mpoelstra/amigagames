#!/usr/bin/env python3
"""Reduce one approved Debris-2 concept mass to a native 48x40 underpainting.

This is deliberately a one-object art study. It does not feed the runtime.
The reduction preserves concept volume for subsequent native pixel cleanup.
"""

from collections import deque
from pathlib import Path
from PIL import Image

from generate_runtime_assets import FRONT16

ROOT = Path(__file__).resolve().parents[1]
INPUT = ROOT / "assets/concept/sparkpaw-stormrail-debris-study-v2.png"
OUTPUT = ROOT / "assets/concept/sparkpaw-stormrail-debris-native-v4-aga16.png"
REVIEW = ROOT / "assets/concept/sparkpaw-stormrail-debris-native-v4-review-8x.png"


def is_background(rgb):
    r, g, b = rgb
    return r > 145 and b > 145 and g < 105 and abs(r - b) < 105


def components(image):
    w, h = image.size
    px = image.load()
    seen = bytearray(w * h)
    found = []
    for y in range(h):
        for x in range(w):
            at = y * w + x
            if seen[at] or is_background(px[x, y]):
                seen[at] = 1
                continue
            queue = deque([(x, y)])
            seen[at] = 1
            x0 = x1 = x
            y0 = y1 = y
            count = 0
            while queue:
                cx, cy = queue.popleft()
                count += 1
                x0 = min(x0, cx); x1 = max(x1, cx)
                y0 = min(y0, cy); y1 = max(y1, cy)
                for nx, ny in ((cx-1,cy),(cx+1,cy),(cx,cy-1),(cx,cy+1)):
                    if nx < 0 or ny < 0 or nx >= w or ny >= h:
                        continue
                    nat = ny * w + nx
                    if seen[nat]:
                        continue
                    seen[nat] = 1
                    if not is_background(px[nx, ny]):
                        queue.append((nx, ny))
            if count > 500:
                found.append((count, (x0, y0, x1 + 1, y1 + 1)))
    return found


def nearest_pen(rgb):
    return min(range(1, 16), key=lambda pen: sum(
        (rgb[channel] - FRONT16[pen][channel]) ** 2 for channel in range(3)))


def main():
    source = Image.open(INPUT).convert("RGB")
    candidates = [item for item in components(source) if item[1][1] < 430]
    if not candidates:
        raise RuntimeError("no large top-row concept object found")
    # The top-left large buttress mass is the first native feasibility target.
    _, box = min(candidates, key=lambda item: (item[1][0], -item[0]))
    crop = source.crop(box)
    mask = Image.new("L", crop.size, 0)
    mp = mask.load(); cp = crop.load()
    for y in range(crop.height):
        for x in range(crop.width):
            if not is_background(cp[x, y]):
                mp[x, y] = 255
    rgba = crop.convert("RGBA")
    rgba.putalpha(mask)

    scale = min(46 / crop.width, 38 / crop.height)
    size = (max(1, round(crop.width * scale)),
            max(1, round(crop.height * scale)))
    reduced = rgba.resize(size, Image.Resampling.LANCZOS)
    canvas = Image.new("P", (48, 40), 0)
    canvas.putpalette([value for rgb in FRONT16 for value in rgb] +
                      [0] * (768 - 48))
    ox = (48 - size[0]) // 2
    oy = (40 - size[1]) // 2
    rp = reduced.load(); out = canvas.load()
    for y in range(size[1]):
        for x in range(size[0]):
            r, g, b, a = rp[x, y]
            if a >= 112:
                out[ox+x, oy+y] = nearest_pen((r, g, b))
    # Black is valid inside genuinely occluded crevices, but never as a comic
    # keyline. Replace only black pixels touching transparency; preserve the
    # concept's internal depth and texture exactly.
    original = canvas.copy()
    src = original.load()
    for y in range(40):
        for x in range(48):
            if src[x, y] != 1:
                continue
            touches_alpha = any(
                nx < 0 or ny < 0 or nx >= 48 or ny >= 40 or src[nx, ny] == 0
                for nx, ny in ((x-1,y),(x+1,y),(x,y-1),(x,y+1)))
            if touches_alpha:
                out[x, y] = 8
    # The source has one broad crushed-black underside after quantization.
    # Lighten only black pixels already supported by two neighbouring steel
    # shadow pixels; this grows coherent planes instead of adding dithering.
    original = canvas.copy()
    src = original.load()
    for y in range(22, 39):
        for x in range(7, 41):
            if src[x, y] != 1:
                continue
            steel_neighbours = sum(
                src[nx, ny] == 8
                for nx, ny in ((x-1,y),(x+1,y),(x,y-1),(x,y+1)))
            if steel_neighbours >= 2:
                out[x, y] = 8
    # Two exact native Stormstone glints anchor this as Storm Ruins material;
    # they are embedded in the central rib rather than drawn as a neon crack.
    out[24, 18] = 5
    out[25, 18] = 6
    canvas.info["transparency"] = 0
    canvas.save(OUTPUT)

    rgba_out = canvas.convert("RGBA")
    alpha = Image.new("L", canvas.size, 0)
    ap = alpha.load(); op = canvas.load()
    for y in range(40):
        for x in range(48):
            if op[x, y]: ap[x, y] = 255
    rgba_out.putalpha(alpha)
    review = Image.new("RGB", (48 * 8, 40 * 8), (255, 0, 255))
    scaled = rgba_out.resize(review.size, Image.Resampling.NEAREST)
    review.paste(scaled, (0, 0), scaled.getchannel("A"))
    review.save(REVIEW)
    print(f"component={box} reduced={size} offset={ox},{oy}")
    print(f"wrote {OUTPUT}")
    print(f"wrote {REVIEW}")


if __name__ == "__main__":
    main()
