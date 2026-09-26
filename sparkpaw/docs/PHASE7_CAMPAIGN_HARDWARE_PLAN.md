# Phase 7 — campaign releases and hardware validation

## 2026-09-26 — alpha.13 Harrier finale integrated

Current local release is 0.7.0-alpha.13 / Phase 7B.1. The user accepted the
Harrier's full pixel-art destruction and louder three-hit cue in focused HD
testing, then requested the complete release and Git publication. The ordinary
campaign includes the one-shot defeat phase before gate opening/results;
HD/WHDLoad SOUNDTEST exposes HARRIER DEFEAT. The ADF retains its no-SOUNDTEST
menu. Full host tests, four native campaign builds, release packaging and
independent ZIP/LHA/three-ADF readback pass. Disk free blocks are 16/184/105;
Disk 1 sits exactly at the 16-block floor. Alpha.12 and the focused test are
archived intact. Public itch downloads remain alpha.8. Exact alpha.13 media,
68020 cadence and real-A1200 playtests remain pending, as does the intermittent
hardware HUD-boundary issue. See `RELEASE_VERIFICATION_0.7.0-alpha.13.md`.


## 2026-09-26 — alpha.12 release: flight controls and complete ReadMe

User explicitly requests new alpha, updated docs and commit/push of all work.
Current release: 0.7.0-alpha.12 / Phase 7B.1. All three sections retained.
Skimmer uses directional Up in both JOYSTICK/JOYPAD, independently of the
on-foot jump mapping. ReadMe for HD and both WHDLoad editions starts with the
user-authored personal note and includes plot, version/content, creator,
contact/itch links and separate walking/flying/keyboard controls.

`make`, `make release`, full host suite and independent checkpoint verifier
PASS. Actual ReadMe readback/parity in all six archives, 75 runtime/bank assets,
icons and all-file three-ADF verification PASS. Nine artifacts and three drawers
are current in dist. Alpha.11 (181 files) and controls-test/evidence (77 files)
archived byte-identically under `dist/older-builds/alpha11-and-controls-20260926`.
Existing ignored builds/evidence remain local; repository changes include the
accumulated WHDLoad work, Level1 studies, art/music sources and release skill.

Public itch still alpha.8 (downloads/devlog checked); notes cover the full delta.
No itch upload or emulator launch. The release and Git publication are explicitly
authorized; native flight confirmation, final-media replay, minimum-68020 cadence
and real hardware remain pending. Intermittent hardware HUD glitch stays open.
See `sparkpaw/docs/RELEASE_VERIFICATION_0.7.0-alpha.12.md` for hashes/budgets;
`RELEASE_NOTES_0.7.0-alpha.12.md` is the canonical English player copy.
Historical entries below retain their original scope and acceptance status.

## 2026-09-26 — alpha.11 local release complete

Approved v5 Level1 ambience integrated and packaged as 0.7.0-alpha.11.
`make`, final `make release`, full host suite and independent checkpoint
verification pass. Nine artifacts and three extracted drawers are current in
dist: HD ZIP/LHA, standard WHDLoad ZIP/LHA, High RAM ZIP/LHA, three ADFs.
All 75 runtime/bank assets, icons and disk file readback verified. Production
omits animation test counters and diagnostic shortcuts. Source and approved
v5 animation data match; prior graphics/audio unchanged.

Alpha.10 (179 files) and v5 test/evidence (80 files) archived byte-identically
under `dist/older-builds/alpha10-and-level1-tests-20260926`. Prior packaging
attempts retained. Public itch still alpha.8; canonical English release notes
cover that full delta. No commit, push, upload or automatic emulator launch.

Disk1 has 22 free 512-byte blocks (11 KiB), passing the existing 16-block floor
but below 16 KiB; this constrains further growth. Disk2/3 have 206/103 blocks.
Details/hashes: `sparkpaw/docs/RELEASE_VERIFICATION_0.7.0-alpha.11.md`.
Focused visuals accepted; final-media replay, minimum-68020 performance and
real hardware remain pending. Intermittent hardware HUD-boundary glitch open.

## 2026-09-26 — alpha.11: approved Level1 electrical ambience

User accepted the fixed-size v5 building light and explicitly requested game
integration and a new alpha release. Candidate identity: 0.7.0-alpha.11,
Phase 7B.1 scenery refinement. Public downloads verified as alpha.8 with the
itch detector; canonical English release notes include the full alpha.8 delta.

Production enables the approved 48-phase sequence in the isolated Level1
renderer: downward tower pulse/crystal response, sixteen sky-discharge sites,
and blinking of exactly five original blue building pixels. Existing palette,
gameplay, HUD and scrolling contracts retained. Inactive rear writes complete
before Copper publication; source bitmap stays immutable. Normal campaign
entry points retained, with no focused-start or diagnostic-save code enabled.

HD loads the raw 99,268-byte frame file. ADF uses the existing CRC-checked packed
asset reader; standard WHDLoad uses that reader's resident bank backend. The
new file belongs only to Level1 (ADF Disk1 and WHD level1 bank), not Stormrail
or Drowned. Extra Chip payload is 96,000 bytes at the native-observed stride;
Fast frame copy 99,268 bytes plus small descriptors. Standard WHDLoad additionally
retains 99,268 bytes in its active raw Level1 bank plus directory overhead.
Loading CPU/storage costs do not establish spare gameplay frame time.

The first ADF packaging attempt ran out of Disk1 space and was preserved under
`build/alpha11-integration/adf-attempt1-preserved`. Host-only lossless SPL1/SPD1
parsing improvements and Shrinkler preset 3 retain the established runtime
formats and exact decoded data. Every packed asset is checked by the real C
reader; final artifact/ADF results are recorded in RELEASE_VERIFICATION below.

User acceptance covers focused FS-UAE v5 visuals; exact CPU configuration was
not supplied. Minimum-68020 cadence, final HD/WHDLoad/ADF replay and real-A1200
verification remain open, as does the intermittent hardware HUD-boundary glitch.
No automatic FS-UAE launch, commit, push or itch upload is authorized/performed.
Earlier entries below retain their historical status.

## Phase 7B.1 — three-section campaign, alpha.9 preparation

The approved HD, WHDLoad and three-disk ADF campaign candidates add Drowned
Turbines after Stormrail, with carried vitals/score, replay, direct section
start, accepted Undertow Circuit v5 music and new Drowned effects. HD/WHDLoad
include the intro and expanded SOUNDTEST; ADF keeps its agreed title start and
no SOUNDTEST. The second-button pull-up and keyboard ACK timing corrections
are compiled into all three new media. Alternative Drowned music studies are
rejected and excluded from runtime. MrDig's controls-only HD regression test
passed on an unspecified configuration; affected hardware has not confirmed
the original bug fixed. OPTIONS causality and 68060 involvement remain open.

`make`, `make release`, full `make test`, independent archive extraction,
74-asset parity, icon checks and all-file three-ADF readback pass. New WHDLoad
and ADF smoke playtests are pending before alpha.9 becomes official. Preserve
alpha.8 and the three approved candidates until then. Real A1200, physical
floppy and the intermittent HUD boundary observation remain open.

## Historical release — 0.7.0-alpha.7, 10 September 2026

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
