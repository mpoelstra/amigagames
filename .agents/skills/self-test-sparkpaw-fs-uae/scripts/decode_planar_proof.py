#!/usr/bin/env python3
"""Decode Sparkpaw's compact native four-plane framebuffer proof."""

from __future__ import annotations

import argparse
from pathlib import Path
import struct

from PIL import Image
import numpy as np

FRONT_COLORS = [
    0x001, 0x111, 0xD41, 0xF92, 0xFEA, 0x26C, 0x3CE, 0x94C,
    0x444, 0x666, 0xA9A, 0xEDC, 0x426, 0x72A, 0xA5D, 0xE26,
]


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--crop-around-target", nargs=2, type=int,
                        metavar=("LEFT", "RIGHT"))
    parser.add_argument("--scale", type=int, default=1)
    args = parser.parse_args()

    data = args.input.read_bytes()
    if len(data)<16 or data[:4] not in (b"ELPF", b"ELP2"):
        parser.error("input is not an ELPF/ELP2 proof")
    width,height,row_bytes,camera_x,target_x,target_y = struct.unpack(
        ">6H", data[4:16])
    plane_bytes = row_bytes*height
    required = 16+4*plane_bytes
    if len(data)<required:
        parser.error(f"truncated proof: {len(data)} bytes, need {required}")

    # Vectorized decoding keeps wide rolling-renderer proofs interactive. The
    # old nested Python loops took minutes for three 1536x256 captures.
    pixels = np.zeros((height, width), dtype=np.uint8)
    for plane in range(4):
        raw = np.frombuffer(
            data, dtype=np.uint8, count=plane_bytes,
            offset=16 + plane * plane_bytes).reshape(height, row_bytes)
        pixels |= np.unpackbits(raw, axis=1)[:, :width] << plane
    image = Image.fromarray(pixels, mode="P")
    palette = []
    for color in FRONT_COLORS:
        palette.extend((((color>>8)&15)*17,
                        ((color>>4)&15)*17,(color&15)*17))
    image.putpalette(palette+[0]*(768-len(palette)))

    if args.crop_around_target:
        left,right = args.crop_around_target
        image = image.crop((max(0,target_x-left),0,
                            min(width,target_x+right),height))
    if args.scale>1:
        image = image.resize((image.width*args.scale,image.height*args.scale),
                             Image.Resampling.NEAREST)
    args.output.parent.mkdir(parents=True,exist_ok=True)
    image.save(args.output)
    print(f"output={args.output.resolve()}")
    print(f"camera_x={camera_x} target_x={target_x} target_y={target_y}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
