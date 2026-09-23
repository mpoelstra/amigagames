# Drowned Turbines: system performance and renderer plan

> Reusable lessons from the full Drowned/Level1 optimization round: [retained 68020 lessons](DROWNED_TURBINES_LESSONS.md#retained-68020-optimization-lessons--september-2026). This is a synthesis of evidence, not a new experiment or an override of [current media/acceptance status](CURRENT_STATUS.md).

## 2026-09-22 — Retained-ring busy run1 analysed; bounded round complete

New90532byte log matches busy_00af8181ffe8fa93904b_sparse and staged executable;
complete footer. Preserved testresults/Drowned-busy-two-copy-020-run1.log/.txt.
Analysis and actual upper-route individual samples in build/drowned-busy-two-copy/
run1-analysis.json and run1-upper-samples.json.82samples;106invalid scope pairs
rejected,0incomplete. User says played; no new detailed visual verdict requested.
Previous B functional acceptance including finale remains intact.

Conditional PAL312-line phase estimates, IRQ/DMA/observer included:
- Ferry last groups4/8 gameplay medians5.96/5.83ms; AI nested1.92/1.92ms.
- Foreground/Bob totals11.22/10.77ms (valid n9/7); all restores2.76/2.69ms,
  ring/dynamic sync2.56/2.63ms, other draws2.56/2.37ms, enemy draw1.15/1.22ms.
  Child scopes are nested/nonadditive; sample sets differ by rejected clocks.
- Filtering actual upper y<140:8pre-reset and7post-reset samples. Actual
  game-begin->Bob-end same-sample elapsed median18.78/17.44ms (valid n7/7),
  max19.81/18.21ms. These are NOT sums of phase medians and exclude earlier
  rear work plus final publication wait. Rejected clocks can bias valid subset.
- Rear update median0.13ms; upper peaks3.85/3.91ms in3of15samples. Executed
  after publication, so it delays NEXT update. Cannot add a sample's own rear
  duration to that same sample's prepublication work as a causal deadline test.
  No water-versus-waterfall operation tag: peak operation is not proven.
- Publication-wait scope yields no valid ferry estimates because raw TOD/beam
  edge guard rejects pairs; exact deadline slack and pure CPU vsDMA unproven.

Raw observer cadence whole46.01FPS; upper pre41.59,post43.30. Not ordinary
FPS and not comparable directly with previous minimally observed A/B. Upper
recorded durations6.30/5.52s, shorter than requested25s; do not claim a long
stability run. One reset, no finale samples. prepared_chip261040 in this run;
external state varies, so allocation reduction remains separate from free-RAM
comparison. No50FPS claim.

Interpretation: remaining pressure is cumulative composition plus gameplay,
with occasional rear-update spikes. Enemy draw alone is not the dominant
cost; further enemy mask micro-tuning cannot remove the total load. Source
checks confirm other_draw groups pontoon/collectibles/core/extra-life/splash,
48collectible slots scanned, and asynchronous presentation still has serialized
composition through WaitBlit. A pipeline would need immutable state/history and
DMA lifetime changes. No single low-risk structural switch is justified by these
samples. Existing all-frame marker overhead also contaminates estimated costs.

Recommendation: conclude this bounded Drowned round with accepted ring gain,
transfer proven layout to level1 next (interlude deferred per user). Preserve
CPU/DMA overlap and rear precompute/scheduling as separate measured future
projects, not promise stable50FPS or blindly reduce update frequency. Direct
CPU water-height rejection remains a small unimplemented opportunity, not a
reason for another standalone micro A/B. No further runtime changes in this
analysis, no new test requested. Active Busy2 drawer/log left intact; no claim
FS-UAE stopped. No emulator launch, commit,push or release; alpha.8 unchanged.


## 2026-09-22 — Two-copy retained; final Drowned diagnostic round staged

User additionally played B to the end successfully, then approved: retain
Drowned improvement, one bounded remaining-hotspot round, level1 transfer
later; interlude deprioritized because already smooth. Full-level manual
functional acceptance now includes finale/station by user report. Original
A/B logs still have no finale samples: do not fabricate timing coverage.

Normal drowned-full now enables SPARKPAW_DROWNED_TWO_COPY_RING. Fresh make
build/sparkpaw-drowned-full (292196bytes) and independent plain control are
byte-identical to accepted B plain:
b78af227b85ffa7faa1e1ed8af637b98e6b01e74da6b3ef6f6ce4762f03fd738.
Music ON; FPS/busy/renderdiag compiled out in normal build. Existing startup
logging unchanged. Historical A/B builder strips new default before recreating
reference flags. Shared ring header added to Make header dependencies.
No level1/interlude renderer flags or published release files changed.

One diagnostic now active:dist/Drowned-Busy2-020-HD/Drowned-Test.
ID busy_00af8181ffe8fa93904b_sparse; SHA256
709b987fdbf3fcf662ae38d9df9248cf0c0dfc53813d42bc04e65a00dd5d5141.
Build records/plain/minimal/assembly in build/drowned-busy-two-copy (old
build/drowned-busy evidence preserved). Uses unchanged tested sparse observer:
1/31updates,16samples/region plus post-reset ferry bucket,144total,28512bytes
explicit Fast allocation. Per-frame inactive branches/bookkeeping remain;
active samples add clock reads/counts. Observer cost unmeasured. No per-frame
disk output, music remains ON. Existing save pauses audio only for diagnostic
flush after play. This is a phase-cost locator, NOT new production FPS proof.
Timing includes IRQ/DMA/observer; nested scopes nonadditive, torn/edge pairs
rejected. It cannot directly separate pure CPU from DMA wait.

Native plain/minimal/sparse compile; separate mask-write audits pass. Actual
collector capacity/reset/clock rejection and FPS/save ownership tests pass.
74assets/56compiled references verified. User confirmed FS-UAE stopped;
ring A/B drawers plus all logs/.uaem archived byte-identically under
 dist/older-builds/20260922-ring-accepted (inventory in new build folder).
68release hashes match before. No emulator launched, commit,push or release.

Route for pending user run:25seconds second ferry half/upper route with
multiple enemies, jump/shoot/reverse; die once, repeat25seconds after checkpoint.
LMB press/release, image freezes, wait30seconds before reset/stop for larger
log. No need to replay finale. One diagnostic build, no new A/B pair.

Remaining analysis decisions: compare scope distribution on this actual
retained renderer, conditional on region/enemy count and reset. Prior12..13ms
Bob/6..7ms game estimates predate ring/bounds/water optimizations and are not
current facts. If game dominates, inspect collision/AI/pure queries (known
water-height check after span scan remains unchanged). If restore/draw dominates,
inspect exact word traffic and setup counts before batching. If ring sync still
dominates, inspect dirty generations. Only investigate cross-phase CPU/Blitter
overlap with immutable history/state after profiling: current game mutates
renderer-consumed state, initial old-target cleanup and canonical updates have
strict ordering. Starting one short blit before gameUpdate is not a complete
pipeline. No further optimization bundled in measurement build. If residual
benefit is small/risky, stop Drowned round and transfer proven layout to level1.


## 2026-09-22 — Ring A/B run1: promising cadence and user improvement

Complete matching fps_569f847b713efc560ccc_A/B logs preserved byte-for-byte
as testresults/Drowned-ring-{A,B}-020-run1.log with provenance sidecars.
Staged executable hashes match builds.json. Analysis:build/drowned-two-copy/
run1-analysis.json. User reports “volgens mij beter”; explicitly confirms B
image/music good including reverse scroll/checkpoint, finale NOT reached.

Approximate PAL50 publication cadence:
- Whole A3261/3637=44.83FPS; B2979/3206=46.46FPS.
- Ferry first A384/447=42.95; B389/415=46.87.
- Ferry last A500/645=38.76; B489/586=41.72 (+7.65% observed).
- Upper after reset A456/589=38.71; B230/275=41.82 (+8.03% observed).
  Two-field share29.17%->19.57%; sample durations11.78s versus5.50s.
- Resets A2/B1; no pauses. A has no upper-before-reset sample; B237/283.
- Whole zero-TOD intervals A0/B2; preserve, do not reinterpret as exact
  deadlines. Reset-pair long intervals recorded separately. Whole three-plus
  A4/B2, including resets; no three-plus in after-reset upper samples.

These manual workloads differ and B's after-reset sample is short. Evidence
supports a useful gain together with the user impression and transfer proof,
but is not a controlled deterministic8% speedup or stable50FPS. Music ON;
minimal observer cost remains unmeasured. Neither log has governor/station
samples. Keep candidate and active pair intact; final-region visual coverage
still required before treating the full-level renderer as accepted. Normal
three-copy default unchanged pending that coverage. No new A/B needed merely
to repeat subjective confirmation; next useful manual check is B's finale.

Memory caveat: bitmap payload reduction106496bytes is established by actual
allocation dimensions. Startup prepared_chip is A152832/B142264 (B10568lower),
so these runs do NOT demonstrate greater total free Chip RAM. Available-memory
snapshots include external OS/emulator state; exact cause not established.
Do not present the allocation saving as measured net free-memory gain.

User asks to carry successful structural work to level1/interlude and supports
larger evidence-driven changes. Source portability review: level1 shares
canonical inactive ring,512logical width,32-bit guarded fetch and window-origin
helper, so next candidate is a separately guarded generalized ring layout.
Audit actual water/collectible CPU paths and world-end clamps; host-test its
assets, scrolling/reversal/restore history and source masks before native A/B.
Current Drowned macro deliberately rejects STORMRAIL_PROOF, which campaign
builds include even during level1: simply enabling it globally is invalid.

Stormrail flight differs: prototypePrepareCompactTarget clears each target
once, then skips ordinary rolling/dynamic sync; restores use stormFlightBlank.
Two copies can save the same bitmap payload but much less steady-flight work.
Audit boarding/approach retirement, unbounded distance/wrap, vehicle/drone/
reward histories, blank restores, finale gate damage/cache and transitions.
Keep current FMODE/presentation; no assumption of Drowned-sized FPS gain.
Implement generalized flag only with distinct per-mode tests and an unchanged
reference binary; no global switch or release modification in this review.

No emulator launched; no commit/push/release. Alpha.8 remains official.


## 2026-09-22 — Two-copy ring implemented; manual A/B pending

Structural candidate `SPARKPAW_DROWNED_TWO_COPY_RING` is opt-in only. Two
complete512px foreground copies/base96 replace three/base512. All world-slot
writes rotate by96; actor/restores and Copper use the corresponding base.
Both inactive target allocations become1024x208x4:106496bytes (104KiB) less
Chip bitmap payload. Canonical world, fetch width/phase, animations, audio,
gameplay and post-publication rear update scheduling remain unchanged.
Default drowned-full still uses three copies, retained enemy bounds and water.

Host ASan/UBSan proof runs actual ring/column/patch/restore/masked-blit kernels
with modeled DMA:10048frames per layout, all4801camera positions in both
directions, wraps, reversals, teleports, reset, overlap, guards and inactive
ownership. Ring DMA words43606896->29071264 (exact one-third reduction);
restore21152832 and masked21157344 unchanged. This is synthetic workload
transfer evidence, NOT native time or FPS. Real140enemy poses/4096history
scenes pass on both layouts. Existing reset/column/patch/water-sync/rear/FPS
regressions pass; water-sync harness needed the new header include path.
No emulator/raster acceptance is implied by host models.

Native plain/cadence builds pass. A plain byte-identical to retained enemyB
plain39b6355bd3fa88cc5a6147290e21153b97bf1b680534f107878a090da2d8be3c.
B plain292196bytes,40bytes smaller. Eight non-renderer plain translation units
byte-identical; four renderer assembly variants have separate mask writes and
no old BLTALWM readback pattern. Actual assembly confirms base96/mask511,
two CPU stores, two1024x208x4 allocations and initial Copper offset8 vs60.
Evidence:build/drowned-two-copy/{builds.json,native-audit.json,host-ring-proof.txt}.

User confirmed FS-UAE stopped. Played enemy drawers, logs and .uaem archived
intact under dist/older-builds/20260922-enemy-bounds-run1; archive hashes saved.
Only active diagnostic pair:dist/Drowned-Ring-{A,B}-020-HD/Drowned-Test.
Both74assets/56compiled references checked, assets identical; all68alpha.8
release hashes unchanged. Minimal in-memory TOD cadence only, music ON,
no broad profiler or per-frame disk logging. Observer cost unmeasured.
No runtime ownership counters; cadence is approximate publication timing.

A: fps_569f847b713efc560ccc_A; SHA256 3e3e3a0f1cb203ce1f7ed7455ea7ceb0f300d61a7ed58f22fca9fbbbdfea7565

B: fps_569f847b713efc560ccc_B; SHA256 cb27d9e30664d5cc13ab49ce5cbfa024819ed41e31f015a0281823bc78a26556

Manual test pending: A then B, opening/reverse-scroll/background,20seconds
second ferry half/upper route, die once,20seconds same route after checkpoint;
inspect finale/weather station too, especially B. LMB press/release, frozen
image expected, wait15seconds then reset/stop. Save both logs; report image,
music and subjective hitches separately. Direct68020 per user scope; no030
acceptance claimed. Do not promote until functional/manual evidence reviewed.
One-third ring transfers is NOT one-third FPS; no50FPS claim. No emulator
launch, commit, push or release. Smaller water-contact CPU query untouched.


22 September 2026. Target: 68020, 2 MiB Chip, 8 MiB Fast, PAL50, no JIT.
User clarification: “zoom uit”, **keep music enabled**. Alpha.8 remains the
release. This is a development investigation, not a release or emulator run.

## Decision

Retain the rear-water and enemy-mask-bounds changes as reduced work, without
claiming a perceptible FPS improvement. Both are now enabled in `drowned-full`.
The retained plain executable is byte-identical to the plain counterpart of the played enemy-B
cadence build: `39b6355bd3fa88cc5a6147290e21153b97bf1b680534f107878a090da2d8be3c`.
No new drawer is staged; the played enemy A/B pair stays intact in dist.

Stop successive small A/B requests for now. The next meaningful renderer
experiment should reduce the physical ring's duplication. First complete the
pixel/ownership model, then implement one compile-guarded two-copy candidate.
If that is insufficient, investigate CPU/Blitter overlap with explicit render
snapshots. A complete renderer replacement is not yet justified by evidence.

## What the latest runs actually say

Both compact logs have verified build IDs, executable hashes and complete
footers. Raw evidence and provenance:
`testresults/Drowned-enemy-bounds-{A,B}-020-run1.log/.txt`.
Calculations: `build/drowned-enemy-bounds/run1-analysis.json`.

| Region | A full cells | B cropped rows | Limitation |
|---|---:|---:|---|
| Whole recorded run | 42.85 | 44.95 | Different workloads; not a speedup claim |
| Second ferry, including reset attribution | 35.07 | 36.08 | A465/B500 intervals; different reset histories |
| Upper route before first reset | 34.38 | no samples | Cannot compare |
| Upper route after reset | 35.06 | 35.78 | A216/B468 intervals, manually played |

Values are publications per second derived from raw PAL TOD fields, not exact
visible deadlines. After-reset upper two-field shares:42.59% versus39.74%;
no three-field intervals in those two upper-route samples. Whole runs have
3/4 three-plus intervals, maxima8fields. A has1reset; B2resets. B spends much
longer in the opening region. Neither run samples governors/station. User
reports little perceived improvement; no separate new image/music verdict.
The observed +0.71/s after reset is compatible with a small benefit, but the
runs do not establish causality or prove full-level regression freedom.

The older complete sparse discovery run is a different build/instrumentation:
`testresults/Drowned-busy-020-run2.log`. Its busy-region conditional medians
suggest ~6–7ms game/update and ~12–13ms Bob composition. Ring/dynamic sync
(~3.5–3.8ms), restores (~3ms), and enemy drawing (~1–1.5ms) are **nested**
inside those totals, not additional phases. Rear updates sometimes cost
~5ms in that older build; the retained water change changes part of this path.
These estimates include IRQ, DMA waiting and observer cost. Samples are small,
134 invalid scope pairs were excluded, and medians must not be added to derive
an exact frame budget. They nevertheless justify looking beyond enemy draws.

## Research: mechanisms and application here

Primary hardware references consulted online on22September:

- [Commodore Hardware Reference Manual, chapter6, printed pp187–193](https://oldcrap.org/wp-content/uploads/2023/04/amiga-all-hw-ref-manual.pdf):
  Blitter time depends on enabled channels and DMA contention; register setup
  is additional. Display/sprite/audio DMA take priority. BLTPRI changes CPU
  arbitration, not display priority. Fast memory can permit CPU work while the
  Blitter owns Chip accesses. The busy-bit read sequence is also documented.
- [Motorola MC68020/EC020 User's Manual, sections4 and5.2.2](https://www.nxp.com/docs/en/data-sheet/MC68020UM.pdf):
  64 longword instruction-cache entries, direct mapped; data is not cached.
  Alignment and external bus width affect accesses. Small instruction count
  or unrolling alone does not establish execution time.
- [Historical transcription of the AA chipset specification, FMODE/LisaModes](https://github.com/rkrajnc/minimig-mist/blob/master/doc/amiga/aga/AGA.guide):
  wider bitplane and sprite fetches have separate controls, alignment and
  scrolling consequences. Treat this transcription as architectural context,
  not a measured timing model or a substitute for our native display proof.

| Common mechanism | Current Sparkpaw evidence | Consequence |
|---|---|---|
| Repeated Chip transfers | Four foreground planes, three physical copies per inactive ring; twelve blits per simple column | Reduce actual duplication, not just C arithmetic |
| Masked Bob traffic and restores | Separate mask/data/destination channels, four planes, repeated restores; grows with on-screen work | Bounds optimization retained, but this is only part of busy-frame work |
| CPU and Blitter serialized | Native main runs game, Copper preparation, then synchronous blit helpers; each helper waits before changing registers | Asynchronous *publication* does not imply substantial CPU/DMA overlap |
| Raster deadline quantization | Renderer waits for a fresh PAL boundary and publishes only at lines0..4 | A small overrun can add a field; average FPS cannot reveal exact missing compute time |
| Instruction locality / spills | Real VBCC output has stack traffic, integer multiplies/divides and out-of-line helper calls | Optimize measured hot loops; don't infer a cache-conflict cause from code size |
| CPU data in Chip | Collision/hazard tables in normal BSS; SFX sources/player masters explicitly Fast; DMA surfaces Chip | Much placement work already exists; normal hunks aren't proof of runtime Fast addresses |
| Audio IRQ load | Music player plus two-voice SFX mixer,112sample blocks at period322; byte loop only for two active voices | Measure IRQ contribution if needed; do not remove music or change sample rate as a shortcut |
| Full-screen restore / naive double buffering | Historical compact full-copy/recenter attempts regressed; current dirty ring avoids them | Do not replace sparse updates with a per-frame full viewport copy |

Already implemented: resident enemy frames and frame pointers, sprite staging
cache, canonical Bob restores (no two additional clean rings), dirty water
merging, precomputed pontoon offsets, tile-step collision/sweeps, hazard-column
cache, shortened scroll columns, DMA patch copies, hoisted Blitter setup,
bounded BLTPRI composition, asynchronous inactive foreground targets, and
post-publication rear scheduling. No hot floating-point operations found in
the eight audited translation units. No per-frame allocation identified in
the audited gameplay/composition chain.

Historical traps remain relevant:
`RENDERER_GLITCH_CORRECTION_PLAN.md` documents failed FMODE3 production rings,
HUD splits and sprite-slot conflicts. `PERFORMANCE_68020_STAGE2_AUDIT.md`
rejects CPU fetch-union pruning H3 (28.96vs28.64 in that old workload), longword
copy assumptions and unsuitable column batching. Those numbers are not current
Drowned performance. Their failure mechanisms must be addressed explicitly.

## Generated-code audit, tied to the binary

Evidence directory: `build/drowned-system-audit/`.
`retained-proof.json` proves current default flags reproduce enemy-B plain.
`map-proof.json` and `verified-link.map` independently rebuild all objects and
link to the same SHA256. `map_build.py` records the reproduction procedure.
The first attempted compiler-driver map capture (`link.map`) contains only
warnings and is **not** a map; use `verified-link.map` only.

Offsets below are within the linked code hunk, not measured load addresses:

| Entry | Offset |
|---|---:|
| rendererPublishGameplay | 0x096a0 |
| rendererDrawGameplayBobs | 0x0972c |
| collisionSolidAt | 0x18a00 |
| enemiesUpdate | 0x1a4d4 |
| gameUpdate | 0x1b5d0 |
| platformWaitBlit | 0x1c174 |
| level1AudioIRQ | 0x41b00 |
| mixRender | 0x42570 |

Observed in generated `build/drowned-enemy-bounds/B-*.s`:

- Main explicitly calls gameUpdate, rendererUpdateGameplay and
  rendererDrawGameplayBobs in sequence, then polls the boundary. There is no
  legacy line253 pre-composition wait in this rolling build. Removing that
  wait “again” would achieve nothing.
- Composition enables BLTPRI, calls restore/water/ring/draw helpers, retires
  the final blit, disables priority, saves history, then marks Copper ready.
  The renderer has26 static WaitBlit call sites; loops execute them repeatedly.
  This count is not a runtime wait count or a time measurement.
- Collision uses shifts for tile division, but still multiplies row index by320
  and calls the dynamic gate helper. Gate rejection is already early by X.
  Predecoded row pointers / spatial mechanism routing are CPU hypotheses,
  not an explanation for all missing FPS. Require call-count/profile evidence.
- Enemy bounds lookup uses shifts/adds and packed top/height; no runtime scan.
  Source plane stride remains original height. Eight non-renderer plain
  translation units match A/B exactly. Separate mask-register writes remain.
- Two-voice mixer loop emits nine instructions/sample, including two pointer
  copies and separate increments; one-voice/silence use inline copy/clear.
  A compact assembly loop is a plausible bounded CPU improvement, but its
  active duty cycle and savings are unmeasured. No change to audio made here.
- Additional native assembly was checked for player, projectiles, Drowned
  level queries, governor, HUD and assets. One concrete avoidable CPU cost:
  `levelPlayerTouchesWater` scans up to20water spans (up to40calls to
  levelWaterLeft) **before** testing bottom>=204. Generated code confirms
  this order. A height-first rejection is logically equivalent for this pure
  query and would avoid that scan on the upper route; a direct water lookup
  is another option. This is a bounded follow-up, not yet implemented or timed.
  It is distinct from the collision hazard cache, which already exists.
- Integer division remains in animation selection and cold paths. For example,
  Spillwing divides by5 for a state animation. The renderer's three /24 sites
  are fade work, not evidence of a continuous busy-scene division bottleneck.

Whole-unit instruction-site counts are recorded in `static-sites.json` only
for audit navigation. They must not be ranked as dynamic CPU costs. Hunk
flags and plentiful Fast free space do not establish actual code/stack/cache
placement. A future load-time TypeOfMem/cache-state probe can check that once,
without adding frame logging or changing cache configuration speculatively.

## Preferred structural experiment: rebased two-copy ring

Current: two alternating1536x208x4 display targets, each three copies of a
512px logical ring. Proposed: two alternating1024x208x4 targets, **two complete
copies**, with the physical origin rebased from512 to96px.

This is a new coordinate/layout contract, not permission to omit current
copy2. Canonical writes would use `(worldX+96)&511`; actor drawing uses
`96+(cameraX&511)+worldX-cameraX`. Every consumer must change consistently.
Keep current32-bit bitplane fetch,48byte guarded fetch, fine-scroll phase,
seven playfield planes, HUD split, sprite format, music and artwork.

`tools/analyze_drowned_ring_layout.py` compiles the existing C contract helpers
and exhaustively checks all4801legal camera positions. The model finds96px
the only tested32px-aligned base in0..512 meeting both constraints:

- Entire resident512px window maps into physical pixels0..1007, leaving16px.
- Guarded fetch intervals lie within64..959; world-pixel fetch phase exactly
  matches the existing ring after translation.
- Every resident16px canonical column maps to one of the two translated slots.

Proof: `build/drowned-system-audit/two-copy-feasibility.json`. This establishes
**geometry only**. It does not prove asynchronous lifecycle, Copper execution,
all restore/sprite/HUD consumers, DMA padding, or runtime speed.

Logical display allocation:319488 ->212992bytes across both targets, saving
106496bytes (104KiB) of Chip. Ring/dynamic copies process two copies instead
of three: one-third fewer copied words, not one-third faster rendering.
New wrap boundaries can change setup counts. Single canonical Bob restores
and Bob drawing are not directly reduced. The old ~3.5..3.8ms sync scope gives
only a rough scale: even a one-third reduction of that *whole* scope would be
~1.2ms, not a promise, and much of that scope may not scale with transfer words.

This differs from rejected H3: no per-word fetch-reach checks and no partially
valid copies. It also differs from full viewport redraw/recentering: the
logical ring remains512px and continues scrolling by new columns.

### Gate1 — complete offline ownership/pixel model

Inventory all literal three-copy,512-base and1536-stride dependencies in
renderer, patch/column/reset helpers, initial copies, target allocation,
scroll/Copper setup, actor and pontoon history, projectile/collectible
restores, and debug assumptions. Preserve the current reference branch.

Build actual-C pixel parity tests with distinct world-column patterns and
asynchronous buffer alternation: both scroll directions; every511/512 wrap;
all world edges; arbitrary small camera deltas; checkpoint teleport; death
and resident replay; overlapping actors; per-target water/patch generations;
all16shift phases;96px pontoon; full finale/station. Poison guards and retire
pending DMA before CPU writes. Validate exact source/destination ranges and
that active target/Copper/rear ownership never changes early.

Exit: all reference-visible pixels and gameplay-state traces equal; no stale
or out-of-range reads; measured logical transfer counts lower. A geometry-only
pass is insufficient to proceed to a user build.

### Gate2 — native candidate and focused manual A/B

Compile opt-in Drowned-only layout; ordinary retained baseline stays available.
Audit emitted address arithmetic, memory hunks and link placement. Same music,
assets, gameplay, fixed publication and minimal diagnostic in both variants.
Optionally count issued blits/copy words in one separate discovery build;
never add per-blit file writes or repurpose the music CIA timers.

One manual pair only after Gate1/native checks: opening/background, second
ferry/upper route, one checkpoint death and repeat, then B-only finale/station
visual coverage. Compare matched busy-region windows and multi-field shares,
not whole-run averages. Suggested practical success criterion: repeatable
~3publications/s busy gain or a clear reduction in two-field frequency with
user-visible improvement, no visual/audio/gameplay regression. This is a
selection threshold, not a forecast. A memory saving alone is useful but
must not be presented as solving the hitches.

### Gate3 — if still insufficient, overlap CPU and DMA

Before a broad rewrite, measure ready-time distribution relative to publication
and distinguish game CPU, render setup and DMA waits. Use a bounded sparse
sample in Fast RAM (field+raster with torn-snapshot rejection), after all
composition DMA, plus region and logical work counts. Current publication-only
logs cannot say how many milliseconds each late frame missed by. Record rear
work separately because it occurs after publication and delays the next frame.
Do not sum nested scopes, equate WaitBlit with pure DMA, or sum phase medians.

Prototype immutable render snapshots and per-target restoration descriptors.
Then examine starting old-target cleanup while the CPU performs independent
Fast-memory update work. Current mutable Enemy drawn fields/history and
canonical patch generation must be separated first. A simple “start blit then
run gameUpdate” overlaps only one short blit and is not the desired pipeline.
A command queue requires measured dispatch costs: a Blitter IRQ per tiny plane
may be worse than polling; a Copper-driven queue adds Chip traffic and must
not disturb display-list ownership. Longword/interleaved-plane batching is a
separate layout experiment, not automatically compatible with current planes.

Exit: snapshot correctness, no input/gameplay timing change, same per-target
history and source lifetimes, demonstrably more useful CPU/DMA overlap under
matched minimal cadence. No assumed50FPS or automatic new fixed timestep.

### Other options, ranked below this sequence

- **Interleaved plane storage/batching:** fewer setup/wait boundaries, same
  copied data; requires both source/destination stride design and changes to
  masked-frame layout. More invasive than the two-copy contract.
- **Hardware-sprite enemies:** potentially removes Bob draw+restore, but scarce
  channels, same-scanline overlaps, palette mapping and foreground occlusion
  require a safe fallback. No reduced enemies or palette downgrade.
- **64-bit playfield fetch:** potentially frees DMA slots, but reopen the exact
  failed production-origin, guards, HUD and sprite-slot issues as a standalone
  display proof. Existing FMODE=0x000d is32-bit playfields plus64px sprites;
  changing it to0x0003 would also change sprite fetch and is not a valid toggle.
- **CPU micro-assembly/precompute:** prioritize a measured hot loop, especially
  mixer or repeated collision queries, after dynamic counts. No blanket rewrite
  or slower simulation/AI tick rate. Keep audio fidelity and logical behavior.

## Perspective and stopping rule

There is credible room for less duplicated work and better scheduling. There
is no evidence yet that a drastic rewrite is necessary, or that it will yield
stable50FPS across this complete level. Retained small reductions did not
resolve the busiest area. Advance one structural hypothesis, reject it if
complexity outweighs measured gain, and report the remaining deadline budget
honestly. No further small user test is requested by this research turn.

## 2026-09-23 — Level1 two-copy candidate staged; opt-in only

User asked to continue after the bounded Drowned analysis. Level1 transfer now
implemented as SPARKPAW_LEVEL1_TWO_COPY_RING, effective ONLY inside the existing
renderer_level1_unit.c isolation. Shared ring header derives internal
SPARKPAW_RING_TWO_COPY; Drowned uses same retained layout. Stormrail unit keeps
three copies/base512. No default campaign/release flag changed.

Important Level1-specific finding:3392px world-end clamp differs from5120px
Drowned. At camera3072, resident origin2880 maps to physical-96 with base96.
Visible fetch remains safe, but full resident-window membership alone is not
proof of valid Bob destination. Candidate prototypeRectFits therefore checks
signed physical>=0 and physical+width<=1024 before existing residency check.
Exhaustive host tests show rejected rectangles up to actual max64px are outside
visible320px view plus16px margin. Histories store only prior successful draws;
restore addresses stay target-local. This guard also covers splash drawing,
which has no separate screen-cull predicate. Source/generated assembly reviewed.
Do not remove this guard or claim Drowned's full-resident geometry proof applies
unchanged to Level1. Guard has CPU cost; net FPS gain still unmeasured.

Actual C host ASan/UBSan: real storm-front3392x208,6592frames/layout, all3073camera
positions forward/back, wrap, teleports, CPU initial/column/dynamic rectangles,
clipping, mutable canonical strips,16/24/32/64px overlapping Bobs, restore
history, active-target immutability and memory guards, exact AGA fetch phase.
DMA is modeled, no native raster proof. First naive all-resident actor test
failed at world end; explicit guard and offscreen-rejection proof resolve it.
Reference/candidate transfer counts differ slightly because synthetic invisible
Bobs are now rejected. No world/gameplay/enemy removal is performed.

Drowned actual kernel regression10048frames/layout still passes unchanged.
TOD counter tests now exercise both Drowned/Level1 regions; save/music ownership,
reset/wrap/pause behavior and campaign Level1 asset/isolation tests pass.
Shared header included in Make dependencies; level1-two-copy build/test targets.

Four native builds compile. A plain matches captured current campaign baseline
(not a claim of alpha.8 byte parity). B plain adds44bytes for layout/bounds.
Seven non-Level1 plain units including Stormrail, dispatcher, main, game and
audio are byte-identical A/B. Stormrail cadence unit also identical. Native
allocation calls verify two1024x208x4 bitmaps versus1536:106496byte payload
saving. CPU column/rectangle loops store two destinations instead of three;
this is not a measured whole-frame FPS gain. Retained ordinary Drowned rebuild
still b78af227b85ffa7faa1e1ed8af637b98e6b01e74da6b3ef6f6ce4762f03fd738.

Diagnostic reuse: existing music-safe read-only TOD counter, with Level1 X bins
512,1024,1536,2048,2560,3072,3328 and unambiguous Level1 log header/variant/ID.
No CIA profiler. Equal seeded direct-start paths; completion held for LMB log,
so these diagnostics never transition to Stormrail. Plain controls retain the
normal campaign. Title/ready, normal completion/transition and hardware remain
separate acceptance boundaries. Observer overhead unmeasured, no exact deadline
or runtime ownership counters. No new production cadence claim.

Active manual pair:dist/Level1-Ring-{A,B}-020-HD/Level1-Test; each59runtime assets
and59compiled references checked, asset sets identical.68alpha.8 release files
byte-identical. Host/native details:build/level1-two-copy/{builds.json,
source-hashes.json,native-audit.json,host-ring-proof.txt,drowned-regression.txt}.

A-plain: plain; 305692bytes; SHA256 e80b9c41de04bca7a519b9afaca05fdf9297684f70c6b98454e97c145795c470

A-cadence: l1ring_49e244338f68fb793341_A; 306788bytes; SHA256 24e1221a28a872b9f858520a4d55bea313209a2f5ad6390efe34b43a2d13668a

B-plain: plain; 305736bytes; SHA256 0175c25a8534422c922506415756e54871bcc1010399a495506c02ef08e568c4

B-cadence: l1ring_49e244338f68fb793341_B; 306828bytes; SHA256 53779275a92815e2b9a3fcc8cb7f51ced7e04cc2b61e4421e6bb46262a1aa6b7

Pending user pass: same68020/2MBChip+8MBFast/PAL50/noJIT scope; A then B,
60..90seconds comparable water/enemy/shot workload with reversal and death.
Especially B right boundary/Core, reverse movement near end. LMB press/release,
frozen image expected, wait15seconds then reset/stop. Keep both logs and report
image/music plus subjective cadence. No030gate claimed; user explicitly keeps
this020 investigation. Candidate not promoted before manual evidence.

Temporary housekeeping exception: user has not answered whether FS-UAE stopped
after Busy2. Its completed log is preserved, but do not move its potentially
mounted drawer. Superseded Drowned-Busy2-020-HD remains intact in dist for now;
only new Level1 pair is active. Archive Busy2 plus .uaem byte-identically after
stopped confirmation. pgrep unavailable in sandbox; no emulator launched.
No commit,push or release. Alpha.8 remains official. Interlude optimization is
deferred at user's request, not inferred from Drowned performance evidence.
