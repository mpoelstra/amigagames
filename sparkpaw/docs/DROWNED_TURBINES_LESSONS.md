# Drowned Turbines — retained design and implementation lessons

Recorded 23 September 2026 from the level development and background-animation
review. This is a reusable lessons document, not a release checklist or the
current test-build inventory. See [CURRENT_STATUS.md](CURRENT_STATUS.md) for
current acceptance and [DROWNED_020_ENGINE_AUDIT.md](DROWNED_020_ENGINE_AUDIT.md)
for subsequent performance work. Other sessions are actively changing those
areas; the rear-animation sections distinguish their initial implementation
from the later optimization synthesis below.

## Retained 68020 optimization lessons — September 2026

This synthesis covers the full-level FPS round, accepted two-copy layout and
Level1 transfer. It does not supersede the later media work in
[CURRENT_STATUS.md](CURRENT_STATUS.md). The user accepted the integrated HD
campaign; that is functional/subjective acceptance, not measured integrated
50 FPS or WHDLoad/ADF/hardware acceptance. Music remained enabled throughout.

### Separate less work, measured cadence and player experience

| Change | Established result | Performance/acceptance boundary |
| --- | --- | --- |
| Rear staging pointer increments | Native code removes 84 repeated row multiplications; same pixels, calls and DMA traffic. | Retained small reduction; user saw no clear change. Do not promote this as the busy-scene solution. |
| Direct rear-water upload | Fast-to-canonical copy then three display blits instead of six blits through a private stage. Counted transfer traffic falls from 9,240 to 5,544 bytes per full water update. | Retained at user request; no convincing matched busy-route FPS improvement. These are bus-transfer counts, not bytes of unique image data or whole-frame savings. |
| Enemy mask vertical bounds | Precomputed top/height removes transparent draw AND subsequent restore rows; original frame plane stride stays intact. | Retained at user request; small/no clear subjective improvement, unmatched runs do not prove a causal gain. |
| Two-copy foreground ring | Two 1024×208×4 target payloads instead of 1536×208×4 save 106,496 bytes (104 KiB); two rather than three mirrored copies. | Drowned user reported improvement and approved the complete level. Minimal-cadence evidence supports a useful gain, not a controlled fixed percentage or stable 50 FPS. |
| Level1 ring transfer | Same allocation reduction, with a Level1-specific physical bounds guard; Stormrail excluded. | User accepted B, then the integrated HD campaign. Whole A48.73/B49.02 FPS came from unequal routes/reset exposure; not a causal speedup measurement. |
| Blitter reset experiment | One A/B run reduced the two reset-associated intervals from 16 to 10 PAL fields. | Not promoted. One reset per run and little perceived change did not address the user's sustained busy-scene problem. Do not silently re-enable it. |

The direct-water path is **not** permission to CPU-write the live display.
Canonical rear memory is non-displayed on this path, outstanding readers are
retired first, and the guarded display is updated through the established DMA
sequence. See the ownership section below.

For Drowned ring A/B, late ferry cadence was 38.76 → 41.72 publications/s;
upper route after reset was 38.71 → 41.82. That last comparison covers only
11.78 versus 5.50 seconds, with different manual workloads. Whole-run numbers
were 44.83 → 46.46. Keep these qualifications attached to the numbers. The
104-KiB allocation saving is arithmetic on actual buffer dimensions; free-Chip
snapshots also include OS/emulator state and did not demonstrate the same net
increase in that A/B. Executable size, allocation payload and available memory
are three different measurements.

Evidence: [full FPS round](DROWNED_FULL_FPS_ROUND.md),
[renderer plan](DROWNED_RENDERER_PERFORMANCE_PLAN.md),
[Level1 transfer](LEVEL1_TWO_COPY_RING_PLAN.md); preserved raw logs in
`testresults/Drowned-ring-{A,B}-020-run1.log`,
`testresults/Drowned-water-direct-{A,B}-020-run1.log`,
`testresults/Drowned-enemy-bounds-{A,B}-020-run1.log` and
`testresults/Level1-ring-{A,B}-020-run1.log`, with provenance sidecars.
Consult the individual records for exact executable hashes and observer flags.

### Busy screens expose accumulated work, not necessarily enemy AI alone

On the retained-ring sparse trace, late-ferry gameplay medians were roughly
5.8–6.0 ms and foreground/Bob composition 10.8–11.2 ms. Nested contributors
included restores around 2.7 ms, ring/dynamic synchronization around 2.6 ms,
other draws around 2.4–2.6 ms and enemy drawing around 1.2 ms. These conditional
estimates include IRQ/DMA/observer effects; they are not pure CPU timings.
Do not add medians or nested parent/child scopes to manufacture frame time.

Rear work happens after publication: occasional approximately 3.9-ms samples
can delay the NEXT simulation/render cycle. The trace did not tag water versus
waterfall, and publication-wait clock pairs were rejected, so neither the
specific rear operation nor exact missed-deadline slack was established.
The 82-sample trace rejected 106 invalid scope pairs; valid subsets can be
biased. Its FPS must not be compared directly with minimally observed runs.

Lesson: reducing enemy count, music quality or vegetation is not justified by
“many things on screen.” First inspect the complete update/restore/draw/sync
chain, recurring transfer volume and serialization. Existing clearance tables,
hazard caches, staging caches and traversal optimizations must be inventoried
before proposing them again as new work.

### Structural savings require a geometry proof, including each level's tail

The successful ring change rebases the physical origin from 512 to 96 and
rotates canonical slots as `(world + 96) & 511`; it is not just changing the
allocation width or skipping the third copy. Preserve guarded AGA fetch,
canonical patch destinations, target-local restore history, wrap and reset.
Actual-C tests compare fetched pixels for all cameras, both scroll directions,
teleports, overlapping Bobs, dynamic patches and inactive-target immutability.

Drowned's proof does not transfer unchanged to Level1. At Level1 camera 3072,
resident origin 2880 maps to physical -96. The visible fetch is valid while part
of the logical resident window is outside the physical target. The retained
Level1 `prototypeRectFits` guard rejects negative/overflow destinations before
writing, including splash drawing without its own screen cull. Proven rejected
rectangles up to the actual 64-pixel actor bound lie outside the visible view
plus 16-pixel margin. Never record restore history for a rejected draw.

Proofs: [Drowned ring test](../tests/test_drowned_two_copy.py),
[Level1 ring test](../tests/test_level1_two_copy.py) and
[layout contract](../src/drowned_ring_layout.h). Stormrail has different target
preparation and remains outside this flag's scope. Historic H3 fetch-union
pruning was a different rejected experiment: fewer writes lost their advantage
to CPU indexing/branch costs. Do not confuse it with the proven rebased layout.

### Measure the build being played and audit the code actually generated

Bind every measurement to executable hash, build ID, observer flags, assets,
route, sample duration and reset state. A log present beside an executable is
not automatically from that executable; startup logs can append across runs.
Preserve raw evidence before staging or replay. Confirm the expected completion
footer, not merely a stable file size or frozen image. Save completion is a
separate operation from playing; a diagnostic frozen image is expected.

Use minimal cadence for A/B and bounded sparse scopes for locating costs. Both
still have observer overhead, including inactive branches on unsampled frames.
Music owns its timer/interrupt resources: never restore the old conflicting
renderdiag setup to obtain more counters. No per-frame disk writes. TOD zero/
two-field phase pairs are not exact visible missed-deadline counts; reject torn
field/raster timestamps and report missing scope evidence honestly. Absence of
an ownership field does not mean zero ownership violations.

Native inspection caught the write-only BLTALWM readback described below;
readable host-register mocks could not. Likewise, compile isolation and actual
68020 assembly parity are stronger evidence than a runtime section flag.
The integrated HD audit preserved existing hot units and used a separate
Drowned module, but a changed final link can still change instruction-cache
placement. Byte-identical function code does not prove identical whole-game
cadence. Retest affected sections after architecture or media-link changes;
do not infer a cache-conflict cause from addresses alone.

### Stop micro-tests when the remaining question is scheduling

The bounded Drowned round ended after the accepted ring improvement; remaining
small water/collision opportunities were not grounds for another series of
manual A/Bs. A future larger step needs a fresh measured hypothesis and budget:
immutable render snapshots, per-target restoration descriptors and explicit
source/DMA lifetimes before overlapping CPU simulation with rendering. Simply
removing `WaitBlit`, starting one short blit before `gameUpdate`, or adding an IRQ
for every tiny plane is not a demonstrated pipeline improvement.

Interleaved planes, hardware-sprite enemies and wider playfield fetch remain
separate proposals with layout, palette, occlusion, channel and display risks.
None is required or proven to guarantee 50 FPS. Preserve the accepted music,
visual quality and gameplay, and stop when measured benefit no longer justifies
complexity. Media packing and cold-load speed are separate from gameplay FPS.

## Background animation: what looked good

The accepted effect was reached through specific visual comparisons:

| Study | User response | Retained lesson |
| --- | --- | --- |
| Shimmer V1: small brightness changes in horizontal palette bands | Almost invisible | Low numerical cost does not guarantee a perceptible effect at game scale. |
| Shimmer V2: stronger whole-band brightness changes | Too harsh / unattractive | Increasing brightness alone is not a substitute for spatial variation. |
| Shimmer V3: scattered short horizontal water streaks | Preferred direction | Local shapes read as reflected light better than broad flashing bands. |
| Shimmer V4: V3 plus restrained shoreline illumination | Approved as **water animation v1** | Combine local glints with a softer secondary motion cue. |
| Four-frame waterfall streaks plus a small foam edge | Approved, then requested at more locations | Animate existing water shapes rather than putting unrelated particles over the landscape. |

The frozen approved baseline is
[assets/concept/drowned-water-animation-v1](../assets/concept/drowned-water-animation-v1/).
Its name must not be confused with the rejected `drowned-water-shimmer-v1` study.
It contains review media, generator snapshot, manifest and approval hashes.
The waterfall review is in
[assets/concept/drowned-waterfall-preview-v1](../assets/concept/drowned-waterfall-preview-v1/).

Glints use staggered 1–8 pixel horizontal dashes within the distant water strip,
not uniformly synchronized flashes. The accepted shoreline contribution peaks
at an 18/255 brightness increment, versus the rejected stronger study's 64.
Four authored waterfall phases provide descending light streaks and a small
moving foam edge. Slight phase offsets across locations avoid synchronized
waterfalls. Judge at native scale and in a complete scene with foreground/HUD,
not only as an enlarged isolated GIF.

The user accepted the corrected animation ingame after the black-stripe fix
below. That is visual acceptance, not proof of stable 50 FPS or acceptance on
all distribution formats and real hardware.

## Turn an approved preview into a reproducible runtime contract

Use one deterministic generator and exact indexed-pixel data for preview and
runtime. Record palette, coordinates, dimensions, cadence, loop length and
frame hashes. Test decoded planar data against approved pixels; do not silently
reinterpret an approved GIF during integration.

Useful sources:

- [preview_drowned_water_glints.py](../tools/preview_drowned_water_glints.py): local glints and restrained shoreline wave.
- [preview_drowned_waterfall.py](../tools/preview_drowned_waterfall.py): representative waterfall and combined preview.
- [build_drowned_rear_ambience.py](../tools/build_drowned_rear_ambience.py): precomputed runtime data and generated layout constants.
- [drowned_rear_ambience.h](../src/drowned_rear_ambience.h): native ownership, scheduling, palette and copy paths.
- [test_drowned_rear_ambience.py](../tests/test_drowned_rear_ambience.py): actual-C host checks and pixel/guard comparisons.

The original combined waterfall preview preserved the approved water pixels
below rear row 184 exactly. Preserve such boundaries when layering a second
effect; otherwise approval of one feature can inadvertently alter another.

## Palette animation has a spatial limitation

The rear uses three planes/eight shared pens. Trees, sky, mountains and water
share colours. Global cycling therefore affects more than water. Even a
restricted horizontal Copper band changes every pixel using those pens in that
band; it is not a semantic water mask.

The accepted combination uses bitmap patches for spatially selective glints
and waterfall flow, plus small raster-restricted palette changes for the shore.
Shore bands start at rear rows 184, 186 and 188, with restoration at 190.
Both AGA high and low colour nibbles must be written and restored, with the
correct bank selection. Patch only the safely prepared Copper list. A cached
phase belongs to each list separately: one global cache can leave the other
list stale. Preserve existing foreground, sprite and HUD palette ownership.

Do not describe Copper effects as free: list execution and bitmap DMA compete
for Chip bandwidth even when CPU work is small.

## Bounded patches instead of full animated panoramas

The initial implementation precomputed 24 water phases and four phases for each
of four waterfalls. The water loop advances every six simulation ticks: nominal
120 ms per phase and 2.88 seconds per loop at 50 simulation ticks/second. This is
simulation cadence, not a guarantee of wall-clock cadence during slow frames.

| Initial integration item | Size / bound |
| --- | --- |
| Complete precomputed frame data in Fast RAM | 205,828 bytes |
| Private Chip DMA staging buffer | 1,848 bytes |
| Extra capacity across two Copper lists | 512 bytes |
| Explicit extra Chip allocation from those items | 2,360 bytes |
| Water area updated | At most 352 pixels wide × 14 rows × 3 planes |
| Work per publication | Water **or** one visible waterfall, not both |

These are component costs, not total level memory or measured free headroom.
No full animated rear bitmap per frame was allocated. Complete small background
rectangles avoid per-pixel particle logic and complicated transparency restores.
The rectangle must contain original background pixels as well as moving water;
blank/transparent pixels cannot simply replace its unaffected surroundings.

Compute patch positions in rear-world coordinates (`cameraX >> 2` here), align
copies to the native word layout and include display-fetch guards. Update newly
exposed water columns even if the animation phase did not change. Visibility
selection and round-robin waterfall updates bound work; inspect fast scrolling
and checkpoint re-entry for delayed or stale patches.

## Buffer ownership and beam timing matter more than a generic “safe line”

The rolling renderer composes the foreground asynchronously. An old fixed-line
reference path is not evidence that a new rear write is safe in that phase.
The original rear update runs immediately after successful line-zero publication;
entry after raster line 64 defers it. The earliest animated rear row is 112
(display line 156 with the existing offset). This provides a working timing
window, not a mathematical completion guarantee on every machine.

Initially, CPU writes went only to the private Chip stage. Blits copied into
both canonical rear data and the guarded display: six plane copies. Updating
only one representation risks stale pixels after scrolling or reconstruction.
Wait for DMA completion before reusing a source or changing its ownership.

**Later optimization must not be confused with that initial rule:** the current
source also contains `SPARKPAW_DROWNED_REAR_DIRECT_WATER`. With a separate guarded
display, it retires outstanding DMA, writes the non-displayed canonical bitmap
from Fast RAM and uses three canonical-to-display blits. This is valid only when
the Copper does not fetch that canonical bitmap and all prior readers have
finished. “Never CPU-write any Chip bitmap” is too broad; the invariant is to
avoid live display/DMA ownership conflicts. See the engine audit for selection,
measurements and acceptance of this path rather than assuming it is universally
active or faster.

## Black waterfall rectangles: a compiled-code lesson

The first native trial showed black rectangles and vertical strips at waterfall
patches and parts of the water strip. The defect was this chained assignment:

```c
hw->bltafwm = hw->bltalwm = 0xffff;
```

With the actual vbcc `-O2 -cpu=68020` build, it wrote BLTALWM, then **read that
write-only register back** to obtain the value for BLTAFWM. The hardware does
not provide the intended value on that read. Correct code uses separate writes:

```c
hw->bltafwm = 0xffff;
hw->bltalwm = 0xffff;
```

The generated assembly then contains two immediate writes, without readback.
Do not chain volatile custom-register assignments or use read-modify-write
expressions on write-only hardware registers. Review actual native assembly
when correctness depends on a hardware access sequence; C-level intent alone
is insufficient.

The host fake treated registers as readable RAM, so the original tests passed.
That was a test-model blind spot, not proof of hardware correctness. Expanded
host coverage now checks masks, all four waterfall rectangles/phases and
untouched plane/row guards. The targeted
[audit_drowned_rear_masks.py](../tools/audit_drowned_rear_masks.py) inspects native
assembly for the known bad pattern and expected separate writes. It is not a
general proof that every register access is legal.

Evidence is preserved as `testresults/Drowned-waterfall-black-stripes-1.png`
and `-2.png`, with provenance TXT sidecars. The rejected drawer was archived
as `dist/older-builds/Drowned-Level-020-H-old-085301`. The corrective build changed
only executable and ReadMe; animation assets were identical. User subsequently
reported the result looked good.

## Broader level-art and interaction lessons

- A panorama sized for an early slice cannot be repeated arbitrarily when the
  level grows. The 1120-pixel source had incompatible edges and produced a hard
  seam near the first Governor. The accepted 1552×208 continuation retained the
  first 832 columns exactly and used an offline connected overlap join. Check
  every camera/fetch window, including guards, not just the final screenshot.
- High-quality station and Governor art exposed the weaker platform kit.
  Consistency requires materials, readable edges and supports across the whole
  level; more hardware sprites are not inherently required for better static
  pixel art. Repeated tiny overlays can create a cut-and-paste ground pattern.
- Root vegetation into the ground. Large trees behind the player and smaller
  shrubs/flowers in front provide depth, but foreground objects should not
  expose implausible feet beneath their base or obscure precision landings.
  Static occlusion still has a rendering cost; “not animated” does not mean free.
- Visual grounding and collision must agree. Geyser bases must meet the platform,
  and decorative deck tops must match where feet actually land. Review both
  standing and jumping, not a single pose.
- The gate's rear/front split made passage convincing. It also requires physical
  consistency: solid header collision, correct foreground post occlusion and a
  readable opening. An attractive sprite alone does not establish the behavior.
- A station reveal must keep Sparkpaw visible and reveal the Core with the
  intended composition before contact. Test the transition while moving, with
  enough approach space; a nice final camera position can still have a bad pan.

## Verification and future reuse

Keep concept approval, host pixel/memory checks, native assembly checks, emulator
visual acceptance, performance evidence and real-hardware acceptance separate.
ASAN/UBSAN cannot model custom-register bus behavior or beam deadlines. A pleasing
GIF cannot validate scrolling, palette restores, DMA ownership or 020 frame time.

For changes to these effects, compare the same music/gameplay build and route:
opening waterfall, precision section, second ferry upper route, deliberate death
and checkpoint repeat, then Governor/station. Preserve logs and executable
provenance before another run overwrites them. Historic diagnostic/audio conflicts
mean instrumentation itself must be validated; unmatched manual runs cannot
establish a small causal FPS gain.

Run generated-asset checks after generation completes. One earlier parallel check
observed an intermediate file; a transient build product is not the final asset.
Stage a new candidate with a byte-preserved prior drawer and a changed-file
inventory. Keep approved art and gameplay constant when isolating a renderer fix.

The original [background ambience proposal](DROWNED_BACKGROUND_AMBIENCE_PROPOSAL.md)
is historical research, not current implementation status. Birds, independent
mist/cloud motion and other proposed effects are not implied by acceptance of
water animation. Do not add them automatically while optimizing the accepted
presentation.
