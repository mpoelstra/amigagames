# Codex handoff: Amiga game workspace

Last updated: 20 August 2026

## Start here

Sparkpaw is the active project. At the start of a task:

1. read this file and `sparkpaw/README.md` completely;
2. inspect current source, `git status`, recent commits and tags;
3. use source/generated manifests as authority;
4. consult the relevant part of `docs/DEVELOPMENT_HISTORY.md` before reopening
   a known renderer, hardware, asset or gameplay problem.

This file is the compact current contract, not a diary. Put chronology,
rejected experiments and detailed evidence in `docs/DEVELOPMENT_HISTORY.md`.

Repository:

```text
amigagame/
  CODEX_HANDOFF.md
  docs/DEVELOPMENT_HISTORY.md
  sparkpaw/                 active AGA game
  chipsnake/                finished prototype
  mrdigs-futsal/            finished prototype
  backups/                  ignored; never delete
  ACM_PDF/                  ignored Amiga manuals
```

Branch `main` is the shared state. Preserve recordings, logs, toolchains and
backups.

## Build, release and verification

Target: PAL A1200/AGA. The supplied real A1200/68030 at about 34.5 MHz may be
the minimum release CPU if stock-68020 50 Hz cannot be reached without visual
or gameplay concessions; FS-UAE/68020 remains the optimization stress target.
Memory minimum remains 2 MB Chip plus 8 MB Fast RAM.

From `sparkpaw/` always run after implementation:

```sh
make PYTHON=../.venv/bin/python3
make release PYTHON=../.venv/bin/python3
```

Current release is `0.6.0-alpha.43`, Phase 6C.1. A normal release contains:

- `Sparkpaw-0.6.0-alpha.43.lha`
- `Sparkpaw-0.6.0-alpha.43.zip`
- `Sparkpaw-0.6.0-alpha.43.adf`
- extracted review drawer `Sparkpaw-0.6.0-alpha.43/`

Do not create the >100 MB Source ZIP unless MrDig explicitly requests it.
Opt-in command: `tools/make_release.py --include-source`.

MrDig supplies authoritative FS-UAE and real-hardware results. Never infer ADF
parity from HD, FS-UAE from host builds, or real hardware from emulation. His
real A1200 has a 68030, so stock-68020 performance is tested in FS-UAE; similar
behavior on a real 68020 is a hypothesis, not supplied hardware verification.

## Current acceptance boundary

- Phase 6C.1 twelve-screen traversal is accepted in supplied FS-UAE/HD testing.
- Alpha.29 elevated-beetle hit behavior is accepted in FS-UAE/HD.
- Alpha.33 overlapping-Strider presentation is accepted in FS-UAE/HD.
- Alpha.34 projectile/wall sweep is accepted in FS-UAE/HD.
- Alpha.39 restores acceptable presentation and speed in FS-UAE at 68030.
- Alpha.39 ADF works on the supplied real A1200/68030.
- Alpha.39 HD and ADF both work in FS-UAE.
- Alpha.39 HD is rejected on the real A1200: after CHARGING the HDMI output is
  mostly black with horizontal remnants and CRT output shows moving cyan noise.
- Alpha.40 HD works and is playable on the real A1200/68030 when roughly
  1.92 MB Chip RAM is free, including Boot With No Startup-Sequence and a
  two-colour Workbench. It fails after CHARGING with 1,430,032 Chip bytes free.
  This isolates the normal-Workbench failure to the current Chip-RAM budget,
  not MaxTransfer, the HD device or the gameplay renderer itself.
- Alpha.41 calculates about 642 KiB permanent Chip savings plus 54 KiB during
  status loading, without removing reachable pixels or animation frames. Its
  HD build loads and plays on the real A1200/68030 from a normal Workbench with
  about 1.4 MB Chip RAM free. This accepts the real-HD Chip-RAM gate. Its ADF
  uses 1,177 blocks and leaves 583; alpha.41 ADF regression is still pending.
- Real-A1200/68030 alpha.40 and alpha.41 footage both reject presentation in
  the first two-Strider scene due to intermittent enemy glitches and apparent
  cadence loss. Beetles are also reported to glitch occasionally. Because the
  issue predates alpha.41, do not blame its Fast-RAM masters or staging change
  without new evidence.
- Alpha.42 promotes the Stage 4G no-copy rolling renderer. Supplied FS-UAE/HD
  testing accepts clean enemy/projectile presentation, smooth cadence and the
  corrected HUD boundary. Its 1,983 measured intervals contain 1,952 one-field
  and 31 two-field updates (49.23 effective FPS), with zero ownership
  violations. New supplied recordings reject alpha.42 performance on FS-UAE
  at 68020 and on the real A1200/68030 at about 34.5 MHz. They do not show an
  obvious return of corruption or trails in sampled frames. The 49.23-FPS
  result is specific to the faster FS-UAE/68030 configuration and must not be
  extrapolated to target hardware. Alpha.42 ADF and Analogue Pocket acceptance
  remain pending; real-A1200 performance is explicitly rejected.
  The bootable alpha.42 ADF uses 1,186 blocks and leaves 574; this is package
  validation only, not ADF gameplay acceptance.
- Stage 5L keeps the coherent Stage 5G early-fetch playfield and replaces the
  six-channel player DMA layout with the same 48x48, 15-colour pixels inside
  one transparent-padded 64-pixel attached AGA pair on channels 0/1. Supplied
  FS-UAE/68030 HD testing reports no corruption, glitches or flicker. Its log
  records 2,163/2,163 one-field intervals (50.00 FPS) and zero ownership
  violations. Alpha.43 promotes that exact route to normal HD and ADF builds.
  Supplied real-A1200/68030 HD and physical-ADF tests, plus Analogue Pocket ADF,
  report no broad renderer corruption. All slower paths reject cadence; real HD
  also repeats or misses some sound events under load. A narrow intermittent
  ground/HUD seam disturbance remains. Package validation reports a bootable
  DOS1/FFS ADF using 1,190 blocks and leaving 570 free.
- Stage 5L in FS-UAE/68020 measures 26.38 effective FPS (136 one-field, 337
  two-field and 78 three-field intervals). This stress configuration is slower
  than the supplied real 34.5 MHz 68030, while FS-UAE/68030 is much faster.
- Alpha.39 ADF on an Analogue Pocket FPGA core shows widespread transient Bob,
  gameplay-field and HUD corruption at 68020/no-cache. Treat this as a useful
  missed-deadline stress signal, not as FS-UAE or real-A1200 equivalence.
- ADF full gameplay parity beyond the supplied real-hardware working result is
  not generalized to every configuration.

Evidence handling: inspect consecutive video frames, preserve bytes, rename new
timestamped files meaningfully and add matching `.txt` sidecars. Separate
observation from diagnosis.

## Display and renderer invariants

- PAL 320x256, resident 3072x256 world.
- AGA dual playfield: four-plane FRONT16 plus three-plane REAR8 at quarter
  camera scroll.
- Player remains a 48x48 15-colour actor, transparently padded in one attached
  64-pixel AGA hardware-sprite pair on DMA channels 0/1.
- Gameplay foreground uses two hidden/displayed target pairs. Each target has
  a logical 512px FRONT16 ring repeated across a 1536px physical stride; the
  resident clean world remains the canonical source for entering columns.
- Two complete Copper lists publish target/pointer state atomically at a fixed
  PAL boundary. CPU and Blitter never modify the displayed target.
- Copper switches to the separate 320x48 HUD at hardware line 252.
- Target-local Bob composition starts after update on the inactive target.
- Active family order: projectile erase, enemy restore, collectible restore,
  splash restore, water maintenance, splash draw, collectible draw, enemy draw,
  projectile draw, final Blitter wait.
- Use packed masks/caches, camera culling and Blitter copy/cookie-cut minterms.
- Never CPU read-modify-write displayed Chip RAM.
- Preserve the alpha.33 stable vertical enemy ordering and intersecting-Strider
  restore union.
- Alpha.37 tight per-frame enemy bounds is rejected: it damaged sprites.
- Alpha.38 hidden 512x208 viewport copying is rejected: it worsened 68020 and
  68030 cadence and logged 3,324 wraps in 3,722 compositions.
- Normal production logging must not insert per-family `WaitBlit` barriers.

The background pictures are resident bitplanes fetched by Agnus, not CPU-
redrawn every frame. Their DMA cost remains a valid performance measurement,
but “background rendering” is not an assumed CPU bottleneck.

## Current gameplay contracts

Player:

- Frames 0..49 accepted base; standing hurt 50..53, crouched hurt 54..57 and
  ledge balance 58..61 are append-only.
- Preserve scale, feet, mirrored facing, run/jump/landing/crouch/turn/shoot/hurt
  selection, three hearts as six health units and accepted life/reset behavior.
- Sprint/jump performance is open; do not change physics before measuring it.

Enemies/projectiles:

- Four camera-managed runtime enemy slots retain persistent spawn state.
- Beetles are 32x24, two HP. Standing fire naturally misses floor beetles by Y;
  crouch fire hits them; geometric standing fire hits elevated beetles.
- Striders are 64x64 FRONT16 Bobs with 28 append-only frames, three HP,
  authored traversal, ranged fire, hurt/death and off-camera persistence.
- Logical Strider collision Y is unchanged; drawing retains the accepted +2px
  visual offset for transparent source rows 62-63.
- Player shots sweep from the physics edge through each crossed X coordinate,
  testing solid geometry before enemy damage.
- Off-screen projectiles and water strips are camera-culled with synchronization
  on entry.

HUD/collectibles/audio:

- HUD is separate and internally double-buffered; only changed counters patch.
- Forty-eight masked diamonds hover through synchronized clean/display updates;
  counter/life award behavior is accepted. Diamond-art replacement is deferred.
- Player plasma owns Paula channel 0. Prioritized gameplay effects own channel
  1; player hurt outranks enemy death, which outranks Strider fire.

Water/route:

- Two animated water hazards and dry gaps are active in the accepted route.
- Full level traversal through the corrected second-water platform is accepted
  in FS-UAE/HD. ADF and real-hardware claims stay evidence-specific.

## Immediate work order

### 1. Correct the remaining HUD seam without reopening Stage 5L

Stage 5L is the immutable renderer baseline. Move or retime only the fixed-HUD
FMODE/pointer setup at the playfield-to-HUD boundary, then gate the isolated
candidate in FS-UAE/68030 before slower testing. Do not change ring ownership,
early-fetch geometry, the wide Sparkpaw pair, art or gameplay to hide the seam.

The title contract is unchanged: 35 black PAL frames after display takeover,
24 fade frames and 225 fully visible title frames. Faster loading before title
takeover must not be confused with a shortened Indivision stabilization delay.

### 2. Optimize only the measured hotspot

Use `sparkpaw/docs/PERFORMANCE_68020_PLAN.md`.

First build a production executable without invasive synchronization and a
separate CIA-timer profiler capable of measuring multiple PAL frames. Measure
game update, player physics states, enemy/projectile simulation, clean-world
maintenance, each Bob family submission and final Blitter stall.

Primary hypotheses to test, in this order:

- replace thousands of two-byte `CopyMem` calls during an entering-column ring
  update with direct word copies or a bounded tall Blitter transfer;
- avoid copying the complete inactive Copper list every update;
- cache each target's wide-player frame/facing and avoid recopying unchanged
  sprite image data while still updating its control words;
- reduce serialized per-plane Bob waits while preserving accepted ordering;
- per-pixel `moveY()` plus full sole scans may cause sprint/jump CPU spikes;
- collectible/enemy iteration or collision sweep may contribute.

The diagnostic implementation is compiled out of production already. Split it
from `renderer.c` for auditability, and later split Copper/ring/Bob/sprite
modules, only through small commits whose normal executable remains byte-for-
byte identical. Source-file size and excluded `#ifdef` branches are not FPS
costs. Do not combine this mechanical refactor with a renderer optimization.

Preserve sprites, colours, 4+3 dual playfield and art. Do not retry alpha.37
bounds or alpha.38 full viewport copies. Benchmark candidates in isolation and
gate first on unchanged 68030 presentation, then FS-UAE 68020 timing.

### 3. Deferred visual work

Only after HD and performance acceptance, replace the world diamond as a
separate art checkpoint: native 16x21 indexed sprite, pen 0 only outside the
silhouette, opaque dark contour/facets and a clean lower point. Preserve Bob
size, mask/cache, hover and draw/restore order.

### 4. Deferred ADF loading optimization

Use `sparkpaw/docs/ADF_LOAD_OPTIMIZATION_PLAN.md`. First measure physical FFS
layout versus runtime file order, then test a byte-identical layout-only ADF.
Consider a sequential ADF-only container only if layout changes are insufficient.

## Documentation rule

Keep this file concise and current. Append implementation narrative and rejected
experiments to `docs/DEVELOPMENT_HISTORY.md`. Update README, handoff, history,
packaged notes and SemVer together for every release candidate.

My request is:
