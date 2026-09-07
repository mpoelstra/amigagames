# Storage, Chip RAM and loading audit — 2026-09-05

Research only while the user tests Storm-Chip1-HD.zip. No further source,
runtime asset, executable or package changes. All estimates below use the
208-row candidate and are additional to its approximately 160 KiB saving.
Native seconds and allocator padding have not been measured. Do not sum
alternatives that replace the same storage.

## Ranked findings

| Candidate | Chip RAM | Storage / loading | Boundary |
| --- | ---: | --- | --- |
| Skip Stormrail's unused Level-1 graphics preparation | 69,720 requested bytes in six cache families | Avoid 187,240 bytes across four graphics files and their conversion work | Audit reset/restore/cleanup consumers before removing calls |
| Compact the mostly empty departure foreground | About 293 KiB at a conservative 512px source width, versus 3392 | About 299,520 fewer raw bytes read/cleared | Requires bounded canonical source reads; cannot simply shrink the header |
| Move original rear sources to Fast, keeping guarded DMA copies | 87,360 logical bytes per 1120x208x3 source; two in Stormrail | Same disk bytes; lower Chip peak | Must prove every active/alternative pointer path uses the guarded copy |
| Reuse already shared assets across section changes | Depends on retained lifetime; not automatically lower peak | Fewer opens, reads, HUD/player conversion jobs | More complex lifetime ownership; preload peak may increase |
| Lossless packed assets for HD | No steady Chip saving by itself | Current 2,892,290 raw asset bytes project to 1,006,837 using existing codecs | New HD loader integration and real CPU/storage timing gate required |

### 1. Unused graphics are the best next bounded step

`assetsLoadGameplay()` reads beetle (8,700), Strider (143,420), Core (34,620)
and extra-life (500) SPBM files during Stormrail: 187,240 bytes in total.
These conversion sources use Fast RAM already, so their file sizes are NOT
Chip savings. `rendererPrepareGameplay()` nevertheless builds these caches:

- Beetle: 2 facings * 9 frames * 24 rows * 3 words * 2 bytes * 5 mask/planes
  = 12,960 Chip bytes.
- Strider stages: 4 slots * 64 rows * 5 words * 2 bytes * 5 = 12,800.
- Core: 18 frames * 48 rows * 4 words * 2 bytes * 5 = 34,560.
- Extra life: 22 rows * 2 words * 2 bytes * 5 = 440.
- Water: 16 frames * 4 planes * 11 rows * 5 words * 2 bytes = 7,040.
- Splash: 4 frames * 16 rows * 3 words * 2 bytes * 5 = 1,920.

Total 69,720 requested Chip bytes (about 68 KiB), plus avoided Fast Strider
conversion and CPU work. Keep shared diamond/heart, HUD, player and Stormrail
caches. `prepareStaticCollectibles()` also allocates Level-1 restore storage;
its consumers need a separate audit, and it is excluded from the total above.

Do not select audio solely by logical asset group: Stormrail departure actually
calls audioPlayJump(), and Stormrail enemies reuse audioPlayStriderShot().
The Level-1 grouping therefore does not prove a sample unused. Broad audio
splitting remains a rejected performance experiment; only evidence-backed
load-time sample selection could be a later isolated memory change.

### 2. Mostly empty foreground is the largest local memory opportunity

The candidate stormrail-front.spbm contains 352,768 planar bytes. An exhaustive
byte scan finds its last nonzero byte at row-byte X=22 (pixels 176..183). The
rest of the 3392px width is zero. A conservative 512x208x4 source would require
53,248 logical bytes and save 299,520 bytes (292.5 KiB).

This is not a safe generator-only change: canonical span, column, Bob restore
and initial collectible preparation currently address the large world by X.
Implement explicit zero-outside-source semantics or a separately bounded
Stormrail source, and guard all CPU/Blitter reads including initial/replay
preparation. Preserve 3392px logical geometry and the three-copy display ring.
Do not release the departure image after boarding: resident replay needs it.
Its zero-filled storage is already cheap inside the ZIP, so do not promise a
similar download-size saving.

### 3. Original rear copies and prepared caches

Each 1120x208x3 original rear occupies 87,360 logical bytes, before allocator
padding. The current guarded renderer copies it to another displayable bitmap.
Stormrail has departure and flight rear pairs. Fast originals could recover
about 170.6 KiB Chip in total while leaving the same displayed buffers, but
setScroll(), Copper setup, validation, replay and alternative compile branches
reference different pointers. This requires a deliberate ownership change.
It does not reduce disk I/O and may retain redundant Fast data unnecessarily.

Prepacked hardware/Bob caches could also reduce CHARGING conversion time, but
may expand files, duplicate raw masters or add new loading peaks. Measure the
remaining preparation stages before choosing a new format. Keep the accepted
Fast player/Strider masters and small Chip DMA stages.

### 4. Measured storage opportunity, not a loading-speed claim

Offline codec runs used the existing SPR1 RLE and SPL1 LZSS packers, including
round-trip verification, and selected the smallest of raw/SPR1/SPL1 per bitmap.
Non-bitmaps remain raw. Results in build/storage-load-audit/assets.json:

- All 48 assets: 2,892,290 -> 1,006,837 bytes (65.2% less), excluding executable,
  filesystem overhead and any new HD loader code. This is an achievable codec
  payload projection, not a produced or playable package.
- Stormrail front: 352,828 -> 7,439 bytes (SPR1).
- Level-1 front: 352,828 -> 38,380 (SPR1).
- Ready-menu atlas: 179,916 -> 39,129 (SPL1).
- Player sheets: 184,380 each -> 98,016 / 98,344 (SPL1).

These codecs are already used by the current disk packaging; those figures
are not an additional 65% ADF saving. Ordinary HD currently reads raw SPBM.
Applying compression to HD could trade reduced I/O for more 68020/030 decoding
work. The historical alpha.35 packed-HD failure and accepted alpha.36 raw
rollback forbid assuming disk success establishes HD safety. Keep the proven
512-byte staging/MaxTransfer safeguard and decode into final allocations.
No zlib proxy result is a proposed native codec.

The HD download ZIP already compresses these files; installed size and ZIP
size must be reported separately. Both player sheets are large and mostly
shared; a common master plus small cockpit patch may reduce installed storage
and duplicate reads, but alters source/manifest/cache contracts. It is a later
format option, not a reason to omit either currently referenced file.

### 5. Loading order, media and deliberate presentation time

Current raw bitmap loading uses 512-byte ordinary buffers and copies to final
planes. Keep that proven hardware safety boundary. Row padding can force
separate smaller reads per row; a buffered reader retaining 512-byte input
across row boundaries could reduce DOS call count while preserving row padding
and exact output, but actual row strides and timing should be logged first.
A sequential container could reduce file opens and physical disk seeks; retain
it as a later disk-focused option, since alpha.2 already orders disk payloads.

Startup intentionally holds the title for 225 PAL fields (4.5 seconds), plus
its lock/fades; CHARGING has a minimum of 100 fields (2 seconds), including
preparation time rather than adding two seconds unconditionally. Those are
accepted presentation contracts, not accidental loading costs. Faster reads
cannot remove them. Direct Stormrail selection happens after Level-1 preload,
then switches section, so that menu path includes extra preparation by design.
Moving section choice earlier is a separate UX/lifetime change.

For future native measurement separate file reads, renderer preparation and
intentional holds; measure cold launch, Continue, direct Stormrail and resident
replay separately. Avoid claiming seconds saved from compressed byte counts.

## Recommended order

Await Chip1 hardware result. Next audit/implement the six unused graphics-cache
families and their four source loads as one focused candidate. Then evaluate
bounded departure-source storage for the largest Chip/raw-I/O win. Rear-source
placement follows if still needed. Only after phase timings justify it, test
packed HD or preconverted caches; keep physical ADF/Gotek timing separate.
Protect all visible pixels, replay/finale residency, Level-1 gameplay and the
current test package. No new test set is staged by this research.

## Historical HD compression rejection located

User explicitly recalled the previous failed attempt. Confirmed in
DEVELOPMENT_HISTORY.md (15 August 2026, alpha.35/36) and preserved evidence:

- `testresults/Phase 6C.1-alpha35-rejected-fsuae-hd-loading-stall.mov`
  with matching TXT: recorded FS-UAE/HD rejection, 30.817 seconds. Title and
  LOADING appear, CHARGING/gameplay do not; user reports upper-left green marks
  and intermittent glitches. Alpha.35 switched four large HD assets to the
  ADF SPR1 route. No real-Amiga alpha.35 result was supplied.
- `testresults/Phase 6C.1-alpha36-fsuae-68020-performance-rejected.mov`
  with matching TXT/log: raw-SPBM rollback reaches gameplay again. Subsequent
  enemy/performance rejection is separate from the resolved HD loading failure.

Both MOV files still exist and SHA256 matches their sidecars. This follow-up
verified history, sidecars and file integrity; it did not newly inspect video
frames. No diagnostic root cause for the packed-HD failure is recorded here.
Do not confuse it with alpha.34's physical-HD post-CHARGING corruption, which
was the motivation for that attempted loader alignment, or the later low-Chip
hardware case. Git checkpoints group multiple alphas; there is no individual
alpha.35/36 commit in the located log to present as an isolated code diff.

Decision: HD compression is deferred historical rejected work, not the next
optimization candidate. Only reopen with a named fix for the prior failure and
separate HD validation. Prefer redundant asset/cache removal and bounded world
storage while retaining the proven raw-HD loading path.

## First follow-up implemented and HD hardware accepted — 2026-09-06

Chip2 implements only ranked candidate 1 (the six cache families and four
source reads). See STORMRAIL_CHIP_RAM_AUDIT.md. User excludes disk compression.
The bounded 512px departure source and Fast rear originals remain separate
future changes: both touch address/restore invariants and should not be mixed
into the first load-selection regression test. Shared lifetime/cockpit-patch
formats likewise remain research, not implemented claims.

On 2026-09-06 the user accepted Storm-Chip2-HD.zip on a real A1200 and
requested retaining its optimizations in the main game. The normal development
source, executable and runtime assets already contain Chip1 + Chip2; they are
now the accepted development baseline, not a new release. Stormrail still needs
more than approximately 1.45 MB free Chip RAM; the precise free/largest-block
threshold and native savings remain unmeasured. Individual transition/replay
checks and other media are not inferred from this general HD acceptance.
