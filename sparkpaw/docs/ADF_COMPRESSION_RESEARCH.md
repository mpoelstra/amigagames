# Two-disk game-over compression proof — 10 September 2026

The current game-over campaign fits on two standard 901,120-byte DOS1/FFS
ADFs without changing the artwork, palettes, samples, musical arrangement or
three starting lives. This is a capacity/native-build candidate, not a release
or a native runtime acceptance result. The existing alpha.5 packages remain
unchanged. Earlier advice to choose three disks was premature: the existing
SPR1/SPL1 layout overflowed, but further lossless compression had not been tried.

## Research and selection

The original [RNC ProPack manual](https://github.com/tobiasvl/rnc_propack/blob/master/PROPACK.TXT)
documents an offline packer and supplied MC68000 unpackers. This illustrates
the relevant game-development pattern: store compressed assets, reconstruct
them at load time, and let rendering/audio consume the ordinary data. It does
not require rendering or replay to understand the disk format.

Amiga's [ILBM documentation](https://amigadev.elowar.com/read/ADCD_2.1/Devices_Manual_guide/node02D9.html)
also describes line-by-line compressed bitmap data. Sparkpaw already uses a
similar simple byte-run option plus 4-KiB-window LZSS. Changing a PNG's download
size cannot help: our disks contain native planar SPBM data, not PNGs.

[Shrinkler](https://github.com/askeksa/Shrinkler) specializes in Amiga executable
compression and includes a decompression header. Its host verifier checks the
reconstructed hunks and relocations. We use normal `-1 -p` mode; no hunk merge,
mini header or overlapping decompression experiment. The upstream header
[clears CPU caches](https://github.com/askeksa/Shrinkler/blob/master/decrunchers/Header.S)
on applicable Kickstarts before the program runs. Native startup duration and
hardware compatibility still need testing. Source snapshot and license are
retained in third_party/shrinkler; tools/crunch_adf_executable.py records hashes
binding the packed executable to its exact original.

[LZ4-68k](https://github.com/arnaud-carre/lz4-68k) prioritizes fast decompression;
[Shrinkler](https://github.com/askeksa/Shrinkler) and
[ZX0](https://github.com/einar-saukas/ZX0) offer other size/speed tradeoffs.
Mark Adler's [puff](https://github.com/madler/zlib/tree/develop/contrib/puff)
is a small bounded DEFLATE decoder, but is explicitly slower than zlib.
None of these asset decoders needs to be added for the present two-disk fit.
Host compression times are not A1200 loading-time measurements.

[LSP](https://github.com/arnaud-carre/LSPlayer) separates music commands from
sample banks. Sparkpaw already shrinks unreachable sample tails during MOD
conversion. For disk storage, exact modular byte differences expose repetition
in the PCM waveform. Our new SPD1 container then applies the existing LZSS
codec, and cumulatively adds the differences back before CRC validation and
playback. Every restored byte must equal the existing HD sample bank. This is
not an instrument, sample-rate, bit-depth or arrangement reduction. Do not
confuse this exact delta transform with a lossy audio-codec setting.

## Measurements

Actual selected candidate figures (filesystem totals below are authoritative):

| Item | Previous stored bytes | Selected stored bytes | Saving |
| --- | ---: | ---: | ---: |
| Current ADF executable | 224,388 | 89,760 | 134,628 |
| Storm Light sample bank | 98,836 (SPL1) | 72,031 (SPD1) | 26,805 |
| Neon Sky sample bank | 97,406 (SPL1) | 70,892 (SPD1) | 26,514 |
| Storm Light music commands | 2,045 | 2,045 | unchanged |
| Game-over artwork | 58,522 | 58,522 | unchanged |

Storm Light is 207,919 decoded bytes including commands. On each disk the
same music occupies 74,076 bytes. Its complete bank is loaded only when needed;
SPD1 adds a byte accumulator to the existing streaming reader, reusing its
4-KiB LZ history. There is no new full-size decoded temporary buffer or
per-frame decompression cost. The executable cruncher adds about 90 kB of
transient allocation during startup; its report says zero persistent overhead.
Its memory hunks retain their original memory type; do not relabel this as a
measured Chip/Fast peak without a native trace.

Earlier ADF-only cache selection saved ~38 kB of executable data, and omission
of the unreachable legacy menu atlas saved ~39 kB on disk. Those optimizations
are already included in the starting point above.

The first broader offline survey also measured DEFLATE and LZMA. Examples:
player sprite atlas 98,016 -> 80,893 bytes with DEFLATE, strider atlas
44,077 -> 28,856, title image 57,635 -> 48,482. These estimates include a
16-byte container allowance and zlib framing; no matching runtime decoder was
integrated. Applying byte differences before DEFLATE gave a 59,579-byte Storm
Light bank. These are future options, not savings claimed for this candidate.
Measurements and the reproducible survey script are under build/compression-study.
We intentionally retain the existing image decoder now that two disks fit.

## Verification and reproduction

From sparkpaw, with a Python installation containing Pillow:

```
python3 tools/build_multidisk_probe.py
python3 tools/generate_disk_status.py
python3 tools/test_multidisk_probe.py
python3 tools/crunch_adf_executable.py
python3 tools/package_multidisk_probe.py --crunched-executable build/multidisk-probe/Sparkpaw-crunched
```

The packager requires the exact input/output hash binding, checks compiled
asset-reference coverage separately for both volumes, reads every file back,
and enforces at least 32 free blocks per disk. New SP07G1/SP07G2 disk markers
prevent silently mixing this candidate with alpha.5's SP07M disks.

The actual reader passes 85 host cases including split reads, repetitive and
random bytes, full sample banks, CRC errors, truncation and invalid backreferences.
ASan/UBSan are enabled. DF1 selection, wrong-marker rejection, DF0 swap and DOS
requester restoration are covered with actual media source and host stubs.
Shrinkler's hunk/relocation verifier reports OK; native ADF compilation succeeds.
All stored assets are decoded and compared with their original bytes during
packing, then ADF filesystem contents are independently read back.

Still pending: user boot on 68030, then 68020; startup time; title/READY music;
complete game-over playback after defeat in both sections; single-drive swaps
and DF1 discovery; returning to title; final real-A1200/Gotek acceptance. No
emulator has been launched by Codex and no release has been created.

## Actual filesystem result

Disk 1: 73 free blocks = 37,376 bytes (36.5 KiB).
Disk 2: 185 free blocks = 94,720 bytes (92.5 KiB).
Both exceed the ordinary 32-block / 16-KiB reserve.

## Release integration

Alpha.7 release packaging now invokes tools/crunch_adf_executable.py and binds
its output hash to the original native build. Source is vendored under
third_party/shrinkler, host binary under ignored build/shrinkler. Standard
32-block reserve remains mandatory. See RELEASE_0_7_0_ALPHA_7.md.
