# Music campaign ADF candidate

> Alpha.5 checkpoint (9 September 2026): campaign music plus mixed SFX now
> ships in HD, ADF and WHDLoad. Positive user HD/ADF reports are recorded;
> new WHDLoad audio and final hardware acceptance remain pending.
> See RELEASE_0_7_0_ALPHA_5.md. Earlier candidate/no-release statements below
> are historical and superseded by the explicitly authorized release.


9 September2026. User accepted Stormrail HD generally (“ok gaat goed”) and
requested a practical ADF version. This does not infer 020 or real-hardware
acceptance. Main HD and all61 alpha.4 release files remain byte-identical.

## Two disks, preserved content

Standard901120-byte DOS1/FFS images, one bootable Disk1 and data Disk2.
Executable178712 bytes. Disk1 free30 blocks/15KiB; Disk2 free318/159KiB.
Compared with alpha.4's9/345 blocks, Disk1 gains10.5KiB headroom despite adding
music+SFX, while Disk2 spends13.5KiB. Packaging used explicit
`--minimum-free-blocks 24` (12KiB reserve); the default32-block guard was missed
by only2 blocks on Disk1. Actual filesystem capacity/readback guards retained;
no packing beyond capacity or blanket lowering of the default.

Like the preceding ADF, no story intro/Hero Drive. Neon Sky remains full-four
channel through title/READY. Copper Sprint and Iron Horizon preserve their
HD-authored music and mixer behavior. Neither visuals nor samples are reduced.
Level1 music only on Disk1; Stormrail music only on Disk2. Shared reads,
including cold collision/audio/results and title-return dependencies, remain
on each required disk. No extra load at the Stormrail Harrier boundary.

## Disk-only space recovery

Existing SPR1/SPL1 encoders produce: pulse score1704/bank7988 bytes, rail
score3017/bank8317 bytes. All SFX now use the existing checked data decoder
in disk builds, both for original Chip effects/results and scaled Fast mixer
copies. Raw HD reads are untouched. Decompression is completed during loading;
Paula and the mixer always receive normal decoded samples.

READY tables formerly contributed40192 bytes to the executable. Extract exact
bytes from ready_dust_mask.h (no art regeneration), pack to6423 bytes, load
once through assetsLoadDiskData into Fast RAM before publishing READY. Keep
resident across section/title cycles and free at platformClose. Wrong-size,
CRC/truncated data or allocation failure follows the controlled load failure
path. HD retains its original embedded const arrays and exact executable hash.
The decoded table allocation replaces approximately the same embedded table
memory; this is a disk-space change, not a large runtime-memory saving.

New markers SP07M1/SP07M2 prevent silent mixing with alpha.4 SP07D1/SP07D2:
old raw effects are incompatible with the new packed-effect loader. Existing
DF0/DF1 lookup, no-requester polling and visible INSERT flow are unchanged.

## Verification and artifacts

Native disk build succeeds. Full host suite and the additional actual-C
loaded/embedded READY test pass:1600 frames across all12 states, exact bitmap/
history parity, wrong-size failure, load-once and repeated cleanup. Existing
actual-C streaming decoder exercises45 integrity/bounds cases; media tests
cover both old/new marker configurations, DF1, DF0 swap and wrong versions.
Every packed shipped asset is compared with its source via that C decoder.
Both ADFs are read back file-for-file, their bitmaps/free blocks and monotone
payload layout checked, Disk1 boot checksum/code verified, and compiled
references checked separately against each disk (only the opposite track and
section graphics excluded). These are host/native-compile checks, not emulator
boot or physical-drive evidence.

Disk1 SHA256:a9d8408e2ef8340a055fc23c57e4ece7ffee38684d7949de05cd0edfe1e70b0f
Disk2 SHA256:1cb8120397fb0cd5d15cee941e2a2976c0a1e5cfeebb4757d225b011060ffb7a
HD SHA256 remains549f43fce3a8de4187b9ebec20758667f8f016f9131389366d57ced30afeaed2.
Build/source manifest, packed files and per-file block inventory:
`build/multidisk-probe/build.json` and `media.json`. Previous entire probe
including logs/backups preserved as `build/multidisk-before-audio-20260909`.
Sole user test set `dist/Music-Campaign-ADF`, instructions in ReadMe.txt.
Prior accepted-general Stormrail HD drawer archived intact under older-builds.

First native gate:030 boot title/Level1 music, Escape/OPTIONS->Stormrail and
Disk2 music/SFX. DF0 swap waits for INSERT; with Disk2 in DF1 expect automatic
discovery. Results/replay/back-to-title as reached. Then020 functional check;
ADF real-A1200/Pocket and floppy loading/audio quality remain distinct gates.
No automatic FS-UAE, release, SemVer, commit, push or HD compression.
