#!/usr/bin/env python3
"""Convert concept PNGs into uncompressed 320x216 16-colour ILBM levels."""

import argparse
import struct
from pathlib import Path

from PIL import Image


WIDTH = 320
HEIGHT = 216
GRID_W = 40
GRID_H = 27

# Pens 0-4 are walkable background, 5-11 are solid terrain, and 12-15
# are reserved by the game for the snake, food, head, and status text.
PALETTE = [
    (0, 12, 38),
    (0, 35, 78),
    (10, 53, 105),
    (18, 76, 140),
    (23, 98, 140),
    (55, 75, 105),
    (181, 83, 28),
    (202, 190, 151),
    (180, 35, 35),
    (247, 218, 48),
    (48, 135, 205),
    (242, 125, 42),
    (0, 204, 0),
    (255, 32, 32),
    (96, 255, 128),
    (255, 255, 255),
]

USABLE_PENS = tuple(range(12))


def chunk(chunk_id: bytes, data: bytes) -> bytes:
    padding = b"\0" if len(data) & 1 else b""
    return chunk_id + struct.pack(">I", len(data)) + data + padding


def crop_and_resize(image: Image.Image) -> Image.Image:
    image = image.convert("RGB")
    target_ratio = WIDTH / HEIGHT
    source_ratio = image.width / image.height
    if source_ratio > target_ratio:
        new_width = round(image.height * target_ratio)
        left = (image.width - new_width) // 2
        image = image.crop((left, 0, left + new_width, image.height))
    else:
        new_height = round(image.width / target_ratio)
        top = (image.height - new_height) // 2
        image = image.crop((0, top, image.width, top + new_height))
    return image.resize((WIDTH, HEIGHT), Image.Resampling.LANCZOS)


def nearest_pen(rgb):
    best_pen = 4
    best_distance = None
    for pen in USABLE_PENS:
        color = PALETTE[pen]
        distance = sum((rgb[i] - color[i]) ** 2 for i in range(3))
        if best_distance is None or distance < best_distance:
            best_distance = distance
            best_pen = pen
    return best_pen


def quantize(image: Image.Image) -> list[int]:
    # Cache repeated colours after resizing; this is both deterministic and
    # avoids allowing the image to consume the reserved gameplay pens.
    cache = {}
    pixels = []
    for rgb in image.getdata():
        pen = cache.get(rgb)
        if pen is None:
            pen = nearest_pen(rgb)
            cache[rgb] = pen
        pixels.append(pen)
    return pixels


def build_mask(pixels: list[int]) -> bytes:
    mask = bytearray(GRID_W * GRID_H)
    for grid_y in range(GRID_H):
        for grid_x in range(GRID_W):
            solid_pixels = 0
            for yy in range(grid_y * 8 + 2, grid_y * 8 + 6):
                for xx in range(grid_x * 8 + 2, grid_x * 8 + 6):
                    pen = pixels[yy * WIDTH + xx]
                    if 5 <= pen <= 11:
                        solid_pixels += 1
            if solid_pixels >= 4:
                mask[grid_y * GRID_W + grid_x] = 1

    # The initial snake occupies x=16..20, y=13.
    for x in range(15, 22):
        mask[13 * GRID_W + x] = 0
    return bytes(mask)


def build_body(pixels: list[int]) -> bytes:
    body = bytearray()
    row_bytes = WIDTH // 8
    for y in range(HEIGHT):
        row = pixels[y * WIDTH:(y + 1) * WIDTH]
        for plane in range(4):
            for byte_x in range(row_bytes):
                value = 0
                for bit in range(8):
                    pen = row[byte_x * 8 + bit]
                    value |= ((pen >> plane) & 1) << (7 - bit)
                body.append(value)
    return bytes(body)


def convert(source: Path, output: Path):
    image = crop_and_resize(Image.open(source))
    pixels = quantize(image)

    bmhd = struct.pack(
        ">HHhhBBBBHBBhh",
        WIDTH, HEIGHT, 0, 0, 4, 0, 0, 0, 0, 10, 11, WIDTH, HEIGHT,
    )
    cmap = b"".join(bytes(color) for color in PALETTE)
    body = build_body(pixels)
    snkm = build_mask(pixels)

    form_data = b"ILBM" + chunk(b"BMHD", bmhd)
    form_data += chunk(b"CMAP", cmap)
    form_data += chunk(b"BODY", body)
    form_data += chunk(b"SNKM", snkm)

    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(b"FORM" + struct.pack(">I", len(form_data)) + form_data)

    preview = Image.new("P", (WIDTH, HEIGHT))
    flat_palette = [component for color in PALETTE for component in color]
    preview.putpalette(flat_palette + [0] * (768 - len(flat_palette)))
    preview.putdata(pixels)
    preview.save(output.with_suffix(".png"))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    convert(args.source, args.output)


if __name__ == "__main__":
    main()
