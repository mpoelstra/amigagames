# Sparkpaw 0.7.0-alpha.2 — Phase 7A.2

Official local all-format checkpoint; source commit/push authorized. No itch
upload performed. Live public downloads are 0.6.0-alpha.68; newest live devlog
is "New alpha 68 version". A stale web cache showed alpha.62, so the live
HTML/detector result is authoritative. Live HTML is retained in
build/checkpoint-alpha2/itch-live.html.

## What is included

Complete Storm Ruins -> Stormrail campaign, carried vitals, resident replay,
ready-menu start-section selection, flight formations/health/debris, Harrier
finale and Stormrail results. Retain tested Level-1 Bob/column/history changes;
no noticeable FPS gain is claimed and performance research stays parked.

HD game bytes remain identical to local alpha.1 and the user-approved build.
Story remains HD/WHDLoad-only. ADF Disk 1 boots title/Level 1; Disk 2 holds
Stormrail/finale/results. Shared files include the collision map on BOTH disks.
Disk 2 in DF1 is detected automatically. Otherwise wait for INSERT before a
DF0 swap. INSERT 1/2 uses the same loading picture, styled text, no helper lines
and no left-mouse cancellation. No per-frame allocation or disk I/O was added.

WHDLoad now compiles the full campaign with F10 hooks and all 48 runtime assets.
The Kickstart slave's old alpha.49 version is corrected; memory/startup mechanism
is unchanged. This new campaign WHDLoad requires native testing; do not inherit
alpha.68 startup/F10 approval. No WHDLoad executable or Kickstart ROM is bundled.

## Manual use and acceptance

- HD: extract ZIP/LHA and launch Sparkpaw from the complete drawer.
- ADF: boot Disk1 in DF0. Put Disk2 in DF1 for automatic continuation, or swap
  DF0 only after INSERT. Keep the active disk present during play/results.
- WHDLoad: extract to the short Sparkpaw-0.7.0-a2-WHDLoad drawer and launch its
  icon with an installed WHDLoad and legal Kickstart 3.1 A1200/RTB. F10 exits.
- PAL A1200/AGA, 68020+, 2 MB Chip + 8 MB Fast remains the minimum.

User explicitly states "de adf zijn goed en insert disk 1 en 2 etc is ook ok".
This is ADF approval in the FS-UAE conversation; CPU was not restated in that
final report. HD 030/020 observations remain recorded in the re-audit. Native
campaign WHDLoad, physical A1200/Gotek timing and Analogue Pocket are open gates.
The intermittent real-Amiga HUD-boundary issue remains open.

## Offline verification

make and make release pass. Full host suite, actual C decoder ASan/UBSan tests,
all packed bitmap parity, per-volume compiled-reference coverage, ADF readback,
boot checksum, forward data allocation, ZIP tests and classic LHa CRC checks
pass. Independent Lhasa extraction matches both HD and WHDLoad staging, including
launcher icons. Amiga path components are <=30 characters. The sole lh0 file
in each compressed LHA is the documented 496-byte incompressible tally sample;
other file members are lh5. Existing compiler optimizer warnings remain.
No automatic FS-UAE run. Verification is not a native timing or memory-peak claim.

HD game SHA256: `e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
WHDLoad game SHA256: `9da874700ffc49e4099506c52b3bd319ff639e9563a3aa0b1c651e17b460e389`.
WHDLoad slave SHA256: `dde09e6998823f883c0843a8d80e1359ffa3e816b9d278590ae12af1fb344e80`.

## Artifact inventory

Six current files plus the same-version extracted HD drawer. Original alpha.68
is explicitly protected and retained separately from the current version;
local alpha.1 and completed disk test drawers are archived byte-identically.

| Artifact | Bytes | SHA256 |
| --- | ---: | --- |
| Sparkpaw-0.7.0-alpha.2.zip | 886173 | `26b9d1361b4d31496a5749b474dad5821debd1f971593af654f9e70255d550fb` |
| Sparkpaw-0.7.0-alpha.2.lha | 892587 | `7716249044b8ef21e83850d20a5c04f38eb9df3d52bd40558ed694aa003e547d` |
| Sparkpaw-0.7.0-alpha.2-Disk1.adf | 901120 | `2d0112944e77ca917328f7f00b1d4fcb7106201ba0f5d554d8f31a88a6bff987` |
| Sparkpaw-0.7.0-alpha.2-Disk2.adf | 901120 | `0e7455b863990298010d4e28966499b3022df6773b764af10677f742de6e0a34` |
| Sparkpaw-0.7.0-alpha.2-WHDLoad.zip | 894217 | `d45ee1de3585e71601e7c99cdac617647b21652c2b090944ae2926946cb822b3` |
| Sparkpaw-0.7.0-alpha.2-WHDLoad.lha | 900017 | `6fabfcf0c5438559e8a93dde41f05babce262f3a06d2d74b2bbe4ae97506753b` |

Detailed verification: build/checkpoint-release-verification.json. No source
ZIP was requested. See CHECKPOINT_ALPHA2_LESSONS.md for retained mistakes and
rules. Public release notes are in RELEASE_NOTES_0_7_0_ALPHA_2.md.
