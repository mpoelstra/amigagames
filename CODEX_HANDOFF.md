# Codex handoff: Amiga game prototypes

Last updated: 5 August 2026

## Purpose of this file

This repository is the shared development workspace for three native Commodore
Amiga game prototypes created by MrDig Productions with extensive AI assistance.
It is deliberately documented for starting a fresh Codex task without carrying
the very long original conversation.

At the start of a new task, read this file completely and then read the README
inside the game directory being changed. Inspect the current files before
editing: this document is context, while source code and generated manifests are
the final authority.

The repository is versioned in Git. `main` is the accepted shared state and the
annotated tag `sparkpaw-pre-modularisation` identifies the stable four-beetle
Sparkpaw baseline before the planned source split. Check `git status`, recent
commits and relevant tags before editing. Make a focused commit after a
validated step; do not combine unrelated renderer, gameplay and asset work.

Historical ZIP backups, test recordings, toolchains, build trees and release
archives remain local and are intentionally ignored by Git. Do not delete or
overwrite them. Git is the normal recovery path for committed source changes;
make an additional dated ZIP before high-risk renderer, audio, physics, asset
format or bulk source-movement work where ignored/generated files also matter.

## Repository map

```text
amigagame/
  CODEX_HANDOFF.md       this handoff
  chipsnake/             finished/releasable Snake prototype
  mrdigs-futsal/         finished/releasable indoor football prototype
  sparkpaw/              active AGA action-platformer prototype
  backups/               ignored local snapshots; never delete casually
  ACM_PDF/               ignored local Amiga C reference manuals
```

`AlexInTown`, if present in the workspace, is third-party/reference source used
only to study older Amiga C techniques. Do not edit or redistribute it as part
of these games unless MrDig explicitly requests that.

Each game must remain self-contained. Never make one game depend on tools,
libraries, music, or assets inside another game directory. ChipSnake and Futsal
already contain separate copies of Light Speed Player; Sparkpaw has its own
toolchain and asset pipeline.

## Shared development environment

- Host: macOS, usually tested through FS-UAE configured as an Amiga 1200.
- Target compiler: workspace-local VBCC/VASM under each game's `.toolchain`.
- NDK headers: each game uses `.toolchain/ndk/Include_H`.
- Asset/release tooling: Python 3, Pillow and small project-owned scripts.
- Primary real-machine target: PAL Amiga 1200, Motorola 68020, 2 MB Chip RAM,
  with Fast RAM recommended.
- ChipSnake and Futsal are conservative classic-Amiga applications; Sparkpaw is
  intentionally AGA-specific.
- Never replace the local toolchain by adding an undeclared dependency on a
  sibling directory.

Usual commands, run from the selected game directory:

```sh
make              # regenerate changed assets and compile
make release      # rebuild executable plus ZIP/LHA/ADF/source packages
make clean        # remove executable/object outputs, then make again
```

MrDig commonly mounts the repository root directly as a hard-drive directory
in FS-UAE and launches the root executable repeatedly. Therefore always rebuild
the root executable as well as releases after code changes. Do not report a
change complete merely because a source file was edited.

## MrDig's FS-UAE test and feedback loop

MrDig performs the authoritative emulator and real-hardware testing. Codex can
cross-build, inspect binaries/assets and analyse supplied evidence, but must not
claim that animation, sound, controls or real-A1200 timing were verified merely
because compilation succeeded.

### How builds are normally tested

- The repository root is commonly mounted directly as an FS-UAE hard drive.
  MrDig opens the selected game drawer and
  launches its newly rebuilt root executable (`snake`, `futsal` or `sparkpaw`).
  For this workflow the release ZIP does not need to be unpacked after every
  iteration; the file in the game directory is the current test binary.
- The normal machine is an Amiga 1200 with Kickstart 3.1, PAL display, 68020,
  2 MB Chip RAM and some Fast RAM. Emulator CPU speed has sometimes been set
  faster than stock. Performance reports should therefore include the relevant
  FS-UAE CPU/speed setting, particularly when judging a 50-fps Sparkpaw build.
- Test the ADF separately when floppy/HD parity matters. ADF startup uses
  `S/startup-sequence` and can expose cache, SetPatch, loading and timing
  differences hidden by a Workbench launch.
- Reset before a clean comparison when direct custom-chip code was previously
  run. Sparkpaw currently has no safe Workbench exit and is intentionally left
  via Amiga/emulator reset.
- Codex must run `make` after implementation and normally `make release` too.
  Check output timestamps and generated asset manifests before asking MrDig to
  retest, so an old executable is not mistaken for the fix.

### What evidence to supply

- A screenshot is best for palette mistakes, clipped art, wrong scale, baseline
  alignment, corrupt planar data, UI placement and a stable collision position.
- A MOV/MP4 is best for animation cadence, transitions, apparent zooming,
  turning momentum, sprite flicker, tearing, scrolling, collision response,
  audio timing and intermittent glitches. Descriptive names such as
  `crouch-shoot-left-right.MOV` or `landing-residue.MOV` are valuable.
- In the accompanying message, state the intended action and the observed
  problem. Include an approximate timestamp when only part of a longer movie
  matters, for example: “at 0:07 the muzzle jumps down” or “around 0:23 one
  display glitch occurs.”
- For input problems, say which port/device and the exact sequence: idle,
  direction held, fire pressed/released, jump/crouch state, and whether the
  problem occurs from HD, ADF or both.
- For audio, identify the exact MOD/effect and capture path. When comparing
  players, provide matching recordings of the same file in VLC and in
  ProTracker/HippoPlayer/gameplay. Phone recordings are useful, but direct
  emulator capture is better for pitch and timing when available.
- For performance, show a repeatable stress case: number of simultaneous shots,
  whether Sparkpaw is running/jumping, approximate point in the level and the
  configured CPU speed. “Feels slow” alone is not enough to isolate the hot
  path.
- A short focused clip is preferable to a long unfocused recording, but never
  trim away the input immediately before a bad transition.

### How Codex should analyse supplied tests

1. Read the user's description first and treat it as the expected-versus-actual
   specification. Do not dismiss a perceived scale or motion problem merely
   because numeric bounding boxes appear consistent.
2. Inspect video metadata with `ffprobe`, then use `ffmpeg` to extract frames at
   relevant timestamps. For motion issues, extract consecutive frames or a
   contact sheet; for subtle scale/baseline problems, crop the actor to an
   identical screen rectangle and compare feet, head, torso and gauntlet.
3. Watch the complete supplied clip when its sequence matters. Sampling only
   the first/last frame misses flicker, one-frame residue and transition bugs.
4. Distinguish observations from hypotheses. Example: “frame 3 contains pixels
   from the preceding pose” is evidence; “the mask restore is late” is a cause
   to verify in source/runtime data.
5. Correlate the visual sequence with animation IDs, timers, facing state,
   physics state and the generated indexed sprite preview. Check both the
   high-resolution source sheet and final 48x48 quantised cells.
6. For corruption/flicker, determine whether the wrong pixels exist in the
   generated asset, sprite mask/planes, Copper pointer update, Bob restore, or
   only after display. Change the narrowest proven subsystem.
7. For collisions, compare the drawn silhouette, physics origin, hitbox and tile
   coordinates separately. Visual art bounds must not silently define physics.
8. For audio, inspect MOD/sample headers and effects, compare converter output,
   VBlank playback rate and Paula channel cleanup. Modern VLC output is only a
   reference, not proof of authentic replay.
9. If a problem is intermittent or cannot be resolved visually, add bounded
   diagnostic counters or a compact log. Do not write every frame from the
   50-Hz gameplay loop: disk I/O changes the timing being measured. Prefer a
   small in-memory trace, occasional samples, or a log written outside the hot
   path.
10. Make one focused fix, rebuild executable and releases, state exactly what
  changed and give MrDig a short regression checklist. Commit accepted source
  milestones and preserve any relevant ignored backup until the retest
  succeeds.

Phone-filmed displays can introduce camera exposure, refresh beating, scaling
and moire. They remain excellent evidence of major motion/corruption, but do
not calculate exact emulator frame rate or palette values from a phone movie
without corroborating data.

## Visual-production workflow: concept art to Amiga asset

Concept art is a design reference, not automatically a runtime-ready asset.
The established workflow is:

1. Establish the target purpose, exact Amiga dimensions, palette budget,
   transparency/index rules and collision meaning before generating art.
2. Generate or edit a high-resolution concept. Preserve the prompt and the
   selected source PNG in the relevant game's `assets/concepts/` or
   `assets/sprites/` directory.
3. Clean the concept on the host: crop, remove unwanted baked text or AI
   fragments, chroma-key if required, and inspect the transparent PNG.
4. Use the project's Python converter to resize and quantise deterministically.
   Do not let the Amiga executable decode arbitrary modern PNG data.
5. Produce a development PNG preview next to the runtime asset and inspect it
   at integer scale. The quantised preview, not the large concept, predicts
   what the Amiga will display.
6. Build the executable so the generated file is exercised by the same loader
   used on the Amiga.
7. Test in FS-UAE and, for timing/palette-sensitive work, on the A1200. A valid
   PNG preview does not prove correct planar layout, palette indices or DMA.
8. Record meaningful generation prompts and special conversion decisions in
   project documentation so later frames can match the established style.

Never hand-copy an unrelated converter from another game. Reuse ideas, but
keep the actual converter, source art, palettes and dependencies inside the
game directory that owns the asset.

### IFF/ILBM conventions in this workspace

- ChipSnake backgrounds are uncompressed 320x216, four-plane, 16-colour ILBM.
  `chipsnake/tools/png_to_ilbm.py` uses a fixed palette and deliberately maps
  source art only to pens 0-11. Pens 12-15 remain reserved for game objects and
  UI. The generated `SNKM` chunk is useful metadata, while current collision is
  derived from palette indices in the centre of each 8x8 cell.
- The ChipSnake illustrated intro is a separate 320x256 asset. Do not apply the
  320x216 playfield crop to it.
- Futsal title/options screens are uncompressed 320x256, four-plane, 16-colour
  ILBM. `png_to_title_ilbm.py` fits and pads the title concept. The options
  converter removes baked labels and recreates the row frames at final Amiga
  resolution; C draws selectable labels and values dynamically.
- Do not bake changing instructions, scores or menu values into concept art.
  Keep decorative structure in the IFF and live information in C.
- ILBM BODY rows are planar and word/byte aligned as required by the target
  format. Chunk lengths are big-endian and padded to an even byte count.
- ChipSnake's current loader accepts uncompressed ILBM. ByteRun1-compressed
  Deluxe Paint files remain backlog; do not claim support before implementing
  and testing it.

Sparkpaw does not use ILBM for the scrolling runtime. Its host tool emits a
compact project-specific `SPBM` format:

```text
SPBM + width(u16 big-endian) + height(u16 big-endian)
     + depth(u8) + flags/reserved(u8) + rowBytes(u16 big-endian)
     + palette + planar bitmap bytes [+ mask]
```

Rows are word aligned with `((width + 15) / 16) * 2` bytes. Generated wide
foreground, rear playfield, collision and sprite files belong in
`sparkpaw/assets/runtime/`; their source PNGs belong under `assets/` and must
not be replaced by manually edited binary data.

## Sparkpaw sprite and animation asset contract

This section is mandatory reading before creating any additional Sparkpaw
pose. Several visible regressions—character zooming, floating feet, clipped
heads/hands, residue and sideways jumps between frames—were caused by breaking
one of these rules.

### Authored pose requirements

- Sparkpaw is an original amber-orange feline hero with a navy scarf and teal
  energy gauntlet. New art must match the established pixel density, outline,
  palette, proportions and perceived body mass.
- Generate right-facing poses only. The runtime generator produces exact
  pixel-mirrored left-facing cells. Independently generated left poses create
  inconsistent anatomy and anchors.
- For ImageGen, request an exact number of isolated poses in one clean strip or
  an explicitly sized grid. Use a pure flat `#ff00ff` chroma background, hard
  pixel edges, no gradients, shadows, text, labels or grid lines, and no loose
  effects disconnected from the character unless the converter expects them.
- Use existing transparent run, crouch, jump and combat sheets as references.
  A title/concept illustration alone is not a scale reference for runtime
  animation.
- Save both the original chroma output and the cleaned transparent PNG. Record
  the prompt in `sparkpaw/docs/IMAGEGEN_PROMPTS.md`.
- The established chroma-removal helper is:

```sh
$PYTHON "$REMOVE_CHROMA_KEY" \
  --input INPUT.png --out OUTPUT-transparent.png \
  --auto-key border --soft-matte \
  --transparent-threshold 12 --opaque-threshold 220 --despill
```

Inspect the transparent result. The generator's largest-connected-component
cleanup removes common detached AI residue, but it cannot recognise a wrong
hand, clipped head, inconsistent scale or a valid-looking unwanted fragment.

### Runtime cell, scale and anchor rules

- Every current pose occupies a fixed 48x48 runtime cell. The cell is a canvas,
  not a box that every pose should fill.
- Crop transparent padding first, but use one scale for the complete animation
  family via `family_scale()`. Never thumbnail or auto-fit each frame
  independently. A crouched or compressed pose must naturally occupy less
  height than a standing pose.
- Place poses centred horizontally and bottom-aligned to the same foot/ground
  baseline. Feet, not the centre of the bounding box, define vertical origin.
- Compare head size, torso width and gauntlet size against adjacent established
  families after quantisation. The bounding box may change because limbs move;
  the character itself must not appear to zoom.
- Tiny documented per-pose compensation is acceptable only to correct a
  demonstrated quantisation/perceived-mass issue. The current jump family uses
  small authored scale factors for this reason. Do not use compensation as a
  substitute for a coherent source sheet.
- Extended weapons may approach or clip one edge by a pixel. Prefer preserving
  body scale over shrinking the entire actor merely to fit one barrel tip.
- Transparency is pen 0. Opaque pixels are remapped to the fixed Sparkpaw
  palette; arbitrary palettes embedded in newly generated sheets are not used
  directly by the Amiga.
- Direction changes are mirrors of identical indexed cells. Keep the logical
  facing/gauntlet meaning in code and art consistent: the energy gauntlet must
  read as the same physical arm when facing either way.
- Collision dimensions are gameplay data. A more extended drawing must not
  silently enlarge the player hitbox; conversely crouch-specific collision must
  stay tied to the proven crouch state, not to transparent PNG bounds.

### Adding frames safely

1. Start from a clean Git state and identify the accepted tag/commit. Make a
  dated local backup too when changing source sheets, generator layout or
  frame IDs because generated and intermediate assets may be ignored.
2. Add the chroma and transparent source sheets under `assets/sprites/`.
3. Append new logical frames after the current 46 whenever possible. Do not
   renumber proven run/jump/turn/idle/fire IDs merely to make a sheet prettier.
4. Update all sources of truth together: generator `frame_count` and row count,
   frame extraction/placement, `assets/sprites/animations.json`, generated
   manifest/preview, C constants/animation selection, Makefile dependencies,
   README frame count and release source list when applicable.
5. Ensure the deterministic mirrored half is generated too; no runtime PNG
   flip or per-pixel mirroring should be added.
6. Run `make`, inspect `sparkpaw-48x48-aga16-source.png` and the indexed preview
   at integer scale, then run `make release`.
7. Test transitions, not only individual poses: enter, hold, reverse, jump,
   land, collide, shoot and return to the preceding state.

## Music and sound workflow

The games use original, reproducibly generated assets. Classic commercial
tracks may inspire broad energy or structure, but do not copy their melodies,
patterns or samples.

### ProTracker music and Light Speed Player

- Editable/source music is a four-channel ProTracker-compatible `.mod`, often
  generated by a project-owned Python script from original signed 8-bit sample
  waveforms and tracker patterns.
- VLC is useful for a quick host preview but is not authoritative. Modules have
  previously sounded correct in VLC and wrong in ProTracker, HippoPlayer or the
  game. Verify on an Amiga-compatible player and through the actual game path.
- Keep samples signed 8-bit mono, lengths even, periods legal, loop start/length
  valid, finetune intentional, and effects within the subset understood by
  real ProTracker and the converter. Avoid clever ambiguous arpeggio/tempo
  tricks merely because a modern decoder accepts them.
- ChipSnake and Futsal use Light Speed Player 1.31 from their own
  `third_party/LSPlayer/` copies. The host converter creates a normal-memory
  `.lsmusic` score and a Chip-RAM `.lsbank` Paula sample bank:

```sh
tools/lsplayer/convert music/track.mod -shrink -fixed50hz
```

  Use the actual project Makefile command/path; the line above describes the
  required flags rather than promising a universal executable location.
- The assembly bridge and VBlank interrupt advance playback at an explicit PAL
  50 Hz. Music must never depend on how often the title/game main loop redraws.
  `-fixed50hz` and explicit display-rate setup are what gave HD, ADF and real
  Amiga playback consistent tempo and pitch.
- Keep source MODs intact. `.lsmusic` and `.lsbank` are generated runtime files,
  not the composition source.
- ChipSnake reserves all four channels for music. Futsal title music may use
  all four; Futsal in-game music deliberately uses only Paula 0-1 so effects
  can own 2-3. Decide channel allocation before composing a new in-game track.

### Paula sound effects

- Current generated effects are signed 8-bit mono at 11025 Hz and padded to an
  even length. Host `.wav` previews under `assets/sfx/previews/` are for review;
  the raw signed bytes or packed bank are what Paula plays.
- Samples played by Paula, and their banks, must be in Chip RAM. Store period,
  volume, priority and cooldown explicitly; do not derive them from a modern
  WAV player at runtime.
- Futsal packs effects in big-endian `MDFS` records and plays them on reserved
  channels 2-3. Sparkpaw's full generated bank is `SPFX`, but the current
  milestone directly loads `sfx/raw/energy-shot.raw` and plays it on Paula
  channel 0 at period 322, volume 60, with a short bounded DMA lifetime.
- Starting/stopping DMA must respect Paula's latch timing. A stale tiny loop or
  incompletely reset channel caused the high-pitched whine previously heard
  after Futsal goals. Never replace this with a CPU-speed busy-delay.
- Use effect priorities and cooldowns to avoid rapid low-value sounds stealing
  critical ones. Stop and clear owned audio DMA on state changes and shutdown.
- Sparkpaw has no music yet. Before adding it, reserve a channel layout that
  leaves room for rapid plasma, jump/land, impacts and later enemy sounds.
  Do not simply start a four-channel MOD and then interrupt arbitrary channels.

Sparkpaw's reproducible SFX definitions are in
`sparkpaw/tools/generate_sparkpaw_sfx.py`; current planned originals include
jump, land, energy shot, charged shot, enemy hit, player hurt, collect,
checkpoint, Stormstone and menu sounds. Add or revise the generator and host
preview first, then integrate one effect at a time.

## Release expectations

For a normal release change, verify:

1. `make` succeeds.
2. The root executable is an AmigaOS loadseg-able binary.
3. `make release` succeeds.
4. ZIP, LHA, bootable ADF, source ZIP and WHDLoad archives are rebuilt where
   that game supports them.
5. The ADF has `S/startup-sequence` and launches the same executable/assets as
   the hard-drive package.
6. Runtime directory layouts and filename case are preserved.
7. Amiga-facing documentation is plain `ReadMe.txt` with a Workbench icon when
   appropriate; Markdown is for source/development documentation.

Never claim FS-UAE or real-hardware behaviour was verified locally when only a
cross-build was performed. MrDig performs the final visual and hardware test.

## Project 1: ChipSnake

Directory: `chipsnake/`

Main source:

- `src/snake.c`: game, screens, IFF loading, collision, high scores and input.
- `src/modplayer.c`, `src/lspplayer.s`: music integration.
- `tools/`: IFF conversion, music generation/validation, icons, releases and
  WHDLoad packaging.

Current product:

- One-screen Snake with edge wrapping and keyboard plus joystick-port-2 input.
- Cursor keys and W/A/S/D are supported.
- Food relocates after seven seconds.
- Random IFF/ILBM backgrounds with palette-index collision semantics.
- Hall of Fame saved to `PROGDIR:chipsnake.highscores` with name entry,
  checksum and built-in defaults led by MrDig.
- Illustrated animated title screen and marquee.
- Original ProTracker music, converted to Light Speed Player data.
- ZIP, LHA, bootable ADF, source and WHDLoad releases.

Important design rule for custom levels:

- pens 0-4 are safe background;
- pens 5-11 are obstacles;
- pens 12-15 are reserved for snake/food/head/text.

Lessons learned:

- Derive collision from palette indices rather than RGB values or visual
  similarity. That fixed apparently random deaths on detailed backgrounds.
- Keep UI animation and music timing independent. Music is advanced by a
  vertical-blank interrupt server, not the variable main loop.
- Convert MODs with LSPlayer `-fixed50hz`; HD and ADF must produce the same
  pitch and tempo.
- Reserve Paula channels through `audio.device` and cleanly stop DMA.
- Avoid CPU-speed delay loops for Paula DMA restarts.
- Text drawn directly into a visible buffer flickers; compose off-screen or
  update only the damaged region.

Read `chipsnake/README.md` for the exact current controls, music files,
background format and build details.

## Project 2: Futsal

Directory: `mrdigs-futsal/`

The game is branded simply **Futsal** on its current public page, although
historical filenames and directories still use `MrDigs-Futsal`.

Main source:

- `src/futsal.c`: rendering, input, physics, AI, options and game state.
- `src/modplayer.c`, `src/lspplayer.s`: fixed-50-Hz music.
- `src/sfxplayer.c`: effects on reserved Paula channels.
- `tools/`: title/options IFF conversion, MOD/SFX generation, icons, release
  and WHDLoad packaging.

Current product:

- One-screen indoor arcade football with wall rebounds.
- Configurable 3v3 or 4v4; 4v4 adds autonomous goalkeepers.
- One player versus CPU or local two-player via both joystick ports.
- Match Setup screen configures players, team size, court, CPU level, music,
  goal limit, time limit and starting game speed.
- Wood, Green, Checkered and Boing Ball courts.
- Passing, temporary close dribble control and charge/release shooting.
- Easy/Medium/Hard CPU; only the red CPU team uses this difficulty in a
  one-player match. Equal support AI remains active for unselected teammates.
- Automatic defensive player switching; player 1 retains Tab as a hidden
  manual fallback. Player 2 manual switching is intentionally disabled.
- Gameplay music is muted by default. `M` toggles it; effects remain active.
- Goalkeepers catch/parry, briefly reposition and distribute with variation.
- ZIP, LHA, bootable ADF, source and WHDLoad releases.

Important input rules:

- Player 1 is joystick port 2 / keyboard blue team.
- Player 2 is joystick port 1 / red team only during a two-player match.
- Only player 1 controls title and Match Setup.
- Tap fire passes; hold and release fires a powered shot.
- Do not overload fire with manual player switching; that previously broke
  shooting feel.

Lessons learned:

- ADF boot and Workbench launch expose timing differences. Use explicit PAL
  50-Hz timing, VBlank-driven LSPlayer and enable the 68020 instruction cache.
- Compose full frames off-screen and page-flip at VBlank to prevent player
  sprite flicker, especially from a minimal floppy boot.
- During gameplay, music uses Paula channels 0-1; effects use 2-3. This permits
  louder effects without interrupting music.
- Stop/reset effect DMA carefully. A stale audio channel produced high-pitched
  tones after goals.
- Cache court backgrounds. Court selection must not add recurring per-frame
  drawing cost.
- Goalkeeper tuning needs deliberate uncertainty. Perfect interception creates
  repeatable no-goal situations; difficulty-linked reaction and error margins
  are more enjoyable.
- Virtually widening the goal near posts can reward visually convincing shots
  without changing the visible goal or goalkeeper collision size.
- UI labels and arrows must be validated on the real 320x256 PAL display, not
  only in a large concept image.

Regression backups:

- `mrdigs-futsal/backups/pre-options-2player-20260731/`
- `mrdigs-futsal/backups/pre-4v4-goalkeeper-20260731/`

Read `mrdigs-futsal/README.md` before modifying controls, AI or releases.

## Project 3: Sparkpaw — The Stormstone Quest

Directory: `sparkpaw/`

This is the active project. It is a native AGA side-scrolling action-platformer
prototype inspired by the feel and polish of classic Amiga games, without
copying their characters, levels, art or music.

### Current technical foundation

- PAL 320x256 at a fixed 50 Hz.
- Five-screen-wide 1280x256 world.
- Two independent three-plane AGA playfields.
- Foreground scrolls at camera speed; rear layer scrolls at quarter speed.
- Player is a 48x48, 15-colour object made from three attached hardware-sprite
  pairs: six of the eight sprite DMA channels are consumed by Sparkpaw.
- C/VBCC, direct custom-chip control and generated planar assets.
- Tile collision, running, crouching, crouch-walking, jumping, falling,
  landing, idle posing and momentum-based direction changes.
- Joystick port 2: left/right run, up jumps, fire shoots.
- Up to six blue/cyan plasma pulses with animated round impacts.
- Four low two-hit clockwork beetles share one art cache and patrol independent
  sections of the test level for combat and performance testing.

Main files:

- `src/sparkpaw.c`: current renderer, copper, sprites, physics, animation,
  input, plasma Bobs and shot audio.
- `src/aga_renderbench.c`: isolated dual-playfield renderer experiment.
- `tools/generate_runtime_assets.py`: palettes, playfields, collision and
  48x48 sprite conversion.
- `tools/make_release.py`: ZIP/LHA/ADF/source packaging.
- `assets/sprites/animations.json`: authoritative animation frame names.
- `docs/IMAGEGEN_PROMPTS.md`: reproducible art prompts.
- `docs/PREPRODUCTION.md`: original game direction.

### Current animation state

There are 50 authored runtime poses:

- 2 base idle/blink;
- 8 run frames;
- 4 jump/fall frames;
- 3 landing frames;
- 3 crouch frames;
- 6 momentum-turn frames;
- 12 slow idle/front-pose frames;
- 4 grounded shooting frames;
- 4 airborne shooting frames;
- 4 dedicated crouch-shooting frames.

All frames use a 48x48 cell, fixed family scaling and stable anchors. MrDig has
iteratively tuned apparent character scale, foot baseline, jumping, landing,
crouching, idle transition and Flashback/Prince-of-Persia-like reversal
momentum. Protect those improvements: never resize each pose independently to
fill its cell, because that recreates the visible zooming problem.

Current airborne shooting assets:

- `assets/sprites/sparkpaw-air-fire-v8-chroma.png`
- `assets/sprites/sparkpaw-air-fire-v8-transparent.png`

They were generated from the established jump and combat references, then
chroma-keyed locally and palette-reduced by the runtime asset generator.

### Accepted crouch-shooting baseline

A separate task has implemented shooting while crouched or crouch-walking. It
was correctly treated as an animation/state addition rather than merely
removing the input guard:

- backup: `backups/Sparkpaw-pre-crouch-shooting-20260804-235436.zip`;
- source art: `assets/sprites/sparkpaw-crouch-fire-v9-chroma.png` and
  `sparkpaw-crouch-fire-v9-transparent.png`;
- frames 46-49: crouched raise, aim, fire/recoil and recover;
- all four use the proven crouch-family scale and baseline;
- `startShot()` now permits crouched fire while retaining turn/cooldown checks;
- `animatePlayer()` selects 46-49 before ordinary crouch/crouch-walk frames;
- crouched shots use the same mirrored horizontal muzzle logic but a dedicated
  lower vertical projectile origin, `py + 29`, measured from frame 48;
- standing/air fire remains at `py + 15`, and grounded/air frame IDs 38-45 are
  unchanged;
- crouch collision/movement rules, rapid tap cooldown, six-projectile limit,
  packed planar renderer and Paula energy-shot playback are unchanged;
- executable and release packages were rebuilt on 4 August 2026.

MrDig confirmed on 5 August 2026 that crouch shooting works completely in
FS-UAE. Treat its poses, scale, baseline, muzzle origin and recovery as an
accepted baseline.

### Current beetle milestone

Milestone 2A originally replaced the static target with one native 32x24 low
clockwork beetle. The accepted current build has four instances sharing the
same nine-frame art cache: four walk frames, one hit frame and four destruction
frames. Each instance has independent patrol, HP, hit and death state and uses
camera-plus-32-pixel culling. Deterministic mirrored facing, the fixed ground
baseline and the low crouch-shot hit zone are established contracts. The first
hit reacts and the second destroys the instance. Player damage, random
spawning and respawn remain out of scope.

The initial one-enemy checkpoint is
`backups/Sparkpaw-pre-milestone2a-20260805-110912.zip`. It is historical, not
the current gameplay baseline.

After the first visual review, the beetle received a focused art refinement.
Its authored column faces left toward Sparkpaw, and the runtime now selects
that column for leftward movement instead of making the beetle appear to walk
backwards. MrDig confirmed in FS-UAE that the corrected direction is good.

A subsequent native-resolution art pass preserves the exact 32x24 cells,
ground baseline, frame IDs and low projectile hit zone, but follows the concept
more closely with a round lens head, twin antennae, domed segmented shell and
more readable jointed legs. Steel blue and violet now separate the enemy from
Sparkpaw's amber palette; cyan is limited to the lens and energy damage.

The same pass replaces the enemy/projectile CPU byte compositing hot path with
direct synchronized Blitter DMA. Compact zero-padded mask and plane rows use
11,808 bytes (about 11.5 KiB) of Chip RAM. Background restore uses minterm $F0 and drawing
uses the standard $CA cookie-cut with equal A/B shifts. Each register setup
waits on BBUSY using the Hardware Reference Manual's preliminary hardware read,
BLTSIZE is written last, and BLITHOG remains disabled. Display, Copper, sprite
and audio DMA therefore retain priority. Backup before this combined art and
performance pass: `backups/Sparkpaw-pre-beetle-art-performance-v4-20260805.zip`.
MrDig's first FS-UAE test found the frame rate more stable, but horizontal
scrolling sometimes showed a torn/corrupt foreground frame. Local inspection
of `sparkpaw/testresults/2026-08-05 18-59-45.mov` isolated a clear example near
36.45 seconds: one foreground frame used mixed scroll/bitplane state while the
adjacent frames were clean. The problem occurred less while firing because the
additional Blitter work moved the subsequent Copper-list writes past frame
start, unintentionally avoiding the race.

The fix stages `setHardwareSprite()` and `setScroll()` during the line-100
gameplay phase, after the Copper consumed those entries for the current frame
and well before the next frame starts. Bob restore/drawing remains after line
300. Backup before this fix:
`backups/Sparkpaw-pre-copper-list-race-fix-20260805.zip`; the MOV was explicitly
excluded from that backup and is not release content. MrDig subsequently
confirmed in FS-UAE that horizontal scrolling no longer tears, both during
normal running and while running and firing. Real-A1200 verification remains
open.

The latest focused iteration polishes the same 32x24 beetle without changing
its baseline, hit zone, frame IDs or three-plane contract. Native pixel clusters
now provide consistent dome highlights, panel depth, under-armour, rivets,
lens glass, brighter mechanical joints and richer destruction debris. Four
fixed enemy instances reuse the same packed art cache at separate floor patrols
across the five-screen test level. They have independent HP, hit and destruction
state; only instances within the camera plus a 32-pixel margin are drawn.
Respawning remains deliberately out of scope. Backup before this iteration:
`backups/Sparkpaw-pre-multi-beetle-polish-20260805.zip`.

### Agreed roadmap after the four-beetle milestone

The next development direction is deliberately ordered. Do not skip directly
to more enemy types while all gameplay still lives in `src/sparkpaw.c`; player
damage, respawn, screen states and level data would otherwise deepen the
monolith and make later renderer-sensitive changes harder to isolate.

#### Phase 1: incremental source modularisation

Restructure proven code without changing visible behaviour, timing, collision
or asset formats. This is not a rewrite. Move one ownership boundary at a time,
then build and regress the current four-beetle level before moving the next one.
The intended destination is approximately:

```text
src/
  main.c                         startup and top-level state transitions
  game.c / game.h                game loop and high-level game state
  platform_amiga.c / .h          takeover/restore, Copper, DMA and Blitter
  renderer.c / renderer.h        playfields, scrolling, sprites and Bob API
  player.c / player.h            input, movement, animation and player state
  enemies.c / enemies.h          fixed pool, AI, damage and enemy rendering
  projectiles.c / .h             projectile pool, movement and hit dispatch
  collision.c / collision.h      tile and actor hit tests
  audio.c / audio.h              Paula ownership, music and effects
  assets.c / assets.h            loading, validation and Chip-RAM preparation
  sparkpaw_types.h               deliberately shared compact data contracts
```

Start with the clearest gameplay boundaries: enemies, projectiles, player and
assets. Move the hardware/renderer code later because Copper write timing,
Blitter ordering, dual-playfield pointers and six-channel player sprites are
accepted sensitive paths. Do not create circular headers or expose custom-chip
register writes to gameplay modules. Keep shared state explicit and small.

Progress on 5 August 2026: the first extraction is accepted. `enemies.c` and
`enemies.h` now own the fixed four-enemy pool, spawn/patrol initialization, AI,
projectile hit detection and damage/death state. The synchronized enemy Bob
cache and restore/draw code deliberately remain in `sparkpaw.c` until the later
renderer boundary. A clean `make` and `make release` succeeded, and MrDig
confirmed that the modularized build still works correctly in FS-UAE. This is
the first boundary, not the completed Phase 1 structure.

The second accepted extraction moves the fixed projectile pool, spawn/movement,
impact state and enemy-hit dispatch into `projectiles.c` and `projectiles.h`.
Packed plasma pattern generation and synchronized Bob restore/draw deliberately
remain in `sparkpaw.c` with the renderer-sensitive code. A clean `make` and
`make release` succeeded, and MrDig confirmed in FS-UAE that the modularized
projectile build still works correctly. Collision ownership is the next
intended boundary before player physics is moved.

The third accepted extraction moves collision-map loading and solid point,
horizontal-span and vertical-span queries into `collision.c` and `collision.h`.
Player movement, enemy patrol and projectile impacts now share that API without
changing their query order or rules. A clean `make` and `make release`
succeeded, and MrDig confirmed in FS-UAE that movement, collision and gameplay
still work correctly. Player state, input, physics and animation selection are
the next intended boundary; hardware-sprite preparation and Copper updates must
remain with the renderer/platform side.

The fourth accepted extraction moves player state, joystick input, shooting,
movement/physics and 50-frame animation selection into `player.c` and
`player.h`. Renderer and camera code consume a stable read-only player state;
hardware-sprite preparation, Copper pointer updates and camera ownership remain
in `sparkpaw.c`. A clean `make` and `make release` succeeded, and MrDig
confirmed in FS-UAE that the complete player, projectile, enemy and scrolling
behaviour still works correctly. Asset loading and preparation are the next
intended boundary because they are prerequisites for the later title-first and
level-loading state flow.

The fifth accepted extraction moves SPBM loading, validation, gameplay-asset
lifetime and partial-failure cleanup into `assets.c` and `assets.h`. Renderer
code consumes read-only asset handles; packed hardware-sprite and Bob cache
preparation deliberately remains in `sparkpaw.c`. A clean `make` and
`make release` succeeded, and MrDig confirmed in FS-UAE that startup, visuals
and gameplay still work correctly. Audio sample lifetime and Paula channel
ownership are the next intended boundary.

The sixth accepted extraction moves energy-shot sample loading and lifetime,
Paula channel 0 playback, shot timing and explicit hardware-active lifecycle
control into `audio.c` and `audio.h`. The platform layer activates audio only
after custom-chip takeover and deactivates it before restoring system DMA. A
clean `make` and `make release` succeeded, and MrDig confirmed in FS-UAE that
startup, gameplay and repeated shot playback still work correctly. Gameplay
orchestration and camera state are the next intended boundary; raster waits,
Copper updates and Bob rendering must remain in `sparkpaw.c` during that step.

The seventh accepted extraction moves gameplay initialization, the established
input/update sequence, frame progression and camera state into `game.c` and
`game.h`. `sparkpaw.c` consumes read-only game state for sprite, scroll and Bob
presentation while retaining both raster phases, Copper updates and Blitter
work in their proven order. A clean `make` and `make release` succeeded, and
MrDig confirmed in FS-UAE that movement, jumping, scrolling, shooting and all
four beetles still work correctly. The remaining Phase 1 work is the sensitive
renderer/platform ownership split; perform that mechanically before adding the
Phase 2 title and loading states.

The eighth accepted extraction moves graphics-library lifetime, custom-chip
takeover/restore, saved DMA and interrupt state, raster reads and the required
preliminary-read Blitter wait into `platform_amiga.c` and
`platform_amiga.h`. Copper construction and concrete Blitter commands remain
in `sparkpaw.c`, and the line-100 and line-300 phases retain their established
order. A clean `make` and `make release` succeeded, and MrDig confirmed in
FS-UAE that startup, visuals, scrolling and gameplay still work correctly.
Next isolate the top-level loop behind an explicit renderer API before moving
the renderer implementation itself.

The ninth accepted extraction moves startup, cleanup and the raster-phased
top-level loop into `main.c` and introduces `renderer.h` as the renderer API.
The implementation remains physically in `sparkpaw.c`; it no longer owns
`main()` or gameplay orchestration. Asset, collision and audio loading retain
their established order, as do the line-100 update and line-300 Bob phases. A
clean `make` and `make release` succeeded, and MrDig confirmed in FS-UAE that
startup, visuals, scrolling and gameplay still work correctly. The next step
is the mechanical `sparkpaw.c` to `renderer.c` rename with no code changes.

Acceptance for every extraction step:

1. `make` succeeds and the root executable is rebuilt.
2. `make release` succeeds and packages match the root build.
3. Asset manifests and protected animation frame IDs remain unchanged unless
   the step explicitly owns an asset change.
4. MrDig rechecks running, jumping, scrolling, rapid fire, crouch fire and all
   four beetles in FS-UAE; do not claim emulator or hardware verification from
   a successful cross-build.
5. Commit each accepted extraction step. Preserve a dated local backup before
  moving renderer/platform ownership or when ignored/generated state makes
  Git alone an incomplete safety net.

#### Phase 2: boot, loading and title states

Use the newly separated modules to introduce an explicit state flow such as:

```text
BOOT -> TITLE_LOADING -> TITLE_READY -> LEVEL_LOADING -> PLAYING
```

The first visual feature after modularisation should be a title-first startup,
not a separate generic loading screen followed by the title. As soon as the
executable can draw, show a lightweight but recognizable AGA title screen based
on the established cover concept art. While initial title/menu resources are
being prepared, that same screen displays `Loading...`. When preparation is
complete, replace the loading status in place with an interactive `Start Game`
option instead of switching to a different start screen.

Activating `Start Game` enters a distinct `LEVEL_LOADING` state. Keep the title
presentation visible or transition to a closely related loading composition
that clearly depicts a floppy/level disk being accessed. Load and prepare the
large playfield, player, enemy and gameplay-audio assets there, then enter
`PLAYING` only when the complete level runtime state is ready. This makes the
two waits understandable: startup prepares the title, while Start Game loads
the selected level and gameplay resources.

Measure the current startup phases and asset costs before choosing what is
embedded or early-resident. Sparkpaw cannot draw before DOS has loaded enough
of the executable to enter its code, so the early title can hide runtime asset
preparation but cannot hide the complete executable load. Keep its first-draw
asset set small and cheap enough that it appears quickly and does not make
startup meaningfully longer. Loading and state transitions must remain correct
from Workbench, HD packages and the bootable ADF.

Screen-state work must not disturb the proven gameplay Copper-list update
window. Define ownership and transitions first; title art, title interaction,
loading progress and later game-over presentation can then evolve independently
from the gameplay loop.

#### Phase 3: player contact damage and hurt presentation

Add explicit actor collision between Sparkpaw and active beetles. Contact is an
enemy hit against the player, not continuous damage every overlapping frame.
The planned response is:

- decrement a life/energy meter once per accepted contact;
- apply short knockback away from the enemy;
- enter a bounded hurt state with temporary input restriction;
- grant roughly 50-75 frames of invulnerability and clear feedback during it;
- add dedicated, consistently scaled 48x48 hurt poses using the established
  authored-right/mirrored-left asset pipeline;
- add HUD/life presentation and the existing planned `player hurt` sound;
- later transition to `GAME_OVER` when energy/lives are exhausted.

Keep player attack hitboxes, player body collision and enemy damage hitboxes as
separate data. Do not derive contact damage directly from transparent art
bounds. Tune the hurt box, knockback and invulnerability from FS-UAE evidence,
then verify on the target A1200 configuration when available.

#### Phase 4: beetle respawn through spawn data

Implement respawn as a generic enemy-spawn system rather than a beetle-only
timer. Each level spawn record should identify enemy type, world position,
patrol/configuration data and respawn policy. On death, deactivate the enemy;
allow reactivation only after a cooldown and when the spawn point is safely
outside the camera/player vicinity. The player must not see an enemy pop into
existence or repeatedly farm one while standing on its spawn point.

Retain a bounded fixed runtime pool and shared per-type art caches. Do not add
dynamic allocation or asset loading to the 50-Hz gameplay loop. Make permanent
versus respawning encounters a per-spawn choice so future level design is not
locked to one global rule.

#### Phase 5: first larger enemy

Create concept art before implementation. Establish silhouette, gameplay role,
pixel dimensions, palette budget, ground anchor, collision boxes, HP, attack
telegraphs and approximate on-screen concurrency before generating animation.
Then make a native-resolution indexed preview and deterministic packed runtime
asset using the existing concept-to-Amiga workflow.

The larger enemy should enter through the generic enemy type/spawn interfaces,
not by duplicating beetle-specific loops. Its larger Bob cost must be budgeted
against dual playfields, player sprites, projectiles and other visible enemies;
camera culling, shared frames and synchronized Blitter restore/draw remain
mandatory. Introduce jumping or another major movement behaviour only after
the basic actor, damage and rendering path is stable.

#### Phase 6: levels and progression

Once two enemy types, player damage and respawn are stable, move patrol/spawn
placements out of `prepare()` into generated level/entity data. Add progression,
checkpoints, collectables and broader screen flow on that foundation. Music
integration should first define Paula channel ownership so rapid plasma,
player/enemy impacts and menu sounds are not arbitrarily interrupted.

Roadmap invariants across all phases:

- preserve the stable dual-playfield renderer, line-100 Copper staging,
  50-frame player baseline and packed Blitter Bob paths;
- maintain PAL 50-fps performance as a design constraint on a stock 68020 with
  2 MB Chip RAM, with Fast RAM recommended;
- keep new runtime assets deterministic and project-owned;
- implement and test one phase in small reviewable steps, without combining a
  renderer rewrite, new gameplay system and new art family in one iteration;
- always run `make` and `make release` after implementation work;
- only MrDig's report may establish FS-UAE or real-hardware behaviour.

### Current projectile renderer

The plasma pulses are software Bobs because only two hardware sprite channels
remain after allocating six to the player.

An early implementation calculated, restored and redrew every projectile pixel
on all three foreground planes every frame. That caused severe slowdowns while
jumping and firing. The current code precomputes five packed planar patterns—
two pulse shimmer phases and three impact phases—and uses the Blitter for
background restore and masked cookie-cut drawing. Do not restore the old
per-pixel `setFrontPixel` or CPU byte-compositing hot paths.

### Known Sparkpaw limitations/backlog

- Clean exit back to Workbench is not yet implemented. Earlier direct Copper
  experiments left the Workbench display and mouse sprite corrupted for seconds
  or indefinitely. Do not re-enable left-mouse exit without treating system
  restoration as a separate tested milestone or moving it to a WHDLoad path.
- For now the user resets the Amiga/emulator to leave the prototype.
- In-game rendering became stable after the isolated renderbench work, but
  Workbench restoration still needs a separate, careful implementation or a
  later WHDLoad path.
- Plasma and beetle Bobs now share the accepted synchronized Blitter approach:
  packed planar caches, $F0 restore, $CA cookie-cut draw, BBUSY waits, BLTSIZE
  written last and no BLITHOG. Keep stress-testing running, jumping, rapid fire
  and four visible enemies together on the A1200 configuration; do not restore
  CPU read-modify-write compositing against displayed Chip RAM.
- Air-fire frames 42-45 are part of the established 50-frame scale/anchor
  baseline. Preserve their body scale and transition timing; real-hardware
  rise/apex/fall verification remains open.
- Crouch-fire frames 46-49 are accepted. Preserve their established scale,
  baseline, hitbox and muzzle origin.
- The four-instance polished beetle pool still needs a focused final stress
  test with rapid fire and camera traversal. Preserve the proven player,
  dual-playfield and synchronized projectile/enemy Blitter paths while fixing
  any observed issue.
- There are no additional enemy types, player damage loop, HUD, menus, music,
  collectables or full level progression yet. This remains an engine milestone.
- Keyboard controls are deferred until after the modularisation milestone;
  current gameplay input is joystick port 2 only.
- Fifty-fps smoothness is a hard design goal. Avoid full-frame CPU copying,
  per-pixel inner loops and redrawing static scenery.

### Sparkpaw backups and baselines

- `backups/Sparkpaw-pre-shooting-20260804-221309.zip`: complete state before
  shooting was introduced.
- `backups/Sparkpaw-pre-crouch-shooting-20260804-235436.zip`: complete state
  immediately before dedicated crouch shooting was introduced.
- `backups/Sparkpaw-pre-milestone2a-20260805-110912.zip`: accepted
  crouch-shooting state immediately before the first moving enemy.
- `backups/Sparkpaw-pre-beetle-art-v2-20260805-151521.zip`: intermediate
  one-beetle art refinement checkpoint.
- `backups/Sparkpaw-pre-beetle-art-v3-20260805.zip`: Milestone 2A state before
  correcting the apparent backwards walk and refining the beetle artwork.
- `backups/Sparkpaw-pre-beetle-art-performance-v4-20260805.zip`: user-tested
  corrected-facing build before the concept-led art and Blitter performance
  pass.
- `backups/Sparkpaw-pre-copper-list-race-fix-20260805.zip`: Blitter build before
  moving Copper-list writes away from frame wrap; local MOV evidence excluded.
- `backups/Sparkpaw-pre-multi-beetle-polish-20260805.zip`: user-confirmed
  tearing-free build before final beetle pixel polish and the four-instance
  test pool; local MOV evidence excluded.
- `sparkpaw/baselines/pre-copper-20260804/`: pre-Copper executable, assets and
  release packages.

Do not overwrite or delete these.

### Required regression test for the next modularisation step

Before extracting a module, record the current result; after the extraction,
run the rebuilt root executable and repeat the same checks:

1. traverse the complete five-screen level and observe all four beetle patrols;
2. verify each beetle turns at both limits and stays grounded and consistently
  scaled;
3. verify standing and airborne shots do not damage beetles;
4. inspect the first crouch-shot reaction and all four destruction stages after
  the second hit, attacking from both directions and while crouch-walking;
5. overlap several pulses, impacts and enemies and watch for Bob residue;
6. combine rapid fire with running, jumping and camera movement;
7. verify the player animation baseline, crouch-fire muzzle origin, scrolling,
  parallax, audio and collision are unchanged;
8. watch specifically for a one-frame mixed-scroll tear during both quiet and
  heavy Blitter workloads.

If performance or residue is poor, change only enemy/projectile Bob ordering or
drawing. Do not rewrite the proven dual-playfield or player hardware-sprite path
at the same time.

Read `sparkpaw/README.md` before implementing the next milestone.

## Cross-project engineering lessons

1. **Build for PAL timing explicitly.** A loop that happens to sound or animate
   correctly from Workbench may run differently from ADF or on real hardware.
2. **Use the Amiga custom chips.** Hardware scrolling, sprites, Copper, Blitter
   and Paula exist to avoid making the 68020 move every pixel or mix every
   sample itself.
3. **Keep interrupt work bounded.** Music in VBlank is appropriate; expensive
   rendering or file I/O is not.
4. **Precompute and cache.** Convert PNG/MOD/WAV assets on the host, cache static
   courts/backgrounds and prepare planar/masked animation data before gameplay.
5. **Respect Chip RAM.** Display bitplanes, hardware sprites, Copper lists and
   Paula samples must live in accessible Chip RAM; put ordinary logic/data in
   Fast RAM where possible.
6. **Synchronise visible updates.** Draw off-screen or inside a safe VBlank
   window, then flip/change pointers. Direct visible-buffer text and sprite
   edits caused flicker in earlier versions.
7. **Test HD and ADF.** Minimal floppy boot lacks the same OS initialization and
   patches as Workbench. Treat parity as a release requirement.
8. **Use stable visual anchors.** Every animation family needs consistent body
   scale, feet/ground baseline and origin. Per-frame auto-fit looks like zooming.
9. **Separate visuals from collision deliberately.** Palette-index rules or an
   explicit collision map are reproducible; sampling arbitrary artwork is not.
10. **Change one risky subsystem at a time.** MrDig tests frequently with MOVs
    and screenshots. Preserve a working baseline before renderer/audio changes.
11. **Rebuild everything requested.** Root executable, runtime assets and release
    archives must describe the same revision.
12. **Keep public claims accurate.** These are AI-assisted original prototypes;
    inspiration from classic games is about feel and engineering, not copied
    artwork, music, characters or levels.
13. **Do not let workload hide a synchronization race.** Sparkpaw's Copper-list
    race became less visible while firing because extra Blitter work shifted
    unsafe writes past frame start. Visible improvement under load can be a
    timing clue, not proof that the renderer is correct. Keep scroll/sprite
    Copper writes in the accepted line-100 phase and Bob work after line 300.
14. **Attribute crashes from evidence, not proximity.** A one-off Workbench
    Software Failure named task `SpeedLoad`, reported as an illegal instruction,
    was not attributable to the Sparkpaw executable and disappeared after a
    reboot. Direct custom-chip takeover means indirect corruption cannot be
    ruled out, but do not change stable game code without recurrence evidence.
    If it returns, record task name, PC/registers, launch path and whether it
    happened during gameplay, startup or reset/Workbench return.
15. **Use Git for source history, not generated releases.** Keep `main` at an
    accepted build, use short feature branches and focused commits, and tag
    playable milestones. `build/`, `dist/`, local SDKs, recordings and backups
    are ignored; publish ZIP/LHA/ADF files through GitHub Releases when needed.

## Recommended first prompt in a new Codex task

```text
We are continuing the Amiga game workspace in this repository.

First read CODEX_HANDOFF.md completely, then read sparkpaw/README.md and inspect
the current Sparkpaw source before changing anything. Sparkpaw is the active
AGA A1200 project. Check git status, recent commits and tags first; use
sparkpaw-pre-modularisation as the accepted four-beetle baseline.

Preserve the stable dual-playfield renderer, line-100 Copper-list staging,
50-frame player animation contract, six-channel hardware-sprite player and the
synchronized Blitter Bob pipelines used by plasma projectiles and the
four-instance clockwork-beetle pool. Preserve packed planar caches, camera
culling, Bob restore/draw ordering and accepted animation/collision contracts.
Do not reintroduce CPU read-modify-write compositing in displayed Chip RAM.

Work in small reviewable steps and do not combine renderer changes with
unrelated gameplay or asset changes. Do not modify or delete ignored local
backups or test evidence. Always run make and make release after implementation.
Do not claim FS-UAE or real-hardware verification unless I provide the result.

My next request is: ...
```

Replace `sparkpaw/README.md` with the relevant game README when continuing
ChipSnake or Futsal instead.
