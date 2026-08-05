#!/usr/bin/env python3
"""Convert a 48x48 24-bit BMP into a classic Amiga Workbench tool icon."""
import struct
import sys
from pathlib import Path


def read_bmp(path):
    data = Path(path).read_bytes()
    if data[:2] != b"BM":
        raise ValueError("input is not a BMP")
    offset = struct.unpack_from("<I", data, 10)[0]
    width, signed_height = struct.unpack_from("<ii", data, 18)
    depth = struct.unpack_from("<H", data, 28)[0]
    if (width, abs(signed_height), depth) != (48, 48, 24):
        raise ValueError("input must be a 48x48 24-bit BMP")
    stride = (width * 3 + 3) & ~3
    rows = []
    for y in range(48):
        source_y = y if signed_height < 0 else 47 - y
        start = offset + source_y * stride
        row = []
        for x in range(48):
            blue, green, red = data[start + x * 3:start + x * 3 + 3]
            row.append((red, green, blue))
        rows.append(row)
    return rows


def pen(rgb):
    red, green, blue = rgb
    if red > 180 and blue > 120 and green < 100:
        return 0
    brightness = (red * 30 + green * 59 + blue * 11) // 100
    if brightness < 45:
        return 1
    if brightness > 205 and abs(red - green) < 60:
        return 2
    return 3


def planar(rows):
    output = bytearray()
    for plane in range(2):
        for row in rows:
            for word_index in range(3):
                word = 0
                for bit in range(16):
                    if pen(row[word_index * 16 + bit]) & (1 << plane):
                        word |= 1 << (15 - bit)
                output += struct.pack(">H", word)
    return bytes(output)


def create_icon(rows):
    gadget = struct.pack(
        ">IhhhhHHHIIIiIHI", 0, 0, 0, 48, 48, 0x0004, 0x0001, 0x0001,
        1, 0, 0, 0, 0, 0, 1,
    )
    disk_object = (
        struct.pack(">HH", 0xE310, 1) + gadget
        + struct.pack(">BBIIiiIIi", 3, 0, 0, 0, -1, -1, 0, 0, 65536)
    )
    image = struct.pack(">hhhhhIBBI", 0, 0, 48, 48, 2, 1, 0x03, 0, 0)
    return disk_object + image + planar(rows)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        raise SystemExit("usage: make_workbench_icon.py INPUT.bmp OUTPUT.info")
    Path(sys.argv[2]).write_bytes(create_icon(read_bmp(sys.argv[1])))
