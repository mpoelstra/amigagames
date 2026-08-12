# Codex handoff: Amiga game workspace

Last updated: 13 August 2026

## Purpose and source of truth

This repository contains three native Commodore Amiga game projects. Sparkpaw
is the active AGA A1200 project. At the start of every task:

1. read this file completely;
2. read `sparkpaw/README.md`;
3. inspect current source, `git status`, recent commits and tags;
4. use source code and generated manifests as final authority.

The former long-form handoff is preserved verbatim as
`docs/DEVELOPMENT_HISTORY.md`. It contains the complete development
chronology, rejected experiments, supplied-test interpretation and older phase
checkpoints. It is historical evidence, not the current specification. Read
the relevant section before reopening a renderer/palette/timing problem,
reusing a rejected animation technique, investigating a regression with known
symptoms, or changing an accepted contract whose rationale is unclear.

This handoff is intentionally only the compact operational starting point for
continuing efficiently in a fresh task. It is not a running diary. At every
completed feature/phase, and always before a requested commit and push:

1. audit this handoff against the actual source, README, current phase and test
   evidence;
2. update it to contain only the resulting canonical state, invariants, open
   limitations and next step;
3. append the relevant implementation history, decisions, rejected attempts,
   supplied FS-UAE/hardware results and technical rationale to
   `docs/DEVELOPMENT_HISTORY.md`;
4. check that neither document presents an obsolete intermediate state as
   current, then include both documentation updates in the checkpoint commit.

Even when a feature required no rejected experiment, record its accepted
result and verification status in the history. Do not let the compact handoff
grow back into a chronological transcript; move detail to the history instead.

## Repository and workflow

```text
amigagame/
  CODEX_HANDOFF.md                     current canonical handoff
  docs/DEVELOPMENT_HISTORY.md          complete historical engineering log
  sparkpaw/                            active AGA action platformer
  chipsnake/                           finished/releasable prototype
  mrdigs-futsal/                       finished/releasable prototype
  backups/                             ignored local snapshots; never delete
  ACM_PDF/                             ignored Amiga reference manuals
```

- Branch `main` is the accepted shared state.
- Tag `sparkpaw-pre-modularisation` is the accepted four-beetle baseline before
  the source split.
- Keep each game self-contained. Do not add dependencies on sibling projects.
- Preserve ignored backups, recordings, logs, toolchains and test evidence.
- Work in small reviewable steps. Do not combine renderer changes with
  unrelated gameplay or asset changes.
- Always run `make` and `make release` after implementation work.
- MrDig performs authoritative FS-UAE and hardware testing. Never claim either
  unless he supplied that result.
- Use Git as normal recovery. Make a dated local backup as well before risky
  renderer, asset-format, audio/physics or bulk source-movement work.

Normal Sparkpaw commands, run from `sparkpaw/`:

```sh
make
make release
make bench       # isolated renderer bench only when relevant
```

The target is PAL A1200/68020 with 2 MB Chip RAM; Fast RAM is recommended.
The workspace-local VBCC/NDK toolchain lives under ignored `.toolchain/`.

## How to handle supplied test evidence

- A screenshot is evidence for palette, scale, grounding, clipping and stable
  corruption. A short MOV is preferable for cadence, one-frame glitches,
  transitions, scrolling, collision and audio timing.
- For video, inspect metadata and consecutive frames. Do not diagnose a motion
  issue from only the first and last frames.
- Separate observation from hypothesis. Correlate visible frames with exact
  animation IDs, timers, facing and generated indexed assets.
- For Bob corruption, distinguish source pixels, masks/planes, cache indexing,
  restore dimensions, Blitter timing and display timing before changing code.
- Phone recordings can contain refresh beating, scaling and exposure effects.
- Make one narrow fix, rebuild both targets and provide a focused regression
  checklist. Commit only after the result is accepted where visual/runtime
  confirmation is required.

## Sparkpaw: current canonical state

### Display and renderer contracts

- PAL 320x256 at 50 Hz, five-screen 1280x256 resident world.
- AGA dual playfield: four-plane foreground plus three-plane rear; rear scrolls
  at quarter camera speed.
- The player is a 48x48, 15-colour object built from three attached hardware-
  sprite pairs and therefore consumes six sprite DMA channels.
- Copper pointer/palette updates are staged after hardware raster line 100,
  after their current-frame consumption and before the next wrap.
- The Copper switches from the scrolling foreground bitmap to the separate HUD
  bitmap at hardware line 252 (`44 + HUD_TOP`).
- The synchronized Bob restore/draw pass starts at line 253 and finishes before
  the next wrap. Line 300 is historical and must not be restored as the active
  threshold: it left too little PAL time for shifted 64x64 four-plane Bobs.
- Preserve packed planar caches, camera culling and this pass order:
  projectile erase, enemy restore, collectible restore, collectible draw,
  enemy draw, projectile draw, final Blitter wait.
- Background restore uses synchronized Blitter copies; Bob drawing uses the
  standard cookie-cut minterm. Never reintroduce CPU read-modify-write
  compositing in displayed Chip RAM.
- `frontClean` and `frontDisplay` remain separate. A Bob's stored drawn type,
  X and Y describe what must be restored even when a runtime slot changes owner.

The line-253 timing and four-plus-three renderer are user-accepted in FS-UAE.
No real-hardware verification has been supplied. Consult
`docs/DEVELOPMENT_HISTORY.md` before changing Copper allocation,
palette banks, BPLCON state, sprite priority or Bob timing.

### Player contract

- The original accepted player baseline is frames 0..49.
- Eight hurt frames were appended without renumbering it: standing 50..53 and
  crouched 54..57. Current total: 58 frames.
- Preserve family-wide scale, stable foot anchors, mirrored facing and the
  six-channel packed hardware-sprite format.
- Accepted movement includes run, jump/fall, landing, crouch/crouch-walk,
  momentum turn, idle performance, grounded/air/crouched shooting and hurt.
- Crouched fire uses its dedicated low pose and muzzle; standing and airborne
  fire remain separate.
- Three hearts are six half-heart units. Contact damage, knockback, bounded
  input lock, invulnerability blinking, life loss and level restart are active.
- Losing a life restarts the level but preserves collected diamonds. Reaching
  the temporary far-right endpoint also reloads the current test level; it is
  a placeholder for later `LEVEL_COMPLETE -> next level`.

### HUD, collectibles and audio

- The fixed 320x48 bottom HUD uses separate presentation buffers and modular
  health/lives/diamond patches. Never CPU-compose into a displayed buffer.
- Diamonds are masked, camera-culled Bobs with synchronized restore/draw.
  The counter awards a life at 50 and supports lives `x1` through `x9`.
- Up to six plasma projectiles share a packed synchronized Bob pipeline.
- Paula channel 0 owns rapid plasma playback. Prioritized gameplay effects use
  channel 1. Channels 2-3 remain reserved for a future music layout.

### Enemy and spawn foundation

- Level data contains typed persistent spawn candidates with authored safe X
  ranges, `{left,right,groundY}` surfaces, initial direction and respawn policy.
- Each complete level load selects 4-6 beetles. Strider level data contains two
  required and one optional candidate, but the current runtime gate activates
  only the first two; do not claim three visible/active Striders yet. Positions
  and fixed-point speeds are randomized from 48, 96 and 192.
- A generic four-slot active enemy pool loads candidates near the camera and
  parks state safely outside it. Persistent encounter count is not simultaneous
  Bob count.
- Destroyed respawning encounters receive a 250-500-frame cooldown and respawn
  only while their complete patrol surface is outside the camera. Enemies can
  therefore reappear indefinitely when the player backtracks.
- Enemy extinction no longer completes/reloads the level.

Clockwork beetles:

- 32x24, three-plane masked Bobs; four walk, one hit and four destruction frames.
- Two HP, independent state and speed-coupled cadence.
- Their low hitbox is intentionally reached by crouched shots; ordinary standing
  and airborne shots pass over them.
- Contact damage, hit/death effects and off-camera respawn are active.

Clockwork Storm Striders (accepted through Phase 5D):

- Upright 64x64, four-plane masked Bobs in the shared foreground `FRONT16` bank.
- Two guaranteed encounters currently patrol a raised platform and a long floor
  route. One optional third candidate exists in level data but is still blocked
  by the current runtime-readiness gate.
- Slots 0..7 are the accepted rigid mechanical gait. Its non-human stiffness is
  deliberate and was accepted as consistent in FS-UAE.
- Slot 8 is a planted premium frontal turn held for six frames only at an
  authored patrol extremum. Facing changes when the hold ends, then walk restarts
  at frame 0.
- Walk advancement must wrap immediately with `(animFrame + 1) & 7`. Allowing
  frame 7 to increment to 8 leaks the front pose for one frame mid-walk.
- Incidental solid/missing-support safety reversals must not select slot 8.
- Slots 9..17 remain reserved placeholders for later attack/shooting, hurt/hit
  and death. Do not spend them on extra walk interpolation.
- Slots 18..23 are appended Phase 5D traversal poses: two cyan compression
  stages, flight, descent, planted landing and recovery. The original 0..17
  contract is unchanged; the packed cache now contains 24 frames.
- The required raised-platform Strider owns one explicit, one-way test-level
  link from its current platform to the adjacent lower floor. Link geometry,
  launch velocity and gravity live in level data rather than AI code.
- Complete telegraph, fixed-point ballistic, landing and recovery state is
  copied into persistent spawn state when a camera slot is parked. Grounded
  patrol resumes on the authored destination only after recovery.
- The destination patrol runs beneath the clear underside of its x=320..415
  start platform and stays between the x=288 column face and x=496 low-platform
  face. Inset-foot bounds prevent the 64px body crossing either solid without
  adding the generic body-aware navigation reserved for Phase 5E.
- Striders are currently non-interactive: they do not yet absorb shots, damage
  Sparkpaw or die/respawn through combat.
- Logical collision cells remain at their authored Y. Drawing uses the accepted
  two-pixel visual offset because source rows 62-63 are transparent.

The final Phase 5C.3 FS-UAE test confirmed stable gait, no mid-walk front-pose
leak and correct endpoint turns for raised and floor Striders. No real-hardware
test exists. Phase 5D was subsequently accepted from MrDig's supplied FS-UAE
tests: the Strider performs the authored jump, turns before the low platform and
then patrols beneath its original raised platform. No real-hardware result has
been supplied for Phase 5D.

## Current roadmap

Completed and accepted:

- Phase 1: incremental source modularisation.
- Phase 2: title/loading/application states.
- Phase 3: player damage, hurt art/audio, HUD, lives and diamonds.
- Phase 4: generic level encounters, variation and safe off-camera respawn.
- Phase 5A/5B: Strider contract, visual direction and AGA colour proof.
- Phase 5C.1: typed multi-enemy level data.
- Phase 5C.2: packed 64x64 Strider Bob integration.
- Phase 5C.3: raised/floor patrol, accepted eight-frame gait and endpoint turn.
- Phase 5D: one authored raised-to-floor jump link with persistent traversal
  state, accepted landing/recovery and safe destination patrol.

### Next: Phase 5E — broader authored Strider traversal

Generalize the accepted 5D proof without turning it into arbitrary pathfinding:

1. give authored surfaces stable IDs and connect them with explicit links;
2. support links and traversal in both world directions;
3. add return links, low/high transitions, gaps and water;
4. define missed/blocked landing and off-camera transition rules;
5. keep cheap logical world-space simulation outside the camera and restore
   complete physics/animation before a Strider enters the visible region;
6. let Striders appear to roam the whole level from either side without camera-
   edge spawning, resetting when jumped over or beginning unseen attacks.

The camera controls rendering, not route ownership. Do not simulate four Bobs
offscreen, but preserve route, position, direction and traversal state. Keep
combat animation/damage and renderer changes separate from this navigation pass.

### Later phases

- Strider combat pass: projectile/contact hitboxes, attack/shooting telegraph,
  hurt/hit, death and safe respawn. Preserve reserved slots or deliberately
  expand the cache contract; do not silently alias jump and death frames.
- Phase 6: level/progression work. First measure a resident 2048px/eight-screen
  repeated-art memory experiment, then build an eight-screen collision/pacing
  greybox, checkpoint/progression state, encounters and finally unique art.

Level 1 target: an original Storm Ruins route approximately 35-50 seconds for a
practised run and one to two minutes for a cautious first playthrough. The cited
ThunderCats Amiga level 1 (about 3:32-4:10 in the supplied video) is only a
pacing/behaviour reference. Do not copy maps, characters, art, music or exact
timing. The eventual larger enemy should continue through the viewport, survive
being jumped over, and traverse authored platforms/gaps/water rather than act as
local beetle-like decoration.

## Known limitations and backlog

- No clean Workbench exit; reset the Amiga/emulator to leave gameplay.
- Current world has continuous floor and no implemented water/gap hazard yet.
- Far-right reload is temporary; real level completion/progression is pending.
- Strider jump/navigation and all Strider combat are pending.
- Music is pending; define Paula ownership before integration.
- Game-over presentation and broader checkpoint/progression flow remain pending.
- Stock 68020/2 MB Chip RAM performance and real-hardware timing remain unproven.
- ADF-only compression, asset reconstruction and eventual multidisk options are
  researched but deliberately unimplemented. Preserve the HD path and consult
  `sparkpaw/docs/ADF_STORAGE_STRATEGY.md` before changing asset formats, release
  layout, disk I/O or adding a trackloader.

## Asset and animation rules

- Runtime uses deterministic project-owned SPBM assets. Rows are word aligned;
  masks follow planar bitmap data where applicable.
- Author right-facing animation families and generate exact indexed mirrors.
- Crop transparent padding, use one scale for a complete family and bottom-align
  by opaque foot pixels. Never auto-fit every pose independently.
- Collision dimensions are gameplay data, not transparent image bounds.
- Keep transparency at pen 0 and remap opaque pixels to the established palette.
- Update generator, frame metadata, C selectors, Makefile dependencies, previews
  and documentation together when changing a frame contract.
- Inspect final indexed previews at integer scale before asking for FS-UAE review.
- Record new generated-art prompts in `sparkpaw/docs/IMAGEGEN_PROMPTS.md`.

For rejected Strider gait sheets, renderer benches, palette migrations,
first-approach corruption, prime-restore failure and the reasoning behind the
rigged walk, consult `docs/DEVELOPMENT_HISTORY.md` before retrying an
old technique.

## Other projects

ChipSnake and MrDigs Futsal are finished/releasable prototypes. Their detailed
technical history now lives in `docs/DEVELOPMENT_HISTORY.md` because
Sparkpaw is active. If work switches project, read that project's README and
inspect its source/build rules before editing. Do not apply Sparkpaw's AGA
renderer assumptions to those projects.

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
