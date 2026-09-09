# READY / OPTIONS / SOUNDTEST performance contract

Mandatory for every READY menu extension, including campaign and ADF variants.
The 68020 navigation regression of 9 September 2026 bypassed the accepted
alpha.3 ready_patch precomputation by adding live text/mask rasterization and
whole-patch comparisons. Correct pixels on host and a successful 030 test do
not establish acceptable 020 frame cost. READY Neon Sky advances from the owned
frame loop, so excessive foreground work delays both particles and music.

## Required boundary

- Author layout in `tools/ready_ui_layout.c`, a HOST-ONLY rasterizer. Never add
  it, its glyph renderer, or the frozen test oracle to native SOURCES.
- `tools/generate_ready_ui_cache.py` rasterizes all valid HD/ADF states OFFLINE,
  deduplicates five independent horizontal bands (including the exact current
  glyph dust margins) and computes transition rectangles. Generated data and
  allocation headers must be current; regenerate after layout/state changes.
- `readyUiInit()` decodes/reconstructs those immutable bands into explicitly
  allocated Fast RAM while CHARGING and OS VBlank music remain live, BEFORE
  fade-to-black and READY takeover. No on-demand/lazy cache misses in menus.
- `readyUiCompose()` only selects five band IDs and copies masks of changed
  bands. `readyUiApply()` compares five IDs for the hidden target and copies
  precomputed rectangles. No font drawing, glyph-margin construction, full
  image comparison, allocation, decompression or filesystem access in either.
- Each hidden target retains its own band IDs. Multiple input/status changes
  may occur before it returns; never assume its content is the preceding
  logical selection. Identical IDs require no image writes or pixel reads.
- Preserve order: restore that target's dust, apply cached menu, draw current
  dust, publish hidden display, owned-frame music tick. Keep current-state
  masks, not a union of inactive labels. No displayed-plane writes.
- Cache sources are Fast RAM. CPU copies only; do not repeat the rejected
  Fast2 experiment of passing Fast pointers to the Chip-only Blitter.
- Adding modes/rows/status values requires updating the offline state mapping,
  runtime selection mapping and exhaustive coverage together. Do not bypass
  the cache because the former atlas/state table has become inconvenient.

## Automated protection

`make test` includes `tests/test_ready_audio_ui.py`. It checks generated-data
freshness, compares all 1,070 states with the frozen pre-fix layout plus the approved Storm Light entry, exercises
all band-variant pairs plus skipped/older hidden targets (4,968 selections),
checks exact current-text masks, and uses real dust restore/draw across switches.
It also guards the title initialization/publication order, campaign cache call
sites, and absence of runtime rasterizer/image-scan dependencies. Keep these
checks when changing the UI; an intentional layout change requires explicit
review of the reference and state coverage, not removing the guard.

Inspect native `-O2 -cpu=68020` output after changing copy code. The repaired
inner copy uses four postincrement MOVE.W operations per loop; the compiler's
previous indexed unrolling created redundant pointer arithmetic. Host time is
not an Amiga benchmark. New timing-sensitive changes require a focused user
020 navigation/music check even when pixel parity passes.

## 9 September repair candidate

`dist/Audio-Options-Fast-HD/Sparkpaw-Audio`, based on local alpha.5 audio-options
layout revision 5. No layout, audio policy, gameplay or pause change in this fix.
77 distinct bands represent 878 states. Decoded cache: 275,184 bytes; complete
UI allocation: 278,111 Fast bytes, previously 72,800 (+205,311). No new Chip
bitmap. Offline delta/zero-run packing stores 65,403 bytes, plus 7,556 transition
bytes and 4,390 mapping bytes (small descriptor tables additional). All decoding
and delta reconstruction occur once per READY preparation, never on navigation.

Example CPU writes per hidden target: main highlight 1,968 bytes; first OPTIONS
row to second 924; second to AUDIO MODE 1,008; main-to-OPTIONS 14,976. These are
copy volumes, not measured elapsed time. Main highlight is a little wider than
the old row-span scheme (1,320); eliminating live rasterization/comparison is
this repair's objective. Page changes still have a larger copy cost.

Host parity and native compilation pass. The user's report rejects the preceding
candidate's 020 navigation; repaired 020 music/dust/cadence acceptance remains
pending. Extra preparation time and Fast usage are explicit tradeoffs. ADF source
selection has host coverage, but no new ADF is packaged or capacity-accepted;
alpha.5 Disk 1's 15 KiB reserve must not be assumed sufficient for larger data.
WHDLoad/hardware runtime acceptance remains separate. No release, commit, push
or automatic FS-UAE run is part of this repair.

## Soundtest retained-display handoff — 9 September 2026

Current candidate supersedes the cache-only drawer:
`dist/Soundtest-Resume-HD/Sparkpaw-Audio`. The user reports occasional brief
visual corruption when loading/starting another module, followed by correct
playback/display. The source used platformFinishTakeover after DOS loading:
it disabled display DMA and strobed COPJMP1 without a raster boundary, even
though the same READY list remained visible throughout the load.

After `platformReleaseForLoading(TRUE)` in Soundtest, use only
`platformResumeMenuAfterLoading()`. It reacquires Blitter/Exec ownership,
masks OS interrupts and disables disk/sprite/inactive audio DMA, preserving
Copper, bitplane pointers, master/display DMA and active LSP DMA. The existing
list continues naturally; no COPJMP1 or COP1LC write. This API requires the
retained READY display and must not replace initial/gameplay takeover. Both
preview loading and Neon reload on exit use it. No new frame work/allocation.

`test_menu_display_resume.py` executes the real function under MMIO/OS mocks,
checks EVERY DMA write and 32 playing/channel-mask cases; the register shim
excludes Copper registers. It guards both Soundtest call sites. Existing menu
lifecycle tests retain all tracks, failure-return and F10 coverage. Native
visual acceptance is pending; these checks establish register/ownership policy,
not observed glitch removal. Synchronous loading may still pause dust/input.

## 10 September — fifth Soundtest track

Soundtest extension (10 September): STORM LIGHT is now the fifth MUSIC TEST
track, using musicPlayGameOver/LSP (never the CIA gameplay preview). Tests
cover all five tracks, failure cleanup, start/stop/restart and bidirectional
wraparound. Offline cache/reference coverage: 1,070 states / 4,968 transitions;
284,383 Fast bytes (+6,272), no extra Chip bitmap. ADF cache unchanged.
New normal-three-life HD drawer: sparkpaw/dist/StormLight-Soundtest-HD,
launch Sparkpaw-Audio. Existing one-life game-over drawer remains unchanged.
Native HD builds; subjective audio/native cadence acceptance pending.

The frozen oracle changes only by appending STORM LIGHT to its track labels; existing layout and rasterizer remain unchanged.
