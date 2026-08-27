#!/usr/bin/env python3
"""Build the isolated plate-1 intro proof as a six-plane SPBM."""
from pathlib import Path
import struct
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
ASSET_DIR = ROOT / "docs/concepts/story-intro/assets"
PLATES = (
    ("intro-plate-01-balance", "intro-plate-01-balance-source-v2-aga-polish.png", (
        ("THE ANCIENT STORMSTONE", "KEPT THE VALLEY'S WEATHER", "IN PERFECT BALANCE."),
        ("FOUR CORES RULED LIGHTNING,", "RAIN, WIND AND WARMTH.", "THE FIFTH BALANCED THEM."))),
    ("intro-plate-02-instruction", "intro-plate-02-instruction-source-v2-aga-polish.png", (
        ("GRAND ARCHIVOLT WAS BUILT", "TO GUARD THE STORMSTONE", "AND CONTROL WILD WEATHER."),
        ("A DAMAGED ORDER COMMANDED:", "CONTAIN ALL WEATHER.", "RELEASE NOTHING."))),
    ("intro-plate-03-reversed-network", "intro-plate-03-reversed-network-source-v5-from-scratch-aga.png", (
        ("ARCHIVOLT TORE THE CORES", "FROM THE STORMSTONE", "AND SEALED THEM AWAY."),
        ("HE REVERSED THE STATIONS.", "EACH CORE DREW ITS ELEMENT", "INWARD WITHOUT END."),
        ("RAIN BECAME FLOODS.", "WIND BECAME HURRICANES.", "LIGHTNING NEVER STOPPED."))),
    ("intro-plate-04-motive", "intro-plate-04-motive-source-v4-aga-polish.png", (
        ("SPARKPAW WATCHED THE STORMS", "TEAR THROUGH HIS HOME.", "SOMEONE HAD TO FREE THE CORES."),
        ("HIS SHARD POWERED GAUNTLET", "COULD FIND AND CARRY THEM,", "BACK TO THE STORMSTONE."))),
    ("intro-plate-05-quest", "intro-plate-05-quest-source-v2-aga-polish.png", (
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

def build_plate(name, source_name, passages):
    source=Image.open(ASSET_DIR/source_name).convert("RGB").resize((320,256),Image.Resampling.LANCZOS)
    height=256+176*len(passages)
    image=Image.new("RGB",(320,height),(5,13,29))
    image.paste(source.crop((0,0,320,168)),(0,0))
    ImageDraw.Draw(image).line((0,167,319,167),fill=(48,177,207),width=1)
    indexed=image.quantize(colors=63,method=Image.Quantize.MEDIANCUT,dither=Image.Dither.NONE)
    palette=indexed.getpalette(); palette[189:192]=[238,220,173]; indexed.putpalette(palette)
    draw=ImageDraw.Draw(indexed)
    for passage,lines in enumerate(passages):
        text_top=256+passage*176+88
        first_y=text_top+(18 if len(lines)==2 else 0)
        for row,value in enumerate(lines):
            medium_text(draw,value,(320-len(value)*9)//2,first_y+row*18,63)
    palette=indexed.getpalette()[:192]
    preview=Image.new("P",(320,256)); preview.putpalette(indexed.getpalette())
    preview.paste(indexed.crop((0,0,320,168)),(0,0)); preview.paste(indexed.crop((0,328,320,416)),(0,168))
    preview.save(ASSET_DIR/f"{name}-aga64-preview.png")
    row_bytes,planes=planar_bytes(indexed,6)
    output=ROOT/"assets/runtime"/f"{name}.spbm"; output.parent.mkdir(parents=True,exist_ok=True)
    output.write_bytes(b"SPBM"+struct.pack(">HHBBH",320,height,6,0,row_bytes)+bytes(palette)+planes)

def main():
    for name,source_name,passages in PLATES: build_plate(name,source_name,passages)

if __name__ == "__main__": main()
