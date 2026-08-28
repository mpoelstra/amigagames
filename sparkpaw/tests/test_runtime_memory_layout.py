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
ready_menu = header("readymenu.spbm")

assert front[:4] == (3392, 208, 4, 0)
assert rear[:4] == (1120, 208, 3, 0)
assert loading[:4] == (320, 256, 6, 0)
assert charging[:4] == (224, 40, 6, 0)
assert ready[:4] == (320, 256, 6, 0)
assert ready_menu[:4] == (160, 416, 6, 0)

ready_data = (RUNTIME / "sparkpaw-ready-screen.spbm").read_bytes()
ready_menu_data = (RUNTIME / "readymenu.spbm").read_bytes()
assert ready_data[12 : 12 + 64 * 3] == ready_menu_data[12 : 12 + 64 * 3]

# State zero in the patch atlas must be byte-identical to the displayed base
# screen's menu band. This protects tear-safe runtime patching from palette or
# planar-layout drift.
header_bytes = 12 + 64 * 3
ready_row_bytes = 40
menu_row_bytes = 20
ready_plane_bytes = ready_row_bytes * 256
menu_plane_bytes = menu_row_bytes * 416
patch_y = 118
patch_x_bytes = 10
patch_bytes = menu_row_bytes * 104
for plane in range(6):
    ready_at = (header_bytes + plane * ready_plane_bytes +
                patch_y * ready_row_bytes + patch_x_bytes)
    menu_at = header_bytes + plane * menu_plane_bytes
    for row in range(104):
        ready_row = ready_at + row * ready_row_bytes
        menu_row = menu_at + row * menu_row_bytes
        assert ready_data[ready_row : ready_row + menu_row_bytes] == \
            ready_menu_data[menu_row : menu_row + menu_row_bytes]

first_state = ready_menu_data[header_bytes : header_bytes + patch_bytes]
second_state = ready_menu_data[
    header_bytes + patch_bytes : header_bytes + patch_bytes * 2
]
assert first_state != second_state

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
    "intro1.spbm",
    "intro2.spbm",
    "intro3.spbm",
    "intro4.spbm",
    "intro5.spbm",
)
for name in fullscreen_assets:
    assert (RUNTIME / name).read_bytes()[12:15] == b"\0\0\0", (
        f"{name}: fullscreen COLOR00 must be black"
    )

# Ready-menu state changes must never patch the bitmap currently being fetched.
# The hidden displayable buffer is completed first and then published through
# an inactive Copper list at an owned PAL frame boundary.
title_source = (ROOT / "src" / "title.c").read_text()
menu_update = title_source[
    title_source.index("static void showReadyMenuState") :
    title_source.index("void titleRunLevelReadyMenu")
]
assert "readyMenuBack=AllocBitMap" in title_source
assert "hidden->Planes[plane]+targetOffset" in menu_update
assert "waitOwnedCopperArmWindow()" in menu_update
assert "hardware->cop1lc=(ULONG)copper[next]" in menu_update
assert "platformSwitchCopper(copper[next])" not in menu_update
assert "hardware->copjmp1" not in menu_update
assert "loading->bitmap->Planes[plane]+targetOffset" not in menu_update
assert "while(rasterLine()<252)" not in menu_update

# Full custom-chip ownership begins while the ready screen is still active.
# Its Copper list has no sprite-pointer moves, so sprite DMA must remain off
# until the gameplay Copper list is explicitly installed.
platform_source = (ROOT / "src" / "platform_amiga.c").read_text()
finish_takeover = platform_source[
    platform_source.index("void platformFinishTakeover") :
    platform_source.index("void platformSwitchCopper")
]
switch_copper = platform_source[
    platform_source.index("void platformSwitchCopper") :
    platform_source.index("void platformRestore")
]
assert "DMAF_SPRITE" not in finish_takeover.split("hardware->dmacon=DMAF_SETCLR", 1)[1]
assert "hardware->dmacon=DMAF_SETCLR|DMAF_SPRITE" in switch_copper

print("PASS: runtime assets retain the complete reachable display span")
