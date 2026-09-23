#!/usr/bin/env python3
"""Generate Sparkpaw's restrained pre-level ready screen and SPBM source."""
from pathlib import Path
import struct

from PIL import Image, ImageDraw, ImageOps

from generate_intro_proof import GLYPHS,medium_text,planar_bytes


ROOT = Path(__file__).resolve().parents[1]
CONCEPT = ROOT / "docs/concepts/ready-screen"
BACKGROUND = CONCEPT / "assets/sparkpaw-ready-background-source-v3.png"
LOGO = CONCEPT / "assets/sparkpaw-logo-wordmark-source-v2.png"
PREVIEW = CONCEPT / "sparkpaw-ready-screen-aga64-preview.png"
OPTIONS_PREVIEW = CONCEPT / "sparkpaw-options-screen-aga64-preview.png"
CAMPAIGN_OPTIONS_PREVIEW = CONCEPT / "sparkpaw-campaign-options-aga64-preview.png"
RUNTIME = ROOT / "assets/runtime/sparkpaw-ready-screen.spbm"
MENU_PATCHES = ROOT / "assets/runtime/readymenu.spbm"
PINNED = ROOT / "assets/concept/ready-alpha9-pinned"
# Alpha.9's approved READY palette. Changing option text must not requantize
# the unchanged title/READY art or alter its colours on any media.
READY_PALETTE = bytes.fromhex(
    "000000fda912fe9e02b1d1cd97aba61ed3ed15b1d0fc9302f88001d68118"
    "467f94e66801cb670da84f08515e6a514a4347362720597625476325374b"
    "2b292a1728404714081f181c161b3315101d0d15410d13240b0c240c0a0b"
    "06102b050d2604091904051203040f04050b010c1c010717020711010711"
    "01070d02051201050f02040f01040f02040d04030d02031002030f02030e"
    "02030b01030f01030e00030e01030a07010402020c01020f01020e01010e"
    "01010800000c000004f7d593")

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
        value = "JOYSTICK" if state == 2 else "JOYPAD"
        medium_text(draw, "OPTIONS", (320 - len("OPTIONS") * 9) // 2,
                    128, (31, 201, 224))
        small_text(draw, "CONTROL", 157, (241, 221, 170), 132)
        small_text(draw, value, 157, (31, 201, 224), 207)
        draw.polygon(((171, 160), (178, 154), (178, 166)),
                     fill=(31, 201, 224))
        draw.polygon(((240, 154), (247, 160), (240, 166)),
                     fill=(31, 201, 224))
        small_text(draw, "FIRE: RETURN", 177, (180, 190, 183))
    else:
        option = state - 4
        selected = option // 4
        secondary = "JOYPAD" if option % 4 >= 2 else "JOYSTICK"
        start_at = "STORMRAIL" if option % 2 else "STORM RUINS"
        medium_text(draw, "OPTIONS", (320 - len("OPTIONS") * 9) // 2,
                    124, (31, 201, 224))
        rows = (("CONTROL", secondary), ("START AT", start_at))
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


def pinned_ready_preview():
    raw=(PINNED / "sparkpaw-ready-screen.spbm").read_bytes()
    image=Image.new("P",(320,256))
    image.putpalette(raw[12:204])
    plane_size=40*256
    pixels=[]
    for y in range(256):
        for x in range(320):
            pixels.append(sum((((raw[204+p*plane_size+y*40+x//8] >>
                                 (7-x%8))&1)<<p) for p in range(6)))
    image.putdata(pixels)
    return image


def ready_background():
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

    return image


def build_ready_screen():
    image = ready_background()
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
    # Keep alpha.9's palette fixed while changing only option glyphs.
    indexed = Image.new("P", (1, 1))
    indexed.putpalette(READY_PALETTE)
    if READY_PALETTE[:3] != b"\0\0\0":
        raise ValueError("ready screen fullscreen COLOR00 must be black")
    indexed_screens = [screen.quantize(palette=indexed,dither=Image.Dither.NONE)
                       for screen in screens]
    palette_data = list(READY_PALETTE)
    pinned_ready_preview().save(PREVIEW)
    indexed_screens[2].save(OPTIONS_PREVIEW)
    indexed_screens[4].save(CAMPAIGN_OPTIONS_PREVIEW)
    # The READY background and both main-menu states are byte-identical to
    # alpha.9. Only the option states are new in this candidate.
    RUNTIME.write_bytes((PINNED / "sparkpaw-ready-screen.spbm").read_bytes())

    patches = Image.new("P", (PATCH_W, PATCH_H * len(indexed_screens)))
    patches.putpalette(palette_data)
    for state, screen in enumerate(indexed_screens):
        patches.paste(screen.crop((PATCH_X, PATCH_Y,
                                   PATCH_X + PATCH_W, PATCH_Y + PATCH_H)),
                      (0, state * PATCH_H))
    packed = bytearray(spbm_payload(patches,PATCH_W,PATCH_H*len(indexed_screens)))
    old = (PINNED / "readymenu.spbm").read_bytes()
    plane_size = 24*PATCH_H*len(indexed_screens)
    for plane in range(6):
        offset=204+plane*plane_size
        packed[offset:offset+24*PATCH_H*2]=old[offset:offset+24*PATCH_H*2]
    MENU_PATCHES.write_bytes(packed)


if __name__ == "__main__":
    CONCEPT.mkdir(parents=True, exist_ok=True)
    RUNTIME.parent.mkdir(parents=True, exist_ok=True)
    build_ready_screen()
