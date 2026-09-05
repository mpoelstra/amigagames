#!/usr/bin/env python3
"""Keep the compact Stormrail master, indexed pixels and SPBM mask identical."""

from pathlib import Path
import struct

from PIL import Image

root = Path(__file__).resolve().parents[1]
master = Image.open(root / "assets/sprites/stormrail-compact-flight-aga16-v2.png").convert("RGBA")
preview = Image.open(root / "assets/concept/sparkpaw-stormrail-runtime-family-v8-aga16.png")
data = (root / "assets/runtime/stormrail-family.spbm").read_bytes()

assert master.size == (80, 32)
alpha_histogram = master.getchannel("A").histogram()
assert not any(alpha_histogram[1:255])
assert preview.size == (640, 120)
assert data[:4] == b"SPBM"
width, height, depth, masked, row_bytes = struct.unpack(">HHBBH", data[4:12])
assert (width, height, depth, masked, row_bytes) == (640, 120, 4, 1, 80)

palette_bytes = (1 << depth) * 3
plane_bytes = row_bytes * height
assert len(data) == 12 + palette_bytes + (depth + 1) * plane_bytes
mask = data[-plane_bytes:]

opaque = master.getchannel("A")
for y in range(32):
    for x in range(80):
        sheet_x = 560 + x
        mask_on = bool(mask[y * row_bytes + (sheet_x >> 3)] &
                       (0x80 >> (sheet_x & 7)))
        pixel_on = preview.getpixel((sheet_x, y)) != 0
        alpha_on = opaque.getpixel((x, y)) == 255
        assert mask_on == pixel_on == alpha_on, (x, y)

print("PASS: compact Stormrail alpha, indexed pixels and mask are identical")
