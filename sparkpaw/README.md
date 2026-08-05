# Sparkpaw: The Stormstone Quest

Milestone 2A of an original Commodore Amiga 1200 AGA action platformer by
MrDig Productions.

This is a deliberately small but real engine test. It validates the risky
parts before broader enemy variety, player damage, music and level progression
are added: a native AGA dual-playfield display, two independently
hardware-scrolled five-screen world layers, a fluid 15-colour hardware-sprite
player, joystick input, solid platform collision, plasma projectiles and a
four-instance clockwork-beetle vertical slice.

## Target

- Commodore Amiga 1200 or compatible AGA Amiga
- Motorola 68020
- 2 MB Chip RAM; Fast RAM recommended
- PAL 320x256 at a fixed 50 Hz
- Two stable 8-colour AGA playfields with true quarter-speed rear parallax
- 48x48 Sparkpaw poses made from three attached sprite pairs, with 24-bit AGA colour
- C with VBCC; small reproducible Python asset and release tools

## Controls

- Joystick port 2: left/right to run, up to jump and fire to shoot
- Hold down to crouch; down plus left/right performs a slower crouch-walk
- Press fire while crouching or crouch-walking to shoot from a dedicated low pose
- Reset the Amiga or emulator to leave this engine milestone

Each separate fire press launches a fast blue/cyan plasma pulse from
Sparkpaw's right-hand gauntlet. Up to six pulses can remain in flight, so the
weapon responds to rapid tapping. Four low clockwork beetles patrol separate
sections of the test level. Standing and airborne shots deliberately pass over
them: crouch and fire twice to destroy each one through a hit reaction and
four-stage destruction sequence. These enemies cannot damage Sparkpaw and do
not respawn yet. Keyboard controls and the HUD are deferred until after the
planned source modularisation. Mouse exit is disabled because clean Workbench
restoration remains a separate technical milestone; reset the Amiga or
emulator to leave the current build.

## Build

Install a project-local VBCC/NDK toolchain under `.toolchain/` as described in
the repository's [build guide](../docs/BUILDING.md). The directory is
intentionally ignored by Git and must not depend on the ChipSnake or Futsal
toolchains. Install the host Python requirements from the repository root with
`python3 -m pip install -r requirements-dev.txt`. Then, from this directory:

```sh
make
```

This regenerates planar runtime assets and builds the native executable
`sparkpaw`. Run `make release` to rebuild all test packages:

- `dist/Sparkpaw-Milestone2A-A1200.lha`
- `dist/Sparkpaw-Milestone2A-A1200.zip`
- `dist/Sparkpaw-Milestone2A-A1200.adf`
- `dist/Sparkpaw-Milestone2A-Source.zip`

The ADF contains `S/startup-sequence` and boots directly into exactly the
same executable and asset files as the HD release.

Run `make bench` to build the isolated `sparkpaw-renderbench`. This small
program validates the dual-playfield foundation before it is allowed back
into the game; see `docs/RENDERBENCH.txt`.

## Source layout

- `src/sparkpaw.c`: current main loop, display, camera and renderer
  implementation
- `src/enemies.c` / `src/enemies.h`: fixed enemy pool, patrol AI, hit detection
  and damage state; this is the first incremental modularisation boundary
- `src/projectiles.c` / `src/projectiles.h`: projectile pool, spawn, movement,
  impact state and hit dispatch; packed plasma rendering remains with the
  renderer-sensitive code in `sparkpaw.c`
- `src/collision.c` / `src/collision.h`: collision-map loading and solid-point,
  horizontal-span and vertical-span tile queries shared by gameplay modules
- `src/player.c` / `src/player.h`: player state, joystick input, shooting,
  movement/physics and the established 50-frame animation selection; hardware
  sprite preparation and Copper updates remain in `sparkpaw.c`
- `tools/generate_runtime_assets.py`: creates wide planar playfields, source
  sprite/enemy planes and masks, packed Bob caches and the tile collision map
- `tools/generate_sparkpaw_sfx.py`: regenerates the Paula-ready raw samples
- `tools/make_release.py`: creates the HD archives, source archive and ADF
- `assets/runtime/`: compact data loaded by the Amiga executable
- `assets/concept/`: full-resolution concept art and AGA preview conversions
- `assets/sprites/`: prototype animation art and named frame metadata
- `assets/enemies/`: native-resolution enemy art, preview and frame metadata
- `assets/sfx/previews/`: WAV previews for later milestones
- `sfx/raw/`: signed 8-bit mono Paula-ready samples; the current build uses the
  energy-shot sample and reserves the others for later milestones

## What to test

Walk through all five screen widths, jump onto and off every platform and
reverse direction frequently. Verify that the foreground follows the camera
while the mountain/tower layer moves at one quarter speed. Compare HD and ADF
behaviour. Useful reports include the exact location and whether the issue
concerns sprite flicker, tearing, collision, camera movement, parallax,
controls or display colours. Combine camera movement with rapid fire and watch
specifically for a one-frame mixed-scroll tear under both quiet and heavy
Blitter workloads.

Check especially whether Sparkpaw retains the same apparent size and foot
position while running, jumping, crouching, shooting and performing the idle
pose. The 50-frame animation set uses one anatomical scale per authored family
and stable shared anchors. It includes an eight-stage grounded run, a
scale-locked four-stage jump, three-stage landing, six-stage momentum-based
direction change, twelve-stage idle performance and dedicated grounded,
airborne and crouched firing poses. Crouching changes posture without enlarging
the character. More in-between poses can be added without changing the DMA
renderer.

The Milestone 2A beetle art is a 32x24, nine-frame, three-plane masked Bob.
Four fixed instances share one packed art cache, retain independent HP and
animation state, and are drawn only near the camera. Their fixed height keeps
ordinary standing and airborne shots above them, while the domed steel/violet
shell, round cyan lens and jointed legs follow the
gameplay concept at native AGA resolution. Enemy and plasma restore/draw passes
use synchronized Blitter DMA with standard-copy and cookie-cut minterms; the
68020 no longer composites their planar rows byte by byte in Chip RAM. Verify
that all four walk smoothly between their fixed patrol limits, mirror cleanly
when turning, remain grounded, ignore standing and airborne fire, react to the
first crouch-shot and play all four destruction stages after the second.
Stress the renderer with several simultaneous plasma pulses and visible
beetles, and watch for residue where enemies and projectiles overlap.
