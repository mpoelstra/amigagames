#!/usr/bin/env python3
"""Generate the native FRONT16 Gate-4A castle-masonry obstacle family."""

from pathlib import Path
from PIL import Image

from generate_runtime_assets import FRONT16, save_spbm
from generate_stormrail_debris_native_animation_study import damaged, reduce_pose
from generate_stormrail_debris_native_family import reduce_component
from generate_stormrail_debris_native_v2_study import INPUT, components

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "assets/runtime/stormrail-obstacles.spbm"
PREVIEW = ROOT / "assets/concept/stormrail-gate4a-obstacles-aga16.png"
SHEET_W, SHEET_H = 400, 40

def main() -> None:
    source = Image.open(INPUT).convert("RGB")
    found = components(source)
    large = sorted((item for item in found if item[1][1] < 430),
                   key=lambda item: item[1][0])
    medium = sorted((item for item in found if 430 <= item[1][1] < 760),
                    key=lambda item: item[1][0])
    small = sorted((item for item in found if item[1][1] >= 760),
                   key=lambda item: item[1][0])
    if tuple(map(len, (large, medium, small))) != (4, 4, 4):
        raise RuntimeError("expected four approved concept masses per size row")

    indexed = Image.new("P", (SHEET_W, SHEET_H), 0)
    indexed.putpalette([value for rgb in FRONT16 for value in rgb]
                       + [0] * (768 - 48))

    # Five 48x40 runtime frames retain the established cache contract. Each
    # orientation comes from concept-resolution material and receives its own
    # exact-size reduction; finished native pixels are never rotated.
    large_a = reduce_component(source, large[0][1], (48, 40), 1)
    large_b = reduce_component(source, large[3][1], (48, 40), 1)
    for x, frame in zip((0, 48, 96, 144, 192), (
            large_a, reduce_pose(source, large[0][1], 13, (48, 40)),
            large_b, reduce_pose(source, large[3][1], -11, (48, 40)),
            damaged(large_a))):
        indexed.paste(frame, (x, 0))

    # Three genuinely different small masses, each with one independently
    # reduced tumble counterpart. Their legacy runtime row remains y=20.
    for variant, angle in enumerate((18, -17, 14)):
        shard = reduce_component(source, small[variant][1], (16, 16), 1)
        turn = reduce_pose(source, small[variant][1], angle, (16, 16))
        indexed.paste(shard, (240 + variant * 32, 20))
        indexed.paste(turn, (256 + variant * 32, 20))

    # A carved medium ruin mass supplies the existing 32x40 pillar role. The
    # two cells remain a controlled pose pair rather than native rotations.
    indexed.paste(reduce_component(source, medium[0][1], (32, 40), 1),
                  (336, 0))
    indexed.paste(reduce_pose(source, medium[0][1], -12, (32, 40)),
                  (368, 0))

    mask_row_bytes=(SHEET_W+7)//8
    mask=bytearray(mask_row_bytes*SHEET_H)
    pixels=indexed.load()
    for y in range(SHEET_H):
        for x in range(SHEET_W):
            if pixels[x,y]:
                mask[y*mask_row_bytes+(x>>3)]|=0x80>>(x&7)

    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    PREVIEW.parent.mkdir(parents=True, exist_ok=True)
    save_spbm(OUTPUT,indexed,FRONT16,depth=4,mask=bytes(mask))
    indexed.resize((SHEET_W*4,SHEET_H*4),Image.Resampling.NEAREST).save(PREVIEW)
    print(f"wrote {OUTPUT}")
    print(f"wrote {PREVIEW}")


if __name__ == "__main__":
    main()
