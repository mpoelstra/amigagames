# WHDLoad loader comparison — 2026-09-23

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


## Conclusion for Sparkpaw

Disk reads are not inherently faster than RAM reads. Compare complete pipelines:
raw disk transfer plus copies versus compressed cache transfer plus decoding
and CRC. The former can win if decoder time dominates; no native throughput or
break-even time has been measured for this setup. A raw RAM-cached transfer
avoids both disk latency and decompression. Per-level loading principally
reduces simultaneous residency and startup work; it does not accelerate the
storage device itself.

The concrete sources below support replacing fine-grained file handling with
bounded direct WHDLoad reads, and treating storage format, game working RAM
and cache policy separately. They do not prove a Sparkpaw speedup or justify
removing its existing ownership/Chip-memory safety boundaries.

## Inspected examples

### Ruff 'n' Tumble — direct sector ranges, decoder kept separate

Official install: https://www.whdload.de/games/RuffNTumble.html
Downloaded source: build/whdload-slave-research-20260923/extracted/
RuffNTumbleInstall/source/RuffNTumbleHD.s.

`read_sectors` (line 1062) translates the game's requested sector count and
offset into bytes and makes one `resload_DiskLoad` call for that range (1087).
It does not loop over each 512-byte sector in this wrapper. Decompression is
separate (`decrunch_and_patch`, 470; `decrunch`, 1093). The install history notes
a decoder move into Fast memory. This is an example of direct range loading,
not evidence that the game is uncompressed or that disk beats cached RAM.

### Oscar — complete-file WHDLoad calls and selective OS emulation

Bundled official developer source:
.toolchain/whdload-dev/WHDLoad/Src/slave-examples/oscar.asm.

`_loader` (295) strips the old drive prefix and calls
`resload_LoadFileDecrunch` directly. `_loadercd` (323) follows the same model.
`_decrunch` (303) copies its legacy decoder to expansion memory on first use.
Main program loading uses the WHDLoad relocate service (104-112); this example
is not a complete Kickstart boot environment. Its narrow emulated services
cannot simply replace Sparkpaw's graphics/DOS/audio/Exec dependencies.

### Battle Isle — KickEmu can coexist with a direct file-load hook

Bundled official source:
.toolchain/whdload-dev/WHDLoad/Src/slave-examples/battleisle.asm.

This uses kick13.s, yet its patched `_loadfile` path (570-629) ultimately calls
`resload_LoadFileDecrunch` on the requested complete file. Thus adopting a
direct asset backend does not inherently require removing the entire KickEmu
environment at the same time. Do not transplant its game-specific allocator
or patch addresses into Sparkpaw; the disabled example branch is not an active
allocation path. This observation is architectural, not a timing benchmark.

### Flashback — explicit memory variants, documented OS-patch rewrite

Official install: https://www.whdload.de/games/Flashback.html
Downloaded ReadMe and slave headers inspected; its nested LZX source archive
is preserved but NOT extracted/read. No source-level loader claim is made here.

The ReadMe documents a rewrite replacing OS functions to reduce requirements,
a separate LOWMEM slave with a zoom feature trade-off, and optional packed data
support. It confirms that explicit variants and compression policies are normal
engineering choices, but its small runtime requirements cannot be transferred
to Sparkpaw. This is precedent for separately documented packages, not an
argument to drop Sparkpaw features or assume its full campaign fits equally.

## What Sparkpaw currently does

`whdload/Sparkpaw.asm` includes kick31.s, reserves 3 MiB game Fast plus
512 KiB Kickstart, and sets IOCACHE=4096. Ordinary asset reads in `src/assets.c`
use a 512-byte staging buffer. Packed input refills are also 512 bytes.
The call chain is application reader -> emulated DOS -> kickfs -> WHDLoad.

The inspected kickfs.s `.a_read` implementation (704-842) serves small requests
from its per-file 4096-byte cache. It refills with `resload_LoadFileOffset` and
copies cached bytes with a byte loop. Requests at least as large as IOCACHE can
use its direct range-read branch. Therefore **512-byte application read does
not mean 512-byte physical disk operation**; previous language suggesting a
one-to-one relation must not be used. Cache hits still have DOS/handler and
copying overhead. Whether a resload call reaches storage depends on WHDLoad's
separate file cache.

The raw 512-byte path was introduced for real-hardware DOS MaxTransfer/Mask
safety. Do not globally increase it for normal HD or ADF. A future direct
WHDLoad-only backend must be independently gated and preserve final Chip/Fast
placement and buffer lifetime. The source comparison identifies avoidable
work; its contribution relative to decompression is still unmeasured.

## Relevant API contracts

Official API: https://whdload.de/docs/autodoc.html
Options: https://www.whdload.de/docs/en/opt.html

`resload_LoadFileOffset` and `resload_DiskLoad` accept a caller-selected range;
`resload_LoadFileDecrunch` handles supported formats on full-file loads.
Sparkpaw's SPL1/SPR1/SPD1 are project formats and do not automatically become
supported by that API. PRELOAD caches files while memory permits. PreloadSmart
can prioritize an existing access sequence; it is not proof of cold-start cache
coverage. `ws_DontCache` excludes matching files entirely and is not a free
per-level cache. These contracts explain possible designs, not their measured
performance in Sparkpaw.

## Bounded next implementation direction

1. Preserve the user's startup contract: executable, intro/music, title/music,
   loading/READY and Level 1 raw. Keep the optimized decoder for later data.
2. Establish actual worst-case game Fast use across startup/READY, all sections
   and replay before reducing ExpMem. The old 5-MiB diagnostic is insufficient.
3. Prototype a WHDLoad-only large-read backend or bounded level container,
   keeping normal HD/ADF readers unchanged. Use a limited staging buffer or
   ownership-safe destination; do not add a duplicate whole-game RAM cache.
4. Stage later-section reads only at existing load transitions. Do not prefetch
   in stats, mutate resident replay assets or assume uncached reads are invisible.
5. Verify decoded parity/bounds and use the user's 68020/8-MiB manual gate for
   first-image latency, all intro gaps, TITLE/LOADING stability, READY and menu
   return. Host operation counts alone do not establish native speed.
6. At a separately authorized release, supply distinct 8-MiB and fully-raw
   higher-memory packages. Higher-memory minimum still needs verification.

No new runtime edits, candidate, emulator launch, release, commit or push were
made in this research step. Existing work/builds remain preserved. Downloads,
extracted sources and SHA-256 provenance are under
build/whdload-slave-research-20260923; only install/source archives were fetched.

## Follow-through: bounded level-bank prototype

The next candidate implements a conservative variant of the direct-read
finding: one large DOS Read per explicitly selected phase container reaches
kickfs's existing resload_LoadFileOffset branch. It avoids a new callback ABI
and retains KickEmu. The game owns common/intro/current-level sources; the
slave excludes files from caching. Only the current level changes at released,
black transitions. This is implemented, not yet timed or accepted natively.
See WHDLOAD_LOADING_INVESTIGATION.md for exact memory trade-offs, remaining
startup bulk-I/O cost and host verification. The previous "no runtime edits"
statement describes the earlier research step only.
