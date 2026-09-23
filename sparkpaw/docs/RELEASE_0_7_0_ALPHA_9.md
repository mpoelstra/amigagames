# Sparkpaw 0.7.0-alpha.9 — Phase 7B.1

Prepared 23 September 2026 from `main`. Public itch downloads were checked
and remain 0.7.0-alpha.8. Uploading to itch.io is outside this release task.

## Scope and evidence

This checkpoint integrates the approved Level 1, Stormrail and Drowned
campaign across HD, WHDLoad and three ADFs. Drowned uses the accepted v5
Undertow Circuit score and bank, unchanged from the approved candidates.
Alternative music studies were rejected and are absent from runtime media.
HD/WHDLoad add Pump Shot and Checkpoint to SOUNDTEST; ADF retains no intro or
SOUNDTEST. The approved three-disk INSERT DISK 3 art is retained.

All media compile the port-2 POTGO `$f000` pull-up after `Disable()` and the
keyboard ACK wait for three raster transitions, sampling the first line after
asserting ACK. MrDig played the HD controls-only executable successfully on
an unspecified configuration and noticed no change. The alpha.9 HD executable
is byte-identical to that played executable. This is a regression playtest,
not confirmation of the original users' bug: MrDig could not reproduce it
beforehand. Affected users have not verified a fix. OPTIONS and a possible
68060 relationship remain unproven.

The three earlier campaign candidates were user-approved in FS-UAE. MrDig
subsequently reported that the corrected alpha.9 WHDLoad and ordinary HD
work, and the rebuilt ADF recognizes Disk 3 in DF2/DF3 in FS-UAE. Exact
configuration details were not supplied. Real A1200, physical floppy
and the intermittent HUD-boundary observation remain unverified/open.
The first alpha.9 WHDLoad package was played and rejected: its title and
loading screens flickered and loading took longer than in the approved
campaign candidate. Its slave requested 5.5 MB ExpMem rather than the
accepted packed candidate's 3.5 MB. The failed drawer and ZIP/LHA are
preserved byte-identically under `dist/older-builds/` with a hash inventory.
The corrected slave requests 3.5 MB and differs from the accepted slave only
in the version text; its 74 packed assets are identical. The user then
reported the corrected WHDLoad works in FS-UAE; the memory difference
explains the earlier observed emulator regression.

## Build and package verification

`make PYTHON=../.venv/bin/python3`, `make release
PYTHON=../.venv/bin/python3`, full `make test PYTHON=../.venv/bin/python3`
and `tools/verify_checkpoint_release.py` pass. The previously failing
`test_drowned_patch_stage.py` extraction and a second stale Stormrail test
harness were repaired; production behavior did not change for those tests.

The HD and WHDLoad ZIP/LHA files were independently extracted and compared
byte-for-byte with their versioned drawers. All 74 HD assets match the
approved HD candidate; all 74 packed WHDLoad assets match the approved WHD
candidate. Both icon layers and Amiga-safe path names passed. Classic LHa uses
`-lh5-` for compressible members; the already Shrinkler-compressed WHDLoad
executable, `collect-spark.raw` and tiny `tally-tick.raw` use `-lh0-` fallback.
CRC and extracted-byte checks pass for each. Each ADF was read back file by
file, with 42/180/76 free blocks. Relative to the accepted ADF candidate,
only the Disk 1 executable's file bytes changed; all files on Disks 2 and 3
are identical. DF0 swap/DF1–DF3 resolver and Disk 3 typography checks pass.
MrDig observed that the first alpha.9 ADF Disk 1 did not recognize Disk 3
in FS-UAE DF2 until moved to DF0 or DF1. This matched its two-drive scan.
At his request the resolver now scans DF0–DF3; the old Disk 1 candidate is
archived with its hash. The host test covers marker selection and asset reads
from DF2/DF3. MrDig agreed to try both drives and reported "ja werkt" for
the rebuilt ADF in FS-UAE. Record this as successful Disk 3 discovery and
Drowned startup on the tested emulator configuration (details unspecified),
not as real-hardware or physical-floppy verification.
The new Disk 1 ADF was built and independently read back, with 42 free blocks;
Disks 2/3 still pass full readback and keep their prior hashes.
`make test PYTHON=../.venv/bin/python3` suite passes after the DF2/DF3 source
change, including the expanded media resolver test. During play the general
release command stopped at its protective WHD drawer comparison because
FS-UAE changed `Sparkpaw.info` and added a `.uaem` file. Once FS-UAE stopped,
the played drawer and sidecar were archived with hashes, a clean drawer was
restored, and the full release build plus independent verification passed.
The release verifier now asserts the packed slave's `$380000` ExpMem field.

## Versioned artifacts

| File in `dist` | Bytes | SHA-256 |
| --- | ---: | --- |
| `Sparkpaw-0.7.0-alpha.9.zip` | 1,611,314 | `ad7f1cf64c0832386ddb0f6159d8f1ff3c7b70470c8b539699898f2424a478e6` |
| `Sparkpaw-0.7.0-alpha.9.lha` | 1,632,109 | `6381c1fee3645a8e632d6595b6d23be3c0568b36aff1820129e17a17b7b0e697` |
| `Sparkpaw-0.7.0-alpha.9-WHDLoad.zip` | 1,529,650 | `71298b1c5107c060f5bca48175806f223a9f58866ff4373b1795997eee16e1b9` |
| `Sparkpaw-0.7.0-alpha.9-WHDLoad.lha` | 1,555,997 | `ebbf7589799f164ce7e02761d7349b9d9a0481e02a6e7b2fdd3f082a474be3fb` |
| `Sparkpaw-0.7.0-alpha.9-Disk1.adf` | 901,120 | `886b8166f6e8f31e3145912102b12745e0ad9bc7e1789410cad402aa9d55c9be` |
| `Sparkpaw-0.7.0-alpha.9-Disk2.adf` | 901,120 | `2ef696adc265569a11584bc4ce768613e5acac0c9e02f0e50e3517377d5d615c` |
| `Sparkpaw-0.7.0-alpha.9-Disk3.adf` | 901,120 | `d328468ec7d677c04647da94ec00181f3e0f638ed0a2a16e68fd519b5928e3dd` |

The extracted review drawers are `Sparkpaw-0.7.0-alpha.9` and
`Sparkpaw-0.7.0-a9-WHDLoad`. Alpha.8 (68 files) and the three approved
campaign candidates (159 files) were archived hash-identically in
`dist/older-builds`. Root `dist` contains only the alpha.9 release set.
