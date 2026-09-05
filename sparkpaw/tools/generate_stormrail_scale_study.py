#!/usr/bin/env python3
"""Build a deterministic native-size review of a compact Skimmer concept."""

import argparse
from pathlib import Path
from PIL import Image, ImageDraw

from generate_runtime_assets import FRONT16, nearest_index

ROOT = Path(__file__).resolve().parents[1]
BACKGROUND = ROOT / "assets/concept/sparkpaw-stormrail-rear-aga8-preview-v3.png"
DEFAULT_SOURCE = ROOT / "assets/concept/sparkpaw-stormrail-skimmer-v5-occupied-chroma.png"
DEFAULT_STEM = "sparkpaw-stormrail-skimmer-v5-scale-study"

TARGETS = ((96, 44), (104, 46), (112, 48))
PANEL_W, LABEL_H, PLAY_H = 320, 24, 208


def extract_magenta(path: Path) -> Image.Image:
    source = Image.open(path).convert("RGBA")
    pixels = source.load()
    for y in range(source.height):
        for x in range(source.width):
            r, g, b, _ = pixels[x, y]
            if r > 220 and b > 180 and g < 80:
                pixels[x, y] = (0, 0, 0, 0)
    bounds = source.getchannel("A").getbbox()
    return source.crop(bounds)


def front16(source: Image.Image) -> Image.Image:
    result = Image.new("RGBA", source.size, (0, 0, 0, 0))
    src, dst = source.load(), result.load()
    for y in range(source.height):
        for x in range(source.width):
            r, g, b, a = src[x, y]
            if a < 96:
                continue
            dst[x, y] = FRONT16[nearest_index((r, g, b), FRONT16,
                                              avoid_zero=True)] + (255,)
    return result


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", type=Path, default=DEFAULT_SOURCE)
    parser.add_argument("--stem", default=DEFAULT_STEM)
    args = parser.parse_args()
    source_path = args.source if args.source.is_absolute() else ROOT / args.source
    output = ROOT / "assets/concept" / f"{args.stem}.png"
    output_2x = ROOT / "assets/concept" / f"{args.stem}-2x.png"

    craft = extract_magenta(source_path)
    rear = Image.open(BACKGROUND).convert("RGB").crop((640, 40, 960, 248))
    study = Image.new("RGB", (PANEL_W * len(TARGETS), LABEL_H + PLAY_H),
                      (4, 5, 12))
    draw = ImageDraw.Draw(study)
    for index, maximum in enumerate(TARGETS):
        scaled = craft.copy()
        scaled.thumbnail(maximum, Image.Resampling.LANCZOS)
        scaled = front16(scaled)
        panel_x = index * PANEL_W
        study.paste(rear, (panel_x, LABEL_H))
        x = panel_x + 28
        y = LABEL_H + (PLAY_H - scaled.height) // 2
        study.paste(scaled, (x, y), scaled)
        label = f"BOX {maximum[0]}x{maximum[1]}  ACTUAL {scaled.width}x{scaled.height}"
        draw.text((panel_x + 8, 7), label, fill=(190, 224, 239))
        draw.rectangle((x - 1, y - 1, x + scaled.width, y + scaled.height),
                       outline=(0, 210, 238))
    output.parent.mkdir(parents=True, exist_ok=True)
    study.save(output)
    study.resize((study.width * 2, study.height * 2),
                 Image.Resampling.NEAREST).save(output_2x)
    print(f"wrote {output}")
    print(f"wrote {output_2x}")


if __name__ == "__main__":
    main()
