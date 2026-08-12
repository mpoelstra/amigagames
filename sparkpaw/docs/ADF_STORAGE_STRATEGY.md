# Sparkpaw ADF storage and multidisk research

Status: research and planning only, 13 August 2026. Nothing in this document is
implemented. Phase 5D is accepted; Phase 5E traversal remains the active roadmap
step. ADF optimization is intentionally deferred.

## Goal and non-negotiable constraints

Keep the HD/LHA/ZIP edition simple and uncompressed unless measurements later
justify sharing a loader. Allow the ADF edition to use a different on-disk
representation, but require it to reconstruct byte-identical runtime data before
the accepted renderer consumes it. Do not trade away the A1200/68020/2 MB Chip
RAM minimum, the direct title/loading presentation, the stable Copper/Blitter
timing or ordinary ADF/Gotek/real-floppy compatibility merely to win bytes.

The current DOS1/FFS ADF is valid and bootable, but its file payload is 852,080
bytes and the filesystem reports 859,136 data bytes used (97.5% of its file-data
allocation). A standard DD Amiga disk exposes 880 KiB as 160 tracks × 11 sectors
× 512 bytes. Sequential reads are substantially faster than scattered reads
because the device buffers whole tracks. These are documented properties of
`trackdisk.device`, not emulator assumptions.

## Current payload and measured compression opportunity

The release payload is dominated by planar data, not the 31,480-byte executable:

| Asset group | Raw bytes | Observation |
|---|---:|---|
| Player source SPBM | 172,860 | both facings and many related frames |
| Foreground world | 163,900 | sparse/repeated generated planar data |
| Rear world | 122,916 | repeated resident scenery |
| Strider source SPBM | 122,940 | mirrored cells and repeated structure |
| Three 64-colour presentation screens | 184,932 | title is much less repetitive than loading screens |
| Everything else, including executable | 84,532 | small individually |

A read-only host experiment compressed every current file independently. zlib
level 9 reduced the 848,882-byte executable-plus-runtime set to 202,091 bytes;
LZMA reduced it to 171,832 bytes. These numbers are **opportunity indicators,
not promised Amiga packer results or timings**. Important zlib proxies were:

| File | Raw | zlib proxy | Ratio |
|---|---:|---:|---:|
| `storm-front.spbm` | 163,900 | 1,359 | 0.8% |
| `storm-rear.spbm` | 122,916 | 3,070 | 2.5% |
| Strider SPBM | 122,940 | 13,877 | 11.3% |
| Player SPBM | 172,860 | 73,855 | 42.7% |
| loading / charging SPBMs together | 123,288 | 27,953 | 22.7% |
| title SPBM | 61,644 | 48,466 | 78.6% |
| executable | 31,480 | 18,669 | 59.3% |

The implication is strong: first pack or reconstruct world and Bob data. An
executable cruncher alone cannot create meaningful future level headroom.

## Proven technology candidates

### Shrinkler

[Shrinkler](https://github.com/askeksa/Shrinkler) is the established Amiga
maximum-ratio executable cruncher used for size-coded productions. It supports
Amiga hunks, all Amiga CPUs/Kickstarts, raw-data mode, 68000 decompression source,
headers with packed/unpacked sizes and overlapped decompression. Its strength is
ratio; startup/decrunch time is the tradeoff. It is an excellent benchmark and
possible executable packer, but should not automatically become the format for
every gameplay asset.

### L-Packer, ZX0/Deflate and LZ4

[L-Packer](https://github.com/arnaud-carre/L-Packer) is an Amiga/Atari executable
and raw-data packer that tries Deflate, ZX0 and LZ4 and selects according to the
target. Its author positions it between Shrinkler's ratio and fast but weaker
traditional crunchers, with roughly 3–5 second decompression for a 64 KiB demo.
It is currently beta and its full source is promised for 1.0, so pinning and
reproducibility must be evaluated before adoption.

[lz4-68k](https://github.com/arnaud-carre/lz4-68k) provides reviewed 68k assembly
decoders: 72-byte smallest, 180-byte normal and a larger fastest variant. The
author explicitly recommends the normal decoder for cached CPUs such as the
68020. LZ4 sacrifices ratio for extremely fast loading/depacking and is a good
candidate for frequently loaded or block-streamed data.

[ZX0](https://github.com/einar-saukas/ZX0) is a compact LZ-family format for
low-end systems with multiple platform decompressors. For Sparkpaw it belongs in
the measured bake-off, not as an assumed winner: decoder availability, 68020
timing, in-place safety and licensing must all be frozen with the exact version.

### AmigaDOS/FFS and `trackdisk.device`

The current DOS1/FFS disk is already the sensible filesystem baseline. The
official [trackdisk device documentation](https://wiki.amigaos.net/wiki/Trackdisk_Device)
states that standard disks provide 880 KiB, that reads are physically performed
a track at a time, that sequential layout can be up to an order of magnitude
faster than scattered reads, and that disk-change state/counters are supported.
This favours a few large, sequential pack files over many tiny files.

The official [Amiga floppy boot/layout documentation](https://wiki.amigaos.net/wiki/Amiga_Floppy_Boot_Process_and_Physical_Layout)
confirms that boot code may use the already-open trackdisk request to load code.
That makes a bootblock/trackloader technically possible, but not automatically
desirable for this game.

## Recommended single-ADF architecture

### Stage A — measurement harness only

1. Add a host report that records raw size, packed size, decode CRC and projected
   FFS blocks for each release asset.
2. Benchmark pinned Shrinkler raw data, L-Packer's candidates and direct LZ4/ZX0
   candidates on representative files.
3. Measure on stock 68020 settings: cold floppy load time, decrunch time, peak
   Chip/Fast allocation and largest free Chip block. Compression ratio alone is
   not an acceptance metric.
4. Keep the report out of the runtime and do not change the HD package.

### Stage B — ADF-only packed asset container

Generate `sparkpaw-adf.pak` only for the floppy release. Give it a small table
containing stable asset ID, codec, packed offset/length, unpacked length,
destination class and CRC. Keep normal SPBM/raw files in HD releases. Select the
ADF loader through a build-time flag or tiny ADF-specific executable so the HD
path stays straightforward.

Pack large assets independently or in 8–32 KiB output blocks. Allocate the final
BitMap/Chip destination first and decode directly into each plane/mask whenever
the codec permits. Reuse one bounded input/scratch buffer and free it between
assets. Never require a second complete 170 KiB unpacked copy in Chip RAM.

Recommended first codec trial:

- LZ4-normal for fast block decode during visible loading;
- ZX0/Deflate as ratio alternatives for screens and cold-loaded level packs;
- Shrinkler for the small executable and as the maximum-ratio comparison;
- leave already noisy Paula samples raw unless measurement proves worthwhile.

Keep the title immediately available or uncompressed so boot never looks hung.
Use the existing `LOADING`/`CHARGING` states for disk I/O/depack feedback. Never
decrunch during the line-253 gameplay Bob window.

### Stage C — asset-aware reconstruction (the highest-value demo technique)

General compression is not the ceiling. The extraordinary world proxy ratios
show that Sparkpaw stores generated repetition verbatim. For the ADF edition:

- store unique 16×16/32×16 tiles plus compact tile maps and expand the resident
  planar worlds during `CHARGING`;
- store only authored/right-facing enemy cells and generate exact indexed
  mirrors while building the existing packed Bob caches;
- test XOR/delta frames inside animation families before the final entropy pack;
- deduplicate identical placeholder cells and reference them from a frame table;
- keep masks derivable from pen zero where generation cost and peak memory are
  acceptable;
- preserve the current HD SPBM generator as the byte-exact oracle.

Every reconstruction path needs a host test that compares decoded planes,
palette, mask and final cache bytes against the ordinary HD assets. This is how
demos obtain large apparent content from small disks: store structure, deltas
and generators, then precalculate during a controlled loading screen.

### Stage D — executable and filesystem polish

After asset packing works, compare Shrinkler and L-Packer on the final ADF-only
executable. Keep whichever has acceptable stock-68020 startup and no change to
Chip allocation or AmigaOS startup behaviour. Order the container sequentially
by actual load phase and keep the number of FFS directory entries low. Expected
filesystem savings are useful but secondary to packed data.

## Multidisk plan

### Preferred first version: ordinary DOS volumes

Use uniquely labelled DOS1 disks and volume-qualified data paths, not `DF0:` and
not `PROGDIR:` for later disks. AmigaDOS can then find a requested volume in
DF0:–DF3 and naturally presents an insert-volume requester when absent. The
[AmigaDOS documentation](https://wiki.amigaos.net/wiki/AmigaDOS_Introduction)
documents volume swapping and assigns; `trackdisk.device` exposes explicit disk
presence and change counters if a custom in-game prompt is later preferable.

Suggested split:

- Disk 1: boot, executable, title/loading UI, common player/HUD/audio/enemy data,
  level 1 pack;
- Disk 2+: one or more complete level packs, unique scenery, level music and
  bosses; duplicate only a tiny swap/loading screen if necessary;
- optional save disk only after save/progression design exists.

Load a complete level pack before hardware takeover or while a controlled
loading display owns the screen. Keep common assets resident across swaps.
Persist gameplay/progression in RAM, validate a disk ID/version/CRC before
continuing, support DF1: when present, and never require repeated ping-pong
swaps during one level.

### Optional later version: sector table through `trackdisk.device`

A filesystem-less sequential pack can avoid FFS metadata and give deterministic
sector placement while still using standard 512-byte sectors and ordinary ADFs.
It requires a sector manifest, checksums, retry/error UI, disk-change handling,
drive-unit selection and careful coordination with the OS/device before the
game's custom-chip takeover. It should only be attempted after the DOS-volume
multidisk build is accepted.

### Not recommended now: raw custom MFM trackloader

Classic demos often combine tiny boot code, raw track loaders, precalculation
and aggressive compression. Direct custom MFM can improve streaming control and
sometimes capacity, but it also owns CIA/disk DMA details, complicates disk
changes and retries, can conflict with system services, and may require extended
ADF/IPF rather than a universally portable standard ADF. Official Amiga guidance
warns that direct CIA programming is incompatible with expecting system
`trackdisk.device` services to keep working. See
[Programming in the Amiga Environment](https://wiki.amigaos.net/wiki/Programming_in_the_Amiga_Environment).

For Sparkpaw the likely saving over a packed DOS1 container is small compared
with the hundreds of kilobytes available from asset reconstruction. Revisit a
raw trackloader only if measured DOS/trackdisk load latency—not capacity—is a
shipping problem, and only with real-floppy, Gotek, FS-UAE and multiple-drive
testing budgeted as its own milestone.

## Acceptance gates for any future implementation

1. HD/LHA/ZIP payload and loader remain unchanged unless separately approved.
2. Decoded ADF assets compare byte-for-byte with canonical generated output.
3. ADF boot, disk-full margin, manifest CRCs and archive contents are verified
   automatically.
4. Peak allocation is proven on the 2 MB Chip/no-Fast configuration.
5. Loading and decrunch times are measured on stock 68020 emulation and then
   supplied real hardware; neither may be inferred from a desktop benchmark.
6. Corrupt/truncated asset and wrong-disk paths fail visibly and safely.
7. Renderer timing and displayed Chip RAM composition remain untouched.
8. Trackloader work, if ever approved, is isolated from gameplay/render changes.

## Proposed decision order

1. Complete Phase 5E without coupling navigation to disk-format work.
2. Before the 2048px Phase 6 experiment, add only the host size/codec report.
3. Use that experiment to compare tiled reconstruction versus generic packing.
4. Implement the ADF-only block container with one asset as a reversible proof.
5. Expand it asset-by-asset, then evaluate executable crunching.
6. Add ordinary AmigaDOS multidisk volumes when content genuinely exceeds one
   comfortably margined packed disk.
7. Consider sector-table or raw-track loading only after measured evidence shows
   that the simpler design cannot meet load-time or capacity goals.
