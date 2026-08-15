#!/usr/bin/env python3
"""Generate isolated Strider and exact REAR16 assets for the AGA benchmark."""

from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "assets/enemies/clockwork-storm-strider-premium-color-idle-source-v2.png"
OUTPUT = ROOT / "assets/runtime/renderbench-strider-idle.raw"
REAR_SOURCE = ROOT / "assets/levels/storm-parallax-rear16-preview.png"
REAR_OUTPUT = ROOT / "assets/runtime/renderbench-rear16.raw"
PREVIEW = ROOT / "assets/enemies/clockwork-storm-strider-64x64-aga15-idle-v3.png"
W = H = 64

# Must match rb16's PF1 colours exactly. Pen zero is transparent.
PALETTE = [
    (0, 0, 17), (9, 10, 24), (18, 27, 54), (32, 47, 86),
    (55, 55, 65), (101, 98, 103), (163, 157, 158), (229, 225, 219),
    (67, 29, 100), (112, 45, 157), (166, 77, 218), (210, 139, 246),
    (0, 112, 170), (0, 207, 239), (201, 246, 255), (224, 35, 104),
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

    rear_source = Image.open(REAR_SOURCE).convert("RGB")
    rear_palette = [
        (0, 0, 17), (0, 17, 51), (17, 34, 85), (34, 68, 119),
        (68, 68, 153), (102, 85, 170), (153, 119, 187), (204, 187, 221),
        (0, 29, 43), (0, 48, 58), (13, 67, 69), (31, 86, 82),
        (56, 105, 91), (79, 103, 117), (45, 145, 194), (151, 211, 224),
    ]
    rear = Image.new("P", (672, 256), 0)
    rear.putpalette([v for rgb in rear_palette for v in rgb] + [0] * (768 - 48))
    source_pixels, rear_pixels = rear_source.load(), rear.load()
    for y in range(min(208, rear_source.height)):
        for x in range(672):
            rgb = source_pixels[x, y]
            rear_pixels[x, y] = min(
                range(16),
                key=lambda i: sum((rgb[c] - rear_palette[i][c]) ** 2
                                  for c in range(3)),
            )
    # graphics.library rounds this 672px displayable bitmap from 84 to 88
    # bytes per row on AGA. Match AllocBitMap's measured stride exactly.
    row_bytes = 88
    plane_size = row_bytes * 256
    rear_planes = bytearray(plane_size * 4)
    for y in range(256):
        for x in range(672):
            pen = rear_pixels[x, y]
            at = y * row_bytes + (x >> 3)
            bit = 0x80 >> (x & 7)
            for plane in range(4):
                if pen & (1 << plane):
                    rear_planes[plane * plane_size + at] |= bit
    REAR_OUTPUT.write_bytes(rear_planes)
    print(f"Generated {REAR_OUTPUT.relative_to(ROOT)} ({len(rear_planes)} bytes)")


if __name__ == "__main__":
    main()
