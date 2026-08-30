#!/usr/bin/env python3
"""Require the native 1UP pickup to carry a complete aligned SPBM mask."""

from pathlib import Path
import struct

path=Path(__file__).resolve().parents[1]/"assets/runtime/sparkpaw-extra-life.spbm"
data=path.read_bytes()
assert data[:4]==b"SPBM"
width,height,depth,masked,row_bytes=struct.unpack(">HHBBH",data[4:12])
assert (width,height,depth,masked,row_bytes)==(32,22,4,1,4)
palette_bytes=(1<<depth)*3
plane_bytes=row_bytes*height
expected=12+palette_bytes+(depth+1)*plane_bytes
assert len(data)==expected,(len(data),expected)
mask=data[-plane_bytes:]
assert any(mask)
# The removed medallion corners must remain transparent.
assert not (mask[0]&0x80)
assert all(mask[y*row_bytes+2:y*row_bytes+4]==b"\0\0" for y in range(height))
print("PASS: extra-life 1UP has a complete aligned transparent mask")
