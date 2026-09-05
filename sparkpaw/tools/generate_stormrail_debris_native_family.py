#!/usr/bin/env python3
"""Build the complete Debris-2 native review family from approved study v2.

Every concept component is reduced independently into its own exact-size cell,
then receives the same bounded native edge/shadow cleanup as approved object v4.
This remains concept/review output and is not consumed by the game.
"""

from pathlib import Path
from PIL import Image

from generate_runtime_assets import FRONT16
from generate_stormrail_debris_native_v2_study import (
    INPUT, components, is_background, nearest_pen,
)

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "assets/concept/sparkpaw-stormrail-debris-native-family-v1-aga16.png"
REVIEW = ROOT / "assets/concept/sparkpaw-stormrail-debris-native-family-v1-review-4x.png"
SHEET_W, SHEET_H = 192, 96


def blank_indexed(size):
    image = Image.new("P", size, 0)
    image.putpalette([value for rgb in FRONT16 for value in rgb] +
                     [0] * (768 - 48))
    return image


def reduce_component(source, box, cell_size, margin):
    crop = source.crop(box)
    mask = Image.new("L", crop.size, 0)
    mp = mask.load(); cp = crop.load()
    for y in range(crop.height):
        for x in range(crop.width):
            if not is_background(cp[x, y]):
                mp[x, y] = 255
    rgba = crop.convert("RGBA")
    rgba.putalpha(mask)

    cell_w, cell_h = cell_size
    scale = min((cell_w - margin * 2) / crop.width,
                (cell_h - margin * 2) / crop.height)
    size = (max(1, round(crop.width * scale)),
            max(1, round(crop.height * scale)))
    reduced = rgba.resize(size, Image.Resampling.LANCZOS)
    cell = blank_indexed(cell_size)
    out = cell.load(); rp = reduced.load()
    ox = (cell_w - size[0]) // 2
    oy = (cell_h - size[1]) // 2
    for y in range(size[1]):
        for x in range(size[0]):
            r, g, b, a = rp[x, y]
            if a >= 112:
                out[ox+x, oy+y] = nearest_pen((r, g, b))

    # Remove only black pixels that touch transparency. Interior black remains
    # available for truly occluded joins and deep perspective fractures.
    original = cell.copy(); src = original.load()
    for y in range(cell_h):
        for x in range(cell_w):
            if src[x, y] != 1:
                continue
            touches_alpha = any(
                nx < 0 or ny < 0 or nx >= cell_w or ny >= cell_h or
                src[nx, ny] == 0
                for nx, ny in ((x-1,y),(x+1,y),(x,y-1),(x,y+1)))
            if touches_alpha:
                out[x, y] = 8

    # Join black underside pixels to an existing steel plane only when at least
    # two orthogonal neighbours already establish that plane. This is bounded
    # cleanup, not a global blur or palette remap.
    original = cell.copy(); src = original.load()
    for y in range(cell_h * 11 // 20, cell_h - 1):
        for x in range(1, cell_w - 1):
            if src[x, y] != 1:
                continue
            steel = sum(src[nx, ny] == 8 for nx, ny in
                        ((x-1,y),(x+1,y),(x,y-1),(x,y+1)))
            if steel >= 2:
                out[x, y] = 8
    cell.info["transparency"] = 0
    return cell


def main():
    source = Image.open(INPUT).convert("RGB")
    found = components(source)
    large = sorted((item for item in found if item[1][1] < 430),
                   key=lambda item: item[1][0])
    medium = sorted((item for item in found if 430 <= item[1][1] < 760),
                    key=lambda item: item[1][0])
    small = sorted((item for item in found if item[1][1] >= 760),
                   key=lambda item: item[1][0])
    if tuple(map(len, (large, medium, small))) != (4, 4, 4):
        raise RuntimeError("expected four unique concept components per row")

    sheet = blank_indexed((SHEET_W, SHEET_H))
    cell_hashes = set()
    for index, (_, box) in enumerate(large):
        cell = reduce_component(source, box, (48, 40), 1)
        sheet.paste(cell, (index * 48, 0))
        cell_hashes.add(cell.tobytes())
    for index, (_, box) in enumerate(medium):
        cell = reduce_component(source, box, (32, 40), 1)
        sheet.paste(cell, (index * 40 + 16, 44))
        cell_hashes.add(cell.tobytes())
    for index, (_, box) in enumerate(small):
        cell = reduce_component(source, box, (16, 16), 1)
        sheet.paste(cell, (index * 20 + 112, 80))
        cell_hashes.add(cell.tobytes())
    if len(cell_hashes) != 12:
        raise RuntimeError("native cells must remain unique")

    sheet.info["transparency"] = 0
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(OUTPUT)

    rgba = sheet.convert("RGBA")
    alpha = Image.new("L", sheet.size, 0)
    ap = alpha.load(); sp = sheet.load()
    for y in range(SHEET_H):
        for x in range(SHEET_W):
            if sp[x, y]: ap[x, y] = 255
    rgba.putalpha(alpha)
    scaled = rgba.resize((SHEET_W * 4, SHEET_H * 4),
                         Image.Resampling.NEAREST)
    review = Image.new("RGB", scaled.size, (255, 0, 255))
    review.paste(scaled, (0, 0), scaled.getchannel("A"))
    review.save(REVIEW)
    print(f"wrote {OUTPUT}")
    print(f"wrote {REVIEW}")
    print("cells=12 unique=12 layout=4x48x40/4x32x40/4x16x16")


if __name__ == "__main__":
    main()
