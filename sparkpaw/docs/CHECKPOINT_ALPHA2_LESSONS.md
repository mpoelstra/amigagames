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
