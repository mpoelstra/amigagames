# Sparkpaw 0.7.0-alpha.3 release record

Released 7 September 2026; Phase 7A.2 maintenance checkpoint.
Public baseline verified by fresh itch HTML download filenames and devlog
"New alpha 68 version": 0.6.0-alpha.68. Cached web text was older; direct HTML
and the skill detector agree. Canonical English notes cover the entire delta.

Current checkpoint: **0.7.0-alpha.3 / Phase 7A.2**, campaign presentation and
memory maintenance, released 7 September 2026. All six packages plus the HD
review drawer are in `sparkpaw/dist` (use `dist` from the Sparkpaw directory).
The verified public itch baseline is still **0.6.0-alpha.68**; publication to
itch is separate from this repository release. Older alpha.2 and test drawers
are preserved byte-identically under `dist/older-builds`.

This checkpoint retains the complete Storm Ruins -> Stormrail campaign and
accepted Chip1/Chip2 savings, adds Hero Drive intro music (HD/WHDLoad), Neon Sky
title-through-READY music (also ADF), twelve background wind particles with
orange accents, faster CPU menu copies on 020 and preparation while CHARGING
remains visible. Gameplay/results remain SFX-only. No Fast-RAM Blitter route.

User acceptance: Chip2 on real A1200; music/dust ADF preceding final menu tuning;
HD 020 READY selection/OPTIONS and final CHARGING transition improvements.
Final alpha.3 ADF/WHDLoad presentation, real-hardware music/READY, physical
ADF/Gotek and Pocket remain separate open gates. About 1.45 MB free Chip RAM
is still insufficient for ordinary HD Stormrail; exact threshold is unknown.
The intermittent real-Amiga HUD-boundary issue stays open. No general gameplay
FPS improvement is claimed. Performance research and music-player research
remain parked.

Build, full host suite, ADF loader/decoder/readback and independent ZIP/LHA/icon
checks pass. Disk 1 has 9 free 512-byte blocks; Disk 2 has 345. See
`docs/RELEASE_0_7_0_ALPHA_3.md`, `docs/ALPHA3_ARTIFACT_SHA256.json` and
`docs/RELEASE_NOTES_0_7_0_ALPHA_3.md` under Sparkpaw for exact scope and hashes.
No routine automatic FS-UAE tests. No retest request for a proven byte-identical
restoration of a user-tested build. Keep at most one future active full test set.

## Engineering scope and lessons

- Chip1 trims unused Stormrail bitmap rows; Chip2 skips four Level-1 graphics
  reads and six unused caches. Estimated combined Chip saving about 228 KiB;
  actual free/largest-block savings are not a measured hardware result.
- Intro/title banks never overlap. Hero Drive uses 190812 Chip bytes and 5752
  Fast bytes; Neon Sky uses 142796 Chip bytes and 7625 Fast bytes. Music stops
  and frees before gameplay. LSP fixed-50-Hz conversion/replay retained.
- READY reuses existing hidden Chip buffers. Read-only background/current-menu
  masks total 40192 bytes; CPU span table is 29952 bytes. Small menu changes
  copy 1320 instead of 14976 bytes; page changes use unrolled 32-bit CPU copies.
- Rejected Fast2: menu patch sources are MEMF_FAST and cannot feed Blitter DMA.
  Check both source and destination memory ownership, not merely Blitter state.
- Move expensive preparation before fade-out; only the displayed bitmap copy
  waits until black. This shortens black duration, not total loading work.
- ADF keeps existing lossless SPL1/SPR1 packing, including title music. No HD
  disk compression added. Default 32-block reserve remains for probes; release
  explicitly requires at least one block because this pair has only 9 on Disk1.
- Full host tests, actual 144-transition C parity, 1600 dust frames, 45 packed
  loader cases, per-volume ADF dependencies and all-file readback pass.
- Both LHA archives use lh5 except classic LHa's documented 496-byte tally-tick
  incompressible lh0 fallback. Independent Lhasa extraction, icons, archive
  path lengths and source-byte parity pass. See JSON for all six sizes/hashes.

## Artifact inventory

- `Sparkpaw-0.7.0-alpha.3.zip`: 1088465 bytes; SHA-256 `dfbebd2b425b9ecec6c42c71075f1d4328a6ac1258167b985c67f3af5c9ce2f1`
- `Sparkpaw-0.7.0-alpha.3.lha`: 1101862 bytes; SHA-256 `dc88f0264ef507ea2e5bbe16da62315f5caed62a260d0a4d2beba90143d465fd`
- `Sparkpaw-0.7.0-alpha.3-Disk1.adf`: 901120 bytes; SHA-256 `64d04532aeebdd1cd4cc380d451b5d4b9336eab32f5ea6af7069763126a0689c`
- `Sparkpaw-0.7.0-alpha.3-Disk2.adf`: 901120 bytes; SHA-256 `7325c787db93f6436a1a0d9d855c2a29ac6a706f9be8cbca848b70dd59924ea4`
- `Sparkpaw-0.7.0-alpha.3-WHDLoad.zip`: 1096530 bytes; SHA-256 `51e6a16b62fbfe5294bcce0550d66ea7530b729688fef9b80b761e445d1dcef2`
- `Sparkpaw-0.7.0-alpha.3-WHDLoad.lha`: 1109308 bytes; SHA-256 `89d554ad4e98d008b289c7f7db734fca879537a283c655b3f9dd361b687bb571`
