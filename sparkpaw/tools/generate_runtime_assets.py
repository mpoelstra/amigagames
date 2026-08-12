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
from pathlib import Path

from PIL import Image, ImageDraw, ImageOps

ROOT = Path(__file__).resolve().parents[1]
RUNTIME = ROOT / "assets" / "runtime"
LEVELS = ROOT / "assets" / "levels"
WORLD_W, WORLD_H = 1280, 256
PARALLAX_W = 640
TILE = 16
BEETLE_W, BEETLE_H = 32, 24
BEETLE_FRAMES = 9

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
REAR8 = [
    (0, 0, 17), (0, 17, 51), (17, 34, 85), (34, 68, 119),
    (68, 68, 153), (102, 85, 170), (153, 119, 187), (204, 187, 221),
]

TITLE_SOURCE = ROOT / "assets" / "concept" / "sparkpaw-title-concept-aga64-preview.png"
TITLE_RUNTIME_PREVIEW = (ROOT / "assets" / "concept" /
                         "sparkpaw-title-aga64-runtime-preview.png")
LEVEL_LOADING_SOURCE = (ROOT / "assets" / "concept" /
                        "sparkpaw-level-loading-concept-v3.png")
LEVEL_LOADING_PREVIEW = (ROOT / "assets" / "concept" /
                         "sparkpaw-level-loading-aga64-preview.png")
LEVEL_CHARGING_SOURCE = (ROOT / "assets" / "concept" /
                         "sparkpaw-level-charging-concept-v2.png")
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
    # Keep the complete authored frame, but reserve a dark two-line separation
    # above it so actors no longer appear to stand directly on the metal beam.
    rgb = rgb.resize((320, HUD_HEIGHT-HUD_SEPARATOR_H),
                     Image.Resampling.LANCZOS)
    base = indexed_image((336, HUD_HEIGHT), HUD_PALETTE, 0)
    src, dst = rgb.load(), base.load()
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
    image = indexed_image((PARALLAX_W, WORLD_H), BG_PALETTE, 1)
    d = ImageDraw.Draw(image)
    # Pixel-banded storm sky: no expensive runtime gradient and no palette cycling.
    for y, pen in ((0, 1), (30, 2), (62, 3), (94, 4), (126, 5), (158, 6), (190, 7)):
        d.rectangle((0, y, PARALLAX_W - 1, min(y + 31, WORLD_H - 1)), fill=pen)
    # Distant mountains and forests repeat slowly enough to remain useful at /4 scroll.
    for base in range(-80, PARALLAX_W + 200, 220):
        peak = 52 + ((base // 220) & 3) * 8
        d.polygon([(base, 190), (base + 95, peak), (base + 205, 190)], fill=13)
        d.polygon([(base + 35, 190), (base + 112, peak + 28), (base + 220, 190)], fill=14)
    # Very sparse distant lights stay in the slowest plane.
    for x, y in ((92, 36), (278, 54), (443, 25), (594, 66)):
        d.point((x, y), fill=12)
        d.line((x - 2, y, x + 2, y), fill=12)
    d.line((535, 26, 522, 52, 535, 49, 515, 82), fill=12, width=2)
    return image


def make_midground() -> Image.Image:
    image = indexed_image((PARALLAX_W, WORLD_H), BG_PALETTE, 0)
    d = ImageDraw.Draw(image)
    # Towers, arches and trees form an independently moving middle distance.
    for x in (70, 335, 590):
        d.rectangle((x, 86, x + 38, 200), fill=2)
        d.rectangle((x - 7, 78, x + 45, 96), fill=3)
        d.polygon([(x - 5, 78), (x + 19, 48), (x + 43, 78)], fill=13)
        for wy in (108, 137, 166):
            d.rectangle((x + 14, wy, x + 23, wy + 12), fill=11)
    for x in range(-10, PARALLAX_W, 34):
        h = 28 + ((x * 13) % 38)
        d.polygon([(x, 223), (x + 17, 223 - h), (x + 34, 223)], fill=15 if x % 68 else 14)
    for x in (16, 280, 544):
        d.line((x, 106, x, 207), fill=2, width=5)
        d.line((x + 70, 106, x + 70, 207), fill=2, width=5)
        d.arc((x, 84, x + 70, 148), 180, 360, fill=6, width=4)
    return image


def make_foreground() -> tuple[Image.Image, bytearray]:
    image = indexed_image((WORLD_W, WORLD_H), FG_PALETTE, 0)
    d = ImageDraw.Draw(image)
    cols, rows = WORLD_W // TILE, 14
    collision = bytearray(cols * rows)

    def block(tx: int, ty: int, tw: int, th: int, style: int = 0) -> None:
        for yy in range(ty, min(ty + th, rows)):
            for xx in range(max(tx, 0), min(tx + tw, cols)):
                collision[yy * cols + xx] = 1
        x0, y0, x1, y1 = tx * TILE, ty * TILE, (tx + tw) * TILE - 1, (ty + th) * TILE - 1
        base = (5, 15, 2, 12)[style & 3]
        edge = (4, 7, 10, 9)[style & 3]
        d.rectangle((x0, y0, x1, y1), fill=1)
        d.rectangle((x0 + 2, y0 + 2, x1 - 1, y1 - 1), fill=base)
        d.line((x0 + 2, y0 + 2, x1 - 1, y0 + 2), fill=edge, width=2)
        for x in range(x0 + 8, x1, 16):
            d.line((x, y0 + 4, x, y1 - 2), fill=2)

    # Continuous floor and varied but readable platforming route over five screens.
    block(0, 13, cols, 1, 0)
    block(8, 10, 7, 1, 2)
    block(20, 8, 6, 1, 0)
    block(31, 11, 8, 1, 3)
    block(43, 9, 5, 1, 1)
    block(53, 7, 9, 1, 2)
    block(67, 10, 8, 1, 0)
    # Short columns establish occlusion and test collision at camera seams.
    block(16, 11, 2, 2, 1)
    block(40, 10, 2, 3, 0)
    block(63, 11, 2, 2, 3)
    # Lamps, chains and Stormstone markings remain non-solid decoration.
    for x in (190, 510, 830, 1150):
        d.line((x, 24, x, 66), fill=4, width=2)
        d.ellipse((x - 5, 62, x + 5, 72), fill=13, outline=7)
    for x in (270, 590, 910, 1230):
        d.polygon([(x, 184), (x + 8, 168), (x + 16, 184), (x + 8, 200)], fill=12, outline=3)
        d.point((x + 8, 176), fill=11)
    return image, collision


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
    cell_w = cell_h = 48
    frame_count = 58
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
    source.save(ROOT / "assets" / "sprites" / "sparkpaw-48x48-aga16-source.png")
    # Preserve all sixteen authored poses. Left half contains the right-facing
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
    bg = make_background()
    mid = make_midground()
    bg.paste(mid, (0, 0), alpha_image(mid))
    fg, collision = make_foreground()
    sprites, mask = make_sprites()
    beetle, beetle_mask = make_clockwork_beetle()
    title, title_palette = load_aga_screen(TITLE_SOURCE)
    title,title_palette = reserve_black_pen_zero(title,title_palette)
    title.save(TITLE_RUNTIME_PREVIEW)
    level_loading,level_charging,level_loading_palette = make_level_loading()
    hud_base,hud_health,hud_lives,hud_diamonds = make_hud()
    collectible_diamond,collectible_diamond_mask = make_collectible_diamond()

    save_spbm(RUNTIME / "sparkpaw-title.spbm",title,title_palette,depth=6)
    save_spbm(RUNTIME / "sparkpaw-level-loading.spbm",level_loading,
              level_loading_palette,depth=6)
    save_spbm(RUNTIME / "sparkpaw-level-charging.spbm",level_charging,
              level_loading_palette,depth=6)
    save_spbm(RUNTIME / "sparkpaw-hud-base.spbm",hud_base,HUD_PALETTE,depth=3)
    save_spbm(RUNTIME / "sparkpaw-hud-health.spbm",hud_health,
              HUD_PALETTE,depth=3)
    save_spbm(RUNTIME / "sparkpaw-hud-lives.spbm",hud_lives,
              HUD_PALETTE,depth=3)
    save_spbm(RUNTIME / "sparkpaw-hud-diamonds.spbm",hud_diamonds,
              HUD_PALETTE,depth=3)
    save_spbm(RUNTIME / "sparkpaw-diamond.spbm",collectible_diamond,
              FRONT8,depth=3,mask=collectible_diamond_mask)

    # Separate hardware-scrollable 3-plane layers for the C dual-playfield
    # renderer. The rear artwork repeats across the entire five-screen world.
    rear_tile = remap(bg, BG_PALETTE, REAR8)
    rear_world = indexed_image((WORLD_W, WORLD_H), REAR8, 0)
    for x in range(0, WORLD_W, PARALLAX_W):
        rear_world.paste(rear_tile, (x, 0))
    front_world = remap(fg, FG_PALETTE, FRONT8, transparent_zero=True)
    sprite_world = remap(sprites, FG_PALETTE, FRONT8, transparent_zero=True)
    sprite_mask = bitmap_mask(sprite_world)
    save_spbm(RUNTIME / "storm-front.spbm", front_world, FRONT8, depth=3)
    save_spbm(RUNTIME / "storm-rear.spbm", rear_world, REAR8, depth=3)
    save_spbm(RUNTIME / "sparkpaw-sprites3.spbm", sprite_world, FRONT8,
              depth=3, mask=sprite_mask)
    save_spbm(RUNTIME / "sparkpaw-sprites4.spbm", sprites, FG_PALETTE,
              depth=4, mask=mask)
    save_spbm(RUNTIME / "clockwork-beetle.spbm", beetle, FRONT8,
              depth=3, mask=beetle_mask)
    (RUNTIME / "energy-shot.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "energy-shot.raw").read_bytes())
    (RUNTIME / "player-hurt.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "player-hurt.raw").read_bytes())
    (RUNTIME / "enemy-hit.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "enemy-hit.raw").read_bytes())
    (RUNTIME / "jump.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "jump.raw").read_bytes())
    (RUNTIME / "collect-spark.raw").write_bytes(
        (ROOT / "sfx" / "raw" / "collect-spark.raw").read_bytes())
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
    bg.crop((0, 0, 320, 256)).save(LEVELS / "storm-background-preview.png")
    fg_preview = bg.crop((0, 0, 320, 256)).convert("RGB")
    mid_alpha = mid.crop((0, 0, 320, 256)).convert("RGBA")
    mid_alpha.putalpha(alpha_image(mid.crop((0, 0, 320, 256))))
    fg_preview.paste(mid_alpha, (0, 0), mid_alpha)
    alpha = fg.crop((0, 0, 320, 256)).convert("RGBA")
    alpha.putalpha(alpha_image(fg.crop((0, 0, 320, 256))))
    fg_preview.paste(alpha, (0, 0), alpha)
    fg_preview.save(LEVELS / "storm-ruins-milestone-preview.png")
    manifest = {
        "world": [WORLD_W, WORLD_H], "tile": TILE, "collision": [WORLD_W // TILE, 14],
        "foreground_palette": FG_PALETTE, "background_palette": BG_PALETTE,
        "sprite_sheet": {
            "size": list(sprites.size), "frame": [48, 48], "frames": 58,
            "depth": 4, "hardware_layout": "three attached sprite pairs",
        },
        "clockwork_beetle": {
            "size": list(beetle.size), "frame": [BEETLE_W, BEETLE_H],
            "frames": BEETLE_FRAMES, "depth": 3,
            "layout": "left-facing column followed by mirrored right-facing column",
        },
    }
    (RUNTIME / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print("Generated Sparkpaw planar runtime assets")


if __name__ == "__main__":
    main()
