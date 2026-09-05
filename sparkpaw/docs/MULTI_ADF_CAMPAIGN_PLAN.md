# Campaign multidisk and loading plan

Current: user approved the corrected ADFs and both styled INSERT prompts.
Included in 0.7.0-alpha.2; see RELEASE_0_7_0_ALPHA_2.md for current hashes.
Physical A1200/Gotek/Pocket and new campaign WHDLoad remain separate gates.
The dated design/prototype progression below preserves historical evidence.


## New video evidence — 2026-09-05

The user's brother supplied `testresults/Phase 6C.10-observed-gotek-loading.mp4`
(original name `gotek-loading.mp4`), with matching TXT sidecar. The 83.26-second
clip visibly carries an alpha.68 image label and shows repeated wide OLED
track changes, including returns to 39/40; the ready menu is visible at the
end. It is not a complete cold-boot timing measurement or proof of exact ADF
hash/firmware/CPU. See GOTek_VIDEO_EVIDENCE.md for the evidence boundary.

The protected local alpha.68 image places root metadata at block880/cylinder40
and S/assets/runtime directory blocks at866-868/cylinder39. This makes metadata
lookup/cache behaviour an additional plausible contributor. The first layout
proof must model directory/header access as well as payload order. Video alone
does not identify block reads or prove cache misses, fragmentation or time saved.
No game/ADF/configuration was changed during analysis.


Active planning after the user's 0.7 HD-first release instruction (2026-09-05).
This is disk-loading work, not reopening Level-1 frame-performance tuning.
The user subsequently authorized a two-disk implementation without story plates,
including automatic DF1 detection. A disk-only prototype is implemented; native
acceptance is pending. See MULTI_ADF_030_TEST.txt for the current gate.

## Verified starting points

`tools/report_campaign_media.py` reads the protected alpha.68 ADF and measures
all 48 campaign assets. Every offline SPR1 bitmap estimate is decoded and
compared with the original bytes. Non-bitmap files stay raw in the estimate.
`build/campaign-media-report/report.json` includes per-file sizes, FFS data-block
lists, headers, extensions, cylinder ranges and contiguous data-run counts.

The existing campaign branch of `assetsLoadGameplay()` uses raw SPBM paths even
when the legacy ADF build has packed support. `PROGDIR:` also refers to the
loaded program's directory, not automatically whichever disk is now in DF0.
Simply splitting files across ADFs or enabling ADF_PACKED_ASSETS cannot produce
a working campaign disk set. Media roots, volume validation, retry/cancel,
packed reads and transitions must be explicit cold-loader responsibilities.

## Historical SPR1 capacity estimates (superseded by actual prototype sizes)

DD DOS1/FFS capacity: 1,760 blocks of 512 bytes. Estimates include file headers
and extension blocks. Counts below add seven minimum filesystem blocks; actual
ReadMe, disk ID metadata and future loader growth still need room.

| Proposed role | Estimated blocks | Free blocks before remaining overhead |
| --- | ---: | ---: |
| Boot/title/story presentation + current 155,128-byte executable | 1,317 | 443 |
| Level-1 data + shared presentation/gameplay, no executable, no story plates | 1,440 | 320 |
| Stormrail data + current shared/carryover loads, no executable, no story plates | 1,593 | 167 |
| Entire campaign + story + executable | 2,865 | -1,105 |

Before the no-intro decision and stronger lossless packing, three disks were the
conservative starting proposal: boot/story, Level 1,
Stormrail. This is a capacity feasibility result, not a promise of support.
Two disks without story are tighter: Level 1 plus the current executable uses
1,748 blocks, leaving only 12 before ReadMe, markers or disk-loader growth.
Do not make an overfull or fragile two-disk commitment from compressed byte
counts alone. Re-evaluate after the actual loader exists; do not lower art
quality or remove gameplay to force a disk count.

Shared files are deliberately duplicated on data disks to avoid returning to
the boot disk during each ordinary section load/results transition. Replay
stays resident. Disk switching belongs at initial section entry, CONTINUE and
an explicit return requiring absent presentation data, never during a frame.

Current Stormrail loading still consumes some Level-1 conversion/audio assets;
current shared audio setup loads 9,812 Harrier sample bytes in Level 1. Budgets
include these actual loads. Logical owner groups alone undercount physical
needs. No asset-lifetime optimization is assumed in the numbers above.

## Alpha.68 layout finding

The original ADF contains 35 files; only two have more than one contiguous data
run (ReadMe and score glyphs). Major graphics and executable data are contiguous.
General file fragmentation is therefore not established as the Gotek cause.
However, the old packager creates files in sorted pathname order, not runtime
read order, and its allocator wraps around the disk:

| File | Data cylinder range (22 sectors/cylinder) |
| --- | --- |
| Sparkpaw | 43–49 |
| LOADING packed image | 77–78 |
| Level-1 front | 19–22 |
| Level-1 rear | 22–29 |
| Player sheet | 0–13 |
| Strider sheet | 50–58 |

Startup source reads these graphics in a different order from disk placement.
This demonstrates a locality mismatch worth testing. It is not a measured DOS
trace, Gotek head-travel total or seconds saved: directory/hash lookups, read
buffering, metadata and rotational latency also matter. Renaming assets is not
a demonstrated cure. Keep the already Amiga-safe <=30-character names.

## Original proposed sequence (current implementation below supersedes disk count)

1. Preserve and test the new HD alpha on the real A1200 independently.
2. Build a layout-only proof under `build`, using unchanged alpha.68 extracted
   payloads and an explicit source-derived load-order manifest. Compare extracted
   bytes, block layout and metadata. Keep the original alpha.68 image untouched.
   Native cold-load timing remains a separate user gate.
3. Introduce a cold media resolver/section selector using existing CampaignSection.
   Validate a versioned disk marker before opening section assets. Close DOS
   handles before changing media; restore OS/DOS ownership before disk I/O.
   Handle wrong/missing disk and retry without loading a partial gameplay frame.
4. Use existing SPR1 decode/CRC and staging-buffer contracts for campaign bitmap
   paths. Preserve decoded assets and final Chip/Fast placement; no new per-frame
   allocations. Place each data volume in actual consumption order.
5. Construct complete images only after capacity and loader tests pass. Cover
   startup, both sections, replay, Continue, Esc, title return, wrong disk and
   failed read. Make only one active full test set; archive prior sets intact.
6. User native acceptance: 030 functionality first, then 020; real Gotek/A1200
   cold timing and disk swaps; Analogue Pocket is a separate gate.

No sequential container/trackloader is justified yet. First measure layout-only
benefit; follow ADF_LOAD_OPTIMIZATION_PLAN.md's escalation rule if insufficient.

## Primary-source online checks

The [FlashFloppy configuration documentation](https://github.com/keirf/flashfloppy/wiki/FF.CFG-Configuration-File)
describes drive timing/configuration options. Keep the same firmware, USB media
and configuration when comparing timings; changing them during the comparison
would confound the filesystem-layout test. No configuration tweak is prescribed.

The [Analogue-Amiga core README](https://github.com/Mazamars312/Analogue-Amiga)
lists 68020, AGA, 2 MB Chip and selectable Fast RAM, ADF read/write, and floppy
0/1 selection during play. Those capabilities make a standard DOS ADF plan
plausible. They do not establish Sparkpaw boot, disk-swap or gameplay acceptance.
Use PAL/AGA, 68020 and at least 8 MB Fast for Sparkpaw; record the actual core,
firmware, cache and drive settings with the eventual test result. No ROM is
included or downloaded by this work.

## Current two-disk prototype — 2026-09-05

Scope: disk 1 boots directly to title and contains Level 1; disk 2 contains
Stormrail/finale/results. Five story plates remain HD-only. Shared presentation,
audio and the still-requested Level-1 conversion assets are duplicated as needed.
This avoids changing the accepted gameplay loader's source lists or renderer
ownership solely to satisfy an idealized volume split.

`SPARKPAW_MULTI_ADF` confines the new resolver, compressed bitmap paths and
presentation adjustments to the disk executable. `diskMediaRequire()` scans
DF0 and DF1 for the bounded SP07D1/SP07D2 marker before section asset loading.
A disk already in DF1 is selected without an INSERT prompt. Otherwise the
existing loading image receives only a 224x40 text patch; no second copy of
its artwork is packaged. Wait for the prompt before ejecting the current disk.
Wrong or absent media while waiting remains at the prompt. The user requested
removal of the helper lines and left-mouse cancellation. The same mechanism requests disk 1 on a cold return.
Replay of the current level retains the existing resident path without reads.

The marker identifies this prototype format/pair, not a cryptographic asset
identity. Change it before staging any incompatible later pair. Missing/corrupt
asset reads fail through existing cleanup; arbitrary early ejection during
presentation/asset reads is not a supported swap boundary.

Bitmaps choose the smaller existing SPR1 run encoding or new SPL1 LZSS encoding.
SPL1 uses a fixed 4 KiB BSS history, 512-byte DOS staging, lengths 3–18 and
backreferences at most 4096 bytes. Size bounds and CRC32 remain checked. No new
per-frame allocation or gameplay I/O is added. BSS placement is DOS-selected;
final asset Chip/Fast allocation flags are unchanged. A replacement loading
bitmap retains the old one until the replacement Copper is installed, then
retires it; that temporary cold-loading overlap is not a memory-peak measurement.

The packager writes standard DOS1/FFS images in explicit source-derived first-use
order, with data allocation starting near the beginning instead of wrapping
from the disk end. Central root/directory metadata stays standard. Exact data
blocks, file headers, extensions and directories are in
`build/multidisk-probe/media.json`. Repeated common presentation loads and DOS
metadata accesses remain; this is not a sequential trackloader or measured
seek trace. Compression and layout are combined for feasibility, so this pair
cannot isolate their separate contributions to load time.

Offline verification includes the actual C reader under ASan/UBSan (45 valid,
corrupt and boundary cases), all packaged bitmap decodes against original bytes,
actual media resolver with DOS stubs (DF1, DF0 swap, wrong marker), all
ADF file readbacks, executable reference coverage, monotonically ordered data
blocks, boot-disk checksum and the full existing host suite. 68020 compilation
passes with the existing renderer optimizer warnings. No native boot, disk swap,
visual, timing, memory-peak, Gotek or Pocket acceptance is implied.

Rebuild from repository root, using the established local toolchain:

```sh
.venv/bin/python3 sparkpaw/tools/generate_disk_status.py
.venv/bin/python3 sparkpaw/tools/test_multidisk_probe.py
.venv/bin/python3 sparkpaw/tools/build_multidisk_probe.py
.venv/bin/python3 sparkpaw/tools/package_multidisk_probe.py
```

These commands write only `build/multidisk-probe`. The build derives flags and
source order from the current production Makefile, removes story and adds only
the disk flags/module. Do not run the legacy release/ADF packager for this proof.
Final hashes and capacity are recorded with the sole staged test pair; rebuilding
FFS timestamps can change image hashes without changing the executable/assets.

## Reported failure and packaging correction

The user reports Level 1 playable from Disk 1, but Stormrail start fails during
loading both after DF0 swap and with Disk 2 already in DF1. CPU/RAM and exact
crash presentation were not specified; this is user evidence, not an emulator
trace. Inspection of the actual failed images confirms Disk 2 omitted
`storm-collision.bin`. `collisionLoad()` unconditionally opens that map even
for Stormrail; a missing file returns FALSE into the existing fatal-load path.
This proves a load-blocking packaging defect consistent with both failures,
not a native proof that every reported crash symptom has been explained.

The corrected pair adds the unchanged 2,968-byte map to Disk 2. Executable and
all original asset bytes are unchanged. The packager now checks compiled
references per disk after excluding only the opposite section's graphics;
union coverage alone is insufficient. The old pair is preserved in
`dist/older-builds/Campaign-2Disk-030-missing-map`. New native gate pending.

## Styled INSERT candidate

User requested LOADING/CHARGING-style text without helper lines. Two generated
strips use the existing palette and 224x40 patch; the floppy art stays exact.
Left-mouse cancellation is removed. Correct media resumes loading automatically.
No gameplay changes; corrected collision-map packaging remains. Prior pair is
archived under older-builds/Campaign-2Disk-Fix-030. Supplied screenshots prove
presentation only, not successful Stormrail entry. Native acceptance pending.

## Historical styled-candidate identity (archived)

Executable: 156284 bytes; SHA256 `649da2d4d1653cdd23977527c4e21a7c202f00e1f3fb209d870b65c54214e833`.

| Disk | Free blocks | SHA256 |
| --- | ---: | --- |
| 1 | 298 | `8ac19f5e59b9449aac29d0a4fba21dbb5cc1fbdc769cce78e0df152c5df5fec7` |
| 2 | 533 | `d28004674c9e61c4d464160d1368ef12907789b59bda05830f30e376280479db` |

Both images are 901,120 bytes; this pair is now archived at
dist/older-builds/Campaign-2Disk-Art-030. Current release image hashes differ
because of FFS timestamps; executable and runtime payloads match this pair.
