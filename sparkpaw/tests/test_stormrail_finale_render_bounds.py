#!/usr/bin/env python3
"""Prove the Gate-6 renderer crop removes transparent pixels only."""

from pathlib import Path
import re


ACTOR_WIDTH = 80
ACTOR_HEIGHT = 46
EXPECTED_BOUNDS = (8, 1, 70, 45)
HEADER = Path(__file__).parents[1] / "src" / "stormrail_gate6_art.h"


def main() -> None:
    source = HEADER.read_text(encoding="ascii")
    match = re.search(
        r"stormGate6ArtPacked\[[^]]+\]\s*=\s*\{(.*?)\};",
        source,
        re.S,
    )
    assert match, "packed Gate-6 pixel array not found"
    values = [int(value, 0) for value in re.findall(r"0x[0-9a-fA-F]+|\b\d+\b", match.group(1))]
    packed_size = ACTOR_WIDTH * ACTOR_HEIGHT // 2
    assert len(values) >= packed_size, "Harrier packed pixel data is truncated"

    pixels = []
    for value in values[:packed_size]:
        pixels.extend((value >> 4, value & 15))
    opaque = [(index % ACTOR_WIDTH, index // ACTOR_WIDTH)
              for index, pen in enumerate(pixels) if pen]
    bounds = (
        min(x for x, _ in opaque), min(y for _, y in opaque),
        max(x for x, _ in opaque), max(y for _, y in opaque),
    )
    assert bounds == EXPECTED_BOUNDS, (bounds, EXPECTED_BOUNDS)

    left, top, right, bottom = EXPECTED_BOUNDS
    assert all(
        pen == 0
        for index, pen in enumerate(pixels)
        if not (left <= index % ACTOR_WIDTH <= right and
                top <= index // ACTOR_WIDTH <= bottom)
    )
    print("PASS: Harrier 63x45 render crop removes transparent pixels only")


if __name__ == "__main__":
    main()
