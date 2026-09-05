#!/usr/bin/env python3
"""Reduce the approved Gate-6 v2 concept into cleaned native FRONT16 cells."""

from pathlib import Path
from PIL import Image

from generate_runtime_assets import FRONT16

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "assets/concept/sparkpaw-stormrail-gate6-native-source-v3.png"
GATE_SOURCE = ROOT / "assets/concept/sparkpaw-stormrail-gate6-storm-ruins-threshold-concept-v2.png"
SHEET = ROOT / "assets/concept/sparkpaw-stormrail-gate6-native-v4-aga16.png"
REVIEW = ROOT / "assets/concept/sparkpaw-stormrail-gate6-native-v4-review-4x.png"
HEADER = ROOT / "src/stormrail_gate6_art.h"
MAGENTA = (255, 0, 255)

COMPONENTS = (
    ("harrier", (75, 140, 910, 735), (80, 46)),
)


def material_pen(rgb: tuple[int, int, int]) -> int:
    """Map concept colour by material role, not merely RGB proximity."""
    r, g, b = rgb
    light = (r * 3 + g * 5 + b * 2) // 10
    spread = max(rgb) - min(rgb)
    if spread > 48 and b > r + 35 and g > r + 25:
        return 6 if light >= 92 else 5
    if spread > 44 and b > g + 16 and r > g + 10:
        return 14 if light >= 100 else (13 if light >= 55 else 12)
    if spread > 62 and r > b + 52 and g > b + 22:
        return 4 if light >= 145 else (3 if light >= 82 else 2)
    if light < 18:
        return 1
    if light < 46:
        return 8
    if light < 83:
        return 9
    if light < 137:
        return 10
    return 11


def keyed_crop(source: Image.Image, box: tuple[int, int, int, int]) -> Image.Image:
    crop = source.crop(box).convert("RGBA")
    pixels = crop.load()
    for y in range(crop.height):
        for x in range(crop.width):
            r, g, b, _ = pixels[x, y]
            if r > 205 and b > 150 and g < 110:
                pixels[x, y] = (0, 0, 0, 0)
    return crop


def reduce(source: Image.Image, size: tuple[int, int], *, fill=False,
           brighten=False, harrier=False, turret=0) -> Image.Image:
    if fill:
        reduced = source.resize(size, Image.Resampling.LANCZOS)
    else:
        source.thumbnail(size, Image.Resampling.LANCZOS)
        reduced = source
    rgba = Image.new("RGBA", size, (0, 0, 0, 0))
    rgba.alpha_composite(reduced, ((size[0] - reduced.width) // 2,
                                   (size[1] - reduced.height) // 2))
    indexed = Image.new("P", size, 0)
    indexed.putpalette([v for rgb in FRONT16 for v in rgb] + [0] * (768 - 48))
    src, dst = rgba.load(), indexed.load()
    for y in range(size[1]):
        for x in range(size[0]):
            r, g, b, a = src[x, y]
            if a >= 104:
                dst[x, y] = material_pen((r, g, b))

    # Native cleanup: remove isolated single-pixel noise, replace only exposed
    # black keyline with material shadow, and keep real interior occlusion.
    original = indexed.copy(); before = original.load()
    for y in range(size[1]):
        for x in range(size[0]):
            pen = before[x, y]
            neighbours = [before[nx, ny] for nx, ny in
                          ((x-1, y), (x+1, y), (x, y-1), (x, y+1))
                          if 0 <= nx < size[0] and 0 <= ny < size[1]]
            solid = [value for value in neighbours if value]
            if pen and not solid:
                dst[x, y] = 0
            elif pen == 1 and any(value == 0 for value in neighbours):
                dst[x, y] = 8 if any(value in (8, 9, 10, 11) for value in solid) else 12
            elif pen and len(solid) >= 3 and all(value != pen for value in solid):
                counts = {value: solid.count(value) for value in set(solid)}
                replacement = max(counts, key=counts.get)
                if counts[replacement] >= 3:
                    dst[x, y] = replacement
    if brighten:
        ramp = {1: 8, 8: 9, 9: 10, 12: 13}
        for y in range(size[1]):
            for x in range(size[0]):
                if dst[x, y] in ramp:
                    dst[x, y] = ramp[dst[x, y]]
    if harrier:
        # The Storm Ruins rear layer is already near-black and blue. Preserve
        # the concept's dark armour, but lift its large shadow masses into the
        # neutral metal ramp so the native sprite reads as one craft instead
        # of apparently disconnected orange highlights in motion.
        # Preserve the FRONT16 steel ramp now that the actor cache uses its
        # correct fixed colour-plane stride. Violet and amber remain accents.
        ramp = {}
        for y in range(size[1]):
            for x in range(size[0]):
                if dst[x, y] in ramp:
                    dst[x, y] = ramp[dst[x, y]]
    if turret:
        # These cells overlap the dark wall. Give the concept-derived housing
        # a deliberate one-pixel steel rim and retain saturated muzzle/core
        # accents, otherwise only the blue core survives in the gameplay
        # composition and the weapon reads as wall decoration.
        accent = 5 if turret == 1 else 7
        stable = {8: 1, 9: accent, 10: 4, 11: 4,
                  12: 7, 13: 7, 14: 7, 15: 7}
        for y in range(size[1]):
            for x in range(size[0]):
                if dst[x, y] in stable:
                    dst[x, y] = stable[dst[x, y]]
        before = indexed.copy(); src_pen = before.load()
        for y in range(size[1]):
            for x in range(size[0]):
                pen = src_pen[x, y]
                if not pen:
                    continue
                exposed = any(src_pen[nx, ny] == 0 for nx, ny in
                              ((x-1, y), (x+1, y), (x, y-1), (x, y+1))
                              if 0 <= nx < size[0] and 0 <= ny < size[1])
                if exposed and pen not in (3, 4, 6, 14, 15):
                    dst[x, y] = 4
                elif turret == 1 and pen == 5:
                    dst[x, y] = 6
                elif turret == 2 and pen in (12, 13):
                    dst[x, y] = 14
    indexed.info["transparency"] = 0
    return indexed


def packed_pixels(cells: list[Image.Image]) -> bytes:
    values = []
    for cell in cells:
        values.extend(cell.getdata())
    return bytes((values[i] << 4) | (values[i + 1] if i + 1 < len(values) else 0)
                 for i in range(0, len(values), 2))


def write_header(data: bytes) -> None:
    lines = [
        "#ifndef STORMRAIL_GATE6_ART_H",
        "#define STORMRAIL_GATE6_ART_H",
        "",
        "/* Generated from the rebuilt native-v4 source; two FRONT16 pixels/byte. */",
        f"#define STORM_GATE6_ART_BYTES {len(data)}",
        "static const unsigned char stormGate6ArtPacked[STORM_GATE6_ART_BYTES]={",
    ]
    for start in range(0, len(data), 16):
        lines.append("    " + ",".join(f"0x{value:02x}" for value in data[start:start+16]) + ",")
    lines += ["};", "", "#endif", ""]
    HEADER.write_text("\n".join(lines))


def main() -> None:
    source = Image.open(SOURCE).convert("RGB")
    cells = [reduce(keyed_crop(source, box), size,
                    harrier=name == "harrier",
                    turret=1 if name == "upper" else (2 if name == "lower" else 0))
             for name, box, size in COMPONENTS]
    gate_source = Image.open(GATE_SOURCE).convert("RGB")
    gate = reduce(keyed_crop(gate_source, (1178, 683, 1245, 991)),
                  (32, 208), fill=True)
    gate_pixels = gate.load()
    for y in range(gate.height):
        for x in range(gate.width):
            if gate_pixels[x, y] in (12, 13, 14, 15):
                gate_pixels[x, y] = 8 if (x+y)&1 else 9
    cells.append(gate)
    sheet = Image.new("P", (160, 208), 0)
    sheet.putpalette([v for rgb in FRONT16 for v in rgb] + [0] * (768 - 48))
    sheet.paste(cells[0], (0, 0)); sheet.paste(cells[1], (128, 0))
    sheet.info["transparency"] = 0
    sheet.save(SHEET)
    review = Image.new("RGB", sheet.size, MAGENTA)
    rgba = sheet.convert("RGBA")
    alpha = Image.new("L", sheet.size, 0)
    ap, sp = alpha.load(), sheet.load()
    for y in range(sheet.height):
        for x in range(sheet.width):
            if sp[x, y]: ap[x, y] = 255
    rgba.putalpha(alpha); review.paste(rgba, (0, 0), alpha)
    review.resize((640, 832), Image.Resampling.NEAREST).save(REVIEW)
    write_header(packed_pixels(cells))
    print(f"wrote {SHEET}")
    print(f"wrote {REVIEW}")
    print(f"wrote {HEADER}")


if __name__ == "__main__":
    main()
