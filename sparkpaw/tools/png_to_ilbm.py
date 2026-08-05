#!/usr/bin/env python3
"""Convert a paletted 320x256 PNG to an Amiga ILBM with ByteRun1 BODY."""

import argparse, struct
from pathlib import Path
from PIL import Image

def chunk(name, data):
    return name + struct.pack(">I", len(data)) + data + (b"\0" if len(data)&1 else b"")

def byterun1(data):
    out=bytearray(); i=0
    while i<len(data):
        run=1
        while i+run<len(data) and run<128 and data[i+run]==data[i]: run+=1
        if run>=3:
            out+=bytes((257-run, data[i])); i+=run; continue
        start=i; i+=run
        while i<len(data) and i-start<128:
            run=1
            while i+run<len(data) and run<128 and data[i+run]==data[i]: run+=1
            if run>=3: break
            i+=run
        literal=data[start:i]; out.append(len(literal)-1); out+=literal
    return bytes(out)

def convert(src, dst):
    image=Image.open(src)
    if image.mode!="P": image=image.convert("RGB").quantize(colors=64)
    if image.size!=(320,256): raise SystemExit("input must be 320x256")
    colors=max(1, len(image.getcolors(maxcolors=256) or []))
    planes=max(1, min(8,(colors-1).bit_length()))
    width,height=image.size; rowbytes=((width+15)//16)*2; pixels=image.load()
    body=bytearray()
    for y in range(height):
        for plane in range(planes):
            row=bytearray(rowbytes)
            for x in range(width):
                if (pixels[x,y]>>plane)&1: row[x>>3]|=1<<(7-(x&7))
            body+=byterun1(row)
    pal=(image.getpalette() or [])[:(1<<planes)*3]
    pal+=([0]*((1<<planes)*3-len(pal)))
    bmhd=struct.pack(">HHhhBBBBHBBhh",width,height,0,0,planes,0,1,0,0,10,11,width,height)
    payload=b"ILBM"+chunk(b"BMHD",bmhd)+chunk(b"CMAP",bytes(pal))+chunk(b"CAMG",struct.pack(">I",0))+chunk(b"BODY",bytes(body))
    Path(dst).write_bytes(b"FORM"+struct.pack(">I",len(payload))+payload)
    print(f"Wrote {dst}: {planes} bitplanes, {len(payload)+8} bytes")

if __name__=="__main__":
    parser=argparse.ArgumentParser(); parser.add_argument("src"); parser.add_argument("dst")
    args=parser.parse_args(); convert(args.src,args.dst)
