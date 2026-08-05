#!/usr/bin/env python3
"""Turn the approved options concept into a clean dynamic 320x256 ILBM."""
import struct
import sys
from pathlib import Path
from PIL import Image, ImageDraw

W, H = 320, 256
PALETTE = [
    (0, 0, 5),       (14, 18, 28),    (34, 42, 58),    (57, 66, 82),
    (2, 13, 39),     (4, 27, 70),     (5, 52, 125),    (21, 91, 180),
    (143, 28, 25),   (220, 62, 35),   (105, 60, 28),   (54, 31, 21),
    (2, 5, 17),      (92, 101, 112),  (158, 165, 166), (222, 226, 217),
]


def chunk(name, data):
    return name + struct.pack(">I", len(data)) + data + (
        b"\0" if len(data) & 1 else b""
    )


def clean_concept(source):
    image = Image.open(source).convert("RGB")
    draw = ImageDraw.Draw(image)
    # Remove baked labels/values while preserving the seven metal row frames.
    for row in range(7):
        top = 330 + row * 91
        draw.rectangle((245, top, 1212, top + 61), fill=(3, 18, 48))
    # Remove the baked footer instructions; C renders the live controls.
    draw.rectangle((330, 985, 1125, 1065), fill=(1, 5, 18))
    return image


def convert(source, output):
    src = clean_concept(source)
    scale = min(W / src.width, H / src.height)
    resized = src.resize(
        (round(src.width * scale), round(src.height * scale)),
        Image.Resampling.LANCZOS,
    )
    canvas = Image.new("RGB", (W, H), PALETTE[0])
    canvas.paste(resized, ((W - resized.width) // 2, (H - resized.height) // 2))

    # The approved concept originally contained seven rows. Rebuild only the
    # inner menu rack at final Amiga resolution so TEAM SIZE gets a genuine
    # eighth framed row while the illustrated stadium cabinet stays intact.
    draw = ImageDraw.Draw(canvas)
    draw.rectangle((44, 74, 276, 220), fill=PALETTE[12])
    for row in range(8):
        top = 77 + row * 18
        draw.rectangle((46, top, 274, top + 15), fill=PALETTE[2])
        draw.rectangle((47, top + 1, 273, top + 14), fill=PALETTE[3])
        draw.rectangle((49, top + 3, 271, top + 13), fill=PALETTE[4])
        draw.line((49, top + 2, 271, top + 2), fill=PALETTE[7])

    palette_image = Image.new("P", (1, 1))
    flat = [component for color in PALETTE for component in color]
    palette_image.putpalette(flat + [0] * (768 - len(flat)))
    image = canvas.quantize(
        palette=palette_image,
        dither=Image.Dither.FLOYDSTEINBERG,
    )
    pixels = list(image.getdata())
    body = bytearray()
    for y in range(H):
        row = pixels[y * W:(y + 1) * W]
        for plane in range(4):
            for bx in range(W // 8):
                value = 0
                for bit in range(8):
                    value |= ((row[bx * 8 + bit] >> plane) & 1) << (7 - bit)
                body.append(value)
    bmhd = struct.pack(">HHhhBBBBHBBhh", W, H, 0, 0, 4, 0, 0, 0, 0, 10, 11, W, H)
    cmap = bytes(flat)
    form = b"ILBM" + chunk(b"BMHD", bmhd) + chunk(b"CMAP", cmap) + chunk(b"BODY", body)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(b"FORM" + struct.pack(">I", len(form)) + form)
    image.convert("RGB").save(output.with_suffix(".png"))
    print("Wrote", output)


if __name__ == "__main__":
    convert(Path(sys.argv[1]), Path(sys.argv[2]))
