#!/usr/bin/env python3
"""Guard the visibility bounds used by the low-Chip runtime assets."""

from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
RUNTIME = ROOT / "assets" / "runtime"


def header(name: str) -> tuple[int, int, int, int, int]:
    data = (RUNTIME / name).read_bytes()
    if data[:4] != b"SPBM":
        raise AssertionError(f"{name}: not SPBM")
    width, height, depth, masked, row_bytes = struct.unpack(">HHBBH", data[4:12])
    palette_bytes = (1 << depth) * 3
    expected = 12 + palette_bytes + row_bytes * height * (depth + masked)
    if len(data) != expected:
        raise AssertionError(f"{name}: {len(data)} bytes, expected {expected}")
    return width, height, depth, masked, row_bytes


front = header("storm-front.spbm")
rear = header("storm-rear.spbm")
loading = header("sparkpaw-level-loading.spbm")
charging = header("level-charge-patch.spbm")
ready = header("sparkpaw-ready-screen.spbm")

assert front[:4] == (3392, 208, 4, 0)
assert rear[:4] == (1120, 208, 3, 0)
assert loading[:4] == (320, 256, 6, 0)
assert charging[:4] == (224, 40, 6, 0)
assert ready[:4] == (320, 256, 6, 0)

# Maximum camera X is 3392-320. Quarter scroll is word-aligned down and the
# Copper fetches 42 bytes (336 pixels). The final fetched pixel must remain in
# the retained 1120px rear span.
max_camera = 3392 - 320
rear_scroll = max_camera // 4
rear_word_start = (rear_scroll // 16) * 16
assert rear_word_start + 42 * 8 <= rear[0]

# The one-shot status patch must use the exact loading-screen palette.
loading_data = (RUNTIME / "sparkpaw-level-loading.spbm").read_bytes()
charging_data = (RUNTIME / "level-charge-patch.spbm").read_bytes()
assert loading_data[12 : 12 + 64 * 3] == charging_data[12 : 12 + 64 * 3]

# Indivision hardware can expose one full-height COLOR00 column outside the
# CRT-visible overscan. Every fullscreen direct-Copper presentation asset must
# therefore reserve palette pen 0 as pure black.
fullscreen_assets = (
    "sparkpaw-title.spbm",
    "sparkpaw-level-loading.spbm",
    "sparkpaw-ready-screen.spbm",
    "intro-plate-01-balance.spbm",
    "intro-plate-02-instruction.spbm",
    "intro-plate-03-reversed-network.spbm",
    "intro-plate-04-motive.spbm",
    "intro-plate-05-quest.spbm",
)
for name in fullscreen_assets:
    assert (RUNTIME / name).read_bytes()[12:15] == b"\0\0\0", (
        f"{name}: fullscreen COLOR00 must be black"
    )

print("PASS: runtime assets retain the complete reachable display span")
