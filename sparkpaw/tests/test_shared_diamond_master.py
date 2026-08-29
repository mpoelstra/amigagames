#!/usr/bin/env python3
"""Require HUD and world diamonds to consume one semantic native master."""

from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
RUNTIME = ROOT / "assets" / "runtime"

DIAMOND_W = 16
DIAMOND_H = 21
HUD_X = 207
HUD_Y = 14

EMPTY = 0
CONTOUR = 1
CREAM = 2
CYAN = 3
SHADE = 4

WORLD_ROLES = {0: EMPTY, 1: CONTOUR, 4: CREAM, 6: CYAN, 5: SHADE}
HUD_ROLES = {1: EMPTY, 2: CONTOUR, 4: CREAM, 7: CYAN, 3: SHADE}


def decode_spbm(name: str) -> tuple[list[list[int]], list[list[int]] | None]:
    data = (RUNTIME / name).read_bytes()
    assert data[:4] == b"SPBM"
    width,height,depth,masked,row_bytes = struct.unpack(">HHBBH",data[4:12])
    bitmap_at = 12 + (1 << depth) * 3
    plane_bytes = row_bytes * height

    def decode(at: int, planes: int) -> list[list[int]]:
        pixels = [[0 for _ in range(width)] for _ in range(height)]
        for plane in range(planes):
            for y in range(height):
                row_at = at + plane * plane_bytes + y * row_bytes
                for x in range(width):
                    if data[row_at + x // 8] & (0x80 >> (x & 7)):
                        pixels[y][x] |= 1 << plane
        return pixels

    pixels = decode(bitmap_at,depth)
    mask = decode(bitmap_at + depth * plane_bytes,1) if masked else None
    return pixels,mask


world,world_mask = decode_spbm("sparkpaw-diamond.spbm")
hud,_ = decode_spbm("sparkpaw-hud-base.spbm")

assert len(world) == DIAMOND_H and len(world[0]) == 32
assert world_mask is not None

world_roles = []
hud_roles = []
for y in range(DIAMOND_H):
    world_row = []
    hud_row = []
    for x in range(DIAMOND_W):
        assert world[y][x] in WORLD_ROLES, (x,y,world[y][x])
        assert hud[HUD_Y+y][HUD_X+x] in HUD_ROLES, (
            x,y,hud[HUD_Y+y][HUD_X+x]
        )
        world_row.append(WORLD_ROLES[world[y][x]])
        hud_row.append(HUD_ROLES[hud[HUD_Y+y][HUD_X+x]])
        assert bool(world_mask[y][x]) == (world_row[-1] != EMPTY)
    world_roles.append(tuple(world_row))
    hud_roles.append(tuple(hud_row))

assert tuple(world_roles) == tuple(hud_roles)
assert all(world[y][x] == 0 and world_mask[y][x] == 0
           for y in range(DIAMOND_H) for x in range(DIAMOND_W,32))
assert all(role == EMPTY or world[y][x] != 0
           for y,row in enumerate(world_roles) for x,role in enumerate(row))

print("PASS: HUD and world diamonds share exact native mask and facet roles")
