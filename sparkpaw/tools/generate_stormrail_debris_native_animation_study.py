#!/usr/bin/env python3
"""Build a non-runtime Debris-2 damage and controlled-tumble art study.

Tumble sources are transformed at concept resolution, independently reduced
and then native-cleaned. Finished native pixels are never rotated or scaled.
"""

from pathlib import Path
from PIL import Image

from generate_runtime_assets import FRONT16
from generate_stormrail_debris_native_family import blank_indexed, reduce_component
from generate_stormrail_debris_native_v2_study import INPUT, components, is_background

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "assets/concept/sparkpaw-stormrail-debris-native-animation-v1-aga16.png"
REVIEW = ROOT / "assets/concept/sparkpaw-stormrail-debris-native-animation-v1-review-6x.png"


def masked_crop(source, box):
    crop = source.crop(box)
    rgba = crop.convert("RGBA")
    px = crop.load()
    alpha = Image.new("L", crop.size, 0)
    ap = alpha.load()
    for y in range(crop.height):
        for x in range(crop.width):
            if not is_background(px[x, y]):
                ap[x, y] = 255
    rgba.putalpha(alpha)
    return rgba


def concept_pose(source, box, angle):
    """Return a rotated high-resolution source on magenta for reducer input."""
    crop = masked_crop(source, box)
    side = max(crop.size) + 80
    canvas = Image.new("RGBA", (side, side), (0, 0, 0, 0))
    canvas.alpha_composite(crop, ((side - crop.width) // 2,
                                  (side - crop.height) // 2))
    turned = canvas.rotate(angle, Image.Resampling.BICUBIC, expand=False)
    bounds = turned.getbbox()
    if bounds is None:
        raise RuntimeError("empty transformed concept pose")
    turned = turned.crop(bounds)
    rgb = Image.new("RGB", turned.size, (255, 0, 255))
    rgb.paste(turned.convert("RGB"), (0, 0), turned.getchannel("A"))
    return rgb


def reduce_pose(source, box, angle, size, margin=1):
    pose = concept_pose(source, box, angle)
    return reduce_component(pose, (0, 0, pose.width, pose.height), size, margin)


def damaged(base):
    """Add one broad displaced fracture and silhouette chip at native scale."""
    out = base.copy()
    p = out.load()
    # Remove a small upper-right chip; it changes the mass before the eye reads
    # any colour cue and remains compatible with a generated transparency mask.
    for x, y in ((38, 8), (39, 8), (40, 8), (39, 9), (40, 9),
                 (41, 9), (40, 10), (41, 10)):
        if x < out.width and y < out.height:
            p[x, y] = 0
    # A stepped, material-coloured fracture separates two planes. Deep violet
    # is confined to the recess; warm/cyan pixels are tiny exposed-stormstone
    # catches, not an emissive line running across the whole object.
    fracture = ((27, 11), (27, 12), (26, 13), (26, 14), (27, 15),
                (27, 16), (28, 17), (28, 18), (29, 19), (29, 20),
                (30, 21), (30, 22), (31, 23), (31, 24), (32, 25))
    for x, y in fracture:
        if p[x, y] != 0:
            p[x, y] = 9
    for x, y in ((26, 12), (25, 14), (27, 17), (28, 20), (30, 23)):
        if p[x, y] != 0:
            p[x, y] = 12
    for x, y, pen in ((27, 14, 3), (28, 18, 5), (29, 18, 6)):
        if p[x, y] != 0:
            p[x, y] = pen
    return out


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
        raise RuntimeError("expected four concept components per size row")

    # Contact sheet: large base/damage/tumble; second large base/tumble;
    # medium base/tumble pairs; two small base/tumble pairs.
    sheet = blank_indexed((240, 116))
    large_a = reduce_component(source, large[0][1], (48, 40), 1)
    large_a_turn = reduce_pose(source, large[0][1], 13, (48, 40))
    large_b = reduce_component(source, large[3][1], (48, 40), 1)
    large_b_turn = reduce_pose(source, large[3][1], -11, (48, 40))
    for x, cell in zip((0, 48, 96, 144, 192),
                       (large_a, damaged(large_a), large_a_turn,
                        large_b, large_b_turn)):
        sheet.paste(cell, (x, 0))

    for index, angle in ((0, -12), (2, 14)):
        base = reduce_component(source, medium[index][1], (32, 40), 1)
        turn = reduce_pose(source, medium[index][1], angle, (32, 40))
        x = 16 + (index // 2) * 96
        sheet.paste(base, (x, 48))
        sheet.paste(turn, (x + 40, 48))

    for pair, (index, angle) in enumerate(((0, 18), (2, -17))):
        base = reduce_component(source, small[index][1], (16, 16), 1)
        turn = reduce_pose(source, small[index][1], angle, (16, 16))
        x = 160 + pair * 40
        # Keep the small-pair review row clear of the 40px-high medium cells.
        # The earlier y=68 placement overlapped the second medium pair in the
        # contact sheet even though the exported source cells were independent.
        sheet.paste(base, (x, 96))
        sheet.paste(turn, (x + 18, 96))

    sheet.info["transparency"] = 0
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(OUTPUT)

    rgba = sheet.convert("RGBA")
    alpha = Image.new("L", sheet.size, 0)
    ap = alpha.load(); sp = sheet.load()
    for y in range(sheet.height):
        for x in range(sheet.width):
            if sp[x, y]:
                ap[x, y] = 255
    rgba.putalpha(alpha)
    scaled = rgba.resize((sheet.width * 6, sheet.height * 6),
                         Image.Resampling.NEAREST)
    review = Image.new("RGB", scaled.size, (255, 0, 255))
    review.paste(scaled, (0, 0), scaled.getchannel("A"))
    review.save(REVIEW)
    print(f"wrote {OUTPUT}")
    print(f"wrote {REVIEW}")


if __name__ == "__main__":
    main()
