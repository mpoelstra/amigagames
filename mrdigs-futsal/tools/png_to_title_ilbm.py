#!/usr/bin/env python3
"""Fit the title concept to a 320x256 uncompressed 16-colour ILBM."""
import struct
import sys
from pathlib import Path
from PIL import Image

W, H = 320, 256

def chunk(name, data):
    return name + struct.pack(">I", len(data)) + data + (b"\0" if len(data)&1 else b"")

def convert(source, output):
    src = Image.open(source).convert("RGB")
    scale = min(W / src.width, H / src.height)
    resized = src.resize((round(src.width*scale), round(src.height*scale)),
                         Image.Resampling.LANCZOS)
    canvas = Image.new("RGB", (W,H), (0,0,12))
    canvas.paste(resized, ((W-resized.width)//2, (H-resized.height)//2))
    image = canvas.quantize(colors=16, method=Image.Quantize.MEDIANCUT,
                            dither=Image.Dither.FLOYDSTEINBERG)
    palette = image.getpalette()[:48]
    pixels = list(image.getdata())
    body = bytearray()
    for y in range(H):
        row = pixels[y*W:(y+1)*W]
        for plane in range(4):
            for bx in range(W//8):
                value = 0
                for bit in range(8):
                    value |= ((row[bx*8+bit] >> plane)&1) << (7-bit)
                body.append(value)
    bmhd = struct.pack(">HHhhBBBBHBBhh",W,H,0,0,4,0,0,0,0,10,11,W,H)
    form = b"ILBM"+chunk(b"BMHD",bmhd)+chunk(b"CMAP",bytes(palette))+chunk(b"BODY",body)
    output.parent.mkdir(parents=True,exist_ok=True)
    output.write_bytes(b"FORM"+struct.pack(">I",len(form))+form)
    image.save(output.with_suffix(".png"))
    print("Wrote",output)

if __name__ == "__main__":
    convert(Path(sys.argv[1]),Path(sys.argv[2]))
