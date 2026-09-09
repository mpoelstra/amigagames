# Release 0.7.0-alpha.7 — Phase 7A.3 audio controls and game over

10 September 2026. Based on RELEASE_0_7_0_ALPHA_6.md, which is preserved as
a working draft rather than evidence of a separately packaged alpha.6.
The previous delivered local release is alpha.5. The public itch download
baseline was checked live using the release skill's detector on 10 September:
**0.6.0-alpha.68**. This release is not automatically uploaded to itch.

## Included changes

The complete Storm Ruins -> Stormrail campaign remains the Phase 7A.3 baseline:
boarding, flight formations/rewards, debris, Harrier finale, section results,
carried campaign vitals, resident replay and direct section selection.

Alpha.7 includes the locally developed audio OPTIONS modes (SFX ONLY, MUSIC
ONLY, SFX + MUSIC), all 16 effect previews and five music previews in
HD/WHDLoad SOUNDTEST, plus P pause in both sections. Pause freezes simulation,
animation and elapsed time; music continues. Escape and WHDLoad F10 stay live.
Title/results audio and explicit previews remain independent of gameplay mode.

Last-life defeat is terminal instead of restoring three lives. A shared native
320x256/64-colour game-over scene uses the approved defeated-Sparkpaw V2 art,
centered text in the user-marked space, total campaign score and PRESS FIRE TO
CONTINUE. StormLight.mod is the user's selected unchanged master; its LSP
score/bank occupies 207,919 decoded bytes. Fire returns through the title route.
Storm Light is also the fifth Soundtest track. No diagnostic one-life shortcut
is enabled in release binaries: all fresh normal/direct starts use three lives.

The HUD initially seeds both hidden buffers from the actual campaign state.
Approach/boarding uses Stormrail's carried health, as flight already did.
OPTIONS -> Stormrail remains a fresh 3-life/6-health start; replay restores the
saved section-entry snapshot. This corrects presentation, not campaign banking.

READY text/masks are rasterized offline into a Fast cache. The full menu covers
1,070 states and 4,968 tested transitions; total UI allocation is 284,383 Fast
bytes. ADF selects only its 50 reachable states (328 transitions), with 98,575
Fast bytes. No runtime font renderer or displayed-plane writes were introduced.

## Editions and two-disk capacity

HD/WHDLoad retain intro and Soundtest, ordinary unpacked runtime assets and
all five musical themes. ADF omits intro/Soundtest, as previously agreed, but
preserves the same gameplay and game-over art/music. Both images are standard
901,120-byte DOS1/FFS ADFs, not extended/custom-track disks.

ADF combines the smaller READY table, omission of the unused legacy menu atlas,
existing image packing, exact SPD1 sample differences plus LZSS, and Shrinkler
executable compression. Unpacking occurs at startup/loading, not gameplay.
Storm Light's sample bank is 72,031 stored bytes; Neon Sky's is 70,892. The
existing 4-KiB LZ window is retained; the new delta accumulator adds no full-bank
scratch allocation. Shrinkler reports about 90 kB temporary startup allocation
and zero persistent overhead. Actual machine timing/memory acceptance is pending.

The packager keeps the normal 32-block/16-KiB reserve on each disk. The latest
capacity test retains 73 blocks (36.5 KiB) on Disk1 and 185 (92.5 KiB) on Disk2.
Final release identities and file sizes are recorded below and in
ALPHA7_ARTIFACT_SHA256.json. SP07G1/SP07G2 markers prevent mixing with alpha.5's
SP07M disks. See ADF_COMPRESSION_RESEARCH.md for research, sources and commands.

## Evidence and known issues

The user supplied positive earlier campaign/audio feedback, game-over screenshots
and specific layout/HUD corrections. Those observations do not establish new
ADF/WHDLoad or real-hardware acceptance. The user approved preparing this alpha
and committing/pushing while the following points remain open:

- Earlier HD game-over test reportedly remained black after Fire following a
  long wait. This is unresolved; no fix is claimed. It predates ADF compression.
- Native testing of the latest HUD carry correction and audio/menu paths.
- ADF executable unpack/startup duration, music, DF0 swaps/DF1 detection and
  game-over return from both sections on the newly packed disks.
- WHDLoad startup/audio/F10 and real-A1200, physical floppy/Gotek and Pocket.
- Intermittent real-Amiga HUD-boundary glitch and exact free-Chip launch threshold.

Minimum remains PAL A1200/AGA, 68020+, 2 MB Chip + 8 MB Fast RAM. General renderer
performance research remains parked. No automatic FS-UAE run was performed.

## Verification

`make`, `make release`, full `make test` and `tools/verify_checkpoint_release.py`
passed with `PYTHON=../.venv/bin/python3`. Final ReadMe wording was repackaged
through the standard HD/WHDLoad packaging functions and independently reverified.
ZIP/LHA extraction matches every runtime file; icon layers/default tools and
30-character path limits pass. LHA file members use lh5 except the documented
incompressible 496-byte tally-tick.raw lh0 fallback. All ADF files were read
back and both per-volume dependency sets checked. A successful compile/archive
check never substitutes for native acceptance above.

## Artifact inventory

| Artifact | Bytes | SHA-256 |
|---|---:|---|
| Sparkpaw-0.7.0-alpha.7.zip | 1288927 | `be1844840597602681cc115034da7dc688aa1e8aa98a2997eee93c5e8fdb0100` |
| Sparkpaw-0.7.0-alpha.7.lha | 1308016 | `d563ff4c4e5fdb9b747d1fe170f44e15663646821aa40f4d3dc126738f553161` |
| Sparkpaw-0.7.0-alpha.7-Disk1.adf | 901120 | `ef223f98bf094d6e9de04d12c56014e0d628ea2bea2732160af1366bd17b9d2d` |
| Sparkpaw-0.7.0-alpha.7-Disk2.adf | 901120 | `432fa56e94c65dae5740373db96a75c87c96917bb4a46ea8f708dca9530581fd` |
| Sparkpaw-0.7.0-alpha.7-WHDLoad.zip | 1296826 | `716b2f354970954d0b1307a175b917e81c278493763425a9a61dc9cecce90c8d` |
| Sparkpaw-0.7.0-alpha.7-WHDLoad.lha | 1315309 | `057d116ed342da0a5b1d4148c66c191d3c8a97187a6b9653dede796f93413629` |
