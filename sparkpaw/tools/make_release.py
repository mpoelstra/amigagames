#!/usr/bin/env python3
"""Create HD, LHA, ZIP and bootable ADF milestone builds."""
from __future__ import annotations

import os
import shutil
import struct
import subprocess
import sys
import tempfile
import time
import re
from pathlib import Path

from pack_adf_asset import decode as decode_adf_asset

ROOT = Path(__file__).resolve().parents[1]
DIST = ROOT / "dist"
STAGE_PARENT = ROOT / "build" / "release"
RELEASE_VERSION = "0.6.0-alpha.44"
ROADMAP_CHECKPOINT = "6C.1"
RELEASE_NAME = f"Sparkpaw-{RELEASE_VERSION}"
STAGE = STAGE_PARENT / RELEASE_NAME
ADF_EXECUTABLE = ROOT / "build" / "sparkpaw-adf"
ADF_FRONT_ASSET = ROOT / "build" / "adf-assets" / "storm-front.spr1"
ADF_REAR_ASSET = ROOT / "build" / "adf-assets" / "storm-rear.spr1"
ADF_STRIDER_ASSET = (
    ROOT / "build" / "adf-assets" / "clockwork-storm-strider.spr1"
)
ADF_PLAYER_ASSET = ROOT / "build" / "adf-assets" / "sparkpaw-sprites4.spr1"

RUNTIME_FILES = (
    "sparkpaw-title.spbm",
    "sparkpaw-level-loading.spbm",
    "level-charge-patch.spbm",
    "sparkpaw-hud-base.spbm",
    "sparkpaw-hud-health.spbm",
    "sparkpaw-hud-lives.spbm",
    "sparkpaw-hud-diamonds.spbm",
    "sparkpaw-diamond.spbm",
    "storm-front.spbm",
    "storm-rear.spbm",
    "storm-collision.bin",
    "sparkpaw-sprites4.spbm",
    "clockwork-beetle.spbm",
    "clockwork-storm-strider.spbm",
    "energy-shot.raw",
    "player-hurt.raw",
    "enemy-hit.raw",
    "enemy-death.raw",
    "strider-shot.raw",
    "jump.raw",
    "collect-spark.raw",
    "water-splash.raw",
)

RUNTIME_README = f"""Sparkpaw: The Stormstone Quest
=================================

AGA alpha {RELEASE_VERSION}
Roadmap checkpoint: Phase {ROADMAP_CHECKPOINT} rolling-renderer migration
MrDig Productions - Copyright 2026

Phase 6B.3 adds the approved thin ground cap; 6B.3A aligns floor actors to
y=200 and 6B.3B adds synchronized sixteen-frame blue water. The HUD todo is open.
Phase 6B.4 adds a four-frame splash, short pre-restart hold and original Paula
water-impact effect. The HUD todo remains open.
Phase 6B.5 appends four ledge-balance poses as player slots 58..61. Standing
still with support under only one foot triggers them; movement interrupts them.
Phase 6B.6 continues the approved lowest-ground visual direction. Production stays
4+3: AGA cannot provide 5+3 dual playfield. A new authored REAR8 layer adds
storm clouds, mountain depth, dense forest silhouettes, ruined towers,
waterfalls and a cyan-lit central ruin without adding a bitplane or changing
the stable renderer. Its unique native source span is extended from 640 to
1024 pixels, moving the former repeated-art boundary beyond the complete
current and planned quarter-speed camera ranges. The accepted sixteen-frame water cache, collision,
line-252 HUD switch and synchronized line-253 Bob pass remain unchanged.
Alpha.17 keeps the three-plane rear bitmap but uses twelve horizontal-blank
Copper palette steps to morph among dedicated sky, mountain and forest colour
ranges. This increases displayed rear colour depth without an eighth plane,
extra bitmap fetch or CPU compositing. Emulator and real-hardware review of the
new palette timing remain pending.
Supplied alpha.17 FS-UAE review accepts the colour improvement but exposes the
sole non-word-aligned diamond's second-word residue. Alpha.18 adds a dedicated
400-byte two-word background cache for that slot; the other 31 collectibles
retain their compact one-word restore path.
Alpha.19 replaces the complete parallax master with a coherent full-height
Storm Ruins panorama. Turbulent cloud masses now reach the upper edge while
the opening vortex/tower, unique mountain passes, ruins, waterfalls and forest
survive exact 1024x208 Copper-banded reduction. Rear dimensions, three-plane
fetch, scroll factor and memory footprint are unchanged.
Supplied alpha.19 FS-UAE review accepts the panorama but finds its destination
tower less recognizable than the title art. Alpha.20 uses a completely new v4
master generated from that title architecture: central gothic spire, side
turrets, castle base and cyan Stormstone heart. One existing mountain palette
index becomes cyan; plane count, Copper steps and memory remain unchanged.
Supplied alpha.20 FS-UAE evidence accepts the title-identity rear baseline.
Alpha.21 starts the separate foreground re-author with a newly generated AGA
ruin kit: whole platform/column families, open braces, gothic cavities and cyan
conduits replace repeated tile boxes. Collision, top edges, water and renderer
memory/DMA contracts remain unchanged.
Supplied alpha.21 FS-UAE review finds the new material better but rejects its
repeated centred supports, uniform ground strip and obsolete floating purple
lozenges/orange lamps as final foreground quality. Alpha.22 replaces them with
a newly generated richer kit: seven slab rhythms, four distinct underside
structures, four pier families and one shallow authored ground facade. The
legacy decoration is removed. Collision geometry, platform bounds, actor
baselines, FRONT16 ownership and renderer work remain unchanged.
Alpha.23 starts Phase 6C with the complete 3072-pixel/twelve-screen resident
route. Four distinct new screens add a broken low bridge, high side-pier,
second animated water opening, broad patrol court and final portal/chasm. The
level now has two 80-pixel water gaps and two dry chasms. Seven persistent
Striders follow authored world-space routes, including bidirectional crossings
over both water gaps and both dry chasms; they keep updating while parked
offscreen. Ten required plus up to four optional beetle candidates share the
same generic four-slot active enemy/Bob pool. Sixteen aligned diamonds extend
the trails to 48 total. The second water location reuses the existing 7,040-byte
frame bank and is copied to clean/display in the synchronized line-253 pass.
No player/enemy frame IDs, line-100 staging or line-252 HUD switch change.
On HD, the same left-mouse exit now adds prepared-peak and post-run Chip/Fast
free/largest values to `renderdiag.log`; no separate executable is needed for
the primary Phase 6C.1 memory/timing return.
The ADF-only build also packs the 184,380-byte player source as a 135,907-byte
SPR1 stream. It uses the accepted 512-byte streaming decoder and leaves the HD
SPBM path unchanged; this saves disk space but does not claim lower resident
Chip use.
Alpha.24 fixes the blocked final ascent found in supplied alpha.23 FS-UAE
evidence. The last dry-gap landing is now a low broken step followed by a
reachable 48-pixel rise to the portal platform; global jump physics is
unchanged. Dry gaps retain distinct fall semantics and gain broken bank faces,
dark recessed bottoms and severed cyan conduit details. Suspended-platform
undersides also alternate shallow lips, braces and deeper machinery silhouettes
without changing solid slabs or renderer work.
Supplied alpha.24 FS-UAE evidence rejects that first correction: Sparkpaw can
stand on the low landing, but its adjacent 48-pixel wall cancels horizontal
motion before his feet clear the portal lip. Alpha.25 shortens the low landing,
leaves a 16-pixel open run between steps, reduces the rise to 32 pixels and
aligns the remaining portal deck at that height. Exact host simulation includes
moveX-before-moveY, the standing hitbox and four approach speeds; all land on
the portal. Global movement, jump velocity and renderer contracts are unchanged.
The four final-deck diamonds are also re-aligned above y=144 with full hover
clearance, and the last beetle patrol follows that corrected solid surface. A
full 48-diamond collision audit also corrects five earlier placements whose
16x21 Bob plus two-pixel low hover touched a platform or the floor.
Supplied alpha.25 FS-UAE evidence rejects the 32-pixel step at the extreme
right/max-speed launch boundary. Alpha.26 lowers the entire portal deck to
y=160: a 16-pixel rise after the existing 16-pixel gap. An exhaustive host
collision model covers all 21 valid launch X positions and 83 horizontal speed
samples (1,743 cases), including exact maximum speed, with zero failures.
Supplied alpha.26 FS-UAE evidence nevertheless rejects that 16-pixel rise.
Alpha.27 removes the vertical step: the low landing and complete portal deck
share y=176 with only one open 16-pixel tile between them. Completion no longer
depends on a wall-adjacent jump; global movement and jump physics stay intact.
Supplied alpha.27 FS-UAE evidence identifies a different blocked route at the
second-water approach: its x=2320 platform sits 64 pixels above the preceding
slab. Alpha.28 lowers that complete platform and attached pier from y=112 to
y=160, leaving a readable but reachable 16-pixel rise. The following water gap,
global jump physics and renderer contracts remain unchanged.
The later portal is restored from alpha.27's unnecessary flat y=176 experiment
to the still-raised y=160 alpha.26 layout. Independently audited diamond
clearance remains intact.
Supplied alpha.28 FS-UAE/HD testing accepts the corrected platform and confirms
Sparkpaw can continue through and complete the full level. ADF gameplay parity,
real-A1200 verification and full-run timing/memory evidence remain pending.
Alpha.29 removes the redundant `lowShot` eligibility predicate from beetle
hits while preserving the existing x+2..29, y+7..23 body rectangle, two-HP
hurt/death lifecycle and renderer. Host tests prove an elevated standing hit,
a floor standing miss through natural Y separation and a floor crouch hit.
MrDig's supplied FS-UAE/HD retest accepts this isolated correction and clean
left-mouse exit.
Alpha.29 native build, host regressions and release validation passed. Its
bootable ADF used 1,549 blocks (774 KiB), leaving 211 blocks free; that did not
establish ADF gameplay parity.
Supplied FS-UAE/HD evidence rejects alpha.30: Strider flicker persists even
when the second Strider is far away, disproving the player-stream hypothesis.
Its diagnostic records 3,525 wraps in 3,844 passes and a 237-line peak at
camera 810 with two off-screen water updates. Alpha.31 removes that experimental
sprite staging and camera-culls water animation per strip. A strip synchronizes
to the current frame as soon as it enters the 16-pixel margin. The world
diamond, enemy art/geometry, FRONT16 planes, line-253 pass and draw order remain
unchanged. Host visibility and beetle-hit regressions pass; FS-UAE review of
the focused alpha.31 correction partially accepts water behavior, but residual
upper-Strider flicker remains. Its quit log records 6,599 wraps in 6,796 passes
and a 219-line zero-water peak. Alpha.32 keeps family priority unchanged while
stably ordering enemy restores and draws from upper to lower world Y. Equal-Y
enemies retain slot order, so upper Bobs complete earlier before their next
scanout. Art, geometry, masks, collectibles, diamond and water are unchanged.
Host ordering, visibility and beetle-hit regressions pass; focused FS-UAE/HD
review of alpha.32 remains pending, with no ADF or real-A1200 claim.
Supplied alpha.32 FS-UAE/HD review accepts separated Strider stability but finds
small head/crest flicker while two Striders intersect. Its zero-water peak is
214 lines. Alpha.33 merges only intersecting Strider restore rectangles, so the
shared background area is restored once before the unchanged stable upper-first
draws. Non-overlapping restores, art, masks, geometry, draw priority, water,
collectibles and diamond remain unchanged. Host overlap-union and existing
regressions pass; focused FS-UAE/HD review remains pending, with no ADF or
real-A1200 claim.
Native/package validation passes. The bootable alpha.33 ADF uses 1,555 blocks
(777 KiB), leaving 205 free; this is host package evidence only.
MrDig's supplied FS-UAE/HD review accepts alpha.33 overlapping-Strider
presentation. Separate evidence rejects projectile occlusion at a narrow
pillar: a flush crouch shot can spawn beyond it and damage the beetle behind.
Alpha.34 tracks collision from Sparkpaw's physical front edge and sweeps every
crossed X pixel to the new projectile leading edge, testing solid geometry
before enemy damage at each sample. Visual muzzle/Bob origins, speed, fire
heights, beetle geometry and renderer remain unchanged. Host sweep and existing
regressions pass; focused FS-UAE/HD review remains pending. No ADF gameplay or
real-A1200 claim is made.
Native/package validation passes. The bootable alpha.34 ADF uses 1,557 blocks
(778 KiB), leaving 203 free; this is host package evidence only.
MrDig's supplied FS-UAE/HD review accepts alpha.34 projectile occlusion. A new
full run at explicit 68020 speed rejects overall timing: 2,293 of 3,015 Bob
passes wrap and the 311-line peak exceeds the 59-line budget. Alpha.35 camera-
culls completely off-screen projectile Bobs and adds separate raster-line
totals/peaks for projectile, enemy, collectible, water and splash families.
The diagnostic label is current. The HD drawer now includes and decodes the
same four packed large assets as ADF, reducing I/O and aligning the transition
path after supplied real-Amiga evidence showed ADF working but alpha.34 HD
corrupting immediately after CHARGING. Alpha.35 68020 and real-HD acceptance
remain pending. Diamond art and display/Copper contracts are unchanged.
Packed CRC validation now uses two nibble-table steps instead of eight bit
iterations per output byte; a host standard-vector test guards equivalence.
Host/native/package validation passes. The alpha.35 ADF uses 1,560 blocks
(780 KiB), leaving 200 free; this is package evidence only.
Supplied alpha.35 FS-UAE/HD evidence rejects the packed path for the normal
executable: LOADING never advances to CHARGING or gameplay and intermittent
display marks occur. Alpha.36 restores raw SPBM loading and package contents
for HD only. ADF retains SPR1 plus the verified faster CRC. Projectile culling,
family profiling, diamond art and display/Copper contracts are unchanged.
FS-UAE alpha.36 and real-HD acceptance remain pending.
Host/native/package validation passes. The alpha.36 ADF uses 1,561 blocks
(780 KiB), leaving 199 free; this is package evidence only.
Supplied alpha.36 FS-UAE/HD evidence accepts return to gameplay but rejects
stock-68020 performance: 3,709 of 4,139 passes wrap and the 310-line peak has
two Striders, one projectile and one collectible. Enemy work dominates at
354,807 accumulated raster lines and a 263-line peak. Alpha.37 normalizes every
enemy animation frame into its exact non-transparent bounds and restores that
same stored rectangle. Intersecting Striders merge their unequal tight bounds.
Art pixels, animation rate, collision, family order and display/Copper contracts
are unchanged. FS-UAE/HD review remains pending.
Supplied FS-UAE/68020 evidence rejects alpha.37: enemy sprites are visibly
damaged and frame rate remains unacceptable. Alpha.38 replaces the impossible
in-place line-253 restore/draw pass with two hidden 512x256 FRONT16 viewport
buffers. Each frame copies the camera-local 512x208 clean foreground, draws
dynamic Bobs only into the hidden target, waits for completion, then publishes
its Copper pointers. The displayed foreground is never written. Enemy,
projectile and splash restore passes disappear; diamonds and water update only
the clean world. HUD remains separate at line 252, rear playfield stays three
planes and Sparkpaw remains an attached hardware sprite. Production diagnostics
no longer serialize every family with WaitBlit. FS-UAE/68020, ADF gameplay and
real-A1200 acceptance remain pending.
Supplied FS-UAE review rejects REAR8 plus foreground v1 as the final visual
quality target while retaining the corrected non-repeating composition, and
accepts rb19c as a worthwhile isolated REAR16 visual proof. The source package
Supplied rd01 production evidence finds seven per-frame diamond restore/draws
in the 172-line high-water frame. Alpha.15 keeps their accepted hover but stores
5,376 bytes of compact original Chip-RAM patches, retains active diamonds in
clean/display and staggers their updates over four frames. Supplied rd02 shows
the next real bottleneck is a six-projectile/four-enemy combat frame. The HD
main executable now keeps the high-water log and clean left-mouse exit, avoiding
separate test drawers. ADF stays reset-only. Production remains 4+3.

Alpha.38 hidden-viewport composition is rejected in supplied FS-UAE testing:
3,324 of 3,722 compositions wrap and performance is worse at both 68020 and a
separate 68030 check. Alpha.39 restores the accepted alpha.33-style resident
foreground restore/draw path and line-253 scheduling. It removes alpha.37 tight
enemy bounds, all alpha.38 viewport copies and invasive per-family diagnostic
waits while retaining accepted collision fixes, culling and raw HD loading.
Alpha.39 emulator and hardware acceptance remain pending.
Supplied follow-up accepts alpha.39 presentation at 68030 in FS-UAE and its ADF
on a real A1200. Both launch routes work in FS-UAE, but real-A1200 HD corrupts
the native display immediately after CHARGING. Alpha.40 stages every raw DOS
read through a 512-byte buffer instead of passing complete 98 KiB world planes
or arbitrary final DMA addresses, avoiding reliance on controller/filesystem
MaxTransfer and Mask behavior. Renderer, art, gameplay and the working packed
ADF path are unchanged. Real-HD acceptance remains pending.
Supplied real-A1200/68030 tests prove alpha.40 HD works with about 1.92 MB Chip
free but fails from a normal 1,430,032-byte-free Workbench. Alpha.41 removes
only unreachable bitmap storage: foreground rows below the line-252 HUD switch
and rear pixels beyond the maximum quarter-scroll fetch. Complete player and
Strider frame masters move bit-identically to Fast RAM; two player stage sets
and four enemy stage slots remain in Chip RAM for custom-chip DMA. LOADING and
CHARGING share the accepted floppy bitmap and replace only the status band.
Calculated permanent Chip saving is about 642 KiB, plus about 54 KiB at the
status-screen peak. Official HD and ADF executables are reset-to-exit and contain
no profiler, mouse-flush or log code. Supplied real-A1200/68030 testing accepts
the HD build from a normal Workbench with about 1.4 MB Chip RAM free. Alpha.41
ADF regression remains pending. Alpha.40 and alpha.41 hardware evidence both
reject the first two-Strider scene because of intermittent enemy glitches and
apparent cadence loss; occasional beetle glitches are also reported. Focused
presentation/deadline isolation is next, before broad stock-68020 optimization.
Host/native/package validation passes; the DOS1/FFS ADF uses 1,177 blocks
(588 KiB) and leaves 583 free.

Alpha.42 promotes the supplied FS-UAE/HD-accepted Stage 4G renderer. Two
complete Copper lists publish atomically at a fixed PAL boundary. Two hidden
FRONT16 targets each keep a logical 512-pixel no-copy ring repeated three times
for seam-safe fetches and complete 64-pixel enemies. Target-local physical Bob
history removes projectile, beetle and Strider trails. The HUD setup begins
immediately after the preceding DDFSTOP, removing the supplied FS-UAE boundary
flicker. REAR8 quarter-speed parallax, palettes, assets, sprites, animation,
HUD content and gameplay are unchanged. The supplied log reports 1,952 one-
field plus 31 two-field intervals, 49.23 effective FPS. ADF, Analogue Pocket
and real-A1200 acceptance remain pending.

Alpha.43 promotes the supplied FS-UAE/68030-HD-accepted Stage 5L renderer.
The coherent Stage 5G 32-bit playfield fetch and four-byte left guard are kept,
while the unchanged 48x48, 15-colour Sparkpaw pixels are transparently padded
to one attached 64-pixel AGA sprite pair on the earliest two DMA channels.
This replaces three 16-pixel attached pairs without changing art, palette,
animation, grounding, collision, HUD, parallax or gameplay. The supplied proof
reports no corruption, glitches or flicker and records 2,163 of 2,163 one-field
intervals (50.00 FPS) with zero rolling-target ownership violations in the fast
FS-UAE/68030 configuration. Official HD and ADF builds contain no diagnostic
logging. Supplied real-A1200/68030 HD and physical-ADF tests, plus Analogue
Pocket ADF, retain broad visual stability but reject cadence. Real HD also
repeats or misses some sound events under load. A narrow intermittent pixel or
black-line disturbance remains at the ground/HUD seam.

Alpha.44 retains Stage 5L and promotes the supplied FS-UAE/68030-HD-accepted
H7 seam correction: FRONT16 colours are masked only on the final transition
scanline before the unchanged fixed HUD. It also packages the accepted Stage 2
CPU/memory work: display-only rolling targets, word-level loading conversion,
patch-only Copper publication, direct/unrolled ring maintenance, tile-span
collision, cached sprite staging and hazard columns, copy-on-unload enemy
state, immediate post-publication updates and target-local diamonds. Supplied
FS-UAE/68020 HD A/B evidence measures the two largest cadence steps at 27.45 to
35.81 FPS for immediate phase start and 35.31 to 42.15 FPS for target-local
diamond composition. Assets, colours, FRONT16+REAR8, parallax, wide attached
Sparkpaw pair, HUD content, animation, collision geometry, object placement,
draw order and gameplay contracts remain unchanged. Official builds contain no
diagnostic logger or test input. Alpha.44 ADF, Analogue Pocket and real-A1200
presentation/performance acceptance remain pending supplied tests.

Requirements
------------

Amiga 1200 or compatible AGA Amiga, Motorola 68020, 2 MB Chip RAM and
8 MB Fast RAM.

Keep the complete {RELEASE_NAME} drawer together. Start it from Shell:

  CD {RELEASE_NAME}
  Sparkpaw

Controls
--------

Joystick port 2: left/right to run, up to jump and fire to shoot. Every fire
press launches a fast blue/cyan plasma pulse; rapid tapping supports several
pulses in flight. Hold down to crouch; combine down with left/right to
crouch-walk, and press fire to shoot from a dedicated low pose. Ten required
plus a bounded optional set of low clockwork beetles patrol the test
level with bounded position and speed variation. Standing fire naturally
passes above floor beetles; crouch fire hits them, while an elevated beetle can
be hit by any shot whose sample overlaps its body. Fire twice to destroy one.
Beetle contact removes one of
six internal half-heart health units, applies knockback and plays a short hurt
effect. Destroyed beetles can return indefinitely after a cooldown once their
patrol zone is safely off-screen. Reaching the far-right world edge resets the
current test level in memory so it can be replayed without rebooting.
A fixed full-width HUD band at the bottom shows the six health units as three
full, half or empty hearts. The Sparkpaw-head counter starts at x3, decreases on
each zero-health reset and temporarily cycles to x3 after the third loss until
the later game-over state is implemented. Forty-eight hovering diamonds form
trails and original arcs throughout the level; the adjacent two-digit HUD
counter records them up to 49. Contact invulnerability is shown by a brief
whole-character blink.
Official HD and ADF executables are reset-to-exit and perform no DOS log write.
Focused diagnostics use the separately built renderdiag executable; its flush
writes and closes the log, then waits for reset without reopening Workbench.

What to test
------------

Walk through all twelve screen widths, jump onto and off each platform, reverse
direction often, and compare the movement on HD and ADF. The rear scenery
must move at one quarter of the foreground speed. Sparkpaw now uses fixed
48x48, 15-colour AGA poses for idle, blink, run, jump, fall, landing and
crouching and shooting. After two seconds at rest Sparkpaw performs an idle
flourish. Check that all beetles remain grounded and turn cleanly. Standing
fire must still miss floor beetles, crouch fire must retain the two-hit sequence,
and standing fire must hit an elevated beetle when geometry overlaps. Watch for Bob residue when plasma pulses
overlap an enemy or two beetles approach the same screen edge.
Please look for sprite flicker, apparent size changes, foot sliding, tearing,
collision errors, camera jumps and parallax glitches.

Seven persistent Clockwork Storm Striders patrol independently across the
twelve-screen route. Their authored traversal links cross both water openings
and both dry chasms in both directions. Offscreen Striders keep simulating in
world space while their Bobs remain parked; verify natural entry from either
screen edge, grounding, traversal, ranged/contact damage, hit/death/respawn and
the absence of stale 64x64 pixels. Music remains outside this milestone.
"""


def crc16(data: bytes) -> int:
    crc = 0
    for byte in data:
        crc ^= byte
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc & 0xFFFF


def dos_time(timestamp: float) -> int:
    value = time.localtime(timestamp)
    return ((max(1980, min(2107, value.tm_year)) - 1980) << 25 |
            value.tm_mon << 21 | value.tm_mday << 16 | value.tm_hour << 11 |
            value.tm_min << 5 | value.tm_sec // 2)


def lha_member(name: str, data: bytes, timestamp: float) -> bytes:
    encoded = name.encode("latin-1")
    body = (b"-lh0-" + struct.pack("<IIIBBB", len(data), len(data),
            dos_time(timestamp), 0x20, 0, len(encoded)) + encoded +
            struct.pack("<H", crc16(data)))
    return bytes((len(body), sum(body) & 255)) + body + data


def copy_runtime() -> None:
    if STAGE_PARENT.exists():
        shutil.rmtree(STAGE_PARENT)
    STAGE.mkdir(parents=True)
    shutil.copy2(ROOT / "sparkpaw", STAGE / "Sparkpaw")
    for name in RUNTIME_FILES:
        target = STAGE / "assets" / "runtime" / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(ROOT / "assets" / "runtime" / name, target)
    (STAGE / "ReadMe.txt").write_text(RUNTIME_README, encoding="ascii")


def make_adf() -> Path:
    adf_root = ROOT / "build" / "adf-root"
    if adf_root.exists():
        shutil.rmtree(adf_root)
    shutil.copytree(STAGE, adf_root)
    shutil.copy2(ADF_EXECUTABLE, adf_root / "Sparkpaw")
    (adf_root / "assets" / "runtime" / "storm-front.spbm").unlink()
    shutil.copy2(
        ADF_FRONT_ASSET,
        adf_root / "assets" / "runtime" / "storm-front.spr1",
    )
    (adf_root / "assets" / "runtime" / "storm-rear.spbm").unlink()
    shutil.copy2(
        ADF_REAR_ASSET,
        adf_root / "assets" / "runtime" / "storm-rear.spr1",
    )
    (adf_root / "assets" / "runtime" /
     "clockwork-storm-strider.spbm").unlink()
    shutil.copy2(
        ADF_STRIDER_ASSET,
        adf_root / "assets" / "runtime" /
        "clockwork-storm-strider.spr1",
    )
    (adf_root / "assets" / "runtime" / "sparkpaw-sprites4.spbm").unlink()
    shutil.copy2(
        ADF_PLAYER_ASSET,
        adf_root / "assets" / "runtime" / "sparkpaw-sprites4.spr1",
    )
    (adf_root / "S").mkdir()
    (adf_root / "S" / "startup-sequence").write_text(
        "Sparkpaw\n", encoding="ascii"
    )
    adf = DIST / f"{RELEASE_NAME}.adf"
    amitools = ROOT / ".toolchain" / "amitools"
    if not (amitools / "amitools" / "tools" / "xdftool.py").is_file():
        raise SystemExit("missing independent sparkpaw/.toolchain/amitools")
    env = os.environ.copy()
    env["PYTHONPATH"] = str(amitools)
    command = [
        sys.executable, "-m", "amitools.tools.xdftool", "-f", str(adf),
        # FFS is native to the target A1200. The ADF-only executable decodes
        # storm-front.spr1; HD archives retain ordinary storm-front.spbm.
        "format", "SparkpawM2A", "DOS1", "+", "boot", "install",
    ]
    for directory in ("S", "assets", "assets/runtime"):
        command.extend(("+", "makedir", directory))
    for path in sorted(adf_root.rglob("*")):
        if path.is_file():
            command.extend(("+", "write", str(path), path.relative_to(adf_root).as_posix()))
    subprocess.run(command, cwd=ROOT, env=env, check=True)
    if adf.stat().st_size != 901120:
        raise SystemExit(f"invalid ADF size: {adf.stat().st_size}")
    subprocess.run([
        sys.executable, "-m", "amitools.tools.xdftool", "-r", str(adf),
        "boot", "show", "+", "list",
    ], cwd=ROOT, env=env, check=True)
    with tempfile.TemporaryDirectory() as temp_dir:
        extracted = Path(temp_dir)
        subprocess.run([
            sys.executable, "-m", "amitools.tools.xdftool", "-r", str(adf),
            "read", "Sparkpaw", str(extracted / "Sparkpaw"),
            "+", "read", "S/startup-sequence",
            str(extracted / "startup-sequence"),
            "+", "read", "assets/runtime/storm-front.spr1",
            str(extracted / "storm-front.spr1"),
            "+", "read", "assets/runtime/storm-rear.spr1",
            str(extracted / "storm-rear.spr1"),
            "+", "read", "assets/runtime/clockwork-storm-strider.spr1",
            str(extracted / "clockwork-storm-strider.spr1"),
            "+", "read", "assets/runtime/sparkpaw-sprites4.spr1",
            str(extracted / "sparkpaw-sprites4.spr1"),
        ], cwd=ROOT, env=env, check=True)
        if (extracted / "Sparkpaw").read_bytes() != ADF_EXECUTABLE.read_bytes():
            raise SystemExit("ADF verification failed: Sparkpaw")
        if (extracted / "startup-sequence").read_bytes() != b"Sparkpaw\n":
            raise SystemExit("ADF verification failed: S/startup-sequence")
        packed = (extracted / "storm-front.spr1").read_bytes()
        if packed != ADF_FRONT_ASSET.read_bytes():
            raise SystemExit("ADF verification failed: storm-front.spr1")
        if decode_adf_asset(packed) != (
            ROOT / "assets" / "runtime" / "storm-front.spbm"
        ).read_bytes():
            raise SystemExit("ADF decode verification failed: storm-front.spr1")
        packed = (extracted / "storm-rear.spr1").read_bytes()
        if packed != ADF_REAR_ASSET.read_bytes():
            raise SystemExit("ADF verification failed: storm-rear.spr1")
        if decode_adf_asset(packed) != (
            ROOT / "assets" / "runtime" / "storm-rear.spbm"
        ).read_bytes():
            raise SystemExit("ADF decode verification failed: storm-rear.spr1")
        packed = (extracted / "clockwork-storm-strider.spr1").read_bytes()
        if packed != ADF_STRIDER_ASSET.read_bytes():
            raise SystemExit(
                "ADF verification failed: clockwork-storm-strider.spr1"
            )
        if decode_adf_asset(packed) != (
            ROOT / "assets" / "runtime" / "clockwork-storm-strider.spbm"
        ).read_bytes():
            raise SystemExit(
                "ADF decode verification failed: clockwork-storm-strider.spr1"
            )
        packed = (extracted / "sparkpaw-sprites4.spr1").read_bytes()
        if packed != ADF_PLAYER_ASSET.read_bytes():
            raise SystemExit("ADF verification failed: sparkpaw-sprites4.spr1")
        if decode_adf_asset(packed) != (
            ROOT / "assets" / "runtime" / "sparkpaw-sprites4.spbm"
        ).read_bytes():
            raise SystemExit(
                "ADF decode verification failed: sparkpaw-sprites4.spr1"
            )
    return adf


def make_source_zip() -> Path:
    source_root = ROOT / "build" / "source-release"
    source_stage = source_root / f"{RELEASE_NAME}-Source"
    if source_root.exists():
        shutil.rmtree(source_root)
    source_stage.mkdir(parents=True)
    for filename in ("Makefile", "README.md"):
        shutil.copy2(ROOT / filename, source_stage / filename)
    for directory, patterns in {
        "src": ("*.c", "*.h", "*.s"),
        "tests": ("*.c",),
        "tools": ("*.py",),
        "docs": ("*.md", "*.txt"),
        "assets": ("*.png", "*.iff", "*.json", "*.gpl", "*.spbm", "*.bin"),
        "sfx": ("*.raw", "*.json", "*.bank"),
    }.items():
        for pattern in patterns:
            for source in (ROOT / directory).rglob(pattern):
                target = source_stage / source.relative_to(ROOT)
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(source, target)
    return Path(shutil.make_archive(
        str(DIST / f"{RELEASE_NAME}-Source"), "zip",
        source_root, source_stage.name,
    ))


def clean_dist_releases() -> None:
    """Keep dist reviewable: one current, consistently versioned release set."""
    DIST.mkdir(exist_ok=True)
    for path in DIST.iterdir():
        if not path.name.startswith("Sparkpaw-"):
            continue
        if path.is_dir():
            shutil.rmtree(path)
        else:
            path.unlink()


def validate_release_identity() -> None:
    """Prevent packaged SemVer and the numbered roadmap phase from drifting."""
    version_match = re.fullmatch(r"0\.(\d+)\.\d+-[0-9A-Za-z.-]+", RELEASE_VERSION)
    phase_match = re.fullmatch(r"(\d+)[A-Z](?:\.\d+)?", ROADMAP_CHECKPOINT)
    if version_match is None:
        raise ValueError(f"invalid Sparkpaw prerelease version: {RELEASE_VERSION}")
    if phase_match is None:
        raise ValueError(f"invalid Sparkpaw roadmap checkpoint: {ROADMAP_CHECKPOINT}")
    if version_match.group(1) != phase_match.group(1):
        raise ValueError(
            "release/roadmap mismatch: "
            f"{RELEASE_VERSION} does not belong to Phase {ROADMAP_CHECKPOINT}"
        )


def main() -> None:
    include_source = sys.argv[1:] == ["--include-source"]
    if sys.argv[1:] and not include_source:
        raise SystemExit("usage: make_release.py [--include-source]")
    validate_release_identity()
    copy_runtime()
    clean_dist_releases()
    zip_path = Path(shutil.make_archive(
        str(DIST / RELEASE_NAME), "zip", STAGE_PARENT, STAGE.name,
    ))
    review_drawer = DIST / RELEASE_NAME
    shutil.copytree(STAGE, review_drawer)
    lha_path = DIST / f"{RELEASE_NAME}.lha"
    with lha_path.open("wb") as output:
        for path in sorted(STAGE.rglob("*")):
            if path.is_file():
                relative = path.relative_to(STAGE_PARENT).as_posix()
                output.write(lha_member(relative, path.read_bytes(), path.stat().st_mtime))
        output.write(b"\0")
    adf_path = make_adf()
    paths = [zip_path, lha_path, adf_path]
    if include_source:
        paths.append(make_source_zip())
    for path in paths:
        print("Wrote", path)
    print("Prepared", review_drawer)


if __name__ == "__main__":
    main()
