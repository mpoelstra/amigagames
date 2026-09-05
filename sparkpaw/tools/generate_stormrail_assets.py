#!/usr/bin/env python3
"""Build the focused Stormrail boarding family and retained drone row."""

from pathlib import Path
from PIL import Image, ImageChops, ImageDraw

from generate_runtime_assets import FRONT16, nearest_index, save_spbm
from generate_stormrail_v6_native_family import native_pair, to_front16

ROOT = Path(__file__).resolve().parents[1]
DRONE_SOURCE = ROOT / "assets/concept/sparkpaw-stormrail-runtime-family-v3-chroma.png"
PLAYER = ROOT / "assets/sprites/sparkpaw-48x48-aga16-source.png"
COMPACT_COMBINED = ROOT / "assets/concept/sparkpaw-stormrail-compact-cockpit-study-v5.png"
PREVIEW = ROOT / "assets/concept/sparkpaw-stormrail-runtime-family-v8-aga16.png"
COMPACT_MASTER = ROOT / "assets/sprites/stormrail-compact-flight-aga16-v2.png"
OUTPUT = ROOT / "assets/runtime/stormrail-family.spbm"

# Visible craft: 104x46. The taller transparent cell gives canonical Paw room
# during contact/settle; it does not enlarge the flight silhouette.
VEHICLE_W, VEHICLE_H = 112, 88
CRAFT_X, CRAFT_Y = 4, 42
DRONE_W, DRONE_H = 48, 32
COMPACT_W, COMPACT_H = 80, 32
SHEET_W, SHEET_H = 640, 120


def keyed_crop(source: Image.Image, box: tuple[int, int, int, int],
               size: tuple[int, int]) -> Image.Image:
    crop = source.crop(box).convert("RGBA")
    pixels = crop.load()
    for y in range(crop.height):
        for x in range(crop.width):
            r, g, b, _ = pixels[x, y]
            if r > 215 and b > 170 and g < 100:
                pixels[x, y] = (0, 0, 0, 0)
    crop.thumbnail(size, Image.Resampling.LANCZOS)
    canvas = Image.new("RGBA", size, (0, 0, 0, 0))
    canvas.alpha_composite(crop, ((size[0] - crop.width) // 2,
                                  size[1] - crop.height))
    return to_front16(canvas)


def pilot_delta(empty: Image.Image, occupied: Image.Image) -> Image.Image:
    layer = Image.new("RGBA", occupied.size, (0, 0, 0, 0))
    ep, op, lp = empty.load(), occupied.load(), layer.load()
    for y in range(occupied.height):
        for x in range(occupied.width):
            if op[x, y] != ep[x, y]:
                lp[x, y] = op[x, y]
    return layer


def native_component(source: Image.Image, size: tuple[int, int]) -> Image.Image:
    """Reduce colour and silhouette independently to an exact native cell."""
    colour = source.resize(size, Image.Resampling.LANCZOS)
    silhouette = source.getchannel("A").resize(size, Image.Resampling.NEAREST)
    colour.putalpha(silhouette.point(lambda value: 255 if value >= 96 else 0))
    return to_front16(colour)


def neutral_backdrop_cutout(source: Image.Image) -> Image.Image:
    """Remove ImageGen's baked white/grey review grid from one source."""
    result = source.convert("RGBA")
    pixels = result.load()
    for y in range(result.height):
        for x in range(result.width):
            r, g, b, _ = pixels[x, y]
            if max(r, g, b) - min(r, g, b) <= 5 and r >= 220:
                pixels[x, y] = (0, 0, 0, 0)
    box = result.getchannel("A").getbbox()
    if box is None:
        raise RuntimeError("compact pilot source has no foreground")
    return result.crop(box)


def vehicle_family() -> list[Image.Image]:
    empty, occupied = native_pair()
    player_sheet = Image.open(PLAYER).convert("RGBA")
    rider_id = 16
    player = player_sheet.crop(((rider_id % 4) * 48,
                                (rider_id // 4) * 48,
                                (rider_id % 4 + 1) * 48,
                                (rider_id // 4 + 1) * 48))
    player = to_front16(player)
    pilot = pilot_delta(empty, occupied)

    frames = []
    base = Image.new("RGBA", (VEHICLE_W, VEHICLE_H), (0, 0, 0, 0))
    base.alpha_composite(empty, (CRAFT_X, CRAFT_Y))
    frames.append(base)

    for player_x, player_y in ((33, 3), (34, 18)):
        frame = Image.new("RGBA", (VEHICLE_W, VEHICLE_H), (0, 0, 0, 0))
        frame.alpha_composite(player, (player_x, player_y))
        frame.alpha_composite(empty, (CRAFT_X, CRAFT_Y))
        frames.append(frame)

    sink = base.copy()
    sink.alpha_composite(pilot, (CRAFT_X, CRAFT_Y - 5))
    frames.append(sink)

    flight = Image.new("RGBA", (VEHICLE_W, VEHICLE_H), (0, 0, 0, 0))
    flight.alpha_composite(occupied, (CRAFT_X, CRAFT_Y))
    frames.append(flight)
    return frames


def compact_flight(frame_empty: Image.Image, frame_occupied: Image.Image) -> Image.Image:
    """Build the flight ship alone from the accepted empty native craft.

    Sparkpaw is a mode-owned attached hardware sprite in flight.  Keeping the
    cockpit empty here avoids palette loss, alpha holes and a second Bob pass.
    """
    del frame_occupied
    bounds = frame_empty.getchannel("A").getbbox()
    if bounds is None:
        raise RuntimeError("empty Stormrail craft has no foreground")
    source = frame_empty.crop(bounds)
    # Leave two native rows of vertical safety while retaining the complete
    # eighty-pixel shooter silhouette and its accepted material clusters.
    scale = min(COMPACT_W / source.width, (COMPACT_H - 2) / source.height)
    width = max(1, round(source.width * scale))
    height = max(1, round(source.height * scale))
    combined = source.resize((width, height), Image.Resampling.NEAREST)
    combined.putalpha(combined.getchannel("A").point(
        lambda value: 255 if value >= 96 else 0))
    combined = to_front16(combined)
    pixels = combined.load()
    for y in range(combined.height):
        for x in range(combined.width):
            if pixels[x, y][:3] == FRONT16[2]:
                pixels[x, y] = FRONT16[3] + (pixels[x, y][3],)
    result = Image.new("RGBA", (COMPACT_W, COMPACT_H), (0, 0, 0, 0))
    result.alpha_composite(combined, ((COMPACT_W - width) // 2,
                                      (COMPACT_H - height) // 2))
    return result


def main() -> None:
    vehicle_frames = vehicle_family()
    source = Image.open(DRONE_SOURCE).convert("RGB")
    drones = [
        keyed_crop(source, (190, 525, 460, 735), (DRONE_W, DRONE_H)),
        keyed_crop(source, (655, 515, 920, 750), (DRONE_W, DRONE_H)),
        keyed_crop(source, (1130, 500, 1460, 750), (DRONE_W, DRONE_H)),
    ]
    rgba = Image.new("RGBA", (SHEET_W, SHEET_H), (0, 0, 0, 0))
    for index, part in enumerate(vehicle_frames):
        rgba.alpha_composite(part, (index * VEHICLE_W, 0))
    compact = compact_flight(vehicle_frames[0], vehicle_frames[4])
    COMPACT_MASTER.parent.mkdir(parents=True, exist_ok=True)
    compact.save(COMPACT_MASTER)
    rgba.alpha_composite(compact, (VEHICLE_W * 5, 0))
    for index, part in enumerate(drones):
        rgba.alpha_composite(part, (index * DRONE_W, VEHICLE_H))

    indexed = Image.new("P", rgba.size, 0)
    indexed.putpalette([value for rgb in FRONT16 for value in rgb]
                       + [0] * (768 - 48))
    mask_row_bytes = (SHEET_W + 7) // 8
    mask = bytearray(mask_row_bytes * SHEET_H)
    src, dst = rgba.load(), indexed.load()
    for y in range(SHEET_H):
        for x in range(SHEET_W):
            r, g, b, a = src[x, y]
            if a < 96:
                continue
            dst[x, y] = nearest_index((r, g, b), FRONT16, avoid_zero=True)
            mask[y * mask_row_bytes + (x >> 3)] |= 0x80 >> (x & 7)
    PREVIEW.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    indexed.save(PREVIEW)
    save_spbm(OUTPUT, indexed, FRONT16, depth=4, mask=bytes(mask))
    print(f"wrote {PREVIEW}")
    print(f"wrote {COMPACT_MASTER}")
    print(f"wrote {OUTPUT}")


if __name__ == "__main__":
    main()
