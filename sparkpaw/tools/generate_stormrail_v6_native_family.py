#!/usr/bin/env python3
"""Build the selected head-only Skimmer as an exact native FRONT16 proof."""

from pathlib import Path

from PIL import Image, ImageChops, ImageDraw

from generate_runtime_assets import FRONT16, nearest_index

ROOT = Path(__file__).resolve().parents[1]
OCCUPIED = ROOT / "assets/concept/sparkpaw-stormrail-skimmer-v6-head-only-chroma.png"
EMPTY = ROOT / "assets/concept/sparkpaw-stormrail-skimmer-v6-empty-chroma.png"
BACKGROUND = ROOT / "assets/concept/sparkpaw-stormrail-rear-aga8-preview-v3.png"
PLAYER = ROOT / "assets/sprites/sparkpaw-48x48-aga16-source.png"
SHEET = ROOT / "assets/concept/sparkpaw-stormrail-skimmer-v6-native-family-aga16.png"
REVIEW = ROOT / "assets/concept/sparkpaw-stormrail-skimmer-v6-native-family-review.png"
REVIEW_3X = ROOT / "assets/concept/sparkpaw-stormrail-skimmer-v6-native-family-review-3x.png"
BOARDING = ROOT / "assets/concept/sparkpaw-stormrail-skimmer-v6-boarding-storyboard.png"
BOARDING_3X = ROOT / "assets/concept/sparkpaw-stormrail-skimmer-v6-boarding-storyboard-3x.png"

FRAME_W, FRAME_H = 104, 46
CRAFT_H = 44


def key_magenta(path: Path) -> Image.Image:
    image = Image.open(path).convert("RGBA")
    pixels = image.load()
    for y in range(image.height):
        for x in range(image.width):
            r, g, b, _ = pixels[x, y]
            if r > 220 and b > 180 and g < 80:
                pixels[x, y] = (0, 0, 0, 0)
    return image


def to_front16(source: Image.Image) -> Image.Image:
    result = Image.new("RGBA", source.size, (0, 0, 0, 0))
    src, dst = source.load(), result.load()
    for y in range(source.height):
        for x in range(source.width):
            r, g, b, a = src[x, y]
            if a < 96:
                continue
            dst[x, y] = FRONT16[nearest_index((r, g, b), FRONT16,
                                              avoid_zero=True)] + (255,)
    return result


def add_hull_accents(frame: Image.Image) -> Image.Image:
    """Restore readable AGA material separation lost during downscaling."""
    result = frame.copy()
    accents = Image.new("RGBA", frame.size, (0, 0, 0, 0))
    draw = ImageDraw.Draw(accents)

    # A restrained inset flank panel breaks up the near-black body without
    # changing the fast wedge silhouette or competing with Sparkpaw's head.
    draw.polygon([(39, 27), (69, 27), (80, 30), (72, 34), (42, 34), (35, 31)],
                 fill=FRONT16[8] + (255,))
    draw.polygon([(40, 28), (66, 28), (76, 30), (69, 33), (42, 33), (36, 31)],
                 fill=FRONT16[9] + (255,))
    draw.line([(41, 28), (65, 28), (74, 30)], fill=FRONT16[10] + (255,),
              width=1)

    # Copper speed stripe plus one cyan powered-cell tick tie the flank back
    # to the nose, engine and Sparkpaw without turning it into visual noise.
    draw.line([(39, 31), (57, 32), (70, 31)], fill=FRONT16[3] + (255,),
              width=1)
    draw.line([(48, 33), (57, 33)], fill=FRONT16[2] + (255,), width=1)
    draw.line([(74, 32), (78, 31)], fill=FRONT16[6] + (255,), width=1)

    # Never paint beyond the already-approved alpha silhouette.
    accents.putalpha(ImageChops.multiply(accents.getchannel("A"),
                                         frame.getchannel("A")))
    result.alpha_composite(accents)
    return result


def native_pair() -> tuple[Image.Image, Image.Image]:
    occupied = key_magenta(OCCUPIED)
    empty = key_magenta(EMPTY)
    canonical = occupied.getchannel("A").getbbox()
    if canonical is None:
        raise RuntimeError("occupied source contains no keyed craft")

    # Both ImageGen sources share one canvas and hull placement. Cropping both
    # through the occupied bounds guarantees identical scale and registration;
    # the empty cockpit's missing head must not make its hull grow.
    occupied = occupied.crop(canonical)
    empty = empty.crop(canonical)
    occupied.thumbnail((FRAME_W, CRAFT_H), Image.Resampling.LANCZOS)
    empty = empty.resize(occupied.size, Image.Resampling.LANCZOS)

    frames = []
    for source in (empty, occupied):
        frame = Image.new("RGBA", (FRAME_W, FRAME_H), (0, 0, 0, 0))
        x = (FRAME_W - source.width) // 2
        y = FRAME_H - source.height
        frame.alpha_composite(to_front16(source), (x, y))
        frames.append(frame)

    # The separate cockpit concept has a red-heavy orange ramp. Normalize only
    # pixels introduced by Sparkpaw to the established player/HUD fur roles:
    # retain the darkest red-orange shadow, but map mid-value fur to the normal
    # FRONT16 orange. Shape, alpha, cream muzzle, eyes and hull remain exact.
    occupied_pixels = occupied.load()
    empty_pixels = empty.load()
    normalized_pixels = frames[1].load()
    frame_x = (FRAME_W - occupied.width) // 2
    frame_y = FRAME_H - occupied.height
    for py in range(occupied.height):
        for px in range(occupied.width):
            rgba = occupied_pixels[px, py]
            if rgba[3] < 96 or rgba == empty_pixels[px, py]:
                continue
            if (nearest_index(rgba[:3], FRONT16, avoid_zero=True) == 2 and
                    sum(rgba[:3]) >= 280):
                normalized_pixels[frame_x + px, frame_y + py] = (
                    FRONT16[3] + (255,))

    frames[0] = add_hull_accents(frames[0])

    # ImageGen supplied the empty cockpit, but it is not allowed to redraw the
    # animated hull. Make that empty frame the canonical vehicle and transplant
    # only the visible head/scarf/cockpit opening from the occupied reduction.
    # Everything outside this small pod rectangle is therefore byte-identical.
    stable_occupied = frames[0].copy()
    pod_box = (44, 0, 68, 25)
    pod = frames[1].crop(pod_box)
    stable_occupied.alpha_composite(pod, pod_box[:2])
    return frames[0], stable_occupied


def save_indexed_sheet(frames: tuple[Image.Image, Image.Image]) -> None:
    rgba = Image.new("RGBA", (FRAME_W * 2, FRAME_H), (0, 0, 0, 0))
    for index, frame in enumerate(frames):
        rgba.alpha_composite(frame, (index * FRAME_W, 0))
    indexed = Image.new("P", rgba.size, 0)
    indexed.putpalette([value for rgb in FRONT16 for value in rgb]
                       + [0] * (768 - 48))
    src, dst = rgba.load(), indexed.load()
    for y in range(rgba.height):
        for x in range(rgba.width):
            r, g, b, a = src[x, y]
            if a >= 96:
                dst[x, y] = nearest_index((r, g, b), FRONT16,
                                          avoid_zero=True)
    indexed.save(SHEET)


def save_review(frames: tuple[Image.Image, Image.Image]) -> None:
    rear = Image.open(BACKGROUND).convert("RGB").crop((640, 40, 960, 248))
    review = Image.new("RGB", (640, 232), (4, 5, 12))
    draw = ImageDraw.Draw(review)
    for index, (label, frame) in enumerate(zip(("EMPTY", "OCCUPIED"), frames)):
        x0 = index * 320
        review.paste(rear, (x0, 24))
        review.paste(frame, (x0 + 28, 24 + (208 - FRAME_H) // 2), frame)
        draw.text((x0 + 8, 7), f"{label} 104x46 FRONT16", fill=(190, 224, 239))
    review.save(REVIEW)
    review.resize((review.width * 3, review.height * 3),
                  Image.Resampling.NEAREST).save(REVIEW_3X)


def save_boarding_storyboard(frames: tuple[Image.Image, Image.Image]) -> None:
    rear = Image.open(BACKGROUND).convert("RGB").crop((640, 40, 960, 248))
    player_sheet = Image.open(PLAYER).convert("RGBA")
    rider_id = 16
    player = player_sheet.crop(((rider_id % 4) * 48,
                                (rider_id // 4) * 48,
                                (rider_id % 4 + 1) * 48,
                                (rider_id // 4 + 1) * 48))
    player = to_front16(player)
    labels = ("JUMP", "CONTACT", "SETTLE", "SINK", "FLIGHT")
    # Scene coordinates preserve the native 104x46 craft. Paw moves down and
    # slightly right; the hull is drawn after him to provide real cockpit
    # occlusion rather than cropped or deformed anatomy.
    player_positions = ((78, 5), (87, 19), (88, 34))
    craft_x, craft_y = 58, 58
    panel_w, panel_h, label_h = 220, 112, 20
    storyboard = Image.new("RGB", (panel_w * len(labels), label_h + panel_h),
                           (4, 5, 12))
    draw = ImageDraw.Draw(storyboard)
    pilot = Image.new("RGBA", frames[1].size, (0, 0, 0, 0))
    empty_pixels, occupied_pixels, pilot_pixels = (
        frames[0].load(), frames[1].load(), pilot.load())
    for py in range(FRAME_H):
        for px in range(FRAME_W):
            if occupied_pixels[px, py] != empty_pixels[px, py]:
                pilot_pixels[px, py] = occupied_pixels[px, py]
    for index, label in enumerate(labels):
        x0 = index * panel_w
        storyboard.paste(rear.crop((0, 48, panel_w, 160)), (x0, label_h))
        scene = Image.new("RGBA", (panel_w, panel_h), (0, 0, 0, 0))
        if index < len(player_positions):
            px, py = player_positions[index]
            scene.alpha_composite(player, (px, py))
            scene.alpha_composite(frames[0], (craft_x, craft_y))
        elif index == 3:
            scene.alpha_composite(frames[0], (craft_x, craft_y))
            scene.alpha_composite(pilot, (craft_x, craft_y - 5))
        else:
            scene.alpha_composite(frames[1], (craft_x, craft_y))
        storyboard.paste(scene, (x0, label_h), scene)
        draw.text((x0 + 7, 5), label, fill=(190, 224, 239))
    storyboard.save(BOARDING)
    storyboard.resize((storyboard.width * 3, storyboard.height * 3),
                      Image.Resampling.NEAREST).save(BOARDING_3X)


def main() -> None:
    frames = native_pair()
    SHEET.parent.mkdir(parents=True, exist_ok=True)
    save_indexed_sheet(frames)
    save_review(frames)
    save_boarding_storyboard(frames)
    print(f"wrote {SHEET}")
    print(f"wrote {REVIEW}")
    print(f"wrote {REVIEW_3X}")
    print(f"wrote {BOARDING}")
    print(f"wrote {BOARDING_3X}")


if __name__ == "__main__":
    main()
