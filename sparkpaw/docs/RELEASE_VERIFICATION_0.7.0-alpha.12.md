# Release verification — 0.7.0-alpha.12

26 September 2026. Phase 7B.1 campaign refinement. The user explicitly
requested the release, documentation update, commit and push of all work.
Public itch downloads and latest devlog still show alpha.8; checked with the
live detector and public page. No itch upload is included.

## Changes since local alpha.11

- Skimmer directional Up now works in JOYSTICK and JOYPAD. Flight no longer
  routes Up through the on-foot jump selector. Button 2 still jumps on foot
  in JOYPAD and does not steer the ship.
- HD, standard WHDLoad and High RAM ReadMe files begin with the supplied
  personal note, retaining its wording, attribution and email. General game,
  plot, current campaign, MrDig Productions, links, controls and edition-specific
  installation sections follow. Only ASCII formatting/wrapping changes the note.
- Release skill and verifier now check completeness and the actual ReadMe
  contents in all six ZIP/LHA archives. Skill also reflects the current three
  disks and two WHDLoad profiles.

## Verified

- `make PYTHON=../.venv/bin/python3`: PASS.
- `make release PYTHON=../.venv/bin/python3`: PASS, four native campaign variants.
- Full `make test PYTHON=../.venv/bin/python3`: PASS. Actual input C coverage
  includes both modes, 1,024 directional register values per mode, button-2-only
  rejection during flight, independent keyboard input and on-foot jump mapping.
- `tools/verify_checkpoint_release.py`: PASS. Independent extraction/CRC of
  ZIP/LHA, all files on three ADFs, 75 raw asset/bank entries, icons, short path
  components and trace-free release flags verified. HD has 80 packaged files,
  standard WHDLoad 11, High RAM WHDLoad 81.
- Actual packaged ReadMe inspected. All editions match their generated text;
  the opening note, version/edition, required sections, contact/itch links and
  included ReleaseNotes reference pass. Text is ASCII and <=80 columns.
- WHD bank dependency harness: 15/14/17 reads for sections 1/2/3, zero missing.
- Existing native compiler optimizer-limit/no-effect warnings and legacy Make
  grouped-target warnings remain; no new build error.
- 181 alpha.11 release files and 77 controls-test/evidence files archived
  byte-identically under `dist/older-builds/alpha11-and-controls-20260926`.
  All older archives, build records and local evidence retained.

## Native acceptance and memory

No FS-UAE instance was launched. Focused Level1 v5 visuals were accepted earlier.
The flight correction has host proof; the user has not supplied a new native
flight test result. Final HD/WHDLoad/ADF replay, minimum-68020 cadence and real
hardware acceptance remain pending. The intermittent real-Amiga HUD-boundary
visual issue remains open. Release authorization does not close these gates.

Target: PAL A1200/AGA, 68020+, 2 MB Chip and 8 MB Fast; High RAM WHDLoad needs
16 MB Fast. Level1 animation retains the prior 96,000 extra Chip bytes observed
natively, 99,268 Fast frame bytes plus metadata, and the additional raw-frame
copy in standard WHDLoad's active Level1 bank. No new animation allocation.
High RAM data preload is 5,757,862 bytes; reservation plus preload is 11,525,030
bytes. These are budget counts, not a hardware startup or performance test.

ADF free space (512-byte blocks):

Disk 1: 22 (11,264 bytes), Disk 2: 206 (105,472 bytes), Disk 3: 103 (52,736 bytes).
Disk1 passes the existing 16-block floor but remains below 16 KiB free.

## Artifacts

| File | Bytes | SHA-256 |
| --- | ---: | --- |
| Sparkpaw-0.7.0-alpha.12.zip | 1631151 | `06ad1bd2915bc182b51e82d266dfa68aabc36f40c5a5857d83438babbf9a833d` |
| Sparkpaw-0.7.0-alpha.12.lha | 1651887 | `870b048c0ae50d6722ce49755577857226d931fc609b5b8910b9e4719d2c53df` |
| Sparkpaw-0.7.0-alpha.12-WHDLoad.zip | 1633662 | `bfbb0449e5d8444b15eb9c76d561db427397f9c8805a00486c0ec4c9f633479e` |
| Sparkpaw-0.7.0-alpha.12-WHDLoad.lha | 1663512 | `48d2f09bb805eafbe14bbf84a72ccb0985360a91cba48ac9eb331fca6c7b0b11` |
| Sparkpaw-0.7.0-alpha.12-WHDLoad-HighRAM.zip | 1636851 | `71652538accd9989f44816aa1429239101c1e347cad8f6adebc9ab0af7eac5e2` |
| Sparkpaw-0.7.0-alpha.12-WHDLoad-HighRAM.lha | 1657004 | `20d79b327a63f6a5a9b914b6cc743914665a17fc68499e3df40528fbe031c8dd` |
| Sparkpaw-0.7.0-alpha.12-Disk1.adf | 901120 | `7a012bed914ff9f4a8b52252ef0109f25d4ce0f9101b8d6440252fbada434f1b` |
| Sparkpaw-0.7.0-alpha.12-Disk2.adf | 901120 | `64125cbfab23b6e5e23dde29ccef859159b229900455d0dc17d61cbbb75d5afa` |
| Sparkpaw-0.7.0-alpha.12-Disk3.adf | 901120 | `a1a2f434cf7904ffa31012a817b235d3a6fa8024098f5eb96e1c859106662d8a` |

Evidence: `build/alpha12-release/` and `build/release-0.7.0-alpha.12/`.
