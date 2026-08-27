#!/usr/bin/env python3
"""Create Sparkpaw project icons with a shared NewIcons cover layer."""
from __future__ import annotations

import struct
import sys
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
AMIGAINFO = ROOT / ".toolchain" / "amigainfo"
NEWICON_SOURCE = ROOT / "assets" / "concept" / "sparkpaw-newicon-cover-source-v1.png"
NEWICON_WIDTH = 86
NEWICON_HEIGHT = 93
NEWICON_COLORS = 34
NEWICON_SENTINEL = "*** DON'T EDIT THE FOLLOWING LINES!! ***"
# Standard OS 2.x/3.x Workbench pens. The fallback deliberately uses only
# these eight stable pens: unlike a 16-colour RomIcon it remains coherent when
# the screen has no FullPalette/RomIcon pen setup. NewIcons-capable systems use
# the separate embedded 34-colour layer.
WORKBENCH_PALETTE = (
    (149, 149, 149), (0, 0, 0), (255, 255, 255), (59, 103, 162),
    (123, 123, 123), (175, 175, 175), (170, 144, 124), (255, 169, 151),
)


def require(path: Path, description: str) -> None:
    if not path.is_file():
        raise SystemExit(f"missing {description}: {path}")


def load_amigainfo_api():
    if str(AMIGAINFO) not in sys.path:
        sys.path.insert(0, str(AMIGAINFO))
    from amigainfo import load, save
    from amigainfo.models import IconType, NewIconImage, NewIconImages
    return load, save, IconType, NewIconImage, NewIconImages


def crop_icon_source() -> Image.Image:
    require(NEWICON_SOURCE, "Sparkpaw icon cover source")
    image = Image.open(NEWICON_SOURCE).convert("RGB")
    target_ratio = NEWICON_WIDTH / NEWICON_HEIGHT
    source_ratio = image.width / image.height
    if source_ratio > target_ratio:
        crop_width = round(image.height * target_ratio)
        left = (image.width - crop_width) // 2
        image = image.crop((left, 0, left + crop_width, image.height))
    else:
        crop_height = round(image.width / target_ratio)
        top = (image.height - crop_height) // 2
        image = image.crop((0, top, image.width, top + crop_height))
    return image.resize((NEWICON_WIDTH, NEWICON_HEIGHT), Image.Resampling.LANCZOS)


def base_icon_bytes() -> bytes:
    """Build an 86x93 three-bitplane standard Workbench fallback."""
    palette_image = Image.new("P", (1, 1))
    flat_palette = [channel for rgb in WORKBENCH_PALETTE for channel in rgb]
    palette_image.putpalette(flat_palette + [0] * (768 - len(flat_palette)))
    indexed = crop_icon_source().quantize(
        palette=palette_image,
        dither=Image.Dither.NONE,
    )
    rows = list(indexed.getdata())
    words_per_row = (NEWICON_WIDTH + 15) // 16
    planar = bytearray()
    for plane in range(3):
        for y in range(NEWICON_HEIGHT):
            for word_index in range(words_per_row):
                word = 0
                for bit in range(16):
                    x = word_index * 16 + bit
                    if x < NEWICON_WIDTH and rows[y * NEWICON_WIDTH + x] & (1 << plane):
                        word |= 1 << (15 - bit)
                planar += struct.pack(">H", word)
    gadget = struct.pack(
        ">IhhhhHHHIIIiIHI", 0, 0, 0, NEWICON_WIDTH, NEWICON_HEIGHT,
        0x0004, 0x0001, 0x0001,
        1, 0, 0, 0, 0, 0, 1,
    )
    disk_object = (struct.pack(">HH", 0xE310, 1) + gadget +
                   struct.pack(">BBIIiiIIi", 3, 0, 0, 0, -1, -1, 0, 0, 65536))
    image_header = struct.pack(
        ">hhhhhIBBI", 0, 0, NEWICON_WIDTH, NEWICON_HEIGHT, 3, 1, 0x07, 0, 0
    )
    return disk_object + image_header + planar


def newicon_image(NewIconImage):
    indexed = crop_icon_source().quantize(
        colors=NEWICON_COLORS,
        method=Image.Quantize.MEDIANCUT,
        dither=Image.Dither.FLOYDSTEINBERG,
    )
    palette_bytes = indexed.getpalette()[:NEWICON_COLORS * 3]
    palette = [tuple(palette_bytes[i:i + 3]) for i in range(0, len(palette_bytes), 3)]
    return NewIconImage(
        width=NEWICON_WIDTH,
        height=NEWICON_HEIGHT,
        transparent=False,
        palette=palette,
        pixel_data=list(indexed.getdata()),
    )


def make_project_icon(default_tool: str, tooltypes: list[str]) -> bytes:
    load, save, IconType, NewIconImage, NewIconImages = load_amigainfo_api()
    icon = load(base_icon_bytes())
    icon.type = IconType.PROJECT
    icon.default_tool = default_tool
    icon.tooltypes = [*tooltypes, " ", NEWICON_SENTINEL]
    icon.newicon = NewIconImages(normal=newicon_image(NewIconImage))
    return save(icon)


def make_readme_icon() -> bytes:
    load, save, IconType, _, _ = load_amigainfo_api()
    icon = load(base_icon_bytes())
    icon.type = IconType.PROJECT
    icon.default_tool = "MultiView"
    icon.tooltypes = []
    return save(icon)
