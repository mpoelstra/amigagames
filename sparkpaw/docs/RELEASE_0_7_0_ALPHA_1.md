# Sparkpaw 0.7.0-alpha.1 — HD release record

Historical local HD-only checkpoint, superseded by RELEASE_0_7_0_ALPHA_2.md.
The original artifacts remain archived byte-identically.

Local release built 2026-09-05 at the user's request. Public baseline verified
with the release skill's itch detector: 0.6.0-alpha.68; the public page was also
cross-checked. No upload, commit, push or tag performed.

Phase 7A.1 records the first full-campaign release/hardware checkpoint, based
on implemented Phase 6D progression. It does not mark deferred architecture
work complete. Gameplay/runtime code is unchanged by packaging; the release
executable matches the accepted 030/020 e6e20db6... test executable exactly.

## Artifacts and verification

- `Sparkpaw-0.7.0-alpha.1.zip`: 886,141 bytes; SHA256 `c685c991086324b51c3f8b7fcca06a437127d7040b15366c95e34a5775cd28a1`.
- `Sparkpaw-0.7.0-alpha.1.lha`: 892,556 bytes; SHA256 `3535b0493b80191e09cc5842e8648f4e7e763f5d1037c7a921e9e6ac88a38d5b`.

Extracted active review drawer: `dist/Sparkpaw-0.7.0-alpha.1`, run `Sparkpaw`.
The previous complete `Campaign-Play-HD` drawer was archived byte-for-byte as
`dist/older-builds/Campaign-Play-HD-pre-0.7`. Protected original alpha.68 remains
in dist for provenance and existing disk/hardware use; it is not another new
campaign test candidate.

Game executable SHA256:
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
48 assets / 51 file members including executable, ReadMe and Workbench icon.
Source manifest equals all embedded runtime references. ZIP CRC/member tests,
classic LHa CRC and independent Lhasa extraction match the drawer and sources.
All path components fit 30 characters. Icon bytes equal the tested NewIcons/
standard fallback generator; the full host icon test passed.

Explicit compression exception: classic LHa chooses stored `-lh0-` for the
496-byte incompressible tally-tick.raw. Every other file is `-lh5-`; directories
are `-lhd-`. This is recorded rather than claiming all file members compressed.
Changing audio bytes or replacing the trusted archiver to force this tiny file
would add risk without benefit. Its CRC/extracted bytes match the source.

`make`, `make release` and the full host suite passed. Independent artifact
verification: `tools/verify_campaign_hd_release.py`. Output and full SHA256s:
`build/release07-verification.json`. Native user acceptance is inherited only
for the identical executable/assets and tested FS-UAE route; real A1200 launch
of this campaign package is pending. Keep the intermittent real-Amiga HUD
boundary issue open. No performance gain is claimed.

The HD-first scope intentionally replaces the skill's default five-format
release cycle: no legacy one-level ADF/WHDLoad is relabelled as 0.7. ADF/media
planning is authorized, but no campaign disk set is built or advertised yet.
The WHDLoad packager now refuses 0.7 until its campaign route is implemented.

## Real-A1200 test

Copy the complete extracted drawer (or extract ZIP/LHA) to the A1200 hard drive.
Use PAL/AGA with 2 MB Chip and at least 8 MB Fast RAM. Start Sparkpaw from its
Workbench icon or Shell. Check normal boot/loading/charging/ready, Level 1,
results/replay, Continue into Stormrail, Harrier/results, replay and return or
Esc to ready. Observe any missing graphics, audio trouble or HUD-boundary lines.
No diagnostic log/save action is needed. Report machine/CPU and launch route
with the result. ADF, WHDLoad and Analogue Pocket are separate future gates.

## Copy-ready public notes

Title: Sparkpaw 0.7.0-alpha.1 — The journey continues

What's new since 0.6.0-alpha.68:

- Continue from Storm Ruins into the new Stormrail Skimmer interlude.
- Jump aboard, fight through aerial formations, dodge ruin debris and collect
  health and diamonds before the Harrier showdown.
- Complete the interlude with its own results screen and instant replay.
- Carry lives, health and your diamond meter into the next section, or choose
  your starting section from the ready-menu options.

This first 0.7 alpha is the complete HD edition for PAL A1200/AGA, 2 MB Chip +
8 MB Fast RAM. Campaign disk editions and further hardware testing follow.
