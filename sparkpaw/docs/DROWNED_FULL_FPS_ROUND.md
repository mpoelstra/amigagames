# Full Drowned 68020 FPS round — 22 September 2026

Scope: full 5120px level, PAL50, 68020, 2 MB Chip + 8 MB Fast, no JIT.
User accepts the corrected rear animation visually. Alpha.8 remains the
official release. No emulator launch, release, commit or push in this round.
Existing local work is preserved. Runtime performance acceptance is pending.

## Bound baseline and evidence

The starting `Drowned-Level-020-HD/Drowned-Test` is 291316 bytes, SHA256
`92bf65300ffba69ee894adc60e72ec557c63232f7c8382b632b1bec4e261825d`.
The newly compiled **A-plain is byte-identical** to this executable. This is a
stronger reference binding than a target name or an inherited diagnostic header.

There was no renderdiag.log in the current drawer. There IS an append-only
startupdiag.log, despite renderdiag being disabled. It grew from 742 to 2226
bytes while the user was running the existing build. Its initial bytes were
recovered as an exact prefix matching the initial SHA256, and both snapshots
are preserved. Six 5120px startup records report prepared free Chip between
152544 and 153840 bytes, but contain no build hash; these are not authenticated
current-build peak/headroom measurements. New A/B logs bind their source
generation and record prepared free/largest Chip and Fast blocks themselves.

After the user confirmed FS-UAE stopped, the complete old drawer and associated
launch metadata were moved byte-exactly to
`dist/older-builds/20260922-fps-baseline/`. All 68 alpha.8 files are unchanged.
The initial inventory covers 385 historical cadence/Drowned logs; the two
relevant raw logs and sidecars also have independent preserved copies.

| Older evidence | Verified archived executable SHA256 | What it establishes |
|---|---|---|
| Unassigned-Drowned-buffer-split-run1.log | 14f38ed40e7abd78719870158a6b6da414696190361c3de6d07841d49ddb00db | 3520px, pre-current-music/ambience; sampled dynamic sync avg3.23ms, p954.26ms, roll avg0.20ms, initial wait0.025ms. Includes DMA waits; scopes are different frames, not additive. |
| Unassigned-Drowned-patch-hoist-run1.log | 7fbc9badfd46850baa00d9efb8dfe3d6a98f5c740b6847cd1b68ca093f00d8e2 | Old minimal cadence: 4527 intervals, 30 two-field, no three-plus, 49.67 overall; late ferry48.84 over only297 intervals. Unequal manual workload, no controlled gain or current-level FPS proof. |

Full paths, raw hashes and package manifests are in
`build/drowned-fps-round/{historical-evidence,baseline-drawer,stage-proof}.json`.
The README's alpha.7 release heading and older performance-parked statements
are stale for this task; they do not override the user's alpha.8/task scope.

## Broad audit and priorities

Actual full-build `-O2 -cpu=68020` output was inspected for renderer, game,
enemies, collision, Spillwing, main, platform, mixer and music adapter. A/B
outputs of all these units except renderer are byte-identical in plain mode.
No instruction-cache placement claim follows: linking the changed renderer
can still shift later code. No address-based cache optimization is included.

| Area | Existing work retained / finding | Decision |
|---|---|---|
| Foreground rendering/DMA | Canonical restores, independent inactive-target histories, three physical ring copies, merged dirty water strips, six-tile canonical water batch, setup/row-offset hoisting. | Old sync profile remains the best larger-cost lead. Removing a ring copy or ownership wait lacks a fetch/pixel proof; not attempted. |
| Scrolling | Generated minimum of entering/evicted column bounds already skips blank rows. Large origin jumps still call full initialization independently for both targets. | Separate reset plus following interval in the new counters. A checkpoint hitch is not automatically ordinary late-ferry load. |
| CPU/AI | Four slots, copy-on-unload, traversal indexing and flight-clearance lookup already exist. Full-level Spillwing assembly retains lookup branches before finale, with collision-scan fallback near/inside finale. | Do not repeat clearance precompute as a new ferry optimization. Respawn loop still calls levelEnemyPatrolSurface for all19 spawns before checking respawn flags; a separate low-risk candidate if normal CPU overhead remains relevant. |
| Collision/projectiles | Hazard-column cache, tile probes, spatial gate rejection and projectile sweeps already present. Full finale delegates to governorSolid. | No physics/sweep simplification. No evidence that enemy count reduction is needed. |
| Sprite staging/vegetation | Fast player masters, two inactive Chip stages, facing/frame cache; clipping intentionally refreshes the master on overlap and after leaving it. Sorted/bucketed static silhouettes already cull distant plants. | Position-aware mask caching would need more cache identity and pixel proof. No blind skip of old actor restores or plant clipping. |
| Audio | Three-channel music +112-byte block SFX mixer; explicit Fast score/effect data and Chip bank/output. CIA-B is owned by music; old diagnostic combination is compile-rejected. | Preserve IRQ priorities, mixing, samples and music. Use a separate read-only field observer, never re-enable old renderdiag beside music. |
| Memory | Resident Walker/frame caches and water batching trade Chip space for less staging/setup. Rear ambience uses205828 Fast bytes,1848 Chip stage and512 additional Copper bytes. | Candidate adds no allocation. No extra Chip cache with the observed narrow startup margin. Normal executable hunks have default allocation flags, so static BSS is not asserted to be explicitly MEMF_FAST. |
| Rear animation | After successful publication only; line>64 defers; one water update OR one fall; fixed private stage; all six blits retire before reuse. Water phase advances every6 simulation ticks, with extra uploads on16px rear-window changes. | Found repeated row-address multiplication in actual native code. Select this bounded CPU-only change for the first A/B. |

The rear water upload is1848 staged bytes at full width. It still performs42
CopyMem calls and six blits copying **3696 destination bytes total**
(1848 to canonical rear and1848 to guarded display). The change does not reduce
these transfers or display/audio DMA bandwidth. Foreground-water transfers and
Bob restores remain separate, potentially much larger costs.

## Candidate B: advancing staging pointers

Only the strided Fast-to-private-Chip row addressing changes. Flatten the
three consecutive planes into42 rows and advance source by sourceStride and
destination by bytes. Preserve contiguous waterfall CopyMem, width/height,
all source data, masks, waits, copy count, scheduling, palette and phases.
`SPARKPAW_DROWNED_REAR_STAGE_REFERENCE` restores the exact old implementation.

Native code proves84 repeated row multiplications disappear from the water
loop, replaced by pointer additions. B-plain is291256 bytes,60 fewer than A;
its BSS and all explicit allocations are unchanged. No native time saved or
FPS gain has been measured. The expected whole-frame gain is modest because
this work is intermittent and DMA traffic remains identical.

## Music-compatible measurement and limits

`SPARKPAW_DROWNED_FPS` is separate from `SPARKPAW_RENDER_DIAGNOSTIC`.
It samples the existing read-only CIA-A TOD counter once immediately after
COPJMP1, before target bookkeeping and rear animation. It never changes CIA-B,
interrupt configuration during play, or the ordinary DMA/publication policy.
Counters and source-generation ID are written once after the normal safe
debug quiescence; LMB/release then frozen image, wait15seconds, reset. No
Workbench return. No per-frame file I/O, scopes or new WaitBlit barriers.

Raw zero, one, two and three-plus deltas are preserved with24-bit wrap handling.
Eight regions attribute intervals to the preceding published player position.
Late-ferry upper/lower rows split before/after any simulation reset; reset and
next interval have their own aggregate. Pausing breaks the sample chain.
There are no ownership counters: do not report zero violations from this log.

Prior music research demonstrated zero/two TOD phase pairs. Earlier sampling
was after publication bookkeeping; this observer moves closer to COPJMP1 but
does NOT claim to solve every TOD phase/IRQ race. Raw two-field counts are not
exact missed-visible-deadline counts. Aggregate publications per elapsed PAL
fields and matched regional samples require native sanity checking. No p95
CPU time can be calculated from these coarse counters.

Both variants have identical observer code and a fixed initial seed0x53504157;
normal play uses a time-derived seed. The observer adds2956 executable bytes
and3040 bytes across loader allocation sizes, including448 additional BSS;
no new explicit Chip allocation. Time overhead remains unmeasured, and added
work before rear updates can affect their scheduling. Logger-free A/B controls
are preserved offline, not offered as extra active test drawers. If observer
results are ambiguous, use a later plain matched pair or narrow trace; do not
expand instrumentation automatically or turn raw counter anomalies into FPS.

## Verification and user test

- ASan/UBSan actual-C pixel oracle passes both reference/candidate, all24
  approved water phases, all four waterfalls/four phases,1824 camera/phase
  water rectangles, guards, Copper phase/cache and scheduler/lifecycle.
- Tests explicitly assert unchanged42/1 staging calls for water/falls and
  identical staged byte counts; six blits per rectangle remain required.
- Native mask audit passes; the old BLTALWM readback remains absent from both
  instrumented renderer outputs. Old CIA profiler symbols are absent from
  instrumented platform output; observer absent from plain outputs.
- Full5120 collision/art/occlusion, actual player stage/cache/blink/master
  checks, pause regression and audio lifetime/mixer parity pass.
- Actual field-counter tests cover phase pairs, wrap, boundaries, resets,
  previous-position attribution and pause exclusion. Actual save-flush body
  passes balanced music/nonmusic interrupt and OS ownership checks.
- Four native builds pass. Each staged variant covers74 identical runtime
  assets and56 embedded references. Build IDs and executable hashes are in
  its ReadMe and `build/drowned-fps-round/builds.json`.

Active pair: `dist/Drowned-FPS-A-020-HD/Drowned-Test` then
`dist/Drowned-FPS-B-020-HD/Drowned-Test`. Explicit user68020 scope supersedes
the test skill's usual030-first sequence. No emulator was started by Codex.

For each:10seconds stationary at opening; play to checkpoint; spend about20
seconds on second ferry/upper ledges with similar shooting/jumping; deliberate
water death; repeat upper route for about20seconds; continue through Governors
to station and inspect later falls. Save before final Core. Report perceived
hitches, sound and any visual anomaly separately. Preserve both logs before
another run. One unequal manual A/B does not establish repeatability.

Reject B for any pixel/audio/gameplay regression or repeatable cadence loss.
Keep performance status pending until supplied results. If the difference is
small/noisy, say so; do not trade away presentation for a nominal50FPS claim.

## Supplied rear-stage run1 and next reset experiment

Both source-generation IDs, executable hashes, all staged manifest files and
complete footers verified. Regional sums match global counters. Raw copies and
sidecars are `testresults/Drowned-full-fps-stage-{A,B}-020-run1.log/.txt`;
`build/drowned-fps-round/run1-analysis.json` retains calculations. User says:
"Geen duidelijk verschil; beeld en muziek goed". Image/audio acceptance is
positive for both runs; no perceived performance gain.

| Aggregate publications per second from raw TOD | A | B |
|---|---:|---:|
| Whole run | 43.99 | 45.01 |
| First ferry | 42.52 | 43.06 |
| Last ferry | 36.40 | 36.52 |
| Last ferry upper, before reset | 33.79 | 36.26 |
| Last ferry upper, after reset | 37.17 | 36.53 |

A has5401intervals/6139fields, B6011/6677. Late-ferry sample sizes are814/802,
whereas total route distribution differs, including longer quiet station time
in B. Even excluding station and reset samples, aggregate43.68/44.68 remains
an unmatched-workload observation. Do not assign the whole-run difference to
the pointer optimization. After-reset upper work does not improve consistently.
Prepared Chip free is153536 in both; largest152248/152560. These are prepared
snapshots, not peak guarantees. Static-Fast allocation placement is not traced.

Both runs have two simulation resets and four long intervals: reset_and_next
contains4intervals/32fields/max8, therefore all four are exactly8raw TODfields
(nominal160ms each). All three-plus observations are in that bucket; zero
remain3/2globally. This separates reset-associated gaps from ordinary upper
route load, but is not a function timer or exact visible-transition trace.

The user authorizes continuation. Source confirms that large camera jumps
invoke prototypeCopyInitial on the alternating targets, whose general span
still performs CPU word copies. The existing canonical patch copier already
supports full-height, full-ring A-to-D DMA. New opt-in
`SPARKPAW_DROWNED_RESET_BLIT` changes only that runtime >=512px-jump branch.
Startup and ordinary small column movement retain their old paths. This is a
source-supported explanation and bounded candidate, not a proven root-cause
fix from the aggregate log alone.

`drowned_reset_copy.h` uses the existing copier, then duplicates the original
initialization's water/collectible history exactly. Actual-function parity tests
protect this shared contract against drift. Each target still receives159744
bytes, in12blits for a ring-aligned origin or24when split at the ring seam.
Unlike the CPU's single source read followed by three destination writes, DMA
reads the source separately percopy:159744rather than53248sourcebytes, an
additional106496Chip-read bytes. CPU instruction work decreases, total bus
traffic increases; the native comparison must decide the tradeoff.

Test `test_drowned_reset_copy.py` executes the actual roll dispatch, old CPU
initializer and new copier against an independent pixel oracle for1734cases:
289origins x2targets x3history phases. All208rows/fourplanes/threecopies,
untouched other buffer/guards, final DMA completion and metadata agree.
Existing1788column tests and counter/safe-flush checks also pass. Native
reference plain exactly matches previous B-plain SHA2568ba2327ef2766ee743e21baba1dda5292285d5030bef6871698e6af373dca30d.
New B-plain is291388bytes (+132), with unchanged BSS/explicit allocations.
Actual assembly shows runtime dispatch into the bounded copier and unchanged
startup function; other inspected plain translation units match A/B. CIA-B
profiler remains absent, mask writes safe. No native speed/visual claim yet.

New active pair: `Drowned-Reset-A-020-HD` / `Drowned-Reset-B-020-HD`, launch
`Drowned-Test`. Both retain the prior rear pointer change and same lightweight
observer/seed, artwork, music and enemies. Each has74verified assets/56refs;
alpha.8's68files remain identical. Played rear-stage pair/logs archived at
`dist/older-builds/20260922-fps-stage-run1/`. New build/stage/native proofs are
in `build/drowned-fps-reset/`; build with `make drowned-fps-reset` after any
required full asset generation. Ordinary `drowned-full` does not enable the
reset experiment.

Short test A then B on020: activate checkpoint, reach the last ferry upper
ledges near the far bank, spend15seconds jumping/shooting, deliberately drop
into water there, compare the respawn pause, then play10seconds after return.
Inspect water, diamonds, enemies and scroll strips for stale/missing pixels.
One repeat is optional if lives permit. No need to replay finale. Save once
with LMB/release, wait15seconds, reset; preserve logs before rerun. Reject for
visual/audio regressions or slower reset. Ordinary ferry throughput remains a
separate next priority; no further unmeasured changes are bundled here.

## 2026-09-22 — Reset result received; busy encounters take priority

User played reset A/B and reports little perceived difference. User explicitly
redirects attention to sustained drops when several enemies/actions are visible.
Treat that as the primary acceptance target, not the isolated reset pause.
Raw logs are preserved in testresults/Drowned-full-fps-reset-{A,B}-020-run1.log
with provenance sidecars; build IDs, executable hashes, complete footers and
regional sums verified. Calculations: build/drowned-fps-reset/run1-analysis.json.
One reset per run: two reset-associated intervals total16fields in A versus10
in B (nominal320ms versus200ms). This is aggregate publication evidence, not
CPU function timing or a subjective acceptance. Late ferry A36.01/B34.90
publications/s; upper before reset34.56/33.42, after37.58/35.90. Unequal manual
workloads: neither a sustained win nor a proven regression can be attributed.
User did not give a separate image/audio verdict for this second pair.
Do not promote reset Blitter or spend the next user test on reset micro-tuning.

Current main.c already skips the line253 wait for rolling builds; asynchronous
composition is active. Existing resident enemy caches and frame-address tables
also remain active. Enemy draw/restore uses full cell heights over four planes.
Offline inspection of exact prepared SPBM masks found Spillwing32poses with
5..12 empty edge rows of24, mean7.5 (31.25% of draw rows); Walker64poses with
6..15 empty edge rows of64, mean7.75 (12.11%). Bounds recorded in
build/drowned-fps-reset/enemy-row-audit.json. This identifies potential redundant
DMA work proportional to enemies, NOT measured CPU/DMA time or FPS gain.
Keep original plane strides, ordering, logical collision cells and target-local
restore history in any trimmed-draw experiment; do not equate draw height with
source plane stride. Draw-only trimming can leave full restores unchanged.

Next investigation must separate game/AI/collision, player staging/occlusion,
enemy/projectile restore+draw and dynamic water/patch synchronization under
actual busy-scene load. Existing minimal logs lack actor-count/work attribution
and CPU scopes, so they cannot establish which family dominates. Any targeted
measurement must preserve music timer ownership and quantify/declare overhead;
no blanket old renderdiag re-enable. Prioritize a materially relevant measured
busy-scene candidate over another peripheral A/B. No new drawer staged in this
review, no runtime code changed. Alpha.8 remains official, no emulator/commit/
push/release. Existing reset pair and original logs remain intact in dist.


## 2026-09-22 — Busy-scene discovery build ready (not an optimization)

User requests continuation with sustained busy-scene FPS as the priority.
One active drawer: `sparkpaw/dist/Drowned-Busy-020-HD/Drowned-Test`.
Build ID `busy_d3d8e3e3d83e1d406994_sparse`, SHA256
`0b5ca6b88c481f4b7c3560328a392730fe0830739dd42da021a2e0b372364dfe`.
The reset Blitter flag is OFF. Enemy-row trimming is NOT implemented pending
actual busy-scene cost evidence; its theoretical saving is not a measured win.

New compile-only diagnostic `SPARKPAW_DROWNED_BUSY` takes read-only CIA-A TOD
and raster checkpoints every 31 frames (prime stride avoids phase-locking to
2/4/6/16-tick animations). Up to 16 samples per region, with a separate late-
ferry-after-reset quota; 144 samples / 28,512 explicit MEMF_FAST bytes total.
No additional Chip allocation. Actual submitted enemy draws by family,
projectile draws, generic masked-Bob and restore word cells are recorded on
sampled frames only. Counts do not cover every dynamic DMA transfer.

Checkpoints cover game/AI, sprite+HUD+Copper, restores, water/patch generation,
ring/dynamic synchronization, enemy/projectile draws, final wait/history and
rear update. No extra WaitBlit, timer ownership/configuration, audio callback
or publication-policy changes. Intervals include IRQ and deferred DMA waiting;
a first wait may retire preceding work, so do not label scopes pure CPU or
add nested AI/game or enemy-restore/all-restores totals. An empty consecutive
checkpoint is a timestamp-overhead probe, not a total observer-cost measure.
Unsampled frames still execute diagnostic guards and sample selection.
Time overhead remains unmeasured; discovery FPS is not production FPS.

`tools/analyze_drowned_busy.py` retains raw zero/two TOD behavior and rejects
incomplete saves, invalid/torn stamps and negative/excessive intervals. Same-
TOD nondecreasing beam intervals need no wrap inference. Cross-TOD estimates
assume PAL312 and aligned field epochs away from raster8..300 edges; retain
that assumption until the supplied raw trace is checked. No exact-deadline
or ownership-violation claim is possible. At most 16 regional samples support
coarse prioritization, not strong percentile or causal FPS claims.

Actual collector ASan/UBSan tests cover allocation failure, sparse gating,
all region/reset quotas and full capacity, torn/edge clock handling, disabled
counts, 24-bit wrap and analyzer attribution/reset exclusion. Existing cadence
and safe audio/OS flush tests pass. Native plain/minimal/sparse builds pass;
plain SHA256 remains byte-identical to the played pre-reset pointer baseline.
Sparse executable297064bytes, +2844versus minimal; static BSS +52bytes.
Native masks remain separate immediate writes; diagnostic calls absent from
plain/minimal hot paths. Proofs and offline controls: build/drowned-busy/.
Rebuild with `make drowned-busy` using prepared current full assets.

Staged with the standard HD stager:74assets,56compiledreferences,76files.
Both played reset drawers/logs/launch metadata are archived intact under
`dist/older-builds/20260922-fps-reset-run1/`. All68alpha.8files unchanged.
Manual020 route:10seconds quiet opening, checkpoint,20seconds busy second
ferry/upper route with normal jumping/shooting, one death, repeat20seconds
after checkpoint. No finale replay needed. LMB press/release, wait30seconds
on frozen image, stop/reset; keep the drawer's renderdiag.log. Only one run,
not A/B. No emulator launched, release, commit or push. Await this discovery
trace before selecting the next sustained-throughput optimization.

## 2026-09-22 — Busy discovery run1: incomplete trace, awaiting stop state

User reports played/log present. Executable SHA256 and source-generation ID
match staged busy_d3d8e3e3d83e1d406994_sparse. Initial read is61544bytes,
ends mid busy_stamp sample53; header advertises82samples and no post_run footer
is present. Same size on recheck. Raw preserved byte-exactly at
sparkpaw/testresults/Drowned-busy-020-run1-partial.log with provenance TXT.
Do not overwrite this initial snapshot if the source later finishes flushing.
Asked whether FS-UAE remains on the frozen save screen or was stopped/reset;
answer pending. Do not request a replay before resolving save completion.

Complete records0..52 only were analyzed separately, with explicit partial
status in build/drowned-busy/run1-partial-analysis.json. No footer was fabricated
and the strict complete-log analyzer correctly rejects the original file.
Header cadence aggregates cover3164intervals/3802fields; lateferry571/829,
but the detailed post-respawn trace is missing. No visual/audio verdict given.

Pre-respawn lateferry has8complete records (1..3drawn Spillwings,0..5shots).
Conditional scanline estimates: game median106.5lines/8valid, AI30/8,
ring+dynamic sync50.5/8, allrestores51.5/8, otherdraw38/8,
enemydraw22/6, Bobtotal204/6. These are nested/differently-valid scopes,
not additive medians or pureCPU/DMA timings. Same-frame emptyprobe is1line.
Rear updates usually2lines but maxima77..80lines across regions. Provisional
lead is combined scene workload/dynamic sync plus intermittent rear cost,
not enemy drawing alone. Trace endpoints near PALwrap are rejected where
TOD/raster epochs are ambiguous (e.g.sample44); do not silently repair them.
No code optimization selected, new build staged, emulator launched or release
changed during this evidence review. Preserve raw timestamps and await status.

## 2026-09-22 — Busy run2 complete; sustained scene cost priorities

User replayed and left the save screen frozen. Verified83/83samples and
post_run=complete; log93263bytes, executable/source ID match staged discovery.
Preserved raw as testresults/Drowned-busy-020-run2.log plus provenance TXT;
strict analyzer output: build/drowned-busy/run2-analysis.json. Original partial
run1 retained. User told FS-UAE may now stop/reset. No new image/audio verdict.
Regional/global cadence sums and raw distribution sums verified. Discovery
aggregate2964intervals/3540fields=41.86publications/s; lateferry478/708=33.76,
upper before reset222/336=33.04, after225/333=33.78. One reset,no pause. These
are observer-contaminated discovery numbers, not ordinary music-build FPS.

Late-ferry before/after groups contain9/7samples. Conditional PAL312 scanline
medians (IRQ/deferred DMA/observer included, nested scopes NOT additive):

| Scope | Before reset | After reset |
|---|---:|---:|
| Game incl AI |98lines,n9|105lines,n7|
| AI nested within game |29lines,n9|30lines,n7|
| Sprite/HUD/Copper |10lines,n9|10lines,n7|
| All Bob work |204lines,n7|190.5lines,n4|
| Ring+dynamic sync nested in Bob |55lines,n9|59.5lines,n6|
| All restores nested in Bob |50lines,n9|44.5lines,n6|
| Enemy draw nested in Bob |23lines,n6|17.5lines,n6|

At nominal64us/line this is about12..13ms Bob work,6..7ms game,
3.5..3.8ms ring/dynamic sync and1.1..1.5ms enemy draw. Do not sum medians
or interpret these as pure CPU timings.134invalid scope pairs were excluded;
near-edge TOD/raster disagreements remain visible in the raw evidence.
Empty timestamp probe median1line confirms nonzero instrumentation cost,
not total overhead. Some rear updates reach77..79lines (~5ms), versus2lines
for the common light path. Instrumented lateframe budgets are close enough to
a PAL boundary for this combined load to matter. This is a scheduling/workload
hypothesis, not proof that any single family explains every missed interval.
Finale-approach group5 has heavier enemies (enemy draw median55.5lines,n8),
so enemy cost must not be generalized from ferry Spillwings to all encounters.

Priority now: ring/dynamic-water transfer workload and interaction with rear
publication work, while retaining all ring copies and single-buffer rear DMA
contracts. Enemy-row trimming is secondary, not the presumed primary fix.
Source review confirms existing merged-water sync and three-copy patch copier;
do not rediscover those as new optimizations. Prepared rear phase row audit
(build/drowned-busy/rear-phase-row-audit.json) shows all14rows vary in planes0/1
and12in plane2, so a simple static-row skip there is not a large win either.
No optimization or new drawer staged in this review. Current discovery drawer
remains available; no further replay needed for this diagnosis. Alpha.8 and
all local work preserved; no emulator, commit, push or release.

## 2026-09-22 — Rear-water transfer A/B staged for busy-scene gain

User authorizes one meaningful optimization round after complete discovery.
Ring/dynamic foreground synchronization already merges dirty water and keeps
three required physical copies; no unsafe copy removal or animation reduction
was selected. Discovery also exposes intermittent rear work around77..79PAL
lines. Candidate targets redundant transfers in THAT path, not a claimed fix
for all ring-sync cost or a promise of50FPS.

Opt-in `SPARKPAW_DROWNED_REAR_DIRECT_WATER`: after successful publication and
the unchanged line<=64 gate, retire DMA, copy strided Fast water rows directly
into private rearWorld bitmap, then three canonical-to-rearDisplay blits.
Reference copies Fast->private stage then six stage->canonical/display blits.
Both destination rectangles, all pixels/phases, palette and schedule retained.
CPU never writes the fetched rearDisplay. Compile guard requires full Drowned
and the separate guarded rear display; both initial Copper pointers and scroll
patching use rearDisplay in this configuration. This narrowly changes which
private Chip buffer receives CPU water writes, not display ownership. Initial
and final DMA retirement retained. Waterfalls keep their one contiguous CPU
stage copy and six-blit path; their small source rows would make direct CPU
row copies an unfavorable change. Dedicated stage allocation retained for them.

Full water upload:42CPU CopyMem calls/1848bytes in both. Blits6->3; logical
Chip transfer bytes9240->5544 (40%less for this upload, NOT40%FPS). No new
allocations or BSS growth. Actual native plain B+420bytes; ordinary default
build does NOT enable candidate. A plain exactly equals played music baseline
SHA2568ba2327ef2766ee743e21baba1dda5292285d5030bef6871698e6af373dca30d.
Actual native other8translation units identical A/B, safe separate mask writes
verified in both plain/cadence renderers; busy profiler and old CIA profiler
absent. Existing minimal TOD cadence is identical apart from variant/build ID.

Host actual-C ASan/UBSan tests pass baseline, old staging reference and new
candidate:24water phases x76windows=1824rectangles, both destination bitmaps,
untouched pixels/guards, all4waterfalls/4phases, 1000scheduler camera steps,
Copper phases and lifecycle. CPU copies assert no pending DMA; DMA source
bounds and exact transfer count asserted. Existing cadence/safe music+OS save
checks pass. Native fourbuilds pass. This proves pixels/work reduction on host
and native instruction selection, not real runtime timing or visual acceptance.

ACTIVE pair: dist/Drowned-Water-A-020-HD and Drowned-Water-B-020-HD, each
Drowned-Test. IDs fps_8e768ab93ca563d8e77f_A/B. A-cadence SHA256
26ab9932ff321fbb9b8e6f0b211cee3425c206369709d2217513f8c46f62102b;
B-cadence13f7c990148ceac307792249ee0e309e48e79582cc6e36ae9f1cfe8e16a6faa9.
Both74identicalassets/56compiledreferences; all68alpha.8files unchanged.
Complete played discovery drawer/log/launch metadata archived intact under
`dist/older-builds/20260922-busy-discovery/`. Raw evidence also remains in
`testresults`. Proofs/controls:build/drowned-rear-direct/. Rebuild with
`make drowned-rear-direct` after any required full asset generation.

User020 A thenB: briefly inspect water while stationary/scrolling; checkpoint;
20seconds busy second-ferry/upper route jumping/shooting; one death; repeat
20seconds after checkpoint. Optional B-only continuation to inspect last
waterfall is visual-only, excluded from matched FPS comparison. LMBrelease,
wait15seconds (compact cadence logs again), reset. Report busy hitches and
image/music separately. Preserve logs before rerun. No automatic emulator,
commit,push or release. Candidate pending; reject for visual/audio errors or
no convincing benefit versus risk. Foreground ring-sync cost remains open.

## 2026-09-22 — Rear-water A/B: no demonstrated busy-route improvement

User played both and reports little perceived difference (uncertain), with
second ferry/upper route and Spillwings still heaviest. Both complete logs,
build IDs, executable hashes and regional sums verified. Raw preserved as
`testresults/Drowned-water-direct-{A,B}-020-run1.log` with provenance TXT;
calculations in `build/drowned-rear-direct/run1-analysis.json`.
Before-reset upper: A201intervals/294fields=34.18publications/s;
B224/326=34.36. No convincing gain. Whole-run A41.25/B44.02 is not causal
performance evidence: A records1reset and252after-reset upper intervals;
B records0resets and0after-reset upper intervals, with different region time.
Aafter-reset upper35.29 has no B counterpart. No new image/music verdict given.
Do not request another same-candidate run merely to chase this small difference.
Candidate stays opt-in/off in normal build and is NOT promoted as an FPS fix.
Logical transfer reduction remains proven; subjective/native benefit is not.

Reassess sustained foreground work, not another rear micro-optimization.
Historical audit reminder: PERFORMANCE_68020_STAGE2_AUDIT.md rejects H3
fetch-union pruning at28.96vs28.64FPS in matched old CPU-copy runs; scaled
indexing and branch/address costs erased reduced writes. Current Drowned uses
DMA transfers, so those numbers are not its current cost, but any renewed
copy-pruning idea must explicitly distinguish the new mechanism and prove
fetch/restore/guard invariants rather than silently relaxing the three-copy
contract or repeating H3. No such candidate implemented in this review.

Existing A/B drawers remain intact in dist; no new executable, emulator,
commit,push or release. Alpha.8 unchanged. Further progress needs a materially
larger foreground scheduling/transfer improvement, not a claim that fewer
transfer bytes already solved busy gameplay.

## 2026-09-22 — Water retained; enemy mask bounds A/B pending

User explicitly requested retaining the rear-water optimization despite no
convincing measured/subjective FPS win. It is now enabled in `drowned-full`;
this supersedes the earlier opt-in/off decision below. No release changes.
Legacy experiment builders strip that default when reproducing old controls.

Next candidate `SPARKPAW_DROWNED_ENEMY_BOUNDS` is opt-in only, targeting
recurring enemy draw AND restore work. Load-time scan of resident masks stores
packed top/height for each pose and offsets existing frame pointers. Original
plane stride remains intact. A separate 2x4 target-local bounds history keeps
restores tied to the previously drawn pose, not current animation or slot type.
Logical drawnY, sorting, collision, culling, source assets, enemies and animation
cadence are unchanged. Walker restore unions require equal cropped heights;
otherwise individual restores. Transparent poses retain full-height fallback.
All foreground ring copies and post-publication rear/DMA contracts retained.

Real source pose mean row reduction: crab28.60%, Walker12.11%, Spillwing31.25%.
These are unweighted asset averages for drawing, NOT FPS gains or measured
restore totals. Union opportunities can change; added lookup/branch cost may
erase part of the saving. Discovery enemy draws were only ~1..1.5ms in the
busy ferry samples, so a large whole-frame gain is not promised. Ring/dynamic
sync and game/update remain substantial and are not solved by this candidate.

Actual C host ASan/UBSan:4096two-buffer scenes, real140poses,16pixel shifts,
overlap, slot/type reuse, despawn/culling and history clears vs full-cell pixel
oracle pass. Actual selection/restore/history and masked blit setup exercised;
DMA and canonical rectangle copy are host models, not a raster timing proof.
Existing frame-table, resident-Walker, rear water/falls, FPS/save tests pass.
Native plain/cadence builds pass. Eight non-renderer plain translation units
are byte-identical A/B. Native table indexing uses shifts/adds; mask scanning
is load-only. Separate immediate BLTAFWM/BLTALWM writes verified in all4outputs.
B adds400bytes ordinary/Fast BSS, no Chip allocation; plain executable+560B.
A plain exactly matches the played water-B plain SHA256
65279e6a6eab455955aeee9e25abae47559c22f442ce90449f1af0616f7b7332.

ACTIVE: `dist/Drowned-Enemy-A-020-HD/Drowned-Test` and B sibling.
IDs fps_faa04f00df1c82503aa6_A/B. Cadence SHA256 A
5e4edf521f94ea290eceb7cc929ea883881497be98c7eca3c2f2f40f844d1cf7;
B86df68ad2bce6db301d00f8222f77877f72ff6ca18fc028b4d0edd645e3641a0.
Both retain water optimization/music and minimal TOD cadence (no busy profiler,
no reset experiment). Observer overhead unmeasured, no ownership counters.
User's explicit020-only task is the gate; no030 acceptance claimed.

After user confirmed FS-UAE stopped, old water pair/logs/.uaem archived intact
under `dist/older-builds/20260922-water-direct-run1`. Raw logs match preserved
run1 testresults copies. Staged only with stage_hd_test.py:74identicalassets,
56embedded references, all68alpha.8release files unchanged. Proofs/builds and
ReadMes under `build/drowned-enemy-bounds`; rebuild `make drowned-enemy-bounds`.

User route A thenB: inspect enemies/background; checkpoint;20seconds second
ferry/upper route jumping/shooting; die once; repeat20seconds after checkpoint.
LMBrelease once, frozen screen expected, wait15seconds before reset. Compare
regional logs plus subjective hitches and image/music. Candidate pending; no
FPS acceptance, automatic emulator, commit, push or release. Alpha.8 official.


## 2026-09-22 — System audit, retained bounds and structural renderer plan

User played enemy A/B, reports little perceived improvement, asks to retain
useful work reductions and research larger causes. Clarified “zoom uit” means
look at the whole system; music stays ON. No request to disable music.

Both complete logs preserved with hashes/provenance as
`testresults/Drowned-enemy-bounds-{A,B}-020-run1.log/.txt`. After-reset upper
A216/308=35.06publications/s; B468/654=35.78. Two-field shares42.59/39.74%.
Not causal proof: different manual workloads, A1/B2resets, B no pre-reset upper
samples. Whole42.85/44.95 is not a speedup claim. No governor/station samples
or separate new image/music verdict. Current enemy A/B drawers/logs left intact.

Enemy bounds now enabled in drowned-full alongside retained rear-water change.
Historical A/B builders strip defaults as needed to reproduce controls. Current
default native plain build is byte-identical to the plain counterpart of played
B:39b6355bd3fa88cc5a6147290e21153b97bf1b680534f107878a090da2d8be3c.
Separate all-object rebuild/link map verifies the same hash. No frame code,
assets, audio settings or presentation changed beyond enabling the played flag.

New active plan: `sparkpaw/docs/DROWNED_RENDERER_PERFORMANCE_PLAN.md`.
Research uses Commodore Hardware Reference Manual, Motorola020manual and
historical AA specification transcription. Audit covers actual native main,
renderer, game, enemies, collision, Spillwing, music/SFX mixer plus player,
projectiles, Drowned queries, governor, HUD and asset code. Evidence/maps under
`build/drowned-system-audit/`; use verified-link.map, not initial link.map.
No cache-conflict or exact CPU-time claim from static code/map offsets.

Preferred structural hypothesis: two complete physical512px ring copies with
base96 instead of three copies/base512, retaining current AGA fetch and art.
New offline `tools/analyze_drowned_ring_layout.py` checks actual contract helpers
for all4801camera positions: resident window0..1007, fetch64..959 within1024,
identical world fetch phase. Geometric feasibility only, NOT runtime safety.
Potential104KiBChip saving and1/3fewer ring-copy words, not1/3FPS. Requires full
coordinate/restore/patch/Copper/history audit and pixel/ownership model before
compile-guarded native A/B. Current three-copy runtime contract unchanged.

Secondary plan: CPU/DMA overlap with immutable render snapshots and measured
queue/dispatch overhead; interleaving, sprite enemies and64-bit fetch ranked
lower. OldFMODE3/H3/full-copy failures explicitly retained as constraints.
Concrete smaller CPU finding: water-contact query scans20spans before height
rejection even on upper route; native output confirms. Not changed/timed yet.

Do not ask for another micro A/B now. Next work: finish two-copy ownership/pixel
model, then one structural experiment if safe. If insufficient, measure ready
time/remaining deadline and CPU/IRQ/Blitter separation before broader rewrite.
No claim50FPS. No emulator launch, commit,push or release; alpha.8 unchanged.


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
