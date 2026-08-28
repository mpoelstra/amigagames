# Sparkpaw and Amiga prototypes: development history

> Historical archive, preserved from the former long-form `CODEX_HANDOFF.md`.
> Statements such as “current”, “next”, “awaiting review” and the embedded
> recommended prompt describe their point in time and are not authoritative
> today. Read the repository-root `CODEX_HANDOFF.md` for the canonical current
> state and roadmap. Use this file for rationale, rejected experiments,
> regression archaeology and older test evidence.

Maintenance rule: append relevant history whenever a feature or phase is
completed, and always before a requested checkpoint commit/push. Record the
implemented result, important decisions, rejected approaches and the exact
verification source (build only, MrDig-supplied FS-UAE, or real hardware).
Audit and update the compact repository-root `CODEX_HANDOFF.md` at the same
time, but keep chronological detail here rather than allowing that handoff to
become a diary again.

Last updated: 28 August 2026

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
- Four guaranteed and up to two optional low two-hit clockwork-beetle
  encounters share one art cache and a bounded four-slot runtime pool. Authored
  zones randomize safe X positions and slow/normal/fast movement; destroyed
  encounters respawn indefinitely after an off-screen cooldown.

Main files:

- `src/main.c`: startup, application states and raster-phased gameplay loop.
- `src/renderer.c`: Copper, dual playfields, hardware sprites and packed Bobs.
- `src/game.c`, `src/player.c`, `src/enemies.c`, `src/projectiles.c`: gameplay
  orchestration and bounded runtime actors.
- `src/level_data.c`: typed authored enemy encounter candidates and policies.
- `src/aga_renderbench.c`: isolated dual-playfield renderer experiment.
- `tools/generate_runtime_assets.py`: palettes, playfields, collision and
  48x48 sprite conversion.
- `tools/make_release.py`: ZIP/LHA/ADF/source packaging.
- `assets/sprites/animations.json`: authoritative animation frame names.
- `docs/IMAGEGEN_PROMPTS.md`: reproducible art prompts.
- `docs/PREPRODUCTION.md`: original game direction.

### Current animation state

There are 58 authored runtime poses. The accepted original 50 remain frames
0-49, followed by standing hurt poses at 50-53 and crouched hurt poses at
54-57:

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
- 4 appended hurt frames: impact, backward slide, airborne recoil and recovery.
- 4 appended crawl-hurt frames capped at 24 opaque pixels so their visible
  silhouette respects the low crouch collision clearance.

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
Individual respawning remains deliberately out of scope. The automatic
whole-level reset after all four destruction sequences is a test replay
convenience, not final respawn or level-completion behaviour. Backup before this iteration:
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

The tenth accepted extraction completes Phase 1 by renaming the renderer
implementation from `sparkpaw.c` to `renderer.c`. The file was verified
byte-for-byte identical before rebuilding; only the Makefile and source map
changed. A clean `make` and `make release` succeeded, the source archive uses
the new filename, and MrDig confirmed in FS-UAE that startup, visuals,
scrolling and gameplay still work correctly. Phase 2 can now begin with an
explicit top-level state model before title assets or loading presentation are
introduced.

The first accepted Phase 2 step adds the complete top-level state vocabulary
to `main.c`: `BOOT`, `TITLE_LOADING`, `TITLE_READY`, `LEVEL_LOADING` and
`PLAYING`. Until early title rendering and title input exist, the compatibility
path deliberately remains `BOOT -> LEVEL_LOADING -> PLAYING`; title states are
not entered as placeholders. Gameplay asset loading and both raster phases are
unchanged. A clean `make` and `make release` succeeded, and MrDig confirmed in
FS-UAE that startup and gameplay still work correctly. Next separate a small
early-resident title asset from the large level/gameplay asset set.

The second accepted Phase 2 step generates the established 320x256 AGA title
preview as a compact 64-colour, six-plane `sparkpaw-title.spbm` and adds a
separate title asset lifetime in `assets.c` and `assets.h`. A full planar
decode reproduced all 81,920 source palette indices exactly. The runtime asset
is 61,644 bytes and is packaged in HD, archive and ADF releases, but the current
startup path does not load or display it yet. A clean `make` and `make release`
succeeded, and MrDig confirmed in FS-UAE that existing startup and gameplay
remain unchanged. Next display this asset through an OS-managed early title
view before level asset loading and custom-chip takeover.

The third accepted Phase 2 step displays the clean 64-colour title through an
OS-managed View while DOS and graphics.library remain active. The first text
overlay experiment was removed because it did not match the authored pixel-art
language; the runtime title now remains pixel-identical to its generated asset.
The original Workbench View is captured before title presentation so gameplay
takeover still restores the correct system display. A clean `make` and
`make release` succeeded, and MrDig confirmed in FS-UAE that the clean title,
transition and gameplay work correctly. The agreed next flow is: show the title
for 150 PAL frames (three seconds), switch to a separate original Sparkpaw
level-disk loading image, load the level, then enter `PLAYING`.

The fourth Phase 2 step remains unaccepted. `TITLE_READY` gives the title an
explicit 150-frame hold, then `LEVEL_LOADING` switches to an original
Stormstone level-disk image while the existing gameplay assets load. The
loading image has its own asset lifetime and its selected source was generated
with the built-in Codex ImageGen workflow from the preserved art brief.
Initial FS-UAE testing exposed a retained mouse sprite, flashes between raw
Views and flat palette-colour screens from a minimal ADF boot. Intuition also
reported `OSERR_TOODEEP` when asked for a six-plane screen despite AGA hardware.
The current committed checkpoint is therefore a diagnostic compatibility
version: deterministic four-plane title/loading assets are presented through
prepared OS-managed screens with hidden pointers, and the loading-to-gameplay
handoff blanks before starting the proven gameplay Copper. It displays both
images from HD and ADF, but 16 colours are not the accepted visual target.
The next focused step must restore the preserved 64-colour sources and present
them through a small direct six-plane AGA Copper path while DOS remains active.
Use complete off-screen Copper lists and VBlank swaps for title, loading and
gameplay transitions; preserve the gameplay renderer and its line-100/line-300
ordering. Do not treat the four-plane checkpoint as the final Phase 2 result.

That focused step is now implemented after checkpoint commit `d39dd5f`. The
title runtime screen is generated losslessly from its preserved indexed 320x256
AGA preview. The loading screen is regenerated deterministically from its
preserved RGB source using the fixed 75-percent crop, Lanczos fit and a
64-colour Fast Octree conversion without dithering; this preserves the cyan and
amber focal details without turning dark source noise into visible speckles.
Decoding either six-plane SPBM reproduces its indexed preview palette and all
81,920 pixels byte-for-byte. `title.c` owns two small Chip RAM Copper lists,
programs exact 320-pixel fetches, enables AGA border blanking and writes both
high and low palette nibbles. Interrupts and DOS remain active. The
loading bitmap and its complete second Copper list are prepared while the first
list continues to scan the title; only the remaining time to a measured minimum
of 150 title frames is idle. Title-to-loading and loading-to-gameplay changes
occur at VBlank, with the old bitmap released only after the new Copper is
active. This direct AGA path has compiled and passed host-side asset checks.
MrDig confirmed in FS-UAE that both direct AGA screens display and transition,
but did not accept their visual quality as materially better; real-hardware
verification also remains pending. Phase 2 is functional but not visually
accepted.

#### Title/loading display lessons from the HD and ADF investigation

1. The earlier HD result was not a 24-bit truecolour framebuffer. It was a
  320x256 indexed image with 64 simultaneously displayed colours; AGA selects
  every one of those palette entries from a 24-bit RGB colour space. The large
  1402x1122 and 1448x1086 PNG concepts are ordinary host-side 24-bit RGB
  sources, and image viewers make them appear smoother through interpolation.
2. Boot medium does not set AGA colour depth or resolution. HD and floppy can
  program identical custom-chip modes. The minimal ADF boot instead exposed
  an OS display-database difference: Intuition returned `OSERR_TOODEEP` for a
  six-plane custom screen even though the A1200 hardware can scan it. Direct
  Copper ownership bypasses that OS-mode dependency and works from the ADF.
3. Floppy constrains capacity and loading latency, not display capability. The
  current bootable ADF uses about 579 KiB of 880 KiB and each 320x256x6 SPBM is
  61,644 bytes, so storage is not what limits these screens to 64 colours.
  More planes increase disk reads, Chip RAM and display DMA, however.
4. The current mode is deliberately PAL low-resolution 320x256 with six
  bitplanes: 64 indexed colours, each with a full 24-bit AGA palette value.
  AGA can support higher-resolution and eight-plane modes, but higher width
  and/or more planes consume proportionally more fetch bandwidth and Chip RAM.
  They require a separate measured presenter experiment, not just a larger
  source PNG. Do not describe 320x256 as AGA's maximum resolution.
5. The next step is not yet a 256-colour conversion. First reproduce the best
  earlier HD appearance through the new floppy-compatible direct Copper path.
  Use commit `ef32941` (`Show Sparkpaw early title view`) as the old HD
  reference and current commit `f52c472` as the direct-AGA reference. Compare
  the exact title SPBM bytes, decoded 81,920 palette indices, all 64 RGB palette
  entries and native 320x256 FS-UAE screenshots with identical scaling and
  filter settings. If the source bitmap and palette are equal but screenshots
  differ, inspect Copper palette-bank writes, display registers and emulator
  scaling before changing the art. If they are not equal, feed the old accepted
  bitmap and palette unchanged into the direct presenter. The goal is to keep
  ADF compatibility without sacrificing the earlier HD appearance.
6. Only after that A/B comparison is understood should 320x256x8 with 256
  indexed colours be tested as a separate quality enhancement. One
  uncompressed screen would use 81,920 bitmap bytes instead of 61,440, and two
  screens would add about 40 KiB before palette/header costs; this appears to
  fit the current ADF but must be benchmarked for startup, Chip RAM, palette
  setup and transition timing before adoption.
7. HAM8 can show many more apparent colours through hold-and-modify encoding,
  but it is not an arbitrary 24-bit framebuffer and can create horizontal
  colour fringing. Treat it as a separate art/encoding mode and probably a
  poor default for crisp title typography and pixel-art edges.
8. A 64-colour conversion can still look worse than its RGB source. Median Cut
  spent many loading-screen entries on dark source noise. Fast Octree without
  dithering reduced measured colour error and background transitions and
  restored cyan/amber focal details, but MrDig still did not find the result
  materially prettier. Preserve the RGB sources and judge conversions at
  nearest-neighbour 320x256 scale in FS-UAE, not only in a smoothed host viewer.
9. The direct presenter needs separate build and active Copper indices. While
  preparing the loading list, reusing the active index would cause the switch
  to reinstall the title list. Keep complete lists off-screen, install only at
  VBlank, and free the old bitmap only after the new list is active.
10. Keep DOS and interrupts available during title/loading. Load and build the
  second screen while the title list continues scanning, measure the minimum
  150-frame hold from the first displayed title VBlank, then switch complete
  states rather than showing an empty View or partially programmed palette.
11. FS-UAE floppy save-images can invalidate ADF debugging. A stale
   `ChipSnake-A1200.sdf` under the FS-UAE Save States directory overlaid the
   newly built disk and made mounted contents disagree with `xdftool`. Delete
   or disable stale `.sdf` overlays before attributing missing or old files to
   the release builder. Both release builders now extract and byte-compare
   critical ADF payloads after writing them.

#### Accepted Phase 2 title/loading presentation (11 August 2026)

The title/loading work is now accepted in FS-UAE and on a real A1200 from both
HD/Workbench and a physical floppy written from the release ADF. The direct
presenter remains 320x256, six-plane, 64-colour AGA and leaves DOS and
interrupts active while startup assets are read. Gameplay takeover, the
line-100 Copper update phase, the line-300 Bob phase and all established
renderer/gameplay contracts remain unchanged.

The final presenter explicitly programs ordinary 64-colour AGA mode with
`ECSENA`, `KILLEHB`, border blanking and complete `DIWHIGH` state. It detaches
the Workbench View with `LoadView(NULL)` and two settling VBlanks before owning
`COP1LC`; this fixed the real-A1200 HD path that otherwise showed only black.
Fades do not exchange full Copper lists every frame. They retain one active
list and stage only its recorded high/low 24-bit palette value words after
raster line 100 for the next frame, eliminating the transition corruption seen
in the HD launch under FS-UAE. Complete bitmap changes still use an inactive
Copper list and a VBlank pointer switch.

The selected title and compact Stormstone-disk loading art are preserved as
source PNGs and converted deterministically to SPBM. Runtime title/loading
palettes reserve pen 0 as pure black; this removed coloured border lines that
the Indivision AGA MK2 exposed outside the CRT's visible overscan. A 35-frame
black PAL lock interval and a 225-frame fully faded title hold give the
Indivision time to switch from Workbench to the game mode. The previously
floppy-only left-edge line also disappeared after this longer stable startup
and the rebuilt physical ADF, supporting a scandoubler lock/sampling cause
rather than corrupt bitmap data.

Startup loading has two truthful phases. `LOADING` remains visible while all
gameplay files are read. Only after the last DOS read/close does the presenter
switch to a palette-identical `CHARGING` screen while
`rendererPrepareGameplay()` builds hardware-sprite DMA, packed Bob data,
bitplanes and the gameplay Copper list. `CHARGING` is held for at least 100 PAL
frames so it remains readable even on accelerated or HD systems. Both status
assets are preloaded, share one palette and switch as complete bitmaps; there
is no CPU read-modify-write compositing into displayed Chip RAM. The old
loading bitmap is freed immediately after the switch to limit peak Chip RAM.

The root executable, HD archives and bootable ADF contain byte-identical
executables and critical runtime assets. MrDig confirmed no transition flicker
in FS-UAE, working HD and physical-floppy startup on the real A1200, and clean
left borders on both the Indivision HDMI output and CRT. Do not reopen this
display path casually; treat future title art, status timing or 256-colour
experiments as separate measured changes.

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

The first visual feature after modularisation is a title-first startup, not a
generic loading screen followed by the title. Show the clean established AGA
title for 150 PAL frames before switching to a separate level-loading image.
Do not place generic runtime text over the authored title artwork.

After the timed title, enter `LEVEL_LOADING` and show a related composition
that clearly depicts an original Sparkpaw floppy/level disk being accessed.
Load and prepare the large playfield, player, enemy and gameplay-audio assets
there, then enter `PLAYING` only when the complete level runtime state is ready.

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

The agreed health model is three hearts represented internally as six
half-heart units. Ordinary beetle contact removes one unit (half a heart);
future stronger attacks may remove one or two units. Water and bottomless pits
remove two units (one full heart) and return Sparkpaw to the last checkpoint.
Do not introduce a second, ambiguous stock of arcade lives yet. Reaching zero
health will later enter an explicit `GAME_OVER` state.

Implement Phase 3 incrementally. First add health state, explicit player/enemy
contact rectangles, one-shot contact acceptance, knockback, bounded input
restriction and about 60 frames of invulnerability. Then append a dedicated
hurt family after the established frame IDs 0-49; never renumber the accepted
50-frame baseline. Add the three-heart full/half/empty HUD and the planned
player-hurt sound as separate reviewable steps rather than mixing gameplay,
animation art, HUD rendering and audio in one change.

Phase 3A is now implemented host-side. `PlayerState` starts with six health
units, and player/enemy modules expose separate authored contact rectangles.
An accepted overlap removes one unit, cancels shooting/turn/crouch state,
launches Sparkpaw away from the enemy, restricts control for 12 frames and
blocks repeat damage for 60 frames. Solid-tile physics, projectile hitboxes and
enemy projectile-damage boxes remain independent. The four-pose hurt family is
now appended as frames 50-53 and selected during the 12-frame input-restricted
recoil; the accepted frames 0-49 remain unchanged. There is deliberately no
HUD, hurt sound or full game-over transition yet; those remain separate Phase
3 reviews. MrDig confirmed in FS-UAE that contact
knockback behaves well in all tested situations. Reaching zero health now
performs an immediate in-memory test reset: assets and packed caches remain
resident, while player, camera, enemies and projectiles return to their initial
state. Enemy/projectile reset preserves the preceding Bob restore rectangles
until the next line-300 pass, preventing stale pixels without clearing or
reloading the wide playfield. `make` and `make release` passed locally. MrDig
confirmed in FS-UAE that the zero-health reset enables rapid repeated enemy
testing. Real-hardware behaviour remains unverified.

The first hurt-animation review exposed one posture invariant under a low
platform: contact code and the hurt-physics branch must not force
`crouching=FALSE`. A crouched player, or any player for whom `canStand()` is
false, retains the crouched collision box throughout recoil and may stand only
after the ordinary clearance check succeeds. This fixes the stuck/embedded
standing state shown in `IMG_2722.MOV` without changing the accepted knockback.
`IMG_2723.MOV` then showed that logical crouch alone was insufficient because
the ordinary hurt art still drew a standing-height silhouette through the
platform. Frames 54-57 therefore form a distinct crouch-hurt family selected
whenever recoil retains crouch posture.

Reviews `IMG_2727.MOV` and `IMG_2728.MOV` led to the final timing/posture pass.
The ordinary hurt source is refined from v10 to v12: its airborne pose follows
a clearer backward trajectory and its last pose is a low hand-braced recovery
instead of near-idle. Both hurt families now spend 2/3/3/4 PAL frames on
impact/slide/recoil/recovery within the unchanged 12-frame input lock. A
crouched contact uses half the ordinary upward impulse and retains a dedicated
`hurtCrouched` posture until landing, preventing a mid-air pop from the low
recovery pose into a standing-height jump/fall pose.

`IMG_2729.MOV` exposed two remaining crouch-contact invariants. First, a seated
hurt silhouette is still too tall even when it shares the ordinary crouch
family's nominal size. The v13 source therefore redraws frames 54-57 as a
horizontal crawl recoil and the generator caps the full family at 24 opaque
pixels. `IMG_2732.MOV` then showed that forced hurt separation could still
produce awkward crossings and could trap the player in narrow spaces, so
beetles remain non-solid during invulnerability. The actual standing-pose leak
came from `canStand()` checking only the gameplay hitbox, whose top begins five
pixels below the visible art. Standing clearance now includes those five
visual pixels, so a normal or hurt standing family is never selected where
Sparkpaw's head cannot fit.

A subsequent still proved one further frame-boundary case: contact is resolved
after physics but before the new animation frame is selected. Near a recovery
boundary the logical crouch flag can change while the displayed frame remains
low, incorrectly selecting standing hurt. Hit posture now latches low when
either the logical state or the currently displayed crouch, crouch-fire or
crawl-hurt family is low. Collision dimensions and the 58 frame IDs remain
unchanged.

`IMG_2734.MOV` isolated the remaining repeated-hit path. Hurt physics returns
before the ordinary grounded input branch, so held down/S was not guaranteed
to reach `player.crouching` before a later accepted hit. Current crouch input
is now latched at the start of every physics update, before that early return,
and participates explicitly in low-hurt selection. Holding crouch therefore
cannot start frames 50-53 across consecutive invulnerability cycles.

`IMG_2736.MOV` exposed the final short high-pose source: landing during low
hurt queued the ordinary landing timer before the hurt animation returned.
Those standing-height frames 14-16 remained hidden until hurt ended and then
took priority over crouch. Crawl-height hurt now suppresses and clears that
landing timer, transitioning directly to low recovery without changing the
accepted ordinary landing sequence.

For easier one-handed filming and test capture, gameplay also accepts keyboard
controls alongside joystick port 2: `W/A/S/D` map to jump/left/crouch/right and
space maps to fire. Because the custom-chip takeover disables CPU interrupts,
this is a bounded CIA-A serial-keyboard poll in the existing line-100 update,
including the required two-raster-line acknowledge pulse only when a raw-key
event arrives. Held state is retained from key-down through key-up; jump and
fire continue through the existing edge-trigger contract. Do not add an
Intuition event loop or re-enable interrupts inside gameplay for keyboard I/O.

MrDig accepted the complete standing/crawl hurt presentation after
`IMG_2736.MOV`: knockback, repeated contact, held-crouch behaviour, low-platform
clearance and recovery now form the Phase 3A baseline. Preserve frames 0-49,
standing hurt 50-53, crawl hurt 54-57, the 2/3/3/4 timing, half-height crouch
impulse and the rule that crawl hurt never queues standing landing frames.

#### Phase 3B: focused gameplay sound effects

This phase was implemented separately from HUD, renderer, level-art and
enemy-respawn work. Its original plan was to document and implement an explicit
Paula channel/priority policy that preserves the existing rapid energy-shot
sound and leaves a future music layout possible. Then generate host previews
and integrate one event at a time in this review order:

1. player hurt, triggered once per accepted damaging contact;
2. enemy hit, distinct from both plasma fire and beetle destruction;
3. jump, triggered only when a grounded jump is accepted.

Use `tools/generate_sparkpaw_sfx.py`, signed 8-bit mono/even-length raw samples,
Chip RAM and bounded DMA lifetimes. Add priority/cooldown rules before sharing
channels; repeated overlaps or held inputs must not retrigger an effect every
frame. Run `make` and `make release` after each accepted integration. HUD hearts
and hurt/invulnerability visual feedback remain the following independent
Phase 3C review.

The main sound-style reference for this phase is the original Amiga
`ThunderCats` game, especially the shooting/action around level 2 at 3:11 and
the broader effects and level rhythm around level 1 at 2:19 in
`https://youtu.be/ZHHgQkxiDfQ`. Use it only for broad qualities such as short
attack, clear arcade feedback and sparse readable layering. All Sparkpaw
samples, waveforms and compositions must remain original; do not copy or rip
source-game audio. The existing plasma shot remains unchanged unless a later
focused review explicitly replaces it.

Phase 3B's first host-side integration is implemented. Paula channel 0 remains
dedicated to the accepted rapid energy-shot path. Paula channel 1 is now the
prioritized gameplay-effect voice, while channels 2-3 remain free for a future
two-channel music layout. The original `player-hurt.raw` sample is loaded into
Chip RAM and plays once only when `playerTakeEnemyHit()` accepts real damage;
invulnerable overlap cannot retrigger it. After the first listening review
found the placeholder three-note effect too melodic, several original
synthesized vocal/impact candidates were reviewed. The selected runtime effect
is `player-hurt-candidate-stomp-low`: a short low body impact followed by a
falling involuntary grunt. Its bounded DMA lifetime is 12 frames, its cooldown
is 16 frames and its priority is 9.

The selected enemy-hit effect is `plasma-hit-candidate-energy-pop`. It plays
only when the projectile pipeline reports an actual beetle hit, uses priority
6, cooldown 4 and a bounded 8-frame DMA lifetime. The selected jump effect is
`jump-candidate-soft-whoosh`. `playerUpdatePhysics()` reports a jump event only
after the established grounded/clearance check accepts the jump; rejected or
held input cannot produce a sound. Jump uses priority 4, cooldown 4 and a
bounded 12-frame DMA lifetime. Both share Paula channel 1 under the explicit
priority policy; neither changes the independent channel-0 plasma-shot path.

For faster Phase 3B testing, completing all four beetle destruction sequences
also invokes the existing in-memory zero-health test reset. It resets player,
camera, projectiles and the complete fixed enemy pool while keeping assets and
packed caches resident and preserving prior Bob restore rectangles. This is a
whole-level test loop, not individual beetle respawn and not the Phase 4
spawn-data system.

#### Phase 3C: health HUD and invulnerability feedback

Phase 3C's first host-side step adds a fixed 320-pixel-wide HUD across the
bottom 48 logical PAL lines, following the placement and panel language of
`assets/concept/sparkpaw-gameplay-concept.png`. The existing six health units
map directly to three full, half or empty hearts; the accepted health and
damage rules are unchanged. The art includes Sparkpaw's portrait, the agreed
starting-life display `x 3`, a diamond icon without an invented count and one
empty framed region for later selected status information.

The HUD is a dedicated three-plane Copper section rather than an overlay in
the scrolling world. Its source is the ImageGen-derived
`assets/concept/sparkpaw-hud-concept-v1.png`; the deterministic host converter
creates the reviewed eight-colour 320x48 preview, one static 336x48 base,
seven compact 80x32 health patches and nine compact 32x24 lives patches.
`hud.c` owns two 336x48 presentation buffers and patches only stale dynamic
regions with direct Blitter copies when state changes. Runtime code stages the
selected buffer's six bitplane pointers during the established line-100
update. The Copper switches at
logical line 208 (hardware line 252), deliberately before the PAL line-255
boundary; the list restart restores normal world pointers next frame. Hardware sprites 6-7
remain free, Sparkpaw remains unchanged on attached sprites 0-5, and no CPU
compositing occurs in displayed Chip RAM. Bob passes and their restore/draw
ordering remain unchanged.

The first programmatically drawn HUD attempt failed in FS-UAE with horizontal
memory-like streaks throughout the bottom band and did not match the concept
art closely enough. The replacement SPBM is stored 336 pixels wide: 320 visible
pixels plus one padded fetch word, giving an exact 42-byte row equal to the
gameplay Copper fetch width. Copper modulo is derived from the loaded bitmap's
actual `BytesPerRow`, not assumed to be zero. The replacement also avoids the
rejected version's post-line-255 switch entirely. The HUD programs its own
eight-colour high/low AGA palette at the split. Treat the supplied corrupt HUD
screenshot as rejected evidence; only the asset-backed replacement is a retest
candidate.

The first asset-backed FS-UAE review confirmed a stable, recognisable HUD but
showed the complete bitmap about 15 pixels too far left: the portrait clipped,
the right edge left empty space, the 14x12 hearts felt undersized and the life
digit sat too close to its panel edge. The HUD split now uses fine scroll 15,
matching the proven zero-world-offset fetch alignment. Hearts are enlarged to
21x18 and vertically centred; the generated `x3/x2/x1` digit is moved six
pixels left within its existing frame. No Copper pointer, modulo, palette or
gameplay-rendering ownership changed in this alignment pass.

The HUD's `x3` life display is functional rather than decorative. `GameState`
starts with three total attempts including the current one. Zero health
decrements the display to `x2` and `x1` while using the established in-memory
reset. Because `GAME_OVER` remains a separate later state, losing the third
attempt temporarily begins a fresh `x3` test cycle. Completing all four beetles
performs the temporary whole-level test replay while preserving the current
lives and diamond count. The rejected first asset-backed
implementation stored 21 complete HUD combinations and consumed 127,008
bitmap bytes. The modular replacement uses about 13.4 KiB of loaded planar
assets plus about 14 KiB for two presentation buffers and the blank companion
plane. It therefore saves roughly 100 KiB of Chip RAM and avoids the future
`lives x health x diamonds` state explosion. Initial buffer construction uses
CPU copies before takeover; after display begins, only synchronized Blitter
copies touch the inactive HUD buffer. No CPU read-modify-write compositing
occurs in displayed Chip RAM.

The HUD deliberately replaces the concept art's five-heart layout with the
accepted three hearts per current life and omits the energy bar, weapon value,
crystal value and gear value until their systems exist. Phase 3C.2 adds the
safe whole-actor invulnerability blink; game-over remains a later state. The
50-diamonds-for-one-life rule is implemented by the completed collectible
step below.

#### Phase 3D.1: diamond collectibles and modular counter

Twenty fixed diamonds occupy a dedicated collectible pool; they do not
consume any of the four enemy slots. Their 32-pixel padded three-plane source
supports arbitrary word shifts without reading into the next row. Rendering
uses the existing synchronized Blitter discipline with camera culling. Reverse
restore order is projectiles, enemies, collectibles; forward draw order is
collectibles, enemies, projectiles, preserving overlap correctness.

Pickup collision uses the accepted player contact bounds. An inactive but
previously drawn diamond retains its restore rectangle until the next Bob
pass. The modular HUD adds one 32x24 patch atlas containing only counter states
00 through 49; health, lives and the static HUD are not duplicated. `GameState`
owns the count. Collecting the fiftieth diamond changes `49` to `00` and
increments the life stock; the compact life atlas supports `x1` through `x9`.
At maximum `x9`, the counter remains at 49 so pickups are not silently consumed
without a reward. The pickup sound was initially
deferred until the first visual/collision review.

The second FS-UAE collectible review requested richer concept fidelity,
animated Superfrog-like readability and clustered placement, while remaining
original. The pool now uses short trails and arcs above authored collision
surfaces. The initial rotation was rejected after FS-UAE video showed its
edge-on phases reading as sparks rather than diamonds. A subsequent four-frame
facet shimmer was also rejected after another FS-UAE video: its changing
internal planes still read as rotation and looked too thin beside the HUD icon.
The accepted retest candidate now uses one fixed, broader 16x24 silhouette with
the HUD's cream contour, bright cyan left face and deep-blue right/lower facets.
Only an eight-step, two-pixel vertical hover remains, with a per-item phase
offset. A host validation checks every complete
hover envelope against `storm-collision.bin`; the accepted set has zero
solid overlaps. The packed source remains word-padded.

The next FS-UAE screenshot exposed two concrete defects in that candidate:
foreground pen 7 is violet (not a cyan highlight), and shifted 16-pixel asset
Bobs could leave cream fragments below the gem through row overread. The world
diamond now uses only the actual foreground cream/cyan/blue pens 4/5/6 plus a
dark outline; violet pen 7 is absent. All static collectible X positions are
word-aligned while retaining their arcs, so the Blitter consumes exactly one
source word per row with no shifted overread. The complete hover envelopes
remain collision-free.

That word-alignment restriction alone did not remove the repeated cream shape;
MrDig supplied another FS-UAE screenshot showing it exactly one collectible
height below every active gem. Offline SPBM inspection confirmed all padded
second words were zero on all planes and mask rows, isolating the fault to the
generic PlanarAsset Blitter path. That path is no longer used for collectibles.
During renderer preparation the diamond is repacked once into the same exact
one-word mask plus three one-word plane caches used by the proven plasma and
beetle approach; gameplay calls `blitMaskedBob()` with sourceWords=1 and the
exact collectible height. This makes reading past a source row impossible without altering the
accepted world Bob ordering.

After residue was fixed, MrDig confirmed the world gem still differed in
orientation, geometry and scale from the HUD icon. The HUD icon is now the
sole geometry master: the generator copies its exact connected 16x21 pixel
component at preview coordinates `(207,14)-(223,35)` without scaling or
redrawing, mapping only HUD pen roles into the gameplay foreground bank. The
compact Bob cache and pickup height are therefore 21 rows. Full hover-envelope
collision validation still reports zero solid overlaps.

MrDig's first FS-UAE collectible review confirmed the hurt chirp and safe
invulnerability blink, but found the counter baseline low, the first diamond
art too arrow-like and several spawn Y values intersecting higher collision
tiles. Lives and diamond digits now share logical top Y=16, three pixels above
the original counter. The replacement diamond uses a cream concept-style
rim, four cyan/blue facets and a dark offset silhouette. Every spawn top is
derived from its local platform/column surface. Picking up a diamond now plays
the original `collect-spark` three-note arcade effect on Paula channel 1 at
priority 5: it may replace jump but cannot interrupt enemy-hit or player-hurt.

The final Phase 3D.1 FS-UAE review confirmed that the exact HUD-derived
diamond, gentle vertical hover, clustered placement, pickup sound and HUD
counter work. A later replay test confirmed the reward boundary: the pickup
after 49 displays 00 and changes x3 to x4, including across several temporary
four-beetle level replays. This is user-provided FS-UAE verification; no
real-hardware verification has been performed.

Lessons from this step:

- use one canonical indexed component for repeated icon geometry; copying the
  HUD diamond proved more reliable than redrawing an approximate world version;
- a fixed silhouette plus positional hover reads more clearly than rotation or
  facet animation at 16-pixel scale;
- validate the complete hover envelope against collision;
- repack small frequently drawn objects into exact mask/plane caches and pass
  the exact height to the proven Blitter path; the generic padded path produced
  a repeated fragment one object-height below the diamond;
- state logic, generated atlases and release assets form one contract. The
  first x4 C logic still shipped with the old three-frame, 900-byte lives atlas;
  regeneration produced the required nine-frame, 2628-byte atlas;
- keep the test replay isolated: it may preserve lives/diamonds for testing,
  while future level-complete, checkpoint and new-game transitions define
  their own persistence rules.

#### Phase 4: generic spawn/level data foundation

Move the four beetle placements into compact generic spawn records before
adding the second enemy type. Each record should identify enemy type, world
position, patrol/configuration data and persistence/respawn policy. Preserve
the current permanent-until-test-replay behaviour first; optional respawn can
then be added as a policy rather than a beetle-only timer. If enabled,
reactivate only after a cooldown while the spawn point is safely outside the
camera/player vicinity. The player must not see an enemy pop into existence or
repeatedly farm one while standing on its spawn point.

Retain a bounded fixed runtime pool and shared per-type art caches. Do not add
dynamic allocation or asset loading to the 50-Hz gameplay loop. Make permanent
versus respawning encounters a per-spawn choice so future level design is not
locked to one global rule.

Phase 4A/4B is implemented host-side. The four former parallel placement
arrays are now compact typed records in `level_data.c`, including exact world
position, patrol limits, initial velocity and persistence policy. Each bounded
runtime enemy slot records the spawn index and enemy type from which it was
initialized. All four records use the permanent policy, so destruction and the
existing complete test replay behave as before; policy-driven cooldown and
off-camera reactivation remain the separate Phase 4C step. Renderer ownership,
Bob caches, culling and restore/draw ordering are unchanged.

Phase 4C.1/4C.2 now adds deterministic bounded encounter variation without
adding respawn yet. Four authored candidates are required and two are optional,
so a complete load or test replay selects four to six beetles. Every selected
spawn chooses an X position inside its collision-safe authored range and one
of the tested fixed-point speeds 80, 96 or 112 while retaining its authored
initial direction. `GameState.enemySeed` records the explicit generation seed,
initialized from the OS-maintained time at level load; the next full replay
derives a reproducible new seed from it and the completed run length.
Six persistent spawn states feed the unchanged four-slot active pool. Slots are
activated near the camera and safely parked when their patrol is far outside
it, while permanent defeat remains attached to the spawn rather than the slot.
Level completion therefore checks all selected spawns, not merely the currently
loaded slots. Individual cooldown respawn remains the separate Phase 4C.3 step.
The renderer still loops over four slots and retains its exact packed-cache,
culling and restore/draw pipeline.

MrDig's first Phase 4C.1/4C.2 FS-UAE test confirmed that complete runs contain
four to six beetles and that defeating every selected beetle triggers the
expected in-memory level replay. Individual respawn is correctly absent at
this checkpoint. The configured movement speeds are selected correctly in C,
but their current 80/96/112 spread was not visibly distinguishable in this
test. Walk animation also still uses the global `(frameCounter >> 2) & 3`
cadence for every beetle, independent of velocity. Review both the speed spread
and velocity-linked walk cadence as a focused tuning step before adding
Phase 4C.3 respawn; do not infer a runtime defect merely from the intentionally
small current fixed-point differences.

That focused tuning step is now implemented host-side. The three fixed-point
speeds are widened to 64, 96 and 128 (12.5, 18.75 and 25 pixels/second at PAL
50 Hz). Each runtime enemy carries its own `walkTick`; accumulated absolute
movement advances a walk frame every 384 fixed-point units. The resulting
cadences are one pose per six, four or three PAL frames, so feet cycle in
proportion to movement and beetles no longer share a globally synchronized
walk phase. Hit and destruction timing, patrol collision, spawn persistence,
four-slot renderer ownership and all Bob contracts remain unchanged. This
tuning requires FS-UAE review before Phase 4C.3 respawn proceeds.

MrDig's first review requested a much stronger classic-arcade distinction,
with clearly very slow, normal and very fast small enemies. The tuning candidate
therefore uses 48, 96 and 192 fixed-point units: 9.375, 18.75 and 37.5
pixels/second. With the same distance accumulator these advance a walk pose
every eight, four and two PAL frames respectively. Even the fastest profile
moves only 0.75 pixel per frame, so it cannot skip a 16-pixel collision tile or
require a different patrol query. This stronger spread awaits FS-UAE review.

Phase 4C.3 changes the temporary test objective to match the intended game.
Enemy destruction is no longer a level-completion condition and
`enemiesAllDefeated()` has been removed. Reaching the exact far-right collision
edge now invokes the existing in-memory replay path; this is explicitly a
temporary stand-in for the later `LEVEL_COMPLETE -> next level` transition.
All six selected encounter candidates use repeat policy. After a complete
destruction sequence a spawn receives a random 250-500-frame cooldown, then
rerolls its safe authored X position and 48/96/192 speed only while its entire
patrol lies beyond the camera safety margin. It remains ready there until the
player approaches and a slot is free. Respawn is unlimited, so repeatedly
walking almost to the right edge and back left continues to generate fresh
encounters without requiring level completion. The four active runtime slots,
packed shared cache, camera culling and synchronized Bob ordering remain
unchanged. Right-edge replay continues to preserve the current lives and
diamond total as the earlier test replay did.

The first FS-UAE exit test exposed that Sparkpaw could visibly reach the right
wall without triggering replay. The temporary exit compared world pixel 1279
against the deliberately narrower contact-damage box (`x + 25`), while solid
tile physics correctly stops the player using `HIT_RIGHT` (`x + 27`). The exit
now queries `playerReachedWorldRight()` and therefore uses the same accepted
solid edge as horizontal collision. Zero health continues to consume one life
and restart the complete level in memory; this is accepted as the provisional
death/restart model, distinct from simply taking one half-heart of damage.

MrDig's final Phase 4 FS-UAE regression on 12 August 2026 accepted the complete
result: four to six encounters, clearly differentiated slow/normal/fast beetle
movement with matching animation cadence, unlimited off-screen respawn during
repeated left/right traversal, right-edge-only level replay, and zero-health
life loss with level restart all behaved correctly. Diamonds remain persistent
across both right-edge replay and life-loss restart while world collectibles
are repopulated. This is user-provided FS-UAE verification; real-A1200 testing
remains open. Phase 4 is complete and Phase 5 may begin with the larger-enemy
design brief before art or runtime implementation.

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

This is the recommended content milestone after the small Phase 4 foundation.
Start with a design brief and concept review only: role, silhouette,
approximate dimensions, HP, contact damage, attack telegraph, collision
rectangles and maximum simultaneous visibility. Do not combine its first art
pass with changes to the dual-playfield renderer or HUD.

Phase 5 concept pass v1 is prepared for review. The proposed enemy is the
original **Clockwork Storm Sentinel**: a stocky two-legged ruin guardian with
dark-steel body, restrained aged-brass family cues, moss wear, piston legs, one
cyan glass core and an oversized shield/ram forearm. Its intended first-pass
gameplay role is a slow heavy patrol with four HP, no jumping or projectile,
an unmistakable cyan charge telegraph and a short horizontal ram. Target
runtime envelope is approximately 48x40 pixels, with at most one Sentinel and
two beetles simultaneously visible; these are design targets, not accepted
runtime contracts yet. The four-pose concept shows guarded idle, heavy walk,
charge and ram/recovery. Sources are
`assets/enemies/clockwork-storm-sentinel-concept-v1-chroma.png` and
`clockwork-storm-sentinel-concept-v1-transparent.png`; the full prompt is in
`docs/IMAGEGEN_PROMPTS.md`. The concept intentionally carries more material
detail than an eight-colour Bob. After MrDig's visual review, create a separate
native-minded production sheet and indexed preview rather than auto-fitting
each concept pose into a runtime cell.

A contrasting Phase 5 concept candidate is also prepared: the **Clockwork
Storm Strider**, a lean spring-loaded ruin machine with long digitigrade piston
legs, compact torso, stabilizer tail and cyan visor/core. Its four-pose sheet
shows alert idle, low fast run, charged crouch compression and a committed
forward leap. The provisional role is a faster three-HP patrol that can later
jump between explicitly authored platforms after a readable cyan telegraph; it
has no shield, ram or projectile. Its lean silhouette should cost materially
less Bob area than the Sentinel, though a leap adds AI, collision and recovery
complexity. Sources are
`assets/enemies/clockwork-storm-strider-concept-v1-chroma.png` and
`clockwork-storm-strider-concept-v1-transparent.png`; prompt details are in
`docs/IMAGEGEN_PROMPTS.md`. Select between Sentinel and Strider before creating
any native production sheet or runtime code; do not combine both enemy types
into the first Phase 5 implementation.

MrDig selected the **Clockwork Storm Strider** as the Phase 5 enemy direction.
The Sentinel concept remains preserved for a later heavy enemy or miniboss; if
used, its shield should block frontal plasma only during an explicit guarded
charge state, not create permanent frontal immunity.

The accepted incremental Strider plan is:

1. **Phase 5A — lock the runtime design contract.** The original target was
   approximately 48x40; the accepted upright revision is 64x64
   pixels and three HP. Standing, crouched and airborne Sparkpaw shots all
   damage it whenever their projectile path intersects its state-specific
   hitbox; there is no shield, reflection or required firing posture. Contact
   damage uses the existing player hurt/invulnerability contract. The level
   should ultimately contain two guaranteed and one optional Strider alongside
   four to six beetles, while retaining only four active runtime enemy slots.
2. **Phase 5B — native production art and palette proof.** Create a fixed-cell,
   shared-scale sheet with alert/idle, four-phase run, two-phase compression,
   launch/flight/descent, landing/recovery, hit and four destruction stages.
   Generate deterministic mirrored cells and an eight-colour indexed preview
   before adding a runtime consumer. Preserve a single grounded foot baseline;
   do not auto-fit individual crouched or airborne poses.
3. **Phase 5C.1 — generic multi-type level data.** Expand persistent spawn
   capacity to cover 4-6 beetles plus 2-3 Striders. Give each encounter an
   authored patrol surface (`left`, `right`, `groundY`) so enemies can occupy
   both the floor and raised platforms without runtime ground searching. Slot
   selection remains type-independent, never parks a visible enemy, and gives
   priority to visible/ahead-of-camera encounters.
4. **Phase 5C.2 — packed Strider Bob integration.** Prepare one shared
   type-specific packed cache and dispatch width, height, frames and culling by
   enemy type while preserving the existing reverse restore/forward draw
   ordering and synchronized Blitter minterms. Prove one static floor Strider
   and one static raised-platform Strider before enabling AI.
5. **Phase 5C.3 — floor and raised-platform patrols.** Enable two guaranteed
   and one optional persistent Strider: at least one on the floor and one
   walking back and forth on a higher authored platform. Each keeps its own
   speed, animation, HP, hit/death and unlimited safe respawn state. Initially
   it never leaves its assigned surface. Target no more than three visible
   enemies during ordinary placement; test two simultaneous Striders before
   allowing three on screen.
6. **Phase 5D — one authored platform jump link.** At an explicit launch zone,
   stop, show a two-stage cyan compression telegraph, follow a fixed proven
   ballistic arc, land on an explicit destination surface and recover before
   resuming patrol. Persist the complete state through camera slot parking. Do
   not implement general pathfinding or infer arbitrary landings from pixels.
7. **Phase 5E — broader traversal.** Only after the single link is accepted,
   add return links, low/high transitions and authored jumps across gaps and
   water, including blocked/missed-landing and off-camera rules. Never respawn
   a Strider midway through a jump or begin an unseen attack immediately beyond
   the camera edge.

Two to three Striders means persistent level encounters, not simultaneous Bob
guarantees. The renderer remains capped at four active enemy slots. The first
Phase 5 implementation includes only the Strider; do not also implement the
Sentinel or change gameplay colour depth, dual-playfield timing or HUD.

Phase 5C.3 is accepted in FS-UAE as of 13 August 2026. Two guaranteed Striders
patrol an authored raised platform and a long floor route, with one optional
third encounter retained in level data. Runtime slots 0..7 contain the accepted
eight-frame rigid mechanical gait; slot 8 is the planted premium front turn,
shown for six frames only at an authored patrol extremum. The decisive final
fix wraps walk advancement directly from frame 7 to frame 0 (`&7`), preventing
the reserved turn frame from leaking into normal locomotion. MrDig confirmed
that both walking and endpoint turns are now correct. The synchronized Bob pass
begins at hardware line 253 after the existing line-252 HUD switch; this timing
fix eliminated the earlier first-approach/reload corruption without changing
the line-100 Copper staging or restore/draw order. No real-hardware validation
has been supplied. Slots 9..17 remain reserved placeholders: later work still
needs explicit Strider shooting/attack, hurt/hit and death animation/behaviour.

The next implementation step is **Phase 5D**, one authored platform jump link.
Keep it narrow: add an explicit launch trigger and destination surface, a clear
two-stage compression telegraph, one fixed ballistic arc, a planted landing and
recovery before patrol resumes. Persist that state through camera parking. Do
not add generic pathfinding, multiple links, gaps/water, combat animations or
longer-level work in the same change. Phase 5E then broadens traversal across
gaps/water; Phase 6 remains the agreed 2048px/eight-screen level experiment and
greybox after the second enemy's current-world behaviour is complete.

#### Phase 5D implementation checkpoint (13 August 2026)

The first narrow traversal link is now implemented for the required raised-
platform Strider. Level data owns the source spawn, right-facing launch zone,
destination patrol surface, landing window and fixed 8.8 launch/gravity values.
The enemy module consumes that record through explicit compression-start,
compression-charged, flight, landing and recovery states. It does not search
collision pixels for a destination or add generic pathfinding. On recovery the
Strider adopts the authored lower-floor surface and resumes its original
randomized grounded patrol speed.

The packed Strider contract expands from 18 to 24 frames. Accepted walk slots
0..7 and planted turn slot 8 are unchanged; combat-response reservations
9..17 remain untouched. Appended slots 18..23 contain deterministic cyan-
signalled compression, flight/descent and planted landing/recovery poses derived
from the accepted palette and silhouette. The renderer still uses the same
type-specific packed cache, camera culling, stored restore rectangle and line-
253 synchronized restore/draw pass.

`EnemySpawnState` already persists the complete runtime `Enemy` value. The new
fixed-point vertical accumulator, velocity, resume speed, traversal phase,
timer and link identity therefore survive runtime-slot parking with no renderer
special case. During flight the stored patrol envelope temporarily covers both
authored surfaces so activation/unloading uses the complete link extent.

Host trajectory checks proved every integer launch origin in the authored
360..364 window lands inside x=416..419 after 55-56 flight frames. The Bob top
never rises above logical y=6, avoiding the existing world-bound cull. Native
`make` succeeds. FS-UAE and real-hardware verification remain outstanding;
Phase 5D is not accepted until MrDig supplies the runtime result.

The first supplied Phase 5D FS-UAE screenshot/report showed that the jump itself
appeared correct and the Strider resumed its lower-floor patrol, but that patrol
then visibly crossed the low platform beginning at x=496. This was not a failed
ballistic or Bob restore: grounded safety probes inspect the planted-foot region,
so an overhead/torso-height solid does not block the current local patrol code.
Keep generic full-body obstruction handling and alternate traversal choices in
Phase 5E. For the Phase 5D proof, narrow only the authored destination surface
from `{416,640,208}` to `{416,496,208}`. Its existing inset leading-foot probe
then reverses before the 64px body overlaps the platform. The jump parameters,
landing window, renderer and collision map remain unchanged. A focused FS-UAE
regression is required before accepting the phase.

The next FS-UAE report confirmed that the x=496 right turn now prevents the low-
platform overlap. It also showed the consequence of retaining destination left
bound 416: the inset left-foot probe reversed at the right edge of the original
raised platform, even though its underside ends at y=143 and the floor Strider's
logical top is y=144. The route should visibly continue beneath that valid
clearance. Extend only the authored left bound to 300. With the 12px left-foot
inset, the actor's left edge reaches x=288 and no farther, stopping at the solid
column occupying x=256..287. The right bound remains 496. This gives a useful
floor patrol beneath the start platform without generic body collision, new
links or geometry changes; focused FS-UAE regression remains required.

MrDig's final Phase 5D FS-UAE report accepted the corrected local result: after
landing the Strider turns before the low platform, then continues left beneath
its original raised platform. Phase 5D is therefore accepted in FS-UAE. No real-
hardware verification has been supplied.

The longer-term traversal target was clarified at acceptance. Striders should
eventually seem to run and jump throughout a richer level in both directions,
entering the viewport naturally from either side and continuing after Sparkpaw
jumps over them. The camera must control only rendering/detail, never create or
terminate the route. Full Bob rendering and expensive local collision may be
skipped far offscreen, but a cheap world-space simulation must retain surface,
link, position, direction and state; full simulation resumes before entry into
the visible region. Phase 5E should express this as stable authored surface IDs
and bidirectional links with offscreen, blocked and missed-landing rules—not
camera-edge spawning or arbitrary pixel pathfinding.

#### Phase 5E.1 stable surface identities (13 August 2026)

The first 5E step is a narrow data/AI refactor with no new visible behaviour.
All ten currently authored patrol surfaces now have explicit stable IDs in one
level table. Spawn candidates retain randomized X ranges, direction, type and
policy but reference only a starting surface ID. Traversal links no longer bind
to a spawn index or embed a copied destination rectangle; they connect a source
surface ID to a destination surface ID and retain only launch/landing/ballistic
parameters.

The runtime link selector scans every record matching the actor's current
surface and chooses one whose direction and launch zone are currently valid. It
does not stop at the first source-surface match, so later authored branches from
one surface remain representable without changing the AI lookup contract.

Persistent `Enemy` state now includes its current surface ID. Initialization
resolves the spawn's surface, the accepted 5D link is selected by current source
surface, and landing recovery changes the actor to the destination surface ID.
The cached patrol bounds remain in runtime state so activation priority, camera
parking and grounded movement remain byte-for-byte in the established path.
Respawn deliberately resolves the spawn's starting surface rather than the
actor's last traversal surface, preserving complete level-replay semantics.

This establishes the data contract needed for a separately authored return link
without adding that link yet. Native `make` and `make release` succeed, including
bootable DOS1/FFS ADF validation. MrDig then confirmed in FS-UAE that all
accepted 5D behaviour remained correct after this refactor, accepting Phase
5E.1. No real-hardware result exists.

#### Phase 5E.2 one authored return link (13 August 2026)

The stable surface graph now contains one reverse traversal record from
`SURFACE_STRIDER_JUMP_FLOOR` to `SURFACE_STRIDER_RAISED_START`. It triggers only
while the Strider travels left in the authored x=440..444 zone after turning at
the low platform. It reuses the accepted compression, flight, descent, landing
and recovery frames; no cache or renderer change is involved.

A direct mirror of the downward trajectory was rejected during host planning
because a 64px actor would cross the platform's right face while still below its
top. The chosen 8.8 arc uses vx=-512, vy=-2656 and gravity=128. Across every
integer launch origin x=440..444, the actor crosses left of x=416 only after its
logical top reaches y=54 or higher clearance, then lands after 30 frames within
x=380..384 on the y=128 raised surface. Its logical top never rises above y=41,
so existing world-bound culling remains valid.

After recovery the persistent current surface becomes the raised surface and the
saved leftward speed resumes. It must reach the left extremum, turn right and may
then encounter the original downward link again, producing a fully authored
two-way loop. Offscreen abstraction, additional links, gaps/water, failure rules,
combat and renderer work remain outside 5E.2. Native `make` and `make release`
succeed, including bootable DOS1/FFS ADF validation. FS-UAE review is required
before acceptance; no real-hardware result exists.

MrDig's first 5E.2 FS-UAE test showed the accepted downward jump but no return:
the Strider stayed on its floor patrol. The cause was deterministic state, not
the return zone or trajectory. Phase 5D recovery cleared `traversalState` but
left `traversalLink` holding completed link index 0. With only one one-way link
this stale ownership had no visible effect; the general link-start guard then
correctly—but permanently—rejected 5E.2 because a link still appeared owned.
Successful recovery now also restores `traversalLink` to
`INVALID_TRAVERSAL_LINK`. The current destination surface ID, patrol bounds,
direction and speed remain intact, allowing the separately authored reverse
link to become eligible. Rebuild and focused FS-UAE regression are required.

MrDig supplied `sparkpaw/testresults/Phase 5E.2-return-loop.mov`, a 10.08-second
60 fps FS-UAE recording of the HD build. It visibly completes the authored loop:
raised platform to floor, safe floor patrol, return arc, raised landing/recovery
and another cycle. Sampled frames show no obvious platform intersection,
incorrect surface reset or persistent Bob residue. Phase 5E.2 is accepted in
FS-UAE/HD. The adjacent ignored `.txt` records metadata and review scope. ADF
parity and real-A1200 verification remain open.

#### Phase 5E.3 parked world-space simulation (13 August 2026)

Previously, camera parking copied complete enemy state into `EnemySpawnState`
but stopped updating it until the route approached the camera again. This froze
offscreen Striders and contradicted the accepted persistent traversal target.
The persistent spawn pass now advances every selected, active, non-respawning
Strider whose runtime slot is parked exactly once at the start of each game
frame. Loaded slots continue through the existing runtime loop, so no Strider
receives two updates on an unload or activation boundary. Beetles deliberately
retain their accepted Phase 4 parking behaviour.

The parked path calls the same deterministic world-space state update used by a
loaded enemy, preserving patrol probes, animation distance, link selection,
telegraph, fixed-point flight, landing and recovery. It allocates no Bob slot and
performs no renderer, cache, restore/draw or displayed Chip-RAM work. Current
patrol/link envelopes still decide when an encounter becomes eligible for one of
the four runtime slots. Native build/release and focused FS-UAE regression are
required before 5E.3 acceptance; no ADF or real-hardware result exists.

MrDig's follow-up FS-UAE test reported that the parked traversal behaviour looks
good, accepting Phase 5E.3. No ADF-specific or real-hardware result was supplied.

#### Phase 5E.4 blocked and missed traversal rules (13 August 2026)

Traversal now validates both extreme authored landing origins before claiming a
link. The direction-specific planted-foot inset must have solid support at the
destination ground Y and clear space eight pixels above it. If either probe is
blocked or unsupported, the Strider remains on its current surface and reverses
away; it does not infer another surface or begin a doomed telegraph.

Each launch stores its source X and starts a bounded flight counter. A descending
actor that reaches destination height outside its landing window, or any flight
that reaches 96 frames, fails deterministically. It returns to the stored source
position and current source-surface bounds, shows the existing planted recovery,
releases the completed link and resumes in the opposite direction. Destination
surface ownership changes only after a successful landing/recovery.

Host collision-map checks confirm both extremes of both accepted links have
support and clearance: downward foot probes x=467/479 at y=208 and upward probes
x=388/404 at y=128. Their visible trajectories therefore remain unchanged. No
new gap, water, surface, animation, renderer or combat behaviour is included.
Native `make` and `make release` succeed, including bootable DOS1/FFS ADF
validation. Focused FS-UAE regression remains open.

The first focused HD/FS-UAE recording exposed one premature fallback in the
accepted downward arc. The actor reaches destination height one update before
entering the landing window, but the initial 5E.4 rule treated every
out-of-window height crossing as a miss. A miss is now declared only after the
actor has passed the window in its travel direction; approaching it remains
valid, while the independent 96-frame limit still bounds malformed flights.
Evidence is retained locally as
`testresults/Phase 5E.4-premature-fallback-regression.mov` with its analysis
sidecar. MrDig's corrected HD/FS-UAE retest then confirmed that the complete
down, lower-floor patrol and return loop works again, accepting Phase 5E.4. No
ADF-specific or real-hardware result was supplied.

#### Phase 5E.5 raised-platform gap proof (13 August 2026)

The second required Strider now owns an authored route rather than only pacing
the long floor. The first implementation crossed the 80px gap left of its
x=848..991 high platform. MrDig's six-second HD/FS-UAE recording showed the
Strider turning and moving slowly near the opposite edge but never reaching the
link. This was not a frozen traversal state: at the slowest existing randomized
8.8 speed (`48`), walking roughly 65 pixels to that remote zone takes almost
seven seconds before telegraph and flight. The encounter therefore looked stuck
and its intended action was outside the camera scene being judged. The evidence
is retained locally as
`testresults/Phase 5E.5-apparent-stall-before-gap-route.mov` with its sidecar.

The revised proof uses the adjacent gap right of that platform. Stable source
bounds remain x=860..980 at ground y=112; the lower x=1072..1199 platform uses
leading-foot bounds x=1084..1188 at ground y=160. Moving right through actor
x=928..932 launches at `vx=1024`, `vy=-832`, gravity `64` and lands at
x=1072..1080. The return moves left through x=1072..1076 with `vx=-1024`,
`vy=-1472`, gravity `64` and lands at x=928..936. These repeatable crossings
stay together in the same camera view and use the accepted bidirectional-link
machinery, not a new navigation system. Existing foreground and collision
geometry are reused: the continuous player floor and safe route remain intact,
and no water/death hazard is implied.
No generated asset, renderer, cache, Bob ordering, combat or animation contract
changes. Native `make` and `make release` succeed, including bootable DOS1/FFS
ADF validation. Focused FS-UAE regression remains required.

MrDig's 13.43-second follow-up HD/FS-UAE recording shows the revised route
completing in both directions: rightward telegraph, gap crossing, planted lower
landing and patrol, followed by the higher leftward return and stable recovery
on the source platform. No premature fallback, floor contact, platform-edge
intersection or apparent stalled state is visible. This accepts Phase 5E.5 for
HD/FS-UAE; ADF-specific and real-hardware verification remain open. Evidence is
retained locally as
`testresults/Phase 5E.5-accepted-bidirectional-gap-loop.mov` with its sidecar.

This closes the scoped Phase 5E traversal pass. The accepted contract now covers
stable surface/link ownership, bidirectional low/high links, logical offscreen
persistence, blocked/missed-flight recovery and a repeated raised-platform gap
loop. Real water/death-hazard semantics belong to later level work rather than
this safe continuous-floor proof. The next isolated implementation step is
Phase 5F.1: Strider body contact through the existing player damage, knockback,
invulnerability, life-loss and reset path. Shooting and Strider hurt/death art
remain separate later steps, and slots 9..17 stay reserved.

#### Phase 5F.1 Strider contact implementation (13 August 2026)

The generic enemy-contact query now accepts active Striders as well as beetles.
A fixed logical Strider box uses x offsets 11..52 and y offsets 7..61 inside the
64x64 cell. This deliberately excludes the broad transparent side margins and
the accepted transparent source rows 62..63 while retaining torso, legs and the
compressed traversal poses. The returned knockback origin is the 64px logical
centre rather than the beetle's 32px centre.

No new damage system is introduced. `gameUpdate()` still performs one contact
query after player/enemy physics and passes the centre to the unchanged
`playerTakeEnemyHit()`, preserving half-heart damage, crouched hurt selection,
directional knockback, invulnerability, sound, life loss and reset. Enemy walk,
turn and traversal state are not changed by contact. Slots 9..17, shooting,
Strider hurt/death, renderer/cache/Bob timing and displayed Chip RAM remain
untouched. Native `make` and `make release` succeed, including bootable DOS1/FFS
ADF validation. Focused FS-UAE review remains required.

MrDig then exercised contact broadly in the HD build under FS-UAE: low/lying
approaches, normal walking contact and jumping contact all caused the expected
Sparkpaw damage. This accepts Phase 5F.1. No ADF-specific or real-hardware
result was supplied. The next isolated step is Phase 5F.2, a camera-aware and
clearly telegraphed Strider ranged attack. Its projectile ownership, visibility
rules and any Paula priority must be explicit; Strider hurt/death remains later.

#### Phase 5F.2 ranged attack implementation (13 August 2026)

The projectile pool now has explicit ownership: indices 0..5 remain Sparkpaw's
accepted rapid plasma capacity and indices 6..7 are hostile-only. Both families
reuse the existing 16x9 packed plasma patterns, background restore, collision
query and synchronized line-253 Bob pass. Hostile pulses move at 1150 in 8.8
units, live for at most 100 frames, cannot hit enemies, and are consumed into
the existing five-frame impact on player overlap. Damage then uses the same
central `playerTakeEnemyHit()` path as body contact. The reset path preserves
prior draw rectangles for both added slots.

A Strider may enter the attack only while grounded, fully inside the viewport,
not turning/traversing/hit/dying, facing Sparkpaw, within 48..208px horizontally
and within 44px vertically. It stores its patrol velocity, stops for a 24-frame
telegraph, selects slot 9 for charge and slot 10 for the six-frame release, then
restores the exact prior direction and begins a 150-frame cooldown. Initial
cooldown is staggered per persistent spawn. Offscreen logical updates may reduce
cooldown but never start an attack; if a charging actor is parked, its pending
shot is discarded rather than materializing on return.

Runtime generation assigns only combat-reserved slots 9 and 10 to cyan-enhanced
charge/release poses and emits an indexed two-frame preview. Slots 11..17 remain
untouched for later hurt/death. No new sample is added: Paula channel allocation
and priorities remain exactly as accepted. Renderer timing, cache layout,
traversal frames 18..23 and displayed Chip RAM composition are unchanged.
Native `make` and `make release` succeed, including bootable DOS1/FFS ADF
validation. Focused FS-UAE review remains required.

MrDig's 25.12-second HD/FS-UAE recording then showed repeated attacks, slower
hostile pulses, Sparkpaw damage/invulnerability and continued Strider routes,
accepting the Phase 5F.2 functional core. The same review correctly identified
three presentation gaps: hostile fire reuses Sparkpaw's cyan pulse, intentionally
has no sound, and the derived slots 9/10 do not show a clearly readable weapon.
The current actor has an arm/claw near the spawn point, not an authored gun,
cannon or gauntlet. Evidence is retained locally as
`testresults/Phase 5F.2-accepted-ranged-core-needs-weapon-polish.mov` with its
sidecar.

Treat the next step as Phase 5F.2A presentation polish, not a new combat system:
author a small integrated storm gauntlet or arm cannon only in slots 9/10, add a
distinct hostile projectile colour treatment, and add one short original shot
sample on Paula channel 1 with an explicit priority below player hurt. Preserve
slots 11..17, projectile ownership, damage rules and synchronized line-253 Bob
timing. ADF-specific and real-hardware verification remain open.

Phase 5F.2A implements that presentation pass without altering attack timing or
pool ownership. Only slots 9/10 receive a small tapered violet arm housing with
a bright muzzle; the complete cell is still mirrored by the existing generator,
and left/right projectile origins were aligned to those endpoints. The 16x9
hostile mask and impact frames remain structurally identical, but their player
blue/cyan pens are remapped to existing foreground violet/magenta pens 13/14
with the neutral bright core retained. This doubles only the tiny generated
plasma pattern cache, not the projectile Bob dimensions or pass order.

An original 0.16-second low electrical thump is generated as
`strider-shot.raw` and loaded with the resident effects. It uses prioritized
Paula channel 1 at priority 7 and a 12-frame cooldown: player hurt priority 9
may always interrupt it, while enemy-hit priority 6 and lower effects cannot
replace it mid-sample. Paula channel 0 stays dedicated to Sparkpaw plasma and
channels 2/3 remain free for later music. Focused FS-UAE presentation review,
ADF-specific testing and real-hardware verification remain open. Native `make`
and `make release` succeed; the latter validates the bootable DOS1/FFS ADF with
the new 1,764-byte sample and reports 886,272 bytes total data/filesystem use.

MrDig's next HD/FS-UAE recording rejected the procedural gauntlet: despite the
functional shot and sound, its solid overlay appeared to emerge from the belly
rather than connect convincingly to an arm. The same recording called out a
small violet dash that was already present at its start and remained after level
reload. Frame comparison shows no projectile flight/impact evolution; it matches
the authored distant parallax storm-light language. The reset/restore path did
not leave a projectile alive, but the new violet hostile palette made background
decoration plausibly read as residue. Evidence is retained locally as
`testresults/Phase 5F.2A-rejected-overlay-and-parallax-light-confusion.mov` with
its sidecar.

The overlay is removed rather than refined. Using the accepted 1254px premium
idle source as the authoritative edit target, built-in image generation created
one replacement source that preserves the full head, torso, blade arm, tail,
legs and stance while replacing only the forward claw/forearm with a connected
storm arm cannon. Chroma-key removal produced a project-local transparent source;
the runtime generator fits that complete actor once and derives both slots 9/10
from identical anatomy. The exact prompt is recorded in `docs/IMAGEGEN_PROMPTS.md`.
The hostile pixel mapping moves from violet/magenta to existing hot orange/red
pens 2/3 with the white core retained, eliminating the parallax-light ambiguity.
Focused FS-UAE review remains required.

MrDig's 3.68-second HD/FS-UAE follow-up accepted the newly authored integrated
arm cannon, mirrored white muzzle and orange/red hostile pulse. The functional
attack remains accepted. The first 0.16-second sound triggered correctly but
was judged too light: its 185 Hz square-wave/tick emphasis read as a small retro
bleep rather than a heavy mechanical discharge. Evidence is retained locally as
`testresults/Phase 5F.2A-accepted-arm-cannon-visual-sound-too-light.mov` with its
sidecar.

Only the deterministic source synthesis changes for the retest. The replacement
is 0.20 seconds and layers a 92 Hz falling body thump, short 184 Hz metallic
crack, low-pass impact noise and brief descending electrical tail. Runtime
volume rises from 60 to 64. Paula channel 1, priority 7, cooldown and attack
timing remain unchanged; player hurt priority 9 still preempts it.
Native `make` and `make release` succeed. The bootable DOS1/FFS ADF validates
with the 2,206-byte replacement raw sample and reports 886,784 bytes total
data/filesystem use. Focused HD/FS-UAE sound review remains open.

The next HD screenshot showed a stationary orange projectile, proving a second
case was real Bob residue rather than the authored violet parallax light. The
cause was a pool-size mismatch introduced with hostile ownership: reset saved
`drawn/drawnX/drawnY` for only slots 0..5, cleared the pool, then restored all
eight entries from the partially uninitialized stack arrays. Hostile slots 6..7
could therefore inherit arbitrary restore state after death or level reload.
The snapshot and restore loops now both cover all eight entries. During the
same boundary audit, Sparkpaw's spawn loop was corrected from all eight entries
to its reserved slots 0..5. The synchronized line-253 erase/draw ordering and
Bob implementation are unchanged. Native `make` and `make release` pass; the
bootable DOS1/FFS ADF remains 886,784 bytes at 97.46% data use. HD/FS-UAE review
should force a reset with a hostile pulse visible and hold player fire to cover
both corrected bounds.

MrDig completed that focused HD/FS-UAE review: the heavier Strider discharge
was accepted and no loose orange hostile shots remained after the reset/pool
boundary correction. Phase 5F.2A is therefore accepted. No real-hardware result
exists. The next recommended isolated step is Phase 5F.3: give the Strider a
clear hurt/hit reaction and HP contract using slots 11..17, preserving slots
18..23 for accepted traversal; implement death and safe respawn in a subsequent
reviewable step rather than combining it with the hit reaction.

Phase 5F.3 implementation gives each Strider its existing authored three-HP
contract at runtime and routes player plasma through the accepted body-aware
box. A successful damage event stops ground movement for a deterministic
14-frame mechanical recoil across reserved slots 11..17, holding each pose for
two ticks. Shots
that overlap during the reaction are consumed but do not stack damage or reset
the reaction. Grounded ranged charge/release can be interrupted cleanly and
enters the existing cooldown; traversal remains dominant and is not
hit-interruptible. HP clamps at one, so this phase cannot enter teardown,
death or respawn. Slots 18..23, route data, contact damage, projectile pool
ownership and the renderer remain unchanged. Native build and release results
are recorded with the implementation checkpoint; FS-UAE and real-hardware
review remain open.

MrDig's first supplied Phase 5F.3 FS-UAE recording confirmed that grounded
plasma hits trigger the new recoil and allow the Strider to resume. It also
exposed two defects. Traversal was excluded wholesale from projectile overlap,
making slots 18..23 invulnerable rather than merely non-interruptible. Hits may
now reduce HP during traversal and set one persistent pending reaction; the
accepted route completes first, then slots 11..17 play after recovery. Further
overlaps while either reaction is pending remain consumed without stacked
damage.

Two supplied screenshots also proved that the Phase 5F.2A shoot cells did not
actually preserve the accepted anatomy. Although they shared the 64x64 cell and
FRONT16 palette, independently fitting and quantizing the complete premium
source produced a 49x60 opaque actor versus roughly 39-42x55-56 for walk, with
a visibly different pen distribution. Slots 9/10 now start from the exact
accepted idle cell and replace only the forward forearm/claw with the existing
premium cannon fragment. The corrected cell is 45x56, keeps the original
head/body/legs/grounding, remains close to the accepted palette balance and
retains the existing charge/release timing and mirrored packed layout. Renderer,
projectile ownership and authored routes remain unchanged. Focused FS-UAE
review of both corrections is pending; no real-hardware result exists.

MrDig's next two FS-UAE recordings showed that the pending traversal reaction
was technically preserved but visually wrong: the Strider completed its jump,
landed, and only then performed recoil for an old hit. Traversal hits now retain
immediate HP loss, projectile impact and hit sound but schedule no later body
reaction; grounded hits still own slots 11..17. The same recordings exposed a
shoot-cell cutout rather than renderer residue. The cannon transplant cleared
from y=18 and copied extra upper fragment pixels, temporarily removing lower
head/neck pixels and leaving a loose-looking mark around the gun. The edit is
now restricted to x>=33, y=24..44 and copies only the matching cannon band;
head, neck and upper torso remain byte-identical to the accepted idle cell.
Renderer restore/draw code remains unchanged. Focused FS-UAE review is pending.

MrDig's 09:06 FS-UAE recording then showed the hostile pulse leaving above the
corrected cannon. Pixel inspection found the real white muzzle at local rows
35..37, while the inherited charge marks remained detached at rows 29..31 and
the 9-pixel projectile was spawned with its centre near row 31.5. The cannon
itself is not moved: charge now centres on row 36 and hostile projectile top Y
moves from `enemyY+27` to `enemyY+32`, placing its centre at row 36.5. Attack
timing, speed, collision, ownership and renderer ordering remain unchanged.

MrDig accepted the corrected Phase 5F.3 behaviour as sufficient to continue.
This supplied FS-UAE acceptance covers grounded hit recoil, traversal damage
without delayed recoil, stable shoot anatomy and the corrected row-36 muzzle/
projectile alignment. No real-hardware result exists.

Phase 5F.4 adds Strider death and safe respawn without consuming any accepted
slot. The complete 0..23 contract stays fixed; four destruction cells append at
24..27. A third grounded hit stops motion, pending fire and contact, then holds
core fracture, collapse, burst and debris for five ticks each. The existing
generic respawn lifecycle supplies the 250–500-frame cooldown and authored
off-camera starting-surface gate, reinitializing the Strider with three HP.
The first isolated implementation kept traversal at one HP non-lethal to avoid
introducing a midair death/route contract before it had been reviewed.

MrDig rejected the first Phase 5F.4 visual: procedurally compressing the idle
looked less deliberate than the beetle death, and the third traversal hit was
still non-lethal. Inspection confirmed that the preserved 64x56 production
proof contains four genuine authored destruction cells, whereas the later
64x64 production-idle sheet has placeholders in those positions. Runtime slots
24..27 now crop those four proof cells, re-quantize them through current
FRONT16, ground them on the row-62 baseline and mirror the final indexed result.
A third hit is lethal in every state; during traversal it clears the active
link/state and velocity, then starts the burst at the current world position.
Authored link data and renderer code are unchanged.

MrDig's supplied 10:00 FS-UAE recording accepted the stronger destruction
silhouettes but exposed a second kind of palette inconsistency: `FRONT16` is a
shared bank, not a per-character identity palette. Generic nearest-colour
conversion mapped the warm legacy explosion source to Sparkpaw-orange pens 2
and 3. Death used `{1:982, 2:291, 3:117, 4:33, 5:81, 6:76, 7:76}` while accepted
walk uses no pens 2/3. The destruction conversion now explicitly remaps pen 2
to dark violet 12, pen 3 to bright violet 14 and pen 4 to white 11. More
importantly, runtime generation asserts that no Strider cell in any family may
contain shared-bank orange pens 2 or 3. This turns visual identity from an
assumption into a build-time contract; shape, timing and renderer remain
unchanged. Focused FS-UAE colour review remains pending.

MrDig correctly rejected the remapped follow-up as still too purple compared
with walk and asked for one consistent creation basis. The deeper correction is
to stop treating a shared palette bank as an art-family contract. The accepted
indexed idle/walk cell is now the Strider visual master. A new high-resolution
four-beat concept was generated from the accepted walk reference to establish
fracture, cyan rupture, controlled breakup and debris, but it was deliberately
not used as runtime anatomy because it reinterpreted proportions. Runtime death
instead copies and spatially separates the exact accepted idle pixels; only
existing Strider pen 6/11/14 sparks are added.

The resulting walk frames measure roughly 75-77% neutral steel/charcoal,
18-20% violet and 5-6% cyan. Death frames retain approximately 75% neutral,
18-23% violet and 2-7% cyan as the debris thins. Generation now enforces both
no orange pens 2/3 and `violet <= neutral` for every Strider cell, including
future shoot, hurt, traversal and death work. The project-bound concept source
is preserved as
`assets/enemies/clockwork-storm-strider-death-master-concept-v1-chroma.png`;
the final runtime remains deterministic indexed art from the accepted master.
Focused FS-UAE review is pending.

MrDig accepted the final walk-master visual basis on 14 August 2026 and asked
to checkpoint Phase 5F.3/5F.4 before starting the ADF optimization sidestep.
This acceptance covers the final generated preview and deterministic indexed
art contract. The supplied 10:00 FS-UAE recording verifies the death lifecycle
and improved silhouettes but predates the final colour correction, so no
final-colour FS-UAE or real-hardware verification is claimed.

The initial attempt to keep 24 total cells by shortening accepted hurt and
reusing 14..17 was rejected before review: MrDig explicitly prioritized the
best append-only content contract over current ADF capacity. Native `make`
passes with 28 frames. As expected, `make release` reaches the DOS1/FFS image
and fails with `No Free Blocks` while writing `storm-rear.spbm`; no successful
new ADF is claimed. The next engineering step after gameplay acceptance is
Stage A from `docs/ADF_STORAGE_STRATEGY.md`, preserving normal HD assets while
measuring packed sizes, CRCs and projected FFS blocks before any ADF loader is
implemented.

The ADF optimization sidestep begins with Stage A only. A new
`make adf-report` target measures the exact release executable, runtime files,
ReadMe and startup sequence without changing any package or Amiga code. It
records raw byte size and CRC32, independently compresses each file with host
zlib-9 and LZMA-9/XZ proxies, verifies both decoded streams byte-for-byte and
projects DOS1 file header, 512-byte data and 72-pointer extension blocks. The
machine-readable JSON and readable Markdown outputs live under ignored
`build/adf-report/`; the HD ZIP/LHA layout and loader remain unchanged.

The first post-Phase-5F.4 report measures 877,595 raw payload bytes and 1,775
total projected filesystem blocks, 15 beyond the 1,760-block DD image. zlib-9
projects 478 total blocks and LZMA-9/XZ 409, demonstrating ample capacity but
not selecting either as an Amiga codec. `storm-front.spbm`, `storm-rear.spbm`
and the Strider cache show the strongest immediate generic-compression value;
the player cache and title remain comparatively expensive. Pinned native codec
comparisons, stock-68020 decode timing and peak Chip/Fast memory are explicitly
still open; desktop proxy timing is not accepted as hardware evidence.

The first reversible Stage B proof then targets only the most repetitive large
asset, `storm-front.spbm`. A deterministic project-owned SPR1 byte-run packer
reduces it from 163,900 to 7,689 bytes. A separately compiled ADF executable
opens `storm-front.spr1`; the HD executable and ZIP/LHA continue to open the
ordinary SPBM. The ADF decoder uses a 512-byte input buffer and writes SPBM
header/palette/plane data directly into the final allocations, avoiding a
second complete raw foreground buffer. It rejects bad magic, truncation,
output overflow, size mismatch, trailing commands and CRC32 mismatch.

`make release` now succeeds again. Its DOS1 inspection reports 1,466 blocks
used (750,592 bytes) and 294 free. Release verification extracts the ADF-only
executable and packed foreground, confirms their exact staged bytes, decodes
SPR1 on the host and compares it byte-for-byte with the canonical HD asset. A
separate ZIP inspection confirms the HD executable and `storm-front.spbm`
remain byte-identical and no SPR1 is present. Host round-trip, boundary and
corruption tests pass. No FS-UAE or real-hardware result is claimed; cold-boot,
title/loading/charging flow and all-five-screen foreground parity are now the
required supplied ADF test before another asset is packed.

MrDig reported that the supplied foreground-packed ADF works correctly. This
accepts the first Stage B proof from user-supplied ADF testing; no real-hardware
result is inferred. The next isolated build reuses the unchanged SPR1 stream
decoder for `storm-rear.spbm`, reducing it from 122,916 to 30,165 bytes. The
rebuilt ADF succeeds with 1,281 blocks used and 479 free. Release extraction and
byte-for-byte host decode verification pass for both world assets, while HD ZIP
parity remains unchanged. Focused supplied ADF review must now confirm the rear
layer and its accepted quarter-speed parallax before another asset family is
added.

MrDig reported that the supplied foreground-plus-rear ADF works correctly,
accepting the second SPR1 proof. The third isolated proof then targets the
Strider cache, the largest remaining gameplay family with useful byte-run
compression. `clockwork-storm-strider.spbm` reduces from 143,420 to 87,914
bytes. The unchanged streaming decoder writes it into the final allocated
bitmap/mask storage and verifies CRC32. The rebuilt ADF uses 1,171 blocks and
leaves 589 free; host extraction and byte-for-byte comparison pass for all
three packed assets. Supplied ADF review is now required across every Strider
state. No real-hardware verification is claimed.

MrDig reported that the supplied three-asset ADF works correctly, accepting the
packed Strider cache in addition to both world layers. This leaves ADF Stage B
at a safe user-tested checkpoint with 589 free blocks; no real-hardware result
is inferred.

The next isolated gameplay/audio step adds one original shared enemy-death cue.
The supplied `thundercats-level1.mov` is used only as a reference for a short,
weighty kill punctuation and broader gameplay inspiration; no sample is copied.
Sparkpaw synthesizes a roughly 0.24-second low mechanical drop, shell-break
noise and small metallic tail. Projectile hit dispatch now distinguishes miss,
non-lethal hit and lethal hit. A lethal beetle second hit or Strider third hit
plays death priority 8 instead of the ordinary priority-6 hit-pop, preventing
two effects from stacking on Paula channel 1. Player hurt priority 9 can still
interrupt it; death can replace Strider fire priority 7 and lower effects. The
sound triggers once when death begins, not during later destruction frames or
respawn. Renderer, animation timing, HP and collision remain unchanged.

MrDig accepted the shared beetle/Strider death cue as-is in supplied testing.
No real-hardware result is inferred.

Release naming is consolidated at this checkpoint. One central
`RELEASE_VERSION` now produces `Sparkpaw-0.5.0-alpha.1.adf`, `.lha`, `.zip` and
`Sparkpaw-0.5.0-alpha.1-Source.zip`; A1200 remains a documented requirement but
is omitted from filenames. Before packaging, the release tool removes older
`Sparkpaw-*` artifacts from ignored `dist/`, preventing milestone and SemVer
sets from being mixed. This cleanup does not touch backups or test evidence.
The SemVer minor follows the broad Phase 5 roadmap; the precise accepted
Phase 5F.4 plus in-progress ADF Stage B checkpoint is recorded in release
documentation rather than encoded into the filename.

Appending six full 64x64 masked source frames exceeded the nearly full DOS0
release disk during `make release`. The bootable ADF now uses DOS1/FFS, which is
native to the A1200 target and stores file data more efficiently; its executable
and runtime payload remain identical to the HD archives. The release tool still
verifies the 901120-byte image, boot block, executable and startup sequence.

Phase 5A and 5B were first implemented as an asset-only, reviewable step. The
initial runtime contract used a 48x40 cell, eighteen frames and three HP. Frame
order is alert idle, weight shift, four run phases, two compression phases,
launch/flight/descent, landing/recovery, hit recoil and four destruction
stages. The right-facing production source is
`assets/enemies/clockwork-storm-strider-production-v2-transparent.png`; its
exact frame contract is recorded in
`assets/enemies/clockwork-storm-strider-animations.json`. Generation applies
one anatomical scale to frames 0-13, allows only oversize destruction stages
to shrink, preserves grounded foot anchoring, creates exact mirrored indexed
cells and maps into the existing eight-colour foreground palette. Review the
native palette proof in
`assets/enemies/clockwork-storm-strider-48x40-aga8.png`; the generated packed
proof is `assets/runtime/clockwork-storm-strider.spbm` (96x720, two directional
columns, 3 bitplanes plus mask). These v2 files remain preserved as the rejected
small quadruped review baseline. At the end of 5B there was deliberately no
runtime consumer, spawn record, AI, collision or renderer dispatch for it.

Phase 5C.1 is implemented as a data-model-only step. Level capacity is now nine
persistent typed encounters: the accepted four required plus two optional
beetles, followed by two required plus one optional Strider. Optional counts
are randomized independently per enemy type, yielding 4-6 beetles and 2-3
Striders without changing the established beetle RNG sequence. Every record
owns an explicit authored `{left,right,groundY}` patrol surface; runtime Bob-top
Y is derived from the type height, so raised-platform placement no longer
depends on beetle dimensions. The Strider records use authored ground lines at
Y=128 (raised), Y=208 (floor) and Y=160 (raised). Empty active slots choose eligible visible encounters
first, then encounters ahead of the camera, then those behind it, independent
of enemy type. Existing off-screen unloading still prevents a visible actor
from being parked.

In the accepted 5C.1 build, only `ENEMY_TYPE_CLOCKWORK_BEETLE` was marked
runtime-ready. The selected Strider states remained persistent level data but
could not enter an active slot; no Strider asset was loaded or drawn, and
beetle rendering, collision and animation remained unchanged.

Phase 5C.2 is implemented and awaits MrDig's FS-UAE review. Gameplay loading
now includes `clockwork-storm-strider.spbm`, and the HD/ZIP/LHA/ADF packaging
includes the same asset. The renderer owns a compact per-type Bob-cache
descriptor for source asset, dimensions, frames, source words and authored
direction order. Beetles retain 32x24, nine frames and three source words;
the first 5C.2 proof used a 48x40 Strider and four source words, while the
corrected upright proof uses 64x64 and five. Both types are converted once into
shared Chip-RAM mask/plane patterns. Strider's right-first source is normalized
to the same logical left/right cache order as beetle's left-first source.

The synchronized Bob pipeline remains reverse restore then forward draw, with
the accepted `0x0fca` masked minterm and no CPU compositing into displayed Chip
RAM. Each slot records `drawnType` independently from its current actor type,
so life-loss/right-edge resets restore the exact old type-specific rectangle
before slot reuse. Culling, bounds and restore dimensions dispatch through the
same type descriptor.

Only the two required Strider encounters may enter runtime slots in this proof.
The first is static idle on the raised platform at groundY=128; the second is
static idle on the floor at groundY=208. The optional third remains gated.
`updateEnemy()` freezes Striders at frame zero, and projectile/contact dispatch
continues to accept beetles only. Therefore the two proofs are intentionally
non-interactive and isolate loading, rendering, culling, grounding, overlap and
reset restoration. Phase 5C.3 must add Strider patrol cadence, all-posture
projectile collision, three-HP hit/death handling, contact damage and unlimited
safe respawn only after this static render proof is accepted.

MrDig's FS-UAE recording `2026-08-12 17-58-10.mov` rejected two aspects of the
first 5C.2 proof. The second Strider appeared after it had already entered the
viewport, together with a newly activated beetle, because four slots could
remain owned by older encounters until their patrol surfaces were 160 pixels
off screen. More importantly, the 48x40 quadruped source used only roughly
25-30 visible pixels in idle because its long flight pose dictated the shared
scale. It read as a small mechanical dog, smaller than Sparkpaw, rather than a
larger second enemy.

The accepted redesign is permanently upright and bipedal. Canine/jackal cues
are limited to its swept sensor head and stabilizer tail; it runs, compresses,
jumps and lands on two reverse-jointed piston legs, with short balance arms
never touching the floor. The accepted concept sources are
`clockwork-storm-strider-biped-concept-v2-{chroma,transparent}.png`; the new
eighteen-pose production sources are
`clockwork-storm-strider-biped-production-v4-{chroma,transparent}.png`. The
runtime cell is now 64x64, with about 48-50 visible idle pixels so it reads at
least as tall as Sparkpaw and far larger than a beetle. Its five-word packed
row supports a shifted 64-pixel Bob; the eight-colour proof is
`clockwork-storm-strider-64x64-aga8.png`.

The late-placement correction reduces invisible slot retention from 160 to 32
pixels and moves the floor proof from x=700 to x=870. The raised proof remains
on groundY=128 with its spawn range tightened so the wider Bob stays within the
authored surface; the floor proof remains on groundY=208. A prior quadruped v3
scale experiment is preserved as rejected review history and is not consumed.
This corrected 5C.2 build still freezes both Striders at idle and keeps them
non-interactive; it requires a new FS-UAE review before 5C.3.

MrDig's next FS-UAE screenshot accepted the corrected scale, grounding and
activation timing: the upright Strider is now clearly larger, fully visible
and the second proof has no observed pop-in. The remaining rejection concerned
art direction. Sparkpaw, beetles and Strider all appeared orange because the
v4 aged-brass source mapped into the same warm foreground pens; the thin,
high-frequency mechanical detail also remained below Sparkpaw's perceived art
quality.

The v5 production repaint preserves all eighteen bipedal poses and the 64x64
contract but gives the Strider a separate cool identity: deep navy/black steel,
large violet armour panels, royal-blue secondary plates and cyan visor/core/
conduits, with no warm metal. It also thickens limbs and armour masses and
consolidates highlights into more deliberate pixel clusters. This uses only the
existing foreground palette's dark, blue, violet, cyan and rare cream pens, so
it requires no Copper, colour-depth or playfield changes. The generator now
consumes `clockwork-storm-strider-biped-production-v5-transparent.png`; v4 is
preserved as the accepted anatomy/scale baseline. The updated native proof
still writes `clockwork-storm-strider-64x64-aga8.png`. This v5 colour/polish
build awaits FS-UAE judgement specifically for silhouette readability against
the dark level and perceived quality beside Sparkpaw.

MrDig approved a reversible AGA 4+3 dual-playfield experiment after a focused
research pass. Important correction: the earlier three-planes-per-playfield
limit applies to OCS/ECS; AGA Lisa officially supports up to four planes per
playfield and `BPLCON3.PF2OF` offsets the second playfield into a separate AGA
palette range. Before experimentation, the complete working 3+3 Phase 5C.2
state was committed and pushed to `main` as `26b281a` (`Add Sparkpaw static
Strider render proof`). Recovery means switching back to `main`; no destructive
rollback or ignored-backup manipulation is required.

The experiment lives only on branch
`codex/sparkpaw-aga-4plus3-renderbench`. Its first rb14 step changes only
`src/aga_renderbench.c`: a 672x256 four-plane foreground plus three-plane rear,
seven interleaved Copper pointers, `BPLCON0=$7600`, `PF2OF=16`, foreground pens
0-15, rear pens 16-23 and unchanged independent 1:1/1:4 scrolling. Foreground
test platforms deliberately use pens 8-15 to prove the seventh bitplane. The
ordinary `sparkpaw` game executable and production `renderer.c` remain the
accepted 3+3 implementation. Run `make bench` and test `sparkpaw-renderbench`
separately before migrating any gameplay renderer or assets.

MrDig's first rb14 FS-UAE run reached an ExecLibrary `DEADEND` software failure:
`Corrupt memory list detected in FreeMem`. The repository's existing
`renderbench.log` was timestamped 4 August and identifies rb13, so it is stale;
the bench writes its log only during normal cleanup and rb14 did not produce a
new valid log. Root cause is deterministic: rb14 added a seventh pointer and 64
AGA colour-register moves but retained rb13's 128-word Copper allocation, so
`buildCopper()` wrote past it and damaged the neighbouring Exec allocation
metadata. Rb15 raises `COP_WORDS` to 256, bounds-checks every ordinary/pointer/
terminator emission, aborts before takeover if overflow is detected, and logs
actual Copper words, capacity and overflow state. Do not use the old rb13 log
as evidence for rb15; request the newly written `renderbench.log` after a clean
rb15 exit.

MrDig tested rb15 in FS-UAE and supplied `2026-08-12 18-47-44.mov` plus the
new `renderbench.log`. The recording shows a stable full-height display,
independent full-speed foreground and quarter-speed rear motion, foreground
pens above seven, and a clean return to Workbench. The log identifies
`2026-08-12-rb15-aga-4plus3-copper-bounds`, reports `result=clean-exit`,
`depth=4+3`, `copper_words=192 capacity=256 overflow=0`, 457 frames and restored
DMA/interrupt/view state. This is user-provided FS-UAE acceptance of the basic
4+3 fetch, palette-offset, scrolling and restoration proof; it is not real
hardware verification. The next isolated bench should use representative
gameplay foreground/rear colours and one 15-colour Strider idle before any
production renderer migration.

Rb16 implements that isolated art proof without modifying the production game.
`tools/generate_renderbench_assets.py` extracts only the accepted v5 idle,
scales it into a 64x64 cell, maps it to a dedicated cool 15-colour foreground
palette and writes a four-plane-plus-mask 2560-byte raw bench asset plus
`clockwork-storm-strider-64x64-aga15-idle.png`. The bench loads and places that
idle once at world x=286/y=144 before display takeover. This setup-only CPU
write never touches a displayed bitmap; a future gameplay migration must keep
the synchronized packed Blitter pipeline. Rb16's rear uses representative
Storm Ruins colours at PF2 offset 16, foreground structures use the new cool
bank, and the Strider scrolls with PF1 while PF2 stays quarter-speed. The log
adds `strider_idle=aga15 loaded=1 size=64x64`. This remains a colour-depth/art
proof only: no dynamic Bob, player, HUD, collision or gameplay renderer change.

#### Phase 6: levels and progression

Once two enemy types, player damage and respawn are stable, move patrol/spawn
placements out of `prepare()` into generated level/entity data. Add progression,
checkpoints, collectables and broader screen flow on that foundation. Music
integration should first define Paula channel ownership so rapid plasma,
player/enemy impacts and menu sounds are not arbitrarily interrupted.

The agreed first-level direction is an original, polished left-to-right Storm
Ruins action-platforming flow: alternate readable enemy encounters with safe
breathing space, raised platforms and jumps across water or open gaps. Broad
inspiration may come from the simple forward pacing of classic Amiga games,
but do not copy their maps, characters, enemy designs, art or timing. The
visual target remains the project concept: violet storm mountains and clouds
in the far parallax, pine forest/waterfalls/broken energy towers in the rear
layer, and mossy stone-machine ruins with cyan conduits in the foreground.

The agreed length reference is the first level in the supplied Thundercats
Amiga video, which runs from about 3:32 to 4:10 in that recording: roughly 38
seconds including one player death. This is a pacing reference only; do not
copy its map, encounters, art or timing. Target Sparkpaw's first level at about
35-50 seconds for a practiced run and roughly one to two minutes for a first
careful playthrough. Begin by measuring and greyboxing an eight-screen,
2048-pixel world rather than the previously discussed 12-15 screens. Eight
screens are 60 percent longer than the current five-screen/1280-pixel test
world and are expected to add roughly 221 KiB of Chip RAM across the resident
foreground source, foreground display buffer and rear bitmap; verify actual
free and largest Chip blocks before accepting that layout.

Phase 6A replaces that rough estimate with an isolated measurable build. A
shared compile-time world-width constant defaults production to 1280; only
`make phase6-memory` compiles 2048 and packages repeated copies of the current
foreground, rear and collision data under ignored
`build/test/Sparkpaw-Phase6A-2048/`. It
does not alter the production level, routes, renderer timing or release files.
The test writes `phase6-memory.log` before hardware takeover after gameplay
assets, display bitmap, HUD, sprites, Bob caches and Copper have prepared.

The exact host allocation delta is 98,304 bytes for `frontClean`, 73,728 for
`rearWorld`, 98,304 for `frontDisplay` and 672 collision bytes. The three bitmap
allocations therefore add 270,336 bytes, not the earlier rough 221 KiB. Host
asset dimensions and package generation pass. Phase 6A remains in review until
MrDig supplies the log from exactly 2 MB Chip/no Fast and confirms the repeated
2048px route reaches its right edge without allocation failure or corruption.

MrDig ran the directly mounted HD drawer in FS-UAE with exactly 2 MB Chip and
no Fast RAM, reached the repeated-art right edge and reset after gameplay. The
generated log records 1,491,656 bytes free (largest 1,424,488) before gameplay
assets, then only 88,136 bytes free with an 86,816-byte largest block after
complete preparation. At that peak the charging bitmap and two title Copper
lists are still live; their later release returns approximately 61,440 plus
1,360 bytes, implying only about 150 KiB steady free and not proving a suitably
large contiguous block. The experiment therefore succeeds technically and
establishes a useful 2 MB/no-Fast stress baseline. Disk compression cannot
change these final bitmap allocations. MrDig subsequently clarified that 2 MB
Chip plus 8 MB Fast is an acceptable production minimum, while explicitly
requiring continued optimization rather than treating Fast capacity as license
for waste. Any rejection or architecture choice is deferred pending a complete
Chip/Fast audit. Initial source inspection found approximately 325 KiB of
player, beetle, Strider and collectible source sheets retained in Chip after
their definitive DMA caches are built, making the earlier steady estimate
knowingly pessimistic. A newly instrumented build must measure peak and
post-conversion Chip/Fast free and largest blocks before resident versus
segmented design is decided. No real-hardware result is claimed.

Phase 6A.2 begins that audit with a lifecycle-only change. The renderer builds
the accepted six-channel player streams and packed Bob caches in the same order
and in Chip memory, then finishes the Copper list while the player palette is
still available. It subsequently releases the CPU-read conversion sheets:
172,800 bytes for the player, 8,640 for the beetle, 143,360 for the Strider and
420 for the diamond, exactly 325,220 bytes total. No display bitmap, gameplay
Blitter source, Copper list, Paula sample, route or gameplay contract changed.
The Phase 6 log now captures Chip and Fast free/largest at the pre-release peak
and after preparation. Host builds and both release variants pass; supplied
FS-UAE retesting remains required.

MrDig then supplied the 2 MB Chip plus 8 MB Fast FS-UAE result: the Phase 6A.2
test build runs correctly and reaches the repeated-art right edge. Its log
records an unchanged pre-release conversion peak of 88,136 bytes Chip free and
an 86,976-byte largest block, with 6,674,368 bytes Fast free. After source
release it records 413,696 bytes Chip free and a 281,600-byte largest block,
recovering 325,560 bytes including allocator/bitmap bookkeeping; Fast remains
6,674,296 bytes free. The resident 2048px world is therefore accepted as a
viable Phase 6 greybox basis, but not as a fixed final level length. The narrow
conversion peak remains the next memory-hygiene target before significant new
animation or music is added. No real-hardware result is claimed.

The first Fast attempt removed `BMF_DISPLAYABLE`, which improved the next
supplied successful test to 271,272 bytes Chip free at the conversion peak and
531,464 steady, but the delta showed that only the approximately 65 KiB masks
had moved: native planar `AllocBitMap` still selected Chip for the planes.
Phase 6A.2 therefore introduced an explicit CPU-only bitmap representation:
the descriptor, planes and masks use `MEMF_ANY`, while every display, Copper,
hardware-sprite, Paula and gameplay-Blitter allocation remains Chip. The final
supplied 2 MB Chip plus 8 MB Fast test runs correctly and reaches the right
edge. It records 531,464 bytes Chip free and a 530,408-byte largest block both
before and after conversion-source release, proving the Chip loading spike is
gone. Fast rises from 6,349,280 to 6,674,416 bytes, returning 325,136 bytes.
Phase 6A is complete; 2048px is accepted as the Phase 6B greybox basis without
fixing the eventual level duration or width. No real-hardware result is claimed.

Phase 6B.1 then promotes the validated 2048px width to production without a
renderer change. The generator preserves all geometry in x=0..1279 and appends
five greybox platform sections plus three short columns in x=1280..2047. Four
new beetle candidates use authored surfaces in that extension (three required,
one optional), while `MAX_ENEMIES` remains four and camera activation/parking
continues to bound simultaneous Bobs. Twelve diamonds extend route guidance and
reward pacing. Existing Strider candidates, surfaces, traversal links and the
third-Strider runtime gate are unchanged. The floor remains continuous so this
step does not silently introduce water/death or respawn semantics; those belong
to 6B.2 and visual water remains a later renderer/asset step. Generated front/
rear assets validate as 2048x256, collision as 128x14. Standard build, memory
regression and release pass; the packed ADF uses 1,229 blocks. Supplied FS-UAE
pacing/geometry review remains required.

MrDig's supplied screenshot and `2026-08-14 15-41-27.mov` then proved that the
new x=1664/1704/1744 diamond trail contained persistent split fragments while
Sparkpaw stood still and did not fire. This rejected the initial interpretation
that the cyan fragments were plasma frames. Source correlation isolated x=1704
as the first non-16px-aligned collectible: a shifted 16px Blit requires two
source words, but the diamond cache provided only one and used a negative source
modulo. The focused fix pads every diamond mask/plane row to two words and passes
that stride to the unchanged cookie-cut routine. It costs 210 Chip bytes and
preserves restore/draw order, timing and free placement. Host builds and release
pass; supplied FS-UAE retest remains required.

The supplied `2026-08-14 15-46-37.mov` revealed a second 6B.1 interaction: near
the end, Sparkpaw fires while jumping and the plasma visibly crosses a beetle on
a raised platform without damage; crouching on that platform hits normally.
The collision function gated all beetle geometry behind the original `lowShot`
boolean, so the new vertical encounter exposed a rule that was too broad. The
focused change marks crouched and airborne launches as beetle-capable while
retaining the exact existing projectile point and beetle hitbox test. Ordinary
grounded standing shots still miss floor beetles; airborne shots only hit when
their world-space Y actually crosses the beetle. Builds and release pass;
supplied FS-UAE retest remains required.

MrDig's supplied follow-up testing accepted both focused fixes: the extended
diamond trail remains complete, and an airborne shot that geometrically crosses
the raised-platform beetle registers correctly. Phase 6B.1 is accepted. The
next isolated step is 6B.2: one mechanically testable water region using a
greybox floor interruption and explicit death/restart semantics. Visual water
work must begin with concept art and remain separate from the hazard logic. No
real-hardware result is claimed.

Phase 6B.2 implements only water mechanics. The generator removes five floor
tiles at x=1584..1663 and the next x=1664 low platform supplies the far landing.
Level data owns the water bounds and y=224 death threshold. Collision remains
solid below the gameplay area everywhere else, but returns open space below
those water columns so the player can genuinely fall. Once the player contact
box reaches the threshold, gameplay removes one life and reuses the accepted
in-memory restart, including its preserved HUD diamond total. No renderer,
palette, water art, splash, audio, enemy route or Strider contract changed.
Standard and ADF builds pass; supplied FS-UAE review remains required.

MrDig's latest supplied movie accepted the Phase 6B.2 mechanical outcome:
Sparkpaw falls through the authored opening and the level restarts. During the
same review he reported a separate boundary defect, especially pronounced on a
real Amiga in both ADF and HD builds: the intended black line between world and
HUD appears intermittently contaminated by adjacent colour. The HUD bitmap was
verified to contain only two all-zero separator rows before its detailed metal
frame begins; moving world/rear pixels occupy the immediately preceding line.
The first asset-only correction increased the band to four all-zero rows, but
supplied FS-UAE footage showed more moving coloured pixels rather than an
improvement. That experiment was therefore rejected and reverted to the
accepted two-row height. Because HUD pen 0 is transparent to hardware sprites,
the focused follow-up fills both separator rows across the full fetched width
with non-zero dark HUD pen 1. The HUD remains 48px high and the accepted
line-252 Copper switch and line-253 Bob pass are unchanged. Supplied FS-UAE and
real-hardware retest initially remained required. MrDig subsequently verified
the two-row opaque-pen correction in FS-UAE and on a real Amiga. The formerly
moving/glitchy boundary pixels are fixed; this is the accepted baseline.

Release naming is advanced with the broad roadmap phase. The Phase 6 release
line is `0.6.0-alpha.1`, replacing the earlier Phase-5-aligned
`0.5.0-alpha.1`. `tools/make_release.py` remains the single version source and
now interpolates that value into the packaged drawer instructions as well as
all artifact names. The SemVer minor follows the broad numbered phase; the
precise lettered checkpoint, currently Phase 6B.2, remains in README and
packaged release notes. Later meaningful packaged checkpoints within Phase 6
increment the prerelease counter. Older `Sparkpaw-*` files in ignored `dist/`
are still removed only by the release packager.

Release identity is now enforced rather than relying only on session discipline.
The packager owns both `RELEASE_VERSION` and `ROADMAP_CHECKPOINT`, interpolates
them into the packaged metadata and rejects a build when the SemVer minor does
not match the numbered roadmap phase. Future roadmap steps must update release
identity as part of the step itself, without waiting for a separate reminder.
A successful `make release` also continues to leave only the current matching
artifact set in `dist/`.

After the earlier supplied acceptance of the two-line opaque HUD boundary,
MrDig reported that he still sees some glitchy behaviour there on a real Amiga.
That later observation supersedes the categorical “fixed” wording but does not
yet identify a cause. The current two-line asset, line-252 HUD switch and
line-253 Bob pass remain unchanged; follow-up is tracked as a separate renderer
regression todo rather than being mixed into Phase 6B.3 gameplay work.

MrDig approved `sparkpaw-water-hazard-concept-v4.png` as the visual basis for
ground and water. Earlier revisions clarified three constraints: use the
full-colour gameplay concept rather than its flatter AGA64 preview; keep the
visible floor mass modest rather than building a tall canal wall; and place
that low ground directly against the HUD rather than leaving a parallax strip
below it. The duplicate smaller diamonds generated in concept v4 are rejected
ImageGen noise, not an asset or placement proposal.

Phase 6B.3 translates only those approved constraints into deterministic level
art. The foreground generator adds an eight-pixel stone/moss cap at y=200..207,
directly above the existing line-252 HUD switch. Across the accepted five-tile
x=1584..1663 opening it draws dark navy storm water, a narrow cyan/white surface
and sparse fixed wave pixels. The 6B.2 collision opening, y=224 death threshold
and restart behaviour remain byte-for-byte in their existing modules. No splash,
audio, renderer, Copper, Bob, collectible or enemy change is included. The
release advances to `0.6.0-alpha.2` / Phase 6B.3. Native and release builds pass;
FS-UAE and real-hardware review remain unclaimed.

MrDig reconfirmed the production minimum as a stock PAL A1200/68020 with 2 MB
Chip and 8 MB Fast. The old 2 MB/no-Fast Phase 6A run remains useful stress
evidence but is not a shipping target and must not drive later renderer choices.

The first runtime screenshot of the thin cap exposed a geometry/art mismatch:
the cap begins at y=200 while the continuous collision floor and floor-owned
enemy surfaces still ended at y=208, visibly sinking actors into the strip.
Phase 6B.3A makes y=200 the continuous non-water floor, changes only authored
groundY=208 surfaces to 200 and moves Sparkpaw's reset Y from 164 to 156. Raised
surfaces and traversal link records remain unchanged. A host fixed-point replay
of all four authored links confirms their landing conditions still succeed at
down x=416, return x=380, gap-right x=1072 and gap-left x=936. Native/ADF build
verification follows; runtime route and grounding review remain unclaimed.

Phase 6B.3B is the isolated renderer follow-up. It replaces the static violet
water fill with sixteen fully blue/cyan/white 80x11 frames held in a 7,040-byte
Chip cache. Review of the supplied runtime movie showed that the first three
separate crests read as isolated roof shapes and that the white particles rose
too synchronously. The revised treatment spans the full 80px opening with one
continuous shallow moving surface. Six bubble tracks have different starting
phases, speeds and rest windows, avoiding a single shared particle beat. The
first eight-frame runtime review exposed a half-period reset jump and crests
occasionally rising above the banks. The final loop covers all sixteen wave
phases, advances every two game ticks and pins its first/last three pixels to
ground height, producing a seamless 25 Hz cycle with clean bank joins.
After projectile erase, enemy
restore and collectible restore, the existing line-253 pass copies the selected
frame with the Blitter into `frontClean` and then `frontDisplay`; only afterward
does it perform the unchanged collectible, enemy and projectile draws. This
keeps next-frame restores synchronized without CPU read-modify-write or a full
foreground duplicate. The update performs eight tiny five-word-by-eleven-row
plane copies only when the animation frame changes. Collision, death/restart,
Copper staging, HUD switch and relative Bob ordering remain unchanged. The
generator writes an exact enlarged palette preview at
`assets/levels/storm-water-animation-aga-preview.png`. Native and packaged
build verification follows; no emulator or hardware result is claimed.

Phase 6B.4 adds the pending impact feedback as two bounded pieces sharing one
event. At bottom y=204, with Sparkpaw's centre inside the 80px opening, gameplay
enters a sixteen-tick water-impact hold. Life is deducted once, the six player
hardware sprites select the existing null sprite and a dedicated four-frame
32x16 cyan/white splash Bob runs before the accepted resident restart. Its
restore precedes the water copy and its draw follows that copy inside the
line-253 pass, so it neither consumes an enemy slot nor changes existing Bob
relative order. The accompanying original 11.025 kHz mono synthetic splash
uses prioritized Paula gameplay channel 1 at priority 10; plasma keeps channel
0. The release advances to `0.6.0-alpha.3` / Phase 6B.4. Runtime behaviour is
not claimed until supplied FS-UAE or hardware evidence exists.

The first supplied 6B.4 sound review found a hard percussive onset before the
more convincing splash tail. The generator therefore removes the low impact
body and instant full-level noise, replacing them with a 45 ms swelling wash,
a separately faded fizz layer and two quiet delayed droplets. Visual timing,
gameplay trigger and Paula ownership are unchanged; runtime review remains open.

Phase 6B.5 adds the optional ledge-balance readability pass. Earlier player-art
history makes this append-only: accepted slots 0..57 remain untouched, while
four same-family poses occupy 58..61 and use the existing exact mirror path.
The generated review strip was chroma-extracted and reduced as one family to
the established 48x48 cell, 15-colour player palette and shared boot baseline.
Runtime selection waits ten stationary grounded ticks and requires support
under exactly one of the x+6/x+25 foot probes; the actor faces the unsupported
side. Existing movement, jump, crouch, shot, turn, hurt and airborne states
retain priority and collision/physics do not change. Release advances to
`0.6.0-alpha.4` / Phase 6B.5; runtime judgement remains unclaimed.

The first supplied FS-UAE review showed the two-point trigger beginning while
too much of Sparkpaw remained on a platform. At the final edge, both probes
were free but the broad horizontal collision still found one last solid pixel;
`grounded` remained true and ordinary front-idle appeared while visibly
suspended. The refinement counts all 24 pixels across the collision sole:
teeter is limited to 4..10 supported pixels and fewer than four releases
grounding so gravity resumes. This changes only extreme-edge support semantics;
full platforms, authored 16px columns and collision dimensions remain intact.

A second supplied clip then exposed a separate semantic false positive: narrow
support inside a gap triggered teeter even though Sparkpaw was directly against
another platform wall on the nominally unsupported side. The selector now also
requires three clear horizontal pixels beside the complete standing collision
height in the missing direction. This suppresses the balance act in wedged or
wall-adjacent positions without moving platforms or broadening collision boxes.

The next FS-UAE screenshots proved that clearing `grounded` after `moveY()` was
insufficient: the following downward step still treated the same 1..3 overlap
pixels as a landing, reset vertical speed and produced a stationary airborne
pose. The four-pixel minimum now lives in downward vertical collision itself;
sub-threshold overlap is traversed, while upward head collision preserves its
established any-pixel rule. This removes the suspension loop at its source.

The following FS-UAE movie showed that Sparkpaw then fell correctly but landed
on the lower floor with the same 1..3px horizontal overlap still embedded in
the ledge wall. The standing-clearance test consequently rejected jump until a
manual sidestep removed it. Weakening `canStand()` would permit corner clipping,
so downward edge release instead resolves x outward by exactly the discarded
support count before continuing. The lower landing is then geometrically clean
and jump eligibility needs no special exception.

MrDig accepted the completed 6B.5 behaviour in supplied FS-UAE testing: teeter
starts sufficiently near a genuine free edge, is suppressed beside an adjacent
platform, sub-threshold support produces a real fall, and the lower landing
allows an immediate jump without a corrective sidestep. No real-hardware result
is claimed. The reusable project skill
`.agents/skills/extend-sparkpaw-animations/` captures the concept-first,
append-only, palette/cache, renderer-order and runtime-regression workflow for
all future player, enemy, collectible, projectile, effect and water animation
families.

MrDig later superseded the fixed 35-50-second target: level 1 should feel much
longer than the current test and should use the supplied ThunderCats level for
forward pacing/progression inspiration. The 2048px/eight-screen build remains
the first resident-memory experiment, not a promise that eight screens or the
old duration is sufficient. Measure a playable greybox before fixing the final
time/width. Additional enemy types are explicitly optional for level 1; route,
height, hazards, breathing space and better placement of the accepted beetles
and Striders should create variety first.

Implement the longer level only after Phase 4 spawn/respawn is stable and the
Phase 5 second enemy works in the current world. Split Phase 6 into reviewable
steps: first a technical 2048-pixel resident-world/memory experiment using
repeated existing graphics, then an eight-screen collision and pacing greybox,
then checkpoint/progression state, encounter placement and finally unique
foreground/rear art. Prefer the fully resident world if measurements leave a
safe 2 MiB Chip-RAM margin; do not introduce seamless disk streaming unless
the resident experiment proves unsuitable. The intended pacing is: safe start,
first beetle, platform/diamond route, second-enemy introduction, brief
breathing space or checkpoint, mixed traversal, stronger encounter and a short
level finale.

Before doubling the current 1280-pixel, five-screen world, build one complete
vertical slice inside it: safe start, beetle contact, one gap, one water jump,
a raised route, the second enemy type, a checkpoint and a final mixed
encounter. Hazards require checkpoint recovery, camera reset and projectile
cleanup; they are not merely new collision-map colours. Once player damage,
checkpoint recovery, generic respawn and two enemy types are stable, measure
whether a ten-screen level remains one resident wide bitmap or should use
generated segments/chunks. Do not double bitplane and collision assets before
making that memory/loading decision.

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
- treat visible Bob count and total restored/drawn area, rather than the total
  number of spawn records, as the primary runtime enemy-performance budget;
  keep fixed pools, camera culling and shared per-type caches when respawning;
- keep the current three-plane/eight-colour gameplay banks until a separate
  4+4 dual-playfield renderbench proves that the extra colour depth, roughly
  one-third additional Bob plane work and extra Chip RAM are affordable.

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
- The four-slot polished beetle runtime pool now feeds four to six persistent
  level encounters with unlimited safe off-screen respawn. Its Phase 4 FS-UAE
  regression is accepted; preserve the proven player, dual-playfield and
  synchronized projectile/enemy Blitter paths during later enemy work.
- There is still only one enemy type and no final game-over, checkpoint,
  music, menus or full level progression. Player damage, the modular HUD,
  invulnerability feedback, keyboard test controls and diamond collectibles
  are implemented and user-tested in FS-UAE.
- The temporary exact-right-edge replay stands in for level completion. Replace
  it with an explicit `LEVEL_COMPLETE -> next level` transition when real
  progression is introduced; do not let the replay path implicitly decide what
  a new level or new game preserves.
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
16. **Program the complete inherited display state.** A direct AGA Copper must
    not assume Workbench and a minimal ADF boot leave the same registers.
    Explicit `DIWHIGH`, `ECSENA`, `KILLEHB`, border blanking and a detached OS
    View were all required for identical 64-colour HD/floppy behaviour.
17. **Stage fades inside the active list's safe window.** Swapping complete
    title Copper lists on every fade frame produced intermittent corrupt
    palettes. Record the palette value-word positions, update them only after
    their current-frame Copper use, and reserve full-list swaps for complete
    screen transitions at VBlank.
18. **Scandoublers expose borders and need lock time.** A CRT can hide a
    one-pixel `COLOR00` edge and does not reproduce an Indivision mode-switch
    delay. Reserve palette pen 0 as black and provide a stable black PAL period
    before fading in; compare CRT and HDMI evidence before moving DIW/DDF.
19. **Name loading phases truthfully.** A quiet floppy does not imply a stalled
    read or decompression. Sparkpaw reads uncompressed SPBM files first, then
    performs silent CPU preparation. Keep `LOADING` through the final read and
    show `CHARGING` only for sprite/Bob/bitplane/Copper preparation.
20. **Ground generated art by opaque pixels, not canvas bounds.** Lanczos can
    leave a transparent final row after resizing. Bottom-align the resulting
    alpha silhouette itself; otherwise a correctly positioned actor can appear
    to hover by one PAL pixel while its physics origin remains correct.
21. **More spawn records are cheap; more visible Bob area is not.** Generic
    respawn and two per-level enemy types can retain fixed pools and shared art
    caches. Performance is governed mainly by simultaneously restored/drawn
    planes and pixels, so cap on-screen concurrency and continue camera culling.
22. **Treat gameplay colour depth as a renderer decision.** A 32x24 three-plane
    Bob cannot reproduce a large RGB concept or Sparkpaw's 15-colour attached
    hardware-sprite richness. Better native clusters and controlled dithering
    are safe art improvements; moving gameplay from 3+3 to 4+4 planes affects
    world memory, display DMA, Copper state and every Bob pass and therefore
    requires a separate measured renderbench milestone.
23. **Temporary states must preserve posture constraints.** Hurt, attack or
    scripted recovery may restrict input, but must not bypass `canStand()` or
    force a standing collision box beneath a low ceiling. Keep logical posture
    valid even when the temporary visual pose is taller than that hitbox.

## Recommended first prompt in a new Codex task

```text
We are continuing the Amiga game workspace in this repository.

First read CODEX_HANDOFF.md completely, then read sparkpaw/README.md and inspect
the current Sparkpaw source before changing anything. Sparkpaw is the active
AGA A1200 project. Check git status, recent commits and tags first; use
sparkpaw-pre-modularisation as the accepted four-beetle baseline.

Preserve the stable dual-playfield renderer, line-100 Copper-list staging,
accepted 50-frame player baseline plus eight appended hurt frames, and the
six-channel hardware-sprite player. Preserve the line-252 HUD display switch
and synchronized line-253 Blitter Bob pass used by plasma projectiles,
collectibles and the generic four-slot enemy pool. Preserve packed planar
caches, camera culling, Bob restore/draw ordering and the accepted beetle and
Strider animation/collision contracts. Strider runtime slots 0..7 are walk,
slot 8 is the endpoint turn, and slots 9..17 remain reserved for later
attack/shooting, hurt/hit and death work. Do not reintroduce CPU
read-modify-write compositing in displayed Chip RAM.

Work in small reviewable steps and do not combine renderer changes with
unrelated gameplay or asset changes. Do not modify or delete ignored local
backups or test evidence. Always run make and make release after implementation.
Do not claim FS-UAE or real-hardware verification unless I provide the result.

My next request is: ...
```

Replace `sparkpaw/README.md` with the relevant game README when continuing
ChipSnake or Futsal instead.
- Latest HUD FS-UAE feedback (2026-08-11): after the 15-pixel fine-scroll
  correction the strip was still perceived about two pixels too far left, and
  the dynamic lives-digit clear rectangle erased part of the life panel's
  bevelled upper-right border.  The generated HUD content now has a two-pixel
  internal X offset and the clear rectangle is restricted to the digit area.
- Follow-up HUD feedback: the lives-digit clear area still clipped a small
  section of the lower inner bevel.  Its bottom is now restricted to logical
  Y=33 and the 1/2/3 glyph is raised two pixels to align with the portrait.
- HUD/playfield separation follow-up: the 48-line HUD now reserves two black
  scanlines above the complete, slightly rescaled metal panel.
- Rejected sprite-clipping experiment: dynamically shortening the six player
  channels' VSTOP at the HUD boundary caused FS-UAE glitches supplied by
  MrDig. Because each cache entry still contains 48 data rows, DMA interpreted
  the unread rows as another sprite header after the shortened stop. The
  clipping was removed immediately; keep the accepted complete 48-row sprite
  stream and its real terminator. Any future hard clip needs correctly packed
  per-height sprite streams, not a control-word-only VSTOP edit.
- MrDig's subsequent FS-UAE test reported no further HUD graphics glitches
  after that VSTOP experiment was removed. This is user-provided emulator
  verification; no real-hardware verification has been performed.
- A user-supplied FS-UAE recording accepted the isolated rb16 AGA 4+3 colour
  proof: the static 64x64 Strider remained coherent while scrolling and the
  display returned cleanly. Its log reported a loaded 4-plane/15-colour pose,
  192 of 256 Copper words and no overflow. This proves colour/display capacity,
  not production dynamic-Bob performance. Rb17 is the next isolated art-only
  candidate: one newly authored, broader indigo/violet/cyan idle pose derived
  from the accepted upright Strider concept. Do not generate an animation sheet
  or migrate the game renderer until this native-size idle is visually accepted.
  MrDig accepted rb17's richer silhouette in FS-UAE but noted that its many
  nearby blue shades still read somewhat monotonously. The apparent loose toe
  below it was confirmed as six isolated pixels on the final source row; rb17a
  reserves a transparent baseline row to remove that scaling residue. A future
  art pass should spend the same 15-colour budget on fewer blue ramp entries
  and stronger secondary violet/steel accents, without changing the renderer.
  The first rb17a screenshot then showed the expected one-pixel visual gap;
  rb17b keeps the clean transparent row but places the static proof at y=145,
  restoring foot contact without reintroducing the residue.
  A closer rb17b screenshot revealed three remaining disconnected clusters on
  source row 62 beneath the continuous soles on row 61. Rb17c clears rows
  62-63 and places the proof at y=146, making row 61 the explicit contact row.
  Rb17c was accepted from a user-supplied FS-UAE screenshot. Commit `25daed8`
  on `codex/sparkpaw-aga-4plus3-renderbench` is the pushed recovery checkpoint.
  Rb18 is the final idle colour-direction proof: retain the rb17c geometry but
  allocate the 15 visible pens to navy shadows, neutral steel, storm violet,
  cyan/ice energy and one magenta warning accent. Validate this native-size
  pose before any 4-plane production migration or animation generation.
  The renderer-only production migration is now in progress on the same
  reversible branch. Its first test build changes the gameplay world from 3+3
  to AGA 4+3: front clean/display assets and all synchronized restore/draw
  pipelines use four planes, rear remains three, the seventh Copper pointer is
  staged before gameplay, PF2 stays offset to pens 16-23, and the line-100 HUD
  stage points the fourth PF1 plane at the existing blank plane. Existing
  beetle/plasma/diamond pens 0-7, pool sizes, ordering, culling, collisions and
  animations are intentionally unchanged. The production Copper allocation is
  raised from 320 to 512 words and palette bank zero is explicitly restored
  before loading the attached hardware-sprite palette. Do not call this
  accepted until MrDig supplies FS-UAE results; no real-hardware test exists.
  MrDig's first FS-UAE production screenshot showed stable gameplay/Bobs/HUD but
  a substantially recoloured rear layer. This is not an acceptable inherent
  4+3 side effect. Audit found that PF2OF=16 correctly selects entries 16-23,
  while the Copper had mistakenly loaded rear colours into AGA bank 1 entries
  32-47. It also left attached sprites sharing entries 16-31 with PF2. The
  correction loads PF1/PF2 together into bank-zero entries 0-31, sets BPLCON4
  to `$0022`, and loads Sparkpaw into bank-one entries 32-47. Retest background,
  player colours and HUD in FS-UAE before accepting the migration.
  MrDig's follow-up FS-UAE screenshot accepted that correction: the original
  blue Storm Ruins rear palette, Sparkpaw hardware-sprite palette, HUD and
  existing beetle/diamond/Bob gameplay all appeared correct while running the
  4+3 production renderer. This accepts the renderer-only migration in FS-UAE;
  no real-hardware verification has been performed. Keep it as a separate
  checkpoint before introducing the rb18 Strider idle into production caches.
  After fast-forwarding and pushing the accepted renderer to `main` at
  `c6abb82`, continue the rb18 production-idle proof on
  `codex/sparkpaw-rb18-strider-production-idle`. It replaces only the visual
  contents of the existing 18-frame, two-facing 64x64 packed Strider sheet:
  every slot repeats the accepted rb18 idle remapped into shared FRONT16.
  Frame count, cache dimensions, static AI, placements, culling, restore/draw
  order and collision remain unchanged. Do not author animation in this step.
  In the first production-idle FS-UAE movie, MrDig reported possible glitches
  during close Sparkpaw/Strider overlap and visible hovering. Frame inspection
  found no persistent Bob rectangles and confirmed BPLCON2 `$0024` already
  places all six player sprite channels ahead of PF1/PF2, so do not change the
  stable priority register on that evidence. The hover is measurable: rb18's
  last opaque row is 61 while rows 62-63 are transparent inside the retained
  64px collision cell. Draw Striders at logical y+2 only; restore uses stored
  drawnY, while physics, placements and collision remain at logical y.
  The follow-up movie/stills made the first-approach corruption reproducible:
  a fragment at the Strider's upper-left appears after initialization/reload,
  disappears after unload/revisit, and returns after the next reload. Root cause
  is runtime-slot handoff in `activateVisibleSpawns()`: it preserved the prior
  Bob's drawn flag/X/Y across slot assignment but lost `drawnType`, so a pending
  64x64 Strider restore could use the replacement spawn's default 32x24 beetle
  cache. Preserve old `drawnType` alongside X/Y exactly as the level-reset path
  already does. This keeps restore dimensions tied to what was actually drawn.
  MrDig confirmed the corrected first pass, unload/revisit and level-reload
  cases in FS-UAE. Commit `26a2510` (`Integrate premium Strider production
  idle`) is merged and pushed on `main`; no real-hardware verification exists.

### Strider traversal target (ThunderCats level-1 reference)

- The intended large-enemy behaviour is not beetle-style local decoration.
  A Strider is a persistent traversal threat: once engaged it may walk through
  the complete visible viewport and continue beyond either edge. Simply jumping
  over it must not kill, despawn, freeze or reset it. The player may defeat it
  or use level geometry/platforms to pass it while it continues its route.
- Later navigation should let Striders jump over gaps/water and between valid
  surfaces in the spirit of the larger enemies in the referenced ThunderCats
  Amiga level 1. This is a world-space AI/path decision, not a camera-bound
  animation trigger. Preserve simulation outside the visible 320px viewport;
  camera culling may skip Bob drawing but must not itself stop or reverse AI.
- Level 1 must eventually contain real discontinuities in the ground: open gaps
  and/or visible water hazards rather than one continuous safe floor. Those
  hazards must be represented in collision/level data, not faked as decoration.
  Sparkpaw must traverse them through authored platforming, while Strider AI
  must detect an unsafe leading edge and later choose a valid jump trajectory;
  neither actor may walk invisibly across water or unsupported empty space.
- Do not implement that complete navigation in the first animation step. First
  author and accept a four-frame grounded walk cycle on the existing raised
  platform, with stable foot baseline and animation distance coupled to actual
  velocity. Then allow continuous patrol through the viewport without automatic
  defeat. Add edge/gap sensing and authored jump arcs as a separate later phase.
- The first four-frame production walk candidate is now wired into Strider
  frame slots 0..3. All four poses use one family scale and opaque-row baseline;
  frames 4..17 deliberately retain the accepted idle until their own art pass.
  The old static-AI return is removed, so the existing fixed-point patrol code
  supplies randomized speed, distance-coupled animation and safe reversal at
  current platform edges. This is only the grounded animation/patrol proof:
  it does not yet implement persistent offscreen traversal or jumping gaps and
  water. Require an FS-UAE animation/grounding regression before acceptance.
  The first FS-UAE movies showed a disconnected fragment beside the hand in
  the generated walk source and an over-busy cadence at the fastest randomized
  velocity. Clean each chroma-keyed pose to its principal connected silhouette
  and use a Strider-specific 768-subpixel frame distance (beetles retain 384).
  This changes neither Strider velocity nor the renderer/cache contract.
  The corrected movie confirmed that cleanup and slower cadence, but also made
  the underlying four-pose gait defect clear: one leg repeatedly led while the
  other merely joined it. Replace slots 0..3 with a complete eight-frame
  alternating cycle in slots 0..7: right contact/down/pass/toe-off followed by
  left contact/down/pass/toe-off. Feet and knees must visibly cross during both
  passing phases. Slots 8..17 retain idle and the packed 18-frame contract.
  The next FS-UAE movie accepted the gait improvement but exposed two proof-data
  limitations. The authored patrol ranges were only 80px and 88px wide for a
  64px actor, leaving merely 16px/24px of travel. Expand the first raised route
  to its complete platform and the second to the long safe continuous floor,
  while using inset planted-foot probes instead of the full transparent 64px
  cell edge. Turning must no longer mirror an airborne walk pose instantly:
  reserve frame 8 for a centered, both-feet-planted pivot and pause there for
  eight game frames before resuming the opposite-facing walk. This is still
  local safe-surface patrol; persistent traversal and gap/water jumps remain a
  separate later navigation phase.
  MrDig accepted the longer routes and planted pivot directionally, but the
  fast floor Strider exposed a second gait-art issue: the eight frames still
  held the hips low and both knees deeply flexed, producing a stiff crouched
  shuffle despite correct leg alternation. Do not add interpolation frames to
  that flawed motion. Replace the eight-frame source with a taller athletic
  walk: longer heel-to-toe stride, near-straight support leg, visibly high
  passing foot, subtle acceptance dip and toe-off rise, opposing arm swing and
  stable head mass. Preserve slots, baseline, cadence, turn frame and AI.
  The next FS-UAE movie improved posture but retained an obvious motion jerk.
  Native-frame measurement showed identical row-61 baselines and only 0.5px
  horizontal centre variation, excluding generator alignment and Bob restore.
  The discontinuity is pose timing, most visible at frame 7 wrapping to 0.
  Upgrade the closed gait to twelve frames (six phases per leg), adding
  mid-stance and pre-contact transitions so frame 11 flows into frame 0 and
  frame 5 into frame 6. Move the already accepted planted pivot unchanged to
  slot 12; keep the total packed cache at 18 frames and leave AI/routes intact.
  MrDig's two follow-up FS-UAE movies rejected that twelve-frame candidate: it
  was less natural and introduced another mid-cycle jerk. Root cause is not
  frame count or Bob alignment but inconsistent whole-pose generation. Unlike
  Sparkpaw's coherent eight-frame run family, independently generated Strider
  poses subtly change joint geometry and mass. Reject/remove the twelve-frame
  source and restore the better upright eight-frame v3 candidate, retaining the
  accepted pivot, patrol routes and foot probes. Two further full-sheet ImageGen
  attempts were rejected before integration (one introduced chromakey-conflict
  green details and identity drift; the other reverted to shuffling and failed
  opposite-half symmetry). Do not keep iterating whole AI-generated sheets.
  The next art step must use fixed-part/rig-based sprite authoring or deliberate
  native-pixel edits with a local looping preview before game integration.
  IMG_2762 confirms the restored eight-frame v3 source still has a mid-cycle
  silhouette glitch and anatomically reads as a dance. Inspection found that
  some limbs are disconnected in source cells and `keep_main_component()` then
  correctly—but undesirably—removes them as residue. Fixing cleanup alone does
  not fix the inconsistent anatomy. A first separate native 64x64 rig audition
  locks the accepted torso and uses exact opposite-half leg coordinates; it
  proves deterministic symmetry/no missing parts but its newly drawn legs are
  too schematic for production and must not enter runtime. Until detailed limb
  art exists, repeat accepted idle in walk slots 0..7 while keeping movement,
  longer routes, planted pivot slot 8 and foot probes. This intentionally glides
  rather than presenting the rejected dance as finished animation.
  IMG_2763 showed that idle-only sliding made the otherwise accepted isolated
  front pivot read as a one-frame glitch: side-idle abruptly became frontal and
  back while no surrounding gait supported that motion. Remove the pivot from
  this temporary baseline as well and repeat the accepted side-idle across all
  18 runtime slots. Keep movement/routes/foot probes, but present no unfinished
  animation. The separate schematic rig preview confused review and is not a
  deliverable; remove it and its helper before continuing. Future walk and turn
  must be reviewed together as one polished local loop before reintegration.
  The static-baseline retest still showed first-approach/reload corruption only
  on the first raised Strider; the later floor Strider remained stable. With
  every runtime frame now byte-identical, this excludes animation art. Preserve
  old drawnType/X/Y restore as before, and additionally mark every newly
  initialized or slot-assigned enemy `needsPrime`. During the existing Blitter
  restore phase, after old enemy/collectible restores and before any Bob draws,
  copy that new actor's complete destination rectangle from `frontClean` once.
  This is a synchronized Blitter restore, not CPU RMW. Clear the flag afterward;
  normal per-frame restore/draw ordering remains unchanged. Require the exact
  first approach, revisit and level-reload FS-UAE regression again.
  IMG_2769 rejects that prime-restore experiment: raised Strider corruption is
  substantially worse, sometimes losing half the upper body, while the later
  floor Strider remains stable. Remove `needsPrime` and the extra restore call
  immediately. Do not retain this experiment. The stronger failure is evidence
  that added 64x64 Blitter work aggravates the underlying timing/bandwidth issue
  rather than cleaning stale destination pixels. Audit post-line-300 workload
  and the scene-specific number/height of simultaneous Bobs before another fix.
  That audit found the concrete timing error: the Copper stops reading
  `frontDisplay` at its existing HUD switch on hardware line 252
  (`44+HUD_TOP`), but the CPU withheld every Bob restore/draw until line 300.
  This left only about twelve PAL raster lines before wrap, formerly adequate
  for small beetles/projectiles but not reliable for moving 64x64 four-plane
  Striders. Begin the unchanged synchronized Bob pass at line 253 and wait for
  wrap with the same threshold. This recovers roughly 47 safe lines without
  changing the line-100 Copper staging, line-252 HUD switch, buffers, cache
  format, Blitter ordering or gameplay. Retest raised/floor Striders under the
  static all-idle baseline before restoring any animation.
  MrDig completed that FS-UAE regression: first raised-Strider approach, later
  return and level reload no longer show corruption; the floor Strider remains
  stable. Accept line 253 as the corrected Bob-pass start. No real-hardware
  verification exists. The reason corruption seemed to appear during animation
  work is that shifted 64px Bobs can require five destination words per row and
  the heavier scene crossed the old line-300 budget intermittently; changing
  silhouettes made missing background bands resemble bad animation frames.
  The all-idle baseline exposed the renderer fault unambiguously. Preserve this
  accepted timing checkpoint before any new locomotion art is integrated.
  The next offline high-resolution leg-rig audition uses the existing 1254px
  premium idle source, never newly generated whole poses. It locks the complete
  upper body and rotates cropped thigh/shin/foot pieces around fixed joints,
  deriving the second half from the same geometry. The first resulting native
  sheet preserves original detail and has no missing pieces, but its stride is
  too small and pelvis overlap seams remain visually unsettled. Keep it offline
  under `build/strider-leg-rig-audition`; do not integrate or ask MrDig to test
  it yet. Refine masks and four master phases locally, then add arms/turn only
  after the leg-only loop reads naturally.
  Reserve the current 18-frame Strider budget explicitly: at most slots 0..7
  walk, slot 8 planted turn, and slots 9..17 for hit/hurt and death. Do not
  consume those combat slots with extra walk interpolation. Jump/landing art
  will likely require a deliberate later cache-contract expansion rather than
  aliasing death frames. The offline rig must use connected hip-knee-ankle
  target chains; independently rotating pieces around their original fixed
  coordinates recreates the small-stride overlap problem.
  The chained-segment/cap attempt closed gaps but became too noisy at 64x64.
  Prefer a rigid mechanical-leg audition: retain each complete premium leg as
  one uninterrupted detailed source piece, rotate it modestly at the hip, and
  derive the opposite half-cycle symmetrically. Add only subtle complete-arm
  swing beneath locked shoulder/torso pixels. Quantify adjacent alpha-silhouette
  deltas, including the wrap, and tune the two half-cycles toward comparable
  changes. Remove all soft green-key fringe before judging native pixels. This
  remains offline and does not alter the reserved 9 combat/death slots.
  The first testable replacement is now `walk-rig-v1`: eight symmetric rigid-
  leg poses authored from the accepted premium source, quantized through the
  exact production `FRONT16` bank before review. Passing poses remain slightly
  asymmetric instead of collapsing both legs into one neutral silhouette, and
  adjacent alpha deltas are substantially more even. Runtime slots 0..7 use
  this loop; slot 8 stays the accepted side idle until a planted turn is ready;
  slots 9..17 remain untouched idle placeholders reserved for later shooting,
  hurt/hit and death work. This is an asset-only locomotion test: renderer,
  line-253 synchronized Bob pass, gameplay routes and collisions are unchanged.
  MrDig accepted `walk-rig-v1` as consistent and worth continuing: it reads as
  a deliberately stiff, heavy mechanical gait rather than a human walk. Add a
  six-frame stationary turnaround at patrol edges/walls. Runtime slot 8 uses a
  planted premium front pose at the same 56px actor height and line-62 foot
  baseline; retain the old facing while it is displayed, then flip facing,
  reset the walk phase and launch from frame 0. Slots 9..17 remain reserved for
  later shooting, hurt/hit and death animation. This small enemy-state change
  does not modify the synchronized renderer or its accepted line-253 timing.
  Follow-up movies plus the broad slot-8 control build show those apparent
  glitches were brief front-pose appearances, not corrupt walk/cache frames:
  replacing slot 8 with a side pose removed the effect while all eight walk
  frames remained stable. Restore the premium planted front pose, but enforce
  one selection contract: only the exact velocity-sign reversal caused by a
  patrol edge, wall or missing next foot support may enter slot 8. Ordinary
  walking selects slots 0..7 exclusively; retain old facing throughout the
  six-frame hold, expose new facing only on expiry, reset walk phase and leave.
  A frame-by-frame FS-UAE check then proved slot 8 could still appear around
  the visual middle of a route: the code grouped authored patrol extrema with
  incidental solid/missing-support probes, so every safety reversal received
  the same visible turn. Split those causes. Only `front < patrolLeft` or
  `front >= patrolRight` may enter slot 8; a wall/support safety reversal stays
  in the 0..7 side loop. The authored patrol extrema already match the visible
  platform ends, so the premium front pose is now reserved for those ends.
  The definitive remaining one-frame leak was simpler and independent of turn
  detection: Strider walk advancement used `animFrame+1`, so walk frame 7
  became reserved frame 8 for one update; only the following update's guard
  reset it to zero. Wrap the increment immediately with `&7`. The explicit
  turn state is consequently the sole writer of frame 8. This explains the
  repeatable mid-route flash on both raised and floor Striders while their real
  endpoint turns remained correct.
- The supplied YouTube URL could only be inspected through a short browser
  preview in this session, not downloaded or measured frame-by-frame. Treat
  MrDig's stated behaviour above as the authoritative design target; do not
  invent exact ThunderCats timings or distances without better source evidence.
- Phase 3C.2 invulnerability feedback uses safe whole-actor blinking: during
  accepted invulnerability, all six player Copper pointers periodically select
  the existing null sprite. Cached 48-row streams, attached-pair control words
  and terminators are never modified, explicitly avoiding the rejected VSTOP
  clipping failure.
- Four new original short hurt auditions were generated:
  `player-hurt-candidate-meow-snap.wav`, `-meow-yelp.wav`, `-meow-rough.wav`
  and `-meow-chirp.wav`. MrDig selected the very short arcade-like
  `meow-chirp`; it now generates the runtime `player-hurt.raw` while preserving
  the accepted damage trigger, Paula channel 1 and priority 9.

## 14 August 2026: repository-local Sparkpaw workflow skills

Five focused Codex skills now preserve recurring project contracts without
turning the handoff into procedural history. `extend-sparkpaw-animations`
already covered append-only player/enemy/effect/water asset integration. The
new `build-sparkpaw-visual-slice` isolates concept-quality environment work and
feasible renderer/palette proofs; `analyze-amiga-test-evidence` separates
observation from renderer/gameplay hypotheses; and
`catalog-sparkpaw-test-evidence` gives reviewed MOV/PNG files a collision-safe
phase/result name plus a same-basename provenance sidecar. The catalog rule
preserves media bytes and never overwrites existing evidence.

`ship-sparkpaw-checkpoint` adds Sparkpaw-specific build, SemVer, documentation
and four-artifact gates before the generic Git shipping workflow. The separate
`study-thundercats-reference` skill routes every Sparkpaw/ThunderCats comparison
to `sparkpaw/testresults/thundercats-level1.mov`, treats it only as a design
reference and opens each activation with “Thunder, thunder, thunder,
ThunderCats, Ho!” No gameplay, renderer, asset, release identity or verification
claim changes in this workflow-only checkpoint.

## 14 August 2026: Phase 6B.6 visual slice begins

MrDig set the post-slice level target explicitly: extend the accepted
2048px/eight-screen route by half to approximately 3072px/twelve screens, add
more authored Strider and beetle encounters and distribute several varied
water gaps. This is Phase 6C after the renderer decision. It retains the generic
four-slot active enemy pool; candidate count and encounter density may grow
without increasing simultaneous enemy Bobs.

Phase 6B.6 starts separately with the existing water area as one representative
environment slice. The built-in ImageGen reference pass used the saturated
gameplay concept for colour/material identity and water concept v4 for the
accepted low-ground composition. It intentionally excludes Sparkpaw, enemies,
diamonds and HUD so environment quality can be judged without invented object
contracts. The resulting review-only `sparkpaw-visual-slice-concept-v1.png`
adds compact carved banks, saturated blue water, one raised ruin platform and
deep forest/mountain/ruin parallax. It awaits approval before exact indexed
indexed previews. Production renderer, geometry, mechanics and Phase 6B.5
gameplay remain unchanged; no FS-UAE or real-hardware result is claimed. The
first concept's foundation was still far too tall despite its useful atmosphere,
so it is retained as rejected review history. V2 edits only that lower geometry
toward a compact strip while preserving the parallax/ruin direction and is now
the review candidate.

MrDig also recorded later, separately scoped polish: Sparkpaw is already viewed
as production-ready; water and splash animation are accepted; beetle colours
could separate further from Sparkpaw; diamond edges retain some pixel residue;
and Strider gameplay may benefit from a quick two-shot burst plus better speed
variation. None of these changes are folded into the Phase 6B.6 environment
slice or renderer comparison.

MrDig selected v2's lowest-ground direction and asked to pursue the richer
concept-water appearance if feasible. A deterministic proof now generates the
same 320x208 scene under production FRONT16/REAR8 4+3 and an authored 32-colour
PF1 art upper bound. Fixed banks replaced an unusable first median-cut attempt
that made the scene too dark and water violet. The corrected masked PF1 RMS is
21.44 for 4+3 and 14.71 for the upper bound. Matched water sheets demonstrate
that both can preserve full-width irregular wavelets and independently phased
bubbles. The hardware audit then caught the decisive constraint before any C
change: AGA permits at most four planes per dual playfield, so eight total are
4+4 rather than 5+3. The extra-colour PF1 image is not a renderer candidate.
Concept-style water remains feasible inside 4+3 with a deliberate four-colour
navy/blue/cyan/foam treatment, but accepted runtime water was deliberately left
unchanged when MrDig redirected the pass toward the larger foreground/parallax
goal.

The approved environment direction was then separated into an authored
parallax master and reduced deterministically to the existing REAR8 bank. The
production rear asset now carries storm clouds, layered mountains, dense forest
silhouettes, ruined towers, waterfalls and a cyan-lit central ruin. This is an
asset-only 4+3 integration: no extra bitplane, Copper change, display timing or
Bob contract was introduced. A matched REAR16 image is preserved as the input
to a possible isolated 4+4 benchmark. Host `make` succeeds; FS-UAE and real
hardware remain unclaimed for this candidate.

MrDig explicitly prioritized visual perfection over retaining the smallest
possible bitmap footprint: additional layers or memory are authorized when a
measured implementation produces a clear improvement and remains viable on the
stock A1200 with 2 MB Chip plus 8 MB Fast. The integrated REAR8 candidate does
not yet spend that budget; the separate REAR16 result supplies the visual case
for a future 4+4 benchmark.

## 14 August 2026: Phase 6B.6 extended REAR8 parallax span

MrDig's supplied `2026-08-14 21-15-35.mov` showed that the first authored rear
direction was a strong improvement but exposed an abrupt repeated-art boundary.
Source inspection confirmed that one non-seamless 640x208 reduction was pasted
repeatedly across the 2048px rear bitmap.

The iconic opening vortex, lightning tower and mountain composition is retained
in a non-destructive v2 source, followed by unique mountain passes, forest,
smaller ruins and calmer distant scenery. Deterministic conversion now produces
a 1024x208 REAR8 strip. At quarter-speed scrolling, the current 2048px world can
sample only through rear pixel 751 and the planned 3072px world through pixel
1007, so the padded repeat at 1024 is outside both complete camera ranges. The
runtime rear bitmap stays 2048x256x3 and therefore adds no Chip memory, bitplane
fetch, Copper or frame-time cost. REAR16 improves host RGB RMS from 20.58 to
19.99 and visibly separates cyan/green ruin material, but remains an unintegrated
4+4 benchmark candidate.

No renderer, collision, foreground, water, gameplay, player, enemy or animation
contract changed. FS-UAE acceptance of the revised span remains pending and no
new real-hardware result is claimed. Foreground concept-quality material work is
the next separate visual-slice step.

## 14 August 2026: Phase 6B.6 foreground material v1

The foreground pass was kept separate from the extended rear integration. The
existing authored collision rectangles, water opening, y=200 floor, platform
bounds and actor baselines are unchanged. Only deterministic FRONT16 drawing was
altered: flat greybox slab interiors now use layered pale load-bearing edges,
irregular steel panels, recessed violet machinery, cyan conduit fragments and
bounded cracks. The thin ground cap uses the same material language without
becoming a tall foundation.

No new palette pen, bitplane, bitmap, runtime copy, Copper instruction or Bob
operation is added. The native preview is a meaningful v1 improvement but not
the final concept-art ceiling: repeated small modules remain visible, green/moss
cannot be added safely without auditing shared foreground/enemy palette ownership,
and larger unique foreground silhouettes remain a later isolated art step.

Host `make PYTHON=../.venv/bin/python3` and
`make release PYTHON=../.venv/bin/python3` both succeed for
`0.6.0-alpha.7`. Release validation confirms the bootable ADF and the sole
current LHA, ZIP, ADF and source-ZIP artifact set. This is host/package evidence
only; FS-UAE review of the new rear span and foreground material remains pending.

## 14 August 2026: alpha.7 visual rejection and isolated REAR16 benchmark

MrDig's supplied FS-UAE recording shows the extended rear composition scrolling
without the former abrupt repeat, but rejects the scene as the final quality
target: broad eight-colour violet/blue regions and repeated rectangular
foreground modules still read closer to OCS/ECS than premium AGA concept art.
The evidence is preserved as
`testresults/Phase 6B.6-rejected-insufficient-aga-depth.mov` with its sidecar.

Research against Commodore's AGA supplement, AGA coding references and
demoscene palette practice confirms that the next hardware-feasible comparison
is 4+4 dual playfield, not 5+3. The isolated rb19 benchmark now loads the exact
REAR16 art into a 672x256 four-plane rear bitmap, uses BPU3/eight bitplane
pointers and writes full 24-bit PF1/PF2 palette values. It compiles on the host.
Its test-only rear data is 86,016 bytes; production's extra plane would cost
65,536 Chip bytes at 2048px or 98,304 at 3072px, with raster fetch increasing
from seven to eight planes. FS-UAE timing and visual evidence remain pending.

The first supplied launch returned code 11 before display takeover. Its synced
log proves the exact cause: `AllocBitMap(672,...)` reports an 88-byte displayable
row stride, while the host asset generator had packed the mathematical 84-byte
pixel width. The Amiga therefore expected 90,112 bytes across four planes but
the file contained 86,016. rb19b generates four 88-byte-stride planes, keeps
both RAW files beside the executable and prints exact read diagnostics. This is
a benchmark asset-layout defect, not evidence against 4+4.

The next supplied rb19b recording proves stable coherent geometry and scrolling,
393 frames to camera 300, all eight planes loaded, no Copper overflow and clean
Workbench restoration, but its displayed palette is invalid. PF2OF=16 reads
indices 16..31 from bank zero; rb19b wrote its second sixteen colours through
COLOR00..15 while selecting bank one, filling indices 32..47 instead. rb19c now
writes COLOR00..31 in bank zero for both AGA high- and low-nibble passes. BPU3,
all eight pointers, 88-byte row stride, modulo and scroll code are unchanged.

Supplied rb19c FS-UAE evidence subsequently shows the intended rich violet,
cyan and green REAR16 scene scrolling coherently and returning cleanly after
1039 frames. It is accepted as an isolated visual proof: the fourth rear plane
has a worthwhile visible benefit. It does not prove production timing because
rb19c omits player sprite DMA, the HUD switch and the line-253 Bob workload.

Alpha.12 therefore adds rb20 as a matched pair. Both 4+3 and 4+4 variants use
the same scene, six active sprite DMA channels, line-252 HUD switch and the same
deliberately busy line-253 restore/water/draw sequence for four 64x64 enemies,
six projectiles and six collectibles. Each log records the final workload line
and detects a PAL-frame wrap. Production remains unchanged at the accepted 4+3
renderer until MrDig supplies both FS-UAE logs; no ADF or real-A1200 result is
claimed.

The supplied rb20 pair subsequently records OVER_BUDGET for both candidates:
4+3 completes after the wrap around line 58 and 4+4 around line 71. The extra
plane therefore shows real DMA pressure, but rb20 cannot decide feasibility
because its control used four simultaneous 64x64 Striders, which current level
data and the two-Strider runtime gate cannot produce.

Alpha.13 replaces that load with rb21's source-derived worst case while keeping
the two binaries otherwise matched. The current 368-pixel collectible culling
window contains at most nine authored diamonds. All eight projectile slots are
used, and the four enemy slots contain the conservative runtime-valid mix of
two 64x64 Striders and two 32x24 beetles. Splash and the expensive alternating
double-buffer water update are included in the same frame. Logging now counts
over-budget frames once and reports absolute elapsed raster lines instead of
counting every post-wrap Blitter wait. Supplied FS-UAE timing remains pending.

The supplied rb21 logs report every synthetic frame over budget: 135 elapsed
raster lines for 4+3 and 141 for 4+4. Both videos remain visually coherent and
the six-line difference is useful eighth-plane DMA evidence, but the control
still fails because independent maxima are not necessarily simultaneous in a
real camera view. This result is catalogued without rejecting REAR16.

Alpha.14 adds rd01 as a compile-time-only production diagnostic. It leaves the
ordinary executable and accepted 4+3 renderer unchanged. During the real
line-253 pass it counts each operation actually performed, measures start/end
raster lines, and retains only the slowest frame with camera position and exact
projectile, beetle, Strider, collectible, splash and water mix. A diagnostic
left-mouse exit restores Workbench before writing `renderdiag.log`; no DOS work
occurs during takeover. Supplied FS-UAE evidence is pending.

MrDig's supplied rd01 HD run writes a clean log after 2958 real production
passes. 1900 crossed the PAL boundary. Its 172-line peak at camera zero restores
and redraws seven diamonds, one beetle and one Strider while also updating water;
no projectile or splash is required. The coherent video is not fixed-50-Hz
proof because a wrapped pass lets the existing loop update again later in the
same field. This is accepted FS-UAE timing evidence, not ADF or hardware proof.

Alpha.15 addresses only the measured collectible cost. Each diamond's original
aligned 16x25 four-plane background is stored once in a compact 5,376-byte Chip
cache. Active diamonds persist in both clean and display worlds, so enemy and
projectile restores retain them. Their accepted eight-position hover remains,
but update ownership is staggered by index across four frames; only changed,
visible diamonds restore and redraw both worlds. Collection restores the cached
background to both. Renderer ordering, masks, palette, collision and gameplay
are unchanged. rd02 will measure the resulting production high-water.

The supplied rd02 session runs 5612 production passes; 4189 wrap. The diamond
optimization is visible in its peak mix—only two collectibles update—but a
later real combat frame at camera 201 restores and redraws six projectiles,
three beetles, one Strider and those two diamonds. It takes 227 raster lines.
This parks REAR16: the next performance work must address production combat,
not fabricate another palette benchmark.

At MrDig's request, alpha.16 compiles the same post-takeover high-water logger
and clean left-mouse restore into the ordinary HD executable. It writes only
after hardware/system restoration. The ADF build omits the diagnostic macro,
keeps reset-to-exit and performs no DOS write. Existing ignored test drawers
and evidence remain untouched, but new normal testing no longer needs them.

Foreground material study v1 demonstrates the desired premium ruin material
depth but is rejected as a geometry source because it adds a left raised
structure. Foreground concept v3 repeats the edit with a strict geometry lock:
the low walking line, water gap and existing right platform remain, while large
asymmetrical structural bays, chipped pale lips, dark braces, violet recesses,
cyan conduits and restrained vegetation replace repeated small box modules.
It is review-only concept art and has not entered runtime conversion.

## 15 August 2026: alpha.17 Copper-banded REAR8 colour pass

The fixed REAR8 reduction and foreground v1 remained visibly below the desired
AGA quality target in supplied FS-UAE review. A fourth rear plane has a proven
visual benefit, but rd02's 227-line production combat peak leaves it parked.
Instead, the existing three-plane rear bitmap now uses the scene's fixed
vertical composition: sky, mountains and forest each receive a purpose-built
eight-colour palette. Twelve Copper changes, issued during the preceding
horizontal blanks, morph between the palettes so the scene does not break into
hard horizontal bands. The native asset is quantized against the exact palette
active at each line; the low forest is graded toward the concept's cold
green/teal range.

The rear bitmap dimensions, quarter-speed scrolling, seven-plane fetch,
line-100 list staging, line-252 HUD switch and synchronized line-253 Bob pass
remain unchanged. The Copper list reservation grows from 1,024 to 1,536 Chip
bytes; there is no extra bitplane, bitmap, Blitter operation or displayed-Chip
CPU compositing. A generated large flat PF1 silhouette experiment was rejected
and removed before release, so foreground v1 and collision geometry are
unchanged. Host generation and compilation succeed. FS-UAE, ADF gameplay and
real-A1200 review of the new Copper timing remain pending.

## 15 August 2026: alpha.18 non-aligned diamond restore correction

Supplied alpha.17 FS-UAE evidence accepts the Copper-banded rear as a substantial
visual improvement and shows no hard palette seam or transient Copper corruption.
The paired post-exit log records a 175-line high-water under four projectiles,
one beetle, one Strider, two collectible updates and water; fixed 50 Hz remains
unproven. Late in the route, a partial cyan diamond remains at the left edge.

Collectible index 28 at x=1704 is the only authored diamond not aligned to a
16-pixel destination word. Its masked draw touches two words, while alpha.15's
compact background cache restored only one. Alpha.18 gives that one slot a
dedicated two-word, four-plane, 25-row cache. This adds 400 Chip bytes and keeps
the other 31 diamonds on the one-word restore path. Copper palettes, collision,
culling windows, hover timing and restore-before-draw ordering are unchanged.
FS-UAE correction evidence remains pending; no ADF or real-A1200 result is
claimed.

## 15 August 2026: alpha.19 complete parallax-master v3 re-author

Review of alpha.17 identified the upper 25-35 rear pixels as the weakest visual
region: long dark horizontal streaks and uneven detail read as unfinished art,
not an AGA limitation. Two procedural top-strip experiments were rejected
before release because they either produced round cloud blobs or merely moved
the inconsistent source texture. The corrective decision was to re-author the
entire parallax master from the accepted concept direction rather than patch
one edge.

The new full-height panorama preserves one opening vortex and lightning-struck
tower in the left quarter, then progresses through unique turbulent cloud
masses, mountain passes, cold-green forest, storm ruins and waterfalls. That
opening landmark is now an explicit narrative contract: it echoes the tower in
the boot title art and foreshadows the distant end-level destination. Later art
passes may not replace it with generic scenery.

Conversion retains the full vertical composition before exact 1024x208
reduction; the old v2 top-biased crop is not reused. The existing per-line
eight indices and twelve-step Copper sky/mountain/forest morph remain unchanged.
Runtime rear bitmap dimensions, three-plane fetch, quarter-speed scroll,
Chip/Fast residency and Bob workload therefore do not increase. Host native
previews pass composition, upper-edge and repeat-range inspection. FS-UAE, ADF
gameplay and real-A1200 review of v3 remain pending.

## 15 August 2026: alpha.20 newly generated title-identity parallax v4

Supplied alpha.19 FS-UAE evidence accepts the full panorama, upper cloud work
and apparent diamond-residue correction, but identifies a narrative mismatch:
the opening ruin reads as a broad rock needle rather than the boot title's
slender Stormstone citadel. The next master is therefore a completely new
ImageGen generation, not an edit, paint-over or extension of v3. The exact
runtime title preview is its architectural authority.

V4 places one isolated destination citadel in the left quarter with a tall
central gothic spire, lower side turrets, a visible castle base, lightning-to-
vortex contact and cyan Stormstone heart. Exact 1024x208 reduction preserves
that silhouette. One of the existing eight mountain-band colour roles changes
from dark blue-grey to cyan so the energy heart, distant conduits and waterfalls
survive; mountain depth remains readable in the native review. No plane,
Copper step, bitmap byte, Blitter operation, collision or gameplay contract is
added. Host previews pass; FS-UAE, ADF gameplay and real-A1200 review of v4
remain pending.

## 15 August 2026: alpha.21 newly generated foreground-kit integration

Supplied alpha.20 FS-UAE evidence accepts the title-identity parallax as the new
rear baseline. Phase 6B.6 therefore moves to the separately scoped foreground.
Rather than polish the repeated procedural boxes, a new orthographic Storm
Ruins kit was generated from concept v3's material direction and the native
viewport's scale language. Chroma removal produces a reusable RGBA source with
broad platform families, gothic columns, open braces and cyan conduits.

Deterministic conversion maps whole source pieces into the exact existing
FRONT16 rectangles. Wide raised platforms may extend transparent decorative
braces below their collision slab; their horizontal top and collision data do
not move. Cyan source pixels are explicitly retained through existing blue/cyan
pens instead of disappearing into dark blue. The fixed y=200 floor remains an
eight-pixel cap above the line-252 HUD and uses broad 64px bays, because a tall
concept facade there would misrepresent collision or require a separate layout
change. Water opening, actor baselines, palette ownership, four-plane front
bitmap size, line-253 Bob pass and renderer DMA are unchanged. Host preview and
build evidence only; FS-UAE, ADF gameplay and real-A1200 review remain pending.

## 15 August 2026: alpha.22 richer foreground architecture kit

MrDig's supplied alpha.21 FS-UAE/HD recording confirms that the new authored
stone/steel material is better than the grey boxes, but rejects it as the final
foreground baseline. Across the complete scroll, the centred T-support repeats
too often, the eight-pixel ground reads as a uniform generic tech strip, and
the old purple lozenges and hanging orange lamps now conflict with the richer
world. Those objects are non-solid legacy decoration with no gameplay function.
The recording is catalogued as
`testresults/Phase 6B.6-alpha21-foreground-v1-review.mov`; its adjacent
`renderdiag.log` predates the run and is not paired timing evidence.

Alpha.22 uses a wholly new orthographic ImageGen source rather than repainting
the first kit. It supplies seven horizontal slab rhythms, four asymmetric
underside structures, four grounded pier families, mirrored bank motifs and a
long shallow facade. Deterministic FRONT16 reduction maps those whole pieces
into the established rectangles and compresses the long facade into the honest
eight visible ground pixels above the HUD. The obsolete purple/orange floating
decoration is deleted from generation. Collision-map generation, walkable tops,
enemy surfaces, traversal links, actor baselines, water, bitmap size, Copper
lists and line-253 workload remain unchanged.

The concept references also motivate real dry gaps and larger L-shaped/portal
ruins. That is explicitly deferred to an isolated level-layout checkpoint:
changing the continuous floor affects collision, pacing, fall semantics and
enemy routes, so it must not be hidden inside this art-only release. Host asset
generation succeeds; native/package build results follow below. FS-UAE/ADF
gameplay and real-A1200 review of alpha.22 are pending. Both required host
commands, `make PYTHON=../.venv/bin/python3` and
`make release PYTHON=../.venv/bin/python3`, pass. Release validation produces
the sole four-file `Sparkpaw-0.6.0-alpha.22` artifact set; the packed ADF front
asset is 24,462 bytes and the complete bootable DOS1/FFS image uses 736 KiB.

## 15 August 2026: alpha.23 Phase 6C.1 twelve-screen authored route

MrDig accepts alpha.22 as the foreground material direction but correctly
identifies route silhouette, not texture density, as the next quality limit.
He authorizes the planned one-and-a-half-length level, two water hazards, two
ordinary gaps, richer platform architecture and substantially more Striders.
The supplied concept screenshots emphasize grounded L-shapes, portal ruins,
broken bridges and vertical side piers. The local ThunderCats reference at
approximately 8–12 and 24–28 seconds reinforces a different reusable principle:
enemies enter the visible field from either direction during continuous play,
rather than existing only as camera-triggered arena events. No reference pixels,
layout or characters are copied.

Production width changes from 2048 to 3072 pixels. The extension authors a
broken low bridge, high side-pier approach, second water opening, broad court
and final portal/chasm. Water occupies x=1584..1663 and x=2432..2511; dry gaps
occupy x=2112..2175 and x=2784..2863. Dry falls consume one life through the
safe resident reset without borrowing splash/audio semantics. The second water
destination reuses the accepted sixteen-frame 7,040-byte Chip bank and adds
only the corresponding clean/display Blits inside the line-253 pass.

Four new required Striders join the three earlier encounters. Authored links
cross both water gaps and both dry gaps in both directions. Existing offscreen
persistence remains authoritative: every parked Strider updates world position,
direction and traversal once per frame without rendering, so camera entry is a
consequence of its route rather than a spawn-at-edge illusion. Ten required
beetle candidates plus bounded optional candidates and 48 collectibles populate
the longer level, while the active enemy/Bob pool remains four slots.

Resident growth versus 2048px is exactly 131,072 bytes for `frontClean`, another
131,072 for `frontDisplay`, 98,304 rear bytes and 896 collision bytes: 360,448
extra bitmap bytes, plus small collectible restore/state growth. This corrects
the initial incomplete estimate that omitted the separate displayed foreground.
The normal diagnostic HD executable now records prepared-peak and post-run
Chip/Fast free/largest blocks in `renderdiag.log` on left-mouse exit.
The 1024px authored rear master still covers the maximum quarter-speed sample:
the 3072px camera ends at 2752, requiring rear pixels only through 1007. Player,
beetle and Strider frame contracts, line-100 Copper staging, line-252 HUD switch,
restore/draw ordering and displayed-Chip compositing rules are unchanged.
Native `make` and `make release` pass. The verified bootable DOS1/FFS ADF uses
1,547 blocks (773 KiB), leaving 213 blocks. FS-UAE timing/pacing, ADF gameplay
and real-A1200 memory remain pending.

The ADF storage audit then applies one isolated low-risk extension of the
already accepted SPR1 path. `sparkpaw-sprites4.spbm` packs from 184,380 to
135,907 bytes and is streamed through the same 512-byte reader into its final
normal-memory asset allocation. HD/LHA/ZIP retain ordinary SPBM data: runtime
compression there would not reduce resident Chip memory. LZ4-68k, Shrinkler
and ZX0 remain measured future candidates rather than silently introduced
dependencies; raw-track loading is also deferred because normal sequential FFS
reads retain compatibility and exploit the track buffer.

## 15 August 2026: alpha.24 final-ascent and dry-chasm correction

The supplied alpha.23 FS-UAE/HD recording is preserved as
`testresults/Phase 6C.1-alpha23-route-blocked-final-ascent.mov`. It confirms the
longer route runs, but rejects the build as a playable baseline: Sparkpaw hits
the final portal wall below a platform whose top is 72 pixels above the floor.
That is authored unreachable geometry, not a reason to increase the global jump
and destabilize every accepted platform contract.

Alpha.24 reshapes only the final portal approach. The dry-gap destination first
provides a low 64px-wide landing at y=176, followed by a 48px rise to the y=128
portal top. Both player steps are collision-readable; the corresponding
bidirectional Strider link and destination surface are retimed to the new
landing window. Host trajectory calculation reaches the forward window on
flight tick 52 and the return window on tick 53.

The two dry openings remain semantically distinct rather than becoming more
water. Their eleven visible rows above the fixed HUD now contain asymmetric
broken bank faces, recessed dark lower edges and severed cyan conduit glints;
collision remains empty throughout. Floating platform art alternates shallow,
braced and deep underside silhouettes using the existing FRONT16 kit and
palette. No bitmap, bitplane, Copper step, Blitter operation, player physics or
hardware-sprite contract is added.

Native `make` and `make release` pass. Release verification decodes all four
ADF-only SPR1 streams byte-for-byte against their SPBM sources. The bootable
DOS1/FFS image uses 1,546 blocks (773 KiB), leaving 214 blocks. This is host
validation only; alpha.24 still needs supplied FS-UAE route/traversal review,
ADF gameplay parity and real-A1200 testing.

## 15 August 2026: alpha.25 collision-path-correct final ascent

The long supplied alpha.24 FS-UAE/HD recording and screenshot reject the first
ascent correction. Sparkpaw reaches the low y=176 landing, but the y=128 portal
begins on its immediately adjacent pixel. Because player physics resolves
horizontal movement before vertical movement, the standing hitbox meets that
wall before the nominal 48px rise is achieved. `moveX()` zeroes velocity, so
the player falls back regardless of the theoretical jump apex. The prior host
trajectory check modelled height only and was therefore insufficient.

Alpha.25 makes the route forgiving rather than frame-perfect. The low landing
ends at x=2911, x=2912..2927 is open, and the complete portal deck begins at
x=2928 with top y=144: a 16px horizontal separation and 32px rise. A host model
reads the generated collision map and reproduces acceleration, fixed-point
gravity, moveX-before-moveY and the standing hitbox. Starts from x=2864, 2868,
2872 and 2880 with zero through maximum horizontal speed all land on the deck
at frame 38 without wall blocking. The adjusted bidirectional Strider arcs land
in their windows on ticks 53 and 52. Jump physics, player animation, renderer,
hazards and memory footprint are unchanged.

The same screenshot exposes a separate level-data mismatch: one far-right
diamond intersects the portal slab. Alpha.25 moves the final four Y positions
to 116, 100, 92 and 108, retaining at least the complete 21px Bob plus its 2px
hover range above the y=144 deck. The final beetle's authored surface/spawn is
likewise moved onto the corrected deck. Rendering, restore caches and active
pool size do not change.

A conservative audit then checks every 16x21 diamond rectangle from hover -2
through +2 against the generated solid map. It finds five additional historical
overlaps: indices 5, 36, 37, 38 and 40. Their Y positions become 174, 86, 86,
118 and 110 respectively. Re-running the same 48-item audit reports zero solid
overlaps. This is placement data only; diamond art, animation, padded source,
restore ownership and Blitter cadence stay intact.

Native `make` and `make release` pass after the route and collectible changes.
Release verification round-trips every ADF-only SPR1 stream against its SPBM
source. The bootable DOS1/FFS image uses 1,548 blocks (774 KiB), leaving 212.
This is host validation; alpha.25 FS-UAE completion, ADF gameplay parity and
real-A1200 behavior remain pending.

## 15 August 2026: alpha.26 exhaustive final-ascent correction

The supplied eight-second alpha.25 FS-UAE/HD recording rejects the second
correction. Consecutive frames show the natural extreme-right, maximum-speed
launch still reaches the wall before the feet clear the y=144 deck. The prior
four-case model stopped at player origin x=2880, but x=2884 is the last valid
standing origin. That missing boundary case rises only about 27px before its
right hitbox reaches the wall, less than the required 32px.

Alpha.26 retains the readable 16px opening but lowers the complete portal deck
from y=144 to y=160, making the rise 16px. The host model reads the regenerated
collision map and executes acceleration, fixed-point gravity, complete standing
hitbox, moveX and moveY in runtime order. It covers every origin x=2864..2884
and speeds 0..648 in steps of eight plus exact 650: 1,743 combinations, zero
failures. Diamonds remain clear and the retimed Strider arcs land forward on
tick 53 and back on tick 52. No global movement or renderer contract changes.

Native `make` and `make release` pass. All four ADF-only streams round-trip
against their SPBM sources; the bootable DOS1/FFS image uses 1,548 blocks
(774 KiB), leaving 212. Alpha.26 FS-UAE completion, ADF gameplay parity and
real-A1200 verification remain explicitly pending.

## 15 August 2026: alpha.27 same-height final portal route

The supplied alpha.26 FS-UAE/HD recording again rejects the final ascent. The
player repeatedly reaches the low landing but does not clear the remaining
16-pixel portal step in practical play. That makes the host model insufficient
as acceptance evidence and the route remains game-blocking.

Alpha.27 removes the vertical step entirely rather than introducing a wall-jump
mechanic to compensate for level geometry. The low landing at x=2864..2911 and
the complete portal deck at x=2928..3071 now share top y=176, separated only by
the existing open x=2912..2927 tile. Portal columns start at the same height.
Global movement, jump velocity, animation slots, collision semantics and all
renderer contracts remain unchanged. Final patrol surfaces move to y=176 and
the bidirectional Strider crossing is retimed for equal-height banks.

The supplied recording is preserved as
`testresults/Phase 6C.1-alpha26-final-ascent-still-blocked.mov` with a matching
metadata/assessment sidecar. Alpha.27 still requires supplied FS-UAE completion
review; it does not establish ADF gameplay parity or real-A1200 behavior.

Native `make` and `make release` pass. Collision-map checks confirm equal y=176
banks, the open 16-pixel interval and absence of the former y=160 wall. All 48
diamond Bobs remain clear throughout their full hover range. Both retimed
Strider directions land on tick 53. ADF-only SPR1 streams round-trip against
their SPBM sources; the DOS1/FFS image uses 1,547 blocks (773 KiB), leaving 213.

## 15 August 2026: alpha.28 corrects the actual unreachable high approach

The supplied alpha.27 FS-UAE/HD recording makes the prior diagnosis error clear.
Sparkpaw is not attempting the final x=2928 portal deck; the two visible
diamonds identify the blocked structure as x=2320/2368, before the second water
gap. Its preceding platform ends at x=2287 with top y=176, while the target at
x=2320..2415 has top y=112. The 32-pixel horizontal opening therefore leads to
an impossible 64-pixel vertical rise and the attached x=2304 pier blocks the
player during every attempt.

Alpha.28 lowers the entire target slab and attached pier to y=160, preserving a
readable 16-pixel rise without altering Sparkpaw's jump, collision or animation
contracts. The subsequent 80-pixel water gap and y=144 far bank remain intact.
The associated persistent Strider link is retimed: the forward and return arcs
both land on host tick 37 with the corrected y=160/y=144 surfaces. Renderer,
palette, Bob ordering and resident-memory ownership are unchanged.

The new evidence also proves alpha.27 modified the wrong later structure. Its
full flattening at x=2928..3071 is therefore rolled back to alpha.26's raised
y=160 geometry rather than retained as an accidental art change. A blind return
to the original y=128 wall is rejected because it would knowingly reintroduce
a 72-pixel barrier. The independently audited diamond positions remain; their
clearance result does not depend on the mistaken platform identity.

The rejected recording is preserved as
`testresults/Phase 6C.1-alpha27-high-approach-unreachable.mov` with matching
sidecar. Alpha.28 still requires supplied FS-UAE route confirmation; ADF parity
and real-A1200 behavior remain unverified.

Native `make` and `make release` pass. A player model using the generated
collision bytes and runtime moveX-before-moveY order covers every origin
x=2240..2260 and 83 speeds from zero through exact maximum: all 1,743 attempts
land on the corrected y=160 platform. All 48 diamonds remain collision-clear;
both corrected water-link arcs land on tick 37 and the restored later-portal
arcs land on ticks 53/52. The final DOS1/FFS ADF uses 1,548 blocks (774 KiB),
leaving 212 after the packaged acceptance note. These are host results, not
FS-UAE acceptance of ADF gameplay.

MrDig's subsequent alpha.28 FS-UAE/HD run accepts the correction: Sparkpaw now
reaches the previously blocking platform, continues through the remainder and
completes the full 3072-pixel level. This accepts Phase 6C.1 route traversal.
The result does not establish ADF gameplay parity, real-A1200 behavior or a
full-run production timing/memory margin; those boundaries remain explicit.

## 15 August 2026: alpha.28 full-run follow-up plan

The subsequent long FS-UAE/HD playthrough supplies the missing production log
and two focused regressions. `renderdiag.log` covers 27,819 frames; 14,172 Bob
passes cross PAL wrap. The peak starts at line 253 and finishes at line 192 of
the next frame, or 251 raster lines versus 59 available (margin -192). It
restores/draws five projectiles, two beetles, two Striders and one collectible,
and updates both water locations. Prepared peak memory remains viable at
170,752 Chip bytes free, 169,456 largest, 6,362,480 Fast bytes free and
6,361,360 largest. This accepts neither production timing nor real-hardware
margin; it does show that memory is not the immediate FS-UAE blocker. The log
is preserved as `testresults/Phase 6C.1-alpha28-full-run-render-overbudget.log`.

The first recording shows intended black HUD-derived diamond facets becoming
transparent holes and its narrow lower pixels reading as a ragged fringe. The
cause is exact: HUD pen zero represents both black/shadow art and background,
while the world Bob correctly treats pen zero as transparent. The future art
step must replace it with one newly authored native 16x21 indexed diamond, pen
zero only outside the object and opaque dark contour/facets inside. Bob size,
mask generation, packed cache, hover and synchronized restore/draw order stay.
Evidence is catalogued as
`testresults/Phase 6C.1-alpha28-diamond-transparency-fringe.mov`.

The second recording shows standing plasma crossing an elevated beetle's body
without impact. Code confirms the sample point is inside the existing beetle
rectangle but is rejected by the additional `lowShot` predicate. That predicate
is redundant: floor beetles naturally reject standing fire because its Y does
not overlap their body, while crouch fire does. The next implementation is only
this eligibility correction, with host tests for elevated-hit and floor-miss;
diamond art follows as a separate checkpoint. Renderer profiling/optimization
comes third and must attribute water, projectile, enemy and collectible costs
before changing the stable 4+3 pipeline. The stale alpha23 diagnostic identifier
must be corrected in that later diagnostic build. Evidence is catalogued as
`testresults/Phase 6C.1-alpha28-elevated-beetle-standing-shot-miss.mov`.

## 15 August 2026: alpha.29 isolated elevated-beetle hitfix

The beetle branch in `enemiesHitProjectile()` no longer requires the legacy
`lowShot` boolean. It still requires an active, non-dying beetle and the exact
existing x+2..29, y+7..23 body rectangle. Health, eight-tick hit reaction,
twenty-tick four-frame death, sound result, respawn lifecycle, projectile
impact handling and the complete line-253 renderer remain unchanged. The
projectile field is intentionally retained to keep this patch narrow; the now
redundant callback parameter is removed so the runtime does not carry an unused
eligibility argument into enemy hit testing.

The runtime and a host regression share `beetleHitboxContains()`. `make test`
proves that a standing sample at y=171 hits a beetle elevated to y=152, that
the identical sample misses a floor beetle at y=176 through natural Y
separation, and that its crouch sample at y=185 still hits that floor beetle.
These are host geometry results only. MrDig's supplied FS-UAE/HD retest of the
normal alpha.29 executable accepts the elevated standing hit, ordinary floor
standing miss, floor crouch hit and clean left-mouse exit. ADF gameplay parity
and real-A1200 verification are not claimed. Diamond art and renderer profiling
are unchanged and remain later strictly separate steps.

`make test`, native `make PYTHON=../.venv/bin/python3` and
`make release PYTHON=../.venv/bin/python3` pass. Release validation round-trips
all four ADF-only SPR1 streams and verifies the bootable DOS1/FFS image. It uses
1,549 blocks (774 KiB) and leaves 211 blocks free. These remain host build and
package checks, not ADF gameplay acceptance.

## 15 August 2026: alpha.30 rejected player sprite-stream hypothesis

The supplied alpha.28 recording now catalogued as
`testresults/Phase 6C.1-alpha28-overlapping-strider-flicker.mov` shows that the
apparent Strider flicker contains transient orange/dark pixels over the pair.
Consecutive recording frames 210/211 and 216/217 isolate corruption and
recovery. This initially suggested unsafe player hardware-sprite mutation.

The six player Copper pointers are consumed at frame start, but the old path
then rewrote POS/CTL in the selected cached sprite streams after raster line
100. With Sparkpaw lower in the field, sprite DMA could still consume those
headers. Alpha.30 makes the complete 62-frame/facing/channel art cache immutable.
Two runtime banks each own six complete 100-word streams. The CPU prepares only
the inactive bank with POS, CTL, all 96 data words and the existing two-word
terminator, then updates the next-field Copper pointers. The current DMA bank
is never modified. This is 2,400 additional Chip bytes; pixels, frame IDs,
palette, player geometry, six attached channels and line-100 list staging stay.

Host tests use the runtime stream builder and prove immutable source data,
correct POS/CTL, complete pixel/terminator copies, independent banks and no
mutation of the active bank. The rejected VSTOP clipping experiment is not
reintroduced. The diamond Bob, enemy Bobs and line-253 pass are unchanged.
Supplied FS-UAE/HD review rejects alpha.30. The later recording catalogued as
`testresults/Phase 6C.1-alpha30-rejected-strider-flicker-persists.mov` shows
heavy flicker and also an isolated Strider failing while the other is far away.
The sprite-stream hypothesis is therefore disproved. No ADF gameplay or
real-A1200 result is claimed.

`make test`, `make PYTHON=../.venv/bin/python3` and
`make release PYTHON=../.venv/bin/python3` pass without compiler warnings.
Release validation round-trips all four ADF-only SPR1 streams and verifies the
bootable DOS1/FFS image. Alpha.30 uses 1,551 blocks (775 KiB), leaving 209 free.
This is host build/package evidence only.

## 15 August 2026: alpha.31 camera-culls resident water animation

The rejected alpha.30 quit diagnostic records 3,525 PAL wraps in 3,844 passes.
Its 237-line peak starts at line 253 and ends at line 178 at camera 810 with one
beetle, two Striders, two collectibles and two water updates. Neither resident
water strip (x=1584 and x=2432) intersects that camera. Those sixteen unnecessary
four-plane clean/display blits extend displayed FRONT16 mutation through the
upper Strider scanlines, where sequential plane writes can expose transient
mixed palette pens.

Alpha.31 removes the rejected 2,400-byte runtime sprite-bank experiment and
keeps the original player stream contract. Water animation now owns a drawn
frame per resident strip and blits only strips intersecting the camera plus a
16-pixel margin. A stale off-screen strip synchronizes directly to the current
frame on entry. Host boundary tests prove both water strips are excluded at
camera 810 and cover entry/exit edges. The world diamond, water art/cache,
enemy art and geometry, FRONT16 layout, line-253 scheduling and Bob restore/draw
order are unchanged. Focused FS-UAE/HD review remains pending; no ADF gameplay
or real-A1200 result is claimed.

`make test`, native `make PYTHON=../.venv/bin/python3` and release
`make release PYTHON=../.venv/bin/python3` pass. The bootable alpha.31 ADF uses
1,551 blocks (775 KiB), leaving 209 free. This is host package evidence only.

Supplied FS-UAE/HD review then accepts both water strips while reporting that
the Strider glitch is improved but not gone. The preserved alpha.31 recording
and quit log show sporadic upper-Strider wrong-colour pixels, 6,599 wraps in
6,796 passes and a 219-line zero-water peak at camera 797. Alpha.31 is therefore
accepted for water culling but rejected as a complete Strider correction.

## 15 August 2026: alpha.32 prioritizes upper enemy Bobs

The remaining corruption occurs on the upper Strider because all enemy restores
and collectible work precede enemy redraw, while slot order can leave that Bob
behind lower actors. Alpha.32 makes both enemy restore and draw traversal a
stable ascending-Y order. Equal-Y enemies retain their original slot order;
the existing projectile, collectible and enemy family priority is unchanged.
This moves upper Strider completion ahead of lower Strider/beetle work without
removing an update or changing total Blitter work.

A host regression proves ascending order and stable equal-Y ordering. Beetle
hit geometry and water visibility regressions still pass. Enemy art, animation,
geometry, masks, FRONT16 layout, collectible hover, world diamond, line-253
start and accepted alpha.31 water culling are unchanged. Focused FS-UAE/HD
review remains pending; no ADF gameplay or real-A1200 result is claimed.

`make test`, native `make PYTHON=../.venv/bin/python3` and release
`make release PYTHON=../.venv/bin/python3` pass. The bootable alpha.32 ADF uses
1,553 blocks (776 KiB), leaving 207 free. This is host package evidence only.

Supplied alpha.32 FS-UAE/HD evidence then accepts the large improvement and
separated Strider stability, while preserving a small remaining head/horn/crest
flicker only as the two upper Striders walk through each other. Its quit log
records a 214-line peak with two Striders and zero water updates.

## 15 August 2026: alpha.33 merges overlapping Strider restores

Two intersecting 64x64 Striders previously restored both complete rectangles,
copying their shared background area twice across four displayed planes before
either unchanged Bob draw completed. Alpha.33 detects only intersecting drawn
Strider rectangles and restores their bounding union once. Both drawn flags and
diagnostic counts retain per-Strider semantics. Separated enemies retain their
ordinary restore path and the stable ascending-Y draw order remains unchanged.

A host regression proves horizontal and vertical union bounds and rejects
edge-touching rectangles. Existing vertical-order, water-culling and beetle-hit
tests still pass. Art, masks, geometry, animation, family priority, collectible
hover, diamond and water are unchanged. Focused FS-UAE/HD overlap review remains
pending; no ADF gameplay or real-A1200 result is claimed.

`make test`, native `make PYTHON=../.venv/bin/python3` and release
`make release PYTHON=../.venv/bin/python3` pass. The bootable alpha.33 ADF uses
1,555 blocks (777 KiB), leaving 205 free. This is host package evidence only.

MrDig's supplied FS-UAE/HD retest accepts alpha.33: separated Striders and the
head/horn/crest overlap remain stable. A separate short recording then exposes
an unrelated projectile-occlusion defect at the narrow floor pillar. A flush
crouch shot appears beyond the pillar and damages the beetle behind; moving
slightly left makes the same shot collide with the pillar.

## 15 August 2026: alpha.34 sweeps projectile collision from the physics edge

Player shots retain their authored visual muzzle position, which is five pixels
beyond Sparkpaw's 32px physics box when facing right. The old update advanced
about nine pixels per frame, sampled only the new leading-edge midpoint and
dispatched enemy damage before solid collision. A muzzle next to a narrow wall
could therefore begin inside/beyond it or skip it between samples.

Alpha.34 adds a collision-only X sample initialized at Sparkpaw's physical front
edge. Each update visits every crossed pixel through the new projectile leading
edge in travel order and tests solid geometry before enemy damage. Enemy shots
use their existing leading edge as the initial sample. Render origins, plasma
pixels, velocity, Y samples, impact lifecycle, projectile capacity and beetle
geometry are unchanged. A host regression proves contiguous sweeps in both
directions; all earlier beetle, water and renderer ordering tests still pass.
Focused FS-UAE/HD review remains pending; no ADF gameplay or real-A1200 result
is claimed.

`make test`, native `make PYTHON=../.venv/bin/python3` and release
`make release PYTHON=../.venv/bin/python3` pass. The bootable alpha.34 ADF uses
1,557 blocks (778 KiB), leaving 203 free. This is host package evidence only.

## 15 August 2026: alpha.35 starts stock-68020 profiling and aligns HD loading

MrDig accepts alpha.34 projectile occlusion in supplied FS-UAE/HD testing, then
identifies that recent reviews had inadvertently used a 68030 configuration.
The new 68020 full run shows substantial frame loss and renewed Strider flicker.
Its quit log records 2,293 wraps in 3,015 passes and a 311-line peak from line
253 to line 252, containing three projectiles, two beetles, one Strider and two
collectibles with no water update. General 68020 performance is rejected.

Alpha.35 removes the first proven invisible cost: active projectiles outside
the camera plus 16-pixel margin are no longer drawn into the resident 3072px
foreground. A previously visible projectile is still restored before it is
parked. A host boundary regression covers both margins. The production log now
records accumulated and peak raster-line costs separately for projectiles,
enemies, collectibles, water and splash, and identifies itself as alpha.35
instead of the stale alpha23 label. The measurement build preserves the 4+3
dual playfield, line-100 staging, line-252 HUD switch, line-253 Bob start,
packed caches, family order and no-CPU-RMW displayed-Chip rule.

Separate supplied real-Amiga evidence shows the alpha.34 floppy/ADF package
working but the HD build entering horizontal display corruption immediately
after CHARGING. The main HD executable had used raw SPBM for four large assets
while the ADF used verified SPR1 streams. Alpha.35 packages and decodes those
same four packed assets on HD, reducing disk I/O and removing that principal
transition-path difference. Its streaming CRC changes from eight polynomial
bit iterations to two nibble-table steps per output byte; the standard
`123456789` CRC32 vector proves identical results. This is a targeted candidate, not a hardware fix
claim. FS-UAE 68020 timing and real-Amiga HD acceptance remain pending; exact
ADF gameplay parity is not inferred. Diamond art and renderer presentation are
unchanged.

All seven host regressions, native `make PYTHON=../.venv/bin/python3` and
`make release PYTHON=../.venv/bin/python3` pass. The bootable alpha.35 ADF uses
1,560 blocks (780 KiB) and leaves 200 free. This is host package evidence only.

## 15 August 2026: alpha.36 rolls rejected packed loading back out of HD

Supplied alpha.35 FS-UAE/HD evidence reaches LOADING but remains there for the
rest of a 30.8-second recording; CHARGING and gameplay never appear. The user
also observes green marks at upper left and intermittent display glitches. This
directly rejects the only alpha.35 loading-path change: compiling and packaging
the normal HD executable with the ADF's SPR1 decoder.

Alpha.36 restores raw SPBM loading and raw HD package contents exactly for the
four large gameplay assets. ADF keeps its required packed streams and the
independently verified two-nibble CRC optimization. The off-screen-projectile
cull, per-family timing profiler, Copper/display structure and all gameplay
changes remain intact. This rollback requires a fresh FS-UAE HD loading gate;
it does not establish the cause or correction of alpha.34's separate real-Amiga
post-CHARGING display corruption.

All seven host regressions, native `make PYTHON=../.venv/bin/python3` and
`make release PYTHON=../.venv/bin/python3` pass. The alpha.36 ADF uses 1,561
blocks (780 KiB), leaving 199 free. This is host package evidence only.

## 15 August 2026: alpha.37 tightens the measured dominant enemy Bob family

Supplied alpha.36 FS-UAE/HD evidence confirms that raw loading again enters
gameplay, then rejects performance and enemy presentation on 68020. Its 4,139-
frame log contains 3,709 wraps and a 310-line worst pass with two Striders, one
projectile and one collectible. Enemy restore/draw dominates: 354,807
accumulated raster lines and a 263-line peak, versus collectibles at 179,035/54,
projectiles at 22,432/257, water at 5,692/25 and splash at 1,854/11.

Mask inspection shows that the fixed 64x64 Strider blits include large
transparent margins: common frames occupy roughly 42x55 pixels. Beetle art
starts around row six despite fixed 32x24 work. Alpha.37 computes exact opaque
bounds for every facing and animation frame while building the existing packed
cache, normalizes pixels into those per-frame rectangles, and records the exact
world rectangle for restore. Intersecting Strider restores now union unequal
tight rectangles. No visible source pixel, collision cell, animation cadence,
family order, camera rule or Copper/display contract changes. Focused FS-UAE
68020 timing and clipping/residue review remain pending.

All seven host regressions, native `make PYTHON=../.venv/bin/python3` and
`make release PYTHON=../.venv/bin/python3` pass. The alpha.37 ADF uses 1,563
blocks (781 KiB), leaving 197 free. This is host package evidence only.

## 15 August 2026: alpha.38 replaces the visible-world Bob pass

Supplied alpha.37 FS-UAE/68020 evidence rejects both presentation and timing:
enemy sprites are damaged and frame rate remains unacceptable. The broader
audit confirms the gameplay/HUD split is sound, but finds one full 3072x256
foreground was restored and redrawn while displayed. Its nominal 59-line
post-HUD window could never contain measured 251--310-line passes. Production
diagnostics additionally forced a WaitBlit after every family, preventing
normal CPU/Blitter overlap. Jumping adds CPU collision probes because vertical
movement resolves per pixel, but Sparkpaw is a hardware sprite and this is not
the dominant source of enemy tearing.

Alpha.38 uses two 512x256 displayable FRONT16 viewport buffers. A 96-pixel
camera guard preserves partially visible Bobs. Each frame updates diamonds and
water only in the clean resident world, Blitter-copies its 512x208 camera-local
slice into the hidden viewport, draws splash/enemies/projectiles there, waits
for completion and only then publishes Copper pointers. No operation writes the
displayed foreground and transient restore passes are gone. Dual playfield 4+3,
the line-252 separate HUD, rear parallax, Bob family priority, packed caches,
camera culling and player hardware sprites remain. The invasive family waits
are removed from the normal diagnostic executable. FS-UAE/68020 behavior,
ADF gameplay parity and real-A1200 behavior remain unverified.

## 16 August 2026: alpha.39 restores the accepted in-place baseline

Supplied alpha.38 FS-UAE/HD evidence rejects the hidden-viewport rewrite on
both 68020 and a separate 68030 check. Its log records 3,324 wraps in 3,722
compositions and a 311-line worst composition. Copying a 512x208 four-plane
foreground slice every update adds about 53 KiB of destination traffic plus
source reads before dynamic Bobs, so this route is abandoned rather than
incrementally tuned.

Alpha.39 restores the alpha.33-style resident foreground restore/draw renderer
and line-253 pass. Alpha.37 tight enemy bounds and all alpha.38 viewport code
are removed. Accepted beetle/projectile collision, off-screen projectile and
water culling, Strider overlap ordering and raw HD loading remain. The normal
diagnostic retains whole-pass logging and left-mouse exit but does not insert
per-family WaitBlit barriers. FS-UAE/68030 presentation is the first gate,
followed by a measured 68020 baseline; neither is yet accepted.

Default releases now contain LHA, ZIP and ADF plus an extracted same-version
review drawer in `dist`. The source ZIP exceeds 100 MB and is created only on
explicit request with `tools/make_release.py --include-source`. Host tests,
native build and release validation pass. The alpha.39 ADF uses 1,564 blocks
(782 KiB), leaving 196 free; this is package evidence only.

## 19 August 2026: alpha.40 bounds real-HD raw reads

MrDig accepts alpha.39 presentation and cadence at 68030 in FS-UAE. He also
accepts the alpha.39 ADF on a real A1200. Both alpha.39 HD and ADF work in
FS-UAE, but supplied real-A1200 HD evidence rejects the transition after
CHARGING: HDMI becomes nearly black with horizontal remnants and the native CRT
shows moving cyan/white noise bands. This rules out an Indivision-only symptom.

The remaining launch-path difference is asset transfer. Packed ADF gameplay
assets are streamed through a 512-byte reader. Raw HD instead issued one DOS
Read for any plane whose file and bitmap row widths match, including 98,304
bytes for each 3072x256 world plane. Classic filesystem/device MaxTransfer and
Mask settings exist specifically because some drivers corrupt unsuitable or
oversized direct transfers. Alpha.40 routes raw headers, palettes, planes and
masks through the same 512-byte staging principle as the working ADF reader,
then CPU-copies into the final allocation. DOS therefore never receives a
complete world plane or arbitrary final DMA address. Asset formats, resident
memory, renderer, Copper, gameplay and ADF decoding are unchanged.
Real-A1200 HD verification remains pending. Host regressions, native build and
release validation pass. The alpha.40 ADF uses 1,566 blocks (783 KiB), leaving
194 free; this is package evidence only.

## 19 August 2026: Analogue Pocket exposes broader alpha.39 timing sensitivity

Supplied alpha.39 ADF footage from the Analogue-Amiga FPGA core, configured for
68020 with caches disabled, AGA/PAL/Turbo, 2 MB Chip and 32 MB Fast, shows
frequent transient red/magenta wedges, fragmented dynamic objects and partial
HUD-heart corruption. The failure is not confined to overlapping Striders.
Because the HUD uses a separate bitmap selected by the Copper at line 252, this
adds missed frame phase or late Copper/HUD publication to the first profiling
questions; static Strider art alone cannot explain the complete observation.

The core is FPGA/Minimig-derived rather than Amiberry software emulation. Its
result is retained as a useful timing-sensitivity stress test, not as proof of
cycle-equivalent stock-A1200/68020 behavior and not as a reversal of the
separately accepted alpha.39 ADF result on the real A1200/68030. No production
code or release changes for this evidence review.

## 19 August 2026: alpha.40 real-HD failure is isolated to Chip RAM

MrDig retested the unchanged alpha.40 HD package on the same real A1200/68030.
Boot With No Startup-Sequence leaves nearly all 2 MB Chip RAM available and the
level loads and plays correctly. A normal Workbench with 1,430,032 free Chip
bytes reproduces the alpha.39/40 post-CHARGING corruption. Reducing that same
Workbench to two colours raises free Chip RAM to 1,924,888 bytes, after which
alpha.40 again loads and plays correctly. This establishes real-hardware HD
operation with sufficient Chip RAM and isolates the remaining normal-Workbench
defect to peak/resident Chip demand. It rejects MaxTransfer as the current
primary cause without generalizing acceptance to the low-memory launch.

The next correction is a measured memory-layout checkpoint. The separate
LOADING and CHARGING screens currently allocate complete 320x256 six-plane
bitmaps although only the word LOADING changes to CHARGING; one retained floppy
image with a changed status area is acceptable. The resident gameplay images
also include rows below the line-252 HUD switch and a rear span beyond the
maximum quarter-scroll fetch. Remove only demonstrably unreachable bitmap data,
consolidate the status screen and place CPU-only data in Fast RAM, while keeping
all custom-chip DMA sources in Chip RAM. The production target remains 2 MB
Chip plus 8 MB Fast.

Official HD and ADF releases no longer need left-mouse Workbench restoration or
runtime logging. Those facilities belong only to explicitly named debug builds
for focused investigation; a later WHDLoad package may own a separate exit
contract. This policy is recorded before implementation, and no new release is
created by this documentation step.

The debug contract is subsequently tightened further. Debug builds do not need
to return to Workbench either. Startup and pre-takeover diagnostics may be
written and closed immediately while DOS is available. Gameplay diagnostics
must accumulate in Fast RAM because filesystem calls are unsafe after Sparkpaw
has disabled interrupts and taken over the machine. A deliberate flush action
will restore only the OS services necessary to write and close the file, then
wait for reset without reopening Workbench. Reset alone cannot preserve an
unflushed RAM buffer. Official builds contain none of this input, restoration,
buffering or file code.

## 19 August 2026: alpha.41 reduces Chip RAM without reducing quality

Alpha.41 implements the measured normal-Workbench memory checkpoint. Both
3072-pixel FRONT16 buffers retain exactly the 208 rows displayed before the
line-252 HUD switch instead of 256 rows. REAR8 retains the complete 1024-pixel
authored master and 208 displayed rows; the removed repeated tail lies beyond
the maximum word-aligned quarter-scroll plus 42-byte fetch. A host regression
proves that final fetch ends within the retained span. This saves 362,496
resident Chip bytes across the two foregrounds and rear without removing any
reachable pixel.

The complete fixed-size 28-frame, two-facing Strider cache is built
bit-identically in Fast RAM. Four 2,560-byte Chip slots, one per possible active
enemy, receive a complete frame only when its frame or facing changes. The 62
player poses likewise remain complete Fast-RAM DMA-stream masters; two
alternating six-channel Chip stages receive their exact words before Copper
publication. These changes save a calculated 133,120 and 146,400 Chip bytes
respectively without bounds cropping, palette changes or frame removal.

LOADING and CHARGING now retain one complete accepted floppy bitmap. While
fully faded black, a 224x40 six-plane CPU-only Fast-RAM patch replaces only the
status band, then the same bitmap fades back in. This removes about 54 KiB from
the status-screen peak. Total calculated permanent Chip saving is 642,016
bytes. CPU-only asset allocations explicitly require the production minimum's
8 MB Fast RAM; every display, Copper, sprite stage, audio sample and Blitter
source remains in Chip RAM.

The official HD target no longer defines render diagnostics; like ADF it is
reset-to-exit and contains no mouse/log path. The separate renderdiag target
buffers during takeover, restores only scheduling/interrupt services on its
flush gesture, writes and closes the log, and waits for reset without reopening
Workbench. Host regressions and native builds pass. FS-UAE presentation,
normal-Workbench real-A1200 HD and ADF regression acceptance remain pending.
Release validation passes; the alpha.41 DOS1/FFS ADF uses 1,177 blocks
(588 KiB) and leaves 583 free. The normal executable and extracted HD review
executable are byte-identical. The current Sparkpaw release set is LHA, ZIP,
ADF and one extracted same-version drawer; no source ZIP is produced.

Supplied real-A1200/68030 HD testing subsequently accepts alpha.41 startup and
gameplay entry from a normal Workbench with about 1.4 MB Chip RAM free. This
closes the real-HD Chip-RAM gate without lowering Workbench colours or using a
minimal boot. Alpha.41 ADF regression remains unclaimed.

The title timing is audited against its accepted implementation: display
takeover still holds black for 35 PAL frames, fades over 24 frames and starts a
225-frame fully visible hold only after the fade. Alpha.41 did not shorten this
Indivision stabilization contract. Its smaller production load path can make
the program reach display takeover sooner, which is distinct from shortening
the on-screen timing.

Two supplied real-hardware recordings are catalogued as
`Phase 6C.1-alpha40-real-a1200-two-strider-glitches.MOV` and
`Phase 6C.1-alpha41-real-a1200-two-strider-glitches.MOV`, with metadata and
scope in matching sidecars. Both show the reported intermittent corruption and
cadence loss in the first two-Strider scene; occasional beetle corruption is
also reported. Camera sampling of a PAL screen prevents frame-exact raster
claims, but the user's direct observation confirms that the fault is visible
on the hardware. Its presence in alpha.40 proves that alpha.41's Fast-RAM
masters and staging are not its point of introduction. Focused Bob scheduling
and deadline safety now precede broad stock-68020 optimization.

A Gotek user also reports repeated early-to-late track motion during ADF
loading. `sparkpaw/docs/ADF_LOAD_OPTIMIZATION_PLAN.md` records a deferred,
measurement-first route: map FFS blocks against actual runtime open order,
benchmark a byte-identical layout-only disk, and consider a sequential ADF-only
container only if physical layout is insufficient. No ADF loading change is
implemented in alpha.41.

## 19 August 2026: pre-renderer-redesign recovery checkpoint

Planning after the supplied alpha.41 hardware evidence rejects treating a
one-visible-Strider limit as the primary fix. The current full-world in-place
Bob path scales presentation correctness with load: two Striders coincide with
the strongest corruption and cadence loss, killing one nearly removes both,
and a repeatable world position can corrupt the HUD. A short diagnostic build
will first correlate Bob completion, raster wraps, per-family cost and active
Copper publication; it is deliberately bounded evidence gathering rather than
an extended profiling project.

The preferred structural proof then uses two complete Copper lists and compact
double-buffered rolling gameplay targets. Hardware fine scroll moves the view,
only newly exposed tile columns are constructed, each target owns its restore
history, and neither CPU nor Blitter writes the displayed target. A
representative slice must cover the repeatable HUD point, two Striders, water,
projectiles and collectibles before full-route migration. Camera-tripwire actor
dormancy is part of the proof; max-one-visible-Strider remains only a fallback
budget or intentional encounter choice. Current foreground art, all sprites,
colours, animation, separate HUD and the three-plane quarter-speed parallax are
non-negotiable invariants. The Analogue Pocket 68020/no-cache configuration is
an explicit additional stress regression because its supplied evidence shows
widespread Bob, gameplay-field and HUD corruption; it remains distinct from
FS-UAE and real-A1200 verification.

`sparkpaw/docs/RENDERER_GLITCH_CORRECTION_PLAN.md` records the complete gates,
memory alternatives and classic/Scorpion research. This is a planning/recovery
checkpoint on the unchanged alpha.41 executable, not a new SemVer release and
not FS-UAE, ADF, Analogue Pocket or hardware acceptance.

## 19 August 2026: bounded Stage 1 instrumentation and rolling design

The separately named HD `sparkpaw-renderdiag` build buffers the newest 1,024
complete frame records (about twenty PAL seconds) in Fast RAM. The packed ADF
variant retains 512 frames (about ten seconds): its roughly 220 KiB text log
fits the approximately 306 KiB remaining floppy space, while the first
1,024-frame ADF attempt correctly failed to create a roughly 427 KiB log. Both
write only through the established safe debug flush. The rings are derived
from the supplied six-to-fourteen-second HUD and Strider recordings; the ADF
run is deliberately limited to the shorter deterministic HUD crossing.
The first two writable-ADF attempts produced no file despite a successful HD
flush. The first exceeded free floppy space; after reducing the ring, the image
still remained byte-identical because the debug flush restored interrupts and
DOS scheduling but left all custom-chip DMA disabled. The debug-only flush now
restores the saved system DMA mask before `Open`/`Write`/`Close`, allowing DF0's
disk DMA to persist the bounded log. Production exit and renderer paths are
unchanged.
They sample update, live-Copper publication, Bob entry and final
Blitter completion; retains deadline/wrap flags, camera/player and all four
enemy slots, family restore/draw counts, both sets of seven intended bitplane
pointers, HUD index, Copper address/generation and Blitter-busy state. The
ordinary HD and ADF targets remain free of this code and input path. The debug
ADF uses the identical packed alpha.41 asset route and must remain writable so
the post-click flush can preserve `renderdiag.log` on DF0.

A host-tested frame-phase helper makes skipped whole fields an explicit input
instead of pretending line-number comparison can observe them. The current
Stage 1 runtime trace can establish observed raster wraps; adding an independent
timer is required before claiming exact multi-field duration from a sampling
gap. No supplied run has yet correlated a visible fault, so the suspected live-
Copper and displayed-Bob races remain hypotheses rather than accepted causes.

The completed isolated Stage 2+4 design specified a measured 336/352-pixel
target width, two complete Copper lists,
per-target dirty ownership, inactive-only water/diamond maintenance, atomic
publication and persistent camera-tripwire actor dormancy. It preserves all
alpha.41 visual/gameplay contracts and excludes the rejected alpha.38 full
viewport copy and a one-visible-Strider cap. No renderer implementation,
FS-UAE, ADF, Analogue Pocket or real-hardware acceptance is claimed here.

## 19 August 2026: Stage 2 atomic-Copper isolation prototype

The first isolated prototype step now owns two complete Copper lists. Runtime
updates clone the displayed list into the inactive list, patch only that
inactive copy, and publish it through `COP1LC` only inside the pre-playfield
deadline. A missed deadline retains the previously displayed list rather than
publishing partially prepared state. Host tests cover the ownership,
publication and rolling-target contracts that the subsequent compact target
implementation must satisfy.

This Stage 2 artifact isolates HUD and Copper publication only. Gameplay Bobs
still use the alpha.41 in-place renderer, so enemy-load glitches and cadence
drops can remain. It is packaged separately as a writable diagnostic ADF for
the repeatable real-A1200 HUD crossing and as an HD-transfer LHA; it does not
replace alpha.41 and carries no FS-UAE, ADF or real-hardware acceptance claim.

## 20 August 2026: Stage 2 rejection and fixed-VBlank correction

Supplied FS-UAE/HD and real-A1200/68030-HD evidence rejects the first Stage 2
artifact. The recordings show widespread composite instability, alternating
HUD damage and Bob ghosts. Its FS-UAE log reports 4,452 field crossings in
4,576 Bob passes. The real-hardware log reports 2,593 in 2,942, with the
retained ring also showing 497 late update entries, 106 line-252 misses, 124
late Bob entries and 866 consecutive records retaining the same Copper address.
The raster-only diagnostic cannot measure skipped whole fields exactly, so its
near-full/zero multi-wrap durations remain lower-bound or ambiguous.

Source inspection identifies a concrete Stage 2 scheduling error: after Bob
completion the prototype restarted the prepared Copper list on any numerical
line below 44. On FS-UAE the final trace completes on lines 4..39, causing the
large Copper preamble to restart at a variable and sometimes very late point.
On real hardware multi-field passes also skip publication and re-enter later
phases. Stage 2B therefore waits through line 300 for one explicit new PAL
boundary, publishes only on lines 0..4, and cannot start another simulation
update while that generation is pending. The trace now records the actual
boundary and publication result.

Stage 2B remains a focused HUD/Copper isolation gate. It intentionally retains
the known-unsafe alpha.41 in-place Bob path and is not the compact rolling
target, a release, or an FS-UAE/ADF/real-hardware acceptance claim.

Supplied Stage 2B FS-UAE/HD follow-up reports normal presentation again but
worse cadence, especially while jumping and with more enemies visible. The log
correlates that result directly: all retained publications succeed at hardware
line 0 or 1 and the Copper addresses alternate without repetition, while 589
of 1,023 update intervals span 624 rather than 312 raster lines. The safety
wait therefore converts the old in-place Bob overrun into an explicit extra
displayed field in 57.6% of the retained window. This accepts only the removal
of Stage 2's new presentation regression in that supplied FS-UAE/HD run;
performance is rejected. The result closes the atomic-publication experiment
and directs the next implementation to the non-displayed gameplay target,
rather than weakening the fixed boundary or optimizing the unsafe path.

## 20 August 2026: first compact inactive-target prototype

The Stage 4 test build replaces the prototype's live resident FRONT16 display
with two compact 448x208 display targets and matching compact clean partners.
The 448-pixel width is derived from the 336-pixel DMA fetch, fine-scroll range,
the established 32-pixel actor margin and a complete 64-pixel Strider guard;
only the existing 336-pixel span is fetched. REAR8 remains resident and keeps
quarter-speed parallax.

Each hidden target owns enemy, projectile and splash restore history. After
restoring that history, an aligned camera crossing retains the overlapping
window and imports only the newly exposed 16-pixel world strip. Current water
and diamond patches synchronize to both the hidden clean/display pair before
new Bobs are drawn. Composition begins immediately after simulation rather
than at line 253, finishes on the hidden target, and publishes that target with
its inactive Copper list at fixed VBlank line 0..4. The displayed target is not
an intended CPU or Blitter destination.

Host tests cover fixed publication bounds, world-edge origin clamping, actor
guard bounds, both-direction word-window shifts, target ownership and existing
gameplay contracts. Native build success alone is not FS-UAE, ADF or hardware
acceptance; first review is deliberately FS-UAE/HD before packaging claims or
real-machine testing.

Supplied Stage 4 FS-UAE/HD review reports improved cadence and no observed
visual glitches, with smaller remaining drops noticed mainly while jumping.
Its retained log has zero ownership violations, 1,024 successful publications
and uninterrupted Copper alternation. Of 1,023 update intervals, 946 remain at
312 PAL lines and 77 take 624; every one of those 77 follows a compact-target
recenter and no non-recenter frame misses. This accepts only that supplied
FS-UAE/HD presentation run. ADF and real-A1200 results remain pending.

## 20 August 2026: Stage 4B recenter-cost experiment

The Stage 4 log isolates the remaining cadence loss to physical target shifts,
so Stage 4B changes only that operation. The hidden target grows from 448 to
512 pixels while the displayed fetch remains 336. The camera can move through
a safe 64-pixel hysteresis band with complete 64-pixel Bobs at either visible
edge; a target is therefore recentered per 64 pixels instead of per 16. The
overlap is moved by the hardware Blitter, ascending when moving towards lower
addresses and descending when moving towards higher addresses, before the new
canonical strip is imported. Diagnostic flag 16 marks every actual recenter.

This is a measured intermediate step, not the final no-copy ring by assertion.
The Commodore overlap-copy contract, the reverse-engineered Speedball 2
two-screen pitch/tile-edge refresh, and the existing Andrew Braybrook rolling-
buffer design reference all favour eliminating physical shifts entirely if a
periodic stall remains. Stage 4B keeps that larger seam-aware Bob/ring change
separate. No FS-UAE, ADF or real-hardware acceptance is claimed for Stage 4B.

Supplied Stage 4B FS-UAE/HD footage rejects that intermediate recenter design.
It shows no observed foreground, Bob or HUD corruption, but steady walking in
both directions repeatedly changes from normal roughly 2--3-pixel camera steps
to a short hold/small step followed by an approximately 5-pixel catch-up. The
user describes the result as a periodic hiccup, more conspicuous than Stage
4A's softer frame-rate loss. The diagnostic log was flushed roughly three
minutes after recording; its retained circular window covers the later idle
period and is not correlated evidence for the movie. Do not spend another
iteration tuning physical-shift frequency: proceed to the planned no-copy,
seam-aware tile/ring window. ADF and real-A1200 Stage 4B remain untested.

## 20 August 2026: Stage 4C no-copy tile-ring prototype

Stage 4C retains the visually clean Stage 4A/4B hidden-target ownership,
target-local enemy/projectile/splash histories and fixed-VBlank Copper
publication, but removes the rejected physical bitmap shift. Each target now
contains one logical 512x208 FRONT16 ring repeated three times across a
1536-pixel physical stride. The unchanged 336-pixel Copper fetch starts in the
middle copy, so both fetches and complete 64-pixel edge Striders cross a ring
seam without moving retained pixels or introducing per-scanline Copper splits.

World columns have fixed modulo-512 slots. At each aligned crossing only the
new canonical 16-pixel column is copied to its three aliases in the inactive
clean/display pair. Bob world positions map into guarded physical coordinates;
the existing per-target histories store those physical positions, preserving
restore unions for Striders and ordinary restore behavior for beetles,
projectiles and splashes. Water and collectible regions synchronize only when
their canonical state differs from that target. REAR8 parallax, HUD, player
hardware sprites, family ordering, assets, palettes and gameplay remain
unchanged.

Host coverage sweeps all aligned cameras through both logical seams and both
world edges, proving the 336-pixel fetch and every 64-pixel actor guard remain
inside the 1536-pixel physical allocation with matching world/ring slots. This
is still a separately named prototype. Build and host success imply no FS-UAE,
ADF or real-hardware acceptance.

Stage 4C's first FS-UAE HD run was rejected despite excellent apparent frame
rate: projectiles, beetles and Striders left persistent copies at their old
positions. Its 1307-frame log reported 47 ring updates, zero ownership
violations and successful atomic publication, isolating the failure from the
Copper hand-off. The cause was a coordinate-domain mismatch in Bob restore:
histories held physical triplicated-ring X positions, but restore subtracted
the world-window origin again. Stage 4D changes only that restore address and
packages a separately named HD retest; it has no FS-UAE, ADF or hardware
acceptance yet.

The Stage 4D FS-UAE HD retest reports smooth cadence and no remaining moving-
Bob trails. Its 4,925-frame log retains zero ownership violations, and every
retained trace publication succeeds with the Blitter idle at the sampled
boundaries. A narrow strip immediately above the HUD still flickers with
camera motion. The Copper previously waited until line 252 horizontal 1 before
replacing seven bitplane pointers, fine scroll and modulos for that same
scanline. Stage 4E moves only the start of that existing setup to the preceding
horizontal blank. It preserves the Stage 4D no-copy ring and has no supplied
FS-UAE, ADF or hardware result yet.

Stage 4E's supplied FS-UAE HD screenshot reduces the moving boundary remnant
to the far-left pixels only. Its retained 1,023 cadence intervals contain
1,022 one-field updates and one two-field update, about 49.95 updates/s, with
zero ownership violations. This supports keeping the no-copy renderer and
rejects changing ground art to mask the boundary. Stage 4F adds a narrow
bitplane-fetch guard around the already-early HUD pointer switch, preventing a
retained gameplay shifter word from entering the first HUD pixels. It also
writes cadence totals and effective FPS directly to future diagnostic logs.

Stage 4F is rejected by its first supplied FS-UAE HD screenshot: temporarily
disabling and reenabling BPLCON0 corrupts and horizontally displaces the entire
HUD. The user stopped without a meaningful gameplay traversal, so its short
49.52-updates/s sample is not comparative gameplay-performance evidence. Its
zero ownership violations still keep the failure localized to the fetch-phase
experiment rather than the rolling target. Stage 4G removes both
BPLCON0 writes, preserves Stage 4E's continuous fetch, and advances the setup
WAIT from horizontal 0xd9 to 0xd1, immediately after the 0xd0 gameplay
DDFSTOP. Cadence logging remains enabled.

## 20 August 2026: alpha.42 rolling-renderer production migration

Supplied Stage 4G FS-UAE/HD testing accepts the final prototype presentation:
no projectile/enemy ghost trails and no remaining moving pixels at the
gameplay/HUD boundary. Its 1,983 cadence intervals contain 1,952 one-field and
31 two-field updates, zero three-plus-field updates and 49.23 effective FPS;
target ownership violations remain zero. This is the accepted FS-UAE/HD gate,
not ADF, Analogue Pocket or real-A1200 evidence.

Alpha.42 compiles the same no-copy renderer into both normal HD and packed-ADF
executables. Official builds contain no diagnostic trace, mouse exit or DOS
log strings. The preserved contracts are FRONT16, REAR8 quarter-speed parallax,
all palettes/assets, six-channel player sprites, animation IDs, 320x48 HUD,
collision and gameplay behavior. The separate renderer diagnostic now profiles
the production rolling path and writes explicit cadence totals/effective FPS.

The required native and release builds, all host regressions, LHA integrity,
ZIP integrity and bootable DOS1/FFS ADF extraction checks pass. The alpha.42
ADF uses 1,186 of 1,760 blocks (593 KiB) and leaves 574 free. Current artifact
SHA-256 values are: LHA cda03c46fb0a7855c8fc3d7f894817c2960b19418fca99b6713cd9081d988e3c,
ZIP 9c47c90a4e53729e38e5d20a0d43996bf19fd1ff2a6a4b9113f1f9c1f00bf23a,
ADF 56652e184acbcc93dbccbf3628d7616abd26dedf5126cc12a7965a0f0157f6cc.

Lessons and rejected branches are consolidated in
`sparkpaw/docs/RENDERER_GLITCH_CORRECTION_PLAN.md`. Obsolete per-stage transfer
instructions and prototype packagers are removed after migration. Supplied
logs/screenshots remain preserved in `sparkpaw/testresults`; generated Stage
1..4G transfer artifacts are no longer part of `dist`.

### Alpha.42 target-speed performance rejection

Two subsequent supplied recordings change the performance conclusion without
reopening the renderer-correctness result. `Renderer alpha42-rejected-fs-uae-
68020-low-cadence.mov` shows low/uneven cadence in the 68020 emulator
configuration. `Renderer alpha42-rejected-real-a1200-68030-low-cadence.MOV`
shows alpha.42 running worse than the pre-session build on the real A1200/68030
at about 34.5 MHz. Sampled frames retain an intact HUD and show no obvious
return of the earlier widespread corruption or Bob trails.

The Stage 4G 49.23-FPS log therefore proved deadline performance only on the
faster FS-UAE/68030 configuration. It cannot be extrapolated to the target CPU
budget. Alpha.42 performance is rejected on both newly supplied configurations;
ADF and Analogue Pocket remain untested. The no-copy ownership architecture
stays as the correctness baseline while the separate CIA-timed profiler from
`sparkpaw/docs/PERFORMANCE_68020_PLAN.md` isolates update, physics, simulation,
Bob submission, final Blitter wait and missed-publication costs. Future real-
hardware runs are deferred until an FS-UAE/68020 candidate shows a measured
gain without visual regression.

### Stage 5 measurement and AGA-fetch candidate

The CIA Timer-B profiler shows the renderer, not gameplay simulation, is the
68020 limit. Stage 5A's supplied FS-UAE/68020 log measured 26.30 effective
updates/s; the complete Bob pass averaged 2.406 PAL fields versus 0.511 for
the whole game update. Stage 5B gave the Blitter priority only during that
bounded pass. Its supplied log improved cadence to 28.27 updates/s and reduced
the average Bob pass to 2.209 fields, but the user still rejected its visible
framerate. Both logs report zero rolling-target ownership violations.

Stage 5C therefore isolates AGA 32-bit bitplane fetch. Seven resident planes
at the old 16-bit mode consume nearly every display DMA slot; the candidate
uses `FMODE.BPL32`, 44-byte fetches, longword-aligned 32-pixel coarse pointer
steps and AGA's extended 0..31 fine-scroll fields. The visible 336-pixel HUD is
unchanged, while its private stride becomes 352 pixels so the final 44-byte
fetch is valid. Sprite fetch remains 16-bit and all six attached player
channels remain enabled. Host scroll/alignment contracts and both native
production and diagnostic builds pass. Stage 5C has no supplied FS-UAE or
real-hardware result yet and is not accepted.

The initial Stage 5C transfer build is rejected before gameplay: both supplied
FS-UAE CPU configurations remained on CHARGING and showed stray upper-left
pixels. The defect was candidate construction, not a measured fetch result:
it assumed `AllocBitMap(352, ..., BMF_DISPLAYABLE)` must return exactly 44
bytes per row and still derived the HUD Copper modulo from the 42-byte source
asset. Revision 2 accepts graphics.library padding, allocates the blank plane
from the actual stride, derives both HUD modulos from that stride, and refuses
custom-chip takeover unless every 32-bit-fetch pointer and row is longword
aligned. Revision 2 still requires fresh FS-UAE evidence.

Supplied Stage 5C2 FS-UAE/68030 video rejects the revised candidate: Sparkpaw
appears to jump relative to the level during continuous traversal. The matching
log separates presentation from simulation. Outside one reset elsewhere in
the substantially longer diagnostic run, camera/player X advance by at most
about one/three pixels per update. The user confirms that no water fall or
level reset occurs in the 17.05-second MOV; the repeated visible displacement
is therefore the 32-bit bitplane
pointer/fine-scroll phase moving the world relative to the hardware sprite.
The same log measures 49.96 effective updates/s (2,786 one-field, two two-field,
zero three-plus intervals) and zero ownership violations. That demonstrates the
potential bandwidth benefit but not an acceptable renderer. Do not spend a
68020 or real-hardware run on this mapping.

Stage 5C3 corrects only the rejected phase formula. Pointer stepping remains
longword-aligned at 32 pixels, while fine scroll now preserves the accepted
FMODE=0 sequence for offsets 0..15 and uses AGA's extended half for 16..31:
delays 15..0 followed by 31..16. The fixed HUD likewise returns from delay 31
to its accepted delay 15. No Bob, gameplay, asset, palette, sprite, HUD artwork
or target-ownership contract changes. FS-UAE/68030 presentation is pending.

Supplied Stage 5C3 FS-UAE/68030 evidence removes the periodic teleport-like
jump but rejects the remaining absolute alignment: the left gameplay edge and
HUD inset/border are wrong, and hit contact appears visually displaced. The
log retains smooth camera/player coordinates, 49.84 effective updates/s and
zero ownership violations, so gameplay hitbox edits would only mask a display
origin error. The fixed-DDF 32-bit-fetch branch is rejected. A correct wide-
fetch design would need dynamically coordinated DDF start, pointer move,
modulo and scroll phase as used by established AGA display drivers; that is no
longer a short isolated register change and should not consume a 68020 or real-
hardware test before a separate proof.

### Stage 5D isolated AGA32 scroll calibration

Stage 5D does not modify or package the game. It is a standalone custom-chip
display that paints deterministic 16-pixel vertical bands and horizontal
reference lines, uses a structurally separate 48-line HUD bitmap, enables
`FMODE.BPL32`, and sweeps every fine-scroll phase across the 32-pixel coarse
pointer boundary. Four PAL fields are held per phase so a recording can
separate a constant absolute-origin error from a phase-dependent jump.

This replaces further empirical full-game pointer tweaks with a bounded proof.
The local Blitz Basic 2 reference shows `DisplayAdjust` examples coordinating
fetch width, DDF and display modulo, while the local WinUAE source models the
FMODE-dependent fetch size and special unaligned pointer behavior explicitly.
Linux `amifb.c` is used only as an additional register-calculation reference;
no Linux component enters Sparkpaw. A phase-dependent fault rejects this
mapping before another game, 68020 or real-hardware build is requested. No
emulator or hardware acceptance is claimed.

Supplied Stage 5D FS-UAE/68030 evidence accepts the isolated phase ordering:
the synthetic main grid advances continuously through repeated 31-to-32
coarse-pointer transitions, the separate lower reference stays fixed, and no
phase-dependent border corruption is visible. The clean-exit log covers 800
PAL fields with longword-aligned 88-byte rows and no Copper overflow. This does
not yet solve Stage 5C3's absolute origin because the proof's allocated sprite
streams are transparent; a second calibration must make a fixed hardware
sprite marker and matching bitplane coordinate visible before any full-game
integration or 68020 test.

Stage 5D2 adds the missing visible hardware-sprite and bitplane markers. The
supplied FS-UAE/68030 recording retains the accepted smooth 32-phase sequence
but rejects absolute alignment: at phase zero the bitplane coordinate is about
one 16-pixel fetch half left of the sprite/logical coordinate. Its clean-exit
log covers 906 PAL fields with no Copper overflow. This confirms the C3 defect
as a stable display-origin error rather than collision or simulation damage.
A two-byte-misaligned wide-fetch pointer is explicitly excluded; the next
isolated proof moves the complete DDF start/stop window together while keeping
fetch count and modulo unchanged.

Stage 5D3 tests that DDF hypothesis by moving the complete window from
`$30..$d0` to `$38..$d8`, leaving fetch count and modulo unchanged. Supplied
FS-UAE/68030 evidence rejects it: the 31-to-32 phase sequence remains smooth
and the lower reference remains fixed, but the bitplane and hardware-sprite
markers still do not coincide around the known phase resets and the left-edge
presentation changes. The clean-exit log covers 543 PAL fields with no Copper
overflow. Do not infer a further register offset from the scaled recording;
the next and final bounded calibration step must be a matched FMODE=0 versus
FMODE=1 A/B proof with a deliberately held phase-zero interval. If that does
not yield an exact relative-origin correction, park the 32-bit-fetch branch
rather than continue empirical DDF tuning.

Stage 5D4 packages that final bounded comparison as two isolated executables.
The FMODE=0 reference and FMODE=1 candidate share the same `$30..$d0` DDF
window, DIW geometry, authored markers, fixed hardware sprite and lower
reference; only fetch width and its required pointer/fine-scroll granularity
differ. Both hold phase zero for 150 PAL fields before sweeping. They live in
separate drawers so their logs cannot overwrite one another. FS-UAE/68030 A/B
evidence is pending; this proof carries no gameplay-performance claim.

The first supplied D4 package did not reach custom-chip takeover: both modes
reported a missing `renderbench-rear16.raw` and returned code 11. One legacy
production-benchmark loading branch still tested the old proof macro instead
of the shared calibration macro. This was a controlled construction failure,
not an FMODE crash or measurement. The branch guard is corrected and both
standalone binaries are rebuilt without an external-art dependency.

Supplied screenshots and clean-exit logs from the rebuilt D4 pair resolve the
relative origin. In identically sized host captures, the FMODE0 bitplane marker
occupies x=172..187 and FMODE1 x=242..257: a 70-capture-pixel displacement,
exactly one visible 16-logical-pixel calibration cell. This accepts D4 as an
FS-UAE/68030 measurement and rejects the current FMODE1 mapping as production-
ready. The conspicuous cyan/white bar is the bitplane marker; the fixed sprite
uses a dark palette entry and is not distinct enough for absolute-coordinate
acceptance. A follow-up, if authorized, should test one aligned longword plus
extended-delay 16-pixel phase bias and improve sprite-marker contrast. Do not
use a two-byte-misaligned wide-fetch pointer.

Stage 5D5 implements only that measured correction in the isolated proof. Its
FMODE1 mapping evaluates scroll at `camera + 16`, so the 32-pixel coarse pointer
remains longword aligned and advances at the matching extended-delay wrap. DDF,
DIW, fetch width and authored bitplane marker remain unchanged. Calibration-
only sprite colour 17 is made orange so the fixed hardware marker is now
visually independent of the cyan/white bitplane marker. The AmigaOS executable
builds and packages successfully; FS-UAE/68030 evidence is pending, and no
gameplay or performance conclusion follows from the host build.

Supplied D5 FS-UAE/68030 evidence accepts the phase-zero correction: automated
image measurement places its bitplane marker at capture x=172..187 and
y=164..841, exactly matching the D4 FMODE0 reference. Its clean-exit log spans
993 fields with the declared 16-pixel aligned phase bias and no Copper overflow.
The later orange left-edge area reported by the user is palette instrumentation,
not origin drift: calibration colour 17 is also REAR8 colour 1 under `PF2OF=16`
and therefore scrolls with the rear grid. The sprite marker remains hidden
behind the opaque playfields at the proof's priority, so absolute sprite-marker
visibility was not achieved. Accept the relative origin only; the screenshot
does not independently validate all temporal transitions or gameplay.

Stage 5E transfers the accepted D5 phase relation into one diagnostic gameplay
candidate. FRONT16 and REAR8 derive both their longword coarse pointers and
extended fine delay from logical scroll `+16`; the fixed FMODE1 HUD uses the
matching delay-31 encoding (`$44ff`). World camera, parallax rate, physical
ring coordinates, Bob coordinates, sprites and collision/gameplay state are
unchanged. New host assertions cover corrected offsets and both 16/32-pixel
phase boundaries. All host tests pass and the AmigaOS HD drawer builds with the
complete runtime assets. Supplied FS-UAE/68030 presentation evidence remains
the next gate; do not infer it from compilation.

Supplied Stage 5E FS-UAE/68030 video rejects the first gameplay integration.
The D5 phase bias fixes the prior global alignment: the HUD, player/world
relation and right edge remain correctly placed, with no reviewed teleport.
However, consecutive frames expose a phase-dependent black strip at the left
gameplay edge, varying from absent to roughly one 16-pixel cell. The log reports
zero rolling-target ownership violations and 1,983 of 1,983 one-field cadence
intervals (50.00 effective FPS on this fast emulator configuration). This is
therefore a left fetch-coverage defect, not gameplay offset, Bob corruption or
a performance rejection. The current `$30..$d0`/44-byte FMODE1 fetch does not
provide enough pre-visible data for every corrected extended-delay phase.
Reject Stage 5E presentation and do not spend a 68020 or hardware run yet.

Stage 5F isolates the remaining coverage correction before another gameplay
build. WinUAE's display model gives FMODE1/lores a 16-CCK, 32-pixel indivisible
fetch unit; `$30..$d0` therefore stores 44 bytes, while moving start to the
next valid earlier unit at `$20` stores 48. The proof supplies the matching
4-byte physical guard before logical x=0, retains the accepted `camera + 16`
phase and longword pointers, and restores `$30..$d0`/44-byte fetching for its
fixed lower reference. This is a coordinated DDF/fetch/modulo/source-layout
test, not another origin tweak. The AmigaOS proof builds; FS-UAE/68030 evidence
is pending before any gameplay integration.

Supplied Stage 5F FS-UAE/68030 evidence accepts that isolated geometry. Exact
image measurement puts the guarded candidate marker at capture x=172..187,
identical to the accepted D4 FMODE0 reference, while the captured left edge is
fully covered. Its clean-exit log spans 1,116 PAL fields and declares
`$20..$d0`, 48 fetched bytes, four physical guard bytes, the retained 16-pixel
bias, zero Copper overflow and within-frame work. This authorizes a bounded
gameplay integration with real physical guards for both playfields; it does
not authorize pointing before an unguarded allocation, changing the fixed HUD
fetch, or claiming 68020, ADF or real-hardware acceptance.

Stage 5G is the bounded gameplay transfer of that accepted geometry. Gameplay
fetching changes to `$20..$d0`/48 bytes while the fixed HUD explicitly restores
`$30..$d0`/44 bytes. FRONT obtains its four-byte prefetch guard from the
existing middle ring copy. REAR is copied once into a slightly wider guarded
Chip-RAM bitmap with logical x=0 four bytes into every row, avoiding an invalid
pointer before the original asset allocation. The accepted `camera + 16`
mapping and all gameplay coordinates remain unchanged. Host/build verification
and FS-UAE/68030 presentation evidence are required before slower targets.

Supplied Stage 5G FS-UAE/68030 evidence rejects that transfer. The guarded
playfields fix the reported left edge and the diagnostic run maintains 49.95
effective FPS across 2,308 frames with zero target-ownership violations, but
Sparkpaw's six attached hardware-sprite channels lose multiple component
planes at different camera positions. Playfields, HUD and enemy Bobs remain
coherent. The isolated Stage 5F proof used null/calibration sprites and therefore
did not expose the production sprite-DMA budget. Moving DDFSTRT from `$30` to
`$20` lets higher-priority bitplane DMA occupy sprite fetch slots; Stage 5G's
early-DDF geometry is not production-safe. Retain the Stage 5F measurement as
useful but reject its direct gameplay integration. The next candidate must keep
the sprite-safe `$30` start and solve edge coverage without stealing those DMA
slots.

Stage 5H explores the bounded AGA-native alternative: keep `$30..$d0` and test
FMODE3's 64-bit bitplane fetch, whose eight-byte unit can provide wider aligned
prefetch without moving DDF into the sprite-DMA region. An isolated FMODE1
corrected-reference versus raw FMODE3 candidate holds phase zero before a
0..63 sweep. The candidate validates eight-byte bitmap alignment and uses a
48-byte fetch; sprite width stays at 16 bits. This stage measures relative
origin only and cannot yet prove production sprite DMA or gameplay.

Supplied Stage 5H FS-UAE/68030 A/B evidence yields an exact calibration. At
identical host size the corrected FMODE1 reference marker occupies x=156..171
and raw FMODE3 x=226..241: 70 capture pixels, exactly one 16-logical-pixel
cell. The candidate log spans 839 fields, reports eight-byte alignment, a
48-byte fetch, clean exit and no Copper overflow; no temporal defect was
reported. Accept this as a relative-origin measurement only. The next isolated
proof should apply logical `+16` to the FMODE3 64-pixel coarse pointer and full
0..63 delay, then verify phase zero and the 63-to-0 wrap before gameplay.

Stage 5H2 applies exactly that measured correction in isolation. FMODE3 remains
at sprite-safe `$30..$d0`; logical scroll is evaluated at `x + 16`, with an
eight-byte coarse pointer and complete 0..63 delay. The phase-zero marker and
63-to-0 transition are the only acceptance targets. Because the proof does not
carry the production six-channel actor, a successful result permits a focused
gameplay build but does not itself close the Stage 5G sprite regression.

Supplied Stage 5H2 FS-UAE/68030 evidence accepts the isolated correction.
Automated measurement places both the corrected FMODE3 marker and Stage 5H
FMODE1 reference at capture x=156..171 and y=172..849. The clean-exit log spans
896 fields with eight-byte alignment, a 48-byte fetch and no Copper overflow,
covering repeated 63-to-0 sweeps; no temporal defect was reported. This permits
a focused gameplay transfer but does not yet accept production sprite DMA,
left-edge presentation, performance or any slower/hardware target.

Stage 5I transfers that exact geometry into one diagnostic gameplay candidate.
The scrolling playfields use FMODE3 at sprite-safe `$30..$d0`, 48 fetched bytes,
eight-byte pointer alignment, 64-pixel coarse steps and logical `+16`; the HUD
switches back to its accepted FMODE1/44-byte path. Sparkpaw's three attached
16-pixel pairs, art and staging remain unchanged, making the Stage 5G sprite
regression an explicit gate alongside left-edge coverage. No slower-target or
hardware claim follows from the build.

Supplied Stage 5I FS-UAE/68030 evidence rejects that integration. Sparkpaw's
three attached sprite pairs render correctly again and the log records 49.87
effective FPS, three wraps and zero ownership violations, but the playfield
origin visibly jumps, variable black strips recur at the left edge and pixels
flicker at the left ground/HUD boundary. World objects remain mutually aligned
while shifting against the fixed HUD, identifying a display-fetch-origin
regression rather than gameplay teleportation. The earlier Stage 5H2 result
covered only an isolated linear bitmap; it did not validate FMODE3 across the
production rolling ring, target alternation, dual playfield and HUD mode split.
Keep Stage 5I and Stage 5G rejected: the former breaks playfield presentation,
while the latter presents the playfield correctly only by stealing DMA slots
from Sparkpaw's hardware sprites.

Stage 5J closes that coverage gap with a production rolling-ring A/B proof
rather than another release candidate. Its FMODE0 reference and FMODE3
candidate run the same real ring, alternating targets, dual playfield,
six-channel Sparkpaw sprite, HUD split and gameplay workload. Diagnostic traces
now include the fetch mode, both fine phases, BPLCON1 and both logical/coarse
offsets on every retained frame. This allows a supplied temporal defect to be
matched to an exact 64-pixel transition. Stage 5J is host-tested and packaged
for the first FS-UAE/68030 HD gate; no presentation result is claimed yet.

Supplied Stage 5J FS-UAE/68030 HD testing accepts the FMODE0 reference's visual
presentation and rejects the FMODE3 candidate with the same world jumps and
displaced strips as Stage 5I. The candidate trace spans all 64 front phases;
offline verification finds zero disagreement between every logged camera,
coarse pointer, fine phase and BPLCON1 value and the implemented formula. Ring
ownership also remains clean. Consequently the production FMODE3 mapping
assumption—not Copper publication or target ownership—is rejected. The 49.54
versus 49.93 FPS figures came from unequal workloads and are not comparative
performance evidence. A repeated gameplay tweak to the same `+16`/64-pixel
formula is explicitly ruled out.

Stage 5K starts the first alternative derived from the wider hardware review.
It keeps FMODE0 and the unchanged six-channel 48x48 Sparkpaw actor, but compares
the accepted `$30..$d0`/42-byte production ring against a single-word-earlier
`$28..$d0`/44-byte candidate. FRONT reads through the middle ring's real
two-byte predecessor and REAR is copied into a guarded display bitmap with the
same two-byte physical prefix; the fixed HUD explicitly restores its accepted
`$30..$d0` fetch. Host tests and both AmigaOS builds pass. This is an
FS-UAE/68030 HD presentation A/B gate only; no result or slower/hardware
acceptance is claimed yet.

The first supplied Stage 5K FS-UAE/68030 run accepts the DDF30 reference's
presentation but cannot evaluate the DDF28 candidate. The candidate reached
gameplay with working input/audio and logged 747 frames at 50.00 FPS, yet its
display remained completely black. Inspection identified a local build defect:
the candidate changed the emitted Copper MOVE's register-address word rather
than its DDFSTRT value, producing a malformed list. Record this as an invalid
test artifact—not a rejection of the FMODE0 `$28` hypothesis—and require a
structural Copper pair check before the corrected rerun.

Stage 5K2 corrects only that invalid artifact. The candidate now emits the
playfield pair `$0092,$0028` directly and restores `$0092,$0030` for the fixed
HUD. Each of the two independently generated rolling Copper lists is scanned
at runtime for exactly one occurrence of each pair; renderer preparation fails
if the structural contract is absent. All host tests and the AmigaOS diagnostic
build pass. The earlier DDF30 reference remains valid, so only this corrected
FS-UAE/68030 HD candidate requires retesting.

Supplied Stage 5K2 FS-UAE/68030 HD evidence rejects the valid `$28` candidate
with the current six-channel actor. The display, world and HUD are coherent and
the log records 1,971 one-field intervals at 50.00 effective FPS with zero
ownership violations, but Sparkpaw loses attached-sprite colour components.
Right-facing frames show extensive white regions; left-facing frames retain
more of the body but lose the tail component. This orientation dependence is
consistent with the same later sprite data contributing to different authored
pixels, while the clean renderer trace rules against ring ownership or Copper
publication. Do not move DDF earlier again with the existing actor. The next
isolated option is an AGA-wide attached Sparkpaw using the earliest channels,
with playfield geometry held fixed.

### 20 August 2026 - Stage 5L AGA-wide player accepted in FS-UAE and promoted as alpha.43

Stage 5L returns to Stage 5G's visually coherent FMODE1 `$20..$d0` playfield
geometry and changes only the player DMA representation. All authored 48x48,
15-colour pixels and all 62 append-only frames are packed into one attached
64-pixel AGA sprite pair on channels 0/1, with transparent pixels 48..63. The
wide streams use separate eight-byte POS/CTL fetch blocks, four DATA plus four
DATB words per row, zero termination blocks and explicitly eight-byte-aligned
Chip staging allocations. Art, palette, facings, animation selection,
grounding, collision, parallax, HUD and gameplay remain unchanged.

MrDig's supplied FS-UAE/68030 HD run reports no corruption, glitches, flicker
or player component loss. `renderdiag.log` records 2,164 frames, 2,163 one-field
intervals, no longer intervals, 50.00 effective FPS and zero rolling-target
ownership violations. This accepts Stage 5L only for the supplied FS-UAE/68030
HD configuration. Alpha.43 promotes the same non-diagnostic renderer flags to
the normal HD and packed-asset ADF executables for real-A1200/68030 testing;
neither real-hardware launch path is accepted before that evidence is supplied.
Host tests, native HD/ADF compilation, ZIP/LHA inspection and independent ADF
round-trip validation pass. The bootable DOS1/FFS alpha.43 ADF uses 1,190
blocks (595 KiB) and leaves 570 free.

Follow-up supplied testing accepts broad alpha.43 renderer stability on the
real 34.5 MHz A1200/68030 from both HD and physical ADF, and on Analogue Pocket
from ADF: no general corruption, trails or broken Sparkpaw components were
reported. It rejects cadence on all three slower paths. Real HD additionally
repeats or misses some sound events under load. A narrow intermittent disturbance
remains at the ground/HUD boundary; the preserved recording is
`Renderer alpha43-rejected-real-a1200-hd-performance-and-hud-seam-flicker.MOV`.
FS-UAE/68020 records 26.38 effective FPS even though Stage 5L reduces average
Bob-pass time by about 29% versus Stage 5A. The accepted rolling ownership,
early-fetch geometry and wide player pair are therefore frozen while HUD seam
timing, entering-column micro-copies, Copper/sprite recopying, serialized
Blitter waits and measured player collision work become the next isolated
optimization subjects.

### Stage 5L-H1 isolated ground/HUD seam diagnostic

The first post-alpha.43 seam candidate changes only the order of one existing
Copper MOVE in the fixed-HUD transition. The WAIT remains at hardware line 251,
horizontal position `$d1`, immediately after the gameplay `$d0` DDFSTOP. The
fixed HUD's FMODE1 restore is now the first MOVE after that WAIT, before the
unchanged BPLCON1, `$30..$d0` HUD fetch geometry, seven HUD pointers, modulos
and palette moves. Line 252 remains the HUD switch and line 253 remains the Bob
pass. Stage 5L rolling ownership, early-fetch geometry, four-byte guards, wide
attached player pair, Bobs, assets, gameplay and production flags are unchanged.

The separately named HD diagnostic drawer is
`sparkpaw/dist/Stage5L-H1-HUD-Seam-FMODE-First-HD/`. Its log identifies the
candidate and exact transition order. Host tests, native production/diagnostic
builds and release-package validation pass; the rebuilt production executable
retains SHA-256
`32fed6e8b0b9a2cbc2050fb1fb17a72ca9f8dc1b327652e115c6368a71365d62`.
Supplied FS-UAE/68030 HD testing rejects H1: the same intermittent short line
fragment or pixels remain in the black seam at the left ground/HUD boundary.
Its log records 913/913 one-field intervals (50.00 FPS), zero rolling ownership
violations and normal Copper alternation. This rejects FMODE ordering as the
cause and separates the seam from missed updates, late target publication and
displayed-target writes. The log is catalogued as `Renderer Stage 5L-H1-
rejected-fs-uae-68030-hud-seam-flicker-renderdiag.log` with a matching sidecar.

Stage 5L-H2 keeps H1's FMODE-first ordering but omits only the redundant HUD
DDFSTOP MOVE. Gameplay already leaves DDFSTOP at `$d0`, exactly the fixed HUD
value; DDFSTRT still changes from `$20` to `$30`. This advances all seven HUD
pointer moves by one Copper MOVE without changing any register value, line-252
HUD ownership, line-253 Bob timing, renderer geometry or production build. The
new FS-UAE/68030 HD drawer is `sparkpaw/dist/Stage5L-H2-HUD-Seam-Retain-
DDFSTOP-HD/`. Construction checks do not accept H2; the Stage 2 hot-path audit
has not begun.

Supplied FS-UAE/68030 HD testing also rejects H2 with no visible improvement.
Its log records 877/877 one-field intervals (50.00 FPS), zero rolling ownership
violations and normal Copper alternation. It is catalogued as `Renderer Stage
5L-H2-rejected-fs-uae-68030-hud-seam-flicker-renderdiag.log` with a matching
sidecar. Native-frame inspection of the earlier real-A1200 MOV confirms the
stable boundary zone but cannot attribute the few scaled/intermittent pixels
reliably to either the last FRONT16 scanline or first HUD scanline.

Stage 5L-H3 therefore tests the split state rather than assuming HUD artwork is
faulty. The authored HUD is natively three planes, but alpha.43 carries it as a
seven-plane dual playfield by inserting four blank-plane pointers. H3 switches
the line-252 section to `BPLCON0=$3200` single-playfield three-plane display and
loads only the three authored HUD pointers. Dimensions, pixels, palette, FMODE,
DDF, modulos, line-253 Bob pass and all Stage 5L/gameplay contracts remain. This
both terminates the gameplay dual-playfield state explicitly and removes eight
pointer MOVEs from the critical boundary. The isolated drawer is `sparkpaw/
dist/Stage5L-H3-HUD-Native-3Plane-HD/`; FS-UAE/68030 acceptance is pending.

Supplied H3 FS-UAE/68030 evidence reports the former left symptom appears
corrected, but rejects the candidate because a small far-right gap now exposes
pixels whose colours change with gameplay scrolling. The screenshot and log
are catalogued as `Renderer Stage 5L-H3-rejected-fs-uae-68030-right-seam-
scroll-pixels.*`. The log measures 3,561/3,561 one-field intervals (50.00 FPS),
zero ownership violations, normal Copper alternation and the intended three
HUD pointers. This isolates the new symptom to horizontal split/fetch coverage,
not cadence or target publication.

Stage 5L-H4 retains H3's native three-plane split and adds exactly one complete
32-bit HUD fetch unit at the right: HUD DDFSTOP becomes `$d8`, fetch size 48
bytes and the private zero-filled HUD stride 384 pixels. The authored 336 HUD
pixels, origin, palette, visible dimensions and gameplay are unchanged. Its
isolated FS-UAE/68030 drawer is `sparkpaw/dist/Stage5L-H4-HUD-Right-Fetch-
Guard-HD/`; construction checks do not constitute presentation acceptance.

Supplied FS-UAE/68030 testing immediately rejects H4: the complete HUD region
is broadly corrupted across many lines. No H4 log was present when reviewed.
The screenshot is catalogued as `Renderer Stage 5L-H4-rejected-fs-uae-68030-
broad-hud-corruption.png` with matching sidecar. The H4 DDF/fetch-size relation
was an invalid uncalibrated guess. More importantly, H3/H4 moved from an
unknown boundary source to a HUD-fetch attribution that the evidence did not
establish. Return to byte-identical alpha.43 Stage 5L. H1 and H2 only reject
their narrow ordering/margin hypotheses; H3 and H4 are rejected branches, not
progressively better baselines. Before another fix, use an isolated A/B
localisation proof to distinguish final FRONT16/REAR8 shifter data, hardware
sprite contribution and first-HUD-row state.

Workspace delivery rule: MrDig mounts `sparkpaw/dist/` as the FS-UAE HD.
Consequently every future user-facing HD proof or diagnostic drawer belongs
directly under that directory, following the existing stage/checkpoint naming
and including its executable, test ReadMe and required runtime assets. A drawer
under `build/` or `build/test/` is an internal intermediate, not a deliverable.

The user then supplied a 19.67-second FS-UAE/68030 MOV of the exact alpha.43
production baseline while moving through the level. It is catalogued as
`Renderer alpha43-rejected-fs-uae-68030-intermittent-left-transition-scanline-
pixels.mov` with a matching sidecar. Consecutive-frame inspection localises the
remaining symptom to a fixed far-left segment in the narrow dark transition/
top-border scanline zone: Sparkpaw and the world scroll independently of it.
This is not evidence of corruption in the authored HUD panel bitmap. It is
consistent with residual or undercovered display data at the split, but the MOV
cannot by itself identify FRONT16, REAR8, sprite-shifter or other Copper state.
H3/H4 remain rejected; the next candidate must start from byte-identical
alpha.43 and perform a source-isolation A/B proof before another geometry fix.

Stage 5L-H5 implements that source-isolation proof as one HD drawer with two
diagnostic executables. A neutralises all eight sprite control registers before
the final gameplay scanline while leaving the alpha.43 bitplane/HUD split
unchanged. B suppresses bitplane output across only the final gameplay and
first HUD scanlines, leaves sprites live, and restores the original seven-plane
dual-playfield state in the following horizontal blank. B's two-line black gap
is intentional and is not a candidate presentation. Both use the exact release
renderer flags plus diagnostics; no H1-H4 macro is enabled. Host tests pass,
both outputs are AmigaOS executables, and the rebuilt production executable is
still byte-identical with SHA-256
`32fed6e8b0b9a2cbc2050fb1fb17a72ca9f8dc1b327652e115c6368a71365d62`.
FS-UAE/68030 evidence is pending; H5 is not a fix or acceptance candidate.

Supplied H5 FS-UAE/68030 testing reports that A (sprites off on the final
playfield line) retains the original flickering pixels, while B (bitplanes off
across the split) removes them but broadly corrupts the HUD. A logs 449/451
one-field intervals (49.77 FPS); B logs 561/562 (49.91 FPS); both record zero
ownership violations. This excludes normal hardware-sprite output and confirms
that bitplane output is necessary, but B's BPLCON0 blackout also interrupts DMA
and pointer progression, so it cannot separate FRONT16 from REAR8.

H6 performs that separation without touching DMA, pointers, fetch or the HUD
split. H6A blacks only palette entries 0..15 (FRONT16) on the final gameplay
scanline; H6B blacks only entries 16..23 (REAR8). The standard alpha.43 HUD
palette writes restore the fixed HUD. Each variant is packaged in its own
self-contained `sparkpaw/dist/` drawer so each writes its own renderdiag.log.
Host tests pass and production remains byte-identical at SHA-256
`32fed6e8b0b9a2cbc2050fb1fb17a72ca9f8dc1b327652e115c6368a71365d62`.
H6 FS-UAE/68030 evidence is pending.

The user reports H6A seam flicker absent with no other visible corruption and
H6B flicker present. H6A records 1,610/1,612 one-field intervals (49.93 FPS)
and zero ownership violations; H6B records 418/418 (50.00 FPS) and zero
violations. This accepts the FRONT16 final-transition-scanline palette mask as
the FS-UAE/68030 seam solution and rejects REAR8 as the source. It does not
claim ADF, Pocket or real-hardware acceptance.

H7 promotes exactly that mask into `RELEASE_RENDERER_FLAGS`, with the original
Stage 5L flags retained separately so rebuilding H5/H6 cannot contaminate their
A/B meaning. The production-shaped H7 executable contains no diagnostic code,
log writer or test input. Host tests pass; the normal build, H7 build product
and packaged executable are byte-identical at SHA-256
`b8c578c536ddc60728bcf4c4124a8a569a2cfbc01b5da91ac61263255f9ae544`.
FS-UAE/68030 presentation of this non-diagnostic executable remains pending.

The user then explicitly reports that the exact production-shaped H7 drawer
looks good in FS-UAE/68030 HD. This accepts H7 presentation and the seam fix for
that configuration only. ADF, Analogue Pocket and real-A1200 results remain
unclaimed. The renderer seam phase is closed and Stage 2 of the whole-codebase
68020 performance plan may begin; no hotspot optimization is authorized before
the broad comparison table is complete.

### 24 August 2026 - Stage 2 performance checkpoint promoted as alpha.44

Alpha.44 retains the immutable Stage 5L display geometry and packages H7's
supplied FS-UAE/68030-HD-accepted final-transition-scanline FRONT16 palette
mask. The whole-codebase audit promoted only isolated candidates that passed
their required host, FS-UAE/68030 and FS-UAE/68020 gates: display-only rolling
targets, patch-only Copper publication, word-level enemy and player loading
conversion, direct and unrolled ring maintenance, tile-span collision, cached
player-sprite staging, cached hazard columns, enemy-state copy on unload, and
immediate post-publication update start. Display-only targets reclaim 319,488
Chip bytes. Loading conversion cuts measured 68020 renderer preparation from
about 40.2 to 5.9 seconds.

The final target-local collectible H4 route keeps diamonds out of broad
canonical dynamic synchronization and redraws them on each inactive target
before later Bobs. Supplied FS-UAE/68030 and FS-UAE/68020 HD testing reports
normal diamonds including enemy overlap and zero ownership violations. On the
68020 stress configuration it raises cadence from 35.31 to 42.15 FPS (+19.4%),
reduces `ring_dynamic` average from 3,940 to 101 CIA ticks and reduces Bob-pass
average from 11,086 to 8,499. Together with immediate update start's earlier
27.45 to 35.81 FPS result, this is material but still below the 50 Hz target.
Canonical diamond synchronization remains available only as a diagnostic
reference. Official HD and ADF executables contain no profiler, logger or test
input. Alpha.44 ADF, Analogue Pocket and real-A1200 results remain pending and
must come from supplied tests; no emulator result is promoted into a hardware
claim.

The alpha.44 native HD and packed-asset ADF builds, ZIP/LHA construction and
independent ADF round-trip validation pass. The bootable DOS1/FFS ADF uses
1,195 blocks (597 KiB) and leaves 565 free. This records construction only;
real-A1200 HD/ADF and Analogue Pocket ADF presentation and cadence remain
pending supplied tests.

### 25 August 2026 - Stage 2 traversal and ring-column checkpoint promoted as alpha.45

Alpha.45 retains Stage 5L, H7, FRONT16 + REAR8, the attached 64px Sparkpaw
pair, assets, palettes, animation, collision and gameplay contracts. Three
isolated follow-up optimizations passed supplied FS-UAE/68030 and FS-UAE/68020
HD presentation gates. A 25-surface by two-direction Fast-RAM traversal lookup
replaces repeated grounded-Strider scans, with invalid or duplicate future data
falling back safely. On 68020 it raises cadence 44.35 to 45.09 FPS, reduces
`enemy_parked` average 792 to 601 ticks and `enemies` 2,036 to 1,784.

Generic restore and masked-Bob helpers now write invariant BLTCON, word-mask
and modulo registers once per four-plane Bob. Jobs, waits, pointers and family
order remain unchanged. Average cadence is flat, but Bob p95 falls 17,282 to
14,752 ticks, so this is retained as a tail reduction rather than an FPS claim.

The normal aligned 16px entering-column roll now uses advancing pointers: one
canonical word is published to the same three physical copies per row. Every
other roll size retains the generic fallback. Matched FS-UAE/68020 testing
raises cadence 44.47 to 45.55 FPS. `ring_roll` average falls 1,691 to 483,
p95 9,878 to 2,782 and maximum 9,941 to 2,815; Bob-pass p95 falls 16,308 to
9,142. Stock-68020 50 Hz remains open. Alpha.45 ADF gameplay, Analogue Pocket
and real-A1200 acceptance require new explicit supplied results. Native HD,
ZIP/LHA and packed-asset ADF construction pass; the bootable DOS1/FFS image
uses 1,197 blocks (598 KiB) and leaves 563 free. This validates packaging only.

### 25 August 2026 - Post-alpha.45 residual-cost plan synchronized

The current planning documents now agree that the broad Stage 2 codebase audit
has been completed and has already produced the alpha.44/45 gains; it is no
longer described as work that still has to begin. The latest matched
FS-UAE/68020 result is 45.55 effective FPS. Its remaining continuous aggregates
are `game_update` average/p95 4,056/5,215 CIA ticks and `bob_pass` 5,840/9,142.
Their 14,357-tick p95 sum is already above the approximately 14,188-tick PAL
field before publication overhead.

The next diagnostic will split those aggregates into CPU-active work,
custom-register setup, `WaitBlit`, Blitter execution and Chip-bus contention.
Current VBCC 68020/68030 assembly, symbol sizes, linker placement and runtime
Fast/Chip allocation evidence must be inspected before choosing another
optimization. Safe CPU/Blitter overlap in the serialized Bob chain is the
leading hypothesis, followed by isolated Strider/Bob preparation and only
measured CPU-read-heavy Fast-RAM mirrors. The ranking may change with evidence.
Every implementation remains one isolated prototype, gated first for
FS-UAE/68030 presentation and then by matched FS-UAE/68020 timing. Stage 5L/H7,
art and gameplay remain fixed. Mechanical source splitting stays a separate
byte-identical refactor, and a controlled 25 Hz update/50 Hz display prototype
remains a last resort rather than ordinary frameskipping.

### 25 August 2026 - Alpha.45 performance and hardware checkpoint accepted

The detailed CIA profiler was found to be a material observer on the 68020
stress configuration. A minimal-cadence build compiles out nested leaf timing
and Bob-family raster sampling while retaining renderer-boundary evidence.
Supplied FS-UAE/68030 HD testing reports normal presentation and 1,578/1,578
one-field intervals (50.00 FPS). Supplied FS-UAE/68020 HD testing also reports
normal presentation and records 1,104 one-field plus 33 two-field intervals out
of 1,137 (48.58 FPS), with no three-field misses or ownership violations. The
preceding fully instrumented run's lower cadence was therefore mostly profiler
overhead rather than missing production optimization.

Final supplied hardware testing accepts alpha.45 presentation and cadence on
the real A1200/68030 at approximately 34.5 MHz from both HD and physical ADF.
The supplied Analogue Pocket 68020 ADF run is also accepted. These results close
the alpha.45 renderer/performance checkpoint without changing the production
executable, renderer geometry, art or gameplay.

The 48.58-FPS FS-UAE/68020 result is now a required regression baseline. Future
features must not casually add broad per-frame scans, complete bitmap/Copper
copies, unconditional Fast-to-Chip staging or serialized small Blits. The
post-checkpoint whole-codebase review found no forgotten continuous large copy,
post-CHARGING asset work, misplaced CPU-heavy Chip data or new compiler helper.
Possible future gains require new evidence: coarse hardware-facing scopes,
eliminating an actual Bob job/wait, a genuinely coalesced projectile sweep, or
a Fast-RAM mirror justified by measured CPU reads. Rejected H5--H7 diamond
persistence, Bob pointer precompute, inline `WaitBlit`, fetch pruning and small
Blitter-column experiments must not be repeated unchanged.

### 25 August 2026 - Post-alpha.45 first Stormstone Core clearing prototype

Story planning now defines the small diamonds as Storm Shards and one large
Stormstone Core as each level's required quest reward. The isolated Level 1
prototype extends the world from 3072px to 3392px with one safe final field,
no enemies or ordinary Shards, and a large Stormkeeper's Waystation: a
stone-and-steel cottage/shrine with a storm-bent tree framing the Core.

The waystation and Core are baked into FRONT16 for the first proof. Touching the
Core performs one rectangle-overlap test and invokes the existing in-memory
level replay; the permanent level-complete/progression flow remains unimplemented.
No renderer, Makefile, release, version or distribution artifact changes are
included. FRONT16 grows 33,280 raw bytes, REAR8 7,488 and collision-map/cache
data 600; the packed foreground/rear comparison grows 12,739 bytes. The full
host suite, dedicated Core-bound test and native 68020 build pass. FS-UAE
presentation/timing and hardware acceptance remain open.

The first supplied FS-UAE/68030 HD run rejects the candidate before gameplay.
The CHARGING image remains onscreen with several short cyan fragments at the
upper-left display edge; the user reports an apparent crash. No
`renderdiag.log` or memory log exists in the exact drawer, so the evidence does
not distinguish gameplay-asset load failure, allocation failure, renderer
layout rejection or memory overwrite. The screenshot and sidecar are preserved
under `sparkpaw/testresults/Phase 6C.2-rejected-fs-uae-68030-charging-glitch.*`.
The 68020 gate is blocked pending a focused startup diagnosis.

The focused diagnostic repeats the user's CHARGING stop and writes
`stage=failed_rear_guard_prepare`. At failure it retains 786360 free Chip bytes
and a 784400-byte largest block, ruling out the guarded bitmap's approximately
90 KiB allocation as a total/contiguous-memory shortage. Source inspection
shows that the old request used logical `rearWorld->width + 32` while validating
against the source bitmap's already padded physical `BytesPerRow + 4`. At the
new 1120px width, graphics.library can place both requests in the same stride
class and the validation correctly refuses to continue.

The isolated correction requests the guarded destination from
`(source->BytesPerRow + PLAYFIELD_GUARD_BYTES) * 8`. Copy offsets, Copper,
fetch geometry, art and gameplay remain unchanged. A new FS-UAE/68030 HD gate
is packaged; acceptance remains pending and no 68020 build is promoted.

The supplied corrected run reaches gameplay and the final clearing. Its startup
log records `renderer_prepare_complete`, with 732624 free Chip bytes and a
731312-byte largest block. The rear panorama appears coherent and unstretched
in the supplied still, but the user rejects the art/pickup presentation: the
old portal remains visible at left, the house/tree touches the right edge and
reduces mainly to grey/black, the static Core lacks polish, and immediate replay
hides the act of collecting it. Evidence is preserved as
`sparkpaw/testresults/Phase 6C.2-rejected-fs-uae-68030-waystation-core-art.*`.

The follow-up remains planning-only. It keeps world width 3392, centres the
final scene at camera x=3072, proposes a clearing-specific FRONT16 palette role
swap for green/bronze material, reserves hardware sprite channels 2/3 for a
six-frame attached Core, adds a deterministic 32-frame collection beat and a
unique priority-11 Paula channel-1 effect. No 68020 gate is created before a
new 68030 visual/function candidate is approved.

The later isolated Phase 6C.2 implementation keeps the Core in the existing
64x48 FRONT16 Bob cache after an extra attached sprite pair proved incompatible
with the protected fetch layout. Supplied FS-UAE/68030 evidence accepts the
stable Core silhouette, calmer internal idle, centred camera, Storm Triumph
sound and delayed replay. It also shows that narrow Core-to-gauntlet lines end
low and disappear behind Sparkpaw's higher-priority hardware sprite. The next
candidate therefore changes only the twelve procedural pickup cells to a
self-contained radial release and patches the existing foreground Copper
colour values for a two-field local illumination. HUD palette, player sprites,
world geometry, cache dimensions and Bob ordering remain unchanged. Host tests
and native 68020 compilation pass; FS-UAE/68030 presentation remains pending.

The supplied 8.4-second 60 fps FS-UAE/68030 HD recording and explicit user
verdict accept the radial release. It preserves the idle silhouette, flashes
only the opaque foreground briefly, restores its palette immediately, keeps
HUD/rear/player colours stable and removes the ambiguous line into Sparkpaw.
Storm Triumph and replay timing remain accepted. A matching minimal-cadence
68020 HD run subsequently passes with normal user-observed presentation. Its
3,244 intervals comprise 3,223 one-field, 21 two-field and zero three-plus
intervals, for 49.67 effective FPS with maximum two fields and zero ownership
violations. One Stormstone Core request produces one Paula start. This clears
the regression gate against alpha.45's 48.58 FPS without claiming an
optimization gain from a differently routed sample. ADF and real hardware
remain untested.

### 26 August 2026 - Alpha.46 packages the first Core clearing

The accepted Phase 6C.2 work is released as `0.6.0-alpha.46`. The package adds
the 3392-pixel Level 1 world, Stormkeeper's Waystation, 64x48 FRONT16 animated
Core Bob, deterministic 50-tick radial collection beat and selected Storm
Triumph Paula sample. The existing Stage 5L/H7 rolling renderer, 4+3 dual-
playfield split, Sparkpaw sprite family, HUD and alpha.45 performance baseline
remain authoritative. Supplied FS-UAE/68030 HD presentation/function and the
49.67-FPS FS-UAE/68020 HD minimal-cadence gate are accepted. Package validation
produces LHA, ZIP, extracted HD drawer and a bootable DOS1/FFS ADF using 1,327
blocks (663 KiB), leaving 433 free. Both Core runtime files are present. This
does not establish alpha.46 ADF gameplay or real-hardware acceptance.

### 27 August 2026 - Alpha.47 packages the five-plate story intro

Phase 6C.3 promotes the supplied FS-UAE/68030- and FS-UAE/68020-HD-accepted
opening story into the normal build before the existing title. Five premium
64-colour AGA plates explain the Stormstone, its Lightning/Rain/Wind/Warmth/
Balance Cores, Grand Archivolts damaged `CONTAIN ALL WEATHER. RELEASE NOTHING.`
order, the reversed stations, Sparkpaws motive and the five-level recovery
quest. Each plate reuses one resident six-plane allocation, Copper-switches to
a stable reading band, scrolls passages on whole pixels, holds for roughly 3.4
seconds and fades text independently. Fire reveals/advances, held Fire skips,
and left mouse immediately skips the complete intro. Keyboard skip was rejected
because AmigaOS consumes keyboard serial events while DOS remains active for
between-plate loading.

The title screen is the accepted art-direction reference. All plates were
redrawn with stronger cyan/orange/violet hierarchy and native-scale pixel
clusters. Plate 3 was rebuilt from scratch as a distant valley overview with
five recessed crystal niches integrated into the Stormstone facade; the
rejected pasted circular overlay is absent.

HD retains ordinary SPBM files. The ADF-only build streams five compressed
intro assets plus compressed loading/charging assets through the established
512-byte SPR1 decoder, keeping one intro plate resident at a time. The release
packager validates every packed stream against its source. The bootable
DOS1/FFS ADF uses 1,707 blocks (853 KiB) and leaves 53 free. This proves package
construction and decode identity, not ADF gameplay. Real-A1200 intro acceptance
also remains open. The Stage 5L/H7 renderer, Phase 6C.2 gameplay, 2 MB Chip plus
8 MB Fast minimum and protected 68020 cadence baseline are unchanged.

### 27 August 2026 - Alpha.48 adds the pre-level ready screen

Phase 6C.4 adds a dedicated 64-colour AGA presentation screen after CHARGING.
It is shown only after all level assets and the gameplay renderer are prepared,
so joystick Fire or Space can fade directly into the playable level without a
second loading pause. The accepted composition uses a standalone crest-free
Sparkpaw wordmark, restrained black negative space, centred prompt and credits,
Level-1-derived edge architecture and Sparkpaw on a small lower-right platform.

The ready image is loaded temporarily in Fast RAM and copied once into the
resident six-plane loading bitmap in Chip RAM. It therefore adds no second
displayable status bitmap and does not alter the protected Stage 5L/H7 gameplay
renderer. An early title-art reuse was rejected as visually cheap. A malformed
32-colour SPBM prototype was 96 bytes short (61,548 versus the required 61,644)
and caused the familiar CHARGING-edge corruption; palette padding plus an exact
size assertion removed that failure. A later input failure was traced to
`WaitTOF()` after OS interrupts had been disabled during hardware takeover.
Owned-display waits now poll a safe raster boundary, making both controls work.

Supplied production-build tests accept presentation, Fire/Space handling and
the immediate level transition on FS-UAE/68030 HD and FS-UAE/68020 HD. Because
these were visual/function builds without cadence instrumentation, they make no
new numerical FPS claim. HD/LHA/ZIP retain the complete five-plate story intro.
To preserve full-quality art and ample floppy capacity, the ADF deliberately
omits only that cinematic and begins at the normal title; LOADING, CHARGING,
the ready screen and gameplay remain common. The final bootable DOS1/FFS image
uses 1,353 blocks (676 KiB), leaving 407 free, and every retained packed stream
decodes identically to its source. This package evidence does not establish ADF
gameplay, Analogue Pocket or real-A1200 acceptance, which remain open.

### 27 August 2026 - Alpha.49 fixes fullscreen COLOR00 and adds WHDLoad packages

Real-A1200/Indivision HD photos expose a stable one-pixel full-height coloured
line at the left of every intro plate and the ready screen. Source inspection
confirms the presenter geometry is shared with the already-clean title/loading
screens, while the new assets assigned non-black colours to palette pen 0.
Their generators now reserve pure-black `COLOR00` using the established
lossless index swap where possible, or the least-used nearest-colour merge
where a source contains no black. Supplied FS-UAE/68030 HD and subsequent real-
A1200/Indivision HD testing accept the correction. A host regression checks
palette bytes 12..14 of every fullscreen direct-Copper SPBM so the failure
cannot silently return. Copper geometry, presenter timing and gameplay remain
unchanged.

Alpha.49 also ships versioned WHDLoad LHA and ZIP archives. The implementation
follows the established ChipSnake and
MrDig's Futsal BootDOS approach but keeps Sparkpaw's A1200/68020 identity:
Kickstart 3.1 emulation fitting the 2 MB Chip plus 8 MB Fast target, the complete HD
runtime asset set under `data/`, PRELOAD/PAL Workbench tooltypes and F10 as the
WHDLoad exit key. Unlike the earlier prototype slaves it does not define the
68000-oriented `NO68020` option.

The package includes a generated four-colour Sparkpaw Workbench icon and a
MultiView ReadMe icon. It deliberately redistributes neither WHDLoad nor a
Kickstart ROM. `make release` now creates the existing LHA, ZIP, bootable ADF
and review drawer first, then assembles and archive-checks
`Sparkpaw-0.6.0-alpha.49-WHDLoad.lha` and `.zip`; `make whdload` rebuilds only
the WHDLoad pair. This is host build/package evidence. Startup, F10 exit,
presentation, audio, gameplay and repeated-launch behavior under WHDLoad and
on real hardware remain pending user-supplied testing.

### 27 August 2026 - Alpha.50 adds secondary-button jump input

Sparkpaw now accepts the secondary button on joystick port 2 as a third source
for the existing jump action alongside joystick Up and keyboard W. During
hardware takeover, port 2 pin 5 is driven high to keep a CD32 pad's shift
register in its reset state; active-low POTINP bit 14 then exposes the ordinary
Amiga second-button/Blue line on pin 9. Restore returns the POT lines to input.

All three jump sources are combined before the existing edge detector. Holding
the secondary button after takeoff therefore cannot trigger another jump on
landing, and pressing it while Up or W is already held cannot synthesize a
second edge. Primary Fire/Space, crouch, shooting, jump acceptance, physics,
animation, Paula audio and renderer timing remain unchanged.

All host regressions and the native VBCC 68020 build pass. MrDig reports that
the supplied production-style HD test works on the real A1200 and explicitly
accepts the secondary-button jump. The exact controller model was not supplied,
so this records real-A1200 HD function without claiming a separately identified
CD32 pad. No FS-UAE, ADF, WHDLoad or Analogue Pocket result is inferred.

After initially requesting a source-only checkpoint, MrDig chose a normal
version increment to keep distributed behavior unambiguous. SemVer advances to
`0.6.0-alpha.50`; the Phase 6C.4 roadmap identity remains unchanged.
The normal native build, host regression suite and complete release pipeline
pass. The bootable DOS1/FFS ADF uses 1,354 blocks (677 KiB), leaving 406 free;
the HD ZIP/LHA, ADF and both WHDLoad archives are host-validated. These package
checks add no ADF or WHDLoad gameplay acceptance beyond the supplied real-A1200
HD result.

### 27 August 2026 - WHDLoad F10 exit diagnosis and hardware candidate

Supplied real-A1200/68030 testing of the alpha.49 WHDLoad package accepts
startup and loading but rejects the documented F10 exit. Source inspection
isolates the boundary: after the ready screen appears, Sparkpaw calls
`Disable()`, clears custom `INTENA` and polls the keyboard CIA directly. The
KickEmu `slv_keyexit` hook lives in the normal keyboard-interrupt route, so it
never observes F10 after custom-chip takeover; Sparkpaw acknowledges and
otherwise discards that raw key itself.

The isolated `WHDLoad-F10-Exit-Real-A1200` candidate adds no interrupt handler
and does not alter the normal HD or ADF executable. Only the
`SPARKPAW_WHDLOAD` build latches raw F10, leaves the ready wait or gameplay
loop, restores DMA/Copper/Exec ownership, frees game resources and returns to
the existing slave, which completes with `resload_Abort(TDREASON_OK)`. The
normal executable remains byte-identical to alpha.50. Host tests, native builds
and ZIP integrity pass; clean Workbench return from both ready screen and
gameplay remains pending supplied real-A1200 testing.

### 27 August 2026 - Alpha.51 promotes the accepted WHDLoad F10 return

MrDig's subsequent real-A1200/68030 test accepts the isolated correction and a
clean return to Workbench. Alpha.51 therefore compiles the direct raw-F10 latch
only into `build/sparkpaw-whdload`, which is the executable packaged under the
WHDLoad drawer's `data/`. The ordinary HD executable and ADF build omit
`SPARKPAW_WHDLOAD`, preserving their prior exit behavior and runtime bytes apart
from the normal release rebuild. The BootDOS slave remains unchanged and still
finishes through `resload_Abort(TDREASON_OK)` after the program returns.

The general lesson is that a slave header's `slv_keyexit` is not sufficient
when a game disables the OS/custom interrupts and consumes raw CIA keyboard
events itself. Such a runtime must either preserve a compatible interrupt route
or explicitly recognize the quit key and unwind through the program/slave
boundary; aborting directly from arbitrary game state would skip Sparkpaw's
resource and hardware restoration.

Icon investigation remains deliberately separate. Sparkpaw currently generates
a 48x48, two-bitplane classic icon. A downloaded `ThunderCats.info` was decoded
as a 90x90, 35-colour NewIcons image with IM1/IM2 states, but its extracted art
does not match the box-cover icon photographed on the real A1200. The downloaded
file therefore cannot establish the hardware icon's format. A 16-colour RomIcon
is a plausible visual match, not an accepted fact; inspect the exact A1200
`.info` before replacing Sparkpaw's icon or documenting a target format.

The normal build, complete host regression suite and release pipeline pass.
Packaging produces the five consistently versioned alpha.51 artifacts plus the
extracted HD review drawer. The bootable DOS1/FFS ADF uses 1,355 blocks
(678 KiB) and leaves 405 free. Both WHDLoad archives pass integrity checks, and
the executable extracted from the ZIP is byte-identical to
`build/sparkpaw-whdload`. These host checks do not add gameplay acceptance
beyond the supplied real-A1200 WHDLoad startup/loading/F10 result.

### 27 August 2026 - Alpha.52 adds dual-layer HD and WHDLoad icons

The exact `ThunderCats.info` copied from MrDig's real A1200 resolves the earlier
format uncertainty. It is an 86x93 NewIcons project icon with 34 embedded
colours and a negligible classic fallback. Its artwork matches the supplied
Workbench photograph, unlike the unrelated downloaded 90x90 NewIcons variant.
This proves the desired format without making ThunderCats artwork a Sparkpaw
asset or style source.

Sparkpaw now owns an original vertical cover composition derived from its
accepted title, environment and character identity. Both ordinary HD and
WHDLoad packages receive the same 86x93, 34-colour embedded NewIcons pixels.
MrDig's supplied real-A1200 photograph accepts their size, colour and
presentation. The HD project icon uses `DefaultTool=Sparkpaw`; the WHDLoad icon
retains `DefaultTool=WHDLoad`, `SLAVE=Sparkpaw.Slave`, `PRELOAD` and `PAL`.

The first FS-UAE preview showed the old four-colour fallback because that
Workbench does not process NewIcons. A subsequent 16-colour RomIcon fallback
was rejected: classic icons contain only pen indices, and this Workbench did
not install the assumed FullPalette/RomIcon colours, producing green, pink and
washed-grey substitutions. The final fallback therefore uses only the eight
standard OS 2.x/3.x pens, three bitplanes and the same 86x93 composition without
dithering. MrDig accepts its decoded preview for now; exact FS-UAE display of
that final fallback remains pending and is not inferred from host rendering.

`tools/make_sparkpaw_icon.py` is the sole generator for both package paths.
`tests/test_sparkpaw_icon.py` reloads the actual encoded bytes and protects both
layer dimensions/depths, 34-colour NewIcons palette, shared pixel data and the
different HD/WHDLoad start metadata. The checkpoint skill now requires this
test and explicitly rejects a future 16-colour fallback unless the target pen
ownership changes. ADF packaging removes the HD `.info` before building the
floppy, so the icon itself consumes no ADF capacity and alters no runtime data.

The normal native build, full host regression suite, skill validation and
release pipeline pass. Both generated ZIPs were reopened and their encoded
project icons independently confirm 86x93/three-bitplane classic plus
86x93/34-colour NewIcons layers and the correct distinct DefaultTool/tooltypes.
Both WHDLoad archives pass integrity checks. The bootable DOS1/FFS ADF uses
1,356 blocks (678 KiB) and leaves 404 free; inspection confirms it contains no
`.info` icon. This host/package evidence adds no new FS-UAE or ADF gameplay
acceptance beyond the supplied icon observations.

### 27 August 2026 - Alpha.53 adds ready-menu control options

Phase 6C.4 now presents `START GAME` and `OPTIONS` in the accepted ready-screen
composition. Start remains selected at entry, so Fire or Space retains the
existing immediate fade into the fully prepared level. Up/down selects Options;
its sole session-local setting assigns the port-2 secondary button to `JUMP`
(the default alpha.50 contract) or `FIRE`. Left/right changes the value and
Fire/Space returns to Start. No preferences file or save format is introduced.

The deterministic generator produces one 320x256 six-plane base and four
320x76 central menu bands in a single CPU-only Fast-RAM atlas. All variants are
quantized together to one 64-colour palette with pure-black pen 0. After custom
takeover, a menu change waits until the beam has passed the affected band and
copies only 18,240 planar bytes into the existing loading/status bitmap. There
is no second displayable Chip bitmap and no change to Stage 5L/H7, the gameplay
Copper, HUD, physics, animation or audio.

When `FIRE` is selected, the secondary line is merged with primary Fire and
Space before the existing shot edge detector, matching alpha.50's anti-
retrigger contract. Host asset/mapping regressions and the native VBCC 68020
build pass. FS-UAE, ADF, WHDLoad and real-hardware behavior remain pending
supplied tests.

The complete release pipeline passes. The raw patch atlas is 73,164 bytes in
Fast RAM and its ADF SPR1 stream is 5,304 bytes. The bootable DOS1/FFS ADF uses
1,346 blocks (673 KiB) and leaves 414 free. HD ZIP/LHA, ADF and both WHDLoad
archives are host-validated; this does not establish runtime acceptance.

Supplied alpha.53 FS-UAE/HD screenshots reject the first menu-patch layout.
After entering Options and returning, the full-width native y=118..193 patch
clears the lower-left architecture and upper part of Sparkpaw's lower-right
composition. The Options value and return hint also leave insufficient space
above the credits. This is deterministic patch coverage, not a Copper,
renderer or emulator-scaling defect.

The focused correction narrows the patch from 320 to the word-aligned central
x=80..239 span, reducing each menu update from 18,240 to 9,120 planar bytes and
preserving both corner compositions. Options now places `SECOND BUTTON` and
`JUMP`/`FIRE` on one compact key/value row with the return hint above a larger
empty credit gap. Host asset regressions and native 68020 compilation pass.
Only the self-contained production-style FS-UAE/68030 HD visual gate is staged;
the alpha.53 release artifacts remain unchanged and correction acceptance is
pending supplied testing.

The supplied v1 FS-UAE/68030 screenshots reject that first correction too.
Although the runtime copy was central-only, the generator still erased the
full 320-pixel source band before extracting it, so both corners remained
clipped. Options also retained crowded credits and asymmetric arrow spacing.

The v2 generator no longer clears a flat band. Every state begins from the
accepted background and changes only its central text; assertions require both
outside regions to remain byte-identical. The atlas now spans x=80..239 and
y=118..221 so main-menu credits can be restored while Options leaves that field
empty. JUMP and FIRE use equal-width glyphs with exactly five empty pixels to
either arrowhead. The superseded v1 drawer is preserved under `older-builds`;
only the v2 production-style FS-UAE/68030 HD drawer remains for acceptance.

The supplied v2 FS-UAE/68030 HD result accepts the corrected layout and menu
function. The same production-style executable is subsequently accepted in
FS-UAE/68020 HD; this confirms presentation, direct start and both mappings but
adds no cadence number because the build contains no logger. MrDig's supplied
real-A1200/68030 HD test also accepts the final menu and verifies that selecting
`FIRE` makes the physical secondary button shoot instead of jump, while Up/W
remain jump and `JUMP` preserves alpha.50. The controller model was not
recorded, so no separately identified CD32-pad claim is made.

The accepted v2 becomes alpha.54. Its 160x416 six-plane CPU-only atlas occupies
50,124 bytes in Fast RAM and each menu update copies 12,480 planar bytes after
scanout. It adds no displayable Chip bitmap and changes no gameplay renderer,
physics, animation or audio behavior. ADF and WHDLoad runtime acceptance remain
pending; FS-UAE/68020 performance continues to rely on the protected earlier
48.58-FPS low-overhead baseline rather than this uninstrumented menu test. The
atlas packs to 40,805 bytes on ADF; the final bootable DOS1/FFS image uses 1,444
blocks (722 KiB) and leaves 316 free.
### 28 August 2026 - Release LHA artifacts gain real compression

The HD and WHDLoad `.lha` artifacts previously used project-owned Python code
that wrote valid level-0 `-lh0-` members. They were archives, but every payload
was stored byte-for-byte without compression. macOS already provided Homebrew
Lhasa 0.6.0 as `lha`; Lhasa can list, test and extract archives but deliberately
cannot create them.

Sparkpaw now uses classic LHa 1.14i-ac20220213, installed in the ignored local
`.toolchain/lha/bin/lha`, to create both release archives with `-lh5-`. An
absolute `LHA` environment override is supported for another creation-capable
installation. Packaging deletes stale output first, invokes LHa from the parent
of the versioned drawer, CRC-tests the result and fails unless at least one
`-lh5-` member is present. The ordinary HD and WHDLoad directory structures and
payload bytes are unchanged.

The normal `make` and `make release` path succeeds. Independent Lhasa extraction
compares byte-for-byte with both staged drawers. The current HD artifact falls
from about 1.9 MiB stored to 586 KiB, and WHDLoad from about 1.9 MiB to 583 KiB.
This changes host release packaging only; the ZIP, ADF, runtime loader, renderer,
gameplay and acceptance boundary are unchanged.

The packaging correction is released under the new `0.6.0-alpha.55` identity;
alpha.54 is not reused for different archive bytes. Alpha.55 contains the same
executables and staged runtime assets as alpha.54, with versioned packaged
ReadMe text, and advances no gameplay roadmap checkpoint. The persistent
`ship-sparkpaw-checkpoint` and
`run-sparkpaw-test-cycle` skills now require creation-capable classic LHa,
`-lh5-` member inspection, CRC validation and independent extraction checks
where applicable, so later sessions cannot silently regress to stored LHA
artifacts.

The final alpha.55 release build and host suite pass. The bootable DOS1/FFS ADF
uses 1,445 blocks (722 KiB) and leaves 315 free; the one-block change from
alpha.54 is versioned packaged ReadMe text, not game data. Independent Lhasa
extraction matches both staged drawers byte-for-byte. HD LHA contains 34
`-lh5-` files at 600,711 bytes; WHDLoad LHA contains 36 at 596,753 bytes. Final
SHA-256 values are: HD LHA
`c9d995d9e93a37c828bd7736cc73c7f2ffa22b5b5faf3a3beabd14714d30818c`,
HD ZIP `9b0fcabe4d104db15a302a62fe1456baccc51b4846b68c8085d5d992f5eb0542`,
ADF `c51ac8796e966ebcae790dc871725d901b69e4806f2a497d9926eb25a24e1f66`,
WHDLoad LHA
`ab276e53ae1ea354f7ffc1ab4e8ffbc14b9a2d026c31b314e462e22d5b3c8a53`
and WHDLoad ZIP
`cd0f442253ac9e0bd123301e97f9a988a5d615112157311d082dca5cc0bb22ca`.

Supplied real-A1200/68030 alpha.55 evidence subsequently rejects WHDLoad intro
traversal. A 14.775-second phone recording shows story plate 1 and both of its
passages normally, followed by the authored fade and a clean return to
Workbench exactly where plate 2 should load. The ordinary alpha.55 HD build
does not reproduce the failure. SnoopDOS shows successful WHDLoad package/asset
discovery, and Workbench displays the 31-character parent drawer shortened to
`Sparkpaw-0.6.0-alpha.55-WHDLoa`. Because that drawer still launches and plate 1
works, its truncation is evidence of a name boundary but not a sufficient root
cause. The plate-2 source filename is also 31 characters, but the passing HD
path means its role remains a WHDLoad-specific hypothesis rather than a proven
general AmigaDOS failure.

The focused `Sparkpaw-WHDIntroDiag` package keeps the alpha.55 WHDLoad assets,
memory configuration, slave and runtime behavior, changes only bounded intro
load logging and uses a 21-character root drawer. It records before/after each
plate load, detailed Open/header/palette/allocation/plane/mask failure, IoErr
and Chip/Fast free/largest values to `data/whdintrodiag.log`. Host tests and the
native 68020 diagnostic build pass. The ZIP contains one self-contained drawer,
passes archive testing, is 596,737 bytes and has SHA-256
`a81b3fe822ce2b9678d163bb20678b4fd5adb89e513a1ae9c8cae4aec1901026`.
Real-hardware diagnosis remains pending the returned log.

The supplied photo of `data/whdintrodiag.log` resolves the failure class. Plate
1 records `load_ok`; plate 2 records `load_failed failure=open ioerr=205`.
AmigaOS NDK `dos/dos.h` defines 205 as `ERROR_OBJECT_NOT_FOUND`. About 2.07 MB
Chip remains free with a 1.92 MB largest block, and about 7.70 MB Fast remains,
so bitmap allocation and plane reading are not reached. Because the diagnostic
root is only 21 characters, the earlier versioned parent truncation is not the
necessary cause. The requested `intro-plate-02-instruction.spbm` component is
itself 31 characters and is the proven WHDLoad name-resolution boundary. The
next correction should use short WHDLoad runtime aliases and preserve ordinary
HD paths until focused real-hardware retest accepts complete intro traversal.

Comparison against the real-hardware-working alpha.52 commit finds no change to
the five intro path strings, WHDLoad compile flags or BootDOS slave through
alpha.54; the intervening runtime change is the ready-menu atlas. Alpha.55 then
replaces hand-written stored level-0 LHA members with classic compressed LHa
output and explicit directory records, while the focused diagnostic was
delivered as ZIP. The regression is therefore not a newly introduced intro
filename in the executable. Earlier success depended on the old archive and
extractor preserving names beyond the compatibility boundary; current
extraction leaves the requested object unresolved. A manifest audit finds
three packaged components over 30 characters: intro plate 2 (31), intro plate 3
(36) and the ready-menu atlas (32). Correct all three for WHDLoad so later
startup does not merely move the same failure to plate 3 or the ready screen.

The isolated correction compiles `SPARKPAW_WHDLOAD_SHORT_NAMES` only into
`build/sparkpaw-whdload-shortnames`. The packager copies the canonical bytes as
`intro2.spbm`, `intro3.spbm` and `readymenu.spbm`, excludes their long aliases
and rejects any remaining runtime component longer than 30 characters. The
resulting self-contained `Sparkpaw-WHDShortNames.zip` has 31 runtime files, a
maximum component length of 28, passes ZIP integrity and contains an executable
whose embedded paths select only the three short aliases. Full host tests and
native 68020 compilation pass. The 596,734-byte ZIP has SHA-256
`86338e2cc449b57496fa15da51479b37b40d32614b7da0ad988af9e4180d8aba`.
The earlier diagnostic ZIP is preserved under `dist/older-builds`.

MrDig's supplied real-A1200/68030 test accepts the short-name correction through
all five intro plates, title, loading, charging and the ready menu. This proves
the component-length diagnosis for the tested WHDLoad route. Alpha.56 promotes
the aliases to the only canonical SPBM names for both ordinary HD and WHDLoad;
ADF packing consumes those same sources for its already-short SPR1 streams.
The external WHDLoad artifact retains its descriptive versioned filename, but
the archived top-level drawer becomes `Sparkpaw-0.6.0-a56-WHDLoad`. Both
packagers now reject any extracted component over 30 characters. The isolated
short-name compiler/package mode is removed because production itself carries
the accepted rule. Asset bytes, renderer, gameplay and memory configuration
are unchanged.

The final alpha.56 host suite, native HD/ADF/WHDLoad builds and package checks
pass. The ADF uses 1,446 DOS1/FFS blocks (723 KiB), leaving 314 free. HD and
WHDLoad ZIP/LHA manifests have a maximum extracted component length of 28;
independent Lhasa extraction compares byte-for-byte with both staged drawers.
The HD LHA contains 34 `-lh5-` files and is 600,695 bytes; WHDLoad contains 36
and is 596,375 bytes. Final SHA-256 values are: HD LHA
`b1cbe338d7c3297a70cc52e92cd67a436f1794c032d0f1f5c078bb1c923d5683`,
HD ZIP `5a5f35b1adf363b29a600497a88befa1433af0637f04fc7b93b6deea6494bd4c`,
ADF `33f3ac0473d3f00a5c5cd5345fa1be26871fb4518f238592387e9472d095bc7e`,
WHDLoad LHA
`f4843eecf730c0e2394182cfca226aad93906385171738e44db96c3575f8f87f`
and WHDLoad ZIP
`c9eed9128131394bbf7e4724f7f5d0ad3041af9a55abe65f9e8ccc18841dd62f`.

### 28 August 2026 - Alpha.57 makes ready-menu publication tear-free

Supplied real-A1200 ADF footage rejects intermittent alpha.54-style menu
transitions. Consecutive frames occasionally contain cyan selector fragments
from both START GAME and OPTIONS while the fixed background remains stable.
Source inspection identifies the ownership violation: `showReadyMenuState()`
waits until raster line 252 and then copies the 160x104, six-plane patch
directly into the bitmap being fetched. The patch remains visible through
approximately hardware line 265, and its 12,480-byte Fast-to-Chip copy can also
cross the next field boundary. Plane-by-plane publication therefore exposes a
mixed old/new state. Floppy I/O is not active at this point.

The correction allocates one additional 320x256 six-plane displayable bitmap
(61,440 bytes Chip RAM). Both buffers receive the complete ready background
while black. Each menu change patches only the hidden buffer, builds the
inactive complete Copper list around it and publishes all six bitplane pointers
together at the next owned PAL boundary. The displayed bitmap is never written.
Gameplay, controls, assets, audio and Stage 5L/H7 remain unchanged. A host
regression protects the hidden-buffer/Copper-swap contract and the native VBCC
68020 build passes.

The self-contained production-style FS-UAE/68030 HD gate is accepted after
rapid main-menu and JUMP/FIRE switching; no logger was present, so this adds no
cadence measurement. The identical minimal real-A1200 HD ZIP is staged for
testing. Alpha.57 packages the candidate now so ADF and WHDLoad can be tested
in parallel, but real-A1200 HD, ADF and WHDLoad runtime acceptance remain
pending supplied results.

The complete alpha.57 host suite, native HD/ADF/WHDLoad builds and initial
package validation pass. The bootable DOS1/FFS ADF uses 1,449 blocks (724 KiB)
and leaves 311 free; this remains package evidence until supplied ADF runtime
testing accepts it.

Independent Lhasa extraction matches both staged drawers byte-for-byte. The HD
LHA contains 34 `-lh5-` files and is 601,194 bytes; WHDLoad contains 36 and is
596,655 bytes. Final SHA-256 values are: HD LHA
`a39e02ec1324cdde9a16a3bc7078cac55efdf8e2d9f2757a0de4a72599d8abe2`,
HD ZIP `3794f85b193e78a07afa451494da00e23fac133fdcc36d310885f644547bf772`,
ADF `6d58eb808ca79cef5558cd8979af5a946932563abda7b7c1304a9687da4011ee`,
WHDLoad LHA
`79f2d9ec62715237206c4d1c2ee0662afd2eb7947e62847ea9663eae3507d66c`
and WHDLoad ZIP
`03dd43455b627948b55e95d5fe95a5a50fb8ea42f59b3c641566eb031285cd99`.

### 28 August 2026 - Alpha.58 closes the real-A1200 ready-screen glitch

The first alpha.57-style hidden-buffer correction passed rapid switching in
FS-UAE/68030 but remained rejected on the real A1200: small fragments still
appeared during fast menu changes. A second candidate armed only COP1LC during
hardware lines 100..249 and omitted COPJMP1, allowing the Copper to adopt the
inactive list through its natural vertical restart. That candidate also passed
FS-UAE/68030, yet real hardware still showed isolated glitches even when START
GAME or OPTIONS was left completely untouched for 15 seconds or longer. This
idle reproduction disproved list publication as the complete explanation.

Source review then found the independent hardware-only ownership defect.
`platformFinishTakeover()` enabled `DMAF_SPRITE` while the active title,
loading and ready Copper lists contain no sprite-pointer moves. Agnus could
therefore fetch through stale pointers inherited from prior display state and
occasionally overlay fragments despite an idle CPU and unchanged bitmap. The
focused correction keeps sprite DMA disabled throughout presentation and
enables it only in `platformSwitchCopper()`, whose sole production caller
installs the gameplay Copper list that initializes the attached-player sprite
pointers. A regression test now protects this sequencing.

The final production-style candidate retains both safety layers: the displayed
ready bitmap is never patched, COP1LC is armed without COPJMP1, and sprite DMA
starts only with its pointer-owning gameplay Copper list. Supplied FS-UAE/68030
HD testing accepts two-minute idle START GAME and OPTIONS holds, rapid menu and
JUMP/FIRE switching, and gameplay entry. Supplied real-A1200/68030 HD testing
then reports no glitches in the matching executable. This accepts only those
two HD paths; ADF, WHDLoad and FS-UAE/68020 runtime acceptance remain open.

The durable rule is broader than this screen: do not enable any custom-chip DMA
channel until the active display state owns and initializes every pointer that
channel may fetch. A static CPU loop is not proof that DMA-visible state is
static. For intermittent presentation faults, an idle hold is therefore a
required discriminator alongside rapid state switching.

The complete alpha.58 host suite and native HD/ADF/WHDLoad builds pass. The
bootable DOS1/FFS ADF uses 1,449 blocks (724 KiB) and leaves 311 free.
Independent Lhasa extraction matches both staged drawers byte-for-byte. The HD
LHA contains 34 `-lh5-` files and is 601,246 bytes; WHDLoad contains 36 and is
596,720 bytes. Final SHA-256 values are: HD LHA
`069f474984bb0ae6eaa8b69b807307bdfa001c91ad209bacc910b4052c6d2eb2`,
HD ZIP `b92b630d95f33fdb317065b4ac6896ec87fca16329009ecde6d1854eddc23ab2`,
ADF `26c3a01a2d56e91474d7a314895d6d26eca4e6930c531aa074b5d519ff187660`,
WHDLoad LHA
`5f9384e27da7f5f1c92cafcd11fbceeb754d1915e01e13f939779e15995460ca`
and WHDLoad ZIP
`b7f9ab3732cb6e65d0dccd9128c1da151f423e843de77d85083e76a2af94bd14`.
