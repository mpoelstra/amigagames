#!/usr/bin/env python3
"""Generate Sparkpaw's restrained pre-level ready screen and SPBM source."""
from pathlib import Path
import struct

from PIL import Image, ImageDraw, ImageOps

from generate_intro_proof import GLYPHS, medium_text, planar_bytes


ROOT = Path(__file__).resolve().parents[1]
CONCEPT = ROOT / "docs/concepts/ready-screen"
BACKGROUND = CONCEPT / "assets/sparkpaw-ready-background-source-v3.png"
LOGO = CONCEPT / "assets/sparkpaw-logo-wordmark-source-v2.png"
PREVIEW = CONCEPT / "sparkpaw-ready-screen-aga64-preview.png"
RUNTIME = ROOT / "assets/runtime/sparkpaw-ready-screen.spbm"


GLYPHS.update({
    "0": ("01110", "10001", "10011", "10101", "11001", "10001", "01110"),
    "1": ("00100", "01100", "00100", "00100", "00100", "00100", "01110"),
    "2": ("01110", "10001", "00001", "00010", "00100", "01000", "11111"),
    "6": ("00110", "01000", "10000", "11110", "10001", "10001", "01110"),
    "%": ("11001", "11010", "00100", "01000", "10110", "00110", "00000"),
})


def small_text(draw, value, y, colour, centre_x=160):
    width = len(value) * 6 - 1
    x = centre_x - width // 2
    for char in value:
        for gy, bits in enumerate(GLYPHS[char]):
            for gx, bit in enumerate(bits):
                if bit == "1":
                    draw.point((x + gx, y + gy), fill=colour)
        x += 6


def build_ready_screen():
    image = Image.new("RGB", (320, 256), (2, 7, 17))
    background = ImageOps.contain(Image.open(BACKGROUND).convert("RGB"),
                                  (320, 256), Image.Resampling.LANCZOS)
    image.paste(background, ((320-background.width)//2, 256-background.height))

    # Keep all typography on the true x=160 centreline. Correct the surrounding
    # composition instead: shift only the raised left architecture four native
    # pixels outward, leaving the continuous bottom rail untouched.
    left_arch = image.crop((0, 126, 76, 231))
    black_fill = image.crop((76, 126, 80, 231))
    image.paste(black_fill, (72, 126))
    image.paste(left_arch, (-4, 126))

    # Image generation separated the accepted mark from its title landscape
    # but returned a baked neutral checkerboard. Convert only that neutral
    # high-value field to alpha; coloured logo highlights remain untouched.
    logo_source = Image.open(LOGO).convert("RGB")
    logo = Image.new("RGBA", logo_source.size, (0, 0, 0, 0))
    src, dst = logo_source.load(), logo.load()
    for y in range(logo.height):
        for x in range(logo.width):
            red, green, blue = src[x, y]
            if max(red, green, blue)-min(red, green, blue) > 12 or \
               max(red, green, blue) < 225:
                dst[x, y] = (red, green, blue, 255)
    bounds = logo.getbbox()
    if bounds is None:
        raise ValueError("transparent logo extraction produced no pixels")
    logo = logo.crop(bounds)
    logo.thumbnail((300, 106), Image.Resampling.LANCZOS)
    image.paste(logo, ((320-logo.width)//2, 7), logo)

    draw = ImageDraw.Draw(image)

    prompt = "PRESS FIRE TO START"
    medium_text(draw, prompt, (320 - len(prompt) * 9) // 2, 133,
                (241, 221, 170))
    draw.line((39, 138, 65, 138), fill=(31, 201, 224))
    draw.line((254, 138, 280, 138), fill=(31, 201, 224))
    small_text(draw, "2026 MRDIG PRODUCTIONS", 201, (180, 190, 183))
    small_text(draw, "100% MADE WITH AI", 213, (71, 175, 198))

    indexed = image.quantize(colors=64, method=Image.Quantize.MEDIANCUT,
                             dither=Image.Dither.NONE)
    palette = indexed.getpalette()[:192]
    palette.extend([0] * (192 - len(palette)))
    indexed.save(PREVIEW)
    row_bytes, planes = planar_bytes(indexed, 6)
    payload = (b"SPBM" + struct.pack(">HHBBH", 320, 256, 6, 0, row_bytes) +
               bytes(palette) + planes)
    expected = 12 + 64 * 3 + row_bytes * 256 * 6
    if len(payload) != expected:
        raise ValueError(f"ready-screen SPBM is {len(payload)} bytes; "
                         f"expected {expected}")
    RUNTIME.write_bytes(payload)


if __name__ == "__main__":
    CONCEPT.mkdir(parents=True, exist_ok=True)
    RUNTIME.parent.mkdir(parents=True, exist_ok=True)
    build_ready_screen()
