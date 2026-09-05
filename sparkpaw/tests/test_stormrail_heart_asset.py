#!/usr/bin/env python3
"""Protect the native, masked and unmistakably heart-shaped Stormrail pickup."""

from pathlib import Path
import struct

path=Path(__file__).resolve().parents[1]/"assets/runtime/stormrail-heart.spbm"
data=path.read_bytes()
assert data[:4]==b"SPBM"
width,height,depth,masked,row_bytes=struct.unpack(">HHBBH",data[4:12])
assert (width,height,depth,masked,row_bytes)==(32,21,4,1,4)
palette_bytes=(1<<depth)*3
plane_bytes=row_bytes*height
assert len(data)==12+palette_bytes+(depth+1)*plane_bytes
mask=data[-plane_bytes:]

def opaque(x: int,y: int) -> bool:
    return bool(mask[y*row_bytes+(x>>3)]&(0x80>>(x&7)))

points=[(x,y) for y in range(height) for x in range(16) if opaque(x,y)]
assert points
assert (min(x for x,_ in points),min(y for _,y in points),
        max(x for x,_ in points),max(y for _,y in points))==(0,2,15,18)
assert not opaque(7,2) and not opaque(8,2)
assert opaque(7,18) and opaque(8,18)
assert all(not opaque(x,y) for y in range(height) for x in range(16,32))
red=data[12+2*3:12+2*3+3]
assert red[0]>red[1]*2 and red[0]>red[2]*2
print("PASS: Stormrail heart is native, masked and keeps its cleft/point silhouette")
