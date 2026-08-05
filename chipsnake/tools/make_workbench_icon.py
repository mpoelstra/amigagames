#!/usr/bin/env python3
"""Convert a 48x48 24-bit BMP into a classic Amiga Workbench tool icon."""

import struct
import sys
from pathlib import Path


def read_bmp(path):
    data = Path(path).read_bytes()
    if data[:2] != b"BM":
        raise ValueError("input is not a BMP")
    pixel_offset = struct.unpack_from("<I", data, 10)[0]
    width, signed_height = struct.unpack_from("<ii", data, 18)
    depth = struct.unpack_from("<H", data, 28)[0]
    if (width, abs(signed_height), depth) != (48, 48, 24):
        raise ValueError("input must be a 48x48 24-bit BMP")
    stride = (width * 3 + 3) & ~3
    top_down = signed_height < 0
    rows = []
    for y in range(48):
        source_y = y if top_down else 47 - y
        row = []
        start = pixel_offset + source_y * stride
        for x in range(48):
            b, g, r = data[start + x * 3:start + x * 3 + 3]
            row.append((r, g, b))
        rows.append(row)
    return rows


def pen_for(rgb):
    r, g, b = rgb
    # The generated source uses magenta as a removable background.
    if r > 180 and b > 120 and g < 100:
        return 0
    # Classic icons inherit Workbench's four pens. Keep black outlines dark,
    # highlights bright, and the snake/chip body on the accent pen.
    brightness = (r * 30 + g * 59 + b * 11) // 100
    if brightness < 45:
        return 1
    if brightness > 205 and abs(r - g) < 60:
        return 2
    return 3


def image_data(rows):
    words_per_row = 3
    output = bytearray()
    for plane in range(2):
        for row in rows:
            for word_index in range(words_per_row):
                word = 0
                for bit in range(16):
                    x = word_index * 16 + bit
                    if pen_for(row[x]) & (1 << plane):
                        word |= 1 << (15 - bit)
                output += struct.pack(">H", word)
    return bytes(output)


def create_icon(rows):
    width = height = 48
    # DiskObject and Gadget are stored big-endian. Non-zero serialized pointer
    # values tell icon.library which following optional structures are present.
    gadget = struct.pack(
        ">IhhhhHHHIIIiIHI",
        0, 0, 0, width, height,
        0x0004, 0x0001, 0x0001,
        1, 0, 0, 0, 0, 0, 1,
    )
    disk_object = (
        struct.pack(">HH", 0xE310, 1)
        + gadget
        + struct.pack(">BBIIiiIIi", 3, 0, 0, 0, -1, -1, 0, 0, 65536)
    )
    image = struct.pack(
        ">hhhhhIBBI", 0, 0, width, height, 2, 1, 0x03, 0x00, 0
    )
    return disk_object + image + image_data(rows)


def main():
    if len(sys.argv) != 3:
        raise SystemExit("usage: make_workbench_icon.py INPUT.bmp OUTPUT.info")
    Path(sys.argv[2]).write_bytes(create_icon(read_bmp(sys.argv[1])))


if __name__ == "__main__":
    main()
