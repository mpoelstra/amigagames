# Drowned Turbines — complete route proposal

## 2026-09-16 — Pontoon water-only travel bounds

User confirms faster startup and acceptable appearance; rejects hull over land.
Evidence:Unassigned-Drowned-pontoon-bank-overlap.png/TXT and startup2-49-80fps.log/TXT.
Latest isolated basin1570intervals/6two-field/0longer,49.80FPS; no enemy workload.
Changed left-edge range128..928 to160..864 (=waterRight960 minus hull96).
Reset starts wholly on water, reverses immediately at full-hull limits and
continues repeating after initial boarding. Speed/bob/art/masks unchanged.
Real-physics test proves both-bank jump exits, boarding,1200ticks full hull
containment including both reversals, carry and reset. Native build passes.
Updated Drowned-Pontoon-020-HD/Drowned-Test, previous drawer/log preserved by
stager,67assets/48refs and68release hashes verified. Proof:build/drowned-pontoon/proof-banks-v3.json.
No release; user visual gate pending.


## 2026-09-16 — Pontoon startup revision, native boot pending

User reports original pontoon executable leaves Workbench visible, no visible
crash/gameplay. Packaged files verified against proof.json; no log present.
Read-only source review found1,966,080 per-pixel water tests before display
takeover. This is a credible long-startup cause, not confirmed sole diagnosis.
Moved mask generation to host asset build: pontoon-clip.bin286720bytes in
big-endian native words. Amiga allocates same RAM, reads precomputed bytes;
no million-iteration initialization loop. Actual loader tested against water
oracle over2293760 mask bits, same image/physics. Adds startupdiag.log stages
loading_files/preparing_renderer/renderer_ready and explicit failure stages,
only for pontoon proof. Native build passes; no emulator launch/boot claim.
User retries updated Drowned-Pontoon-020-HD/Drowned-Test. Official stager
preserves prior drawer;67assets/48refs verified,68release files unchanged.
Proof:build/drowned-pontoon/proof-startup-v2.json. No release/version change.


## 2026-09-16 — Playable pontoon proof ready

User approved native pontoon preview and requested runtime proof. Isolated
SPARKPAW_DROWNED_PONTOON candidate:800px water basin160..960,96px float starts128,
ends928,2px/tick, starts when boarded, reverses at both ends. One-pixel bob
carries grounded player before input. Downward-only support uses existing4px
minimum overlap, jumping detaches, water death resets boat via drownedReset.
No enemy spawns or active collectibles; gate/jet collision/animation disabled
only in this proof. Flat shores, original2400px route assets preserved elsewhere.

Native approved art embedded1120bytes including masked4planes and word guard.
Per-target Bob history restores before background updates. Draw follows water
synchronization. Immutable lower-mask lookup286720Fast bytes built from actual
waterPatternPen;112-byte copy/frame into1120-byte Chip mask/art allocation.
Two deck heights,16water phases,80x offsets; art bits immutable, old DMA retired
before mutable mask reuse. Same HUD/sprite/display boundaries; no new wake/audio.

Validation: native020 build passes; host actual player/collision tests cover
boarding,1200ticks carry/bob/reversal,jump detach,far-bank landing,water miss/reset.
2293760 lower-mask pixels checked against actual water predicate, including
padding; ordinary route traversal tests still pass. These are not emulator or
performance claims. User020 motion/water overlap/cadence pending.

Current drawer dist/Drowned-Pontoon-020-HD/Drowned-Test (66assets/47refs).
Previous full route and log preserved byte-identically under
 dist/older-builds/20260916-before-pontoon/Drowned-PatchDMA-020-HD.
68release files unchanged. Proof:build/drowned-pontoon/proof.json.
No release/version change. Full level, Spillwing and checkpoint still pending.


2026-09-16. Proposal for user review; no runtime implementation authorized by
this document. Supersedes earlier length estimates only if accepted. Existing
2400px candidate and accepted art remain protected. Names: Drowned Turbines,
Rain Core, Pump Walker, Turbine Crab. Machine finale remains a proposal.

## Direction

One continuous industrial waterworks route, approximately4480px (14 screens).
Target first successful passage4–6minutes, practiced passage2–3minutes; design
targets, not measurements. Difficulty comes from movement decisions, readable
combinations and optional speed routes, not repeated waits or enemy saturation.
Existing precision passage stays approximately its current length. Introduce
new flying enemy on dry land, then combine it with a moving pontoon crossing.
Do not stack flyer attacks onto the current narrow geyser landing by default.

## Route sketch (coordinates provisional)

| Span | Beat | Content |
|---|---|---|
|0–960|Arrival/bypass|Existing introduction, first geyser and permanent-open gate; tune early enemy approach rather than replace approved scene.|
|960–1376|Maintenance deck|Walker elevation changes and water traversal; retain attack/movement variety.|
|1376–2128|Precision supports|Accepted six low/high/higher/low/high/low supports and timed geyser. Short optional high pickup branch; do not require blind landing.|
|2128–2304|Service landing|Breathing space; introduce one flying enemy over safe floor. Proposed single midpoint checkpoint here, separate new state contract.|
|2304–2944|Flooded basin|One metal maintenance pontoon, approximately96px wide. First jump onto it is safe; then sequential flyer passes. Normal crossing8–12seconds target. Optional broken overhead service route for faster advanced crossing.|
|2944–3712|Return to sluices|Second gate: shoot visible panel on approach from raised deck; opening completes during the approach. Third gate: combine a short low/high route with one readable jet rhythm and a ground enemy. Gates open permanently for the attempt.|
|3712–4224|Pressure Governor|Proposed traversal-based machine finale: three regulator locks on different heights. Position, fire in signalled safe windows, move onward. Aim25–45seconds first clear,15–25practiced; no fixed minute-long wave survival.|
|4224–4480|Rain station|Quiet separate station/Bob, final path and Rain Core reward. Same HUD and campaign carry conventions.|

## New enemy: Spillwing (working English name)

Small mechanical turbine drone, distinct horizontal rotor silhouette. Native
cell32x32 starting budget. Cyan/steel body with warm warning lamp, differentiated
from Walker and Crab through silhouette and flight, not a new palette bank.
Approach visible, brief hover/rotor warning, committed shallow swoop, recovery.
No continuous player tracking; no projectiles initially. Two hits starting
proposal. Swoop crosses an accessible player shot lane; no new aiming control.
First teach on dry floor. Basin starts with one active flyer, then two staggered
passes only if020 measurement permits. Fixed encounter order and bounded speed
variants; respawn after leaving encounter, not endless replacements while
standing on pontoon. Existing generic enemy slots remain the starting budget.
Concept review, then native idle/fly/tell/swoop/hit/electric-collapse review,
then integration. No new raster art silently integrated from this proposal.

## Pontoon and speed route

Metal float with drums and worn deck, visually belonging to the pumping plant.
Horizontal fixed-point motion along a fixed track; no physical wave simulation
or vertical camera. Small visual ripple permitted independently of collision.
Starts when boarded, reaches far bank, remains available/returns under explicit
recovery rules. On death/checkpoint retry, reset to boarding bank. Never require
waiting through a full return cycle. Verify player carry, jumping detachment,
landing, knockback, death, reversal and camera interactions before enemies.

Overhead route is optional and harder, with safe visible starts/landings and a
clear rejoin. It may bypass the ride's wait but not skip an essential progression
switch. No gate must be operated from an unseen panel. Gates persist open for
that attempt. Pressure cycles should become reproducible on encounter entry,
then run without tracking player decisions; this is a proposed behavior change,
not a claim about the currently global pressure timer. Reset semantics must be
specified alongside checkpoint and future timing mode.

## Fairness and replay

One primary precision demand plus one secondary threat at a time. Preserve
small pauses between peaks; secrets away from mandatory geyser landing. Three
optional discoveries: early concealed maintenance pocket, high precision spur,
and overhead reservoir reward. No score/pickup farming after death.

Recommend one midpoint checkpoint at service landing. Requires explicit snapshot
of player/score/pickups/gates/enemy seed/hazard state and resettable pontoon;
not merely moving the respawn coordinate. Time-trial mode remains deferred:
future consistent timing, pause/death rules and attempt reset are separate work.
Plan deterministic starts and routes now, no timer/HUD additions now.

## A1200 budget

Retain resident Walker cache and patch DMA working baseline49.88FPS from manual
run (different workload; no guaranteed50Hz). Current Chip free599176bytes.
Extending uncompressed4-plane208px front from2400 to4480 adds216320bytes,
leaving approximately382856 before other changes/allocator differences.
Rear panorama must extend: at quarter scroll max camera4160 needs coverage
through1360px, beyond current1120px; no accidental edge wrap/stretch. Extra
rear width240 at3planes208px is18720raw bytes before padding/guards. Flyer cache,
pontoon, station and loading peaks require a full inventory before final art.
Do not preload every new large frame family into Chip by default. Keep visible
pool bounded and reuse atlas/stages. No storage-driven colour/music reduction.
One level need not mean all world source pixels live in Chip: if needed, measure
Fast-backed region preparation ahead of camera into fixed Chip staging, without
runtime disk access; this is an architectural fallback, not current behavior.
HD/WHDLoad/ADF share presentation. Disk count deferred to actual packaging.

## Execution gates

1. Approve route direction, pontoon/optional fast route and machine-finale
   recommendation. Complete blockout of full path using accepted material;
   test reachable routes, blind jumps, full-run fatigue and checkpoint scope.
2. Isolated short moving-platform proof with existing temporary deck art,
   clearly labelled collision prototype; no new final art or flyer yet.
3. Spillwing concept and animation review, dry-floor encounter, then basin
   combination. Test020 heavy scene and compare matched passage workloads.
4. Gate instances and independent state, final combination run, checkpoint
   snapshot/reset proof and secrets. Avoid accidental synchronized gates.
5. Governor/station/Core art review and integration, original music, full-level
   cadence/memory and repeated death/continue coverage. Only then media planning.

## Research informing the proposal

Nintendo developers discuss how limited options in2D games can make one mistake
cascade, and allowing different ways to overcome challenges:
https://www.nintendo.com/au/news-and-articles/ask-the-developer-vol-11-super-mario-bros-wonder-chapter-3/
Application here is our own inference: optional harder fast route, readable
choices and a recovery point, rather than adding assistance controls.
GDC Celeste level-design talk is linked in this overview (not fully watched):
https://www.gamedeveloper.com/design/video-the-level-design-of-i-celeste-i-
No detailed claims attributed to the unwatched talk.

## User refinement — pontoon waterline reference

User specifies that the float should move partly through water, not appear
perched on top. Reference: Brian the Lion, https://youtu.be/K3Bavupw4Kc?t=82,
from1:22. Web fetch failed; video motion has not been independently inspected.
The following is the proposed translation of the user's explicit requirement,
not a claim about observed frames of that video:

- Show deck and upper flotation body above the waterline; conceal lower hull
  behind the foreground water surface. Avoid a complete dry sprite sitting
  above a blue strip.
- Add a restrained animated bow ripple and trailing wake aligned with travel;
  reuse the established water palette and visual rhythm.
- Keep waterline occlusion within the playfield, above the unchanged HUD.
- Any subtle bob must keep player support visually attached to the deck;
  decide actual collision displacement versus deck-stable animation in the
  moving-platform proof. No decorative deck bob beneath stationary feet.
- Prove draw/restore order and wake cleanup with existing water updates on020.
  The mask/occlusion implementation is still to be designed; no promise of
  real-time water simulation or extra transparency planes.
- Review side-on concept with explicit deck/waterline/submerged hull, then an
  animation preview before integrating final art.

### Reference subsequently inspected through Chrome

YouTube playback succeeded through user-authorized Chrome UI after web fetch
failed. Inspected images around1:25/1:26 and1:30. Rounded log ends project above
the water, while a continuous bright wave crest overlaps their lower silhouette;
at1:30 the same overlap applies to a green floating creature. Wave profile
changes between inspected frames. The clear transferable visual principle is
animated foreground-water occlusion, not merely placing a whole raft above the
surface. This does not establish the original game's rendering implementation,
exact bob amplitude or presence of a separate wake effect. For Drowned, retain
its own water palette and HUD boundary; any bow ripple/wake remains our design
proposal rather than a claimed observation from this reference.

## Pontoon concept v1 available

assets/concept/drowned-pontoon-concept-v1.png: generated material/silhouette and waterline review. Not integrated, not a native palette or animation proof. After concept review, derive native96px-wide art and an offline waterline animation preview before runtime integration. Existing PatchDMA test remains untouched.

## 2026-09-16 — Pontoon native motion review

User approved industrial pontoon concept. New offline conversion/preview:
tools/preview_drowned_pontoon.py and assets/concept/drowned-pontoon-native-v1/.
96x16 cell compiled from approved upper concept silhouette, same exact12-bit
foreground palette. Largest alpha component excludes labels. No new raster
identity. Existing waterPatternPen C function is extracted, compiled and
executed for16frames at25Hz; native water passes in front of lower hull.
Scripted lateral motion and1px bob move the existing player together with deck.
No actual platform collision, carry or landing physics; no020FPS claim.
Animated waterline-5x.gif is offline preview, not emulator capture. No extra
wake yet. Current test drawer and68release files hash-verified unchanged.
User native art/motion review pending; then implement isolated moving-platform
physics and water occlusion with host checks before user runtime test.


## 2026-09-16 — Playable Spillwing dry-floor audition

User approved compact24x24 animation review and asked to continue. Added isolated
SPARKPAW_DROWNED_SPILLWING target: one1HP flyer atx280 over flat dry floor,
patrol160..576, hover120/121,12-tick warning, committed32-tick96px swoop reaching
y164,70-tick re-arm. No homing during swoop. Existing off-camera respawn and
once-only20score preserved. Death24ticks: two hit-flash poses then four electric
collapse poses. Shot bounds mirror body; smaller contact box excludes rotor.

Approved16frame art converted to48x384 left/right SPBM with decoder parity.
24px cache requires byte-aligned source reads (right begins byte3), last
half-word masked and guard zero. Existing32/64px fast path remains unchanged.
Cache23040Chip bytes, no per-frame transformations or additional pool slots.
Temporary compile-only small-enemy cache alias; Crab and Spillwing do NOT yet
coexist. Full integration needs distinct third-family cache/type ownership.
No water/collectibles/machinery in this safe-floor combat audition. Pontoon,
upper route and mixed encounter remain subsequent gates. Existing HUD/audio.

Host sanitized tests: actual flight, mirrored point/sweep/contact bounds,
committed arc,1HP/death frame bounds/score/respawn; actual cache all16frames,
both facings,4planes/mask/zero guards. Existing encounter and full route tests
pass. Candidate and default campaign native builds pass with existing warnings.
No emulator execution or FPS claim. User's established020 configuration remains
the requested test target; native visual/function/cadence acceptance pending.

Current dist/Drowned-Spillwing-020-HD/Drowned-Test:66runtime assets,47 literal
references.68 alpha.8 release hashes unchanged. Previous pontoon drawer and
.uaem preserved under dist/older-builds/20260916-before-spillwing, all files
verified. Proof:build/drowned-spillwing/proof.json. No release/version/commit.

## 2026-09-16 — Spillwing duo challenge candidate

User accepts single flyer visuals/function and finds it easy to hit; requests
multiple flyers with varied heights/speeds and apparently random attacks.
No renderdiag.log exists in single-flyer drawer; no measured FPS acceptance.
SPARKPAW_SPILLWING_PAIR candidate adds second required spawn at400 (first280),
shared dry patrol160..672. Low/high hover120/96; patrol256/384 fixed-point;
swoop768/1024 (3/4px per tick), high arc1.5x depth reaches162 vs low164.
Both1HP, same16frames and23040Chip cache; one extra active Bob, no new cache.
Initial pauses60/103ticks; each independently cycles irregular authored
49..114tick re-arm pauses,12tick warning and32tick committed non-homing arc.
This is deterministic variation, not runtime random targeting. Simultaneous
attacks allowed; safe dry floor before pontoon/mixed-family integration.

Native build and sanitized single/pair/cache tests pass:6000ticks two actors,
height/frame/world bounds, varying attack intervals and nearest swept target.
Current Drowned-Spillwing-020-HD/Drowned-Test replaced through official stager;
previous single preserved byte-identically,66assets/47refs,68release hashes
unchanged. Proof build/drowned-spillwing/proof-pair.json. User duo gameplay and
020 cadence pending. No release or emulator execution.

## 2026-09-16 — Pontoon combat candidate + duo50FPS evidence

User accepts dry duo, requests1/2 flyer variation during crossing. Duo log
verified against proof-pair executable:1993intervals/all one-field,50.00FPS,
0ownership violations over39.86s;23shots,2kills,14jumps,4hurt. Chip607816free.
Stable complete copy testresults/Unassigned-Drowned-spillwing-pair-50fps.log/TXT.
This is dry-floor acceptance, not water/boat performance.

Added drowned-ferry target combining approved pontoon and Spillwing families.
Same800px basin160..960,96px boat160..864 and carry/reversal/water masks.
Three required1HP spatial encounters: lowx328 region200..432; highx584
region448..736; lowx856 region704..960. Last low can overlap high; killing
changes pressure. No scripted visible despawn or random mid-dive homing.
Reuses one23040Chip enemy cache, existing pool; no new art or upper route.
Small-family compile alias still excludes Crab coexistence until later type
integration. Corrected macro interaction: water animation/collision enabled
for combined ferry, pontoon no-enemy bypass disabled only for ferry. Gate
remains disabled. Diagnostic header now identifies actual ferry3spawn layout.

Native build and host tests pass: real player/pontoon1200ticks carry/bob/
reversal, jump/exit/water-reset; all3 enemies activate and dive, max2 visible
on tested ride, each crosses real deck shot lane. Existing duo tests and
2293760water mask pixels pass. Native combat feel/cadence not yet measured.

Current dist/Drowned-Ferry-020-HD/Drowned-Test,67assets/48compiled refs.
68alpha.8release hashes unchanged. Previous duo including log archived and
hash-verified in older-builds/20260916-before-ferry. Proof:build/drowned-ferry/proof.json.
No release/version/commit or automatic emulator launch. Next user020 combined
combat/water test; high alternate route and full-level family integration later.

## 2026-09-16 — Five-flyer ferry candidate

User played ferry v1 and asks for more enemies. Verified full log against v1
executable:2883intervals/2865one/18two/0three+,49.68FPS over58.02s,
0ownership violations.26shots,6kills,16jumps,6hurt,2water;604808Chip free.
Preserved testresults/Unassigned-Drowned-ferry-three-49-68fps.log/TXT.

Ferry v2 has5 required low/high/low/high/low flyers at328/520/704/848/952,
patrol regions200..432/384..672/552..808/704..992/800..1056. More density
in second half and bank exit. Same AI/art/HP/cache/boat/water; no new family
allocation. Existing pool4 bounds actor count, but up to4 can be visible if
not killed. This is a heavier cadence candidate, not a measured50FPS claim.
Host real1200tick ride proves all5 activated/attacked/crossed deck shot line,
peak4 visible, boarding/carry/reversal/jump/exit/reset preserved. Native build
passes. New diagnostic header identifiesv2/5spawn.

Updated dist/Drowned-Ferry-020-HD/Drowned-Test through official stager,
67assets/48refs;68release hashes unchanged. Previous v1 drawer and log
archived and byte-verified. Proof build/drowned-ferry/proof-five.json.
User difficulty/020cadence pending. No release or emulator launch.

## 2026-09-16 — Visible-only player shots, ferry v3 candidate

User lost all lives, finds5flyers more fun but jump/fire spam and offscreen
kills too easy; suggests overhead blocking structures. Log verified against
v2 executable:1716intervals/1661one/55two/0three+,48.44FPS over35.42s,
0ownership violations,48shots/8kills/17jump requests/3hurt/3water. Complete
post_run footer despite game-over. Preserved testresults/Unassigned-Drowned-ferry-five-48-44fps.log/TXT.
Cadence lower than prior3flyer49.68; different workload/deaths, not causal
proof. Current candidate must remeasure; do not normalize the slowdown.

Fixed shared Drowned projectile update, SPARKPAW_DROWNED_SLICE guard: clip
enemy sweep to camera..camera+319, reject entirely unseen ranges, retire
player bullets as leading edge exits. Last visible segment still hits; Bob
drawn history retained for restoration. Pixel reference applies same gate.
Hostile projectiles unchanged. Applies to all Drowned enemy types including
Pump Walker/Crab when rebuilt; original Level1 and published releases unchanged.
No enemy AI or art change. Fewer surviving offscreen bullets may lower work,
but no FPS gain claimed. V3 diagnostic header identifies visible_player_shots.

Native build, actual optimized/pixel projectile tests (both edges,scrolling,
visible fragments,offscreen miss,restore history,hostile lifetime), real ferry
physics/5enemy coverage and player damage tests pass. Updated same
Drowned-Ferry-020-HD/Drowned-Test;67assets/48refs/68release hashes preserved.
V2 and log archived intact, proof build/drowned-ferry/proof-visible.json.
User visual/cadence gate pending. No release or emulator launch.

Proposed next geometry: one low overhead maintenance platform with a solid
underside, blocking repeated high jumps and shots through steel; then open
water recovery space. Reuse accepted industrial materials, preview layout
before integrating. First evaluate visible-only shots and cadence; avoid
masking that change by simultaneously adding obstacles/enemy load.

## 2026-09-16 — Ferry transfer platform + six diamonds

User accepts visible-only shots and requests central platform: leave boat,
collect1/2diamonds, boat travels beneath, jump back on; diamonds over water.
Prior v3 log verified:1485intervals/1464one/21two/0three+,49.30FPS,
0ownership violations,30.12s. Complete stable copy:
testresults/Unassigned-Drowned-ferry-visible-49-30fps.log/TXT. Mixed workload,
not controlled evidence of FPS gain from retiring offscreen bullets.

Ferryv4 adds96x16 solid deck x528..624,y160..176 using accepted platform
material (no new art family). Gap to deck189/190 permits boat, not crouched
player. Six diamonds:280,140;408,132;544,130;592,130;744,132;856,140.
Enables authored collectibles only in ferry, keeps other isolated proof
sentinels inactive. Two deck diamonds pickup/reset persistence tested.

Found carry bypassed wall collision. Pontoon horizontal carry now calls the
existing player moveX through playerCarryHorizontal, retaining existing
side/head/crouch collision. No pushing through the new obstacle. Same speed,
bob/water masks. Flight checks proposed32tick arc against solid tiles once
when arming; blocked attempts recheck after8ticks, no mid-dive homing. Prevents
Spillwings flying through steel; performance cost needs native measurement.

Host tests actual player/collision: shots block at both deck edges, standing
and crouching cannot be carried through, complete boat/deck/boat route proven
(launch atboat450,40ticks right, wait forboat610, jump/right lands53ticks later).
3000enemy ticks show no cell/solid overlap. Existing flat-basin1200tick carry,
all5enemy coverage and visible-shot/damage tests pass. Flat baseline test
explicitly loads original pontoon map; new platform test loads ferry map.
Native build passes. Static layout inspected; timing/art/cadence user gate pending.

Updated Drowned-Ferry-020-HD/Drowned-Test,67assets/48refs/68release hashes
preserved. V3 and its log archived byte-identically; proof-platform.json in
build/drowned-ferry. No release/version/commit or emulator launch.

## 2026-09-16 — Precision upper-route candidate

User explicitly wants a hard tiny-platform route BEFORE and AFTER middle,
while retaining middle-to-pontoon exit. Added9single16x16 solid ledges at
(64,160),(160,128),(256,96),(352,112),(448,96),(560,96),(672,128),
(768,96),(864,128). Starts on departure bank, upper minimumy96 preserves
jump headroom. Same accepted platform materials, camera/HUD/palette. Middle
528..624,y160 remains branch; upper can descend there and rejoin at672,128.
Existing6diamonds plus3 at256,66;560,66;864,98. Five flyers unchanged count.

Rejected early low over-water ledges: they blocked pontoon carry and original
transfer. Repositioned initial ascent onto shore; upper ledge over middle
raised to96 to leave its walking/afsprong space free. Trials at80 clipped
jump into ceiling; final minimum96. Final host tests prove each real-physics
upper link, descending to middle, joining upper from middle, and complete
boat/middle/boat path (launchboat420,40ticks right, walk to middle x590,
short right-input jump back; landing53ticks). Tests check collectibles,
wall/shot/crouch carry,3000enemy ticks no solid overlap. Patrol now reverses
before solid tiles, in addition to existing pre-checked committed swoops.
No renderer/cache enlargement or new enemy family. Native build passes.

Latest supplied middle-onlyv4 log matches proof-platform:1485intervals,
1419one/66two/0three+,47.87FPS,0ownership violations;22shots,5kills.
Preserved testresults/Unassigned-Drowned-ferry-middle-platform.log/TXT.
This is below cadence target; do not normalize it. Upper route performance
and feel pending; after route review prioritize targeted cost investigation,
particularly new flight geometry tests alongside water/collectible workload.

Current Drowned-Ferry-020-HD/Drowned-Test(v5):67assets/48refs,68release hashes
unchanged. V4+log archived intact and verified. Proof:build/drowned-ferry/proof-upper.json.
No release/version/commit or automatic emulator. User tests precision and
middle-to-boat choice; full-level progression/third cache family still pending.

## 2026-09-16 — Upper route restricted to second half (v6)

User rejects full upper route and requests mandatory pontoon first half.
Removed6ledges before/over middle and their2bonus diamonds. Keep three16px
ledges at672,128 /768,96 /864,128, and7total diamonds. Start on pontoon,
choose upper or boat at unchanged middle deck528..624,y160. All5flyers and
art/physics/visible shots unchanged. Generated assets rebuilt from base so
removed tiles leave no collision or painted residue.

Host actual physics proves empty first-half air, middle-to-upper and all
remaining links/final bank; boat/middle/boat still passes. Native build passes.
Previous full-route log verified:2420intervals/2298one/122two/0three+,
47.60FPS over50.84s,0ownership violations. Stable complete log preserved as
testresults/Unassigned-Drowned-upper-full-47-60fps.log/TXT. FPS remains below
target; no improvement claimed from layout reduction. Performance work still
needed after this user-directed geometry gate.

Current same Drowned-Ferry-020-HD/Drowned-Test(v6),67assets/48refs,
68release hashes preserved. Rejectedv5+log archived byte-identically.
Proof:build/drowned-ferry/proof-upper-short.json. User route/FPS pending;
no release/version/commit or automatic emulator launch.

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
