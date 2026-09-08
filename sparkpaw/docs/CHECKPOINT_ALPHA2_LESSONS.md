# Lessons retained at 0.7.0-alpha.2

## Performance evidence

- Reproduce the original executable byte-identically before naming compiled-code
  comparisons after a historical release. Alpha.68 provenance is established;
  48.58 FPS belonged to alpha.45. The 49.30 collision-cache log lacks its footer.
- Different manual routes cannot establish a small causal FPS gain. User A/B
  play narrowed Level-1 differences to practically none; retain approved small
  changes without a speed claim. Performance research is explicitly parked.
- Audio split, gameUpdate extraction/completion caching and linker-order changes
  were rejected. Do not repeat them without substantially new evidence. Failed
  diagnostic alpha.68 charging runs are not valid performance measurements.
- Protect complete frame chains, renderer helpers, publication order, memory
  ownership and Stormrail evidence; file length/program size is not a diagnosis.

## Disk packaging and presentation

- A complete union of files across disks does not prove either disk is usable.
  The first pair omitted storm-collision.bin from Disk 2 even though collisionLoad
  opens it unconditionally during Stormrail entry. This blocked both DF0 swaps
  and DF1 starts. Check compiled dependencies PER VOLUME, including collision,
  shared audio and logical Level-1 assets still read by Stormrail preparation.
- Logical ownership and actual load/residency differ. Deliberate common-file
  duplication fits and preserves accepted lifetimes; do not force a renderer
  refactor merely to obtain an idealized physical group split.
- Decoder host tests alone do not validate media resolution or live Copper
  lifetimes. Exercise actual decoder code, per-volume coverage, CRC/readback,
  drive selection, failed media and source-to-package identity separately.
- The user expects Disk 2 already in DF1 to be discovered without a prompt;
  otherwise use the same loading image with only a status-strip change.
  INSERT now matches the bevelled cyan art. Technical helper lines and mouse
  cancellation were unwanted; wait automatically for the right disk.
- Never free an image while the current Copper still points at it. Publish the
  replacement before retiring the old allocation; preserve that cold boundary.
- Compression is lossless storage work, not an FPS optimization. Maintain the
  512-byte DOS buffer, bounded 4 KiB history and final Chip/Fast asset placement;
  add no per-frame allocation or reads.
- File order now follows first use and avoids payload wrap-around. Central FFS
  metadata and repeated reads still cause seeks. The Gotek OLED video does not
  quantify seek timing; no seconds-saved claim without matched hardware tests.

## Releases and evidence

- User approval is medium-specific: HD and ADF acceptance cannot establish a
  newly built campaign WHDLoad's startup, F10, memory or transition behaviour.
- Audit every build target, not just renamed archives. The old WHDLoad target
  compiled only one level and its slave still reported alpha.49. It now uses
  campaign/split-renderer flags, F10 hooks, all 48 files and the new slave version.
- Keep raw user evidence, failed candidates and alpha.68 immutable. Archive
  completed drawers only after hashing, and keep one current artifact set.
- Fresh live itch downloads override stale web-search caches: live page and
  detector show alpha.68 with a matching devlog; one cached tool page showed
  alpha.62. Record the live baseline and write the complete player-facing delta.
- Runtime assets and tools must be reproducible from committed sources; do not
  depend on a temporary audit manifest for production compiler flags or hashes.


## Release-notes language and completeness

Standing user agreement: all player-facing "What's new since the latest itch
version" text is English, including title/headings/compatibility note, even in
a Dutch conversation. Keep a versioned RELEASE_NOTES_*.md as the canonical copy.
The first handoff was incorrectly Dutch and too short. Compare the whole public
baseline delta: campaign progression, boarding/flight, scenery, enemies/rewards,
debris, finale/audio/results and package-specific changes. Do not present an
existing alpha.68 feature as new (Level-1 instant replay and button assignment
already existed), or turn speculative performance work into a promised gain.

## Free Chip RAM and campaign peaks — 2026-09-05

A successful Level-1 launch is not a campaign-wide memory guarantee. The user
reports real-Amiga HD/WHDLoad success but a separate HD Stormrail crash with
about 1.45 MB Chip free at launch. Audit actual asset heights, duplicate DMA
copies and section-specific caches before sacrificing art or replay. Installed
2 MB Chip capacity differs from free/largest-block capacity. An old isolated
flight log does not prove the integrated preload/results peak. See
STORMRAIL_CHIP_RAM_AUDIT.md; allocation cause and actual savings remain unmeasured.

## Accepted optimization is not a lower memory guarantee — 2026-09-06

Chip2 HD is user-accepted on real A1200 and retained in normal development.
The user explicitly reports Stormrail still requires more than about 1.45 MB
free Chip RAM. Keep functional acceptance separate from minimum-memory claims:
source allocation savings do not establish peak availability or fragmentation.
Match the accepted ZIP's executable and all assets before promoting its status;
preserve shipped alpha.2 artifacts and avoid unnecessary rebuild/test variants.

## Music lifetime and peak memory — 2026-09-06

READY uses disabled Exec interrupts; an OS VBlank player alone would stop there.
Preserve active audio DMA at takeover and explicitly tick from the owned display
wait, then stop DMA before releasing music samples. Ending before gameplay does
not remove the preceding load/READY peak: report that additional Chip bank.
Intro duration must come from actual eleven passages/fades, not an old storyboard.

## Intro cue completion — 7 September 2026

Bound a cinematic cue by the converted stream frame count; scene I/O and manual
advances vary, so a looping player can otherwise restart the peaceful opening
at the end of a slow intro. Stop on skip/completion and release its bank before
allocating the next cue. Archive cleanup preserves contents/hashes and records
path moves, including old public releases and user drawers.

## Music on ADF — 7 September 2026

Measure actual FFS capacity including file headers, not only raw payload sums.
Neon Sky's raw bank overflows Disk 1; existing disk-only lossless packing saves
space while preserving decoded music. Route music reads through the same media
resolver as graphics/SFX so return-to-title works with Disk 2. Strip intro music
references with the story flag. Keep music assets out of the generic bitmap
Makefile generation rule to avoid conflicting recipes.

## Background-only READY animation — 7 September 2026

Almost-black background uses multiple palette entries; COLOR00 alone is not a
sufficient mask. Protect the union of all menu states plus complete foreground
silhouettes so dark interior art is not treated as background. Restore dirty
bytes per hidden target before patches/new particles. Keep the clean READY
source untouched and coalesce input changes into the existing one-frame music/
Copper cadence. Native timing remains a user-test gate.

## Avoid invisible background barriers — 7 September 2026

The user rejected broad artwork exclusion rectangles and a union of inactive
menu text because dust disappeared over visually empty background. Use current
state masks and connected-background silhouettes instead. Full-width motion can
still be visually truncated by masking; inspect both before adjusting lifetime.
Read-only mask growth saves Chip buffers but still costs executable/disk space.

## READY particle silhouettes — 7 September 2026

An extra pixel beneath a short streak reads as a tiny object/ship at native
resolution. Keep wind streaks single-row and vary brightness with the existing
palette; use sparse warm pulses for accents without new Chip assets.

READY dust feedback: intensity can mean density rather than brightness. Very
short, rare accents can also be lost to foreground occlusion; tune duration
and staggered frequency as well as palette contrast.

## READY input costs share the music frame budget — 7 September 2026

A frame-driven music tick exposes foreground menu copy stalls as audio slowdown.
Audit input-triggered work separately from idle animation: 624 tiny CopyMem calls
and 14976 Chip bytes per buffer update can matter on 020 despite 030 acceptance.
Precompute differences outside the owned loop and verify every state transition
against full-patch output. Byte reduction is not a native timing measurement.

Distinguish selection changes from page changes: sparse dirty spans reduced
START/OPTIONS highlight writes by 91%, but full OPTIONS page transitions still
cover every row. Trial the already-owned Blitter for those rectangles while
keeping the accepted sparse CPU path. Wait before CPU dust writes resumes.

## Fast2 DMA ownership regression — 7 September 2026

The preceding proposed Blitter shortcut was invalid: menu patch planes live in
Fast RAM (dmaSource=FALSE), not Chip RAM. Owning/enabling the Blitter and using a
hidden Chip destination does not make the source DMA-accessible. Check both
ends before replacing a CPU copy with DMA; host pixel parity cannot validate
Amiga address visibility. Fast2 rejected, known working CPU baseline restored.

User preference: a proven byte-identical restoration does not require another
manual retest. Reserve the next user test for an actual new candidate.

Precomputation that saves menu frame time can create a black loading pause if
placed after fade-out. Perform CPU preparation and hidden-buffer seeding while
the current loading image remains visible; defer displayed-buffer writes until
black. Distinguish reducing black duration from reducing total load time.

## Alpha.3 checkpoint consolidation — 7 September 2026

The music, Chip RAM, READY performance and rejected Fast2 DMA lessons above
are retained in alpha.3. See RELEASE_0_7_0_ALPHA_3.md for verification and
medium-specific acceptance. Do not infer fresh WHDLoad/hardware acceptance
from HD testing or repeat user tests after a proven byte-identical restoration.

## Direct section-start presentation — 7 September 2026

A shared direct-start helper can accidentally hide presentation behind a media
compile flag. Test HD, WHDLoad and ADF call ordering, and distinguish OPTIONS
section start from campaign CONTINUE. Visible loading belongs to all cold
section switches, not only the edition that may need a disk swap.

## Intro display retirement — 7 September 2026

A black palette does not stop Chip DMA. Retire Copper/bitplane fetches and wait
for the boundary before freeing the displayed intro, including between plates.
Preserve audio DMA and avoid solving lifetime hazards by adding overlapping
Chip buffers. Latch intro skip during fades; close intro input before title.
Source lifetime checks are not native proof of an intermittent reported flash.

Alpha.4 preserves the intro DMA and direct-start lessons above. Do not claim
FS-UAE-only input mapping without evidence; no broader control rewrite shipped.

## Audio ownership versus replay fidelity — 7 September 2026

Fixed50 is the current LSP conversion choice, not a ptplayer requirement.
Separate MOD interpretation, audio scheduling and Paula voice ownership.
A timer-driven player still stalls when interrupts are masked. Blank MOD tracks
are not register isolation. Borrowing loses notes; it does not restore elapsed
sample phase. Three music voices plus one mixed SFX output can preserve melody,
but must pay CPU/headroom/buffering and changes stereo placement.

Existing SFX share period 322, making a bounded two-voice SFX mixer worth testing
without music resampling. No cost/quality result is inferred. Current diagnostic
profiler owns CIA-B Timer B: never measure a CIA player by reprogramming its DMA
timer. Source budgets and upstream benchmarks are not native acceptance. See
AUDIO_SYSTEM_PLAN.md; no implementation or release change in this research.

The user explicitly accepts converting MOD masters to a suitable runtime format.
Preserve tempo/fidelity rather than mandating raw MOD playback. Conversion alone
does not create Paula voices; offline phrase combination, runtime mixing and
startup sample synthesis exchange different CPU/Chip/Fast/disk costs. Full PCM
music is especially expensive on the nearly full Disk 1. Research comparisons
must budget complete data/code and preserve current source masters.

## First isolated audio proof — 7 September 2026

Channel masking in a player may suppress LC/LEN but still write PER for an empty
track. Inspect every call/write path; the three-voice proof explicitly skips the
certified-empty fourth-track replay, retaining normal player ownership at init/
end. A dedicated SFX channel must remain protected from live master-volume calls.
Protect shared voice publication against higher-priority audio interrupts.

Inventory effect triggers as well as files: debris reuses hit/death/hurt, health
shares pickup at a different volume. Two virtual voices is a concurrency limit,
not a two-sound library. Host previews use the actual C mixer and pinned MOD
renderer but cannot establish native scheduling, panning feel or hardware sound.
Explicit standalone staging can use literal references; preserve the full-game
manifest default and hash every protected release before/after staging.

## Audio proof timing evidence — 7 September 2026

A complete, positively heard 030 audition does not establish a 020 CPU budget.
Separate VPOSR/VHPOSR reads can tear; combining a raster position with a
lower-priority software VBlank count can produce inconsistent timestamps.
Do not translate coarse late-gap counters into dropouts or raster maxima into
CPU cost without validating the clock. Preserve raw logs and subjective
listening feedback independently.

The follow-up timing candidate uses interrupt-safe ReadEClock in the OS-live
harness, covering CIA music and DMA services as well as mixing. Count nested
services once when summing exclusive totals; retain inclusive duration for
service latency. Calibrate read overhead, retain an unmeasured listening
control, and state dispatch/accounting limits. Do not carry this OS-live
clock or CIA ownership blindly into the interrupt-disabled gameplay owner.

The E-clock 020 result preserves a distinction between audible success and
affordable game-time cost: 13.32% instrumented service-body fraction despite
no heard faults. Never extrapolate the 030 run's 0.262% to stock 020, or treat
CPU model labels as equivalent emulator timing configurations. Prioritize the
measured mixer scope; prove block-based silence/tail handling before adoption.

For fixed-rate voices, divide buffers at sample endings and select silence,
copy or addition once per span. Preserve a frozen reference renderer in host
tests and compare state as well as PCM: completion counts, final pointers and
priority clearing can regress even when the preview sounds similar. Inspect
68020 compiler output, but require native timing before claiming a speedup.

Supplied 020 block-mixer evidence lowers measured service fraction from 13.32%
to 3.06%, with mixer totals about 85.2% lower and unchanged music/DMA cost.
Host exact-output parity and native timing together support the optimization;
the average alone does not prove worst-window headroom during gameplay.

The first gameplay audio gate uses a matched direct-start harness. Forbid and
Disable are distinct: permit only required custom interrupt sources while
keeping scheduling/display handlers out. Balance takeover/restore interrupt
nesting and quiesce owned CIA sources before restoring OS masks. Never share
CIA-B Timer B with the old profiler. Host lifecycle mocks validate bookkeeping,
not real interrupt scheduling; require native input/display/exit evidence.

Post-publication TOD snapshots can yield zero/two-field pairs even in a
positively perceived 030 run. Preserve zero deltas; never turn them into
>50-FPS claims or classify every two-field delta as a missed visible deadline.
Unequal manual routes require normalized counts and explicit uncertainty.

A positive 020 listening/smoothness report can coexist with anomalous timing
counters. Preserve both: gameplay audio B contains two three-field and two
seven-field TOD deltas without event timestamps. Do not explain them away as
water/respawn or measurement artifacts without evidence. Resolve sampling and
correlate events before interpreting unmatched-route cadence as audio cost.

When investigating rare long intervals, sample named phases and record reset/
water context. Keep rare-event storage separate from abundant zero/two timing
examples and report dropped records. A timestamp after a publication function
includes its bookkeeping; it is not the instant COPJMP1 was written. Coherent
raster reads in a diagnostic do not fix the production boundary reader.

020 phase trace reproduces paired seven-field gaps in original-SFX A and
music B at water reset, in renderer/Bob work. Alternating rolling-window
rebuilds explain the pair at source level. Separate these from B's ordinary
three-field boundary misses. One publication attempt does not imply no missed
window: the caller may already have waited a whole field before that attempt.

Fixed-work comparisons should index inputs by simulation step, preserve input
edge semantics and use the real collision/actor code. Validate route coverage
on host, then check native selected-state hashes and event requests before
attributing time differences. Keep warmup separate from the measured window.
A hash of selected state supports comparability but does not prove all actor/
renderer state identical. Lightweight raw clock histograms still have phase
and observer limits; they are not exact visible-deadline instruments.


Fixed-route 020 audio evidence: equal game-state hashes and event requests do
not imply equal sound admissions. Original SFX releases priority by estimated
game-update countdown; IRQ mixing releases by sample consumption. Keep request,
start and suppression counts separate, especially when frame duration varies.
Report aggregate fixed-work elapsed cost separately from CPU utilization and
raw per-publication TOD deltas; one A/B pair cannot establish repeatability.


Prioritize perceptible faults and functional adoption over indefinite audio
microbenchmarks once listening and bounded target-CPU evidence are sufficient.
A small measurable difference is not automatically a blocker. Standalone
audio proofs also hide shared ownership: the main title player already owns
audio.device, so a second player must share that reservation rather than
allocate the same four channels again. Preload must not reset Paula/filter;
stop must clear a pending two-phase CIA-B DMA handoff before replay.


Extend accepted audio to another section by selecting its score during load,
not by changing the IRQ hot path or rewriting priorities. Keep only the active
track resident and use the existing direct section selector for auditions.
A baked chord sample still consumes one Paula voice; account for sample memory
instead of presenting its harmony as additional runtime channels.

ADF capacity: examine embedded immutable tables before reducing art/music or
adding a new codec. Externalizing exact READY masks saved about33KiB gross
with the existing checked disk decoder, without changing HD or runtime output.
When a loader changes raw samples to packed data, change media markers too.
Compare compiled dependencies per disk, excluding only the opposite section's
proven-unreachable graphics/track. Keep replay data resident.
Do not run VC -notmpfile HD and ADF builds concurrently: both emit src/*.o and
can cross-link objects with different macros. Serialize native builds and check
the protected HD hash after disk-only changes; host tests can run independently.

## Alpha.5 release closure — 9 September 2026

- Deliver verified hardware packages first when requested; finish the detailed
  documentation afterward without silently changing the delivered binaries.
- Compare the live itch download filenames with the latest devlog. Cached web
  results showed alpha.62 while live downloads and the 30 August devlog confirmed
  alpha.68. Release notes must cover the entire intervening campaign delta.
- An HD audio pass does not establish WHDLoad acceptance: its takeover/restore
  and F10 paths require separate native testing even when compilation passes.
- Preserve the exact old release bytes when archiving; filesystem metadata can
  change regenerated ADF hashes even with identical verified file payloads.
- Report mixer service fractions as measured service time, not guaranteed game
  FPS. Keep hardware limits and remaining shared-effect priority contention clear.
