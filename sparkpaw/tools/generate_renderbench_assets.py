#!/usr/bin/env python3
"""Generate the isolated rb17 premium 15-colour Strider idle proof."""

from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "assets/enemies/clockwork-storm-strider-premium-idle-source.png"
OUTPUT = ROOT / "assets/runtime/renderbench-strider-idle.raw"
PREVIEW = ROOT / "assets/enemies/clockwork-storm-strider-64x64-aga15-idle-v2.png"
W = H = 64

# Must match rb16's PF1 colours exactly. Pen zero is transparent.
PALETTE = [
    (0, 0, 17), (10, 8, 18), (25, 25, 48), (37, 43, 82),
    (49, 61, 119), (58, 81, 164), (62, 112, 204), (51, 204, 238),
    (77, 35, 105), (105, 48, 145), (137, 66, 178), (176, 91, 205),
    (93, 130, 190), (151, 178, 218), (221, 225, 232), (255, 244, 198),
]


def nearest(rgb):
    return min(range(1, 16),
               key=lambda i: sum((rgb[c] - PALETTE[i][c]) ** 2 for c in range(3)))


def main():
    source = Image.open(SOURCE).convert("RGBA")
    # rb17's source is a single chroma-keyed pose, not an animation sheet.
    pixels = source.load()
    for y in range(source.height):
        for x in range(source.width):
            r, g, b, _ = pixels[x, y]
            if g > 150 and g > r * 1.45 and g > b * 1.45:
                pixels[x, y] = (r, g, b, 0)
    cell = source
    bounds = cell.getchannel("A").getbbox()
    if not bounds:
        raise SystemExit("empty Strider idle cell")
    pose = cell.crop(bounds)
    # Keep a transparent guard row below the grounded silhouette. The rb17
    # source used the full final row and left a six-pixel downsampling remnant
    # between the feet that looked like a loose toe in motion.
    scale = min(62 / pose.width, 57 / pose.height)
    pose = pose.resize((round(pose.width * scale), round(pose.height * scale)),
                       Image.Resampling.NEAREST)
    indexed = Image.new("P", (W, H), 0)
    indexed.putpalette([v for rgb in PALETTE for v in rgb] + [0] * (768 - 48))
    canvas = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    canvas.alpha_composite(pose, ((W - pose.width) // 2, H - 1 - pose.height))
    src, dst = canvas.load(), indexed.load()
    for y in range(H):
        for x in range(W):
            r, g, b, a = src[x, y]
            if a >= 96:
                dst[x, y] = nearest((r, g, b))
    # The generated source tapers both feet into three disconnected clusters
    # below their actual soles. At 64x64 these read as loose pixels, not toes.
    # Row 61 contains the last two connected sole shapes; keep rows 62-63 clear
    # and ground row 61 through the benchmark's placement coordinate.
    for y in (62, 63):
        for x in range(W):
            dst[x, y] = 0
    indexed.info["transparency"] = 0
    indexed.save(PREVIEW)

    row_bytes = W // 8
    plane_size = row_bytes * H
    planes = bytearray(plane_size * 4)
    mask = bytearray(plane_size)
    pixels = indexed.load()
    for y in range(H):
        for x in range(W):
            pen = pixels[x, y]
            if not pen:
                continue
            at = y * row_bytes + (x >> 3)
            bit = 0x80 >> (x & 7)
            mask[at] |= bit
            for plane in range(4):
                if pen & (1 << plane):
                    planes[plane * plane_size + at] |= bit
    OUTPUT.write_bytes(planes + mask)
    print(f"Generated {OUTPUT.relative_to(ROOT)} ({len(planes) + len(mask)} bytes)")


if __name__ == "__main__":
    main()
