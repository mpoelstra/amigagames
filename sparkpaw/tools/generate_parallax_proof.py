#!/usr/bin/env python3
"""Generate exact native REAR8 and feasible REAR16 parallax previews."""

from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "assets/concept/sparkpaw-parallax-master-concept-v2.png"
OUT8 = ROOT / "assets/levels/storm-parallax-rear8-preview.png"
OUT16 = ROOT / "assets/levels/storm-parallax-rear16-preview.png"
COMPARE = ROOT / "assets/levels/storm-parallax-rear8-vs-rear16.png"
W, H = 1024, 208

REAR8 = [
    (0, 0, 17), (0, 17, 51), (17, 34, 85), (34, 68, 119),
    (68, 68, 153), (102, 85, 170), (153, 119, 187), (204, 187, 221),
]
REAR16 = REAR8 + [
    (0, 29, 43), (0, 48, 58), (13, 67, 69), (31, 86, 82),
    (56, 105, 91), (79, 103, 117), (45, 145, 194), (151, 211, 224),
]


def remap(source, palette):
    result = Image.new("RGB", source.size)
    src, dst = source.load(), result.load()
    for y in range(source.height):
        for x in range(source.width):
            rgb = src[x, y]
            dst[x, y] = min(
                palette,
                key=lambda color: sum((rgb[c] - color[c]) ** 2
                                      for c in range(3)),
            )
    return result


def rms(source, candidate):
    a, b = source.load(), candidate.load()
    total = 0
    for y in range(H):
        for x in range(W):
            total += sum((a[x, y][c] - b[x, y][c]) ** 2 for c in range(3))
    return (total / (W * H * 3)) ** 0.5


def main():
    source = Image.open(SOURCE).convert("RGB")
    crop_h = round(source.width * H / W)
    top = 0
    source = source.crop((0, top, source.width, top + crop_h))
    source = source.resize((W, H), Image.Resampling.LANCZOS)
    rear8 = remap(source, REAR8)
    rear16 = remap(source, REAR16)
    rear8.save(OUT8)
    rear16.save(OUT16)
    compare = Image.new("RGB", (W * 2 + 2, H), (0, 0, 0))
    compare.paste(rear8, (0, 0))
    compare.paste(rear16, (W + 2, 0))
    compare.save(COMPARE)
    print(f"REAR8 RMS={rms(source, rear8):.2f}")
    print(f"REAR16 RMS={rms(source, rear16):.2f}")
    print(f"Wrote {COMPARE.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
