#!/usr/bin/env python3
"""Author the Debris 2.0 feasibility family directly on the native AGA grid.

This is review art only. It deliberately does not replace the Gate-4 runtime
SPBM or change any renderer/gameplay contract.
"""

from pathlib import Path
from PIL import Image, ImageDraw

from generate_runtime_assets import FRONT16

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "assets/concept/sparkpaw-stormrail-debris-native-v1-aga16.png"
REVIEW = ROOT / "assets/concept/sparkpaw-stormrail-debris-native-v1-review-4x.png"
SHEET_W, SHEET_H = 224, 64


def poly(draw, points, pen):
    draw.polygon(points, fill=pen)


def pixel_cluster(draw, points, pen):
    for x, y in points:
        draw.point((x, y), fill=pen)


def arch_spring(im, ox, oy):
    """Large severed arch spring: broken mass first, carved curve second."""
    d = ImageDraw.Draw(im)
    poly(d, [(ox+2,oy+8),(ox+8,oy+3),(ox+18,oy+1),(ox+28,oy+3),
             (ox+37,oy+2),(ox+45,oy+8),(ox+47,oy+18),(ox+44,oy+27),
             (ox+39,oy+30),(ox+36,oy+37),(ox+27,oy+39),(ox+21,oy+35),
             (ox+14,oy+38),(ox+9,oy+33),(ox+3,oy+34),(ox,oy+27),
             (ox+3,oy+20),(ox,oy+15)], 9)
    poly(d, [(ox+3,oy+9),(ox+9,oy+5),(ox+18,oy+3),(ox+27,oy+5),
             (ox+36,oy+4),(ox+43,oy+9),(ox+44,oy+15),(ox+37,oy+17),
             (ox+30,oy+15),(ox+24,oy+18),(ox+15,oy+15),(ox+10,oy+19),
             (ox+4,oy+17)], 10)
    poly(d, [(ox+8,oy+6),(ox+17,oy+3),(ox+25,oy+5),(ox+20,oy+9),
             (ox+10,oy+11),(ox+4,oy+10)], 11)
    poly(d, [(ox+5,oy+20),(ox+11,oy+17),(ox+19,oy+18),(ox+25,oy+21),
             (ox+31,oy+17),(ox+39,oy+19),(ox+42,oy+24),(ox+37,oy+28),
             (ox+32,oy+34),(ox+26,oy+35),(ox+21,oy+31),(ox+15,oy+34),
             (ox+10,oy+29),(ox+4,oy+30),(ox+2,oy+26)], 8)
    # The arch recess is a material shadow, never a transparent hole.
    poly(d, [(ox+9,oy+29),(ox+12,oy+23),(ox+18,oy+20),(ox+25,oy+21),
             (ox+31,oy+25),(ox+34,oy+32),(ox+29,oy+35),(ox+25,oy+29),
             (ox+20,oy+26),(ox+15,oy+27),(ox+13,oy+33)], 12)
    d.line([(ox+11,oy+27),(ox+14,oy+22),(ox+19,oy+20),(ox+25,oy+22),
            (ox+30,oy+26)], fill=11)
    d.line([(ox+36,oy+5),(ox+43,oy+10),(ox+43,oy+17)], fill=8)
    d.line([(ox+27,oy+5),(ox+25,oy+13),(ox+29,oy+17)], fill=8)
    pixel_cluster(d, [(ox+13,oy+8),(ox+32,oy+9),(ox+7,oy+24),
                      (ox+36,oy+23),(ox+29,oy+31)], 11)
    pixel_cluster(d, [(ox+39,oy+13),(ox+38,oy+14)], 6)
    d.line([(ox+7,oy+14),(ox+11,oy+13),(ox+14,oy+15)], fill=8)
    d.line([(ox+31,oy+6),(ox+34,oy+8)], fill=11)


def buttress_chunk(im, ox, oy):
    """Large diagonal buttress/rib torn from a wall."""
    d = ImageDraw.Draw(im)
    poly(d, [(ox+2,oy+13),(ox+7,oy+7),(ox+14,oy+6),(ox+18,oy+2),
             (ox+27,oy+1),(ox+32,oy+5),(ox+41,oy+7),(ox+46,oy+14),
             (ox+44,oy+23),(ox+47,oy+29),(ox+41,oy+35),(ox+33,oy+34),
             (ox+27,oy+39),(ox+19,oy+36),(ox+13,oy+38),(ox+8,oy+33),
             (ox+2,oy+32),(ox,oy+24),(ox+3,oy+19)], 9)
    poly(d, [(ox+4,oy+14),(ox+9,oy+9),(ox+16,oy+8),(ox+20,oy+4),
             (ox+28,oy+3),(ox+32,oy+7),(ox+39,oy+9),(ox+43,oy+14),
             (ox+39,oy+19),(ox+31,oy+17),(ox+25,oy+19),(ox+18,oy+15),
             (ox+12,oy+18),(ox+5,oy+18)], 10)
    poly(d, [(ox+8,oy+10),(ox+16,oy+8),(ox+20,oy+5),(ox+27,oy+4),
             (ox+24,oy+9),(ox+17,oy+13),(ox+10,oy+14),(ox+5,oy+15)], 11)
    poly(d, [(ox+5,oy+21),(ox+12,oy+18),(ox+18,oy+19),(ox+25,oy+22),
             (ox+32,oy+18),(ox+40,oy+21),(ox+42,oy+27),(ox+38,oy+32),
             (ox+31,oy+31),(ox+27,oy+35),(ox+20,oy+33),(ox+14,oy+35),
             (ox+10,oy+31),(ox+4,oy+30),(ox+2,oy+25)], 8)
    # One broad carved rib, not a set of outlined bricks.
    poly(d, [(ox+13,oy+29),(ox+16,oy+17),(ox+22,oy+9),(ox+28,oy+6),
             (ox+32,oy+9),(ox+27,oy+13),(ox+23,oy+20),(ox+21,oy+31),
             (ox+18,oy+35)], 11)
    d.line([(ox+30,oy+8),(ox+35,oy+12),(ox+38,oy+18)], fill=8)
    d.line([(ox+6,oy+23),(ox+12,oy+21),(ox+15,oy+23)], fill=10)
    pixel_cluster(d, [(ox+8,oy+27),(ox+35,oy+26),(ox+30,oy+30)], 8)
    d.line([(ox+33,oy+10),(ox+38,oy+12),(ox+40,oy+15)], fill=8)
    d.line([(ox+5,oy+18),(ox+9,oy+17)], fill=11)


def battlement_mass(im, ox, oy, cracked=False):
    """Large battered parapet/lintel mass with a heavy fracture underside."""
    d = ImageDraw.Draw(im)
    poly(d, [(ox+1,oy+9),(ox+6,oy+5),(ox+13,oy+5),(ox+16,oy+1),
             (ox+24,oy+2),(ox+27,oy+6),(ox+34,oy+4),(ox+39,oy+7),
             (ox+45,oy+8),(ox+47,oy+15),(ox+44,oy+20),(ox+46,oy+27),
             (ox+40,oy+29),(ox+37,oy+35),(ox+29,oy+34),(ox+24,oy+39),
             (ox+16,oy+36),(ox+10,oy+38),(ox+6,oy+33),(ox+1,oy+32),
             (ox+3,oy+25),(ox,oy+20)], 9)
    poly(d, [(ox+3,oy+10),(ox+8,oy+7),(ox+15,oy+7),(ox+18,oy+3),
             (ox+23,oy+4),(ox+26,oy+8),(ox+34,oy+6),(ox+39,oy+9),
             (ox+44,oy+10),(ox+43,oy+16),(ox+35,oy+16),(ox+29,oy+18),
             (ox+21,oy+15),(ox+15,oy+18),(ox+8,oy+16),(ox+3,oy+17)], 10)
    poly(d, [(ox+7,oy+8),(ox+15,oy+7),(ox+18,oy+4),(ox+23,oy+5),
             (ox+25,oy+8),(ox+20,oy+11),(ox+11,oy+12),(ox+4,oy+11)], 11)
    poly(d, [(ox+4,oy+20),(ox+11,oy+17),(ox+18,oy+19),(ox+24,oy+16),
             (ox+30,oy+20),(ox+37,oy+17),(ox+42,oy+20),(ox+42,oy+26),
             (ox+37,oy+27),(ox+34,oy+32),(ox+28,oy+31),(ox+24,oy+35),
             (ox+18,oy+32),(ox+12,oy+35),(ox+8,oy+30),(ox+3,oy+29)], 8)
    poly(d, [(ox+8,oy+28),(ox+13,oy+23),(ox+19,oy+22),(ox+24,oy+25),
             (ox+30,oy+23),(ox+36,oy+25),(ox+34,oy+31),(ox+28,oy+30),
             (ox+24,oy+34),(ox+18,oy+31),(ox+12,oy+34)], 12)
    d.line([(ox+6,oy+20),(ox+13,oy+19),(ox+18,oy+21)], fill=10)
    d.line([(ox+32,oy+7),(ox+38,oy+10),(ox+42,oy+13)], fill=8)
    pixel_cluster(d, [(ox+9,oy+11),(ox+16,oy+8),(ox+35,oy+13)], 11)
    if cracked:
        d.line([(ox+25,oy+6),(ox+23,oy+13),(ox+27,oy+18),
                (ox+24,oy+24),(ox+27,oy+31)], fill=12, width=2)
        pixel_cluster(d, [(ox+24,oy+14),(ox+26,oy+19)], 3)


def medium_lintel(im, ox, oy):
    d = ImageDraw.Draw(im)
    poly(d, [(ox+1,oy+9),(ox+6,oy+4),(ox+14,oy+3),(ox+20,oy+5),
             (ox+27,oy+4),(ox+31,oy+9),(ox+29,oy+17),(ox+31,oy+24),
             (ox+25,oy+29),(ox+18,oy+27),(ox+13,oy+31),(ox+7,oy+27),
             (ox+2,oy+28),(ox,oy+20),(ox+3,oy+15)], 9)
    poly(d, [(ox+3,oy+10),(ox+8,oy+6),(ox+14,oy+5),(ox+20,oy+7),
             (ox+27,oy+6),(ox+29,oy+10),(ox+25,oy+14),(ox+18,oy+13),
             (ox+13,oy+16),(ox+7,oy+13),(ox+3,oy+14)], 10)
    poly(d, [(ox+7,oy+7),(ox+14,oy+5),(ox+19,oy+7),(ox+14,oy+10),
             (ox+6,oy+11)], 11)
    poly(d, [(ox+4,oy+17),(ox+10,oy+14),(ox+16,oy+17),(ox+22,oy+14),
             (ox+28,oy+17),(ox+27,oy+23),(ox+22,oy+26),(ox+17,oy+24),
             (ox+13,oy+28),(ox+8,oy+24),(ox+3,oy+25)], 8)
    d.line([(ox+9,oy+22),(ox+13,oy+18),(ox+18,oy+19)], fill=11)
    d.line([(ox+22,oy+7),(ox+27,oy+9)], fill=8)


def medium_pillar(im, ox, oy):
    d = ImageDraw.Draw(im)
    poly(d, [(ox+6,oy+2),(ox+14,oy),(ox+22,oy+3),(ox+27,oy+8),
             (ox+25,oy+14),(ox+29,oy+20),(ox+26,oy+29),(ox+19,oy+31),
             (ox+14,oy+36),(ox+7,oy+33),(ox+3,oy+27),(ox+5,oy+20),
             (ox+2,oy+14)], 9)
    poly(d, [(ox+7,oy+4),(ox+14,oy+2),(ox+21,oy+4),(ox+24,oy+8),
             (ox+21,oy+12),(ox+12,oy+11),(ox+6,oy+8)], 10)
    poly(d, [(ox+10,oy+4),(ox+15,oy+2),(ox+20,oy+4),(ox+17,oy+7),
             (ox+9,oy+8)], 11)
    poly(d, [(ox+8,oy+13),(ox+20,oy+12),(ox+23,oy+18),(ox+21,oy+27),
             (ox+17,oy+30),(ox+13,oy+27),(ox+8,oy+29),(ox+5,oy+24)], 8)
    poly(d, [(ox+12,oy+15),(ox+17,oy+13),(ox+19,oy+18),(ox+18,oy+26),
             (ox+14,oy+28),(ox+11,oy+24)], 10)
    d.line([(ox+8,oy+30),(ox+14,oy+28),(ox+20,oy+30)], fill=11)
    pixel_cluster(d, [(ox+22,oy+20),(ox+23,oy+20)], 6)


def small_chunk(im, ox, oy, variant):
    d = ImageDraw.Draw(im)
    outlines = (
        [(1,11),(3,4),(8,1),(14,4),(15,10),(11,15),(5,14)],
        [(1,5),(6,1),(13,2),(15,8),(12,14),(6,15),(2,12)],
        [(2,3),(10,1),(15,6),(14,12),(9,15),(3,13),(0,8)],
    )
    pts = [(ox+x,oy+y) for x,y in outlines[variant]]
    poly(d, pts, 9)
    if variant == 0:
        poly(d, [(ox+3,oy+10),(ox+5,oy+5),(ox+9,oy+3),(ox+13,oy+5),
                 (ox+13,oy+9),(ox+10,oy+13),(ox+5,oy+12)], 10)
        d.line([(ox+5,oy+6),(ox+9,oy+3),(ox+12,oy+5)], fill=11)
    elif variant == 1:
        poly(d, [(ox+3,oy+6),(ox+7,oy+3),(ox+12,oy+4),(ox+13,oy+8),
                 (ox+10,oy+12),(ox+6,oy+13),(ox+3,oy+10)], 10)
        poly(d, [(ox+5,oy+6),(ox+8,oy+3),(ox+11,oy+4),(ox+9,oy+7)], 11)
    else:
        poly(d, [(ox+3,oy+5),(ox+9,oy+3),(ox+13,oy+6),(ox+12,oy+11),
                 (ox+8,oy+13),(ox+3,oy+11),(ox+2,oy+8)], 10)
        d.line([(ox+4,oy+5),(ox+9,oy+3),(ox+12,oy+6)], fill=11)
        d.line([(ox+3,oy+9),(ox+8,oy+8),(ox+11,oy+10)], fill=10)
    if variant == 0:
        d.line([(ox+5,oy+13),(ox+10,oy+14),(ox+13,oy+10)], fill=8)
    elif variant == 1:
        d.line([(ox+6,oy+14),(ox+11,oy+13),(ox+14,oy+9)], fill=8)
    else:
        d.line([(ox+4,oy+12),(ox+9,oy+14),(ox+13,oy+11)], fill=8)


def main():
    indexed = Image.new("P", (SHEET_W, SHEET_H), 0)
    indexed.putpalette([value for rgb in FRONT16 for value in rgb] +
                       [0] * (768 - 48))

    arch_spring(indexed, 0, 0)
    buttress_chunk(indexed, 48, 0)
    battlement_mass(indexed, 96, 0, cracked=True)
    medium_lintel(indexed, 152, 4)
    medium_pillar(indexed, 188, 1)
    small_chunk(indexed, 152, 44, 0)
    small_chunk(indexed, 172, 44, 1)
    small_chunk(indexed, 192, 44, 2)

    indexed.info["transparency"] = 0
    SOURCE.parent.mkdir(parents=True, exist_ok=True)
    indexed.save(SOURCE)

    # Magenta exists only in this review composite, never in indexed source.
    rgba = indexed.convert("RGBA")
    alpha = Image.new("L", indexed.size, 0)
    ap = alpha.load(); px = indexed.load()
    for y in range(SHEET_H):
        for x in range(SHEET_W):
            if px[x, y]: ap[x, y] = 255
    rgba.putalpha(alpha)
    review = Image.new("RGB", (SHEET_W * 4, SHEET_H * 4), (255, 0, 255))
    review.paste(rgba.resize((SHEET_W * 4, SHEET_H * 4),
                             Image.Resampling.NEAREST), (0, 0),
                 rgba.getchannel("A").resize((SHEET_W * 4, SHEET_H * 4),
                                              Image.Resampling.NEAREST))
    review.save(REVIEW)
    print(f"wrote {SOURCE}")
    print(f"wrote {REVIEW}")


if __name__ == "__main__":
    main()
