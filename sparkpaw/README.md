# Sparkpaw: The Stormstone Quest

Milestone 2A of an original Commodore Amiga 1200 AGA action platformer by
MrDig Productions.

This is a deliberately small but real engine test. It validates the risky
parts before broader enemy variety, music and level progression are added: a
native AGA dual-playfield display, two independently
hardware-scrolled five-screen world layers, a fluid 15-colour hardware-sprite
player, joystick input, solid platform collision, plasma projectiles and a
bounded clockwork-beetle vertical slice.

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
- Keyboard: `A`/`D` move, `W` jumps, `S` crouches and space shoots
- Reset the Amiga or emulator to leave this engine milestone

Each separate fire press launches a fast blue/cyan plasma pulse from
Sparkpaw's right-hand gauntlet. Up to six pulses can remain in flight, so the
weapon responds to rapid tapping. Four guaranteed and up to two optional low
clockwork beetles patrol safe authored zones; their exact X positions and
48/96/192 movement speeds vary on each complete test replay. Their independent
walk cadence follows the selected speed. Standing and
airborne shots deliberately pass over
them: crouch and fire twice to destroy each one through a hit reaction and
four-stage destruction sequence. Contact with an active beetle now removes one
of six internal half-heart health units, applies brief knockback/input lock and
grants one second of invulnerability. Dedicated standing and crawl-height
hurt art is present; the eventual game-over presentation remains a focused
later step. A full-width fixed HUD band across the bottom now shows the
existing six health units as three full, half or empty hearts and reserves
separate framed panels for the active life counter and diamonds. Original
player-hurt, enemy-hit and accepted-jump effects now share a prioritized Paula
gameplay channel;
the existing rapid plasma sound remains independently available. Destroyed
beetles receive a five-to-ten-second cooldown and can respawn indefinitely only
after their complete patrol zone is safely outside the camera. Returning through
earlier level areas therefore creates fresh encounters. Reaching the far-right
world edge temporarily resets the player, camera, projectiles, collectibles and
enemy encounter state in memory without reloading resident level assets. This
right-edge replay stands in for the later `LEVEL_COMPLETE -> next level` flow.
Mouse exit is disabled because clean Workbench
restoration remains a separate technical milestone; reset the Amiga or
emulator to leave the current build.

Phase 5C.2 also places two non-interactive Clockwork Storm Strider render
proofs: one frozen in idle on a raised platform and one frozen on the floor.
They deliberately have no patrol, projectile hitbox or contact damage yet.
This isolates validation of their 64x64 packed Bob cache, type-specific
culling and background restoration from the later AI/collision step. Their
cool navy/violet/blue armour and cyan energy identity deliberately avoid the
warm orange palette shared by Sparkpaw and the beetles.

The HUD also shows the current attempt stock. A new test run starts at `x3`;
each zero-health reset steps through `x2` and `x1`. Until the dedicated
game-over state is implemented, losing the third attempt starts a fresh `x3`
test cycle. Reaching the right edge replays the test level while preserving the
current life stock and diamond count; final level-completion persistence will
be defined with real progression.
The HUD is modular rather than a table of complete life/health combinations:
one static base, compact health and lives patch atlases, and two presentation
buffers keep updates tear-free. Only a stale dynamic panel is copied with the
Blitter when its value changes; gameplay never CPU-composites displayed Chip
RAM. Twenty fixed diamonds form short trails and original arcs across the
five-screen test level. One fixed HUD-matched silhouette and a gentle
two-pixel hover animate their camera-culled Bobs without rotation. Picking one
up removes its Bob, plays a short arcade collect effect and increments the
compact two-digit HUD counter. The fiftieth pickup changes `49` to `00` and
awards one life. The display supports `x1` through `x9`; at `x9` the counter
remains at 49 rather than silently consuming unrewarded pickups.

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

- `src/main.c`: startup, cleanup, explicit application states and the
  raster-phased top-level loop
- `src/title.c` / `src/title.h`: direct six-plane AGA title/loading presenter
  with ordinary (non-EHB) 64-colour palettes, black border blanking, smooth
  VBlank-synchronised 24-bit palette fades, Chip RAM Copper lists and complete
  screen switches while DOS remains live

The startup presentation leaves a black PAL display briefly in place for
scandoubler lock, then keeps the fully faded title visible before loading.
While files are read the loading screen says `LOADING`; once disk I/O is done
it switches to `CHARGING` during the silent sprite, Bob, bitplane and Copper
preparation phase. This second phase remains visible for at least two seconds,
including on accelerated systems. Both screens share one 64-colour palette.
- `src/renderer.c` / `src/renderer.h`: gameplay display, Copper construction,
  six-channel hardware-sprite player, fixed bottom HUD Copper split, packed
  render caches and Bob rendering behind an explicit renderer API
- `src/game.c` / `src/game.h`: gameplay initialization and update ordering,
  frame progression and camera state
- `src/platform_amiga.c` / `src/platform_amiga.h`: graphics-library lifetime,
  custom-chip takeover/restore, raster reads and the required Blitter wait;
  Copper construction and concrete rendering commands remain in `renderer.c`
- `src/enemies.c` / `src/enemies.h`: fixed enemy pool, patrol AI, hit detection
  and damage state; runtime slots retain an explicit link to their spawn record
- `src/level_data.c` / `src/level_data.h`: compact typed enemy spawn records
  containing safe position ranges, authored `{left,right,groundY}` patrol
  surfaces, initial direction and persistence policy; four beetle encounters
  are required and two are optional. Two required and one optional Strider
  record are prepared but deliberately runtime-disabled until its Bob consumer
  exists
- `src/projectiles.c` / `src/projectiles.h`: projectile pool, spawn, movement,
  impact state and hit dispatch; packed plasma rendering remains with the
  renderer-sensitive code in `renderer.c`
- `src/collision.c` / `src/collision.h`: collision-map loading and solid-point,
  horizontal-span and vertical-span tile queries shared by gameplay modules
- `src/player.c` / `src/player.h`: player state, joystick input, shooting,
  movement/physics, the accepted 50-frame baseline and eight appended standing
  and crouched hurt poses; hardware
  sprite preparation and Copper updates remain in `renderer.c`
- `src/assets.c` / `src/assets.h`: SPBM loading, validation, gameplay-asset
  lifetime and cleanup plus a separate early-title lifetime; packed
  hardware-sprite and Bob cache preparation remains in `renderer.c`
- `src/audio.c` / `src/audio.h`: energy-shot, player-hurt, enemy-hit and jump
  sample loading, Paula channel 0 rapid-shot playback, prioritized channel 1 gameplay effects
  and explicit hardware-active lifecycle control; channels 2-3 remain reserved
  for a future music layout
- `tools/generate_runtime_assets.py`: creates wide planar playfields, source
  sprite/enemy planes and masks, packed Bob caches and the tile collision map
- `tools/generate_sparkpaw_sfx.py`: regenerates the Paula-ready raw samples
- `tools/make_release.py`: creates the HD archives, source archive and ADF
- `assets/runtime/`: compact data loaded by the Amiga executable
- `assets/concept/`: full-resolution concept art and AGA preview conversions
- `assets/sprites/`: prototype animation art and named frame metadata
- `assets/enemies/`: native-resolution enemy art, preview and frame metadata
  (including the 64x64, eighteen-frame upright Clockwork Storm Strider palette
  proof and preserved earlier concept/scale review sources)
- `assets/sfx/previews/`: WAV previews for later milestones
- `sfx/raw/`: signed 8-bit mono Paula-ready samples; the current build uses the
  energy-shot, player-hurt, enemy-hit and jump samples and reserves the others
  for later steps

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
pose. Frames 0-49 use one anatomical scale per authored family
and stable shared anchors. It includes an eight-stage grounded run, a
scale-locked four-stage jump, three-stage landing, six-stage momentum-based
direction change, twelve-stage idle performance and dedicated grounded,
airborne and crouched firing poses. Frames 50-53 append contact impact,
backward slide, airborne recoil and recovery without renumbering the accepted
baseline. Frames 54-57 provide a separate 24-pixel-high crawl-recoil family so
contact recoil respects the low crouch clearance beneath a platform.
Crouching changes posture without enlarging
the character. More in-between poses can be added without changing the DMA
renderer.

For the Phase 5C.2 proof, verify that exactly two static Striders appear: a
raised-platform instance in the early level and a floor instance farther
right. Walk both on and off screen repeatedly, fire plasma across them and
trigger a life-loss or right-edge reset. They must remain visually anchored,
must not damage Sparkpaw or absorb shots yet, and must leave no stale 64x64 Bob
pixels. Beetle behaviour should remain unchanged.

The Milestone 2A beetle art is a 32x24, nine-frame, three-plane masked Bob.
Four to six level instances share one packed art cache and retain independent
HP and animation state. A bounded four-slot runtime pool activates them near
the camera and parks their state when safely distant. Their fixed height keeps
ordinary standing and airborne shots above them, while the domed steel/violet
shell, round cyan lens and jointed legs follow the
gameplay concept at native AGA resolution. Enemy and plasma restore/draw passes
use synchronized Blitter DMA with standard-copy and cookie-cut minterms; the
68020 no longer composites their planar rows byte by byte in Chip RAM. Verify
that all selected beetles walk smoothly between their patrol limits, mirror cleanly
when turning, remain grounded, ignore standing and airborne fire, react to the
first crouch-shot and play all four destruction stages after the second.
Stress the renderer with several simultaneous plasma pulses and visible
beetles, and watch for residue where enemies and projectiles overlap.
