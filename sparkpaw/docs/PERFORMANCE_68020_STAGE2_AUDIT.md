# Sparkpaw Stage 2 whole-codebase 68020 audit

Status: completed living audit on the alpha.45 Stage 5L/H7 baseline. The broad
whole-codebase inventory and initial ranking are complete, and multiple
measured candidates have since been accepted or rejected below. It remains the
authority for selecting the next isolated prototype; production diagnostics
remain compiled out.

## Measurement basis

- Production compiler: VBCC `+aos68k -O2 -cpu=68020 -notmpfile` with the H7
  release flags.
- Production SHA-256 before profiler-only work:
  `b8c578c536ddc60728bcf4c4124a8a569a2cfbc01b5da91ac61263255f9ae544`.
- Assembly generated for all fifteen production C translation units with those
  exact flags. Production `performance_profile.c` reduces to one assembly line,
  confirming that diagnostics remain compiled out.
- Existing CIA evidence: Stage 5L FS-UAE/68020 26.38 effective FPS; Bob pass
  average 24,357 ticks; player average 4,303 ticks. These averages are not yet
  enough to rank intermittent versus continuous costs.
- The diagnostic profiler now retains a bounded 1,024-sample window and emits
  median, p95, average and maximum. Production remains byte-identical after
  this profiler-only change.

## Runtime call-path inventory

| Area | Runtime frequency and bounded work | Memory/domain and current evidence |
| --- | --- | --- |
| `main.c:main` | One game update, inactive Copper patch, Bob pass and publish wait per completed tick. Raster waits are polling loops. | CPU control plus custom-register reads. A missed field extends the same tick; it is not uncontrolled frameskipping. |
| `game.c:gameUpdate` | Once per tick. Dispatches input/player, up to four active enemies, 48 collectible tests, eight projectile slots, contacts, audio and animation. Early returns occur for splash, death and reset. | Mostly Fast/static state; indirect callbacks enter collision, audio and enemy-hit paths. |
| `player.c:playerUpdatePhysics` | Once per ordinary tick. `moveX` advances per crossed pixel and scans a vertical span; `moveY` advances per crossed pixel and scans the 24-pixel sole. `canStand` scans multiple horizontal spans. | Collision map is CPU memory. Generated 68020 assembly contains signed long divides for animation `/10`, `%250`, `%180` and `/100` turn braking, plus fixed/variable shifts. Scene-sensitive candidate. |
| `enemies.c` | Pool size four every tick, plus bounded spawn-state scans over up to 24 authored spawns. Contact and projectile-hit paths rescan the active pool. | Fast/static state and indirect `solidAt`/projectile callbacks. Assembly contains `divul` for RNG modulo, signed `/5`, variable shifts and indirect calls. Needs scene-separated CIA scopes. |
| `projectiles.c` | Eight slots scanned per tick. Every active projectile sweeps each crossed X pixel, calling `solidAt`; player shots additionally call `enemiesHitProjectile`, which scans up to four enemies. | Fast/static projectile/enemy/collision state. Indirect calls are visible in VBCC output. Maximum work occurs with six player and two enemy projectiles. |
| `collectibles.c` | All 48 entries tested every ordinary tick regardless of camera or active density. Renderer also scans all 48 for synchronization and Bob work. | Fast/static state; fixed bounded scan is proven, cost still unmeasured. |
| `audio.c` | `audioUpdate` once on normal ticks and selected reset/early-return paths. Logical play requests are event-driven; Paula starts are gated by hardware state, cooldown and priority. | Samples reside in Chip RAM; request state is CPU memory; starts write Paula registers. Requests and actual starts are not yet counted separately. |
| `renderer.c:rendererUpdateGameplay` | Once per tick. Copies the complete 1,536-byte active Copper list, stages player sprite data, patches HUD pointers and scroll. | Copper and sprite stages are Chip RAM; masters are Fast RAM. Three mandatory known candidates live here. |
| Rolling ring | Usually no entering-column copy; camera crossing a 16px boundary can copy up to 208 rows x four planes x three physical copies x clean/display = 4,992 two-byte `CopyMem` calls. | Fast/Chip source-to-Chip targets with heavy call overhead and bus traffic; periodic hitch candidate, not continuous average cost. |
| Hardware sprite staging | Two 64px attached channels copy `SPRITE_WORDS*2` each tick, about 1,600 bytes total, even if frame/facing did not change. | Fast master to alternating Chip stages. Control words still need per-tick updates. |
| HUD | State comparison every tick. On change, up to three patches run plane-by-plane and finish with `WaitBlit`; unchanged HUD returns early. | Chip atlas to Chip double buffers. Event-driven rather than continuous cost. |
| Bob pass | Restore/draw scans projectile, four enemy, 48 collectible, water and splash families. Plane operations are repeatedly serialized with `WaitBlit`, followed by a final wait. | Predominantly Chip-to-Chip Blitter work plus CPU setup and fixed seven-plane display contention. Existing aggregate measurement is the largest known continuous section. |
| Loading/cache preparation | Asset reads/conversions, pattern construction and initial ring copies occur before takeover/after `CHARGING`. CPU-only sprite masters are allocated Fast and released after conversion. | Static inspection finds no asset-file loading or pattern construction in the ordinary gameplay loop. Dynamic ring/water/collectible maintenance remains runtime work. |
| Input/platform | Joystick reads are direct; keyboard event acknowledgement deliberately polls two raster-line changes only when serial input is pending. `platformWaitBlit` busy-polls custom state. | CIA/custom-register access. Keyboard acknowledgement can be a rare latency spike and needs a call/event count. |

## Assembly findings requiring measurement

- VBCC correctly emits direct 68020 integer multiply/divide instructions; no
  compiler runtime helper was found for the inspected arithmetic.
- Player animation/state code contains real `divs.l`/`divsl.l`, not optimized
  shifts, for divisors 10, 100, 180 and 250.
- Enemy RNG modulo emits `divul.l`; enemy animation contains signed division by
  five and multiple indirect calls.
- Projectile sweep and enemy simulation retain indirect callback calls in their
  inner paths.
- Collision tile addressing uses a constant multiply by 192 after coordinate
  reduction. The pixel-span callers, rather than `collisionSolidAt` alone, set
  the maximum call count.
- Renderer assembly is large because it includes preparation and runtime code;
  source/translation-unit size is not treated as runtime cost.

## Required next measurements

Add diagnostic-only CIA scopes for input, animation, enemy update/contact,
projectile update/contact, audio update, Copper-list copy, ring roll, dynamic
ring synchronization, sprite image staging, HUD update, each Bob restore/draw
family and final `WaitBlit`. Count audio logical requests and actual Paula
channel starts separately. Then collect matched idle, run, sprint/jump,
Strider, overlap, projectile and water/collectible scenes before ranking.

The final ranked table must retain the five mandated candidates and may not be
used to start an optimization until these cross-subsystem measurements are
comparable.

## Broad candidate table before dynamic ranking

This is the complete comparison set for the first profiler run. Its order is
deliberately provisional: bounded source/assembly evidence can establish work
and risk, but only the matched CIA samples can rank continuous costs against
periodic spikes. No row is authorization to optimize yet.

| Candidate: file and function | Triggering scene | Frequency and bound | Current cost evidence | Evidence versus hypothesis | Potential win | Risk | Required regression tests |
| --- | --- | --- | --- | --- | --- | --- | --- |
| `renderer.c:prototypeRollRing` entering-column copy | Crossing each 16px camera boundary | Zero on most ticks; at a boundary up to 4,992 calls copying two bytes | Bound proven from 208 rows x 4 planes x 3 physical copies x clean/display; CIA leaf scope added | Call count and Chip traffic proven; actual hitch size pending | Very high periodic-hitch reduction if calls can be coalesced safely | High: rolling ownership, guards and clean/display coherence | H7 presentation, both directions across repeated boundaries, collision/draw order, 68020 p95/max |
| `renderer.c:rendererUpdateGameplay` inactive Copper copy | Every completed game tick | One complete approximately 1,536-byte copy | Size and frequency proven; CIA `copper_copy` added | Continuous Chip write proven; share of tick pending | Medium/high continuous saving | Medium/high: atomic publication and inactive-list ownership are immutable | H7 seam, Copper generation/publication log, long bidirectional scroll, missed deadlines |
| `renderer.c:stagePlayerHardwareSprites` image staging | Every completed tick | Approximately 1,600 image bytes plus control words | Size/frequency proven; CIA `sprite_stage` added | Recopy on unchanged frame/facing proven; cache benefit pending | Medium continuous saving during unchanged animation frames | Medium: attached 64px pair, alternating stages and position controls | Every player animation/facing transition, screen edges, seam, long run |
| `renderer.c:rendererRunBobPass` restore/draw families | Every presented frame; cost scales with visible actors | Plane-per-plane operations with repeated `WaitBlit`; all families bounded by pools | Existing aggregate average 24,357 CIA ticks; leaf restore/draw/final-wait scopes added | Largest measured aggregate; CPU setup versus Blitter wait versus bus contention pending | Very high if safe overlap exists | Very high: restore order, union bounds, masks and Chip contention | All Bob families alone/overlapped, two Striders, maximum shots, water, H7 presentation |
| `player.c:moveX`, `moveY`, `canStand`, `playerUpdatePhysics` | All gameplay; worst while jumping/running around geometry | Per crossed pixel; each vertical step scans a 24px sole; clearance/span scans are also per pixel | Existing player average 4,303 CIA ticks; direct `divs` and collision loops visible in assembly | Aggregate is measured; contribution of collision versus state arithmetic pending | High continuous saving if equivalent span/tile queries replace repeated point calls | High: exact collision, ledges, crouch and landing contracts | Full collision suite plus jump/run/crouch/ceiling/ledge/water scenes |
| `collectibles.c:collectiblesCollect` and renderer collectible synchronization | Every ordinary tick and Bob pass | Two independent full scans of all 48 slots regardless of camera/active count | Loop bounds proven; CIA game and Bob-family scopes added | Redundant/offscreen scanning proven; absolute cost pending | Low/medium, potentially continuous | Low/medium: collection order and restore history | All collectibles, camera edges, life award, overlapping Bob restores |
| `projectiles.c:projectilesUpdate` plus enemy hit callback | Every ordinary tick | Eight slots; every active shot sweeps crossed X pixels; player shots may scan four enemies per pixel | Bounds and indirect calls proven in assembly; update/contact scopes added | Work shape proven; worst-scene cost pending | Medium/high under maximum shots | Medium/high: tunnelling prevention and hit ordering | Existing projectile tests, max shots, walls, floor/elevated beetles, Striders |
| `enemies.c:enemiesUpdate`, contact and hit scans | Every ordinary tick | Four active slots plus bounded authored-spawn scan; contact/hit rescan pool | Direct divide/modulo, variable shifts and callbacks proven; update/contact scopes added | Instruction mix proven; density-dependent cost pending | Medium | Medium/high: deterministic RNG, spawn order and enemy animation | First/two/overlapping Striders, beetles, seeded replay, contact and projectile hits |
| `main.c` raster waits and publication loop | Every completed tick/frame | Fixed raster gates; missed field can add an entire display period | Existing cadence evidence: FS-UAE/68020 about 26.38 effective FPS; publish-wait scope retained | Deadline loss proven globally; which producer caused it pending | No direct saving unless producer work is shortened; high diagnostic value | Very high if cadence semantics change, so no early rewrite | 50Hz display service, atomic publication, long-run frame counts, no frameskipping |
| `hud.c:hudUpdate` patch Blits | State changes only | Comparisons every tick; up to three seven-plane patches, serialized by waits | Event-driven bounds proven; CIA `hud_update` added | Likely low median but possible event spike; pending | Low average, possible p95 improvement | Medium: exact HUD content and double-buffer publication | Health/lives/diamonds changes, H7 seam, rapid damage/collect events |
| `audio.c` request scheduling and Paula starts | Requests are event-driven; update on normal and selected early-return paths | Small fixed state, but logical requests may be suppressed or replayed independently of starts | Separate per-event request/start counters added; CIA `audio_update` added | Real-HD missing/repeated SFX reported; CPU cost and scheduling cause both pending | Probably low FPS gain, high correctness value | Medium: priority, cooldown and channel ownership | Rapid shots/hits/collects/water; compare requests, starts and missed updates |
| `platform_amiga.c:platformReadInput` keyboard acknowledgement | Every tick for joystick; raster polling only when serial data is pending | Direct register reads; rare acknowledgement waits for two raster changes | Control flow proven; CIA `input` added | Likely cheap median with rare spike; pending | Low average, possible maximum reduction | High if hardware acknowledgement timing changes | Joystick, keyboard, simultaneous inputs, real hardware if changed |
| Loading/cache work in `assets.c`, renderer preparation and title setup | Startup/transition only | File reads, allocation, unpacking and conversion are outside ordinary gameplay | Call-path inspection finds no asset I/O or full conversion after CHARGING | Absence from normal loop proven; dynamic ring/cache maintenance remains separately measured | No gameplay FPS win from optimizing confirmed loading-only work | Low for isolated loading work, but out of current hotpath scope | HD and ADF loading, memory reports; only needed if later changed |

### Decision rule after the profiler log

Rank first by total CIA ticks per game tick/presented frame, then use p95 and
maximum to expose boundary/event hitches. Separate CPU work from time spent in
`WaitBlit` and from Chip-bus-heavy copies. The first prototype must improve a
measured row without changing the H7 contracts; architectural source splitting
is a separate byte-identical operation and cannot count as a performance win.

## First matched profiler comparison

The supplied mixed-gameplay runs validate the profiler and are sufficiently
long to rank the broad continuous costs. They do not replace later matched
scene-specific before/after tests for the selected prototype.

| Measurement | FS-UAE/68030 HD | FS-UAE/68020 HD |
| --- | ---: | ---: |
| Frames / cadence intervals | 1,171 / 1,170 | 901 / 900 |
| Effective cadence | 49.91 FPS | 27.57 FPS |
| One / two / three-plus fields | 1,168 / 2 / 0 | 242 / 584 / 74 |
| Game update average | 249 ticks | 6,909 ticks |
| Playerphysics average | 126 ticks | 3,840 ticks |
| Bob pass average | 3,700 ticks | 29,574 ticks |
| Compact-target preparation average | 748 ticks | 24,509 ticks |
| Dynamic-ring synchronization average | 512 ticks | 23,689 ticks |
| Enemy draw average | 1,441 ticks | 1,702 ticks |
| Entering-column ring roll p95 / maximum | 1,679 / 3,314 ticks | 3,669 / 53,999 ticks |

### Ranked conclusions from the first comparison

1. `prototypeSynchronizeDynamic` is the dominant continuous 68020 cost. Its
   animated water/collectible synchronization performs many tiny canonical
   rectangle copies into the inactive target. This is measured evidence, not
   merely the earlier entering-column hypothesis.
2. `prototypeRollTarget` remains the dominant intermittent spike. Its low
   median but 53,999-tick maximum matches the bounded thousands of two-byte
   `CopyMem` calls when entering columns are populated.
3. Playerphysics/collision is the largest non-renderer CPU candidate in this
   workload, averaging 3,840 ticks and reaching 9,306.
4. Enemy Bob draw/restore remains real continuous Blitter/Chip work, but on
   68020 it is far smaller than dynamic target synchronization. The 68030 run
   had hidden that relationship by making CPU copies disproportionately cheap.
5. Copper copy and player sprite staging are continuous but together average
   only 878 ticks on 68020. They remain valid later low-risk candidates, not
   the first optimization.
6. Projectiles, enemies, collectibles, HUD, input and audio are not competitive
   with the leading renderer and playerphysics costs in this mixed run.

The first isolated optimization prototype should therefore address only the
small-copy structure inside dynamic inactive-target synchronization. It must
not change ring geometry, animation, collectible positions, water, draw order,
the H7 seam operation or atomic Copper publication. Before/after measurements
must use the same 68030 presentation gate and 68020 workload.

## Candidate 1 result: direct dynamic word copy

The candidate reads each aligned canonical source word once and writes it
directly to the three clean plus three display ring destinations. It replaces
six repeated small `CopyMem` calls and repeated source reads without changing
which rectangles synchronize or when they are eligible.

| Dynamic synchronization | A reference 68020 | B word copy 68020 | Change |
| --- | ---: | ---: | ---: |
| Median | 23,423 | 9,708 | -58.6% |
| p95 | 46,510 | 18,892 | -59.4% |
| Average | 22,433 | 9,578 | -57.3% |
| Maximum | 64,999 | 23,410 | -64.0% |
| Bob-pass average | 28,097 | 15,452 | -45.0% |

The user reports no visible difference or glitches in B on either FS-UAE/68030
or FS-UAE/68020. The unmatched whole-run cadence is 27.14 FPS for A and 26.34
FPS for B, so do not claim a total-FPS increase from these user-driven scenes.
The scoped cause is nevertheless strongly improved and retained as proven.
Overall 68020 cadence remains rejected.

The next structural renderer candidate removes canonical-world collectible
animation from rolling synchronization. The reference restores/draws each
visible hovering diamond into the canonical world, then CPU-copies its changed
rectangle into three physical copies of the inactive target. Candidate B keeps
the canonical background diamond-free, stores collectible history per target,
restores directly from the canonical base and draws the diamond onto only the
inactive target after rolling. Position, hover phase, palette, Bob dimensions
and collectible/enemy draw order remain unchanged; water is not modified.
Histories outside a newly rolled target window are discarded rather than
mapped onto unrelated ring slots. Gate traversal in both directions, camera
edges, collection/removal, untouched diamonds, HUD count and reload first on
FS-UAE/68030.

The first supplied FS-UAE/68030 H1 run rejects B visually: diamonds alternate
rapidly between visible and invisible while their hover phase continues. Copper
ownership remains clean at zero violations. H1 nevertheless demonstrates the
potential: `ring_dynamic` average falls 91 to 1 CIA tick and complete Bob-pass
average falls 3575 to 3081 (-13.8%). The cause is target history surviving a
roll even when entering background columns overwrite the corresponding
diamond pixels. H2 invalidates only histories whose rectangle intersects the
exact entering strip before rolling, then redraws normally afterward. An
exhaustive host test compares this interval predicate against a per-pixel
reference. H1 is rejected; H2 requires a fresh FS-UAE/68030 presentation gate.

The supplied FS-UAE/68030 H2 run remains visually rejected: B diamonds still
flicker and visibly tremble between Y positions, while A is correct and Copper
ownership remains zero. H2's Bob-pass average is 3223 versus A's 3711 ticks
(-13.2%), but visual correctness takes precedence. The second cause is the
canonical four-way animation stagger: alternating targets are prepared on
opposite tick parities, so some index groups can update their stored hover Y on
one target but never the other. H3 removes that stagger only for target-local
histories and redraws whenever that target's stored Y differs from desired Y.
This may add diamond Blits; require a fresh 68030 visual and timing gate. H1
and H2 remain rejected.

The supplied FS-UAE/68030 H3 run isolates the remaining correctness failure:
diamonds away from enemies are stable and match A, collection by Sparkpaw is
stable, but a beetle or Strider restore overlapping a diamond makes B flicker.
This is expected from canonical restore order: enemy/projectile/splash restores
copy the diamond-free base over a target-local diamond while collectible
history still marks it drawn. H3 also removes the apparent performance case:
although `ring_dynamic` falls 108 to 1 tick, collectible restore+draw rises from
about 544 to 772 ticks and complete Bob-pass average is 3878 versus A's 3459.
Correct overlap invalidation would require still more redraws. Reject the whole
target-local collectible architecture, remove H1/H2/H3 implementation and
retain canonical collectible synchronization. No MOV or 68020 gate is needed.

The next high-cost CPU experiment targets `ring_dynamic` (about 5,900 CIA
ticks average in the latest 68020 runs). Candidate B combines pairs of aligned
16-bit source/destination words into 32-bit loads and six 32-bit stores, with a
16-bit tail for odd word counts. Motorola documents that the 68020 permits
misaligned data operands but may require additional bus cycles; therefore the
`+2 mod 4` case is explicitly treated as a measured risk rather than assumed
faster. Actual VBCC `-O2 -cpu=68020` assembly contains one `move.l` source read
and six `move.l` destination writes per pair. Await the FS-UAE/68030 visual
gate, then measure the effect on FS-UAE/68020; do not promote from assembly
inspection alone.

Supplied FS-UAE/68030 testing reports both variants visually correct. B lowers
`ring_dynamic` average from 202 to 159 CIA ticks (-21.3%) and p95 from 387 to
322 (-16.8%); median changes 156 to 149. Both runs remain effectively 50 Hz,
each with one two-field interval and zero ownership violations. Different
routes make broad Bob totals and maxima unsuitable for attribution. Proceed to
the matched FS-UAE/68020 gate before promotion.

The supplied FS-UAE/68020 gate rejects the longword route as a target-CPU
optimization. Both variants look and play correctly, but whole-run cadence is
29.92 FPS for A and 29.62 for B. More importantly, `ring_dynamic` median is
6,026 versus 6,021 ticks and p95 is 12,161 versus 12,172: no meaningful tail
improvement. The unmatched average changes 6,066 to 5,780 (-4.7%), while B has
heavier projectile and ring-roll scenes, so that small difference is not a
clean win. The 68030 instruction-count benefit does not survive the 68020
alignment/bus cost. Remove the prototype and retain 16-bit word stores.

The next isolated candidate prunes physical ring writes that no possible
Stage 5L fetch can read. An exhaustive host test over all 512 camera phases
proves the fetch union is physical x=480..1375: only the final 32 pixels of
copy 0, all of copy 1 and the first 352 pixels of copy 2 are reachable. The
candidate applies this invariant to both entering canonical columns and
visible dynamic water/diamond spans. Allocation, ring geometry, Copper
pointers, camera formulas and rendered content remain unchanged. It awaits a
68030 visual edge/wrap gate before any 68020 measurement or promotion.

Supplied FS-UAE/68030 HD testing accepts both all-copies A and fetch-relevant B
visually. Both logs report 50.00 Hz, no multi-field intervals and zero target
ownership violations. Their routes are not cost-matched: B contains heavier
Bob and scroll activity, so its higher ring averages do not prove a regression
or a win. Continue to the intended FS-UAE/68020 A/B cadence gate; no ADF,
Pocket or real-hardware acceptance is inferred.

## Candidate: direct enemy traversal lookup

The traversal direct-lookup candidate passes the supplied FS-UAE/68030 visual
gate: both the linear-search reference and direct-lookup candidate look and
play normally. Both logs record exactly 50.00 FPS and zero ownership
violations. The retained windows do not show a meaningful timing separation:
`enemies` averages 76 ticks in A and 75 in B, while `enemy_parked` averages
21 ticks in both. This fast-68030 result therefore proves presentation only,
not a performance win. The earlier FS-UAE/68020 breakdown measured
`enemy_parked` at 751 ticks average, so proceed to a matched 68020 A/B gate
before retaining or rejecting the lookup.

Supplied FS-UAE/68020 HD testing reports both variants visually normal. The
direct lookup raises effective cadence from 44.35 to 45.09 FPS (+0.74 FPS,
+1.7%). Its targeted `enemy_parked` average falls from 792 to 601 CIA ticks
(-24.1%), p95 from 1,066 to 723 (-32.2%) and maximum from 1,084 to 807
(-25.6%). Complete `enemies` average falls from 2,036 to 1,784 (-12.4%) and
`game_update` average from 4,295 to 4,042 (-5.9%). B's Bob-pass average is
higher (7,722 versus 7,108), so a lighter render workload does not explain the
measured update saving. Retain direct traversal lookup as production default
and preserve the linear scan behind
`SPARKPAW_ENEMY_TRAVERSAL_LINEAR_REFERENCE`. Overall 45.09 FPS remains below
the stock-68020 target; no ADF, Pocket or real-A1200 acceptance is inferred.

The next isolated Bob candidate keeps every restore/draw job, plane order,
`WaitBlit`, pointer, mask, modulo and draw-family order unchanged. The generic
restore and masked-Bob helpers currently rewrite identical BLTCON, first/last
word mask and modulo registers before each of four planes. Candidate B writes
those invariant registers after the first wait only, then changes only the
source/destination pointers and BLTSIZE for later planes. This targets CPU and
custom-register bus setup rather than Blitter execution. Require a busy
FS-UAE/68030 presentation gate before any 68020 timing test.

Supplied FS-UAE/68030 HD testing reports both Bob-setup variants visually
normal and both sustain exactly 50.00 FPS with zero ownership violations.
Hoisted B lowers complete Bob-pass average from 3,368 to 3,184 CIA ticks
(-5.5%). Directly affected projectile restore falls 109 to 85 (-22.0%), enemy
restore 677 to 594 (-12.3%) and enemy draw 1,498 to 1,370 (-8.5%). The retained
windows have different family load—collectible draw is higher in B—so this is
a positive presentation gate rather than production proof. Proceed to matched
FS-UAE/68020 A/B testing; infer no ADF, Pocket or real-hardware acceptance.

Supplied FS-UAE/68020 HD testing also reports both variants visually normal.
Effective cadence is statistically flat and slightly lower in B, 43.98 versus
43.83 FPS, so no average-FPS gain is claimed. Complete Bob-pass average is
also flat at 7,466 versus 7,443 CIA ticks. The deadline-facing tail improves:
Bob p95 falls from 17,282 to 14,752 (-14.6%) and maximum from 21,577 to 20,511
(-4.9%). B carries higher enemy-draw average (1,720 versus 1,603), while its
collectible restore and draw averages fall 879 to 838 and 956 to 893. Retain
the low-risk invariant-register hoist as a p95/hitch reduction, make it the
production default, and preserve the old writes behind
`SPARKPAW_BOB_PER_PLANE_SETUP_REFERENCE`. This does not close overall 68020
performance acceptance; no ADF, Pocket or real-A1200 acceptance is inferred.

The next isolated entering-column candidate specializes only the normal aligned
16px camera roll. The generic routine currently executes a one-iteration word
loop and recomputes row addresses for every one of 208 rows and four planes.
Candidate B instead advances one canonical and one display pointer per row,
loads one word and writes the same three physical ring-copy offsets. Any roll
other than exactly +16 or -16 pixels retains the accepted generic fallback.
Ring geometry, origin, guards, fetch, ownership and content are unchanged.
Require the bidirectional FS-UAE/68030 edge/wrap presentation gate first.

Supplied bidirectional FS-UAE/68030 HD testing reports both variants visually
normal at exactly 50.00 FPS with zero ownership violations. Specialized B
reduces `ring_roll` average from 40 to 12 CIA ticks (-70.0%), p95 from 273 to
91 (-66.7%) and maximum from 343 to 151 (-56.0%). `bob_compact_target` follows
from 43 to 13 average ticks. B's complete Bob-pass is heavier (3,295 versus
2,827), so the scoped win is not caused by a lighter overall scene. Proceed to
the matched FS-UAE/68020 gate; infer no ADF, Pocket or hardware acceptance.

Supplied FS-UAE/68020 HD testing reports both variants visually normal with
zero ownership violations. The closely matched runs raise effective cadence
from 44.47 to 45.55 FPS (+1.08 FPS, +2.4%). Specialized B lowers `ring_roll`
average from 1,691 to 483 CIA ticks (-71.4%), p95 from 9,878 to 2,782 (-71.8%)
and maximum from 9,941 to 2,815 (-71.7%). `bob_compact_target` average falls
1,856 to 654 and p95 9,979 to 2,882; complete Bob-pass p95 falls 16,308 to
9,142 (-43.9%). Player and enemy averages remain closely matched, supporting
causal attribution to the column routine. Retain the specialized aligned-16px
route as production default and preserve all other roll sizes behind the same
generic fallback; expose the old all-generic path through
`SPARKPAW_RING_COLUMN_GENERIC_REFERENCE`. Overall stock-68020 cadence remains
below 50 Hz; no ADF, Pocket or real-A1200 acceptance is inferred.

The next isolated enemy-update candidate gates only the 24-entry respawn-state
scan. Production currently walks every authored spawn each tick even when no
enemy is pending respawn. Candidate B tracks whether any pending state exists;
the original loop runs unchanged from the first respawn request until the last
pending state is cleared, and is otherwise skipped. Parked-Strider simulation,
active slots, spawn priority, RNG, policies and renderer behavior are unchanged.
Require a 68030 lifecycle gate including death, parking and actual reactivation
before measuring the scoped `enemies` cost on 68020.

The supplied long FS-UAE/68030 HD runs report both variants visually and
functionally correct and each contains 19 enemy deaths. Both sustain exactly
50.00 FPS with zero ownership violations. The scoped `enemies` average is 74
CIA ticks for always-scan A and 76 for gated B; median and p95 are identical at
90 and 136. The fixed scan is negligible on this configuration and the added
state branch does not produce a saving. Reject and remove the candidate before
a 68020 gate; no ADF, Pocket or real-hardware acceptance is inferred.

Before another enemy optimization, diagnostic-only CIA scopes now decompose
`enemiesUpdate` into parked persistent simulation, active runtime slots,
respawn maintenance and camera activation. The production build remains
byte-identical. One long mixed FS-UAE/68020 run will rank these subpaths before
selecting any further prototype.

The supplied FS-UAE/68020 breakdown run records 44.40 FPS with zero ownership
violations. `enemiesUpdate` averages 1,992 CIA ticks: parked persistent
simulation contributes 751, camera/spawn activation 598, active slots 256 and
respawn maintenance 257. The largest cost is therefore not onscreen enemy
physics. Assembly inspection of parked Strider simulation shows that every
grounded update linearly scans all 12 authored traversal links, including a
`muls #22` per candidate.

The next isolated candidate precomputes a 25-surface by two-direction Fast-RAM
lookup from the immutable traversal data. Current data has at most one link per
source surface and direction; a duplicate or invalid future layout disables
the table and falls back to the exact linear search. Launch ranges, destination
validation, collision probes and state transitions remain unchanged. Gate all
Strider traversal, parking and reactivation first on FS-UAE/68030.

## Collectible H4 FS-UAE/68030 gate

Corrected H4 removes every history-based redraw suppression: each inactive
target restores all prior diamonds from the diamond-free canonical source,
performs ring/water maintenance, redraws every visible diamond and then draws
enemies. Supplied FS-UAE/68030 HD testing reports A and B visually normal,
including enemy/diamond overlap; the H1--H3 diamond glitches are absent. Both
runs sustain 50.00 FPS with zero ownership violations. B reduces
`ring_dynamic` average from 105 to 1 CIA tick and `bob_compact` from 140 to 43,
but collectible restore+draw rises from 488 to 1,645 and Bob-pass average from
3,527 to 4,675. The workloads differ (22 versus 33 collections), so this is not
a matched total-cost regression proof. There is no 68030 performance case for
B; continue only because canonical dynamic synchronization was orders of
magnitude more expensive in recent 68020 measurements. Require the matched
FS-UAE/68020 A/B result before retaining or rejecting H4. No ADF, Pocket or
real-hardware acceptance is inferred.

The supplied matched FS-UAE/68020 HD H4 gate accepts target-local collectible
composition. Both variants look correct, including enemy/diamond overlap, with
zero ownership violations. Effective cadence rises from 35.31 to 42.15 FPS
(+6.84 FPS, +19.4%); one-field intervals rise from 563/964 (58.4%) to
870/1069 (81.4%). `ring_dynamic` average falls from 3,940 to 101 CIA ticks
(-97.4%), `bob_compact_target` from 5,701 to 1,718 (-69.9%), and Bob-pass
average from 11,086 to 8,499 (-23.3%). Collectible restore+draw rises from
1,177 to 2,429 ticks, but the removed synchronization work dominates. The
workloads are close at 22 versus 23 collections; B also carries higher
projectile load. Retain H4 as the production default and preserve canonical
diamond synchronization behind `SPARKPAW_COLLECTIBLE_CANONICAL_SYNC_REFERENCE`.
Overall stock-68020 cadence remains below 50 Hz. No ADF, Pocket or real-A1200
acceptance is inferred.

Current collectible H5 candidate keeps an unchanged diamond resident in each
target and invalidates it only for a target-origin roll, hover or collection
change, possible water overlap, or overlap with the complete word-rounded
restore footprint of that target's prior enemy, projectile or splash Bobs.
Host contract tests cover horizontal word expansion and exact touching edges.
The accepted H4 always-restore/redraw route remains A; H5 requires the supplied
FS-UAE/68030 overlap/scroll/reload presentation gate before any 68020 timing
test or production decision.

Supplied FS-UAE/68030 HD testing reports both H5 variants visually correct.
Both sustain 50.00 FPS with zero wraps and ownership violations. Dirty B lowers
collectible restore average from 893 to 472 CIA ticks (-47.1%) and draw from
1,235 to 609 (-50.7%), a combined 2,128 to 1,081 (-49.2%). Total Bob-pass is
4,031 versus 4,011, but B's enemy restore+draw load is much higher at 2,615
versus 1,644, masking the scoped saving. Proceed to a matched FS-UAE/68020 A/B
gate; do not promote from the 68030 result alone.

The supplied FS-UAE/68020 HD H5 result rejects promotion. Both variants remain
visually correct with zero ownership violations, but cadence falls from 43.37
to 42.51 FPS. Collectible restore+draw is flat at 2,250 versus 2,259 CIA ticks:
saved draws are consumed by repeatedly scanning prior Bob histories for every
diamond retained in a target, including offscreen residents. H6 makes only the
evidence-directed correction: after mandatory origin/hover/collection checks,
perform overlap scans solely inside the current camera margin. Require a fresh
FS-UAE/68030 presentation gate before 68020 timing; abandon this branch if H6
does not recover scoped cost.

Supplied FS-UAE/68030 HD H6 testing reports both variants visually correct at
50.00 FPS with zero wraps and ownership violations. Visible-only dirty B lowers
collectible restore average from 769 to 442 CIA ticks and draw from 981 to 560,
a combined 1,750 to 1,002 (-42.7%). B simultaneously carries substantially
higher enemy/projectile Bob load, explaining its flat total Bob-pass. Proceed
to the decisive matched FS-UAE/68020 A/B gate; do not promote from this result.

The supplied FS-UAE/68020 HD H6 gate rejects promotion. Both variants are
reported visually correct and both retain zero ownership violations, but B
reduces effective cadence from 44.87 to 44.19 FPS. Its one-field share falls
from 860/971 (88.6%) to 946/1,089 (86.9%). Collectible restore+draw changes
only from 2,335 to 2,272 CIA ticks (-2.7%); restore itself rises from 1,080 to
1,273. Although total Bob-pass average falls from 8,395 to 7,794 (-7.2%), B's
different enemy/projectile workload and worse cadence prevent attributing a
frame-rate win. Reject H5/H6 persistent-diamond scanning and retain the simpler
accepted H4 always-restore/redraw route. No ADF, Pocket or real-hardware
acceptance is inferred.

Collectible H7 keeps H5/H6's useful goal but removes their expensive search
shape. A 192-column Fast-RAM lookup maps 16-pixel world columns directly to at
most two collectible indices. Before restore, prior projectile, enemy and
splash rectangles plus pending water updates mark only intersecting target
diamonds dirty; exact vertical overlap is still checked. Origin, hover and
collection changes remain mandatory invalidations. Unchanged target-local
diamonds therefore persist without scanning 48 diamonds against every Bob
history. H4 remains the reference. Require the FS-UAE/68030 overlap, scroll,
water and reload presentation gate before any 68020 timing claim.

Supplied FS-UAE/68030 HD testing reports H7 A and B visually correct through
the requested overlap, traversal, collection and water workload. Both sustain
50.00 FPS with only one-field intervals and zero ownership violations. Spatial
dirty B lowers collectible restore average from 690 to 507 CIA ticks and draw
from 895 to 645, a combined 1,585 to 1,152 (-27.3%). Complete Bob-pass average
also falls from 4,539 to 4,428 (-2.4%) even though B carries substantially
higher enemy/projectile restore and draw load. Proceed to matched FS-UAE/68020
A/B testing; no ADF, Pocket or real-hardware acceptance is inferred.

The supplied FS-UAE/68020 HD H7 result rejects promotion. Both variants are
reported visually correct with zero ownership violations, but effective
cadence falls from 43.00 FPS in A to 41.50 in B; the one-field share falls
from 813/971 (83.7%) to 766/963 (79.5%). The directly affected collectible
restore+draw total improves only from 2,291 to 2,181 CIA ticks (-4.8%), while
the complete Bob-pass is effectively identical at 9,285 versus 9,284. B does
carry heavier projectile update/draw work, so the top-line loss is not wholly
attributed to H7, but the small scoped saving does not justify its lookup and
dirty-state complexity. Reject H7, remove it, and retain accepted H4. No ADF,
Pocket or real-hardware acceptance is inferred.

The supplied FS-UAE/68020 HD A/B gate reports both variants visually normal
and zero target-ownership violations. B raises effective cadence from 27.45 to
35.81 FPS (+8.36 FPS, +30.5%). One-field intervals rise from 186/1,039 (17.9%)
to 653/1,081 (60.4%); two-field intervals fall from 853 to 428 and wraps from
875 to 466. The workloads are comparable: Bob-pass average is 10,569 versus
10,420 ticks, game update 4,182 versus 4,019, and enemy/collectible/ring costs
are close. The gain therefore comes from reclaiming the phase, not from a
lighter scene. Promote immediate post-publication update as the default and
retain the old wait only behind `SPARKPAW_UPDATE_LINE100_REFERENCE`. Overall
35.81 FPS remains below the 50 Hz target. No ADF, Pocket or real-hardware
acceptance is inferred.

The first 68020 gate measures 29.72 FPS for A and 30.14 FPS for B, a nominal
+0.42 FPS (+1.4%), with both variants visually accepted. This is not cleanly
attributable: B's retained window contains materially heavier ring-roll,
dynamic-ring and Bob activity. Inspection also finds that the first candidate
performed two reachability comparisons for every copied word. H2 therefore
keeps the same fetch-union invariant but splits spans once at logical x=352
and x=480, then selects a straight 2- or 4-store loop outside the inner word
loop. Generated 68020 assembly confirms no reachability checks or helper calls
remain inside those loops. Re-test H2 against the unchanged reference.

The H2 68020 logs measure 28.29 FPS for A and 28.90 for B, but again contain
materially different active ring/Bob distributions and the user notices no
clear cadence difference. Assembly inspection exposes a remaining 68020 cost:
H2's selected loops use scaled indexed destination stores where the reference
uses address-register post-increment. H3 changes only those selected stores to
post-increment pointers; the fetch-union pruning and span split are unchanged.

The supplied H3 FS-UAE/68030 HD gate is visually accepted. Its diagnostic log
reports 2,174/2,174 one-field intervals (50.00 Hz), zero multi-field intervals
and zero target-ownership violations. Proceed to the isolated H3 68020 A/B
measurement; no ADF, Pocket or real-hardware acceptance is claimed.

The H3 68020 gate rejects fetch-union copy pruning. The matched runs measure
28.96 FPS for A and 28.64 FPS for B (-0.32 FPS). Their workloads are close:
1,056 versus 1,031 frames, similar Bob totals, and `ring_dynamic` averages
6,007 versus 6,033 ticks. Candidate `ring_roll` is worse at 3,752 versus 3,340
average ticks and 17,772 versus 16,212 p95. Both variants remain visually
correct, but fewer Chip writes do not overcome selection/addressing costs on
the target CPU. Do not promote this candidate. Return priority to the much
larger serialized Bob restore/draw and wait chain.

Profiler decomposition shows the broad Bob-pass number includes compact-target
ring work and its WaitBlit; enemy restore plus draw is much smaller. A separate
constant candidate is the 1,536-byte active-to-inactive Copper-list CopyMem:
the matched 68020 logs measure it at 518/519 average ticks per update. Both
lists are built completely at setup, while every runtime-varying world pointer,
HUD pointer, player-sprite pointer and scroll word is explicitly patched into
the inactive list before publication. Prototype patch-only publication by
removing just this redundant full-list copy; preserve atomic list ownership.

Supplied FS-UAE/68030 HD testing accepts both Copper variants visually. Both
logs measure 50.00 Hz with only one-field intervals and zero ownership
violations. Patch-only B measures `copper_copy` at zero; A averages only four
ticks on this faster emulator configuration, so cadence remains indistinguish-
able there. Continue to the intended 68020 A/B gate, where prior runs measured
the full copy near 518 ticks. No ADF, Pocket or real-hardware claim follows.

The supplied 68020 gate accepts patch-only B visually and measures a clean
constant win: 29.27 FPS versus 28.51 for A (+0.76 FPS, +2.7%). Complete
`copper_patch` falls from 1,085 to 589 average ticks, while sprite staging,
HUD patching and scroll patching remain effectively identical. The removed
copy itself falls from 519 to 15 profiler-overhead ticks, accounting for about
496 ticks per update. Promote patch-only publication as the production default;
retain the full-copy path only behind `SPARKPAW_COPPER_FULL_COPY_REFERENCE`.
Acceptance is limited to the supplied FS-UAE/68030 and FS-UAE/68020 HD runs.

For the next high-impact prototype, exploit an existing verified allocation
property without changing bitmap geometry: the four planes of the canonical
source and each rolling clean/display target are contiguous with exactly one
plane-size stride. A full-height 16px entering column can therefore traverse
all four planes in one Blit. Candidate B replaces 4,992 CPU word transfers with
six A-to-D Blits (clean/display times three physical copies), each 832 rows by
one word. It validates all five bitmap layouts at runtime and falls back to the
accepted CPU loop if any plane is not contiguous. No Copper modulo, origin,
ring geometry, Bob order, art or gameplay contract changes.

The first 68030 run is visually clean but does not exercise the candidate:
`canonical_multiplane_blit=0` proves the strict contiguity guard selected the
CPU fallback. Do not treat this as Blitter-route acceptance. Log the expected
plane size and the 0->1, 1->2 and 2->3 address deltas for the source and four
rolling bitmaps before deciding whether a stride-aware version is possible.

The supplied layout probe shows every rolling target fully contiguous at
39,936 bytes per plane. The source uses 79,872 bytes per plane; planes 1->2 and
2->3 are contiguous, while plane 0 is separately allocated (357,192-byte
delta). H3 therefore uses two Blits per physical destination: plane 0 alone and
planes 1..3 as one contiguous 624-row operation. This makes twelve Blits for
the six destinations. It eliminates CPU destination stores but rereads source
data per destination, so Chip-bus cost may outweigh the CPU saving; require
measurement rather than assuming a win.

The H3 FS-UAE/68030 run activates the candidate (`canonical_multiplane_blit=1`)
and rejects it before a 68020 gate. `ring_roll` worsens from 103 to 604 average
ticks, p95 from 526 to 3,162 and maximum from 644 to 3,208. Broad Bob-pass
average rises from 2,565 to 3,186 ticks. Both variants remain at 50.00 Hz with
zero ownership violations, but twelve serialized Blits reread the canonical
source for every destination; the CPU loop reads each word once and fans it
out to six stores. Remove the prototype. Replicated ring writes are not a good
Blitter offload unless source reuse can be preserved.

Preserve source reuse in the next candidate by taking one post-initial-diamond
canonical snapshot in Fast RAM. Entering-column loops keep their existing one
read plus six writes, but the read leaves the Chip bus. When a rolled span
intersects either water region or a diamond patch, invalidate that target's
dynamic metadata so the existing dynamic sync overwrites the static snapshot
with current water/collectible state. Host tests cover exact-touch and one-pixel
overlap boundaries. Fast memory cost is four canonical plane allocations;
rolling geometry and all Chip display targets remain unchanged.

Supplied FS-UAE/68030 testing reports both variants visually correct at 50.00
Hz with zero ownership violations. The matched FS-UAE/68020 scoped result
rejects the candidate: `ring_roll` average is 3,011 ticks for the Chip-source
reference and 3,016 for the Fast-source candidate; p95 is 16,266 versus 16,339.
Overall cadence differs because the played scenes differ (30.37 versus 28.77
FPS), so it is not used for attribution. The necessary six Chip writes and
display-bus pressure dominate; an extra approximately 312 KiB Fast allocation
has no measurable benefit. Remove the prototype and keep the Chip-source path.

The next isolated candidate attacks those six writes structurally. Target-local
Bob history now retains both the physical ring destination X and canonical
world-source X. Candidate B restores old Bobs directly from the canonical clean
world into the inactive display ring, omits both target-local clean bitmaps and
writes entering/dynamic background data only to display. This should remove
approximately 312 KiB of Chip allocation and halve canonical-to-target stores.
It does not change ring geometry, draw order, Bob bounds, animation or Copper
publication. The first gate is FS-UAE/68030 presentation across enemies,
projectiles, diamonds, water, reload and 512-pixel direction changes.

Supplied FS-UAE/68030 testing reports both variants visually correct at 50.00
Hz with zero ownership violations. Candidate B raises prepared-peak free Chip
RAM from 465,376 to 784,864 bytes, an exact 319,488-byte saving. Scoped average
costs fall for `ring_roll` from 70 to 43 ticks, `ring_dynamic` from 153 to 97
and `bob_compact_target` from 226 to 142. Broad Bob totals are not attributed
because B exercised more enemy/projectile work. Proceed to matched FS-UAE/68020
testing; do not promote before that cadence and presentation gate.

Supplied FS-UAE/68020 testing also reports both variants visually correct with
zero ownership violations. The directly affected averages fall consistently:
`ring_roll` 3,077 to 1,828 ticks (-40.6%), `ring_dynamic` 5,944 to 3,614
(-39.2%), `bob_compact_target` 9,103 to 5,523 (-39.3%) and the complete Bob
pass 14,238 to 10,515 (-26.2%). Overall cadence is 29.62 versus 27.77 FPS, but
candidate B crosses more ring boundaries (729 versus 590 wraps), so that
unmatched top-line number is not attributed. Its worst cadence interval is two
fields rather than A's three. Promote canonical restore/display-only targets
as production default; retain the old allocation behind
`SPARKPAW_TARGET_CLEAN_REFERENCE`. Overall 68020 cadence remains rejected.

The final production-default FS-UAE/68030 HD gate is visually accepted by the
user. Its log records 1,147 one-field intervals, one two-field interval, no
three-plus interval, 49.95 effective FPS and zero ownership violations.
Prepared-peak memory is 785,872 bytes free Chip with a 784,624-byte largest
block. This accepts the display-only route for supplied FS-UAE/68030 and
FS-UAE/68020 HD testing only; no ADF, Pocket or real-hardware acceptance is
inferred.

The next isolated CPU candidate removes the unconditional complete `Enemy`
structure copy from every loaded slot on every tick. Persistent spawn state is
not read while `loadedSlot` owns a runtime enemy; candidate B copies the current
state only at the camera-unload transition. Death policy flags remain updated
in place and respawn initialization does not consume the stale enemy payload.
The first gate covers patrol, firing, traversal, death, camera parking and
reactivation on FS-UAE/68030 before any 68020 timing claim.

Supplied FS-UAE/68030 testing reports both variants visually correct at 50.00
Hz with zero ownership violations. The scoped `enemies` average is 66 ticks in
A and 67 in B, so no saving is claimed on the fast 68030. Proceed to one
FS-UAE/68020 gate because its cost of a complete structure copy differs; reject
and remove B unless that scoped measurement falls clearly.

Supplied FS-UAE/68020 testing reports both variants visually normal, including
enemy parking/reactivation. Candidate B lowers the scoped `enemies` average
from 1,860 to 1,762 ticks (-5.3%), median from 1,848 to 1,736 (-6.1%) and
`game_update` average from 3,865 to 3,707 (-4.1%). Top-line cadence is 27.48
versus 29.00 FPS, but the unmatched routes prevent assigning all +1.52 FPS to
this change. Retain copy-on-unload as production default and preserve the old
route behind `SPARKPAW_ENEMY_COPY_EVERY_TICK_REFERENCE`. Overall 68020 cadence
remains rejected.

The next isolated collision candidate precomputes the immutable hazard status
of all 3,072 world columns at load time. Floor/support checks then replace the
repeated `levelHazardColumnAt()` call and water/dry-gap boundary scans with one
Fast/BSS byte read. The cache costs 3 KiB outside Chip RAM. A host test compares
all columns against the exact two water and two dry-gap intervals. First gate
ordinary floor, edges, water death/reload, enemy support and projectiles on
FS-UAE/68030 before measuring 68020.

Supplied FS-UAE/68030 HD testing reports both hazard variants visually and
functionally correct. Both sustain exactly 50.00 FPS with every interval one
field. B lowers `game_update` average from 211 to 192 CIA ticks, but projectile
activity is not matched, so the full difference cannot be assigned to the
cache. `player` average changes 41 to 39 and `enemies` 88 to 86 ticks. Treat
this as a successful presentation gate, not a proven performance result;
measure a matched FS-UAE/68020 A/B pair before deciding retention.

Supplied FS-UAE/68020 HD testing also reports both variants visually and
functionally correct. Overall cadence is indistinguishable: A records 27.07
FPS and B 27.05 FPS, while B's run has materially heavier projectile work.
The scoped collision consumers remain positive: `player` average falls 922 to
875 ticks (-5.1%) and median 756 to 599 (-20.8%); `enemies` average falls 1760
to 1748 (-0.7%). Retain the 3 KiB non-Chip hazard cache as production default
and preserve the former boundary scan behind
`SPARKPAW_COLLISION_HAZARD_SCAN_REFERENCE`. This is a small CPU saving, not a
measurable whole-game FPS improvement; overall 68020 cadence remains rejected.

The next isolated projectile candidate targets only repeated enemy scans.
Player plasma currently advances across every crossed X pixel and calls the
four-slot enemy hit dispatcher at each pixel. Candidate B first finds the
nearest eligible enemy hitbox entry once, then retains the existing per-pixel
geometry sweep up to that point. Geometry still executes before enemy damage
at an equal X, and the existing damage dispatcher remains authoritative. An
exhaustive host comparison proves left/right interval entry against the pixel
reference for every interval in the test domain. Gate walls, platforms,
beetles, Striders, both directions, impact and sounds on FS-UAE/68030 first.

Supplied FS-UAE/68030 HD testing reports both projectile variants visually and
functionally correct. Both sustain exactly 50.00 FPS with only one-field
intervals. Both runs issue 82 player-shot requests. B reduces `projectiles`
average from 20 to 12 CIA ticks (-40%), p95 from 91 to 46 (-49.5%), and maximum
from 174 to 91 (-47.7%). A records 21 hits/12 kills and B 20/11, so the scene
is close but not perfectly identical. The scoped reduction is strong enough
for a matched FS-UAE/68020 A/B gate; do not promote before that result.

The supplied FS-UAE/68020 HD A/B result rejects promotion. Both variants look
and play correctly, but whole-game cadence is not comparable because B carries
more enemy load. In the scoped `projectiles` section, average cost is unchanged
at 362 versus 366 ticks and median regresses from 106 to 234. B does reduce p95
from 1291 to 847 (-34.4%) and maximum from 3529 to 1614 (-54.3%), but it pays
the preselection overhead on ordinary active-shot frames without reducing
average CPU cost. Remove the prototype and retain the simpler pixel-order
dispatcher. Preserve these results as evidence that a future swept design must
also eliminate or coalesce geometry work rather than adding an enemy-only
prepass. Overall 68020 cadence remains rejected.

The long 68020 CHARGING phase is a new separate loading observation. Measure
asset unpacking, sprite/cache conversion, initial ring population and deliberate
title/status holds independently; do not mix loading changes with runtime
candidate promotion.

### CHARGING measurement

The first supplied FS-UAE/68020 loading profile attributes about 40.2 seconds
to measured renderer preparation: broad setup 1,112 PAL frames, Strider cache
716, water 74, beetle cache 52, initial rolling targets 31 and all remaining
named pattern/Copper stages 23. The deliberate 100-frame minimum is already
covered by this longer work and is not the dominant delay.

Both enemy cache builders currently revisit every source pixel, read its mask
and four planar bits, then reconstruct word-aligned destination planes. Since
the authored facings, frames and widths are already word-aligned planar data,
an exact word-level transfer is the first isolated loading candidate. Broad
setup requires later subdivision; it must not be guessed to be hardware-sprite
conversion alone because bitmap clearing, HUD composition, guarded rear copying
and diagnostic allocation share that interval.

Candidate H2 corrects H1's guard-word regression and transfers only authored
planar words. Supplied FS-UAE/68030 and FS-UAE/68020 testing reports clean enemy
presentation. On 68020, beetle conversion falls from 52 to seven PAL frames and
Strider conversion from 716 to 100. Total measured renderer preparation falls
from about 40.2 to 26.9 seconds, saving about 13.3 seconds. Retain H2. Broad
setup remains approximately 22.2 seconds and requires finer attribution.

The follow-up setup breakdown records 1,098 of 1,114 setup frames in
`buildHardwareSprites`, about 22.0 seconds and 98.6 percent of that group.
Target allocation takes eight frames, rear guard four, HUD and diagnostic trace
two each, and Copper allocation less than one. The next isolated loading
candidate is therefore word-level player hardware-sprite conversion; retain
the authored fourth transparent chunk and all attached-sprite contracts.

The first player-sprite word-copy gate on supplied FS-UAE/68030 records the
entire player-sprite conversion in one PAL frame, with 50.00 Hz cadence and zero
rolling-target ownership violations during 598 presented frames. This proves
the diagnostic run is structurally clean, but visual acceptance remains the
user's observation and 68020 loading improvement requires the matched stress
test before promotion.

Supplied FS-UAE/68030 and FS-UAE/68020 testing subsequently reports Sparkpaw's
presentation visually correct. On 68020, `player_sprites` falls from 1,098 to
45 PAL frames: 1,053 frames, or about 21.1 seconds, removed. Total measured
renderer preparation falls from roughly 26.9 to 5.9 seconds. The 68020 run has
zero ownership violations; its 24.84 effective FPS remains below acceptance
and confirms this is a loading optimization rather than a gameplay-cadence
optimization. Retain the word-copy route as the default and keep the old path
behind `SPARKPAW_PLAYER_SPRITE_PIXEL_REFERENCE`.

The next isolated runtime candidate unrolls only the fixed three-copy inner
loop of direct dynamic synchronization. Supplied FS-UAE/68030 A/B testing
reports identical presentation and gameplay through diamonds, animated water,
a water death and level reload. B lowers `ring_dynamic` average from 254 to 187
CIA ticks (-26.4%) and p95 from 499 to 354 (-29.1%); both runs remain near 50
Hz with zero ownership violations. The unmatched Bob-pass totals are not used
as evidence. A matched FS-UAE/68020 A/B measurement is required before retain
or reject.

Supplied FS-UAE/68020 A/B testing also reports identical presentation. The
unrolled route reduces `ring_dynamic` median from 9,726 to 6,001 ticks (-38.3%),
p95 from 18,828 to 12,166 (-35.4%), average from 13,540 to 5,736 (-57.6%), and
maximum from 20,926 to 15,197 (-27.4%). Whole-run cadence is not comparable:
A spans 2,253 calls at 27.66 FPS while B spans 922 at 25.75 FPS and B's retained
window includes much heavier entering-column work. Retain the scoped unrolled
improvement, make it default, and preserve the old loop behind
`SPARKPAW_DYNAMIC_RING_INDEXED_REFERENCE`. Overall 68020 cadence remains
rejected; entering-column roll spikes remain a separate candidate.

The entering-column direct-word candidate passes the supplied FS-UAE/68030
bidirectional-scroll presentation gate. Against CopyMem A, B reduces
`ring_roll` average 313 to 83 ticks (-73.5%), p95 1,646 to 500 (-69.6%), and
maximum 2,060 to 1,589 (-22.9%). Both runs hold 50.00 Hz with zero ownership
violations. A 68020 A/B stress measurement remains required before promotion.

Supplied FS-UAE/68020 A/B testing reports both routes visually correct. B
reduces `ring_roll` average from 5,313 to 3,217 ticks (-39.4%), p95 from 64,486
to 16,218 (-74.9%), and maximum from 64,622 to 16,333 (-74.7%). Bob-pass p95
also falls from 68,300 to 25,908 ticks, consistent with removal of the roll
spike. Initial `ring_targets` loading falls from 32 to 22 PAL frames. Whole-run
cadence is unmatched and moves from 27.43 FPS in A to 26.56 in B, so no average
FPS gain is claimed. Retain direct canonical word copy as default and keep the
old tiny-CopyMem path behind `SPARKPAW_CANONICAL_RING_COPYMEM_REFERENCE`.

The tile-span collision candidate replaces repeated per-pixel tests with one
test per distinct 16px collision tile, while explicitly retaining the y=200
floor boundary. An exhaustive host comparison across more than 60 million
horizontal and vertical cases matches the pixel reference. Supplied
FS-UAE/68030 testing reports identical gameplay. B reduces `player` average
from 92 to 26 ticks (-71.7%), p95 from 242 to 89 (-63.2%), and median from 45
to 33 (-26.7%). Both runs remain near 50 Hz with zero ownership violations.
Require the 68020 A/B gate before promotion.

The supplied FS-UAE/68020 A/B runs are closely matched at 1,019 and 1,035
presented frames and report identical gameplay. Tile-span B raises effective
cadence from 26.11 to 30.09 FPS (+3.98 FPS, +15.2%). `player` average falls
from 4,170 to 888 ticks (-78.7%), p95 from 8,688 to 1,505 (-82.7%), and median
from 2,152 to 756 (-64.9%). `game_update` average falls from 7,440 to 3,725
ticks (-49.9%), while three-plus-field intervals fall from 95 to zero. Retain
tile-span collision as default and preserve the old implementation behind
`SPARKPAW_COLLISION_PIXEL_SPAN_REFERENCE`. Overall 30.09 FPS remains below the
50 Hz target, so performance acceptance is still open.

A follow-up Blitter prototype for the remaining dynamic water/diamond copies
is rejected at the FS-UAE/68030 gate. Presentation stays correct, but cadence
falls from 49.96 to 38.09 FPS. `ring_dynamic` average rises from 164 to 3,796
ticks (over 23x), while Bob-pass average rises from 4,347 to 7,913. Twenty-four
small serialized Blitter jobs and their Chip-bus contention cost far more than
the retained CPU word-copy loop. Do not pursue this route on 68020; the
prototype implementation and build targets were removed.

The next isolated candidate addresses hardware-sprite staging. The reference
copies two 800-byte authored attached-sprite streams from Fast RAM to the next
Chip-RAM stage on every visible update, even when facing and animation frame
are unchanged. Candidate B caches facing/frame independently for both
alternating stages. It still patches sprite position/control words and Copper
pointers every update; it suppresses only an identical 1,600-byte image copy.
Host tests cover both-stage warm-up, frame/facing changes and hidden intervals.
VBCC 68020 assembly inspection confirms the cache comparison is inline and the
existing `CopyMem` call is conditionally bypassed, with no compiler helper call.
This candidate awaits the FS-UAE/68030 presentation and timing gate.

Supplied FS-UAE/68030 testing reports both variants visually correct. A records
49.91 FPS, five two-field intervals and sprite-stage median/p95/average/maximum
of 0/46/8/321 CIA ticks. B records 49.96 FPS, two two-field intervals and
0/45/7/170 ticks. The scoped change is positive but too small on the fast 68030
to justify promotion by itself; broad Bob totals are unmatched. Proceed to a
matched FS-UAE/68020 A/B gate.

Supplied FS-UAE/68020 testing also reports both variants visually and
functionally correct. A records 28.73 effective FPS over 1,658 intervals;
B records 29.92 over 1,307, but the different duration/route prevents assigning
the full +1.19 FPS to this change. The scoped sprite-stage result is clear:
median falls 423 to 74 ticks (-82.5%) and average 407 to 236 (-42.0%). P95
rises 431 to 451 because an actual pose change still performs the copy after
the cache comparison. Retain the cache as production default; preserve the
old route behind `SPARKPAW_SPRITE_STAGE_ALWAYS_COPY_REFERENCE`. Overall
68020 cadence remains rejected.

## Candidate: reclaim the line-0 to line-100 phase

Generated production-route 68020 assembly confirms that the rolling build
already starts Bob composition directly after update; the old line-253 wait is
compiled only into the non-rolling fallback. It also confirms a separate
continuous main-loop wait after every fixed-boundary publication: the next game
update is delayed until PAL raster line 100. This leaves roughly 100/312 of a
PAL field, about 6.4 ms, unused before CPU and inactive-target work begins.

Candidate B removes only this line-100 wait. The supplied FS-UAE/68030 HD gate
reports both A and B visually normal, with zero ownership violations and
exactly 50.00 FPS: A records 1,239/1,239 one-field intervals and B records
1,122/1,122. B begins work around line 0--7 and finishes earlier; its longer
`publish_wait` is the expected transfer of formerly front-loaded idle time to
the fixed safe boundary. Differing scene load prevents comparing Bob-family
averages. Proceed to the decisive matched FS-UAE/68020 A/B gate; no ADF,
Pocket or real-hardware acceptance is inferred.

## Post-alpha.45 residual-cost decision

Alpha.44/45 incorporate the accepted seam, collision, phase-start, rolling,
diamond, sprite-stage, enemy-state, traversal, Bob-register and aligned-column
work documented above. The latest matched FS-UAE/68020 A/B result reaches
45.55 effective FPS with 1,598 one-field and 173 two-field intervals over 1,771
presentations. This leaves about 4.45 FPS to the 50 Hz target for that workload;
it is not ADF, Pocket or real-A1200 acceptance.

The remaining continuous aggregates are approximately:

| Scope | Average | p95 | Current interpretation |
| --- | ---: | ---: | --- |
| `game_update` | 4,056 ticks | 5,215 ticks | CPU/game-state aggregate; leaf work and memory domain need a current split |
| `bob_pass` | 5,840 ticks | 9,142 ticks | Mix of CPU setup, custom writes, Blitter execution, `WaitBlit` and Chip contention |

The p95 sum is 14,357 CIA ticks, above the approximately 14,188-tick PAL-field
budget before publication and miscellaneous overhead. The next diagnostic must
therefore distinguish CPU-active time from `WaitBlit`, Blitter-active time and
Chip-bus pressure instead of treating both aggregates as ordinary C cost.

Before changing code, regenerate actual production and diagnostic VBCC
68020/68030 assembly and symbol sizes for Bob restore/draw, Strider frame
staging, collectible/projectile Bobs, enemy activation/update, ring/Copper
patching and compiler helpers. Verify linker and runtime memory domains:
CPU-only state, lookup tables, caches and profiler buffers should be Fast RAM;
Agnus/Blitter/Paula-visible data must remain Chip RAM. A mirror is justified
only by measured CPU reads and must not introduce recurring Chip-to-Fast copy
work.

Provisional ranking after that measurement split:

1. reduce serialized Bob waits through safe CPU preparation during an active
   Blit, waiting only before registers or data are actually reused;
2. isolate Strider preparation/staging from its Blitter execution cost;
3. prepare or bundle restore/draw jobs without changing draw order, planes,
   pixels, ownership or H7 timing;
4. add Fast-RAM mirrors for proven CPU-read-heavy Chip data;
5. consider unchanged-Bob persistence only with complete overlap invalidation;
6. select smaller gameplay/input/audio candidates only if the new profile
   ranks them higher.

Every candidate remains an isolated A/B: host/assembly verification,
FS-UAE/68030 presentation, then matched FS-UAE/68020 cadence. Research is
targeted at a measured 68020/AGA scheduling or memory question. Mechanical
source splitting remains a separate byte-identical refactor. Fixed 25 Hz game
updates with 50 Hz display service remain a last-resort standalone prototype;
uncontrolled frameskipping is forbidden.

## Rejected post-alpha.45 Bob pointer-precompute prototype

The residual-cost profiler measures roughly fifty `WaitBlit` calls per
FS-UAE/68020 frame. It records 2,900 wait ticks per frame; subtracting the
approximately 19-tick empty-wait measurement floor bounds actual waiting near
1,950 ticks per frame. This is material, but the per-wait instrumentation also
explains why diagnostic cadence is lower than the byte-identical production
build.

An isolated prototype moved the four source/destination plane-address
calculations ahead of each existing wait. It retained every Blit, register,
plane, family order, pixel and ownership contract. Supplied FS-UAE/68030 HD
testing reports A and B visually correct. B appeared scoped-positive there:
Bob-pass average 5,433 to 5,013 ticks and p95 8,399 to 7,508.

Supplied FS-UAE/68020 HD testing also reports both visually correct, but rejects
B's timing. The runs are not perfectly scene-matched, yet B moves effective
cadence 38.56 to 34.29 FPS and Bob-pass average 7,369 to 8,955 ticks. The
precomputed stack arrays add work when the preceding Blit has already completed,
so there is no acceptable 68020 evidence of useful overlap. The implementation,
targets and source test instructions were removed. Preserve alpha.45's current
address schedule and pursue a candidate that eliminates actual jobs/waits or
reduces transferred Chip data instead.

## Rejected post-alpha.45 inline WaitBlit prototype

An isolated prototype replaced the seven static C call sites of
`platformWaitBlit()` with the identical register poll in place. Supplied
FS-UAE/68030 and FS-UAE/68020 HD testing reports both A and B visually correct.
The 68030 run suggested a small Bob-pass reduction, but the 68020 workloads
were not scene-matched and provide no top-line win: effective cadence moves
44.21 to 42.55 FPS while B also contains a heavier projectile/Strider scene.
At roughly fifty runtime calls per frame, removing only seven `jsr`/`rts`
pairs has too little bounded upside to justify more tests. The candidate,
targets and test instructions were removed.

Before selecting another Bob micro-optimization, use the minimal-cadence
diagnostic to bound profiler observer cost. It preserves renderer boundary
sampling and the clean-exit log but compiles out nested CIA performance scopes
and per-family raster timing. This separates the remaining production cadence
gap from instrumentation overhead.

Supplied FS-UAE HD testing reports normal presentation for both minimal builds.
The 68030 run is exactly 50.00 FPS: 1,578/1,578 intervals use one PAL field,
with zero ownership violations. The 68020 run reaches 48.58 FPS: 1,104
one-field and 33 two-field intervals out of 1,137, no three-field interval and
zero ownership violations. Compared with the 44.21-FPS function-call reference
from the immediately preceding fully instrumented test, at least 4.37 FPS of
the apparent gap was diagnostic observer cost under these supplied workloads.
The runs are not frame-for-frame matched, so this is a bounded conclusion, not
an exact production-versus-diagnostic delta.

The retained trace shows the remaining missed-field flag principally under
the heaviest recorded states: the right-end camera around 1403 with two
Striders and five collectibles, and the first frames after level reload with
up to seven collectibles. This makes broad average-cost micro-optimization a
poor next choice. Pursue only a tail-specific reduction that also has useful
bounded upside on real hardware. The normal alpha.45 executable contains none
of the diagnostic scopes or log writer; FS-UAE/68020 is now near the 50-Hz
target. Subsequent supplied tests accept alpha.45 presentation and cadence on
the approximately 34.5 MHz real A1200/68030 from HD and physical ADF and on
the Analogue Pocket 68020 ADF path.

## Post-minimal-cadence whole-codebase recheck

A second production-route review after the 48.58-FPS minimal-cadence result
rechecks the complete main loop, gameplay dispatch, physics/collision, enemies,
projectiles, collectibles, audio, input, HUD, Copper/ring work, sprite staging,
Bob composition, loading calls and explicit memory domains. Current VBCC
`-O2 -cpu=68020` assembly was regenerated for player, enemy, projectile and
collectible code. It uses native 68020 multiply/divide instructions and direct
or bounded indirect calls; no new compiler-runtime arithmetic helper appears.

No forgotten continuous large copy or post-CHARGING asset operation was found.
Runtime `CopyMem` sites are the already accepted specialized entering-column
fallback, pose-cached hardware-sprite staging, changed Strider stage copies and
event-driven HUD work. CPU masters and state are Fast/public memory where
appropriate; Blitter, Copper, sprite and Paula sources correctly remain Chip.
Creating mirrors for those Chip sources would add synchronization without a
proven recurring CPU-read saving.

The remaining ordinary CPU scopes are not new big-gun candidates. Player
physics already uses the accepted tile-span collision route; projectile enemy
preselection was measured and rejected; direct traversal lookup and enemy
copy-on-unload are already defaults; collectible gameplay collision scans 48
small Fast-state records but measured only about 210 diagnostic ticks. Audio,
input, HUD and contact scopes are smaller still. Keyboard acknowledge can busy
wait for two raster-line changes on a key transition, as required by the Amiga
keyboard protocol, but is event-driven and not a joystick-frame cost.

The apparent tail correlation with five to seven visible diamonds does not
justify reopening collectible persistence. H5, H6 and H7 already tested full,
visible-only and column-indexed spatial invalidation against enemy, projectile,
splash, water, hover, collection and origin changes. Their bookkeeping erased
the scoped Blit saving on 68020 and worsened top-line cadence. H4 always-redraw
therefore remains the lower-risk measured winner.

Conclusion: no newly missed source-level optimization has enough bounded
upside to explain another four or five production FPS. The minimal diagnostic
itself still copies renderer trace state and samples raster boundaries, so its
33 two-field intervals are an upper bound on the normal executable's misses.
The subsequently supplied alpha.45 real-A1200/HD, physical-ADF and Pocket runs
accept the checkpoint. No performance prototype remains active. If a future
feature regresses cadence, profile that hardware-facing gap with coarse scopes
rather than adding another FS-UAE-only micro-candidate.
