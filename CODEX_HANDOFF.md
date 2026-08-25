# Codex handoff: Amiga game workspace

Last updated: 25 August 2026

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

Current release is `0.6.0-alpha.45`, Phase 6C.1. A normal release contains:

- `Sparkpaw-0.6.0-alpha.45.lha`
- `Sparkpaw-0.6.0-alpha.45.zip`
- `Sparkpaw-0.6.0-alpha.45.adf`
- extracted review drawer `Sparkpaw-0.6.0-alpha.45/`

MrDig mounts `sparkpaw/dist/` directly as an FS-UAE HD volume. Every
user-facing FS-UAE HD test or diagnostic drawer must therefore be created
directly under `sparkpaw/dist/`, following the established clearly named
stage/checkpoint structure and including its executable, `ReadMe.txt` and
complete required `assets/runtime/` subtree. Do not deliver such drawers only
under `build/` or `build/test/`; those are internal build intermediates and are
not visible in the mounted HD.

For every future A/B test, create one fully self-contained subdrawer per
variant. Each subdrawer must contain its own executable, complete runtime
assets and ReadMe, so `PROGDIR:renderdiag.log` is naturally unique. Never ask
MrDig to rename or move a log between variants.

Keep the mounted `sparkpaw/dist/` root uncluttered. It should contain only the
current alpha release artifact set and the single currently active diagnostic
or A/B test set; `my-files/` and `older-builds/` remain as fixed storage
drawers. As soon as a diagnostic set is superseded or its evidence has been
preserved in `sparkpaw/testresults/`, move its complete self-contained drawers
and matching FS-UAE `.uaem` metadata intact into `sparkpaw/dist/older-builds/`.
Do not leave multiple generations of debug/test drawers in the mounted root,
and never delete their logs while tidying it.

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
- Alpha.44 packages the supplied FS-UAE/68030-HD-accepted H7 seam correction
  and accepted Stage2 defaults without changing Stage 5L geometry, assets or
  gameplay. Its DOS1/FFS ADF validates at 1,195 blocks used and 565 free.
  Alpha.44 HD/ADF gameplay on the real A1200 and ADF gameplay on Analogue
  Pocket remain pending supplied tests; package validation is not acceptance.
- Alpha.45 packages three more isolated Stage2 defaults accepted in supplied
  FS-UAE/68030 and FS-UAE/68020 HD tests: direct Strider traversal lookup,
  invariant Bob-register setup and a specialized aligned 16px entering-column
  copy. The latest matched 68020 A/B raises cadence 44.47 to 45.55 FPS and
  lowers ring-roll p95 9,878 to 2,782 CIA ticks; Bob-pass p95 falls 16,308 to
  9,142. Final supplied testing accepts alpha.45 presentation and cadence on
  the approximately 34.5 MHz real A1200/68030 from both HD and physical ADF,
  and accepts the Analogue Pocket 68020 ADF path. Its bootable
  DOS1/FFS ADF validates at 1,197 blocks used and 563 free; this is package
  construction evidence only.
- A post-alpha.45 minimal-cadence diagnostic removes nested CIA scopes and Bob
  family raster timing while retaining renderer-boundary cadence sampling.
  Supplied FS-UAE/HD testing reports normal presentation. FS-UAE/68030 records
  1,578/1,578 one-field intervals (50.00 FPS); FS-UAE/68020 records 1,104
  one-field and 33 two-field intervals out of 1,137 (48.58 FPS), with no
  three-field intervals or ownership violations. The immediately preceding
  fully instrumented 68020 reference measured 44.21 FPS under a different
  workload, proving that most of the apparent remaining 4--5 FPS deficit was
  profiler observer cost. Do not extrapolate this diagnostic result to ADF,
  Pocket or real hardware by itself. Those acceptances were supplied later as
  the separate final alpha.45 tests above.
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

## Current work order

### 1. Preserve the completed Stage 5L/H7 renderer baseline

Stage 5L plus H7 is the immutable renderer baseline. Do not change the fixed-HUD
FMODE/pointer split, ring ownership, early-fetch geometry, the wide Sparkpaw
pair, art or gameplay during residual performance work without new renderer
evidence and a separately gated correction.

H5 established bitplane rather than sprite origin. H6A then removed the seam
by masking FRONT16 colours on only the final transition scanline; H6B's REAR8
mask did not. The user explicitly accepts H6A in FS-UAE/68030 with no other
visible corruption; its log records 49.93 FPS and zero ownership violations.
The same operation was production-compiled and subsequently accepted by the
user in FS-UAE/68030 HD as H7. H7 intentionally created no log.

The original H7 gate accepted only FS-UAE/68030 HD. Final alpha.45 testing now
also accepts the complete result on real-A1200/68030 HD, physical ADF and
Analogue Pocket ADF. Do not reopen the Copper split, Stage 5L
fetch geometry, rolling ownership or wide-sprite layout during performance
work without new evidence of a renderer regression.

The title contract is unchanged: 35 black PAL frames after display takeover,
24 fade frames and 225 fully visible title frames. Faster loading before title
takeover must not be confused with a shortened Indivision stabilization delay.

### 2. Preserve the completed alpha.45 performance checkpoint

Use `sparkpaw/docs/PERFORMANCE_68020_PLAN.md`.

The broad C/assembly audit and ranked table produced the alpha.44/45 gains.
The final low-overhead diagnostic records 48.58 effective FPS on FS-UAE/68020:
1,104 one-field and 33 two-field intervals out of 1,137, no three-field misses
and zero ownership violations. This near-50-Hz result is a protected regression
baseline for every later feature and refactor.

The residual profiler and follow-up review already split CPU work, custom
register setup and `WaitBlit`, regenerated VBCC output, checked runtime copies
and re-audited Fast/Chip allocation. Most of the prior apparent 4--5 FPS gap was
observer cost from the detailed profiler. No forgotten continuous large copy,
runtime asset load or compiler helper remains as an active big-gun candidate.

Future candidate order, only after a measured regression or new feature cost:

1. remove an actual Bob job or wait with proven safe CPU/Blitter overlap;
2. coalesce projectile geometry and enemy work rather than add a prepass;
3. add a Fast-RAM mirror only for measured repeated CPU reads from Chip;
4. use coarse hardware-facing scopes if a hardware-only gap returns;
5. do not repeat rejected diamond persistence, pointer-precompute, inline-wait,
   fetch-pruning or tiny-Blitter experiments unchanged.

Research 68020 scheduling, AGA bus arbitration, Blitter behavior and Fast/Chip
placement only for a concrete measured question. Keep any prototype isolated:
first FS-UAE/68030 presentation, then matched FS-UAE/68020 timing. Production
must remain diagnostic-free. A fixed 25 Hz game update with 50 Hz display
service remains a separate last-resort prototype, never uncontrolled skipping.

The diagnostic implementation is compiled out of production already. Split it
from `renderer.c` for auditability, and later split Copper/ring/Bob/sprite
modules, only through small commits whose normal executable remains byte-for-
byte identical. Source-file size and excluded `#ifdef` branches are not FPS
costs. Do not combine this mechanical refactor with a renderer optimization.

Preserve sprites, colours, 4+3 dual playfield and art. Do not retry alpha.37
bounds or alpha.38 full viewport copies. Benchmark candidates in isolation and
gate first on unchanged 68030 presentation, then FS-UAE 68020 timing.

Accepted Stage2 Sprite Stage result: A retains the unconditional
two-channel 1,600-byte Fast-to-Chip sprite image copy. B caches facing/frame per
alternating Chip stage and skips only identical image copies; position/control
words and Copper pointers remain per-update. The cache state-machine host test
and full host suite pass, and actual VBCC 68020 assembly confirms a direct
conditional branch around `CopyMem`. Gate the two self-contained drawers in
FS-UAE/68030 and FS-UAE/68020 testing reports both visually correct. On 68020,
sprite-stage median falls 423 to 74 CIA ticks and average 407 to 236; unmatched
whole-run cadence is 28.73 versus 29.92 FPS and is not attributed wholly to the
cache. B is now production default, with A preserved behind
`SPARKPAW_SPRITE_STAGE_ALWAYS_COPY_REFERENCE`. Overall 68020 cadence remains
rejected; continue with a larger measured hotspot.

Accepted Stage2 canonical-restore result: production rolling targets now keep
only their display bitmaps. Target-local history retains canonical world X as
well as physical ring X, so old Bobs restore directly from the canonical clean
world without changing draw order or bounds. Supplied FS-UAE/68030 and
FS-UAE/68020 HD testing reports the A/B variants visually correct with zero
ownership violations. On 68020, `ring_roll`, `ring_dynamic` and
`bob_compact_target` averages fall by about 39--41%, and the complete Bob pass
falls 26.2% in the supplied runs. Prepared-peak free Chip rises by exactly
319,488 bytes in the A/B gate. The final production-default FS-UAE/68030 log
records 49.95 FPS (1,147 one-field and one two-field interval), zero ownership
violations and 785,872 bytes free Chip at prepared peak; the user reports clean
presentation. Preserve the old architecture behind
`SPARKPAW_TARGET_CLEAN_REFERENCE`. Overall 68020 cadence remains rejected. No
ADF, Analogue Pocket or real-A1200 acceptance is claimed for this Stage2
change.

Accepted Stage2 enemy-state result: loaded runtime enemies are authoritative;
their complete state is copied to persistent spawn storage only when camera
parking occurs. Supplied FS-UAE/68030 and FS-UAE/68020 HD testing reports
normal presentation and correct parking/reactivation. On 68020, `enemies`
average falls 1,860 to 1,762 CIA ticks (-5.3%) and `game_update` average falls
3,865 to 3,707 (-4.1%). Retain this as default and keep the former per-tick
copy behind `SPARKPAW_ENEMY_COPY_EVERY_TICK_REFERENCE`. No ADF, Pocket or
real-hardware acceptance is inferred.

Accepted Stage2 phase-start result: the rolling main loop now starts its next
game update and inactive-target composition immediately after fixed-boundary
publication instead of idling until PAL raster line 100. Supplied FS-UAE/68030
and FS-UAE/68020 HD A/B testing reports both variants visually normal with zero
ownership violations. On 68020, effective cadence rises from 27.45 to 35.81 FPS
(+30.5%); one-field intervals rise from 17.9% to 60.4%, two-field intervals
fall from 853 to 428 and wraps from 875 to 466 under comparable measured work.
Immediate start is the production default; preserve the old gate only behind
`SPARKPAW_UPDATE_LINE100_REFERENCE`. Overall 68020 cadence remains below 50 Hz.
No ADF, Pocket or real-A1200 acceptance is inferred.

Supplied FS-UAE/68030 H3 conclusively rejects target-local collectibles.
Diamonds are stable away from enemies and collection is stable, but enemy Bob
restores erase overlapping target-local diamonds because canonical restore
sources are intentionally diamond-free. Performance no longer favors the
corrected route: `ring_dynamic` is 108 to 1 tick, but collectible restore+draw
rises roughly 544 to 772 and Bob-pass average is 3459 for A versus 3878 for B.
Further overlap invalidation would add redraws. H1/H2/H3 implementation, tests
and build targets are removed; retain canonical synchronization. No MOV,
FS-UAE/68020, ADF, Pocket or real-hardware acceptance is needed or inferred.

Accepted Stage2 hazard-cache result, initial 68030 gate: supplied FS-UAE/68030 HD A/B testing
reports normal presentation, collision, water death/reload and enemy behavior.
Both runs sustain 50.00 FPS. B's `game_update` average is 192 versus 211 CIA
ticks for A, but scene/projectile load is not matched closely enough to claim
that difference. The candidate uses 3 KiB non-Chip BSS; its subsequent 68020
decision is recorded immediately below.

The subsequent supplied FS-UAE/68020 HD A/B test reports both variants normal.
Cadence is effectively identical at 27.07 versus 27.05 FPS under unmatched
projectile load. Scoped results favor the cache: `player` average 922 to 875
ticks (-5.1%), median 756 to 599 (-20.8%), and `enemies` average 1760 to 1748
(-0.7%). Retain the cache as default and the prior scan behind
`SPARKPAW_COLLISION_HAZARD_SCAN_REFERENCE`; classify this as a small CPU win,
not a whole-game FPS gain. No ADF, Pocket or real-hardware claim is inferred.

Rejected Stage2 projectile-sweep result, initial 68030 gate: supplied FS-UAE/68030 HD testing
reports both variants visually and functionally normal at exactly 50.00 FPS.
Both issue 82 shot requests. Candidate B reduces `projectiles` average 20 to 12
CIA ticks, p95 91 to 46 and maximum 174 to 91; A has one additional hit and
kill, so scene load is close but not identical. Its subsequent 68020 rejection
is recorded immediately below.

The subsequent supplied FS-UAE/68020 HD result rejects that projectile-only
enemy prepass. Presentation remains correct, but `projectiles` average is flat
at 362 versus 366 ticks and median regresses 106 to 234. P95 improves 1291 to
847 and maximum 3529 to 1614, so spikes shrink, but ordinary frames pay for the
extra scan and total FPS does not improve. The implementation and build targets
were removed; retain the original pixel-ordered dispatcher. A future swept
candidate must coalesce geometry and enemy work together. No ADF, Pocket or
real-hardware claim is inferred.

Historical Stage2 collectible H1 is visually rejected in supplied FS-UAE/68030
HD testing: target-local diamonds flicker visible/invisible despite zero Copper
ownership violations. H1 did reduce `ring_dynamic` average 91 to 1 CIA tick
and Bob-pass average 3575 to 3081 (-13.8%). Root cause: entering roll columns
can overwrite a target-local diamond while its history still says drawn. H2
invalidated exactly those histories using an exhaustively tested entering-strip
overlap predicate; the subsequent H2 result is recorded below.

Supplied FS-UAE/68030 H2 also rejects target-local collectibles: A is correct,
but B still flickers and trembles between diamond Y positions despite zero
ownership violations. H2 retains a Bob-pass average reduction of about 13.2%.
Cause: the canonical `(frameCounter&3)==(index&3)` stagger is incompatible with
per-target history because alternating targets own opposite tick parities; an
index group can update one buffer but not the other. H3 removes that stagger
only for target-local mode and converges each target whenever its stored hover
Y is stale. H1/H2/H3 were ultimately rejected; H4's accepted replacement is
recorded later in this file.

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

Accepted Stage2 collectible H4 result: target-local diamond composition is the
production default. Supplied FS-UAE/68030 and FS-UAE/68020 HD testing reports
normal diamonds including enemy overlap, with zero ownership violations. On
68020, cadence rises from 35.31 to 42.15 FPS (+19.4%), `ring_dynamic` average
falls 3,940 to 101 ticks and Bob-pass average 11,086 to 8,499. Preserve the old
canonical diamond synchronization only behind
`SPARKPAW_COLLECTIBLE_CANONICAL_SYNC_REFERENCE`. Overall stock-68020 cadence
remains below 50 Hz. No ADF, Pocket or real-A1200 claim is inferred.

Accepted Stage2 alpha.45 results: direct traversal lookup raises matched
FS-UAE/68020 cadence 44.35 to 45.09 FPS and lowers `enemy_parked` average 792
to 601 ticks. Hoisted invariant Bob registers leave average cadence flat but
lower Bob p95 17,282 to 14,752. The specialized aligned-16px ring-column route
raises cadence 44.47 to 45.55 FPS and lowers `ring_roll` average 1,691 to 483
and p95 9,878 to 2,782 ticks. All supplied FS-UAE/68030 and FS-UAE/68020 HD
gates report normal presentation. Preserve the former routes behind explicit
reference flags. No ADF, Pocket or real-A1200 claim is inferred.
