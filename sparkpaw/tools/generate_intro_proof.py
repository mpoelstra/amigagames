#!/usr/bin/env python3
"""Build the isolated plate-1 intro proof as a six-plane SPBM."""
from pathlib import Path
import struct
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
ASSET_DIR = ROOT / "docs/concepts/story-intro/assets"
PLATES = (
    ("intro1", "intro-plate-01-balance", "intro-plate-01-balance-source-v2-aga-polish.png", (
        ("THE ANCIENT STORMSTONE", "KEPT THE VALLEY'S WEATHER", "IN PERFECT BALANCE."),
        ("FOUR CORES RULED LIGHTNING,", "RAIN, WIND AND WARMTH.", "THE FIFTH BALANCED THEM."))),
    ("intro2", "intro-plate-02-instruction", "intro-plate-02-instruction-source-v2-aga-polish.png", (
        ("GRAND ARCHIVOLT WAS BUILT", "TO GUARD THE STORMSTONE", "AND CONTROL WILD WEATHER."),
        ("A DAMAGED ORDER COMMANDED:", "CONTAIN ALL WEATHER.", "RELEASE NOTHING."))),
    ("intro3", "intro-plate-03-reversed-network", "intro-plate-03-reversed-network-source-v5-from-scratch-aga.png", (
        ("ARCHIVOLT TORE THE CORES", "FROM THE STORMSTONE", "AND SEALED THEM AWAY."),
        ("HE REVERSED THE STATIONS.", "EACH CORE DREW ITS ELEMENT", "INWARD WITHOUT END."),
        ("RAIN BECAME FLOODS.", "WIND BECAME HURRICANES.", "LIGHTNING NEVER STOPPED."))),
    ("intro4", "intro-plate-04-motive", "intro-plate-04-motive-source-v4-aga-polish.png", (
        ("SPARKPAW WATCHED THE STORMS", "TEAR THROUGH HIS HOME.", "SOMEONE HAD TO FREE THE CORES."),
        ("HIS SHARD POWERED GAUNTLET", "COULD FIND AND CARRY THEM,", "BACK TO THE STORMSTONE."))),
    ("intro5", "intro-plate-05-quest", "intro-plate-05-quest-source-v2-aga-polish.png", (
        ("RECOVER ONE CORE FROM", "EACH WEATHER STATION."),
        ("RETURN THEM TO THE STORMSTONE.", "RESTORE THE NATURAL SKY.", "STOP GRAND ARCHIVOLT."))),
)

def planar_bytes(image, depth):
    width, height = image.size
    row_bytes = ((width + 15) // 16) * 2
    out = bytearray(row_bytes * height * depth)
    pixels = image.load(); plane_size = row_bytes * height
    for y in range(height):
        for x in range(width):
            value = int(pixels[x, y]); mask = 0x80 >> (x & 7)
            offset = y * row_bytes + (x >> 3)
            for plane in range(depth):
                if value & (1 << plane): out[plane * plane_size + offset] |= mask
    return row_bytes, bytes(out)

def reserve_black_pen_zero(image, palette, colour_count=64):
    """Reserve COLOR00 black, using the smallest merge if black is absent."""
    palette = list(palette)
    mapping = list(range(256))
    try:
        black_pen = palette[:colour_count].index((0, 0, 0))
    except ValueError:
        black_pen = -1
    if black_pen >= 0:
        if black_pen != 0:
            mapping[0], mapping[black_pen] = black_pen, 0
            palette[0], palette[black_pen] = palette[black_pen], palette[0]
    else:
        histogram = image.histogram()[:colour_count]
        drop = min(range(1, colour_count), key=lambda pen: histogram[pen])
        replacement = min(
            (pen for pen in range(1, colour_count) if pen != drop),
            key=lambda pen: sum((palette[drop][channel]-palette[pen][channel])**2
                                for channel in range(3)),
        )
        mapping[drop] = replacement
        mapping[0] = drop
        palette[drop] = palette[0]
        palette[0] = (0, 0, 0)
    remapped = image.point(mapping)
    remapped.putpalette([value for rgb in palette for value in rgb] +
                        [0] * (768-len(palette)*3))
    return remapped, palette

def reserve_white_hint_pen(image, palette, colour_count=63):
    """Merge the least-used art pen and reserve it as stable hint white."""
    palette = list(palette)
    histogram = image.histogram()[:colour_count]
    hint_pen = min(range(1, colour_count), key=lambda pen: histogram[pen])
    replacement = min(
        (pen for pen in range(1, colour_count) if pen != hint_pen),
        key=lambda pen: sum((palette[hint_pen][channel]-palette[pen][channel])**2
                            for channel in range(3)),
    )
    mapping = list(range(256))
    mapping[hint_pen] = replacement
    remapped = image.point(mapping)
    palette[hint_pen] = (255, 255, 255)
    remapped.putpalette([value for rgb in palette for value in rgb] +
                        [0] * (768-len(palette)*3))
    return remapped, palette, hint_pen

GLYPHS = {
 "A":("01110","10001","10001","11111","10001","10001","10001"), "B":("11110","10001","10001","11110","10001","10001","11110"),
 "C":("01111","10000","10000","10000","10000","10000","01111"), "E":("11111","10000","10000","11110","10000","10000","11111"),
 "D":("11110","10001","10001","10001","10001","10001","11110"), "G":("01111","10000","10000","10111","10001","10001","01111"),
 "F":("11111","10000","10000","11110","10000","10000","10000"), "H":("10001","10001","10001","11111","10001","10001","10001"),
 "I":("11111","00100","00100","00100","00100","00100","11111"), "K":("10001","10010","10100","11000","10100","10010","10001"),
 "L":("10000","10000","10000","10000","10000","10000","11111"), "M":("10001","11011","10101","10101","10001","10001","10001"),
 "N":("10001","11001","10101","10011","10001","10001","10001"), "O":("01110","10001","10001","10001","10001","10001","01110"),
 "P":("11110","10001","10001","11110","10000","10000","10000"), "R":("11110","10001","10001","11110","10100","10010","10001"),
 "S":("01111","10000","10000","01110","00001","00001","11110"), "T":("11111","00100","00100","00100","00100","00100","00100"),
 "U":("10001","10001","10001","10001","10001","10001","01110"), "V":("10001","10001","10001","10001","10001","01010","00100"),
 "W":("10001","10001","10001","10101","10101","10101","01010"), "Y":("10001","10001","01010","00100","00100","00100","00100"),
 "'":("00100","00100","00000","00000","00000","00000","00000"), ".":("00000","00000","00000","00000","00000","00110","00110"),
 ",":("00000","00000","00000","00000","00110","00100","01000"),
 ":":("00000","00110","00110","00000","00110","00110","00000"),
 " ":("00000",)*7,
}

INTRO_HINT_X = 8
INTRO_HINT_Y = 157
INTRO_HINT_TEXT = "LMB to skip intro"

SMALL_GLYPHS = {
    "L": ("10000", "10000", "10000", "10000", "10000", "10000", "11111"),
    "M": ("10001", "11011", "10101", "10101", "10001", "10001", "10001"),
    "B": ("11110", "10001", "10001", "11110", "10001", "10001", "11110"),
    "t": ("00100", "00100", "11111", "00100", "00100", "00101", "00010"),
    "o": ("00000", "00000", "01110", "10001", "10001", "10001", "01110"),
    "s": ("00000", "00000", "01111", "10000", "01110", "00001", "11110"),
    "k": ("10000", "10000", "10010", "10100", "11000", "10100", "10010"),
    "i": ("00100", "00000", "01100", "00100", "00100", "00100", "01110"),
    "p": ("00000", "00000", "11110", "10001", "11110", "10000", "10000"),
    "n": ("00000", "00000", "11110", "10001", "10001", "10001", "10001"),
    "r": ("00000", "00000", "10110", "11001", "10000", "10000", "10000"),
    " ": ("00000",) * 7,
}

def medium_text(draw, text, x, y, colour):
    """Render the 5x7 face at a crisp 3:2 scale without antialiasing."""
    for char in text:
        for gy,bits in enumerate(GLYPHS[char]):
            y0=y+(gy*3)//2; y1=y+((gy+1)*3)//2-1
            for gx,bit in enumerate(bits):
                if bit=="1":
                    x0=x+(gx*3)//2; x1=x+((gx+1)*3)//2-1
                    draw.rectangle((x0,y0,x1,y1),fill=colour)
        x+=9

def small_text(draw, text, x, y, colour):
    """Render the unobtrusive mixed-case intro-skip hint at native pixels."""
    for char in text:
        for gy,bits in enumerate(SMALL_GLYPHS[char]):
            for gx,bit in enumerate(bits):
                if bit == "1":
                    draw.point((x+gx,y+gy),fill=colour)
        x += 6

def build_plate(name, preview_name, source_name, passages):
    source=Image.open(ASSET_DIR/source_name).convert("RGB").resize((320,256),Image.Resampling.LANCZOS)
    height=256+176*len(passages)
    image=Image.new("RGB",(320,height),(5,13,29))
    image.paste(source.crop((0,0,320,168)),(0,0))
    ImageDraw.Draw(image).line((0,167,319,167),fill=(48,177,207),width=1)
    indexed=image.quantize(colors=63,method=Image.Quantize.MEDIANCUT,dither=Image.Dither.NONE)
    palette_data=indexed.getpalette()[:192]
    palette=[tuple(palette_data[index:index+3]) for index in range(0,192,3)]
    indexed,palette=reserve_black_pen_zero(indexed,palette,63)
    hint_pen=None
    if name == "intro1":
        indexed,palette,hint_pen=reserve_white_hint_pen(indexed,palette,63)
    palette[63]=(238,220,173)
    indexed.putpalette([value for rgb in palette for value in rgb] + [0]*576)
    draw=ImageDraw.Draw(indexed)
    if hint_pen is not None:
        small_text(draw,INTRO_HINT_TEXT,INTRO_HINT_X+1,INTRO_HINT_Y+1,0)
        small_text(draw,INTRO_HINT_TEXT,INTRO_HINT_X,INTRO_HINT_Y,hint_pen)
    for passage,lines in enumerate(passages):
        page_top=256+passage*176
        text_top=page_top+88
        first_y=text_top+(18 if len(lines)==2 else 0)
        for row,value in enumerate(lines):
            medium_text(draw,value,(320-len(value)*9)//2,first_y+row*18,63)
    palette_data=indexed.getpalette()[:192]
    if palette_data[:3] != [0,0,0]:
        raise ValueError(f"{name}: fullscreen COLOR00 must be black")
    preview=Image.new("P",(320,256)); preview.putpalette(indexed.getpalette())
    preview.paste(indexed.crop((0,0,320,168)),(0,0)); preview.paste(indexed.crop((0,328,320,416)),(0,168))
    preview.save(ASSET_DIR/f"{preview_name}-aga64-preview.png")
    row_bytes,planes=planar_bytes(indexed,6)
    output=ROOT/"assets/runtime"/f"{name}.spbm"; output.parent.mkdir(parents=True,exist_ok=True)
    output.write_bytes(b"SPBM"+struct.pack(">HHBBH",320,height,6,0,row_bytes)+bytes(palette_data)+planes)

def main():
    for name,preview_name,source_name,passages in PLATES:
        build_plate(name,preview_name,source_name,passages)

if __name__ == "__main__": main()
