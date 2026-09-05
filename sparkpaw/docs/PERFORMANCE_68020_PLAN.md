# Sparkpaw stock-68020 performance plan

> Current status (2026-09-05): Performance investigation is parked by explicit user decision. This plan preserves historical goals/evidence; do not resume experiments from its next-step instructions. See [status index](CURRENT_STATUS.md).

Status: completed performance checkpoint on the alpha.45 Stage 5L/H7 baseline.
The whole-codebase Stage 2 audit, isolated prototypes and post-checkpoint review
are complete. A low-overhead FS-UAE/68020 diagnostic reaches 48.58 effective
FPS with no three-field misses or ownership violations. Supplied final testing
accepts alpha.45 on the approximately 34.5 MHz real A1200/68030 from HD and
physical ADF and on Analogue Pocket 68020 ADF. Preserve this near-50-Hz stress
result and the Stage 5L renderer as explicit regression baselines.

Historical residual split: `game_update` and `bob_pass` were separated into
CPU work, custom-register setup, `WaitBlit`, Blitter execution and Chip-bus
contention. Their latest averages are approximately 4,056 and 5,840 CIA ticks;
their p95 values are 5,215 and 9,142, whose 14,357-tick sum already exceeds the
roughly 14,188-tick PAL-field budget before publication overhead. Verify actual
Fast/Chip placement from linker/allocation evidence and inspect current VBCC
68020/68030 assembly before selecting the next candidate. The leading route is
safe CPU/Blitter overlap in the serialized Bob chain, followed by Strider/Bob
preparation and proven CPU-read-heavy Fast-RAM mirrors. Re-rank if new evidence
points elsewhere. The subsequent minimal-cadence run showed that most of the
apparent remaining 4--5 FPS deficit was profiler observer cost, not production
work. No further prototype is active.

Hardware policy: the user has authorized the supplied real A1200/68030 at
about 34.5 MHz as the minimum release CPU if necessary. Continue using
FS-UAE/68020 as a stress target and seek measured gains there, but do not trade
away parallax, assets, colours, sprites, animation, HUD or gameplay contracts
solely to claim stock-68020 acceptance.

Future performance work is optional and evidence-triggered. Candidate ideas
that remain technically plausible are coarse real-hardware scopes, eliminating
an actual Bob job or wait rather than rearranging pointer arithmetic, combining
geometry and enemy work in a genuinely coalesced projectile sweep, and a
Fast-RAM mirror only when repeated CPU reads from Chip are measured. Preserve
the rejected-results ledger: H5--H7 diamond persistence, Bob pointer
precomputation, inline `WaitBlit`, fetch-union pruning and small Blitter column
copies did not improve 68020 cadence enough to retain.

## Known evidence

- Alpha.39 restores acceptable presentation and cadence in supplied FS-UAE at
  68030.
- Alpha.42's no-copy rolling renderer removes the observed enemy/projectile
  trails and HUD-boundary flicker in supplied FS-UAE testing, but its cadence
  is rejected in the supplied FS-UAE/68020 and real-A1200/68030 recordings.
- FS-UAE at 68020 remains seriously slow. Sprinting/jumping drops more frames
  than ordinary grounded movement; Strider glitches increase under that load.
- Alpha.39 ADF on the Analogue Pocket's Analogue-Amiga FPGA core at 68020 with
  caches disabled shows transient gameplay-field, Bob and separate-HUD
  corruption. This is timing-sensitivity evidence, not cycle-equivalent stock
  A1200 proof.
- Stage 5L on FS-UAE/68030 holds 50.00 FPS for 2,163 intervals with no visual
  corruption. On FS-UAE/68020 it measures 26.38 FPS: 136 one-field, 337
  two-field and 78 three-field intervals, with 270 wraps in 552 frames.
- Relative to Stage 5A on the same reported FS-UAE/68020 configuration, average
  Bob-pass time falls from 34,138 to 24,357 CIA ticks (about 29%), but complete
  cadence remains effectively unchanged at 26.30 versus 26.38 FPS.
- Alpha.43 on the real 34.5 MHz A1200/68030 and Analogue Pocket retains broad
  graphical stability but has similarly unacceptable cadence. Real HD also
  exhibits repeated or missed sound events under load.
- Alpha.37 tight Bob bounds damaged sprites and did not improve cadence.
- Alpha.38 hidden viewport composition added a mandatory 512x208x4 copy and
  made both 68020 and 68030 worse. Neither technique may return unchanged.
- The rear and foreground pictures are not redrawn by the CPU each frame.
  Agnus fetches seven resident bitplanes through dual-playfield DMA; frame code
  principally changes Copper pointers and fine scroll. Parallax CPU rendering
  is therefore not the assumed cause, although its three continuous bitplanes
  consume fixed Chip-bus DMA and must be included in contention measurements.

## Non-negotiable visual and gameplay contracts

- Preserve PAL 320x256, FRONT16 plus REAR8 dual playfield, authored palettes,
  title/foreground/rear art, player hardware sprites and all enemy pixels.
- Preserve line-252 separate HUD, camera behavior, collision geometry, draw
  order and accepted alpha.33 Strider overlap presentation.
- Never optimize by clipping sprite art, reducing colours/planes, deleting
  parallax or writing displayed Chip RAM from CPU code.
- Every candidate is an isolated benchmark first. A production release follows
  only after 68030 presentation regression and 68020 timing checks.

## Stage 1: trustworthy measurement

Build two executables from the same source:

1. production, with no profiler waits or per-family synchronization;
2. profiler, using a saved/restored CIA hardware timer so durations can exceed
   one PAL frame without aliasing at raster wrap.

Measure independently:

- input plus complete `gameUpdate()`;
- player physics while idle, running, sprinting, rising and falling;
- enemy simulation and projectile collision;
- clean-world diamond and water maintenance;

Also record the raster line at entry and exit of `gameUpdate()`, Copper-list
publication and the Bob pass. A raster wait is not a deadline guard: explicitly
detect an overrun past the safe publication/line-253 phase and retain the prior
complete frame instead of beginning work in an unsafe display phase. Test this
before interpreting per-family savings; HUD corruption means missed-phase
behavior must be separated from raw family cost.
- projectile, enemy, collectible and splash restore/draw command submission;
- time stalled in the final `WaitBlit`;
- complete update-to-present duration and missed 20 ms deadlines.

Use repeatable scenes: empty grounded run, repeated jump/sprint, two Striders,
two intersecting Striders, projectile maximum and water/collectible peak. Store
median, 95th percentile and maximum rather than only one worst frame.

## Stage 2: whole-codebase C hot-path audit

Status: completed as the broad static/dynamic inventory and maintained as a
living measured audit in `PERFORMANCE_68020_STAGE2_AUDIT.md`. New measurements
must update its ranking before another optimization is promoted.

Audit every C path that can run during loading-to-gameplay transition, a game
tick, frame preparation or display publication. Do not assume `renderer.c` is
the only or largest source of lost cadence. Cover at minimum:

- `main.c`: loop cadence, repeated service calls, phase waits and publication;
- `game.c`: update ordering, active/offscreen entity work and event dispatch;
- `player.c`: physics, animation selection, collision and input state;
- enemy, projectile, collectible and water simulation and collision paths;
- `renderer.c`: ring maintenance, Copper patching, sprite staging and Bobs;
- HUD updates, audio request/playback paths, input and Amiga platform helpers;
- asset/cache preparation that may unexpectedly recur after `CHARGING`.

For each runtime function, record or estimate call frequency per game tick and
per presented frame. Inspect nested loops, maximum iteration counts, tiny or
repeated memory copies, Chip-RAM versus Fast-RAM access, redundant calculations,
offscreen/inactive scans, and CPU work that could overlap safely with the
Blitter. Specifically search for multiplication, division and modulo, variable
shifts, structure copies, signed widening, indirect calls and library/compiler
runtime helpers that are disproportionately expensive on 68020/68030.

Do not judge cost from C syntax alone. Compile representative production and
diagnostic objects with the actual VBCC flags, inspect the generated 68k
assembly and symbol sizes for suspicious functions, and confirm hypotheses with
CIA-timer scopes. Add only narrowly targeted profiler slots; production must
remain free of diagnostics. Record median, 95th percentile, maximum, calls per
frame, target memory domain and whether cost is CPU time, Blitter wait or Chip-
bus contention.

Produce a ranked audit table before optimizing anything. Each entry must state:

1. source/function and triggering scene;
2. measured or bounded frequency and cost;
3. evidence versus inference;
4. proposed optimization and expected saving;
5. visual/gameplay/timing risk and required regression gates.

The five already identified candidates remain mandatory audit entries, but
their final order is decided by comparable measurements across the whole game:

1. up to about 4,992 two-byte `CopyMem` calls during one entering-column ring
   update;
2. the complete 1,536-byte inactive Copper-list copy on every game update;
3. about 1,600 bytes of wide-player sprite image data recopied even when frame
   and facing are unchanged;
4. serialized per-plane Bob restore/draw operations and `WaitBlit` calls;
5. player physics and collision work, including per-pixel `moveY()` and full-
   sole/span scans.

Also compare audio request generation with Paula playback. Repeated or absent
effects under load may originate in input/update/event scheduling rather than
the autonomous DMA playback itself. Instrument logical requests separately
from channel starts without changing priority or sound contracts.

An optimization may start outside `renderer.c` when the ranked evidence shows
greater savings in gameplay, collision, audio, input or platform code. Change
one measured cause per prototype so its effect remains attributable.

## Stage 3: CPU/gameplay candidates

The first measured hypothesis is vertical player collision. `moveY()` advances
one pixel at a time and each step scans the 24-pixel sole through
`collisionSolidHorizontal()`. Jump velocity can therefore add many collision
map calls even though Sparkpaw itself is a hardware sprite. Replace only after
measurement with tile-span/leading-edge queries and exhaustive host tests for
ceilings, ledges, landing, water, crouch clearance and the accepted route.

Also measure before changing:

- repeated full-span probes in `moveX()`, `canStand()` and ledge support;
- all 48 collectibles and seven persistent Striders updating every tick;
- projectile swept collision under rapid fire;
- division/modulo generated by animation and state code on 68020.

Prefer lookup tables, shifts and tile-run queries only where profiles show a
material cost. Keep simulation deterministic.

### Initial static renderer findings after alpha.43

Production and diagnostic binaries were compared before any source split. The
normal executable contains none of the diagnostic strings or log writer and is
7,268 bytes smaller than the Stage 5L proof. `performance_profile.c` and its
calls compile to no-ops without `SPARKPAW_RENDER_DIAGNOSTIC`. Moving those
blocks to `renderer_diagnostics.c` improves reviewability, not frame rate.

The first concrete renderer candidates found before the whole-codebase audit
are:

1. `prototypeCopyCanonicalSpan()` can issue about 4,992 two-byte `CopyMem`
   calls for one 16-pixel, 208-line entering-column update across three ring
   copies, four planes and clean/display targets. Benchmark direct word copies
   or bounded tall Blitter operations; this is the strongest periodic-hitch
   candidate.
2. `rendererUpdateGameplay()` copies the complete 1,536-byte Copper list to the
   inactive list before patching a small dynamic subset every update. Benchmark
   independently maintained lists or template-only initialization.
3. `setHardwareSprite()` recopies about 1,600 bytes of wide-player image data
   every update. Cache frame/facing per target stage and update only POS/CTL
   when its image is unchanged.
4. Bob restore/draw uses repeated plane-by-plane `WaitBlit` serialization and
   remains the dominant measured section at 24,357 average CIA ticks.
5. Player physics averages 4,303 ticks and still warrants a measured leading-
   edge/tile-span candidate, but it is not the largest observed section.

Refactor policy: first preserve the normal executable hash while extracting
diagnostics, then optionally split Copper, ring, Bob and sprite internals. Do
not mix a file-boundary refactor with a timing change, and keep all rejected
proof targets reproducible until their compact lessons are archived.

## Stage 4: Blitter and Chip-bus candidates

Use the Hardware Reference Manual's channel-cost formula to predict each exact
blit, then validate on the production display with seven bitplanes and sprites.
Candidate order:

1. skip restores/draws only for objectively unchanged or fully culled objects;
2. coalesce overlapping dirty background restores without changing Bob bounds;
3. reduce Blitter setup/waits by batching safe operations while preserving
   family priority;
4. benchmark interleaved display/cache layouts as an isolated proof, because a
   single taller operation may reduce plane-by-plane setup but changes modulo
   and cache contracts;
5. retain the accepted FMODE1 bitplane fetch and 64-pixel attached player pair;
   benchmark any additional AGA sprite use separately and reject it on any
   priority, palette, overlap, HUD or DMA regression.

Stage 5C isolates the conservative 32-bit form (`FMODE.BPL32=1`): 44 fetched
bytes at the existing DDF window, longword-aligned 32-pixel pointer steps and
AGA extended fine scroll. The HUD keeps its authored 336 visible pixels but
uses a 352-pixel private stride. This must be rejected on any wrap, HUD,
parallax, sprite or edge artifact; compilation alone is not acceptance.

Stage 5C2 proved 49.96 FPS on the fast FS-UAE/68030 configuration but is
visually rejected: its fine-scroll delay used 31..0 instead of retaining the
accepted FMODE=0 phase. Stage 5C3 changes only that sequence to 15..0,31..16;
it requires a fresh 68030 presentation gate before any 68020 measurement.

Stages 5D through 5D5 isolate the remaining origin defect. A matched FMODE0/
FMODE1 capture measured the candidate exactly 16 logical pixels to the right;
the aligned `camera + 16` phase correction then returned the synthetic marker
pixel-identically to the FMODE0 reference through the user-observed sweep.
Stage 5E applies that relation to gameplay and the fixed HUD without changing
DDF or using misaligned pointers. Gate it on FS-UAE/68030 presentation before
using its diagnostic cadence log for a 68020 performance decision.

Do not retry full viewport copies. Dirty work must remain proportional to
changed visible objects.

### Completed fetch/sprite branch: Stage 5L

Stage 5G's early FMODE1 fetch and physical guards produced the coherent world
but stole later DMA slots from the six-channel actor. FMODE3 production mapping
and the FMODE0 `$28` alternative were rejected. Stage 5L resolves the branch by
packing the unchanged actor into one transparent-padded 64-pixel attached pair
on channels 0/1. It is accepted for broad presentation across the supplied
emulator, real-Amiga and FPGA paths and is now the immutable performance
baseline. Hybrid/full-player-Bob fallbacks are closed unless new hardware
evidence invalidates the wide pair.

Additional AGA sprites remain a hypothesis, not the next automatic change.
Channels 2..7 are newly available, but early DDF can still deny later slots;
objects overlap vertically; attached colour depth consumes pairs; and moving a
Bob to sprites trades restore/draw work for fixed DMA and Copper scheduling.
Measure projectiles or another small vertically bounded family in isolation
before considering enemies or collectibles.

Primary hardware basis: the Commodore Hardware Reference Manual states that
bitplane DMA can take precedence over sprite DMA and that wider/scrolling
displays can make higher-numbered sprites unusable. Its sprite chapters confirm
that an attached 15-colour object consumes a channel pair and that channel
reuse requires vertical separation. Commodore's AA/CD32 material confirms
32/64-pixel AGA sprites, attached sprites in all resolutions, dual four-plane
playfields and independent FMODE controls for bitplane and sprite fetch width.

## Stage 5: decision gates

- Gate A: bit-identical/visually accepted 68030 presentation.
- Gate B: no new Strider, beetle, projectile, diamond, water or HUD corruption.
- Gate C: measured 68020 improvement in jump/sprint and the two-Strider scene.
- Gate D: full route with the explicitly packaged debug build, followed by a
  deliberate safe log flush and reset. Gameplay samples buffer in Fast RAM;
  production HD/ADF contain no profiler, flush input or log facility.
- Gate E: only then package a new production checkpoint and seek real hardware.

If no candidate reaches a stable 50 Hz deadline, use an explicit fixed 25 Hz
game-update cadence with 50 Hz display/Copper service as a deliberate fallback,
not uncontrolled frame skipping. Evaluate input feel and animation speed before
accepting it.

## Primary references

- Commodore Amiga Hardware Reference Manual, Blitter Hardware and Blitter
  Speed: https://www.amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0118.html
- Amiga ROM Kernel Reference Manual: AmigaDOS, Mask and MaxTransfer:
  https://developer.amigaos3.net/sites/default/files/downloads/2024-10/Amiga_ROM_Kernel_Reference_Manual_DOS.pdf
- AGA fetch-mode programming notes (to be treated as a benchmark hypothesis,
  not authority): https://jvaltane.kapsi.fi/amiga/howtocode/aga.html
