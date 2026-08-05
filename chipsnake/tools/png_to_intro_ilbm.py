#!/usr/bin/env python3
"""Convert the ChipSnake title concept to a 320x256, 16-colour ILBM."""

import struct
import sys
from pathlib import Path

from PIL import Image


WIDTH = 320
HEIGHT = 256
PALETTE = [
    (0, 0, 17),       (0, 17, 68),      (17, 51, 136),   (34, 102, 221),
    (34, 51, 85),     (85, 51, 34),     (221, 51, 17),   (255, 136, 17),
    (255, 204, 51),   (51, 187, 68),    (102, 153, 51),  (51, 85, 34),
    (119, 119, 153),  (187, 187, 204),  (238, 221, 187), (255, 255, 255),
]


def chunk(chunk_id: bytes, data: bytes) -> bytes:
    return chunk_id + struct.pack(">I", len(data)) + data + (
        b"\0" if len(data) & 1 else b""
    )


def nearest_pen(rgb):
    return min(
        range(16),
        key=lambda pen: sum((rgb[i] - PALETTE[pen][i]) ** 2 for i in range(3)),
    )


def convert(source: Path, output: Path):
    image = Image.open(source).convert("RGB").resize(
        (WIDTH, HEIGHT), Image.Resampling.LANCZOS
    )
    # The concept contains a small blue chevron in its otherwise empty lower
    # area.  Clear that area for the runtime star twinkles and text marquee.
    for y in range(184, HEIGHT):
        for x in range(WIDTH):
            image.putpixel((x, y), PALETTE[0])
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
        + [0] * (768 - 48)
    )
    preview.putdata(pixels)
    preview.save(output.with_suffix(".png"))


if __name__ == "__main__":
    convert(Path(sys.argv[1]), Path(sys.argv[2]))
