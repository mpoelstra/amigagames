#!/usr/bin/env python3
"""Generate a native 4+3 proof and hypothetical 32-colour PF1 art bound."""

from pathlib import Path
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "assets/concept/sparkpaw-visual-slice-concept-v2.png"
OUT_DIR = ROOT / "assets/levels"
OUT_43 = OUT_DIR / "storm-visual-slice-aga-4plus3-preview.png"
OUT_53 = OUT_DIR / "storm-visual-slice-32color-pf1-upper-bound.png"
OUT_COMPARE = OUT_DIR / "storm-visual-slice-aga-comparison.png"
OUT_WATER_43 = OUT_DIR / "storm-water-concept-4plus3-animation.png"
OUT_WATER_53 = OUT_DIR / "storm-water-concept-32color-upper-bound.png"
OUT_WATER_COMPARE = OUT_DIR / "storm-water-concept-aga-comparison.png"
REPORT = ROOT / "docs/VISUAL_SLICE_PROOF.md"
W, H = 320, 208

# Production banks from generate_runtime_assets.py. Keep the 4+3 control exact.
FRONT16 = [
    (0, 0, 17), (17, 17, 17), (221, 68, 17), (255, 153, 34),
    (255, 238, 170), (34, 102, 204), (51, 204, 238), (153, 68, 204),
    (55, 55, 65), (101, 98, 103), (163, 157, 158), (229, 225, 219),
    (67, 29, 100), (112, 45, 157), (166, 77, 218), (224, 35, 104),
]
REAR8 = [
    (0, 0, 17), (0, 17, 51), (17, 34, 85), (34, 68, 119),
    (68, 68, 153), (102, 85, 170), (153, 119, 187), (204, 187, 221),
]
# Candidate-only additions spend the extra PF1 bank on water depth, cyan foam,
# neutral ruin modelling and restrained moss rather than duplicate blue shades.
FRONT32 = FRONT16 + [
    (0, 29, 72), (0, 55, 125), (0, 85, 183), (0, 126, 226),
    (35, 171, 246), (139, 226, 255), (216, 248, 255), (30, 38, 57),
    (72, 82, 99), (124, 128, 137), (184, 181, 174), (53, 72, 46),
    (82, 111, 52), (121, 153, 61), (33, 91, 109), (82, 150, 160),
]
WATER4 = [FRONT16[0], FRONT16[1], FRONT16[5], FRONT16[6], FRONT16[11]]


def remap_fixed(image, palette, avoid_zero=False):
    source = image.load()
    output = Image.new("RGB", image.size)
    target = output.load()
    start = 1 if avoid_zero else 0
    for y in range(image.height):
        for x in range(image.width):
            rgb = source[x, y]
            index = min(
                range(start, len(palette)),
                key=lambda i: sum((rgb[c] - palette[i][c]) ** 2
                                  for c in range(3)),
            )
            target[x, y] = palette[index]
    return output


def error(reference, candidate, mask=None):
    ref = reference.load()
    got = candidate.load()
    selected = mask.load() if mask else None
    total = count = 0
    for y in range(H):
        for x in range(W):
            if selected is not None and not selected[x, y]:
                continue
            total += sum((ref[x, y][c] - got[x, y][c]) ** 2 for c in range(3))
            count += 3
    return (total / count) ** 0.5 if count else 0.0


def native_source():
    source = Image.open(SOURCE).convert("RGB")
    # Crop the accepted low-ground composition so its walkable lip maps to
    # native y=200 and only eight compact material rows remain above the HUD.
    crop_h = round(source.height * 0.924)
    crop_w = round(crop_h * W / H)
    left = (source.width - crop_w) // 2
    cropped = source.crop((left, 0, left + crop_w, crop_h))
    return cropped.resize((W, H), Image.Resampling.LANCZOS)


def foreground_mask():
    mask = Image.new("1", (W, H), 0)
    draw = ImageDraw.Draw(mask)
    # Ground cap/banks: exact production contact at y=200.
    draw.rectangle((0, 198, W - 1, H - 1), fill=1)
    # Preserve the approved raised ruin as foreground without converting the
    # complete parallax painting into PF1.
    draw.polygon(
        [(232, 130), (289, 130), (289, 143), (282, 147),
         (282, 198), (250, 198), (250, 146), (232, 141)],
        fill=1,
    )
    return mask


def compose(reference, front_colors):
    mask = foreground_mask()
    rear = remap_fixed(reference, REAR8)
    front_palette = FRONT16 if front_colors == 16 else FRONT32
    front = remap_fixed(reference, front_palette, avoid_zero=True)
    combined = rear.copy()
    combined.paste(front, (0, 0), mask)
    return combined, mask


def water_frame(frame):
    image = Image.new("RGB", (80, 11), FRONT16[0])
    pixels = image.load()
    curve = (1, 1, 0, 0, 1, 2, 2, 1, 0, 0, 1, 1, 2, 1, 0, 1)
    bubbles = ((7, 0, 7), (19, 11, 9), (31, 4, 6),
               (46, 15, 10), (60, 7, 8), (72, 13, 7))
    for x in range(80):
        surface = 1 + curve[((x // 2) + frame) & 15]
        if x < 2 or x >= 78:
            surface = 3
        for y in range(surface, 11):
            depth = y - surface
            if depth == 0:
                rgb = (216, 248, 255) if ((x + frame * 3) & 7) < 2 else (35, 171, 246)
            elif depth == 1:
                rgb = (35, 171, 246) if ((x + frame) & 7) else (139, 226, 255)
            elif depth < 5:
                rgb = (0, 126, 226)
            elif depth < 8:
                rgb = (0, 85, 183)
            else:
                rgb = (0, 55, 125)
            pixels[x, y] = rgb
    for i, (base_x, start, life) in enumerate(bubbles):
        age = (frame + 16 - start) & 15
        if age >= life:
            continue
        x = base_x + (1 if ((frame + i) & 3) == 0 else 0)
        y = 10 - (age * 7 // life)
        if 0 <= x < 80 and 3 <= y < 11:
            pixels[x, y] = (216, 248, 255) if i & 1 else (139, 226, 255)
    return image


def water_sheet(palette):
    sheet = Image.new("RGB", (80, 11 * 16), FRONT16[0])
    for frame in range(16):
        source = water_frame(frame)
        mapped = remap_fixed(source, palette, avoid_zero=True)
        # Restore pen-zero air above the varying surface.
        for y in range(11):
            for x in range(80):
                if source.getpixel((x, y)) == FRONT16[0]:
                    mapped.putpixel((x, y), FRONT16[0])
        sheet.paste(mapped, (0, frame * 11))
    return sheet


def main():
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    reference = native_source()
    proof43, mask = compose(reference, 16)
    proof53, _ = compose(reference, 32)
    proof43.save(OUT_43)
    proof53.save(OUT_53)

    compare = Image.new("RGB", (W * 2 + 2, H), (0, 0, 0))
    compare.paste(proof43, (0, 0))
    compare.paste(proof53, (W + 2, 0))
    compare.save(OUT_COMPARE)

    water43 = water_sheet(WATER4).resize((320, 704), Image.Resampling.NEAREST)
    water53 = water_sheet(FRONT32).resize((320, 704), Image.Resampling.NEAREST)
    water43.save(OUT_WATER_43)
    water53.save(OUT_WATER_53)
    water_compare = Image.new("RGB", (642, 704), FRONT16[0])
    water_compare.paste(water43, (0, 0))
    water_compare.paste(water53, (322, 0))
    water_compare.save(OUT_WATER_COMPARE)

    front_pixels = sum(1 for y in range(H) for x in range(W)
                       if mask.getpixel((x, y)))
    rms43 = error(reference, proof43)
    rms53 = error(reference, proof53)
    front43 = error(reference, proof43, mask)
    front53 = error(reference, proof53, mask)
    slice_plane = (W // 8) * H
    world_plane = (2048 // 8) * H
    route_plane = (3072 // 8) * H
    REPORT.write_text(f"""# Phase 6B.6 visual-slice proof

Generated deterministically from `sparkpaw-visual-slice-concept-v2.png`.
The same 320x208 scene, crop, PF1 mask and eight-colour PF2 reduction are used
for both images. The right side changes PF1 from 16 to 32 colours as an art-only
upper bound. It is **not** an implementable AGA 5+3 dual-playfield mode: AGA
alternates odd/even bitplanes and permits at most four planes per playfield.

## Host image comparison

- Native scene: {W}x{H}; foreground mask: {front_pixels} pixels
- 4+3 combined RGB RMS error: {rms43:.2f}
- hypothetical 32-colour PF1 combined RGB RMS error: {rms53:.2f}
- 4-plane PF1 masked RMS error: {front43:.2f}
- hypothetical 32-colour PF1 masked RMS error: {front53:.2f}
- Preview: `assets/levels/storm-visual-slice-aga-comparison.png`
- Matched sixteen-frame water proof:
  `assets/levels/storm-water-concept-aga-comparison.png`

These host colour errors measure palette fidelity only. They do not establish
Amiga frame time, Copper safety, Blitter capacity or subjective visual value.

## Exact extra-plane storage

The rejected hypothetical fifth PF1 plane would add one bit per foreground
pixel if the hardware mode existed:

- one 320x208 slice bitmap: {slice_plane:,} bytes
- one 2048x208 resident bitmap: {world_plane:,} bytes
- one future 3072x208 resident bitmap: {route_plane:,} bytes

Production keeps both a clean source and a displayed foreground representation,
so the 2048px minimum direct increase is {world_plane * 2:,} bytes across those
two full-width representations before Bob caches, water/effect caches, asset
headers or alignment. At 3072px it becomes {route_plane * 2:,} bytes. Exact
Chip/Fast placement must be measured in the Amiga bench rather than inferred.

## Copper and runtime proof still required

AGA's eighth bitplane is PF2's fourth plane, producing 4+4 rather than 5+3.
Therefore do not build or claim a 5+3 benchmark. Production 4+3 remains
authoritative. A future 4+4 comparison may measure rear-playfield colour gains,
but it cannot provide extra foreground/water colours. Pursue concept-style
water first inside FRONT16 using the matched four-colour proof.
""", encoding="utf-8")
    print(f"Wrote {OUT_43.relative_to(ROOT)}")
    print(f"Wrote {OUT_53.relative_to(ROOT)}")
    print(f"Wrote {OUT_COMPARE.relative_to(ROOT)}")
    print(f"Wrote {OUT_WATER_43.relative_to(ROOT)}")
    print(f"Wrote {OUT_WATER_53.relative_to(ROOT)}")
    print(f"Wrote {OUT_WATER_COMPARE.relative_to(ROOT)}")
    print(f"Wrote {REPORT.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
