#!/usr/bin/env python3
"""Generate Sparkpaw's restrained pre-level ready screen and SPBM source."""
from pathlib import Path
import struct

from PIL import Image, ImageDraw, ImageOps

from generate_intro_proof import (GLYPHS, medium_text, planar_bytes,
                                  reserve_black_pen_zero)


ROOT = Path(__file__).resolve().parents[1]
CONCEPT = ROOT / "docs/concepts/ready-screen"
BACKGROUND = CONCEPT / "assets/sparkpaw-ready-background-source-v3.png"
LOGO = CONCEPT / "assets/sparkpaw-logo-wordmark-source-v2.png"
PREVIEW = CONCEPT / "sparkpaw-ready-screen-aga64-preview.png"
OPTIONS_PREVIEW = CONCEPT / "sparkpaw-options-screen-aga64-preview.png"
CAMPAIGN_OPTIONS_PREVIEW = CONCEPT / "sparkpaw-campaign-options-aga64-preview.png"
RUNTIME = ROOT / "assets/runtime/sparkpaw-ready-screen.spbm"
MENU_PATCHES = ROOT / "assets/runtime/readymenu.spbm"

PATCH_Y = 118
PATCH_H = 104
PATCH_X = 64
PATCH_W = 192


GLYPHS.update({
    "0": ("01110", "10001", "10011", "10101", "11001", "10001", "01110"),
    "1": ("00100", "01100", "00100", "00100", "00100", "00100", "01110"),
    "2": ("01110", "10001", "00001", "00010", "00100", "01000", "11111"),
    "6": ("00110", "01000", "10000", "11110", "10001", "10001", "01110"),
    "J": ("00111", "00010", "00010", "00010", "10010", "10010", "01100"),
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


def small_text_left(draw, value, x, y, colour):
    small_text(draw, value, y, colour, x + (len(value) * 6 - 1) // 2)


def small_text_right(draw, value, right_x, y, colour):
    small_text_left(draw, value, right_x - (len(value) * 6 - 1), y, colour)


def menu_screen(image, state):
    image = image.copy()
    draw = ImageDraw.Draw(image)

    if state < 2:
        selected = state
        labels = ("START GAME", "OPTIONS")
        for index, label in enumerate(labels):
            colour = (31, 201, 224) if index == selected else (241, 221, 170)
            y = 129 + index * 25
            medium_text(draw, label, (320 - len(label) * 9) // 2, y, colour)
            if index == selected:
                draw.line((82, y + 6, 105, y + 6), fill=(31, 201, 224))
                draw.line((214, y + 6, 237, y + 6), fill=(31, 201, 224))
        small_text(draw, "2026 MRDIG PRODUCTIONS", 201, (180, 190, 183))
        small_text(draw, "100% MADE WITH AI", 213, (71, 175, 198))
    elif state < 4:
        value = "JUMP" if state == 2 else "FIRE"
        medium_text(draw, "OPTIONS", (320 - len("OPTIONS") * 9) // 2,
                    128, (31, 201, 224))
        small_text(draw, "SECOND BUTTON", 157, (241, 221, 170), 132)
        small_text(draw, value, 157, (31, 201, 224), 207)
        draw.polygon(((183, 160), (190, 154), (190, 166)),
                     fill=(31, 201, 224))
        draw.polygon(((224, 154), (231, 160), (224, 166)),
                     fill=(31, 201, 224))
        small_text(draw, "FIRE: RETURN", 177, (180, 190, 183))
    else:
        option = state - 4
        selected = option // 4
        secondary = "FIRE" if option % 4 >= 2 else "JUMP"
        start_at = "STORMRAIL" if option % 2 else "STORM RUINS"
        medium_text(draw, "OPTIONS", (320 - len("OPTIONS") * 9) // 2,
                    124, (31, 201, 224))
        rows = (("SECOND BUTTON", secondary), ("START AT", start_at))
        for row, (label, value) in enumerate(rows):
            y = 149 + row * 20
            colour = (31, 201, 224) if row == selected else (180, 190, 183)
            small_text_right(draw, label, 150, y, (241, 221, 170))
            small_text_left(draw, value, 174, y, colour)
            if row == selected:
                value_right = 174 + len(value) * 6 - 2
                draw.polygon(((160, y + 3), (167, y - 3), (167, y + 9)),
                             fill=colour)
                draw.polygon(((value_right + 8, y - 3),
                              (value_right + 15, y + 3),
                              (value_right + 8, y + 9)),
                             fill=colour)
        small_text(draw, "FIRE: RETURN", 190, (180, 190, 183))
    return image


def spbm_payload(indexed, width, height, depth=6):
    palette_data = indexed.getpalette()[:(1 << depth) * 3]
    row_bytes, planes = planar_bytes(indexed, depth)
    payload = (b"SPBM" + struct.pack(">HHBBH", width, height, depth, 0,
                                     row_bytes) +
               bytes(palette_data) + planes)
    expected = 12 + (1 << depth) * 3 + row_bytes * height * depth
    if len(payload) != expected:
        raise ValueError(f"SPBM is {len(payload)} bytes; expected {expected}")
    return payload


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

    screens = [menu_screen(image, state) for state in range(12)]
    for state, screen in enumerate(screens):
        for box in ((0, PATCH_Y, PATCH_X, PATCH_Y + PATCH_H),
                    (PATCH_X + PATCH_W, PATCH_Y, 320,
                     PATCH_Y + PATCH_H)):
            if screen.crop(box).tobytes() != image.crop(box).tobytes():
                raise ValueError(f"menu state {state} changed corner pixels")
    for state in (2, 3):
        if screens[state].crop((PATCH_X, 195, PATCH_X + PATCH_W, 215)).tobytes() \
                != image.crop((PATCH_X, 195, PATCH_X + PATCH_W, 215)).tobytes():
            raise ValueError("Options credits field must remain empty")
    # Quantize the four accepted alpha.68 states exactly as before. New
    # campaign-only patches are then mapped into that fixed palette so adding
    # the shortcut cannot recolour the established ready/options screens.
    combined = Image.new("RGB", (320, 256 * 4))
    for state, screen in enumerate(screens[:4]):
        combined.paste(screen, (0, state * 256))
    indexed = combined.quantize(colors=64, method=Image.Quantize.MEDIANCUT,
                                dither=Image.Dither.NONE)
    raw_palette = indexed.getpalette()[:192]
    palette = [tuple(raw_palette[index:index + 3])
               for index in range(0, 192, 3)]
    indexed, palette = reserve_black_pen_zero(indexed, palette)
    palette_data = [value for rgb in palette for value in rgb]
    if palette_data[:3] != [0, 0, 0]:
        raise ValueError("ready screen fullscreen COLOR00 must be black")
    indexed.putpalette(palette_data)
    indexed_screens = [indexed.crop((0, state * 256, 320,
                                     (state + 1) * 256))
                       for state in range(4)]
    indexed_screens.extend(screen.quantize(
        palette=indexed, dither=Image.Dither.NONE) for screen in screens[4:])
    indexed_screens[0].save(PREVIEW)
    indexed_screens[2].save(OPTIONS_PREVIEW)
    indexed_screens[4].save(CAMPAIGN_OPTIONS_PREVIEW)
    RUNTIME.write_bytes(spbm_payload(indexed_screens[0], 320, 256))

    patches = Image.new("P", (PATCH_W, PATCH_H * len(indexed_screens)))
    patches.putpalette(palette_data)
    for state, screen in enumerate(indexed_screens):
        patches.paste(screen.crop((PATCH_X, PATCH_Y,
                                   PATCH_X + PATCH_W, PATCH_Y + PATCH_H)),
                      (0, state * PATCH_H))
    MENU_PATCHES.write_bytes(spbm_payload(
        patches, PATCH_W, PATCH_H * len(indexed_screens)))


if __name__ == "__main__":
    CONCEPT.mkdir(parents=True, exist_ok=True)
    RUNTIME.parent.mkdir(parents=True, exist_ok=True)
    build_ready_screen()
