# Drowned Turbines — ontwerp- en implementatieplan

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


## 2026-09-16 — Precision geyser housing correction

User test:48.14FPS,218/5664 two-field intervals(3.85%),zero longer intervals.
Worst composition camera656,one Crab/four pickups; no cause for all misses
inferred. Evidence preserved under testresults/Unassigned-Drowned-precision-020-48-14fps
and floating-nozzle-1/2 PNG/TXT. User reports floating nozzle and requests no
diamond there. Cause: compact64px jet patch includes only top nozzle row;
remaining five static rows were missing from the extended route foreground.
Generator now places the complete approved32x6 housing at1792,170, directly
meeting platform top176. Exact nontransparent pixel parity with original
nozzle at448,194 checked. Animation and hazard coordinates unchanged. Removed
coin1800,148;26diamonds remain. Native build and real player/AI route tests pass.
Updated dist/Drowned-Precision-020-HD/Drowned-Test via official staging;
previous drawer/log archived automatically.66assets/47refs,68release hashes
preserved. Proof:build/drowned-route/proof-precision-foot.json. No new performance
claim, no release; user visual confirmation pending.


## 2026-09-16 — Extended precision route with timed geyser (test pending)

User requests longer precision platforming, low/high/higher/low/high/low, and
explicitly wants the geyser to occupy the whole narrow landing rather than a
safe waiting half. Route now 2400px, ten 80px water strips, six precision
supports at x1456/1568/1680/1792/1904/2016, tops176/144/112/176/144/176,
all32px wide. Final bank starts2128. Final Crab moved to2256; earlier encounters
and four Walker traversal links unchanged. 27 diamonds. Existing materials
extended for tall supports; no new concept raster, palette or animation family.

Third dynamic patch reuses the existing 32x64 geyser at1792,107, with translated
hazard at1804..1811,y128..167. Shares the existing200-tick cycle and atlas/stage;
o new effect cache. Wait on the preceding high support, descend during quiet
phase and jump onward. Active135..184, warning100..129, recovery185..199.
Real-physics host landing proofs:41/39/39/63/39/55/53 ticks. Releasing horizontal
input brakes the long descent; held-right overshoots the geyser support.
The inlet nozzle remains narrow, but the ordinary player body standing over
this32px support intersects the erupting hazard. No artificial full-width hitbox.
Timing/readability/difficulty and native FPS still require user evaluation.

Resident Walker optimization and diamond restoration retained. Front allocation
increases49920 Chip bytes for480px extra width; actual free memory pending log.
Native build, sanitized route/AI/jet boundary tests, diamond restore and resident
cache parity tests passed. Staged66 assets/47 references; only route art/map
assets change, all68 alpha.8 release files preserved. Current drawer:
dist/Drowned-Precision-020-HD/Drowned-Test. Previous tested resident drawer and
log archived intact under dist/older-builds/20260916-before-precision-route.
Proof:build/drowned-route/proof-precision.json. No release or version bump.


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

## Route2 fixes partial pickup remnants — 16 September 2026

User reports FPS dips and half pickups remaining after collection on narrow
supports. Screenshot cataloged byte-identically with sidecar at testresults/
Unassigned-Drowned-route1-partial-pickup-remnants.png. Original clipboard file
retained. Shows right halves of cyan diamonds,not a second collectible state.
Cause: route coins19/21 atx1464/1576 are8px off16px word boundaries. Active
ring restore used16px except legacy hardcoded index28,leaving the right8px.
Renderer now uses compiled drownedCollectibleRestoreWidth(x) under ROUTE in
the default non-canonical-reference ring path:16px aligned,32px otherwise.
Positions/art/collection/score unchanged. Same Blit count,no new cache/buffer;
unaligned restores transfer one additional word per row. Legacy Level1 and
its canonical-reference paths retain their existing contracts.
New test_drowned_collectible_restore.py compiles the actual width helper and
uses actual diamond mask against pixel truth for16 alignments,hover extremes,
ring positions and two target histories:896 pass;old16px fails784 cases.
Registered make test. Native build passes;66 staged assets identical to v1.

V1 log preserved with sidecar:Unassigned-Drowned-route1-020-cadence-49-10fps.log.
Candidate proof verified,complete footer.3309 intervals:3249 one/60 two/0
three-plus,max2;49.10FPS,1.813% late20ms intervals,ownership0.56 compositor
crossings not ring rolls. Worst compose542 camera628 (before new precision
section) has1Walker/3collectibles/1water update; no hotspot causality proven.
Prepared freeChip845128/largest843968,Fast5305800/largest5303336. User-stated
FS-UAE HD68020/2MBChip/8MBFast,config not freshly inspected. FPS dips remain
an open concern; no unrelated optimization or speedup claim in this fix.

Active:Drowned-Route2-020-HD/Drowned-Test. Collect both platform diamonds,
scroll away/back and save cadence with left mouse+15s frozen hold. New native
visual/cadence acceptance pending. V1 drawer/log/metadata intact under
older-builds/20260916-route1-pickup-remnants. Proof:build/drowned-route/proof-v2.json.
68 alpha.8 release hashes unchanged. No release,commit or auto-emulator run.

## Drowned 1920px route v1 staged — 16 September 2026

User authorizes expansion and suggests precision platforms over repeated water,
or later a raft crossing with flying enemies. Implemented first precision
section now; raft/carry physics/flying family are explicitly deferred, not
implemented. Active `sparkpaw/dist/Drowned-Route1-020-HD/Drowned-Test`.
Current route supersedes the earlier proposed segment ordering: preserve the
known first960px/gate/geyser composition, then extend behind the gate.
864..1088: raised maintenance deck with a Walker jumping up/down40px.
1088..1360: Walker crossing an80px water gap both ways.
1376..1680: three80px water gaps with32px support islands; two authored tops
at176 and160; dry landing from1680. Last Crab/recovery strip ends at1920.
Six spawn sites total (three Walkers/three Crabs),26 diamonds. Usually sparse;
actual host camera tests see peak3 active enemies. No final Core/level ending,
new music or raft/flying enemies in this intermediate route.

Single layout source assets/levels/drowned-route-v1.json generates geometry,
collision and C route/coin tables via tools/build_drowned_route.py. Target
`make drowned-route PYTHON=../.venv/bin/python3`, compile-only
SPARKPAW_DROWNED_ROUTE and WORLD_W1920. Water count6; existing80px animation
cache reused and existing viewport culling retained. Existing gate visibility,
head-shot fix, copper projectile/audio, HUD, palette, mechanisms and enemy art
retained. Foreground +99,840 Chip bytes; collision data +840 bytes; these are
specific deltas, not a measured native free-memory total. Rear/mechanism/enemy/
shot asset bytes identical to previous candidate. Ring targets remain fixed.
Potential extra water/Bob load in busy views requires native020 measurement.

Actual player physics tests prove both narrow ledge landings, far bank and
raised deck. Actual enemies/collision tests cover all4 traversal links over8
seeds with no failed landings, valid poses/height and supporting floor/body
clearance probes; peak3 active. Full `make test` passes. Native build passes;
final diagnostic-only route identity line added and native rebuilt afterwards.
No automatic emulator or real-hardware claim. Layout still review image at
build/drowned-route/route-layout.png omits dynamic water/enemies/HUD; not an
emulator screenshot. Asset planar roundtrip and package hashes verified.

66 packaged assets/47 literal references;68 alpha.8 files unchanged. Previous
GateView drawer plus any log/launcher metadata archived byte-identically at
older-builds/20260916-before-route1. Proof:build/drowned-route/proof.json.
Test2–3min,especially BEYOND gate: Walker climb/water routes,precision section,
combat and backtracking. Left mouse once,wait15s frozen save hold,stop/reset.
Native reachability/feel/readability/FPS and full-route memory remain pending.
No release/version/commit change. Next raft concept should add a separate
moving-platform/carry proof and reviewed flying enemy before route integration.

## Gate visibility fix and next route scope — 16 September 2026

User: little jumps look okay; wants higher platforms/water traversal and asks
whether it is time for a wider level/more platforms/enemies. Reports gate can
be opened before visible. Current short hop remains an animation audition.
Next proposed scope:1920px playable route (not the final whole level), roughly
0..384 arrival,384..800 raised platforms,800..1184 authored water crossing,
1184..1600 water/geyser encounter,1600..1920 gate/exit. Aim6 spawn locations,
usually1–2 visible enemies; validate an intentional heavier overlap separately.
Walker links must use actual landing/support geometry, both directions where
appropriate, and pass host trajectory/blocked-destination/offscreen/respawn
checks. Preserve artwork/HUD/020 budget; new art concept first if required.
This expanded layout is proposed, NOT implemented in current960px build.

Gate fix implemented: drownedSetView(cameraX) caches whether panel768..783
fits viewport with8px reading margin; both direct-hit and sweep reject when
invisible. Reset disables it until game publishes view. One update per game
tick, no per-pixel camera lookup. Tests cover early/offscreen,partial and exact
visibility boundary,both projectile directions,normal activation and reset.
Actual slice physics/projectile test and6000tick enemy variants pass; native
build passes. Current candidate Drowned-GateView-020-HD/Drowned-Test retains
short hop, NOT expanded route.66 assets/47 refs,68 release files unchanged.
Jump drawer/log/metadata archived intact:older-builds/20260916-before-gate-view.
Proof:build/drowned-enemy-audit/proof-gate-view.json. New gate native test pending.

Jump log complete,hash-verified and preserved with sidecar under testresults/
Unassigned-Drowned-jump-020-cadence-49-64fps.log.1392 intervals:1382 one/10 two/
0 three-plus,max2;49.64FPS,0.718% late,ownership0.11 compose raster crossings,
not ring rolls. User-statedFS-UAE HD68020/2MBChip/8MBFast; no fresh config read.
Short positive motion/cadence result, not proof of full-level enemy headroom.
No release,commit or automatic emulator run.

## Local action audition v1 —15 September2026

User likes gait direction; new offline action study at
assets/enemies/drowned-actions-review-v1/scene-3x.gif and individual6x GIFs.
Generator:tools/preview_drowned_enemy_actions.py. Fixed detailed native parts,
no complete AI-generated sheet. Walker:24tick charge,6tick recoil,7 hit poses,
4x5tick forward knee collapse, sensor darkening and small pressure leak.
Crab:short hit and4-state shell fracture/leg fold. Copper/steel roles retained.
Palette checked (leak highlight corrected to actual level pen11). No runtime,
projectiles,damage,respawn or cache/slot edits. Idle/wreck holds and loop reset
are review staging only, no gameplay commitment. Existing packages and68
releases verified unchanged. Contact sheets inspected; motion quality pending
user review. Turn/jump/rotor and complete native family audit remain before
integration. No hardware/performance claim. Gait proof retained unchanged.

## Local gait audition v1 —15 September2026

User needs motion to judge native art. New offline fixed-part8-frame loops:
assets/enemies/drowned-walk-review-v1/scene-3x.gif (also1x), individual6x GIFs,
RGBA sheets and4x contacts. Generator:tools/preview_drowned_enemy_walk.py.
Uses detailed pixels from native idle masters, no generated full-sheet anatomy
or schematic replacement legs. Walker thigh/shin/foot parts use fixed-length
2-link joints and planted flat feet; scene travel matches stance displacement.
Crab uses four depth-ordered leg parts and unchanged shell. No new colours;
nearest-neighbour transforms checked against master colours. No runtime/dist
changes; three previous test packages and68 releases hash-verified unchanged.
This is a gait audition, not finished animation acceptance: small-scale joins,
foot contact, body rigidity and crab leg visibility require user motion review.
Stop-and-mirror is merely facing-test staging, NOT an authored final turn.
Rotor, attacks/hit/death/jump loops not implemented here. Review gait first,
then author complete readable family; do not integrate incomplete glide/turn.
No FPS or hardware claim for illustrative GIF timing. User review pending.

## Native idle proof v1 —15 September2026

User approved copper-crab/cool-walker colour direction and asks continuation.
Generated isolated two-idle source via built-in ImageGen, saved as
assets/enemies/drowned-enemies-idle-source-v1.png (real alpha channel; RGB
background tint under transparent pixels is not opaque scenery). Exact prompt
sibling TXT. Native offline converter:tools/prepare_drowned_enemy_idle.py.
Outputs:assets/enemies/drowned-native-idle-v1/ (indexed/RGBA PNGs,1x and
nearest-neighbour scale/scene proofs,manifest with source bounds and pen counts).
Only alpha>=128 defines cropping bounds, avoiding faint halo widening and
silently shrinking bodies. Uniform aspect scale per idle; no per-frame fits.
Crab32x24 opaque bbox[1,6,31,23];walker64x64 bbox[8,5,56,63].
Existing Drowned12bit palette used; material-aware allowed pens avoid player
orange2/3. Small sensor/leg readability remains a native review question.
Scene uses current front/rear sources, original48x48 Sparkpaw frame and HUD;
static composition only, not emulator capture or accepted animation.
User native-size review pending. Do not integrate or animate until native
master approval; then refine fixed parts and local walk+turn loop. Source
concept is not a topology-proof rig; no death/jump family generated here.
All3 encounter/Opt5 play packages and68 release hashes checked unchanged.
No runtime,cache,physics,release or performance change.

## Drowned enemy art concept v1 —15 September2026

User explicitly requests enemy-design lessons, anatomy rules, premium AGA
pixelart and good animations/deaths. Read relevant skills and later Strider
history: no repeated whole generated sheets; lock native master/parts then
review local gait+turn/death loops. Built-in concept source preserved at
sparkpaw/assets/enemies/drowned-enemies-concept-v1.png; exact prompt sibling
TXT plus IMAGEGEN_PROMPTS entry. See docs/DROWNED_ENEMY_ART_PLAN.md.
Concept shows turbine crab and biped pumpwalker; user review pending. No
runtime/dist changes. Sheet violates requested flat background/relative scale
and rotor blade count; not a native sprite proof or approved animation sheet.
Next after design review:32x24/64x64 native masters, actual-size Sparkpaw/
scene comparison, then stable anatomy gait+turn and death with material-role
checks. All current Opt5/encounter builds remain unchanged.

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

## Opt3 retained; broader engine audit and events — 14 September 2026

User explicitly requests retaining Opt3 and reports smoother feel; retain it
without claiming proven gain. User also requests broader engine/renderer and
compiled-code audit. See docs/DROWNED_020_ENGINE_AUDIT.md for findings.
CORRECTION: log `wraps` counts compositor raster crossings, NOT ring rolls.
Earlier notes calling it ring wraps were inaccurate. Do not infer scrolling
causation from that aggregate. New eight-bucket event diagnostic correlates
actual ring-column/water/raster flags of preceding update with its interval.
No extra timer reads or broad scopes; small counting overhead remains.
Active manual test: dist/Drowned-Events-020-HD/Drowned-Events. Same Opt3
runtime/art; rebuilt minimal executable is byte-identical to played Opt3.
Compiler audit: gate collision calls frame/overlap routines before distant-x
rejection; saves five registers and frame uses32bit division when open.
Next bounded optimization hypothesis is early spatial/state rejection;
not yet implemented or measured. Geiser division lower priority.
Actual five translation units compiled to build/drowned-slice/audit-*.asm;
final linked address/cache-placement attribution still requires proof.
63 assets/47 references verified;68 release files unchanged. Opt3 including
log/metadata archived intact in older-builds/20260914-before-drowned-events/.
Next user run60-90s:~10s stationary near active geyser, then move through
water/geyser/gate, jumps/shots; no pause; LMB release then15s save/hold.
Event data will separate raster boundary crossings from actual ring updates.

## Opt3 020 result — 14 September 2026

Complete verified user log:2734 intervals,2685 one-field,49 two-field,
0 three-plus;49.11fps,51wraps,0 ownership violations. Sample55.66s;
long interval share1.79%. Opt2:49.07fps,40/2126long(1.88%),43.32s,40wraps.
Difference0.04fps is not convincing gain across manual workloads. Opt3
remains candidate, not performance-accepted. No further runtime edit this review.
Worst aggregate frame0/camera0; do not infer a moving spike from this entry.
Similar wrap/long counts are only an investigation lead, not causal proof.
Original log retained; exact copy/TXT:
`testresults/Unassigned-Drowned-Opt3-020-cadence-49fps.log`.
Executable/assets verified against opt3-package-proof.json;68 release files
unchanged. Complete memory footer:prepared Chip963640/largest962392,
Fast5367224/largest5365800;post-run Fast5744520. Lifecycle snapshots only.
Active drawer remains dist/Drowned-Opt3-020-HD. Next investigate individual
roll events versus long frames and game-update costs before another copy
micro-optimization. Stable50Hz/enemy headroom still unproven.

## Drowned Opt3 water transfer candidate — 14 September 2026

Active manual020 test: `sparkpaw/dist/Drowned-Opt3-020-HD/Drowned-Opt`.
Single change from played Opt2: Drowned canonical water-to-target sync now
uses the same fixed-stride rectangle copier as the mechanisms. Original
phase tracking, visibility, 80x11 pixels at y197, clipping, wrapping and
three physical copies retained. No asset/animation/allocation change.
Compile-guarded to Drowned canonical restore; campaign paths unchanged.
Normal campaign executable rebuilt byte-identical to pre-change binary.
Minimal cadence instrumentation unchanged. Full host suite including27840
actual-C ASan/UBSan rectangle cases (water dimensions and edge clipping)
and native candidate/production builds pass. No emulator self-test.
63 runtime assets/47 embedded references verified; all assets match Opt2;
68 alpha.8 files unchanged. Opt2 including full user log/metadata archived
intact under dist/older-builds/20260914-before-drowned-opt3/.
Proof: build/drowned-slice/opt3-package-proof.json.
Prior targeted water sync avg1.83ms motivates this hypothesis, not a gain
claim. Compare new minimal cadence to Opt2 49.07fps,40/2126 long intervals,
43.32s/40wraps; account for manual workload differences. Next user60-90s
water/geyser/gate with camera movement/jumps/shots, no pause, LMB release
then15s save/hold. Sustained50Hz and enemy headroom remain unproven.

## Opt2 020 result — 14 September 2026

User completed Opt2. Complete verified log:2126 intervals,2086 one-field,
40 two-field,0 three-plus;49.07fps,0 ownership violations. Long intervals
1.88% versus Opt1 3.55%. Sample43.32s/40 wraps versus21.58s/37 wraps.
Manual workloads differ; lower long-frame share is encouraging, not a
controlled attribution of gain to the kernel. Absolute long intervals
40 versus37 remain similar with similar wrap totals. Do not infer causal
correlation from aggregate counts. Worst frame686,camera433.
Prepared free Chip964344/largest962784,Fast5367384/largest5366104;
post-run Fast5744416. Complete footer; lifecycle snapshots only.
Executable/assets match opt2-package-proof.json;68 alpha.8 files unchanged.
Original log kept; copy plus TXT at
`testresults/Unassigned-Drowned-Opt2-020-cadence-49fps.log`.
Active drawer remains `dist/Drowned-Opt2-020-HD`. No runtime changes this review.
Next target: water target sync and remaining camera433 spike; prior targeted
water sync avg1.83ms. Stable50Hz and spare enemy capacity remain unproven.

## Drowned Opt2 candidate and profile result — 13 September 2026

Active test: `sparkpaw/dist/Drowned-Opt2-020-HD/Drowned-Opt`.
User completed targeted profile:1104 intervals,1063 one-field,41 two-field,
0 three-plus,48.20fps,0 ownership violations; complete log/RAM footer.
Preserved in testresults/Unassigned-Drowned-020-targeted-profile.log +TXT.
Profile observer overhead prevents direct Opt1 speed comparison.
Mechanism target sync max5358 CIA ticks(~7.55ms),p953958(~5.58ms);
water target sync avg1300(~1.83ms),p952262(~3.19ms). Canonical water
avg152(~0.21ms). Game update avg3015(~4.25ms) also deserves follow-up.
Aggregate maxima need not occur together; no frame-level causal proof.

Opt2 hypothesis: dedicated Drowned canonical rectangle copy advances row
pointers instead of rebuilding addresses every row. Preserves all three
physical ring copies, clipping, wrapping, exact source pixels and target
phase bookkeeping. No asset, animation, gameplay or allocation changes.
Only mechanism target sync changed; water/game optimizations deferred to
avoid combining hypotheses. Seven timing scopes disabled: original minimal
cadence configuration restored. Actual C ASan/UBSan oracle covers27840
rectangles/four planes/three copies; full host suite and native candidate
build pass. Compiler assembly retained in build/drowned-slice/opt2-renderer.asm.
No automatic emulator run; FPS gain/50Hz/enemy headroom still unproven.
63 assets/47 literal references verified, assets byte-identical to profile,
all68 alpha.8 release files unchanged. Full profile drawer/log archived intact
under dist/older-builds/20260913-before-drowned-opt2/.
Next manual020 run60-90s through geyser/water/gate, jump/shoot, no pause;
LMB press/release then~15s save/hold. Compare cautiously with Opt1 48.28fps.

## Targeted 020 profile — 13 September 2026

Active manual test: `sparkpaw/dist/Drowned-Profile-020-HD/Drowned-Profile`.
Seven disjoint CIA scopes: game update, sprite stage, canonical water,
canonical mechanism patches, ring roll, dynamic water sync, mechanism sync.
Existing broad/nested and wait profiling remain disabled. Profile flag and
scope count explicitly logged; cadence includes observer overhead and must
not be treated as an Opt1 speed comparison. This is diagnosis, not Opt2.
All visuals/assets/gameplay unchanged. Rebuilt minimal cadence executable
is byte-identical to the played Opt1 executable. Full host suite and native
profile/minimal builds pass. No automatic emulator test. Two extra profile
slots add 8192 sample bytes plus their small counters only in this profile.
63 runtime assets/47 executable references verified; all68 alpha.8 release
files unchanged. Opt1 drawer, complete user log and metadata archived intact
under `dist/older-builds/20260913-before-drowned-profile/`.
Package hashes: `build/drowned-slice/profile-package-proof.json`.
Next: manual60-90s on68020 real/cycle-exact,JIT0,2MBChip+8MBFast through
water/geyser/gate, no pause; LMB release then15s save/hold. Read seven scope
median/p95/max values before choosing the next runtime optimization; validate
its gain separately with minimal cadence. Sustained50Hz/enemy margin pending.


Datum: 13 september 2026. Status: eerste geïsoleerde omgevings-/mechanismetestslice
gebouwd en verpakt voor gebruikersreview op FS-UAE/68030. Ontwerp/native richting
geaccepteerd; native visuals en gameplay nog niet geaccepteerd. Geen release,
versiewijziging, commit of push.

## Opt1 gemeten — 13 september 2026

48,28fps tegenover44,26 eerder;37/1042 lange intervallen(3,55%),geen3-field
intervallen,0 ownership violations. Gebruiker ervaart verbetering. Complete
log en RAM-footer behouden in testresults/Unassigned-Drowned-Opt1-020-cadence-48fps.log.
Handmatig workloadverschil:21,58s/37wraps versus28,58s/18wraps,geen gecontroleerde
A/B-winstclaim. Nog geen vaste50Hz of bewezen ruimte voor enemies. Volgende
vraag is de oorzaak van resterende37 lange intervallen gericht meten.

## Opt1 — compact kopiëren, 13 september 2026

Actuele test `dist/Drowned-Opt1-020-HD/Drowned-Opt`. Geiser32×64 en poort80×61
vervangen96×96-overdrachten, met pixelgelijke pixels en dezelfde timing.
Fast atlas105984→43376 bytes; Chip stage1152→610. Alle277 overgangen en echte
C-overdrachten getest; volledige hostsuite/native builds slagen. FPS-winst
nog meten. Compacte loguitvoer behoudt geheugenfooter; oude meting behouden.
Verdere optimalisatie op basis van die meting. Enemy-headroom blijft apart te
bewijzen:50fps in deze gedeeltelijke omgevingsslice is daarvoor onvoldoende.

## Eerste 020-meting — 13 september 2026

44,26 fps over1265 intervallen (~28,58s):1105×20ms,156×40ms,4×60ms.
0 ownership violations; volledige samenvatting, detailtrace/opslag afgebroken.
Geen RAM-footer. Bewijs behouden met sidecar onder testresults als
`Unassigned-Drowned-Slice4-020-cadence-44fps-partial.log`. Performancegate niet
gehaald; oorzaak nog niet vastgesteld. CPU-configuratie/zichtbare locaties
uitgevraagd. Volgende analyse richt zich op lokale96x96-patchkosten, ringwerk,
update en sprite-stage; geen grafische versobering of nieuwe levelinhoud.

## Eerst 68020 meten — 13 september 2026

Slice4-presentatie geaccepteerd; gebruiker ziet drops op 020. Verdere uitbreiding
pauzeert voor de performancegate. Actueel: `dist/Drowned-Cadence-020-HD`, start
`Drowned-Cadence`. Identieke art/gameplay, minimale bestaande cadence-instrumentatie;
geen optimalisaties. Na 60-90s bewegen LMB indrukken/loslaten, beeld blijft staan,
~10s wachten en emulator stoppen/resetten. Log plus CPU-instellingen en locatie
van drops beoordelen voordat een gerichte profiler/A-B-wijziging wordt gekozen.

## Slice 4 — solide bovenkast, 13 september 2026

Goedgekeurde poortkast geïntegreerd met permanente botsing en lokale zichtbare
hoofdruimtecontrole. Geiser/route/HUD blijven gelijk. Actuele test:
`dist/Drowned-Slice4-030-HD/Drowned-Slice`. Builds/hostsuite slagen; 030-review
volgt. Zie [poortreview](DROWNED_GATE_SOLIDITY_REVIEW.md). Slice3 gearchiveerd.

## Slice 3 — geiser en sluisdiepte, 13 september 2026

Goedgekeurde geiserrichting geïntegreerd als acht native frames, met verzonken
ijzeren rand en borrelende waarschuwing. De rechter sluisstaander valt vóór
Sparkpaw; de linker blijft achter hem. Damage/collision/route/HUD blijven gelijk.
Actuele test: `dist/Drowned-Slice3-030-HD/Drowned-Slice`. Slice2 intact gearchiveerd.
Native/normal builds en volledige hostsuite slagen; gebruikersreview 030 volgt,
daarna 020-metingen. Geen release. Zie [review en vervolg](DROWNED_SLICE2_REVIEW.md).

## Slice 2 — goedgekeurde art geïntegreerd, 13 september 2026

De gebruiker accepteert native foreground, jetpreview en sluisopening en
vraagt verder te gaan. Actuele native test: `dist/Drowned-Slice2-030-HD`,
executable `Drowned-Slice`. Dit vervangt Slice1 voor review. Eén coherente
omgevings-/mechanismetestslice; eigen enemies/music en full campaign blijven
latere stappen. Geen release/version/commit/push.

Nieuwe voorgrond uit de goedgekeurde indexed art, met dezelfde twee 40px
platformrises, watergaten en playerphysics. Eerste cap uitgebreid met hetzelfde
materiaal tot de bestaande 96px collisionbreedte. Het voorgestelde palet is
naar de werkelijk gebruikte 12-bit Copperwaarden gebracht; sourcepens 0..7
behouden hun oude rollen. Koude paletselectie initialiseert zowel buildCopper
als de bestaande per-frame frontColor writes, die anders het nieuwe palet
zouden overschrijven. Player/HUD eigen banken blijven gelijk. Playerplasma
gebruikt 4/5/6; water 5/6/11, diamonds blijven cyan/lichte highlights. De
onbereikbare enemy/Core/hostile-families zijn nog niet goedgekeurd voor het
nieuwe koperpalet en mogen niet impliciet als gereed worden beschouwd.

Jet: 200 gameplayticks per cyclus; 100 rust, 30 borrelen, 5 opbouw, 50 actief,
15 herstel. Vier actieve waterframes op stappen van vier ticks. Centrale
kolom x460..467/y152..191 doet alleen in de actieve fase schade; buitenste
spatten, waarschuwing en verval blijven onschadelijk. Geen nieuwe physics.

Sluis: bevestigd schot, korte ontgrendeling, twaalf liftstanden; volledig open
bij gateTick 43. De botsingsonderkant volgt 197 minus de actuele lift, zodat
doorgang vrij wordt zodra Sparkpaws collisionbox past. Pixel-/tile-spanpariteit
ook op niet-16px-uitgelijnde onderkanten getest. Drempel en frame zijn vaste art.
Sluis en drukritme resetten bij life-loss; collectibleprogress blijft behouden.

23 complete lokale 96×96 patches (112..207 hoog), opgeslagen als 105.984 bytes
planes in Fast RAM. Eén 1152-byte Chip-plane-stage wordt seriëel hergebruikt:
WaitBlit → CPU-copy Fast naar stage → Blitter naar canonical. De Blitter krijgt
nooit een Fast-pointer. Daarna synchroniseren beide inactive-target histories
na de bestaande ringroll. Updates worden buiten de cameramarge overgeslagen;
binnenkomst haalt direct de actuele fase op. Geen per-frame allocatie.
Dit zijn concrete bron-/stagegroottes, geen gemeten totale geheugenvrijheid.
Native 020/50fps, Chip-piek en grootste vrije blok blijven open meetgates.

Verificatie: gewone native build en kandidaatbuild slagen. Volledige bestaande
hostsuite slaagt; nieuwe tests voeren werkelijke C voor traversal, projectielen,
gate-state, dynamische collision en jetdamage uit. Extra renderertransfertest
met AddressSanitizer/UBSan en een Blittermodel controleert alle 23 frames,
stagepointer/guards, canonical pixelgebieden, culling en beide targetstates.
Offline native-palette open/gesloten composities geïnspecteerd. Geen automatische
FS-UAE-run en geen claim dat de usertest al geslaagd is.

Stager: 63 runtimebestanden, 47 executable references; 68 alpha.8-releasefiles
hash-identiek behouden. Slice1 met eventuele sidecars/evidence intact naar
`dist/older-builds/20260913-before-drowned-slice2/`. De test-ReadMe beschrijft
030-acties, controls, drie levens, geen logger, geen Core/results/finale en
nog geen eigen muziek/enemies. Volgende stap: menselijke 030-review van art,
animatie, shots, passeren, terugscrollen, pauze en life-reset. Pas daarna 020.

## Sluisopening — interactieve artpreview, 13 september 2026

Gebruiker accepteert de jet-artpreview en vraagt verder te gaan. Volgende
review: `assets/concept/drowned-sluice-review-v1/index.html`, zelfstandig HTML
met embedded PNGs. Generator `tools/preview_drowned_sluice.py`, canvascompositor
`tools/drowned_sluice_review.html`. Bestaande goedgekeurde indexed sluisart;
geen nieuwe imagegen-bron of gewijzigde runtime-assets.

Knop/Spatie simuleert een treffer op de bediening, niet een echt projectiel.
180ms ontgrendeling, vervolgens twaalf standen met vaste intervallen; open
vanaf 840ms. Alleen het binnenste schot schuift onder een vaste clip naar boven;
frame, rolkast, zijgeleiders, leidingen en drempel blijven staan. Cyan indicator
blijft branden; herhaald activeren heeft geen effect tot Opnieuw/R. De opening
laat de achterplaat zien. Speler/HUD blijven vaste referentie. Geen doorgang-
collision, SFX, echte schoten of native FPS-claim in deze preview.

JavaScript-syntax en daadwerkelijk preview-statepad getest met Node/canvasmock:
laden, treffer, opening, blijvend open, dubbele activatie, reset en HUD-grens.
Daarna is de browserpreview geopend en de open stand visueel gecontroleerd:
vaste geleiders/frame, zichtbare achterplaat en cyan indicator aanwezig.
Geen Amiga-pixelbewijs. Sluisanimatieacceptatie staat open;
jet-art akkoord verandert de eerder open gameplay-/020-gates niet.
Geen nieuwe build of wijziging in dist. Na artacceptatie de goedgekeurde
voorgrond, jet en sluis als één samenhangende native kandidaat integreren,
inclusief paletcompatibiliteit, damage/collision en beide scrollbuffers.

## Drukjetanimatie — native artreview, 13 september 2026

De gebruiker accepteert de native foreground-/sluisreview en vraagt verder
te gaan. Acht nieuwe jetframes staan in `assets/concept/drowned-jet-native-v1/`:
`preview-1x.gif`, `preview-3x.gif`, `frames-4x.png`, indexed atlas en SPBM.
Reproduceren: `.venv/bin/python3 sparkpaw/tools/preview_drowned_jet.py`.
Alle cellen 32×64, één uniforme schaal voor de hele bronfamilie, vaste baseline.
Geen per-frame rescale. Pen 0 transparant; water gebruikt 5, 6 en 11 uit het
reviewpalet. Nozzle blijft één vast onderdeel uit de goedgekeurde conceptbron.

Beelden: klein borrelen, opbouw, vier actieve stroomfasen, inzakken en laatste
spatten. Preview toont rust, borrelwaarschuwing, opbouw, circa 12,5 updates/s
voor de actieve art en herstel. Dit staat los van 50fps gameplay/presentatie.
Geen claims over perfecte temporele loopsluiting of native performance uit
alleen een GIF. Player staat iets verder rechts als schaalreferentie, met exact
dezelfde spritecel. HUD is pixel-identiek; achtergrond/sluis staan stil.

Ruwe atlasplanes: 8192 bytes; eventuele masks 2048 bytes. Twee losse
frame-stages zouden 2048 bytes planes vragen, exclusief masks, guards en
restores. Dit zijn rekenwaarden, geen gemeten Chip-allocaties. Integratie mag
geen full-screen GIF-frames als runtimecache gebruiken: alleen lokale jetframes.
Schadebox, warning/active-grens en passeerbaarheid moeten bij gameplayintegratie
tegen de goedgekeurde art worden afgestemd en getest. De huidige bron is hoger
uitgevallen dan de oude 48px jet; dat is geen impliciet goedgekeurde hazardwijziging.

SPBM-indexroundtrip en HUD-invariant gecontroleerd, native frames visueel
geïnspecteerd. Geen runtime-C, build, release of distwijziging in deze stap.
Animatieacceptatie staat open. Geaccepteerde native sluisart betekent nog geen
acceptatie van een sluisopeningsanimatie; die volgt afzonderlijk.

## Native foreground review — 13 september 2026

De gebruiker accepteert `drowned-polish-direction-v1.png` als richting.
Vervolg: `assets/concept/drowned-polish-native-v1/preview-1x.png` (320×256)
en `preview-4x.png` (exact 4× nearest-neighbor). Afzonderlijke FRONT16/REAR8
SPBMs en indexed PNGs; manifest met paletten en rekenwaarden. Reproduceren:
`.venv/bin/python3 sparkpaw/tools/preview_drowned_polish.py` vanaf root.

De nieuwe foregroundbron is met imagegen uit de goedgekeurde richting gemaakt.
De gegenereerde layout klopte niet met de native maatvoering; daarom zijn
platform, complete sluisconstructie en vloercaps afzonderlijk geschaald.
Platformtop y160, vloer y200, HUD y208. Speler exact bestaande 48×48-framecel,
HUD ongewijzigde atlascompositie. Eén wateropening van 80px. Dit is een
reviewcompositie, geen vervanging van de volledige slice-map/collision.
De sluis heeft nu zichtbaar frame, rolmechanisme, lamellen, koperleiding en
aangebouwde amber bediening. Drukjetanimatie blijft de volgende artstap.

Voorgesteld foregroundpalet houdt pens 0..7 vast, maar gebruikt 8..15 voor
koel staal en koper. Daardoor is deze art nog niet drop-in compatible met alle
shared Bobs/effecten: die audit hoort vóór integratie. Water gebruikt de
bestaande frameformule binnen het voorgestelde palet. Achtergrond is bewust
nog de bestaande native panorama (uitsnede), zonder nieuwe scherpteclaim.
Geen extra planes; deze twee enkele bitmaps samen 58.240 bytes raw planes,
niet het totale runtime-/cache-/RAM-budget. 020/50fps niet gemeten.

SPBM pixel/palet-roundtrips en onveranderde HUD gecontroleerd. Runtime- en
distbestanden vóór/na de generator gehasht en identiek. Native preview visueel
geïnspecteerd. Geen C-wijzigingen, build of release. Gebruikersreview van deze
native art staat open; bronconceptakkoord is geen runtime-acceptatie.

## Nieuwe artlat en gebruikersreview — 13 september 2026

De gebruiker wijst de grafische uitvoering van Slice1 af: korrelige/herhaalde
voorgrond, onduidelijke sluis en te statische drukjet. Het gameplayidee spreekt
wel aan. De screenshot toont cyan paneel en opgetrokken schot, passend bij de
open toestand; de voorafgaande schotrespons is daarmee niet bewezen.

Nieuwe expliciete prioriteit: maak Drowned zo mooi mogelijk qua pixelart,
animatie en AGA-afwerking. Level 1 is geen grafisch plafond; een eventuele
Level-1-restyle is later werk, geen uitbreiding van de huidige scope.
ADF-capaciteit beperkt de artkeuzes nu niet. Eerdere twee-/drie-diskbudgetten
blijven historische metingen voor latere packaging, geen actuele artgate.
Doelhardware blijft 68020, 50 fps, 2 MB Chip en 8 MB Fast. Dit is een te bewijzen
doel, geen claim dat de nieuwe art of animatie al binnen budget draait.

Eerst betere conceptart, daarna een eerlijke native review met dezelfde speler,
HUD vanaf y208 en vloer y200. Geen nieuwe resolutie nodig voor animatie.
Scherpte komt eerst uit ontworpen pixelclusters, duidelijke contouren,
waardescheiding en selectieve details; geen generieke downscale/quantisatie
als eindproduct. Bestaande 4+3 blijft de werkbasis. Meer bitplanes vergt een
apart gemeten experiment en wordt niet impliciet toegezegd.

Nieuwe materiaal-/vormstudie: `assets/concept/drowned-polish-direction-v1.png`,
via built-in imagegen, exact prompt in IMAGEGEN_PROMPTS. Review pending.
Sluis met frame/geleiders/rolkast en verbonden bediening; drukmond; staalcaps
met stevige beugels; rustiger verre installatie. De gegenereerde platformhoogte,
vloerdikte en detaildichtheid zijn geen goedgekeurde gameplaygeometrie en passen
niet zonder herontwerp op 320×208. Geen runtimewijziging in deze conceptstap.

Animatievoorstel voor later: lokale prebuilt jetframes met opbouw, bewegende
uitstoot en verval; echte stapsgewijze sluisbeweging met duidelijke openstand.
Daarna hooguit één kleine achtergrondwaterval als eerste levend accent.
Geen volledige geanimeerde rearbitmap of brede regenlaag als startpunt.
Gedeelde rearbitmap kan DMA-zichtbaar zijn: vóór rearanimatie expliciet veilige
update-/bufferownership vastleggen, niet zomaar de frontpatchroute kopiëren.
Meet Chip-piek, grootste vrije blok en 020-cadence met echte gameplaybelasting.
Nieuwe art wordt vóór runtime-integratie beoordeeld; de huidige test is geen
visueel geaccepteerde basis om verder uit te bouwen.

## Eerste speelbare omgevingstest — 13 september 2026

Actieve test: `dist/Drowned-Slice1-030-HD`, executable `Drowned-Slice`.
Start direct in een 960px route met dezelfde Sparkpaw, HUD, physics, water en
rolling AGA-renderer. Twee 80px watergaten (240/608), bereikbare 40px verhoogde
caps, twaalf diamonds, één drukjet en één schietbare sluisregelaar.
De volledige 1120px rearbron blijft als toekomstig doorlopend master gebruikt;
de korte route toont vanzelfsprekend alleen het eerste deel daarvan.

Dit is een vroege deelgate vóór fase 3: géén volledige representatieve slice.
Eigen enemyart, enemyplaatsing, muziekloop, definitieve effectart, weerstation,
Rain Core en finale ontbreken nog. Drukjet/sluis zijn eenvoudige lokale
mechanismeblockouts bij de goedgekeurde conceptuele richting; hun presentatie
is niet als definitieve productieart geaccepteerd. Geen vervangende Core of
verzonnen level-complete aan het einde. Rechts kan de speler teruglopen.
Escape en terminale nederlaag verlaten deze gefocuste test; opnieuw starten
geeft een verse poging. P pauzeert volgens het bestaande contract.

`SPARKPAW_DROWNED_SLICE` selecteert deze kandidaat bij compilatie. De gewone
campagne bevat geen nieuwe per-frame sectionchecks. `drowned_slice.c` bezit
eigen water-/spawngegevens (nul enemyspawns), warning/damage/sluisstate en
projectile-panelhit. De bestaande geoptimaliseerde projectsweep kiest de
paneelhit vóór de verderop gelegen solide sluis. De sluis blijft gedurende
24 ticks fysiek dicht en wordt daarna volledig vrij; life-reset sluit hem.
Drukcyclus: 100 veilig, 30 waarschuwing, 50 actief, uitgedrukt in gameplayticks,
niet gegarandeerde milliseconden. Schade gebruikt bestaande invulnerability.

Alleen kandidaatbestanden onder `build/drowned-slice/assets`: eigen front,
collision, achterplaat en 64×480×4 atlas met zes volledige lokale 64×80 patches.
Geen overschrijving van storm-assets. De atlas vraagt 15.360 bytes Chip-planes;
metadata/padding komen erbij. Canonical patches worden na Bob-restores bijgewerkt,
en daarna per inactive target gesynchroniseerd na de gewone ringroll. Geen
allocatie in de frameloop en geen writes naar de actieve displaytarget.
Copper-waits blijven gelijk; de nieuwe vaste REAR8 wordt in de bestaande
paletteslots geprogrammeerd. Geheugenpiek, performance en hardwareweergave zijn
nog niet gemeten. De algemene enemycaches worden voorlopig nog voorbereid;
cache-/assetownershipoptimalisatie is een latere gate.

Verificatie: native kandidaat én gewone `make` slagen; volledige bestaande
`make test`-suite slaagt. Nieuwe `tests/test_drowned_slice.py` compileert en
voert werkelijke playerphysics, collision, projectiles en mechanisme-C uit:
beide wateroversteken/platform bereikbaar, schot opent sluis, doorgang vóór/na
opening, reset, waarschuwing-/damagegrenzen en sweeps vanuit beide richtingen.
Dit vervangt geen visuele emulator-/hardwaretest. Geen automatische FS-UAE-run.

Verpakt met `tools/stage_hd_test.py`: 63 runtimebestanden gecontroleerd,
47 literal executable assetreferences ontdekt, 68 alpha.8-releasebestanden
byte-identiek behouden. De vorige SpriteGuard-test is intact gearchiveerd naar
`dist/older-builds/20260913-before-drowned-slice/`. De gewone release blijft
alpha.8; fysieke floppy-spritefix en HD/WHDLoad zijn volgens gebruikersrapport
geaccepteerd. Oudere alpha.7/pending-notities hieronder zijn historisch.

Eerste gebruikersgate: circa 2–3 minuten FS-UAE/68030, heen/terug scrollen,
beide watergaten, drukwaarschuwing/contact, paneel schieten, teruglopen,
pauze en life-reset. Geen diagnostic logger in deze visual build. Pas na deze
review de coherente enemy-/audio-slice uitbreiden; 68020 en de volledige
HD/WHDLoad/ADF-campaignflow blijven aparte latere gates.

Reproduceren vanuit `sparkpaw`:
`make PYTHON=../.venv/bin/python3 drowned-slice` en
`../.venv/bin/python3 tests/test_drowned_slice.py`.

## Besluitvoorstel

Maak Level 2 tot een horizontaal platformlevel door een overstroomde
weercentrale: **Drowned Turbines**, met de **Rain Core** als doel. Sparkpaw
herstelt plaatselijk de afvoer door ritmische drukstoten te passeren en
sluisregelaars te ontgrendelen. Water is een leesbare dreiging onder droge
looproutes. De finale is een compacte machineproef rond een vastgelopen
hoofdregelaar, gevolgd door een rustige Core-kamer.

Begin met een 960-pixel speelbare slice, inclusief eigen goedgekeurde art en
een eerste eigen muziekloop. Ontwerp daarna een resident level van voorlopig
3392 pixels met circa 5–7 minuten eerste speeltijd, inclusief verkennen en
finale. Lengte en tijd zijn ontwerpdoelen, geen gemeten eigenschappen. Gebruik
eerst dezelfde wereldomvang als Storm Ruins; extra lengte komt pas ter sprake
als de slice aantoont dat de gewenste afwisseling daar niet in past.

Behoud dezelfde AGA-art, animaties, muziek en SFX op HD, WHDLoad en ADF.
Twee disks hebben de voorkeur, maar krijgen een vroege capaciteitsgate.
Drie disks zijn de uitwijkroute bij onvoldoende aantoonbare ruimte; kleur,
muziekkwaliteit en levelinhoud zijn geen sluitpost.


## Goedgekeurde verfijning — 13 september 2026

### Volledige achtergrond — native concept gereed voor beoordeling

De gebruiker vraagt terecht de achtergrond nu voor de volledige circa 3000px
route te ontwerpen. Voorlopige wereldbreedte blijft 3392. Nieuwe zelfstandige
review: `assets/concept/drowned-panorama-v1/index.html`, met routeknoppen,
cameraschuif, overzichtsvenster en optionele vaste schaalreferentie van de
goedgekeurde voorgrond/player. Die overlay is geen uitwerking van het volledige
terrein. De 960px gameplay-slice blijft de eerste latere implementatiegate.

De nieuwe panorama bevat één doorlopende compositie: onderhoudsinlaat →
turbinehof → dominante hoofdinstallatie → rustiger regenvallei. Bij kwartscroll
overlappen opeenvolgende beelden sterk; dit zijn geen zes aparte achterkamers
die exact op levelcoördinaten wisselen. Concrete lokale herkenning komt ook
uit foreground-architectuur en encountercompositie. Alle landmarks staan in
de verte; geen ingeschilderde loopplatforms. Regen/watervallen zijn in deze
achtergrondstudie statisch, geen impliciete extra animatieopdracht.

Expliciete verduidelijking na de gebruikersvraag over het weerstation: Level 1
bouwt het afzonderlijk ontworpen huisje met `waystation()` in
`tools/generate_runtime_assets.py` in de statische FRONT16-wereldbitmap. Het
huisje is geen hardware sprite of per-frame Bob. De geanimeerde Core is een Bob.
Drowned volgt dezelfde scheiding: station later apart in de voorgrond, Rain
Core ervoor. De achtergrond bevat uitsluitend de rustige omgeving erachter.

Technische bron: `renderer.c:setScroll` ontvangt `cameraX >> 2` en de huidige
4+3-guardroute gebruikt 44 fetchbytes (352 pixels), met een afzonderlijke
leading guard in `prepareRearGuardedDisplay`. Max camera = 3392−320 = 3072;
max rear = 768; conservatieve dekking 768+352 = 1120 pixels. Het zichtbare
laatste venster is x=768..1087; de laatste 32 pixels zijn extra bronmarge.
Alle 3073 camera-originposities zijn offline gecontroleerd. Dit is dekking,
geen vervanging voor native Copper/fine-scroll-validatie.

Nieuwe bron V1 had een ongeschikte hoogteverhouding en blijft als afgewezen
conversiebron bewaard. V2 gebruikt een geïnspecteerde letterboxcrop om ronde
turbines niet plat te drukken. Een vaste omzetting naar exact het geaccepteerde
achtpenspalet geeft `rear-indexed.png` en `drowned-rear.spbm` (1120×208×3).
Alle indices/palettebytes zijn exact teruggelezen; SPR1/SPL1 worden lossless
naar dezelfde SPBM gedecodeerd. Geen nieuwe paletwissels of 4+4 nodig voor deze
artstudie. Selectie van het nieuwe palet in de echte renderer blijft later werk.

Gemeten: planes 87.360 bytes, SPBM met header/palet 87.396; SPR1 66.510 en
SPL1 64.226 bytes (62,7 KiB). Dat laatste is één bestand vóór FFS-overhead,
geen bewijs dat de complete nieuwe campagne op twee disks past. Guarded copy
heeft minimaal 142 bytes/rij = 88.608 bytes voor drie planes; graphics.library
kan verder padderen. Beide kopieën en de rest van gameplay/presentatie tellen
mee in de latere native geheugenpiek. Geen hardware-/FPS-claim.

Reproduceren vanaf root:
`.venv/bin/python3 sparkpaw/tools/prepare_drowned_panorama.py`.
De tool schrijft alleen conceptreviewbestanden. Runtime-asset-hashes blijven
ongewijzigd. JavaScript-syntax van de zelfstandige review gecontroleerd; zes
native camerabeelden en de hele panorama visueel geïnspecteerd. Geen build,
release, dist-wijziging of wijzigingen aan runtime C/assembly in deze stap.

### Eerste native artstudie — visueel geaccepteerd

De gebruiker noemt de native preview overtuigend en bevestigt expliciet dat
exact dezelfde HUD als Level 1 en Stormrail blijft gelden. Water blijft in
het speelveld: animatie y=197..207, droge vloer y=200, HUD vanaf y=208.
Geen extra waterstrook tussen gameplay en HUD. Dit akkoord betreft de native
uitstraling; gameplay, scrollende art en hardware zijn afzonderlijke gates.

### Afzonderlijke lagen en water — 960px offline voorbereiding

`assets/concept/drowned-layers-v1/index.html` is een zelfvoorzienende
interactieve lagenstudie met cameraschuif en automatisch heen-en-weer scrollen.
Het is nadrukkelijk geen speelbare Amiga-build. Reproduceren met
`.venv/bin/python3 sparkpaw/tools/prepare_drowned_layers.py` vanaf de root.

Een imagegen-bewerking verwijdert de ingeschilderde platforms en nabije
voorgrond uit de achterplaat; de aparte voorgrond hergebruikt het geaccepteerde
native materiaal zonder nieuw palet of schaalwijziging. Drie platformplaatsen
vormen een eerste terreinindeling, nog geen compleet ontworpen encounterslice.
De grond heeft twee 80px wateropeningen op x=240 en x=608. Hun zestien frames
komen rechtstreeks uit de bestaande Level-1-generatorformule; geen nieuwe
waterstijl. De HUD gebruikt de ongewijzigde geaccepteerde native previewstrook.

Alle 641 gehele camera-originposities hebben volledige foregrounddekking en
352px rear-fetchdekking bij quarter-scroll. De 512×208 rear hoeft binnen deze
960px route niet te herhalen. Alle 14.080 waterpixels volgen de bestaande
formule; iedere SPBM is pixel/palet-exact teruggelezen en de HUD blijft bij alle
zestien waterfasen identiek. Native runtime-assets en top-level dist-bestanden
bleven tijdens de generatorrun hash-identiek. HTML-JavaScript syntax gecontroleerd.

Ruwe planes: 99.840 bytes front + 39.936 rear + 7.040 water = 146.816 bytes,
exclusief headers, rendererbuffers, masks, HUD, player, audio en allocator.
Dit is geen volledig Chip-RAM-budget, native FPS-meting of ADF-capaciteitsbewijs.
Camera-/waterbeweging in een browser bewijst geen Amiga-renderertiming.

Resterend vóór de echte representatieve slice: vastgelegde bereikbare collision-
geometrie, geïsoleerde test-entry/assetselectie, native palette-consumptie,
drukjet/sluisinteractie, beoordeelde kleine enemyfamilie en eigen muziekloop.
Nieuwe spawns en collisiondata mogen niet via Level-1-constanten doorlekken.
De verkenning bevestigt dat `collisionLoad` nog een vaste storm-map opent en
waterlocaties en enemy-surfaces in `level_data` statisch zijn: deze koude
selectiegrenzen moeten expliciet worden uitgebreid, niet met hernoemde assets
over bestaande storm-bestanden worden omzeild. Geen wijzigingen aan die runtime-
modules in deze artvoorbereiding.

De gebruiker accepteerde conceptboard V1 als richting en vroeg om een echte
320×208/4+3-vertaling met bestaande Sparkpaw. Die staat nu onder
`assets/concept/drowned-native-v1/preview-1x.png` en `preview-4x.png`.
Reproduceren: `.venv/bin/python3 sparkpaw/tools/preview_drowned_native.py`
vanaf de repositoryroot. De bron is met built-in imagegen gemaakt; exacte
prompt staat in IMAGEGEN_PROMPTS. Geen runtimebestand of dist-artifact gewijzigd.

De studie gebruikt exact het bestaande FRONT16, een nieuw constant REAR8-palet
met 12-bit RGB-stappen, playerframe 0 uit de bestaande SPBM zonder schaling of
recolour, en bestaande HUD-atlascellen. Beide review-SPBMs worden teruggelezen
en pixel/palet-identiteit wordt geassert. Ruwe beeldplanes: 33.280 front +
24.960 rear = 58.240 bytes; dit is één scherm, geen compleet resident RAM-budget.

Visueel blijven turbinevorm en diepte herkenbaar. Voorgrond wordt donkerder en
verliest veel groen/koper; gebruiker accepteert deze underpainting als richting.
Native pixel-cleanup en betere materiaalrollen zijn nog niet als geaccepteerd
beschouwd. Het achtergrondbeeld bevat nog onderliggende voorgrondvormen en
is dus geen schone scrollbare parallaxbron. Maskerranden, losse lagen, seams,
collision, water/jetanimatie en native cadence worden later apart bewezen.
De nieuwe bronplaat en native vertaling zijn geen automatische acceptatie van
een gewijzigde gameplaygeometrie, enemyart of finale.

De gebruiker heeft de wereldrichting en onderstaande verfijningen goedgekeurd.
Volgende werk is de gefaseerde slicevoorbereiding; geen release-autorisatie. De keuze
voor machinefinale versus bewegende eindbaas blijft expliciet open.

- De Rain Core wordt bij een eigen klein weerstation behaald, zoals in Level 1:
  een pomphuisje met keramisch dak, koperen regenmeters, verlichte deur en kalm
  waterbekken. Eerst finale, daarna een rustige herkenbare Core-beloning.
- Eén kleine en één grotere bewegende vijand, beide met bestaande respawnregels
  en begrensde snelheidsvariatie; de grote krijgt Strider-achtige sprongroutes.
- Gevechtsintensiteit en ontmoetingsdichtheid worden aan Level 1 gespiegeld.
  De eerder genoemde twee gelijktijdige vijanden zijn een beginbudget voor de
  slice, geen harde vermindering van de volledige Level-1-achtige actie.
- Water is overal onderdeel van de sfeer. Gevechtsstukken, platformstukken en
  rustpunten wisselen af; pas later combinaties van één vijand met een jet.
  Geen level dat grotendeels bestaat uit wachten op drukvensters.
- Het grotere 28-frame enemyontwerp verhoogt de assetraming naar 165–280 KiB
  unieke lossless payload. Meet de werkelijke conversie; drie disks worden
  hierdoor waarschijnlijker, zonder de kwaliteit vooraf in te perken.

Eerste conceptboard toont gameplaymateriaal, beide vijandrollen en het aparte
weerstation. Een machine-arena mag als onbesliste studie worden getoond, maar
wordt niet stilzwijgend de goedgekeurde finale. Muziekpreview en native
conversie volgen na deze eerste visuele richting, vóór runtime-integratie.

## 1. Actuele basis en bronconflicten

De eerste gelezen bronnen waren `CODEX_HANDOFF.md`, `README.md` en
`CURRENT_STATUS.md`. Hun alpha.7/spritefix-pending-koppen zijn verouderd.
In `dist` staan de zes alpha.8-archieven/ADFs en de uitgepakte alpha.8-HD-map,
naast `Disk2-SpriteGuard-ADF` en `older-builds`. Git HEAD is
`0b15ab2` (alpha.7); een gebouwde alpha.8 betekent dus niet automatisch dat
git of alle releaseadministratie daarmee gelijkloopt.

De gebruiker bevestigt op 13 september: de ADF-spritefix werkt op echte
floppies, inclusief Level 1 uitspelen, CONTINUE, wisselen naar Disk 2 en lang
wachten op INSERT DISK 2. HD en WHDLoad werken volgens zijn hardwaretests
eveneens goed. Dat is de actuele gebruikersacceptatie voor deze routes.
Dit plan verklaart daarmee geen afzonderlijk gemeten minimale vrije Chip-RAM,
nieuwe 68020-cadence, Pocket-test of elk oud incident automatisch afgedaan.

Bij aanvang gewijzigde bestanden: `CODEX_HANDOFF.md`, `Makefile`,
`docs/CURRENT_STATUS.md`, `docs/ITCH_IO_DESCRIPTION.html`,
`docs/RELEASE_NOTES_0_7_0_ALPHA_7.md`, `src/title.c`,
`tools/make_release.py`, `whdload/Sparkpaw.Slave`, `whdload/Sparkpaw.asm`;
ongetrackt: `docs/RELEASE_NOTES_0_7_0_ALPHA_7.html` en
`tests/test_adf_presentation_sprites.py`. Alle bestaande wijzigingen blijven
behouden. Alpha.8-artifacts worden niet herbouwd of verplaatst.

### Naam en verhaal

De verhaalbron [STORY_AND_INTRO_PLAN.md](concepts/story-intro/STORY_AND_INTRO_PLAN.md)
legt de volgorde expliciet vast:

1. Storm Ruins — Lightning Core.
2. Drowned Turbines — Rain Core.
3. Gale Foundry — Wind Core.
4. Ember Observatory — Warmth Core.
5. Archivolt's Sky Archive — Balance Core.

Stormrail is het intermezzo tussen de eerste twee platformlevels. PREPRODUCTION
noemt Storm Ruins eveneens de eerste wereld. De oude Stormrail-verhaalparagraaf
noemt Drowned Turbines al als bestemming. Daarentegen noemen de latere
Harrier-conceptnotities en de kop/boundary van STORMRAIL_GATE6_FINALE_CONTRACT
ten onrechte “Level 2: Storm Ruins”. Voor dit plan geldt **Drowned Turbines**.
De bestaande finalegeometrie, Harrier en muur blijven gewoon geldig: de muur
is de buitenste grens van de regeninstallatie, gebouwd met verwante materialen.
Geen hernoeming van bestaande `storm-*` Level-1-assets.

De oude interludescène met een vastgelopen hoofdsluis is geen reeds uitgevoerd
plotpunt: de geaccepteerde Harrier-finale vervangt die climax. Ook schade aan
de Skimmer is oude conceptintentie, geen bewezen runtimegebeurtenis. Voorstel:
de Skimmer meert heel maar oververhit aan; zijn servicelamp verklaart waarom
Sparkpaw te voet verdergaat. Geen nieuw ongeluk of verplichte verliesanimatie.

## 2. Sfeer en aankomst

Na Harriers nederlaag en de bestaande uitvlucht blijven Stormrails results
staan. Een nieuwe CONTINUE-keuze brengt de speler via de bestaande gecontroleerde
laadgrens naar Drowned Turbines. Geen extra disk-I/O midden in Harriers gevecht
en geen nieuwe film tussen gevecht en bestaande results.

Het eerste beeld: links de aangemeerde Skimmer op een droge onderhoudskade;
rechts verdwijnen enorme turbinehuizen in regenmist. Water stroomt zichtbaar
de verkeerde kant op, naar dikke toevoerbuizen die van de centrale weg leiden.
Een rustige eerste 160 pixels geven ruimte om weer aan lopen en springen te
wennen. Sparkpaw start bestuurbaar op de kade; een nieuwe uitstappose is niet
nodig. De Skimmer is hier een kleine statische decorplaat.

Archivolt heeft regen opgesloten en de circulatie omgekeerd. Sparkpaw opent
eerst lokale bypasses en bevrijdt vervolgens de Rain Core uit de hoofdregelaar.
De slotcompositie toont dat de leidingen naar het archief lopen. Herstelde
afvoer legt een onderhoudsroute naar Gale Foundry bloot, maar start nog geen
ongebouwd Level 3. De wereldwijde Stormstone wordt pas na alle vijf Cores
hersteld; Level 2 lost slechts deze lokale regenopstopping op.

Eén optionele grap op een bord: “RAINFALL STORED FOR YOUR CONVENIENCE.”
Geen dialoogpauze, uitlegpaneel of nieuw bedieningsscherm nodig.

| Onderdeel | Storm Ruins | Drowned Turbines |
| --- | --- | --- |
| Grote vormen | Ruïnes, bos, bergen en losse platforms | Ronde turbinehuizen, zware pijpen, sluisschotten en serviceloopbruggen |
| Materialen | Mossteen, violet staal, cyan leidingen | Nat petrolstaal, lichte keramische randen, gedempt koper, donkere waterkamers |
| Diepte | Open vallei en herkenbare toren | Industriële binnenhoven, afwisselend regenlicht en diepe pomphallen |
| Actie | Verplaatsen en vechten langs vaste obstakels | Ritme lezen, een veilige plek kiezen en drukvensters benutten |
| Water | Afzonderlijke valkuilen | Doorlopende thematische aanwezigheid; alleen duidelijk gemarkeerde open bassins zijn gevaarlijk |
| Climax | Rustige waystation/Core-clearing | Hoofdregelaar ontgrendelen, dan stilte en Rain Core |

De turbines zijn grote achtergrondsilhouetten met beperkte lokale animatie.
Geen volledig draaiende platforms, radiaal level of druk achtergrondlawaai
achter ieder sprongtraject. Vorm, licht en compositie maken het onderscheid;
een andere tint op Storm Ruins is onvoldoende.

## 3. Route, tempo en spelregels

Voorlopige coördinaten dienen als blockout, niet als vastgestelde collisiondata.
De camera blijft horizontaal en volgt de geaccepteerde gecentreerde speler.
Gebruik de 208 beeldrijen van het speelveld en laat bovenroutes binnen dat
beeld vallen. Geen verticale scroll of schermgrid.

| Wereld-x | Beat | Spelinhoud | Richttijd eerste poging |
| --- | --- | --- | --- |
| 0–479 | Onderhoudskade | Veilige aankomst, korte sprong, eerste drukventiel zonder vijand | 30–45 s |
| 480–1119 | Inlaatbruggen | Droge eilandjes, één drukstoot per passage, eerste Silt Skitter, optionele bovenroute | 50–70 s |
| 1120–1759 | Bypasshal | Eerste sluisregelaar, terugblik op veranderd lokaal water, rustige beloningsnis | 50–70 s |
| 1760–2399 | Turbinegalerij | Twee afwisselende drukzones, Pump Warden, keuze tussen korte timingroute en langere veilige route | 60–90 s |
| 2400–3039 | Hoofdregelaar | Voorbereidingsruimte en een volledige machinefinale in de laatste vaste arena | 70–100 s |
| 3040–3391 | Regenweerstation | Rustige Core-onthulling, pickup, zichtbaar herstelde afvoer en results | 20–35 s |

### Drukstoten: de hoofdmechaniek

Een vaste opening in vloer of wand heeft drie eenduidige toestanden:
rust, zichtbaar opbouwen, uitstoot. Startvoorstel voor een gewone cyclus:
60 ticks rust, 30 ticks waarschuwing, 30 ticks uitstoot, 30 ticks herstel.
Bij 50 Hz is dat drie seconden. De centrale waarschuwing duurt 0,6 s en is
zichtbaar in een mechanische wijzer én een kleine stoom-/waterbel; geluid is
ondersteunend en nooit noodzakelijk.

Eerst alleen verticale jets onder een sprong, later twee jets met verschoven
fase. Maximaal twee zichtbare actieve hazardzones; begin de slice met één.
Raakvlak alleen in de uitstootfase, één half hart schade via de bestaande
hurt/invulnerability-regels. Geen instant death door een nauwelijks zichtbare
nevel. Een waterbassin behoudt het bestaande splash/life-loss-contract.
Altijd een droge wachtplek buiten de hitbox; geen verplichte schade bij slechte
instaptiming of bij binnenkomst van buiten beeld.

### Sluizen en platforming

Een verlichte regelaar is met de bestaande normale energieschoten te activeren.
Geen charged-shot-afhankelijkheid: die staat in oud preproductionmateriaal,
maar wordt hier niet als beschikbare mechanic aangenomen. Richting en hoogte
maken lopen, springen en crouch-fire bruikbaar zonder extra knop.

Een geopende bypass legt een **vaste** lage looproute vrij of schakelt een
lokale jet uit. Geen wereldwijde watersimulatie. Een sluisschot beweegt buiten
de spelerbaan; pas na de animatie wordt de doorgang geopend. Het mag niet op
de speler sluiten. Begin met een eenmalige, niet-terugdraaiende toestand.
Implementatie vraagt een kleine dynamische collisionlaag en exact dezelfde
toestand voor pixels, botsing en herstel van beide renderbuffers.

Sprongafstanden worden afgeleid van de bestaande physics. Gewone verplichte
sprongen mikken op maximaal 75% van de gemeten veilige horizontale reikwijdte
op de betreffende hoogte. De slice mag geen pixelprecisie of nieuwe physics
nodig hebben. Geheime routes mogen scherper zijn, met een veilige terugweg.

### Vijanden

- **Silt Skitter:** nieuwe lage reinigingsmachine, richtcel 32×24. Twee HP,
  hergebruik van beetle-patrol/contact/hit/death. Eigen nat-stalen kap, kleine
  koperkleurige borstels en schrapende loop. Crouch-fire blijft de herkenbare
  tegenactie. Nieuwe art, geen zwaardere AI.
- **Pump Warden:** grotere bewegende pompwachter, richtcel 64×64, drie HP.
  Eigen art op de bestaande Strider-lifecycle: lopen, draaien, aangekondigd
  springen via authored links, landen, schieten, hurt en death. Verschillende
  begrensde snelheden en bestaande off-camera persistentie/respawnregels.
  De vaste Pump Warden vervalt; geen derde enemyfamilie toevoegen.

Normaal maximaal twee vijanden tegelijk op de relevante speelroute, technisch
binnen de bestaande vier slots. Behoud zes spelershots en twee hostile shots
van platformgameplay; de grotere Stormrail-pools zijn geen vrij budget.
Geen nieuwe enemyfamilie die stilzwijgend extra algemene pools reserveert.
De machinefinale gebruikt hazard/regelaarstate, geen vijfde generieke vijand.

### Geheimen, beloningen en levens

Richt op 40–48 geplaatste Shards binnen de bestaande 48-capaciteit, verdeeld
over verplichte route en twee geheimen. Behoud 50 Shards = één extra leven,
de bestaande score/HUD-regels en unieke award-IDs; geen nieuwe muntsoort.
Geheim A is een droge servicegang onder een geopende bypass met Shards.
Geheim B is een moeilijker bovenpad naar één bestaande 1UP-beloning, met een
andere onthulling dan opnieuw “achter de Core kruipen”. Maximaal één award
per levelpoging; waterdood mag geen farmingloop opleveren.

Voor de eerste slice en eerste complete versie blijft life-loss een resident
herstart van het level volgens de bestaande regels. **Geen verborgen aanname
dat een midlevel-checkpoint al bestaat.** Houd finaleherstel mild en de route
compact. Als de complete 5–7-minutenroute in tests te veel herhaling geeft,
wordt één checkpoint na de bypasshal een afzonderlijke, expliciete uitbreiding:
spawn, sluisstate, pickups, score-awards en hazardfasen moeten dan samen worden
vastgelegd. Geen los respawnpunt boven veranderd water.

### Finale: Pressure Governor — voorstel, keuze nog open

Een herkenbare hoofdregelaar blokkeert de doorgang naar het weerstation.
De Rain Core bevindt zich bij dat station, niet in de gevechtsmachine. Drie zichtbare
vergrendelingen moeten na elkaar worden uitgeschakeld vanuit lage, middelhoge
en weer lage vaste standplekken. Elk venster gebruikt dezelfde geleerde
waarschuwing → jet → rust. Alleen de actieve vergrendeling is kwetsbaar;
drie normale treffers per vergrendeling is het eerste afstelvoorstel.
Niet tegelijk een nieuwe vijand, een nieuw sprongtype en een nieuw aanvalspatroon.

De laatste ronde verkort de rust, behoudt ten minste de 30-tick-waarschuwing
en houdt altijd één veilige wachtplek open. Geen tijdslimiet of terugkerende
add-waves. Na de derde vergrendeling stoppen de jets, het grote turbinehart
komt langzaam tot rust en de weg naar de Core opent. De grote machine bestaat
vooral uit statische FRONT/REAR-art met kleine geanimeerde kleppen; geen
schermvullende Bob of Harrier-herskin. Voorlopig 60–90 s gevechtsdoel, geen
extra boss-track of diskload.

## 4. Eerste representatieve speelbare slice

**“Bypass No. 3”**, 960 pixels, circa 60–90 seconden eerste passage, vrij
heen-en-weer te spelen met een compile-guarded direct-start in een latere
zelfstandige testmap. De productiecamera, player, HUD, schieten, botsing,
waterimpact, audio en renderer blijven echt.

1. 0–159: droge aankomst en één goed zichtbare turbine in de diepte.
2. 160–383: veilige uitleg van één drukjet, twee korte sprongen en Shardboog.
3. 384–639: één Silt Skitter en een schietbare bypassregelaar; hun eerste
   interacties staan uit elkaar, niet boven dezelfde valkuil.
4. 640–799: geopende lage servicepassage met optionele beloning.
5. 800–959: een tweede toepassing van de jet en een rustige eindnis.

Minimaal één schermbreedte heeft definitieve materiaalrichting en eigen
voor-/achtergrond; de rest gebruikt dezelfde coherente goedgekeurde kit.
Bevat de eigen wateranimatie en muziekloop. Pump Warden en volledige finale
komen later; de slice beantwoordt eerst of **drukritme + natte machinewereld**
leuk, duidelijk en wezenlijk anders voelt.

Acceptatievragen: zijn veilige randen direct leesbaar, klopt de schaal, is
de waarschuwing zonder geluid duidelijk, voelt wachten kort en bewust, is
de sluisverandering begrijpelijk en blijft muziek/SFX helder? Geen weken
interne polijsting vóór deze eerste menselijke beoordeling.

## 5. Art, animatie, effecten en muziek

### Conceptpakket vóór runtime-integratie

Maak later één samenhangend conceptpakket: aankomstbeeld, representatief
320×208-speelbeeld met echte HUD eronder, materialenkit, beide vijanden en
Pressure Governor/Rain Core. Toon rustige én actieve jettoestand. Eerst
compositie/identiteit goedkeuren, vervolgens exacte indexed/native previews
op 1× en nearest-neighbour vergroting. Conceptresolutie alleen is geen bewijs
dat materiaal, maskers en silhouet bij native grootte werken.

Gebruik bij nieuwe rasterconcepten de imagegen-skill, bewaar versies en exacte
prompts in IMAGEGEN_PROMPTS. Geen genereren of integreren in deze planstap.
Nieuwe art gaat niet de runtime in vóór expliciete conceptbeoordeling. Geen
ad-hoc polygonen als uiteindelijke machine- of vijandart.

| Asset | Eerste omvang / behandeling |
| --- | --- |
| Voorgrondkit | Droge rand, smalle loopbrug, platform, pijpknie, rooster, sluis, servicewand; gedeelde verbindingen zonder zichtbare tegeldozen |
| Achtergrond | Eén samenhangende quarter-scroll-panorama met inlaat, turbinehal en rain-vault; stabiele semantische penrollen |
| Aankomst | Statische kleine Skimmer/kade; bestaande Sparkpaw-poses |
| Water | Bestaande 80×11/16-frame-aanpak als technische basis; nieuwe native kleuren/compositie alleen na review |
| Jet | Circa 16×48, voorlopig 6–8 prebuilt frames; waarschuwing, actieve kolom en herstel apart leesbaar |
| Sluisschot | Circa 32×48, 4–6 authored standen; kleine lokale patches |
| Silt Skitter | 32×24, negen basisframes met bestaande lifecycle als uitgangspunt |
| Pump Warden | 64×64, bestaande 28-frame Strider-lifecycle als art- en cachebudget |
| Hoofdregelaar | Statische architectuur plus kleine slot-/wijzerframes; zichtbaar stadium per vergrendeling |
| Rain Core | Eigen druppel/lensachtige kern binnen bestaande 64×48 Core-presentatie, maximaal de bestaande 18-frame-klasse |
| SFX | Drukopbouw, uitstoot, sluisontgrendeling; Core-cue zo nodig eigen variant; gewone actie-/pickup-SFX hergebruiken |

Geen nieuwe playerframes nodig voor de basis. De huidige speler gebruikt één
attached 64-pixel AGA-spritepaar op kanalen 0/1 voor het 48×48 beeld; de oudere
zes-kanaalsbeschrijving in de animatieskill is historisch. Bron/actuele
renderer gaat voor die verouderde regel. Append-only frame-IDs, vaste voeten,
exacte mirrors, palette/material identity en Fast-masters/Chip-stages blijven.

### Eigen track: werktitel “Undertow Circuit”

Voorstel: 144 BPM, 64 maten in 4/4, ongeveer 107 seconden per loop. Een
herkenbaar kort dalend motief, pulserende bas, metaalachtige percussie en een
helder antwoordmotief zodra het refrein opent. Meer spanning en ruimte dan
Copper Sprint (164) en Iron Horizon (172), met genoeg ritme om door te lopen.
Een atmosferische middensectie geeft de turbinehal diepte; geen alleenstaande
regenambience in plaats van een volwaardig nummer.

Gebruik de bestaande drie muziekkanalen van de CIA-getimede ProTracker-backend;
MOD-kanaal vier blijft leeg, inclusief effects. Paula AUD3 blijft voor de
bestaande twee gemixte effectstemmen, met gereserveerde plasmafunctie en
prioriteiten. Geen continu watersample dat waarschuwing/hurt verdringt.
Drukwaarschuwingen werken zelfstandig, niet op de songpositie.

Eerste muziekbudget: 12–24 KiB decoded Chip-samples en 16–24 KiB Fast-score,
met circa 12–24 KiB lossless diskpayload als streefbereik, niet als garantie.
Beoordeel eerst een beluisterbare compositie, daarna muziek plus echte gameplay-
SFX in de slice. Hergebruik van bestaande klanken mag, hergebruik van een
hele bestaande track is niet het doel. Eén track loopt van aankomst tot finale;
life-reset onderbreekt de huidige lifecycle niet, resident replay herstart hem.
Results behouden hun bestaande afhandeling. Soundtest krijgt later alleen op
HD/WHDLoad de zesde track, inclusief offline READY-cache/state-dekking.

## 6. Engine: hergebruik en noodzakelijke uitbreidingen

| Grens / modules | Werk bij latere implementatie | Behoud / verificatie |
| --- | --- | --- |
| `campaign_contract.h`, results/main | Nieuwe typed section, post-Stormrail entrysnapshot, Rain-Core-state; Stormrail CONTINUE | Bank score exact eenmaal, carried lives/health/Shards vanaf eerste HUD-frame |
| `assets.c`, ownership manifest | Drowned assetgroep en load-time sectiondescriptor in plaats van alleen Stormrail-boolean | Alleen actieve levelgraphics/caches; geldige loadvolgorde en volledige partial-failure cleanup |
| `collision.c`, `level_data` | Eigen map/hazards/spawns; kleine sluis-overlay | `collisionLoad` opent nu vast storm-collision.bin: dit expliciet vervangen, niet alleen nieuw bestand verpakken |
| Gameplay | Bounded pressure/sluis-state met vaste timers | Geen per-frame allocatie, globale watersimulatie of extra invoer |
| Enemies/cache | Eigen Skitter-art via bestaande gedragsfamilie; Pump Warden-state/cache | Vier algemene slots, bestaande projectilecaps; spawnpersistentie en restorevolgorde |
| Renderer/generator | Nieuwe native assets, lokale effectpatches en sectionselectie bij voorbereiding | 4+3, Copper-/HUD-grenzen, rolling rings, culling en canonical restores |
| Audio load/backend | Derde gameplay-score/bank geselecteerd tijdens laden; nieuwe cue-mappings | Geen nieuwe sectionlookup in audiomixer-hotpath; juiste CIA/DMA-stop vóór vrijgeven |
| Media/packager | Per-section dependencies en volume mapping; zo nodig echte Disk-3-flow | Geen disknummer in physics/renderer; DF0/DF1, markers, leesfouten en decoded parity |

Dit is een gerichte uitbreiding van bestaande grenzen, geen brede herschrijving.
PHASE6D's uitgestelde typed loadselectie wordt nu functioneel nodig door een
derde sectie; algemene hotpath-extractie en performanceonderzoek blijven
geparkeerd. Raadpleeg bestaande afwijzingen vóór een vermeende optimalisatie.

Stormrail-replay blijft teruggaan naar zijn post-Level-1-snapshot. Drowned-replay
herstelt zijn eigen entrysnapshot met post-Stormrail-vitals en scorebasis;
Rain-Core/sluizen/awards/timers beginnen opnieuw. Behoud originele life-loss-
regels apart van results-replay. BACK TO TITLE en Escape wissen de campagne
volgens het bestaande contract. Geen cumulatief bijboeken door herhaald replay.
De huidige CampaignState heeft alleen postLevel1-velden: extra snapshots zijn
dus werkelijk werk, geen al ondersteunde generieke campaignfunctie.

Na Drowned voorlopig REPLAY LEVEL en BACK TO TITLE. CONTINUE naar Gale Foundry
verschijnt pas wanneer die bestemming speelbaar is. Direct OPTIONS-start is
later een verse drietal-levens/zes-health-start met expliciet gedefinieerde
Core-context, nooit onbedoeld een oude campaignsnapshot. De nieuwe menutekst
vereist offline cache-uitbreiding; geen live rasterisatie in READY.

## 7. AGA, geheugen en performance

### Vast presentatiecontract

PAL 320×256, 208-rijig speelbeeld plus bestaande 48-rijige HUD. FRONT16 vier
planes, REAR8 drie planes op quarter scroll. Speler-/HUD-paletten blijven
geïsoleerd. Gebruik bestaande rear-bandpalette/morph-techniek met stabiele
penbetekenis; geen per-X paletteconversie die panorama-seams veroorzaakt.
Werk nieuwe voorgrondkleuren uit met shared projectile/diamond/enemyrollen
zichtbaar erbij: een gedeelde FRONT-pen mag geen vijand onverwacht herkleuren.

Kleurvoorstel: petrol/navy diepte, koel licht staal op beloopbare randen,
gedempt koper op bediening, cyan als energie/wateraccent; vijandelijke schoten
blijven warm en anders dan Sparkpaws cyan. Foreground-paletterollen en hun
eventuele levelvariant pas vastleggen na indexed review van alle shared Bobs.
Geen toezegging van een vrij nieuw 16-kleurenpalet zonder die compatibiliteit.

Geen 4+4 als startvoorwaarde. Dat geeft uitsluitend extra achtergronddetail,
geen extra foregroundkleuren, en vereist een eigen gemeten rendererproof.
5+3 is niet mogelijk in AGA dual playfield. Fullscreenplaten blijven waar
gebruikt hun bestaande 64-kleurenpresentatie met pen 0 puur zwart houden.

Behoud line-100 Copper-staging, HUD-switch op hardwarelijn 252, de bestaande
gesynchroniseerde update/restore/draw-grens rond lijn 253, atomair publiceren
van beide Copper-targets en Blitter-waits. Geen CPU read-modify-write op
getoonde Chip-bitmaps. Water/sluiswijzigingen werken via canonical state en
correct herstel op beide targets, nooit door het zichtbare beeld te patchen.

### Rekenkader en gates

De onderstaande waarden zijn planaire rekenwaarden/ontwerpplafonds, geen
native allocationmetingen. Headers, masks, padding, DMA-guards, caches,
executable, audio en OS/WHDLoad-kosten komen erbij.

| Post | Rekenwaarde / doel |
| --- | --- |
| Resident clean foreground 3392×208×4 | 352.768 bytes = 344,5 KiB |
| Twee 1536×208×4 rolling targets | 319.488 bytes = 312 KiB, exclusief verdere guards |
| Rearbron bij 1120×208×3 | 87.360 bytes = 85,3 KiB per kopie; daadwerkelijke guarded kopieën apart tellen |
| Elke extra 320 px canonical foreground | +33.280 bytes; +80 px quarter-scroll rear circa 6.240 bytes per rear-kopie |
| Nieuwe lokale effect-/klepcaches | Streef samen ≤24 KiB extra Chip boven vergelijkbare platformbasis |
| Nieuwe muzieksamples | 12–24 KiB Chip; slechts één gameplaytrack resident |
| Nieuwe sectionstate/tabellen | Streef ≤64 KiB Fast, los van grafische masters/caches |

Streef naar minimaal 256 KiB vrije Chip-marge op het slechtste gemeten
campagnelaadpunt in de afgesproken hardwareconfiguratie en ten minste 1 MiB
Fast over. Dit zijn conservatieve projectgates, geen bewezen minimumspec.
Meet ook het grootste vrije aaneengesloten Chip-blok: vrije som alleen is
onvoldoende. Doelplatform blijft 2 MB Chip + 8 MB Fast; geïnstalleerd is niet
hetzelfde als vrij bij HD-start. Leg de gebruikte Workbench-situatie vast.

Meet cold load, CHARGING/preparation, gameplay met water/vijanden, finale,
results, resident replay, game over en alle sectionovergangen. Stop audio/DMA
en vervang zichtbare Copper-pointers vóór oude allocations verdwijnen. Laad
geen complete Stormrail- en Drowned-cache tegelijk. Houd Fast-conversiemasters
en kleine DMA-zichtbare Chip-stages gescheiden; Blitter leest geen Fast RAM.

De eerste slice gebruikt bestaande geometrie en pools om extra kosten klein
te houden. Vergelijk op 68020 rustige traversal, jets+water+shots+enemy en
finale als werkelijk bereikbare workloads. Streef near-50 Hz met nul ownership-
overtredingen, geen structurele drie-fieldmisses. Bescherm de historische
48,58 FPS alpha.45-regressiereferentie zonder die alpha.8 te noemen; verkrijg
een passende alpha.8-vergelijking wanneer nieuw werk de oude levels kan raken.
Rapporteer intervalverdeling/effectieve FPS met lichte diagnostiek; timings van
een zware profiler zijn geen gebruikerscadence. Onderzoek een materiële
achteruitgang vóór acceptatie, ook in Storm Ruins en Stormrail.

## 8. Opslag: feitelijke alpha.8 en keuze twee/drie disks

Op 13 september zijn de echte alpha.8-ADFs read-only geopend met de lokale
amitools FFS-reader. Beide zijn DOS1/FFS, 901.120 bytes. De vrije blokken komen
uit hun filesystembitmap, niet uit ZIP-grootte of oude documentatie.

| ADF | Gebruikt / vrij (512-byte blocks) | Vrij | Groei boven 32-block reserve |
| --- | --- | --- | --- |
| Disk 1 | 1687 / 73 | 36,5 KiB | 20,5 KiB |
| Disk 2 | 1575 / 185 | 92,5 KiB | 76,5 KiB |
| Samen | 3262 / 258 | 129 KiB | 97 KiB |

SHA256 Disk 1: `f47c2c5d4b5e5c67ba19c615206362fc1506f7c35a6e4d6837957b89945b1cc7`.
SHA256 Disk 2: `9c768c91c28394f8413d350dd3aa2f1cb7c7c91e1c0983e9baa9a29192f8ac1e`.

Gemeten voorbeelden op deze disks: Level-1 front 38.380 stored bytes, rear
69.497, player 98.016, Strider 44.077; Stormrail flight-rear 60.744, family
9.317, rail-score+bank 11.334. De gecrunchte executable op Disk 1 is 89.884
bytes. De bestaande compressiewinst is dus al verbruikt in deze baseline.
SPD1/SPL1/SPR1 en Shrinkler mogen niet nogmaals als nieuwe besparing worden
ingeboekt.

### Voorlopig nieuw-contentbudget

| Unieke nieuwe inhoud | Verwachte lossless opgeslagen omvang |
| --- | --- |
| Foreground | 35–65 KiB |
| Rearpanorama | 55–85 KiB |
| Vijanden, kleppen, jets en Core | 55–90 KiB |
| Eigen muziek | 12–24 KiB |
| Collision/route/SFX | 8–16 KiB |
| Totaal unieke payload | **165–280 KiB** |

Dit is een raming uit bestaande assetgroottes; geen compressiegarantie voor
nog ongetekende art. Reserveer daarnaast voorlopig 10–25 KiB voor groei van
de gecrunchte executable en extra FFS-metadata. Geen nieuwe volledige player-
atlas gepland. De blokreserve komt daar nog bovenop, en code op Disk 1 kan niet
zomaar uit de vrije Disk-2-blokken betaald worden.

Bij gelijkblijvende layout past dit dus **niet verantwoord**. Disk 2 bevat
echter nog Level-1-carryover, onder meer beetle 3.831, Strider 44.077, Core
8.017, extra-life 221 en collision 2.968 bytes. Alleen deze vijf bestanden
tellen 59.114 bytes (57,7 KiB), vóór hun FFS-blokken. Dit is een auditkandidaat,
geen geautoriseerde verwijderlijst: de collisionmap wordt nu werkelijk gelezen
en Drowned kan sommige families nodig hebben. Audio zoals jump/Strider-shot
is bovendien echt gedeeld ondanks historische LEVEL1-groepering.

### Voorkeurslayout: twee disks

- Disk 1: boot/executable, presentatie en Storm Ruins.
- Disk 2: Stormrail en Drowned, met werkelijk benodigde gedeelde presentatie,
  gameplay en beide tracks. Geen swap tussen die twee secties.
- Beide behouden lokaal de bestanden die results/game over en mediawachten
  werkelijk nodig hebben. Replay blijft resident en vraagt geen disk.

Eerst de typed dependency-audit afronden en uitsluitend aantoonbaar onnodige
carryover verwijderen. De 57,7 KiB potentiële payloadwinst zou Disk 2 grofweg
naar 134 KiB uitbreidingsruimte boven reserve brengen; dat is nog krap naast
165–280 KiB nieuwe inhoud. Een tweede compacte lossless-optimalisatie of
gerichte herverdeling kan daarom nodig blijken, maar wordt pas gekozen uit
werkelijke native bestandsmetingen.

Onderzoek hergebruik van bestaand player-/effectmateriaal en identieke
presentatiebestanden vóór een nieuw codecproject. Shared bestanden van Disk 2
naar alleen Disk 1 schuiven is geen gratis besparing: dat kan nieuwe
results/game-over-swaps of hogere residente RAM-pieken veroorzaken. Geen
bestand verwijderen omdat de union van beide disks nog compleet is.
Een andere codec is uitsluitend een afzonderlijke bewezen lossless stap met
bounded decode, CRC, 512-byte I/O, hunk/relocation-validatie en native laadtijd.
Geen hypothetische compressieratio als fitbewijs.

### Wanneer drie disks kiezen

Kies Disk 3 als de volledige goedgekeurde assetset, codegroei, afhankelijkheden
en FFS-overhead na de beperkte audit niet minstens 32 vrije blokken **op elke
disk** laten; of wanneer twee disks alleen haalbaar zijn met extra swaps
binnen gameplay/results, hoge RAM-residency of een grote onzekere loaderrewrite.
Plan productie praktisch op de mogelijkheid van drie, maar maak de keuze pas
na de eerste native art/music-conversie en een complete forecast.

Drie-disklayout: Disk 1 boot/Storm Ruins; Disk 2 Stormrail; Disk 3 Drowned.
Disk 3 bevat zijn eigen cold-load-dependencies plus gedeelde HUD/results/game-
over/prompt-assets en volledige muziek. Eén normale wissel bij Stormrail
CONTINUE; daarna geen wissel tijdens Drowned, finale, dood of resident replay.
Terug naar titel mag via de bestaande Disk-1-laadgrens. Direct Drowned-start
vanaf Disk 1 vraagt Disk 3; de schijf is geen zelfstandig bootproduct.
De historische THREE_ADF-scaffolding is niet als af of getest beschouwd.

HD/WHDLoad krijgen dezelfde decoded bestanden, ongecomprimeerde gewone HD
assetinvoer blijft behouden. Als globale orde van grootte: dezelfde-sized nieuwe
front+rear leveren al circa 430 KiB raw data; inclusief families/music/SFX is
circa 0,55–0,9 MiB extra geïnstalleerde content een eerste forecast. ZIP/LHA-
downloadomvang en WHDLoad-residency apart meten. Geen laad-/RAM-garantie uit
een archiefgrootte afleiden. Intro en Soundtest mogen alleen HD/WHDLoad blijven;
alle levelpresentatie en muziek blijft identiek op floppy.

## 9. Gefaseerde uitvoering en stopmomenten

| Fase | Concreet resultaat | Gate vóór volgende fase |
| --- | --- | --- |
| 0 — ontwerpbesluit | Dit plan, naamcorrectie en scope | Gebruiker kiest hoofdrichting/finale; geen code |
| 1 — concept en muziek | Samenhangende artboards, vijandsilhouetten, eigen muziekpreview | Expliciete conceptreview; afwijzingen vastleggen |
| 2 — native haalbaarheid | Exacte indexed previews, asset/cacheraming, typed dependency-ontwerp en twee-/drie-diskforecast | Art blijft goed op 1×; RAM/storage geen ongedekte belofte |
| 3 — Bypass No. 3 | Coherente 960px slice, één enemy, jet, sluis, eigen audio | Host/native smoke, dan gebruiker FS-UAE/68030: art/feel/function |
| 4 — technische slice-gate | Dezelfde geaccepteerde slice op 68020 en RAM-piekmeting; vroege ADF capacity-probe | Geen corruption, ownershipfouten of materiële cadenceregressie; diskkeuze onderbouwd |
| 5 — volledig level | Zes beats, Pump Warden, secrets, complete machinefinale/Core | Gefocuste 030-playtest van compleet level; lengte/difficulty/checkpointbesluit |
| 6 — campaignintegratie | Stormrail CONTINUE, snapshots, results/replay/menu/cleanup en audioselectie | Host lifecycle/failure-tests; hele campaign, beide oude secties behouden |
| 7 — alle media | Zelfstandige HD/WHDLoad/ADF-kandidaten met manifest/readback | 030 → 020; echte HD/WHDLoad/floppies incl. swaps en lang wachten |
| 8 — afzonderlijke afronding | Alleen op later verzoek: checkpoint/releaseadministratie | Gebruiker heeft presentatie/gameplay/media geaccepteerd en vraagt shipping |

Nieuwe gameplaytests richten zich op echte risico's: warning/damage-grenzen,
collision/pixels na sluisopening, beide buffers na omkeren/replay, pooluitputting,
off-camera hazardentry, eenmalige score/Core/pickups, carry vanaf HUD-frame 1,
partial load cleanup en timer/DMA-eigenaarschap. Geen runtime-tests voor alleen
dit document. Na codewerk reguliere native build en passende hostsuite;
geen releasebuild tijdens open concept-/sliceacceptatie.

Staging volgt `tools/stage_hd_test.py` en de autoritatieve runtime-manifesten.
Eén actuele zelfvoorzienende testset in dist; alpha.8 behouden en hashcontroleren.
Direct-start is een testflag, nooit productieroute. Geen automatische FS-UAE-
playtests vanuit dist; gebruiker beoordeelt vroege slices zelf. Bij expliciete
diagnostiek de normale log-save/freeze-instructie gebruiken, niet bij een
gewone visuele build doen alsof er een log bestaat.

ADF-controle gebeurt per volume, met daadwerkelijke C-decoder/CRC, file-readback,
FFS-free-blocks, juiste markers en gecompileerde plus dynamische dependencies.
Hardwaretestmatrix: cold boot; Level1→Stormrail→Drowned; DF0-wissel; juiste disk
al in DF1; verkeerde/oude disk; lang INSERT laten staan; replay zonder lezen;
terminal game over; terug naar Disk 1; HD/WHDLoad gelijkwaardige visuals/audio;
WHDLoad F10. Nieuwe tests bewijzen de uitbreiding, de huidige succesvolle
alpha.8-hardwaretests worden niet als onbevestigd teruggezet.

## 10. Alleen de belangrijkste creatieve keuzes

1. **Hoofdgevoel:** mijn voorstel is droge platformroutes door een overstroomde
   machinewereld, met drukritme en lokale sluizen. Vrij zwemmen, zuurstof en
   bewegende draagplatforms vallen buiten deze eerste levelopzet.
2. **Finale:** mijn voorstel is de Pressure Governor als interactieve
   machineproef, waarna de Rain Core rust krijgt. Een zelfstandig mobiel
   bosswezen is een werkelijk andere art/AI/performance-opdracht en vervangt
   deze finale indien gewenst; het komt er niet automatisch bovenop.

Tempo, concrete maten, poollimieten en mediakeuze worden binnen deze richting
uitgewerkt en getest. De gebruiker hoeft niet ieder technisch detail vooraf
te kiezen. Nieuwe art blijft wel een expliciet afzonderlijk beoordelingsmoment.

## Geraadpleegde contracten en werkafspraken

- [Preproduction](PREPRODUCTION.md), [Stormrail-plan](STORMRAIL_INTERLUDE_PLAN.md),
  [progressie](PHASE6D_PROGRESSION_PLAN.md), [Phase 7](PHASE7_CAMPAIGN_HARDWARE_PLAN.md),
  [lessen alpha.2 en latere aanvullingen](CHECKPOINT_ALPHA2_LESSONS.md).
- [Campaign loop](CAMPAIGN_LOOP_CONTRACT.md), [asset ownership](CAMPAIGN_ASSET_OWNERSHIP.md),
  [Harrier-finale](STORMRAIL_GATE6_FINALE_CONTRACT.md),
  [AGA-artkwaliteit](AGA_ART_QUALITY_CONTRACT.md).
- [Stormrail muziek](STORMRAIL_MUSIC.md), [campaignaudio op ADF](MUSIC_CAMPAIGN_ADF.md),
  [storage/memory](STORAGE_MEMORY_LOADING_AUDIT.md),
  [bestaande compressie](ADF_COMPRESSION_RESEARCH.md), relevante renderer-,
  collision-, campaign-, projectile- en ownershipbroncode en development history.
- Skills: `build-sparkpaw-visual-slice`, `extend-sparkpaw-animations`,
  `run-sparkpaw-test-cycle`. Dit plan volgt hun concept-first, native-art,
  eigenaarschap en menselijke testgates. Oudere skillregels over zes player-
  spritekanalen of automatisch releasen worden niet boven huidige source en
  de expliciete opdracht “alleen plan” geplaatst.
