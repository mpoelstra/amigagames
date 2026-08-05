#!/usr/bin/env python3
"""Generate a 48x48 source bitmap for the classic Workbench icon."""
import struct
from pathlib import Path

W = H = 48
TRANSPARENT = (255, 0, 255)
BLACK = (10, 10, 18)
WHITE = (245, 245, 235)
ACCENT = (30, 110, 220)
pixels = [[TRANSPARENT for _ in range(W)] for _ in range(H)]


def pixel(x, y, colour):
    if 0 <= x < W and 0 <= y < H:
        pixels[y][x] = colour


def rect(x1, y1, x2, y2, colour):
    for y in range(y1, y2 + 1):
        for x in range(x1, x2 + 1):
            pixel(x, y, colour)


def line(x1, y1, x2, y2, colour):
    dx, sx = abs(x2 - x1), 1 if x1 < x2 else -1
    dy, sy = -abs(y2 - y1), 1 if y1 < y2 else -1
    error = dx + dy
    while True:
        pixel(x1, y1, colour)
        if x1 == x2 and y1 == y2:
            break
        twice = error * 2
        if twice >= dy:
            error += dy
            x1 += sx
        if twice <= dx:
            error += dx
            y1 += sy


# Indoor goal with bright posts and an accent-coloured net.
rect(3, 8, 44, 11, BLACK)
rect(3, 8, 6, 37, BLACK)
rect(41, 8, 44, 37, BLACK)
rect(6, 11, 41, 13, WHITE)
rect(6, 13, 8, 36, WHITE)
rect(39, 13, 41, 36, WHITE)
for x in range(11, 40, 7):
    line(x, 14, x, 34, ACCENT)
for y in range(17, 35, 6):
    line(9, y, 38, y, ACCENT)

# Football in front of the net.
for y in range(21, 45):
    for x in range(12, 36):
        dx, dy = x - 23.5, y - 32.5
        distance = dx * dx + dy * dy
        if distance <= 144:
            pixel(x, y, BLACK if distance >= 106 else WHITE)
rect(21, 29, 26, 35, BLACK)
for x1, y1, x2, y2 in (
    (16, 24, 20, 28), (31, 24, 27, 28),
    (14, 34, 20, 34), (33, 34, 27, 34),
    (18, 41, 22, 36), (29, 41, 26, 36),
):
    line(x1, y1, x2, y2, BLACK)


def write_bmp(path):
    row_size = (W * 3 + 3) & ~3
    body = bytearray()
    for row in pixels:
        for red, green, blue in row:
            body += bytes((blue, green, red))
        body += b"\0" * (row_size - W * 3)
    offset = 54
    header = (
        b"BM" + struct.pack("<IHHI", offset + len(body), 0, 0, offset)
        + struct.pack("<IiiHHIIiiII", 40, W, -H, 1, 24, 0, len(body),
                      2835, 2835, 0, 0)
    )
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(header + body)


if __name__ == "__main__":
    output = Path(__file__).resolve().parent.parent / "build/icon/mrdigs-futsal-icon-48.bmp"
    write_bmp(output)
    print(output)
