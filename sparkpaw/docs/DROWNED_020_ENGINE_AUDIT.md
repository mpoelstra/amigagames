# Drowned 020 engine audit — 14 September 2026

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

## 2026-09-16 — Patch DMA user run:49.88 FPS

User reports Blitter patch candidate feels slightly better. Played drawer
verified against proof-patchdma.json;68release hashes unchanged. Preserved
Unassigned-Drowned-patchdma-020-49-88fps.log/TXT under testresults. Session-stated
HD/FS-UAE68020/2MBChip/8MBFast; config not freshly inspected.
5060 intervals:5048 one-field,12 two-field,0 longer,49.88FPS,ownership0.
Two-field share0.24% versus3.85% in prior CPU run(48.14FPS). Chip free599176,
largest597800. No framebuffer allocation added; free-memory difference is not
attributed to new buffers. Workloads differ:98 shots/8 deaths versus193/16;
therefore this is encouraging evidence with user-perceived improvement, not a
controlled +1.74FPS causal result or locked50Hz guarantee. Worst composition
camera753:2Walkers,1Crab,1projectile,2collectibles,no canonical water update.
Retain patch DMA candidate as working Drowned baseline; original CPU path
remains recoverable. Current drawer Drowned-PatchDMA-020-HD. No new build or
runtime edits in this evidence cycle. Full campaign/ADF/WHDLoad and real
hardware validation not implied. Future route expansions should retain
resident Walker and patch DMA flags and remeasure memory/cadence.


## 2026-09-16 — Geyser accepted; patch DMA performance candidate

User accepts corrected platform nozzle. Latest verified HD run:48.14FPS,
5304 intervals,5100 one-field/204 two-field/0 longer,ownership0. FreeChip599880.
Preserved testresults/Unassigned-Drowned-precision-foot-020-48-14fps.log/TXT.
Worst camera626,3collectibles,no enemies/no canonical water update; do not
attribute remaining misses to the new geyser from this maximum alone.

New isolated SPARKPAW_DROWNED_PATCH_BLIT switches canonical-to-inactive-ring
mechanism rectangle transfers from CPU word loops to A-to-D Blitter copies.
Same aligned clipping, four planes, three ring copies, modulo strides, source
and target Chip buffers; waits retire DMA before reuse/return. No new buffers,
art, animation timing, water changes or gameplay changes. CPU path retained.
Native build and actual CPU/DMA register simulation vs pixel oracle passed
27840 rectangles per mode, including clipping and wrapping. This establishes
pixel transfer parity, not Amiga performance. Blitter contention remains a
measurement question. All66 packaged assets identical;68release files intact.
Current test:dist/Drowned-PatchDMA-020-HD/Drowned-Test. Prior drawer preserved
under dist/older-builds/20260916-before-patchdma. Proof:build/drowned-route/proof-patchdma.json.
User020 cadence/visual gate pending; no release/version change.


## 2026-09-16 — Resident Walker user test: 49.49 FPS

User played the resident-cache HD candidate and reports it felt slightly better.
Verified played drawer against proof-resident.json; release hashes unchanged.
Preserved raw evidence: testresults/Unassigned-Drowned-resident-walker-020-49-49fps.log
and matching TXT. Configuration is session-reported 68020 / 2 MB Chip / 8 MB Fast,
not freshly read from the emulator. 2965 intervals: 2935 one-field, 30 two-field,
zero three-plus; 49.49 effective FPS; zero ownership violations. Miss share
1.01%, versus 4.86% in Route2 (47.68 FPS). These are different manual workloads:
71 player shots versus 116, 11 enemy deaths in both, 2 Walker shots versus 1.
Therefore the result supports retaining the resident candidate as the working
Drowned baseline, but does not prove a controlled +1.81 FPS optimization gain
or locked 50 FPS. Default non-candidate renderer remains unchanged.

Measured prepared Chip free 652424 bytes, largest block 651176; Fast free
5511600. Exactly 192000 fewer free Chip bytes than Route2, matching the planned
tradeoff. Worst composition now camera 612, one Walker, three collectibles,
one water update; this single maximum does not locate all remaining misses.
No runtime changes or new test drawer in this evidence-only cycle. Current
active drawer remains dist/Drowned-Resident-020-HD. Next performance work should
focus on remaining draw/restore and scroll overlap with matched workloads;
retain visual quality and measure memory again as the level grows. No ADF,
WHDLoad, real-hardware or full-campaign acceptance inferred from this HD run.


## 2026-09-16 — Resident Walker cache candidate (020 test pending)

User confirms the route pickup/diamond restore correction. Remaining perceived
FPS dips occur near Pump Walkers; the larger final water section feels smooth.
Preserved Route2 evidence: `Unassigned-Drowned-route2-walker-load-47-68fps.log`
and TXT under testresults: 2326 intervals, 113 two-field, no longer intervals,
47.68 effective FPS, zero ownership violations. Worst composition: two Walkers,
one Crab, three projectiles, camera 1052. This does not establish the cause of
every missed interval; manual runs are not controlled benchmarks.

New diagnostic `dist/Drowned-Resident-020-HD/Drowned-Test` keeps the same route,
art, audio, poses and pickup fix. Compile-only `SPARKPAW_DROWNED_RESIDENT_WALKER`
places the 204800-byte Walker frame cache in Chip instead of Fast and removes
12800 bytes of mutable Chip stages: net +192000 Chip (187.5 KiB), -204800 Fast.
Direct immutable frame pointers remove 3200-byte pose copies during gameplay.
Actual memory availability and FPS gain remain unverified until user testing.
Default staged path is retained. Native build and ASan/UBSan actual-selector
parity tests passed (32 frames, both facings, four slots; resident zero copies).
All 66 packaged assets match Route2; all 68 release files are unchanged.
Previous drawer including logs and metadata preserved under
`dist/older-builds/20260916-before-resident-walker`.
Proof: `build/drowned-route/proof-resident.json`. No release or version change.

## First encounter user result —15 September2026

User played all3 builds and reports no visual anomalies; possible first
encounter hiccup, less/absent later, uncertain. Keep observation open.
All3 package hashes verified;68 alpha.8 release files unchanged.
Measured encounter:2561intervals,2537one-field,24two-field,0three-plus,
49.53fps,51.70s,0ownership violations. Long share0.937%; environment Opt5
0.102%,49.94fps, but manual workload differs and plain builds unmeasured.
42 player shots,2 Strider shots,7 enemy death requests confirm combat activity.
Worst aggregate frame473/camera433 records no enemies/projectiles drawn;
this cannot attribute all long frames or confirm first-activation cause.
Source:patterns/stages built at startup; activation copies existing state;
do not explain the user's hitch as lazy asset loading without evidence.
Log/TXT preserved:testresults/Unassigned-Drowned-Encounter-020-cadence-49-53fps.log.
Original remains in encounter diagnostic drawer. Full memory footer present.
Prepared Chip962328/largest960936,Fast5363392/largest5361400;
post-run Fast5740424. Lifecycle snapshots only.
Visual review positive; sustained50Hz/full-level/enemy margin still unproven.
No runtime changes this review. Next bounded diagnostic should correlate
activation/live enemy counts/combat with individual long intervals if the
first-encounter hitch is investigated; avoid speculative cache fixes.

## Opt5 play builds and first enemy encounter —15 September2026

User authorized next step:retain Opt5, diagnostics-free play build, then
representative enemy load encounter. Three self-contained active drawers:
- dist/Drowned-Opt5-Play-HD/Drowned-Play:environment only,no diagnostics.
- dist/Drowned-Encounter-Play-HD/Drowned-Play:encounter,no diagnostics.
- dist/Drowned-Encounter-020-HD/Drowned-Test:encounter,minimal cadence.
All use SPARKPAW_DROWNED_COLUMN_BLIT and retain Opt1..5 changes. Enemy flags
are isolated, not part of ordinary campaign. No runtime art edits/releases.
Required Beetle spawn552..568,surface496..608,floor200; Strider360..376,
surface328..448,floor200. Strider is clear of raised platform; Beetle fits
under it. Patrol bounds apply to existing foot probes, not full art edges.
Both use existing random speed48/96/192 and ENEMY_POLICY_RESPAWN; Strider
shoots. Level1 enemy assets temporary; no new designs/music/traversal routes.
This first encounter deliberately tests two AI types + shots + geyser/water;
full eventual density/jumps still require their own authored test.
Projectile callbacks now select nearest enemy or panel on sweep in either
direction, then dispatch hit; prior slice callbacks only hit panel.
Host actual enemy AI test verifies support bounds1200ticks,shots,projectile
ordering,panel activation,death,score and offscreen respawn. Initial test
assumed full-art patrol bounds; corrected to actual existing foot contract.
All three native builds and the full host suite pass after test correction.
63 assets/47 refs per drawer; all assets matchOpt5,68 release files unchanged.
Prior Opt5/log archived intact:older-builds/20260915-before-drowned-encounter/.
Proof:build/drowned-slice/encounter-package-proof.json. No autoplay.
Next user test Drowned-Test on020 real/cycle-exact,JIT0,2MBChip+8MBFast,
60-90s fighting both at geiser/water,open gate,leave and return for respawn;
no pause,LMB release then15s save/hold. Plain builds do not produce logs.
No FPS/enemy-headroom or visual palette acceptance claim until user review.

## Opt5 020 result —15 September2026

User played and saw no glitches. Exact package verified. Complete log:
2938intervals,2935one-field,3two-field,0three-plus;49.94fps,0ownership
violations. Sample58.82s. Long share0.102% versusOpt4 0.397%;~74% lower.
Manual workloads differ; encouraging gain, no controlled same-input claim.
Three compositor raster crossings;worst frame1112/camera433 remains.
User visual acceptance applies to this run, not proof of all DMA edge cases.
Keep Opt5 as latest tested candidate; requires drowned-column target and
SPARKPAW_DROWNED_COLUMN_BLIT. Ordinary drowned-cadence still uses Opt4 CPU
column path by design. No runtime edits/promotions/releases in this review.
Active:dist/Drowned-Opt5-020-HD. Original log kept; copy/TXT:
`testresults/Unassigned-Drowned-Opt5-020-cadence-49-94fps.log`.
Prepared Chip963640/largest962392,Fast5365944/largest5364784;
post-run Fast5742976. Lifecycle snapshots only.68 alpha.8 files unchanged.
Almost50Hz in partial slice, not stable50Hz or demonstrated enemy capacity.
Next priorities: retain successful bounded changes, assess residual frame
budget under representative enemies rather than infer margin from roundedFPS;
continue compiler/game-update audit without claiming hardware/media acceptance.

## Opt5 column DMA candidate — 15 September2026

Active manual test:dist/Drowned-Opt5-020-HD/Drowned-Opt.
Separate drowned-column target/SPARKPAW_DROWNED_COLUMN_BLIT flag on Opt4.
Only +/-16px canonical ring-column copy changes:12 one-word-wide blits
replace832 CPU row iterations (4planes x208rows), preserving all3 copies.
Source canonical front and inactive destination are Chip-backed. Final wait
retires DMA before CPU patch writes. Larger rolls keep generic path.
No new allocations, art or gameplay changes. Blitter rereads source3 times;
speed advantage uncertain. Previous targeted ring roll p95~3.92ms motivates
measurement, not a claim of DMA superiority or root cause at camera433.
At rightward camera432, origin336 admits world832 column; this coincides
with gate/other work but aggregate logs do not isolate their exact overlap.
Source/compiler CPU loop inspected; candidate host DMA model verifies all60
world columns, all planes/copies, strides, guards, twelve blits and final wait.
Native candidate and full host suite pass.
Opt4 minimal rebuilt byte-identical; code fallback intact.63 assets/47refs
verified, assets identical,68 alpha.8 release files unchanged.
Opt4/log archived intact under older-builds/20260915-before-drowned-opt5/.
Proof:build/drowned-slice/opt5-package-proof.json. No automatic emulator run.
Next02060-90s scroll left/right through geyser/water/gate, jump/shoot; check
strips/trails/missing pixels; no pause; LMB release then15s save/hold.
Compare against Opt4 49.80fps,10/2518long with workload caveats.
Reject DMA candidate if cadence/visuals regress. Stable50Hz/enemies pending.

## Opt4 020 result — 15 September2026

User completed exact verified Opt4 build:2518 intervals,2508 one-field,
10 two-field,0 three-plus;49.80fps,0 ownership violations. Complete log
and memory footer. Long share0.397% versus Opt3 1.792%;~78% lower share.
Sample50.56s versus55.66s. Stronger improvement than prior micro-changes,
but manual inputs differ; do not claim controlled same-workload gain.
11 compositor raster wraps are NOT ring-update counts. No event buckets
in this minimal build. Worst frame860/camera433 remains unresolved.
Original retained; copy/TXT at
`testresults/Unassigned-Drowned-Opt4-020-cadence-49-80fps.log`.
Package hashes match opt4-package-proof.json;68 release files unchanged.
Prepared Chip963640/largest962392,Fast5366200/largest5365040;
post-run Fast5743496. Lifecycle snapshots only. Retain gate early rejection;
next focus remaining camera433/column-transfer peak, broader engine audit.
Still not sustained50Hz or proof of enemy capacity. No runtime edits this review.
Active drawer:dist/Drowned-Opt4-020-HD.

## Opt4 gate collision candidate — 15 September 2026

Active user test: dist/Drowned-Opt4-020-HD/Drowned-Opt. Opt3 retained;
only gate collision evaluation order changes. Reject x outside800..831
before frame lookup; permanent header returns true, fully open/outside-y
returns false before calculating leaf bottom. All original point semantics.
Actual68020-O2 assembly confirms distant probes skip both calls, saved
registers reduce5->3; remaining frame lookup only on relevant leaf probes.
Assembly: build/drowned-slice/opt4-gate.asm. This proves code change, not FPS.
Truth table in actual-state host test covers45 ticks/states (closed, all
opening, fully open/saturated),963 x259 point grid plus WORD extremes:
11223765 grid comparisons plus180 extreme checks. Existing physics,
projectile/sweep/header tests and full host suite pass; native candidate passes.
No art/animation/allocation changes. Event counters off, same minimal cadence
as Opt3.63 assets/47 references verified, assets identical,68 alpha.8 files
unchanged. Events drawer/log archived intact under
older-builds/20260915-before-drowned-opt4/. Opt3 remains separately preserved.
Next user02060-90s:move/jump/shoot around geyser and gate; open gate and
continue on both sides, jump under header. No pause; LMB release then15s save.
Compare cautiously with Opt3 49.11fps,49/2734long; no gain or50Hz/headroom claim.

## Event correlation result — 15 September 2026

Complete verified log:2451 intervals,48 long,49.03fps,0 ownership violations.
8 buckets sum exactly to cadence totals.44/48 long intervals follow ring-column
updates:44/260(16.92%) versus4/2191(0.183%) without column update.
Strong association, not exclusive causation: other work coincides with copies.
Canonical water alone is not supported as main cause: no-column/no-raster-wrap
buckets total2187 intervals including1094 water updates, with0 long intervals.
47 raster wraps,46 long intervals with that bit; wraps are NOT ring counts.
Worst frame892/camera433. Full log preserved with TXT:
`testresults/Unassigned-Drowned-020-event-correlation.log`.
Exact package hashes and68 release files verified; original log retained.
Opt3 remains retained by explicit user preference. No runtime change this
review. Next bounded candidate: gate collision early spatial/state rejection
from compiled-code audit; then minimal cadence measurement. Continue ring-copy
and inclusive game-update audit, without combining hypotheses or claiming50Hz.


Opt3 retained by explicit user preference: user feels smoother movement;
49.11 versus49.07fps does not independently establish an optimization gain.
No art, timing, palette or music reductions authorized by this audit.

## Compiled-code evidence

Generated actual isolated Opt3 translation units using project vbcc,
+aos68k -O2 -cpu=68020 and the minimal-cadence renderer flags.
Outputs: build/drowned-slice/audit-{drowned_slice,collision,game,player,renderer}.asm.
These are compiler outputs, not a disassembly of the played executable.
Final link addresses/cache-set placement have NOT yet been verified.

1. Collision: collisionSolidAt calls drownedGateSolid before normal bounds/
   terrain handling. The compiled gate routine saves five registers, calls
   drownedGateFrame and drownedHeaderOverlaps before rejecting distant x.
   Gate-frame calculation uses divs.l #3 when gateTick>=10, including fully
   open state. Thus queries far from the gate perform irrelevant mechanism
   work. Highest-priority bounded candidate: reject x outside800..831 before
   frame computation, and handle permanent header/open door before lifting
   calculation. Preserve every collision boundary, opening tick and query
   caller. Test point truth tables through all43 opening ticks plus existing
   player/projectile tests; then measure minimal cadence. No gain claimed.
2. Geiser animation: compiler emits signed division/remainder instructions
   for the active frame formula despite a bounded nonnegative tick range.
   Unsigned local phase/masks may simplify it. Lower priority: selected once
   per render update, unlike collision queries. Do not combine with item1.
3. Renderer: water and mechanism row-stride changes retained. Per-word Chip
   writes and three physical ring copies still required by current contract.
   Aggregate counters cannot establish which calls coincide with long frames.
   Avoid removing ring copies or changing DMA ownership without a separate
   geometry/fetch proof and matched hardware measurement.
4. Gameplay: targeted game_update average4.25ms is a broad inclusive scope.
   Includes movement/collision, enemy pool, pickups, projectiles and audio.
   Inspect dynamic calls and generated code before deleting apparently empty
   enemy work: real enemies will be added, so a slice-only zero-enemy shortcut
   would not establish future headroom. Prefer real hot-path improvements.
5. Sprite staging: prior p95~0.65ms, maximum~3.68ms. Masked gate overlap forces
   fresh copies to preserve source pixels. Potential cache identity must include
   pose, facing, world position and occlusion; preserve both inactive stages.
   Measure overlap/non-overlap before adding another cache/allocation.

## Correction and next measurement

The existing `wraps` field counts compositor raster-boundary crossings;
it does NOT count scroll-ring movements. Earlier handoff/result wording
calling it ring wraps was inaccurate. A long interval and a raster crossing
can both reflect the same lateness, not establish a scrolling cause.

Drowned-Events records eight buckets, using existing diagnostic flags and
existing CIA interval read. Bits:0 ring-column update;1 canonical water update;
2 compositor raster crossing. Read preceding update flags before reset and
attribute the following update-to-update interval to them. Count total and
long intervals for each bucket. No additional timer reads. Small observer
cost remains; correlation alone is not causation. Last update without a
following interval intentionally has no bucket, matching cadence totals.

Continue broader source/compiler audit after the event result. For any
code-layout hypothesis retain exact executable hash and obtain verified link
addresses; do not infer cache behavior from source appearance or assembly size.

## 2026-09-16 — Ferry flight clearance optimization candidate

User approves v6 route and authorizes020 optimization. Verified latest log:
914intervals/839one/75two/0three+,46.20FPS over19.78s,0ownership violations.
Worst snapshot3flyers/3collectibles/6water updates; minimal diagnostics do not
attribute exact CPU cost. Preserved Unassigned-Drowned-upper-short-46-20fps.log/TXT.

Identified repeated static geometry work: clearSwoop up to96 horizontal scans
per attempt plus3per patrol update. New generator builds2400-byte8flag-per-x
lookup from final collision map and actual arc constant. Flags encode both
profiles/directions and hover bob phases. Static normal/Fast program data,
no Chip allocation or displayed-buffer changes. SPARKPAW_SPILLWING_CLEARANCE_REFERENCE
retains original scanner for proof. Identical blocking, cooldown, attack and
patrol decisions; table valid for current static ferry geometry only. Future
moving gates/platforms require invalidation or separate dynamic checks.

Exhaustive sanitized actual-C comparison:9600swoop predicates plus19200full
updates, byte-identical Enemy state. Reference2399629horizontal probes across
this exhaustive TEST (not a gameplay-run count), candidate0. Uses real loaded
collision map. Existing middle/boat and upper-route physics pass. Nativebuild
passes. VBCC-O2-cpu68020 assembly audit: reference7collision symbol refs,
candidate0; source table embedded in normal program data. No measuredFPS gain
yet. Same67runtime assets independently hash-identical to v6.

Current Drowned-Ferry-020-HD/Drowned-Test(v7),67assets/48refs,68release hashes
unchanged. Accepted geometryv6 and log archived and hash-verified.
Proof build/drowned-ferry/proof-clearance.json. User matched combat/cadence
pass pending; no release/version/commit/emulator launch. If drops remain,
profile water+collectible drawing/restore work separately, without weakening
visuals or accepted gameplay. Do not call this optimization accepted yet.

## 2026-09-16 — Flight lookup candidate result: incomplete performance win

User played v7; no explicit visual/glitch verdict. Exact executable verified
against proof-clearance.json; stable full footer, evidence preserved in
Unassigned-Drowned-clearance-47-64fps.log/TXT.1257intervals:1199one,56two,
2three-plus,max4fields;47.64FPS,0ownership violations.29shots/4kills/12jump
requests/5collects/1water. Prior v6 was46.20FPS with no three-plus intervals.
Different manual workload: higher average alone does NOT prove overall win.
Two longer hitches remain/newly observed; lookup retained only as candidate,
not final native acceptance. Correctness parity and removed CPU scans stand.
Worst snapshot1flyer/3shots/3diamonds/6water updates suggests inspect water,
collectible/vlot restore/update/draw next; minimal log cannot attribute cause
or the two long intervals. No runtime/build/release change this review.


## 2026-09-16 — Wider renderer and generated 68020 code audit

Research-only pass requested after v7 playthrough. No executable, runtime
asset, release, level layout or staging changes. Existing local changes retained.
The v7 result remains 47.64 FPS with two intervals of at least three fields;
its minimal log does not attribute those intervals to an individual subsystem.

Compiled the entire renderer to assembly using the actual ferry Makefile flags
(VBCC -O2 -cpu=68020 plus all ferry/AGA/minimal diagnostic defines), replacing
only the link inputs/output with -S src/renderer.c. Compilation succeeded.
Reproduction command: build/drowned-ferry/renderer-audit-command.txt; generated
code: build/drowned-ferry/renderer-audit.asm. This is current-source compiler
output, not disassembly of the user's staged executable.

Findings and priority:

1. Water synchronization copies each dirty 80px strip separately into all three
   physical copies of the inactive ring, four planes each. Adjacent changed
   strips can potentially be merged before the existing bounded copier splits
   at the 512px ring seam. Example: six strips at x160,240,320,400,480,560 and
   ring origin160 currently require 84 blits (one strip crosses the seam).
   One 480px copy would require 24. This is fewer setups, not 71% faster frames:
   copied bytes remain identical and canonical water generation still runs.
   An offline interval model checked all 1024 dirty subsets across 65 origins
   for both adjacent and gapped strips: 133120 cases, identical ordered pixel
   coverage and never more chunks. Result: water-batch-research.json in the
   build drawer. This is NOT an actual-C/pixel/DMA test or native measurement.
   Implementation should merge only adjacent dirty strips, maintain each
   waterFrame entry, preserve clipping, all three copies and final waits.
2. Actual assembly for drawPontoon contains divsl.l #80 plus multiplies by80
   and56 before its 112-byte CopyMem into Chip memory (around lines8371–8375).
   This confirms the compiler has not strength-reduced the modulo. A bounded
   phase or precomputed offset could remove it, but is a secondary candidate:
   only once per visible frame; requires reversal/reset/bob equivalence proof.
3. Renderer assembly contains no performanceProfileBegin/End calls. Broad
   profiling is already removed by minimal cadence defines; the profile writer
   remains. Cadence counters still cost something, but deleting logging is not
   a supported explanation/fix for the remaining drops.
4. Canonical update, ring synchronization, Bob restoration and masked drawing
   all share the Chip bus. Fewer enemy collision scans alone cannot eliminate
   those transfer costs. Prioritize water batching, then measure restoration
   and drawing separately if needed. Keep gameplay, water cadence and AGA
   presentation fixed for a useful comparison. Do not remove ownership waits
   or alter Copper publication timing speculatively.

Primary documentation consulted:
- Commodore Amiga Hardware Reference Manual, chapter6 (original manual mirror):
  https://www.theflatnet.de/pub/cbm/amiga/AmigaDevDocs/hard_6.html
  Blits require register setup; published copy timings explicitly exclude setup
  and DMA contention. Display/audio/Copper compete with Blitter/CPU for Chip
  access. Existing A-to-D copy is already the appropriate channel combination.
  Its old OCS display slot examples are not an AGA frame-time measurement.
- Motorola M68020 User's Manual, chapter4, published by NXP:
  https://www.nxp.com/docs/en/data-sheet/MC68020UM.pdf
  Instruction cache has 64 longword entries; data accesses are not cached.
  More Fast RAM is useful for prepared data but does not remove Chip DMA cost.
  No cache-conflict or instruction-cycle-based FPS claim was made.

Next concrete experiment: isolated adjacent-water batching with reference
switch, actual copier pixel/phase parity tests (including ring seams, clean
strips and gaps), native build, then matched user 020 combat/cadence test.
No additional playtest is needed for this research-only pass.

## 2026-09-16 — Adjacent water synchronization candidate

User authorizes the water-copy optimization after the wider renderer audit.
Ferry-only drowned_water_sync.h groups adjacent dirty80px strips, then uses
the existing bounded DMA copier. Clean strips and gaps terminate groups;
per-strip phase bookkeeping, clipping, all three physical copies and final
waits preserved. No extra runtime allocation, visual/cadence/gameplay change.
SPARKPAW_WATER_SYNC_REFERENCE retains the previous path.

Actual C helper + actual copier tested against independent pixel oracle with
ASan/UBSan:133120 cases across all dirty subsets, ring origins, adjacent/gapped
layouts and different canonical phases. All pixels and phases match, second
sync is a no-op, no extra blits. Example84->24 operations; not an FPS claim.
Existing CPU/DMA copier tests also pass27840 rectangles per mode. Native ferry
build passes. Full renderer reference assembly is byte-identical to the
pre-change audit; candidate assembly confirms one copy per merged run.

Current dist/Drowned-Ferry-020-HD/Drowned-Test is the water candidate;67assets
byte-identical,48embedded refs,68release files preserved. Previous drawer
and log archived and hash-verified; proof build/drowned-ferry/proof-waterbatch.json.
ReadMe identifies candidate; log v7 continues to identify unchanged layout/AI,
so use executable hash for this comparison. Last baseline47.64FPS with2long
intervals; no new FPS measurement. User020 cadence/visual acceptance pending.
No emulator launched, release, version, commit or push.

## 2026-09-16 — Water batching playtest: positive result

2026-09-16: user FS-UAE playthrough, established PAL50 68020/2MB Chip/8MB Fast configuration. User: "oogde iets beter volgens mij". No explicit exhaustive glitch verdict.
Source: sparkpaw/dist/Drowned-Ferry-020-HD/renderdiag.log. Complete post_run footer. Executable SHA256 matches proof-waterbatch.json. Log v7 describes unchanged layout/AI; candidate identified by executable hash.
1415 intervals:1384 one-field,31 two-field,0 three-plus,max2;48.92FPS. Missed interval share2.19%, previously4.61% (1257intervals,56two,2three-plus,max4;47.64FPS).
New workload49shots/4kills/16jump requests(15starts)/5collects/1water/2hurt; prior29shots/4kills/12jump requests/5collects/1water. Manual workloads differ; positive evidence, not controlled causal measurement.
0ownership violations. Chip607400free/largest606184; Fast5224936prepared. Worst snapshot is frame1/camera0/water_updates0, not proof that gameplay-water work is costless.
Retain water batching as current working candidate. Full50FPS and broader/native hardware acceptance remain open. No runtime change in review.

Evidence: testresults/Unassigned-Drowned-waterbatch-48-92fps.log and matching TXT.

## 2026-09-16 — Pontoon mask offset candidate

User authorizes one focused final optimization pass before content expansion.
Retain water batching (last run48.92FPS,31two-field/1415intervals,0three-plus).
Ferry-only prepared position/phase offsets replace the visible pontoon mask
address division by80 and multiplications by80/56.1538bytes normal program
BSS, no extra Chip allocation or asset bytes; preparation once at asset load.
Position indexed directly from existing physics, so no new movement state.
Reference switch SPARKPAW_PONTOON_OFFSET_REFERENCE retains original formula.

ASan/UBSan actual lookup comparisons67680: every705legal integer x, both
deck heights,16frames, three preparations; offsets identical and bounded.
Actual mask test now exercises candidate lookup:2293760pixels match independent
water-pattern oracle. Native build passes. Full renderer reference assembly
byte-identical to previous waterbatch assembly. Compiler first emitted a
three-argument call; lookup made an expression (each argument evaluated once)
so final hot assembly has direct reads/add/shift, no division/multiplication
or added helper call. Evidence build/drowned-ferry/offset-*-excerpt.asm.

Broader copy review: projectiles/enemies restore only when their target history
says drawn; inactive/dead state alone cannot safely skip old image removal.
Diamonds restore a word-aligned footprint, including after collection, and
water/other actor restoration can overlap it. Existing enemy union optimization
applies to Striders; blindly extending to separated flyers may copy larger
areas than saved. No such change included; a separate actual pixel/ownership
proof plus measurement would be needed. Preserve final waits and draw order.

Current dist/Drowned-Ferry-020-HD/Drowned-Test is offset candidate;67assets
byte-identical,48runtime references,68release hashes preserved. Previous
waterbatch build and full log archived and hash-verified. Proof:
build/drowned-ferry/proof-pontoon-offset.json. User020 cadence/masking gate
pending; small expected gain only, no FPS claim. No runtime gameplay changes,
release/version/commit/push or emulator launch.

## 2026-09-16 — Pontoon offsets playtest: similar measured cadence

User FS-UAE playthrough, established PAL50 68020/2MB Chip/8MB Fast configuration. User reports "weer iets beter volgens mij"; no exhaustive visual acceptance inferred.
Executable hash matches proof-pontoon-offset.json. Complete post_run footer; preserved original log bytes. Header v7 describes unchanged layout/AI, not unique optimization identity.
1441 intervals:1407one-field,34two-field,0three-plus,max2;48.84FPS. Previous waterbatch run:1415intervals,31two-field,0three-plus;48.92FPS. Miss shares2.36% vs2.19%. Numerically essentially similar, no measured improvement established and no controlled regression established.
Current71shots/5kills/17jumps/5collect requests/1water/1hurt vs prior49shots/4kills/16jump requests/5collects/1water/2hurt. More shooting is not sufficient workload normalization: early kills can also reduce rendering work.
0ownership violations. Prepared Chip606696free/largest605576 vs prior607400/606184; Fast5225168free/largest5223976. Whole-system allocation snapshots differ; no extra explicit Chip allocation in candidate source, but do not claim identical measured Chip usage or infer cause of704-byte difference.
Retain current candidate provisionally given correctness proof and positive user impression; do not label this lookup a demonstrated FPS win. Waterbatch improvement remains prior evidence. Full50FPS target remains open. No new build/runtime change during review.

Evidence: testresults/Unassigned-Drowned-offset-48-84fps.log and matching TXT.

## 2026-09-19 — Connected Drowned level candidate (3520px)

User asks to connect all existing content, then explicitly continues. Built
a single3520px route: accepted land0..2400, translated ferry basin2400..3200,
landing shore3200..3520. Middle deck2768..2864,y160; upper tiny ledges2912,128 /
3008,96 /3104,128, exclusively after middle.33diamonds,11spawn sites,20water
strips. Same original four active enemy slots. Existing gate plus both geysers
re-enabled; no loading boundary. Machine finale, extra gates, station/Rain Core,
new music/checkpoint and campaign handoff remain future work, not claimed done.

Joined-only appended enemy type2 and own24x24/16frame cache for Spillwing;
Crab retains32x24/22frames/type0, Walker64x64/32frames/type1. Independent asset
loading, cache lifetime, hit/contact/death dispatch. No Enemy struct/pool growth.
Caught/removed accidental cache rebuilding in rendererResetGameplay before
staging; cache built once and freed via existing type-count loop. Legacy
isolated flyer alias retained when JOINED is absent.

Keep water batching and mask offsets; move pontoon bounds2400..3104; offline
clearance table now generated for actual map width3520. Existing rear1120px
fits final camera with no extension. Exact native-pixel equality proven for
land0..2399 and translated ferry source160..1279; original rear/patches/three
enemy art/clip bytes unchanged. Known extra Chip allocation relative to ferry:
116480bytes foreground +31680bytes separate Crab cache; transient allocation
peaks/largest blocks and020 cadence still need native evidence.

Actual-C ASan/UBSan tests: all11sites/3families activate over full forward/back
camera sweep; cache frame bounds; static flyer collision; per-family hits/death;
visible panel/open/reset; both jets; shifted boat/middle/boat route and allfour
upper transitions; no first-half overhead bypass;33pickups and water boundary.
Existing land test passes precision jumps and all4Walker traversal links over
8seeds. Existing ferry clearance test passes9600predicates+19200updates;24px
cache, visible shots, projectile damage, post occlusion tests pass. Mask offsets
pass67680cases for each isolated/joined bounds. Native joined build succeeds,
only pre-existing main/ready_ui warnings. No emulator auto-run.

Current dist/Drowned-Level-020-HD/Drowned-Test;68assets/49embedded refs, all
68release hashes preserved. Staged via stage_hd_test against old ferry drawer
then renamed verified new drawer. Prior offset build+log archived byte-identical.
Proof build/drowned-joined/proof-joined.json; generator build_drowned_joined.py,
manifest and joined-layout.png in build/drowned-joined. User full-run/020 memory,
FPS, combined gate/water/flyer visuals and life-reset acceptance pending.
No release/version/commit/push.

## 2026-09-19 — Checkpoint priority + regional FPS preparation

User reports repeated deaths and late-ferry drops, requests checkpoint and FPS
investigation. Played joined log verified/preserved:7100intervals/108two/0three+,
49.25FPS,0ownership violations,Chip461864free/largest460512. Aggregate masks
section-specific cost; do not dismiss user report. Evidence
testresults/Unassigned-Drowned-joined-49-25fps.log/TXT.

Prepared camera correction: joined build inherited fixed Level1 end-lock at
playerX3072, inside ferry. Joined now retains centered/clamped follow there;
legacy behavior unchanged. Actual-function host test passes. Not claimed FPS
fix. Added minimal previous-player-X cadence counters for land/precision/
approach/ferry_first/ferry_last/shore;120bytes, no new timer reads or frame I/O.
Header stale pontoon basin corrected. Source native build succeeds, not staged.

Checkpoint concept image assets/concept/drowned-checkpoint-beacon-v1.png,
full prompt TXT; steel/copper mechanical marker, amber->mint lamp, hinged
metal pennant. Pending user concept approval; native art must fix intermediate
hinge and omit broad presentation glow. Sound audition
assets/audio/checkpoint-v1/checkpoint.wav/raw:6828bytes,period322,~0.62sec,
mechanical click+rising tones, no voice; not runtime integrated.

Checkpoint state module prepared but NOT linked/wired: grounded one-shot
activation at2320,32tick activation, persists life loss, clears fresh attempt.
19600host contract cases pass in both test modes, plus regional boundaries/
totals and joined/legacy camera. Integration/respawn/render/audio still pending.
Plan safe Crab patrol adjustment and preserve score/diamonds/gate while boat
resets to left bank. Read docs/DROWNED_CHECKPOINT_REVIEW.md before continuing.

Current dist remains exactly the played joined build; all68release files
hash-identical. No new user test requested until checkpoint concept approval
and actual native art/animation/integration. Built build/sparkpaw-drowned-joined
now contains prepared camera/regional changes and differs from staged proof;
do not misidentify it as the played build. No release/commit/push/emulator run.

## 2026-09-19 — Checkpoint run1 log preserved; regional diagnostic bug

Preserved testresults/Unassigned-Drowned-checkpoint-regional-run1.log/TXT from
old-182507 drawer (first checkpoint, before no-Core fix), verified executable
against proof-checkpoint-first.json and complete post_run footer.4557intervals,
110two-field,0three-plus,48.82FPS,0ownership violations; checkpoint1request/1start.
User reports recurring drops. Chip456576free/largest455224, Fast5186632free.
REGIONAL SPLIT INVALID: rendererDiagnosticUpdateEntry memsets diagnosticCurrent
before drownedCadenceRecord reads its playerX, assigning all samples toland.
Fix next: retain previousplayerX independently and test full entry lifecycle.
Current no-Core stagedbuild has same bug; user doing another run, do not swap
build while playing. Whole-run cadence valid; cannot localize ferry fromthislog.
No runtime change made in this analysis.0.43FPS below prior49.25 with different
workload is not controlled regression evidence. See sidecar for exactprovenance.

## 2026-09-19 — Regional cadence attribution v2 staged

User offers to wait for improved log; repaired prior-player-X capture BEFORE
memset of diagnosticCurrent. Existing global cadence algorithm unchanged.
Log joined header now has region_attribution=2. Full actual entry-function test
(test_drowned_cadence_entry.py) covers all region boundaries both directions,
first sample,1/2/3field intervals and ferry->checkpoint teleport; regional totals
match global totals. Sanitizers pass; mutation restoring original fault is
rejected. Native020 build passes with existing warnings only. No gameplay/art
change; checkpoint and no-Core fixes retained. Runtime regional evidence pending.
Restaged Drowned-Level-020-HD/Drowned-Test;70assets/51literalrefs verified,
68release files unchanged. Previous no-Core drawer archived intact at
/Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-182814. proof-checkpoint.json current; proof-checkpoint-nocore.json previous.
User next run should save once with LMB, frozen image expected, wait15seconds.

## 2026-09-19 — Regional v2 full run: first ferry half is hotspot

User reached end; unexpected falling1-up. Evidence preserved as
Unassigned-Drowned-regions-v2-full-run.log/TXT; exact current executable verified,
complete footer, region_attribution=2; regional sums agree with global.
6334intervals:83two-field,0three-plus,49.35FPS,0ownership violations.
land49.47/14misses; precision49.38/6; approach49.80/2;
ferry_first47.82/34of748(4.55%); ferry_last49.43/27of2348(1.15%);
shore50.00/0of920. Firsthalf now measured investigation priority; bothferry
sections61/83misses. No family CPU/blitter attribution in minimal log.
Checkpoint1/1sound; extra_life1/1. Inherited Level1 extraLifeShouldReveal still
unguarded in game.c: needs Drowned gameplay/render exclusion; Core alreadyhidden.
No changes staged this analysis (user asked log first). Differentworkload and
18.4sec50FPSshore mean49.35vs48.82 does not prove optimization.

## 2026-09-19 — Late-ferry drops confirmed; precision Spillwing staged

Same-build repeat preserved: testresults/Unassigned-Drowned-regions-v2-run2.log/TXT,
plus precision request PNG/TXT.5310intervals,161two-field,0three-plus,48.52FPS,
0ownership. Firstferry46.62FPS/29of401misses; last41.68/101of506(19.96%).
Both halves need investigation; prior last49.43 does not establish healthyfps.
User-requested precision Spillwing appended asID11 at1840,patrol1728..2144,
high profile. Ferry IDs/profiles untouched; same4activepool, existing art and
precomputed collisionclearance. Actual joined C tests pass12spawns, flyer solid
avoidance, jumps and checkpoint resets. Inherited Level1 1up gameplay/render
now disabled in Drowned; renderer tests confirm Level1 behavior unchanged.
Native020 build passes existing warnings only. No FPS fix claimed.
Current dist/Drowned-Level-020-HD/Drowned-Test staged70assets/51refs;
68releasefiles unchanged. Prior drawer/log preserved byte-exact at /Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-183827.
proof-checkpoint.json current; proof-checkpoint-regions-v2.json prior.
User precision encounter feel/FPS gate pending. No emulator/release/commit.

## 2026-09-19 — Patch/water setup reuse candidate, 020 gate pending

Latest evidence Unassigned-Drowned-precision-flyer-run1.log/TXT preserved and
verified. User accepts precision flyer/no perceived extra drops.2737intervals,
76two-field,0three-plus,48.64FPS,0ownership. Precision49.36(6/463misses),
firstferry45.95(17/193),last44.85(28/244). Short ferry samples, not controlledAB.
No shore samples. Both ferry sections remain concern.

Inspected enemy copy-on-unload (already default), Spillwing precomputed collision
(already default), resident caches, pontoon and water paths; retained them.
Concrete redundant work: drownedCopyPatchRect rewrote6constant Blitter words
for everyplane/copy,12times per clipped chunk. Now onlyfirstplane/firstcopy;
66registerword writes removed perchunk. Precompute bltsize once. All DMA waits,
4planes,3copies, pixel spans and strides retained; no new allocations or art/
physics/enemy changes. SPARKPAW_DROWNED_PATCH_SETUP_REFERENCE retains old setup.
VBCC+aos68k -O2 -cpu68020 actual-header probe assembly in
build/drowned-patch-setup-audit/{reference,candidate}.s confirms branches skip
setup and cached size is written froma5, not repeated shifts/stack intermediates.
This is reduced work evidence, NOT measured CPUtime/FPSgain.

ActualC ASan/UBSan pixel oracle:48720rectangles EACH CPU/referenceDMA/candidateDMA,
including160/320/512waterwidths andwrap/clips. Integratedwater133120cases pass,
pixel/phase parity and DMAwait/repeatnoop behavior. Native020 build passes existing
main/ready_ui warnings only. Added missing patchheader Make dependency, ensuring
actual candidate rebuild. No new timing scopes; same regionalminimaldiagnostic.

Staged dist/Drowned-Level-020-HD/Drowned-Test; all70runtimeassets byte-identical
to playedbaseline,51literalrefs covered,68officialreleasefiles unchanged.
Previous drawer/log archived byte-exact at /Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-184938.
proof-checkpoint.json current; proof-before-patch-setup.json baseline.
User gate: bothferryhalves withshoot/jump/return, waterseams/residue andcadence;
saveLMB/freeze/wait15sec/reset. No emulator, release, commit or push.

## 2026-09-19 — Enemy frame address precompute candidate

Patch-setup run preserved Unassigned-Drowned-patch-setup-run1.log/TXT:
3278intervals,67two+2three-field,48.93FPS,0ownership. Firstferry47.40vs45.95,
last45.90vs44.85 previous, butdifferentworkload; not controlledgain. Two60ms
frames(approach/firstferry) are open concern, setup candidate provisional.

User asks furtheroptimization/precompute. Added joined-only frame pointer table
src/drowned_enemy_frames.h:3families x2facings x32slots x2pointers=1536normalBSS
bytes, zeroextraChip. Prepared after each cacheallocation, addresses refer to
same Chip bits/masks; framecounts capped32. Used forCrab/Spillwing draw and
residentWalker selection. ReferenceSPARKPAW_DROWNED_FRAME_ADDRESS_REFERENCE.
All140validframe pairs tested withASan/UBSan againstoldlayout, repeatedprepare,
lastwordwrites andlimitguard; existing24pxcachepixeltest passes. Native020build
passes existing warnings only. VBCC020probe inbuild/drowned-frame-audit shows
lookup uses shifts/pointerloads, replacing layout multiplications; thisdoesnot
measure wholeframeFPS or reduceBlittertraffic. Sameassets/behavior/diagnostics.
StagedDrowned-Level-020-HD/Drowned-Test,70assetsbyteidentical,51refs,68release
filesunchanged. Priorarchive:/Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-190131. proof-checkpoint.json current,
proof-before-frame-address.json prior. User020cadence/visualgatepending.
Noemulator/release/commit. Checktwo60msframes andbothferryhalves next.

## 2026-09-19 — Canonical water batching + first precision Spillwing

User requests firstprecision flyer andbroaderFPSthinking. Latest frame-address
run preserved Unassigned-Drowned-frame-address-run1.log/TXT plus requestPNG/TXT.
3210intervals,72two,0three-plus,48.90FPS,ownership0. Firstferry46.69,last43.19,
not measuredgain over prior47.40/45.90; userfeelsbetter butlateupperroutestutters.
Short/differentworkloads, earliermicrooptimizationsremainprovisional.

Larger candidate: src/drowned_water_batch.h prepares6exact80pxrepeat tiles per
frame/plane. Extra42240Chip bytes, no new diskassets. Same16frames/25Hzphase,
samepixels/bankramps/bubbles; contiguousdirtyvisiblecanonicalstrips groupedmax6.
Up to24canonicalDMAcommands become4; same number of copiedpixels, fewer setups/
launches/waits. Target water synchronization unchanged. Reference macro
SPARKPAW_DROWNED_WATER_BATCH_REFERENCE restores old canonicalpath.
NoFPSclaim. PriorfreeChip456576 implies~414336beforeallocationoverhead; verifylog.
Cache freed/reset onrelease. ActualC DMApixeloracle25728cases passes all16frames,
continuous/gappedlayouts,partialdirty,culling,repeatnoop. Native020buildpasses
existingwarnings. No quality/enemy reductions.

Opening flyer appendedID12 at1488,lowprofile,patrol1328..1552. Wider1648patrol
failedactualsolidavoidancetest duehoveredge; narrowedbeforehigherpillarpasses.
ExistingID11precision/ferryIDs unchanged. Same4activepool,13spawns. ActualC joined
sanitizer testpassesall13spawns/families,flyersolids,jumps/checkpoint.

StagedDrowned-Level-020-HD/Drowned-Test,70assetsbyteidenticalto baseline,
51refs/68releasefiles unchanged. Previousdrawer/logarchive:/Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-191155.
Proof-checkpoint.json current; proof-before-water-batch.json prior.
User gatebothferryhalves/upperroute,waterseams/boatwaterline,Chipheadroom,FPS.
Newenemy affects precisionworkload; ferrygeometry/enemyprofiles unchanged.
No emulator/release/commit. Tests added toMake hostsuite.

## 2026-09-19 — Late-ferry rotating profiler staged, not FPS candidate

Waterbatch userlog preserved Unassigned-Drowned-water-batch-run1.log/TXT.
3273intervals,120two,0three-plus,48.23FPS,ownership0. Firstferry48.93,
last43.67(87of601misses); user reports littlelateimprovement. Chip411744free,
largest410312. Worst snapshotcamera2802,3smallfamilyBobs,2shots,2collectibles,
6waterupdates; no single-snapshot causality. Currentwaterbatch gainsunproven,
retained provisionally for profiling. Minimal baselineproof-before-ferry-profile.json.

New target drowned-joined-profile, SPARKPAW_DROWNED_FERRY_PROFILE, current
Drowned-Level-020-HD/Drowned-Test. Onlyplayerx2800..3199: select1of8scopes each
frame,17frame cycleavoids poweroftwoanimation phasealias. Scopes gameupdate,
displayupdate,totalBobpass,enemies,compacttarget(sync+roll),canonicalwater,
enemyrestore,enemydraw. Maximum1timerpair/frame; aggregateupperplayerY<140count.
Not separateupper/lower timing distributions. Parent/child samples fromdifferent
frames, notadditive. Draw scopes include precedingpendingDMAwaits; avoid equating
CPUorBlitterexclusively. Nonzeroobservercost; do NOT compareFPS asacceptance.
Actualselector/macrotest3200frames passesboundaries/nesting/timerpairlimit.
Actualcadenceentrytest/mutation passes. Native020buildpassesexistingwarnings.
Normalminimalbuild retainsdisabledinstrumentation. Noart/gameplaychanges.

Nativecompileinitiallyrejectedpreprocessordirectivesinsideopenmacroargs; fixed
withcompleteguardedcalls andrebuiltsuccessfullybeforefinalhandoff. An intermediate
olderprofilerbinary wasbrieflystagedduringverification thenarchivedand replaced;
notuseraccepted. Authoritativeproof-checkpoint.json now finalsuccessfullybuilt
profiler. Finalpriorarchive:/Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-192118. Originalplayedminimaldrawer/log remain
intact in dist/older-builds/Drowned-Level-020-H-old-192013.
70assets/51refsverified,68releasefilesunchanged. Userplay30-60sec inlateferry,
upperroute/fighting/return; LMBsave,wait15sec/reset. Nextchoosecausaloptimization
fromscopeevidence. No emulator/release/commit/push.

## 2026-09-19 — Late-ferry upper-route profile identifies buffer work

Preserved Unassigned-Drowned-ferry-profile-upper-twice.log/TXT; exactstagedexe
verified, completefooter. Userupperroute,deliberatewaterdeath/checkpoint,upperagain.
544lateframes,501upperY<140(92.1%). Late41.40FPS/113doubleframes isobserver-
contaminateddiagnostic,notperformanceacceptance.0ownership,0threeplus.
CIA avg:game4.98ms;displayupdate0.65ms;totalBob11.06ms(p9514.01);
compacttarget3.90ms(p956.93);enemyupdate1.30ms;enemydraw1.21ms;
enemyrestore0.70ms;canonicalwater0.78ms. Parent/childnotadditive, different
sampledframes andpendingBlitterwaits. Strongestmeasuredsubsystemlead iscompact
bufferpreparation(scrollroll+water sync+initialwait), notenemyAIalone.
FullcolumnDMAstill208rowsx4planesx3copies; canonicalwaterbatchdoesnotremove
triplicatedtargetsync. Nextfocusreducebuffertraffic/guaranteedblankrows orsplit
roll/synctiming. No speculativevisual/culling shortcuts. Profilerdrawerunchanged.
Chip414336free/largest413304;water1/checkpoint1audio;Core/1up0. Source/evidence
analysisonlythisturn,no runtimechange ornewbuild. Usercheckpointreturnreported.

## 2026-09-19 — Shortened scrolling columns candidate staged

The upper-route profile points to compact-target buffer preparation (average
3.90 ms, p95 6.93 ms including pending DMA waits), rather than enemy AI alone.
The joined renderer now skips guaranteed blank upper rows when replacing one
16px ring-buffer column. It takes the minimum bound of the evicted and incoming
world columns; actor restores precede rolling. Generated bounds cover actual
foreground pixels plus water, mechanisms, beacon and collectible footprints.
All four planes, three physical copies and DMA completion waits remain intact.
Initial/full resets retain full copies; invalid positions fall back to full height.
SPARKPAW_DROWNED_FULL_COLUMN_REFERENCE retains the reference implementation.

The generated 220-byte table needs no extra Chip allocation. Representative late
ferry exchanges skip an average 149 of 208 rows (about 72% less column pixel
traffic, NOT a measured frame-time or FPS gain). Water target sync remains work.
Actual helper and prototypeRollTarget DMA oracle tests pass 1788 cases with
sanitizers, forward/backward scrolling, dynamic rows and guarded buffers. Existing
60-column full-height test passes. Native 68020 build and compiled probe reviewed:
row-offset multiplies are outside the DMA loop. Runtime FPS/visual acceptance is
pending; previous speculative optimizations remain provisional.

Drowned-Level-020-HD/Drowned-Test now contains the normal minimal regional cadence
build, targeted ferry profiler OFF. All 70 runtime assets match the preceding
candidate, 51 executable references verified, all 68 alpha.8 files unchanged.
Prior profiler drawer and log archived byte-identically in
`dist/older-builds/Drowned-Level-020-H-old-193411`.
Proof: build/drowned-joined/proof-checkpoint.json; prior proof-before-column-tops.json.
Repeat upper route, deliberate water death/checkpoint, upper route again; inspect
scroll edges and restored actors/diamonds. LMB once/release, wait 15 seconds on
frozen image, reset. Compare next cadence with minimal water-batch evidence, not
the profiler's observer-contaminated FPS. If gains are insufficient, separate
remaining target water-sync traffic from roll cost before another candidate.
No emulator launch, release, commit or push.

## 2026-09-19 — Shortened columns: first cadence result

 3139 intervals: 3098 one-field, 39 two-field, 2 three-plus, max3;49.32FPS.
0 ownership violations. Late ferry:542 intervals,37 misses,0 three-plus,
46.80FPS;6.83% intervals miss20ms versus87/601=14.48%,43.67FPS in
Unassigned-Drowned-water-batch-run1.log (same minimal instrumentation).
First ferry49.70 versus48.93FPS. Land,precision,shore50.00FPS thisrun.
Two60ms intervals: one approach,one firstferry. Cause not inferable from
aggregate counters; checkpoint/loading causality not established.
Different manual workloads: promising measured association, not controlled
causal gain or final acceptance. Profiler41.40FPS is not a valid baseline.
Chip414336free/largest413304; Fastprepared5184896/largest5183816.
Checkpoint audio1/1,water2/2,Core0,extraLife0.
Retain candidate provisionally. Remaining focus: lateferry target water-sync
traffic and pending DMA waits; distinguish roll from sync if profiling again.
No additional build or runtime changes made during this evidence review.

## 2026-09-19 — Buffer split profiling staged after column gain

Continued investigation: water synchronization already merges adjacent dirty
strips and skips unchanged phases. Three physical ring copies remain required
by the current renderer contract. The previous compact-target profile conflated
initial pending DMA wait, ring roll and dynamic water sync, so no unmeasured
copy-elision or visual reduction was introduced.

The focused ferry profiler now samples eight scopes: game update, total Bob pass,
compact target, canonical water, ring roll, ring dynamic sync, initial compact
Blitter wait, enemy draw. One timer pair per eligible frame, same 17-frame cycle,
player x2800..3199. Log marker buffer_split=1. Roll includes no-scroll calls;
initial wait covers only the pre-compact wait, not every renderer wait. Dynamic
sync includes its copy completion waits. Parent/child costs are sampled on
different frames and cannot be added. This measures remaining work after the
shortened-column change; profiler cadence is NOT an optimization verdict.

Actual selector/macro test passes 3200 frames, checks selected new scopes,
boundaries and exactly one pair per eligible frame. Actual regional cadence
lifecycle/mutation test passes. Native 68020 profile build passes with existing
warnings only. Normal build gains no new instrumentation (wait wrapper guarded).
Drowned-Level-020-HD/Drowned-Test is now this profiler. All 70 runtime assets
unchanged, 51 literal references verified; 68 alpha.8 release files unchanged.
Prior minimal-column build and user log preserved byte-identically in
`dist/older-builds/Drowned-Level-020-H-old-195037`; prior proof saved as
build/drowned-joined/proof-before-buffer-split.json.

User test: upper late ferry, water/checkpoint, upper again, preferably 30-60 sec
in this area. LMB press/release, wait 15 sec, reset. Next use split costs to select
one causal optimization, then return to minimal cadence for the FPS comparison.
No gameplay/art change, emulator launch, release, commit or push.

## 2026-09-19 — Water sync profile and patch invariant candidate

Preserved Unassigned-Drowned-buffer-split-run1.log/TXT, exact executable verified,
complete footer. User upperroute, waterdeath/checkpoint, upper again; second felt
heavier. Aggregate profile does not separate passages. 2964 intervals,57two,
0threeplus,49.05FPS;late46.57 (40/544 misses),ownership0. Profile observer cost;
not a minimal-cadence speed verdict. 491/544 profile frames upperY<140.
CIA averages: dynamic sync3.23ms (p954.26), roll0.20ms (p951.33), initialwait
0.025ms, compact3.49ms, canonicalwater0.75ms, game4.86ms, Bob11.04ms.
Bob maximum37.40ms is an isolated sampled duration, not proof of a specific
cause. Parent/child scopes are sampled on different frames, not additive.

Changed only generic Drowned canonical patch copier: compute source/destination
row offsets once per rectangle instead of once per plane/chunk; move invariant
DMA setup outside plane/copy loops. Safe setup wait added per clipped chunk;
all existing copy/final waits, pixels, four planes and three copies preserved.
Reference setup macro retained. No memory growth or visual/gameplay reduction.
Native probe build/drowned-patch-setup-audit/hoisted.s confirms both multiplies
before loops; previous candidate.s retains compiler comparison. This is a modest
CPU-overhead hypothesis, not a reduction of water DMA bytes or proven FPS gain.
Actual CPU/referenceDMA/candidateDMA tests pass 48720 rectangles each; merged
water helper passes133120 cases;277 authored mechanism transitions pass.
Native normal020 build passes existing warnings only. Runtime acceptance pending.

Drowned-Level-020-HD/Drowned-Test now normal minimal cadence, profiler OFF.
70 assets unchanged,51 compiled refs verified,68 release files untouched.
Previous profile/log archived byte-exactly at older-builds/Drowned-Level-020-H-old-201416.
proof-before-patch-hoist.json preserves predecessor. Next compare with minimal
column-tops run1 (late46.80FPS), not profiler FPS. User repeat upper/checkpoint/upper,
LMB once/release,wait15sec/reset. Watch water,gate,geysers,beacon for glitches.
No emulator launch, release, commit or push.

## 2026-09-19 — Patch hoist first cadence run: encouraging, shorter ferry sample

Patch invariant hoisting first run, reviewed 2026-09-19.
Exact staged executable verified against proof-checkpoint.json; complete log,
minimal regional cadence, profiler OFF. Established FS-UAE HD 68020,
2MB Chip/8MB Fast. User says played; exact route and visual verdict unspecified.
4527 intervals:4497one,30two,0threeplus,max2;49.67FPS,ownership0.
Lateferry297intervals,7misses (2.36%),48.84FPS versus prior minimal column
run542intervals,37misses (6.83%),46.80FPS. Firstferry178intervals50.00FPS,
versus512intervals49.70FPS. Land49.63,precision49.92,approach/shore50.00.
No60ms intervals thisrun. Chip414336free/largest413304.
Material workload difference: lateferry sample about half the prior run;
land2982intervals versus1027. Whole-run FPS not a fair causal comparison.
No proof that microoptimization alone produced regional improvement, nor
that second upper-route passage is fixed. Candidate retained provisionally.
Prior evidence: Unassigned-Drowned-column-tops-run1.log. No new build.


## 2026-09-22 — Full-level FPS A/B staged; user 020 measurement pending

User accepts corrected rear ambience visually and requests full5120 FPS work
on68020/2MBChip/8MBFast/PAL50/noJIT, preserving music/art/gameplay. Alpha.8
remains official; no release, commit, push or automatic emulator.

Read current contracts and historical engine/audio timing evidence. Starting
Drowned-Test SHA25692bf65300ffba69ee894adc60e72ec557c63232f7c8382b632b1bec4e261825d
has NO renderdiag; logger-free A rebuild is byte-identical. Existing startupdiag
was appended during user's run: initial742bytes recovered by exact prefix hash,
current2226bytes separately preserved. User confirmed emulator stopped before
archiving full drawer/metadata at dist/older-builds/20260922-fps-baseline/.
All68alpha.8files unchanged. Historical logs bound to archived executables;
old3520px nonmusic timings are not measurements of current full music level.

One bounded B change: advancing row pointers in rear private staging removes
84 repeated native row multiplies per water upload. Same42CopyMem calls,
1848staged bytes,6blits,3696destination bytes, masks/waits/schedule/animation.
No allocation growth; plain binary60bytes smaller. Actual reference/candidate
pixel/Copper/guard tests and native write-only mask audit pass. This proves
less CPU work, NOT FPSgain. Broader findings and next hypotheses documented.

Active pair: dist/Drowned-FPS-A-020-HD and Drowned-FPS-B-020-HD; launch
Drowned-Test. Both musicv5, same74assets/56refs, fixed matching initial seed.
Separate SPARKPAW_DROWNED_FPS reads CIA-A TOD once immediately after COPJMP1,
no CIA-B timer/profile ownership. Eight regions plus late upper/lower before/
after reset; reset+next and pause accounted separately. Preserve rawzero/two
phase pairs: NOT exact deadline misses. Ownership counts unavailable. Observer
adds2956filebytes/3040loader allocationbytes including448BSS, no explicitChip;
time overhead unmeasured. Build ID in logs/ReadMes; proofs/assembly/plaincontrols
at build/drowned-fps-round. Host counters and actual safe-flush lifecycle pass;
fullworld/sprite/pause/audio regression checks pass; nativefourbuilds pass.

Manual A then B: opening10sec; full route to checkpoint; lateferry upperroute
combat/back-forth20sec; waterdeath/checkpoint; sameupperroute20sec; Governors/
station and laterfalls. BeforeCore LMBpress/release,wait15sec,frozenimage,reset.
Each writes own renderdiag.log; preserve before rerun. Explicit020scope overrides
usualskill030first. Need user's visuals/audio/smoothness AND logs; no50FPSclaim.
Details: sparkpaw/docs/DROWNED_FULL_FPS_ROUND.md. Next do not blindly enable old
renderdiag alongside music or combine other optimizations before this A/B.

## 2026-09-22 — Rear-stage A/B reviewed; runtime reset-copy A/B staged

User played both rear-stage variants and reports no clear speed difference,
with image and music good. Both complete logs/build IDs/executables verified
and raw evidence preserved as testresults/Drowned-full-fps-stage-A/B-020-run1.log
with TXT sidecars. Raw aggregate publications/s: A43.99/B45.01; late ferry
36.40/36.52. Upper before reset33.79/36.26, after reset37.17/36.53. Unequal
manual workloads, no consistent late-ferry win. Retain rear pointers as a small
work reduction, NOT a measured FPS success. All four long intervals in EACH
run are in reset_and_next: four times8TODfields, two simulation resets. No
three-plus elsewhere; zero/two phase caveat and no ownership counter remain.

User explicitly says continue. New isolated SPARKPAW_DROWNED_RESET_BLIT uses
existing bounded canonical DMA copier only for >=512px runtime camera jumps.
Startup remains CPU. Full208rows, fourplanes, allthree physical ring copies,
waits and exact water/collectible history retained. No new allocations. Same
159744destinationbytes per target,12/24blits; Blitter rereads source percopy,
106496more source bytes than CPU path. Speed advantage UNMEASURED. This targets
reset pauses, not ordinary late-ferry performance. Default full target does
NOT enable this new flag pending user gate.

ASan/UBSan actual oldCPU/newDMA/independent pixel oracle:1734resets, every
valid16pxorigin over5120world, both buffers/directions, guards andhistory.
Existing1788short-columncases pass. Actual counters/flush tests pass; native
fourbuilds pass. New A-plain is byte-identical to preceding B-plain. Other
inspected translation units identical A/B; native reset branch calls DMA,
startup path retained, mask writes remain safe. New B-plain +132bytes, noBSS
orChipbuffer growth. Proofs/assembly: build/drowned-fps-reset/.

ACTIVE: dist/Drowned-Reset-A-020-HD and Drowned-Reset-B-020-HD, executable
Drowned-Test. ACPUreset/BBlitterreset; both priorrear pointers/musicv5, same
TODobserver/seed,74assets/56refs;68alpha.8files unchanged. Played FPS pair
and complete logs archived intact at dist/older-builds/20260922-fps-stage-run1/.
Reproduce: make drowned-fps-reset (prepared current full assets required).
Manual020: checkpoint, LAST ferry upperledges nearfarbank,15secjump/shoot,
deliberatewaterfall, observe respawn pause,10secplay afterreturn, inspect
water/diamonds/enemies/scrollstrips. LMBrelease/wait15sec/frozenimage/reset.
No finale replay needed. Ordinary ferry sync/margin remains separate research.
No emulator launch, release, commit or push. Detailed report:
sparkpaw/docs/DROWNED_FULL_FPS_ROUND.md. Await user's reset/image/audio/log gate.

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
