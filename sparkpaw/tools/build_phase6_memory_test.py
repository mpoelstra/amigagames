#!/usr/bin/env python3
"""Build an isolated 2048px repeated-art Phase 6A memory test package."""
from __future__ import annotations

import json
import shutil
import struct
from pathlib import Path

from make_release import ROOT, RUNTIME_FILES, RUNTIME_README

SOURCE_WIDTH = 1280
TEST_WIDTH = 2048
HEIGHT = 256
TILE = 16
BUILD = ROOT / "build" / "test"
PACKAGE = BUILD / "Sparkpaw-Phase6A-2048"


def repeat_row(row: bytes, size: int) -> bytes:
    repeats = (size + len(row) - 1) // len(row)
    return (row * repeats)[:size]


def expand_spbm(source: Path, destination: Path) -> dict[str, int]:
    raw = source.read_bytes()
    magic, width, height, depth, has_mask, row_bytes = struct.unpack_from(
        ">4sHHBBH", raw
    )
    if magic != b"SPBM" or width != SOURCE_WIDTH or height != HEIGHT:
        raise ValueError(f"unexpected Phase 6A source geometry: {source}")
    palette_bytes = (1 << depth) * 3
    planes = depth + (1 if has_mask else 0)
    data_at = 12 + palette_bytes
    if len(raw) != data_at + planes * height * row_bytes:
        raise ValueError(f"invalid SPBM length: {source}")
    test_row_bytes = TEST_WIDTH // 8
    output = bytearray(struct.pack(
        ">4sHHBBH", magic, TEST_WIDTH, height, depth, has_mask, test_row_bytes
    ))
    output.extend(raw[12:data_at])
    for row in range(planes * height):
        start = data_at + row * row_bytes
        output.extend(repeat_row(raw[start:start + row_bytes], test_row_bytes))
    destination.write_bytes(output)
    return {"source_bytes": len(raw), "test_bytes": len(output)}


def expand_collision(source: Path, destination: Path) -> dict[str, int]:
    raw = source.read_bytes()
    source_cols, test_cols, rows = SOURCE_WIDTH // TILE, TEST_WIDTH // TILE, 14
    if len(raw) != source_cols * rows:
        raise ValueError("unexpected collision-map geometry")
    output = bytearray()
    for row in range(rows):
        cells = raw[row * source_cols:(row + 1) * source_cols]
        output.extend(repeat_row(cells, test_cols))
    destination.write_bytes(output)
    return {"source_bytes": len(raw), "test_bytes": len(output)}


def main() -> None:
    if PACKAGE.exists():
        shutil.rmtree(PACKAGE)
    BUILD.mkdir(parents=True,exist_ok=True)
    runtime = PACKAGE / "assets" / "runtime"
    runtime.mkdir(parents=True)
    shutil.copy2(ROOT / "build" / "sparkpaw-phase6-memory", PACKAGE / "Sparkpaw")
    for name in RUNTIME_FILES:
        shutil.copy2(ROOT / "assets" / "runtime" / name, runtime / name)
    sizes = {
        "storm-front.spbm": expand_spbm(
            ROOT / "assets" / "runtime" / "storm-front.spbm",
            runtime / "storm-front.spbm",
        ),
        "storm-rear.spbm": expand_spbm(
            ROOT / "assets" / "runtime" / "storm-rear.spbm",
            runtime / "storm-rear.spbm",
        ),
        "storm-collision.bin": expand_collision(
            ROOT / "assets" / "runtime" / "storm-collision.bin",
            runtime / "storm-collision.bin",
        ),
    }
    (PACKAGE / "ReadMe.txt").write_text(
        RUNTIME_README + "\nPHASE 6A MEMORY TEST\n"
        "This isolated HD package repeats existing art across 2048 pixels.\n"
        "Use 2 MB Chip plus 8 MB Fast. After gameplay appears, reach the "
        "right edge, reset and return phase6-memory.log.\n",
        encoding="ascii",
    )
    chip_delta = {
        "front_clean": (TEST_WIDTH - SOURCE_WIDTH) // 8 * HEIGHT * 4,
        "rear_world": (TEST_WIDTH - SOURCE_WIDTH) // 8 * HEIGHT * 3,
        "front_display": (TEST_WIDTH - SOURCE_WIDTH) // 8 * HEIGHT * 4,
        "collision_bytes": (TEST_WIDTH - SOURCE_WIDTH) // TILE * 14,
    }
    report = {
        "schema": 1,
        "scope": "phase6a-2048px-repeated-art-memory-test",
        "source_width": SOURCE_WIDTH,
        "test_width": TEST_WIDTH,
        "asset_sizes": sizes,
        "projected_resident_delta_bytes": chip_delta,
        "projected_bitmap_delta_bytes": sum(chip_delta.values()) -
                                          chip_delta["collision_bytes"],
    }
    (BUILD / "phase6-memory-host-report.json").write_text(
        json.dumps(report,indent=2) + "\n",encoding="utf-8"
    )
    print(json.dumps(report,indent=2))
    print(f"Wrote {PACKAGE}")


if __name__ == "__main__":
    main()
