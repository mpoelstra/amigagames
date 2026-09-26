#!/usr/bin/env python3
"""Render a non-runtime Level 1 rear-animation study from exact SPBM indices."""
from __future__ import annotations

import json
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "assets/concept/level1-rear-ambience-study-v1"
REAR = ROOT / "assets/runtime/storm-rear.spbm"
FRONT = ROOT / "assets/runtime/storm-front.spbm"
CAMERA = 0

BANDS = (
    ((0, 0, 1), (0, 0, 4), (1, 1, 6), (2, 1, 8),
     (4, 2, 9), (6, 4, 11), (9, 6, 12), (13, 11, 14)),
    ((0, 0, 1), (1, 1, 4), (2, 10, 13), (3, 3, 7),
     (4, 3, 9), (6, 5, 10), (9, 7, 12), (12, 10, 13)),
    ((0, 0, 1), (0, 1, 2), (0, 2, 3), (1, 3, 4),
     (1, 4, 4), (2, 5, 5), (4, 6, 6), (6, 8, 9)),
)


def decode(path: Path):
    data = path.read_bytes()
    assert data[:4] == b"SPBM"
    width = int.from_bytes(data[4:6], "big")
    height = int.from_bytes(data[6:8], "big")
    depth = data[8]
    assert not data[9]
    stride = int.from_bytes(data[10:12], "big")
    palette = [tuple(data[12 + i * 3:15 + i * 3]) for i in range(1 << depth)]
    offset = 12 + (1 << depth) * 3
    plane_bytes = stride * height
    assert len(data) == offset + plane_bytes * depth
    pixels = []
    for y in range(height):
        row = []
        for x in range(width):
            at = y * stride + x // 8
            mask = 0x80 >> (x & 7)
            row.append(sum(1 << plane for plane in range(depth)
                           if data[offset + plane * plane_bytes + at] & mask))
        pixels.append(row)
    return pixels, palette


def rear_palette(y):
    if y < 68:
        source, target, step, steps = 0, 0, 0, 1
    elif y < 84:
        source, target, step, steps = 0, 1, min(4, (y - 68) // 4 + 1), 4
    elif y < 139:
        source, target, step, steps = 1, 1, 0, 1
    elif y < 163:
        source, target, step, steps = 1, 2, min(8, (y - 139) // 3 + 1), 8
    else:
        source, target, step, steps = 2, 2, 0, 1
    return [tuple((BANDS[source][pen][component] * (steps - step)
                   + BANDS[target][pen][component] * step) // steps * 17
                  for component in range(3)) for pen in range(8)]


def near_original(pixels, x, y, target, radius=1):
    return any(pixels[ny][nx] == target
               for ny in range(max(0, y - radius), min(len(pixels), y + radius + 1))
               for nx in range(max(0, x - radius), min(len(pixels[0]), x + radius + 1)))


def apply_clouds(original, pixels, phase):
    # Sparse highlights on existing cloud edges, including sites beyond view 0.
    sites = ((24, 24, 76, 52), (105, 17, 155, 39), (268, 20, 316, 48),
             (405, 27, 451, 51), (563, 15, 612, 43), (738, 22, 791, 46),
             (921, 28, 968, 52), (1034, 16, 1086, 41))
    count = 0
    for site, (x0, y0, x1, y1) in enumerate(sites):
        if (site + phase) % 3 == 2:
            continue
        for y in range(y0, y1):
            for x in range(x0, x1):
                value = original[y][x]
                if (value == 6 and near_original(original, x, y, 7, 2)) or (
                        value == 5 and near_original(original, x, y, 6, 1)):
                    if (x * 7 + y * 11 + site * 13 + phase) % 5 == 0:
                        pixels[y][x] = min(7, value + 1)
                        count += 1
    return count


def apply_lightning(original, pixels, phase):
    # Full-frame replacement over the already painted bolt, not a new line.
    if not phase:
        return 0
    count = 0
    for y in range(18, 81):
        for x in range(200, 230):
            value = original[y][x]
            if value not in (4, 5, 6) or not near_original(original, x, y, 7):
                continue
            selector = (x * 17 + y * 7) % 7
            if (phase == 1 and selector < 1) or (phase == 2 and selector < 3):
                pixels[y][x] = min(7, value + 1)
                count += 1
    return count


def apply_crystal(original, pixels, phase):
    if not phase:
        return 0
    count = 0
    for y in range(78, 87):
        for x in range(208, 223):
            if original[y][x] in (1, 3, 4, 5) and near_original(
                    original, x, y, 2, 1):
                if (phase == 2 and (x + y) % 2 == 0) or (phase == 1 and (x + y) % 4 == 0):
                    pixels[y][x] = 2
                    count += 1
    return count


def compose(rear, front, front_palette):
    image = Image.new("RGB", (320, 208))
    out = image.load()
    for y in range(208):
        colors = rear_palette(y)
        for x in range(320):
            front_pen = front[y][CAMERA + x]
            rear_x = (CAMERA >> 2) + x
            out[x, y] = front_palette[front_pen] if front_pen else colors[rear[y][rear_x]]
    return image


def rear_panorama(rear):
    image = Image.new("RGB", (1120, 208))
    out = image.load()
    for y in range(208):
        colors = rear_palette(y)
        for x in range(1120):
            out[x, y] = colors[rear[y][x]]
    return image


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    rear, _ = decode(REAR)
    front, front_palette = decode(FRONT)
    frames = []
    records = []
    states = (("idle", 0, 0, 0), ("clouds", 1, 0, 0),
              ("charge", 1, 1, 0), ("discharge", 2, 2, 2),
              ("afterglow", 0, 0, 1), ("rest", 0, 0, 0))
    for name, cloud_phase, bolt_phase, crystal_phase in states:
        pixels = [row[:] for row in rear]
        counts = {
            "cloud": apply_clouds(rear, pixels, cloud_phase) if cloud_phase else 0,
            "lightning": apply_lightning(rear, pixels, bolt_phase),
            "crystal": apply_crystal(rear, pixels, crystal_phase),
        }
        frame = compose(pixels, front, front_palette)
        frame.save(OUT / f"{name}-native.png")
        frames.append(frame.resize((1280, 832), Image.Resampling.NEAREST))
        records.append({"name": name, "changed_indexed_pixels": counts})
        if name in ("idle", "clouds"):
            rear_panorama(pixels).resize((2240, 416), Image.Resampling.NEAREST).save(
                OUT / f"{name}-panorama-2x.png")
    frames[0].save(OUT / "sequence-4x.gif", save_all=True,
                   append_images=frames[1:], duration=[700, 450, 180, 180, 500, 700], loop=0)
    board = Image.new("RGB", (1280, 416 * 3), (8, 9, 26))
    for index, frame in enumerate(frames):
        # Six 640x416 comparisons, two per row, preserve native nearest-neighbour pixels.
        small = frame.resize((640, 416), Image.Resampling.NEAREST)
        board.paste(small, ((index % 2) * 640, (index // 2) * 416))
    board.save(OUT / "storyboard-2x.png")
    (OUT / "manifest.json").write_text(json.dumps({
        "purpose": "visual study only; no runtime frames or timing acceptance",
        "source": str(REAR.relative_to(ROOT)),
        "camera": CAMERA,
        "index_rectangles": {"lightning": [192, 18, 48, 64],
                             "crystal": [208, 78, 16, 10]},
        "frames": records,
    }, indent=2) + "\n")
    print(OUT)


if __name__ == "__main__":
    main()
