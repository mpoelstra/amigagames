# Release verification — 0.7.0-alpha.11

26 September 2026. Local alpha release, Phase 7B.1 scenery refinement.
Public itch baseline: alpha.8 (live detector); no upload, commit or push.

## Verified

- `make PYTHON=../.venv/bin/python3`: pass. `make release PYTHON=../.venv/bin/python3`: final run passes after preserving the first insufficient-space ADF attempt.
- Full `make test`: pass. Asset-ownership test now reads the current versioned campaign executable rather than a preserved historical build.
- `tests/test_level1_rear_release.py`: raw/packed loader error paths, cleanup, 2,048 buffer/camera/reset cases, 48-frame decode parity and production hot path with counters compiled out pass under ASan/UBSan.
- Original 79 runtime assets unchanged. New 99,268-byte `l1-electric.bin` exactly matches approved v5 SHA256 `48aaa6f1b816737140e25305a5a4a1ad379a6faf69b2e1e3143a1c257c09df91`.
- Four complete native campaign builds; production flags exclude focused Level1 start, FPS, render diagnostics and loading trace. Native compiler warnings are the existing optimizer-limit and no-effect warnings; legacy Make grouped-target warnings remain.
- `tools/verify_checkpoint_release.py`: independent ZIP/LHA extraction, CRCs, icon metadata, 30-character path components, 75 raw runtime/bank entries and all-file three-ADF readback pass.
- LHA members use classic LHa compression, with only the verifier's permitted incompressible-file exceptions. All three icon variants match the established NewIcons/classic fallback contract.
- WHDLoad normal edition keeps the current 8 MB banked configuration; High RAM retains the separate >=16 MB configuration. High RAM data preload 5,757,914 bytes, reservation plus preload 11,525,082 bytes; these are storage/reservation counts, not native acceptance.
- 179 alpha.10 files and 80 v5-test/evidence files archived byte-identically under `dist/older-builds/alpha10-and-level1-tests-20260926`. Earlier packaging attempts and all older archives retained.

## Costs and acceptance boundary

Level1 adds 96,000 Chip bytes at the native-observed stride, 99,268 Fast bytes
for frames plus small metadata. Standard WHDLoad additionally retains the raw
frame data in the current Level1 bank. At most 5,184 destination bytes / eight
patches / 24 blits were observed in host stress, not a native timing bound.

V5 focused FS-UAE visuals accepted by the user. CPU configuration was not
explicitly supplied. Final release replay on HD, WHDLoad and ADF, minimum PAL
68020 cadence and real-A1200 testing remain pending. The intermittent real-Amiga
HUD-boundary glitch is still open. No emulator was launched by Codex.

ADF space is recovered losslessly with a better host-side SPL1/SPD1 parse and
Shrinkler preset 3; runtime formats are unchanged. Disk free blocks (512 bytes):

Disk 1: 22 (11,264 bytes), Disk 2: 206 (105,472 bytes), Disk 3: 103 (52,736 bytes).
Disk1 passes the existing 16-block packaging floor, but has less than 16 KiB free; future asset growth must revisit the budget.

## Artifacts

| File | Bytes | SHA-256 |
| --- | ---: | --- |
| Sparkpaw-0.7.0-alpha.11.zip | 1628653 | `571d490e7d9fb6a28bba6951f3f7b155872c6d47ce311830c36a9e0ceb18b317` |
| Sparkpaw-0.7.0-alpha.11.lha | 1649297 | `56cc782db477e1c9e0fa6b9dce43d00ccfc18d1620d6d1bb7228486aca49b89a` |
| Sparkpaw-0.7.0-alpha.11-WHDLoad.zip | 1631497 | `cc16e53a31a895f692d76e93a7396fa894478dc21e20d0e645a2770e66959b9f` |
| Sparkpaw-0.7.0-alpha.11-WHDLoad.lha | 1661288 | `71feb153f7a8563e60c84314cfe1654c3deec8693821bcfe5cac77b6c81fbe8f` |
| Sparkpaw-0.7.0-alpha.11-WHDLoad-HighRAM.zip | 1634559 | `fe8ccd8579085db9ec78a74502099d6023ef0546c6904a82f3c209f4f42a1bd2` |
| Sparkpaw-0.7.0-alpha.11-WHDLoad-HighRAM.lha | 1654656 | `19c43ca225887d485e65aaef6155bc4b450e22c51038b25bc011e339e7682977` |
| Sparkpaw-0.7.0-alpha.11-Disk1.adf | 901120 | `83111dcd6a033a9bba747976ff68aa7a3677799f906535b3780e6895dda27cb7` |
| Sparkpaw-0.7.0-alpha.11-Disk2.adf | 901120 | `ff37d4a48630ecaa0fbbad34b733912929311072286e2e4c0d16edc6f0cef9af` |
| Sparkpaw-0.7.0-alpha.11-Disk3.adf | 901120 | `c28543f5159a9022be88d2b3ace29dc00130cbeb37a62d4e9f9e799ccacb9126` |

Full evidence: `build/alpha11-integration/` and `build/release-0.7.0-alpha.11/`.
