#!/usr/bin/env python3
"""Prepare AGA-oriented concept previews, tiles and prototype sprites."""

import json
from pathlib import Path
from PIL import Image, ImageDraw, ImageOps

ROOT = Path(__file__).resolve().parent.parent

FRONT = [
    (255, 0, 255), (14, 17, 30), (35, 31, 42), (65, 55, 55),
    (96, 72, 52), (137, 87, 43), (196, 119, 38), (244, 164, 47),
    (255, 207, 91), (230, 225, 190), (17, 57, 75), (16, 105, 111),
    (20, 183, 178), (91, 245, 226), (50, 91, 48), (103, 154, 62),
]
BACK = [
    (7, 11, 28), (18, 24, 57), (30, 40, 83), (49, 52, 105),
    (72, 65, 132), (105, 85, 162), (147, 113, 187), (205, 163, 208),
    (20, 48, 59), (27, 70, 68), (39, 92, 73), (57, 116, 80),
    (78, 139, 91), (66, 118, 139), (96, 157, 178), (160, 213, 221),
]


def nearest(color, palette, start=0):
    r, g, b = color
    return min(range(start, len(palette)),
               key=lambda i: sum((a-b) ** 2 for a, b in zip((r, g, b), palette[i])))


def save_indexed_rgba(source, out, palette, size):
    source = source.convert("RGBA")
    source.thumbnail(size, Image.Resampling.NEAREST)
    canvas = Image.new("P", size, 0)
    flat = [v for rgb in palette for v in rgb] + [0] * (768-len(palette)*3)
    canvas.putpalette(flat)
    ox, oy = (size[0]-source.width)//2, (size[1]-source.height)//2
    src = source.load(); dst = canvas.load()
    for y in range(source.height):
        for x in range(source.width):
            if src[x, y][3] >= 80:
                dst[ox+x, oy+y] = nearest(src[x, y][:3], palette, 1)
    canvas.info["transparency"] = 0
    canvas.save(out, optimize=False)


def concepts():
    concept_dir = ROOT / "assets" / "concept"
    for name in ("sparkpaw-title-concept", "sparkpaw-gameplay-concept"):
        source = Image.open(concept_dir / f"{name}.png").convert("RGB")
        fitted = ImageOps.fit(source, (320, 256), Image.Resampling.LANCZOS)
        fitted.quantize(colors=64, method=Image.Quantize.MEDIANCUT,
                        dither=Image.Dither.FLOYDSTEINBERG).save(
            concept_dir / f"{name}-aga64-preview.png")


def sprites():
    source = Image.open(ROOT / "assets" / "sprites" /
                        "sparkpaw-sprites-reference-transparent.png").convert("RGBA")
    sheet = Image.new("P", (4*32, 4*40), 0)
    sheet.putpalette([v for rgb in FRONT for v in rgb] + [0] * (768-48))
    names = [
        "idle", "blink", "run-contact", "run-passing",
        "run-airborne", "run-recovery", "jump-rise", "jump-apex",
        "fall", "land", "crouch", "aim", "shoot", "hurt",
        "victory", "death",
    ]
    frames = ROOT / "assets" / "sprites" / "frames"
    frames.mkdir(exist_ok=True)
    cw, ch = source.width//4, source.height//4
    for index, name in enumerate(names):
        cell = source.crop(((index%4)*cw, (index//4)*ch,
                            (index%4+1)*cw, (index//4+1)*ch))
        box = cell.getchannel("A").getbbox()
        if box:
            cell = cell.crop(box)
        cell.thumbnail((28, 36), Image.Resampling.NEAREST)
        frame_path = frames / f"{index:02d}-{name}.png"
        save_indexed_rgba(cell, frame_path, FRONT, (32, 40))
        frame = Image.open(frame_path)
        sheet.paste(frame, ((index%4)*32, (index//4)*40))
    sheet.info["transparency"] = 0
    sheet.save(ROOT / "assets" / "sprites" / "sparkpaw-32x40-aga16.png")
    (ROOT / "assets" / "sprites" / "animations.json").write_text(
        json.dumps({"cell_width": 32, "cell_height": 40,
                    "frames": names,
                    "prototype_note": "Reference frames require final pixel cleanup."},
                   indent=2) + "\n")


def tiles():
    out = Image.new("P", (128, 64), 0)
    out.putpalette([v for rgb in FRONT for v in rgb] + [0] * (768-48))
    d = ImageDraw.Draw(out)
    def tile(n):
        return (n % 8 * 16, n // 8 * 16)
    # 0 transparent; 1-7 rock/platform variants.
    for n in range(1, 8):
        x, y = tile(n); d.rectangle((x, y, x+15, y+15), fill=2)
        d.rectangle((x+1, y+1, x+14, y+14), fill=3)
        d.line((x+2, y+3, x+13, y+3), fill=4)
        d.line((x+3, y+12, x+12, y+12), fill=1)
        if n & 1: d.line((x+8, y+4, x+5, y+11), fill=2)
        if n & 2: d.line((x+10, y+5, x+13, y+9), fill=5)
    # Moss top, technological block, energy conduit, cracked block.
    x, y = tile(8); d.rectangle((x, y+5, x+15, y+15), fill=3)
    d.rectangle((x, y+3, x+15, y+6), fill=15); d.point((x+3, y+7), fill=14)
    x, y = tile(9); d.rectangle((x, y, x+15, y+15), fill=1)
    d.rectangle((x+2, y+2, x+13, y+13), outline=10); d.rectangle((x+6, y+6, x+9, y+9), fill=12)
    x, y = tile(10); d.rectangle((x, y, x+15, y+15), fill=2)
    d.line((x, y+8, x+5, y+8, x+7, y+4, x+9, y+12, x+11, y+8, x+15, y+8), fill=13, width=2)
    x, y = tile(11); d.rectangle((x, y, x+15, y+15), fill=3)
    d.line((x+3, y+2, x+8, y+7, x+5, y+13), fill=1); d.line((x+8, y+7, x+13, y+4), fill=1)
    # Spike, stormstone collectible, spring pad, checkpoint.
    x, y = tile(12)
    for q in range(0, 16, 5): d.polygon((x+q, y+15, x+q+2, y+5, x+q+5, y+15), fill=9)
    x, y = tile(13); d.polygon((x+8, y+1, x+14, y+8, x+8, y+15, x+2, y+8), fill=13); d.polygon((x+8, y+4, x+11, y+8, x+8, y+12, x+5, y+8), fill=12)
    x, y = tile(14); d.rectangle((x+1, y+11, x+14, y+14), fill=6); d.line((x+3, y+10, x+6, y+6, x+9, y+10, x+12, y+6), fill=8, width=2)
    x, y = tile(15); d.rectangle((x+7, y+3, x+9, y+15), fill=9); d.polygon((x+8, y, x+14, y+5, x+8, y+10, x+2, y+5), fill=12)
    # Remaining tiles are decorative background-compatible foreground pieces.
    for n in range(16, 32):
        x, y = tile(n)
        if n & 1: d.line((x, y+15, x+15, y), fill=10)
        if n & 2: d.line((x, y, x+15, y+15), fill=11)
        if n & 4: d.rectangle((x+6, y+6, x+9, y+9), fill=12)
        if n & 8: d.rectangle((x, y+13, x+15, y+15), fill=14)
    out.info["transparency"] = 0
    out.save(ROOT / "assets" / "tiles" / "storm-ruins-16x16-aga16.png")


def palettes():
    palette_dir = ROOT / "assets" / "palette"
    payload = {"foreground_transparent_index": 0,
               "foreground": FRONT, "background": BACK,
               "target": "AGA dual playfield, 4 bitplanes per playfield"}
    (palette_dir / "storm-ruins-aga.json").write_text(json.dumps(payload, indent=2)+"\n")
    lines = ["GIMP Palette", "Name: Sparkpaw Storm Ruins", "Columns: 8", "#"]
    for index, color in enumerate(FRONT+BACK):
        lines.append(f"{color[0]:3d} {color[1]:3d} {color[2]:3d}\tSparkpaw {index:02d}")
    (palette_dir / "storm-ruins-aga.gpl").write_text("\n".join(lines)+"\n")


if __name__ == "__main__":
    concepts(); sprites(); tiles(); palettes()
    print("Prepared AGA concept previews, sprite sheet, tiles and palettes")
