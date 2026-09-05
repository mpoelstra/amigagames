#!/usr/bin/env python3
"""Create hardware-honest 3-plane studies from the Stormrail route concept."""

from pathlib import Path

from PIL import Image, ImageDraw


ROOT = Path(__file__).resolve().parents[1]
CONCEPT = ROOT / "assets" / "concept"
SOURCE = CONCEPT / "sparkpaw-stormrail-route-spans-concept-v2.png"
CRAFT_SOURCE = CONCEPT / "sparkpaw-stormrail-runtime-family-v7-aga16.png"
NAMES = ("departure", "rain-valley", "waterfall-basin", "rail-citadel")
CENTRES = (300, 820, 1370, 1900)
SCREEN_W, SCREEN_H = 320, 208

# Semantic pen order remains dark-to-light across all anchors so Copper
# interpolation never swaps a cloud highlight into a forest-shadow role.
REGION_BANKS_12 = {
    "departure": (
        ((0,0,1),(1,1,3),(3,2,6),(5,3,8),(3,5,6),(6,5,9),(10,7,12),(14,11,15)),
        ((0,0,1),(1,1,3),(2,3,5),(4,3,7),(2,5,5),(5,6,8),(9,8,11),(12,11,14)),
        ((0,0,1),(0,1,2),(1,2,3),(2,3,4),(2,4,3),(4,5,5),(7,6,8),(10,9,11))),
    "rain-valley": (
        ((0,0,1),(1,2,3),(2,4,5),(3,5,7),(4,6,6),(6,7,8),(8,9,10),(11,12,13)),
        ((0,0,1),(1,2,3),(2,4,5),(3,5,6),(4,6,6),(6,8,8),(9,10,11),(12,13,13)),
        ((0,0,1),(0,1,2),(1,3,3),(2,4,4),(3,5,4),(5,6,6),(7,8,8),(10,11,11))),
    "waterfall-basin": (
        ((0,0,1),(1,2,3),(3,3,5),(4,5,7),(5,6,7),(7,8,9),(10,11,12),(13,14,15)),
        ((0,0,1),(1,2,3),(2,4,5),(4,5,6),(5,7,7),(7,9,9),(10,12,12),(14,15,15)),
        ((0,0,1),(0,1,2),(1,3,3),(2,4,4),(3,5,4),(5,7,6),(8,9,8),(11,12,11))),
    "rail-citadel": (
        ((0,0,1),(0,2,3),(1,3,5),(2,5,7),(2,6,7),(4,8,9),(7,11,12),(10,14,15)),
        ((0,0,1),(0,2,3),(1,4,5),(2,5,6),(3,7,7),(5,9,9),(8,12,12),(11,15,15)),
        ((0,0,1),(0,1,2),(0,3,3),(1,4,4),(2,5,4),(3,7,6),(6,9,8),(9,12,11))),
}


def amiga12(rgb):
    return tuple(round(channel / 17) * 17 for channel in rgb)


def luminance(rgb):
    return rgb[0] * 3 + rgb[1] * 6 + rgb[2]


def anchor_palette(image, top, bottom):
    sample = image.crop((0, top, SCREEN_W, bottom))
    reduced = sample.quantize(colors=7, method=Image.Quantize.MEDIANCUT,
                              dither=Image.Dither.NONE)
    raw = reduced.getpalette()
    colours = {amiga12(tuple(raw[i:i + 3])) for i in range(0, 21, 3)}
    colours.discard((0, 0, 0))
    while len(colours) < 7:
        colours.add((17 * len(colours), 17 * len(colours),
                     17 * len(colours)))
    return [(0, 0, 0)] + sorted(colours, key=luminance)[:7]


def interpolate(a, b, amount):
    return [tuple(round((a[i][c] * (1 - amount) + b[i][c] * amount) / 17) * 17
                  for c in range(3)) for i in range(8)]


def palette_for_y(anchors, y):
    # Sixteen effective Copper palette stages: enough to eliminate visible
    # horizontal colour bands without pretending that REAR8 has true RGB.
    step = min(15, (y * 16) // SCREEN_H)
    at = step / 15
    if at < .5:
        return interpolate(anchors[0], anchors[1], at * 2)
    return interpolate(anchors[1], anchors[2], (at - .5) * 2)


def nearest(rgb, palette):
    return min(range(8), key=lambda i: sum((rgb[c] - palette[i][c]) ** 2
                                           for c in range(3)))


def native_view(source, centre, name, banks12=None):
    crop_w, crop_h = 768, 500
    left = max(0, min(source.width - crop_w, centre - crop_w // 2))
    top = 82
    view = source.crop((left, top, left + crop_w, top + crop_h)).resize(
        (SCREEN_W, SCREEN_H), Image.Resampling.LANCZOS)
    anchors = tuple([tuple(channel * 17 for channel in colour)
                     for colour in bank]
                    for bank in (banks12 or REGION_BANKS_12[name]))
    result = Image.new("RGB", view.size)
    src, dst = view.load(), result.load()
    for y in range(SCREEN_H):
        palette = palette_for_y(anchors, y)
        for x in range(SCREEN_W):
            dst[x, y] = palette[nearest(src[x, y], palette)]
    return result, anchors


def main():
    source = Image.open(SOURCE).convert("RGB")
    family = Image.open(CRAFT_SOURCE)
    craft = family.crop((448, 42, 560, 88)).convert("RGBA")
    alpha = Image.new("L", craft.size, 255)
    source_indices = family.crop((448, 42, 560, 88))
    alpha.putdata([0 if pen == 0 else 255 for pen in source_indices.getdata()])
    craft.putalpha(alpha)
    views = []
    for name, centre in zip(NAMES, CENTRES):
        view, anchors = native_view(source, centre, name)
        view.save(CONCEPT / f"sparkpaw-stormrail-{name}-aga8-study-v3.png")
        views.append(view)

    sheet = Image.new("RGB", (SCREEN_W * 2, SCREEN_H * 2 + 28), (3, 4, 10))
    draw = ImageDraw.Draw(sheet)
    for i, (name, view) in enumerate(zip(NAMES, views)):
        x = (i & 1) * SCREEN_W
        y = (i >> 1) * (SCREEN_H + 14)
        sheet.paste(view, (x, y))
        draw.text((x + 5, y + SCREEN_H + 2), name.upper(), fill=(221, 211, 174))
    sheet.resize((sheet.width * 2, sheet.height * 2),
                 Image.Resampling.NEAREST).save(
        CONCEPT / "sparkpaw-stormrail-route-spans-aga8-study-v3.png")
    combat = Image.new("RGB", (SCREEN_W * 2, SCREEN_H * 2), (0, 0, 0))
    for i, view in enumerate(views):
        composed = view.convert("RGBA")
        composed.alpha_composite(craft, (42, 72))
        combat.paste(composed.convert("RGB"),
                     ((i & 1) * SCREEN_W, (i >> 1) * SCREEN_H))
    combat.resize((combat.width * 2, combat.height * 2),
                  Image.Resampling.NEAREST).save(
        CONCEPT / "sparkpaw-stormrail-route-combat-readability-aga8-v1.png")
    joins = Image.new("RGB", (SCREEN_W * 3, SCREEN_H), (0, 0, 0))
    for i in range(3):
        left, right = NAMES[i], NAMES[i + 1]
        blended = tuple(tuple(tuple((a + b) // 2 for a, b in zip(ca, cb))
                              for ca, cb in zip(ba, bb))
                        for ba, bb in zip(REGION_BANKS_12[left],
                                          REGION_BANKS_12[right]))
        centre = (CENTRES[i] + CENTRES[i + 1]) // 2
        join, _ = native_view(source, centre, left, blended)
        joins.paste(join, (i * SCREEN_W, 0))
    joins.resize((joins.width * 2, joins.height * 2),
                 Image.Resampling.NEAREST).save(
        CONCEPT / "sparkpaw-stormrail-route-joins-aga8-study-v1.png")
    print("generated four 320x208 REAR8 route-span studies")


if __name__ == "__main__":
    main()
