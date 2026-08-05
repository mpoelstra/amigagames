# Sparkpaw: The Stormstone Quest

Milestone 2A of an original Commodore Amiga 1200 AGA action platformer by
MrDig Productions.

This is a deliberately small but real engine test. It validates the risky
parts before enemies, music and level content are added: a native
AGA dual-playfield display, two independently hardware-scrolled five-screen
world layers, a fluid 15-colour hardware-sprite player, joystick input and
solid platform collision and the first complete enemy vertical slice.

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
not respawn yet.
Keyboard controls and the HUD are temporarily absent. Mouse exit is
disabled so an accidental click cannot interrupt a test. Clean Workbench
restoration remains on the technical backlog.

## Build

The workspace copy contains a private `.toolchain` so it does not depend on
the ChipSnake or Futsal directories. From this directory:

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

- `src/sparkpaw.c`: display, camera, input, movement, animation and collision
- `tools/generate_runtime_assets.py`: creates wide planar playfields, source
  sprite planes/mask and the tile collision map
- `tools/make_release.py`: creates the HD archives, source archive and ADF
- `assets/runtime/`: compact data loaded by the Amiga executable
- `assets/concept/`: full-resolution concept art and AGA preview conversions
- `assets/sprites/`: prototype animation art and named frame metadata
- `assets/enemies/`: native-resolution enemy art, preview and frame metadata
- `assets/sfx/previews/`: WAV previews for later milestones
- `sfx/raw/`: signed 8-bit mono Paula-ready samples for later milestones

## What to test

Walk through all five screen widths, jump onto and off every platform and
reverse direction frequently. Verify that the foreground follows the camera
while the mountain/tower layer moves at one quarter speed. Compare HD and ADF
behaviour. Useful reports include the exact location and whether the issue
concerns sprite flicker, tearing, collision, camera movement, parallax,
controls or display colours.

Check especially whether Sparkpaw retains the same apparent size and foot
position while running, jumping, crouching, shooting and performing the idle pose. The
50-pose animation source uses one anatomical scale per authored family and a
shared pixel-exact foot baseline. It includes an eight-stage grounded run,
a scale-locked four-stage jump, three-stage landing, six-stage momentum-based
direction change and twelve-stage idle
performance plus four dedicated crouch-fire poses; crouching changes posture
without enlarging the character. More
in-between poses can be added without changing the DMA renderer.

The Milestone 2A beetle is a 32x24, nine-frame, three-plane masked Bob. Four
instances share one packed art cache and only camera-near beetles are drawn.
Its
fixed height keeps ordinary standing and airborne shots above the enemy, while
its domed steel/violet shell, round cyan lens and jointed legs follow the
gameplay concept at native AGA resolution. Enemy and plasma restore/draw passes
use synchronized Blitter DMA with standard-copy and cookie-cut minterms; the
68020 no longer composites their planar rows byte by byte in Chip RAM. Verify
that all four walk smoothly between their fixed patrol limits, mirror cleanly
when turning, remain grounded, ignore standing and airborne fire, react to the
first crouch-shot and play all four destruction stages after the second.
Stress it with several simultaneous plasma pulses and watch for residue where
the enemy and projectiles overlap.
