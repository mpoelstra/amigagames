# Sparkpaw real-hardware renderer glitch correction plan

Status: investigation and planning only. No renderer change is implemented by
this document.

## Objective

Remove the repeatable alpha.41 ADF HUD corruption and the load-dependent
Strider/beetle flicker as one renderer-safety problem where the evidence supports
that conclusion. Preserve every pixel, colour, animation, collision contract,
dual-playfield 4+3 composition, separate HUD, parallax and gameplay behavior.
Do not add map-coordinate or enemy-type workarounds.

## Chosen execution order

Keep Stage 1 deliberately short: obtain only enough trace evidence to correlate
the fixed HUD point and two-Strider load with missed completion or unsafe
publication. Then proceed directly to an isolated Stage 2 plus Stage 4 rolling-
buffer prototype. Stage 3's one-visible-Strider budget is a measured fallback
or an intentional encounter-design option, not the preferred production fix.
Do not spend a release checkpoint optimizing the unsafe full-world in-place
path before testing the structural design.

## Evidence boundary

- Real A1200/68030 alpha.40 and alpha.41 HD footage shows intermittent enemy
  corruption around the first two-Strider scene. Beetles are also reported to
  glitch occasionally.
- The user reports occasional glitches at later, more enemy-heavy positions as
  well; in the observed cases at least one Strider appears to be on screen.
  The strongest first-two-Strider corruption coincides with an obvious frame-
  rate drop. This supports a load threshold rather than a defective map cell,
  while remaining an observation to correlate with the diagnostic trace.
- In the same first-two-Strider scene, killing one Strider makes corruption on
  the remaining Strider almost disappear and partially restores frame rate.
  This within-scene load reduction is strong evidence against one defective
  Strider asset/frame and for a threshold in composition time or Chip-bus use.
- Real A1200/68030 alpha.41 ADF footage shows broad temporary HUD corruption at
  the same early world position in both directions, reproducibly after reboot.
- Camera footage cannot identify the exact corrupt pointer or raster line, but
  the deterministic on-machine reproduction rules out a one-off capture beat.
- Historical renderer logs prove that the line-253 in-place Bob pass routinely
  wraps into the following PAL frame. Examples include alpha.30 at 3,525 wraps
  in 3,844 passes and alpha.31 at 6,599 wraps in 6,796 passes.

## Source audit: current unsafe contracts

The game loop waits by comparing only the current raster-line number. It has no
frame epoch and cannot distinguish “this frame before the deadline” from “a
later frame after one or more wraps.” A long Bob pass can therefore re-enter
update/publication in an unintended phase.

One active Copper list is patched in place. `rendererUpdateGameplay()` writes
the two 16-bit halves of world, sprite and HUD pointers while that same list is
being consumed. The HUD pointer moves are below the line-252 wait, so a late
update can expose a mixed old/new pointer for one frame. This is the leading
explanation for full-band HUD noise, but must be measured rather than assumed.

The Bob pass restores and draws directly into the displayed resident
`frontDisplay`. Once it wraps past PAL line 311, those writes race the next
frame's scanout. Existing vertical ordering only changes which enemy is drawn
first; it cannot make an over-budget in-place pass safe. This is sufficient to
explain load-dependent partial Strider/beetle presentation.

## Stage 1: deterministic diagnostic proof

Create a separately named debug executable; production HD/ADF must remain free
of logging and debug input. Buffer a compact fixed-size trace in Fast RAM and
flush only through the established debug reset workflow.

For each frame record:

- monotonically detected PAL frame epoch plus raster line at update entry,
  publication entry/exit, Bob entry and final Blitter completion;
- whether line 100, line 252, line 253 or a full-frame deadline was missed;
- camera/player X, active/drawn enemy slot, spawn, type, frame and facing;
- restore/draw counts by projectile, beetle, Strider, collectible, splash and
  water family;
- HUD buffer index and complete pointer values intended for all seven Copper
  bitplane entries;
- Copper list address/current publication generation and Blitter-busy state.

Add a host-tested frame-phase state machine. Raster wrap detection must be
explicit and correct even when sampling skips lines or an entire frame.

Run two focused supplied tests:

1. alpha.41-equivalent ADF, repeatedly cross the deterministic HUD point in
   both directions after a cold boot;
2. HD, use a controlled load ladder: no visible Strider, one Strider, the first
   two simultaneous Striders, and later mixed enemy-heavy scenes. Park and
   traverse each scene, including a reported beetle-glitch area, and correlate
   every visible glitch and cadence drop with the trace. At the first pair,
   capture a before/after segment that changes only one factor by killing one
   Strider while keeping the camera and remaining enemy in the same scene.

The diagnostic gate is a timestamped event that correlates visible corruption
with a missed phase, mixed publication generation or in-place scanout overlap.
If HUD corruption occurs with none of these, stop and inspect address bounds,
Copper-list integrity and DMA fetch configuration before changing scheduling.

## Stage 2: make display-state publication atomic

Build two complete Copper lists in Chip RAM. The CPU patches only the inactive
list, including world scroll, player sprite and HUD pointers. Publish one list
address once at a proven frame boundary; never modify entries still reachable
by the active Copper.

Use a frame epoch/deadline state machine instead of independent line-number
loops:

- one simulation update may begin only in its assigned phase;
- if publication misses its boundary, retain the prior complete Copper state;
- never start a second update merely because a wrapped raster line now happens
  to be numerically above 100;
- count dropped publications explicitly while keeping input/simulation policy
  deterministic.

Host tests must model on-time, line-252-late, one-wrap and multi-wrap cases and
prove that no active-list word is changed.

Gate this stage first on the repeatable ADF HUD point. It should remove full-HUD
pointer corruption even before Bob composition is redesigned. This is an
isolation gate, not final acceptance if enemies still flicker.

## Stage 3: low-memory load-budget proof

Before allocating another full world, test an explicit encounter budget: at
most one Strider may be rendered inside the camera/culling region. Other
Striders remain persistent but wait outside the presentation margin, and enter
only after the current Strider dies or leaves far enough that no pop is visible.
This is a legitimate level-direction choice, not a hidden emergency skip.

Host tests must cover approach from both directions, traversal links, death,
respawn, shots and camera reversal. Every authored Strider encounter remains;
only simultaneous visibility changes. Compare identical scenes with zero, one
and two Striders in a diagnostic build to quantify the saved CPU, Blitter and
Chip-bus work.

This candidate is accepted as the complete in-place solution only if the worst
full-route Bob pass finishes before the next visible scanout with a measured
safety margin and zero wraps. It cannot by itself excuse the current live-Copper
publication or a remaining one-Strider overrun. The user already reports rare
later glitches whenever at least one Strider is visible, so assume that the cap
reduces risk until measurement proves it removes the unsafe condition.

In parallel, benchmark these low/no-Chip-growth candidates independently:

1. interleaved FRONT16 display rows and repeated per-plane masks so a complete
   multi-plane restore/draw can be issued as one taller Blitter operation rather
   than four separately submitted plane blits;
2. AGA 32- and 64-bit fetch modes with correct alignment, modulo, sprites and
   fine scrolling, measuring freed DMA slots rather than assuming a gain;
3. skip unchanged enemy redraws only where both position and target-buffer state
   prove it safe; consider an explicit 25 Hz Strider motion/animation cadence
   only as a designed presentation choice, not uncontrolled frame loss;
4. combine or schedule overlapping restore regions only when byte traffic is
   actually reduced and accepted overlap order remains unchanged.

### Parallax/background finding

The current foreground and rear pictures are resident bitplanes; the CPU does
not redraw them per frame. Slowing the parallax scroll therefore saves almost
no CPU work. REAR8 does consume three continuous bitplane DMA streams—roughly
26,208 fetched bytes per 208-line frame at the current 42-byte fetch—so its bus
cost must be included in the profile. Test fetch mode and exact fetch width
before considering any visual change. Do not delete parallax or reduce its
three planes merely because a synthetic benchmark becomes faster.

## Stage 4: compact rolling renderer prototype

Prototype the structural design directly with two screen-sized FRONT16 targets,
each slightly wider than the viewport, plus only the clean tile/strip state the
implementation actually needs. Start near 336x208 but derive the final guarded
width from fetch mode, word alignment and the full fine-scroll range rather than
hard-coding that estimate.

- hardware fine-scroll handles pixel movement;
- only newly exposed tile columns or dirty dynamic clean regions are built;
- each target owns its previous-object/dirty history;
- water and diamond changes reach a target only while it is inactive;
- current Bobs are drawn, the final Blitter operation completes, and only then
  is the target published through the inactive Copper list;
- neither CPU nor Blitter ever writes the target currently scanned by Agnus.

Do not revive alpha.38's rejected unconditional 512x208 full clean-world copy.
The proof must exercise fine scrolling, both camera directions, every tile-ring
wrap, water, diamonds, intersecting Striders and both world edges. Compare CPU,
Blitter and Chip-bus time against alpha.41 rather than assuming the smaller
memory footprint is automatically faster.

Keep full-world ping-pong only as the simpler fallback. It would retain
`frontClean` and add another 3072x208x4 display bitmap (319,488 bytes at the
current 384-byte stride), with per-target dirty history and no writes to the
displayed target. Alpha.41's 642,016-byte permanent saving makes that plausible,
but it consumes much of the recovered Chip margin and does not improve working-
set locality. Measure normal-Workbench free/largest blocks before considering
it; do not silently spend the complete alpha.41 gain.

### Classic platformers and current Scorpion practice

Treat Superfrog, Ruff n Tumble, Marvin's Marvellous Adventure, Brian the Lion,
the Amiga conversions of Aladdin and The Lion King, and Global Gladiators as a
comparative reverse-engineering set. Their long worlds and dense presentation
make bounded tile/window buffers, camera-local actor activation and deliberate
Bob/sprite budgets technically likely, but the exact implementation of each
title is not established by a primary programmer source. Do not cite inferred
engine details as facts; inspect their Copper lists, bitplane pointers, map
access and draw traffic before using a title as proof.

The actively maintained Scorpion Engine provides a better documented modern
comparison. Its official optimization guidance recommends:

- keeping the screen buffer only slightly larger than the visible screen;
- making distant actors dormant and scanning them at camera tripwires rather
  than executing every actor every frame;
- recycling or trashing actors after they leave an appropriate camera window;
- using hardware sprites when the palette and channel budget permit it, with
  Blitter objects as the fallback;
- keeping object widths aligned to 16 pixels, reducing animated/foreground tile
  work and selecting the highest usable AGA fetch mode;
- keeping panels small and avoiding unnecessary variable changes because those
  trigger panel refreshes.

These are design references, not a drop-in engine recipe. Sparkpaw already uses
six of eight hardware-sprite channels for its 16-colour player, so a 64x64
16-colour Strider cannot simply be moved to the two remaining channels. Its
32-pixel beetles and 64-pixel Striders are already favourably width-aligned,
and the separate three-plane HUD is already conceptually sound. The largest
actionable differences are the full-width resident world target and persistent
enemy processing: prototype the bounded rolling buffer and camera-tripwire
dormancy together, while retaining persistent gameplay state outside the
active window.

## Stage 5: timing and performance after correctness

With displayed-memory writes removed, profile CIA-timed costs per family using
the existing stock-68020 plan. Optimize only measured hotspots. The glitch fix
may include scheduling work required for safety, but broad physics, Blitter DMA,
fetch-mode or 25 Hz policy changes remain a later checkpoint.

Do not use uncontrolled frame skipping. If composition cannot complete in one
frame, retain the last complete display buffer and publish the next complete
buffer later. Simulation/display cadence must be explicit and measured.

## Acceptance gates

1. Host: frame-phase, inactive-Copper publication, per-buffer dirty ownership,
   bounds and existing renderer/gameplay regressions pass.
2. FS-UAE 68030: unchanged assets/colours and clean traversal of HUD trigger,
   two-Strider overlap, beetles, projectiles, diamonds and water.
3. Real A1200/68030 HD: repeated two-Strider and beetle scenes show no flicker.
4. Real A1200/68030 ADF: cold boot, repeated bidirectional HUD crossing and the
   same enemy scenes show no corruption.
5. Analogue Pocket Amiga core at the supplied 68020/no-cache, AGA/PAL/turbo,
   2 MB Chip plus 32 MB Fast configuration: repeat the alpha.39 route and verify
   that Bob, gameplay-field and HUD corruption is absent. This is a separate
   FPGA/core stress result, never a substitute for FS-UAE or real hardware.
6. Memory: normal Workbench with about 1.4 MB free Chip RAM still loads; record
   total and largest Chip/Fast blocks rather than inferring from compilation.
7. Only after presentation acceptance, resume the separate FS-UAE stock-68020
   performance plan.

No FS-UAE, ADF or hardware acceptance may be claimed until supplied testing
establishes its corresponding gate.

## External engineering references

- Andrew Braybrook, *Scrolling on the Amiga* (Uridium 2 and Fire & Ice):
  http://uridiumauthor.blogspot.com/2017/12/scrolling-on-amiga.html — describes
  double/triple buffering, per-screen restoration lists, VBlank Copper updates,
  interleaved one-operation Bob plotting and rolling tile buffers.
- Commodore, *Amiga Hardware Reference Manual*, memory/DMA overview:
  http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0005.html
- Commodore, *Amiga Hardware Reference Manual*, Blitter speed:
  https://www.amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node0118.html
- Martin Pedersen interview on *Hybris* and *Battle Squadron*:
  https://cope-com.com/martin-pedersen-interview/ — Battle Squadron moved its
  larger colourful enemies from sprites to Bobs, updated enemy Bobs every
  second frame, and erased/redrew them in raster bands to obtain about 1.5
  frames of plotting time without double buffering. This is evidence for
  deliberate actor/cadence budgets, but Sparkpaw should not copy its beam-race
  scheduling without proving safety around the free camera and separate HUD.
- Scorpion Engine official demo repository and wiki optimization guidance:
  https://github.com/earok/scorpion-editor-demos,
  https://github.com/earok/scorpion-editor-demos/wiki/Optimizations and
  https://github.com/earok/scorpion-editor-demos/wiki/Recycling — documents
  bounded buffers, actor dormancy/recycling and tripwires, hardware-sprite
  selection, 16-pixel alignment, AGA fetch-mode choices and panel update costs.

Comparative reverse-engineering backlog: Banshee, Xenon, Xenon 2, Superfrog,
Ruff n Tumble, Marvin's Marvellous Adventure, Brian the Lion, Aladdin, The Lion
King and Global Gladiators are useful long-level/actor-budget references, but
no sufficiently detailed primary programmer source has yet been found for each
title's exact background-buffer layout. Do not turn visible behavior or review
commentary into undocumented engine facts; inspect executable memory access,
Copper lists or authoritative developer material before using them as design
evidence.
