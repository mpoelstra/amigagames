# Drowned enemy art — concept v1,15 September2026

## 2026-09-16 — Spillwing action animation review

User approves24x24 concept and scale, explicitly wants harder-to-hit flying
enemy than earlier levels. Working combat proposal:1HP, short readable warning,
committed swoop, no long free shot window. Generated offline16frames:
fly4,warn2,dive2,recover2,hit2,death4 in fixed24x24 cells. Native accepted body
preserved except1 clearer sensor pixel; rotor phases anchored at11,5; body
rotates offline around mast11,7 with nearest sampling, no runtime transforms
or per-frame scale.18degree tilt rejected by bounds check;12degree fits without
cropping. Electric hit transition does not imply multipleHP. Preview warning
240ms, purely proposed timing. Scripted motion GIF is not emulator/AI proof.

Assets:assets/enemies/spillwing-actions-review-v1; generator:
tools/preview_spillwing_actions.py. Palette matches approved native foreground.
User review pending before dry-floor encounter integration. No runtime,
release, test drawer, enemy counts or memory-cache changes this turn.


## 2026-09-16 — Spillwing compact concept review

User requested smaller flying enemy and approved24x24 cell direction, body
approximately18x12. Generated spillwing-concept-v1.png with built-in ImageGen,
existing enemy material sheet as reference; exact prompt in same-name TXT.
Compact steel pod, short connected top rotor, copper fittings and forward sensor.
Provisional24x24 exact foreground-palette size audition produced by
 tools/preview_spillwing_idle.py in assets/enemies/spillwing-idle-review-v1/.
Scale strip uses existing player and approved96px pontoon unchanged. Source
concept and provisional native clusters await user review; no animation,
AI, cache, runtime or test-drawer changes. Next refine native sensor/silhouette,
rotor poses, anticipation/swoop, hit and electric death only after review.
No per-frame autoscaling or new actor anatomy. Full level/upper route remains
planned; native flight envelope must stay compact for its clearance.

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

## Pump Walker dry jump audition — 16 September 2026

User authorized a small jump route after confirming V4 projectile damage.
Active: sparkpaw/dist/Drowned-Jump-020-HD/Drowned-Test (68020/2MBChip/8MBFast).
New isolated SPARKPAW_DROWNED_JUMP_PROOF target drowned-jump adds two authored
links on existing surface1, no engine physics change or extra enemies/geometry.
Right launch x346..350 -> landing382..388; left386..390 ->349..354.
Fixed-point VX +/-320,VY-900,gravity60: about36px travel,25px high,29ticks
flight. Existing12tick compression,5tick landing,7tick recovery. Dry-ground
animation audition, NOT water-gap traversal or full-level route acceptance.
Existing spawn speeds, shot identity, damage and respawn retained.

Host actual enemies.c/drowned_slice.c sanitizer test now runs6000ticks in
legacy/art/jump variants, asserts both flight and landing directions,no route
failure,valid poses/height/patrol limits, shots, deaths and respawn. Projectile
head/crouch/iframes regression also passes. Native compile passes. All66 staged
assets byte-identical to V4;47 executable refs;68 alpha.8 release hashes intact.
No new animation cache/asset allocation. Native motion/feel/cadence pending;
no automatic emulator or release. Leave Walker alive to watch both directions,
then test combat/respawn and save with left mouse +15s frozen hold.
V4 drawer/log/metadata archived intact at older-builds/20260916-before-drowned-jump.
Proof:build/drowned-enemy-audit/proof-jump.json. Last accepted V4 cadence49.69FPS
is not a measurement of this changed workload.

## Drowned v4 user confirms projectile damage — 16 September 2026

User played Drowned-Enemies4-020-HD and explicitly confirms Walker shots now
cause damage. Keep head-hit correction. No new explicit muzzle-alignment,
crouch or both-facing-direction acceptance inferred.
Complete log preserved with sidecar at testresults/Unassigned-Drowned-enemies4-
020-head-hit-confirmed-49-69fps.log; candidate proof-v4 and68 alpha.8 release
hashes verified unchanged.2263 intervals:2249 one-field/14 two/0 three-plus,
max2;49.69FPS,0.619% late20ms intervals,ownership0. Prepared Chip free942968,
Fast5307552;post-run Fast5716040.12 compose raster crossings are not ring rolls.
Stated FS-UAE HD68020/2MBChip/8MBFast; no fresh config read. Good current cadence,
not controlled proof of improvement versus v3 or full-level headroom. Existing
v2's two longer hitches remain historical evidence, not explained by this run.
Active v4 drawer retained; no code/build/release changes this review.

## Drowned head-shot collision correction v4 — 15 September 2026

User accepts v3 sound/look; suspects low muzzle alignment and reports shots
passing through Sparkpaw without damage. Source + actual-function host repro:
standing floor200 gives player.y161; torso contact top168. High Walker shot
v3 top160 had collision bottom167, missing by1px despite intersecting the head.
New candidate-only playerProjectileBounds retains narrow contact X/bottom,
includes standing head at player.y-4; crouching keeps original low bounds.
Game calls it only for hostile-projectile damage after existing enemy contact.
Body contact, physics, damage amount and invulnerability are unchanged.
Muzzle line raised1px: projectile top enemy.y+23,center+27 matches fire-pose
aperture near row26 plus1 Bob grounding. Visual fit in motion remains user gate.

New tests/test_drowned_projectile_damage.py compiles actual extracted player
bounds/damage functions and full projectiles.c with sanitizers. Reproduces old
miss and verifies health loss from both sides, invulnerability, crouch evasion,
airborne hit and below-feet miss. Registered in make test. Actual dual-mode
enemy test and native build pass. No automatic emulator or release.

V3 log preserved +sidecar:testresults/Unassigned-Drowned-enemies3-020-cadence-
48-97fps.log. Candidate files hash-verified;complete footer.2488 intervals,
2436 one-field/52 two/0 three-plus,max2;48.97FPS,2.09% late intervals,
ownership0. Worst compose371 camera273 includes1 Crab/1 Walker; no causal
attribution to sound/rendering.50 compositor raster crossings are not ring rolls.
Prepared freeChip943672,Fast5306816;post-runChip943672,Fast5716808.
User-stated FS-UAE HD68020/2MBChip/8MBFast; config not re-read. Slower than
previous manual run, workloads differ; keep performance concern open.

Active: sparkpaw/dist/Drowned-Enemies4-020-HD/Drowned-Test. Test standing
head hit without blinking, crouch evasion and both facing directions. Left mouse
once,wait15s in frozen save hold,stop/reset.66 assets/47 literal refs;68 alpha.8
release files unchanged. V3 drawer/log/launchers intact under older-builds/
20260915-native-enemies-v3. Proof:build/drowned-enemy-audit/proof-v4.json.
New art/audio bytes unchanged from v3. V4 native gameplay/cadence pending.

## Pump Walker shot identity v3 — 15 September 2026

User accepts v2 visuals, requests heavier own Walker sound and optionally own
shot shape/colour. Reports one or two hitches. Complete v2 log preserved with
sidecar at testresults/Unassigned-Drowned-enemies2-020-cadence-49-66fps.log.
Verified played files against proof-v2.4967 intervals:4937 one-field,28 two,
2 three-plus,max4;49.66FPS,0 ownership errors.30/4967=0.604% late intervals,
max80ms. Do NOT describe this as no long hitches. Sparse minimal cadence log
cannot locate/attribute the2 long intervals. Worst compose frame939,camera436,
no enemy/projectile/collectible draws. FreeChip944920,Fast5307624 prepared.
Stated FS-UAE HD68020/2MBChip/8MBFast, no new config read or hardware claim.

V3 isolated candidate loads pump-shot.raw in place of strider-shot.raw via
SPARKPAW_DROWNED_ENEMY_ART; same priority,cooldown,period322,Paula channel and
silence reload. Diagnostic label pump_walker_shot; existing API/slot retained
in this two-enemy prototype. Future music/campaign integration must register
its own sound event/mixer asset rather than assuming the legacy Strider slot
is a campaign-wide identity. Other builds retain existing sound/path.
Deterministic synth tools/build_pump_shot.py produces signed8-bit2864 bytes,
0.260s,peak112,no clipping,zero first/last samples; +658 Chip bytes vs2206.
Low mechanical impulse/harmonics with short filtered-air decay. Preview/source:
assets/enemies/pump-shot-v1/ (WAV/raw/manifest/projectile GIF).
Projectiles use compact warm copper shoulders,pale core,short segmented trail;
exact existing16x9 dimensions,2 flying patterns,cache count/Blit count unchanged.
Native C pixel function compiled for host preview and mirror check, not a
separately approximated image. Synthesis is build-time only. Audio modes and
actual platform helper tests, real enemy sanitizer test and native build pass.
Listening quality and v3 cadence still require user play; no automatic emulator.

Active: sparkpaw/dist/Drowned-Enemies3-020-HD/Drowned-Test,66 packaged assets,
47 literal refs,68 alpha.8 release files unchanged. V2 drawer/log/metadata
archived intact at dist/older-builds/20260915-native-enemies-v2.
Proof:build/drowned-enemy-audit/proof-v3.json. No release/commit/version change.
After play press/release left mouse,wait15s in frozen save hold,stop/reset.
Keep the two longer v2 intervals as an open issue; do not ascribe them to SFX
or the geyser without event/frame evidence.

## Drowned enemy v1 cadence and v2 electrical polish — 15 September 2026

User played the native-enemy candidate: FPS felt okay; collapse looks good,
requests small electrical malfunction accents; suspects nozzle misalignment.
Original complete log preserved byte-for-byte with matching sidecar at
`testresults/Unassigned-Drowned-native-enemies-020-cadence-49-69fps.log`.
Played candidate files verified against original proof. Legacy diagnostic
alpha41 header is not the actual binary version. Stated FS-UAE HD68020,
2MB Chip/8MB Fast; no fresh emulator-config verification this run.
3292 intervals:3272 one-field/20 two-field/0 three-plus,max2;49.69fps,
0.6075% missed20ms deadlines,ownership violations0. Compositor crossings22
are not ring-roll counts.38 player shots,3 Walker shots,6 death events.
Prepared freeChip944920/largest943672;Fast5307704/largest5306560.
Post-run freeChip944920,Fast5717696. Minimal cadence, broad profiling disabled.
Worst frame1331 camera437 has no enemy/projectile draw/restore counts: no
specific culprit established.49.53fps previous encounter used different manual
workload; do not claim a controlled improvement or full-level headroom proof.

V2 adds sparse cyan/white arcs and sparks inside existing death frames;
mechanical collapse retained. Exact indexed comparison confirms ONLY Walker
24..27 and Crab10..13 differ; sizes, palettes, masks dimensions/cache allocation
unchanged. New source/review files:assets/enemies/drowned-family-v2/.
V1 generator snapshot retained in drowned-family-v1/generator-v1.py.
Source fire-pose inspection: bore center near row27 plus1 Bob grounding,
projectile center was24. Candidate-only projectile Y changed20->24 (center28).
Visible shot alignment still needs user playback in both directions.

Active test: `sparkpaw/dist/Drowned-Enemies2-020-HD/Drowned-Test`.
Native build and dual-mode actual-enemy sanitizer test pass;65 assets,
47 literal references;68 alpha.8 files unchanged. V1 drawer including log and
launcher metadata archived intact at dist/older-builds/20260915-native-enemies-v1.
Proof:build/drowned-enemy-audit/proof-v2.json. No release or auto-emulator run.
V2 art/nozzle/cadence acceptance pending. Press/release left mouse,wait15s in
frozen save hold,then stop/reset FS-UAE. User may assess offline death GIFs too.

## Native Drowned enemies integrated candidate — 15 September 2026

User accepted the refined Pump Walker recoil and explicitly authorized finishing
poses/animations and placing both enemies in the isolated level test. New active
manual test: `sparkpaw/dist/Drowned-Enemies-020-HD/Drowned-Test` (paths from root).
68020 / 2 MB Chip / 8 MB Fast, minimal cadence instrumentation. User requested
this 020 gate directly; no automatic emulator run or hardware acceptance.

`tools/build_drowned_enemy_assets.py` compiles accepted native pixels into
32 Pump Walker and 22 Turbine Crab slots with exact mirrors and pen-zero masks.
Review sheets/turn GIFs/manifest: `assets/enemies/drowned-family-v1/`.
Walker slots 0..27 keep legacy semantic roles; 28..31 add a planted front-pivot
turn. Crab 0..7 walk/rotor, 8..9 reserved, 10..13 collapse, 14..17 turn,
18..21 hit. Front-pivot parts are assembled at native size with retained material
clusters. Compression/flight/descent/landing/recovery poses are included; current
floor patrols have no traversal links, so jump animation is NOT exercised here.
User-approved Walker hit/death and Crab hit/death pixel families retained.

`SPARKPAW_DROWNED_ENEMY_ART` isolates paths/counts/selectors and grounding.
Existing two-type Bob caches, AI speeds, health, score, offscreen respawn,
collision cells, renderer order and staging dimensions retained. Pump Walker
uses +1px visual grounding (legacy Strider +2), nozzle-aligned projectile Y,
and 384 fixed-point walk phase distance to match its 1.5px stance step. This
increases frame-change frequency versus old Strider: measure cadence, do not
infer unchanged per-frame cost merely from unchanged cell dimensions.
Crab's death selection is explicitly mapped to its four valid cached poses.

Memory deltas against old encounter: +18,720 Chip bytes for Crab cache,
+25,600 Fast cache bytes and +32,960 Fast source bitmap/mask bytes. These are
asset allocation deltas, not a measured total free-memory claim. Source assets
remain Fast; Walker Chip staging size unchanged. Runtime CPU does no rigging,
rotation or pixel generation. Candidate has 65 packaged assets / 47 executable
literal references. Native build and full `make test` pass; real AI host test
runs both legacy and candidate modes under sanitizers, checks frame bounds,
nozzle Y, shots, death/score and offscreen respawn. Final gait change rechecked.
Legacy enemies 68020 -O2 assembly is identical to HEAD excluding filename idnt.

Proof: `sparkpaw/build/drowned-enemy-audit/proof.json`. All 68 alpha.8 release
files verified unchanged. Previous Drowned drawers/logs and loose launcher
metadata archived intact under `dist/older-builds/20260915-before-native-enemies`.
Test 60–90s with both enemies, turns, hits/deaths, water/geyser, gate and respawn.
Press/release left mouse once, wait at least 15s in deliberate frozen save hold,
then stop/reset FS-UAE. No release/version/commit change. New in-game visuals,
turn quality, nozzle alignment and 020 cadence remain pending user testing.

## Pump Walker recoil refinement v2 — 15 September 2026

English enemy names: **Pump Walker** and **Turbine Crab**. User accepts the
breakdown/collapse direction and leaves Crab recoil unchanged for now; asks
for Walker torso recoil to connect naturally to its legs.
Offline v2: assets/enemies/drowned-actions-review-v2/ (under sparkpaw from
workspace root). Fire/hit shift the hips with the torso and solve the existing
thigh/shin parts around planted feet; hit settles directly to the exact idle.
Visible foot pixels in rows 58–63 verified unchanged in all fire/hit poses.
Walker idle/charge/death and every Crab pose are byte-identical to v1.
Palette and all three test-package/release inventory hashes checked unchanged.
V1 generator preserved in drowned-actions-review-v1/generator-v1.py; active
script tools/preview_drowned_enemy_actions.py produces v2. Contact inspected;
revised motion awaits user review. These are Python/Pillow composited GIFs,
not emulator captures or performance evidence. No runtime integration.

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

## Enemy concept colour variant v2 —15 September2026

User likes v1 designs but requests stronger colour distinction. Built-in
ImageGen colour-edit concept v2:crab dominant weathered copper-brown shell,
dark steel legs/rotor;walker dominant cool slate/steel,small copper fittings.
Both retain sparse cyan sensor accents. Source assets/enemies/drowned-enemies-
concept-v2.png;exact prompt sibling -prompt.txt. V1 preserved. V2 pending
user review; no native mapping/anatomical pixel-parity claim. Generated image
remains a concept with original scale/background limitations. No runtime,
asset-pack or release changes. Native material roles must distinguish crab
copper from Sparkpaw's orange; verify against blue environment at native size.


Status:concept review only. Existing encounter/runtime/dist unchanged.
Source:assets/enemies/drowned-enemies-concept-v1.png.
Exact built-in ImageGen prompt:assets/enemies/drowned-enemies-concept-v1-prompt.txt.

## Retained lessons

Read extend-sparkpaw-animations and imagegen skills; IMAGEGEN_PROMPTS Strider
biped/scale/palette history; docs/DEVELOPMENT_HISTORY shoot/hurt/death and
later gait rejection notes. Later evidence supersedes early accepted sheets.
Do not generate whole production animation sheets repeatedly. Disconnected
limbs, gait asymmetry, shifting joints/mass and cleanup deleting limbs are
source anatomy faults. A perfect mirror/loop test alone does not make a good
walk. Review walk and turn together. No simplistic new stick legs or static
sliding presented as final quality. One approved native master and detailed
fixed parts/rig or deliberate pixel edits drive all states.
Shared palette membership is insufficient: material/colour proportions in
death must match idle, not turn the enemy orange/purple. Derive destruction
from the accepted anatomy. No scale reduction to fit extended muzzle flashes,
wide limbs or debris. Keep separate projectiles/effects out of pose scaling.

## Proposed anatomy

Turbine crab:32x24 existing small-enemy cell;4 walking legs total, near/far
pairs distinguished by depth shading, one compact gripping claw, recessed
side rotor, fixed pivots. Body mass and rotor axis locked. Walking must show
weight transfer without adding/removing legs. Death:rotor stutters, shell
fractures, joints fold, chassis rests/debris; same material mass and size.
Pompwalker:64x64 existing large-enemy cell;2 hydraulic legs, fixed hip/knee/
ankle joints, broad feet, pressure tank torso, fixed gauge mount, short torso
nozzle and1 service arm, hose with fixed endpoints. Never quadrupedal.
Walking speed varies per spawn using current policy; shot anticipation and
fire remain fixed readable timings. Hit recoil settles immediately, no delayed
hit dance after traversal. Death:pressure failure, asymmetric knee collapse,
tank settling, contained steam/debris. Not generic explosion replacing robot.

Dominant cool steel and navy, restrained copper fittings, sparse cyan.
Drowned foreground palette differs from old violet Strider extension. Native
mapping must explicitly assess palette roles/ratios rather than naive nearest
colour. Pen0 transparency. Same palette and artwork across HD/WHDLoad/ADF.

## v1 review limits

Generated sheet is an attractive design proposal, NOT native art proof.
Model did not obey requested flat magenta background, exact shared scale
between creatures, or3-blade rotor (extra blades visible). Four-pose anatomy
is not production locked. Detailed gauge/nozzle may simplify at64px. Crab
legs/rotor need32x24 readability check. No automatic background cleanup or
runtime integration. User design acceptance remains pending.

## Next gates

1. User selects/revises designs. Establish one native idle master per family
   in exact32x24 and64x64 cells, same baseline rules as current caches.
   Show nearest-neighbour enlargement plus actual-size comparison beside
   Sparkpaw and in Drowned background. Concept board alone cannot approve size.
2. Lock silhouette, material proportions, limb topology, pivots and facing.
   Define fixed-part art with clean occlusion; no independent pose rescaling.
3. Review polished walk+turn loop locally; then shoot charge/fire/recovery,
   hit and death loops. Preserve accepted existing slot0..27 semantics;
   replacements are level-local families, no renumbering legacy assets.
   Proposed stage count stays within current cache until audited otherwise.
   Small enemy9 slots; large28 slots (walk0..7,turn8,shoot9..10,hit11..17,
   traversal18..23,death24..27). Confirm actual selectors at integration.
4. Native indexed pose bounds/palette-role counts/mirrors/loop closure;
   no missing pixels, planted feet or unintended scale/body mass changes.
5. Integrate only approved family; keep source/runtime/cache ownership scoped
   per level. Measure real Chip/Fast growth and native02050Hz on actual
   two-enemy encounter including shots/death/respawn/geyser/water. Four-frame
   death budget is current baseline, not permission to silently enlarge caches.

No new runtime/death behavior, releases, commits, or artifact repackaging.
