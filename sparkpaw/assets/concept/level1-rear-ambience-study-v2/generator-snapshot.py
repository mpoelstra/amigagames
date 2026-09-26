#!/usr/bin/env python3
"""Render a non-runtime Level 1 rear-animation study from exact SPBM indices."""
from __future__ import annotations

import json
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "assets/concept/level1-rear-ambience-study-v2"
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
    # V1's scattered edge brightening was rejected. Keep the clouds intact
    # while reviewing a much narrower, contour-preserving tower treatment.
    return 0


def apply_lightning(original, pixels, phase):
    # Authored points INSIDE the existing bright bolt. Never dilate its edge
    # or touch the surrounding vortex. A small fall in intensity followed by
    # the original image gives a restrained pulse using only existing pens.
    if not phase:
        return 0
    core = ((215, 35), (214, 36), (213, 38),
            (218, 51), (219, 52), (220, 53),
            (218, 59), (217, 60), (216, 61))
    selected = core if phase == 2 else core[::3]
    for x, y in selected:
        assert original[y][x] == 7
        pixels[y][x] = 6
    return len(selected)


def apply_crystal(original, pixels, phase):
    if not phase:
        return 0
    # Shade only the existing lower facet, then restore its original cyan.
    # The 12-pixel crystal never gains a halo or changes its outline.
    points = ((213, 84), (214, 84)) if phase == 2 else ((213, 84),)
    for x, y in points:
        assert original[y][x] == 2
        pixels[y][x] = 3
    return len(points)


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
    states = (("idle", 0, 0, 0), ("soften", 0, 1, 0),
              ("low", 0, 2, 0), ("recover", 0, 1, 0),
              ("crystal", 0, 0, 1), ("rest", 0, 0, 0))
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
        if name == "idle":
            rear_panorama(pixels).resize((2240, 416), Image.Resampling.NEAREST).save(
                OUT / f"{name}-panorama-2x.png")
    frames[0].save(OUT / "sequence-4x.gif", save_all=True,
                   append_images=frames[1:], duration=[4200, 220, 260, 220, 180, 2000], loop=0)
    details = [f.crop((192*4, 20*4, 240*4, 96*4)).resize(
        (384, 608), Image.Resampling.NEAREST) for f in frames]
    details[0].save(OUT / "tower-detail-8x.gif", save_all=True,
                   append_images=details[1:], duration=[4200, 220, 260, 220, 180, 2000], loop=0)
    board = Image.new("RGB", (1280, 416 * 3), (8, 9, 26))
    for index, frame in enumerate(frames):
        # Six 640x416 comparisons, two per row, preserve native nearest-neighbour pixels.
        small = frame.resize((640, 416), Image.Resampling.NEAREST)
        board.paste(small, ((index % 2) * 640, (index // 2) * 416))
    board.save(OUT / "storyboard-2x.png")
    (OUT / "manifest.json").write_text(json.dumps({
        "purpose": "v2 visual review: preserved bolt contour, nine existing core pixels maximum; no native acceptance",
        "source": str(REAR.relative_to(ROOT)),
        "camera": CAMERA,
        "combined_patch": [208, 35, 16, 50],
        "planar_bytes_per_phase": 300,
        "unique_states": 4,
        "clouds": "static in this revision; full-level ambience remains deferred",
        "frames": records,
    }, indent=2) + "\n")
    print(OUT)


if __name__ == "__main__":
    main()
