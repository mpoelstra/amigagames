# Level 1: verified production comparison — 2026-09-05

Current status: [CURRENT_STATUS.md](CURRENT_STATUS.md). Performance research
is explicitly parked by user decision. The Bob/column/history changes are
retained in production and the played e6e20db6... build. 030 passed; the user
reported no noteworthy 020 change and authorized retention. No FPS gain or
explanation of the tiny alpha.68 difference is claimed. No test is pending.

The audit below preserves original comparisons and offline probes. Its
“played baseline” cf861a, Bob-only b0eb6ca and then-offline probes are historical
stages; the current production and drawer SHA is recorded in the status index.
Original source evidence, measurements and rejection boundaries remain valid.

## Baseline provenance

| Image | bytes | SHA256 |
| --- | ---: | --- |
| Original `dist/Sparkpaw-0.6.0-alpha.68/Sparkpaw` | 66,504 | `fde43fce34be0ab97a787690fff8e701e17ef023ed99568f7c1a984d2f534111` |
| Ordinary alpha.68 rebuild | 66,504 | `fde43fce34be0ab97a787690fff8e701e17ef023ed99568f7c1a984d2f534111` |
| Played `build/sparkpaw-campaign-play` and `dist/Campaign-Play-HD/Sparkpaw-Play` | 155,208 | `cf861a89702d5642c4292502c1b40bc834992c54c7d407925f685ff00dee5656` |

All 47 tracked `src`/Makefile files in
`/tmp/sparkpaw-alpha68.DNFCny/sparkpaw` match release commit
`20af96763994b787c7d8affd445e33625ea18252`. `git diff f011b60 20af967 --
sparkpaw/src sparkpaw/Makefile` is empty. `make sparkpaw` in that temporary
snapshot reproduces the ordinary release, with no diagnosis flags. The
original release was never written. Temporary snapshot survival is not assumed
for later sessions: the commits and file hashes are recorded.

`build/layout-audit-alpha68-release-verified` contains independently compiled
objects, assembly, link map and a second byte-identical link. The equivalent
played-image audit is `build/layout-audit-campaign-play-verified`.
Both use vbcc `+aos68k -O2 -cpu=68020 -notmpfile`, the same local toolchain,
normal release renderer flags and ordinary HD assets. The integrated command
adds campaign, Stormrail and split-renderer feature flags, but neither image
has render or transition logging. Full commands are in their manifests.

Supporting artifacts are under `build/level1-production-audit`: source hashes,
asset comparison, Hunk sizes, protected-file inventory, helper diffs, the
isolated correction patch, host harnesses and compiler probes. Assembly
normalization renames local labels and the Level-1 public prefix; it is a
screening tool, **not** proof that differently named callees do identical work.
The Bob-wrapper discovery demonstrates why following those calls matters.

## Executed frame chain

Ordinary Level 1 still executes:

`gameUpdate -> completion check -> renderer update -> Bob composition ->
wait until raster >=300 -> wait until raster <300 -> publish -> next update`.

There is no new line-100 wait, frame throttle, delayed second update, or
per-frame logger in the played image. Main adds an Esc query and one resident
section-selector branch before each direct update/draw/publish call. The
publication branch is retried only if publication fails. The Level-1 update,
Bob-orchestration and publish instruction shapes match alpha.68 after local
label/public-name normalization; their helper calls require separate review.
PAL publication still uses the same inactive-target ownership contract.

`gameUpdate` tests the active section before the unchanged source Level-1 body.
It skips Stormrail simulation. Its native local stack frame is nevertheless
48 bytes instead of 24, with the same 40-byte saved-register set. Level-1
register allocation and some stack accesses differ. This is a confirmed
code-generation difference, not a reason to repeat the rejected gameUpdate
extraction plus completion-cache experiment. Production calls completion once;
the diagnostic double-completion issue is not present here.

Input still enters `playerReadInput` on the same gameplay branches, with the
same joystick/button/physics/animation functions. Keyboard decoding additionally
recognizes Esc; the ordinary no-key-event path does not acknowledge an extra
key. `platformRasterLine`, field counter and Blitter-wait function bodies are
unchanged. Common audio playback/latch functions are unchanged; `audioUpdate`
has the four known extra Harrier cooldown checks. The audio split remains
rejected and is not repeated.

`enemies.o`, `projectiles.o`, `collectibles.o` and `level_data.o` are each
**byte-identical** to the corresponding alpha.68 object. This is stronger than
source similarity: enemy parking, collision callbacks, projectile sweeps,
diamond simulation and authored routes did not acquire hidden Stormrail loops.
The retained collision selector still adds one local test in `collisionSolidAt`
versus alpha.68; it does not call into `game.c` for every query. The remaining
collision functions have matching normalized instruction shapes.

## Renderer helpers: findings and boundaries

### Extra masked-Bob wrapper

Alpha.68's `blitMaskedBobTarget` directly programs the four planes. The current
function calls `blitMaskedBobTargetStride`, supplying `height` again as
`planeRows`. The latter parameter is required by the accepted Stormrail finale
cache, whose physical plane height differs from visible draw height.

In the played Level-1 assembly, local `l1857` repacks nine arguments (36 bytes),
calls `l1856`, adjusts the stack and returns. It is an ordinary call, not a
tail call or an inlined adapter. Four static caller sites enter that adapter:
the generic Bob wrapper, diamond-to-world helper, target-local diamonds and
extra life. Via the generic path this affects enemies, projectiles, splash and
Core as well. The number of executions depends on visible objects and ring
copies. Alpha.68 already had the generic outer Bob wrapper; only the newly
introduced inner adapter is the avoidable difference.

A single offline correction preserves the exact Stormrail function and its
fixed-stride semantics while compiling the original direct body for the
Level-1 translation unit. It adds no allocation, pool, draw, wait or gameplay
change. Source changes exist **only** in
`build/level1-production-audit/bob-direct-source/src/renderer.c`; the reviewable
patch is `build/level1-production-audit/bob-direct.patch`.

Validation:

- Extracted actual alpha.68/current/corrected helpers produce identical
  per-plane register snapshots across 36,864 cases, covering all 1,536 ring X
  positions and varied widths/heights, with four waits per call. This proves
  register programming equivalence, not bus timing or visual acceptance.
- Corrected direct helper matches alpha.68's normalized assembly exactly.
- All 16 other integrated objects, including main/game/audio, the dispatcher
  and the complete Stormrail renderer, are byte-identical to the played build.
- Ordinary compiler link and independent object/map link match exactly:
  155,136 bytes, SHA256
  `b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6`.
- The existing full host suite passes. Existing compiler optimizer-limit
  warnings remain; no compiler success is labelled native acceptance.

This saves one nested adapter per affected draw. The 72-byte file reduction
is not the argument for the correction. Final link addresses still shift,
including Stormrail addresses despite unchanged object bytes; a later user
cadence gate must therefore cover both sections. Nothing has been staged.

### Entering-column loop

`prototypeCopyCanonicalColumn` has a constant-false `flightBlank` expression in
Level 1. VBCC removes the branch but changes the word load from
`move.w (a3),a0` to `moveq #0,d7; move.w (a3),d7; move.l d7,a0`.
This adds two instructions per row, or **1,664 instructions per complete
208-row, four-plane column**, plus one saved register. It does not add extra
Chip reads/writes per row. The helper runs when an alternating target rolls
by exactly +16 or -16 pixels, not on every frame or every pixel scrolled.

A separate compile-only probe restores the original non-Stormrail source body:
its helper becomes alpha.68-identical after label normalization and its
Stormrail object remains byte-identical. No full executable was linked for
this probe, and it is not combined with the Bob correction. This is especially
relevant to a small camera-motion cadence difference and deserves its own
comparison rather than a speculative object reorder.

### Other changes

- `blitRestoreRect` uses the shared unsigned restore-width helper. Its setup
  has small instruction changes, while its four-plane Blitter loop is the
  same shape. Do not replace the accepted Stormrail damage-footprint logic.
- Sprite staging still conditionally copies exactly 1,600 bytes for a changed
  player image; no new unconditional image copy was found. The cached frame
  local and 63-frame table change instruction/register selection.
- Level-1 `PrototypeTarget` remains 426 bytes, two entries/852 bytes. The old
  combined-renderer 752-byte stride is **not** the played Level-1 layout.
- Shared game state grows from 38 to 496 bytes, but the existing Level-1
  fields keep their offsets. No full GameState copy/clear was found in an
  ordinary frame; resets remain lifecycle operations.
- History restore/save, water maintenance, enemy staging, target preparation
  and common ring/Bob helpers were compared beyond the outer draw function.
  Most normalize identically; this does not erase the adapter, column and
  sprite/setup exceptions above.

## Assets and actual workload

Of 37 files in the original release's runtime asset directory, 34 are
byte-identical in the active drawer. The three different files are
`readymenu.spbm`, `sparkpaw-level-complete.spbm` and
`sparkpaw-score-glyphs.spbm`: presentation assets outside ordinary Level-1
composition. Front, rear, collision, player, beetle, Strider, diamond, Core,
extra-life and existing audio samples match.

There is a separate generated-cache exception: shared `plasmaPatternPen`
changes Level-1 hostile impact patterns 7/8/9 to the Harrier variants. An
exhaustive comparison of ten patterns, two facings and every 16x9 pixel finds
88, 64 and 66 changed pen positions respectively; pattern 9 also changes 56
mask positions. Player patterns 0..4 and ordinary hostile flight 5/6 match.
Level-1 impact selection can reach all three changed patterns. This is a
concrete art-isolation issue, but cache dimensions, draw geometry and Blit counts
are unchanged, so it is not an established cadence explanation. It is recorded
separately and is not silently changed in the performance correction.

Enemy count/speed/starting positions are seeded using `DateStamp`; optional
spawns and speeds vary. Identical route tables and identical enemy code do not
make two manual runs identical workloads. Camera/seed/object-state matching
would be needed for a quantitative causal attribution.

## RAM: capacity, placement and traffic are different questions

Hunk-header requests, excluding loader bookkeeping and runtime allocations:

| Component | alpha.68 bytes | played image bytes | delta |
| --- | ---: | ---: | ---: |
| CODE | 62,920 | 144,396 | +81,476 |
| BSS | 17,000 | 22,208 | +5,208 |
| other hunks | 136 | 148 | +12 |
| total | 80,056 | 166,752 | **+86,696** |

All six hunks in both executables request ordinary memory (type bits 00), not
forced Chip or forced Fast. Hunk flags describe requirements; they do not prove
actual runtime placement. The configured local A1200 file declares 2 MB Chip
and 8 MB Fast, but this does not prove the exact boot/free-list state of the
user's played run.

Additional observed Level-1 allocation requests:

- Four unconditionally loaded Harrier samples: **9,812 bytes Chip**, before
  allocator rounding/bookkeeping (2646 + 2426 + 2756 + 1984). They are not
  played in Level 1 and use the existing Paula voices when needed in Stormrail.
- `PLAYER_ANIM_FRAMES` grows 62 -> 63 globally: four extra 800-byte Fast sprite
  streams, **3,200 bytes Fast**, plus pointer-table storage already counted in
  BSS. This is cold conversion of an unused Level-1 slot, not an extra DMA
  channel or per-frame 3,200-byte copy.
- The Level-1 graphics selector exits before loading Stormrail flight rear,
  heart, family and obstacle sources. Its renderer does not allocate their
  caches. Both renderer objects' static storage is resident, but only the
  selected renderer owns live gameplay allocations.
- Displayable bitmaps and DMA caches remain Chip; CPU-only source planes and
  large master caches explicitly request Fast. Existing frame paths do not
  allocate new storage or load files.

These figures are specific deltas, not a complete measured free-memory total.
There are no logger-free runtime `TypeOfMem` results for CODE/BSS/stack and
no matched largest-free-block snapshots. Do not infer Fast exhaustion, Chip
fallback, fragmentation, a leak, or placement parity from executable size.
Idle allocated Chip bytes do not themselves add DMA transfers. Falling back
from Fast to Chip for executed code/stack/CPU data, or adding actual Chip
traffic, would be materially different and is still an open placement question.

### Primary documentation consulted online

The Exec manual explains that ordinary allocations prefer available Fast RAM
and can fall back to Chip; Fast avoids CPU/custom-chip contention. This makes
actual placement and largest usable blocks more informative than total bytes:
[Commodore RKM, Dynamic Memory Allocation](https://amigadev.grimore.org/Libraries_Manual_guide/node0288.html).

The DOS manual documents memory-type bits in Hunk headers (§11.2.1) and
LoadSeg's allocation of executable segments (§11.3):
[Amiga RKM DOS](https://developer.amigaos3.net/sites/default/files/downloads/2024-10/Amiga_ROM_Kernel_Reference_Manual_DOS.pdf).

The processor manual describes the 64-longword direct-mapped instruction
cache (§4.1). Executed instruction addresses matter; unused resident code does
not automatically occupy it. This supports investigating access patterns,
not claiming a cache conflict from a larger file:
[Motorola MC68020/MC68EC020 User's Manual](https://www.nxp.com/docs/en/data-sheet/MC68020UM.pdf).

## Historical investigation order and larger-game lessons (parked)

1. Keep the two concrete helper costs independent. The Bob correction is a
   complete offline candidate; the column change is a compiler probe. Evaluate
   one changed subsystem at a time against the preserved logger-free image,
   with user-visible 68030 health then stock-020 feel and protected Stormrail.
   Do not stage a combined micro-optimization bundle from these findings.
2. If a memory hypothesis needs testing, prefer one bounded **startup-only**
   inventory after preparation and after a campaign loop: actual CODE/BSS/stack
   memory type, major allocation pointers/sizes and Chip/Fast free/largest.
   Avoid per-frame instrumentation and do not rebuild alpha.68 with the already
   failed diagnostic route. This inventory has not been implemented or requested.
3. For later multi-level growth, budget loading peaks separately from steady
   state and measure repeated-loop recovery. Separate file ownership, live
   allocations and executed helper dependencies. A shared helper's new argument
   or a global frame-count constant can leak into an older level even when its
   outer renderer is physically split. Check generated caches as well as files.
4. Track worst-frame work and PAL deadline crossings, not just mean FPS. An
   occasional column admission or object overlap may exceed a frame while most
   frames remain under budget. Reducing cold resident data addresses capacity;
   removing repeated calls/copies addresses frame cost. Neither substitutes for
   the other.
5. Do not introduce overlays, streaming, a new allocator, union refactors or
   code padding solely because the game is bigger. Require measured placement,
   allocation lifetime or execution evidence first. Keep the accepted resident
   replay/transition boundaries and Stormrail contracts intact.

Audio split, gameUpdate extraction plus completion caching, object-order
changes, and target-history splitting remain rejected experiments. The optional
direct-diagnostic-call experiment remains offline and lower priority. No
FS-UAE session, release, SemVer change, ADF, multidisk package or extra active
user drawer was created for this audit.

## Broader optimization audit — 2026-09-05 (probe-stage evidence)

The latest comparison narrows the reported regression to near parity. Workload
spikes exist in both versions; the useful objective is more margin before a
PAL publication deadline, not extrapolating static instruction counts to FPS.
The user explicitly permits accepting the small difference while researching
further. No new staged candidate is justified solely by this observation.

### Newly identified pre-publication history work

Both verified alpha.68 and the integrated renderer expose the two-target actor
history union after saving the prepared target, then expose it again after
publication. The two calls are NOT value-identical: `prototypeActiveCopper`
changes which target supplies coordinates. The relevant evidence is that the
first result has no consumer in the normal logger-free publication interval:

- Main proceeds from the Bob pass to raster polling and publication retries.
  Disabled profiler macros introduce no actor reads; diagnostics are absent.
- A failed publication returns before reading/writing actor histories. A
  successful publication sets the active target then overwrites the union.
- No game update, input/audio update, reset or transition runs between those
  points. Platform takeover disables interrupts; hardware DMA does not read
  the C enemy/projectile history fields.
- The helper only changes exposed actor drawn flags/coordinates/type, not the
  two stored target histories or simulation fields. When neither target draws
  an actor, neither invocation changes its stale coordinate fields.

A compile-only probe based on the currently staged Bob source retains the first
call for Stormrail and render diagnostics, and omits it only in normal Level 1.
The actual generated `rendererLevel1DrawGameplayBobs` differs by one removed
`jsr l1282`; all other instructions in that function match. The publication
function is raw-assembly identical. The separate Stormrail object is byte
identical to the staged candidate's object. This removes one four-enemy plus
eight-projectile scan: twelve accessor calls plus union/coordinate work per
composed frame. This overhead already exists in alpha.68: it is an opportunity
for general improvement, not evidence of the integration regression's cause.

`build/level1-production-audit/check_history_publication.py` extracts the actual
Level-1 helper into a host harness. 10,000 two-target state combinations test
both active targets, differing positions/types, drawn flags and stale fields;
the complete exposed actor state after publication is identical with/without
the first call, and stored target histories remain unchanged. Accessors count
24 versus 12 calls. The harness does not emulate DMA, raster timing or native
transitions; the no-consumer argument above is a separate source-path audit.
Artifacts: `history-probe.json`, `history-probe.s`, `test_history_publication.c`
and the two `*.history.diff` files. Native compiler checks passed. No full
binary was linked or staged and no production code changed for this probe.

### Ranking and limits

1. **Restore the specialized column copy.** Strong verified regression evidence:
   the offline Level-1 helper matches alpha.68 after local-label normalization,
   removing two instructions per row/plane, 1,664 per full entering column.
   This is periodic scrolling work, not a cost every frame. The independent
   column probe does not yet include the currently staged Bob change. A future
   single-change test must therefore use the actual staged baseline as parent.
2. **Omit the unconsumed first history exposure.** Newly verified constant frame
   work above; promising as a separate experiment after an integrated build and
   the normal functional checks. Preserve the final exposure and all stored
   target history; do not delete union semantics generally.
3. **Reduce CPU/Blitter work under load.** More actors increase restore/draw and
   setup traffic in both releases. Existing ordering, overlap merging and caches
   already address this. Collectible restore/draw also scan 48 entries apiece,
   but erasing old-target history makes naive visible-only filtering unsafe.
   No new proven, safe reduction has been demonstrated for these paths yet.
4. **Memory placement, not executable size alone.** The earlier audit quantifies
   roughly 86.7 KB additional requested Hunk storage and 9,812 bytes of extra
   loaded Chip samples, but actual loaded CODE/BSS/stack addresses remain
   unmeasured. Cold residency is not repeated DMA. No evidence yet shows new
   per-frame allocations, larger Level-1 display DMA, or an allocation fallback
   into Chip RAM. Moving/culling cold assets might recover capacity without
   changing FPS; do not promise speed from that alone.

The Commodore [Hardware Reference Manual, Blitter Operations and System DMA](https://www.amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node012B.html)
explains shared Chip-memory cycles and priority, including that Fast-memory
accesses typically do not use those cycles. Applied here, the useful question
is which active CPU/Blitter accesses collide, not how many level assets exist
on disk. Display/audio DMA demand and actual address placement matter. The
[MC68020 User Manual](https://www.nxp.com/docs/en/data-sheet/MC68020UM.pdf),
sections 4.1 and 8, also makes instruction-cache and timing context relevant;
static instruction savings are not a measured cycle or FPS result.

Do not revive rejected pointer precomputation before WaitBlit, inline WaitBlit,
diamond persistence, column-Blitter copying, audio/gameUpdate extraction or
link-object reordering without substantially new evidence. Their extra CPU
bookkeeping and observed 020 regressions constrain the next step. No speculative
allocation framework, general architectural split or frame-pacing redesign is
warranted by the now tiny subjective gap. Keep candidate effects separable.
