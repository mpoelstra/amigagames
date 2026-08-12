#!/usr/bin/env python3
"""Offline rigid-leg Strider walk audition; never used by the Amiga build."""

from pathlib import Path
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "assets/enemies/clockwork-storm-strider-premium-color-idle-source-v2.png"
OUT = ROOT / "build/strider-leg-rig-audition"
RUNTIME_SOURCE = (ROOT / "assets/enemies" /
                  "clockwork-storm-strider-64x64-aga15-walk-rig-v1.png")
TURN_SOURCE = (ROOT / "assets/enemies" /
               "clockwork-storm-strider-64x64-aga15-turn-v1.png")

# One complete detailed leg per depth layer. Keeping knee, ankle and foot in a
# single source piece trades bend for perfect pixel continuity at 64x64.
LEGS = {
    "near": ((350, 650, 665, 1145), (557, 774)),
    "far": ((630, 650, 940, 1145), (731, 777)),
}

ARMS = {
    "near": ((340, 485, 540, 850), (438, 555)),
    "far": ((735, 480, 935, 795), (786, 548)),
}

# Half-cycle is mirrored exactly. A small planted-side compression gives weight
# without changing the locked upper-body silhouette.
PHASES = (
    (-10, 10, 0),
    (-7, 7, 3),
    (-2, 2, 0),
    (6, -6, -2),
    (10, -10, 0),
    (7, -7, 3),
    (2, -2, 0),
    (-6, 6, -2),
)
ARM_PHASES = (-7, -4, 0, 4, 7, 4, 0, -4)

# Exact runtime foreground bank. Audition the native pixels through the same
# sixteen pens the four-plane Bob will use, rather than judging rich RGB art.
FRONT16 = (
    (0, 0, 17), (17, 17, 17), (221, 68, 17), (255, 153, 34),
    (255, 238, 170), (34, 102, 204), (51, 204, 238), (153, 68, 204),
    (55, 55, 65), (101, 98, 103), (163, 157, 158), (229, 225, 219),
    (67, 29, 100), (112, 45, 157), (166, 77, 218), (224, 35, 104),
)


def remove_green(source):
    out = source.convert("RGBA")
    pixels = out.load()
    for y in range(out.height):
        for x in range(out.width):
            r, g, b, a = pixels[x, y]
            # The source uses a softly varying green key. Cyan highlights have
            # blue >= green, so this broader dominance test removes the fringe
            # without eating the visor or joint lights.
            if g > 80 and g > r * 1.08 and g > b * 1.08:
                pixels[x, y] = (r, g, b, 0)
    return out


def cut(source, spec):
    box, pivot = spec
    return source.crop(box), (pivot[0] - box[0], pivot[1] - box[1])


def paste_rotated(canvas, part, world_pivot, angle):
    piece, pivot = part
    side = max(piece.width, piece.height) * 2
    origin = (side // 2, side // 2)
    padded = Image.new("RGBA", (side, side), (0, 0, 0, 0))
    padded.alpha_composite(piece, (origin[0] - pivot[0], origin[1] - pivot[1]))
    image = padded.rotate(angle, Image.Resampling.NEAREST,
                          center=origin, expand=False)
    canvas.alpha_composite(image, (world_pivot[0] - origin[0],
                                   world_pivot[1] - origin[1]))


def runtime_palette(source):
    """Apply the production colour bank while retaining alpha for previews."""
    out = Image.new("RGBA", source.size, (0, 0, 0, 0))
    src, dst = source.load(), out.load()
    for y in range(source.height):
        for x in range(source.width):
            red, green, blue, alpha = src[x, y]
            if alpha < 96:
                continue
            pen = min(range(1, len(FRONT16)), key=lambda index: (
                (red - FRONT16[index][0]) ** 2
                + (green - FRONT16[index][1]) ** 2
                + (blue - FRONT16[index][2]) ** 2))
            dst[x, y] = (*FRONT16[pen], 255)
    return out


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    source = remove_green(Image.open(SOURCE))
    legs = {name: cut(source, spec) for name, spec in LEGS.items()}
    arms = {name: cut(source, spec) for name, spec in ARMS.items()}

    # Fixed head/torso/pelvis are composited last to hide hip/shoulder seams.
    # Remove the dangling arm regions from this locked layer; compact shoulder
    # caps remain, so rotated complete arms tuck underneath them.
    upper = source.copy()
    alpha = upper.getchannel("A")
    mask = Image.new("L", upper.size, 0)
    d = ImageDraw.Draw(mask)
    d.rectangle((0, 0, upper.width, 810), fill=255)
    d.polygon(((340, 540), (430, 520), (520, 610), (520, 850),
               (340, 850)), fill=0)
    d.polygon(((760, 530), (850, 540), (935, 620), (935, 805),
               (760, 805)), fill=0)
    # Re-cover only the shoulder/pelvis cores after arm removal.
    d.rectangle((430, 330, 795, 810), fill=255)
    upper.putalpha(Image.composite(alpha, Image.new("L", alpha.size, 0), mask))

    high = []
    for index, (near_angle, far_angle, bob) in enumerate(PHASES):
        frame = Image.new("RGBA", source.size, (0, 0, 0, 0))
        paste_rotated(frame, legs["far"], (731, 777 + bob), far_angle)
        paste_rotated(frame, arms["far"], (786, 548 + bob),
                      -ARM_PHASES[index])
        paste_rotated(frame, legs["near"], (557, 774 + bob), near_angle)
        paste_rotated(frame, arms["near"], (438, 555 + bob),
                      ARM_PHASES[index])
        frame.alpha_composite(upper, (0, bob))
        high.append(frame)

    # Find one family crop and one family scale; never normalize per pose.
    union = None
    for frame in high:
        box = frame.getbbox()
        union = box if union is None else (min(union[0], box[0]),
                                           min(union[1], box[1]),
                                           max(union[2], box[2]),
                                           max(union[3], box[3]))
    scale = min(58 / (union[2] - union[0]), 56 / (union[3] - union[1]))
    frames = []
    for frame in high:
        actor = frame.crop(union)
        actor = actor.resize((round(actor.width * scale), round(actor.height * scale)),
                             Image.Resampling.NEAREST)
        native = Image.new("RGBA", (64, 64), (0, 0, 0, 0))
        native.alpha_composite(actor, ((64 - actor.width) // 2, 62 - actor.height))
        frames.append(runtime_palette(native))

    sheet = Image.new("RGBA", (64 * 8, 64), (0, 0, 0, 0))
    for i, frame in enumerate(frames):
        sheet.alpha_composite(frame, (i * 64, 0))
    sheet.save(OUT / "strider-leg-rig-sheet.png")
    sheet.save(RUNTIME_SOURCE)

    # One planted front pose is held only by the explicit direction-change
    # state. Normalize this separate pose family to the same 56px actor height
    # and line-62 foot baseline as the reviewed side loop.
    turn_high = remove_green(Image.open(ROOT / "assets/enemies" /
        "clockwork-storm-strider-premium-turn-source.png"))
    turn_actor = turn_high.crop(turn_high.getbbox())
    turn_scale = 56 / turn_actor.height
    turn_actor = turn_actor.resize((round(turn_actor.width * turn_scale),
                                    round(turn_actor.height * turn_scale)),
                                   Image.Resampling.NEAREST)
    turn = Image.new("RGBA", (64, 64), (0, 0, 0, 0))
    turn.alpha_composite(turn_actor, ((64 - turn_actor.width) // 2,
                                      62 - turn_actor.height))
    runtime_palette(turn).save(TURN_SOURCE)
    contact = Image.new("RGBA", (64 * 4 * 4, 64 * 2 * 4), (0, 0, 17, 255))
    contact_draw = ImageDraw.Draw(contact)
    for i, frame in enumerate(frames):
        large_frame = frame.resize((256, 256), Image.Resampling.NEAREST)
        x = (i & 3) * 256
        y = (i >> 2) * 256
        contact.alpha_composite(large_frame, (x, y))
        contact_draw.text((x + 8, y + 8), str(i), fill=(255, 255, 255, 255))
    contact.save(OUT / "strider-leg-rig-contact.png")

    # Alpha-silhouette deltas expose discontinuities independent of palette.
    deltas = []
    for i, frame in enumerate(frames):
        a = frame.getchannel("A")
        b = frames[(i + 1) & 7].getchannel("A")
        changed = sum(1 for av, bv in zip(a.getdata(), b.getdata())
                      if (av >= 96) != (bv >= 96))
        deltas.append(changed)
    (OUT / "strider-leg-rig-deltas.txt").write_text(
        "\n".join(f"{i}->{(i + 1) & 7}: {value}" for i, value in enumerate(deltas)) + "\n")
    large = [f.resize((384, 384), Image.Resampling.NEAREST) for f in frames]
    large[0].save(OUT / "strider-leg-rig-loop.gif", save_all=True,
                  append_images=large[1:], duration=125, loop=0, disposal=2)
    print(OUT / "strider-leg-rig-sheet.png")
    print(OUT / "strider-leg-rig-loop.gif")
    print(OUT / "strider-leg-rig-deltas.txt")


if __name__ == "__main__":
    main()
