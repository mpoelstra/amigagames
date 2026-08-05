#!/usr/bin/env python3
"""Convert the Hall of Fame concept into a text-free 320x256 ILBM."""

import struct
import sys
from pathlib import Path

from PIL import Image


WIDTH, HEIGHT = 320, 256
PALETTE = [
    (0, 0, 17),       (8, 12, 28),      (18, 25, 48),     (35, 45, 75),
    (63, 72, 105),    (111, 116, 148),  (172, 174, 195),  (238, 238, 244),
    (67, 91, 31),     (101, 132, 45),   (139, 163, 70),   (43, 62, 26),
    (120, 53, 20),    (205, 105, 40),   (242, 164, 72),   (255, 214, 132),
]


def chunk(identifier, data):
    return identifier + struct.pack(">I", len(data)) + data + (
        b"\0" if len(data) & 1 else b""
    )


def nearest_pen(rgb):
    return min(
        range(16),
        key=lambda pen: sum((rgb[i] - PALETTE[pen][i]) ** 2 for i in range(3)),
    )


def convert(source, output):
    image = Image.open(source).convert("RGB").resize(
        (WIDTH, HEIGHT), Image.Resampling.LANCZOS
    )

    # Remove the generated sample names and scores. The game renders the real
    # persistent table here with its own compact 5x7 font.
    for y in range(78, 207):
        for x in range(88, 233):
            shade = PALETTE[1] if ((x // 2 + y // 2) & 7) else PALETTE[2]
            image.putpixel((x, y), shade)

    cache = {}
    pixels = []
    for rgb in image.getdata():
        if rgb not in cache:
            cache[rgb] = nearest_pen(rgb)
        pixels.append(cache[rgb])

    body = bytearray()
    for y in range(HEIGHT):
        row = pixels[y * WIDTH:(y + 1) * WIDTH]
        for plane in range(4):
            for byte_x in range(WIDTH // 8):
                value = 0
                for bit in range(8):
                    value |= ((row[byte_x * 8 + bit] >> plane) & 1) << (7 - bit)
                body.append(value)

    bmhd = struct.pack(
        ">HHhhBBBBHBBhh",
        WIDTH, HEIGHT, 0, 0, 4, 0, 0, 0, 0, 10, 11, WIDTH, HEIGHT,
    )
    cmap = b"".join(bytes(color) for color in PALETTE)
    form = b"ILBM" + chunk(b"BMHD", bmhd) + chunk(b"CMAP", cmap)
    form += chunk(b"BODY", bytes(body))
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(b"FORM" + struct.pack(">I", len(form)) + form)

    preview = Image.new("P", (WIDTH, HEIGHT))
    preview.putpalette(
        [component for color in PALETTE for component in color]
        + [0] * (768 - len(PALETTE) * 3)
    )
    preview.putdata(pixels)
    preview.save(output.with_suffix(".png"))


if __name__ == "__main__":
    convert(Path(sys.argv[1]), Path(sys.argv[2]))
