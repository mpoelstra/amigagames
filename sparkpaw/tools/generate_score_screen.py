#!/usr/bin/env python3
"""Create the native 64-colour level-complete screen and glyph atlas."""
from pathlib import Path
import struct

from PIL import Image, ImageDraw, ImageOps

from generate_intro_proof import GLYPHS, planar_bytes, reserve_black_pen_zero


ROOT = Path(__file__).resolve().parents[1]
CONCEPT = ROOT / "docs/concepts/score-system"
SOURCE = CONCEPT / "sparkpaw-level-complete-score-concept-v4-title-character.png"
PREVIEW = CONCEPT / "sparkpaw-level-complete-score-aga64-preview.png"
RUNTIME = ROOT / "assets/runtime/sparkpaw-level-complete.spbm"
GLYPH_RUNTIME = ROOT / "assets/runtime/sparkpaw-score-glyphs.spbm"
ROWS = (116, 137, 158, 179)
VALUE_Y_OFFSET = 3
LABELS = ("ENEMY BONUS", "DIAMOND BONUS", "TIME BONUS", "TOTAL SCORE")
LABEL_X = 91
LABEL_Y_OFFSET = 5
LABEL_CLEAR = (88, 172)
NUMBER_X = 176
TILE_W, TILE_H = 8, 12
FIELD_CELLS = 7
CLEAR_H = 16
CHARS = "0123456789X"
PROMPT = "REPLAY LEVEL"
PROMPT_W = 144
INK = (248, 222, 154)
LABEL_INK = (12, 218, 231)
PANEL = (2, 9, 20)

GLYPHS.update({
    "0": ("01110","10001","10011","10101","11001","10001","01110"),
    "1": ("00100","01100","00100","00100","00100","00100","01110"),
    "2": ("01110","10001","00001","00010","00100","01000","11111"),
    "3": ("11110","00001","00001","01110","00001","00001","11110"),
    "4": ("00010","00110","01010","10010","11111","00010","00010"),
    "5": ("11111","10000","11110","00001","00001","10001","01110"),
    "6": ("00110","01000","10000","11110","10001","10001","01110"),
    "7": ("11111","00001","00010","00100","01000","01000","01000"),
    "8": ("01110","10001","10001","01110","10001","10001","01110"),
    "9": ("01110","10001","10001","01111","00001","00010","11100"),
    "X": ("10001","01010","00100","00100","00100","01010","10001"),
})


def spbm(image: Image.Image, width: int, height: int) -> bytes:
    row_bytes, planes = planar_bytes(image, 6)
    palette = bytes(image.getpalette()[:192])
    return (b"SPBM" + struct.pack(">HHBBH", width, height, 6, 0, row_bytes) +
            palette + planes)


def draw_glyph(draw: ImageDraw.ImageDraw, char: str, x: int, y: int) -> None:
    for gy, bits in enumerate(GLYPHS[char]):
        for gx, bit in enumerate(bits):
            if bit == "1":
                draw.point((x + gx + 1, y + gy + 2), fill=INK)


def draw_value(image: Image.Image, text: str, row: int) -> None:
    draw = ImageDraw.Draw(image)
    x = NUMBER_X + (FIELD_CELLS-len(text))*TILE_W
    for char in text:
        if char != " ":
            draw_glyph(draw, char, x, ROWS[row] + VALUE_Y_OFFSET)
        x += TILE_W


def draw_label(draw: ImageDraw.ImageDraw, text: str, row: int) -> None:
    x = LABEL_X
    y = ROWS[row] + LABEL_Y_OFFSET
    for char in text:
        for gy, bits in enumerate(GLYPHS[char]):
            for gx, bit in enumerate(bits):
                if bit == "1":
                    draw.point((x + gx, y + gy), fill=LABEL_INK)
        x += 6


def draw_prompt(draw: ImageDraw.ImageDraw) -> None:
    x = (320-(len(PROMPT)*6-1))//2
    for char in PROMPT:
        for gy, bits in enumerate(GLYPHS[char]):
            for gx, bit in enumerate(bits):
                if bit == "1":
                    draw.point((x+gx, 234+gy), fill=LABEL_INK)
        x += 6


def build() -> None:
    source = Image.open(SOURCE).convert("RGB")
    source = ImageOps.fit(source, (320, 240), Image.Resampling.LANCZOS)
    base = Image.new("RGB", (320, 256), (0, 0, 0))
    base.paste(source, (0, 8))
    draw = ImageDraw.Draw(base)
    for row, y in enumerate(ROWS):
        # The source concept is reduced with filtering, so its baked labels
        # are deliberately replaced by native 5x7 faces. This keeps labels
        # and dynamic values on the same hard-pixel grid.
        draw.rectangle((LABEL_CLEAR[0], y, LABEL_CLEAR[1]-1,
                        y+CLEAR_H-1), fill=PANEL)
        draw.rectangle((NUMBER_X, y,
                        NUMBER_X + FIELD_CELLS*TILE_W-1,
                        y+CLEAR_H-1), fill=PANEL)
        draw_label(draw, LABELS[row], row)

    glyphs = Image.new("RGB", (PROMPT_W, TILE_H*(len(CHARS)+1)), PANEL)
    glyph_draw = ImageDraw.Draw(glyphs)
    for index, char in enumerate(CHARS):
        draw_glyph(glyph_draw, char, 0, index*TILE_H)
    prompt_x = (PROMPT_W-(len(PROMPT)*6-1))//2
    for char in PROMPT:
        for gy, bits in enumerate(GLYPHS[char]):
            for gx, bit in enumerate(bits):
                if bit == "1":
                    glyph_draw.point((prompt_x+gx,
                                      len(CHARS)*TILE_H+gy+2),
                                     fill=LABEL_INK)
        prompt_x += 6

    combined = Image.new("RGB", (320, 256 + glyphs.height), PANEL)
    combined.paste(base, (0, 0))
    combined.paste(glyphs, (0, 256))
    indexed = combined.quantize(colors=64, method=Image.Quantize.MEDIANCUT,
                                dither=Image.Dither.NONE)
    raw = indexed.getpalette()[:192]
    palette = [tuple(raw[i:i+3]) for i in range(0, 192, 3)]
    indexed, palette = reserve_black_pen_zero(indexed, palette)
    palette_data = [v for rgb in palette for v in rgb]
    indexed.putpalette(palette_data)
    indexed_base = indexed.crop((0, 0, 320, 256))
    indexed_glyphs = indexed.crop((0, 256, PROMPT_W, 256+glyphs.height))
    indexed_base.putpalette(palette_data)
    indexed_glyphs.putpalette(palette_data)

    preview = indexed_base.convert("RGB")
    draw_value(preview, "007X020", 0)
    draw_value(preview, "042X005", 1)
    draw_value(preview, "036X010", 2)
    draw_value(preview, "012450", 3)
    draw_prompt(ImageDraw.Draw(preview))
    preview = preview.quantize(palette=indexed_base, dither=Image.Dither.NONE)
    preview.save(PREVIEW)
    RUNTIME.write_bytes(spbm(indexed_base, 320, 256))
    GLYPH_RUNTIME.write_bytes(spbm(indexed_glyphs, PROMPT_W,
                                   TILE_H*(len(CHARS)+1)))


if __name__ == "__main__":
    build()
