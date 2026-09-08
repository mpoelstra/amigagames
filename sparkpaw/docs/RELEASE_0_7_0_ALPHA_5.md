# Release 0.7.0-alpha.5 — Phase 7A.3

Current checkpoint: **0.7.0-alpha.5 / Phase 7A.3**, released 9 September 2026.
The sole current release is the six HD/ADF/WHDLoad packages plus HD review
drawer in `sparkpaw/dist`. The public itch download baseline, checked live,
is **0.6.0-alpha.68**; this release has not been published to itch.

Intro Hero Drive and title-to-READY Neon Sky retain four-channel LightSpeedPlayer
playback. Level 1 now plays Copper Sprint (164 BPM); Stormrail plays Iron Horizon
(172 BPM). Three music channels use CIA-timed ProTracker replay; the fourth
Paula channel carries two mixed effect voices, one reserved for plasma and one
priority-managed for the other effects. All 16 existing effects remain supported.
Results retain their original sound effects. General gameplay-performance
research stays parked; no renderer or gameplay redesign is part of this release.

HD/WHDLoad contain the cinematic intro; the two ordinary 880-KiB ADFs start at
the title. ADF-only lossless packing of audio and external READY masks leaves
15 KiB free on Disk 1 and 159 KiB on Disk 2. Assets are unpacked before use,
not during gameplay mixing. HD/WHDLoad assets remain unpacked.

Evidence: repeated user 68020/68030 audio/gameplay trials were positive; the
integrated HD tracks and two-ADF candidate also received positive user reports.
The final HD executable matches the accepted Stormrail candidate. Builds, full
host suite, ADF decoder/readback, independent archive extraction and icon checks
pass. New WHDLoad audio/F10 and final real-hardware, physical floppy/Gotek and
Pocket acceptance remain open; compilation is not runtime acceptance.
Target remains PAL A1200/AGA, 68020+, 2 MB Chip + 8 MB Fast RAM. The intermittent
real-Amiga HUD-boundary issue and exact free-Chip launch threshold remain open.

Next step: test these alpha.5 packages on real hardware and record platform-
specific findings. No routine automatic FS-UAE tests or further microbenchmarks.
See `sparkpaw/docs/RELEASE_0_7_0_ALPHA_5.md` for inventory, hashes and evidence,
and `sparkpaw/docs/RELEASE_NOTES_0_7_0_ALPHA_5.md` for the full English itch delta.
Superseded releases and test drawers are preserved intact in `dist/older-builds`.

## Verification and artifact identity

`make`, `make release`, the full `make test` suite and
`tools/verify_checkpoint_release.py` passed with `PYTHON=../.venv/bin/python3`.
The independent verifier extracts ZIP/LHA, compares every file, checks both
Workbench icon layers and reads back all ADF files through the disk layout.
Classic LHa emits lh5 file members except the documented incompressible
496-byte tally-tick.raw lh0 fallback. Archive paths pass the 30-character guard.
The two 901120-byte DOS1 disks have 30 and 318 free 512-byte blocks respectively.
No automatic FS-UAE run was used for release validation.

WHDLoad was rebuilt with the shared gameplay backend and updated alpha.5 slave.
Its native startup, audio and F10 restore remain a separate acceptance gate.
The preserved HD acceptance cannot establish those WHDLoad-specific outcomes.
ADF regeneration may change filesystem metadata and therefore whole-disk hashes;
file payload checks, not a claim of identical disk bytes, establish package parity.

The live itch download page and newest devlog were checked on 9 September:
all downloads name alpha.68; “New alpha 68 version” is dated 30 August 2026,
18:49 UTC. A stale web-tool page showed alpha.62 and was rejected as baseline.
The fetched HTML is preserved locally in build/checkpoint-alpha5/itch-live.html.
Public devlog: https://mrdig.itch.io/sparkpaw/devlog/1646318/new-alpha-68-version

## Audio and memory

Paula channels 0–2 replay the three-channel gameplay MOD; channel 3 receives
an approximately 11 kHz mix of two effect voices through two 112-byte buffers.
CIA timing separates musical tempo from render cadence, but interrupts and
mixing still consume CPU time. The block mixer avoids per-sample work for
silence and single-voice spans. The isolated 020 proof measured service fraction
falling from 13.32% to 3.06%; this is not an integrated worst-case FPS guarantee.

Only one section's score/bank is resident. Additional main-game allocations
are approximately 11.3 KiB sample bank plus 224 bytes in Chip, and 56 KiB Fast
for Level 1 or 64 KiB for Stormrail, excluding additional code. Legacy effects
remain available for results. ADF additionally keeps 40192 bytes of READY masks
in Fast; their packed disk form is 6423 bytes. No HD disk compression was added.

## Artifact inventory

Sizes and SHA-256 values below refer to the already delivered release bytes.
ALPHA5_ARTIFACT_SHA256.json also records every file in the HD review drawer.

| Artifact | Bytes | SHA-256 |
|---|---:|---|
| Sparkpaw-0.7.0-alpha.5.zip | 1112799 | `5fa48e03ef4e790848b7eb1aa57b151b8f4024e95c76bf904191cfb202e75d7a` |
| Sparkpaw-0.7.0-alpha.5.lha | 1125218 | `7c4a958af996baa58b7c9e4b03f0b07e75a5ff7023876f4fc87fee887ad73f46` |
| Sparkpaw-0.7.0-alpha.5-Disk1.adf | 901120 | `df10b257e392c5b7d12cf05794e02098d3f2fb4b4f42d1c6ab7c9eab899916df` |
| Sparkpaw-0.7.0-alpha.5-Disk2.adf | 901120 | `6c664ea59f30c042ce2263f27a3d2190f2143ec755aa4fd7ffb6df4326fdfbb8` |
| Sparkpaw-0.7.0-alpha.5-WHDLoad.zip | 1120914 | `c565f51db6aac34469f99efe4b114fc98e0bb70984037bf17b13ea3f9b787cd1` |
| Sparkpaw-0.7.0-alpha.5-WHDLoad.lha | 1132681 | `dc51dbc793d7b6913f39453f5df91c123149b02ad5eca130c2d96dc0df5ee47d` |
