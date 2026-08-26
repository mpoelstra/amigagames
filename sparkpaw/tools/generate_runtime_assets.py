#!/usr/bin/env python3
"""Generate the compact planar assets used by the Amiga milestone build.

The files use a deliberately tiny format so the 68020 code does not need a
general PNG/IFF decoder while the playfield is running:

  SPBM + width(u16be) + height(u16be) + depth(u8) + reserved(u8)
       + row_bytes(u16be) + palette ((1 << depth) * RGB bytes) + bitmap data

The sprite mask is stored directly after its four image planes.
"""

from __future__ import annotations

import json
import math
import struct
from collections import Counter
from pathlib import Path

from PIL import Image, ImageDraw, ImageOps

ROOT = Path(__file__).resolve().parents[1]
RUNTIME = ROOT / "assets" / "runtime"
LEVELS = ROOT / "assets" / "levels"
WORLD_W, WORLD_H = 3392, 256
GAMEPLAY_H = 208
# Preserve the accepted 1024px rear master byte-for-byte and add a guarded
# 96px continuation for the new quarter-speed camera reach. 1120px keeps the
# three-plane row stride longword aligned for the production renderer.
PARALLAX_ACCEPTED_W = 1024
PARALLAX_W = 1120
TILE = 16
BEETLE_W, BEETLE_H = 32, 24
BEETLE_FRAMES = 9
STRIDER_W, STRIDER_H = 64, 64
STRIDER_FRAMES = 28
CORE_RUNTIME_FRAMES = 18
STRIDER_FORBIDDEN_PENS = {2, 3}  # Sparkpaw orange, never Strider identity
STRIDER_NEUTRAL_PENS = {1, 8, 9, 10, 11}
STRIDER_VIOLET_PENS = {7, 12, 13, 14, 15}

FG_PALETTE = [
    (0, 0, 0), (10, 8, 18), (29, 22, 39), (238, 242, 224),
    (158, 164, 178), (76, 70, 91), (211, 55, 32), (255, 132, 42),
    (24, 77, 150), (54, 151, 225), (58, 142, 72), (77, 210, 184),
    (119, 57, 145), (244, 216, 64), (229, 38, 54), (111, 62, 35),
]

BG_PALETTE = [
    (3, 5, 18), (7, 12, 35), (10, 20, 52), (15, 31, 67),
    (24, 43, 78), (38, 55, 86), (61, 70, 94), (91, 82, 101),
    (121, 94, 104), (154, 111, 105), (193, 137, 101), (232, 177, 112),
    (255, 216, 150), (46, 30, 62), (25, 53, 69), (18, 76, 76),
]

# Pens 0..15 remain the foreground/sprite colours. The scenery occupies
# pens 16..31 in one reliable, hardware-scrollable 5-plane AGA world.
WORLD_PALETTE = FG_PALETTE + BG_PALETTE

# The production renderer uses an AGA dual playfield: three planes for the
# player/platform layer and three planes for the independently scrolling rear
# scenery.  These are the exact eight-colour banks programmed by its Copper.
FRONT8 = [
    (0, 0, 17), (17, 17, 17), (221, 68, 17), (255, 153, 34),
    (255, 238, 170), (34, 102, 204), (51, 204, 238), (153, 68, 204),
]
# Production 4+3 migration palette. Pens 0..7 retain the accepted gameplay
# mapping byte-for-byte; pens 8..15 add the proven rb18 steel/violet range.
# Existing art therefore remains visually stable while four-plane Bobs and
# world buffers can be exercised before any animation or gameplay art change.
FRONT16 = FRONT8 + [
    (55, 55, 65), (101, 98, 103), (163, 157, 158), (229, 225, 219),
    (67, 29, 100), (112, 45, 157), (166, 77, 218), (224, 35, 104),
]
CLEARING_FRONT16 = [
    FRONT16[0], FRONT16[1], (35, 79, 48), (146, 99, 43),
    FRONT16[4], FRONT16[5], FRONT16[6], (88, 142, 69),
    FRONT16[8], FRONT16[9], FRONT16[10], FRONT16[11],
    FRONT16[12], FRONT16[13], FRONT16[14], (126, 173, 82),
]
REAR8 = [
    (0, 0, 17), (0, 17, 51), (17, 34, 85), (34, 68, 119),
    (68, 68, 153), (102, 85, 170), (153, 119, 187), (204, 187, 221),
]

# Three Copper-switched palettes retain the 3-plane rear bitmap while giving
# the fixed-height sky, mountain and forest bands their own material range.
# Index zero stays the same near-black in every band to keep blanking and the
# transparent front-playfield relationship stable.
REAR8_BANDS = [
    [(0, 0, 1), (0, 0, 4), (1, 1, 6), (2, 1, 8),
     (4, 2, 9), (6, 4, 11), (9, 6, 12), (13, 11, 14)],
    [(0, 0, 1), (1, 1, 4), (2, 10, 13), (3, 3, 7),
     (4, 3, 9), (6, 5, 10), (9, 7, 12), (12, 10, 13)],
    [(0, 0, 1), (0, 1, 2), (0, 2, 3), (1, 3, 4),
     (1, 4, 4), (2, 5, 5), (4, 6, 6), (6, 8, 9)],
]
REAR8_MORPHS = [(64 + step * 4, 0, 1, step, 4)
                for step in range(1, 5)] + [
    (136 + step * 3, 1, 2, step, 8) for step in range(1, 9)
]


def rear_palette_for_y(y: int) -> list[tuple[int, int, int]]:
    anchor = REAR8_BANDS[0]
    for at, source, target, step, steps in REAR8_MORPHS:
        if y < at:
            break
        anchor = [tuple(((REAR8_BANDS[source][pen][channel] * (steps-step) +
                          REAR8_BANDS[target][pen][channel] * step) // steps) * 17
                        for channel in range(3)) for pen in range(8)]
    if anchor is REAR8_BANDS[0]:
        return [tuple(channel * 17 for channel in rgb) for rgb in anchor]
    return anchor

TITLE_SOURCE = ROOT / "assets" / "concept" / "sparkpaw-title-concept-aga64-preview.png"
TITLE_RUNTIME_PREVIEW = (ROOT / "assets" / "concept" /
                         "sparkpaw-title-aga64-runtime-preview.png")
LEVEL_LOADING_SOURCE = (ROOT / "assets" / "concept" /
                        "sparkpaw-level-loading-concept-v3.png")
LEVEL_LOADING_PREVIEW = (ROOT / "assets" / "concept" /
                         "sparkpaw-level-loading-aga64-preview.png")
LEVEL_CHARGING_SOURCE = (ROOT / "assets" / "concept" /
                         "sparkpaw-level-charging-concept-v2.png")
PARALLAX_MASTER = (ROOT / "assets" / "concept" /
                   "sparkpaw-parallax-master-concept-v4.png")
FOREGROUND_KIT = (ROOT / "assets" / "concept" /
                  "sparkpaw-foreground-kit-concept-v2.png")
WAYSTATION_SOURCE = (ROOT / "assets" / "concept" /
                     "sparkpaw-stormkeeper-waystation-source-v2.png")
CORE_SOURCE = (ROOT / "assets" / "concept" /
               "sparkpaw-stormstone-core-six-frame-source-v1.png")
LEVEL_CHARGING_PREVIEW = (ROOT / "assets" / "concept" /
                          "sparkpaw-level-charging-aga64-preview.png")
HUD_SOURCE = ROOT / "assets" / "concept" / "sparkpaw-hud-concept-v1.png"
HUD_RUNTIME_PREVIEW = ROOT / "assets" / "concept" / "sparkpaw-hud-aga8-preview.png"
DIAMOND_RUNTIME_PREVIEW = (ROOT / "assets" / "concept" /
                           "sparkpaw-diamond-aga8-preview.png")
HUD_HEIGHT = 48
HUD_LIVES = 9
HUD_PREVIEW_LIVES = 3
HUD_X_OFFSET = 2
HUD_SEPARATOR_H = 2
HUD_DIAMOND_STATES = 50
WATER_GAPS = ((99,104),(152,157))
DRY_GAPS = ((132,136),(174,179))
GROUND_CAP_TOP = 200
HUD_PALETTE = [
    (0, 0, 0), (5, 16, 25), (28, 37, 43), (102, 98, 88),
    (239, 218, 164), (222, 47, 45), (239, 151, 39), (31, 201, 224),
]


def nearest_index(rgb: tuple[int, int, int], palette: list[tuple[int, int, int]], *, avoid_zero=False) -> int:
    begin = 1 if avoid_zero else 0
    return min(range(begin, len(palette)), key=lambda i: sum((rgb[j] - palette[i][j]) ** 2 for j in range(3)))


def indexed_image(size: tuple[int, int], palette: list[tuple[int, int, int]], fill: int = 0) -> Image.Image:
    image = Image.new("P", size, fill)
    flat = [v for rgb in palette for v in rgb] + [0] * (768 - len(palette) * 3)
    image.putpalette(flat)
    return image


def planar_bytes(image: Image.Image, depth: int) -> tuple[int, bytes]:
    width, height = image.size
    row_bytes = ((width + 15) // 16) * 2
    pix = image.load()
    out = bytearray(row_bytes * height * depth)
    plane_size = row_bytes * height
    for y in range(height):
        for x in range(width):
            value = int(pix[x, y])
            mask = 0x80 >> (x & 7)
            byte = y * row_bytes + (x >> 3)
            for plane in range(depth):
                if value & (1 << plane):
                    out[plane * plane_size + byte] |= mask
    return row_bytes, bytes(out)


def bitmap_mask(image: Image.Image) -> bytes:
    row_bytes = ((image.width + 15) // 16) * 2
    out = bytearray(row_bytes * image.height)
    pix = image.load()
    for y in range(image.height):
        for x in range(image.width):
            if int(pix[x, y]) != 0:
                out[y * row_bytes + (x >> 3)] |= 0x80 >> (x & 7)
    return bytes(out)


def alpha_image(image: Image.Image) -> Image.Image:
    return Image.frombytes("L", image.size,
                           bytes(0 if p == 0 else 255 for p in image.getdata()))


def keep_main_component(image: Image.Image) -> Image.Image:
    """Remove disconnected AI specks that would corrupt a pose's baseline.

    Several authored crouch/landing cells contain convincing cat artwork plus
    isolated fragments roughly one hundred pixels below it.  Cropping to the
    complete alpha bounds made those fragments the feet and lifted the actual
    cat into the air.  Movement poses are single silhouettes, so retaining
    their largest 8-connected component is deterministic and lossless.
    """
    alpha = image.getchannel("A")
    pixels = alpha.load()
    seen: set[tuple[int, int]] = set()
    largest: list[tuple[int, int]] = []
    for y in range(image.height):
        for x in range(image.width):
            if pixels[x, y] < 96 or (x, y) in seen:
                continue
            component: list[tuple[int, int]] = []
            stack = [(x, y)]
            seen.add((x, y))
            while stack:
                px, py = stack.pop()
                component.append((px, py))
                for nx in range(px - 1, px + 2):
                    for ny in range(py - 1, py + 2):
                        if (0 <= nx < image.width and 0 <= ny < image.height and
                                (nx, ny) not in seen and pixels[nx, ny] >= 96):
                            seen.add((nx, ny))
                            stack.append((nx, ny))
            if len(component) > len(largest):
                largest = component
    cleaned = Image.new("RGBA", image.size, (0, 0, 0, 0))
    source, target = image.load(), cleaned.load()
    for x, y in largest:
        target[x, y] = source[x, y]
    return cleaned


def remap(image: Image.Image, source_palette: list[tuple[int, int, int]],
          target_palette: list[tuple[int, int, int]], *, transparent_zero=False) -> Image.Image:
    out = indexed_image(image.size, target_palette, 0)
    src, dst = image.load(), out.load()
    for y in range(image.height):
        for x in range(image.width):
            pen = int(src[x, y])
            if transparent_zero and pen == 0:
                dst[x, y] = 0
            else:
                dst[x, y] = nearest_index(source_palette[pen], target_palette,
                                          avoid_zero=transparent_zero)
    return out


def save_spbm(path: Path, image: Image.Image, palette: list[tuple[int, int, int]], depth: int = 4, mask: bytes = b"") -> None:
    row_bytes, bits = planar_bytes(image, depth)
    header = b"SPBM" + struct.pack(">HHBBH", image.width, image.height, depth, 1 if mask else 0, row_bytes)
    pal = bytes(v for rgb in palette[:1 << depth] for v in rgb)
    path.write_bytes(header + pal + bits + mask)


def load_aga_screen(path: Path) -> tuple[Image.Image, list[tuple[int, int, int]]]:
    with Image.open(path) as source:
        if source.size != (320, 256) or source.mode != "P":
            raise ValueError(f"AGA screen must be a 320x256 indexed image: {path}")
        image = source.copy()
        palette_data = source.getpalette()
    if palette_data is None or image.getextrema()[1] >= 64:
        raise ValueError(f"AGA screen must use palette indices 0..63: {path}")
    palette = [tuple(palette_data[index:index + 3])
               for index in range(0, 64 * 3, 3)]
    return image, palette


def make_hud() -> tuple[Image.Image, Image.Image, Image.Image, Image.Image]:
    """Build one static HUD plus compact dynamic-panel patch atlases."""
    with Image.open(HUD_SOURCE) as source:
        rgb = source.convert("RGB").crop((14, 238, 1969, 550))
    # Keep the complete authored frame, but reserve an opaque dark two-line
    # separation above it. HUD pen 0 is transparent to hardware sprites, so
    # the separator must use a non-zero HUD pen across the full fetched width.
    rgb = rgb.resize((320, HUD_HEIGHT-HUD_SEPARATOR_H),
                     Image.Resampling.LANCZOS)
    base = indexed_image((336, HUD_HEIGHT), HUD_PALETTE, 0)
    src, dst = rgb.load(), base.load()
    for y in range(HUD_SEPARATOR_H):
        for x in range(base.width):
            dst[x, y] = 1
    for y in range(HUD_HEIGHT-HUD_SEPARATOR_H):
        for x in range(320):
            dst[x + HUD_X_OFFSET, y + HUD_SEPARATOR_H] = nearest_index(
                src[x, y], HUD_PALETTE)

    heart_rows = (0x36, 0x7f, 0x7f, 0x3e, 0x1c, 0x08)
    def shape(x: int, y: int) -> bool:
        return 0 <= x < 7 and 0 <= y < 6 and bool(heart_rows[y] & (0x40 >> x))

    digits = {
        0:(0x7,0x5,0x5,0x5,0x7), 1:(0x2,0x6,0x2,0x2,0x7),
        2:(0x6,0x1,0x2,0x4,0x7), 3:(0x6,0x1,0x2,0x1,0x6),
        4:(0x5,0x5,0x7,0x1,0x1), 5:(0x7,0x4,0x6,0x1,0x6),
        6:(0x3,0x4,0x7,0x5,0x7), 7:(0x7,0x1,0x2,0x2,0x2),
        8:(0x7,0x5,0x7,0x5,0x7), 9:(0x7,0x5,0x7,0x1,0x6),
    }
    health_box = (48, 12, 128, 44)
    lives_box = (160, 12, 192, 36)
    diamonds_box = (224, 12, 256, 36)
    draw = ImageDraw.Draw(base)
    draw.rectangle((49 + HUD_X_OFFSET, 11 + HUD_SEPARATOR_H,
                    125 + HUD_X_OFFSET, 40 + HUD_SEPARATOR_H), fill=1)
    # Only erase the generated source digit.  Wider rectangles damage the
    # life panel's bevelled upper-right and lower border.
    draw.rectangle((168 + HUD_X_OFFSET, 14 + HUD_SEPARATOR_H,
                    180 + HUD_X_OFFSET, 33 + HUD_SEPARATOR_H), fill=1)

    def draw_lives(frame: Image.Image, lives: int) -> None:
        draw = ImageDraw.Draw(frame)
        for y,row in enumerate(digits[lives]):
            for x in range(3):
                if row&(0x4>>x):
                    draw.rectangle((171 + HUD_X_OFFSET + x*2,
                                    14 + HUD_SEPARATOR_H + y*3,
                                    172 + HUD_X_OFFSET + x*2,
                                    16 + HUD_SEPARATOR_H + y*3),fill=4)

    def draw_health(frame: Image.Image, health: int) -> None:
        draw = ImageDraw.Draw(frame)
        for heart in range(3):
            units = health - heart * 2
            for y in range(6):
                for x in range(7):
                    if not shape(x, y):
                        continue
                    border = (not shape(x - 1, y) or not shape(x + 1, y) or
                              not shape(x, y - 1) or not shape(x, y + 1))
                    if border:
                        pen = 2 if y >= 3 or x >= 5 else 4
                    elif units >= 2 or (units == 1 and x <= 3):
                        pen = 5
                    else:
                        pen = 1
                    left = 52 + HUD_X_OFFSET + heart * 25 + x * 3
                    top = 15 + HUD_SEPARATOR_H + y * 3
                    draw.rectangle((left, top, left + 2, top + 2), fill=pen)

    def draw_diamonds(frame: Image.Image, count: int) -> None:
        draw = ImageDraw.Draw(frame)
        for column,value in enumerate((count//10,count%10)):
            for y,row in enumerate(digits[value]):
                for x in range(3):
                    if row&(0x4>>x):
                        left=228+column*10+x*2
                        top=14+HUD_SEPARATOR_H+y*3
                        draw.rectangle((left,top,left+1,top+2),fill=4)

    health_atlas = indexed_image((health_box[2]-health_box[0],
                                  (health_box[3]-health_box[1])*7),
                                 HUD_PALETTE, 0)
    for health in range(7):
        frame = base.copy()
        draw_health(frame, health)
        patch = frame.crop(health_box)
        health_atlas.paste(patch, (0, health*(health_box[3]-health_box[1])))

    lives_atlas = indexed_image((lives_box[2]-lives_box[0],
                                 (lives_box[3]-lives_box[1])*HUD_LIVES),
                                HUD_PALETTE, 0)
    for lives in range(1,HUD_LIVES+1):
        frame = base.copy()
        draw_lives(frame, lives)
        patch = frame.crop(lives_box)
        lives_atlas.paste(patch, (0, (lives-1)*(lives_box[3]-lives_box[1])))

    diamonds_atlas = indexed_image(
        (diamonds_box[2]-diamonds_box[0],
         (diamonds_box[3]-diamonds_box[1])*HUD_DIAMOND_STATES),
        HUD_PALETTE, 0)
    for count in range(HUD_DIAMOND_STATES):
        frame = base.copy()
        draw_diamonds(frame, count)
        patch = frame.crop(diamonds_box)
        diamonds_atlas.paste(
            patch, (0, count*(diamonds_box[3]-diamonds_box[1])))

    preview = base.copy()
    draw_health(preview, 6)
    draw_lives(preview, HUD_PREVIEW_LIVES)
    draw_diamonds(preview, 0)
    preview.crop((0, 0, 320, HUD_HEIGHT)).save(HUD_RUNTIME_PREVIEW)
    return base, health_atlas, lives_atlas, diamonds_atlas


def make_collectible_diamond() -> tuple[Image.Image, bytes]:
    """Copy the exact 16x21 HUD diamond pixels into the gameplay bank."""
    hud = Image.open(HUD_RUNTIME_PREVIEW)
    # This is the connected diamond component in the generated HUD preview.
    source = hud.crop((207,14,223,35))
    image=indexed_image((32,21),FRONT8,0)
    src,dst=source.load(),image.load()
    # HUD 0/2/4/7 = black/shadow/cream/cyan. Map them directly to the
    # corresponding gameplay-bank roles; no scaling or geometry redraw.
    mapping={0:0,2:1,4:4,7:6}
    for y in range(21):
        for x in range(16):
            dst[x,y]=mapping.get(int(src[x,y]),0)
    image.crop((0,0,16,21)).save(DIAMOND_RUNTIME_PREVIEW)
    return image,bitmap_mask(image)


def reserve_black_pen_zero(
        image: Image.Image,
        palette: list[tuple[int, int, int]],
        ) -> tuple[Image.Image, list[tuple[int, int, int]]]:
    """Make the hardware border pen black with the smallest indexed change.

    Some scandoublers expose a one-pixel COLOR00 border that a CRT overscans.
    If the image already has pure black, swapping that entry with pen zero is
    RGB-lossless. Otherwise merge the least-used nonzero pen into its nearest
    neighbour and use the vacated entry to preserve the old pen-zero colour.
    """
    palette = list(palette)
    mapping = list(range(256))
    try:
        black_pen = palette.index((0, 0, 0))
    except ValueError:
        black_pen = -1
    if black_pen >= 0:
        if black_pen != 0:
            mapping[0], mapping[black_pen] = black_pen, 0
            palette[0], palette[black_pen] = palette[black_pen], palette[0]
    else:
        histogram = image.histogram()[:64]
        drop = min(range(1, 64), key=lambda pen: histogram[pen])
        replacement = min(
            (pen for pen in range(1, 64) if pen != drop),
            key=lambda pen: sum((palette[drop][c]-palette[pen][c])**2
                                for c in range(3)),
        )
        mapping[drop] = replacement
        mapping[0] = drop
        palette[drop] = palette[0]
        palette[0] = (0, 0, 0)
    remapped = image.point(mapping)
    remapped.putpalette([v for rgb in palette for v in rgb] + [0]*(768-192))
    return remapped,palette


def fit_screen_source(path: Path) -> Image.Image:
    with Image.open(path) as source:
        return ImageOps.fit(source.convert("RGB"),(320,256),
                            Image.Resampling.LANCZOS)


def make_level_loading() -> tuple[Image.Image, Image.Image,
                                  list[tuple[int, int, int]]]:
    fitted = fit_screen_source(LEVEL_LOADING_SOURCE)
    image = fitted.quantize(colors=64,method=Image.Quantize.FASTOCTREE,
                            dither=Image.Dither.NONE)
    palette_data = image.getpalette()
    if palette_data is None:
        raise ValueError("loading screen conversion produced no palette")
    palette = [tuple(palette_data[index:index + 3])
               for index in range(0,64*3,3)]
    image,palette = reserve_black_pen_zero(image,palette)
    image.save(LEVEL_LOADING_PREVIEW)
    charging_rgb = fit_screen_source(LEVEL_CHARGING_SOURCE)
    charging = indexed_image(charging_rgb.size,palette)
    source_pixels=charging_rgb.load()
    charging_pixels=charging.load()
    for y in range(charging.height):
        for x in range(charging.width):
            charging_pixels[x,y]=nearest_index(source_pixels[x,y],palette)
    charging.save(LEVEL_CHARGING_PREVIEW)
    return image,charging,palette


def make_background() -> Image.Image:
    image = indexed_image((PARALLAX_W, WORLD_H), REAR8, 2)
    d = ImageDraw.Draw(image)
    # Saturated storm bands and broken clouds, all inside the fixed REAR8 bank.
    d.rectangle((0, 0, PARALLAX_W - 1, 31), fill=2)
    d.rectangle((0, 32, PARALLAX_W - 1, 69), fill=3)
    d.rectangle((0, 70, PARALLAX_W - 1, 112), fill=2)
    for x in range(-24, PARALLAX_W, 92):
        d.polygon([(x, 42), (x + 22, 31), (x + 54, 35),
                   (x + 76, 27), (x + 104, 44)], fill=4)
        d.line((x + 12, 47, x + 78, 47), fill=5)

    # Two mountain depths with deliberate highlight facets instead of flat
    # greybox triangles. Shapes overlap the 640px repeat at both edges.
    for base, peak in ((-120, 58), (70, 22), (285, 46), (480, 18)):
        d.polygon([(base, 175), (base + 95, peak), (base + 210, 175)], fill=4)
        d.polygon([(base + 95, peak), (base + 65, 92),
                   (base + 98, 74), (base + 120, 114)], fill=6)
        d.polygon([(base + 95, peak), (base + 120, 114),
                   (base + 180, 175), (base + 124, 153)], fill=4)
        d.line((base + 95, peak + 3, base + 113, 63), fill=5, width=2)
    for base in (-20, 190, 400, 610):
        d.polygon([(base, 188), (base + 70, 88), (base + 152, 188)], fill=3)
        d.polygon([(base + 70, 88), (base + 88, 136),
                   (base + 127, 188), (base + 94, 165)], fill=2)

    # Distant broken storm towers and waterfalls establish the concept's
    # vertical landmarks without stealing foreground contrast.
    for x, top, width in ((38, 65, 26), (276, 78, 22), (520, 55, 30)):
        d.rectangle((x, top, x + width, 179), fill=3)
        d.polygon([(x - 4, top), (x + width // 2, top - 24),
                   (x + width + 3, top)], fill=4)
        d.line((x + width // 2, top + 5, x + width // 2, 171), fill=6, width=2)
        for y in range(top + 18, 166, 25):
            d.line((x + 4, y, x + width - 4, y - 5), fill=5)
    for x, top in ((148, 105), (422, 92)):
        d.line((x, top, x, 190), fill=6, width=3)
        d.line((x + 4, top + 8, x + 4, 190), fill=7, width=1)

    # One broad lightning fork animates only through scrolling; no palette
    # cycling or Copper changes are introduced.
    d.line((592, 18, 579, 42, 590, 40, 568, 70), fill=6, width=1)
    return image


def make_authored_parallax() -> Image.Image:
    """Reduce the approved wide parallax master directly into exact REAR8."""
    source = Image.open(PARALLAX_MASTER).convert("RGB")
    # V3 was authored as one complete sky/mountain/ruin/forest panorama. Keep
    # its full vertical composition rather than applying v2's top-biased crop.
    source = source.resize((PARALLAX_ACCEPTED_W, 208), Image.Resampling.LANCZOS)
    image = indexed_image((PARALLAX_W, WORLD_H), rear_palette_for_y(0), 1)
    src, dst = source.load(), image.load()
    for y in range(208):
        palette = rear_palette_for_y(y)
        for x in range(PARALLAX_W):
            # Existing cameras retain their exact accepted 0..1023 source.
            # The final 96px mirror the quiet forest tail and sit mostly behind
            # the new foreground landmark; no earlier landmark repeat moves.
            source_x=x if x<PARALLAX_ACCEPTED_W else 2047-x
            r, g, b = src[source_x, y]
            # The low forest is colour-graded toward the concept's cold green
            # stone/trees before exact palette reduction. Geometry and pixels
            # still come from the approved authored master.
            if y >= 145:
                r = (r * 3) // 5
                g = min(255, (g * 6) // 5 + 8)
                b = (b * 4) // 5
            dst[x, y] = nearest_index((r, g, b), palette)
    return image


def rear_rgb(image: Image.Image) -> Image.Image:
    """Render band-indexed rear pixels as the Copper will display them."""
    result = Image.new("RGB", image.size)
    src, dst = image.load(), result.load()
    for y in range(image.height):
        source_y = y % WORLD_H
        palette = rear_palette_for_y(source_y)
        for x in range(image.width):
            dst[x, y] = palette[int(src[x, y])]
    return result


def make_midground() -> Image.Image:
    image = indexed_image((PARALLAX_W, WORLD_H), REAR8, 0)
    d = ImageDraw.Draw(image)
    # Three forest silhouettes use stepped branch clusters rather than a row of
    # identical triangles. Their values stay below gameplay silhouettes.
    for depth, base_y, step, pen in ((0, 211, 29, 3), (1, 222, 25, 2),
                                     (2, 230, 21, 1)):
        for x in range(-18 + depth * 7, PARALLAX_W + 24, step):
            h = 31 + ((x * 7 + depth * 19) % (42 - depth * 7))
            cx = x + step // 2
            d.rectangle((cx - 1, base_y - h, cx + 1, base_y), fill=pen)
            for branch in range(6, h - 3, 7):
                half = max(3, (h - branch) // 5)
                y = base_y - branch
                d.polygon([(cx, y - 8), (cx - half, y + 3),
                           (cx + half, y + 3)], fill=pen)

    # Near parallax ruins and arches echo the concept's carved technology.
    for x, top, width in ((84, 118, 42), (326, 99, 50), (552, 126, 38)):
        d.rectangle((x, top, x + width, 218), fill=1)
        d.rectangle((x + 5, top + 5, x + width - 5, 218), fill=2)
        d.line((x + 4, top + 3, x + width - 4, top + 3), fill=5, width=2)
        d.polygon([(x - 4, top), (x + width // 2, top - 20),
                   (x + width + 4, top)], fill=3)
        d.line((x + width // 2, top + 10, x + width // 2, 205), fill=7, width=2)
        d.rectangle((x + width // 2 - 4, top + 42,
                     x + width // 2 + 4, top + 54), fill=0)
    for x in (8, 238, 456):
        d.line((x, 139, x, 218), fill=1, width=6)
        d.line((x + 76, 139, x + 76, 218), fill=1, width=6)
        d.arc((x, 104, x + 76, 171), 180, 360, fill=3, width=5)
        d.arc((x + 7, 112, x + 69, 168), 180, 360, fill=5, width=2)
    return image


def make_foreground() -> tuple[Image.Image, bytearray]:
    image = indexed_image((WORLD_W, WORLD_H), FRONT16, 0)
    d = ImageDraw.Draw(image)
    kit = Image.open(FOREGROUND_KIT).convert("RGBA")
    platform_parts = (
        (105,44,218,98), (284,44,500,102), (565,44,1231,128),
        (104,130,864,208), (103,218,1011,284),
        (104,305,1279,376), (97,395,1444,468),
    )
    support_parts = (
        (123,501,419,610), (478,501,678,608),
        (738,501,1086,608), (1139,501,1402,599),
    )
    column_parts = (
        (416,648,551,838), (616,648,741,838),
        (807,648,932,836), (992,650,1113,837),
    )
    cols, rows = WORLD_W // TILE, 14
    collision = bytearray(cols * rows)

    def kit_piece(crop: tuple[int,int,int,int], box: tuple[int,int,int,int]) -> None:
        x0,y0,x1,y1=box
        part=kit.crop(crop).resize((x1-x0+1,y1-y0+1),Image.Resampling.LANCZOS)
        pixels=part.load()
        for py in range(part.height):
            for px in range(part.width):
                r,g,b,a=pixels[px,py]
                if a>=80:
                    r=min(255,r+20); g=min(255,g+20); b=min(255,b+24)
                    if g>r+16 and b>r+24:
                        pen=6 if g+b>250 else 5
                    else:
                        pen=nearest_index((r,g,b),FRONT16,avoid_zero=True)
                    image.putpixel((x0+px,y0+py),pen)

    def block(tx: int, ty: int, tw: int, th: int, style: int = 0) -> None:
        for yy in range(ty, min(ty + th, rows)):
            for xx in range(max(tx, 0), min(tx + tw, cols)):
                collision[yy * cols + xx] = 1
        x0, y0, x1, y1 = tx * TILE, ty * TILE, (tx + tw) * TILE - 1, (ty + th) * TILE - 1
        base = (8, 9, 8, 9)[style & 3]
        edge = (10, 10, 11, 10)[style & 3]
        # Layered Storm Ruins construction: a pale load-bearing lip, irregular
        # steel slabs and recessed violet/cyan machinery. This stays inside the
        # established FRONT16 bank and never changes the collision rectangle.
        d.rectangle((x0, y0, x1, y1), fill=1)
        d.rectangle((x0 + 1, y0 + 2, x1 - 1, y1 - 1), fill=base)
        d.line((x0 + 1, y0, x1 - 1, y0), fill=11)
        d.line((x0 + 2, y0 + 1, x1 - 2, y0 + 1), fill=edge)
        d.line((x0 + 2, y1 - 1, x1 - 2, y1 - 1), fill=1)
        if th>1:
            crop=column_parts[(tx+style)%len(column_parts)]
            art_y1=y1
        elif tw>=6 and y0<GROUND_CAP_TOP:
            crop=support_parts[(tx//8+style)%len(support_parts)]
            # Alternate shallow lips, open braces and deeper machinery masses.
            # These are decorative silhouettes below the unchanged solid slab.
            art_y1=min(GROUND_CAP_TOP-1,y0+(22,38,30,46)[style&3])
        else:
            crop=platform_parts[(tx+tw+style*3)%len(platform_parts)]
            art_y1=y1
        kit_piece(crop,(x0,y0,x1,art_y1))
        # Reassert a continuous collision-readable top lip after indexed
        # reduction; all other pixels come from the newly generated kit.
        d.line((x0,y0,x1,y0),fill=11)
        d.line((x0+1,y0+1,x1-1,y0+1),fill=edge)

    def waystation() -> None:
        """Reduce the landmark into the clearing-specific FRONT16 roles."""
        source=Image.open(WAYSTATION_SOURCE).convert("RGB")
        pixels=source.load()
        # ImageGen's checkerboard is baked RGB rather than alpha. Treat only
        # bright near-neutral pixels as background; the landmark itself is
        # intentionally dark/cyan and remains well outside this key.
        mask=Image.new("L",source.size,0)
        mask_pixels=mask.load()
        for py in range(source.height):
            for px in range(source.width):
                r,g,b=pixels[px,py]
                if not (min(r,g,b)>=210 and max(r,g,b)-min(r,g,b)<=14):
                    mask_pixels[px,py]=255
        bbox=mask.getbbox()
        if not bbox:
            raise ValueError("Stormkeeper waystation source has no landmark")
        landmark=source.crop(bbox).convert("RGBA")
        landmark.putalpha(mask.crop(bbox))
        landmark.thumbnail((200,145),Image.Resampling.LANCZOS)
        src=landmark.load()
        x0,y0=3132+(200-landmark.width)//2,55+(145-landmark.height)
        for py in range(landmark.height):
            for px in range(landmark.width):
                r,g,b,a=src[px,py]
                if a>=96:
                    image.putpixel((x0+px,y0+py),
                                   nearest_index((r,g,b),CLEARING_FRONT16,
                                                 avoid_zero=True))

    # Continuous collision floor and varied but readable Phase 6B greybox over
    # eight screens. The visible 6B.3 cap is applied after the legacy eight-
    # colour remap so it can use the existing fourth-plane steel/violet pens.
    block(0, 13, cols, 1, 0)
    for gap_left,gap_right in WATER_GAPS+DRY_GAPS:
        for tx in range(gap_left,gap_right):
            collision[13 * cols + tx] = 0
        d.rectangle((gap_left*TILE,13*TILE,gap_right*TILE-1,WORLD_H-1),fill=0)
    block(8, 10, 7, 1, 2)
    block(20, 8, 6, 1, 0)
    block(31, 11, 8, 1, 3)
    block(43, 9, 5, 1, 1)
    block(53, 7, 9, 1, 2)
    block(67, 10, 8, 1, 0)
    block(82, 10, 7, 1, 1)
    block(92, 8, 6, 1, 2)
    block(104, 11, 7, 1, 3)
    block(115, 9, 6, 1, 0)
    block(123, 7, 4, 1, 2)
    # Phase 6C extends the route to twelve screens. These are authored beats,
    # not a repeated four-screen tail: dry chasm, high approach, second water,
    # long patrol court, bidirectional Strider chasm and final portal ascent.
    block(128, 9, 4, 1, 1)
    block(136,11, 7, 1, 3)
    # Alpha.28 lowers the previously unreachable 64px water approach. The
    # complete slab remains raised, but only 16px above its y=176 approach.
    block(145,10, 6, 1, 0)
    block(157, 9, 8, 1, 2)
    block(166,11, 8, 1, 1)
    # Keep the later portal architecturally raised but conservatively reachable.
    # Alpha.28 rolls back alpha.27's unnecessary flattening after evidence showed
    # that the reported blocker was the earlier x=2320 water approach.
    block(179,11, 3, 1, 1)
    block(183,10, 3, 1, 3)
    block(188,10, 4, 1, 0)
    # Phase 6C.2 adds one safe reward field. The old raised portal deck ends at
    # x=3072; Sparkpaw drops onto the ordinary y=200 floor and approaches the
    # landmark without a new platform, hazard or enemy surface.
    waystation()
    # Short columns establish occlusion and test collision at camera seams.
    block(16, 11, 2, 2, 1)
    block(40, 10, 2, 3, 0)
    block(63, 11, 2, 2, 3)
    block(80, 11, 2, 2, 2)
    block(112, 11, 2, 2, 3)
    # Attached piers turn several slabs into the authored L/portal silhouettes
    # from the approved concept references instead of floating shelf repeats.
    block(144, 10, 2, 3, 1)
    block(165,  9, 2, 4, 0)
    block(186, 10, 2, 3, 2)
    block(190, 10, 2, 3, 3)
    return image, collision


def make_stormstone_core() -> tuple[Image.Image, bytes]:
    """Build six stable idle and twelve radial Core-release frames."""
    source=Image.open(CORE_SOURCE).convert("RGB")
    cell_w=source.width//6
    sheet=indexed_image((64,48*CORE_RUNTIME_FRAMES),FRONT16,0)
    for frame in range(6):
        cell=source.crop((frame*cell_w,0,(frame+1)*cell_w,source.height))
        alpha=Image.new("L",cell.size,0)
        cp,ap=cell.load(),alpha.load()
        for y in range(cell.height):
            for x in range(cell.width):
                r,g,b=cp[x,y]
                if not (r>150 and b>100 and g<105 and r>g*1.7):
                    ap[x,y]=255
        bbox=alpha.getbbox()
        if not bbox:
            raise ValueError(f"Stormstone Core frame {frame} is empty")
        art=cell.crop(bbox).convert("RGBA")
        art.putalpha(alpha.crop(bbox))
        art.thumbnail((60,44),Image.Resampling.LANCZOS)
        x0=(64-art.width)//2
        y0=frame*48+(48-art.height)//2
        pixels=art.load()
        for y in range(art.height):
            for x in range(art.width):
                r,g,b,a=pixels[x,y]
                if a>=96:
                    sheet.putpixel((x0+x,y0+y),
                                   nearest_index((r,g,b),FRONT16,
                                                 avoid_zero=True))
    # ImageGen's six cells vary slightly in mass. Runtime idle deliberately
    # keeps frame zero's silhouette/mask exact and moves only the inner light.
    base=sheet.crop((0,0,64,48))
    vein_pens=(5,7,6,4,6,5)
    for frame,vein_pen in enumerate(vein_pens):
        stable=base.copy()
        glow=ImageDraw.Draw(stable)
        # Preserve every outline/mass pixel; only a narrow lightning vein and
        # two intermittent motes change. This reads as energy travelling
        # inside a stable crystal instead of whole-object heartbeat flashing.
        glow.line(((33,7),(30,17),(34,25),(29,36)),fill=vein_pen,width=1)
        if frame in (2,3): glow.point((20,19),fill=6)
        if frame in (3,4): glow.point((44,29),fill=4 if frame==3 else 6)
        sheet.paste(stable,(0,frame*48))

    # Pickup frames are a self-contained level-end signature. The Core first
    # compresses into a white-hot centre, then throws chunky asymmetrical rays
    # and fragments in every direction. Keeping the burst around the shrine
    # avoids relying on a Bob endpoint that the higher-priority player sprite
    # would cover.
    rays=((0.00,30),(0.52,35),(1.08,27),(1.57,22),(2.12,31),
          (2.65,38),(3.14,32),(3.70,37),(4.18,29),(4.71,23),
          (5.20,31),(5.78,36))
    for stage in range(12):
        cell=indexed_image((64,48),FRONT16,0)
        draw=ImageDraw.Draw(cell)
        if stage<5:
            if stage<2:
                scaled=base.copy().point(
                    lambda pen: 3 if pen in (4,6,9,11) else pen)
            else:
                size=(48,34,20)[stage-2]
                scaled=base.resize((size,max(12,int(48*size/64))),
                                   Image.Resampling.NEAREST)
            cell.paste(scaled,((64-scaled.width)//2,(48-scaled.height)//2))
        if 1<=stage<=8:
            travel=stage-1
            inner=2+travel*3
            length=max(3,13-abs(4-travel)*2)
            for index,(angle,limit) in enumerate(rays):
                reach=min(limit,inner+length+(index&1)*2)
                start=max(2,reach-length)
                x1=32+int(math.cos(angle)*start)
                y1=24+int(math.sin(angle)*start*.70)
                x2=32+int(math.cos(angle)*reach)
                y2=24+int(math.sin(angle)*reach*.70)
                pen=(3,4,9,11,13)[(index+stage)%5]
                draw.line((x1,y1,x2,y2),fill=pen,
                          width=2 if 2<=stage<=5 and index%3==0 else 1)
        if 2<=stage<=10:
            distance=5+(stage-2)*3
            for index,angle in enumerate((.30,1.85,2.85,4.05,5.45)):
                x=32+int(math.cos(angle)*distance)
                y=24+int(math.sin(angle)*distance*.65)
                if 1<=x<63 and 1<=y<47:
                    draw.point((x,y),fill=(4,6,9,11,13)[index])
                    if stage in (4,5,6): draw.point((x+1,y),fill=3)
        if stage<=3:
            radius=(2,4,3,2)[stage]
            draw.ellipse((32-radius,24-radius,32+radius,24+radius),fill=3)
            draw.point((32,24),fill=4)
        sheet.paste(cell,(0,(6+stage)*48))
    return sheet,bitmap_mask(sheet)


def make_stormstone_core_triumph_raw() -> bytes:
    """Reproduce the selected 02 Storm Triumph Paula sample from source."""
    rate=11025
    sample_count=int(rate*1.15)

    def tone(freq: float,start: float,duration: float,volume: float) -> list[float]:
        values=[0.0]*sample_count
        begin=int(start*rate)
        length=int(duration*rate)
        for index in range(length):
            phase=math.sin(2*math.pi*freq*index/rate)
            env=min(1.0,index/(rate*.008))*max(0.0,1-index/length)**1.3
            values[begin+index]+=phase*env*volume
        return values

    tracks=(tone(392,.00,.22,.55),tone(523,.14,.24,.65),
            tone(659,.29,.27,.72),tone(784,.46,.52,.92),
            tone(1568,.48,.42,.3),tone(98,.00,.18,.3))
    mixed=[sum(values) for values in zip(*tracks)]
    peak=max(1.0,max(abs(value) for value in mixed))
    signed=[int(max(-127,min(127,value*118/peak))) for value in mixed]
    return bytes(value&255 for value in signed)


def apply_ground_water_treatment(image: Image.Image) -> None:
    """Apply approved 6B.3 art in the established four-plane front bank."""
    image.putpalette([v for rgb in FRONT16 for v in rgb]+[0]*(768-48))
    d=ImageDraw.Draw(image)
    # Thin concept-quality ruin profile directly above the HUD; never a tall
    # foundation. It shares the raised-platform slab language without changing
    # the accepted y=200 collision line.
    kit=Image.open(FOREGROUND_KIT).convert("RGBA")
    ground=kit.crop((84,896,1456,969)).resize((512,8),Image.Resampling.LANCZOS)
    if not hasattr(Image.Transpose,"FLIP_LEFT_RIGHT"):
        raise RuntimeError("Pillow transpose support is required")
    for section_x in range(0,WORLD_W,512):
        part=ground if ((section_x//512)&1)==0 else ground.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
        src=part.load()
        for py in range(8):
            for px in range(min(512,WORLD_W-section_x)):
                r,g,b,a=src[px,py]
                if a>=80:
                    if g>r+16 and b>r+24:
                        pen=6 if g+b>250 else 5
                    else:
                        pen=nearest_index((min(255,r+20),min(255,g+20),min(255,b+24)),
                                          FRONT16,avoid_zero=True)
                    image.putpixel((section_x+px,GROUND_CAP_TOP+py),pen)
                else:
                    image.putpixel((section_x+px,GROUND_CAP_TOP+py),1)
    d.line((0,GROUND_CAP_TOP,WORLD_W-1,GROUND_CAP_TOP),fill=11)
    d.line((0,GROUND_CAP_TOP+1,WORLD_W-1,GROUND_CAP_TOP+1),fill=10)
    d.line((0,207,WORLD_W-1,207),fill=1)
    # Dry chasms remain a different hazard from water. Eleven visible pixels is
    # all the fixed HUD boundary permits, so use broken bank faces, a recessed
    # bottom and severed conduit glints instead of pretending this is a deep
    # vertical vista. All pixels inside the opening remain non-solid.
    for gap_index,(gap_left,gap_right) in enumerate(DRY_GAPS):
        x0=gap_left*TILE; x1=gap_right*TILE-1
        d.rectangle((x0,197,x1,207),fill=0)
        d.polygon(((x0,197),(x0+11,197),(x0+8,201),(x0+5,207),(x0,207)),fill=1)
        d.polygon(((x1-11,197),(x1,197),(x1,207),(x1-5,207),(x1-8,202)),fill=1)
        d.line((x0,197,x0+9,197,x0+6,202),fill=10)
        d.line((x1-9,197,x1,197),fill=10)
        d.line((x0+2,199,x0+5,202,x0+4,206),fill=8)
        d.line((x1-3,199,x1-6,203,x1-5,206),fill=9)
        abyss_top=204+(gap_index&1)
        d.rectangle((x0+7,abyss_top,x1-7,207),fill=1)
        for tooth_x in range(x0+12+(gap_index*5),x1-8,17):
            d.polygon(((tooth_x,abyss_top),(tooth_x+4,abyss_top),
                       (tooth_x+2,abyss_top-2)),fill=1)
        conduit_y=202+(gap_index&1)
        d.line((x0+7,conduit_y,x0+14,conduit_y+3),fill=5)
        d.point((x0+8,conduit_y),fill=6)
        d.line((x1-14,conduit_y+2,x1-8,conduit_y),fill=5)
        d.point((x1-9,conduit_y),fill=6)
    for gap_left,gap_right in WATER_GAPS:
        water_left=gap_left*TILE
        for y in range(11):
            for x in range((gap_right-gap_left)*TILE):
                d.point((water_left+x,197+y),fill=water_animation_pen(0,x,y))


def water_animation_pen(frame: int,x: int,y: int) -> int:
    surface_curve=(1,1,0,0,0,1,1,2,2,2,1,1,0,0,1,1)
    bubble_x=(7,19,31,46,60,72)
    bubble_start=(0,11,4,15,7,13)
    bubble_life=(7,9,6,10,8,7)
    surface=1+surface_curve[((x>>1)+frame)&15]
    if x<3 or x>=77:
        surface=3
    if y<surface:
        return 0
    if y==surface:
        return 11 if ((((x>>2)+frame*3)&7)==0 or
                      ((x+frame*6)&31)==17) else 6
    if y==surface+1:
        return 5 if ((x+frame)&7) else 6
    for bubble in range(6):
        age=(frame+16-bubble_start[bubble])&15
        life=bubble_life[bubble]
        if age<life:
            bx=bubble_x[bubble]+(1 if ((frame+bubble)&3)==0 else 0)
            by=10-((age*7)//life)
            if by<=surface+1:
                continue
            if x==bx and y==by:
                return 11 if bubble&1 else 6
            if not (bubble&1) and x==bx+1 and y==by:
                return 6
    return 5


def make_water_animation_preview() -> Image.Image:
    """Preview the renderer's exact sixteen-frame 80x11 water pen formula."""
    sheet=indexed_image((80,11*16),FRONT16,0)
    pixels=sheet.load()
    for frame in range(16):
        for y in range(11):
            for x in range(80):
                pixels[x,frame*11+y]=water_animation_pen(frame,x,y)
    return sheet.resize((320,704),Image.Resampling.NEAREST)


def draw_beetle_frame(frame: int) -> Image.Image:
    """Draw the first enemy directly at its final AGA pixel resolution."""
    image = indexed_image((BEETLE_W, BEETLE_H), FRONT8, 0)
    d = ImageDraw.Draw(image)
    if frame == 8:
        for x, y, pen in ((2, 18, 7), (6, 8, 6), (11, 3, 4), (17, 20, 5),
                          (23, 5, 7), (29, 14, 6), (14, 12, 5), (26, 22, 4)):
            d.point((x, y), fill=pen)
            if pen in (6, 7):
                d.point((x + 1, y), fill=pen)
        d.line((9, 15, 12, 12), fill=1)
        d.point((10, 14), fill=4)
        d.line((20, 18, 23, 20), fill=1)
        d.point((21, 18), fill=7)
        return image

    if frame == 7:
        for x, y, pen in ((3, 17, 5), (7, 8, 7), (12, 19, 4), (17, 6, 5),
                          (22, 16, 7), (28, 9, 5)):
            d.rectangle((x, y, x + 2, y + 1), fill=1)
            d.point((x + 1, y), fill=pen)
        d.line((11, 12, 15, 8), fill=6)
        d.line((15, 8, 18, 13), fill=5)
        d.point((16, 10), fill=4)
        d.point((5, 16), fill=6)
        d.point((24, 12), fill=4)
        return image

    phase = frame if frame < 4 else 1
    # Keep the feet on the accepted row-22 ground line, but seat the normal
    # silhouette two pixels lower than the first beetle pass.  The shorter
    # antennae leave honest visual clearance below a standing plasma pulse;
    # collision and the 32x24 Bob origin remain deliberately unchanged.
    body_y = (6, 7, 6, 7)[phase]
    if frame == 4:
        body_y = 7
    elif frame == 6:
        body_y = 9

    # Six jointed legs retain a common ground line while changing stride.
    feet = (
        ((4, 13, 21), (9, 18, 28)),
        ((6, 15, 23), (8, 18, 26)),
        ((8, 18, 27), (5, 14, 23)),
        ((6, 16, 25), (7, 18, 29)),
    )[phase]
    if frame == 6:
        feet = ((3, 13, 24), (8, 20, 30))
    for root, foot in zip((8, 16, 24), feet[0]):
        d.line((root, body_y + 11, root - 1, 19, foot, 22), fill=1, width=2)
        d.line((foot, 22, min(31, foot + 2), 22), fill=4)
        d.point((root - 1, 19), fill=5)
        d.point((root, body_y + 12), fill=7)
    for root, foot in zip((11, 20, 27), feet[1]):
        d.line((root, body_y + 11, root + 1, 19, foot, 22), fill=1, width=2)
        d.line((max(0, foot - 1), 22, min(31, foot + 1), 22), fill=4)
        d.point((root + 1, 19), fill=7)
        d.point((root, body_y + 12), fill=5)

    # A compact three-segment dome carries the concept's armour volume without
    # filling the cell with noisy single pixels.  Steel highlights describe a
    # single light direction; violet is reserved for the recessed lower shell.
    d.ellipse((8, body_y, 31, body_y + 13), fill=1)
    d.ellipse((10, body_y + 1, 29, body_y + 12), fill=7)
    d.polygon(((11, body_y + 8), (13, body_y + 4), (17, body_y + 2),
               (18, body_y + 12), (12, body_y + 11)), fill=5)
    d.polygon(((20, body_y + 2), (25, body_y + 4), (28, body_y + 7),
               (27, body_y + 10), (20, body_y + 12)), fill=5)
    d.line((19, body_y + 2, 19, body_y + 12), fill=1)
    d.line((12, body_y + 5, 15, body_y + 2), fill=4)
    d.line((16, body_y + 2, 17, body_y + 2), fill=3)
    d.line((21, body_y + 3, 24, body_y + 4), fill=4)
    d.point((25, body_y + 5), fill=3)
    d.line((12, body_y + 10, 17, body_y + 12), fill=1)
    d.line((21, body_y + 12, 26, body_y + 10), fill=1)
    d.point((28, body_y + 8), fill=3)

    # The face plate overlaps the shell as one readable round head.  A dark
    # bezel gives the cyan lens enough contrast to survive PAL scaling.
    d.ellipse((1, body_y + 5, 14, body_y + 17), fill=1)
    d.ellipse((3, body_y + 6, 12, body_y + 15), fill=5)
    d.ellipse((4, body_y + 7, 11, body_y + 14), fill=1)
    d.ellipse((5, body_y + 8, 10, body_y + 13), fill=6)
    d.line((6, body_y + 8, 8, body_y + 8), fill=4)
    d.point((6, body_y + 9), fill=4)
    d.point((9, body_y + 12), fill=5)
    d.point((4, body_y + 14), fill=7)
    d.point((11, body_y + 9), fill=4)
    d.line((5, body_y + 5, 4, body_y + 1 + (phase & 1)), fill=1)
    d.line((9, body_y + 5, 10, body_y + 1 - (phase & 1)), fill=1)
    d.point((4, body_y + (phase & 1)), fill=6)
    d.point((10, body_y - (phase & 1)), fill=6)
    d.point((2, body_y + 10), fill=4)

    if frame == 4:
        d.line((14, 8, 18, 10, 15, 13, 21, 17), fill=6, width=2)
        d.line((22, 8, 20, 11, 24, 14), fill=4)
        d.point((30, 7), fill=6)
        d.point((8, 6), fill=4)
        d.point((3, 18), fill=6)
    elif frame == 5:
        d.line((16, 5, 20, 9, 17, 13, 23, 18), fill=6, width=2)
        d.line((25, 7, 21, 11, 25, 15), fill=4)
        d.point((12, 8), fill=6)
        d.point((27, 18), fill=7)
    elif frame == 6:
        d.line((10, 14, 15, 11, 19, 16, 25, 12), fill=6, width=2)
        d.line((5, 19, 2, 21), fill=7)
        d.line((12, 20, 16, 22), fill=5)
        d.point((29, 20), fill=4)
    return image


def make_clockwork_beetle() -> tuple[Image.Image, bytes]:
    concept = Image.open(ROOT / "assets" / "enemies" /
                         "clockwork-beetle-concept-v2-transparent.png").convert("RGBA")
    poses = []
    for frame in range(BEETLE_FRAMES):
        cell = grid_cell(concept, 3, 3, frame)
        bounds = cell.getchannel("A").getbbox()
        poses.append(cell.crop(bounds) if bounds else
                     Image.new("RGBA", (1, 1), (0, 0, 0, 0)))

    # One scale for the complete four-frame walk family.  The hit frame keeps
    # that same body scale (its electric accents may clip at the cell edge),
    # while the destruction stages are allowed only to shrink as parts spread.
    # The generated source includes tall antennae.  A single isotropic fit
    # would let those two thin details shrink the entire armoured body.  Use
    # one shared X/Y reduction for the family instead: the very slight native
    # wide-pixel correction restores the concept's heavy, planted silhouette.
    walk_scale_x = 31 / max(p.width for p in poses[:4])
    walk_scale_y = 18 / max(p.height for p in poses[:4])
    cells = []
    for frame, pose in enumerate(poses):
        scale_x, scale_y = walk_scale_x, walk_scale_y
        if frame >= 5:
            fit = min(walk_scale_x, walk_scale_y,
                      31 / pose.width, 21 / pose.height)
            scale_x = scale_y = fit
        width = max(1, round(pose.width * scale_x))
        height = max(1, round(pose.height * scale_y))
        reduced = pose.resize((width, height), Image.Resampling.LANCZOS)
        reduced_bounds = reduced.getchannel("A").getbbox()
        opaque_bottom = reduced_bounds[3] if reduced_bounds else height
        canvas = Image.new("RGBA", (BEETLE_W, BEETLE_H), (0, 0, 0, 0))
        # Align the actual opaque silhouette, not the resized RGBA canvas.
        # Lanczos can leave a transparent final source row; grounding against
        # the canvas height made the beetle appear to hover by one PAL pixel.
        canvas.alpha_composite(reduced, ((BEETLE_W - width) // 2,
                                         BEETLE_H - opaque_bottom))
        indexed = indexed_image((BEETLE_W, BEETLE_H), FRONT8, 0)
        source_pixels, target_pixels = canvas.load(), indexed.load()
        for y in range(BEETLE_H):
            for x in range(BEETLE_W):
                red, green, blue, alpha = source_pixels[x, y]
                if alpha >= 96:
                    target_pixels[x, y] = nearest_index(
                        (red, green, blue), FRONT8, avoid_zero=True)
        cells.append(indexed)

    sheet = indexed_image((BEETLE_W * 2, BEETLE_H * BEETLE_FRAMES), FRONT8, 0)
    preview = indexed_image((BEETLE_W * BEETLE_FRAMES, BEETLE_H), FRONT8, 0)
    for frame, cell in enumerate(cells):
        sheet.paste(cell, (0, frame * BEETLE_H))
        sheet.paste(cell.transpose(Image.Transpose.FLIP_LEFT_RIGHT),
                    (BEETLE_W, frame * BEETLE_H))
        preview.paste(cell, (frame * BEETLE_W, 0))
    preview.info["transparency"] = 0
    preview.save(ROOT / "assets" / "enemies" /
                 "clockwork-beetle-32x24-aga8.png")
    return sheet, bitmap_mask(sheet)


def make_clockwork_strider() -> tuple[Image.Image, bytes]:
    # Slots 0..7 hold the reviewed rigid mechanical walk and slot 8 the planted
    # frontal turn. Slots 9/10 are ranged attack, 11..17 are the Phase 5F.3
    # non-lethal hit reaction, 18..23 remain traversal-only, and Phase 5F.4
    # appends destruction frames 24..27 without renumbering either contract.
    def indexed_cell(source: Image.Image) -> Image.Image:
        bounds = source.getchannel("A").getbbox()
        opaque_bottom = bounds[3] if bounds else source.height
        canvas = Image.new("RGBA", (STRIDER_W, STRIDER_H), (0, 0, 0, 0))
        canvas.alpha_composite(source, ((STRIDER_W - source.width) // 2,
                                        62 - opaque_bottom))
        indexed = indexed_image((STRIDER_W, STRIDER_H), FRONT16, 0)
        source_pixels, target_pixels = canvas.load(), indexed.load()
        for y in range(STRIDER_H):
            for x in range(STRIDER_W):
                red, green, blue, alpha = source_pixels[x, y]
                if alpha >= 96:
                    target_pixels[x, y] = nearest_index(
                        (red, green, blue), FRONT16, avoid_zero=True)
        return indexed

    idle_source = Image.open(ROOT / "assets" / "enemies" /
                             "clockwork-storm-strider-64x64-aga15-idle-v3.png").convert("RGBA")
    idle = indexed_cell(idle_source)
    cells = [idle.copy() for _ in range(STRIDER_FRAMES)]
    walk_source = Image.open(ROOT / "assets" / "enemies" /
                             "clockwork-storm-strider-64x64-aga15-walk-rig-v1.png").convert("RGBA")
    for frame in range(8):
        cells[frame] = indexed_cell(
            walk_source.crop((frame * STRIDER_W, 0,
                              (frame + 1) * STRIDER_W, STRIDER_H)))
    turn_source = Image.open(ROOT / "assets" / "enemies" /
                             "clockwork-storm-strider-64x64-aga15-turn-v1.png").convert("RGBA")
    cells[8] = indexed_cell(turn_source)

    # Phase 5D appends traversal art after the untouched 0..17 locomotion and
    # combat-reservation contract. These deterministic poses retain the
    # accepted silhouette and palette while making the cyan charge readable.
    def compressed(source: Image.Image, height: int, glow: int) -> Image.Image:
        resized = source.resize((STRIDER_W, height), Image.Resampling.NEAREST)
        cell = indexed_image((STRIDER_W, STRIDER_H), FRONT16, 0)
        cell.paste(resized, (0, 62 - height))
        pixels = cell.load()
        for y in range(max(0, 58 - glow), 62):
            for x in range(18, 46):
                if pixels[x, y] and ((x + y) & 3) == 0:
                    pixels[x, y] = 6
        return cell

    cells[18] = compressed(idle, 56, 2)
    cells[19] = compressed(idle, 50, 5)
    cells[20] = compressed(idle, 54, 3)
    cells[21] = compressed(idle, 57, 2)
    cells[22] = compressed(idle, 49, 4)
    cells[23] = compressed(idle, 58, 1)

    # Phase 5F.2A replaces the rejected procedural belly-level overlay with one
    # coherent premium attack source. Fit the full actor once, then derive both
    # runtime cells from identical anatomy; only the muzzle energy differs.
    attack_source = Image.open(ROOT / "assets" / "enemies" /
        "clockwork-storm-strider-premium-shoot-source-v1-transparent.png").convert("RGBA")
    attack_bounds = attack_source.getchannel("A").getbbox()
    if not attack_bounds:
        raise ValueError("empty Strider shoot source")
    attack_source = attack_source.crop(attack_bounds)
    attack_scale = min(62 / attack_source.width, 60 / attack_source.height)
    attack_source = attack_source.resize(
        (max(1, round(attack_source.width * attack_scale)),
         max(1, round(attack_source.height * attack_scale))),
        Image.Resampling.LANCZOS)
    premium_attack = indexed_cell(attack_source)
    # Preserve the exact accepted idle anatomy, grounding and palette balance.
    # Only replace the forward forearm/claw with the premium cannon fragment;
    # its separate source was four pixels taller and substantially wider when
    # scaled as a complete actor. Shift the fragment down to the idle shoulder.
    attack = idle.copy()
    attack_pixels = attack.load()
    premium_pixels = premium_attack.load()
    for y in range(24, 45):
        for x in range(33, STRIDER_W):
            attack_pixels[x, y] = 0
    for source_y in range(20, 41):
        target_y = source_y + 4
        for x in range(33, STRIDER_W):
            pen = premium_pixels[x, source_y]
            if pen:
                attack_pixels[x, target_y] = pen
    cells[9] = attack.copy()
    cells[10] = attack.copy()
    muzzle_bounds = cells[9].getbbox()
    if muzzle_bounds:
        muzzle_x = muzzle_bounds[2] - 1
        muzzle_y = 36
        charge = ImageDraw.Draw(cells[9])
        charge.point((muzzle_x, muzzle_y - 1), fill=14)
        charge.point((muzzle_x, muzzle_y), fill=4)
        charge.point((muzzle_x, muzzle_y + 1), fill=14)

    # A seven-stage mechanical recoil uses the accepted idle anatomy and foot
    # anchor. The right-facing source recoils left, then settles; the packed
    # mirror supplies the exact opposite reaction without a second code path.
    recoil_offsets = ((-2, 0), (-5, 0), (-8, 0), (-9, 0),
                      (-6, 0), (-3, 0), (0, 0))
    for index, (offset_x, offset_y) in enumerate(recoil_offsets):
        recoil = indexed_image((STRIDER_W, STRIDER_H), FRONT16, 0)
        recoil.paste(idle, (offset_x, offset_y))
        spark = ImageDraw.Draw(recoil)
        spark_x = 49 + offset_x
        spark_y = 19 + offset_y
        if index < 5:
            spark.line((spark_x - 2, spark_y, spark_x + 2, spark_y), fill=14)
            spark.line((spark_x, spark_y - 2, spark_x, spark_y + 2), fill=6)
            spark.point((spark_x - 4 - index, spark_y - 3), fill=14)
            if index < 3:
                spark.line((spark_x - 7, spark_y + 5,
                            spark_x - 5, spark_y + 3), fill=4)
        cells[11 + index] = recoil

    # Phase 5F.4 is append-only and uses the accepted indexed idle itself as
    # the sole anatomy/material source. The generated high-resolution concept
    # defines only the four beats; no foreign RGB pixels enter runtime art.
    def paste_nonzero(target: Image.Image, source: Image.Image,
                      box: tuple[int, int, int, int],
                      destination: tuple[int, int]) -> None:
        target_pixels, source_pixels = target.load(), source.load()
        left, top, right, bottom = box
        dest_x, dest_y = destination
        for source_y in range(top, bottom):
            for source_x in range(left, right):
                pen = source_pixels[source_x, source_y]
                x = dest_x + source_x - left
                y = dest_y + source_y - top
                if pen and 0 <= x < STRIDER_W and 0 <= y < STRIDER_H:
                    target_pixels[x, y] = pen

    fracture = idle.copy()
    fracture_pixels = fracture.load()
    for x, y in ((30, 20), (31, 21), (31, 22), (32, 23), (31, 24),
                 (32, 25), (33, 26), (32, 27), (31, 28), (32, 29),
                 (31, 30), (30, 31), (31, 32), (30, 33), (29, 34),
                 (30, 35), (29, 36), (28, 37), (29, 38), (28, 39)):
        if fracture_pixels[x, y]:
            fracture_pixels[x, y] = 6 if ((x + y) & 1) else 11
    cells[24] = fracture

    rupture = indexed_image((STRIDER_W, STRIDER_H), FRONT16, 0)
    rupture_pixels, idle_pixels = rupture.load(), idle.load()
    for y in range(STRIDER_H):
        for x in range(STRIDER_W):
            pen = idle_pixels[x, y]
            if not pen:
                continue
            if y < 20:
                target_x, target_y = x + 2, y - 2
            elif x < 27 and y < 45:
                target_x, target_y = x - 3, y + 1
            elif x > 35 and y < 46:
                target_x, target_y = x + 4, y + 1
            else:
                target_x, target_y = x, y
            if 0 <= target_x < STRIDER_W and 0 <= target_y < STRIDER_H:
                rupture_pixels[target_x, target_y] = pen
    rupture_draw = ImageDraw.Draw(rupture)
    rupture_draw.line((30, 21, 34, 37), fill=6)
    rupture_draw.line((34, 27, 39, 31), fill=11)
    rupture_draw.point((25, 29), fill=14)
    rupture_draw.point((42, 25), fill=6)
    cells[25] = rupture

    burst = indexed_image((STRIDER_W, STRIDER_H), FRONT16, 0)
    paste_nonzero(burst, idle, (19, 6, 47, 20), (23, 1))
    paste_nonzero(burst, idle, (10, 20, 27, 43), (3, 25))
    paste_nonzero(burst, idle, (35, 20, 53, 45), (43, 24))
    paste_nonzero(burst, idle, (18, 38, 32, 62), (15, 39))
    paste_nonzero(burst, idle, (31, 38, 46, 62), (35, 39))
    burst_draw = ImageDraw.Draw(burst)
    burst_draw.line((27, 25, 36, 36), fill=6)
    burst_draw.line((35, 25, 27, 37), fill=11)
    burst_draw.point((20, 20), fill=14)
    burst_draw.point((45, 18), fill=6)
    cells[26] = burst

    debris = indexed_image((STRIDER_W, STRIDER_H), FRONT16, 0)
    paste_nonzero(debris, idle, (24, 8, 39, 17), (37, 49))
    paste_nonzero(debris, idle, (15, 50, 29, 62), (12, 50))
    paste_nonzero(debris, idle, (33, 50, 48, 62), (31, 50))
    debris_draw = ImageDraw.Draw(debris)
    debris_draw.point((25, 47), fill=6)
    debris_draw.point((31, 43), fill=11)
    debris_draw.point((47, 46), fill=14)
    debris_draw.point((52, 53), fill=6)
    cells[27] = debris

    for frame, cell in enumerate(cells):
        pen_counts = Counter(cell.getdata())
        forbidden = set(pen_counts) & STRIDER_FORBIDDEN_PENS
        if forbidden:
            raise ValueError(
                f"Strider frame {frame} uses forbidden identity pens {sorted(forbidden)}")
        neutral_count = sum(pen_counts[pen] for pen in STRIDER_NEUTRAL_PENS)
        violet_count = sum(pen_counts[pen] for pen in STRIDER_VIOLET_PENS)
        if violet_count > neutral_count:
            raise ValueError(
                f"Strider frame {frame} violet material exceeds steel/charcoal balance")

    sheet = indexed_image((STRIDER_W * 2, STRIDER_H * STRIDER_FRAMES),
                          FRONT16, 0)
    preview = indexed_image((STRIDER_W * 8, STRIDER_H), FRONT16, 0)
    attack_preview = indexed_image((STRIDER_W * 2, STRIDER_H), FRONT16, 0)
    hit_preview = indexed_image((STRIDER_W * 7, STRIDER_H), FRONT16, 0)
    traversal_preview = indexed_image((STRIDER_W * 6, STRIDER_H), FRONT16, 0)
    death_preview = indexed_image((STRIDER_W * 4, STRIDER_H), FRONT16, 0)
    for frame, cell in enumerate(cells):
        sheet.paste(cell, (0, frame * STRIDER_H))
        sheet.paste(cell.transpose(Image.Transpose.FLIP_LEFT_RIGHT),
                    (STRIDER_W, frame * STRIDER_H))
        if frame < 8:
            preview.paste(cell, (frame * STRIDER_W, 0))
        if 9 <= frame <= 10:
            attack_preview.paste(cell, ((frame - 9) * STRIDER_W, 0))
        if 11 <= frame <= 17:
            hit_preview.paste(cell, ((frame - 11) * STRIDER_W, 0))
        if 18 <= frame < 24:
            traversal_preview.paste(cell, ((frame - 18) * STRIDER_W, 0))
        if 24 <= frame < 28:
            death_preview.paste(cell, ((frame - 24) * STRIDER_W, 0))
    preview.info["transparency"] = 0
    preview.save(ROOT / "assets" / "enemies" /
                 "clockwork-storm-strider-64x64-aga15-walk-preview-v1.png")
    attack_preview.info["transparency"] = 0
    attack_preview.save(ROOT / "assets" / "enemies" /
                        "clockwork-storm-strider-64x64-aga15-shoot-preview-v1.png")
    hit_preview.info["transparency"] = 0
    hit_preview.save(ROOT / "assets" / "enemies" /
                     "clockwork-storm-strider-64x64-aga15-hit-preview-v1.png")
    traversal_preview.info["transparency"] = 0
    traversal_preview.save(ROOT / "assets" / "enemies" /
                           "clockwork-storm-strider-64x64-aga15-traversal-preview-v1.png")
    death_preview.info["transparency"] = 0
    death_preview.save(ROOT / "assets" / "enemies" /
                       "clockwork-storm-strider-64x64-aga15-death-preview-v1.png")
    return sheet, bitmap_mask(sheet)


def cropped_component(image: Image.Image) -> Image.Image:
    image = keep_main_component(image)
    bounds = image.getchannel("A").getbbox()
    return image.crop(bounds) if bounds else Image.new("RGBA", (1, 1), (0, 0, 0, 0))


def fixed_scale_pose(pose: Image.Image, scale: float) -> Image.Image:
    """Scale every animation family uniformly instead of filling each cell.

    Per-frame thumbnail scaling made a crouching Sparkpaw look as large as a
    standing Sparkpaw. A single scale per authored sheet preserves anatomy;
    the 48x48 cell is merely a canvas and never a target bounding box.
    """
    width = max(1, round(pose.width * scale))
    height = max(1, round(pose.height * scale))
    # Detached attack effects in the legacy sheet may be exceptionally wide.
    # They are not locomotion frames, but still need to remain inside a cell.
    if width > 48 or height > 48:
        fit = min(48 / width, 48 / height)
        width, height = max(1, round(width * fit)), max(1, round(height * fit))
    return pose.resize((width, height), Image.Resampling.LANCZOS)


def grid_cell(sheet: Image.Image, cols: int, rows: int, index: int) -> Image.Image:
    """Crop a generated grid without assuming dimensions divide perfectly."""
    col, row = index % cols, index // cols
    return sheet.crop((round(col * sheet.width / cols),
                       round(row * sheet.height / rows),
                       round((col + 1) * sheet.width / cols),
                       round((row + 1) * sheet.height / rows)))


def family_scale(poses: list[Image.Image], max_width: int = 46,
                 max_height: int = 44) -> float:
    """One scale for a complete animation family; never resize per frame."""
    return min(max_width / max(p.width for p in poses),
               max_height / max(p.height for p in poses))


def make_sprites() -> tuple[Image.Image, bytes]:
    reference = Image.open(ROOT / "assets" / "sprites" /
                           "sparkpaw-sprites-reference-transparent.png").convert("RGBA")
    supplemental = Image.open(ROOT / "assets" / "sprites" /
                              "sparkpaw-supplemental-poses-v2-transparent.png").convert("RGBA")
    run_sheet = Image.open(ROOT / "assets" / "sprites" /
                           "sparkpaw-run-compact-v3-transparent.png").convert("RGBA")
    jump_sheet = Image.open(ROOT / "assets" / "sprites" /
                            "sparkpaw-jump-refinement-v5-transparent.png").convert("RGBA")
    landing_sheet = Image.open(ROOT / "assets" / "sprites" /
                               "sparkpaw-landing-refinement-v6-transparent.png").convert("RGBA")
    motion_sheet = Image.open(ROOT / "assets" / "sprites" /
                              "sparkpaw-motion-refinement-v3-transparent.png").convert("RGBA")
    turn_sheet = Image.open(ROOT / "assets" / "sprites" /
                            "sparkpaw-turn-refinement-v4-transparent.png").convert("RGBA")
    idle_sheet = Image.open(ROOT / "assets" / "sprites" /
                            "sparkpaw-idle-refinement-v3-transparent.png").convert("RGBA")
    combat_sheet = Image.open(ROOT / "assets" / "sprites" /
                              "sparkpaw-combat-refinement-v7-transparent.png").convert("RGBA")
    air_fire_sheet = Image.open(ROOT / "assets" / "sprites" /
                                "sparkpaw-air-fire-v8-transparent.png").convert("RGBA")
    crouch_fire_sheet = Image.open(ROOT / "assets" / "sprites" /
                                   "sparkpaw-crouch-fire-v9-transparent.png").convert("RGBA")
    hurt_sheet = Image.open(ROOT / "assets" / "sprites" /
                            "sparkpaw-hurt-v12-transparent.png").convert("RGBA")
    crouch_hurt_sheet = Image.open(ROOT / "assets" / "sprites" /
                                   "sparkpaw-crouch-hurt-v13-transparent.png").convert("RGBA")
    ledge_sheet = Image.open(ROOT / "assets" / "sprites" /
                             "sparkpaw-ledge-teeter-v1-transparent.png").convert("RGBA")
    cell_w = cell_h = 48
    frame_count = 62
    rows = math.ceil(frame_count / 4)
    source = Image.new("RGBA", (cell_w * 4, cell_h * rows), (0, 0, 0, 0))
    ref_w, ref_h = reference.width // 4, reference.height // 4

    def old_pose(index: int) -> Image.Image:
        return cropped_component(reference.crop(((index & 3) * ref_w,
                                                  (index >> 2) * ref_h,
                                                  ((index & 3) + 1) * ref_w,
                                                  ((index >> 2) + 1) * ref_h)))

    def place(frame: int, pose: Image.Image, scale: float) -> None:
        pose = fixed_scale_pose(pose, scale)
        x = (cell_w - pose.width) // 2
        y = cell_h - pose.height
        source.alpha_composite(pose, ((frame & 3) * cell_w + x,
                                      (frame >> 2) * cell_h + y))

    idle_source = old_pose(0)
    legacy_scale = 44 / idle_source.height
    run_poses = [cropped_component(grid_cell(run_sheet, 4, 2, i)) for i in range(8)]
    jump_poses = [cropped_component(grid_cell(jump_sheet, 4, 1, i)) for i in range(4)]
    landing_poses = [cropped_component(grid_cell(landing_sheet, 3, 1, i)) for i in range(3)]
    motion_poses = [cropped_component(grid_cell(motion_sheet, 4, 4, i)) for i in range(16)]
    turn_poses = [cropped_component(grid_cell(turn_sheet, 3, 2, i)) for i in range(6)]
    crouch_poses = [cropped_component(grid_cell(supplemental, 3, 2, i)) for i in range(3)]
    idle_poses = [cropped_component(grid_cell(idle_sheet, 4, 3, i)) for i in range(12)]
    combat_poses = [cropped_component(grid_cell(combat_sheet, 4, 1, i)) for i in range(4)]
    air_fire_poses = [cropped_component(grid_cell(air_fire_sheet, 4, 1, i)) for i in range(4)]
    crouch_fire_poses = [cropped_component(grid_cell(crouch_fire_sheet, 4, 1, i)) for i in range(4)]
    hurt_poses = [cropped_component(grid_cell(hurt_sheet, 4, 1, i)) for i in range(4)]
    crouch_hurt_poses = [cropped_component(grid_cell(crouch_hurt_sheet, 4, 1, i)) for i in range(4)]
    ledge_poses = [cropped_component(grid_cell(ledge_sheet, 4, 1, i))
                   for i in range(4)]
    run_scale = family_scale(run_poses)
    jump_scale = family_scale(jump_poses)
    landing_scale = family_scale(landing_poses)
    turn_scale = family_scale(turn_poses)
    crouch_scale = family_scale(crouch_poses, max_width=48)
    idle_scale = family_scale(idle_poses)
    combat_scale = family_scale(combat_poses, max_width=48)
    hurt_scale = family_scale(hurt_poses, max_width=48)
    crouch_hurt_scale = family_scale(crouch_hurt_poses,
                                     max_width=48,max_height=24)
    ledge_scale = family_scale(ledge_poses)
    # Match the established airborne body scale. The extended barrel may clip
    # by a pixel at the 48-pixel hardware-sprite cell edge, but shrinking the
    # whole actor would create the much more visible in-air zoom artifact.
    air_fire_scale = 44 / max(p.height for p in air_fire_poses)

    # Logical runtime order: base idle/blink, refined eight-frame run,
    # proven jump arc, matching three-stage landing, proven crouch, six-stage turn,
    # and a twelve-stage slow idle performance.
    place(0, old_pose(0), legacy_scale)
    place(1, old_pose(1), legacy_scale)
    for i, pose in enumerate(run_poses):
        place(2 + i, pose, run_scale)
    # At 48x48 the open apex and pre-contact silhouettes lose one or two
    # raster rows during quantisation and read as a camera zoom.  Tiny authored
    # compensations keep perceived body mass stable without forcing every
    # differently articulated pose to fill its complete cell.
    jump_scale_compensation = (1.0, 1.04, 1.0, 1.025)
    for i, pose in enumerate(jump_poses):
        place(10 + i, pose, jump_scale * jump_scale_compensation[i])
    for i, pose in enumerate(landing_poses):
        place(14 + i, pose, landing_scale)
    for i, pose in enumerate(crouch_poses):
        place(17 + i, pose, crouch_scale)
    for i, pose in enumerate(turn_poses):
        place(20 + i, pose, turn_scale)
    for i, pose in enumerate(idle_poses):
        place(26 + i, pose, idle_scale)
    for i, pose in enumerate(combat_poses):
        place(38 + i, pose, combat_scale)
    for i, pose in enumerate(air_fire_poses):
        place(42 + i, pose, air_fire_scale)
    # Crouch-fire shares the proven crouch family's authored scale and ground
    # baseline. The extended gauntlet must not make the actor stand up or zoom.
    for i, pose in enumerate(crouch_fire_poses):
        place(46 + i, pose, crouch_scale)
    # Hurt poses append after the accepted 0-49 contract. One family scale
    # preserves Sparkpaw's body mass while recoil changes his silhouette.
    for i, pose in enumerate(hurt_poses):
        place(50 + i, pose, hurt_scale)
    # The crouched hurt family may not exceed the established 30-pixel crouch
    # silhouette, so temporary visual recoil respects low-platform clearance.
    for i, pose in enumerate(crouch_hurt_poses):
        place(54 + i, pose, crouch_hurt_scale)
    # Phase 6B.5 appends the approved review family. Never replace or renumber
    # the accepted 0..57 baseline; all four cells share one scale and baseline.
    for i, pose in enumerate(ledge_poses):
        place(58 + i, pose, ledge_scale)
    source.save(ROOT / "assets" / "sprites" / "sparkpaw-48x48-aga16-source.png")
    # Preserve all authored poses. Left half contains the right-facing
    # cells and the second half contains deterministic pixel-perfect mirrors.
    # Direction changes therefore need no runtime flip or pixel copying.
    doubled = Image.new("RGBA", (source.width * 2, source.height), (0, 0, 0, 0))
    doubled.paste(source, (0, 0), source)
    for row in range(rows):
        for col in range(4):
            cell = source.crop((col * cell_w, row * cell_h,
                                col * cell_w + cell_w, row * cell_h + cell_h))
            doubled.paste(cell.transpose(Image.Transpose.FLIP_LEFT_RIGHT),
                          (source.width + col * cell_w, row * cell_h))
    source = doubled
    image = indexed_image(source.size, FG_PALETTE, 0)
    src = source.load()
    dst = image.load()
    mask_row = ((source.width + 15) // 16) * 2
    mask = bytearray(mask_row * source.height)
    for y in range(source.height):
        for x in range(source.width):
            r, g, b, a = src[x, y]
            if a < 96:
                continue
            dst[x, y] = nearest_index((r, g, b), FG_PALETTE, avoid_zero=True)
            mask[y * mask_row + (x >> 3)] |= 0x80 >> (x & 7)
    # Keep an exact, human-viewable copy of the 15-colour right-facing frames.
    preview = image.crop((0, 0, image.width // 2, image.height))
    preview.info["transparency"] = 0
    preview.save(ROOT / "assets" / "sprites" / "sparkpaw-48x48-aga16.png")
    return image, bytes(mask)


def main() -> None:
    RUNTIME.mkdir(parents=True, exist_ok=True)
    LEVELS.mkdir(parents=True, exist_ok=True)
    bg = make_authored_parallax()
    fg, collision = make_foreground()
    sprites, mask = make_sprites()
    beetle, beetle_mask = make_clockwork_beetle()
    strider, strider_mask = make_clockwork_strider()
    title, title_palette = load_aga_screen(TITLE_SOURCE)
    title,title_palette = reserve_black_pen_zero(title,title_palette)
    title.save(TITLE_RUNTIME_PREVIEW)
    level_loading,level_charging,level_loading_palette = make_level_loading()
    hud_base,hud_health,hud_lives,hud_diamonds = make_hud()
    collectible_diamond,collectible_diamond_mask = make_collectible_diamond()
    stormstone_core,stormstone_core_mask = make_stormstone_core()

    save_spbm(RUNTIME / "sparkpaw-title.spbm",title,title_palette,depth=6)
    save_spbm(RUNTIME / "sparkpaw-level-loading.spbm",level_loading,
              level_loading_palette,depth=6)
    # Runtime keeps the accepted LOADING floppy picture resident and replaces
    # only this word-sized status band while faded to black.  The separate
    # generated preview remains useful for visual review, but a second complete
    # 320x256 six-plane bitmap no longer consumes Chip RAM.
    charging_patch = level_charging.crop((48, 192, 272, 232))
    save_spbm(RUNTIME / "level-charge-patch.spbm",charging_patch,
              level_loading_palette,depth=6)
    save_spbm(RUNTIME / "sparkpaw-hud-base.spbm",hud_base,HUD_PALETTE,depth=3)
    save_spbm(RUNTIME / "sparkpaw-hud-health.spbm",hud_health,
              HUD_PALETTE,depth=3)
    save_spbm(RUNTIME / "sparkpaw-hud-lives.spbm",hud_lives,
              HUD_PALETTE,depth=3)
    save_spbm(RUNTIME / "sparkpaw-hud-diamonds.spbm",hud_diamonds,
              HUD_PALETTE,depth=3)
    save_spbm(RUNTIME / "sparkpaw-diamond.spbm",collectible_diamond,
              FRONT16,depth=4,mask=collectible_diamond_mask)
    save_spbm(RUNTIME / "stormstone-core.spbm",stormstone_core,
              FRONT16,depth=4,mask=stormstone_core_mask)
    core_preview=stormstone_core.resize(
        (64*4,48*CORE_RUNTIME_FRAMES*4),Image.Resampling.NEAREST)
    core_preview.save(ROOT / "assets" / "concept" /
                      "sparkpaw-stormstone-core-aga16-preview-v2.png")

    # Separate hardware-scrollable 3-plane layers for the C dual-playfield
    # renderer. The 1024px authored rear span covers every quarter-speed source
    # coordinate visible in both the current 2048px world and planned 3072px
    # route; the padded repeat is never reached by either camera range.
    rear_tile = bg
    rear_world = indexed_image((WORLD_W, WORLD_H), REAR8, 0)
    for x in range(0, WORLD_W, PARALLAX_W):
        rear_world.paste(rear_tile, (x, 0))
    rear_preview = rear_rgb(rear_world)
    front_world = fg
    apply_ground_water_treatment(front_world)
    sprite_world = remap(sprites, FG_PALETTE, FRONT8, transparent_zero=True)
    sprite_mask = bitmap_mask(sprite_world)
    # The Copper changes to the independent HUD after 208 gameplay rows.  The
    # camera's maximum quarter-scroll fetch ends inside the authored 1024px
    # rear master, so the repeated tail and hidden bottom rows were never
    # observable and need not occupy displayable Chip RAM.
    save_spbm(RUNTIME / "storm-front.spbm",
              front_world.crop((0, 0, WORLD_W, GAMEPLAY_H)),
              FRONT16, depth=4)
    save_spbm(RUNTIME / "storm-rear.spbm",
              rear_world.crop((0, 0, PARALLAX_W, GAMEPLAY_H)),
              REAR8, depth=3)
    save_spbm(RUNTIME / "sparkpaw-sprites3.spbm", sprite_world, FRONT8,
              depth=3, mask=sprite_mask)
    save_spbm(RUNTIME / "sparkpaw-sprites4.spbm", sprites, FG_PALETTE,
              depth=4, mask=mask)
    save_spbm(RUNTIME / "clockwork-beetle.spbm", beetle, FRONT16,
              depth=4, mask=beetle_mask)
    save_spbm(RUNTIME / "clockwork-storm-strider.spbm", strider, FRONT16,
              depth=4, mask=strider_mask)
    (RUNTIME / "energy-shot.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "energy-shot.raw").read_bytes())
    (RUNTIME / "player-hurt.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "player-hurt.raw").read_bytes())
    (RUNTIME / "enemy-hit.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "enemy-hit.raw").read_bytes())
    (RUNTIME / "enemy-death.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "enemy-death.raw").read_bytes())
    (RUNTIME / "strider-shot.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "strider-shot.raw").read_bytes())
    (RUNTIME / "jump.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "jump.raw").read_bytes())
    (RUNTIME / "collect-spark.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "collect-spark.raw").read_bytes())
    (RUNTIME / "water-splash.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "water-splash.raw").read_bytes())
    (RUNTIME / "stormstone-core.raw").write_bytes(
        make_stormstone_core_triumph_raw())
    world = indexed_image((WORLD_W, WORLD_H), WORLD_PALETTE, 16)
    shifted_bg = bg.point(lambda p: p + 16)
    for x in range(0, WORLD_W, PARALLAX_W):
        world.paste(shifted_bg, (x, 0))
    world_pixels = world.load()
    fg_pixels = fg.load()
    for y in range(WORLD_H):
        for x in range(WORLD_W):
            pen = int(fg_pixels[x, y])
            if pen:
                world_pixels[x, y] = pen
    save_spbm(RUNTIME / "storm-world.spbm", world, WORLD_PALETTE, depth=5)
    save_spbm(RUNTIME / "sparkpaw-sprites.spbm", sprites, WORLD_PALETTE, depth=5, mask=mask)
    (RUNTIME / "storm-background.spbm").unlink(missing_ok=True)
    (RUNTIME / "storm-foreground.spbm").unlink(missing_ok=True)
    (RUNTIME / "storm-midground.spbm").unlink(missing_ok=True)
    (RUNTIME / "storm-collision.bin").write_bytes(collision)
    authored_rear_preview=rear_rgb(bg)
    authored_rear_preview.crop((0, 0, 320, 256)).save(
        LEVELS / "storm-background-preview.png")
    authored_rear_preview.crop((0,0,PARALLAX_W,208)).save(
        LEVELS / "storm-parallax-copper-banded-preview.png")
    fg_preview = rear_preview.crop((0, 0, 320, 256))
    alpha = fg.crop((0, 0, 320, 256)).convert("RGBA")
    alpha.putalpha(alpha_image(fg.crop((0, 0, 320, 256))))
    fg_preview.paste(alpha, (0, 0), alpha)
    fg_preview.save(LEVELS / "storm-ruins-milestone-preview.png")
    # Actual 4+3 palette preview at the water camera position. The rear crop
    # uses the renderer's quarter-speed offset; the front crop is world-space.
    water_preview = rear_preview.crop((376,0,696,256))
    water_front = front_world.crop((1504,0,1824,256)).convert("RGBA")
    water_front.putalpha(alpha_image(front_world.crop((1504,0,1824,256))))
    water_preview.paste(water_front,(0,0),water_front)
    water_preview.save(LEVELS / "storm-water-hazard-aga-preview.png")
    # Four hardware-exact panels cover the complete Phase 6C route extension.
    # Each uses the renderer's quarter-speed rear sample and world-space front.
    route_preview=Image.new("RGB",(1280,208))
    for panel,camera_x in enumerate((1920,2176,2432,2752)):
        panel_image=rear_preview.crop((camera_x//4,0,camera_x//4+320,208))
        panel_front=front_world.crop((camera_x,0,camera_x+320,208)).convert("RGBA")
        panel_front.putalpha(alpha_image(front_world.crop((camera_x,0,camera_x+320,208))))
        panel_image.paste(panel_front,(0,0),panel_front)
        route_preview.paste(panel_image,(panel*320,0))
    route_preview.save(LEVELS / "storm-phase6c-route-aga-preview.png")
    core_camera=WORLD_W-320
    core_preview=rear_preview.crop((core_camera//4,0,
                                    core_camera//4+320,208))
    core_front_indexed=front_world.crop((core_camera,0,core_camera+320,208))
    core_front=core_front_indexed.convert("RGBA")
    core_front.putalpha(alpha_image(core_front_indexed))
    core_preview.paste(core_front,(0,0),core_front)
    core_frame=stormstone_core.crop((0,0,64,48)).convert("RGBA")
    core_frame.putalpha(alpha_image(stormstone_core.crop((0,0,64,48))))
    core_preview.paste(core_frame,(128,112),core_frame)
    core_preview.save(LEVELS / "storm-level1-core-clearing-aga-preview.png")
    make_water_animation_preview().save(
        LEVELS / "storm-water-animation-aga-preview.png")
    manifest = {
        "world": [WORLD_W, WORLD_H], "tile": TILE, "collision": [WORLD_W // TILE, 14],
        "foreground_palette": FRONT16, "background_palette": REAR8,
        "sprite_sheet": {
            "size": list(sprites.size), "frame": [48, 48], "frames": 62,
            "depth": 4, "hardware_layout": "three attached sprite pairs",
        },
        "clockwork_beetle": {
            "size": list(beetle.size), "frame": [BEETLE_W, BEETLE_H],
            "frames": BEETLE_FRAMES, "depth": 4,
            "visual_planes": 3,
            "layout": "left-facing column followed by mirrored right-facing column",
        },
        "clockwork_storm_strider": {
            "size": list(strider.size), "frame": [STRIDER_W, STRIDER_H],
            "frames": STRIDER_FRAMES, "depth": 4,
            "layout": "right-facing column followed by mirrored left-facing column",
        },
    }
    (RUNTIME / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print("Generated Sparkpaw planar runtime assets")


if __name__ == "__main__":
    main()
