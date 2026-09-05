# Sparkpaw C architecture and performance plan

> Work order update: user chose the 0.7.0-alpha.2 all-format checkpoint / hardware tests
> first, then media/loading work. Standalone Gate-2 refactoring is superseded;
> use it only as needed at the media boundary. See PHASE7_CAMPAIGN_HARDWARE_PLAN.md.

Current status: [CURRENT_STATUS.md](CURRENT_STATUS.md). Performance research
is explicitly parked by user decision. The Bob/column/history changes are
retained in production and the played e6e20db6... build. 030 passed; the user
reported no noteworthy 020 change and authorized retention. No FPS gain or
explanation of the tiny alpha.68 difference is claimed. No test is pending.

Next proposal is only the bounded cold asset-selector part of Gate 2.
CampaignSection and the decision/snapshot contract already exist. Gates 3–5
are deferred, not bundled work. Historical experiments below are not active
instructions. No new implementation is authorized by this documentation pass.

## Why architecture and performance are one plan

The current combined program is functionally viable, but the stock-68020 A/B
tests prove that compile/runtime integration can damage an otherwise isolated
level even when its assets and state remain logically separated. File length
alone is not a defect; hotpath shape, branch placement, instruction locality,
Chip-bus work and ownership boundaries are the relevant constraints.

Measured 2026-09-04 baselines:

| Route/build | intervals | one field | two fields | three fields | FPS | ownership |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| historical alpha.45 Level 1 minimal cadence | 1,137 | 1,104 | 33 | 0 | 48.58 | 0 |
| integrated campaign Level 1, rejected | 1,925 | 1,734 | 188 | 3 | 45.42 | 0 |
| current Level 1 with campaign/Stormrail compiled out | 1,545 | 1,476 | 69 | 0 | 47.86 | 0 |
| protected complete Stormrail interlude | 5,177 | 5,175 | 1 | 1 | 49.97 | 0 |
| integrated campaign Stormrail | 5,321 | 5,310 | 10 | 1 | 49.88 | 0 |

The isolated Level-1 build recovers 2.44 FPS and removes all three-field
intervals. Compiled campaign/Stormrail integration is therefore the dominant
Level-1 regression. The remaining 0.72-FPS difference is not yet assigned
because the historical and current routes are not cycle-identical. Stormrail
shows no meaningful reciprocal regression.

## Non-negotiable contracts

- Preserve alpha.68 Level-1 art, collision, movement, encounters, replay,
  Copper waits, line-252 HUD and renderer ownership.
- Preserve Stormrail distance 15500, local finale timers, 120/60-HP Harrier
  phases, award 60/320 points, persistent pickups, damage, wall/dust and one
  resident load.
- Preserve bounded static pools and all Chip/Fast allocation lifetimes. Add no
  per-frame allocation, scaling, rotation or per-pixel effect.
- Keep `CampaignState`, section-local state and renderer/runtime ownership as
  distinct lifetimes. Only explicit carried vitals cross section boundaries.
- Keep Level-1 and Stormrail assets in their declared ownership groups. A
  future volume loader consumes section manifests; gameplay never selects a
  disk number.
- Do not change SemVer, alpha.68 artifacts, ADF/WHDLoad or release state while
  executing this plan.

## Target architecture

`CampaignState` contains only cross-section progress. A tagged `SectionState`
owns one level's score, time, enemies, pickups, awards and replay reset. A
`RuntimeState` owns renderer, audio, input and loaded assets for exactly one
active section.

A future read-only `SectionDescriptor` provides:

- section id and asset-manifest id;
- init, update and results-extraction adapters;
- renderer prepare/composition/release adapters;
- replay and campaign-transition policy;
- later, a media-volume id consumed only by the loader.

Application flow consumes descriptors. Level code does not know disk numbers
or other levels' asset filenames.

## Measured implementation sequence

### Gate 0 — preserve evidence and inspect generated code — complete

Preserve both 2026-09-04 logs. Compare 68020 generated code and call/layout
for integrated versus isolated `gameUpdate`, `rendererUpdateGameplay` and
`rendererDrawGameplayBobs`. Identify checks or layout changes reached by Level
1; do not infer cost from source size alone.

Generated-code result (VBCC `-O2 -cpu=68020`, identical minimal-cadence
flags):

- complete renderer assembly grows from 13,842 / 191,322 bytes of text in the
  isolated build to 27,981 / 353,928 in the integrated build;
- `rendererUpdateGameplay` grows from 36 to 56 assembly lines;
- `rendererDrawGameplayBobs` grows from 97 to 122 assembly lines;
- `PrototypeTarget` grows from 426 to 752 bytes because it also owns bounded
  Stormrail history. Generated Level-1 code does not copy that history while
  Stormrail is inactive, so this is not currently identified as per-frame bulk
  copy cost;
- the integrated Level-1 hotpath does execute extra section/mode branches
  around scroll selection and Bob-family sequencing. This matches the measured
  isolation recovery and is the bounded Gate-1 hypothesis.

Exit met: generated code selects outer section dispatch and compact Level-1
update/composition as the first A/B candidate. Raw source or binary size alone
is not treated as proof of cost.

### Gate 1 — section hotpath split

Move the section choice to one outer dispatch for update and composition.
Create compact Level-1 and Stormrail entrypoints. Level-1 inner update, scroll,
history and Bob sequencing must not repeatedly inspect Stormrail mode/state.
Stormrail approach/flight/finale may dispatch internally because those phases
belong to that section. Keep shared measured Blitter/Copper primitives in place.

Exit: host contracts and native compile pass; integrated Level 1 returns toward
48.58 FPS with zero three-field and ownership violations; integrated Stormrail
remains near 49.9 FPS with zero ownership violations. Reject or revise the
split if either level regresses.

Candidate A (`SPARKPAW_SECTION_HOTPATH_SPLIT`) is rejected and removed. It
recorded 44.20 FPS (1,449 one-field, 210 two-field and three three-plus
intervals out of 1,662; maximum four fields; zero ownership violations) and the
user perceived no improvement. Duplicating a direct Level-1 sequence enlarged
the combined hot function from 122 to 142 assembly lines and worsened locality.
An architectural boundary is not automatically a performance win when emitted
inside the same large translation unit.

The broad 2026-09-04 Level-1 profiler is deliberately observer-contaminated:
its 33.09 FPS is not an acceptance result. It does exclude several intuitive
causes: gameplay update (median 3904 ticks), scroll patch (128), audio (35) and
the final Blitter wait (57) are not anomalous. Relative to the established
optimized Level-1 profile, the clearest tail growth is in ring roll (p95 4740),
compact-target preparation (p95 5495) and their containing Bob pass (p95
12362). This selects a data-layout experiment before another control-flow
rewrite.

Candidate B (`SPARKPAW_SECTION_HISTORY_SPLIT`) moves the two bounded Stormrail
Bob-history records beside, rather than inside, the two shared
`PrototypeTarget` records. It adds no storage class, pool, allocation or
per-frame work; Stormrail remains indexed by the same target. The candidate
restores Level 1's compact target layout/stride and creates a real
section-ownership boundary. Accept only through the ordinary minimal 68020
cadence gate, then recheck complete Stormrail cadence before retaining it.

Candidate B is rejected and removed. The minimal-cadence user run recorded
43.89 FPS: 2,189 one-field, 346 two-field and three three-plus intervals out of
2,538, maximum four fields and zero ownership violations. The run was longer
and busier, but its missed-frame share also worsened from the rejected combined
baseline's 9.9% to 13.8%. Separating the history improved ownership semantics
but did not recover Level-1 cadence, so target-structure size is not retained
as the causal hypothesis.

### Gate 2 — typed section lifecycle

Replace Boolean world/asset selection with `enum CampaignSection`. Introduce
explicit, idempotent prepare/reset/release boundaries and pointer/size ownership
tests. Do not change filenames, load order or visible loading transitions.

Exit: replay, continue, back-to-title and Escape host contracts pass; both
accepted cadence gates remain within their established distributions.

### Gate 3 — section-local state and adapters

Separate Level-1 and Stormrail reset/update/result state behind adapters while
keeping carried player vitals explicit. Remove cross-module queries where a
typed section parameter or immutable descriptor suffices.

Exit: no section-local value can survive replay or cross-section load; host
tests cover failure and partial initialization; both 68020 gates remain green.

### Gate 4 — cold renderer ownership split

Split allocation/conversion, prepare/reset/release and per-section composition
ownership. Do not move proven inner Blitter routines merely to shorten a file.
Use before/after disassembly and cadence for every moved hot function.

Exit: idempotent cleanup and failure injection pass; asset groups are physically
loadable independently; Level-1 and Stormrail visual/cadence baselines hold.

### Gate 5 — future media-volume readiness

Only when multidisk work is authorized, add a media-volume field to descriptors
and place disk validation/UI above asset loading. Recalculate packed capacity
and create separate ADF contracts. No disk concern enters game or renderer code.

## Mandatory performance review loop

At every architecture gate and every later meaningful gameplay/renderer change:

1. State the affected hot/cold path, expected cost and rollback boundary.
2. Run host contracts and native 68020 compilechecks first.
3. Inspect generated code when a hot function or translation-unit boundary
   changes; source cleanliness alone is not evidence.
4. Use the lightest adequate native test: minimal cadence for acceptance,
   targeted profiling only after a measured regression, broad profiling only
   for discovery.
5. Compare interval distribution, maximum fields and ownership—not FPS alone.
6. Test both sections whenever shared or dispatch code changes. A win in one
   level may not be paid for by the other.
7. Record the user-visible report separately from log-derived facts.
8. Update this table, diagnosis and next gate when evidence changes. Never keep
   executing an architectural sequence whose measured assumptions were falsified.

Only one full user-test drawer remains active. FS-UAE is user-run unless one
predeclared native-only diagnostic meets the exceptional self-test contract.

## Historical next action (superseded by the current audit)

Gate 1 diagnosis remains active. Both same-translation-unit control-flow
duplication and target-history layout splitting failed and were removed.
Generated assembly shows only a handful of extra inactive Stormrail branches
inside the public Level-1 renderer entrypoints, while minimal trace averages do
not reproduce the broad profiler's apparent Bob regression. The active A/B is
therefore causal attribution: A is the complete integrated renderer; B keeps
the integrated campaign/game/main but asks vbcc to compile `renderer.c` as the
Level-1-only translation unit. B cannot enter Stormrail and is not a production
candidate. A material B recovery selects physical renderer separation; no
recovery excludes renderer translation-unit reachability and moves diagnosis
to game/main.

The supplied stock-68020 A/B decisively selects physical renderer separation.
A (complete integrated renderer) recorded 44.56 FPS over 1,845 intervals:
1,625 one-field, 217 two-field, three three-plus, maximum four and zero
ownership violations. B (integrated campaign/game/main with Level-1-only
`renderer.c`) recorded 48.23 FPS over the longer 2,755-interval run: 2,654
one-field, 101 two-field, zero three-plus, maximum two and zero ownership
violations. Missed-frame share fell from 11.9% to 3.7%, close to alpha.68's
48.58 FPS. Retained-trace averages also fell in the renderer-dominated region:
Bob pass 156.13 to 123.42 raster lines and publish wait 118.00 to 102.68.

The diagnostic B stubs are not a shippable implementation and cannot enter
Stormrail. Gate 1 now requires a real Level-1/Stormrail renderer boundary:
Level 1 must compile to the isolated hot code while Stormrail retains its
accepted compositor, with shared Copper/Blitter primitives and allocation
ownership explicit. First re-establish Level-1 minimal cadence, then run the
complete Stormrail cadence gate and the campaign transition/replay contracts.

### External technical check

Primary documentation supports the mechanism, with an important boundary.
Motorola documents the MC68020 instruction cache as only 256 bytes and direct
mapped, so hot-code placement/locality can matter even when inactive code is
never semantically entered. Linker documentation likewise makes object/input
section placement explicit; fine-grained unused-code removal requires the
compiler to emit independently collectable sections. Sparkpaw's vbcc/vlink
Amiga Hunk build does not automatically gain the ELF/GCC
`-ffunction-sections`/`--gc-sections` model.

Therefore separate section renderer objects plus one cold dispatcher are a
sound architecture direction and are independently supported by Sparkpaw's
measured A/B. The initial migration seam compiles the legacy renderer body for
both owners to avoid changing proven primitives while establishing the API
boundary. This is deliberately transitional: it increases the diagnostic
executable from 135 KiB to 171 KiB. Before release or multidisk work, extract
the genuinely shared Copper/Blitter core once and leave only section-specific
code/state in each renderer object. Never present textual inclusion/duplication
itself as the final architecture or as proof of performance.

Supplied FS-UAE/68030 testing accepts the first complete split-renderer
campaign build as working correctly. This establishes the functional gate only;
it does not yet accept stock-68020 cadence or the transitional code duplication.
The next required gate is minimal-cadence Level 1 with the physical dispatcher,
followed—only after a Level-1 win—by the complete Stormrail cadence route.

The first physical-split 68020 pass improves the integrated renderer from
44.56 to 46.85 FPS and removes all three-plus intervals, but its 125 two-field
intervals out of 1,862 (6.7%) remain visibly below alpha.68 and the 48.23-FPS
isolation proof. Retained trace averages are Bob 132.23 and publish 110.77
raster lines. The wrapper dispatcher still made each hot phase perform a
section branch and second call. The next candidate binds update, Bob compose
and publish function pointers once during renderer load; lifecycle dispatch
remains explicit and cold. Supplied 68030 smoke testing accepts Level 1 and the
Continue transition into Stormrail with these bound operations. The exact code
then records 47.25 FPS over 1,879 intervals: 1,770 one-field, 109 two-field
(5.8%), zero three-plus, maximum two and zero ownership violations. MrDig sees
at most a small improvement and does not consider it alpha.68 quality. This is
therefore a partial result, not acceptance.

The next bounded candidate removes the remaining indirect calls. Split-build
clients branch on the already resident section selector and call the selected
Level-1 or Stormrail operation directly. Renderer lifecycle dispatch remains
cold and explicit. Supplied FS-UAE/68030 testing accepts Level 1, its results
and Continue into visibly running Stormrail with this calling convention. The
minimal stock-68020 result is 48.42 FPS over 1,657 intervals: 1,603 one-field,
54 two-field (3.26%), zero three-plus, maximum two and zero ownership
violations. This is a material recovery from 47.25 FPS / 5.8% and is close to
alpha.68's 48.58 FPS / 2.9%, but MrDig still perceives it as only slightly
better rather than clearly alpha.68 quality. The retained final 1,023 trace
intervals contain only three misses; most misses happened before that bounded
window. Treat direct calls as the accepted structural direction, but retain
stock-68020 feel as pending rather than letting the aggregate overrule supplied
play evidence.

Offline comparison against the committed alpha.68 gameplay loop identifies a
second ownership leak: every campaign frame still compiled and executed the
focused Stormrail whole-display capture selector before `gameUpdate()`. Level 1
therefore traversed finale-active, finale-phase and several timer/capture
branches even though campaign builds never request proof frames. The next
candidate excludes this block only when `SPARKPAW_CAMPAIGN` is defined; focused
Stormrail proof targets retain it. A host isolation assertion protects this
boundary. Supplied FS-UAE/68030 testing accepts Level 1 and Continue into
visibly running Stormrail after the exclusion. The sole active gate is now the
matching minimal stock-68020 Level-1 cadence run.

That run records 48.32 FPS over 1,732 intervals: 1,672 one-field, 60 two-field
(3.46%), zero three-plus, maximum two and zero ownership violations. Against
48.42 FPS / 3.26% for the preceding direct-call run, this is neutral variation,
not a performance win. Keep proof capture out of campaign builds for correct
ownership, but do not credit it toward cadence recovery. Offline hot-path
review next targets `collisionSolidAt()`: every Level-1 collision query gained
a cross-module `gameStormrailActive()` call solely to select Stormrail's short
departure cliff. Any correction must preserve Stormrail collision while making
Level 1 use locally owned mode state or its original query path. The bounded
candidate now caches the section selector once in `collisionLoad()`, when
gameplay is already unloaded and its owner is stable. `collisionSolidAt()`
retains one local byte branch but no longer calls across modules on every tile
or pixel probe. Host contracts require both the cached Stormrail cliff selector
and absence of `gameStormrailActive()` from the query body. Gate Level-1 and
Stormrail departure collision on 68030 before measuring 68020 cadence. Supplied
FS-UAE/68030 testing accepts Level-1 floors/platforms and the Stormrail
departure/boarding route with this cache; the matching minimal stock-68020
Level-1 cadence run records 49.30 FPS over 1,772 intervals: 1,747 one-field,
25 two-field (1.41%), zero three-plus, maximum two and zero ownership
violations. This clearly beats both the preceding direct-call best (48.42 FPS,
3.26%) and protected alpha.68 cadence (48.58 FPS, 2.9%). Retain the local
load-time collision selector as a proven architecture/performance correction.
Explicit supplied confirmation of visible Level-1 feel remains required before
closing the stock-68020 gate.

MrDig confirms that the collision-cache run also feels better, while asking
whether more cross-level hot-path leakage remains. A complete shared-module
scan finds one next bounded candidate: `audioUpdate()` added four Harrier-only
cooldown tests to every Level-1 frame. Split this into the original common/
Level-1 update and `audioUpdateStormrail()`, which calls the common update and
then advances exactly those four boss cooldowns. This preserves all timing and
Paula ownership while removing four always-false Level-1 branches. Other
remaining section checks are either necessary once-per-frame dispatch or cold
lifecycle code and are not yet candidates. Gate audio correctness on 68030
before deciding whether a further 68020 measurement is worthwhile. Supplied
FS-UAE/68030 testing accepts Level-1 shot/hit/death/jump/collect audio,
Stormrail shooting and Harrier charge/fire cues. Proceed with one minimal 68020
Level-1 cadence run against the strong 49.30-FPS collision-cache baseline.

The audio-split run records 48.02 FPS over 2,023 intervals: 1,940 one-field,
83 two-field (4.10%), zero three-plus, maximum two and zero ownership
violations. MrDig sees no improvement and identifies the early encounter with
two Striders plus other enemies as the persistent concern. Reject and fully
revert the audio split; fewer source branches are not a win when final code
placement and scene cadence regress. The trace ring begins at frame 1,022, so
this complete-level sample cannot diagnose the early cluster. Return to the
49.30-FPS collision-cache code and make the next evidence capture end directly
after that early encounter, preserving its per-frame update/Bob/object data.

The targeted early-scene capture records 47.43 FPS over 758 intervals: 717
one-field, 41 two-field, zero three-plus and zero ownership violations. The
retained boundary rows expose 23 individual misses, with sixteen concentrated
at camera X 128-226. That cluster combines roughly 2-3 beetles, one Strider,
1-3 projectiles and 3-4 collectibles; the Bob phase is normally the largest
measured renderer phase there. The later camera-X ~733 two-Strider appearance
contains only one exposed miss, so object labels alone are not a sufficient
explanation: overlap, position and the combined update/patch/Bob budget matter.

Do not rewrite the accepted Level-1 Bob paths from this single trace. The next
causal gate is a short, route-matched stock-68020 A/B: A is the exact alpha.68
source at `f011b60`; B is the current collision-cache executable. Both are
compiled with the same vbcc flags and `SPARKPAW_MINIMAL_CADENCE_DIAGNOSTIC`,
and both must be stopped immediately after the same early encounter. If A is
materially better in this matched window, compare generated code/layout for
the Level-1 restore/draw loops before changing C. If it is not, treat the
reported difference as route/scene variance and preserve the proven paths.
The first 1,024-frame alpha diagnostic failed at the charging-to-renderer
transition even though every referenced asset matched the alpha.68 release.
That transition allocates the old trace ring in Fast RAM and exits immediately
when allocation fails. The controlled A/B therefore uses the same 256-frame
power-of-two ring in both executables and stops during/at the end of the target
encounter. This is a diagnostic-memory bound, not a production-code change.
The reduced 256-frame alpha build also hangs at `CHARGING` after a brief visual
glitch. Therefore the allocation hypothesis is not sufficient and the matched
instrumented-alpha A/B is withdrawn: neither alpha diagnostic is valid cadence
evidence and no further user run should be requested from this route. Offline
68020 assembly comparison shows the top-level Level-1 Bob orchestration retains
the same operation order and call shape after translation-unit isolation. The
next investigation compares the called restore/draw routines and their emitted
layout without modifying or executing alpha.68.

The broader frame-chain audit identifies a more important boundary than the
early encounter itself. Integrated `gameUpdate()` placed the complete
Stormrail departure/flight/finale simulation before the Level-1 body in one
very large C function. Extracting that unchanged body to cold internal
`updateStormrail()` leaves one section check and call at the Level-1 entry.
Generated 68020 assembly for the resulting Level-1 body is otherwise identical
to alpha.68's instruction sequence; this restores independent vbcc register
allocation without duplicating GameState or changing section timing.

The same audit finds a measurement-only tax: the Level-1 cadence branch called
`gameLevelComplete()` once to retain a completed frame and then again for the
normal transition. Cache the immutable result once per frame. This removes an
unfair integrated-diagnostic difference; production semantics remain the same.
Protect both boundaries in `test_campaign_level1_isolation.py`, run a short
68030 Level-1/Stormrail functional gate, and only then measure stock 68020.

The supplied 68030 gate accepts Level 1, Stormrail and both Esc-to-ready paths,
but the full stock-68020 run rejects the candidate: 46.66 FPS over 1,805
intervals, with 1,676 one-field and 129 two-field intervals, zero three-plus
and zero ownership violations. MrDig likewise reports no large improvement.
This is materially worse than the collision-cache build's 49.30 FPS / 1.41%
two-field result. Revert the game-function extraction and single-completion
experiment together before selecting the next candidate; do not retain a
cleaner C boundary whose final linked image regresses.

The result adds a mandatory whole-program layer to this plan. Matching a hot
function's local assembly is insufficient on the direct-mapped 256-byte 68020
instruction cache: extraction can change total object size, neighbouring
functions, link addresses and cache-set conflicts. In the transitional split
build, `renderer_level1_unit.c` is appended after every former source object,
whereas alpha.68 linked `renderer.c` immediately after `main.c`. The next
bounded offline investigation must compare/reorder object placement so the
Level-1 renderer and its caller-facing hot code occupy a stable early region,
while keeping Stormrail renderer code cold and last. Measure this independently
from semantic refactors.

The supplied 68030 gate accepts that link-order-only build in both sections.
The stock-68020 run nevertheless rejects it: 47.71 FPS over 1,629 intervals,
with 1,551 one-field and 78 two-field intervals (4.79%), zero three-plus and
zero ownership violations. MrDig perceives at most a slight improvement and
still not alpha.68 feel. Restore the appended split-renderer order, whose
collision-cache executable measured 49.30 FPS / 1.41% two-field. A superficially
alpha-like object neighbourhood is not sufficient after `main` and the full
program image have changed; future placement work needs actual symbol/address
or controlled padding evidence, not source-list resemblance.
