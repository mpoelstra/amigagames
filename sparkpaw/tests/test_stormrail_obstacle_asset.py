#!/usr/bin/env python3
"""Validate the bounded native Gate-4A rock/shard SPBM family."""

from pathlib import Path
import struct

root=Path(__file__).resolve().parents[1]
data=(root/"assets/runtime/stormrail-obstacles.spbm").read_bytes()
assert data[:4]==b"SPBM"
width,height,depth,masked,row_bytes=struct.unpack(">HHBBH",data[4:12])
assert (width,height,depth,masked,row_bytes)==(400,40,4,1,50)
palette_bytes=(1<<depth)*3
plane_bytes=row_bytes*height
assert len(data)==12+palette_bytes+(depth+1)*plane_bytes
mask=data[-plane_bytes:]

def occupied(x0,y0,width,height):
    return any(mask[y*row_bytes+(x>>3)]&(0x80>>(x&7))
               for y in range(y0,y0+height)
               for x in range(x0,x0+width))

assert occupied(0,0,48,40)
assert occupied(48,0,48,40)
assert occupied(96,0,48,40)
assert occupied(144,0,48,40)
assert occupied(192,0,48,40)
for x in (240,256,272,288,304,320):
    assert occupied(x,20,16,16)
assert occupied(336,0,32,40)
assert occupied(368,0,32,40)
print("PASS: Gate-4C tumble frames fit the bounded native sheet")
