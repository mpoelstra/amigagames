# WHDLoad loading investigation — 2026-09-23

## Current conclusion — 25 September 2026

This document retains the dated investigation history below. Current local
release is **0.7.0-alpha.10**; CONTROL and the retained loading optimizations
are included. Standard WHDLoad uses raw common/intro/current-level banks,
no PRELOAD/file cache, and NOCACHE; separate HighRAM uses raw files/PRELOAD
and requires >=16 MB Fast. Both reserve 5 MB game + 512 KB Kickstart.
The measured 57.16% reduction is CHARGING->READY20.68 ->8.86s in the prior
same-code FS-UAE020 A/B, not an alpha.8 or real-hardware speed comparison.
Alpha.8 failed ExpMem allocation on the 8 MB setup. First-image startup
investigation is deferred; final trace-free release/HighRAM playtests remain open.

Dist cleanup is complete: alpha.10 artifacts/drawers remain, previous root
items are intact in `dist/older-builds/alpha10-cleanup-20260925`, including
WHD-NoCache-B-8M, originalalpha8, alpha9 and Controls-Mode-HD.
See [current status](CURRENT_STATUS.md) and
[release verification](RELEASE_VERIFICATION_0.7.0-alpha.10.md).
Older statements about currentalpha9, pending A/B tests, active diagnostic
paths or waiting to archive describe their historical date, not next actions.


## State and evidence

Local release remains 0.7.0-alpha.9, commit 8769381. HEAD/main is fcd8573
(CONTROL options, unreleased); origin is https://github.com/mpoelstra/amigagames.git.
Initial tracked tree clean; numerous untracked concept/audio/research assets
are preserved. No emulator launch, runtime code change, build, release, commit
or push in this investigation. No dist drawer moved or removed.

MrDig reports real A1200/68030 approximately 34.5 MHz: alpha.8 intro starts
promptly, transitions are nearly instant and music ends with intro; short
LOADING/CHARGING and smooth level loads. Alpha.9 starts with extra black,
approximately one second black between plates, music ends before intro,
and longer title/LOADING/CHARGING and level loads. Music CONTINUES during the
black gaps (explicit follow-up). RAM, WHDLoad version and identity of the
hardware-installed alpha.9 package are not yet confirmed. This report supersedes
any impression that the FS-UAE acceptance proved hardware load performance.
No numerical full phase timings have been supplied. Menu return is unmeasured.

## Plan and verified package comparison

First compare stored packages, then trace startup/level/menu separately, then
ask for a bounded same-machine comparison before selecting a code change.

| Package | Game file bytes | Asset files | Stored asset bytes | ExpMem |
| --- | ---: | ---: | ---: | ---: |
| Archived alpha.8 ZIP | 305828 | 59 raw | 3558686 | 0x800000 |
| Campaign-WHD-Cache-020 | 131072 | 74 | 1778401 | 0x380000 |
| Campaign-WHD-Soundtest-SFX | 131836 | 74 | 1778401 | 0x380000 |
| Current alpha.9 ZIP | 131832 | 74 | 1778401 | 0x380000 |

All have PRELOAD in their icon. Alpha.9 has 49 SPL1, 17 SPR1, 6 SPD1
and two raw assets, decoding to 5040578 bytes. All 74 stored assets match BOTH
accepted packed drawers byte-for-byte. Alpha.9 versus Soundtest differs only
in game executable, slave and ReadMe; prior release audit attributes slave
change to version text. Current extracted alpha.9 matches ZIP game, slave and
assets; icon differs and contains WHDLoad-added PreloadSize=2062233 plus a
filesystem sidecar. PreloadSize is not proof of complete cache coverage.
ZIP CRC passes. These are package measurements, not elapsed-time measurements.

The corrected alpha.9 has the accepted 3.5 MiB reservation, not the rejected
5.5 MiB slave. Alpha.8's 8 MiB reservation is independently read from its
archived slave; do not assume its hardware configuration was the 8 MiB Fast
emulator target. Do not conflate alpha.8 with the packed baseline.

## Source findings and causal boundaries

- Startup: tools/make_campaign_release.py uses Shrinkler -1 -p for the
  alpha.9 executable. Its startup depacker precedes main; then intro image
  and music loading precede the visible story. This adds possible CPU delay
  independently of WHDLoad's initial PRELOAD/splash. No native timings yet.
- Intro: src/title.c:titleShowInternal fades out, stops raster/Copper/sprite
  DMA, frees the old image, then loads the next image with audio/VBlank alive.
  src/assets.c:packedRead produces every byte through LZ/RLE/delta logic and
  packedCRC32Byte (two nibble-table updates per byte). intro1 is stored as
  45483 bytes but expands to the same 146124-byte bitmap as alpha.8.
  Extra decoding thus lengthens black between plates while music continues.
  This closely matches the report; its exact contribution remains unmeasured.
- LOADING: main.c:loadLevelFiles loads gameplay graphics, collision and audio.
  Packed graphics/music/audio add decode work here, even with complete PRELOAD.
- CHARGING: rendererPrepareGameplay builds runtime renderer data; its nominal
  minimum is 100 PAL fields (2 s), measured from chargingStartFrame, not an
  unconditional extra 2 s. titleShowLevelReady subsequently loads READY data
  while the charging display is still present, so visible CHARGING includes
  more than renderer preparation. Do not attribute its entire regression to
  the asset decoder without phase timing.
- Menu return: APP_RETURN_READY cleans renderer/audio, loads title, reloads
  Level 1 and rebuilds renderer/READY. Escape therefore repeats real work;
  it is not a resident menu-only swap. Results return also has a 225-field
  title hold. Resident replay and cross-section Continue are distinct routes.
- Slave uses CACHECHIP. Local kick31.s explicitly enables instruction cache
  (without data cache); no evidence for a newly disabled instruction cache.
  Do not change cache/DMA policy speculatively.

Main hypothesis: packed runtime CPU work is a real-hardware regression that
fast FS-UAE acceptance did not expose. Partial PRELOAD/OS switching remains a
secondary alternative until exact hardware package/settings are checked.
CONTROL is unreleased and cannot explain the released alpha.9 package.

## Existing measurements and documentation

WHDLOAD_PRELOAD_PACKED_EXPERIMENT.md records qualitative fast/stable FS-UAE
acceptance, not real-machine load seconds. Archived Diag4 memory checkpoints
establish reaching renderer_ready; they do not timestamp file reads. Its
reported five-minute load and short black-flash recording are a rejected raw
candidate, not a timing baseline for current alpha.9.
STORAGE_MEMORY_LOADING_AUDIT.md already warns that smaller files do not prove
faster loads and separates decoding, setup and intentional holds. Older Stage2
loading_frames data is a different engine/build/workload, not an alpha.8/a9
comparison. Existing intro diagnostic writes/flushes files between phases,
which can itself trigger WHDLoad OS switches; do not enable it unchanged for
performance measurement. Current production drawers have no timing logger.

Official docs checked: https://www.whdload.de/docs/en/opt.html (PRELOAD,
PreloadSize, NoFileCache, NoCache, ReadDelay) and
https://www.whdload.de/docs/en/howto.html (execution/OS switching).
PRELOAD caches stored files as memory permits; game-specific SPL1/SPR1/SPD1
still need application decoding. PreloadSize is progress metadata, not a RAM
allocation or completeness guarantee.

## Next focused user test (existing builds; no new candidate yet)

On the same real A1200, compare the corrected current alpha.9 with the archived
accepted packed drawer dist/older-builds/campaign-approved-prior/
Campaign-WHD-Soundtest-SFX, both launched via their Sparkpaw Workbench icon.
Use the same boot/background conditions and leave intro unskipped. Record:
(1) WHDLoad splash disappearance to first intro image, (2) one black plate gap
and whether music continues, (3) LOADING to CHARGING, (4) CHARGING to READY.
Start default Storm Ruins, press Escape after approximately five seconds and
record (5) Escape to READY. Exit with F10. Seconds estimated with a clock are
sufficient for the first gate; no complete campaign replay is needed.

If both packed versions are similarly slow, that isolates the change shared
by packed packaging from alpha.9-specific controls. If they differ, verify
hardware-installed slave/icon/executable identities before editing the loader.
A later one-variable candidate can remove startup executable compression OR
change selected asset packing, after a PRELOAD memory budget check. Do not
remove all compression blindly: campaign raw assets alone exceed 5 MB.
If attribution remains ambiguous, collect phase/read/decode counters in memory
and flush only once at an explicitly documented end, not per file. Native
seconds and successful speed restoration require user results.

## Preservation

All 26885 current dist files hashed in
build/whdload-loading-investigation-20260923/dist-before.json; package asset
sizes saved beside it. Existing builds, user evidence and untracked work remain
in place. No test is newly completed, so no archiving is needed in this pass.

User considered unpacking everything before intro, then explicitly noted that
it would lengthen the initial invisible wait. Do not implement eager full-game
unpacking. Selective raw intro storage is a possible later single-variable
experiment, subject to preload budget and hardware verification. Five intro
images total 229486 stored / 772860 raw bytes: 543374 additional cached bytes if raw.
Post-investigation SHA-256 check: all 26885 existing dist files unchanged.


## Follow-up: 8 MiB candidate, user gate pending

User prefers a concrete potential fix over the extra real-hardware baseline
comparison. User confirmed his own machine has abundant Fast RAM but insists
on the product target of 8 MiB. A dynamic high-memory profile was discussed,
then explicitly deferred: focus ONLY on the 8 MiB version first.
The old hardware-comparison request above is superseded by the new FS-UAE gate.

Staged `dist/WHD-FastLoad-8M`, launched through its Sparkpaw WHDLoad icon.
Source baseline includes fcd8573 CONTROL options; it is not an alpha.9 release.
Hybrid compile flag is SPARKPAW_WHD_HYBRID, within the existing packed WHDLoad
build. The raw fallback uses the established 512-byte DOS buffer and validates
consumption/length; like the ordinary raw path it has no per-byte runtime CRC.
Packed streams retain all decoder/CRC checks. No eager whole-game unpacking,
extra decoded cache, renderer change or display-hold change. Normal packed,
HD and ADF builds do not compile the fallback.

35 raw assets (including the two already-raw collision files), 27 SPL1,
7 SPR1 and 5 SPD1. Raw assets cover all five intro plates, title/loading/
charging/READY/menu, shared HUD/SFX and selected Level-1 graphics/audio.
The 352828-byte storm-front and 143420-byte strider stay packed because their
raw cost would threaten cache headroom. Presentation music and most section
2/3 data remain packed. Shared raw assets can also benefit later sections,
but no speed claim is made for any route without play results.

The executable is uncrunched: 617748 bytes, avoiding Shrinkler startup work.
Assets total 2702787 bytes; total preload payload is 3320535 bytes. The exact
corrected alpha.9 slave is retained, with 0x380000 ExpMem. Arithmetic remainder
of 8 MiB after that reservation and payload is 1398057 bytes, BEFORE WHDLoad,
Workbench backups, allocation overhead and fragmentation. The stage tool caps
payload at 3.25 MiB; this is a candidate budget, NOT a full-PRELOAD guarantee.
The required user gate is PAL FS-UAE/68030, 2 MiB Chip/8 MiB Fast, no JIT and
the same ordinary Workbench environment. Flicker or repeated black loads rejects
this budget. Do not raise emulator RAM to make this gate pass.

`tools/build_campaign_drowned.py --whdload-hybrid --output-dir <new-dir>`
requires a fresh isolated build directory and never overwrites prior builds.
`tools/stage_whdload_fastload.py` consumes the authoritative runtime manifest,
uses alpha.9 packed bytes for retained compression, verifies decoded source
parity (only the current CONTROL menu differs from alpha.9), tests every
applicable asset through the actual C reader, and hash-checks the release set.
It refuses to overwrite the candidate. No separate runtime file list is invented.

Native build passes. Dedicated ASan/UBSan host test covers 160 split-read,
raw truncation and packed CRC/truncation cases in both hybrid and legacy modes.
All 74 staged references and file identities match; all 26885 pre-existing dist
files remain byte-identical. Build manifests and preservation checks are in
`build/whdload-fastload-hybrid-20260923`; reader tests in
`build/whdload-fastload-reader-20260923`. The preliminary raw-only native build
is preserved in `build/whdload-fastload-raw-20260923` but is NOT staged, delivered
or claimed as an 8 MiB candidate. No dynamic launcher was implemented.

The existing Controls-Mode-HD drawer is preserved; it is a separate prior
control test and has not been silently treated as superseded hardware evidence.
No existing drawer has been archived/deleted in this follow-up. No emulator
launch, new release, commit or push. FS-UAE and real-hardware results pending.

Test: unskipped intro -> TITLE -> LOADING -> CHARGING -> READY; start Level 1,
play 10 s, Escape to READY, repeat once. If stable, direct-start Stormrail and
Drowned and Escape back; F10 quits. Report flicker, black gaps, intro/music sync,
and approximate phase durations. No diagnostic log and no full campaign replay
required for this first gate. Exact instructions are in the candidate ReadMe.

## Deferred by user: prepare next section during results

User proposed unpacking Level 2 during Level-1 stats and Level 3 during Level-2
stats. This is a useful latency-hiding direction, not implemented in this first
candidate. Source inspection shows `platformFinishTakeover(titleCopperList())`
precedes `titleRunLevelCompleteMenu` / `titleRunLevelCompleteWithBonusMenu`;
Exec interrupts are disabled. Current gameplay renderer/audio allocations stay
resident through the decision for REPLAY. `rendererCleanup`/`audioUnload` and
next-section `loadLevelFiles` occur after CONTINUE, once loading ownership is
restored. Existing comments document a rejected visible-score renderer reload.
Calling the blocking DOS-backed reader inside the tally is therefore unsafe,
even if the underlying files are likely cached by WHDLoad.

A later bounded implementation must reserve and stage selected compressed
input while DOS is live, then decode incrementally from RAM with an explicit
per-frame budget while tally/audio/input continue. It must use separate output
storage, retain the current renderer for REPLAY, cancel/free on replay/back,
and transfer ownership only on CONTINUE. Early Fire/skip must work when the
prefetch is unfinished; ordinary loading must finish the remaining work.
Start with one measured high-value next-section asset and free-memory gate,
not all of Level 2 or 3, and do not mutate active renderer/Copper/Chip targets.
Measure additional Chip/Fast peak and timing on the 8 MiB configuration before
expanding it. The user subsequently explicitly deferred this work because of the risk.
It is outside the current task; do not implement it as an automatic next step.
No hidden-loading or transition-speed win is claimed.

Latest user decision: no preloading/unpacking during stats for now. Preserve
results/REPLAY exactly; finish only the 8 MiB startup/loading candidate.

Final host gate: full `make test PYTHON=../.venv/bin/python3` passes.
Native build, sanitized reader and staged file checks pass. This remains
a pending user-play candidate; no emulator or hardware runtime claim.


## User rejects WHD-FastLoad-8M; conservative compressed successor

User reports TITLE/LOADING flicker after intro and when loading levels. This
rejects the first candidate in the requested FS-UAE gate; no new numerical
timing or exact restated configuration was supplied. The larger preload set
added 1410302 bytes over alpha.9. Host cache exhaustion/OS switches are strongly
suspected from the unchanged slave plus payload growth, but are not directly
measured by this observation. The static remaining-memory gate was too
optimistic and must not be reused as evidence of full caching.

FS-UAE stopped was explicitly confirmed. The full played drawer, including
all 79 files, moved hash-identically to
`dist/older-builds/WHD-FastLoad-8M-rejected`; archive manifest is preserved.
No contents, evidence or prior builds were deleted. Hybrid source remains
compile-guarded solely to reproduce rejected work; do not promote it.

Replacement experiment: SPARKPAW_WHD_FAST_DECODE, selected with
`build_campaign_drowned.py --whdload-fast-decode --output-dir <new-dir>`.
It dispatches LZ/RLE run state once per token/chunk rather than once per output
byte, preserves split-read state and overlap/window wrap, and uses one
256-entry IEEE CRC32 lookup per byte instead of two nibble lookups. The table
adds 960 bytes per linked assets module versus the original table (two
namespaced engines); it is not a second asset cache. Legacy builds retain
original code via compile guards. File representations and decoded pixels are
unchanged except the already-current CONTROL menu. Shrinkler startup remains.
No change to memory reservation, CPU-cache flags, display ownership, renderer,
waits or result/REPLAY lifetime. Stats prefetch and dynamic RAM selection remain
explicitly out of scope.

Native fast-decoder build passes. The initial token-only native intermediate
is preserved at `build/whdload-fastdecode-20260923`, not staged; final table-plus-
run candidate is `build/whdload-fastdecode-crc-20260923`. Sanitized C-source tests
in `build/whdload-fastdecode-tests-20260923` pass 240 cases across hybrid,
legacy and fast readers: raw split reads/truncation, all three codecs, window
wrap/overlap, packed CRC/truncation, invalid backreference, wrong declared
output sizes and trailing input. The staging tool also checks each actual
candidate asset against canonical decoded bytes with the native reader source.
This proves decoding parity, NOT 68030 speed or flicker-free operation.

Current replacement is `sparkpaw/dist/WHD-FastDecode-8M` (Sparkpaw icon).
Verified payload is 1910389 bytes: only 156 bytes above alpha.9, leaving
2808203 bytes before host overhead after the unchanged 3.5-MiB reservation.
Slave and icon are byte-identical to alpha.9; 73/74 asset files are identical,
with only the current CONTROL menu repacked. Native build, Shrinkler verification,
240 sanitized reader cases and all asset-source comparisons pass. All 26885
original dist files remain byte-identical. Test intro to READY, Level 1 for
10 seconds, Escape back: flicker-free FS-UAE acceptance first, speed second.
The startup Shrinkler wait is unchanged. No runtime acceptance yet.

Final stored executable: 133220 bytes. Assets: 1777169 bytes. Only
level-ready-menu.spr1 differs from alpha.9 (unreleased CONTROL menu);
all other compressed assets remain byte-identical. ReadMe gives exact manual
FS-UAE gate; no logger or self-run emulator. The full host suite passed for the
prior iteration; this iteration ran the focused reader/native/package gates,
not another full suite. All original dist hashes were rechecked.


## 2026-09-23 — 68020 startup requirement; two future WHDLoad packages

MrDig now tests in FS-UAE/68020 and reports that intro still starts far too
late and black gaps remain between plates. This rejects FastDecode as meeting
the startup goal; it does not establish a new flicker verdict or a measured
speed delta versus alpha.9. No new machine RAM details were restated.
The next 8-MiB design MUST use uncompressed executable, intro images/music,
title/music, loading/charging/READY and all Level-1 assets. Faster decoding may
remain for compressed later-section assets (explicit user clarification).
Longer Level-2/3 loading is acceptable on 8 MiB; stats prefetch remains excluded.
Use 68020 as the first timing gate per the user's new test choice; a fast 030
emulator result is no longer sufficient to accept intro timing.

For a future authorized release, provide TWO SEPARATE WHDLoad packages:
8-MiB startup-priority and a fully uncompressed higher-Fast-RAM package with
its own verified requirement. No automatic RAM-selecting combined package.
This is a release-design requirement, NOT authorization to release now.

Further user observation: disk activity is visible only at Sparkpaw startup,
not during its later waits (platform not restated). This supports CPU decode
as a hypothesis; absence of a visible LED is not a cache-miss measurement.
Do not equate black/flickering output with proven physical disk access.
WHDLoad PRELOAD caches stored bytes; it does not decode Sparkpaw's formats.

Budget audit: the 40 boot/Level-1 files alone total 2397907 raw bytes; with the
617748-byte hybrid executable reference that is 3015655 bytes before filesystem
metadata. With current 0x380000 ExpMem, only 1702937 of 8 MiB remain BEFORE host
overhead. Excluding later files alone is therefore not a proven fix. WHDLoad
ws_DontCache supports exclusions, but forcing the current small-read loader to
uncached data risks repeated OS switches/flicker. Need a measured game-memory
budget and coalesced late-load lifecycle, not another optimistic larger cache
candidate. No new build staged in this scope clarification. Existing FastDecode
and all prior work preserved; stop state for its current user run is unknown.
Details: `sparkpaw/docs/WHDLOAD_LOADING_INVESTIGATION.md`.

Architecture implications:
- Stop treating complete campaign compression as the startup-speed answer.
- Preserve the faster decoder for later sections; its actual win is unmeasured.
- Do not merely set ws_DontCache for all later files: kickfs uses a 4096-byte
  IOCACHE and packed inputs refill in 512-byte chunks. Uncached reads may
  repeatedly switch to the host; a future late loader must coalesce reads at
  a controlled transition and avoid stats/REPLAY ownership changes.
- Existing Diag4 logs used 5 MiB game Fast, reached a 3216816-byte free snapshot
  during Drowned setup (~2026064 bytes used), and freed temporary data later.
  That is not a complete high-water measurement of current startup/READY,
  all campaign sections or all failure/replay paths. Do not simply shrink the
  game reservation to 2 MiB based on it. READY also owns a 306544-byte cache.
- Raw boot budget is saved in build/whdload-loading-investigation-20260923/
  boot-raw-budget.json. The future fully-raw package minimum is not yet proven.
- Official references: WHDLoad opt.html PRELOAD/PreloadSmart and
  https://whdload.de/docs/autodoc.html ws_DontCache and resload_LoadFileOffset.
  These describe supported mechanisms, not evidence that our game has used
  them successfully. No user test is requested for an unbuilt architecture.


## 2026-09-23 — external WHDLoad loader research

Inspected official RuffNTumble source, bundled Oscar and Battle Isle sources,
and Flashback install documentation. Direct range/full-file WHDLoad loading is
used in these examples; Battle Isle combines it with KickEmu. Sparkpaw's DOS /
kickfs / streaming-reader chain adds copying and dispatch. Correction: its
512-byte application reads are buffered by kickfs IOCACHE=4096, not one
physical read per 512 bytes. Disk is not faster than RAM; raw disk loading may
beat RAM plus slow decoding, which requires native measurement. Per-level
loading reduces residency/startup work, not media latency. No new runtime
change or test build in this research. See
`sparkpaw/docs/WHDLOAD_SLAVE_LOADING_RESEARCH.md` for source locations,
provenance, limitations and the bounded direct-WHDLoad backend direction.



## 2026-09-23 — WHDLoad level-bank candidate (manual 8-MiB gate pending)

New unnumbered candidate: `sparkpaw/dist/WHD-LevelBanks-8M`, launch Sparkpaw.
Build/proofs: `sparkpaw/build/whdload-banks-v2-20260923`.
The WHD-only reader uses explicit phase containers through large DOS Reads;
inspected kickfs dispatches these to resload_LoadFileOffset directly. There is
no injected slave callback ABI. The slave reserves 5 MiB game Fast + 512 KiB
Kickstart, disables file caching with ws_DontCache="#?", and omits PRELOAD.
Game-owned bank buffers live INSIDE the 5-MiB budget, not in an additional host
cache. Default HD/ADF/packed-release paths remain compile-isolated.

Raw executable (622476 bytes), common/menu/audio and all intro/Level-1 assets.
Three bulk reads at startup fetch common, intro and Level 1; this is raw disk
I/O before the first image, not an eager whole-campaign decompression pass.
Intro source bank is freed after the final/skip fade. Later sections are read
only after leaving results/READY, while black, before the loading image.
Only the current level bank is retained; sources for future levels do not grow
the resident set. Common Soundtest audio stays available without hidden I/O.
Existing final Chip/Fast assets still own their normal copied/decoded buffers.
No stats prefetch, replay mutation or dynamic high-memory profile.

Bank buffers: startup 2837614 bytes (2.71 MiB), post-intro maximum 2064538
(1.97 MiB). These are exact SOURCE-buffer sizes, not whole-game high-water
measurements or proof of 8-MiB fit. Faster decoder remains for later graphics.
Remaining 2.5 MiB outside ExpMem is for WHDLoad/host overhead; no throughput,
first-image delay, flicker-free behavior or hardware speedup is assumed.
Native build, all 74 decoded source comparisons, sanitized bank lifecycle /
bounds / allocation-short-read failures / 10 malformed containers pass; the
existing 240 reader integrity cases and full `make test` suite also pass.
The log is in the build directory; manual runtime acceptance remains separate.
All 26885 original dist files remain byte-identical, and the staged package
matches its verified inventory.

User confirmed FS-UAE stopped before replacing FastDecode. Its full contents
and any launcher are archived hash-identically under
`dist/older-builds/WHD-FastDecode-8M-startup-slow`;
archival and release parity manifests accompany the new build. The earlier
failed compile directory is retained too. CONTROL and all prior evidence/builds
remain intact. No emulator launch, release, version bump, commit or push.
First gate: 68020/2 MiB Chip/8 MiB Fast/PAL/no JIT, unskipped intro -> READY,
10 seconds Level 1 -> Escape twice; if stable, START AT Stormrail and Drowned,
Escape back. Report first-image wait, intro gaps/music sync, visible-screen
flicker and approximate level/menu loading. No real-hardware replay yet.
Future authorized releases still require separate 8-MiB and all-raw packages.

Implementation notes:

- `src/whd_banks.c` is linked ONCE in the host. Namespaced Drowned objects
  reference its external interface, so there is no second bank heap. Asset
  and collision I/O wrappers never silently fall back to disk. Music/SFX
  already use assetsLoadDiskData in WHD_PACKED mode and inherit the backend.
- SPB1 has a validated 16-byte header, sorted 40-byte directory entries,
  contiguous bounds-checked payloads and a 2-MiB maximum per bank. Four virtual
  handles retain DOS Seek's previous-position result. A bank cannot be changed
  while any handle is open. Raw assets keep raw-path integrity behavior;
  compressed later data retain format/length/CRC checks.
- Common: 1249390 bytes /43 assets; intro: 773076 /5; Level 1: 815148 /8;
  Stormrail: 236418 /7; Drowned: 440159 /11. No asset is omitted or duplicated
  between banks. Common includes all preview tracks/effects because Soundtest
  can request these while Level 1 is prepared; it is not a graphics cache for
  future levels. Larger future common audio still needs a budget review.
- Startup fetches exactly these three source banks before the first image;
  this is a deliberate current compromise to avoid any disk/OS switch while
  intro/title/loading are visible. It can still produce an initial disk wait.
  This is NOT a guarantee of instant intro or a claim that disk beats RAM.
- A same-section menu return reuses the current bank. Returning from later
  sections fetches Level 1 while black, before publishing TITLE/LOADING.
  Section 2/3 fetches happen after renderer/audio teardown; stats and resident
  replay remain untouched. Compressed later assets decode while LOADING is
  visible; optional later-section lazy assets remain in that section bank.
- Large fresh-handle DOS Read calls avoid seeding the 4096-byte kickfs cache
  and use its direct range-read branch. This still uses DOS open/size/close;
  it is NOT a completely DOS-free resload ABI or proof of one physical device
  transaction per bank. Host tests count application reads, not native I/O.
- The private build copies kick31.s and changes only ws_DontCache's pointer;
  the SDK file is unchanged and its source hash is recorded. Slave ExpMem is
  0x580000, the normal 5-MiB game setting, with '#?' exclusion verified from
  the assembled slave. PRELOAD alone being absent would not exclude caching.
- The first native compile failed on a missing DOS BPTR declaration in the
  new header; preserved in build/whdload-banks-20260923. Corrected V2 compiles
  with only the existing unrelated warnings. No failed build was staged.
- Automated bank tests cover all staged bytes, open-handle exclusion,
  same-section no-read, explicit section replacement, intro release, cleanup,
  repeated boot, missing names, four-handle bounds, signed Seek extremes,
  allocation/short-read failure and malformed directory/header payloads.
  ASan/UBSan decoder comparison covers every asset, including the current
  CONTROL menu, against the canonical source. No FS-UAE was launched.


## 2026-09-23 — LevelBanks startup accepted by report; shared-file correction

User reports the LevelBanks candidate's intro starts/transitions correctly,
music ends in sync, TITLE appears quickly, and intro -> READY has no flicker.
Level 1 played twice and Escape returned through TITLE/CHARGING. This is
FS-UAE evidence in the ongoing 020 test context; RAM/JIT settings were not
restated, and it is not real-hardware or complete-campaign acceptance.
Both START AT Stormrail and Drowned show LOADING, then return to Workbench.
User estimates CHARGING around 10 seconds BOTH at startup and after Escape.
This is approximate observation, not logged phase timing.

Confirmed packaging defect: three cross-level dependencies were incorrectly
owned only by the Level-1 bank. Real collisionLoad still opens
storm-collision.bin for Stormrail. Drowned's assetsLoadGameplay uses
sparkpaw-sprites4.spbm and sparkpaw-extra-life.spbm. Those files were absent
from its resident banks after selecting the next section. The existing code
then returns a load failure and exits; a CPU exception is not established.
The new host dependency test executes these actual C selectors with each
native module's compile flags and the real bank backend: old package misses
exactly 1/2 files for sections 2/3, fixed package misses none (14/14/17 calls).
It stubs allocation/rendering and therefore does not prove native gameplay.
The earlier all-file parity tests missed this phase-reachability contract.

Correction staged at the SAME `sparkpaw/dist/WHD-LevelBanks-8M` / Sparkpaw icon.
Only move these three raw entries into common.spb; no duplicate assets. Native
executable, slave, icon and every individual asset remain byte-identical, as do
intro/Level-2/Level-3 banks. Total startup source buffers remain 2837614 bytes,
post-intro maximum 2064538 bytes. No renderer, timer, audio or cache-policy edit.
New proofs: `sparkpaw/build/whdload-banks-shared-20260923`;
before/fixed selector proofs: `build/whd-bank-dependencies-before2-20260923`
and `build/whd-bank-dependencies-fixed-20260923` inside sparkpaw.
All 74 decoded source checks and sanitized bank lifetime/fault/bounds tests pass.
Native/full-suite evidence belongs to the unchanged earlier executable; no
unnecessary full-suite rebuild was made for this packaging correction.

User confirmed FS-UAE stopped. Previous drawer and launcher archived intact,
hash-verified as `dist/older-builds/WHD-LevelBanks-8M-shared-miss`. Status of
that build: intro/Level 1 accepted by report; later sections rejected.
Focused retest: skip intro if desired, START AT Stormrail -> play 10 seconds
-> Escape, then Drowned -> play -> Escape. Report gameplay entry, return and
visible loading glitches. No repeated hardware transfer/full campaign needed.
No emulator launch, release, version change, commit or push.

CHARGING remains OPEN, unchanged by this package-only repair. Its visible
period contains rendererPrepareGameplay, an elapsed minimum of 100 PAL fields
(~2 seconds, not an unconditional extra 2 seconds), then READY image/menu
preparation before fading. readyUiInit reconstructs a 306544-byte embedded
RLE/delta cache every READY entry, even with raw external assets. Thus the
previous shorthand "no decompression until READY" applied only to external
assets and missed this embedded menu-cache work. Neither its time nor renderer
preparation has been measured separately. Do not attribute all 10 seconds to
that cache, disk reads, RAM pressure or a longer programmed hold. Next timing
probe should separate renderer preparation, minimum-wait remainder, READY
asset copies and UI cache reconstruction; buffer counters in RAM and export
only by an explicit safe action, never during visible loading/menu phases.



## 2026-09-24 — later levels load; phase-timing diagnostic prepared

User confirms that levels load after the shared-file correction. Approximate
FS-UAE observations: CHARGING -> READY ~10 seconds, Stormrail visible LOADING
~20 seconds, Drowned LOADING ~30–35 seconds. These are user estimates, not
instrumented measurements; no new real-hardware acceptance. Preserve the now
working corrected LevelBanks build as the functional reference.

Next diagnostic: `sparkpaw/dist/WHD-LevelTimes-8M`, Sparkpaw icon. Native build
and proof folder `sparkpaw/build/whdload-times-20260923` (work crossed midnight).
Only loading instrumentation: per-section gameplay assets, collision, audio,
renderer preparation, remaining CHARGING minimum wait, READY total and nested
READY menu-cache reconstruction. Up to192 RAM rows, no writes until explicit
LEFT MOUSE at READY. No gameplay profiler or optimization is enabled.
Guest graphics VBlank counter, 50 fields/sec with PAL: this measures guest
preparation elapsed fields, NOT physical disk/host OS-switch wall time.
ready_menu_cache is INCLUDED in ready_total and must not be added twice.
Free Chip/Fast snapshots are phase endpoints, not complete high-water marks.

Manual test: same FS-UAE020 /2 MiB Chip /8 MiB Fast /PAL /no JIT configuration;
intro may be skipped. Start Level1 briefly -> Escape; START AT Stormrail ->
brief play -> Escape; START AT Drowned -> brief play -> Escape. At final READY,
press/release LEFT MOUSE ONCE: deliberately frozen save state. Wait a few
seconds, then F10 to exit WHDLoad/flush pending writes before stopping FS-UAE.
Read `dist/WHD-LevelTimes-8M/data/load-times.log`; require complete=1 and
no overflow. No full campaign or real-hardware run requested. Log persistence
and native timing remain pending the user's test; no emulator was launched.

Native diagnostic:624800bytes. Banks/slave/icon are byte-identical to the
working candidate, so the 74-asset decoded proof carries over by exact parity.
Actual per-section selector tests pass14/14/17 calls; bank lifetime and malformed
input tests pass. Actual collector ASan/UBSan tests cover nesting, phase section
ownership, timer wrap, failed calls, saturation and explicit-only log writes.
A non-diagnostic rebuild is BYTE-IDENTICAL to the working622476-byte executable
(SHA256649a3126867025c6c19795bb98f1ca899a305d9acb3f991ccf6836b51b4d9370),
proving disabled hooks preserve this build. Its proof is in
`build/whdload-trace-off-proof-20260924`. The extracted-C Drowned-driver and Stormrail-start test harnesses now include
the real disabled trace macro header; ownership/failure assertions are retained.
All make-test checks pass across host-suite-v2.log and host-suite-tail.log after
those harness fixes; initial failed runs are also preserved. All26885 original
dist files remain byte-identical; validation.json records the checks.

User freshly confirmed FS-UAE stopped. Working LevelBanks is archived intact under
`dist/older-builds/WHD-LevelBanks-8M-working`, including all launchers/evidence
with verified hash parity; the diagnostic is now staged. No new release, commit or push; alpha.9
remains unchanged. Do not infer the bottleneck from total load times alone:
use the upcoming log to decide whether the next bounded change targets asset
decode/copy, renderer construction or the repeatedly built READY cache.



## 2026-09-24 — loading log analyzed; raw current-level experiment

Completed36-row/no-overflow LevelTimes log preserved with SHA-256 and parsed
rows in `sparkpaw/build/whdload-times-analysis-20260924`. Typical CHARGING:
renderer6.30s +READY3.42s =9.72s; menu cache2.24s is INCLUDED in READY, minimum
wait remainder0. Stormrail scoped total19.44s (assets13.88/renderer5.08),
Drowned34.40s (assets14.90/renderer18.02). Drowned's renderer phase ALSO decodes
pontoon/background data, so do not call all18.02seconds pure graphics setup.
First Level1 return is a real unexplained outlier:26.66s renderer+READY, while
all three other visits are9.72s. User denies pause/turbo/settings changes.
Guest PAL-field timing does not measure disk/host-switch wall time.

New `sparkpaw/dist/WHD-LevelRaw-8M` / Sparkpaw icon changes only Level2/3 bank
storage to raw. Timed executable, slave, icon and all startup banks remain
byte-identical. Only the current level is fetched; no whole-game preload or
stats work.5MiB game +512KiB Kickstart reservation unchanged. Estimated Drowned
free Fast at the previously measured renderer endpoint:735491bytes after
994949extra source bytes. This is NOT a peak/fragmentation or8MiB-fit proof.
Actual74asset parity, bank/collector and14/14/17dependency checks pass; runtime
acceptance pending. No new native/full-suite run needed for identical code.

User completed the instructed save/F10/stop workflow. The completed timing
map, original log and launcher were archived hash-identically at
`dist/older-builds/WHD-LevelTimes-8M-measured`; working non-diagnostic baseline
also remains archived. New proof: `build/whdload-levelraw-v2-20260924`.
Test same020/8MiB: START AT Stormrail -> brief play -> Escape, then Drowned ->
brief play -> Escape; left-click final READY, wait/freeze, F10, stop emulator.
Read `data/load-times.log` from the NEW drawer. Watch for larger black disk-read
intervals or allocation failures. CHARGING and its unexplained outlier remain
open. Full report: `sparkpaw/docs/WHDLOAD_LOADING_MEASUREMENTS.md`.
No emulator launch, release, version bump, commit or push.
