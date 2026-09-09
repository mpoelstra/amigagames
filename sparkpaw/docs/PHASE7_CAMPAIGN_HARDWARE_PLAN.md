# Phase 7 — campaign releases and hardware validation

## Current release — 0.7.0-alpha.7, 10 September 2026

Phase 7A.3 now includes audio OPTIONS, five-track/16-effect Soundtest in HD
and WHDLoad, P pause, terminal game over with full AGA art and Storm Light,
centered score/prompt, and HUD carry from Stormrail boarding onward. Normal
fresh/direct starts use three lives; replay restores section-entry vitals.
Two ordinary ADFs retain full image/audio quality using lossless disk packing
and Shrinkler startup compression. Both exceed the 16-KiB free-space reserve.

Build, release packaging, full host suite and independent checkpoint checks
pass. No automatic emulator run. Earlier HD black-after-Fire remains a known
unresolved issue; new ADF/WHDLoad, HUD/audio and real-hardware acceptance are
pending. Do not equate packaging verification with runtime acceptance.

Release set: six alpha.7 HD/ADF/WHDLoad files plus the HD review drawer in dist.
Alpha.5 is archived intact after alpha.7 verification. Current HD/ADF test
variants remain alongside the release per explicit user request. User-authored
itch text, dated development statistics and alpha.5 release-header art have
been preserved. Alpha.7 itch page copy and new release documents are updated.
Public itch still serves alpha.68 (checked live 10 September); no itch upload.

Authoritative details: sparkpaw/docs/RELEASE_0_7_0_ALPHA_7.md,
sparkpaw/docs/RELEASE_NOTES_0_7_0_ALPHA_7.md and
sparkpaw/docs/ALPHA7_ARTIFACT_SHA256.json. The alpha.6 file is a preserved
working draft used as the basis for this new release, not a published package.

## Earlier working notes (historical; current release statement above wins)


## Phase 7A.3: campaign soundtrack release — 9 September 2026

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

## Historical checkpoint records

Alpha.3 checkpoint (7 September 2026): see RELEASE_0_7_0_ALPHA_3.md for the
current artifact set and acceptance matrix. HD READY/menu/transition user
accepted, previous music/dust ADF user accepted; final alpha.3 ADF/WHDLoad
presentation and hardware tests remain open. Disk1 free: 9 blocks; Disk2: 345.
Historical candidate status below does not supersede this checkpoint.

## Phase 7A.1: local HD checkpoint (historical)

0.7.0-alpha.1 packaged the full story, Level 1 and Stormrail campaign from the
accepted e6e20db6... HD executable. It was not uploaded to itch and is now
archived intact under dist/older-builds. Phase 6D remains its progression design.

## Phase 7A.2: all-format alpha checkpoint

0.7.0-alpha.2 packages that same HD game plus the user-approved two-ADF route
and a newly rebuilt campaign WHDLoad. Disk 1 boots title/Level 1 without story;
Disk 2 holds Stormrail/finale/results. Both drives are scanned before prompting.
INSERT uses the original loading picture and styled status strip, with no helper
text or mouse cancellation. Shared assets are deliberately duplicated as needed.

The user explicitly approved ADF functionality and both disk prompts. Ordinary
HD approval is retained. On 2026-09-05 the user reports successful real-Amiga
HD and campaign WHDLoad tests, without enumerating individual F10/intro checks.
A separate HD run with about 1.45 MB free Chip RAM plays Level 1 but crashes
on Stormrail. See STORMRAIL_CHIP_RAM_AUDIT.md for the bounded memory follow-up.
Physical ADF/Gotek, cold timing and Analogue Pocket remain open. Minimum stays
PAL A1200/AGA, 68020+, 2 MB Chip + 8 MB Fast; installed capacity is not free
launch memory. The intermittent physical HUD issue remains open.

Current inventory, hashes and manual instructions: RELEASE_0_7_0_ALPHA_2.md.
Public itch baseline is still alpha.68. Checkpoint commit/push is complete; no further release or commit/push is
authorized by the current maintenance request. Frame-performance research and larger refactors stay
parked. This checkpoint does not declare all proposed architecture gates done.

## Chip2 HD acceptance — 2026-09-06

On 2026-09-06 the user accepted Storm-Chip2-HD.zip on a real A1200 and
requested retaining its optimizations in the main game. The normal development
source, executable and runtime assets already contain Chip1 + Chip2; they are
now the accepted development baseline, not a new release. Stormrail still needs
more than approximately 1.45 MB free Chip RAM; the precise free/largest-block
threshold and native savings remain unmeasured. Individual transition/replay
checks and other media are not inferred from this general HD acceptance.
