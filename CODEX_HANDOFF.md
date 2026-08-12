# Codex handoff: Amiga game prototypes

Last updated: 12 August 2026

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
