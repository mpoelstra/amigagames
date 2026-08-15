#!/usr/bin/env python3
"""Build an isolated 3072px Phase 6C.1 production-memory test package."""
from __future__ import annotations

import json
import shutil
from pathlib import Path

from make_release import ROOT, RUNTIME_FILES, RUNTIME_README

BASELINE_WIDTH = 2048
TEST_WIDTH = 3072
HEIGHT = 256
TILE = 16
BUILD = ROOT / "build" / "test"
PACKAGE = BUILD / "Sparkpaw-Phase6C1-3072"


def main() -> None:
    if PACKAGE.exists():
        shutil.rmtree(PACKAGE)
    BUILD.mkdir(parents=True,exist_ok=True)
    runtime = PACKAGE / "assets" / "runtime"
    runtime.mkdir(parents=True)
    shutil.copy2(ROOT / "build" / "sparkpaw-phase6-memory", PACKAGE / "Sparkpaw")
    for name in RUNTIME_FILES:
        shutil.copy2(ROOT / "assets" / "runtime" / name, runtime / name)
    sizes = {name: {"bytes": (runtime / name).stat().st_size}
             for name in ("storm-front.spbm","storm-rear.spbm",
                          "storm-collision.bin")}
    (PACKAGE / "ReadMe.txt").write_text(
        RUNTIME_README + "\nPHASE 6C.1 MEMORY TEST\n"
        "This isolated HD package runs the authored 3072-pixel route.\n"
        "Use 2 MB Chip plus 8 MB Fast. After gameplay appears, reach the "
        "right edge, reset and return phase6-memory.log.\n",
        encoding="ascii",
    )
    chip_delta = {
        "front_clean": (TEST_WIDTH - BASELINE_WIDTH) // 8 * HEIGHT * 4,
        "rear_world": (TEST_WIDTH - BASELINE_WIDTH) // 8 * HEIGHT * 3,
        "front_display": (TEST_WIDTH - BASELINE_WIDTH) // 8 * HEIGHT * 4,
        "collision_bytes": (TEST_WIDTH - BASELINE_WIDTH) // TILE * 14,
    }
    report = {
        "schema": 1,
        "scope": "phase6c1-3072px-production-memory-regression",
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
