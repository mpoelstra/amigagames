# Drowned Turbines — music and final section, 19 September 2026

## 2026-09-22 — Tail diamonds and both flower species staged

User played full5120 route: broadly accepts it, requests diamonds after ferry
and integration of both flower concepts, A behind player/B in front, sparse.
Added12 tail diamonds (3264..4816),45total in existing48-slot pool; maximum4
tail diamonds per320px viewport. Includes three on Walker deck, one on Governor
deck, rest across dry approach/inter-machine spaces/quiet station approach.
Generated collectible header AND conservative column-top bounds include them.

Integrated12 small flower clumps:6purple/violet A behind player,6cream B in
front of feet. Native16pen palette unchanged. Approved coral concept maps to
violet; preserve blossom highlights during reduction instead of averaging away.
Clumps16..18px wide/11..13px high, rooted at201, near existing trees/shrubs.
No animation, no new Chip buffers. Six small foreground silhouettes reuse
inactive player stage;81-byte64px lookup skips distant occluders. Original
trees/shrubs/station group preserved; foreground source bitmap dimensions same.

Native executable288432bytes. ASAN/UBSAN actual collectible init/collect/
repeat/respawn tests pass;45active/12new and dry roots checked. Full-world sprite
staging/cache/blink tests and all-pixel mask oracle pass with bucket lookup,
both gates/reset/hazards, collision-byte and station-pixel parity pass.
Native flower preview reviewed at build/drowned-full/flowers-native.png.
No measured FPS claim. Musicv5 enabled, no renderdiag.

Current dist/Drowned-Level-020-HD/Drowned-Test staged73assets/55refs;
68official alpha.8 files unchanged. Previous full route preserved byte-exact
at dist/older-builds/Drowned-Level-020-H-old-004012. Proof in
build/drowned-full/proof.json; previous proof saved as proof-before-flowers.json.
Next: user's020 visual/gameplay review. No release/version/commit.

## 2026-09-22 — Full5120 route with varied vegetation staged

User accepted v12 vegetation/materials and authorized whole-level integration;
requested variety between both tree types and varied shrubs. Current manual
candidate dist/Drowned-Level-020-HD/Drowned-Test now starts the full route.
5120px total; prior route retained to3248, accepted1872px finale shifted+3248.
Core4888, station4872, endcamera4800 with40px player margin; final gate4528.
19spawn sites/20surfaces; original four active enemy slots unchanged.
New art across ground/platforms,14 extra trees (7spruce/7fir), variable sizes
and reflection,10 varied lower-leg shrubs. Existing station group exactpixels
preserved. Precision piers/ferry water kept clear; no vegetation animation.
Flowers concept v1 remains unapproved/unintegrated.

Full guard combines both gate interactions, jet hazards, checkpoint and Governor.
Ten canonical patches reuse610-byte Chip stage. Multiple static silhouettes
mask inactive attached player stages, preserving cache/master ownership.
Joined Spillwing clearance table retained byte-identical before finale; actual
geometry reference used in finale. No blanket loss of prior ferry precompute.
Longer foreground adds166400Chip bitmap bytes, rear extension31200:197600 total
(~193KiB) compared with3520px joined art. No additional animation/Chip staging.
No measured FPS/memory-headroom claim; manual020 boot/play remains pending.

Native build286004bytes. Host ASAN/UBSAN tests: actual full/isolated camera/Core,
Governor states, both gates and saved/unsaved reset, both original jet hazards,
checkpoint cases, whole-world sprite staging/cache/blink, per-pixel foreground
mask oracle. Collision bytes through3247 unchanged; station-group pixel parity;
dry tree placement and rear coverage checked. git diff --check clean.
Staged73assets/55literalrefs;68official alpha.8 files byte-identical. v12 archived
intact at dist/older-builds/Drowned-Level-020-H-old-002512. Proof:
build/drowned-full/proof.json. Musicv5 on; renderdiag off. No autoemulator,
campaign integration/release/version/commit. Next user full020 review; flowers
separate concept review and FPS investigation if needed.

## 2026-09-21 — Governor v12 vegetation integrated, user test pending

User played v11: ground approximately accepted, FPS investigation deferred.
Approved vegetation v1 now integrated into the quiet gate-to-station corridor:
64x136 spruce at1416 and48x96 fir at1552 behind player,32x20 shrub at1504
in front of lower legs. All rooted at201, overlapping the ground top by1px.
Static16pen foreground art; no extra Chip stage or animation.80byte immutable
shrub silhouette clips only the inactive player sprite stage during overlap.
Original gate clipping preserved; source sprites remain immutable.

Native Governor build passed. Actual sprite staging tested against a pixel
oracle under ASAN/UBSAN: both directions, cache reuse, blinking, inactive-stage
ownership and restoring after leaving shrub. Camera, Governor and Core visibility
host tests passed. No measured FPS claim; manual020 visual acceptance pending.
Current dist/Drowned-Level-020-HD/Drowned-Test is v12. Prior drawer archived
byte-exact at dist/older-builds/Drowned-Level-020-H-old-232853;68 official release
files unchanged. Music v5 on, renderdiag off. Whole-level distribution/joining
still follows acceptance of this focused art pass.

## 2026-09-21 — Ground cleanup staged; vegetation concept v1

User rejects v10 irregular ground/endcap pattern and orphan copperpost, prefers
large trees behind player and small trees/shrubs infront rooted toground. V11
removes extra post art and entire additional masking path/buffer; original gate
clipping remains. Floor uses one complete32px panel with regular joins and
continuous rail, not partial endcaps. Nativebuild and sprite-clipping hosttests
pass. Staged73assets/55refs,68officialreleasefiles unchanged, previousdrawer
archived byteexact. Ground/post screenshots catalogued Drowned-v10-ground-and-post-rejected-a/b.

First imagegen request hitusage limit; following user continuation succeeded.
Saved assets/concept/drowned-vegetation-v1.png and exactpromptTXT. Four distinct
spruce/fir/sapling/lowfern-heather silhouettes, rooted lowmoss bases. Review pending.
Source has soft backing/halo despite transparency request: remove before native
conversion, simplify fine detail into nativeclusters/currentpalette. No vegetation
runtime yet. No additionalanimation requested. Next native grounded preview then
safe foreground plant clipping only lowerlegs; avoid hazards/landings. Wholelevel
art/joining still follows finalslice acceptance. No release/commit/FPSclaim.


## 2026-09-21 — Governor v10 approved platform kit native proof

User approves platform-kit-v2 and authorizes static art first, one foreground
passage; later whole-level consistency/joining. build_governor_platforms.py crops
approved source, flood-removes edge-connected blue backing, nativepalette maps,
composes broad192x16 and144x16decks and footed32px supports at existing144/152tops.
Floor stays200..207, new quiet64x8 material tile. Reviewed native host views
platform-kit-native.png and foreground-passage-native.png in build/drowned-governor.
One18x56 static copper-bearing post1456,144 in quiet postgate corridor. Existing
gate sprite mask reused via translated coordinates for secondpost; mask352normal
bytes, no additional Chip buffer or animation. Extra sprite copying/clipping only
near posts or on leaving clipped cached poses. No measured FPS claim.

Expanded actual sprite-stage sanitizer test covers both configurations, second
post pixel oracle, movement/directions/cache/blink/inactive-stage and unchanged
masters. Existing camera/Governor tests pass. Native compile passes existing
warnings.73assets/55refs staged,68release files unchanged; previousdrawer archived
byteexact. User020 art/passage/smoothness pending. Narrowpier art and extra trees
not yet integrated; wholelevel application follows focused acceptance. Geometry,
mechanics, music unchanged. Current drawer Drowned-Level-020-HD/Drowned-Test.


## 2026-09-21 — Consistent platform kit concept v2

User accepts camera and station/boiler quality, rejects cheap repeated platform
fragments. Authorizes whole-level material consistency and occasional conifer
accents, with focused final-section art proof FIRST, then full-route application
and joining. Built-in imagegen material reference station-v3 creates concept
assets/concept/drowned-platform-kit-v2.png + exactpromptTXT. Broad steel walkway,
substantial copper-bearing posts, three narrow pier heights, ground ends/middle,
conifer. Concept pending approval, no runtime or dist changes. Preserve geometry;
raised end caps must flatten to collision top; ground source needs adaptation to
8px floor strip, not new tall ground. Native16pen conversion/masks and scale review
next, then short user020 proof before spreading kit across whole level.


## 2026-09-21 — Governor v9 camera keeps player visible

User v8 screenshot shows station but Sparkpaw offscreen. Confirmed source fault:
unconditional wanted1552 atplayer1376 outran stopped player. Existing v8test even
asserted that wrong outcome. Added desired-camera upper bound playerX-40 in
Governor only, retains5px ease and1552end cap. Player can stop or reverse without
camera leaving them behind. Test now holds300frames at trigger, pauses at every
3px approach step, reverses through trigger, and tests uninterrupted3px approach;
requires40px player margin and complete station framing before earliestpickup.
Sanitized actual-function test passes, native build passes existingwarnings.
73assets/55refs,68officialreleasefiles unchanged; prior drawer byteexact archive
recorded in proof-checkpoint.json. Screenshot/TXT preserved as
Drowned-Governor-v8-camera-outruns-player. User020 stop/reverse/pickup pending.
Other v8 platform/nozzle fixes not independently user-accepted yet. No FPSclaim.


## 2026-09-21 — Governor v8 end composition/nozzles/platform fixes

User supplied5screenshots, catalogued Drowned-Governor-v7-*.png/TXT. Confirmed
station too close to gate and Core delayed by camera>=1200visibility threshold;
geyser static lips omitted; user reports partial first-platform stance, exact
motion/root cause not established from stills. Station/Core moved+192 (1624/1640),
end bounds1872, camera endpoint1552 triggeredplayer1376. Same5px ease. Actual camera
simulation3px/frame reaches final composition before earliest pickup overlap.
Core visibility now normal viewport clipping pluscompletedGovernor, no camera
threshold. Pickup x1624..1656; jumpheightunchanged. Restored both approved static
lip assets x832/1088,y194 under animated patches. First platform top136->144,
16pxsolid cap/art/upperWalker surface aligned; samejump links/horizontal geometry.
This is a platform correction candidate, not proven cause from supplied stills.

Native build, actual camera/pickup and visibility/governor host tests pass;
73assets/55refs staged,68officialrelease files unchanged. Previous evidence and
build archived byteexact; proof-checkpoint.json recordsinventory. No emulatorrun,
FPSclaim,release or newart. User020 landing/sideapproach/nozzles/endcamera pending.


## 2026-09-21 — Rain Core packaging correction verified

First v7 staging accidentally selected production stormstone-core.spbm: stager's
additional-runtime-dir intentionally never overrides production assets. External
parity check caught mismatch. Fixed source to dedicated compile-guarded
PROGDIR:assets/runtime/rain-core.spbm (both load paths), generator emits own name.
Never modify canonical Level1 Core. Rebuilt/restaged with73assets/55literal refs;
all68release files unchanged. Incorrect staged drawer also archived intact.
Correct finalCore header/dimensions and mask byte parity verified against original,
colours differ deliberately; no new cache allocation. Full ending still pending
user020 camera/visual/pickup/results/replay test. Current proof-checkpoint.json
contains corrected inventory; previous entry's72asset/override statement superseded.


## 2026-09-21 — Native Rain station ending integrated (v7)

User approved sourcev3 station/tree and requested runtime integration. New
build_rain_station.py crops alpha, preserves aspect in224x176 envelope, native
Drowned16pens, thresholdedalpha128; grounds building at200 atx1432. Static art
added to existing3520-wide foreground, no extra Chipbuffer. Tree extends above
building. Blue/turquoise remap of original18 Core frames retains64x48 and original
mask bytes, existing cache allocation. Candidate-only stormstone-core.spbm override.
Host composition at build/drowned-governor/rain-end-scene.png, inspected.

Focused playable bound now1680; camera eases max5px/update to1360 once playerx1380
(beyond gatepost1364), clamped1360 otherwise. Core centred1448,hover112 aboveleft
pedestal. Visible only after all locks and43tick gateopening complete, camera>=1200.
Pickup gated by completion and box1432..1464,y120..159 (jump), existing50tick burst,
worldflash, sound, generic results/replay path. No campaign CONTINUE in this target.
No inherited secret1up. Station gauge/windows static in this pass; no separate
lens activation animation yet. Full joined level/campaign mapping remains later.

Actual-C camera/pickup tests, Governor completion/offset tests, renderer visibility
across3521positions in Level1/unfinishedDrowned/Governor pass. Core mask preserved,
SPBMforeground decode parity checked. Native020compile passes existing warnings.
No emulator launched, no FPS numbers claimed; musicv5, diagnosticsOFF. Staged
Drowned-Level-020-HD/Drowned-Test,72assets/55refs;68release files unchanged. Previous
v6drawer/evidence archived byte-exact per proof-checkpoint.json. User camera,
station/Core visuals, results/replay and020smoothness pending. No release/commit.


## 2026-09-21 — Unique Rain station required

User rejects recoloured Level1 station. Reuse technical static-foreground/CoreBob
architecture only, not station design. New source based on lower panel of
assets/concept/drowned-finale-station-v1.png: corrugated steel shelter, rain tank,
copper gutter/pipes, mast/rain gauge, warm amber windows against cool backdrop.
Target same approximate200x145 native envelope including mast. Separate empty
Core pedestal so the Core remains independently animated and collectible.
New imagegen source review required before runtime integration. Earlier
recolour review preserved as rejected; dist unchanged.

## 2026-09-21 — Rain station native review, before integration

User accepts Walker approach and authorizes ending/camera/station/Core. Produced
assets/concept/drowned-rain-station-v1 via tools/preview_rain_station.py using
existing Level1 waystation source and18-frame Core family, current Drowned16pens.
Station200x145max, grounded200; Core same64x48 footprint/hover112, blue/turquoise
remap preserving silhouettes, idle and pickup frames. Host PNG/GIF only; not
emulator evidence. User requested new/changed art review before integration;
review pending. Existing v6 dist remains intact, no new runtime staged.

Confirmed Level1 station is baked foreground (not separate Bob); Core is Bob.
Camera eases toward fixed final composition at max5px/update. Proposed focused
proof extends gate exit into quiet final320px composition, station centred,
camera transition after player clears nearpost1346..1364 plus margin. Player
retains control. Need implement stage-specific camera endpoint, Core visibility
and pickup guard gated by completed Governor, and Rain results identity. Prevent
inherited secret1up and unintended campaign CONTINUE. SameHUD, no global palette
swap, no new stationBob/Chipbuffer required. Full-route mapping remains later.


## 2026-09-21 — Governor v6 compact aggressive Walker approach

User approves placing two aggressive Pump Walkers BEFORE Governor rather than
extending combat after reward gate. Prepended448px to focused prototype, arena
translated intact; collision bounds now1408px, asset allocation stays3520px.
Ground Walker surface96..416y200; upper192..384y136 with16px solid deck leaving
48px underneath. Same approved platform materials. Governor entrance, art,
patches, header, near-post mask, enemies and hazard/projectile coordinates shift448.
Governor APIs translate world coordinates; tests explicitly wrap local arena
assertions at new world offset. No production Level1/normal Drowned tuning change.

Walker speeds256/224 fixedpoint versus ordinary48/96/192; initial shot delays65/95,
subsequent90 versus150ticks. Existing shot telegraph/HP preserved. Four same-surface
jump links reuse320,-900,60 ballistic motion; all20launch pixels reach landing
windows in simulation. Two required Walker spawn sites added to four existing
arena sites, same four-slot active pool. No wave gate or mandatory kills.
Upper/lower platform transfers deferred; this first pass uses bounded patrol hops.
Native build, actual-C Governor offset/phase/hit/exit tests, Spillwing tests pass.
No native play or FPS claim. Musicv5, diagnosticsOFF.72assets/55refs staged;
68official release files unchanged. Previous drawer/proof archived byte-exact.
User020 aggression/smoothness pending. Station/RainCore and final camera scene
remain next, then full-route/campaign integration; no release/commit/push.


## 2026-09-21 — Governor v5 materials and enemy pressure

User accepts reused gate, rejects plain elevated platform and requests more enemies,
then explicitly2or3Spillwings. Reused approved precision-route deck/support pixels
at unchanged432..576/y152..168 collision cap, with same support positions. No new
palette or raster concept. Two Crabs now patrol112..336 and576..816; two Spillwings
use low576..816 and high288..704 flights with existing alternating speed/pause AI.
Existing four-slot enemy pool unchanged; all existing respawn policies retained.
Governor bypasses full-route precomputed clearance (wrong geometry here) and uses
existing actual-collision reference flight checks for deck/closedgate correctness.
This increases flight-check CPU work; no FPS claim without user test. Musicv5 and
machine cycles unchanged. Native build, Governor and Spillwing host tests pass.
Staged Drowned-Level-020-HD/Drowned-Test:72assets/55refs;68release files unchanged.
Previous drawer archived byte-exact, path in proof-checkpoint.json. Screenshot
preserved at testresults/Drowned-Governor-v4-platform-style.png with sidecar.
User020 difficulty/readability/performance pending; no diagnostics/log in musicbuild.


## 2026-09-21 — Governor v4 reuses existing sluice exit

User reports v3 reasonably smooth; screenshot rejects the placeholder arrow exit.
Preserved PNG and sidecar in testresults/Drowned-Governor-v3-placeholder-exit.
V4 copies accepted gate from slice x768 to832, reuses original14 gate atlas states
and80x61 transfer (same610-byte Chip stage). Last machine completion starts43tick
lift. Solid header x864..896 y120..136 remains after opening; leaf collision follows
original lift table. Near-post sprite mask moved from834 to898 only in Governor.
Header overlap guard enabled with relocated bounds; original level constants stay
first for offline asset generator parsing. No direct shot activation of finale gate.
Musicv5/mechanics unchanged. Native build and actual-C completion/lift/header tests
pass. Dist staged with72 assets/55 refs,68release files unchanged; prior drawer
archived byte-exact (path in build/drowned-joined/proof-checkpoint.json).
No FPS logging or measured FPS claim; user visual acceptance pending.


## 2026-09-21 — Governor native art integrated (user test pending)

User approved native tower preview. Isolated Governor v3 now stages the reviewed
steel/copper towers as static foreground with animated 32x64 port patches.
Port assembly lowered12px: visible low port y168..184 includes normal standing
projectile centre177. Seven opening poses and seven closing poses per damage
state; three hit lamps and disabled indication. Existing 100/75/55-tick exposure,
45-warning/55-vent, raised platform and one Turbine Crab unchanged. Platform now
uses steel/copper material. Exit is still a simple prototype shutter; pipe
connections, final station/Rain Core and full-route integration remain future work.

Native build and sanitized actual-C phase/hit/visibility/sweep/reset tests pass;
art indices verified for all three locks/damage stages over200ticks each.
40x1024 linked art bytes versus24x1024 before (+16KiB); same610-byte Chip staging
buffer, no new Chip allocation. Static foreground SPBM decode parity passes.
No emulator run or measured FPS claim. Musicv5 enabled, diagnostics/profiler OFF:
no renderdiag.log or mouse-save expectation. User020 visual/performance pending.

Current manual drawer: dist/Drowned-Level-020-HD/Drowned-Test. Stager verified
72 assets,55 executable references, all68 official release files unchanged.
Previous drawer/evidence preserved byte-exact at
 dist/older-builds/Drowned-Level-020-H-old-091958.
Proof: build/drowned-joined/proof-checkpoint.json; prior proof preserved as
proof-before-governor-native.json. No release, commit or push.


Status: music review v1 produced; finale proposal awaiting the user's machine
versus mobile-boss choice. No runtime changes or new test build in this step.
Uses DROWNED_TURBINES_PLAN and the later DROWNED_FULL_LEVEL_PROPOSAL; actual
joined route is3520px, basin2400..3200. Historical route coordinates are not
instructions to move accepted precision/boat sections.

## Music first

Undertow Circuit follows the original144BPM/64bar proposal. Review the complete
106.58sec host render and27sec theme extract in music/drowned. Three tracks,
fourth empty; score17468Fastbytes,bank19798Chipbytes, module37266bytes.
No native replay or FPS claim; this first composition needs listening approval.

Integration sequence after audition:
1. Add a selected gameplay-track descriptor at load, retaining existing Level1
   and Stormrail bytes and their APIs where possible; validate Drowned bank and
   score sizes explicitly. No new section lookup in IRQ/mixer hot paths.
2. Add Drowned's checkpoint and distinct Pump Walker sound to mixed-effect
   mappings. Checkpoint currently calls direct Paula1 playGameplaySample, which
   conflicts with music. Preserve all event priorities/gains/cooldowns and
   plasma's reserved logical voice. Audit every new event before enabling music.
3. Link existing three-channel ptplayer and two-effect AUD3 mixer in a dedicated
   joined music test target. Life loss continues song; replay restarts; pause,
   results, stop/unload retain established ownership and vector restoration.
4. Host lifecycle/mixer tests, native build, complete runtime manifest, same
   020 route with music and combat. Measure Chip/Fast after loading and compare
   minimal cadence with the protected SFX-only build. No automatic emulator.
5. Only then extend Soundtest's track count/cache in HD/WHDLoad and campaign/media
   selection. Same score/samples across all editions; disk count decided later.

## Final route proposal

Retain0..3200 exactly. Extend world to4480px by developing existing shore and
adding960px; no long blank walk after the ferry. Coordinates below are design
anchors, to be validated by actual jump/camera/collision tests before runtime.

| Span | Purpose | Proposed play |
|---|---|---|
|3200–3328|Recovery and readable approach|Dry landing, two visible diamonds, view of the next gate; no surprise flyer on landing.|
|3328–3712|Return to sluices|Two independent permanent-open gates, first operated from approach; second with low/high platforms, one timed jet and one ground enemy. No simultaneous flyer swarm.|
|3712–4224|Pressure Governor, if selected|Three regulator locks, low → middle → low, existing run/jump/shoot controls.|
|4224–4480|Rain weather station|Quiet dry path, a separate station Bob, Rain Core reveal and pickup, results.|

Gate panel must be on-screen and reachable by the actual projectile; no unseen
activation. Reuse approved solid lintel, head collision, rear-left/front-right
layering and permanently opened state. Independent instance IDs required;
existing gate remains intact. Camera should show landing/next active panel.

Recommended recovery: a second instance of the approved beacon before the
Governor, at the safe approach around3680. Keep one active checkpoint ID, latest
activated wins. Never force replaying the full boat section after learning a
finale pattern. This is a proposed extension, not an already implemented feature.

## Pressure Governor encounter (proposal, not yet approved)

A fixed industrial regulator with three small shuttered pressure locks;
mostly static art, only small shutters, gauges and jets animated. Do not use a
huge animated Bob or another generic enemy-pool slot. Steel, copper and cyan
match approved gates; the body should read as a solid machine, not scaffolding.

Sequential low/middle/low target positions. Each lock needs three normal hits.
Only the indicated lock accepts hits. Amber lamp and mechanical cue warn for
at least30ticks; a jet vents; cyan release phase exposes the target. Safe dry
waiting perch always available, including when entering the arena late in a
cycle. Teach one cycle at the first lock; the third can shorten recovery, never
warning. No invulnerable countdown after all hits, add waves or music-beat input.
Target25–45seconds first clear,15–25practiced: supersedes original60–90sec arena.
Timings are tuning hypotheses, not a promise before projectile/jump simulation.

After final lock: jets stop, all target hits disable, small mechanism animation
settles and exit opens permanently. Player remains controllable. No Rain Core
emerges from the machine. A short dry walk leads to the separate rain station;
lens illuminates, Core appears, pickup happens once and drives the existing
results sequence with Rain identity. No inherited Lightning Core or secret1up.

State proposal: dormant → warning → vent → exposed → next lock → settling →
complete. No global real-time scripting or per-frame memory allocation.
Death before completion resets encounter locks/phase at finale checkpoint,
preserves prior pickups/score/opened route gates; finale score only once on
completion. Death after completion must not reactivate hazards or duplicate
reward. Fresh attempt clears all stage flags; resident replay restores the
section-entry snapshot, not a death snapshot. Core flag and result award have
separate one-shot guards. Store chapter/carry explicitly at campaign integration.

## Art review and performance gates

Before runtime integration, review a coherent concept showing: solid Governor,
three readable lock elevations, safe perch, and separate small weather station
with a rain lens/weather vane. Station remains an independently layered Bob as
in Level1. Rain Core has a distinct drop/lens silhouette within existing64x48
presentation. Concept first, then native-palette motion review, then integration.
No new generated art has been approved by this document.

Retain4+3 AGA split, palette ownership, HUD boundary and inactive-buffer writes.
World extension alone adds at least99840bytes to a4-plane960x208 canonical
foreground before file packing/allocator overhead; inspect actual frontClean
allocation domain, other full-width maps and rear buffers before budgeting.
Reuse small patch staging; cap concurrently animated locks and jet regions.
CPU-side state and music score in Fast, DMA assets in Chip; no trade of colours
or frame quality for disk space. Current414KB free Chip is not a finale budget.

Gates in order: music audition → mixed gameplay music proof; finale selection →
concept art approval → real-physics route/lock timing proof → focused final-area
020 test → connected level test → campaign/results/replay/media verification.
Preserve current SFX-only optimized drawer until a self-contained music candidate
passes packaging checks. No release/version change/commit/push in this phase.

## 2026-09-20 — Undertow Circuit v4 integrated in Drowned music test

User approves v4 for now and requests in-level integration before confirmation
runs. New make target drowned-music links existing three-channel ptplayer and
AUD3 two-effect mixer with joined level. Rain score17468Fastbytes,bank21622Chip
plus224Chip mixer buffers (allocator/code/effect-copy overhead additional).
Compile-selected Drowned track load validates exact sizes and signature; other
tracks retain prior paths/sizes. Drowned effect4 uses pump-shot.raw; checkpoint
appended16 with priority10,volume58,cooldown32 and AUDIO_MIX dispatch before any
Paula1 direct write. Existing IDs/priorities preserved. No per-frame track lookup.

Music build deliberately omits SPARKPAW_RENDER_DIAGNOSTIC and all CIA profiling;
level1_audio.c ownership guard stays intact. NO renderdiag.log or LMB log save in
this drawer. Runtime smoothness is subjective until compatible cadence-only
instrumentation is separately designed. Life loss retains existing song lifecycle;
user should verify checkpoint, damage/shots, water, pause/resume and upper route.

Native020 build passes (initial unresolved gameStormrailActive in isolated target
fixed by compile-time FALSE selection before staging). Host actual backend
lifecycle runs both ordinary and Drowned variants, all allocation failures,
checkpoint mapping/priority/cooldown,24starts/stops each and ownership cleanup.
Mixer oracle40000operations/13225tails passes. No emulator launched.

Staged dist/Drowned-Level-020-HD/Drowned-Test with72assets/55literalrefs;
v4 split files exact byte parity.68alpha.8releasefiles unchanged. Prior SFX-only
cadence drawer/log archived byte-identically in older-builds/Drowned-Level-020-H-old-111058.
Proof-checkpoint.json identifies music/no diagnostics; prior proof-before-music-v4.json.
README explicitly says reset/stop after playing, no mouse log or Workbench promise.
Music/art/gameplay native approval and memory peaks pending user run. Finale not
implemented; proposal remains in DROWNED_ENDGAME_IMPLEMENTATION.md. No release,
version change,commit or push.

## 2026-09-20 — Pressure Governor isolated gameplay proof staged

User finds art unclear and explicitly requests playable test, then says continue.
Built compile-guarded SPARKPAW_DROWNED_GOVERNOR test, make drowned-governor;
starts at ordinary x36 in a short dry arena bounded0..960. Existing3520 asset
allocation reused for isolation; real final layout/campaign not integrated.
Targets x256/y136,512/y88,768/y136; raised solid platform432..576,y152..168.
Ground200. Numbered32x64 test panels with closed slats, amber warning, exposed
cyan bullseye,3remaining-hit lamps and done checkmark. These are procedural
readability mockups, not approved finale pixelart. Same music v5/HUD/player/jet.

Each lock:45tickwarning,55tickvent,100tickexposed,3hits, sequential. Future/closed
panels consume bullets but take no damage. Full panel must be on-screen with
margin. Jets x384 (first2locks) and640(lastlock), existing64pxjet visual; safe
dry floor around jets. Third lock stops jets and opens solid exit864..896.
No Core,station,results,new SFX or enemy waves. Walking through E completes the
proof informally. Death resets locks and returns start; normal lives retained.
No actual new beacon/checkpoint needed within this short prototype.

Renderer reuses six compact patches and existing610Chipstage; generated
4*6*4*256=24576bytes normal-program art table. Full-column bounds0 for this
prototype prevent incorrect inherited upper-row skips. Static foreground has
matching raised platform. Collision conditional uses exact prototype geometry;
spawns/collectibles disabled and old water/boat/checkpoint moved or remain out
of reachable area. Main full-level paths behind guard remain preserved.

Actual-state ASan/UBSan test covers closed/exposed hits, hitcount/stages,both
sweep directions,offscreen rejection,warning/vent,exit/reset and solid bounds.
Native build passes; no emulator or actual movement/FPS acceptance claimed.
Artwork reviewed: corrected prototype base to darksteel pen8 (initial preview
used orange pen2). Generator remains deterministic, host pixels only.

Current dist/Drowned-Level-020-HD/Drowned-Test is ISOLATED Governor,not fulllevel.
72assets/55refs verified;68alpha.8files unchanged. Previous fulllevel+v5 drawer
preserved byte-identically at older-builds/Drowned-Level-020-H-old-184853.
Proof-before-governor.json preserves prior music state. No renderdiag.log or
leftmouse save; music owns CIA. User judge readability/timing/jumps/fun,stop/reset.
No release/version/commit/push. Concept machine art and real weatherstation remain
pending; do not mistake this explicit test-graphics proof for final art approval.

## 2026-09-20 — Governor v2 challenge pass

User reports initial mechanic works but asks whether it becomes difficult,
perhaps enemies, and authorizes continuation. Tightened exposed windows from
100ticks each to100/75/55(2.0/1.5/1.1seconds). Same45warning+55vent; no surprise
warning reduction. Adds one Turbine Crab spawn624..640, surface576..816,y200,
existing respawn policy. No inherited traversal links or other route spawns.
Governor encounter now arbitrates real enemy/panel sweeps by nearest hit and
routes damage to enemies before panel fallback. All outside proof compileguards
unchanged. Same test art, music,geometry and exit. Not final encounter acceptance.

Actual-C state tests cover each window boundary/cycle and reset; actual encounter
callbacks tested with enemy shim for left/right nearest ordering and enemy damage.
Native020 build passes. Staged same Drowned-Level-020-HD/Drowned-Test;
72assets/55refs,68releasefiles unchanged. Prior v1/log drawer preserved byte-exact
at older-builds/Drowned-Level-020-H-old-192207. Proof-before-governor-v2.json.
No renderdiag/noLMBsave. User judge pressure/fairness/clarity and enemy interference;
no automatic emulator,no measuredFPS claim,no release/commit/push.
