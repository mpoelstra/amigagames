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
assert ready_menu[:4] == (192, 1248, 6, 0)

ready_data = (RUNTIME / "sparkpaw-ready-screen.spbm").read_bytes()
ready_menu_data = (RUNTIME / "readymenu.spbm").read_bytes()
assert ready_data[12 : 12 + 64 * 3] == ready_menu_data[12 : 12 + 64 * 3]

# State zero in the patch atlas must be byte-identical to the displayed base
# screen's menu band. This protects tear-safe runtime patching from palette or
# planar-layout drift.
header_bytes = 12 + 64 * 3
ready_row_bytes = 40
menu_row_bytes = 24
ready_plane_bytes = ready_row_bytes * 256
menu_plane_bytes = menu_row_bytes * 1248
patch_y = 118
patch_x_bytes = 8
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

# HD review drawers must be staged from the release manifest, never from the
# older Makefile runtime subset that predates the ready screen and menu atlas.
makefile_source = (ROOT / "Makefile").read_text()
intro_package = makefile_source[
    makefile_source.index("intro-proof-package:") :
    makefile_source.index("$(ADF_TARGET):")
]
stager_source = (ROOT / "tools" / "stage_hd_test.py").read_text()
assert "tools/stage_hd_test.py" in intro_package
assert "cp $(RUNTIME_ASSETS)" not in intro_package
assert "from make_release import DIST, ROOT, RUNTIME_FILES" in stager_source
assert '"sparkpaw-ready-screen.spbm"' in (ROOT / "tools" / "make_release.py").read_text()
assert '"readymenu.spbm"' in (ROOT / "tools" / "make_release.py").read_text()

# Plate 1 carries one fixed white lower-left skip affordance in its
# non-scrolling illustration. Decode its exact native pixels and guard later
# plates against accidentally retaining the repeated label.
skip_hint = "LMB to skip intro"
small_glyphs = {
    "L": ("10000", "10000", "10000", "10000", "10000", "10000", "11111"),
    "M": ("10001", "11011", "10101", "10101", "10001", "10001", "10001"),
    "B": ("11110", "10001", "10001", "11110", "10001", "10001", "11110"),
    "t": ("00100", "00100", "11111", "00100", "00100", "00101", "00010"),
    "o": ("00000", "00000", "01110", "10001", "10001", "10001", "01110"),
    "s": ("00000", "00000", "01111", "10000", "01110", "00001", "11110"),
    "k": ("10000", "10000", "10010", "10100", "11000", "10100", "10010"),
    "i": ("00100", "00000", "01100", "00100", "00100", "00100", "01110"),
    "p": ("00000", "00000", "11110", "10001", "11110", "10000", "10000"),
    "n": ("00000", "00000", "11110", "10001", "10001", "10001", "10001"),
    "r": ("00000", "00000", "10110", "11001", "10000", "10000", "10000"),
    " ": ("00000",) * 7,
}
for plate in range(1, 6):
    name = f"intro{plate}.spbm"
    data = (RUNTIME / name).read_bytes()
    width, height, depth, masked, row_bytes = struct.unpack(">HHBBH", data[4:12])
    assert (width, depth, masked, row_bytes) == (320, 6, 0, 40)
    bitmap = 12 + 64 * 3
    plane_bytes = row_bytes * height
    palette = [tuple(data[12 + pen * 3 : 15 + pen * 3]) for pen in range(64)]
    white_matches = 0
    foreground_pixels = 0
    for char_index, char in enumerate(skip_hint):
        for glyph_y, bits in enumerate(small_glyphs[char]):
            for glyph_x, bit in enumerate(bits):
                pen = 0
                pixel_x = 8 + char_index * 6 + glyph_x
                pixel_y = 157 + glyph_y
                byte_offset = pixel_y * row_bytes + pixel_x // 8
                mask = 0x80 >> (pixel_x & 7)
                for plane in range(6):
                    if data[bitmap + plane * plane_bytes + byte_offset] & mask:
                        pen |= 1 << plane
                if bit == "1":
                    foreground_pixels += 1
                    if palette[pen] == (255, 255, 255):
                        white_matches += 1
    if plate == 1:
        assert palette.count((255, 255, 255)) == 1
        assert white_matches == foreground_pixels, (
            f"{name}: missing fixed white LMB hint"
        )
        # This pixel is the black shadow of the L's top-left pixel and is not
        # overwritten by the white foreground glyph.
        shadow_x, shadow_y = 9, 158
        shadow_pen = 0
        shadow_offset = shadow_y * row_bytes + shadow_x // 8
        shadow_mask = 0x80 >> (shadow_x & 7)
        for plane in range(6):
            if data[bitmap + plane * plane_bytes + shadow_offset] & shadow_mask:
                shadow_pen |= 1 << plane
        assert shadow_pen == 0, f"{name}: fixed LMB hint lost its black shadow"
    else:
        assert white_matches != foreground_pixels, (
            f"{name}: repeated fixed LMB hint must be absent after plate 1"
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
