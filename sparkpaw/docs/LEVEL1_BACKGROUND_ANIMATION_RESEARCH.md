# Next-session brief: Level 1 background animation

## 2026-09-26 — alpha.11 local release complete

Approved v5 Level1 ambience integrated and packaged as 0.7.0-alpha.11.
`make`, final `make release`, full host suite and independent checkpoint
verification pass. Nine artifacts and three extracted drawers are current in
dist: HD ZIP/LHA, standard WHDLoad ZIP/LHA, High RAM ZIP/LHA, three ADFs.
All 75 runtime/bank assets, icons and disk file readback verified. Production
omits animation test counters and diagnostic shortcuts. Source and approved
v5 animation data match; prior graphics/audio unchanged.

Alpha.10 (179 files) and v5 test/evidence (80 files) archived byte-identically
under `dist/older-builds/alpha10-and-level1-tests-20260926`. Prior packaging
attempts retained. Public itch still alpha.8; canonical English release notes
cover that full delta. No commit, push, upload or automatic emulator launch.

Disk1 has 22 free 512-byte blocks (11 KiB), passing the existing 16-block floor
but below 16 KiB; this constrains further growth. Disk2/3 have 206/103 blocks.
Details/hashes: `sparkpaw/docs/RELEASE_VERIFICATION_0.7.0-alpha.11.md`.
Focused visuals accepted; final-media replay, minimum-68020 performance and
real hardware remain pending. Intermittent hardware HUD-boundary glitch open.

## 2026-09-26 — alpha.11: approved Level1 electrical ambience

User accepted the fixed-size v5 building light and explicitly requested game
integration and a new alpha release. Candidate identity: 0.7.0-alpha.11,
Phase 7B.1 scenery refinement. Public downloads verified as alpha.8 with the
itch detector; canonical English release notes include the full alpha.8 delta.

Production enables the approved 48-phase sequence in the isolated Level1
renderer: downward tower pulse/crystal response, sixteen sky-discharge sites,
and blinking of exactly five original blue building pixels. Existing palette,
gameplay, HUD and scrolling contracts retained. Inactive rear writes complete
before Copper publication; source bitmap stays immutable. Normal campaign
entry points retained, with no focused-start or diagnostic-save code enabled.

HD loads the raw 99,268-byte frame file. ADF uses the existing CRC-checked packed
asset reader; standard WHDLoad uses that reader's resident bank backend. The
new file belongs only to Level1 (ADF Disk1 and WHD level1 bank), not Stormrail
or Drowned. Extra Chip payload is 96,000 bytes at the native-observed stride;
Fast frame copy 99,268 bytes plus small descriptors. Standard WHDLoad additionally
retains 99,268 bytes in its active raw Level1 bank plus directory overhead.
Loading CPU/storage costs do not establish spare gameplay frame time.

The first ADF packaging attempt ran out of Disk1 space and was preserved under
`build/alpha11-integration/adf-attempt1-preserved`. Host-only lossless SPL1/SPD1
parsing improvements and Shrinkler preset 3 retain the established runtime
formats and exact decoded data. Every packed asset is checked by the real C
reader; final artifact/ADF results are recorded in RELEASE_VERIFICATION below.

User acceptance covers focused FS-UAE v5 visuals; exact CPU configuration was
not supplied. Minimum-68020 cadence, final HD/WHDLoad/ADF replay and real-A1200
verification remain open, as does the intermittent hardware HUD-boundary glitch.
No automatic FS-UAE launch, commit, push or itch upload is authorized/performed.
Earlier entries below retain their historical status.

## 2026-09-26 — Level1 v5 fixes building-light silhouette

User rejected v4's enlarged building glow in the supplied FS-UAE screenshot.
V5 modifies only the original five cyan pixels at rear x771/y117–121: brief
blue dimming/blinks, fixed size, no halo and no white expansion. All 48 frames
are verified unchanged from v4 outside that building patch; inside it all
non-core pixels equal the original artwork. Sky and tower animation retained.
Native compile, independent planar reconstruction and actual-C sanitizer buffer
stress pass (2,048 cases, native-observed stride). Frame blob 99,268 bytes;
Chip allocation unchanged (96,000 extra bytes observed in v3). No new cadence
claim. User visual acceptance of v5 and minimum 68020 performance remain pending.

Active test: `sparkpaw/dist/L1-Electric-v5-030-HD/Level1-Test`, same PAL 68030
configuration. Inspect building for 10 seconds, unpaused left mouse press/release
to save, wait 15 seconds after deliberate freeze. V4 including evidence archived
byte-identically under `sparkpaw/dist/older-builds/L1-Electric-v4-030-HD-20260926`.
179 release files and 79 production assets unchanged. Proofs and supplied original
screenshot: `sparkpaw/build/level1-electric-v5-20260926/`. Status/history updated;
no emulator launch, release, commit or push. Alpha.10 remains current.

## 2026-09-26 — Level1 electric v4: more sky and building pulse

User likes v3 in FS-UAE and requests more sky discharges plus a conspicuous
pulse in the cyan slit of the building marked in their screenshot. Exact CPU
configuration was not supplied. V4 doubles sky sites from 8 to 16, preserves
the tower sequence, and pulses the existing slit at rear x771/y117–121 with
a bright core and bounded blue spill. Existing eight pens and Copper palette
remain unchanged. Review frames: `assets/concept/level1-rear-ambience-study-v4/`.

Active focused HD test: `dist/L1-Electric-v4-030-HD/Level1-Test`, first PAL
68030 visual gate with 2 MB Chip / 8 MB Fast. V3 and its complete user log were
archived byte-identically to `dist/older-builds/L1-Electric-v3-030-HD-20260926/`.
No release, commit, push or emulator launch. Alpha.10 remains current.

V3 log: build_id=l1electric_v3_candidate; post_run=complete; 8,050 intervals /
8,086 PAL fields (~49.78 FPS), 37 two-field, no three-plus, one zero interval;
rear-specific unsafe=0. Broad ownership counters unavailable. Mostly late-level
samples; not a matched 68020 performance acceptance. The log reports 96,000
extra Chip bytes: actual 152-byte display stride gives 94,848 rear bytes plus
1,152 staging. This corrects the earlier 91,008-byte host estimate. No further
Chip allocation is added by v4. Fast frame file grows 58,756 -> 99,556 bytes
(+40,800), plus descriptor/cache metadata. Executable 318,644 bytes (+556 vs
v3); added loading time and CPU/Blitter duration remain unmeasured.

V4 has 18 complete planar patches. Native build passes; independently decoded
frames reconstruct all 48 source previews exactly. Actual C sanitizer harness
passes 2,048 alternating camera/phase/reset cases at both earlier host stride
and native-observed stride, allocation/file failures, active/source immutability
and guards. Stress maximum 5,184 destination bytes / 8 patches (24 plane blits;
15,552 aggregate Chip transfer bytes including CPU staging and Blitter reads/
writes), not native worst-case time. Other tested native assembly units remain
byte-identical to v3 (Stormrail, game, main, audio_mix, platform_amiga).
Renderer algorithm unchanged; log variant=v3 describes that engine while v4's
build_id identifies the new candidate. No full-suite rerun for this art/data
iteration; previous full-suite pass belongs to v3.

Staging verified 75 runtime assets, 60 literal executable references, 179 release
files and all 79 production runtime files unchanged. Proofs, original screenshot,
user log and source inventories: `build/level1-electric-v4-20260926/`.
User should inspect denser sky, original tower and marked building for 60–90s,
scroll back once, then press/release left mouse while unpaused to save this
candidate's own renderdiag.log; deliberate freeze, wait 15s before emulator stop.
V4 visual acceptance, minimum PAL A1200/AGA 68020 cadence and campaign/media
integration remain pending. Earlier sections below are historical records.

## 2026-09-26 — Approved Level1 electric v3 staged for native test

User approved the v3 preview and explicitly requested a playable test. Candidate
is staged in `sparkpaw/dist/L1-Electric-v3-030-HD/` (`Level1-Test`), unnumbered;
release remains 0.7.0-alpha.10. First gate: user-run PAL A1200/68030 with
2 MB Chip / 8 MB Fast, followed by paired baseline/candidate 68020 testing.
No emulator was launched; native visual quality, cadence and load time are
not yet accepted. Existing builds, rejected previews and local changes retained.

The optional `SPARKPAW_LEVEL1_REAR_AMBIENCE` path reproduces all 48 approved
indexed frames: existing-bolt downward pulse, crystal response, eight sky sites.
Complete rectangular bitmap frames follow the Drowned art technique, but updates
use a second guarded rear buffer tied to the inactive Copper/foreground index.
All patch DMA completes before publication; the canonical bitmap stays immutable.
Palette/Copper colours and HUD/gameplay logic are unchanged. Six simulation ticks
per phase; pause freezes the sequence. No free gameplay time is assumed from
WHDLoad loading gains.

Incremental allocation: 89,856-byte rear display plus 1,152-byte Chip staging
(91,008 Chip bytes total), 58,756 Fast bytes for the animation file plus small
metadata/allocator overhead. File is 58,756 bytes; executable grows 2,836 bytes
against the matching focused baseline. Host stress observed at most five patches,
3,456 destination bytes / 15 plane blits per prepared frame (10,368 aggregate
Chip transfer bytes including staging copy and blitter source/destination).
These are transfer counts, not measured CPU/Blitter time or proven frame margin.
Loading adds file read/allocation and second-buffer initialization; native cost
remains unmeasured. Minimum stays PAL A1200/AGA, 68020, 2 MB Chip, 8 MB Fast.

Native baseline/candidate compile and full host suite pass. Actual C sanitizer
harness passes allocation/file failures and 2,048 alternating camera/phase/reset
cases; active/source immutability, guard bytes and retirement checked. Independent
planar decoding matches every approved frame. Native assembly remains identical
for game, main, audio_mix, platform_amiga and Stormrail renderer; unflagged Level1
assembly also matches pre-edit baseline. This does not establish native timing.

Build/proof records: `sparkpaw/build/level1-electric-v3-20260926/`, including
`verification.json`, `rear-host-proof.log`, `host-suite.log`, baseline/candidate
and assembly. Staging verified 75 declared assets / 60 discovered references.
Fifteen Drowned assets absent from the production source directory were copied
from current alpha.10 into the candidate supplemental build directory only;
provenance is in `supplemental-release-assets.json`. All 179 release files and
79 production runtime files match their initial hashes. No release/commit/push.

Short playtest: watch opening 10 seconds; play/scroll right and back for 60–90
seconds, inspect bolt/crystal/sky alignment, HUD, audio and transitions. While
unpaused press and release left mouse to save `renderdiag.log`; the focused test
freezes deliberately. Wait 15 seconds before stopping/resetting the emulator.
Full controls and limitations are in the drawer's `ReadMe.txt`.

## 2026-09-26 — v2 rejected as invisible; v3 electrical pulse study

User explicitly wants a visible glow and a top-to-bottom pulse through the
existing lightning, triggering the crystal on each arrival, plus local
electrical pulses/discharges throughout the level sky. This supersedes v2's
nine-pixel dimming and static-cloud scope. Both rejected studies are preserved.
V3 at `assets/concept/level1-rear-ambience-study-v3/` follows the existing bolt
with a travelling bright packet, crystal response and eight sky-discharge
sites. Opening, tower-detail, full-panorama and four-camera GIFs are provided.
All frames stay within the existing eight rear pens; no runtime/asset/dist
changes or native acceptance. The visual sequence is 48 x 120ms; tower pulse
repeats every 2.88s. Its bounded 48x64 tower patch is 1,152 bytes/state; v2's
smaller budget no longer describes this proposal. Beam timing and scheduling
of simultaneous tower/sky changes remain open before runtime integration.

## 2026-09-26 — v1 rejected; quieter v2 visual study

Owner rejected v1 as unattractive and insufficiently subtle/fitted to existing
art. Preserve it and its generator snapshot. V2 at
`assets/concept/level1-rear-ambience-study-v2/` keeps the bolt contour unchanged:
at most nine authored existing light-core pixels briefly dim one palette step,
then restore. One existing crystal-facet pixel responds; no expanded cyan halo.
The original image occupies 6.2 of 7.08 seconds. Clouds are static for this
focused revision; full-level sky ambience remains pending, not cancelled.
Four unique states fit a combined 16x50x3 rectangle: 300 bytes/state and
1,200 raw bytes total. Native beam timing remains unproved. No runtime build,
FS-UAE, release, commit or push. V2 awaits visual review.

The original brief and earlier findings below are historical context; v1 is
not an approved baseline.

Status: requested feasibility investigation only. No animation selected,
implemented or approved; no new roadmap checkpoint or release authorized.

## Starting point

Local release: 0.7.0-alpha.10, Phase 7B.1. Main HEAD remains fcd8573;
substantial local changes and untracked research are intentional and must be
preserved. Read CURRENT_STATUS and CODEX_HANDOFF before trusting historical
status paragraphs. Public itch was alpha.8 when checked on 25 September 2026.
Dist holds alpha.10 only, plus older-builds. All preceding root releases and
test drawers are preserved in older-builds/alpha10-cleanup-20260925.

Target PAL A1200/AGA, 68020, 2 MB Chip and 8 MB Fast. The owner's faster
68030/34.5 MHz with more Fast RAM is not the minimum specification.
The 57% WHDLoad result concerns CHARGING->READY in the earlier FS-UAE A/B;
it proves neither gameplay headroom nor final alpha.10 frame cadence.
Final trace-free release and HighRAM gameplay checks remain outstanding.

## Read before proposing an implementation

- DROWNED_TURBINES_LESSONS.md, especially rear-animation ownership, shared
  palette limitations, raster timing and write-only Blitter-register hazards.
- LEVEL1_TWO_COPY_RING_PLAN.md and existing Level1 evidence/host tests.
- Current Level1 renderer unit, rear asset/layout, Copper lists, scrolling,
  display publication and DMA ownership; inspect actual compiled flags too.
- Current WHDLoad bank ownership and HD/ADF packaging budgets if adding assets.

Use build-sparkpaw-visual-slice for the visual investigation, and the relevant
analysis/test-cycle skills if comparing evidence or staging a candidate.
Do not transfer Drowned's timings, palette ownership or safe write window to
Level1 without checking its own layout and scheduling.

## Research output

First establish how the existing rear is stored, scrolled and displayed, its
palette/band ownership and measured performance/memory baseline. Clearly label
unknowns. Propose a few visually meaningful, small effects grounded in the
actual Level1 art, comparing limited palette/Copper changes and small bitmap
patches where appropriate. Include the option to retain a static background
if headroom is insufficient. Do not allocate full animated backdrops by default.

For each plausible option describe visible result, changed pixels/pens,
Chip/Fast storage (resident and temporary), CPU/Blitter traffic, update rate,
scroll/occlusion seams and effect on loading/storage per edition. Distinguish
calculated costs from measured frame deadlines. Protect the existing two-copy
ring bounds, HUD boundary, foreground palette and other campaign sections.
Historical 48.58 FPS and later unequal-route A/B averages are reference evidence,
not fresh measurements of the alpha.10 baseline.

Start with an investigation plan and recommendation; do not change runtime
code before the candidate and risks are sufficiently bounded. If a playable
prototype is subsequently authorized, use one small compile-isolated,
unnumbered candidate, preserving alpha.10. Obtain an early visual review and
then matched candidate/baseline cadence evidence with minimal instrumentation.
Do not launch FS-UAE. Ask the user for a concrete, short test when native
measurements are needed. Never infer real-hardware acceptance from host tests.
No release, commit, push or stats-screen prefetch is authorized by this brief.

## 2026-09-25 — first feasibility audit and research plan

Plan: (1) pin the alpha.10 source, assets, palette and renderer ownership;
(2) compare local bitmap animation with restricted palette changes at native
scale; (3) calculate incremental memory and transfer bounds; (4) only after a
single visual candidate is chosen, make an isolated unnumbered prototype and
obtain a short user-run PAL/68020 cadence test. No runtime or asset was changed
in this audit, and FS-UAE was not started.

### Source and baseline facts

- The release flags in `Makefile` retain 4-plane PF1, 3-plane PF2, AGA32
  guarded fetch, a two-copy 1024x208x4 Level1 foreground ring and an isolated
  `renderer_level1_unit.c`. `renderer.c` prepares the inactive Copper list and
  publishes it at line zero. Level1 has no Drowned rear-ambience call after
  publication; its timing window cannot be inferred from Drowned.
- `storm-rear.spbm` is 1120x208x3, 140 bytes/row, 87,360 planar bytes and
  87,396 file bytes. `assets.c` loads it as a displayable Chip bitmap. The
  guarded display duplicates it at at least 144 bytes/row, or 89,856 planar
  bytes. Both representations must stay consistent when animated. Existing
  source plus guarded display payload is at least 177,216 Chip bytes, before
  allocation overhead. The two foreground display rings total 212,992 Chip
  payload bytes. Neither number is free-memory evidence.
- Rear scroll is `cameraX >> 2` and shares the prepared Copper list with
  foreground scroll/pointers. Rear pixels, including the tower, are one bitmap,
  not independently moving objects. The HUD palette/pointer switch begins at
  line 252, and the line-253 restore/update path must remain untouched.
- PF2 uses eight shared pens (16..23). The Level1 Copper already rewrites all
  eight rear colours at twelve raster stages, with four changes around rear
  rows 68..84 and eight around rows 139..163. Sprites occupy entries 32..47.
  Counting the source indices shows pens 2..6 throughout sky and mountains;
  none is an isolated sky-only, lightning-only or crystal-only pen. A global
  pulse or simple sky-band palette cycle would recolour silhouettes and terrain.
- The historical alpha.45 minimally observed Level1 reference was 48.58 FPS.
  Later Level1 ring A/B whole-run values were 48.73/49.02 FPS with unequal
  routes. Neither is an alpha.10 matched gameplay measurement or proof of spare
  frame time. The earlier 57.16% WHDLoad gain measured CHARGING->READY loading.
  Current free/largest Chip and Fast blocks, busy-scene deadlines, per-frame
  Blitter waits and alpha.10 cadence remain unmeasured.

### Candidate comparison (incremental calculated costs)

| Option | Visible result and scope | Resident/transfer bound | Main risk |
| --- | --- | --- | --- |
| A. Small authored sky accents, preferred first visual study | Several fixed positions across the 1120-pixel rear panorama acquire slow, unsynchronised 32x16 violet cloud-edge highlights. A separate rare 64x48 lightning path at the tower is an optional second gate, not part of the first runtime prototype. Four complete 3-plane phases per 32x16 patch cost 768 bytes Fast; eight unique sites cost 6,144 bytes Fast before metadata/compression. One 32x16 Chip stage is 192 bytes. Updating source and guarded display costs 192+192 destination bytes and up to six plane blits per site, or one carefully owned CPU source write plus three display blits. At most one site per publication and no more than about 6–8 changes/second globally is the proposed bound. | Newly exposed columns, phase staleness on fast/reverse scrolling, and CPU/Chip contention after publication. Complete authored rectangles must preserve all unchanged background pixels; no transparency shortcut. |
| B. Tower crystal micro-pulse | One 16x16 area subtly brightens across four phases, preferably with a spatial halo confined to the tower. Four complete 3-plane frames: 384 bytes Fast; one 96-byte Chip stage; 192 destination bytes to keep both rear copies in sync, with up to six plane blits per update. Slow irregular 3–5 changes/second. | At game scale it may be too small; front objects can occlude it. The art needs exact rear-world placement and should never illuminate the HUD or foreground pens. |
| C. Restricted palette pulse | A tiny change to one or two PF2 colour values in a narrow raster span could suggest distant weather with only a small phase table and Copper-list word updates, no bitmap stage or blits. Two lists need independent phase state; any added WAIT/MOVE words consume Chip list capacity and bus slots. Exact list growth requires a compiled list-count check. | Shared pens also tint mountains/tower in that span, existing 12-stage gradients can expose steps, and AGA high/low nibbles plus HUD restoration must be handled. Use only after an indexed visual proof; not the first choice. |
| D. Static rear | Zero added load, memory, Blitter and frame work. | Less ambience, but the correct fallback if native cadence or visual review rejects the effect. |

For scale, a 64x48 lightning rectangle needs 1,152 bytes per 3-plane phase;
four phases need 4,608 bytes Fast and a 1,152-byte Chip stage. A full alternate
1120x208x3 panorama would cost another 87,360 bytes per frame and is outside
this proposal. These sizes are uncompressed planar payloads, not measured
allocation high-water marks or on-disk package deltas. If implemented, HD and
the standard/HighRAM WHDLoad banks must carry the new assets, and the three-ADF
Level1 volume needs an independent capacity/readback check. No storage or load
time increase is claimed until the exact encoded asset and package are built.

Recommendation: make a native-size, non-runtime study of A first, using accents
already implied by the storm clouds in the supplied screenshot. Keep the tower
lightning and crystal as later independent additions if that study reads well.
Avoid a synchronized whole-sky flash and independent cloud line scrolling; the
latter would shear the shared mountain/sky bitmap. Before any runtime patch,
pin exact world rectangles and indices, prove source/guarded-display update
ordering, Copper capacity, scroll-edge coverage and allocation cleanup, and
measure alpha.10 versus one isolated candidate in the same busy Level1 route.
The user must run any FS-UAE test; host arithmetic cannot establish 20 ms
deadlines, PAL 68020 smoothness, or real-A1200 acceptance.

### What carries over from Drowned, and what needs a new proof

Drowned's accepted water used short, staggered bitmap glints and a restrained
shore palette change; its waterfalls used four authored rectangles with moving
streaks and foam. The runtime copies a complete authored frame rectangle into
both the canonical rear bitmap and its guarded display copy. For Level1, use
that same visual composition: small, staggered cloud-edge patches and authored
frames placed over the existing lightning/tower image, with still frames
between events. The transferable rule is spatial selectivity, slow phase
offsets, exact indexed frames and one bounded update at a time.

The Drowned implementation itself is not transferable unchanged. Level1's
rear width, image content, shared-pen distribution, source and guarded-display
strides, Copper list capacity, scroll exposures and post-publication work differ.
Drowned's line-64 deferral and earliest animated row 112 were measured/design
choices for its water region; Level1's cloud effects are much higher in the
image and need their own beam/DMA-safe schedule. This makes sky animation a
timing question even when its byte count is small. Any native prototype must
first prove no live rear write, no stale guarded copy and no extra HUD-boundary
or gameplay deadline miss. No prototype is authorized by this plan alone.

### Agreed visual direction — 2026-09-25

The owner likes the combination of subtle sky highlights, tower lightning and
the glowing tower crystal. Keep these as one coherent weather language with
three independently gated elements, rather than firing all three together:

1. Slow, sparse cloud-edge highlights at separated rear-world positions across
   the full level. Avoid a regular sweep, simultaneous flashes or an always
   flickering sky. This is the first native-size visual study.
2. Animate the lightning already painted above and into the tower by placing
   authored rectangular animation patches over that part of the existing rear
   bitmap, as Drowned does for its waterfalls. Each patch frame includes the
   original sky/tower pixels plus changing lightning pixels; the idle frame
   reproduces the present image exactly. A short sequence makes the existing
   bolt flare and discharge, then returns to the original image. This is not a
   new bolt or a whole-sky flash. Map the exact rectangles and indexed pixels
   before finalizing memory and DMA costs.
3. A small crystal afterglow that briefly responds to that discharge, plus
   an occasional much weaker independent breath. Keep tower silhouette and
   foreground objects legible. Its proposed 16x16 bound is provisional until
   the actual rear-art footprint is mapped.

The aesthetic direction is agreed, but no frames, timing curve, placement,
runtime ownership design or performance gate is approved yet. Next deliverable
is a static/native-size storyboard and exact indexed-pixel/rectangle inventory
for review; still no gameplay build or FS-UAE launch.

### 2026-09-25 — native-indexed visual study v1

After the owner's go-ahead, a reproducible offline visual study was generated
at `assets/concept/level1-rear-ambience-study-v1/` from the current SPBM
indices. It shows the original idle image, sparse cloud accents across eight
rear-world sites, a short brightening over the **existing** tower lightning,
and a small crystal response. The idle and rest frames preserve the original
rear pixels. The six-panel storyboard and GIF are review artifacts; their
durations are not a runtime cadence proposal. No production asset or runtime
source changed, and no FS-UAE session was started.

Visual inspection found the cyan crystal is still indexed in the Level1 rear
bitmap; the Copper's existing mid-height palette supplies its cyan appearance.
Thus the tower sequence can be authored as one full replacement rectangle,
which avoids overlapping lightning/crystal patches fighting over the same
pixels. A provisional word-aligned (192,18,48,70) rectangle costs 1,260 planar
bytes per phase, or 5,040 bytes Fast for four complete phases; conservative
Chip staging is 1,260 bytes and two rear destinations total 2,520 bytes per
update plus six plane blits/waits. Eight 64x28-or-smaller cloud sites with two
complete states are bounded above by 10,752 raw Fast bytes. These are planning
bounds, not package sizes or measured frame costs.

The newly identified hard risk is vertical position: the lightning patch
starts at rear row 18. Drowned's accepted water/fall update starts much lower,
so its post-publication line-64 deferral cannot be copied to this effect.
Before any runtime candidate, prove a Level1-specific beam-safe update window
or choose a different buffer/scheduling design. This study alone is not enough
to claim gameplay feasibility on the minimum PAL A1200.
