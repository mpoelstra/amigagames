# Sparkpaw current status and next work

## 2026-09-26 — alpha.13 release complete locally; media playtest pending

The user accepted the focused Harrier destruction and final loud
boom-boom-BOOM cue, then requested normal-game integration, an alpha release,
documentation, commit and push. Current local version is 0.7.0-alpha.13,
Phase 7B.1. All three campaign sections, controls, HUD, scrolling, music,
pause, gate and results paths remain. The approved defeat art/sound are in
the ordinary HD, standard/High RAM WHDLoad and three-ADF builds. HD/WHDLoad
SOUNDTEST now selects HARRIER DEFEAT and loads/releases its Chip preview only
on demand; ADF has no SOUNDTEST.

`make`, full `make test`, `make release` and the independent checkpoint verifier
pass. All six ZIP/LHA ReadMe files, 76 WHD bank assets and every file on each
ADF read back; Disk free blocks are 16/184/105. The initial 13-block ADF
packaging attempt was retained; selected large bitmaps now use a wider host
lossless match search with unchanged Amiga decoder and exact decoded bytes.
Nine alpha.13 artifacts and three drawers are in `dist`. Alpha.12 (181 files)
and the final focused test (78 files) were archived intact. Public itch still
offers alpha.8; no upload or Codex FS-UAE launch. The accepted focused HD
feedback does not prove these exact release media, 68020 cadence or real
hardware. The intermittent hardware HUD-boundary issue remains open.
See `RELEASE_VERIFICATION_0.7.0-alpha.13.md` for budgets and hashes.


## 2026-09-26 — Harrier defeat SFX body revised after user listening

The user reports the previous cue remained quieter than Sparkpaw's shot, even
in SFX ONLY. This was supported by the PCM: its first 50 ms measured about
11.4 raw RMS versus 59.2 for the shot; the two lead impacts decayed rapidly.
The new 1.24 s / 13,672-byte sample uses stronger first and second impacts,
a sustained final blast, more audible upper body and soft saturation with a
short fade. Measured raw 50 ms RMS is 63.0 / 72.3 / 84.0 at the three onsets;
the final blast is still about 45.0 RMS at 0.7 s. Peak is 122/127 without
hard clipping. The existing 120/128 music-mixer gain and one-shot request
remain unchanged. Paula SFX ONLY plays the raw sample at hardware volume 64.
Compared with the prior cue, each Stormrail-only Chip fallback and Fast mixer
sample grows by 1,324 bytes; no extra voice, IRQ or per-frame Blitter work is
added. The 1.24 s sound fits within the 64-tick / 1.28 s defeat phase. These
host measurements do not prove perceived loudness on FS-UAE or real A1200.
The new cue is staged in `dist/Harrier-Death-030-HD/Harrier-Test`; the prior
candidate is intact in `dist/older-builds/Harrier-Death-030-H-old-193747`.
The focused art/sample/lifecycle, audio-mixer and asset-ownership tests pass,
as does the 68020-target native compile. Staging verified 76 runtime assets,
61 executable references and all 181 alpha.12 release files unchanged.
Staged executable SHA-256 remains
`feabf251d362e160af178361e4d9fd1f22978dc474ed477fce6510c24bd46814`;
new raw cue SHA-256 is
`2b58180c43a5bed750df445957166f3a283fdb880fe42ff3e223cf5803a7e9`.
No FS-UAE launch, release, commit or push. Ask the user for a short comparison
in MUSIC + SFX and SFX ONLY before claiming audio acceptance.


## 2026-09-26 — Harrier defeat correction after first user screenshot

The user's FS-UAE screenshot shows a straight left edge on the fire and the
user reports the boss cue is barely audible.
The screenshot is preserved with provenance at
`testresults/Phase 7B.1-rejected-harrier-defeat-left-edge-fs-uae-030.png`.
The flame generator had a fixed
left X limit of 20 native pixels; it now renders down to X=4. The native crop
grows from 96x64 to 112x64, with the same hull anchor, while the preview-only
gate art starts outside the cropped sprite. The gate clamp remains in force.
The 13 frames now occupy 66,560 program/Fast bytes and one 5,120-byte Chip
stage (+8,320 and +640 bytes respectively); worst-case masked draw/restore
traffic is about 24 KiB per visible frame and the once-per-five-ticks stage
copy is at most 50 KiB/s. These are traffic estimates, not native timings.

The 12,348-byte cue is regenerated with peak 124 instead of 104;
the Stormrail mixer gain rises from 64/128 to 120/128. Combined nominal
amplitude is about 2.24 times the first test (~7 dB). On cue start, a
remaining shot voice is retired so the two signed 8-bit voices cannot wrap
through the louder boom. Fallback Paula gain remains at its hardware maximum
64 and benefits only from the sample gain. The 68030 screenshot proves only
the reported single-frame visual defect, not timing or audio quality. A new
user playtest is pending; minimum 68020 cadence remains unverified.
The corrected 68030 drawer is staged at `dist/Harrier-Death-030-HD/`.
The previous 96x64 drawer was archived intact as
`dist/older-builds/Harrier-Death-030-H-old-192502`. The full host suite and
68020-target native compilation pass. Staging verified 76 runtime assets, 61
executable references and all 181 alpha.12 release files byte-identical.
The staged executable SHA-256 is
`feabf251d362e160af178361e4d9fd1f22978dc474ed477fce6510c24bd46814`;
the runtime cue SHA-256 is
`42a22fb1d49fef83907dc4981ca44cc65fe279e1e0b7733278fd160027a19430`.
No FS-UAE launch, release, commit or push.

## 2026-09-26 — Harrier defeat candidate staged for first user gate

Approved v4 visual direction and v5 higher-pitched three-hit SFX are integrated
in an unnumbered, direct-to-finale HD candidate:
`dist/Harrier-Death-030-HD/Harrier-Test`. The 64-tick defeat phase follows the
one-time score award, retires both shot pools, blocks damage/attacks/control,
keeps the gate closed, then enters the unchanged 48-tick gate opening and exit
to results. Escape/new-game init resets the phase; main's P pause stops its
simulation clock. The sound request occurs once on phase entry. Other level
paths retain their original art/audio selection.

The animation stores 13 prebuilt masked 96x64 frames (58,240 bytes program/Fast
data), copies one 4,480-byte frame to Chip staging at most every five PAL ticks,
and draws/restores one Bob over four colour planes on each active frame. The
upper bound is four masked plane draws plus four plane restores per visible
frame, over at most seven 16-pixel words by 64 rows. Source/destination traffic
is roughly 21 KiB per such frame, and stage refresh is at most 43.75 KiB/s
during the 1.2-second visible portion; these are transfer counts, not measured
CPU/Blitter duration or proof of spare PAL time. The
12,348-byte 11,025 Hz sample is loaded into Chip for the fallback Paula path
and Fast for the music mixer only in Stormrail. Added raw storage is 70,588
bytes plus code/packaging overhead; native CPU and Blitter duration is not yet
measured. Baseline alpha.12 Disk2 had 206 free 512-byte blocks, but ADF fit is
not claimed for this HD candidate. Minimum remains PAL A1200/AGA 68020,
2 MB Chip/8 MB Fast, subject to the later user cadence gate.

Native campaign and focused direct-start compiles pass. The full host
regression suite and the new frame/sample/lifecycle proof pass. Staging verified
76 declared runtime assets, 61 literal executable references and all 181
alpha.12 release files byte-identical. Missing unchanged Drowned source assets
were sourced from the current extracted release by the staging tool. No FS-UAE
launch, release, commit or push. First user test: PAL 68030 visual/function;
68020 cadence remains pending.

The first 80x64 staged draft clipped the blast edge and was moved intact to
`dist/older-builds/Harrier-Death-030-H-old-180928` by the stage helper. The
active 96x64 drawer is the corrected version. Its X origin stops at the closed
gate, avoiding new gate-row repair traffic.
Final readback matches the built executable and generated source sample
byte-for-byte. Active executable SHA-256 is
`2eb4c8de306a88c03cd38ebb839610111278764faf4752d1e98801dd1c6811bb`;
SFX SHA-256 is
`fbb2572e4cd205d128234796f6e91cfc37afb1ded9f8810435f7c18c03c7d625`.

## 2026-09-26 — Harrier defeat preview awaiting approval

User approved v4 visuals. The sound felt too low/out of place, so v5 changes
only the preview SFX: higher pitch on all three impacts, a brief upper harmonic
for attack definition, less noise and a shorter brighter electrical tail.
`build/harrier-defeat-preview-v5/harrier-defeat-preview.wav` is pending user
listening; the v4 visual remains approved. No game/test build yet.

User accepted v3 as the right visual direction but requested a fuller explosion
and a cleaner three-hit "boom boom BOOM" with deeper bass. V4 preview at
`build/harrier-defeat-preview-v4/` expands the irregular fire plumes while
preserving the ruptures and residue. Its revised original sound is 1.12 s,
11,025 Hz mono 8-bit: 12,348 raw bytes with peak 104/127 and no digital
clipping in the generated sample. User review of v4 is pending. Runtime,
native audio playback and 68020 costs remain untested.

User also found v2's main blast too vector-like; v3 keeps the accepted opening
and residual tail but builds the central fire from irregular native-resolution
pixel clusters and shards cropped from the Harrier sprite. Review GIF:
`build/harrier-defeat-preview-v3/harrier-defeat-preview.gif`. Sound unchanged.
No runtime work or test drawer yet.

User found the first preview's small initial ruptures acceptable but the later
light-ball finish insufficient. Revised visual preview v2 is under
`build/harrier-defeat-preview-v2/`: a 64-tick localized layered fireburst,
flying armour fragments and fading sparks. Its sound file currently repeats
the first preview for comparison; audio approval also remains open. No runtime
integration or test build has been made.

Release remains 0.7.0-alpha.12. A 52-PAL-tick offline concept preview shows
three localized hull ruptures, a central burst and sparse fading fragments;
an original 11,025 Hz mono 8-bit sound preview lasts 0.96 s (10,584 raw bytes).
Files: `build/harrier-defeat-preview/`; reproducible preview source:
`tools/preview_harrier_defeat.py`. Neither preview is integrated into the game.
The current lethal hit awards score once and enters the 48-tick gate opening
immediately. Candidate integration must add a bounded defeat phase, retire
shots, preserve pause/restart/results contracts, and prove 68020/Chip/Blitter
costs before acceptance. User visual/audio approval is pending. No native test
drawer, release, commit, push or FS-UAE run was made.

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

## 2026-09-26 — User personal note opens player ReadMe

Added the supplied user-authored personal note verbatim to
`docs/PERSONAL_NOTE.txt` and made `tools/game_readme.py` prepend it to all
HD/WHDLoad ReadMe variants. Only line wrapping, whitespace and plaintext
email-link formatting change; spelling and attribution are intentionally
preserved. Refreshed three review copies and the active controls-test ReadMe;
previous text copies saved under `build/controls-readme-20260926/before-personal-note`.
Verified opening words match the source, ASCII and <=80-column layout.
Numbered release packages remain unchanged.

Updated `.agents/skills/ship-sparkpaw-checkpoint/SKILL.md` to require complete
player ReadMe content and actual ZIP/LHA ReadMe readback/parity for all three
editions, including the personal note, plot, creator/contact/itch links,
current version, controls and edition-specific installation requirements.
No new build/release/commit/push; flight-controls native review still pending.

## 2026-09-26 — ReadMe refresh and JOYPAD flight correction

User requests general player ReadMe text and identifies Up not steering the
Skimmer in JOYPAD mode. Fixed playerReadFlightInput to read directional Up
regardless of platforming jump mode; button 2 no longer steers the ship.
On-foot exclusive JOYSTICK Up / JOYPAD button-2 jump mapping retained.
Actual input C tests pass, including both modes, 1,024 directional register
values each, keyboard Up, held fire and second-button-only negative case.
Complete native HD campaign compiles; 75 assets/references staged and 181
current release files unchanged.

`tools/game_readme.py` now supplies all three release ReadMe variants with
general game description, accepted Archivolt plot, current campaign features,
requirements/install instructions, separate walking/flying controls, MrDig
Productions credit, Codex collaboration and official itch/profile URLs. Plain
ASCII, <=80 columns. `make_campaign_release.py` consumes this shared source.
Review copies: `build/controls-readme-20260926/ReadMe-HD.txt`,
`ReadMe-WHDLoad.txt`, `ReadMe-WHDLoad-HighRAM.txt`.

Unnumbered candidate: `dist/Controls-Readme-030-HD/Sparkpaw`. Use READY OPTIONS
CONTROL=JOYPAD, start section Stormrail, fly all four directions and shoot for
20 seconds; repeat JOYSTICK, optionally check on-foot jump mapping. No logging
or diagnostic save gesture. Native feel remains pending. Existing alpha.11
release archives are preserved; refreshed ReadMe text and control fix have not
yet been published in numbered packages. No FS-UAE launch, release, commit or
push performed for this follow-up.

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

## 2026-09-26 — Level1 v5 fixes building-light silhouette

User rejected v4's enlarged building glow in the supplied FS-UAE screenshot.
V5 modifies only the original five cyan pixels at rear x771/y117–121: brief
blue dimming/blinks, fixed size, no halo and no white expansion. All 48 frames
are verified unchanged from v4 outside that building patch; inside it all
non-core pixels equal the original artwork. Sky and tower animation retained.
Native compile, independent planar reconstruction and actual-C sanitizer buffer
stress pass (2,048 cases, native-observed stride). Frame blob 99,268 bytes;
Chip allocation unchanged (96,000 extra bytes observed in v3). No new cadence
claim. User visual acceptance of v5 and minimum 68020 performance remain pending.

Active test: `sparkpaw/dist/L1-Electric-v5-030-HD/Level1-Test`, same PAL 68030
configuration. Inspect building for 10 seconds, unpaused left mouse press/release
to save, wait 15 seconds after deliberate freeze. V4 including evidence archived
byte-identically under `sparkpaw/dist/older-builds/L1-Electric-v4-030-HD-20260926`.
179 release files and 79 production assets unchanged. Proofs and supplied original
screenshot: `sparkpaw/build/level1-electric-v5-20260926/`. Status/history updated;
no emulator launch, release, commit or push. Alpha.10 remains current.

## 2026-09-26 — Level1 electric v4: more sky and building pulse

User likes v3 in FS-UAE and requests more sky discharges plus a conspicuous
pulse in the cyan slit of the building marked in their screenshot. Exact CPU
configuration was not supplied. V4 doubles sky sites from 8 to 16, preserves
the tower sequence, and pulses the existing slit at rear x771/y117–121 with
a bright core and bounded blue spill. Existing eight pens and Copper palette
remain unchanged. Review frames: `assets/concept/level1-rear-ambience-study-v4/`.

Active focused HD test: `dist/L1-Electric-v4-030-HD/Level1-Test`, first PAL
68030 visual gate with 2 MB Chip / 8 MB Fast. V3 and its complete user log were
archived byte-identically to `dist/older-builds/L1-Electric-v3-030-HD-20260926/`.
No release, commit, push or emulator launch. Alpha.10 remains current.

V3 log: build_id=l1electric_v3_candidate; post_run=complete; 8,050 intervals /
8,086 PAL fields (~49.78 FPS), 37 two-field, no three-plus, one zero interval;
rear-specific unsafe=0. Broad ownership counters unavailable. Mostly late-level
samples; not a matched 68020 performance acceptance. The log reports 96,000
extra Chip bytes: actual 152-byte display stride gives 94,848 rear bytes plus
1,152 staging. This corrects the earlier 91,008-byte host estimate. No further
Chip allocation is added by v4. Fast frame file grows 58,756 -> 99,556 bytes
(+40,800), plus descriptor/cache metadata. Executable 318,644 bytes (+556 vs
v3); added loading time and CPU/Blitter duration remain unmeasured.

V4 has 18 complete planar patches. Native build passes; independently decoded
frames reconstruct all 48 source previews exactly. Actual C sanitizer harness
passes 2,048 alternating camera/phase/reset cases at both earlier host stride
and native-observed stride, allocation/file failures, active/source immutability
and guards. Stress maximum 5,184 destination bytes / 8 patches (24 plane blits;
15,552 aggregate Chip transfer bytes including CPU staging and Blitter reads/
writes), not native worst-case time. Other tested native assembly units remain
byte-identical to v3 (Stormrail, game, main, audio_mix, platform_amiga).
Renderer algorithm unchanged; log variant=v3 describes that engine while v4's
build_id identifies the new candidate. No full-suite rerun for this art/data
iteration; previous full-suite pass belongs to v3.

Staging verified 75 runtime assets, 60 literal executable references, 179 release
files and all 79 production runtime files unchanged. Proofs, original screenshot,
user log and source inventories: `build/level1-electric-v4-20260926/`.
User should inspect denser sky, original tower and marked building for 60–90s,
scroll back once, then press/release left mouse while unpaused to save this
candidate's own renderdiag.log; deliberate freeze, wait 15s before emulator stop.
V4 visual acceptance, minimum PAL A1200/AGA 68020 cadence and campaign/media
integration remain pending. Earlier sections below are historical records.

## 2026-09-26 — Approved Level1 electric v3 staged for native test

User approved the v3 preview and explicitly requested a playable test. Candidate
is staged in `sparkpaw/dist/L1-Electric-v3-030-HD/` (`Level1-Test`), unnumbered;
release remains 0.7.0-alpha.10. First gate: user-run PAL A1200/68030 with
2 MB Chip / 8 MB Fast, followed by paired baseline/candidate 68020 testing.
No emulator was launched; native visual quality, cadence and load time are
not yet accepted. Existing builds, rejected previews and local changes retained.

The optional `SPARKPAW_LEVEL1_REAR_AMBIENCE` path reproduces all 48 approved
indexed frames: existing-bolt downward pulse, crystal response, eight sky sites.
Complete rectangular bitmap frames follow the Drowned art technique, but updates
use a second guarded rear buffer tied to the inactive Copper/foreground index.
All patch DMA completes before publication; the canonical bitmap stays immutable.
Palette/Copper colours and HUD/gameplay logic are unchanged. Six simulation ticks
per phase; pause freezes the sequence. No free gameplay time is assumed from
WHDLoad loading gains.

Incremental allocation: 89,856-byte rear display plus 1,152-byte Chip staging
(91,008 Chip bytes total), 58,756 Fast bytes for the animation file plus small
metadata/allocator overhead. File is 58,756 bytes; executable grows 2,836 bytes
against the matching focused baseline. Host stress observed at most five patches,
3,456 destination bytes / 15 plane blits per prepared frame (10,368 aggregate
Chip transfer bytes including staging copy and blitter source/destination).
These are transfer counts, not measured CPU/Blitter time or proven frame margin.
Loading adds file read/allocation and second-buffer initialization; native cost
remains unmeasured. Minimum stays PAL A1200/AGA, 68020, 2 MB Chip, 8 MB Fast.

Native baseline/candidate compile and full host suite pass. Actual C sanitizer
harness passes allocation/file failures and 2,048 alternating camera/phase/reset
cases; active/source immutability, guard bytes and retirement checked. Independent
planar decoding matches every approved frame. Native assembly remains identical
for game, main, audio_mix, platform_amiga and Stormrail renderer; unflagged Level1
assembly also matches pre-edit baseline. This does not establish native timing.

Build/proof records: `sparkpaw/build/level1-electric-v3-20260926/`, including
`verification.json`, `rear-host-proof.log`, `host-suite.log`, baseline/candidate
and assembly. Staging verified 75 declared assets / 60 discovered references.
Fifteen Drowned assets absent from the production source directory were copied
from current alpha.10 into the candidate supplemental build directory only;
provenance is in `supplemental-release-assets.json`. All 179 release files and
79 production runtime files match their initial hashes. No release/commit/push.

Short playtest: watch opening 10 seconds; play/scroll right and back for 60–90
seconds, inspect bolt/crystal/sky alignment, HUD, audio and transitions. While
unpaused press and release left mouse to save `renderdiag.log`; the focused test
freezes deliberately. Wait 15 seconds before stopping/resetting the emulator.
Full controls and limitations are in the drawer's `ReadMe.txt`.

## 2026-09-26 — Level1 electrical ambience v3 for review

V2 rejected as almost invisible. User requests a visibly glowing existing bolt,
downward travelling pulse with crystal response, and electrical discharges
across the whole sky. V3 host study supplies opening/tower/full-panorama/four-
camera GIFs in `assets/concept/level1-rear-ambience-study-v3/`. Eight rear pens
retained; source art, runtime and dist untouched. V1/V2 preserved. Native beam
scheduling remains unproved; no playtest/release/commit/push. Alpha.10 current.

## 2026-09-26 — Level1 ambience v1 rejected, v2 visual review

User requests a subtler animation fitted to existing lightning/art. V1 is
preserved with generator snapshot. V2 (`assets/concept/level1-rear-ambience-study-v2`)
changes only up to nine existing bolt-core pixels and one crystal-facet pixel,
with a long unchanged rest. Clouds remain static during this focused revision;
full-level ambience remains pending. No runtime/production/dist changes or
native acceptance. Release remains 0.7.0-alpha.10. See the research document.

## 2026-09-25 — documentation reconciled; next research topic

Current docs now explicitly distinguish alpha.10 from historical WHDLoad
investigation entries and mark dist archival complete. BUILDING documents
9artifacts/3ADFs/twoWHD profiles and the current campaign packaging entry point;
the legacy standalone WHDLoad ReadMe template is labeled obsolete for alpha.10.
No packaged alpha.10 files were changed by this documentation pass.

Next requested topic: investigate feasibility of Level1 background animation.
See `sparkpaw/docs/LEVEL1_BACKGROUND_ANIMATION_RESEARCH.md` for scope, source
references and proof boundaries. No effect chosen or runtime implementation
started. Target remains68020/2MBChip/8MBFast; Drowned rear-animation lessons
require Level1-specific ownership/timing checks. WHDLoad loading results are
not gameplay headroom evidence. Begin with source/budget investigation and a
small visual proposal; user performs any FS-UAE test. No release/commit/push.


## 2026-09-25 — dist cleanup completed at user request

User explicitly requested only alpha.10 remain in dist. All superseded root
items, including Controls-Mode-HD, restoredalpha8, alpha9 artifacts/drawers,
WHD-NoCache-B-8M and the alpha9 .uaem sidecar, moved intact into
`sparkpaw/dist/older-builds/alpha10-cleanup-20260925`.
No deletion or overwrite. Full before/after SHA256 inventory verifies all
27385 files unchanged through path mapping, including existing older-builds
and alpha.10. Root now contains only alpha.10's9 artifacts/3drawers plus
older-builds and Finder metadata. Proof: build/release-0.7.0-alpha.10/
dist-cleanup-before.json and dist-cleanup-verified.json.
Release acceptance boundaries unchanged; no emulator launch, commit or push.


## 2026-09-25 — alpha.10 packaged and verified locally

Local current release is 0.7.0-alpha.10 / Phase7B.1. Public itch downloads
freshly checked: alpha.8. No upload, commit, push or FS-UAE launch performed.
`make PYTHON=../.venv/bin/python3`, `make release PYTHON=../.venv/bin/python3`,
full `make test PYTHON=../.venv/bin/python3` and independent
`tools/verify_checkpoint_release.py` all PASS. Existing compiler optimizer /
no-effect warnings and duplicate Makefile '&' target warnings remain.

Dist contains 9 new artifacts: HD ZIP/LHA, standard8MB WHDLoad ZIP/LHA,
separate HighRAM WHDLoad ZIP/LHA and Disk1/2/3 ADF; all3 extracted drawers.
Native builds are isolated in `build/release-0.7.0-alpha.10/{hd,banks,highram,adf}`.
Production binaries have no load trace/state flags or load-times.log string.
Standard banks:74 raw canonical assets, actual C ASan/UBSan backend tests,
malformed/failure lifecycle cases and all14/14/17 section dependency reads pass.
Independent ZIP CRC/LHa test/Lhasa extraction parity passes for HD79 files,
standardWHD11 files and HighRAM80 files. All Amiga components <=30 chars;
icons tested; 3ADF filesystems and every file read back. Stable SP09D media
identifiers are format-compatibility markers, not a displayed release version.
HighRAM raw preload5,655,834 bytes + slave5,767,168 =11,423,002 bytes before
host overhead; >=16MiB is a conservative target, not a hardware acceptance.

Reports, archive byte sizes/SHA256: build/release-0.7.0-alpha.10/
checkpoint-release-verification.json and release-artifacts.json.
Full logs: build/alpha10-{make,release,tests,verification}.log.
All27,206 files present in dist before this release and all79 runtime source
files verified unchanged. Main previous build backed up under
build/release-alpha10-preservation. Existing alpha9, restoredalpha8 and B test
remain in dist temporarily: latest FS-UAE log is still active, and the stop
question has not been answered. Do not move these mounted drawers until stop.
Controls-Mode-HD is unrelated and remains protected. Archive intact, neverdelete.

Final trace-free release has package/host verification only. A focused user
check remains: standard8MB on same020 settings, intro->READY, Level1/Escape,
then short Stormrail/Drowned starts; report flicker/load failure. No mouse-log
save is present. HighRAM requires a separate >=16MiB playtest. No need to
repeat broad startup debugging now. 57% remains the earlier same-code A/B
CHARGING result, not a measured final-binary or hardware performance guarantee.


## 2026-09-25 — alpha.10 release preparation; alpha.8 memory baseline rejected

User supplied a screenshot of original alpha.8 WHDLoad 20.0 failing with
"Can't allocate ExpMem." on the target FS-UAE setup. Preserved byte-for-byte
as `sparkpaw/testresults/Phase 7B.1-alpha8-WHDLoad-ExpMem-failure.png` plus TXT.
Its 8 MiB reservation cannot coexist with host overhead in total 8 MiB Fast.
User's real Amiga has substantially more Fast RAM. This is NOT alpha.8 timing
evidence: withdraw the remembered alpha.8 speed comparison on target memory.

User authorizes next alpha if first-intro improvement needs more investigation.
Bounded review finds raw executable and raw intro already present. Bank boot
measurement begins after Kickstart/executable/platform startup and excludes
host switch wall time, so it cannot explain the entire estimated 10 seconds.
The deliberate 35-PAL-field display lock is retained. No new startup change,
splash override or fresh emulator run; further cold-start profiling deferred.

Preparing 0.7.0-alpha.10, same Phase 7B.1, public itch baseline alpha.8.
Standard WHDLoad uses raw common/intro/current-level banks, raw executable,
5 MiB game + 512 KiB Kickstart, no file caching/PRELOAD, and the played B
NOCACHE option. READY run-copy/XOR optimization and fast decoder remain.
The diagnostic collector and CPU-state probe are absent from production builds.
Separate HighRAM binary uses raw individual assets and PRELOAD, normal CPU
cache policy, target >=16 MiB Fast. No dynamic profile or stats prefetch.
HighRAM package correctness is testable locally; gameplay is still unplayed.
The 57.16% result is specifically the identical-code FS-UAE 020 comparison:
CHARGING->READY20.68 ->8.86s, three visits each, user reports no flicker.
Not an alpha.8 comparison, total startup gain, or real-hardware claim.
Whole-campaign cadence and hardware cache effects remain unmeasured.

Release tooling now uses isolated versioned build directories, keeps previous
builds and evidence, and independently verifies both WHD profiles and 3 ADFs.
The old skill's packed/PRELOAD3.5MiB prescription is superseded for the standard
edition by the user's explicitly tested and requested banked8MiB workflow.
CONTROL fcd8573 is included; no commit/push or public upload authorized.
The intermittent real-Amiga two-line HUD-boundary glitch remains open.
Build/package validation and final archival status will be recorded below.


## 2026-09-25 — configuration A/B: NOCACHE faster; original alpha8 restored

User played both on FS-UAE and explicitly stopped emulator. B feels twice as
fast CHARGING->READY; no flicker in either. Startup still feels long but user
now questions remembered alpha8 duration and requests original WHDLoad back.
Both complete36-row/no-overflow logs preserved with SHA and parsed phases in
`sparkpaw/build/whdload-cache-ab-results-20260925`. All rows ok=1.
A CHARGING20.68s on ALL3 visits (renderer17.48+READY3.20).
B CHARGING8.86s on ALL3 visits (renderer6.96+READY1.90),57.16% shorter.
A setup/enemies/effects:2.80/5.54/8.46-8.48; B1.14/2.20/3.34s.
Byte parity of played executable/slave/banks rechecked; saved icons confirm
A SLAVE/PAL, B SLAVE/PAL/NOCACHE, with no other added option. Evidence now
strongly isolates CPU-cache OPTION influence on this FS-UAE configuration;
it does NOT prove why caches hurt, a real-hardware benefit, or a general
production recommendation to disable caches. Do not keep calling NOCACHE
only a slower control; it actually won this manual loading comparison.
Gameplay cadence/full campaign and realhardware remain unmeasured.

A completed drawer/log/launcher archived intact as
`dist/older-builds/WHD-Cache-A-8M-measured`. B stays at `dist/WHD-NoCache-B-8M`
for the requested startup comparison; original B log safely copied above.
Original alpha8 WHD ZIP CRC passes and64 file payloads restored byte-for-byte
to `dist/Sparkpaw-0.7.0-a8-WHDLoad`; source ZIP and all26885 pre-investigation
dist files unchanged. Original slave, PRELOAD/PAL icon and executable retained.
No new build/release/version/commit/push; alpha9 remains current release.
Alpha8's original ExpMem0x800000 remains, not a claimed total8MiB fit; do not
silently alter its slave or emulator RAM. If original fails memory gate,
record exact error before designing a separate comparable configuration.

Next user comparison: same FS-UAE settings, original alpha8 Sparkpaw icon;
measure click-to-first-intro and CHARGING->READY separately. Note any WHDLoad
splash-window wait separately if possible. Alpha8 has NO diagnostic log-save
mouse gesture: exit normally using F10; do not request left-click/freeze.
New source work deferred until this requested reference comparison. Current
first-image10s remains user estimate, not inferred from bank guest timings.


## 2026-09-25 — WHDLoad configuration audit and identical-code A/B prepared

User correctly stresses raw intro/READY/Level1 and current-level banks are
ALREADY implemented. Do not present that as work still to be done. No new
runtime changes today. Audit binary SetCPU sequence at offset92: alpha8,
alpha9 and current all203c0000393e223c00007f3f4ead0060 (same value/mask/call).
Thus no demonstrated cache-policy change between releases. ExpMem differs:
alpha8 0x800000, alpha9 0x380000, current0x580000. Alpha8's whole8MiB
reservation must NOT be blindly reinstated on total8MiB target with host OS.
Global S:WHDLoad.prefs read explicitly read-only from configured HDF partition0:
all options commented out. No disk/config writes. Latest FS-UAE log Sept24
14:07 confirms68020/MMU0/JIT0/real/cycle-exact and8MiB Fast. Saved config file
still says030, demonstrating why actual run log matters. Preserve both facts;
do not blame changed settings. Audit snapshots in
`sparkpaw/build/whdload-config-audit-20260925/configuration-audit.json`.

Configuration-only pair STAGED in dist; manual A/B test pending:
WHD-Cache-A-8M and WHD-NoCache-B-8M. Both use byte-identical played ReadyFast
executable SHA654ee3d446fd52f6a8e1c9dcc31be56a506729b961bba27257ca68697f390660,
same slave/five raw banks/reservation. Only B adds NOCACHE to icon; ReadMe also
differs. CPU cache is NOT file caching. Neither changes PRELOAD, file-cache
exclusion, startup bank strategy or per-level loading. No new instrumentation,
Supervisor probe or build/link placement changes. This deliberately tests
whether instruction caching affects the fast/slow preparation regimes, NOT a
proposed NOCACHE speed fix. Same behavior in A/B would narrow the hypothesis,
not establish a root cause or prove alpha8 parity. Startup10s remains open.

Prepare/stage tool `sparkpaw/tools/stage_whdload_cache_ab.py` verifies source
inventory, exact executable and pair-only differences, protects releases and
archives completed LoadState intact with its original log/launcher after
stop evidence. The latest preserved FS-UAE log confirms streams flushed,
emulation thread stopped and end of main; the async stop question is thus
no longer needed for that completed session. Process listing was unavailable.
LoadState archived hash-identically in older-builds/WHD-LoadState-8M-measured.
Both dist A/B inventories and archived log match the saved proofs.
Manual plan after staging: same020/8MiB/PAL, fresh emulator start for each,
A thenB, intro then Level1/Escape twice, left-click READY, wait frozen, F10,
stop. Separate logs. No full campaign/hardware test. No release/commit/push.
Sources: https://whdload.de/docs/en/opt.html (NoCache vs NoFileCache) and
https://whdload.de/docs/en/cache.html. All timing conclusions remain pending.


## 2026-09-24 — LoadState log complete, initial loading also slow

User reported F10 initially did not respond, then succeeded. Exact drawer
`dist/WHD-LoadState-8M/data/load-times.log` exists,13916bytes, complete=1,
43 phase rows plus43 state rows, overflow=0, all ok=1. Saved byte-identically
with parsed rows in `sparkpaw/build/whdload-loadstate-analysis-20260924`.
CHARGING renderer+READY:21.08s initially,21.04s and21.08s after Escape.
Initial renderer879fields, returns877/879; READY175 each. Thus this run is
already slow before gameplay; do NOT keep claiming only Escape triggers it.
Diagnostic build/placement/instrumentation and run-state differences remain
possible; this is not a matched fast/slow transition within this log.
CACR remains1 at all snapshots (no observed register transition); this alone
cannot exclude cache mapping/placement effects. Ordinary prep DMA991/INTENA
24620 remain consistent. CIA-A TOD closely matches guest VBlank durations
except boot/release boundaries. boot_banks9guest fields /12TOD ticks is NOT
click-to-first-picture wall time and does not explain the reported10seconds.
No proven cause/fix yet. F10's delayed response did not truncate this log;
its specific delay cause is unmeasured. No new runtime edit, build, archive,
release, commit, push or emulator launch. Current drawer retained intact.


## 2026-09-24 — LoadState diagnostic isolates slow Level1 returns

User asked to continue and explicitly confirmed FS-UAE stopped. Source audit
of platformReleaseForLoading, audio stop/unload, ptplayer adapter, music and
KickEmu has NOT proved a fix. Slow return spans raw assets/collision/audio,
renderer and READY, so another isolated codec change cannot explain it.
Same-Level1 bank selection skips actual DOS/disk work; later-level selection
reads a bank. This is a useful comparison, not proof of cache/IRQ causation.

Next `dist/WHD-LoadState-8M` / Sparkpaw icon is diagnostic-only relative to
ReadyFast. Same optimized menu, raw banks, slave/cache policy and display code.
`--load-state` requires `--load-trace`; records before/after CACR (read through
Exec Supervisor,68020+), DMACONR, INTENAR, INTREQR, CIA-B CRA/CRB and CIA-A TOD.
No CACR writes, cache flush, interrupt changes, or CIA ICR reads (read clears
pending state). New phases: boot_banks, return_release, return_cleanup,
return_title. State lines reference zero-based phase-row indices; decimal
values. Existing nested renderer/menu phases must not be counted twice.
CIA-A TOD delta is supplementary: do not assume WHDLoad OS switches preserve
wall-clock meaning. boot_banks excludes WHDLoad/Kickstart/program/platform
startup; still ask approximate click-to-first-picture time. No synthetic CPU
benchmark or deliberate disk reload is introduced. Snapshot table increases
diagnostic BSS, not persistent asset cache. Slow-return improvement not promised.

Native build `build/whdload-loadstate-v2-20260924`:626708bytes, SHA256
b59aaf2898b1baf0f3dcff33b8efffbd76cb19822835ddf8f69010e5c5da9ee6.
First build retained: compiler could not open /var/tmp temporary file;
escalated retry succeeded. No emulator was started. Actual native object
contains MOVEC CACR->D0 and RTE, no MOVEC write. State/basic collector sanitizer
tests, bank lifecycle/fault tests,74 asset reader checks,14/14/17 section
reachability and Drowned/menu-resume/Stormrail loading tests pass. Full suite
not repeated for this diagnostic-only change; prior ReadyFast full suite
passed. Native Supervisor/state reads still require user's emulator gate.

ReadyFast drawer including original log and launcher archived hash-identically
at `dist/older-builds/WHD-ReadyFast-8M-measured`. Staged inventory verified and
all26885 initial dist files unchanged. Manual test same020/2MiB Chip/8MiB Fast/
PAL/noJIT: let intro run, Level1 brief play ->Escape twice, same settings,
left-click final READY, wait frozen, F10 flush/exit then stop FS-UAE. Read this
drawer's `data/load-times.log`. No SOUNDTEST detour or later-level/hardware test
required. If startup fails, capture/report exact error, no blind retries.
No release, version bump, commit or push. Alpha9 stays current.

Reference checked: https://whdload.de/docs/en/cache.html and
https://www.whdload.de/docs/autodoc.html (resload_SetCPU/SetCACR). WHDLoad owns
cache policy; we sample rather than force CACR bits. A CACR value alone is
not proof of per-region cacheability on MMU systems.


## 2026-09-24 — ReadyFast result: successful menu optimization, slow returns recur

User: initial CHARGING ~8s, both Level1 Escape returns ~17s, WHDLoad launch to
first intro ~10s. Complete36-row/no-overflow log saved byte-identically in
`sparkpaw/build/whdload-readyfast-analysis-20260924`. All phases ok=1.
Initial renderer6.30 +READY1.76 =8.06s (previous9.72); menu cache0.60s versus
2.24 before. Renderer subphases: setup0.96, enemies2.00, effects3.12,
targets0.22, final0.00. Nested subphases already included in total.
BOTH Level1 returns: renderer17.48 +READY3.20 =20.68s; cache1.62/1.64s.
Setup2.80, enemies5.54, effects8.48/8.50, targets0.60, final0.06/0.04.
Even assets1.92 versus0.58, collision0.26 versus0.10, audio1.10 versus0.40:
broad phase slowdown, not just menu reconstruction or a fixed wait. Minimum
wait remains0. Earlier26.66s return anomaly has now recurred in a distinct
build; do not call it resolved or blame user settings. Exact mechanism is
still unproven (CPU/cache/interrupt/platform restore remain hypotheses).

Boot currently loads common+intro+Level1 banks before first intro:2837614
source bytes, plus WHDLoad/Kickstart/program startup and display preparation.
Thus raw intro does NOT imply an immediate first picture. User's10s startup
is valid observed wall time, not established by the current loading counters.
It should remain an optimization target, not be marked acceptable/normal.
No matched alpha8 startup timing. Later bank reads may differ from same-Level1
return: bank selector skips disk read when section stays1; this correlation
does not prove a WHDLoad cache-reset cause. No speculative runtime fix made
in this result review. Current ReadyFast drawer stays intact; emulator stop
has not been reconfirmed for this run. No release, commit, push or emulator run.


## 2026-09-24 — READY cache optimization and renderer subphase candidate

After measured raw-bank success, user asks to continue. FS-UAE explicitly
confirmed stopped. Next candidate `dist/WHD-ReadyFast-8M` / Sparkpaw icon:
READY RLE chooses literal/zero once per run and uses memcpy/memset; delta
reconstruction processes four bytes per loop with bounded pointer increments.
No new menu cache or persistent allocation, no lazy work during READY input,
no framebuffer ownership, fade, music, gameplay or bank policy changes.
Native assembly confirms inline longword copy/clear and four XOR bytes per
loop. The archived before-assembly multiplies band height by196 on EVERY
byte; the candidate computes that length once per band. Exact speedup is NOT known until manual 020 measurement; do not promise
the full2.24s disappears. Source changes apply to HD and ADF menu preparation;
ADF pixel tests pass but native ADF/hardware acceptance remains untested.

Loading-only collector adds five nested renderer phases (setup, enemies,
effects, targets, final) within renderer_prepare. They must NOT be added to
parent time. Existing historical Stage2 optimized renderer was ~5.9s, but that
is a different workload/build, not a matched alpha8 comparison. No alpha8
source tag exists locally; the archived alpha8 artifacts remain protected.
Do not invent a precise alpha8 CHARGING baseline.

Build `build/whdload-readyfast-v2-20260924`:625704 bytes,
SHA256654ee3d446fd52f6a8e1c9dcc31be56a506729b961bba27257ca68697f390660.
All five raw banks, slave and icon match played LevelRaw byte-for-byte.
74 actual-reader asset checks, bank lifecycle/fault sanitizer checks,
14/14/17 section dependency checks and loading collector tests pass.
READY exhaustive parity:1436 HD states/6262 transitions,50 ADF states/328
transitions, cache allocations unchanged309471/104847 Fast bytes.
Full host suite PASSED with make test PYTHON=../.venv/bin/python3. Initial
system-Python attempt lacked dependencies; retained log, reran using project
venv. Played LevelRaw drawer+log+launcher archived hash-identically as
`dist/older-builds/WHD-LevelRaw-8M-measured`. ReadyFast is now staged and its
inventory verified; all26885 original dist files unchanged. Emulator result
remains pending.

Focused manual gate: same020/2MiB Chip/8MiB Fast/PAL/noJIT. Inspect initial
intro/title/CHARGING/READY; briefly navigate OPTIONS and SOUNDTEST for text,
music and dust, play Level1 briefly, Escape, left-click final READY once.
Expected frozen save state; wait, F10 to flush/exit WHDLoad, then stop FS-UAE.
No full campaign or real-hardware run requested yet. Read THIS drawer's
`data/load-times.log`. No release/version bump, commit, push or emulator launch.


## 2026-09-24 — raw current-level user test measured

User reports everything seems faster; CHARGING after intro/title and after
Escape now feels longest. Complete LevelRaw log:36 rows, all ok=1, overflow=0;
original bytes and parsed rows preserved in
`sparkpaw/build/whdload-levelraw-analysis-20260924` (repo-relative).
Guest PAL-field scoped loading: Stormrail19.44 ->5.96s; Drowned34.40 ->9.24s.
Stormrail assets13.88 ->0.40s, renderer unchanged5.08s. Drowned assets14.90
->0.70s and renderer18.02 ->7.06s (includes previously packed late assets).
CHARGING renderer6.30 +READY3.42 =9.72s at ALL four visits; minimum wait0.
Previous26.66s anomaly did not recur; cause remains unexplained.
Drowned renderer-end free Fast735488bytes; endpoint only, no peak proof.
Disk/host-switch wall time excluded. User has not supplied a separate explicit
flicker verdict or new hardware result. Keep candidate in place, no archive
or replacement while emulator stop is unconfirmed. Next investigation is
Level1 renderer preparation6.30s plus READY3.42s (menu reconstruction2.24s
included), preserving READY cache/display ownership and8MiB memory budget.
No runtime code change, release, commit or push in this results review.


## 2026-09-24 — loading log analyzed; raw current-level experiment

Completed36-row/no-overflow LevelTimes log preserved with SHA-256 and parsed
rows in `sparkpaw/build/whdload-times-analysis-20260924`. Typical CHARGING:
renderer6.30s +READY3.42s =9.72s; menu cache2.24s is INCLUDED in READY, minimum
wait remainder0. Stormrail scoped total19.44s (assets13.88/renderer5.08),
Drowned34.40s (assets14.90/renderer18.02). Drowned's renderer phase ALSO decodes
pontoon/background data, so do not call all18.02seconds pure graphics setup.
First Level1 return is a real unexplained outlier:26.66s renderer+READY, while
all three other visits are9.72s. User denies pause/turbo/settings changes.
Guest PAL-field timing does not measure disk/host-switch wall time.

New `sparkpaw/dist/WHD-LevelRaw-8M` / Sparkpaw icon changes only Level2/3 bank
storage to raw. Timed executable, slave, icon and all startup banks remain
byte-identical. Only the current level is fetched; no whole-game preload or
stats work.5MiB game +512KiB Kickstart reservation unchanged. Estimated Drowned
free Fast at the previously measured renderer endpoint:735491bytes after
994949extra source bytes. This is NOT a peak/fragmentation or8MiB-fit proof.
Actual74asset parity, bank/collector and14/14/17dependency checks pass; runtime
acceptance pending. No new native/full-suite run needed for identical code.

User completed the instructed save/F10/stop workflow. The completed timing
map, original log and launcher were archived hash-identically at
`dist/older-builds/WHD-LevelTimes-8M-measured`; working non-diagnostic baseline
also remains archived. New proof: `build/whdload-levelraw-v2-20260924`.
Test same020/8MiB: START AT Stormrail -> brief play -> Escape, then Drowned ->
brief play -> Escape; left-click final READY, wait/freeze, F10, stop emulator.
Read `data/load-times.log` from the NEW drawer. Watch for larger black disk-read
intervals or allocation failures. CHARGING and its unexplained outlier remain
open. Full report: `sparkpaw/docs/WHDLOAD_LOADING_MEASUREMENTS.md`.
No emulator launch, release, version bump, commit or push.

## 2026-09-24 — later levels load; phase-timing diagnostic prepared

User confirms that levels load after the shared-file correction. Approximate
FS-UAE observations: CHARGING -> READY ~10 seconds, Stormrail visible LOADING
~20 seconds, Drowned LOADING ~30–35 seconds. These are user estimates, not
instrumented measurements; no new real-hardware acceptance. Preserve the now
working corrected LevelBanks build as the functional reference.

Next diagnostic: `sparkpaw/dist/WHD-LevelTimes-8M`, Sparkpaw icon. Native build
and proof folder `sparkpaw/build/whdload-times-20260923` (work crossed midnight).
Only loading instrumentation: per-section gameplay assets, collision, audio,
renderer preparation, remaining CHARGING minimum wait, READY total and nested
READY menu-cache reconstruction. Up to192 RAM rows, no writes until explicit
LEFT MOUSE at READY. No gameplay profiler or optimization is enabled.
Guest graphics VBlank counter, 50 fields/sec with PAL: this measures guest
preparation elapsed fields, NOT physical disk/host OS-switch wall time.
ready_menu_cache is INCLUDED in ready_total and must not be added twice.
Free Chip/Fast snapshots are phase endpoints, not complete high-water marks.

Manual test: same FS-UAE020 /2 MiB Chip /8 MiB Fast /PAL /no JIT configuration;
intro may be skipped. Start Level1 briefly -> Escape; START AT Stormrail ->
brief play -> Escape; START AT Drowned -> brief play -> Escape. At final READY,
press/release LEFT MOUSE ONCE: deliberately frozen save state. Wait a few
seconds, then F10 to exit WHDLoad/flush pending writes before stopping FS-UAE.
Read `dist/WHD-LevelTimes-8M/data/load-times.log`; require complete=1 and
no overflow. No full campaign or real-hardware run requested. Log persistence
and native timing remain pending the user's test; no emulator was launched.

Native diagnostic:624800bytes. Banks/slave/icon are byte-identical to the
working candidate, so the 74-asset decoded proof carries over by exact parity.
Actual per-section selector tests pass14/14/17 calls; bank lifetime and malformed
input tests pass. Actual collector ASan/UBSan tests cover nesting, phase section
ownership, timer wrap, failed calls, saturation and explicit-only log writes.
A non-diagnostic rebuild is BYTE-IDENTICAL to the working622476-byte executable
(SHA256649a3126867025c6c19795bb98f1ca899a305d9acb3f991ccf6836b51b4d9370),
proving disabled hooks preserve this build. Its proof is in
`build/whdload-trace-off-proof-20260924`. The extracted-C Drowned-driver and Stormrail-start test harnesses now include
the real disabled trace macro header; ownership/failure assertions are retained.
All make-test checks pass across host-suite-v2.log and host-suite-tail.log after
those harness fixes; initial failed runs are also preserved. All26885 original
dist files remain byte-identical; validation.json records the checks.

User freshly confirmed FS-UAE stopped. Working LevelBanks is archived intact under
`dist/older-builds/WHD-LevelBanks-8M-working`, including all launchers/evidence
with verified hash parity; the diagnostic is now staged. No new release, commit or push; alpha.9
remains unchanged. Do not infer the bottleneck from total load times alone:
use the upcoming log to decide whether the next bounded change targets asset
decode/copy, renderer construction or the repeatedly built READY cache.

## 2026-09-23 — LevelBanks startup accepted by report; shared-file correction

User reports the LevelBanks candidate's intro starts/transitions correctly,
music ends in sync, TITLE appears quickly, and intro -> READY has no flicker.
Level 1 played twice and Escape returned through TITLE/CHARGING. This is
FS-UAE evidence in the ongoing 020 test context; RAM/JIT settings were not
restated, and it is not real-hardware or complete-campaign acceptance.
Both START AT Stormrail and Drowned show LOADING, then return to Workbench.
User estimates CHARGING around 10 seconds BOTH at startup and after Escape.
This is approximate observation, not logged phase timing.

Confirmed packaging defect: three cross-level dependencies were incorrectly
owned only by the Level-1 bank. Real collisionLoad still opens
storm-collision.bin for Stormrail. Drowned's assetsLoadGameplay uses
sparkpaw-sprites4.spbm and sparkpaw-extra-life.spbm. Those files were absent
from its resident banks after selecting the next section. The existing code
then returns a load failure and exits; a CPU exception is not established.
The new host dependency test executes these actual C selectors with each
native module's compile flags and the real bank backend: old package misses
exactly 1/2 files for sections 2/3, fixed package misses none (14/14/17 calls).
It stubs allocation/rendering and therefore does not prove native gameplay.
The earlier all-file parity tests missed this phase-reachability contract.

Correction staged at the SAME `sparkpaw/dist/WHD-LevelBanks-8M` / Sparkpaw icon.
Only move these three raw entries into common.spb; no duplicate assets. Native
executable, slave, icon and every individual asset remain byte-identical, as do
intro/Level-2/Level-3 banks. Total startup source buffers remain 2837614 bytes,
post-intro maximum 2064538 bytes. No renderer, timer, audio or cache-policy edit.
New proofs: `sparkpaw/build/whdload-banks-shared-20260923`;
before/fixed selector proofs: `build/whd-bank-dependencies-before2-20260923`
and `build/whd-bank-dependencies-fixed-20260923` inside sparkpaw.
All 74 decoded source checks and sanitized bank lifetime/fault/bounds tests pass.
Native/full-suite evidence belongs to the unchanged earlier executable; no
unnecessary full-suite rebuild was made for this packaging correction.

User confirmed FS-UAE stopped. Previous drawer and launcher archived intact,
hash-verified as `dist/older-builds/WHD-LevelBanks-8M-shared-miss`. Status of
that build: intro/Level 1 accepted by report; later sections rejected.
Focused retest: skip intro if desired, START AT Stormrail -> play 10 seconds
-> Escape, then Drowned -> play -> Escape. Report gameplay entry, return and
visible loading glitches. No repeated hardware transfer/full campaign needed.
No emulator launch, release, version change, commit or push.

CHARGING remains OPEN, unchanged by this package-only repair. Its visible
period contains rendererPrepareGameplay, an elapsed minimum of 100 PAL fields
(~2 seconds, not an unconditional extra 2 seconds), then READY image/menu
preparation before fading. readyUiInit reconstructs a 306544-byte embedded
RLE/delta cache every READY entry, even with raw external assets. Thus the
previous shorthand "no decompression until READY" applied only to external
assets and missed this embedded menu-cache work. Neither its time nor renderer
preparation has been measured separately. Do not attribute all 10 seconds to
that cache, disk reads, RAM pressure or a longer programmed hold. Next timing
probe should separate renderer preparation, minimum-wait remainder, READY
asset copies and UI cache reconstruction; buffer counters in RAM and export
only by an explicit safe action, never during visible loading/menu phases.

## 2026-09-23 — WHDLoad level-bank candidate (manual 8-MiB gate pending)

New unnumbered candidate: `sparkpaw/dist/WHD-LevelBanks-8M`, launch Sparkpaw.
Build/proofs: `sparkpaw/build/whdload-banks-v2-20260923`.
The WHD-only reader uses explicit phase containers through large DOS Reads;
inspected kickfs dispatches these to resload_LoadFileOffset directly. There is
no injected slave callback ABI. The slave reserves 5 MiB game Fast + 512 KiB
Kickstart, disables file caching with ws_DontCache="#?", and omits PRELOAD.
Game-owned bank buffers live INSIDE the 5-MiB budget, not in an additional host
cache. Default HD/ADF/packed-release paths remain compile-isolated.

Raw executable (622476 bytes), common/menu/audio and all intro/Level-1 assets.
Three bulk reads at startup fetch common, intro and Level 1; this is raw disk
I/O before the first image, not an eager whole-campaign decompression pass.
Intro source bank is freed after the final/skip fade. Later sections are read
only after leaving results/READY, while black, before the loading image.
Only the current level bank is retained; sources for future levels do not grow
the resident set. Common Soundtest audio stays available without hidden I/O.
Existing final Chip/Fast assets still own their normal copied/decoded buffers.
No stats prefetch, replay mutation or dynamic high-memory profile.

Bank buffers: startup 2837614 bytes (2.71 MiB), post-intro maximum 2064538
(1.97 MiB). These are exact SOURCE-buffer sizes, not whole-game high-water
measurements or proof of 8-MiB fit. Faster decoder remains for later graphics.
Remaining 2.5 MiB outside ExpMem is for WHDLoad/host overhead; no throughput,
first-image delay, flicker-free behavior or hardware speedup is assumed.
Native build, all 74 decoded source comparisons, sanitized bank lifecycle /
bounds / allocation-short-read failures / 10 malformed containers pass; the
existing 240 reader integrity cases and full `make test` suite also pass.
The log is in the build directory; manual runtime acceptance remains separate.
All 26885 original dist files remain byte-identical, and the staged package
matches its verified inventory.

User confirmed FS-UAE stopped before replacing FastDecode. Its full contents
and any launcher are archived hash-identically under
`dist/older-builds/WHD-FastDecode-8M-startup-slow`;
archival and release parity manifests accompany the new build. The earlier
failed compile directory is retained too. CONTROL and all prior evidence/builds
remain intact. No emulator launch, release, version bump, commit or push.
First gate: 68020/2 MiB Chip/8 MiB Fast/PAL/no JIT, unskipped intro -> READY,
10 seconds Level 1 -> Escape twice; if stable, START AT Stormrail and Drowned,
Escape back. Report first-image wait, intro gaps/music sync, visible-screen
flicker and approximate level/menu loading. No real-hardware replay yet.
Future authorized releases still require separate 8-MiB and all-raw packages.

## 2026-09-23 — external WHDLoad loader research

Inspected official RuffNTumble source, bundled Oscar and Battle Isle sources,
and Flashback install documentation. Direct range/full-file WHDLoad loading is
used in these examples; Battle Isle combines it with KickEmu. Sparkpaw's DOS /
kickfs / streaming-reader chain adds copying and dispatch. Correction: its
512-byte application reads are buffered by kickfs IOCACHE=4096, not one
physical read per 512 bytes. Disk is not faster than RAM; raw disk loading may
beat RAM plus slow decoding, which requires native measurement. Per-level
loading reduces residency/startup work, not media latency. No new runtime
change or test build in this research. See
`sparkpaw/docs/WHDLOAD_SLAVE_LOADING_RESEARCH.md` for source locations,
provenance, limitations and the bounded direct-WHDLoad backend direction.

## 2026-09-23 — 68020 startup requirement; two future WHDLoad packages

MrDig now tests in FS-UAE/68020 and reports that intro still starts far too
late and black gaps remain between plates. This rejects FastDecode as meeting
the startup goal; it does not establish a new flicker verdict or a measured
speed delta versus alpha.9. No new machine RAM details were restated.
The next 8-MiB design MUST use uncompressed executable, intro images/music,
title/music, loading/charging/READY and all Level-1 assets. Faster decoding may
remain for compressed later-section assets (explicit user clarification).
Longer Level-2/3 loading is acceptable on 8 MiB; stats prefetch remains excluded.
Use 68020 as the first timing gate per the user's new test choice; a fast 030
emulator result is no longer sufficient to accept intro timing.

For a future authorized release, provide TWO SEPARATE WHDLoad packages:
8-MiB startup-priority and a fully uncompressed higher-Fast-RAM package with
its own verified requirement. No automatic RAM-selecting combined package.
This is a release-design requirement, NOT authorization to release now.

Further user observation: disk activity is visible only at Sparkpaw startup,
not during its later waits (platform not restated). This supports CPU decode
as a hypothesis; absence of a visible LED is not a cache-miss measurement.
Do not equate black/flickering output with proven physical disk access.
WHDLoad PRELOAD caches stored bytes; it does not decode Sparkpaw's formats.

Budget audit: the 40 boot/Level-1 files alone total 2397907 raw bytes; with the
617748-byte hybrid executable reference that is 3015655 bytes before filesystem
metadata. With current 0x380000 ExpMem, only 1702937 of 8 MiB remain BEFORE host
overhead. Excluding later files alone is therefore not a proven fix. WHDLoad
ws_DontCache supports exclusions, but forcing the current small-read loader to
uncached data risks repeated OS switches/flicker. Need a measured game-memory
budget and coalesced late-load lifecycle, not another optimistic larger cache
candidate. No new build staged in this scope clarification. Existing FastDecode
and all prior work preserved; stop state for its current user run is unknown.
Details: `sparkpaw/docs/WHDLOAD_LOADING_INVESTIGATION.md`.

## 2026-09-23 — raw-mix WHDLoad candidate rejected for flicker

Current replacement is `sparkpaw/dist/WHD-FastDecode-8M` (Sparkpaw icon).
Verified payload is 1910389 bytes: only 156 bytes above alpha.9, leaving
2808203 bytes before host overhead after the unchanged 3.5-MiB reservation.
Slave and icon are byte-identical to alpha.9; 73/74 asset files are identical,
with only the current CONTROL menu repacked. Native build, Shrinkler verification,
240 sanitized reader cases and all asset-source comparisons pass. All 26885
original dist files remain byte-identical. Test intro to READY, Level 1 for
10 seconds, Escape back: flicker-free FS-UAE acceptance first, speed second.
The startup Shrinkler wait is unchanged. No runtime acceptance yet.

MrDig reports renewed flickering TITLE and LOADING after intro and during
level loads in WHD-FastLoad-8M, in the requested FS-UAE test context; exact
settings were not restated. Reject this candidate. Its 3320535-byte preload
payload versus the accepted 1910233-byte alpha.9 payload consumed an extra
1410302 bytes of host cache headroom. Partial PRELOAD/OS switching is the
leading explanation, not a directly measured cache-miss result. Do not
repeat that optimistic 1.3-MiB-before-overhead budget or claim raw loading fixed
real-hardware startup. User confirmed FS-UAE stopped; all 79 files and any
launcher were archived hash-identically to
`dist/older-builds/WHD-FastLoad-8M-rejected`; manifest in
`build/whdload-fastload-hybrid-20260923/rejected-archive.json`.

A new bounded fast-decoder candidate retains compressed storage and the
accepted slave/PRELOAD policy. It handles run state per token and uses a
256-entry CRC32 table, retaining integrity/bounds checks and the same 4-KiB
window. No eager unpack, stats prefetch, high-memory branch or runtime cache
change. Native build and 240 sanitized hybrid/legacy/fast reader cases pass;
new staging/user result follows in WHDLOAD_LOADING_INVESTIGATION.md.
Release remains alpha.9; no commit/push/release or emulator launch.

## 2026-09-23 — WHDLoad fast-loading 8 MiB candidate; user test pending

At MrDig's request, staged `sparkpaw/dist/WHD-FastLoad-8M` for a first manual
FS-UAE/68030, 2 MiB Chip/8 MiB Fast/no-JIT gate. Hybrid asset reads use raw
intro/title/loading/READY/HUD and selected Level-1 data, retaining compressed
large foreground/strider and most later-section assets. Executable is no longer
Shrinkler-packed. Exact corrected alpha.9 slave/PRELOAD retained. Payload
3320535 bytes plus 3.5 MiB ExpMem leaves 1398057 bytes BEFORE host overhead;
full cache coverage/flicker-free operation and speed remain unverified.
Native build, full `make test`, 160 sanitized reader cases and all 74 asset
checks pass. Existing 26885 dist files remain byte-identical; CONTROL and
other prior builds retained.
No emulator launch, release, commit or push. Local release stays alpha.9;
this candidate includes unreleased fcd8573 CONTROL. Only 8 MiB is in scope:
dynamic high-memory selection was discussed then deferred by the user.
Next-section preparation during stats was discussed, then explicitly deferred
by MrDig because of risk. It is outside current scope; results/REPLAY are
unchanged. Do not implement prefetch as an automatic follow-up.
Details and exact test: `sparkpaw/docs/WHDLOAD_LOADING_INVESTIGATION.md`.

## 2026-09-23 — WHDLoad loading regression on real 68030; investigation open

MrDig reports alpha.9 WHDLoad slower than alpha.8 on real A1200/68030
approximately 34.5 MHz: delayed intro, roughly one-second black plate gaps
while music continues, earlier music ending, longer LOADING/CHARGING and level
loads. Packed-baseline acceptance was FS-UAE only. Package inspection confirms
alpha.8 raw assets versus alpha.9 runtime-packed assets and Shrinkler startup;
all 74 alpha.9 packed assets match the accepted campaign baseline, with the
correct 0x380000 slave reservation. Decode/CRC work during black plate loads is
the leading hypothesis, not a measured full explanation of CHARGING or cache
coverage. Menu return reloads Level 1 and needs separate timing. See
`sparkpaw/docs/WHDLOAD_LOADING_INVESTIGATION.md` (from repo root) for package
sizes, source paths, evidence limits and the focused existing-build comparison.
No runtime code/build/release/commit/push or emulator launch. Current release
remains alpha.9; fcd8573 CONTROL remains unreleased. Existing dist/research files
preserved; whole-dist hash inventory saved under build/whdload-loading-investigation-20260923.

## 2026-09-23 — CONTROL options candidate; preliminary user verdict

OPTIONS now offers CONTROL: JOYSTICK or JOYPAD. JOYSTICK jumps with Up and
ignores button 2; JOYPAD jumps with button 2 and ignores Up. Primary Fire alone
handles controller shooting. W and Space remain available in both modes, with
separate edge tracking from controller inputs so a held controller input does
not block a fresh keyboard press. Stormrail flight applies the same selected
controller source to upward movement. The alpha.9 pin-9 pull-up and keyboard
ACK fixes are retained. This is unnumbered work against local alpha.9; no
FS-UAE, affected-user or real-hardware acceptance is claimed yet. MrDig says
the focused candidate "lijkt goed", without a route, controller or machine
configuration. This is a preliminary positive verdict, not a complete control
matrix or hardware confirmation.
Native integrated HD, ADF and packed-WHDLoad builds and full `make test` pass.
ADF/WHDLoad were compiled only; no candidate packages or runtime results. A focused HD
candidate is staged at `dist/Controls-Mode-HD/Sparkpaw-Test` with all 74
runtime references and assets; the alpha.9 release inventory is byte-identical.
Only `readymenu.spbm` differs from alpha.9 among runtime assets. The READY
background and main-menu patches retain alpha.9 bytes. An earlier unplayed
drawer was archived intact under `dist/older-builds` during restaging.

## 2026-09-23 — alpha.9 release checkpoint and controls regression result

Current local release is `0.7.0-alpha.9`: HD, WHDLoad and three ADFs.
MrDig reported the corrected WHDLoad and ordinary HD work in FS-UAE, and
the rebuilt ADF finds Disk 3 in DF2/DF3. Exact emulator configuration was
not supplied. Full host tests, release build and package checks pass.
Alpha.8 and three approved campaign candidate drawers are now archived
hash-identically in `dist/older-builds`. Public itch downloads remain
alpha.8; itch upload is outside scope. Affected hardware users have not
confirmed the controls fix; real-A1200 and physical-floppy testing remain
open.

The first alpha.9 WHDLoad candidate failed MrDig's startup/loading test:
title and loading flickered and loading was slow. Its generic slave requested
5.5 MB ExpMem, versus 3.5 MB in the approved packed WHDLoad candidate.
The rejected package is preserved in `dist/older-builds` with hashes. The
repacked alpha.9 uses the 3.5 MB slave; apart from version text, that slave
matches the approved one. Its packed assets match, and the game executable
is byte-identical to the first alpha.9 candidate, preserving the controls
fix. Package verification passes, and the user reported corrected WHDLoad
startup works in FS-UAE. The release verifier now requires `$380000` ExpMem
for packed WHDLoad.
MrDig also tried ADF Disk 3 in FS-UAE DF2 and it was not recognized until
moved to DF0 or DF1. This matched the original resolver's two-drive scan.
At his request, the alpha.9 resolver now scans DF0 through DF3 and opens
assets from whichever drive holds the matching disk marker. A host test
covers Disk 3 selection and asset paths in DF2 and DF3. The previously
tested Disk 1 ADF is archived with its hash. MrDig agreed to test both
DF2 and DF3, then reported "ja werkt" for the rebuilt ADF in FS-UAE.
This accepts the observed Disk 3 discovery/start route in that emulator;
the exact FS-UAE CPU/RAM configuration was not specified.

The three archived campaign candidates are approved for the previously
reported FS-UAE routes. MrDig additionally played
`dist/Controls-Pullup-HD/Sparkpaw-Test`: it works well and feels unchanged.
The test configuration was not specified. This checks for an observed
regression, but cannot verify the original user failure because MrDig could
not reproduce it beforehand. The affected users have not yet confirmed the
correction. OPTIONS and a possible 68060 connection remain hypotheses.

This release is `0.7.0-alpha.9` with Drowned Turbines, approved v5
music, HD/WHDLoad Soundtest effects, three ADF disks and the two controls
corrections on every medium. Rejected alternative music stays outside the
runtime. The older alpha.8 release and approved candidate drawers are
preserved in `dist/older-builds` after packaging and targeted user playtests.
No real-A1200 or physical-floppy acceptance is claimed.
The played controls drawer was archived byte-identically under
`dist/older-builds/Controls-Pullup-HD-approved-regression` (76 matching SHA-256
files). The three approved campaign drawers and alpha.8 are archived with
their file hashes preserved.
Host `make test` is fully green after repairing the patch-stage extraction
and the Stormrail-load test harness. Native `make` and `make release` pass.
The seven alpha.9 artifacts were built; independent HD/WHDLoad ZIP and LHA
extraction, 74-asset parity, icons and per-file readback of all three ADFs
pass. ADF free blocks are 42/180/76. Targeted WHDLoad, HD and ADF smoke
tests were reported successful in FS-UAE; prior campaign playtests remain
evidence for their earlier exact candidates only.
All 74 WHDLoad packed assets match the accepted WHDLoad candidate byte-for-byte.
Independent per-file comparison with the accepted ADF candidate finds only
`Sparkpaw` changed on Disk 1; every file on Disks 2 and 3 is unchanged.

## 2026-09-23 — controls hardware candidate; user verification pending

Investigated reports of jump/fire lockout after second-button use or OPTIONS.
Corrected missing port-2 pin-9 pull-up (POTGO $3000 -> $f000, set after Disable)
and an independent keyboard ACK minimum-duration error (three raster changes
instead of two, initial beam sample after asserting ACK). A modeled passive
button reproduces the old merged-action latch blocking keyboard too; this is
not reproduction on the reporters' hardware. OPTIONS entry alone has no proven
causal bug; suspected 060 and exact Monster model remain unconfirmed.

New unnumbered `sparkpaw/dist/Controls-Pullup-HD/Sparkpaw-Test` awaits manual
play. Native HD and focused input/menu/audio/campaign checks pass. Broad suite
stops on the pre-existing Drowned patch-stage extraction error; no full-suite
pass claimed. All 74 candidate assets match accepted HD; all 228 pre-existing
alpha.8/approved-candidate files are hash-identical. Old platform-source rebuild
exactly reproduces approved HD. The three accepted campaign drawers remain in
place and older-builds is untouched. No FS-UAE launch, release, commit, push or
version bump. Details: `sparkpaw/docs/CONTROL_HARDWARE_INVESTIGATION.md`.


Latest user verdict: `dist/Campaign-HD-Soundtest-SFX` and `dist/Campaign-WHD-Soundtest-SFX` are also approved ("ok bevonden"). This supersedes the pending-play wording below. These exact HD/WHDLoad candidates, including the Drowned Pump Shot and Checkpoint Soundtest update, join the already accepted `dist/Campaign-ADF-Disk3-Type` as active user-approved campaign baselines. The user supplied no new detailed route-by-route or real-A1200 report with this verdict. Earlier HD/WHDLoad candidates remain archived intact for comparison; alpha.8 remains the official release without commit, push or version bump.

Dist cleanup: after the user confirmed FS-UAE was stopped, the played HD and WHDLoad baseline drawers plus the WHDLoad `.uaem` launcher were archived intact under `dist/older-builds/`. SHA-256 matched for all 157 moved files; all 68 official alpha.8 files stayed byte-identical (`build/campaign-drowned/dist-cleanup-20260923-sfx.json`). Root `dist` contains alpha.8 and the three now-approved campaign candidates. Packaging scripts refer to the archived earlier HD asset source. Earlier root paths below are historical.

Earlier ADF FS-UAE report: `dist/Campaign-ADF-Disk3-Type` passes the full Level 1 -> Stormrail -> Drowned campaign, START AT and disk swapping. The complete-line INSERT DISK 3 art is approved. This is acceptance for those ADF emulator routes; physical floppy and real A1200 remain untested. The three-disk ADF retains its agreed no-Soundtest presentation. Drowned PUMP SHOT and CHECKPOINT are added after HEALTH PICKUP in HD/WHDLoad Soundtest. Native 68020 builds, host menu/cache tests and WHDLoad per-asset reader checks pass; the newest HD assets are byte-identical to earlier approved HD assets. The user subsequently approved the new HD/WHDLoad candidates as stated above. Earlier approved drawers remain intact. Alpha.8 remains official; no commit/push/release.

Session close: a future user bug has not yet been described. Diagnose it against the three user-approved candidates in root `dist`; use `older-builds/Campaign-Drowned-020-HD` and `older-builds/Campaign-WHD-Cache-020` for earlier comparisons when useful. Ask for the exact build, reproduction and evidence only if missing; inspect supplied captures through the test-evidence skills. Preserve the dirty workspace, accepted drawers and logs. The chronological notes below describe earlier states and their pending claims are superseded by this status. No Codex emulator launch, commit, push, version bump or release.

MrDig visually approves the complete-line `INSERT DISK 3` typography preview ("het is nu mooi"). This is art approval only; the new `dist/Campaign-ADF-Disk3-Type` has not yet received a user ADF runtime/full-campaign verdict. Earlier `Campaign-ADF-Disk3-Flow` did request Disk 3 and load Drowned in FS-UAE, but its art was rejected. Technical readback confirms that `disk3-patch.spr1` is the only file changed on each new ADF. FS-UAE was checked stopped; rejected `Campaign-ADF-Disk3-Glyph` was moved hash-identically to `dist/older-builds/Campaign-ADF-Disk3-Glyph-rejected` (manifest `build/campaign-drowned/adf/dist-cleanup-20260923-type-approved.json`). Official alpha.8 unchanged; real hardware untested.

User rejects the code-drawn INSERT DISK 3 glyph; screenshot `testresults/Unassigned-rejected-ADF-code-glyph-3.png` plus TXT. INSERT DISK 1/2 are complete imagegen raster lines, not a runtime font. A new complete `INSERT DISK 3` line was generated from their source reference and saved as `assets/concept/sparkpaw-insert-disk-type-v2-disk3.png`. `tools/generate_disk_status.py` applies the same crop, 216x24 native resize, loading-palette quantization and 224x40 inset to all three lines. New active `dist/Campaign-ADF-Disk3-Type` passes full ADF readback (42/180/76 free blocks) and the pinned 1/2 + common conversion test; only `disk3-patch.spr1` differs from the previous candidate on each ADF. Comparison preview: `build/multidisk-probe/status/preview-2x.png`. The rejected `Campaign-ADF-Disk3-Glyph` drawer was subsequently archived intact after FS-UAE stopped. New art needs user visual approval and ADF playtest; real hardware remains untested. Alpha.8 official.

User rejected the `Campaign-ADF-Disk3-Art` preview: its 3 looked like two overlapping 2s. The new native-indexed glyph has an open left waist, a single middle bar and a right lower stroke, while the approved 1/2 hashes, status frame, palette and all pixels outside the numeral cell are pinned by `tests/test_disk3_status_consistency.py`. New active drawer: `dist/Campaign-ADF-Disk3-Glyph`; complete ADF readback gives 42/180/76 free blocks. The old candidate was archived intact after FS-UAE was confirmed stopped at `dist/older-builds/Campaign-ADF-Disk3-Art-rejected-preview` (manifest `build/campaign-drowned/adf/dist-cleanup-20260923-disk3-glyph.json`). User visual and ADF playtest pending; real hardware untested.

User FS-UAE test confirms `Campaign-ADF-Disk3-Flow` now displays the Disk 3 request and loads Drowned after inserting Disk 3. Its large, separately pasted numeral 3 is visually rejected; screenshot and provenance: `testresults/Unassigned-rejected-ADF-disk3-typography.png` plus TXT. Corrected `dist/Campaign-ADF-Disk3-Art` derives its native indexed 3 from the approved Disk 2 typography and changes only 101 pixels in a 16x7 numeral cell. `docs/ADF_INSERT_DISK_ART_CONTRACT.md` pins the Disk 1/2 source hashes and geometry; `tests/test_disk3_status_consistency.py` passes. Complete ADF readback gives 42/180/76 free blocks, and file-by-file comparison with the played candidate shows only `disk3-patch.spr1` changed on each disk. The corrected art awaits user visual approval; real hardware remains untested. FS-UAE was confirmed stopped and the replaced four-file drawer archived intact at `dist/older-builds/Campaign-ADF-Disk3-Flow-typography-rejected`, with hash manifest `build/campaign-drowned/adf/dist-cleanup-20260923-disk3-art.json`. Alpha.8 remains official.

Latest ADF FS-UAE report rejects `Campaign-ADF-Menu-Disk3`: START AT now displays Drowned Turbines, but START GAME yields a black screen before a visible Disk 3 request. The shared ADF Drowned entry performed disk I/O while direct READY still owned interrupts/Blitter. `platformReleaseForLoading(TRUE)` is now called before loading/requester I/O on that shared path, matching the already DOS-live Stormrail Continue route. New `dist/Campaign-ADF-Disk3-Flow` passes native build, all disk readbacks (42/180/76 free blocks), DF0/DF1 resolver and Drowned lifecycle host tests. FS-UAE user testing remains pending; the fix is not yet proven in play. FS-UAE was confirmed stopped, and the rejected four-file candidate was moved hash-identically to `dist/older-builds/Campaign-ADF-Menu-Disk3-black-screen`; manifest `build/campaign-drowned/adf/dist-cleanup-20260923-direct-drowned-black.json`. Official alpha.8 untouched.

Dist cleanup follow-up: FS-UAE was confirmed stopped (launcher remains open). Rejected `Campaign-Drowned-ADF` was moved intact to `dist/older-builds/Campaign-Drowned-ADF-rejected-menu`; all four file hashes matched. Manifest: `build/campaign-drowned/adf/dist-cleanup-20260923-menu-rejection.json`. Active ADF candidate: `dist/Campaign-ADF-Menu-Disk3`.

Latest FS-UAE report: the packed `dist/Campaign-WHD-Cache-020` WHDLoad candidate also handles F10 in Drowned Turbines and the tested transitions work. Stable TITLE/LOADING and fast loading were reported earlier. This is WHDLoad FS-UAE acceptance for those observed behaviors, not real-A1200 acceptance. The first three-disk ADF candidate is rejected by `testresults/Unassigned-rejected-ADF-start-options-crossfield.mov` and TXT: START AT does not show Drowned and changes SECOND BUTTON. The ADF READY cache and index covered only two sections. New `dist/Campaign-ADF-Menu-Disk3` encodes all three sections while keeping the established no-intro, no-Soundtest ADF presentation. Native build, asset decode, complete per-disk readback and DF0/DF1 resolver tests pass; disk free blocks are 42/180/76. The user's separate possible black screen after Stormrail Continue without an apparent Disk 3 request remains an open ADF playtest issue; the recording does not cover it. The source requests disk 3 and the host resolver test passes, but neither proves the user-visible transition. The rejected drawer was subsequently archived intact after FS-UAE stopped. Alpha.8 remains official and unchanged.

## 2026-09-23 — integrated HD campaign accepted; WHDLoad/ADF pending

MrDig now reports the active `dist/Campaign-WHD-Cache-020` WHDLoad candidate runs well in FS-UAE: TITLE and LOADING no longer flicker, and loading times are fast again. This confirms the reported presentation/loading improvement for the played configuration; F10, complete campaign transitions, ADF and real hardware remain separately unconfirmed. This candidate stores 49 SPL1, 17 SPR1 and 6 SPD1 losslessly compressed assets plus two raw files: 5,040,578 -> 1,778,401 bytes. Shrinkler reduces the executable 608,852 -> 131,072 bytes. Installed drawer: 1,930,568 bytes. PRELOAD is a cache policy for these stored files, not a compression codec.

Dist cleanup completed after confirming FS-UAE itself was stopped; only its launcher remained open. The seven superseded WHDLoad drawers and original WHDLoad ZIP are now in `dist/older-builds/`, including all Diag2/3/4 logs. Their per-file SHA-256 hashes matched before and after the move; manifest: `build/campaign-drowned/whdload/dist-cleanup-20260923-post-diag4.json`. The `dist` root retains alpha.8, accepted `Campaign-Drowned-020-HD`, three-disk `Campaign-Drowned-ADF`, and current `Campaign-WHD-Cache-020`. All 68 alpha.8 files are byte-identical. Historical `dist/Campaign-Drowned-WHD-*` references below point to these archived drawers.

User FS-UAE screenshot in `testresults/Campaign-Drowned-WHDLoad-ExpMem-allocation-failure.png` rejects the first unnumbered WHDLoad candidate at startup: WHDLoad 20.0 reports "Can't allocate ExpMem." The slave reserved all 8 MB Fast including Kickstart and icon used PRELOAD. A corrected unnumbered candidate is staged at `dist/Campaign-Drowned-WHD-Fix` with 0x580000 ExpMem and no PRELOAD; identical campaign executable/74 assets. Native slave assembly, header, icon and file checks pass. The user subsequently reports it reaches gameplay but has TITLE/LOADING flicker; complete campaign and real-hardware acceptance remain pending. The earlier failing candidate remains intact in dist for provenance.

User now reports that the no-PRELOAD fix runs but TITLE/LOADING repeatedly flicker during startup and after Escape, while CHARGING/READY/gameplay stabilize. A separate packed PRELOAD WHDLoad experiment was staged at `dist/Campaign-Drowned-WHD-Preload`: 74 lossless packed assets total 1,778,401 bytes, Shrinkler executable 131,024 bytes, 0x480000 ExpMem. User playtesting rejected it as a flicker correction (below). Historical alpha.35 HD SPR1 loading stalled at LOADING; see `WHDLOAD_PRELOAD_PACKED_EXPERIMENT.md`. The accepted raw HD and ADF packages are unchanged.

User's subsequent FS-UAE test rejects packed PRELOAD as a flicker fix: cold TITLE/LOADING and direct Stormrail loading still flicker; Escape can be stable once and flicker again after another section start. Direct Drowned returned to Workbench from LOADING in both raw/no-PRELOAD and packed/PRELOAD WHDLoad, without a visible message. Accepted HD direct Drowned works. Subsequent Diag2/3 logs isolate the Drowned renderer stride failure below. Do not treat any WHDLoad candidate as accepted.

User played Diag2. Its logs pinpoint failure in `aga32DisplayLayoutValid()` after Drowned assets, collision, audio, target/Copper, HUD, sprites and rear guard succeeded. At failure about 884,712 Chip and 3,217,592 Fast bytes remain free. Diag3 subsequently isolated the rear stride (below). Diag2/3 logs remain intact under `dist/older-builds/`.

User played Diag3: rear stride was 198 bytes; front 128, HUD 44 and all pointers 4-byte aligned. Drowned rear source uses 194-byte rows plus a four-byte left guard. The WHDLoad Kick31 run gave exactly 198, failing AGA32 validation. The accepted HD build did not fail this check; its exact allocated stride is unmeasured. Source now rounds guarded rear allocation to 200 bytes, a two-byte row padding with unchanged source pixels. Alpha.8 slave/icon retain PRELOAD and 0x800000 ExpMem but have no Drowned rear; its 59 asset files total 3,558,686 bytes versus the campaign's 74 raw assets/5,040,578 bytes. Diag2/3 logs were archived intact as noted above.

User played `dist/Campaign-Drowned-WHD-Diag4`: Drowned gameplay now starts. Its logs prove the rear stride is 200 and renderer setup completes with 523,720 Chip and 3,633,536 Fast bytes free. User reports about five minutes to load, continued TITLE/LOADING flicker and F10 ineffective in Drowned. The 12.05-second recording `testresults/Unassigned-rejected-Drowned-WHD-Diag4-loading-black-flashes.mov` and TXT establish brief LOADING-image flashes against mostly black frames; the full five-minute duration and F10 are user reports outside the clip. Source review found that Drowned's separate loop omitted the WHDLoad quit-flag check. A `DROWNED_CAMPAIGN_QUIT` return and parent Workbench DMA/View restoration are now compiled into `dist/Campaign-WHD-Cache-020`. This unnumbered candidate uses lossless packed assets (1,778,401 bytes), Shrinkler executable (131,072 bytes), PRELOAD and 0x380000 ExpMem to leave more host cache room than the rejected 0x480000 packed candidate. Native build, host lifecycle/F10, Hunk namespace, codec/readback, slave header and 74-file checks pass. Whether all files preload, visual flicker, real loading time, F10 exit and campaign transitions work in FS-UAE is pending MrDig's test; ADF and real A1200 remain unproven. Official alpha.8 files remained byte-identical.

User explicitly approved `dist/Campaign-Drowned-020-HD/Sparkpaw-Test` after playing the complete Level 1 -> Stormrail -> Drowned path, Continue, carried lives/health/diamonds/score, Drowned stats/Replay, direct Options start and Undertow Circuit in Soundtest. Image and music work well. This supersedes the pending HD wording below. The approval does not transfer to WHDLoad, ADF or real A1200. Alpha.8 remains the official release without version bump, commit or push.

An unnumbered WHDLoad candidate is staged at `dist/Campaign-Drowned-WHDLoad` with 74 executable-referenced assets and the existing BootDOS slave. A three-disk ADF candidate is staged at `dist/Campaign-Drowned-ADF` with boot/Level 1, Stormrail and Drowned volumes. The ADF uses lossless compressed assets, a Shrinkler executable, and the established presentation without intro or Soundtest, per user choice. Its disks have 43/180/76 free blocks after full per-file readback. The Disk 3 request strip was corrected to match the Disk 1/2 typography and frame; the prior candidate is preserved intact in `dist/older-builds/Campaign-Drowned-ADF-before-disk3-art-fix`. Media request, native reader and campaign host tests pass; WHDLoad/ADF playtests and real hardware remain unproven.

## 2026-09-23 — Level1 B accepted; integrated three-section HD candidate staged

User played Level1 A/B, reports it works and explicitly retains B. Saved complete
logs `testresults/Level1-ring-{A,B}-020-run1.log` with provenance sidecars and
`build/level1-two-copy/run1-analysis.json`. Whole-run A48.73/B49.02 FPS; unequal
routes/reset exposure, no causal speedup claim. LEVEL1_TWO_COPY_RING now belongs
to RELEASE_RENDERER_FLAGS, still effective only inside renderer_level1_unit.
Official alpha.8 files/version remain untouched; this supersedes opt-in status.

User then requested complete integration. New HD target `make campaign-drowned
PYTHON=../.venv/bin/python3` produces a single three-section executable:
Storm Ruins -> Continue -> Stormrail -> Continue -> Drowned -> Replay/Back.
Ready Options adds direct Drowned; Soundtest adds UNDERTOW CIRCUIT (v5).
Lives, health, diamond remainder and banked score pass through an immutable
section entry; replay restores entry vitals/fresh local tally without double
banking. Drowned uses existing stats art/tally, own enemies/diamonds/elapsed/score
and existing120s par policy. Direct start gives3lives/6health/0diamonds/0bank.

See `sparkpaw/docs/DROWNED_CAMPAIGN_INTEGRATION.md` (root-relative) for architecture
and acceptance boundaries. Drowned is a separate namespaced in-process engine,
no subprocess/per-frame section branch. Parent relinquishes renderer/audio/DMA
before module entry; module closes before title/Ready return. Static code grows;
this is a migration seam, not the final shared-primitives architecture for disks.

Checks passed: actual driver ASan/UBSan lifecycle including replay, Escape,
gameover, pause and seven injected loader/result failure boundaries; campaign
snapshots/HUD carry; real vasm/vlink namespace fixture, unchanged object payloads,
288 isolated definitions; menu controls including third section/sixth music track;
preview start/stop/failure/IRQ ownership; old1070menu states unchanged, new1292
states/5478transitions and ADF50states/328transitions; new labels visually checked
from host rasterization. Actual native Level1/Stormrail renderers, dispatcher,
game, mixer and platform assembly exactly match played B. Audio gameplay prefix
unchanged, new helper only in menu path. Nine Drowned units audited against
ordinary full flags: unchanged code except appended game/player carry helpers.
New main driver and final link placement still require runtime acceptance.

Active `dist/Campaign-Drowned-020-HD/Sparkpaw-Test`,606528bytes,
SHA256 de11d25a9e4334d70a5d0ab4961e62c9af055471bce51950e9261094bb6401e3.
74assets/74compiled references; all74assets match prior Drowned drawer; all68
alpha.8 release files byte-identical. Loader allocation table totals693244bytes
(including BSS, before runtime assets); requested8MB Fast supports code growth.
No new gameplay Chip buffers; actual integrated free-memory/fragmentation and
transition audio still need manual evidence. Ordinary music build, no diagnostics,
no mouse-save freeze, no measured integrated FPS claim.

User confirmed FS-UAE stopped. Three superseded drawers with logs and launchers
moved intact/hash-verified to `dist/older-builds/20260923-campaign-inputs`.
Only integrated candidate plus official alpha.8 set remain active. No emulator,
release, commit or push. HD manual acceptance pending: direct Drowned/Escape,
Soundtest return, Stormrail Continue and HUD carry, Drowned finish/replay/back,
then full campaign inclLevel1. ADF/WHDLoad/hardware not integrated or accepted.

## 2026-09-23 — Level1 two-copy candidate staged; opt-in only

User asked to continue after the bounded Drowned analysis. Level1 transfer now
implemented as SPARKPAW_LEVEL1_TWO_COPY_RING, effective ONLY inside the existing
renderer_level1_unit.c isolation. Shared ring header derives internal
SPARKPAW_RING_TWO_COPY; Drowned uses same retained layout. Stormrail unit keeps
three copies/base512. No default campaign/release flag changed.

Important Level1-specific finding:3392px world-end clamp differs from5120px
Drowned. At camera3072, resident origin2880 maps to physical-96 with base96.
Visible fetch remains safe, but full resident-window membership alone is not
proof of valid Bob destination. Candidate prototypeRectFits therefore checks
signed physical>=0 and physical+width<=1024 before existing residency check.
Exhaustive host tests show rejected rectangles up to actual max64px are outside
visible320px view plus16px margin. Histories store only prior successful draws;
restore addresses stay target-local. This guard also covers splash drawing,
which has no separate screen-cull predicate. Source/generated assembly reviewed.
Do not remove this guard or claim Drowned's full-resident geometry proof applies
unchanged to Level1. Guard has CPU cost; net FPS gain still unmeasured.

Actual C host ASan/UBSan: real storm-front3392x208,6592frames/layout, all3073camera
positions forward/back, wrap, teleports, CPU initial/column/dynamic rectangles,
clipping, mutable canonical strips,16/24/32/64px overlapping Bobs, restore
history, active-target immutability and memory guards, exact AGA fetch phase.
DMA is modeled, no native raster proof. First naive all-resident actor test
failed at world end; explicit guard and offscreen-rejection proof resolve it.
Reference/candidate transfer counts differ slightly because synthetic invisible
Bobs are now rejected. No world/gameplay/enemy removal is performed.

Drowned actual kernel regression10048frames/layout still passes unchanged.
TOD counter tests now exercise both Drowned/Level1 regions; save/music ownership,
reset/wrap/pause behavior and campaign Level1 asset/isolation tests pass.
Shared header included in Make dependencies; level1-two-copy build/test targets.

Four native builds compile. A plain matches captured current campaign baseline
(not a claim of alpha.8 byte parity). B plain adds44bytes for layout/bounds.
Seven non-Level1 plain units including Stormrail, dispatcher, main, game and
audio are byte-identical A/B. Stormrail cadence unit also identical. Native
allocation calls verify two1024x208x4 bitmaps versus1536:106496byte payload
saving. CPU column/rectangle loops store two destinations instead of three;
this is not a measured whole-frame FPS gain. Retained ordinary Drowned rebuild
still b78af227b85ffa7faa1e1ed8af637b98e6b01e74da6b3ef6f6ce4762f03fd738.

Diagnostic reuse: existing music-safe read-only TOD counter, with Level1 X bins
512,1024,1536,2048,2560,3072,3328 and unambiguous Level1 log header/variant/ID.
No CIA profiler. Equal seeded direct-start paths; completion held for LMB log,
so these diagnostics never transition to Stormrail. Plain controls retain the
normal campaign. Title/ready, normal completion/transition and hardware remain
separate acceptance boundaries. Observer overhead unmeasured, no exact deadline
or runtime ownership counters. No new production cadence claim.

Active manual pair:dist/Level1-Ring-{A,B}-020-HD/Level1-Test; each59runtime assets
and59compiled references checked, asset sets identical.68alpha.8 release files
byte-identical. Host/native details:build/level1-two-copy/{builds.json,
source-hashes.json,native-audit.json,host-ring-proof.txt,drowned-regression.txt}.

A-plain: plain; 305692bytes; SHA256 e80b9c41de04bca7a519b9afaca05fdf9297684f70c6b98454e97c145795c470

A-cadence: l1ring_49e244338f68fb793341_A; 306788bytes; SHA256 24e1221a28a872b9f858520a4d55bea313209a2f5ad6390efe34b43a2d13668a

B-plain: plain; 305736bytes; SHA256 0175c25a8534422c922506415756e54871bcc1010399a495506c02ef08e568c4

B-cadence: l1ring_49e244338f68fb793341_B; 306828bytes; SHA256 53779275a92815e2b9a3fcc8cb7f51ced7e04cc2b61e4421e6bb46262a1aa6b7

Pending user pass: same68020/2MBChip+8MBFast/PAL50/noJIT scope; A then B,
60..90seconds comparable water/enemy/shot workload with reversal and death.
Especially B right boundary/Core, reverse movement near end. LMB press/release,
frozen image expected, wait15seconds then reset/stop. Keep both logs and report
image/music plus subjective cadence. No030gate claimed; user explicitly keeps
this020 investigation. Candidate not promoted before manual evidence.

Temporary housekeeping exception: user has not answered whether FS-UAE stopped
after Busy2. Its completed log is preserved, but do not move its potentially
mounted drawer. Superseded Drowned-Busy2-020-HD remains intact in dist for now;
only new Level1 pair is active. Archive Busy2 plus .uaem byte-identically after
stopped confirmation. pgrep unavailable in sandbox; no emulator launched.
No commit,push or release. Alpha.8 remains official. Interlude optimization is
deferred at user's request, not inferred from Drowned performance evidence.


## 2026-09-22 — Retained-ring busy run1 analysed; bounded round complete

New90532byte log matches busy_00af8181ffe8fa93904b_sparse and staged executable;
complete footer. Preserved testresults/Drowned-busy-two-copy-020-run1.log/.txt.
Analysis and actual upper-route individual samples in build/drowned-busy-two-copy/
run1-analysis.json and run1-upper-samples.json.82samples;106invalid scope pairs
rejected,0incomplete. User says played; no new detailed visual verdict requested.
Previous B functional acceptance including finale remains intact.

Conditional PAL312-line phase estimates, IRQ/DMA/observer included:
- Ferry last groups4/8 gameplay medians5.96/5.83ms; AI nested1.92/1.92ms.
- Foreground/Bob totals11.22/10.77ms (valid n9/7); all restores2.76/2.69ms,
  ring/dynamic sync2.56/2.63ms, other draws2.56/2.37ms, enemy draw1.15/1.22ms.
  Child scopes are nested/nonadditive; sample sets differ by rejected clocks.
- Filtering actual upper y<140:8pre-reset and7post-reset samples. Actual
  game-begin->Bob-end same-sample elapsed median18.78/17.44ms (valid n7/7),
  max19.81/18.21ms. These are NOT sums of phase medians and exclude earlier
  rear work plus final publication wait. Rejected clocks can bias valid subset.
- Rear update median0.13ms; upper peaks3.85/3.91ms in3of15samples. Executed
  after publication, so it delays NEXT update. Cannot add a sample's own rear
  duration to that same sample's prepublication work as a causal deadline test.
  No water-versus-waterfall operation tag: peak operation is not proven.
- Publication-wait scope yields no valid ferry estimates because raw TOD/beam
  edge guard rejects pairs; exact deadline slack and pure CPU vsDMA unproven.

Raw observer cadence whole46.01FPS; upper pre41.59,post43.30. Not ordinary
FPS and not comparable directly with previous minimally observed A/B. Upper
recorded durations6.30/5.52s, shorter than requested25s; do not claim a long
stability run. One reset, no finale samples. prepared_chip261040 in this run;
external state varies, so allocation reduction remains separate from free-RAM
comparison. No50FPS claim.

Interpretation: remaining pressure is cumulative composition plus gameplay,
with occasional rear-update spikes. Enemy draw alone is not the dominant
cost; further enemy mask micro-tuning cannot remove the total load. Source
checks confirm other_draw groups pontoon/collectibles/core/extra-life/splash,
48collectible slots scanned, and asynchronous presentation still has serialized
composition through WaitBlit. A pipeline would need immutable state/history and
DMA lifetime changes. No single low-risk structural switch is justified by these
samples. Existing all-frame marker overhead also contaminates estimated costs.

Recommendation: conclude this bounded Drowned round with accepted ring gain,
transfer proven layout to level1 next (interlude deferred per user). Preserve
CPU/DMA overlap and rear precompute/scheduling as separate measured future
projects, not promise stable50FPS or blindly reduce update frequency. Direct
CPU water-height rejection remains a small unimplemented opportunity, not a
reason for another standalone micro A/B. No further runtime changes in this
analysis, no new test requested. Active Busy2 drawer/log left intact; no claim
FS-UAE stopped. No emulator launch, commit,push or release; alpha.8 unchanged.


## 2026-09-22 — Two-copy retained; final Drowned diagnostic round staged

User additionally played B to the end successfully, then approved: retain
Drowned improvement, one bounded remaining-hotspot round, level1 transfer
later; interlude deprioritized because already smooth. Full-level manual
functional acceptance now includes finale/station by user report. Original
A/B logs still have no finale samples: do not fabricate timing coverage.

Normal drowned-full now enables SPARKPAW_DROWNED_TWO_COPY_RING. Fresh make
build/sparkpaw-drowned-full (292196bytes) and independent plain control are
byte-identical to accepted B plain:
b78af227b85ffa7faa1e1ed8af637b98e6b01e74da6b3ef6f6ce4762f03fd738.
Music ON; FPS/busy/renderdiag compiled out in normal build. Existing startup
logging unchanged. Historical A/B builder strips new default before recreating
reference flags. Shared ring header added to Make header dependencies.
No level1/interlude renderer flags or published release files changed.

One diagnostic now active:dist/Drowned-Busy2-020-HD/Drowned-Test.
ID busy_00af8181ffe8fa93904b_sparse; SHA256
709b987fdbf3fcf662ae38d9df9248cf0c0dfc53813d42bc04e65a00dd5d5141.
Build records/plain/minimal/assembly in build/drowned-busy-two-copy (old
build/drowned-busy evidence preserved). Uses unchanged tested sparse observer:
1/31updates,16samples/region plus post-reset ferry bucket,144total,28512bytes
explicit Fast allocation. Per-frame inactive branches/bookkeeping remain;
active samples add clock reads/counts. Observer cost unmeasured. No per-frame
disk output, music remains ON. Existing save pauses audio only for diagnostic
flush after play. This is a phase-cost locator, NOT new production FPS proof.
Timing includes IRQ/DMA/observer; nested scopes nonadditive, torn/edge pairs
rejected. It cannot directly separate pure CPU from DMA wait.

Native plain/minimal/sparse compile; separate mask-write audits pass. Actual
collector capacity/reset/clock rejection and FPS/save ownership tests pass.
74assets/56compiled references verified. User confirmed FS-UAE stopped;
ring A/B drawers plus all logs/.uaem archived byte-identically under
 dist/older-builds/20260922-ring-accepted (inventory in new build folder).
68release hashes match before. No emulator launched, commit,push or release.

Route for pending user run:25seconds second ferry half/upper route with
multiple enemies, jump/shoot/reverse; die once, repeat25seconds after checkpoint.
LMB press/release, image freezes, wait30seconds before reset/stop for larger
log. No need to replay finale. One diagnostic build, no new A/B pair.

Remaining analysis decisions: compare scope distribution on this actual
retained renderer, conditional on region/enemy count and reset. Prior12..13ms
Bob/6..7ms game estimates predate ring/bounds/water optimizations and are not
current facts. If game dominates, inspect collision/AI/pure queries (known
water-height check after span scan remains unchanged). If restore/draw dominates,
inspect exact word traffic and setup counts before batching. If ring sync still
dominates, inspect dirty generations. Only investigate cross-phase CPU/Blitter
overlap with immutable history/state after profiling: current game mutates
renderer-consumed state, initial old-target cleanup and canonical updates have
strict ordering. Starting one short blit before gameUpdate is not a complete
pipeline. No further optimization bundled in measurement build. If residual
benefit is small/risky, stop Drowned round and transfer proven layout to level1.


## 2026-09-22 — Ring A/B run1: promising cadence and user improvement

Complete matching fps_569f847b713efc560ccc_A/B logs preserved byte-for-byte
as testresults/Drowned-ring-{A,B}-020-run1.log with provenance sidecars.
Staged executable hashes match builds.json. Analysis:build/drowned-two-copy/
run1-analysis.json. User reports “volgens mij beter”; explicitly confirms B
image/music good including reverse scroll/checkpoint, finale NOT reached.

Approximate PAL50 publication cadence:
- Whole A3261/3637=44.83FPS; B2979/3206=46.46FPS.
- Ferry first A384/447=42.95; B389/415=46.87.
- Ferry last A500/645=38.76; B489/586=41.72 (+7.65% observed).
- Upper after reset A456/589=38.71; B230/275=41.82 (+8.03% observed).
  Two-field share29.17%->19.57%; sample durations11.78s versus5.50s.
- Resets A2/B1; no pauses. A has no upper-before-reset sample; B237/283.
- Whole zero-TOD intervals A0/B2; preserve, do not reinterpret as exact
  deadlines. Reset-pair long intervals recorded separately. Whole three-plus
  A4/B2, including resets; no three-plus in after-reset upper samples.

These manual workloads differ and B's after-reset sample is short. Evidence
supports a useful gain together with the user impression and transfer proof,
but is not a controlled deterministic8% speedup or stable50FPS. Music ON;
minimal observer cost remains unmeasured. Neither log has governor/station
samples. Keep candidate and active pair intact; final-region visual coverage
still required before treating the full-level renderer as accepted. Normal
three-copy default unchanged pending that coverage. No new A/B needed merely
to repeat subjective confirmation; next useful manual check is B's finale.

Memory caveat: bitmap payload reduction106496bytes is established by actual
allocation dimensions. Startup prepared_chip is A152832/B142264 (B10568lower),
so these runs do NOT demonstrate greater total free Chip RAM. Available-memory
snapshots include external OS/emulator state; exact cause not established.
Do not present the allocation saving as measured net free-memory gain.

User asks to carry successful structural work to level1/interlude and supports
larger evidence-driven changes. Source portability review: level1 shares
canonical inactive ring,512logical width,32-bit guarded fetch and window-origin
helper, so next candidate is a separately guarded generalized ring layout.
Audit actual water/collectible CPU paths and world-end clamps; host-test its
assets, scrolling/reversal/restore history and source masks before native A/B.
Current Drowned macro deliberately rejects STORMRAIL_PROOF, which campaign
builds include even during level1: simply enabling it globally is invalid.

Stormrail flight differs: prototypePrepareCompactTarget clears each target
once, then skips ordinary rolling/dynamic sync; restores use stormFlightBlank.
Two copies can save the same bitmap payload but much less steady-flight work.
Audit boarding/approach retirement, unbounded distance/wrap, vehicle/drone/
reward histories, blank restores, finale gate damage/cache and transitions.
Keep current FMODE/presentation; no assumption of Drowned-sized FPS gain.
Implement generalized flag only with distinct per-mode tests and an unchanged
reference binary; no global switch or release modification in this review.

No emulator launched; no commit/push/release. Alpha.8 remains official.


## 2026-09-22 — Two-copy ring implemented; manual A/B pending

Structural candidate `SPARKPAW_DROWNED_TWO_COPY_RING` is opt-in only. Two
complete512px foreground copies/base96 replace three/base512. All world-slot
writes rotate by96; actor/restores and Copper use the corresponding base.
Both inactive target allocations become1024x208x4:106496bytes (104KiB) less
Chip bitmap payload. Canonical world, fetch width/phase, animations, audio,
gameplay and post-publication rear update scheduling remain unchanged.
Default drowned-full still uses three copies, retained enemy bounds and water.

Host ASan/UBSan proof runs actual ring/column/patch/restore/masked-blit kernels
with modeled DMA:10048frames per layout, all4801camera positions in both
directions, wraps, reversals, teleports, reset, overlap, guards and inactive
ownership. Ring DMA words43606896->29071264 (exact one-third reduction);
restore21152832 and masked21157344 unchanged. This is synthetic workload
transfer evidence, NOT native time or FPS. Real140enemy poses/4096history
scenes pass on both layouts. Existing reset/column/patch/water-sync/rear/FPS
regressions pass; water-sync harness needed the new header include path.
No emulator/raster acceptance is implied by host models.

Native plain/cadence builds pass. A plain byte-identical to retained enemyB
plain39b6355bd3fa88cc5a6147290e21153b97bf1b680534f107878a090da2d8be3c.
B plain292196bytes,40bytes smaller. Eight non-renderer plain translation units
byte-identical; four renderer assembly variants have separate mask writes and
no old BLTALWM readback pattern. Actual assembly confirms base96/mask511,
two CPU stores, two1024x208x4 allocations and initial Copper offset8 vs60.
Evidence:build/drowned-two-copy/{builds.json,native-audit.json,host-ring-proof.txt}.

User confirmed FS-UAE stopped. Played enemy drawers, logs and .uaem archived
intact under dist/older-builds/20260922-enemy-bounds-run1; archive hashes saved.
Only active diagnostic pair:dist/Drowned-Ring-{A,B}-020-HD/Drowned-Test.
Both74assets/56compiled references checked, assets identical; all68alpha.8
release hashes unchanged. Minimal in-memory TOD cadence only, music ON,
no broad profiler or per-frame disk logging. Observer cost unmeasured.
No runtime ownership counters; cadence is approximate publication timing.

A: fps_569f847b713efc560ccc_A; SHA256 3e3e3a0f1cb203ce1f7ed7455ea7ceb0f300d61a7ed58f22fca9fbbbdfea7565

B: fps_569f847b713efc560ccc_B; SHA256 cb27d9e30664d5cc13ab49ce5cbfa024819ed41e31f015a0281823bc78a26556

Manual test pending: A then B, opening/reverse-scroll/background,20seconds
second ferry half/upper route, die once,20seconds same route after checkpoint;
inspect finale/weather station too, especially B. LMB press/release, frozen
image expected, wait15seconds then reset/stop. Save both logs; report image,
music and subjective hitches separately. Direct68020 per user scope; no030
acceptance claimed. Do not promote until functional/manual evidence reviewed.
One-third ring transfers is NOT one-third FPS; no50FPS claim. No emulator
launch, commit, push or release. Smaller water-contact CPU query untouched.


## 2026-09-22 — System audit, retained bounds and structural renderer plan

User played enemy A/B, reports little perceived improvement, asks to retain
useful work reductions and research larger causes. Clarified “zoom uit” means
look at the whole system; music stays ON. No request to disable music.

Both complete logs preserved with hashes/provenance as
`testresults/Drowned-enemy-bounds-{A,B}-020-run1.log/.txt`. After-reset upper
A216/308=35.06publications/s; B468/654=35.78. Two-field shares42.59/39.74%.
Not causal proof: different manual workloads, A1/B2resets, B no pre-reset upper
samples. Whole42.85/44.95 is not a speedup claim. No governor/station samples
or separate new image/music verdict. Current enemy A/B drawers/logs left intact.

Enemy bounds now enabled in drowned-full alongside retained rear-water change.
Historical A/B builders strip defaults as needed to reproduce controls. Current
default native plain build is byte-identical to the plain counterpart of played
B:39b6355bd3fa88cc5a6147290e21153b97bf1b680534f107878a090da2d8be3c.
Separate all-object rebuild/link map verifies the same hash. No frame code,
assets, audio settings or presentation changed beyond enabling the played flag.

New active plan: `sparkpaw/docs/DROWNED_RENDERER_PERFORMANCE_PLAN.md`.
Research uses Commodore Hardware Reference Manual, Motorola020manual and
historical AA specification transcription. Audit covers actual native main,
renderer, game, enemies, collision, Spillwing, music/SFX mixer plus player,
projectiles, Drowned queries, governor, HUD and asset code. Evidence/maps under
`build/drowned-system-audit/`; use verified-link.map, not initial link.map.
No cache-conflict or exact CPU-time claim from static code/map offsets.

Preferred structural hypothesis: two complete physical512px ring copies with
base96 instead of three copies/base512, retaining current AGA fetch and art.
New offline `tools/analyze_drowned_ring_layout.py` checks actual contract helpers
for all4801camera positions: resident window0..1007, fetch64..959 within1024,
identical world fetch phase. Geometric feasibility only, NOT runtime safety.
Potential104KiBChip saving and1/3fewer ring-copy words, not1/3FPS. Requires full
coordinate/restore/patch/Copper/history audit and pixel/ownership model before
compile-guarded native A/B. Current three-copy runtime contract unchanged.

Secondary plan: CPU/DMA overlap with immutable render snapshots and measured
queue/dispatch overhead; interleaving, sprite enemies and64-bit fetch ranked
lower. OldFMODE3/H3/full-copy failures explicitly retained as constraints.
Concrete smaller CPU finding: water-contact query scans20spans before height
rejection even on upper route; native output confirms. Not changed/timed yet.

Do not ask for another micro A/B now. Next work: finish two-copy ownership/pixel
model, then one structural experiment if safe. If insufficient, measure ready
time/remaining deadline and CPU/IRQ/Blitter separation before broader rewrite.
No claim50FPS. No emulator launch, commit,push or release; alpha.8 unchanged.

## 2026-09-22 — Water retained; enemy mask bounds A/B pending

User explicitly requested retaining the rear-water optimization despite no
convincing measured/subjective FPS win. It is now enabled in `drowned-full`;
this supersedes the earlier opt-in/off decision below. No release changes.
Legacy experiment builders strip that default when reproducing old controls.

Next candidate `SPARKPAW_DROWNED_ENEMY_BOUNDS` is opt-in only, targeting
recurring enemy draw AND restore work. Load-time scan of resident masks stores
packed top/height for each pose and offsets existing frame pointers. Original
plane stride remains intact. A separate 2x4 target-local bounds history keeps
restores tied to the previously drawn pose, not current animation or slot type.
Logical drawnY, sorting, collision, culling, source assets, enemies and animation
cadence are unchanged. Walker restore unions require equal cropped heights;
otherwise individual restores. Transparent poses retain full-height fallback.
All foreground ring copies and post-publication rear/DMA contracts retained.

Real source pose mean row reduction: crab28.60%, Walker12.11%, Spillwing31.25%.
These are unweighted asset averages for drawing, NOT FPS gains or measured
restore totals. Union opportunities can change; added lookup/branch cost may
erase part of the saving. Discovery enemy draws were only ~1..1.5ms in the
busy ferry samples, so a large whole-frame gain is not promised. Ring/dynamic
sync and game/update remain substantial and are not solved by this candidate.

Actual C host ASan/UBSan:4096two-buffer scenes, real140poses,16pixel shifts,
overlap, slot/type reuse, despawn/culling and history clears vs full-cell pixel
oracle pass. Actual selection/restore/history and masked blit setup exercised;
DMA and canonical rectangle copy are host models, not a raster timing proof.
Existing frame-table, resident-Walker, rear water/falls, FPS/save tests pass.
Native plain/cadence builds pass. Eight non-renderer plain translation units
are byte-identical A/B. Native table indexing uses shifts/adds; mask scanning
is load-only. Separate immediate BLTAFWM/BLTALWM writes verified in all4outputs.
B adds400bytes ordinary/Fast BSS, no Chip allocation; plain executable+560B.
A plain exactly matches the played water-B plain SHA256
65279e6a6eab455955aeee9e25abae47559c22f442ce90449f1af0616f7b7332.

ACTIVE: `dist/Drowned-Enemy-A-020-HD/Drowned-Test` and B sibling.
IDs fps_faa04f00df1c82503aa6_A/B. Cadence SHA256 A
5e4edf521f94ea290eceb7cc929ea883881497be98c7eca3c2f2f40f844d1cf7;
B86df68ad2bce6db301d00f8222f77877f72ff6ca18fc028b4d0edd645e3641a0.
Both retain water optimization/music and minimal TOD cadence (no busy profiler,
no reset experiment). Observer overhead unmeasured, no ownership counters.
User's explicit020-only task is the gate; no030 acceptance claimed.

After user confirmed FS-UAE stopped, old water pair/logs/.uaem archived intact
under `dist/older-builds/20260922-water-direct-run1`. Raw logs match preserved
run1 testresults copies. Staged only with stage_hd_test.py:74identicalassets,
56embedded references, all68alpha.8release files unchanged. Proofs/builds and
ReadMes under `build/drowned-enemy-bounds`; rebuild `make drowned-enemy-bounds`.

User route A thenB: inspect enemies/background; checkpoint;20seconds second
ferry/upper route jumping/shooting; die once; repeat20seconds after checkpoint.
LMBrelease once, frozen screen expected, wait15seconds before reset. Compare
regional logs plus subjective hitches and image/music. Candidate pending; no
FPS acceptance, automatic emulator, commit, push or release. Alpha.8 official.

## 2026-09-22 — Rear-water A/B: no demonstrated busy-route improvement

User played both and reports little perceived difference (uncertain), with
second ferry/upper route and Spillwings still heaviest. Both complete logs,
build IDs, executable hashes and regional sums verified. Raw preserved as
`testresults/Drowned-water-direct-{A,B}-020-run1.log` with provenance TXT;
calculations in `build/drowned-rear-direct/run1-analysis.json`.
Before-reset upper: A201intervals/294fields=34.18publications/s;
B224/326=34.36. No convincing gain. Whole-run A41.25/B44.02 is not causal
performance evidence: A records1reset and252after-reset upper intervals;
B records0resets and0after-reset upper intervals, with different region time.
Aafter-reset upper35.29 has no B counterpart. No new image/music verdict given.
Do not request another same-candidate run merely to chase this small difference.
Candidate stays opt-in/off in normal build and is NOT promoted as an FPS fix.
Logical transfer reduction remains proven; subjective/native benefit is not.

Reassess sustained foreground work, not another rear micro-optimization.
Historical audit reminder: PERFORMANCE_68020_STAGE2_AUDIT.md rejects H3
fetch-union pruning at28.96vs28.64FPS in matched old CPU-copy runs; scaled
indexing and branch/address costs erased reduced writes. Current Drowned uses
DMA transfers, so those numbers are not its current cost, but any renewed
copy-pruning idea must explicitly distinguish the new mechanism and prove
fetch/restore/guard invariants rather than silently relaxing the three-copy
contract or repeating H3. No such candidate implemented in this review.

Existing A/B drawers remain intact in dist; no new executable, emulator,
commit,push or release. Alpha.8 unchanged. Further progress needs a materially
larger foreground scheduling/transfer improvement, not a claim that fewer
transfer bytes already solved busy gameplay.


## 2026-09-22 — Rear-water transfer A/B staged for busy-scene gain

User authorizes one meaningful optimization round after complete discovery.
Ring/dynamic foreground synchronization already merges dirty water and keeps
three required physical copies; no unsafe copy removal or animation reduction
was selected. Discovery also exposes intermittent rear work around77..79PAL
lines. Candidate targets redundant transfers in THAT path, not a claimed fix
for all ring-sync cost or a promise of50FPS.

Opt-in `SPARKPAW_DROWNED_REAR_DIRECT_WATER`: after successful publication and
the unchanged line<=64 gate, retire DMA, copy strided Fast water rows directly
into private rearWorld bitmap, then three canonical-to-rearDisplay blits.
Reference copies Fast->private stage then six stage->canonical/display blits.
Both destination rectangles, all pixels/phases, palette and schedule retained.
CPU never writes the fetched rearDisplay. Compile guard requires full Drowned
and the separate guarded rear display; both initial Copper pointers and scroll
patching use rearDisplay in this configuration. This narrowly changes which
private Chip buffer receives CPU water writes, not display ownership. Initial
and final DMA retirement retained. Waterfalls keep their one contiguous CPU
stage copy and six-blit path; their small source rows would make direct CPU
row copies an unfavorable change. Dedicated stage allocation retained for them.

Full water upload:42CPU CopyMem calls/1848bytes in both. Blits6->3; logical
Chip transfer bytes9240->5544 (40%less for this upload, NOT40%FPS). No new
allocations or BSS growth. Actual native plain B+420bytes; ordinary default
build does NOT enable candidate. A plain exactly equals played music baseline
SHA2568ba2327ef2766ee743e21baba1dda5292285d5030bef6871698e6af373dca30d.
Actual native other8translation units identical A/B, safe separate mask writes
verified in both plain/cadence renderers; busy profiler and old CIA profiler
absent. Existing minimal TOD cadence is identical apart from variant/build ID.

Host actual-C ASan/UBSan tests pass baseline, old staging reference and new
candidate:24water phases x76windows=1824rectangles, both destination bitmaps,
untouched pixels/guards, all4waterfalls/4phases, 1000scheduler camera steps,
Copper phases and lifecycle. CPU copies assert no pending DMA; DMA source
bounds and exact transfer count asserted. Existing cadence/safe music+OS save
checks pass. Native fourbuilds pass. This proves pixels/work reduction on host
and native instruction selection, not real runtime timing or visual acceptance.

ACTIVE pair: dist/Drowned-Water-A-020-HD and Drowned-Water-B-020-HD, each
Drowned-Test. IDs fps_8e768ab93ca563d8e77f_A/B. A-cadence SHA256
26ab9932ff321fbb9b8e6f0b211cee3425c206369709d2217513f8c46f62102b;
B-cadence13f7c990148ceac307792249ee0e309e48e79582cc6e36ae9f1cfe8e16a6faa9.
Both74identicalassets/56compiledreferences; all68alpha.8files unchanged.
Complete played discovery drawer/log/launch metadata archived intact under
`dist/older-builds/20260922-busy-discovery/`. Raw evidence also remains in
`testresults`. Proofs/controls:build/drowned-rear-direct/. Rebuild with
`make drowned-rear-direct` after any required full asset generation.

User020 A thenB: briefly inspect water while stationary/scrolling; checkpoint;
20seconds busy second-ferry/upper route jumping/shooting; one death; repeat
20seconds after checkpoint. Optional B-only continuation to inspect last
waterfall is visual-only, excluded from matched FPS comparison. LMBrelease,
wait15seconds (compact cadence logs again), reset. Report busy hitches and
image/music separately. Preserve logs before rerun. No automatic emulator,
commit,push or release. Candidate pending; reject for visual/audio errors or
no convincing benefit versus risk. Foreground ring-sync cost remains open.


## 2026-09-22 — Busy run2 complete; sustained scene cost priorities

User replayed and left the save screen frozen. Verified83/83samples and
post_run=complete; log93263bytes, executable/source ID match staged discovery.
Preserved raw as testresults/Drowned-busy-020-run2.log plus provenance TXT;
strict analyzer output: build/drowned-busy/run2-analysis.json. Original partial
run1 retained. User told FS-UAE may now stop/reset. No new image/audio verdict.
Regional/global cadence sums and raw distribution sums verified. Discovery
aggregate2964intervals/3540fields=41.86publications/s; lateferry478/708=33.76,
upper before reset222/336=33.04, after225/333=33.78. One reset,no pause. These
are observer-contaminated discovery numbers, not ordinary music-build FPS.

Late-ferry before/after groups contain9/7samples. Conditional PAL312 scanline
medians (IRQ/deferred DMA/observer included, nested scopes NOT additive):

| Scope | Before reset | After reset |
|---|---:|---:|
| Game incl AI |98lines,n9|105lines,n7|
| AI nested within game |29lines,n9|30lines,n7|
| Sprite/HUD/Copper |10lines,n9|10lines,n7|
| All Bob work |204lines,n7|190.5lines,n4|
| Ring+dynamic sync nested in Bob |55lines,n9|59.5lines,n6|
| All restores nested in Bob |50lines,n9|44.5lines,n6|
| Enemy draw nested in Bob |23lines,n6|17.5lines,n6|

At nominal64us/line this is about12..13ms Bob work,6..7ms game,
3.5..3.8ms ring/dynamic sync and1.1..1.5ms enemy draw. Do not sum medians
or interpret these as pure CPU timings.134invalid scope pairs were excluded;
near-edge TOD/raster disagreements remain visible in the raw evidence.
Empty timestamp probe median1line confirms nonzero instrumentation cost,
not total overhead. Some rear updates reach77..79lines (~5ms), versus2lines
for the common light path. Instrumented lateframe budgets are close enough to
a PAL boundary for this combined load to matter. This is a scheduling/workload
hypothesis, not proof that any single family explains every missed interval.
Finale-approach group5 has heavier enemies (enemy draw median55.5lines,n8),
so enemy cost must not be generalized from ferry Spillwings to all encounters.

Priority now: ring/dynamic-water transfer workload and interaction with rear
publication work, while retaining all ring copies and single-buffer rear DMA
contracts. Enemy-row trimming is secondary, not the presumed primary fix.
Source review confirms existing merged-water sync and three-copy patch copier;
do not rediscover those as new optimizations. Prepared rear phase row audit
(build/drowned-busy/rear-phase-row-audit.json) shows all14rows vary in planes0/1
and12in plane2, so a simple static-row skip there is not a large win either.
No optimization or new drawer staged in this review. Current discovery drawer
remains available; no further replay needed for this diagnosis. Alpha.8 and
all local work preserved; no emulator, commit, push or release.


## 2026-09-22 — Busy-scene discovery build ready (not an optimization)

User requests continuation with sustained busy-scene FPS as the priority.
One active drawer: `sparkpaw/dist/Drowned-Busy-020-HD/Drowned-Test`.
Build ID `busy_d3d8e3e3d83e1d406994_sparse`, SHA256
`0b5ca6b88c481f4b7c3560328a392730fe0830739dd42da021a2e0b372364dfe`.
The reset Blitter flag is OFF. Enemy-row trimming is NOT implemented pending
actual busy-scene cost evidence; its theoretical saving is not a measured win.

New compile-only diagnostic `SPARKPAW_DROWNED_BUSY` takes read-only CIA-A TOD
and raster checkpoints every 31 frames (prime stride avoids phase-locking to
2/4/6/16-tick animations). Up to 16 samples per region, with a separate late-
ferry-after-reset quota; 144 samples / 28,512 explicit MEMF_FAST bytes total.
No additional Chip allocation. Actual submitted enemy draws by family,
projectile draws, generic masked-Bob and restore word cells are recorded on
sampled frames only. Counts do not cover every dynamic DMA transfer.

Checkpoints cover game/AI, sprite+HUD+Copper, restores, water/patch generation,
ring/dynamic synchronization, enemy/projectile draws, final wait/history and
rear update. No extra WaitBlit, timer ownership/configuration, audio callback
or publication-policy changes. Intervals include IRQ and deferred DMA waiting;
a first wait may retire preceding work, so do not label scopes pure CPU or
add nested AI/game or enemy-restore/all-restores totals. An empty consecutive
checkpoint is a timestamp-overhead probe, not a total observer-cost measure.
Unsampled frames still execute diagnostic guards and sample selection.
Time overhead remains unmeasured; discovery FPS is not production FPS.

`tools/analyze_drowned_busy.py` retains raw zero/two TOD behavior and rejects
incomplete saves, invalid/torn stamps and negative/excessive intervals. Same-
TOD nondecreasing beam intervals need no wrap inference. Cross-TOD estimates
assume PAL312 and aligned field epochs away from raster8..300 edges; retain
that assumption until the supplied raw trace is checked. No exact-deadline
or ownership-violation claim is possible. At most 16 regional samples support
coarse prioritization, not strong percentile or causal FPS claims.

Actual collector ASan/UBSan tests cover allocation failure, sparse gating,
all region/reset quotas and full capacity, torn/edge clock handling, disabled
counts, 24-bit wrap and analyzer attribution/reset exclusion. Existing cadence
and safe audio/OS flush tests pass. Native plain/minimal/sparse builds pass;
plain SHA256 remains byte-identical to the played pre-reset pointer baseline.
Sparse executable297064bytes, +2844versus minimal; static BSS +52bytes.
Native masks remain separate immediate writes; diagnostic calls absent from
plain/minimal hot paths. Proofs and offline controls: build/drowned-busy/.
Rebuild with `make drowned-busy` using prepared current full assets.

Staged with the standard HD stager:74assets,56compiledreferences,76files.
Both played reset drawers/logs/launch metadata are archived intact under
`dist/older-builds/20260922-fps-reset-run1/`. All68alpha.8files unchanged.
Manual020 route:10seconds quiet opening, checkpoint,20seconds busy second
ferry/upper route with normal jumping/shooting, one death, repeat20seconds
after checkpoint. No finale replay needed. LMB press/release, wait30seconds
on frozen image, stop/reset; keep the drawer's renderdiag.log. Only one run,
not A/B. No emulator launched, release, commit or push. Await this discovery
trace before selecting the next sustained-throughput optimization.


## 2026-09-22 — Reset result received; busy encounters take priority

User played reset A/B and reports little perceived difference. User explicitly
redirects attention to sustained drops when several enemies/actions are visible.
Treat that as the primary acceptance target, not the isolated reset pause.
Raw logs are preserved in testresults/Drowned-full-fps-reset-{A,B}-020-run1.log
with provenance sidecars; build IDs, executable hashes, complete footers and
regional sums verified. Calculations: build/drowned-fps-reset/run1-analysis.json.
One reset per run: two reset-associated intervals total16fields in A versus10
in B (nominal320ms versus200ms). This is aggregate publication evidence, not
CPU function timing or a subjective acceptance. Late ferry A36.01/B34.90
publications/s; upper before reset34.56/33.42, after37.58/35.90. Unequal manual
workloads: neither a sustained win nor a proven regression can be attributed.
User did not give a separate image/audio verdict for this second pair.
Do not promote reset Blitter or spend the next user test on reset micro-tuning.

Current main.c already skips the line253 wait for rolling builds; asynchronous
composition is active. Existing resident enemy caches and frame-address tables
also remain active. Enemy draw/restore uses full cell heights over four planes.
Offline inspection of exact prepared SPBM masks found Spillwing32poses with
5..12 empty edge rows of24, mean7.5 (31.25% of draw rows); Walker64poses with
6..15 empty edge rows of64, mean7.75 (12.11%). Bounds recorded in
build/drowned-fps-reset/enemy-row-audit.json. This identifies potential redundant
DMA work proportional to enemies, NOT measured CPU/DMA time or FPS gain.
Keep original plane strides, ordering, logical collision cells and target-local
restore history in any trimmed-draw experiment; do not equate draw height with
source plane stride. Draw-only trimming can leave full restores unchanged.

Next investigation must separate game/AI/collision, player staging/occlusion,
enemy/projectile restore+draw and dynamic water/patch synchronization under
actual busy-scene load. Existing minimal logs lack actor-count/work attribution
and CPU scopes, so they cannot establish which family dominates. Any targeted
measurement must preserve music timer ownership and quantify/declare overhead;
no blanket old renderdiag re-enable. Prioritize a materially relevant measured
busy-scene candidate over another peripheral A/B. No new drawer staged in this
review, no runtime code changed. Alpha.8 remains official, no emulator/commit/
push/release. Existing reset pair and original logs remain intact in dist.


## 2026-09-22 — Rear-stage A/B reviewed; runtime reset-copy A/B staged

User played both rear-stage variants and reports no clear speed difference,
with image and music good. Both complete logs/build IDs/executables verified
and raw evidence preserved as testresults/Drowned-full-fps-stage-A/B-020-run1.log
with TXT sidecars. Raw aggregate publications/s: A43.99/B45.01; late ferry
36.40/36.52. Upper before reset33.79/36.26, after reset37.17/36.53. Unequal
manual workloads, no consistent late-ferry win. Retain rear pointers as a small
work reduction, NOT a measured FPS success. All four long intervals in EACH
run are in reset_and_next: four times8TODfields, two simulation resets. No
three-plus elsewhere; zero/two phase caveat and no ownership counter remain.

User explicitly says continue. New isolated SPARKPAW_DROWNED_RESET_BLIT uses
existing bounded canonical DMA copier only for >=512px runtime camera jumps.
Startup remains CPU. Full208rows, fourplanes, allthree physical ring copies,
waits and exact water/collectible history retained. No new allocations. Same
159744destinationbytes per target,12/24blits; Blitter rereads source percopy,
106496more source bytes than CPU path. Speed advantage UNMEASURED. This targets
reset pauses, not ordinary late-ferry performance. Default full target does
NOT enable this new flag pending user gate.

ASan/UBSan actual oldCPU/newDMA/independent pixel oracle:1734resets, every
valid16pxorigin over5120world, both buffers/directions, guards andhistory.
Existing1788short-columncases pass. Actual counters/flush tests pass; native
fourbuilds pass. New A-plain is byte-identical to preceding B-plain. Other
inspected translation units identical A/B; native reset branch calls DMA,
startup path retained, mask writes remain safe. New B-plain +132bytes, noBSS
orChipbuffer growth. Proofs/assembly: build/drowned-fps-reset/.

ACTIVE: dist/Drowned-Reset-A-020-HD and Drowned-Reset-B-020-HD, executable
Drowned-Test. ACPUreset/BBlitterreset; both priorrear pointers/musicv5, same
TODobserver/seed,74assets/56refs;68alpha.8files unchanged. Played FPS pair
and complete logs archived intact at dist/older-builds/20260922-fps-stage-run1/.
Reproduce: make drowned-fps-reset (prepared current full assets required).
Manual020: checkpoint, LAST ferry upperledges nearfarbank,15secjump/shoot,
deliberatewaterfall, observe respawn pause,10secplay afterreturn, inspect
water/diamonds/enemies/scrollstrips. LMBrelease/wait15sec/frozenimage/reset.
No finale replay needed. Ordinary ferry sync/margin remains separate research.
No emulator launch, release, commit or push. Detailed report:
sparkpaw/docs/DROWNED_FULL_FPS_ROUND.md. Await user's reset/image/audio/log gate.


## 2026-09-22 — Full-level FPS A/B staged; user 020 measurement pending

User accepts corrected rear ambience visually and requests full5120 FPS work
on68020/2MBChip/8MBFast/PAL50/noJIT, preserving music/art/gameplay. Alpha.8
remains official; no release, commit, push or automatic emulator.

Read current contracts and historical engine/audio timing evidence. Starting
Drowned-Test SHA25692bf65300ffba69ee894adc60e72ec557c63232f7c8382b632b1bec4e261825d
has NO renderdiag; logger-free A rebuild is byte-identical. Existing startupdiag
was appended during user's run: initial742bytes recovered by exact prefix hash,
current2226bytes separately preserved. User confirmed emulator stopped before
archiving full drawer/metadata at dist/older-builds/20260922-fps-baseline/.
All68alpha.8files unchanged. Historical logs bound to archived executables;
old3520px nonmusic timings are not measurements of current full music level.

One bounded B change: advancing row pointers in rear private staging removes
84 repeated native row multiplies per water upload. Same42CopyMem calls,
1848staged bytes,6blits,3696destination bytes, masks/waits/schedule/animation.
No allocation growth; plain binary60bytes smaller. Actual reference/candidate
pixel/Copper/guard tests and native write-only mask audit pass. This proves
less CPU work, NOT FPSgain. Broader findings and next hypotheses documented.

Active pair: dist/Drowned-FPS-A-020-HD and Drowned-FPS-B-020-HD; launch
Drowned-Test. Both musicv5, same74assets/56refs, fixed matching initial seed.
Separate SPARKPAW_DROWNED_FPS reads CIA-A TOD once immediately after COPJMP1,
no CIA-B timer/profile ownership. Eight regions plus late upper/lower before/
after reset; reset+next and pause accounted separately. Preserve rawzero/two
phase pairs: NOT exact deadline misses. Ownership counts unavailable. Observer
adds2956filebytes/3040loader allocationbytes including448BSS, no explicitChip;
time overhead unmeasured. Build ID in logs/ReadMes; proofs/assembly/plaincontrols
at build/drowned-fps-round. Host counters and actual safe-flush lifecycle pass;
fullworld/sprite/pause/audio regression checks pass; nativefourbuilds pass.

Manual A then B: opening10sec; full route to checkpoint; lateferry upperroute
combat/back-forth20sec; waterdeath/checkpoint; sameupperroute20sec; Governors/
station and laterfalls. BeforeCore LMBpress/release,wait15sec,frozenimage,reset.
Each writes own renderdiag.log; preserve before rerun. Explicit020scope overrides
usualskill030first. Need user's visuals/audio/smoothness AND logs; no50FPSclaim.
Details: sparkpaw/docs/DROWNED_FULL_FPS_ROUND.md. Next do not blindly enable old
renderdiag alongside music or combine other optimizations before this A/B.


## 2026-09-22 — Rear waterfall black stripes: native mask fix staged

User screenshots show black rectangles/vertical strips at waterfall patches and
water strip. Preserved with provenance as testresults/Drowned-waterfall-black-stripes-1/2.png
and TXT sidecars. Initial animation candidate rejected.

Confirmed compiled-code defect: chained volatile assignment
`hw->bltafwm=hw->bltalwm=0xffff` makes vbcc read back write-only BLTALWM
before writing BLTAFWM. Separate statements now produce two immediate -1
writes to offsets68/70; tools/audit_drowned_rear_masks.py audits actual full-build
68020 assembly and reproduces the defect from saved prior assembly. Other
renderer mask assignments already separate. Prior host fake registers were
readable RAM and missed this hardware restriction; host pass was insufficient.

Host ASAN/UBSAN test now checks both masks and all four waterfall rectangles
across four phases, including untouched plane/row guards. Existing water/colour
parity tests, full-level tests, native assembly audit and native build pass.
No art/gameplay changes. Staged dist/Drowned-Level-020-HD/Drowned-Test:
only executable and ReadMe differ from archived faulty candidate
`dist/older-builds/Drowned-Level-020-H-old-085301`;74assets/56refs verified,
68official alpha.8 files unchanged. Proof build/drowned-full/proof.json.
Manual020 visual acceptance and performance remain pending. Test first waterfall
and water strip for black bars, then later falls. No automatic emulator run,
release, version bump, commit or push.


## 2026-09-22 — Tail diamonds and both flower species staged

User played full5120 route: broadly accepts it, requests diamonds after ferry
and integration of both flower concepts, A behind player/B in front, sparse.
Added12 tail diamonds (3264..4816),45total in existing48-slot pool; maximum4
tail diamonds per320px viewport. Includes three on Walker deck, one on Governor
deck, rest across dry approach/inter-machine spaces/quiet station approach.
Generated collectible header AND conservative column-top bounds include them.

Integrated12 small flower clumps:6purple/violet A behind player,6cream B in
front of feet. Native16pen palette unchanged. Approved coral concept maps to
violet; preserve blossom highlights during reduction instead of averaging away.
Clumps16..18px wide/11..13px high, rooted at201, near existing trees/shrubs.
No animation, no new Chip buffers. Six small foreground silhouettes reuse
inactive player stage;81-byte64px lookup skips distant occluders. Original
trees/shrubs/station group preserved; foreground source bitmap dimensions same.

Native executable288432bytes. ASAN/UBSAN actual collectible init/collect/
repeat/respawn tests pass;45active/12new and dry roots checked. Full-world sprite
staging/cache/blink tests and all-pixel mask oracle pass with bucket lookup,
both gates/reset/hazards, collision-byte and station-pixel parity pass.
Native flower preview reviewed at build/drowned-full/flowers-native.png.
No measured FPS claim. Musicv5 enabled, no renderdiag.

Current dist/Drowned-Level-020-HD/Drowned-Test staged73assets/55refs;
68official alpha.8 files unchanged. Previous full route preserved byte-exact
at dist/older-builds/Drowned-Level-020-H-old-004012. Proof in
build/drowned-full/proof.json; previous proof saved as proof-before-flowers.json.
Next: user's020 visual/gameplay review. No release/version/commit.

## 2026-09-22 — Full5120 route with varied vegetation staged

User accepted v12 vegetation/materials and authorized whole-level integration;
requested variety between both tree types and varied shrubs. Current manual
candidate dist/Drowned-Level-020-HD/Drowned-Test now starts the full route.
5120px total; prior route retained to3248, accepted1872px finale shifted+3248.
Core4888, station4872, endcamera4800 with40px player margin; final gate4528.
19spawn sites/20surfaces; original four active enemy slots unchanged.
New art across ground/platforms,14 extra trees (7spruce/7fir), variable sizes
and reflection,10 varied lower-leg shrubs. Existing station group exactpixels
preserved. Precision piers/ferry water kept clear; no vegetation animation.
Flowers concept v1 remains unapproved/unintegrated.

Full guard combines both gate interactions, jet hazards, checkpoint and Governor.
Ten canonical patches reuse610-byte Chip stage. Multiple static silhouettes
mask inactive attached player stages, preserving cache/master ownership.
Joined Spillwing clearance table retained byte-identical before finale; actual
geometry reference used in finale. No blanket loss of prior ferry precompute.
Longer foreground adds166400Chip bitmap bytes, rear extension31200:197600 total
(~193KiB) compared with3520px joined art. No additional animation/Chip staging.
No measured FPS/memory-headroom claim; manual020 boot/play remains pending.

Native build286004bytes. Host ASAN/UBSAN tests: actual full/isolated camera/Core,
Governor states, both gates and saved/unsaved reset, both original jet hazards,
checkpoint cases, whole-world sprite staging/cache/blink, per-pixel foreground
mask oracle. Collision bytes through3247 unchanged; station-group pixel parity;
dry tree placement and rear coverage checked. git diff --check clean.
Staged73assets/55literalrefs;68official alpha.8 files byte-identical. v12 archived
intact at dist/older-builds/Drowned-Level-020-H-old-002512. Proof:
build/drowned-full/proof.json. Musicv5 on; renderdiag off. No autoemulator,
campaign integration/release/version/commit. Next user full020 review; flowers
separate concept review and FPS investigation if needed.

## 2026-09-22 — Connection-loss recovery: full integration in progress

Latest user accepts v12 vegetation/platform/ground and requests whole-level art
and vegetation, preserving the station group; small colourful flowers proposed.
Full integration source exists: tools/build_drowned_full.py, drowned-full target,
SPARKPAW_DROWNED_FULL guarded renderer/geometry and foreground masks. Native
build/sparkpaw-drowned-full exists (281936bytes). Route5120, finale offset3248,
19spawn sites/20surfaces. Not staged or user-tested: dist remains Governor v12.
Important: current generator has12 extra trees but generated manifest still14;
regenerate/rebuild before staging (edit landed during generation). Nine shrubs.
Station group preserved by exact paste. Flower concept drowned-flowers-v1.png
exists, NOT approved/integrated. Record prompt/review in imagegen history.
Host occlusion/full-world tests restarted after connection loss; do not infer
success from interrupted tool sessions. Complete integration audit (camera/Core,
both gates, checkpoint/reset, patch routing, world memory), rebuild and stage
through stage_hd_test.py preserving v12 and alpha.8. No FPS acceptance yet.

## 2026-09-21 — Governor v12 vegetation integrated, user test pending

User played v11: ground approximately accepted, FPS investigation deferred.
Approved vegetation v1 now integrated into the quiet gate-to-station corridor:
64x136 spruce at1416 and48x96 fir at1552 behind player,32x20 shrub at1504
in front of lower legs. All rooted at201, overlapping the ground top by1px.
Static16pen foreground art; no extra Chip stage or animation.80byte immutable
shrub silhouette clips only the inactive player sprite stage during overlap.
Original gate clipping preserved; source sprites remain immutable.

Native Governor build passed. Actual sprite staging tested against a pixel
oracle under ASAN/UBSAN: both directions, cache reuse, blinking, inactive-stage
ownership and restoring after leaving shrub. Camera, Governor and Core visibility
host tests passed. No measured FPS claim; manual020 visual acceptance pending.
Current dist/Drowned-Level-020-HD/Drowned-Test is v12. Prior drawer archived
byte-exact at dist/older-builds/Drowned-Level-020-H-old-232853;68 official release
files unchanged. Music v5 on, renderdiag off. Whole-level distribution/joining
still follows acceptance of this focused art pass.

## 2026-09-21 — Ground cleanup staged; vegetation concept v1

User rejects v10 irregular ground/endcap pattern and orphan copperpost, prefers
large trees behind player and small trees/shrubs infront rooted toground. V11
removes extra post art and entire additional masking path/buffer; original gate
clipping remains. Floor uses one complete32px panel with regular joins and
continuous rail, not partial endcaps. Nativebuild and sprite-clipping hosttests
pass. Staged73assets/55refs,68officialreleasefiles unchanged, previousdrawer
archived byteexact. Ground/post screenshots catalogued Drowned-v10-ground-and-post-rejected-a/b.

First imagegen request hitusage limit; following user continuation succeeded.
Saved assets/concept/drowned-vegetation-v1.png and exactpromptTXT. Four distinct
spruce/fir/sapling/lowfern-heather silhouettes, rooted lowmoss bases. Review pending.
Source has soft backing/halo despite transparency request: remove before native
conversion, simplify fine detail into nativeclusters/currentpalette. No vegetation
runtime yet. No additionalanimation requested. Next native grounded preview then
safe foreground plant clipping only lowerlegs; avoid hazards/landings. Wholelevel
art/joining still follows finalslice acceptance. No release/commit/FPSclaim.


## 2026-09-21 — Governor v10 approved platform kit native proof

User approves platform-kit-v2 and authorizes static art first, one foreground
passage; later whole-level consistency/joining. build_governor_platforms.py crops
approved source, flood-removes edge-connected blue backing, nativepalette maps,
composes broad192x16 and144x16decks and footed32px supports at existing144/152tops.
Floor stays200..207, new quiet64x8 material tile. Reviewed native host views
platform-kit-native.png and foreground-passage-native.png in build/drowned-governor.
One18x56 static copper-bearing post1456,144 in quiet postgate corridor. Existing
gate sprite mask reused via translated coordinates for secondpost; mask352normal
bytes, no additional Chip buffer or animation. Extra sprite copying/clipping only
near posts or on leaving clipped cached poses. No measured FPS claim.

Expanded actual sprite-stage sanitizer test covers both configurations, second
post pixel oracle, movement/directions/cache/blink/inactive-stage and unchanged
masters. Existing camera/Governor tests pass. Native compile passes existing
warnings.73assets/55refs staged,68release files unchanged; previousdrawer archived
byteexact. User020 art/passage/smoothness pending. Narrowpier art and extra trees
not yet integrated; wholelevel application follows focused acceptance. Geometry,
mechanics, music unchanged. Current drawer Drowned-Level-020-HD/Drowned-Test.


## 2026-09-21 — Consistent platform kit concept v2

User accepts camera and station/boiler quality, rejects cheap repeated platform
fragments. Authorizes whole-level material consistency and occasional conifer
accents, with focused final-section art proof FIRST, then full-route application
and joining. Built-in imagegen material reference station-v3 creates concept
assets/concept/drowned-platform-kit-v2.png + exactpromptTXT. Broad steel walkway,
substantial copper-bearing posts, three narrow pier heights, ground ends/middle,
conifer. Concept pending approval, no runtime or dist changes. Preserve geometry;
raised end caps must flatten to collision top; ground source needs adaptation to
8px floor strip, not new tall ground. Native16pen conversion/masks and scale review
next, then short user020 proof before spreading kit across whole level.


## 2026-09-21 — Governor v9 camera keeps player visible

User v8 screenshot shows station but Sparkpaw offscreen. Confirmed source fault:
unconditional wanted1552 atplayer1376 outran stopped player. Existing v8test even
asserted that wrong outcome. Added desired-camera upper bound playerX-40 in
Governor only, retains5px ease and1552end cap. Player can stop or reverse without
camera leaving them behind. Test now holds300frames at trigger, pauses at every
3px approach step, reverses through trigger, and tests uninterrupted3px approach;
requires40px player margin and complete station framing before earliestpickup.
Sanitized actual-function test passes, native build passes existingwarnings.
73assets/55refs,68officialreleasefiles unchanged; prior drawer byteexact archive
recorded in proof-checkpoint.json. Screenshot/TXT preserved as
Drowned-Governor-v8-camera-outruns-player. User020 stop/reverse/pickup pending.
Other v8 platform/nozzle fixes not independently user-accepted yet. No FPSclaim.


## 2026-09-21 — Governor v8 end composition/nozzles/platform fixes

User supplied5screenshots, catalogued Drowned-Governor-v7-*.png/TXT. Confirmed
station too close to gate and Core delayed by camera>=1200visibility threshold;
geyser static lips omitted; user reports partial first-platform stance, exact
motion/root cause not established from stills. Station/Core moved+192 (1624/1640),
end bounds1872, camera endpoint1552 triggeredplayer1376. Same5px ease. Actual camera
simulation3px/frame reaches final composition before earliest pickup overlap.
Core visibility now normal viewport clipping pluscompletedGovernor, no camera
threshold. Pickup x1624..1656; jumpheightunchanged. Restored both approved static
lip assets x832/1088,y194 under animated patches. First platform top136->144,
16pxsolid cap/art/upperWalker surface aligned; samejump links/horizontal geometry.
This is a platform correction candidate, not proven cause from supplied stills.

Native build, actual camera/pickup and visibility/governor host tests pass;
73assets/55refs staged,68officialrelease files unchanged. Previous evidence and
build archived byteexact; proof-checkpoint.json recordsinventory. No emulatorrun,
FPSclaim,release or newart. User020 landing/sideapproach/nozzles/endcamera pending.


## 2026-09-21 — Rain Core packaging correction verified

First v7 staging accidentally selected production stormstone-core.spbm: stager's
additional-runtime-dir intentionally never overrides production assets. External
parity check caught mismatch. Fixed source to dedicated compile-guarded
PROGDIR:assets/runtime/rain-core.spbm (both load paths), generator emits own name.
Never modify canonical Level1 Core. Rebuilt/restaged with73assets/55literal refs;
all68release files unchanged. Incorrect staged drawer also archived intact.
Correct finalCore header/dimensions and mask byte parity verified against original,
colours differ deliberately; no new cache allocation. Full ending still pending
user020 camera/visual/pickup/results/replay test. Current proof-checkpoint.json
contains corrected inventory; previous entry's72asset/override statement superseded.


## 2026-09-21 — Native Rain station ending integrated (v7)

User approved sourcev3 station/tree and requested runtime integration. New
build_rain_station.py crops alpha, preserves aspect in224x176 envelope, native
Drowned16pens, thresholdedalpha128; grounds building at200 atx1432. Static art
added to existing3520-wide foreground, no extra Chipbuffer. Tree extends above
building. Blue/turquoise remap of original18 Core frames retains64x48 and original
mask bytes, existing cache allocation. Candidate-only stormstone-core.spbm override.
Host composition at build/drowned-governor/rain-end-scene.png, inspected.

Focused playable bound now1680; camera eases max5px/update to1360 once playerx1380
(beyond gatepost1364), clamped1360 otherwise. Core centred1448,hover112 aboveleft
pedestal. Visible only after all locks and43tick gateopening complete, camera>=1200.
Pickup gated by completion and box1432..1464,y120..159 (jump), existing50tick burst,
worldflash, sound, generic results/replay path. No campaign CONTINUE in this target.
No inherited secret1up. Station gauge/windows static in this pass; no separate
lens activation animation yet. Full joined level/campaign mapping remains later.

Actual-C camera/pickup tests, Governor completion/offset tests, renderer visibility
across3521positions in Level1/unfinishedDrowned/Governor pass. Core mask preserved,
SPBMforeground decode parity checked. Native020compile passes existing warnings.
No emulator launched, no FPS numbers claimed; musicv5, diagnosticsOFF. Staged
Drowned-Level-020-HD/Drowned-Test,72assets/55refs;68release files unchanged. Previous
v6drawer/evidence archived byte-exact per proof-checkpoint.json. User camera,
station/Core visuals, results/replay and020smoothness pending. No release/commit.


## 2026-09-21 — Rain station companion conifer review

User approves unique v2 station and requests a characteristic tree behind right
side, matching background conifers; each station should have its own tree/identity.
Built-in imagegen edit saved as assets/concept/drowned-rain-station-source-v3.png
with exact prompt TXT. Dark blue-green irregular mountain conifer behind tank/roof,
canopy above roof, warm windows and left instruments remain readable. V2 retained.
Concept review pending, no runtime/dist changes. Native conversion should preserve
building scale (roughly200x145) and allow tree to extend its envelope rather than
shrink building to fit the previous total bounds. Continue camera/Core/results
work after native art review.

## 2026-09-21 — Unique Rain weatherstation source v2

User rejects recoloured Level1 building; requests own Drowned station matching
original concept, same size and contrasting colours. Built-in imagegen uses lower
panel of drowned-finale-station-v1.png. Saved source and prompt at
assets/concept/drowned-rain-station-source-v2.png/.txt. Industrial steel hut,
copper rain tank/gutters, mast/rain gauge, amber windows, separate empty Core
pedestal. Source inspected; not native16pen reduction or runtime yet. Review
pending. Earlier station-v1 review explicitly rejected, retained. Existing
Governor v6 remains in dist. Next: native200x145 envelope conversion/composition,
then camera/reveal/Corepickup/results integration after visual approval.


## 2026-09-21 — Rain station native review, before integration

User accepts Walker approach and authorizes ending/camera/station/Core. Produced
assets/concept/drowned-rain-station-v1 via tools/preview_rain_station.py using
existing Level1 waystation source and18-frame Core family, current Drowned16pens.
Station200x145max, grounded200; Core same64x48 footprint/hover112, blue/turquoise
remap preserving silhouettes, idle and pickup frames. Host PNG/GIF only; not
emulator evidence. User requested new/changed art review before integration;
review pending. Existing v6 dist remains intact, no new runtime staged.

Confirmed Level1 station is baked foreground (not separate Bob); Core is Bob.
Camera eases toward fixed final composition at max5px/update. Proposed focused
proof extends gate exit into quiet final320px composition, station centred,
camera transition after player clears nearpost1346..1364 plus margin. Player
retains control. Need implement stage-specific camera endpoint, Core visibility
and pickup guard gated by completed Governor, and Rain results identity. Prevent
inherited secret1up and unintended campaign CONTINUE. SameHUD, no global palette
swap, no new stationBob/Chipbuffer required. Full-route mapping remains later.


## 2026-09-21 — Governor v6 compact aggressive Walker approach

User approves placing two aggressive Pump Walkers BEFORE Governor rather than
extending combat after reward gate. Prepended448px to focused prototype, arena
translated intact; collision bounds now1408px, asset allocation stays3520px.
Ground Walker surface96..416y200; upper192..384y136 with16px solid deck leaving
48px underneath. Same approved platform materials. Governor entrance, art,
patches, header, near-post mask, enemies and hazard/projectile coordinates shift448.
Governor APIs translate world coordinates; tests explicitly wrap local arena
assertions at new world offset. No production Level1/normal Drowned tuning change.

Walker speeds256/224 fixedpoint versus ordinary48/96/192; initial shot delays65/95,
subsequent90 versus150ticks. Existing shot telegraph/HP preserved. Four same-surface
jump links reuse320,-900,60 ballistic motion; all20launch pixels reach landing
windows in simulation. Two required Walker spawn sites added to four existing
arena sites, same four-slot active pool. No wave gate or mandatory kills.
Upper/lower platform transfers deferred; this first pass uses bounded patrol hops.
Native build, actual-C Governor offset/phase/hit/exit tests, Spillwing tests pass.
No native play or FPS claim. Musicv5, diagnosticsOFF.72assets/55refs staged;
68official release files unchanged. Previous drawer/proof archived byte-exact.
User020 aggression/smoothness pending. Station/RainCore and final camera scene
remain next, then full-route/campaign integration; no release/commit/push.


## 2026-09-21 — Governor v5 materials and enemy pressure

User accepts reused gate, rejects plain elevated platform and requests more enemies,
then explicitly2or3Spillwings. Reused approved precision-route deck/support pixels
at unchanged432..576/y152..168 collision cap, with same support positions. No new
palette or raster concept. Two Crabs now patrol112..336 and576..816; two Spillwings
use low576..816 and high288..704 flights with existing alternating speed/pause AI.
Existing four-slot enemy pool unchanged; all existing respawn policies retained.
Governor bypasses full-route precomputed clearance (wrong geometry here) and uses
existing actual-collision reference flight checks for deck/closedgate correctness.
This increases flight-check CPU work; no FPS claim without user test. Musicv5 and
machine cycles unchanged. Native build, Governor and Spillwing host tests pass.
Staged Drowned-Level-020-HD/Drowned-Test:72assets/55refs;68release files unchanged.
Previous drawer archived byte-exact, path in proof-checkpoint.json. Screenshot
preserved at testresults/Drowned-Governor-v4-platform-style.png with sidecar.
User020 difficulty/readability/performance pending; no diagnostics/log in musicbuild.


## 2026-09-21 — Governor v4 reuses existing sluice exit

User reports v3 reasonably smooth; screenshot rejects the placeholder arrow exit.
Preserved PNG and sidecar in testresults/Drowned-Governor-v3-placeholder-exit.
V4 copies accepted gate from slice x768 to832, reuses original14 gate atlas states
and80x61 transfer (same610-byte Chip stage). Last machine completion starts43tick
lift. Solid header x864..896 y120..136 remains after opening; leaf collision follows
original lift table. Near-post sprite mask moved from834 to898 only in Governor.
Header overlap guard enabled with relocated bounds; original level constants stay
first for offline asset generator parsing. No direct shot activation of finale gate.
Musicv5/mechanics unchanged. Native build and actual-C completion/lift/header tests
pass. Dist staged with72 assets/55 refs,68release files unchanged; prior drawer
archived byte-exact (path in build/drowned-joined/proof-checkpoint.json).
No FPS logging or measured FPS claim; user visual acceptance pending.


## 2026-09-21 — Governor native art integrated (user test pending)

User approved native tower preview. Isolated Governor v3 now stages the reviewed
steel/copper towers as static foreground with animated 32x64 port patches.
Port assembly lowered12px: visible low port y168..184 includes normal standing
projectile centre177. Seven opening poses and seven closing poses per damage
state; three hit lamps and disabled indication. Existing 100/75/55-tick exposure,
45-warning/55-vent, raised platform and one Turbine Crab unchanged. Platform now
uses steel/copper material. Exit is still a simple prototype shutter; pipe
connections, final station/Rain Core and full-route integration remain future work.

Native build and sanitized actual-C phase/hit/visibility/sweep/reset tests pass;
art indices verified for all three locks/damage stages over200ticks each.
40x1024 linked art bytes versus24x1024 before (+16KiB); same610-byte Chip staging
buffer, no new Chip allocation. Static foreground SPBM decode parity passes.
No emulator run or measured FPS claim. Musicv5 enabled, diagnostics/profiler OFF:
no renderdiag.log or mouse-save expectation. User020 visual/performance pending.

Current manual drawer: dist/Drowned-Level-020-HD/Drowned-Test. Stager verified
72 assets,55 executable references, all68 official release files unchanged.
Previous drawer/evidence preserved byte-exact at
 dist/older-builds/Drowned-Level-020-H-old-091958.
Proof: build/drowned-joined/proof-checkpoint.json; prior proof preserved as
proof-before-governor-native.json. No release, commit or push.


## 2026-09-20 — Governor native pixelart/shutter review

User accepts gameplay v2 and requests polished AGA pixelart. Produced versioned
RGBA tower source via imagegen using approved concept material direction;
assets/concept/drowned-governor-tower-source-v1.png/TXT. Tool returned alpha,
not magenta; preserved source. Converted in tools/prepare_governor_art.py with
one aspect-preserving scale to57x128 within80x128 native cell, current16pen
foreground palette, pen0 transparency, no dithering. Native SPBM decoded parity
passes. Full body mainly static; moving shutter and hit lamps remain wholly
inside existing32x64 lower patch. Deterministic review GIF opens split shutters,
shows exposed cyan port, extinguishes three lamps then subdued done indication;
all pixels outside that patch verified identical throughout animation.

Review assets in assets/concept/drowned-governor-native-v1: scene-4x.png,
tower-6x.png,states-4x.png,shutters-preview.gif,tower.spbm. Actual128high body
maps low top72 and high top24 onto tested ground200/perch152. Exact port/hitbox
alignment needs integration review (generous old24x32 hitbox versus narrower
visible port); do not claim final gameplay art match yet. No renderer,gameplay,
dist or audio change. Existing v2 remains staged. Art/motion review pending
before integrating, then static pipe connections/platform polish and own exit
art; Rain weatherstation follows separately. No native/FPS claim or release.

## 2026-09-20 — Governor v2 challenge pass

User reports initial mechanic works but asks whether it becomes difficult,
perhaps enemies, and authorizes continuation. Tightened exposed windows from
100ticks each to100/75/55(2.0/1.5/1.1seconds). Same45warning+55vent; no surprise
warning reduction. Adds one Turbine Crab spawn624..640, surface576..816,y200,
existing respawn policy. No inherited traversal links or other route spawns.
Governor encounter now arbitrates real enemy/panel sweeps by nearest hit and
routes damage to enemies before panel fallback. All outside proof compileguards
unchanged. Same test art, music,geometry and exit. Not final encounter acceptance.

Actual-C state tests cover each window boundary/cycle and reset; actual encounter
callbacks tested with enemy shim for left/right nearest ordering and enemy damage.
Native020 build passes. Staged same Drowned-Level-020-HD/Drowned-Test;
72assets/55refs,68releasefiles unchanged. Prior v1/log drawer preserved byte-exact
at older-builds/Drowned-Level-020-H-old-192207. Proof-before-governor-v2.json.
No renderdiag/noLMBsave. User judge pressure/fairness/clarity and enemy interference;
no automatic emulator,no measuredFPS claim,no release/commit/push.

## 2026-09-20 — Pressure Governor isolated gameplay proof staged

User finds art unclear and explicitly requests playable test, then says continue.
Built compile-guarded SPARKPAW_DROWNED_GOVERNOR test, make drowned-governor;
starts at ordinary x36 in a short dry arena bounded0..960. Existing3520 asset
allocation reused for isolation; real final layout/campaign not integrated.
Targets x256/y136,512/y88,768/y136; raised solid platform432..576,y152..168.
Ground200. Numbered32x64 test panels with closed slats, amber warning, exposed
cyan bullseye,3remaining-hit lamps and done checkmark. These are procedural
readability mockups, not approved finale pixelart. Same music v5/HUD/player/jet.

Each lock:45tickwarning,55tickvent,100tickexposed,3hits, sequential. Future/closed
panels consume bullets but take no damage. Full panel must be on-screen with
margin. Jets x384 (first2locks) and640(lastlock), existing64pxjet visual; safe
dry floor around jets. Third lock stops jets and opens solid exit864..896.
No Core,station,results,new SFX or enemy waves. Walking through E completes the
proof informally. Death resets locks and returns start; normal lives retained.
No actual new beacon/checkpoint needed within this short prototype.

Renderer reuses six compact patches and existing610Chipstage; generated
4*6*4*256=24576bytes normal-program art table. Full-column bounds0 for this
prototype prevent incorrect inherited upper-row skips. Static foreground has
matching raised platform. Collision conditional uses exact prototype geometry;
spawns/collectibles disabled and old water/boat/checkpoint moved or remain out
of reachable area. Main full-level paths behind guard remain preserved.

Actual-state ASan/UBSan test covers closed/exposed hits, hitcount/stages,both
sweep directions,offscreen rejection,warning/vent,exit/reset and solid bounds.
Native build passes; no emulator or actual movement/FPS acceptance claimed.
Artwork reviewed: corrected prototype base to darksteel pen8 (initial preview
used orange pen2). Generator remains deterministic, host pixels only.

Current dist/Drowned-Level-020-HD/Drowned-Test is ISOLATED Governor,not fulllevel.
72assets/55refs verified;68alpha.8files unchanged. Previous fulllevel+v5 drawer
preserved byte-identically at older-builds/Drowned-Level-020-H-old-184853.
Proof-before-governor.json preserves prior music state. No renderdiag.log or
leftmouse save; music owns CIA. User judge readability/timing/jumps/fun,stop/reset.
No release/version/commit/push. Concept machine art and real weatherstation remain
pending; do not mistake this explicit test-graphics proof for final art approval.

## 2026-09-20 — Music v5 accepted; finale/station concept review

User says music is good and requests continuation with level ending. Preserve
v5 as current music. Produced assets/concept/drowned-finale-station-v1.png/TXT
with imagegen using accepted pontoon scene for materials/palette. Two panels:
Pressure Governor (three low/mid/low locks) and separate small rain weather
station with Rain Core. Concept only; not native indexed art, not final collision
layout. Generated fox is scale reference, never new player art. No HUD or rear
replacement implied. Illustrated path gap/platform accessibility must be resolved
with actual physics; retain safe dry waiting place and second beacon proposal.
Prompt and pending review logged in IMAGEGEN_PROMPTS.md. Governor remains the
recommended finale direction; user has not explicitly resolved boss alternative.
No new runtime changes/build/dist mutation this turn. Current music drawer stays
playable. Next obtain concept/direction feedback, then native parts and geometry
proof; return sluices/independent instances and finale state follow endgame plan.

## 2026-09-20 — Undertow v5 continuous melody staged

User tested music and requests constant melody/tempo, finding the quiet passage
out of place. Tempo was already144BPM; v5 removes sparse opening/bridge and
lower-register breakdown, using warm v4 lead plus full drums/bass throughout.
Existing recurring motif and small later variation retained; no bright v2 lead.
Generator asserts at least8percussion,6bass,5lead notes per bar. Empty fourth
track, sizes, duration106.58sec and no host clipping checks pass. Sample bank
byte-identical21622bytes, score17468bytes. Prior v4 preserved.

Current Drowned-Level-020-HD/Drowned-Test has only rain-score.bin and ReadMe
changed versus played v4 drawer; executable and all other assets byte-identical.
72assets/55refs,68releasefiles preserved. Prior drawer archived intact at
older-builds/Drowned-Level-020-H-old-112210. Music target now copies v5 score/bank
from source to build assets for reproducible future staging. No native rebuild
needed for this score-only edit. No renderdiag.log; CIA remains music-owned.
User audible approval pending; no FPS claim, release,commit or push.

## 2026-09-20 — Undertow Circuit v4 integrated in Drowned music test

User approves v4 for now and requests in-level integration before confirmation
runs. New make target drowned-music links existing three-channel ptplayer and
AUD3 two-effect mixer with joined level. Rain score17468Fastbytes,bank21622Chip
plus224Chip mixer buffers (allocator/code/effect-copy overhead additional).
Compile-selected Drowned track load validates exact sizes and signature; other
tracks retain prior paths/sizes. Drowned effect4 uses pump-shot.raw; checkpoint
appended16 with priority10,volume58,cooldown32 and AUDIO_MIX dispatch before any
Paula1 direct write. Existing IDs/priorities preserved. No per-frame track lookup.

Music build deliberately omits SPARKPAW_RENDER_DIAGNOSTIC and all CIA profiling;
level1_audio.c ownership guard stays intact. NO renderdiag.log or LMB log save in
this drawer. Runtime smoothness is subjective until compatible cadence-only
instrumentation is separately designed. Life loss retains existing song lifecycle;
user should verify checkpoint, damage/shots, water, pause/resume and upper route.

Native020 build passes (initial unresolved gameStormrailActive in isolated target
fixed by compile-time FALSE selection before staging). Host actual backend
lifecycle runs both ordinary and Drowned variants, all allocation failures,
checkpoint mapping/priority/cooldown,24starts/stops each and ownership cleanup.
Mixer oracle40000operations/13225tails passes. No emulator launched.

Staged dist/Drowned-Level-020-HD/Drowned-Test with72assets/55literalrefs;
v4 split files exact byte parity.68alpha.8releasefiles unchanged. Prior SFX-only
cadence drawer/log archived byte-identically in older-builds/Drowned-Level-020-H-old-111058.
Proof-checkpoint.json identifies music/no diagnostics; prior proof-before-music-v4.json.
README explicitly says reset/stop after playing, no mouse log or Workbench promise.
Music/art/gameplay native approval and memory peaks pending user run. Finale not
implemented; proposal remains in DROWNED_ENDGAME_IMPLEMENTATION.md. No release,
version change,commit or push.

## 2026-09-19 — Undertow v4: distinct level-specific theme review

User perceives title-melody resemblance and asks original,cool,catchy music that
fits Drowned Turbines. V3 did not import title notes; a note-only MOD audit finds
at most3 consecutive pitch indices shared with any Neon Sky channel. This does
not refute perceived similarity (timbre/harmony/rhythm/sample tuning omitted).
Do not dismiss the feedback or claim algorithmic originality proof.

Created sibling music/drowned-v4: new E-minor harmony over Em/Em/C/Bm/Am/C/Em/B,
syncopated recurring motif with falling fifth response, new bass/kick rhythm,
low-register bell variation and quiet bridge. Warm non-looping reed retained;
no bright sustained v2 lead.144BPM64bars106.58sec,3voices,fourth empty;
same21622Chip samplebytes and17468Fastscore. Full/theme micromod previews.
Sample/channel/duration/no-clipping validation passes. Title-note audit JSON
retained as limited source evidence, not listening acceptance. No runtime or
dist change. Musical review pending; existing finale decision remains open.

## 2026-09-19 — Undertow v2 rejected; v3 warmer middle ground

User rejects v2 as too bright/shrill and prefers v1. Preserve both; do not
integrate v2. Created music/drowned-v3 from v1, retaining drums/bass/atmosphere.
New recurring eight-bar melody with fewer notes and more space; non-looping
warm reed with gentle attack, fundamental-dominant spectrum and no FM, instead
of v2 sustained harmonic lead. Main melody stays at/below A4. Same144BPM,
64bars,106.58sec,3voices, fourth empty. Bank21622Chipbytes,score17468Fastbytes.
Generator length/channel/duration/no-clipping assertions pass. Host micromod
full and27sec theme previews ready; subjective approval pending. No native or
runtime/dist changes. Finale choice still open; music audition is current task.

## 2026-09-19 — Undertow Circuit v2 melodic review

User requests more epic/catchy/melodic music. New sibling music/drowned-v2
preserves v1. Repeated8bar call/answer hook, sustained harmonic lead with clear
breaths, stronger bass rhythm, bell bridge and returning theme.144BPM64bars,
106.58sec;27sec chorus preview. Three voices,fourth empty. Bank20810Chipbytes,
score17468Fastbytes; new looped lead1012bytes. Host micromod generator checks
loop bounds, sample lengths, reserved channel, duration and no clipping pass.
Not listened/accepted by user or tested in native ptplayer; no runtime integration
or dist changes. Finale choice from previous turn remains open.

## 2026-09-19 — Undertow Circuit review and finale implementation proposal

User redirects priority to original level music and planned ending. Produced
music/drowned/generate.py, original144BPM64bar three-voice MOD,106.58sec full
micromod WAV and27sec theme extract. Score17468Fastbytes,bank19798Chipbytes;
reserved fourth channel empty, sample lengths/duration/no-clipping checks pass.
Host rendering only; composition audition and native balance/FPS pending.
Project .venv lacks numpy; generated with bundled Codex Python runtime.
No runtime integration or dist changes yet. Current optimized SFX-only build
and previous evidence preserved. No release/commit/push.

DROWNED_ENDGAME_IMPLEMENTATION.md reconciles current3520 route with proposed
4480world: preserve boat2400..3200, return sluices3328..3712, proposedGovernor
3712..4224, separate station/Core4224..4480. Original machine-versus-mobile-boss
choice was still open; asynchronous user question asked, no answer at writing.
Do not infer machine acceptance from this proposal. New art still concept-first.
Recommend second existing-style beacon before finale; detailed reset/award states
and performance gates documented. Earlier60–90sec machine arena superseded by
later25–45sec traversal-based design proposal.

Integration concern found: joined target currently has no gameplay music flag;
checkpoint directly writes Paula1 and must route through effect mixer before
music enabled. Existing level1AudioLoad validates only two exact track sizes;
new track selector, mappings and lifecycle tests required. Music raw payload
37266bytes exceeds original12–24KiB disk aspiration; no packing/ADF claim.
Next user audition, then dedicated music+SFX native candidate; finale choice and
concept review before final-area integration.

## 2026-09-19 — Patch hoist first cadence run: encouraging, shorter ferry sample

Patch invariant hoisting first run, reviewed 2026-09-19.
Exact staged executable verified against proof-checkpoint.json; complete log,
minimal regional cadence, profiler OFF. Established FS-UAE HD 68020,
2MB Chip/8MB Fast. User says played; exact route and visual verdict unspecified.
4527 intervals:4497one,30two,0threeplus,max2;49.67FPS,ownership0.
Lateferry297intervals,7misses (2.36%),48.84FPS versus prior minimal column
run542intervals,37misses (6.83%),46.80FPS. Firstferry178intervals50.00FPS,
versus512intervals49.70FPS. Land49.63,precision49.92,approach/shore50.00.
No60ms intervals thisrun. Chip414336free/largest413304.
Material workload difference: lateferry sample about half the prior run;
land2982intervals versus1027. Whole-run FPS not a fair causal comparison.
No proof that microoptimization alone produced regional improvement, nor
that second upper-route passage is fixed. Candidate retained provisionally.
Prior evidence: Unassigned-Drowned-column-tops-run1.log. No new build.

## 2026-09-19 — Water sync profile and patch invariant candidate

Preserved Unassigned-Drowned-buffer-split-run1.log/TXT, exact executable verified,
complete footer. User upperroute, waterdeath/checkpoint, upper again; second felt
heavier. Aggregate profile does not separate passages. 2964 intervals,57two,
0threeplus,49.05FPS;late46.57 (40/544 misses),ownership0. Profile observer cost;
not a minimal-cadence speed verdict. 491/544 profile frames upperY<140.
CIA averages: dynamic sync3.23ms (p954.26), roll0.20ms (p951.33), initialwait
0.025ms, compact3.49ms, canonicalwater0.75ms, game4.86ms, Bob11.04ms.
Bob maximum37.40ms is an isolated sampled duration, not proof of a specific
cause. Parent/child scopes are sampled on different frames, not additive.

Changed only generic Drowned canonical patch copier: compute source/destination
row offsets once per rectangle instead of once per plane/chunk; move invariant
DMA setup outside plane/copy loops. Safe setup wait added per clipped chunk;
all existing copy/final waits, pixels, four planes and three copies preserved.
Reference setup macro retained. No memory growth or visual/gameplay reduction.
Native probe build/drowned-patch-setup-audit/hoisted.s confirms both multiplies
before loops; previous candidate.s retains compiler comparison. This is a modest
CPU-overhead hypothesis, not a reduction of water DMA bytes or proven FPS gain.
Actual CPU/referenceDMA/candidateDMA tests pass 48720 rectangles each; merged
water helper passes133120 cases;277 authored mechanism transitions pass.
Native normal020 build passes existing warnings only. Runtime acceptance pending.

Drowned-Level-020-HD/Drowned-Test now normal minimal cadence, profiler OFF.
70 assets unchanged,51 compiled refs verified,68 release files untouched.
Previous profile/log archived byte-exactly at older-builds/Drowned-Level-020-H-old-201416.
proof-before-patch-hoist.json preserves predecessor. Next compare with minimal
column-tops run1 (late46.80FPS), not profiler FPS. User repeat upper/checkpoint/upper,
LMB once/release,wait15sec/reset. Watch water,gate,geysers,beacon for glitches.
No emulator launch, release, commit or push.

## 2026-09-19 — Buffer split profiling staged after column gain

Continued investigation: water synchronization already merges adjacent dirty
strips and skips unchanged phases. Three physical ring copies remain required
by the current renderer contract. The previous compact-target profile conflated
initial pending DMA wait, ring roll and dynamic water sync, so no unmeasured
copy-elision or visual reduction was introduced.

The focused ferry profiler now samples eight scopes: game update, total Bob pass,
compact target, canonical water, ring roll, ring dynamic sync, initial compact
Blitter wait, enemy draw. One timer pair per eligible frame, same 17-frame cycle,
player x2800..3199. Log marker buffer_split=1. Roll includes no-scroll calls;
initial wait covers only the pre-compact wait, not every renderer wait. Dynamic
sync includes its copy completion waits. Parent/child costs are sampled on
different frames and cannot be added. This measures remaining work after the
shortened-column change; profiler cadence is NOT an optimization verdict.

Actual selector/macro test passes 3200 frames, checks selected new scopes,
boundaries and exactly one pair per eligible frame. Actual regional cadence
lifecycle/mutation test passes. Native 68020 profile build passes with existing
warnings only. Normal build gains no new instrumentation (wait wrapper guarded).
Drowned-Level-020-HD/Drowned-Test is now this profiler. All 70 runtime assets
unchanged, 51 literal references verified; 68 alpha.8 release files unchanged.
Prior minimal-column build and user log preserved byte-identically in
`dist/older-builds/Drowned-Level-020-H-old-195037`; prior proof saved as
build/drowned-joined/proof-before-buffer-split.json.

User test: upper late ferry, water/checkpoint, upper again, preferably 30-60 sec
in this area. LMB press/release, wait 15 sec, reset. Next use split costs to select
one causal optimization, then return to minimal cadence for the FPS comparison.
No gameplay/art change, emulator launch, release, commit or push.

## 2026-09-19 — Shortened columns: first cadence result

 3139 intervals: 3098 one-field, 39 two-field, 2 three-plus, max3;49.32FPS.
0 ownership violations. Late ferry:542 intervals,37 misses,0 three-plus,
46.80FPS;6.83% intervals miss20ms versus87/601=14.48%,43.67FPS in
Unassigned-Drowned-water-batch-run1.log (same minimal instrumentation).
First ferry49.70 versus48.93FPS. Land,precision,shore50.00FPS thisrun.
Two60ms intervals: one approach,one firstferry. Cause not inferable from
aggregate counters; checkpoint/loading causality not established.
Different manual workloads: promising measured association, not controlled
causal gain or final acceptance. Profiler41.40FPS is not a valid baseline.
Chip414336free/largest413304; Fastprepared5184896/largest5183816.
Checkpoint audio1/1,water2/2,Core0,extraLife0.
Retain candidate provisionally. Remaining focus: lateferry target water-sync
traffic and pending DMA waits; distinguish roll from sync if profiling again.
No additional build or runtime changes made during this evidence review.

## 2026-09-19 — Shortened scrolling columns candidate staged

The upper-route profile points to compact-target buffer preparation (average
3.90 ms, p95 6.93 ms including pending DMA waits), rather than enemy AI alone.
The joined renderer now skips guaranteed blank upper rows when replacing one
16px ring-buffer column. It takes the minimum bound of the evicted and incoming
world columns; actor restores precede rolling. Generated bounds cover actual
foreground pixels plus water, mechanisms, beacon and collectible footprints.
All four planes, three physical copies and DMA completion waits remain intact.
Initial/full resets retain full copies; invalid positions fall back to full height.
SPARKPAW_DROWNED_FULL_COLUMN_REFERENCE retains the reference implementation.

The generated 220-byte table needs no extra Chip allocation. Representative late
ferry exchanges skip an average 149 of 208 rows (about 72% less column pixel
traffic, NOT a measured frame-time or FPS gain). Water target sync remains work.
Actual helper and prototypeRollTarget DMA oracle tests pass 1788 cases with
sanitizers, forward/backward scrolling, dynamic rows and guarded buffers. Existing
60-column full-height test passes. Native 68020 build and compiled probe reviewed:
row-offset multiplies are outside the DMA loop. Runtime FPS/visual acceptance is
pending; previous speculative optimizations remain provisional.

Drowned-Level-020-HD/Drowned-Test now contains the normal minimal regional cadence
build, targeted ferry profiler OFF. All 70 runtime assets match the preceding
candidate, 51 executable references verified, all 68 alpha.8 files unchanged.
Prior profiler drawer and log archived byte-identically in
`dist/older-builds/Drowned-Level-020-H-old-193411`.
Proof: build/drowned-joined/proof-checkpoint.json; prior proof-before-column-tops.json.
Repeat upper route, deliberate water death/checkpoint, upper route again; inspect
scroll edges and restored actors/diamonds. LMB once/release, wait 15 seconds on
frozen image, reset. Compare next cadence with minimal water-batch evidence, not
the profiler's observer-contaminated FPS. If gains are insufficient, separate
remaining target water-sync traffic from roll cost before another candidate.
No emulator launch, release, commit or push.

## 2026-09-19 — Late-ferry upper-route profile identifies buffer work

Preserved Unassigned-Drowned-ferry-profile-upper-twice.log/TXT; exactstagedexe
verified, completefooter. Userupperroute,deliberatewaterdeath/checkpoint,upperagain.
544lateframes,501upperY<140(92.1%). Late41.40FPS/113doubleframes isobserver-
contaminateddiagnostic,notperformanceacceptance.0ownership,0threeplus.
CIA avg:game4.98ms;displayupdate0.65ms;totalBob11.06ms(p9514.01);
compacttarget3.90ms(p956.93);enemyupdate1.30ms;enemydraw1.21ms;
enemyrestore0.70ms;canonicalwater0.78ms. Parent/childnotadditive, different
sampledframes andpendingBlitterwaits. Strongestmeasuredsubsystemlead iscompact
bufferpreparation(scrollroll+water sync+initialwait), notenemyAIalone.
FullcolumnDMAstill208rowsx4planesx3copies; canonicalwaterbatchdoesnotremove
triplicatedtargetsync. Nextfocusreducebuffertraffic/guaranteedblankrows orsplit
roll/synctiming. No speculativevisual/culling shortcuts. Profilerdrawerunchanged.
Chip414336free/largest413304;water1/checkpoint1audio;Core/1up0. Source/evidence
analysisonlythisturn,no runtimechange ornewbuild. Usercheckpointreturnreported.


## 2026-09-19 — Late-ferry rotating profiler staged, not FPS candidate

Waterbatch userlog preserved Unassigned-Drowned-water-batch-run1.log/TXT.
3273intervals,120two,0three-plus,48.23FPS,ownership0. Firstferry48.93,
last43.67(87of601misses); user reports littlelateimprovement. Chip411744free,
largest410312. Worst snapshotcamera2802,3smallfamilyBobs,2shots,2collectibles,
6waterupdates; no single-snapshot causality. Currentwaterbatch gainsunproven,
retained provisionally for profiling. Minimal baselineproof-before-ferry-profile.json.

New target drowned-joined-profile, SPARKPAW_DROWNED_FERRY_PROFILE, current
Drowned-Level-020-HD/Drowned-Test. Onlyplayerx2800..3199: select1of8scopes each
frame,17frame cycleavoids poweroftwoanimation phasealias. Scopes gameupdate,
displayupdate,totalBobpass,enemies,compacttarget(sync+roll),canonicalwater,
enemyrestore,enemydraw. Maximum1timerpair/frame; aggregateupperplayerY<140count.
Not separateupper/lower timing distributions. Parent/child samples fromdifferent
frames, notadditive. Draw scopes include precedingpendingDMAwaits; avoid equating
CPUorBlitterexclusively. Nonzeroobservercost; do NOT compareFPS asacceptance.
Actualselector/macrotest3200frames passesboundaries/nesting/timerpairlimit.
Actualcadenceentrytest/mutation passes. Native020buildpassesexistingwarnings.
Normalminimalbuild retainsdisabledinstrumentation. Noart/gameplaychanges.

Nativecompileinitiallyrejectedpreprocessordirectivesinsideopenmacroargs; fixed
withcompleteguardedcalls andrebuiltsuccessfullybeforefinalhandoff. An intermediate
olderprofilerbinary wasbrieflystagedduringverification thenarchivedand replaced;
notuseraccepted. Authoritativeproof-checkpoint.json now finalsuccessfullybuilt
profiler. Finalpriorarchive:/Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-192118. Originalplayedminimaldrawer/log remain
intact in dist/older-builds/Drowned-Level-020-H-old-192013.
70assets/51refsverified,68releasefilesunchanged. Userplay30-60sec inlateferry,
upperroute/fighting/return; LMBsave,wait15sec/reset. Nextchoosecausaloptimization
fromscopeevidence. No emulator/release/commit/push.


## 2026-09-19 — Canonical water batching + first precision Spillwing

User requests firstprecision flyer andbroaderFPSthinking. Latest frame-address
run preserved Unassigned-Drowned-frame-address-run1.log/TXT plus requestPNG/TXT.
3210intervals,72two,0three-plus,48.90FPS,ownership0. Firstferry46.69,last43.19,
not measuredgain over prior47.40/45.90; userfeelsbetter butlateupperroutestutters.
Short/differentworkloads, earliermicrooptimizationsremainprovisional.

Larger candidate: src/drowned_water_batch.h prepares6exact80pxrepeat tiles per
frame/plane. Extra42240Chip bytes, no new diskassets. Same16frames/25Hzphase,
samepixels/bankramps/bubbles; contiguousdirtyvisiblecanonicalstrips groupedmax6.
Up to24canonicalDMAcommands become4; same number of copiedpixels, fewer setups/
launches/waits. Target water synchronization unchanged. Reference macro
SPARKPAW_DROWNED_WATER_BATCH_REFERENCE restores old canonicalpath.
NoFPSclaim. PriorfreeChip456576 implies~414336beforeallocationoverhead; verifylog.
Cache freed/reset onrelease. ActualC DMApixeloracle25728cases passes all16frames,
continuous/gappedlayouts,partialdirty,culling,repeatnoop. Native020buildpasses
existingwarnings. No quality/enemy reductions.

Opening flyer appendedID12 at1488,lowprofile,patrol1328..1552. Wider1648patrol
failedactualsolidavoidancetest duehoveredge; narrowedbeforehigherpillarpasses.
ExistingID11precision/ferryIDs unchanged. Same4activepool,13spawns. ActualC joined
sanitizer testpassesall13spawns/families,flyersolids,jumps/checkpoint.

StagedDrowned-Level-020-HD/Drowned-Test,70assetsbyteidenticalto baseline,
51refs/68releasefiles unchanged. Previousdrawer/logarchive:/Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-191155.
Proof-checkpoint.json current; proof-before-water-batch.json prior.
User gatebothferryhalves/upperroute,waterseams/boatwaterline,Chipheadroom,FPS.
Newenemy affects precisionworkload; ferrygeometry/enemyprofiles unchanged.
No emulator/release/commit. Tests added toMake hostsuite.


## 2026-09-19 — Enemy frame address precompute candidate

Patch-setup run preserved Unassigned-Drowned-patch-setup-run1.log/TXT:
3278intervals,67two+2three-field,48.93FPS,0ownership. Firstferry47.40vs45.95,
last45.90vs44.85 previous, butdifferentworkload; not controlledgain. Two60ms
frames(approach/firstferry) are open concern, setup candidate provisional.

User asks furtheroptimization/precompute. Added joined-only frame pointer table
src/drowned_enemy_frames.h:3families x2facings x32slots x2pointers=1536normalBSS
bytes, zeroextraChip. Prepared after each cacheallocation, addresses refer to
same Chip bits/masks; framecounts capped32. Used forCrab/Spillwing draw and
residentWalker selection. ReferenceSPARKPAW_DROWNED_FRAME_ADDRESS_REFERENCE.
All140validframe pairs tested withASan/UBSan againstoldlayout, repeatedprepare,
lastwordwrites andlimitguard; existing24pxcachepixeltest passes. Native020build
passes existing warnings only. VBCC020probe inbuild/drowned-frame-audit shows
lookup uses shifts/pointerloads, replacing layout multiplications; thisdoesnot
measure wholeframeFPS or reduceBlittertraffic. Sameassets/behavior/diagnostics.
StagedDrowned-Level-020-HD/Drowned-Test,70assetsbyteidentical,51refs,68release
filesunchanged. Priorarchive:/Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-190131. proof-checkpoint.json current,
proof-before-frame-address.json prior. User020cadence/visualgatepending.
Noemulator/release/commit. Checktwo60msframes andbothferryhalves next.


## 2026-09-19 — Patch/water setup reuse candidate, 020 gate pending

Latest evidence Unassigned-Drowned-precision-flyer-run1.log/TXT preserved and
verified. User accepts precision flyer/no perceived extra drops.2737intervals,
76two-field,0three-plus,48.64FPS,0ownership. Precision49.36(6/463misses),
firstferry45.95(17/193),last44.85(28/244). Short ferry samples, not controlledAB.
No shore samples. Both ferry sections remain concern.

Inspected enemy copy-on-unload (already default), Spillwing precomputed collision
(already default), resident caches, pontoon and water paths; retained them.
Concrete redundant work: drownedCopyPatchRect rewrote6constant Blitter words
for everyplane/copy,12times per clipped chunk. Now onlyfirstplane/firstcopy;
66registerword writes removed perchunk. Precompute bltsize once. All DMA waits,
4planes,3copies, pixel spans and strides retained; no new allocations or art/
physics/enemy changes. SPARKPAW_DROWNED_PATCH_SETUP_REFERENCE retains old setup.
VBCC+aos68k -O2 -cpu68020 actual-header probe assembly in
build/drowned-patch-setup-audit/{reference,candidate}.s confirms branches skip
setup and cached size is written froma5, not repeated shifts/stack intermediates.
This is reduced work evidence, NOT measured CPUtime/FPSgain.

ActualC ASan/UBSan pixel oracle:48720rectangles EACH CPU/referenceDMA/candidateDMA,
including160/320/512waterwidths andwrap/clips. Integratedwater133120cases pass,
pixel/phase parity and DMAwait/repeatnoop behavior. Native020 build passes existing
main/ready_ui warnings only. Added missing patchheader Make dependency, ensuring
actual candidate rebuild. No new timing scopes; same regionalminimaldiagnostic.

Staged dist/Drowned-Level-020-HD/Drowned-Test; all70runtimeassets byte-identical
to playedbaseline,51literalrefs covered,68officialreleasefiles unchanged.
Previous drawer/log archived byte-exact at /Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-184938.
proof-checkpoint.json current; proof-before-patch-setup.json baseline.
User gate: bothferryhalves withshoot/jump/return, waterseams/residue andcadence;
saveLMB/freeze/wait15sec/reset. No emulator, release, commit or push.


## 2026-09-19 — Late-ferry drops confirmed; precision Spillwing staged

Same-build repeat preserved: testresults/Unassigned-Drowned-regions-v2-run2.log/TXT,
plus precision request PNG/TXT.5310intervals,161two-field,0three-plus,48.52FPS,
0ownership. Firstferry46.62FPS/29of401misses; last41.68/101of506(19.96%).
Both halves need investigation; prior last49.43 does not establish healthyfps.
User-requested precision Spillwing appended asID11 at1840,patrol1728..2144,
high profile. Ferry IDs/profiles untouched; same4activepool, existing art and
precomputed collisionclearance. Actual joined C tests pass12spawns, flyer solid
avoidance, jumps and checkpoint resets. Inherited Level1 1up gameplay/render
now disabled in Drowned; renderer tests confirm Level1 behavior unchanged.
Native020 build passes existing warnings only. No FPS fix claimed.
Current dist/Drowned-Level-020-HD/Drowned-Test staged70assets/51refs;
68releasefiles unchanged. Prior drawer/log preserved byte-exact at /Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-183827.
proof-checkpoint.json current; proof-checkpoint-regions-v2.json prior.
User precision encounter feel/FPS gate pending. No emulator/release/commit.


## 2026-09-19 — Regional cadence attribution v2 staged

User offers to wait for improved log; repaired prior-player-X capture BEFORE
memset of diagnosticCurrent. Existing global cadence algorithm unchanged.
Log joined header now has region_attribution=2. Full actual entry-function test
(test_drowned_cadence_entry.py) covers all region boundaries both directions,
first sample,1/2/3field intervals and ferry->checkpoint teleport; regional totals
match global totals. Sanitizers pass; mutation restoring original fault is
rejected. Native020 build passes with existing warnings only. No gameplay/art
change; checkpoint and no-Core fixes retained. Runtime regional evidence pending.
Restaged Drowned-Level-020-HD/Drowned-Test;70assets/51literalrefs verified,
68release files unchanged. Previous no-Core drawer archived intact at
/Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-182814. proof-checkpoint.json current; proof-checkpoint-nocore.json previous.
User next run should save once with LMB, frozen image expected, wait15seconds.


## 2026-09-19 — Remove inherited Core from unfinished Drowned shore

User spotted a Core near the ferry exit. Source confirmed inherited Level1
renderer Bob at3232; Drowned pickup predicate was already FALSE. Guarded
coreRenderVisible for all Drowned slices; Level1 remains unchanged. No authored
Drowned finale exists yet. Actual-function test passes3521camera positions in
both modes; native020 build passes with existing warnings only.
Restaged Drowned-Level-020-HD/Drowned-Test, user play pending. Previous checkpoint
candidate and current log preserved byte-exact in /Users/mpoelstra/Projects/amigagame/sparkpaw/dist/older-builds/Drowned-Level-020-H-old-182507.
70assets/51literalrefs checked; all68official release files remain byte-identical.
proof-checkpoint.json now identifies this follow-up; first proof preserved as
proof-checkpoint-first.json. No FPS/gameplay changes or new performance claim.


## 2026-09-19 — Approved beacon + sound v2 integrated; 020 test pending

User approved sound v2 and requested integration with the approved beacon.
Current active drawer: sparkpaw/dist/Drowned-Level-020-HD/Drowned-Test.
Native3520px joined candidate includes checkpoint at2320,152, fixed-pivot
8-frame48x48 marker, amber->cyan lamp and approved5286-byte period322 sound.
Body/pennant derived from imagegen parts-v1; backdrop excluded by explicit
silhouette, one rigid offline pivot, existing foreground16pens. No runtime
rotation/alpha blending. Own9216-byte Fast planar atlas, no new Chip stage;
existing610-byte stage reused; sound adds5286 Chip bytes plus allocator overhead.
Actual prepared free memory/cadence and animation/audio feel await user log/play.

Grounded touch triggers once without reward/refill. Life loss respawns2320,161,
camera2176, full normal health,75ticks protection. Boat resets2400/waits;
opened/opening gate retained, score/collected diamonds/enemy reward flags retained,
old Bob restore histories retained. Fresh attempt clears checkpoint/gate.
Crab approach patrol shortened2192..2288, spawn2240..2256 to reserve safe marker.
Joined camera end-lock3072 fix and120-byte per-region cadence counters included;
not a claimed FPS optimization. Existing water batching/pontoon lookup retained.

Native68020 compile passes (only existing main/ready_ui warnings). Actual C
host tests with sanitizers pass:19600activation cases in each mode, regional
counts, joined/legacy camera, actual game reset plus real player/mechanism/
enemy/projectile/collectible modules; boat-middle-boat, four upper jumps,
11spawns/3families and bounded frames. Renderer/audio native feel not host-proven.
Native frame sheet inspected; atlas decoder parity and approved raw byte parity.

Staging verified70assets/51compiled literalrefs. All68official alpha.8 release
files byte-identical. Previous played drawer including log archived byte-exact
at dist/older-builds/Drowned-Level-020-H-old-181929. New authoritative candidate
proof: build/drowned-joined/proof-checkpoint.json (proof-joined.json historical).
No emulator launched; no release, commit or push. Ask user to activate, die on
ferry, check saved lamp/respawn/boat/progress and save log once with left mouse;
image intentionally freezes, wait15sec then reset. Regional FPS evidence pending.


## 2026-09-19 — Checkpoint beacon approved, sound revised

User approves the beacon concept. User finds v1 sound too similar to1-up and
agrees to latch+soft hum+one confirmation tone, no voice. Generated audition
assets/audio/checkpoint-v2/checkpoint.wav/raw via build_checkpoint_sound_v2.py:
~0.48sec,5286bytes,period322,peak104, WAV/raw byte parity verified. V1 preserved.
Existing1-up generator uses four rising notes659/784/988/1319Hz plus high tail;
v2 uses inharmonic click,147Hz hum,one fixed880Hz ping, no note sequence.
Sound subjective review pending; no claim of listened verification.
Beacon direction approved; native fixed-pivot poses/animation and actual
checkpoint game/reset/render/audio integration remain to complete. Dist remains
played joined build; prepared camera/regional changes not yet staged.

## 2026-09-19 — Checkpoint priority + regional FPS preparation

User reports repeated deaths and late-ferry drops, requests checkpoint and FPS
investigation. Played joined log verified/preserved:7100intervals/108two/0three+,
49.25FPS,0ownership violations,Chip461864free/largest460512. Aggregate masks
section-specific cost; do not dismiss user report. Evidence
testresults/Unassigned-Drowned-joined-49-25fps.log/TXT.

Prepared camera correction: joined build inherited fixed Level1 end-lock at
playerX3072, inside ferry. Joined now retains centered/clamped follow there;
legacy behavior unchanged. Actual-function host test passes. Not claimed FPS
fix. Added minimal previous-player-X cadence counters for land/precision/
approach/ferry_first/ferry_last/shore;120bytes, no new timer reads or frame I/O.
Header stale pontoon basin corrected. Source native build succeeds, not staged.

Checkpoint concept image assets/concept/drowned-checkpoint-beacon-v1.png,
full prompt TXT; steel/copper mechanical marker, amber->mint lamp, hinged
metal pennant. Pending user concept approval; native art must fix intermediate
hinge and omit broad presentation glow. Sound audition
assets/audio/checkpoint-v1/checkpoint.wav/raw:6828bytes,period322,~0.62sec,
mechanical click+rising tones, no voice; not runtime integrated.

Checkpoint state module prepared but NOT linked/wired: grounded one-shot
activation at2320,32tick activation, persists life loss, clears fresh attempt.
19600host contract cases pass in both test modes, plus regional boundaries/
totals and joined/legacy camera. Integration/respawn/render/audio still pending.
Plan safe Crab patrol adjustment and preserve score/diamonds/gate while boat
resets to left bank. Read docs/DROWNED_CHECKPOINT_REVIEW.md before continuing.

Current dist remains exactly the played joined build; all68release files
hash-identical. No new user test requested until checkpoint concept approval
and actual native art/animation/integration. Built build/sparkpaw-drowned-joined
now contains prepared camera/regional changes and differs from staged proof;
do not misidentify it as the played build. No release/commit/push/emulator run.

## 2026-09-19 — Connected Drowned level candidate (3520px)

User asks to connect all existing content, then explicitly continues. Built
a single3520px route: accepted land0..2400, translated ferry basin2400..3200,
landing shore3200..3520. Middle deck2768..2864,y160; upper tiny ledges2912,128 /
3008,96 /3104,128, exclusively after middle.33diamonds,11spawn sites,20water
strips. Same original four active enemy slots. Existing gate plus both geysers
re-enabled; no loading boundary. Machine finale, extra gates, station/Rain Core,
new music/checkpoint and campaign handoff remain future work, not claimed done.

Joined-only appended enemy type2 and own24x24/16frame cache for Spillwing;
Crab retains32x24/22frames/type0, Walker64x64/32frames/type1. Independent asset
loading, cache lifetime, hit/contact/death dispatch. No Enemy struct/pool growth.
Caught/removed accidental cache rebuilding in rendererResetGameplay before
staging; cache built once and freed via existing type-count loop. Legacy
isolated flyer alias retained when JOINED is absent.

Keep water batching and mask offsets; move pontoon bounds2400..3104; offline
clearance table now generated for actual map width3520. Existing rear1120px
fits final camera with no extension. Exact native-pixel equality proven for
land0..2399 and translated ferry source160..1279; original rear/patches/three
enemy art/clip bytes unchanged. Known extra Chip allocation relative to ferry:
116480bytes foreground +31680bytes separate Crab cache; transient allocation
peaks/largest blocks and020 cadence still need native evidence.

Actual-C ASan/UBSan tests: all11sites/3families activate over full forward/back
camera sweep; cache frame bounds; static flyer collision; per-family hits/death;
visible panel/open/reset; both jets; shifted boat/middle/boat route and allfour
upper transitions; no first-half overhead bypass;33pickups and water boundary.
Existing land test passes precision jumps and all4Walker traversal links over
8seeds. Existing ferry clearance test passes9600predicates+19200updates;24px
cache, visible shots, projectile damage, post occlusion tests pass. Mask offsets
pass67680cases for each isolated/joined bounds. Native joined build succeeds,
only pre-existing main/ready_ui warnings. No emulator auto-run.

Current dist/Drowned-Level-020-HD/Drowned-Test;68assets/49embedded refs, all
68release hashes preserved. Staged via stage_hd_test against old ferry drawer
then renamed verified new drawer. Prior offset build+log archived byte-identical.
Proof build/drowned-joined/proof-joined.json; generator build_drowned_joined.py,
manifest and joined-layout.png in build/drowned-joined. User full-run/020 memory,
FPS, combined gate/water/flyer visuals and life-reset acceptance pending.
No release/version/commit/push.

## 2026-09-16 — Pontoon offsets playtest: similar measured cadence

User FS-UAE playthrough, established PAL50 68020/2MB Chip/8MB Fast configuration. User reports "weer iets beter volgens mij"; no exhaustive visual acceptance inferred.
Executable hash matches proof-pontoon-offset.json. Complete post_run footer; preserved original log bytes. Header v7 describes unchanged layout/AI, not unique optimization identity.
1441 intervals:1407one-field,34two-field,0three-plus,max2;48.84FPS. Previous waterbatch run:1415intervals,31two-field,0three-plus;48.92FPS. Miss shares2.36% vs2.19%. Numerically essentially similar, no measured improvement established and no controlled regression established.
Current71shots/5kills/17jumps/5collect requests/1water/1hurt vs prior49shots/4kills/16jump requests/5collects/1water/2hurt. More shooting is not sufficient workload normalization: early kills can also reduce rendering work.
0ownership violations. Prepared Chip606696free/largest605576 vs prior607400/606184; Fast5225168free/largest5223976. Whole-system allocation snapshots differ; no extra explicit Chip allocation in candidate source, but do not claim identical measured Chip usage or infer cause of704-byte difference.
Retain current candidate provisionally given correctness proof and positive user impression; do not label this lookup a demonstrated FPS win. Waterbatch improvement remains prior evidence. Full50FPS target remains open. No new build/runtime change during review.

Evidence: testresults/Unassigned-Drowned-offset-48-84fps.log and matching TXT.

## 2026-09-16 — Pontoon mask offset candidate

User authorizes one focused final optimization pass before content expansion.
Retain water batching (last run48.92FPS,31two-field/1415intervals,0three-plus).
Ferry-only prepared position/phase offsets replace the visible pontoon mask
address division by80 and multiplications by80/56.1538bytes normal program
BSS, no extra Chip allocation or asset bytes; preparation once at asset load.
Position indexed directly from existing physics, so no new movement state.
Reference switch SPARKPAW_PONTOON_OFFSET_REFERENCE retains original formula.

ASan/UBSan actual lookup comparisons67680: every705legal integer x, both
deck heights,16frames, three preparations; offsets identical and bounded.
Actual mask test now exercises candidate lookup:2293760pixels match independent
water-pattern oracle. Native build passes. Full renderer reference assembly
byte-identical to previous waterbatch assembly. Compiler first emitted a
three-argument call; lookup made an expression (each argument evaluated once)
so final hot assembly has direct reads/add/shift, no division/multiplication
or added helper call. Evidence build/drowned-ferry/offset-*-excerpt.asm.

Broader copy review: projectiles/enemies restore only when their target history
says drawn; inactive/dead state alone cannot safely skip old image removal.
Diamonds restore a word-aligned footprint, including after collection, and
water/other actor restoration can overlap it. Existing enemy union optimization
applies to Striders; blindly extending to separated flyers may copy larger
areas than saved. No such change included; a separate actual pixel/ownership
proof plus measurement would be needed. Preserve final waits and draw order.

Current dist/Drowned-Ferry-020-HD/Drowned-Test is offset candidate;67assets
byte-identical,48runtime references,68release hashes preserved. Previous
waterbatch build and full log archived and hash-verified. Proof:
build/drowned-ferry/proof-pontoon-offset.json. User020 cadence/masking gate
pending; small expected gain only, no FPS claim. No runtime gameplay changes,
release/version/commit/push or emulator launch.

## 2026-09-16 — Water batching playtest: positive result

2026-09-16: user FS-UAE playthrough, established PAL50 68020/2MB Chip/8MB Fast configuration. User: "oogde iets beter volgens mij". No explicit exhaustive glitch verdict.
Source: sparkpaw/dist/Drowned-Ferry-020-HD/renderdiag.log. Complete post_run footer. Executable SHA256 matches proof-waterbatch.json. Log v7 describes unchanged layout/AI; candidate identified by executable hash.
1415 intervals:1384 one-field,31 two-field,0 three-plus,max2;48.92FPS. Missed interval share2.19%, previously4.61% (1257intervals,56two,2three-plus,max4;47.64FPS).
New workload49shots/4kills/16jump requests(15starts)/5collects/1water/2hurt; prior29shots/4kills/12jump requests/5collects/1water. Manual workloads differ; positive evidence, not controlled causal measurement.
0ownership violations. Chip607400free/largest606184; Fast5224936prepared. Worst snapshot is frame1/camera0/water_updates0, not proof that gameplay-water work is costless.
Retain water batching as current working candidate. Full50FPS and broader/native hardware acceptance remain open. No runtime change in review.

Evidence: testresults/Unassigned-Drowned-waterbatch-48-92fps.log and matching TXT.

## 2026-09-16 — Adjacent water synchronization candidate

User authorizes the water-copy optimization after the wider renderer audit.
Ferry-only drowned_water_sync.h groups adjacent dirty80px strips, then uses
the existing bounded DMA copier. Clean strips and gaps terminate groups;
per-strip phase bookkeeping, clipping, all three physical copies and final
waits preserved. No extra runtime allocation, visual/cadence/gameplay change.
SPARKPAW_WATER_SYNC_REFERENCE retains the previous path.

Actual C helper + actual copier tested against independent pixel oracle with
ASan/UBSan:133120 cases across all dirty subsets, ring origins, adjacent/gapped
layouts and different canonical phases. All pixels and phases match, second
sync is a no-op, no extra blits. Example84->24 operations; not an FPS claim.
Existing CPU/DMA copier tests also pass27840 rectangles per mode. Native ferry
build passes. Full renderer reference assembly is byte-identical to the
pre-change audit; candidate assembly confirms one copy per merged run.

Current dist/Drowned-Ferry-020-HD/Drowned-Test is the water candidate;67assets
byte-identical,48embedded refs,68release files preserved. Previous drawer
and log archived and hash-verified; proof build/drowned-ferry/proof-waterbatch.json.
ReadMe identifies candidate; log v7 continues to identify unchanged layout/AI,
so use executable hash for this comparison. Last baseline47.64FPS with2long
intervals; no new FPS measurement. User020 cadence/visual acceptance pending.
No emulator launched, release, version, commit or push.


## 2026-09-16 — Flight lookup candidate result: incomplete performance win

User played v7; no explicit visual/glitch verdict. Exact executable verified
against proof-clearance.json; stable full footer, evidence preserved in
Unassigned-Drowned-clearance-47-64fps.log/TXT.1257intervals:1199one,56two,
2three-plus,max4fields;47.64FPS,0ownership violations.29shots/4kills/12jump
requests/5collects/1water. Prior v6 was46.20FPS with no three-plus intervals.
Different manual workload: higher average alone does NOT prove overall win.
Two longer hitches remain/newly observed; lookup retained only as candidate,
not final native acceptance. Correctness parity and removed CPU scans stand.
Worst snapshot1flyer/3shots/3diamonds/6water updates suggests inspect water,
collectible/vlot restore/update/draw next; minimal log cannot attribute cause
or the two long intervals. No runtime/build/release change this review.


## 2026-09-16 — Ferry flight clearance optimization candidate

User approves v6 route and authorizes020 optimization. Verified latest log:
914intervals/839one/75two/0three+,46.20FPS over19.78s,0ownership violations.
Worst snapshot3flyers/3collectibles/6water updates; minimal diagnostics do not
attribute exact CPU cost. Preserved Unassigned-Drowned-upper-short-46-20fps.log/TXT.

Identified repeated static geometry work: clearSwoop up to96 horizontal scans
per attempt plus3per patrol update. New generator builds2400-byte8flag-per-x
lookup from final collision map and actual arc constant. Flags encode both
profiles/directions and hover bob phases. Static normal/Fast program data,
no Chip allocation or displayed-buffer changes. SPARKPAW_SPILLWING_CLEARANCE_REFERENCE
retains original scanner for proof. Identical blocking, cooldown, attack and
patrol decisions; table valid for current static ferry geometry only. Future
moving gates/platforms require invalidation or separate dynamic checks.

Exhaustive sanitized actual-C comparison:9600swoop predicates plus19200full
updates, byte-identical Enemy state. Reference2399629horizontal probes across
this exhaustive TEST (not a gameplay-run count), candidate0. Uses real loaded
collision map. Existing middle/boat and upper-route physics pass. Nativebuild
passes. VBCC-O2-cpu68020 assembly audit: reference7collision symbol refs,
candidate0; source table embedded in normal program data. No measuredFPS gain
yet. Same67runtime assets independently hash-identical to v6.

Current Drowned-Ferry-020-HD/Drowned-Test(v7),67assets/48refs,68release hashes
unchanged. Accepted geometryv6 and log archived and hash-verified.
Proof build/drowned-ferry/proof-clearance.json. User matched combat/cadence
pass pending; no release/version/commit/emulator launch. If drops remain,
profile water+collectible drawing/restore work separately, without weakening
visuals or accepted gameplay. Do not call this optimization accepted yet.


## 2026-09-16 — Upper route restricted to second half (v6)

User rejects full upper route and requests mandatory pontoon first half.
Removed6ledges before/over middle and their2bonus diamonds. Keep three16px
ledges at672,128 /768,96 /864,128, and7total diamonds. Start on pontoon,
choose upper or boat at unchanged middle deck528..624,y160. All5flyers and
art/physics/visible shots unchanged. Generated assets rebuilt from base so
removed tiles leave no collision or painted residue.

Host actual physics proves empty first-half air, middle-to-upper and all
remaining links/final bank; boat/middle/boat still passes. Native build passes.
Previous full-route log verified:2420intervals/2298one/122two/0three+,
47.60FPS over50.84s,0ownership violations. Stable complete log preserved as
testresults/Unassigned-Drowned-upper-full-47-60fps.log/TXT. FPS remains below
target; no improvement claimed from layout reduction. Performance work still
needed after this user-directed geometry gate.

Current same Drowned-Ferry-020-HD/Drowned-Test(v6),67assets/48refs,
68release hashes preserved. Rejectedv5+log archived byte-identically.
Proof:build/drowned-ferry/proof-upper-short.json. User route/FPS pending;
no release/version/commit or automatic emulator launch.


## 2026-09-16 — Precision upper-route candidate

User explicitly wants a hard tiny-platform route BEFORE and AFTER middle,
while retaining middle-to-pontoon exit. Added9single16x16 solid ledges at
(64,160),(160,128),(256,96),(352,112),(448,96),(560,96),(672,128),
(768,96),(864,128). Starts on departure bank, upper minimumy96 preserves
jump headroom. Same accepted platform materials, camera/HUD/palette. Middle
528..624,y160 remains branch; upper can descend there and rejoin at672,128.
Existing6diamonds plus3 at256,66;560,66;864,98. Five flyers unchanged count.

Rejected early low over-water ledges: they blocked pontoon carry and original
transfer. Repositioned initial ascent onto shore; upper ledge over middle
raised to96 to leave its walking/afsprong space free. Trials at80 clipped
jump into ceiling; final minimum96. Final host tests prove each real-physics
upper link, descending to middle, joining upper from middle, and complete
boat/middle/boat path (launchboat420,40ticks right, walk to middle x590,
short right-input jump back; landing53ticks). Tests check collectibles,
wall/shot/crouch carry,3000enemy ticks no solid overlap. Patrol now reverses
before solid tiles, in addition to existing pre-checked committed swoops.
No renderer/cache enlargement or new enemy family. Native build passes.

Latest supplied middle-onlyv4 log matches proof-platform:1485intervals,
1419one/66two/0three+,47.87FPS,0ownership violations;22shots,5kills.
Preserved testresults/Unassigned-Drowned-ferry-middle-platform.log/TXT.
This is below cadence target; do not normalize it. Upper route performance
and feel pending; after route review prioritize targeted cost investigation,
particularly new flight geometry tests alongside water/collectible workload.

Current Drowned-Ferry-020-HD/Drowned-Test(v5):67assets/48refs,68release hashes
unchanged. V4+log archived intact and verified. Proof:build/drowned-ferry/proof-upper.json.
No release/version/commit or automatic emulator. User tests precision and
middle-to-boat choice; full-level progression/third cache family still pending.


## 2026-09-16 — Ferry transfer platform + six diamonds

User accepts visible-only shots and requests central platform: leave boat,
collect1/2diamonds, boat travels beneath, jump back on; diamonds over water.
Prior v3 log verified:1485intervals/1464one/21two/0three+,49.30FPS,
0ownership violations,30.12s. Complete stable copy:
testresults/Unassigned-Drowned-ferry-visible-49-30fps.log/TXT. Mixed workload,
not controlled evidence of FPS gain from retiring offscreen bullets.

Ferryv4 adds96x16 solid deck x528..624,y160..176 using accepted platform
material (no new art family). Gap to deck189/190 permits boat, not crouched
player. Six diamonds:280,140;408,132;544,130;592,130;744,132;856,140.
Enables authored collectibles only in ferry, keeps other isolated proof
sentinels inactive. Two deck diamonds pickup/reset persistence tested.

Found carry bypassed wall collision. Pontoon horizontal carry now calls the
existing player moveX through playerCarryHorizontal, retaining existing
side/head/crouch collision. No pushing through the new obstacle. Same speed,
bob/water masks. Flight checks proposed32tick arc against solid tiles once
when arming; blocked attempts recheck after8ticks, no mid-dive homing. Prevents
Spillwings flying through steel; performance cost needs native measurement.

Host tests actual player/collision: shots block at both deck edges, standing
and crouching cannot be carried through, complete boat/deck/boat route proven
(launch atboat450,40ticks right, wait forboat610, jump/right lands53ticks later).
3000enemy ticks show no cell/solid overlap. Existing flat-basin1200tick carry,
all5enemy coverage and visible-shot/damage tests pass. Flat baseline test
explicitly loads original pontoon map; new platform test loads ferry map.
Native build passes. Static layout inspected; timing/art/cadence user gate pending.

Updated Drowned-Ferry-020-HD/Drowned-Test,67assets/48refs/68release hashes
preserved. V3 and its log archived byte-identically; proof-platform.json in
build/drowned-ferry. No release/version/commit or emulator launch.


## 2026-09-16 — Visible-only player shots, ferry v3 candidate

User lost all lives, finds5flyers more fun but jump/fire spam and offscreen
kills too easy; suggests overhead blocking structures. Log verified against
v2 executable:1716intervals/1661one/55two/0three+,48.44FPS over35.42s,
0ownership violations,48shots/8kills/17jump requests/3hurt/3water. Complete
post_run footer despite game-over. Preserved testresults/Unassigned-Drowned-ferry-five-48-44fps.log/TXT.
Cadence lower than prior3flyer49.68; different workload/deaths, not causal
proof. Current candidate must remeasure; do not normalize the slowdown.

Fixed shared Drowned projectile update, SPARKPAW_DROWNED_SLICE guard: clip
enemy sweep to camera..camera+319, reject entirely unseen ranges, retire
player bullets as leading edge exits. Last visible segment still hits; Bob
drawn history retained for restoration. Pixel reference applies same gate.
Hostile projectiles unchanged. Applies to all Drowned enemy types including
Pump Walker/Crab when rebuilt; original Level1 and published releases unchanged.
No enemy AI or art change. Fewer surviving offscreen bullets may lower work,
but no FPS gain claimed. V3 diagnostic header identifies visible_player_shots.

Native build, actual optimized/pixel projectile tests (both edges,scrolling,
visible fragments,offscreen miss,restore history,hostile lifetime), real ferry
physics/5enemy coverage and player damage tests pass. Updated same
Drowned-Ferry-020-HD/Drowned-Test;67assets/48refs/68release hashes preserved.
V2 and log archived intact, proof build/drowned-ferry/proof-visible.json.
User visual/cadence gate pending. No release or emulator launch.

Proposed next geometry: one low overhead maintenance platform with a solid
underside, blocking repeated high jumps and shots through steel; then open
water recovery space. Reuse accepted industrial materials, preview layout
before integrating. First evaluate visible-only shots and cadence; avoid
masking that change by simultaneously adding obstacles/enemy load.


## 2026-09-16 — Five-flyer ferry candidate

User played ferry v1 and asks for more enemies. Verified full log against v1
executable:2883intervals/2865one/18two/0three+,49.68FPS over58.02s,
0ownership violations.26shots,6kills,16jumps,6hurt,2water;604808Chip free.
Preserved testresults/Unassigned-Drowned-ferry-three-49-68fps.log/TXT.

Ferry v2 has5 required low/high/low/high/low flyers at328/520/704/848/952,
patrol regions200..432/384..672/552..808/704..992/800..1056. More density
in second half and bank exit. Same AI/art/HP/cache/boat/water; no new family
allocation. Existing pool4 bounds actor count, but up to4 can be visible if
not killed. This is a heavier cadence candidate, not a measured50FPS claim.
Host real1200tick ride proves all5 activated/attacked/crossed deck shot line,
peak4 visible, boarding/carry/reversal/jump/exit/reset preserved. Native build
passes. New diagnostic header identifiesv2/5spawn.

Updated dist/Drowned-Ferry-020-HD/Drowned-Test through official stager,
67assets/48refs;68release hashes unchanged. Previous v1 drawer and log
archived and byte-verified. Proof build/drowned-ferry/proof-five.json.
User difficulty/020cadence pending. No release or emulator launch.


## 2026-09-16 — Pontoon combat candidate + duo50FPS evidence

User accepts dry duo, requests1/2 flyer variation during crossing. Duo log
verified against proof-pair executable:1993intervals/all one-field,50.00FPS,
0ownership violations over39.86s;23shots,2kills,14jumps,4hurt. Chip607816free.
Stable complete copy testresults/Unassigned-Drowned-spillwing-pair-50fps.log/TXT.
This is dry-floor acceptance, not water/boat performance.

Added drowned-ferry target combining approved pontoon and Spillwing families.
Same800px basin160..960,96px boat160..864 and carry/reversal/water masks.
Three required1HP spatial encounters: lowx328 region200..432; highx584
region448..736; lowx856 region704..960. Last low can overlap high; killing
changes pressure. No scripted visible despawn or random mid-dive homing.
Reuses one23040Chip enemy cache, existing pool; no new art or upper route.
Small-family compile alias still excludes Crab coexistence until later type
integration. Corrected macro interaction: water animation/collision enabled
for combined ferry, pontoon no-enemy bypass disabled only for ferry. Gate
remains disabled. Diagnostic header now identifies actual ferry3spawn layout.

Native build and host tests pass: real player/pontoon1200ticks carry/bob/
reversal, jump/exit/water-reset; all3 enemies activate and dive, max2 visible
on tested ride, each crosses real deck shot lane. Existing duo tests and
2293760water mask pixels pass. Native combat feel/cadence not yet measured.

Current dist/Drowned-Ferry-020-HD/Drowned-Test,67assets/48compiled refs.
68alpha.8release hashes unchanged. Previous duo including log archived and
hash-verified in older-builds/20260916-before-ferry. Proof:build/drowned-ferry/proof.json.
No release/version/commit or automatic emulator launch. Next user020 combined
combat/water test; high alternate route and full-level family integration later.


## 2026-09-16 — Single Spillwing log received:50.00FPS

Log arrived during duo staging and is preserved with the original single
executable in older-builds/Drowned-Spillwing-0-old-215840. Executable hash
matches single proof.json. Copy: testresults/Unassigned-Drowned-spillwing-single-50fps.log/TXT.
Complete stable post_run footer.11439intervals, all one-field,0two/three+,
50.00FPS over228.78s,0ownership violations;6shots,3enemy deaths,21jumps,5hurt.
608520Chip free,largest607504. This supersedes the earlier no-log observation.
User accepts single visuals/function, says easy to hit. Single dry arena only;
duo and pontoon performance still pending. Inherited diagnostic route header
reports6sites/10water despite isolated setup; use executable proof and actual
water_updates0, not that stale header, for provenance.


## 2026-09-16 — Spillwing duo challenge candidate

User accepts single flyer visuals/function and finds it easy to hit; requests
multiple flyers with varied heights/speeds and apparently random attacks.
No renderdiag.log exists in single-flyer drawer; no measured FPS acceptance.
SPARKPAW_SPILLWING_PAIR candidate adds second required spawn at400 (first280),
shared dry patrol160..672. Low/high hover120/96; patrol256/384 fixed-point;
swoop768/1024 (3/4px per tick), high arc1.5x depth reaches162 vs low164.
Both1HP, same16frames and23040Chip cache; one extra active Bob, no new cache.
Initial pauses60/103ticks; each independently cycles irregular authored
49..114tick re-arm pauses,12tick warning and32tick committed non-homing arc.
This is deterministic variation, not runtime random targeting. Simultaneous
attacks allowed; safe dry floor before pontoon/mixed-family integration.

Native build and sanitized single/pair/cache tests pass:6000ticks two actors,
height/frame/world bounds, varying attack intervals and nearest swept target.
Current Drowned-Spillwing-020-HD/Drowned-Test replaced through official stager;
previous single preserved byte-identically,66assets/47refs,68release hashes
unchanged. Proof build/drowned-spillwing/proof-pair.json. User duo gameplay and
020 cadence pending. No release or emulator execution.


## 2026-09-16 — Playable Spillwing dry-floor audition

User approved compact24x24 animation review and asked to continue. Added isolated
SPARKPAW_DROWNED_SPILLWING target: one1HP flyer atx280 over flat dry floor,
patrol160..576, hover120/121,12-tick warning, committed32-tick96px swoop reaching
y164,70-tick re-arm. No homing during swoop. Existing off-camera respawn and
once-only20score preserved. Death24ticks: two hit-flash poses then four electric
collapse poses. Shot bounds mirror body; smaller contact box excludes rotor.

Approved16frame art converted to48x384 left/right SPBM with decoder parity.
24px cache requires byte-aligned source reads (right begins byte3), last
half-word masked and guard zero. Existing32/64px fast path remains unchanged.
Cache23040Chip bytes, no per-frame transformations or additional pool slots.
Temporary compile-only small-enemy cache alias; Crab and Spillwing do NOT yet
coexist. Full integration needs distinct third-family cache/type ownership.
No water/collectibles/machinery in this safe-floor combat audition. Pontoon,
upper route and mixed encounter remain subsequent gates. Existing HUD/audio.

Host sanitized tests: actual flight, mirrored point/sweep/contact bounds,
committed arc,1HP/death frame bounds/score/respawn; actual cache all16frames,
both facings,4planes/mask/zero guards. Existing encounter and full route tests
pass. Candidate and default campaign native builds pass with existing warnings.
No emulator execution or FPS claim. User's established020 configuration remains
the requested test target; native visual/function/cadence acceptance pending.

Current dist/Drowned-Spillwing-020-HD/Drowned-Test:66runtime assets,47 literal
references.68 alpha.8 release hashes unchanged. Previous pontoon drawer and
.uaem preserved under dist/older-builds/20260916-before-spillwing, all files
verified. Proof:build/drowned-spillwing/proof.json. No release/version/commit.


## 2026-09-16 — Spillwing action animation review

User approves24x24 concept and scale, explicitly wants harder-to-hit flying
enemy than earlier levels. Working combat proposal:1HP, short readable warning,
committed swoop, no long free shot window. Generated offline16frames:
fly4,warn2,dive2,recover2,hit2,death4 in fixed24x24 cells. Native accepted body
preserved except1 clearer sensor pixel; rotor phases anchored at11,5; body
rotates offline around mast11,7 with nearest sampling, no runtime transforms
or per-frame scale.18degree tilt rejected by bounds check;12degree fits without
cropping. Electric hit transition does not imply multipleHP. Preview warning
240ms, purely proposed timing. Scripted motion GIF is not emulator/AI proof.

Assets:assets/enemies/spillwing-actions-review-v1; generator:
tools/preview_spillwing_actions.py. Palette matches approved native foreground.
User review pending before dry-floor encounter integration. No runtime,
release, test drawer, enemy counts or memory-cache changes this turn.


## 2026-09-16 — Spillwing compact concept review

User requested smaller flying enemy and approved24x24 cell direction, body
approximately18x12. Generated spillwing-concept-v1.png with built-in ImageGen,
existing enemy material sheet as reference; exact prompt in same-name TXT.
Compact steel pod, short connected top rotor, copper fittings and forward sensor.
Provisional24x24 exact foreground-palette size audition produced by
 tools/preview_spillwing_idle.py in assets/enemies/spillwing-idle-review-v1/.
Scale strip uses existing player and approved96px pontoon unchanged. Source
concept and provisional native clusters await user review; no animation,
AI, cache, runtime or test-drawer changes. Next refine native sensor/silhouette,
rotor poses, anticipation/swoop, hit and electric death only after review.
No per-frame autoscaling or new actor anatomy. Full level/upper route remains
planned; native flight envelope must stay compact for its clearance.


## 2026-09-16 — Pontoon water-only travel bounds

User confirms faster startup and acceptable appearance; rejects hull over land.
Evidence:Unassigned-Drowned-pontoon-bank-overlap.png/TXT and startup2-49-80fps.log/TXT.
Latest isolated basin1570intervals/6two-field/0longer,49.80FPS; no enemy workload.
Changed left-edge range128..928 to160..864 (=waterRight960 minus hull96).
Reset starts wholly on water, reverses immediately at full-hull limits and
continues repeating after initial boarding. Speed/bob/art/masks unchanged.
Real-physics test proves both-bank jump exits, boarding,1200ticks full hull
containment including both reversals, carry and reset. Native build passes.
Updated Drowned-Pontoon-020-HD/Drowned-Test, previous drawer/log preserved by
stager,67assets/48refs and68release hashes verified. Proof:build/drowned-pontoon/proof-banks-v3.json.
No release; user visual gate pending.


## 2026-09-16 — Pontoon startup revision, native boot pending

User reports original pontoon executable leaves Workbench visible, no visible
crash/gameplay. Packaged files verified against proof.json; no log present.
Read-only source review found1,966,080 per-pixel water tests before display
takeover. This is a credible long-startup cause, not confirmed sole diagnosis.
Moved mask generation to host asset build: pontoon-clip.bin286720bytes in
big-endian native words. Amiga allocates same RAM, reads precomputed bytes;
no million-iteration initialization loop. Actual loader tested against water
oracle over2293760 mask bits, same image/physics. Adds startupdiag.log stages
loading_files/preparing_renderer/renderer_ready and explicit failure stages,
only for pontoon proof. Native build passes; no emulator launch/boot claim.
User retries updated Drowned-Pontoon-020-HD/Drowned-Test. Official stager
preserves prior drawer;67assets/48refs verified,68release files unchanged.
Proof:build/drowned-pontoon/proof-startup-v2.json. No release/version change.


## 2026-09-16 — Playable pontoon proof ready

User approved native pontoon preview and requested runtime proof. Isolated
SPARKPAW_DROWNED_PONTOON candidate:800px water basin160..960,96px float starts128,
ends928,2px/tick, starts when boarded, reverses at both ends. One-pixel bob
carries grounded player before input. Downward-only support uses existing4px
minimum overlap, jumping detaches, water death resets boat via drownedReset.
No enemy spawns or active collectibles; gate/jet collision/animation disabled
only in this proof. Flat shores, original2400px route assets preserved elsewhere.

Native approved art embedded1120bytes including masked4planes and word guard.
Per-target Bob history restores before background updates. Draw follows water
synchronization. Immutable lower-mask lookup286720Fast bytes built from actual
waterPatternPen;112-byte copy/frame into1120-byte Chip mask/art allocation.
Two deck heights,16water phases,80x offsets; art bits immutable, old DMA retired
before mutable mask reuse. Same HUD/sprite/display boundaries; no new wake/audio.

Validation: native020 build passes; host actual player/collision tests cover
boarding,1200ticks carry/bob/reversal,jump detach,far-bank landing,water miss/reset.
2293760 lower-mask pixels checked against actual water predicate, including
padding; ordinary route traversal tests still pass. These are not emulator or
performance claims. User020 motion/water overlap/cadence pending.

Current drawer dist/Drowned-Pontoon-020-HD/Drowned-Test (66assets/47refs).
Previous full route and log preserved byte-identically under
 dist/older-builds/20260916-before-pontoon/Drowned-PatchDMA-020-HD.
68release files unchanged. Proof:build/drowned-pontoon/proof.json.
No release/version change. Full level, Spillwing and checkpoint still pending.


## 2026-09-16 — Pontoon native motion review

User approved industrial pontoon concept. New offline conversion/preview:
tools/preview_drowned_pontoon.py and assets/concept/drowned-pontoon-native-v1/.
96x16 cell compiled from approved upper concept silhouette, same exact12-bit
foreground palette. Largest alpha component excludes labels. No new raster
identity. Existing waterPatternPen C function is extracted, compiled and
executed for16frames at25Hz; native water passes in front of lower hull.
Scripted lateral motion and1px bob move the existing player together with deck.
No actual platform collision, carry or landing physics; no020FPS claim.
Animated waterline-5x.gif is offline preview, not emulator capture. No extra
wake yet. Current test drawer and68release files hash-verified unchanged.
User native art/motion review pending; then implement isolated moving-platform
physics and water occlusion with host checks before user runtime test.


## 2026-09-16 — Patch DMA user run:49.88 FPS

User reports Blitter patch candidate feels slightly better. Played drawer
verified against proof-patchdma.json;68release hashes unchanged. Preserved
Unassigned-Drowned-patchdma-020-49-88fps.log/TXT under testresults. Session-stated
HD/FS-UAE68020/2MBChip/8MBFast; config not freshly inspected.
5060 intervals:5048 one-field,12 two-field,0 longer,49.88FPS,ownership0.
Two-field share0.24% versus3.85% in prior CPU run(48.14FPS). Chip free599176,
largest597800. No framebuffer allocation added; free-memory difference is not
attributed to new buffers. Workloads differ:98 shots/8 deaths versus193/16;
therefore this is encouraging evidence with user-perceived improvement, not a
controlled +1.74FPS causal result or locked50Hz guarantee. Worst composition
camera753:2Walkers,1Crab,1projectile,2collectibles,no canonical water update.
Retain patch DMA candidate as working Drowned baseline; original CPU path
remains recoverable. Current drawer Drowned-PatchDMA-020-HD. No new build or
runtime edits in this evidence cycle. Full campaign/ADF/WHDLoad and real
hardware validation not implied. Future route expansions should retain
resident Walker and patch DMA flags and remeasure memory/cadence.


## 2026-09-16 — Geyser accepted; patch DMA performance candidate

User accepts corrected platform nozzle. Latest verified HD run:48.14FPS,
5304 intervals,5100 one-field/204 two-field/0 longer,ownership0. FreeChip599880.
Preserved testresults/Unassigned-Drowned-precision-foot-020-48-14fps.log/TXT.
Worst camera626,3collectibles,no enemies/no canonical water update; do not
attribute remaining misses to the new geyser from this maximum alone.

New isolated SPARKPAW_DROWNED_PATCH_BLIT switches canonical-to-inactive-ring
mechanism rectangle transfers from CPU word loops to A-to-D Blitter copies.
Same aligned clipping, four planes, three ring copies, modulo strides, source
and target Chip buffers; waits retire DMA before reuse/return. No new buffers,
art, animation timing, water changes or gameplay changes. CPU path retained.
Native build and actual CPU/DMA register simulation vs pixel oracle passed
27840 rectangles per mode, including clipping and wrapping. This establishes
pixel transfer parity, not Amiga performance. Blitter contention remains a
measurement question. All66 packaged assets identical;68release files intact.
Current test:dist/Drowned-PatchDMA-020-HD/Drowned-Test. Prior drawer preserved
under dist/older-builds/20260916-before-patchdma. Proof:build/drowned-route/proof-patchdma.json.
User020 cadence/visual gate pending; no release/version change.


## 2026-09-16 — Precision geyser housing correction

User test:48.14FPS,218/5664 two-field intervals(3.85%),zero longer intervals.
Worst composition camera656,one Crab/four pickups; no cause for all misses
inferred. Evidence preserved under testresults/Unassigned-Drowned-precision-020-48-14fps
and floating-nozzle-1/2 PNG/TXT. User reports floating nozzle and requests no
diamond there. Cause: compact64px jet patch includes only top nozzle row;
remaining five static rows were missing from the extended route foreground.
Generator now places the complete approved32x6 housing at1792,170, directly
meeting platform top176. Exact nontransparent pixel parity with original
nozzle at448,194 checked. Animation and hazard coordinates unchanged. Removed
coin1800,148;26diamonds remain. Native build and real player/AI route tests pass.
Updated dist/Drowned-Precision-020-HD/Drowned-Test via official staging;
previous drawer/log archived automatically.66assets/47refs,68release hashes
preserved. Proof:build/drowned-route/proof-precision-foot.json. No new performance
claim, no release; user visual confirmation pending.


## 2026-09-16 — Extended precision route with timed geyser (test pending)

User requests longer precision platforming, low/high/higher/low/high/low, and
explicitly wants the geyser to occupy the whole narrow landing rather than a
safe waiting half. Route now 2400px, ten 80px water strips, six precision
supports at x1456/1568/1680/1792/1904/2016, tops176/144/112/176/144/176,
all32px wide. Final bank starts2128. Final Crab moved to2256; earlier encounters
and four Walker traversal links unchanged. 27 diamonds. Existing materials
extended for tall supports; no new concept raster, palette or animation family.

Third dynamic patch reuses the existing 32x64 geyser at1792,107, with translated
hazard at1804..1811,y128..167. Shares the existing200-tick cycle and atlas/stage;
o new effect cache. Wait on the preceding high support, descend during quiet
phase and jump onward. Active135..184, warning100..129, recovery185..199.
Real-physics host landing proofs:41/39/39/63/39/55/53 ticks. Releasing horizontal
input brakes the long descent; held-right overshoots the geyser support.
The inlet nozzle remains narrow, but the ordinary player body standing over
this32px support intersects the erupting hazard. No artificial full-width hitbox.
Timing/readability/difficulty and native FPS still require user evaluation.

Resident Walker optimization and diamond restoration retained. Front allocation
increases49920 Chip bytes for480px extra width; actual free memory pending log.
Native build, sanitized route/AI/jet boundary tests, diamond restore and resident
cache parity tests passed. Staged66 assets/47 references; only route art/map
assets change, all68 alpha.8 release files preserved. Current drawer:
dist/Drowned-Precision-020-HD/Drowned-Test. Previous tested resident drawer and
log archived intact under dist/older-builds/20260916-before-precision-route.
Proof:build/drowned-route/proof-precision.json. No release or version bump.


## 2026-09-16 — Resident Walker user test: 49.49 FPS

User played the resident-cache HD candidate and reports it felt slightly better.
Verified played drawer against proof-resident.json; release hashes unchanged.
Preserved raw evidence: testresults/Unassigned-Drowned-resident-walker-020-49-49fps.log
and matching TXT. Configuration is session-reported 68020 / 2 MB Chip / 8 MB Fast,
not freshly read from the emulator. 2965 intervals: 2935 one-field, 30 two-field,
zero three-plus; 49.49 effective FPS; zero ownership violations. Miss share
1.01%, versus 4.86% in Route2 (47.68 FPS). These are different manual workloads:
71 player shots versus 116, 11 enemy deaths in both, 2 Walker shots versus 1.
Therefore the result supports retaining the resident candidate as the working
Drowned baseline, but does not prove a controlled +1.81 FPS optimization gain
or locked 50 FPS. Default non-candidate renderer remains unchanged.

Measured prepared Chip free 652424 bytes, largest block 651176; Fast free
5511600. Exactly 192000 fewer free Chip bytes than Route2, matching the planned
tradeoff. Worst composition now camera 612, one Walker, three collectibles,
one water update; this single maximum does not locate all remaining misses.
No runtime changes or new test drawer in this evidence-only cycle. Current
active drawer remains dist/Drowned-Resident-020-HD. Next performance work should
focus on remaining draw/restore and scroll overlap with matched workloads;
retain visual quality and measure memory again as the level grows. No ADF,
WHDLoad, real-hardware or full-campaign acceptance inferred from this HD run.


## 2026-09-16 — Resident Walker cache candidate (020 test pending)

User confirms the route pickup/diamond restore correction. Remaining perceived
FPS dips occur near Pump Walkers; the larger final water section feels smooth.
Preserved Route2 evidence: `Unassigned-Drowned-route2-walker-load-47-68fps.log`
and TXT under testresults: 2326 intervals, 113 two-field, no longer intervals,
47.68 effective FPS, zero ownership violations. Worst composition: two Walkers,
one Crab, three projectiles, camera 1052. This does not establish the cause of
every missed interval; manual runs are not controlled benchmarks.

New diagnostic `dist/Drowned-Resident-020-HD/Drowned-Test` keeps the same route,
art, audio, poses and pickup fix. Compile-only `SPARKPAW_DROWNED_RESIDENT_WALKER`
places the 204800-byte Walker frame cache in Chip instead of Fast and removes
12800 bytes of mutable Chip stages: net +192000 Chip (187.5 KiB), -204800 Fast.
Direct immutable frame pointers remove 3200-byte pose copies during gameplay.
Actual memory availability and FPS gain remain unverified until user testing.
Default staged path is retained. Native build and ASan/UBSan actual-selector
parity tests passed (32 frames, both facings, four slots; resident zero copies).
All 66 packaged assets match Route2; all 68 release files are unchanged.
Previous drawer including logs and metadata preserved under
`dist/older-builds/20260916-before-resident-walker`.
Proof: `build/drowned-route/proof-resident.json`. No release or version change.

## Route2 fixes partial pickup remnants — 16 September 2026

User reports FPS dips and half pickups remaining after collection on narrow
supports. Screenshot cataloged byte-identically with sidecar at testresults/
Unassigned-Drowned-route1-partial-pickup-remnants.png. Original clipboard file
retained. Shows right halves of cyan diamonds,not a second collectible state.
Cause: route coins19/21 atx1464/1576 are8px off16px word boundaries. Active
ring restore used16px except legacy hardcoded index28,leaving the right8px.
Renderer now uses compiled drownedCollectibleRestoreWidth(x) under ROUTE in
the default non-canonical-reference ring path:16px aligned,32px otherwise.
Positions/art/collection/score unchanged. Same Blit count,no new cache/buffer;
unaligned restores transfer one additional word per row. Legacy Level1 and
its canonical-reference paths retain their existing contracts.
New test_drowned_collectible_restore.py compiles the actual width helper and
uses actual diamond mask against pixel truth for16 alignments,hover extremes,
ring positions and two target histories:896 pass;old16px fails784 cases.
Registered make test. Native build passes;66 staged assets identical to v1.

V1 log preserved with sidecar:Unassigned-Drowned-route1-020-cadence-49-10fps.log.
Candidate proof verified,complete footer.3309 intervals:3249 one/60 two/0
three-plus,max2;49.10FPS,1.813% late20ms intervals,ownership0.56 compositor
crossings not ring rolls. Worst compose542 camera628 (before new precision
section) has1Walker/3collectibles/1water update; no hotspot causality proven.
Prepared freeChip845128/largest843968,Fast5305800/largest5303336. User-stated
FS-UAE HD68020/2MBChip/8MBFast,config not freshly inspected. FPS dips remain
an open concern; no unrelated optimization or speedup claim in this fix.

Active:Drowned-Route2-020-HD/Drowned-Test. Collect both platform diamonds,
scroll away/back and save cadence with left mouse+15s frozen hold. New native
visual/cadence acceptance pending. V1 drawer/log/metadata intact under
older-builds/20260916-route1-pickup-remnants. Proof:build/drowned-route/proof-v2.json.
68 alpha.8 release hashes unchanged. No release,commit or auto-emulator run.

## Drowned 1920px route v1 staged — 16 September 2026

User authorizes expansion and suggests precision platforms over repeated water,
or later a raft crossing with flying enemies. Implemented first precision
section now; raft/carry physics/flying family are explicitly deferred, not
implemented. Active `sparkpaw/dist/Drowned-Route1-020-HD/Drowned-Test`.
Current route supersedes the earlier proposed segment ordering: preserve the
known first960px/gate/geyser composition, then extend behind the gate.
864..1088: raised maintenance deck with a Walker jumping up/down40px.
1088..1360: Walker crossing an80px water gap both ways.
1376..1680: three80px water gaps with32px support islands; two authored tops
at176 and160; dry landing from1680. Last Crab/recovery strip ends at1920.
Six spawn sites total (three Walkers/three Crabs),26 diamonds. Usually sparse;
actual host camera tests see peak3 active enemies. No final Core/level ending,
new music or raft/flying enemies in this intermediate route.

Single layout source assets/levels/drowned-route-v1.json generates geometry,
collision and C route/coin tables via tools/build_drowned_route.py. Target
`make drowned-route PYTHON=../.venv/bin/python3`, compile-only
SPARKPAW_DROWNED_ROUTE and WORLD_W1920. Water count6; existing80px animation
cache reused and existing viewport culling retained. Existing gate visibility,
head-shot fix, copper projectile/audio, HUD, palette, mechanisms and enemy art
retained. Foreground +99,840 Chip bytes; collision data +840 bytes; these are
specific deltas, not a measured native free-memory total. Rear/mechanism/enemy/
shot asset bytes identical to previous candidate. Ring targets remain fixed.
Potential extra water/Bob load in busy views requires native020 measurement.

Actual player physics tests prove both narrow ledge landings, far bank and
raised deck. Actual enemies/collision tests cover all4 traversal links over8
seeds with no failed landings, valid poses/height and supporting floor/body
clearance probes; peak3 active. Full `make test` passes. Native build passes;
final diagnostic-only route identity line added and native rebuilt afterwards.
No automatic emulator or real-hardware claim. Layout still review image at
build/drowned-route/route-layout.png omits dynamic water/enemies/HUD; not an
emulator screenshot. Asset planar roundtrip and package hashes verified.

66 packaged assets/47 literal references;68 alpha.8 files unchanged. Previous
GateView drawer plus any log/launcher metadata archived byte-identically at
older-builds/20260916-before-route1. Proof:build/drowned-route/proof.json.
Test2–3min,especially BEYOND gate: Walker climb/water routes,precision section,
combat and backtracking. Left mouse once,wait15s frozen save hold,stop/reset.
Native reachability/feel/readability/FPS and full-route memory remain pending.
No release/version/commit change. Next raft concept should add a separate
moving-platform/carry proof and reviewed flying enemy before route integration.

## Gate visibility fix and next route scope — 16 September 2026

User: little jumps look okay; wants higher platforms/water traversal and asks
whether it is time for a wider level/more platforms/enemies. Reports gate can
be opened before visible. Current short hop remains an animation audition.
Next proposed scope:1920px playable route (not the final whole level), roughly
0..384 arrival,384..800 raised platforms,800..1184 authored water crossing,
1184..1600 water/geyser encounter,1600..1920 gate/exit. Aim6 spawn locations,
usually1–2 visible enemies; validate an intentional heavier overlap separately.
Walker links must use actual landing/support geometry, both directions where
appropriate, and pass host trajectory/blocked-destination/offscreen/respawn
checks. Preserve artwork/HUD/020 budget; new art concept first if required.
This expanded layout is proposed, NOT implemented in current960px build.

Gate fix implemented: drownedSetView(cameraX) caches whether panel768..783
fits viewport with8px reading margin; both direct-hit and sweep reject when
invisible. Reset disables it until game publishes view. One update per game
tick, no per-pixel camera lookup. Tests cover early/offscreen,partial and exact
visibility boundary,both projectile directions,normal activation and reset.
Actual slice physics/projectile test and6000tick enemy variants pass; native
build passes. Current candidate Drowned-GateView-020-HD/Drowned-Test retains
short hop, NOT expanded route.66 assets/47 refs,68 release files unchanged.
Jump drawer/log/metadata archived intact:older-builds/20260916-before-gate-view.
Proof:build/drowned-enemy-audit/proof-gate-view.json. New gate native test pending.

Jump log complete,hash-verified and preserved with sidecar under testresults/
Unassigned-Drowned-jump-020-cadence-49-64fps.log.1392 intervals:1382 one/10 two/
0 three-plus,max2;49.64FPS,0.718% late,ownership0.11 compose raster crossings,
not ring rolls. User-statedFS-UAE HD68020/2MBChip/8MBFast; no fresh config read.
Short positive motion/cadence result, not proof of full-level enemy headroom.
No release,commit or automatic emulator run.

## Pump Walker dry jump audition — 16 September 2026

User authorized a small jump route after confirming V4 projectile damage.
Active: sparkpaw/dist/Drowned-Jump-020-HD/Drowned-Test (68020/2MBChip/8MBFast).
New isolated SPARKPAW_DROWNED_JUMP_PROOF target drowned-jump adds two authored
links on existing surface1, no engine physics change or extra enemies/geometry.
Right launch x346..350 -> landing382..388; left386..390 ->349..354.
Fixed-point VX +/-320,VY-900,gravity60: about36px travel,25px high,29ticks
flight. Existing12tick compression,5tick landing,7tick recovery. Dry-ground
animation audition, NOT water-gap traversal or full-level route acceptance.
Existing spawn speeds, shot identity, damage and respawn retained.

Host actual enemies.c/drowned_slice.c sanitizer test now runs6000ticks in
legacy/art/jump variants, asserts both flight and landing directions,no route
failure,valid poses/height/patrol limits, shots, deaths and respawn. Projectile
head/crouch/iframes regression also passes. Native compile passes. All66 staged
assets byte-identical to V4;47 executable refs;68 alpha.8 release hashes intact.
No new animation cache/asset allocation. Native motion/feel/cadence pending;
no automatic emulator or release. Leave Walker alive to watch both directions,
then test combat/respawn and save with left mouse +15s frozen hold.
V4 drawer/log/metadata archived intact at older-builds/20260916-before-drowned-jump.
Proof:build/drowned-enemy-audit/proof-jump.json. Last accepted V4 cadence49.69FPS
is not a measurement of this changed workload.

## Drowned v4 user confirms projectile damage — 16 September 2026

User played Drowned-Enemies4-020-HD and explicitly confirms Walker shots now
cause damage. Keep head-hit correction. No new explicit muzzle-alignment,
crouch or both-facing-direction acceptance inferred.
Complete log preserved with sidecar at testresults/Unassigned-Drowned-enemies4-
020-head-hit-confirmed-49-69fps.log; candidate proof-v4 and68 alpha.8 release
hashes verified unchanged.2263 intervals:2249 one-field/14 two/0 three-plus,
max2;49.69FPS,0.619% late20ms intervals,ownership0. Prepared Chip free942968,
Fast5307552;post-run Fast5716040.12 compose raster crossings are not ring rolls.
Stated FS-UAE HD68020/2MBChip/8MBFast; no fresh config read. Good current cadence,
not controlled proof of improvement versus v3 or full-level headroom. Existing
v2's two longer hitches remain historical evidence, not explained by this run.
Active v4 drawer retained; no code/build/release changes this review.

## Drowned head-shot collision correction v4 — 15 September 2026

User accepts v3 sound/look; suspects low muzzle alignment and reports shots
passing through Sparkpaw without damage. Source + actual-function host repro:
standing floor200 gives player.y161; torso contact top168. High Walker shot
v3 top160 had collision bottom167, missing by1px despite intersecting the head.
New candidate-only playerProjectileBounds retains narrow contact X/bottom,
includes standing head at player.y-4; crouching keeps original low bounds.
Game calls it only for hostile-projectile damage after existing enemy contact.
Body contact, physics, damage amount and invulnerability are unchanged.
Muzzle line raised1px: projectile top enemy.y+23,center+27 matches fire-pose
aperture near row26 plus1 Bob grounding. Visual fit in motion remains user gate.

New tests/test_drowned_projectile_damage.py compiles actual extracted player
bounds/damage functions and full projectiles.c with sanitizers. Reproduces old
miss and verifies health loss from both sides, invulnerability, crouch evasion,
airborne hit and below-feet miss. Registered in make test. Actual dual-mode
enemy test and native build pass. No automatic emulator or release.

V3 log preserved +sidecar:testresults/Unassigned-Drowned-enemies3-020-cadence-
48-97fps.log. Candidate files hash-verified;complete footer.2488 intervals,
2436 one-field/52 two/0 three-plus,max2;48.97FPS,2.09% late intervals,
ownership0. Worst compose371 camera273 includes1 Crab/1 Walker; no causal
attribution to sound/rendering.50 compositor raster crossings are not ring rolls.
Prepared freeChip943672,Fast5306816;post-runChip943672,Fast5716808.
User-stated FS-UAE HD68020/2MBChip/8MBFast; config not re-read. Slower than
previous manual run, workloads differ; keep performance concern open.

Active: sparkpaw/dist/Drowned-Enemies4-020-HD/Drowned-Test. Test standing
head hit without blinking, crouch evasion and both facing directions. Left mouse
once,wait15s in frozen save hold,stop/reset.66 assets/47 literal refs;68 alpha.8
release files unchanged. V3 drawer/log/launchers intact under older-builds/
20260915-native-enemies-v3. Proof:build/drowned-enemy-audit/proof-v4.json.
New art/audio bytes unchanged from v3. V4 native gameplay/cadence pending.

## Pump Walker shot identity v3 — 15 September 2026

User accepts v2 visuals, requests heavier own Walker sound and optionally own
shot shape/colour. Reports one or two hitches. Complete v2 log preserved with
sidecar at testresults/Unassigned-Drowned-enemies2-020-cadence-49-66fps.log.
Verified played files against proof-v2.4967 intervals:4937 one-field,28 two,
2 three-plus,max4;49.66FPS,0 ownership errors.30/4967=0.604% late intervals,
max80ms. Do NOT describe this as no long hitches. Sparse minimal cadence log
cannot locate/attribute the2 long intervals. Worst compose frame939,camera436,
no enemy/projectile/collectible draws. FreeChip944920,Fast5307624 prepared.
Stated FS-UAE HD68020/2MBChip/8MBFast, no new config read or hardware claim.

V3 isolated candidate loads pump-shot.raw in place of strider-shot.raw via
SPARKPAW_DROWNED_ENEMY_ART; same priority,cooldown,period322,Paula channel and
silence reload. Diagnostic label pump_walker_shot; existing API/slot retained
in this two-enemy prototype. Future music/campaign integration must register
its own sound event/mixer asset rather than assuming the legacy Strider slot
is a campaign-wide identity. Other builds retain existing sound/path.
Deterministic synth tools/build_pump_shot.py produces signed8-bit2864 bytes,
0.260s,peak112,no clipping,zero first/last samples; +658 Chip bytes vs2206.
Low mechanical impulse/harmonics with short filtered-air decay. Preview/source:
assets/enemies/pump-shot-v1/ (WAV/raw/manifest/projectile GIF).
Projectiles use compact warm copper shoulders,pale core,short segmented trail;
exact existing16x9 dimensions,2 flying patterns,cache count/Blit count unchanged.
Native C pixel function compiled for host preview and mirror check, not a
separately approximated image. Synthesis is build-time only. Audio modes and
actual platform helper tests, real enemy sanitizer test and native build pass.
Listening quality and v3 cadence still require user play; no automatic emulator.

Active: sparkpaw/dist/Drowned-Enemies3-020-HD/Drowned-Test,66 packaged assets,
47 literal refs,68 alpha.8 release files unchanged. V2 drawer/log/metadata
archived intact at dist/older-builds/20260915-native-enemies-v2.
Proof:build/drowned-enemy-audit/proof-v3.json. No release/commit/version change.
After play press/release left mouse,wait15s in frozen save hold,stop/reset.
Keep the two longer v2 intervals as an open issue; do not ascribe them to SFX
or the geyser without event/frame evidence.

## Drowned enemy v1 cadence and v2 electrical polish — 15 September 2026

User played the native-enemy candidate: FPS felt okay; collapse looks good,
requests small electrical malfunction accents; suspects nozzle misalignment.
Original complete log preserved byte-for-byte with matching sidecar at
`testresults/Unassigned-Drowned-native-enemies-020-cadence-49-69fps.log`.
Played candidate files verified against original proof. Legacy diagnostic
alpha41 header is not the actual binary version. Stated FS-UAE HD68020,
2MB Chip/8MB Fast; no fresh emulator-config verification this run.
3292 intervals:3272 one-field/20 two-field/0 three-plus,max2;49.69fps,
0.6075% missed20ms deadlines,ownership violations0. Compositor crossings22
are not ring-roll counts.38 player shots,3 Walker shots,6 death events.
Prepared freeChip944920/largest943672;Fast5307704/largest5306560.
Post-run freeChip944920,Fast5717696. Minimal cadence, broad profiling disabled.
Worst frame1331 camera437 has no enemy/projectile draw/restore counts: no
specific culprit established.49.53fps previous encounter used different manual
workload; do not claim a controlled improvement or full-level headroom proof.

V2 adds sparse cyan/white arcs and sparks inside existing death frames;
mechanical collapse retained. Exact indexed comparison confirms ONLY Walker
24..27 and Crab10..13 differ; sizes, palettes, masks dimensions/cache allocation
unchanged. New source/review files:assets/enemies/drowned-family-v2/.
V1 generator snapshot retained in drowned-family-v1/generator-v1.py.
Source fire-pose inspection: bore center near row27 plus1 Bob grounding,
projectile center was24. Candidate-only projectile Y changed20->24 (center28).
Visible shot alignment still needs user playback in both directions.

Active test: `sparkpaw/dist/Drowned-Enemies2-020-HD/Drowned-Test`.
Native build and dual-mode actual-enemy sanitizer test pass;65 assets,
47 literal references;68 alpha.8 files unchanged. V1 drawer including log and
launcher metadata archived intact at dist/older-builds/20260915-native-enemies-v1.
Proof:build/drowned-enemy-audit/proof-v2.json. No release or auto-emulator run.
V2 art/nozzle/cadence acceptance pending. Press/release left mouse,wait15s in
frozen save hold,then stop/reset FS-UAE. User may assess offline death GIFs too.

## Native Drowned enemies integrated candidate — 15 September 2026

User accepted the refined Pump Walker recoil and explicitly authorized finishing
poses/animations and placing both enemies in the isolated level test. New active
manual test: `sparkpaw/dist/Drowned-Enemies-020-HD/Drowned-Test` (paths from root).
68020 / 2 MB Chip / 8 MB Fast, minimal cadence instrumentation. User requested
this 020 gate directly; no automatic emulator run or hardware acceptance.

`tools/build_drowned_enemy_assets.py` compiles accepted native pixels into
32 Pump Walker and 22 Turbine Crab slots with exact mirrors and pen-zero masks.
Review sheets/turn GIFs/manifest: `assets/enemies/drowned-family-v1/`.
Walker slots 0..27 keep legacy semantic roles; 28..31 add a planted front-pivot
turn. Crab 0..7 walk/rotor, 8..9 reserved, 10..13 collapse, 14..17 turn,
18..21 hit. Front-pivot parts are assembled at native size with retained material
clusters. Compression/flight/descent/landing/recovery poses are included; current
floor patrols have no traversal links, so jump animation is NOT exercised here.
User-approved Walker hit/death and Crab hit/death pixel families retained.

`SPARKPAW_DROWNED_ENEMY_ART` isolates paths/counts/selectors and grounding.
Existing two-type Bob caches, AI speeds, health, score, offscreen respawn,
collision cells, renderer order and staging dimensions retained. Pump Walker
uses +1px visual grounding (legacy Strider +2), nozzle-aligned projectile Y,
and 384 fixed-point walk phase distance to match its 1.5px stance step. This
increases frame-change frequency versus old Strider: measure cadence, do not
infer unchanged per-frame cost merely from unchanged cell dimensions.
Crab's death selection is explicitly mapped to its four valid cached poses.

Memory deltas against old encounter: +18,720 Chip bytes for Crab cache,
+25,600 Fast cache bytes and +32,960 Fast source bitmap/mask bytes. These are
asset allocation deltas, not a measured total free-memory claim. Source assets
remain Fast; Walker Chip staging size unchanged. Runtime CPU does no rigging,
rotation or pixel generation. Candidate has 65 packaged assets / 47 executable
literal references. Native build and full `make test` pass; real AI host test
runs both legacy and candidate modes under sanitizers, checks frame bounds,
nozzle Y, shots, death/score and offscreen respawn. Final gait change rechecked.
Legacy enemies 68020 -O2 assembly is identical to HEAD excluding filename idnt.

Proof: `sparkpaw/build/drowned-enemy-audit/proof.json`. All 68 alpha.8 release
files verified unchanged. Previous Drowned drawers/logs and loose launcher
metadata archived intact under `dist/older-builds/20260915-before-native-enemies`.
Test 60–90s with both enemies, turns, hits/deaths, water/geyser, gate and respawn.
Press/release left mouse once, wait at least 15s in deliberate frozen save hold,
then stop/reset FS-UAE. No release/version/commit change. New in-game visuals,
turn quality, nozzle alignment and 020 cadence remain pending user testing.

## Pump Walker recoil refinement v2 — 15 September 2026

English enemy names: **Pump Walker** and **Turbine Crab**. User accepts the
breakdown/collapse direction and leaves Crab recoil unchanged for now; asks
for Walker torso recoil to connect naturally to its legs.
Offline v2: assets/enemies/drowned-actions-review-v2/ (under sparkpaw from
workspace root). Fire/hit shift the hips with the torso and solve the existing
thigh/shin parts around planted feet; hit settles directly to the exact idle.
Visible foot pixels in rows 58–63 verified unchanged in all fire/hit poses.
Walker idle/charge/death and every Crab pose are byte-identical to v1.
Palette and all three test-package/release inventory hashes checked unchanged.
V1 generator preserved in drowned-actions-review-v1/generator-v1.py; active
script tools/preview_drowned_enemy_actions.py produces v2. Contact inspected;
revised motion awaits user review. These are Python/Pillow composited GIFs,
not emulator captures or performance evidence. No runtime integration.

## Local action audition v1 —15 September2026

User likes gait direction; new offline action study at
assets/enemies/drowned-actions-review-v1/scene-3x.gif and individual6x GIFs.
Generator:tools/preview_drowned_enemy_actions.py. Fixed detailed native parts,
no complete AI-generated sheet. Walker:24tick charge,6tick recoil,7 hit poses,
4x5tick forward knee collapse, sensor darkening and small pressure leak.
Crab:short hit and4-state shell fracture/leg fold. Copper/steel roles retained.
Palette checked (leak highlight corrected to actual level pen11). No runtime,
projectiles,damage,respawn or cache/slot edits. Idle/wreck holds and loop reset
are review staging only, no gameplay commitment. Existing packages and68
releases verified unchanged. Contact sheets inspected; motion quality pending
user review. Turn/jump/rotor and complete native family audit remain before
integration. No hardware/performance claim. Gait proof retained unchanged.

## Local gait audition v1 —15 September2026

User needs motion to judge native art. New offline fixed-part8-frame loops:
assets/enemies/drowned-walk-review-v1/scene-3x.gif (also1x), individual6x GIFs,
RGBA sheets and4x contacts. Generator:tools/preview_drowned_enemy_walk.py.
Uses detailed pixels from native idle masters, no generated full-sheet anatomy
or schematic replacement legs. Walker thigh/shin/foot parts use fixed-length
2-link joints and planted flat feet; scene travel matches stance displacement.
Crab uses four depth-ordered leg parts and unchanged shell. No new colours;
nearest-neighbour transforms checked against master colours. No runtime/dist
changes; three previous test packages and68 releases hash-verified unchanged.
This is a gait audition, not finished animation acceptance: small-scale joins,
foot contact, body rigidity and crab leg visibility require user motion review.
Stop-and-mirror is merely facing-test staging, NOT an authored final turn.
Rotor, attacks/hit/death/jump loops not implemented here. Review gait first,
then author complete readable family; do not integrate incomplete glide/turn.
No FPS or hardware claim for illustrative GIF timing. User review pending.

## Native idle proof v1 —15 September2026

User approved copper-crab/cool-walker colour direction and asks continuation.
Generated isolated two-idle source via built-in ImageGen, saved as
assets/enemies/drowned-enemies-idle-source-v1.png (real alpha channel; RGB
background tint under transparent pixels is not opaque scenery). Exact prompt
sibling TXT. Native offline converter:tools/prepare_drowned_enemy_idle.py.
Outputs:assets/enemies/drowned-native-idle-v1/ (indexed/RGBA PNGs,1x and
nearest-neighbour scale/scene proofs,manifest with source bounds and pen counts).
Only alpha>=128 defines cropping bounds, avoiding faint halo widening and
silently shrinking bodies. Uniform aspect scale per idle; no per-frame fits.
Crab32x24 opaque bbox[1,6,31,23];walker64x64 bbox[8,5,56,63].
Existing Drowned12bit palette used; material-aware allowed pens avoid player
orange2/3. Small sensor/leg readability remains a native review question.
Scene uses current front/rear sources, original48x48 Sparkpaw frame and HUD;
static composition only, not emulator capture or accepted animation.
User native-size review pending. Do not integrate or animate until native
master approval; then refine fixed parts and local walk+turn loop. Source
concept is not a topology-proof rig; no death/jump family generated here.
All3 encounter/Opt5 play packages and68 release hashes checked unchanged.
No runtime,cache,physics,release or performance change.

## Enemy concept colour variant v2 —15 September2026

User likes v1 designs but requests stronger colour distinction. Built-in
ImageGen colour-edit concept v2:crab dominant weathered copper-brown shell,
dark steel legs/rotor;walker dominant cool slate/steel,small copper fittings.
Both retain sparse cyan sensor accents. Source assets/enemies/drowned-enemies-
concept-v2.png;exact prompt sibling -prompt.txt. V1 preserved. V2 pending
user review; no native mapping/anatomical pixel-parity claim. Generated image
remains a concept with original scale/background limitations. No runtime,
asset-pack or release changes. Native material roles must distinguish crab
copper from Sparkpaw's orange; verify against blue environment at native size.

## Drowned enemy art concept v1 —15 September2026

User explicitly requests enemy-design lessons, anatomy rules, premium AGA
pixelart and good animations/deaths. Read relevant skills and later Strider
history: no repeated whole generated sheets; lock native master/parts then
review local gait+turn/death loops. Built-in concept source preserved at
sparkpaw/assets/enemies/drowned-enemies-concept-v1.png; exact prompt sibling
TXT plus IMAGEGEN_PROMPTS entry. See docs/DROWNED_ENEMY_ART_PLAN.md.
Concept shows turbine crab and biped pumpwalker; user review pending. No
runtime/dist changes. Sheet violates requested flat background/relative scale
and rotor blade count; not a native sprite proof or approved animation sheet.
Next after design review:32x24/64x64 native masters, actual-size Sparkpaw/
scene comparison, then stable anatomy gait+turn and death with material-role
checks. All current Opt5/encounter builds remain unchanged.

## First encounter user result —15 September2026

User played all3 builds and reports no visual anomalies; possible first
encounter hiccup, less/absent later, uncertain. Keep observation open.
All3 package hashes verified;68 alpha.8 release files unchanged.
Measured encounter:2561intervals,2537one-field,24two-field,0three-plus,
49.53fps,51.70s,0ownership violations. Long share0.937%; environment Opt5
0.102%,49.94fps, but manual workload differs and plain builds unmeasured.
42 player shots,2 Strider shots,7 enemy death requests confirm combat activity.
Worst aggregate frame473/camera433 records no enemies/projectiles drawn;
this cannot attribute all long frames or confirm first-activation cause.
Source:patterns/stages built at startup; activation copies existing state;
do not explain the user's hitch as lazy asset loading without evidence.
Log/TXT preserved:testresults/Unassigned-Drowned-Encounter-020-cadence-49-53fps.log.
Original remains in encounter diagnostic drawer. Full memory footer present.
Prepared Chip962328/largest960936,Fast5363392/largest5361400;
post-run Fast5740424. Lifecycle snapshots only.
Visual review positive; sustained50Hz/full-level/enemy margin still unproven.
No runtime changes this review. Next bounded diagnostic should correlate
activation/live enemy counts/combat with individual long intervals if the
first-encounter hitch is investigated; avoid speculative cache fixes.

## Opt5 play builds and first enemy encounter —15 September2026

User authorized next step:retain Opt5, diagnostics-free play build, then
representative enemy load encounter. Three self-contained active drawers:
- dist/Drowned-Opt5-Play-HD/Drowned-Play:environment only,no diagnostics.
- dist/Drowned-Encounter-Play-HD/Drowned-Play:encounter,no diagnostics.
- dist/Drowned-Encounter-020-HD/Drowned-Test:encounter,minimal cadence.
All use SPARKPAW_DROWNED_COLUMN_BLIT and retain Opt1..5 changes. Enemy flags
are isolated, not part of ordinary campaign. No runtime art edits/releases.
Required Beetle spawn552..568,surface496..608,floor200; Strider360..376,
surface328..448,floor200. Strider is clear of raised platform; Beetle fits
under it. Patrol bounds apply to existing foot probes, not full art edges.
Both use existing random speed48/96/192 and ENEMY_POLICY_RESPAWN; Strider
shoots. Level1 enemy assets temporary; no new designs/music/traversal routes.
This first encounter deliberately tests two AI types + shots + geyser/water;
full eventual density/jumps still require their own authored test.
Projectile callbacks now select nearest enemy or panel on sweep in either
direction, then dispatch hit; prior slice callbacks only hit panel.
Host actual enemy AI test verifies support bounds1200ticks,shots,projectile
ordering,panel activation,death,score and offscreen respawn. Initial test
assumed full-art patrol bounds; corrected to actual existing foot contract.
All three native builds and the full host suite pass after test correction.
63 assets/47 refs per drawer; all assets matchOpt5,68 release files unchanged.
Prior Opt5/log archived intact:older-builds/20260915-before-drowned-encounter/.
Proof:build/drowned-slice/encounter-package-proof.json. No autoplay.
Next user test Drowned-Test on020 real/cycle-exact,JIT0,2MBChip+8MBFast,
60-90s fighting both at geiser/water,open gate,leave and return for respawn;
no pause,LMB release then15s save/hold. Plain builds do not produce logs.
No FPS/enemy-headroom or visual palette acceptance claim until user review.

## Opt5 020 result —15 September2026

User played and saw no glitches. Exact package verified. Complete log:
2938intervals,2935one-field,3two-field,0three-plus;49.94fps,0ownership
violations. Sample58.82s. Long share0.102% versusOpt4 0.397%;~74% lower.
Manual workloads differ; encouraging gain, no controlled same-input claim.
Three compositor raster crossings;worst frame1112/camera433 remains.
User visual acceptance applies to this run, not proof of all DMA edge cases.
Keep Opt5 as latest tested candidate; requires drowned-column target and
SPARKPAW_DROWNED_COLUMN_BLIT. Ordinary drowned-cadence still uses Opt4 CPU
column path by design. No runtime edits/promotions/releases in this review.
Active:dist/Drowned-Opt5-020-HD. Original log kept; copy/TXT:
`testresults/Unassigned-Drowned-Opt5-020-cadence-49-94fps.log`.
Prepared Chip963640/largest962392,Fast5365944/largest5364784;
post-run Fast5742976. Lifecycle snapshots only.68 alpha.8 files unchanged.
Almost50Hz in partial slice, not stable50Hz or demonstrated enemy capacity.
Next priorities: retain successful bounded changes, assess residual frame
budget under representative enemies rather than infer margin from roundedFPS;
continue compiler/game-update audit without claiming hardware/media acceptance.

## Opt5 column DMA candidate — 15 September2026

Active manual test:dist/Drowned-Opt5-020-HD/Drowned-Opt.
Separate drowned-column target/SPARKPAW_DROWNED_COLUMN_BLIT flag on Opt4.
Only +/-16px canonical ring-column copy changes:12 one-word-wide blits
replace832 CPU row iterations (4planes x208rows), preserving all3 copies.
Source canonical front and inactive destination are Chip-backed. Final wait
retires DMA before CPU patch writes. Larger rolls keep generic path.
No new allocations, art or gameplay changes. Blitter rereads source3 times;
speed advantage uncertain. Previous targeted ring roll p95~3.92ms motivates
measurement, not a claim of DMA superiority or root cause at camera433.
At rightward camera432, origin336 admits world832 column; this coincides
with gate/other work but aggregate logs do not isolate their exact overlap.
Source/compiler CPU loop inspected; candidate host DMA model verifies all60
world columns, all planes/copies, strides, guards, twelve blits and final wait.
Native candidate and full host suite pass.
Opt4 minimal rebuilt byte-identical; code fallback intact.63 assets/47refs
verified, assets identical,68 alpha.8 release files unchanged.
Opt4/log archived intact under older-builds/20260915-before-drowned-opt5/.
Proof:build/drowned-slice/opt5-package-proof.json. No automatic emulator run.
Next02060-90s scroll left/right through geyser/water/gate, jump/shoot; check
strips/trails/missing pixels; no pause; LMB release then15s save/hold.
Compare against Opt4 49.80fps,10/2518long with workload caveats.
Reject DMA candidate if cadence/visuals regress. Stable50Hz/enemies pending.

## Opt4 020 result — 15 September2026

User completed exact verified Opt4 build:2518 intervals,2508 one-field,
10 two-field,0 three-plus;49.80fps,0 ownership violations. Complete log
and memory footer. Long share0.397% versus Opt3 1.792%;~78% lower share.
Sample50.56s versus55.66s. Stronger improvement than prior micro-changes,
but manual inputs differ; do not claim controlled same-workload gain.
11 compositor raster wraps are NOT ring-update counts. No event buckets
in this minimal build. Worst frame860/camera433 remains unresolved.
Original retained; copy/TXT at
`testresults/Unassigned-Drowned-Opt4-020-cadence-49-80fps.log`.
Package hashes match opt4-package-proof.json;68 release files unchanged.
Prepared Chip963640/largest962392,Fast5366200/largest5365040;
post-run Fast5743496. Lifecycle snapshots only. Retain gate early rejection;
next focus remaining camera433/column-transfer peak, broader engine audit.
Still not sustained50Hz or proof of enemy capacity. No runtime edits this review.
Active drawer:dist/Drowned-Opt4-020-HD.

## Opt4 gate collision candidate — 15 September 2026

Active user test: dist/Drowned-Opt4-020-HD/Drowned-Opt. Opt3 retained;
only gate collision evaluation order changes. Reject x outside800..831
before frame lookup; permanent header returns true, fully open/outside-y
returns false before calculating leaf bottom. All original point semantics.
Actual68020-O2 assembly confirms distant probes skip both calls, saved
registers reduce5->3; remaining frame lookup only on relevant leaf probes.
Assembly: build/drowned-slice/opt4-gate.asm. This proves code change, not FPS.
Truth table in actual-state host test covers45 ticks/states (closed, all
opening, fully open/saturated),963 x259 point grid plus WORD extremes:
11223765 grid comparisons plus180 extreme checks. Existing physics,
projectile/sweep/header tests and full host suite pass; native candidate passes.
No art/animation/allocation changes. Event counters off, same minimal cadence
as Opt3.63 assets/47 references verified, assets identical,68 alpha.8 files
unchanged. Events drawer/log archived intact under
older-builds/20260915-before-drowned-opt4/. Opt3 remains separately preserved.
Next user02060-90s:move/jump/shoot around geyser and gate; open gate and
continue on both sides, jump under header. No pause; LMB release then15s save.
Compare cautiously with Opt3 49.11fps,49/2734long; no gain or50Hz/headroom claim.

## Event correlation result — 15 September 2026

Complete verified log:2451 intervals,48 long,49.03fps,0 ownership violations.
8 buckets sum exactly to cadence totals.44/48 long intervals follow ring-column
updates:44/260(16.92%) versus4/2191(0.183%) without column update.
Strong association, not exclusive causation: other work coincides with copies.
Canonical water alone is not supported as main cause: no-column/no-raster-wrap
buckets total2187 intervals including1094 water updates, with0 long intervals.
47 raster wraps,46 long intervals with that bit; wraps are NOT ring counts.
Worst frame892/camera433. Full log preserved with TXT:
`testresults/Unassigned-Drowned-020-event-correlation.log`.
Exact package hashes and68 release files verified; original log retained.
Opt3 remains retained by explicit user preference. No runtime change this
review. Next bounded candidate: gate collision early spatial/state rejection
from compiled-code audit; then minimal cadence measurement. Continue ring-copy
and inclusive game-update audit, without combining hypotheses or claiming50Hz.

## Opt3 retained; broader engine audit and events — 14 September 2026

User explicitly requests retaining Opt3 and reports smoother feel; retain it
without claiming proven gain. User also requests broader engine/renderer and
compiled-code audit. See docs/DROWNED_020_ENGINE_AUDIT.md for findings.
CORRECTION: log `wraps` counts compositor raster crossings, NOT ring rolls.
Earlier notes calling it ring wraps were inaccurate. Do not infer scrolling
causation from that aggregate. New eight-bucket event diagnostic correlates
actual ring-column/water/raster flags of preceding update with its interval.
No extra timer reads or broad scopes; small counting overhead remains.
Active manual test: dist/Drowned-Events-020-HD/Drowned-Events. Same Opt3
runtime/art; rebuilt minimal executable is byte-identical to played Opt3.
Compiler audit: gate collision calls frame/overlap routines before distant-x
rejection; saves five registers and frame uses32bit division when open.
Next bounded optimization hypothesis is early spatial/state rejection;
not yet implemented or measured. Geiser division lower priority.
Actual five translation units compiled to build/drowned-slice/audit-*.asm;
final linked address/cache-placement attribution still requires proof.
63 assets/47 references verified;68 release files unchanged. Opt3 including
log/metadata archived intact in older-builds/20260914-before-drowned-events/.
Next user run60-90s:~10s stationary near active geyser, then move through
water/geyser/gate, jumps/shots; no pause; LMB release then15s save/hold.
Event data will separate raster boundary crossings from actual ring updates.

## Opt3 020 result — 14 September 2026

Complete verified user log:2734 intervals,2685 one-field,49 two-field,
0 three-plus;49.11fps,51wraps,0 ownership violations. Sample55.66s;
long interval share1.79%. Opt2:49.07fps,40/2126long(1.88%),43.32s,40wraps.
Difference0.04fps is not convincing gain across manual workloads. Opt3
remains candidate, not performance-accepted. No further runtime edit this review.
Worst aggregate frame0/camera0; do not infer a moving spike from this entry.
Similar wrap/long counts are only an investigation lead, not causal proof.
Original log retained; exact copy/TXT:
`testresults/Unassigned-Drowned-Opt3-020-cadence-49fps.log`.
Executable/assets verified against opt3-package-proof.json;68 release files
unchanged. Complete memory footer:prepared Chip963640/largest962392,
Fast5367224/largest5365800;post-run Fast5744520. Lifecycle snapshots only.
Active drawer remains dist/Drowned-Opt3-020-HD. Next investigate individual
roll events versus long frames and game-update costs before another copy
micro-optimization. Stable50Hz/enemy headroom still unproven.

## Drowned Opt3 water transfer candidate — 14 September 2026

Active manual020 test: `sparkpaw/dist/Drowned-Opt3-020-HD/Drowned-Opt`.
Single change from played Opt2: Drowned canonical water-to-target sync now
uses the same fixed-stride rectangle copier as the mechanisms. Original
phase tracking, visibility, 80x11 pixels at y197, clipping, wrapping and
three physical copies retained. No asset/animation/allocation change.
Compile-guarded to Drowned canonical restore; campaign paths unchanged.
Normal campaign executable rebuilt byte-identical to pre-change binary.
Minimal cadence instrumentation unchanged. Full host suite including27840
actual-C ASan/UBSan rectangle cases (water dimensions and edge clipping)
and native candidate/production builds pass. No emulator self-test.
63 runtime assets/47 embedded references verified; all assets match Opt2;
68 alpha.8 files unchanged. Opt2 including full user log/metadata archived
intact under dist/older-builds/20260914-before-drowned-opt3/.
Proof: build/drowned-slice/opt3-package-proof.json.
Prior targeted water sync avg1.83ms motivates this hypothesis, not a gain
claim. Compare new minimal cadence to Opt2 49.07fps,40/2126 long intervals,
43.32s/40wraps; account for manual workload differences. Next user60-90s
water/geyser/gate with camera movement/jumps/shots, no pause, LMB release
then15s save/hold. Sustained50Hz and enemy headroom remain unproven.

## Opt2 020 result — 14 September 2026

User completed Opt2. Complete verified log:2126 intervals,2086 one-field,
40 two-field,0 three-plus;49.07fps,0 ownership violations. Long intervals
1.88% versus Opt1 3.55%. Sample43.32s/40 wraps versus21.58s/37 wraps.
Manual workloads differ; lower long-frame share is encouraging, not a
controlled attribution of gain to the kernel. Absolute long intervals
40 versus37 remain similar with similar wrap totals. Do not infer causal
correlation from aggregate counts. Worst frame686,camera433.
Prepared free Chip964344/largest962784,Fast5367384/largest5366104;
post-run Fast5744416. Complete footer; lifecycle snapshots only.
Executable/assets match opt2-package-proof.json;68 alpha.8 files unchanged.
Original log kept; copy plus TXT at
`testresults/Unassigned-Drowned-Opt2-020-cadence-49fps.log`.
Active drawer remains `dist/Drowned-Opt2-020-HD`. No runtime changes this review.
Next target: water target sync and remaining camera433 spike; prior targeted
water sync avg1.83ms. Stable50Hz and spare enemy capacity remain unproven.

## Drowned Opt2 candidate and profile result — 13 September 2026

Active test: `sparkpaw/dist/Drowned-Opt2-020-HD/Drowned-Opt`.
User completed targeted profile:1104 intervals,1063 one-field,41 two-field,
0 three-plus,48.20fps,0 ownership violations; complete log/RAM footer.
Preserved in testresults/Unassigned-Drowned-020-targeted-profile.log +TXT.
Profile observer overhead prevents direct Opt1 speed comparison.
Mechanism target sync max5358 CIA ticks(~7.55ms),p953958(~5.58ms);
water target sync avg1300(~1.83ms),p952262(~3.19ms). Canonical water
avg152(~0.21ms). Game update avg3015(~4.25ms) also deserves follow-up.
Aggregate maxima need not occur together; no frame-level causal proof.

Opt2 hypothesis: dedicated Drowned canonical rectangle copy advances row
pointers instead of rebuilding addresses every row. Preserves all three
physical ring copies, clipping, wrapping, exact source pixels and target
phase bookkeeping. No asset, animation, gameplay or allocation changes.
Only mechanism target sync changed; water/game optimizations deferred to
avoid combining hypotheses. Seven timing scopes disabled: original minimal
cadence configuration restored. Actual C ASan/UBSan oracle covers27840
rectangles/four planes/three copies; full host suite and native candidate
build pass. Compiler assembly retained in build/drowned-slice/opt2-renderer.asm.
No automatic emulator run; FPS gain/50Hz/enemy headroom still unproven.
63 assets/47 literal references verified, assets byte-identical to profile,
all68 alpha.8 release files unchanged. Full profile drawer/log archived intact
under dist/older-builds/20260913-before-drowned-opt2/.
Next manual020 run60-90s through geyser/water/gate, jump/shoot, no pause;
LMB press/release then~15s save/hold. Compare cautiously with Opt1 48.28fps.

## Targeted 020 profile — 13 September 2026

Active manual test: `sparkpaw/dist/Drowned-Profile-020-HD/Drowned-Profile`.
Seven disjoint CIA scopes: game update, sprite stage, canonical water,
canonical mechanism patches, ring roll, dynamic water sync, mechanism sync.
Existing broad/nested and wait profiling remain disabled. Profile flag and
scope count explicitly logged; cadence includes observer overhead and must
not be treated as an Opt1 speed comparison. This is diagnosis, not Opt2.
All visuals/assets/gameplay unchanged. Rebuilt minimal cadence executable
is byte-identical to the played Opt1 executable. Full host suite and native
profile/minimal builds pass. No automatic emulator test. Two extra profile
slots add 8192 sample bytes plus their small counters only in this profile.
63 runtime assets/47 executable references verified; all68 alpha.8 release
files unchanged. Opt1 drawer, complete user log and metadata archived intact
under `dist/older-builds/20260913-before-drowned-profile/`.
Package hashes: `build/drowned-slice/profile-package-proof.json`.
Next: manual60-90s on68020 real/cycle-exact,JIT0,2MBChip+8MBFast through
water/geyser/gate, no pause; LMB release then15s save/hold. Read seven scope
median/p95/max values before choosing the next runtime optimization; validate
its gain separately with minimal cadence. Sustained50Hz/enemy margin pending.

## Opt1 020 result — 13 September 2026

User reports slightly improved feel. Full log:1042 intervals,1005 one-field,
37 two-field,0 three-plus;48.28fps;0 ownership violations. Long intervals3.55%
versus prior12.65%. Sample21.58s versus28.58s;37 ring wraps versus18 previously.
Manual workloads differ: do not present this as controlled same-workload gain.
Aggregate FPS increase4.02 (~9.1%) supports the subjective improvement.
Still not sustained50Hz or proof of spare enemy capacity.

Complete prepared_peak/post_run footer present;7795-byte log. Prepared free:
Chip963640 (largest962392), Fast5367224 (largest5365784). Post-run Fast5742752.
These are diagnostic lifecycle snapshots, not worst-case full-level RAM proof.
Worst logged frame440,camera433; no targeted family timing in minimal build.
No memory ownership violations recorded. Original log retained; copy unchanged.
Native executable hash matches opt1-package-proof.json. Same 020 test context;
last verified configuration68020 real/cycle-exact,JIT0,2MBChip+8MBFast.

Next performance question: identify remaining37 long intervals with bounded
update/ring/patch/sprite timing, then validate any change with minimal cadence.
Preserve visual quality and approved gameplay. Full enemy/music workload gate
still required after stable base cadence; no claim of completion or release.

## Drowned Opt1 — compact transfers, 13 September 2026

User requests thorough optimization with future enemy headroom. Active manual
020 test: sparkpaw/dist/Drowned-Opt1-020-HD/Drowned-Opt. Preserve visuals.
First measured hypothesis: replace full96x96 local transfers with jet32x64
(x448/y131) and gate80x61(x768/y136), both canonical and target synchronization.
No game state/animation timing/collision changes. Existing atlas repacked as
16x5422 planar word stream: jet offsets f*256; gate2304+(f-9)*610 per plane.
Loader rejects old96x2208 layout. No added runtime cache. Fast planes reduced
105984->43376 (-62608); Chip stage1152->610 (-542). Per-change rectangle area
falls77.78% jet,47.05% gate. FPS gain/headroom NOT established yet.
All277 full-patch transitions pixel-identical; played baseline full art and
foreground independently compared. Real renderer transfer under ASan/UBSan
checks all frames, guards, DMA staging, target phase sync and culling. Full host
suite and native diagnostic/normal builds pass. Existing per-frame minimal
cadence instrumentation retained; only save output omits verbose trace for
Drowned minimal runs, so prepared_peak/post_run follow promptly.
Stager verified63 assets/47 references;68 alpha.8 files unchanged. Baseline
including partial log archived intact under older-builds/20260913-before-drowned-opt1/.
Next user run on confirmed020 real/cycle-exact,2MBChip+8MBFast,JIT0:60-90s
moving through geyser/water/gate, LMB press/release then ~15s save/hold. Read new
log from exact drawer. Compare workload cautiously with44.26FPS baseline;
if still missing50Hz, add targeted patch/ring/update measurements, not broad
unmeasured rewrites. Stable50Hz in empty slice alone is not enemy headroom proof.
No release/version/commit/push or automatic emulator run.

## 020 configuration confirmed from emulator log — 13 September 2026

User notices more drops near the geyser, possibly with gap-water in view.
FS-UAE Cache/Logs/debug.uae (21:41) and fs-uae.log.txt (21:44) confirm effective
68020, 2048KB Chip,8192KB Fast,cpu_speed=real,CPU/memory/Blitter cycle-exact,
JIT=0. Saved a1200.fs-uae preset still says68030; do not confuse that preset
with the effective last-run configuration. Only Launcher was running at read.
Read-only config/log snapshots preserved under testresults as
Unassigned-Drowned-Slice4-020-effective-config.uae and matching -emulator.log.
No config changes. Separate water (every2 game ticks) and geyser active frames
(every4 game ticks) can add work in one view. Geiser copies a96x96 patch for
its32x64 effect plus target synchronization: bounded-area optimization is a
specific hypothesis, not measured proof. Preserve visual quality; investigate
patch and water/scroll costs next under this confirmed configuration.

## First Drowned 020 measurement — 13 September 2026

User completed Drowned-Cadence-020-HD run. Complete aggregate header reports
1265 intervals:1105 one-field,156 two-field,4 three-field;44.26 FPS,
12.65% long intervals,0 ownership violations. Sample covers about28.58s.
Partial detail log ends mid-record363; post_run/prepared_peak absent. Do not
claim full flush or RAM numbers. Preserved byte-identically as
sparkpaw/testresults/Unassigned-Drowned-Slice4-020-cadence-44fps-partial.log
with TXT; original log retained. Executable matches package proof SHA256.
Stale alpha41 log-title is hardcoded, not the actual build identity.
CPU clock/JIT/cycle-exact and subjective location requested, pending.
No cause established. Source audit: local jet still transfers full96x96 patches
for a32x64 effect; both inactive targets synchronize phase changes. This is a
profiling hypothesis, not proof of the drops. Need targeted update/patch/sprite/
ring-cost measurements; keep art intact. Broad family timings are disabled.
No new runtime/build changes in this analysis; current cadence drawer retained.
Do not compare this workload directly to historical Level1 FPS or advance level
scope before the performance gate. Ten-second flush advice was insufficient
for this observed partial detailed log; use compact output or completion marker
in the next diagnostic, retaining the existing evidence.

## Drowned 68020 cadence gate — 13 September 2026

User accepts Slice4 presentation and reports visible FPS drops after a 020 run.
Pause further art/gameplay expansion until measured. Exact CPU/emulator settings
and location of drops have not yet been supplied; no numeric FPS inferred.
Active test: sparkpaw/dist/Drowned-Cadence-020-HD/Drowned-Cadence.
Same 63 assets byte-identical to played Slice4, same route and gameplay;
no optimization. Existing minimal-cadence diagnostic, broad family/wait
profiling disabled. Direct-slice exit now saves before display cleanup and
holds the final image, matching the usual LMB save lifecycle. Normal Slice4
rebuilt byte-identically (SHA256 e720e5e76671a5587643c5d0ca948430431a795ec58711378646e05a1954d060).
Native diagnostic/baseline builds and pause/sprite-occlusion host checks pass.
63 assets/47 references verified, 68 alpha.8 files unchanged. Previous Slice4
archived intact under older-builds/20260913-before-drowned-cadence/.
User: 60-90 seconds moving through route/geyser/open gate, no pause; press and
release LMB, expect frozen image, wait ~10 seconds then stop/reset. Read the
renderdiag.log from this exact drawer; correlate with subjective drop location
and CPU speed/cycle-exact/JIT settings. Do not equate microbenchmark/diagnostic
FPS with full-level performance. No automatic emulator run or release.

## Drowned Slice4 — solid housing integrated, 13 September 2026

User approved the solid gate concept. Current manual test:
sparkpaw/dist/Drowned-Slice4-030-HD/Drowned-Slice, first FS-UAE/68030.
Fixed opaque housing at x800..831/y120..135, shared by all 14 gate states.
Same constants drive art and permanent collision; explicit vertical-span
intersection handles its non-tile-aligned top. Local player head clearance
uses the full sprite top (player.y-8), stopping upward/side penetration without
damage; ordinary production physics is compile-isolated from these changes.
Actual C tests cover interior jump/fall, both side approaches, landing on top,
crouch/stand, reset, sweep/span parity plus existing traversal/mechanisms.
Native slice/normal builds and full host suite pass. 393 front pixels changed,
all inside housing; geyser patches byte-identical. No new asset allocation.
Stager: 63 assets/47 executable references; 68 alpha.8 files unchanged.
Slice3 archived byte-identically under older-builds/20260913-before-drowned-slice4/.
No automatic emulator run; new visual/function review pending, 020 timing later.
See sparkpaw/docs/DROWNED_GATE_SOLIDITY_REVIEW.md. No release/version/commit/push.

## Slice3 gate solidity feedback — 13 September 2026

User accepts improved geyser and through-gate depth, rejects upper machinery
transparency and jumping through the header. MOV 21-07-03 was supplied after
initial absence, inspected in consecutive frames and catalogued with both
screenshots. See sparkpaw/docs/DROWNED_GATE_SOLIDITY_REVIEW.md for evidence,
cause and concrete geometry/visual-head/span-query requirements.
New art: sparkpaw/assets/concept/drowned-solid-gate-source-v1.png (opaque roller
housing, closed/open), pending user concept review before native integration.
No runtime or dist changes this turn; Slice3 remains the active candidate.

## Drowned Slice 3 — 13 September 2026

User approved the recessed boiling-water concept and requested integration.
Active manual test: sparkpaw/dist/Drowned-Slice3-030-HD/Drowned-Slice.
Eight native 32x64 water frames replace the rejected cyan jet, with a fixed
32x6 iron lip hiding the outlet. Same water pens 0/5/6/11, blue-dominant role
balance, whole-family scale, second sheet row registered +4px as a unit.
Warning now alternates low frames; hazard timing and hitbox remain unchanged.
Right sluice post clips only the inactive attached-player stage; left post
stays behind Paw. Same HUD/physics/world/panorama. No enemies/music/Core yet.
Native slice and normal campaign builds plus full host suite pass. 63 assets,
47 executable references checked; 68 alpha.8 release files byte-identical.
Slice2 archived intact under dist/older-builds/20260913-before-drowned-slice3/.
No automatic emulator run; 030 visual/function review next, 020 timing later.
Same 105984 Fast patch bytes / 1152 Chip stage, plus 352-byte near-post mask
and stage flags. Whole-program RAM and 50fps are not established by host tests.
Sources/manifests/previews: assets/concept/drowned-geyser-native-v2/ under sparkpaw.
See DROWNED_SLICE2_REVIEW.md for original evidence and follow-up; no release.

## Drowned Slice 2 follow-up — 13 September 2026

User MOV review requests right-post occlusion and rejects the jet/nozzle art.
See sparkpaw/docs/DROWNED_SLICE2_REVIEW.md for evidence and next gates.
Source now has isolated inactive-player-stage near-post clipping, with native
slice/normal builds and full host suite passing; no emulator verification yet.
New recessed boiling-water concept: sparkpaw/assets/concept/drowned-geyser-source-v2.png,
pending user review and native palette conversion. Dist Slice2 remains unchanged;
next coherent staging combines the depth fix and approved jet. No release.

## Drowned Slice 2 — 13 September 2026

User approved the new native foreground, jet art and shutter animation.
Integrated focused candidate: sparkpaw/dist/Drowned-Slice2-030-HD/Drowned-Slice.
New steel foreground, correct foreground Copper palette, animated pressure jet
and opening shutter with collision following the moving lower edge. Existing
player/HUD/water and two-gap route retained. No enemy instances/new music/Core.
Native candidate and normal builds, existing host suite and actual C mechanism/
renderer-transfer tests pass; FS-UAE/030 visual acceptance is pending. No auto
emulator run. 020/50fps and whole-program Chip/Fast measurements remain open.
23 local patches occupy 105984 Fast plane bytes; one 1152-byte Chip stage is
reused after WaitBlit. No DMA pointers into Fast and no per-frame allocations.
Stager verified 63 assets/47 references; 68 alpha.8 files remain byte-identical.
Slice1 archived intact at sparkpaw/dist/older-builds/20260913-before-drowned-slice2/.
No release/version/commit/push. Preserve all local changes. See active Drowned
plan and candidate ReadMe; next action is the user's 030 visual/function review.

## Drowned visual review supersedes pending status — 13 September 2026

User rejects Slice1 foreground/mechanism art; gameplay idea remains promising.
Prioritize stronger pixelart and animation; Level 1 may be restyled later and
is not the quality ceiling. ADF capacity is deferred, but 68020 / 50 fps /
2 MB Chip + 8 MB Fast remain design and measurement gates. New material/form
concept: sparkpaw/assets/concept/drowned-polish-direction-v1.png, review pending;
not native indexed art or approved geometry. No runtime/build/dist changes in
this concept step. See DROWNED_TURBINES_PLAN.md for acceptance and next gates.

## Drowned Turbines environment candidate — 13 September 2026

Current built release in dist is **alpha.8**. User confirms physical-floppy
spritefix acceptance: finish Level 1, CONTINUE, Disk 2 swap and long INSERT
DISK 2 waits all work. HD/WHDLoad also pass their hardware tests. Historical
alpha.7/pending statements below do not override this newer user evidence.

Active focused test: `sparkpaw/dist/Drowned-Slice1-030-HD/Drowned-Slice` (paths
relative to workspace root). Direct-start 960px environment with existing
player/HUD/water, two gaps, timed pressure jet and shoot-to-open shutter.
Compile-isolated candidate; no release/version/commit/push. Native candidate,
normal build, full host suite and actual C traversal/mechanism checks pass.
FS-UAE/68030 visual acceptance is pending; no automatic emulator launch.
No enemy instances/new music/Core/finale yet: this is an early partial gate.
See sparkpaw/docs/DROWNED_TURBINES_PLAN.md and the staged ReadMe for exact scope.
Stager verified 63 assets/47 executable references and preserved 68 release
files. Previous SpriteGuard drawer archived intact under
sparkpaw/dist/older-builds/20260913-before-drowned-slice/. Preserve all local
changes; next step is user 030 review before broader slice work or 020 staging.

## Physical floppy Disk 2 sprite-guard candidate — 10 September 2026

User reports Level1 -> CONTINUE -> Disk2 glitches/hang on several real
floppies, not reproducible in FS-UAE/Pocket; HD/WHDLoad work. Direct OPTIONS
Stormrail and defeat -> game over -> Disk1 work on physical floppies.
IMG_3185.MOV is preserved as Phase 7A.3-real-amiga-adf-disk2-prompt-glitches.MOV
with sidecar. Consecutive final frames show narrow coloured noise strips;
initial coarse sampling missed them. A crash endpoint is not visible.

Candidate: dist/Disk2-SpriteGuard-ADF, use both images. ADF-only presentation
Copper clears sprite DMA, all eight CTL/DATA/DATB registers and points sprites
to 16 bytes of cleared resident Chip RAM each frame. Copper buffers grow by
400 Chip bytes total; null sprite adds 16. No gameplay/audio/compression change.
This tests stale sprite display state; actual unexpected sprite DMA enable and
the hang cause are unproven. Native build, actual Copper host test (398/480
words), menu/campaign/game-over checks, Shrinkler verification and all ADF
file/decoder readbacks pass. Free blocks 73/185. Real-floppy acceptance pending;
no emulator run, release, commit or push. Previous HD/ADF test pair archived
intact under older-builds/*-before-sprite-guard; alpha.7 release unchanged.

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


## HUD campaign carry correction — 10 September 2026

User reports default 3 lives/full hearts in Stormrail approach/boarding after
finishing Level 1 with 2 lives/3 health units; flight shows the carried values.
Source: hudPrepare seeded both initial buffers with defaults, while the live
renderer selected fresh walking-player health until LAUNCH_OUT. Both hidden
HUD buffers now start from current lives/diamonds/score and the appropriate
health source; Stormrail HUD reads carried Stormrail health from approach on.
No gameplay vitals or campaign banking rules are changed.

Explicitly preserve OPTIONS -> Stormrail: a fresh direct start gets 3 lives,
6 health units (3 full hearts), zero carried diamonds/score. Replay restores
its section-entry snapshot. Actual HUD initialization/selection tests cover
these cases plus normal Level-1 health; actual OPTIONS helper ordering/failure
checks and campaign contract pass. HD/ADF native builds succeed. Visual native
acceptance remains pending. The separate black-after-Fire report is still open.
Current HD + ADF test directories are refreshed together; previous versions
are archived intact, alpha.5 remains unchanged.


## Current HD + ADF comparison — 10 September 2026

Explicit user request: retain both media variants of the latest test state in
dist. Active set: GameOver-Current-030-HD (launch Sparkpaw) and
GameOver-2Disk-030-ADF. Both start with normal three lives; HD retains its
intro/Soundtest. This is one current test generation, an explicit exception to
only one test drawer. Preserve alpha.5 and archive only superseded generations.

User reports a suspected black screen after Fire on game over, after waiting
on that screen for a long time. User confirms the earlier HD game-over test, before ADF compression. Do not
attribute it to the one-second input gate or claim a fix. HD is rebuilt from
the current source for reproduction/comparison. Native transition acceptance
is still pending. No runtime change was made on the basis of this report.


## Two-disk compression candidate — 10 September 2026

Two standard ADFs now fit the complete centered game-over scene and unchanged
Storm Light music. Shrinkler reduces the native executable 224,388 -> 89,760
bytes; lossless SPD1 delta+LZ stores Storm Light's sample bank in 72,031 bytes
and Neon Sky's in 70,892. Native data reader: 85 host cases pass, including
full banks and corruption; disk marker/DF0/DF1 source checks pass. Shrinkler
hunk/relocation verification and complete ADF readback pass. Free blocks:
Disk1 73 (36.5 KiB), Disk2 185 (92.5 KiB), both above the 32-block reserve.
New markers SP07G1/SP07G2 prevent mixing with alpha.5 disks.

Active test set: **GameOver-Current-030-HD + GameOver-2Disk-030-ADF**, with normal **three lives**.
GameOver-Centered-030-HD is now archived under dist/older-builds.
Latest alpha.5 release remains byte-identical. Native boot, unpack duration,
audio, single-drive swaps and final hardware acceptance are pending. No auto
FS-UAE run, release, version change, commit or push. Three disks are no longer
needed for the measured current content; old three-disk recommendations below
are superseded. Experimental SPARKPAW_THREE_ADF code is unused.
Research, sources, comparison and reproduction: docs/ADF_COMPRESSION_RESEARCH.md
(relative to sparkpaw). Release packaging integration is a later accepted
checkpoint; the current candidate uses tools/crunch_adf_executable.py followed
by package_multidisk_probe.py --crunched-executable with its proof JSON.


## Standing dist policy — user instruction, 10 September 2026

Keep `sparkpaw/dist/` limited to the **latest alpha release artifact set**
(all its HD/ADF/WHDLoad packages and extracted release drawers),
**only the latest test version**, and `older-builds/`. Preserve existing
fixed user storage such as `my-files/` if present. After staging and verifying
a newer test, immediately move every superseded test version intact into
`dist/older-builds/`, including logs, assets and associated `.uaem`/`.info`
metadata. Never delete or overwrite archived evidence; use a unique archive
name on collisions. Do not archive the latest alpha release when adding a
test. Older alpha releases move to older-builds only after a newer release
is complete and verified. No additional permission is needed for this
standing housekeeping instruction. An explicit user-requested A/B comparison
may retain its variants together as the latest test set.

Current release: **0.7.0-alpha.7**.
Active test set: **GameOver-Current-030-HD + GameOver-2Disk-030-ADF**.
GameOver-StormLight-030-HD, Soundtest-Resume-HD and StormLight-Soundtest-HD
are archived under older-builds. Historical test paths below describe prior
work and must not be interpreted as additional active test versions.


Game-over layout revision (10 September): user screenshot marks the open
area left of Sparkpaw. Shared static text and dynamic total now center at
native x123; title y35, total label y70, digits y87, instruction y113.
BACK TO TITLE replaced by PRESS FIRE TO CONTINUE; Fire still returns to title.
Outlined small text remains readable over the clouds. Generated coordinate
constants keep the runtime total aligned with the offline preview.
New one-life HD candidate: sparkpaw/dist/GameOver-Centered-030-HD/GameOver-1Life.
Includes Storm Light Soundtest entry. Prior test drawers preserved.


Soundtest extension (10 September): STORM LIGHT is now the fifth MUSIC TEST
track, using musicPlayGameOver/LSP (never the CIA gameplay preview). Tests
cover all five tracks, failure cleanup, start/stop/restart and bidirectional
wraparound. Offline cache/reference coverage: 1,070 states / 4,968 transitions;
284,383 Fast bytes (+6,272), no extra Chip bitmap. ADF cache unchanged.
New normal-three-life HD drawer: sparkpaw/dist/StormLight-Soundtest-HD,
launch Sparkpaw-Audio. Existing one-life game-over drawer remains unchanged.
Native HD builds; subjective audio/native cadence acceptance pending.


Game-over candidate (10 September 2026): approved defeated-Sparkpaw V2 is
converted to a shared 320x256/64-colour scene. Last life is terminal, with
score preservation and BACK TO TITLE. User selected StormLight.mod after
rejecting both generated After the Storm cues; identical master copied into
music/game-over, LSP data 207,919 bytes. HD quick test starts with one life:
`sparkpaw/dist/GameOver-StormLight-030-HD/GameOver-1Life`. Normal HD remains
three lives. Host suite passed before music replacement; focused terminal-life,
Copper fade, ownership and campaign checks also passed after replacement.
Native HD compiles; user visual/audio/030 acceptance is still pending.
ADF cache subset passes all 50 states/328 transitions and saves ~38 kB.
Two-disk capacity failed even with the rejected 79-kB cue and obsolete menu
atlas removed. No new ADF set is ready; three-disk layout/coverage/prompts and
swap verification remain unfinished. SPARKPAW_THREE_ADF code is experimental
scaffolding, not a usable build mode yet. Preserve all previous local audio,
itch text, statistics and release art. No release/version/commit/push.


READY/Soundtest repair candidate (9 September):
`sparkpaw/dist/Soundtest-Resume-HD/Sparkpaw-Audio` retains the offline 020 menu
cache repair and now resumes the still-visible menu after music loading without
restarting Copper or disabling display DMA. The user reports occasional brief
glitches when starting another module; source identifies an unsynchronized
full takeover at that point. Repaired native glitch acceptance remains pending;
no general 020-cache acceptance is inferred from this report. Mandatory future
menu rules: `sparkpaw/docs/READY_UI_PERFORMANCE_CONTRACT.md`. Audio options,
soundtest, local P-pause and layout are retained; alpha.5 release files and
local itch/statistics/artwork are preserved. No release/commit/push or automatic
emulator run. The preceding Fast-HD candidate is archived intact.


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

## Audio options / soundtest candidate — 9 September 2026

Candidate against alpha.5 / `a249c98`: user confirms gameplay-only AUDIO
MODE directly in the existing OPTIONS screen for every edition. HD/WHDLoad
adds SOUNDTEST there, opening two selectors: SFX TEST and MUSIC TEST. No AUDIO
submenu. Up/Down selects rows; Left/Right changes values. Intro/title/results
audio stays unchanged. Full 16-effect inventory,
four-track availability, READY/IRQ/DMA ownership, memory/media costs and one
implemented candidate: [AUDIO_OPTIONS_SOUNDTEST_RESEARCH.md](AUDIO_OPTIONS_SOUNDTEST_RESEARCH.md).
HD candidate staged; full host suite and HD native compilation pass. User native
acceptance is pending; no automatic emulator run, release, commit or push.

## Historical investigation record — superseded by alpha.5 above

## Phase trace 020 findings — 8 September 2026

Both complete 020 logs show two seven-field gaps at water reset + next frame,
including original-SFX A. Time is in renderer/Bob phase, consistent with both
rolling targets rebuilding after the camera jump. New audio is not required
for those gaps; general renderer optimization remains parked.
B separately has six three-field ordinary-gameplay gaps, with rendering near/
after the line<=4 publication window. Audio/trace/workload attribution remains
open; no full performance acceptance or runtime fix. Logs preserved; active
Level1-Trace-A/B-030-HD pair retained. See LEVEL1_AUDIO_PROOF.md for exact data.

## Active Level-1 audio gameplay candidate — 7 September 2026

Level1-Audio-A-030-HD (original SFX) and Level1-Audio-B-030-HD (music+mixer),
executable Level1-Audio. Direct Level-1 start, same game/render source and seed.
LMB/Escape stops, restores OS and saves. Native build and adapter/stager host
checks pass. Supplied 030 and 020 gameplay/audio is positive; both 020 logs
complete. User notices no FPS difference, B includes water and longer play.
020 aggregate rates ~49.68/49.27; B includes zero/two-field pairs plus two
three- and two seven-field deltas. Timing attribution remains OPEN; no full
performance acceptance or audio-regression claim. Review sampling boundaries
before another targeted trace; routes differ. See LEVEL1_AUDIO_PROOF.md.
No production src edits or release changes. Prior Audio-Block pair archived.
See LEVEL1_AUDIO_PROOF.md and experiments/audio-gameplay/README.md.

## Block-mixer 020 result — 7 September 2026

Supplied Audio-Block-A/B 020 listening is positive; both scripts complete and
all effect counts match. Measured service fraction falls from 13.32% to 3.06%;
mixer time falls about 85.2%, with music/DMA nearly unchanged. This supports
the next scoped Level-1 integration candidate. Average service timing is not
worst-window CPU-budget or integrated gameplay acceptance. Production and all
release baselines remain unchanged. Logs and limits: LEVEL1_AUDIO_PROOF.md.
Current pair remains available; archive intact when staging the next candidate.

## Previous audio timing gate — 7 September 2026

User authorized timing then Level-1 gameplay proof. Active set:
Audio-EClock-A-030-HD (ReadEClock service timing) and Audio-EClock-B-030-HD
(unmeasured control). Both run Audio-Level1 for 94 seconds, save and return.
Supplied E-clock A/B 030 and 020 listening is positive; effect counts match.
020 instrumented service bodies consume 13.32% of elapsed time, mostly mixer
(12.04%). This does not establish exact production CPU cost and does not meet
the provisional efficiency gate. Reduce isolated mixer work before gameplay
integration; general gameplay performance stays parked. See proof document.
Previous audition drawer archived intact. See LEVEL1_AUDIO_PROOF.md.

## Completed standalone listening candidate — 7 September 2026

Research advanced to one user-authorized audio-only proof. See
[LEVEL1_AUDIO_PROOF.md](LEVEL1_AUDIO_PROOF.md). Sole drawer Audio-Level1-030-HD;
run Audio-Level1 for ~94 seconds or LMB stop/save. It returns to the OS.
Copper Sprint is three-voice 164-BPM Level-1 accompaniment, with all 15 original
SFX plus louder health variant. Intro/title retain four channels unchanged.
Native compile/host checks pass. User completed both 030 and 020 auditions, reporting good sound and no
perceived difference. All effect start/suppression counts match between runs.
Raster timing is inconclusive; integrated gameplay remains unverified. See the proof document for evidence.
All 61 alpha.4 release files remain unchanged; no new release/commit/push.


## Audio system research — 7 September 2026

User reopens player/sharing research and broadens it to the complete audio system.
Research only; alpha.4 runtime/releases remain unchanged. Prefer evaluating an
independent audio clock, faithful direct/converted replay and three uninterrupted music voices
plus two SFX mixed onto the fourth Paula output. Two music/two direct SFX is the
fallback. No backend selected or integrated; no native concurrency proof.
See [audio system plan](AUDIO_SYSTEM_PLAN.md) and [source audit](INGAME_MUSIC_AUDIT.md).
General gameplay performance remains parked. No build/emulator/release/commit/push.

## Alpha.4 maintenance checkpoint — 7 September 2026

Includes direct OPTIONS Stormrail loading and safe intro DMA retirement/LMB
latching. No further input rewrite: joystick-Fire still advances text, no new
VBlank input server, and FS-UAE-only behavior is not established. Native symptom
resolution remains pending. User requested release/commit/push with minimal
docs and no new itch notes. See RELEASE_0_7_0_ALPHA_4.md and artifact JSON.

Earlier alpha.3/candidate entries below are historical.

## Intro skip/display retirement candidate — 7 September 2026

User reports intermittent old intro imagery after title when repeatedly
clicking LMB or skipping during text scroll. No recording supplied; timing
cause not proven. Source audit found active intro Copper/bitplane DMA remained
pointed at Chip memory after assetsUnloadStoryIntro, until next plate/title
installation. After the existing black fade, stop RASTER/COPPER/SPRITE DMA and
wait a VBlank before unloading. Audio DMA/OS interrupts stay active. Every plate
uses this retirement; installCopper re-enables display DMA for the next image.
No extra Chip bitmap or simultaneous title/intro allocation.

LMB skip now latches through fades and passage waits (including held-fire entry).
Reset on a new title invocation; intro input disabled before title loading.
It cannot restart the intro via repeated clicks. Frame polling can still miss
sub-frame clicks; no unconditional guarantee for input during blocking I/O.

Native build and full host suite pass. Actual C skip test covers 240 passage
press timings, latch persistence and held-fire handling; structural guards check
DMA retirement before free. Native symptom resolution remains pending.
Single active set: dist/Intro-Skip-HD, includes pending Stormrail direct-start
loading fix. Previous candidate archived byte-identically; 52 assets verified,
all 61 alpha.3 release files unchanged. No emulator, release, commit or push.


## Direct Stormrail OPTIONS loading candidate — 7 September 2026

After alpha.3 release, user reports direct OPTIONS -> START AT STORMRAIL ->
START GAME loads over black, although Level-1 CONTINUE shows the disk image.
Source confirms switchPreparedLevel1ToStormrail guarded titleShowReplayLoading
with SPARKPAW_MULTI_ADF. Removed that guard so HD/WHDLoad use the same loading
presenter before teardown/load. Successful preparation now fades that presenter
before callers take over for gameplay. Existing ADF CHARGING call retained.
All three direct-start call sites (initial, Escape and back-to-title) share the
helper. Normal Level-1 start/CONTINUE implementation is unchanged.

Native build and full host suite pass. New test compiles the actual helper
with HD/WHDLoad/ADF flags and checks load ordering and failure exits. Staged
52 assets in dist/Storm-Start-Loading-HD; all 61 alpha.3 release files preserved.
Native visible transition pending. No new release, commit/push or emulator.


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

## Historical candidate log (superseded by checkpoint above)

## READY Fast4 transition preparation — 7 September 2026

User reports Fast3 better but a black pause between CHARGING and READY.
Source confirmed readyPatchPrepare and palette matching occurred after the
CHARGING fade. Moved CPU preparation and hidden READY buffer seeding before
fade-to-black, while CHARGING and OS VBlank music remain active. The displayed
loading bitmap is still overwritten only after the fade and WaitTOF. Existing
fades, CPU patch behavior and memory allocations unchanged. This removes the
preparation plus one 61440-byte copy from the black interval, not from total
loading work. Remaining native duration is unmeasured.

Native build and full host suite pass. Ordering guards cover preparation and
hidden copy before fade, displayed copy after fade. Staged 52 assets at
`dist/Ready-Fast4-HD`; all 57 release files unchanged. Fast3 archived intact.
User 020 CHARGING-to-READY visual/music check pending. No emulator, ADF,
release, commit or push.


## READY Fast3 CPU page copy — 7 September 2026

User confirms restored Fast-HD works and authorizes another optimization.
Standing preference: do not request another user test for a confidently proven
byte-identical restoration of a previously working version.

Large main/options transitions now use six unrolled uint32_t CPU copies per
row, with plane-major pointer stepping; offsets 4728/2496 and row strides
40/24 are four-byte aligned. Source stays in Fast RAM; no DMA access or extra
buffers. Small selection transitions retain the accepted sparse word copies.
Generated native 68020 assembly confirms six MOVE.L instructions per row and
simple pointer increments; no per-word branch. This proves code shape only,
not timing or disappearance of the music hiccup.

Native compile and full host suite pass, including actual C parity for all
144 transitions. Staged 52 assets in dist/Ready-Fast3-HD; all 57 release files
unchanged. Ready-Fast-HD archived intact. Focused 020 OPTIONS open/close test
pending. No emulator, new ADF, release, commit or push.


## Fast2 rejected; working CPU baseline restored — 7 September 2026

User screenshot shows a black OPTIONS rectangle while surrounding art and dust
remain visible. Evidence preserved as testresults/Unassigned-rejected-ready-fast2-
black-options.png with matching sidecar (filename has no line break).
Source confirms menu patches are dmaSource=FALSE/MEMF_FAST. Fast2 incorrectly
passed those Fast RAM pointers to the Chip-only Blitter. This was a missed
source-memory ownership check, not proof of a Copper timing problem.

Removed the Blitter route; CPU changed-span implementation restored. Rebuilt
executable is byte-identical to user-tested Ready-Fast-HD. Restored that whole
drawer byte-for-byte to dist/Ready-Fast-HD; rejected Fast2 archived intact.
Added source ownership guard against direct menu Blitter DMA. The working 020
selection/particles result stands. Small music hiccup opening/closing OPTIONS
remains unresolved; no further speculative performance change in this repair.
No emulator, release, ADF, commit or push.


## READY page-switch Blitter candidate — 7 September 2026

User reports Ready-Fast-HD much better on 68020: UP/DOWN in both pages no
longer slows music, particles work well. One small music hiccup remains when
opening/closing OPTIONS. Source/data audit: these transitions still copy all
14976 bytes (unlike the 1320-byte selection change). Added a large-page-only
Blitter path: six 192x104 A-to-D copies with 0/16 byte source/destination
modulos. Existing platformWaitBlit guards register setup, each plane and final
completion before CPU dust. READY already owns/enables Blitter DMA. Only the
hidden bitmap is written. Small selections retain the approved CPU span path.
No added buffers, music-player changes or gameplay changes. Actual timing and
Blitter visual behavior remain user 020 gates, not host-verified claims.

Native compile and full host suite pass. Staged 52 assets in Ready-Fast2-HD;
all 57 release files unchanged. Ready-Fast-HD archived byte-identically. No
ADF build, emulator, release, commit or push. Ask for the focused OPTIONS
open/close test plus retained UP/DOWN smoothness and clean text/particles.


## READY menu 68020 performance candidate — 7 September 2026

User reports dust4 ADF works, and HD READY is smooth on 030 but rapid UP/DOWN
on 020 stalls particles and slows music. No quantitative timing or hardware
acceptance is inferred. Source shows each changed hidden menu buffer copied
14976 bytes with 624 CopyMem calls; the owned READY loop also drives music.
This is a plausible missed-frame source, not a measured timing diagnosis.

Added ready_patch.c: precompute word-aligned changed spans for each row and all
12x12 menu-state pairs during READY preparation. CPU-only BSS costs 29952 bytes;
no additional Chip bitmap. Restore dust first, patch only differences into the
hidden buffer, then draw dust and publish via existing Copper boundary. Direct
16-bit copies replace per-row library calls. START GAME/OPTIONS copies 1320
bytes, 91.2% fewer than 14976, per buffer update. Music player/cadence, particles
and gameplay are unchanged. Native runtime savings remain unmeasured.

Native build and full host suite pass. Actual C parity covers all 144 menu
transitions including unchanged surroundings; dust restoration/masks cover
1600 frames. Structural ownership test updated for the extracted patch helper.
Staged 52 assets in dist/Ready-Fast-HD; all 57 release files preserved. Dust4
HD+ADF set archived byte-identically under older-builds/Ready-Dust4-HD. Focused
020 rapid-input/music test pending. No new ADF until this candidate is reviewed;
future ADF capacity still needs checking (previous pair had 5 KiB free Disk 1).
No emulator, release, commit or push.


## READY dust 4 ADF candidate — 7 September 2026

User approves current dust appearance and requests the corresponding ADF trial.
Built no-intro, title-music two-ADF candidate from current source (200792-byte
executable). Disk 1 fits with only 10 free blocks/5 KiB; Disk 2 retains 345
blocks/172.5 KiB. Initial packaging stopped at the default 32-block reserve.
Added explicit --minimum-free-blocks option, default still 32, and used 1 for
this test. No filesystem capacity/readback checks bypassed. Both 901120-byte
DOS1 images, boot checksum, all file readbacks, per-volume compiled references
and actual C packed decoding pass; 45 loader cases and disk-media host tests
also pass. Existing ADF lossless packing retained; HD compression unchanged.

Pair staged inside the one active set: dist/Ready-Dust4-HD/ADF/Dust-Disk1.adf
and Dust-Disk2.adf. All 57 release files unchanged. ADF runtime, disk swaps,
020 cadence and hardware remain pending for this pair; no emulator was run.
Small remaining Disk 1 capacity must be reconsidered before further growth.


## READY dust 4 — 7 September 2026

User clarifies intensity means particle density, not brightness; orange was
hard to see. Increased slots from eight to twelve (+50%), preserving v3 cool
brightness and single-row streaks. Warm accents now last twenty frames (0.4s
at 50 Hz) per particle every 192 frames, staggered across slots. Foreground
occlusion remains intentional. Existing 48-byte-entry history capacity is
unchanged (at most 24 distinct dirty bytes for twelve four-pixel streaks).
No additional Chip bitmap or assets. Native build and full host tests pass;
1,600 frames verify restoration and masking with the expanded y tracks.
Sole active candidate is `dist/Ready-Dust4-HD` (52 assets), v3 archived intact.
All 57 release files preserved. Native appearance/timing await user testing.


## READY dust 3 — 7 September 2026

User finds v2 better and requests smoother streaks and slightly stronger overall
intensity, with occasional logo-orange highlights. Removed the lower pixel from
the longest particle: all shapes now occupy a single row. Raised all five cool
brightness targets and added one staggered eight-frame orange pulse per 256
frames, using an existing logo palette colour. Background masking and movement
are unchanged; no additional Chip bitmap or runtime asset.

Native build and full host suite pass, including 1,600 actual C frames, exact
restoration, current-state masking and single-row geometry. Staged 52 assets in
`dist/Ready-Dust3-HD`; preserved all 57 alpha.2 files. V2 archived byte-for-byte
under `dist/older-builds/Ready-Dust2-HD`. Native visual/timing acceptance remains
pending. No emulator, ADF, release, commit or push.


## READY dust 2 — 7 September 2026

User rejects first dust appearance in supplied 2026-09-07 18-07-14.mov: short
lower paths, disappearance/reappearance and little colour movement. Preserved
as testresults/Unassigned-rejected-ready-dust-masking.mov with matching sidecar.
Sampled temporal frames and source reviewed; fixed rectangles and inactive-menu
text union explained invisible barriers. Initial x lifetimes already spanned
328 pixels, so this corrects masking rather than promising new lifetime logic.

Replaced conservative rectangles with edge-connected dark-background masks,
including a one-pixel foreground margin and protection for enclosed art interiors.
Menu masks now select only the current state. Removed artificial x=16/304 edges;
particles traverse offscreen-to-offscreen and hide only behind actual foreground.
Added an eight-step blue/cyan glow cycle, retaining small grit shapes and varied
right-to-left speeds. Mask storage is now 40,192 read-only bytes, 29,952 more
than v1; no extra Chip bitmap. Future ADF capacity must be rechecked before
packaging this larger executable there. No ADF built in this revision.

Actual C host tests cover 1,600 frames with twelve menu masks, exact restoration
and no writes outside allowed pixels; generated mask parity, native compile,
full host suite and 52-asset staging pass. All 57 alpha.2 release files preserved.
Still preview inspected; native motion/music/visual approval remains pending.
Sole active drawer: dist/Ready-Dust2-HD. First version archived intact under
older-builds/Ready-Dust-HD. No emulator, release, commit or push. Music replay
research stays parked; no changes to music or the Stormrail gameplay renderer.


## Current checkpoint: 0.7.0-alpha.2 / Phase 7A.2

User requested an official checkpoint, all package formats, documentation,
lessons learned, commit and push. Public itch remains alpha.68 (live download
names and newest devlog verified); no itch upload is part of this checkpoint.

Current release in `dist`: HD ZIP/LHA, Disk1/Disk2 ADF, WHDLoad ZIP/LHA and the
same-version extracted HD drawer. Protected original alpha.68 remains intact.
The previous local alpha.1 and completed disk test drawer are archived intact.
See [release inventory and tests](RELEASE_0_7_0_ALPHA_2.md).

| Medium / scope | Evidence and remaining gate |
| --- | --- |
| HD campaign | User accepted 030 functionality and retained small optimizations after no noteworthy 020 gain; game executable remains byte-identical |
| Two-ADF campaign | User explicitly approved the corrected ADFs and INSERT DISK 1/2; FS-UAE report, exact CPU not restated in final approval |
| Campaign WHDLoad | User reports successful real-Amiga testing on 2026-09-05; individual intro/F10/replay checks were not enumerated |
| Physical Amiga HD / WHDLoad | User reports both successful on 2026-09-05; HD with about 1.45 MB free Chip RAM plays Level 1 but immediately shows black/top flicker on Stormrail loading; two-colour Workbench with 1.8+ MB free runs HD including Stormrail. WHDLoad runs Stormrail from the lower-free-memory Workbench setup. Exact failing allocation remains unmeasured |
| Physical ADF / Gotek | Hardware test and matched cold-load timing remain open |
| Analogue Pocket | Separate unverified gate |
| Intermittent physical HUD boundary issue | Remains open; no new evidence closes it |

HD game SHA256:
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
This is the released alpha.2 executable hash, not the newer development hash.
The release contains the accepted complete, logger-free campaign. Both sections
retain their gameplay, rendering, audio, replay, continue and return contracts.
Story is included in HD/WHDLoad and deliberately absent from ADF.


## READY background dust candidate — 7 September 2026

User authorized a visual trial of Stormrail-style right-to-left dust, visible
only in the dark background behind all foreground art/text. Implemented eight
small particles using Stormrail's shape/speed family, mapped to existing READY
palette colours. A generated 10,240-byte read-only mask conservatively excludes
complete foreground regions and the union of all menu-state text, with a one-
pixel margin. Existing loading/READY hidden buffers are reused; no extra Chip
bitmap or runtime asset. Per-buffer original-byte histories restore old dust
before optional menu patches and new particles are drawn. Only hidden planes
are edited; Copper publication and the existing owned-frame music tick follow.
Menu changes are coalesced into that same frame, rather than adding a wait.

Actual C host replay of 1,600 frames proves exact restoration, bounded histories
and no writes outside the mask. Actual-C still preview inspected in
build/ready-dust-preview.png. Full host suite, native compile and 52-file staging
passed; all 57 alpha.2 release files are unchanged. Visual smoothness, subtlety,
menu/fade transitions and music cadence remain user FS-UAE/030 gates.

Sole active test: dist/Ready-Dust-HD. The preceding music HD/ADF test set is
preserved byte-for-byte at dist/older-builds/Intro-Title-Music-HD. No new ADF,
release, commit/push or automatic emulator run. Music-player research remains
parked, and the reported working music ADF remains recorded as user acceptance.

## Latest user feedback — 7 September 2026

Music ADF works per user report; exact CPU/detail not restated. Real A1200 and
020/performance are not inferred. Music replay alternatives research is parked
by the user. Stormrail-like dust is now implemented as the new READY candidate described above.

## Current candidate — 7 September 2026

User selected VIII Hero Drive for the intro. It now plays once from the first
intro fade, stops at skip/completion, and releases its bank before title loading.
Neon Sky then plays title through READY; gameplay/results remain SFX-only.
The LSP frame count bounds intro replay so slow I/O cannot restart the cue.
Intro bank is 190,812 Chip bytes, score 5,752 Fast bytes; 3,028 PAL frames
(~60.56 seconds). The title bank is separate, not simultaneously resident.

Sole active test drawer: `dist/Intro-Title-Music-HD`, 52 runtime files. Native
build, complete host suite (including repeated intro/title switches and failure
cleanup), LSP simulated replay and staged byte parity pass. No emulator run or
native audio acceptance. User FS-UAE/030 first, then accepted 020/performance,
then real A1200. No release, commit or push.

At the user's request, dist now contains only all seven alpha.2 release entries,
the new test drawer and older-builds. All 28 other top-level entries (including
alpha.68, Chip2, the previous title test, my-files and even-older-builds) were
moved into older-builds and their file hashes checked. No files deleted.
Archive mapping and build/package evidence: build/intro-music/.

See [music contract](TITLE_AND_INTRO_MUSIC.md).


## Title music ADF test — 7 September 2026

User requests ADF testing without story intro and asks whether title music fits.
The first raw-music Disk 1 exceeded capacity. The disk-only music loader now
reuses the existing CRC-checked SPL1/SPR1 reader for score/bank, decoding directly
into Fast/Chip allocations without an extra full-size copy. Both decoded files
are identical to HD: 150,421 raw bytes become 99,650 stored bytes. No lossy sample
change and no HD compression. Intro music references are excluded from no-story
builds; neither Hero Drive nor story plates are on the ADFs. Music files follow
the established disk resolver, including DF1 and DF0 swap handling.

Same active music test set: dist/Intro-Title-Music-HD/ADF/Music-Disk1.adf and
Music-Disk2.adf. Disk 1 has 92 free blocks / 46 KiB, Disk 2 has 345 / 172.5 KiB.
Both are 901,120-byte DOS1/FFS images; Disk 1 boot checksum verified. Full file
readback, actual C decode/CRC comparison, per-volume dependency checks, 45
reader cases, DOS-stub media tests, full host suite and native compilation pass.
Actual music continuity during floppy I/O remains user FS-UAE/030 testing, then
accepted 020/performance and real hardware. No automatic emulator run.

All 57 latest-release files and the existing HD test payload remain unchanged.
No release, commit or push. Build/readback/capacity evidence and hashes are in
build/multidisk-probe/media.json and music-*.log. Historical probe packager now
includes title-music ownership/order but excludes HD-only intro music.

## Accepted Chip2 basis (before the music candidate)

On 2026-09-06 the user accepted Storm-Chip2-HD.zip on a real A1200 and
requested retaining its optimizations in the main game. The normal development
source, executable and runtime assets already contain Chip1 + Chip2; they are
now the accepted development baseline, not a new release. Stormrail still needs
more than approximately 1.45 MB free Chip RAM; the precise free/largest-block
threshold and native savings remain unmeasured. Individual transition/replay
checks and other media are not inferred from this general HD acceptance.

Accepted HD evidence: `dist/older-builds/Storm-Chip2-HD.zip`, SHA256
`d959f3d1e1984f4a5273d566188c912a4c15391d0f4974af76c76babbbb1ca60`.
Accepted Chip2 executable SHA256:
`a1e09562203e2036d27a3e56a4278a85ae5242041f0350cddfd0b298df5cd526`.
At Chip2 acceptance, executable and all 48 runtime assets matched its ZIP.
Chip1's 208-row worlds and Chip2's omission of four Level-1-only graphics
loads/six cache families are retained. Estimated combined Chip reduction is
about 228 KiB; 187,240 file bytes are avoided per Stormrail load. Timing and
actual native memory savings are not measured. No active test is requested;
the accepted ZIP is retained as evidence at its existing path.

See [Chip audit](STORMRAIL_CHIP_RAM_AUDIT.md) and
[further opportunities](STORAGE_MEMORY_LOADING_AUDIT.md). Further memory work
requires a separate bounded task. HD compression remains explicitly excluded.
No automatic FS-UAE, new package, release, commit or push. Physical ADF/Gotek
and Pocket remain separate gates. Only the selected Hero Drive and Neon Sky tracks are integrated in the new candidate.

## Performance research stays parked

User comparisons narrowed Level-1 differences versus original alpha.68 to
practically none. Retain the accepted Bob/column/history changes without a
perceptible speed claim. Do not repeat rejected audio split, gameUpdate extraction,
completion-cache or linker-order experiments without substantially new evidence.
No new enemy-placement changes or architecture refactor are scheduled.
See [re-audit](LEVEL1_PERFORMANCE_EVIDENCE_REAUDIT.md) and
[production baseline audit](LEVEL1_PRODUCTION_BASELINE_AUDIT.md).

## Document authority

- [Lessons learned](CHECKPOINT_ALPHA2_LESSONS.md): evidence, packaging, ownership,
  presentation and release safeguards, including the missing collision-map error.
- [Phase 7 roadmap](PHASE7_CAMPAIGN_HARDWARE_PLAN.md): released/package boundary
  and pending hardware validation; Phase 6D retains progression design context.
- [Campaign loop](CAMPAIGN_LOOP_CONTRACT.md), [asset ownership](CAMPAIGN_ASSET_OWNERSHIP.md),
  [multidisk](MULTI_ADF_CAMPAIGN_PLAN.md): authoritative behaviour and media rules.
- Stormrail finale/results/debris contracts remain authoritative.
- Dated *_TEST.txt, NEXT_SESSION_* and old handoff entries are historical unless
  this index explicitly reactivates them. Do not resurrect archived candidates.
- Gate 2 typed loader selection and larger architecture work remain deferred;
  physical disk duplication preserves the actual current loader without refactor.

## 2026-09-22 — Continuous background extension, review only

User identified a hard vertical seam before the first governor. Full builder currently repeats the1120px rear to1520px; this joins unrelated edges. Screenshot preserved as `testresults/Drowned-background-seam-before-first-governor.png` with TXT sidecar.

`tools/prepare_drowned_rear_extension.py` creates a1552x208 eight-pen three-plane review asset in `assets/concept/drowned-panorama-v2/`. New imagegen continuation references the old last320px; original first832px remain byte-identical. Offline connected low-error overlap join uses only source pixels, no runtime crossfade. Reviewed native panorama and six finale compositions. Exact planar roundtrip, palette limit, original prefix and all4801 conservative352px fetch windows pass. Extra32px beyond current1520px is2496 planar bytes per bitmap; actual allocated Chip delta needs native allocation evidence. No FPS claim.

Current full test drawer and runtime/build assets unchanged while user tests plants/diamonds. New background is pending visual approval; not integrated. After approval replace the repeat block in `build_drowned_full.py`, add asset dependency, rebuild and stage via test-cycle workflow. Do not silently use rejected studies1/2.

## 2026-09-22 — Continuous rear panorama staged for ingame review

User approved ingame trial. Full builder consumes `assets/concept/drowned-panorama-v2/drowned-rear.spbm` with palette/prefix/fetch bounds assertions; Makefile depends on this asset. Native build passes. Staged `dist/Drowned-Level-020-HD/Drowned-Test` with73assets/55literalrefs. Compared entire previous drawer: ONLY ReadMe and drowned-rear.spbm changed; executable byte-identical, gameplay/music/front art unchanged. Previous drawer and evidence archived intact in `dist/older-builds/Drowned-Level-020-H-old-005237`;68official alpha.8 files unchanged. Test first governor approach through station: no repeated rear seam, natural scroll. No diagnostics/FPS claim; manual020acceptance pending.

## 2026-09-22 — Water shimmer preview only

User chose only water shimmer. `tools/preview_drowned_water_shimmer.py` generates procedural palette-law preview in `assets/concept/drowned-water-shimmer-v1/`:3.2sec loop,160ms updates, PF2pens6/7 only in y188..199 across3four-line bands with staggered phase and maximum10/255brightness delta. Same8pens per band; fullAGA high/low-nibble writes needed in future candidate, no bitmap animation. Exact fixed-palette GIF RGB fidelity, unchanged pixels outside band, foreground invariance at7camera positions pass. Native palette-sequence.json is reproducible contract, not implemented runtime. Scene and enlarged detail GIF available for approval. Shared-pen shore fragments in same band may also vary; no claim of water-only semantic mask. Current dist unchanged; no emulator/FPS claim.

## 2026-09-22 — Water shimmer V2 preview

User rejected V1 as barely visible. V2 in assets/concept/drowned-water-shimmer-v2 uses brighter short glints (delta -8..64), three staggered bands covering y184..199,120ms steps/2.4sec loop. Still two PF2pens per band, no moving bitmaps. OriginalV1 retained; generator --version1/2 reproducible. Exact GIF palette and outside-band/foreground checks pass. Native integration remains pending visual review; dist untouched.

## 2026-09-22 — Water shimmer V3: local horizontal glints

User rejected V2 palette bands aesthetically and requested small horizontal streaks distributed across water. New procedural `tools/preview_drowned_water_glints.py` creates `assets/concept/drowned-water-shimmer-v3/scene-3x.gif` and detail GIF. Rear-world deterministic scattered positions,y185..198;1–8px dashes grow/brighten/shorten over staggered phases,24frames/120ms steps. Native8pens unchanged; palette cycling abandoned for this proposal. Exact GIF RGB, band bounds, loop periodicity and foreground occlusion checks pass. Concept only: localized pixel animation needs an efficient prebuilt patch implementation and020measurement if approved; no claim of Copper-only cost. Current dist unchanged.

## 2026-09-22 — Water V4 combined glints and quiet shore swell

User requested V3 horizontal glints combined with softer occasional shoreline illumination. Generator `preview_drowned_water_glints.py --version4` now produces `assets/concept/drowned-water-shimmer-v4/scene-3x.gif` and detail GIF. V3 retained. Same scattered glints plus three2-line bands y184..189, pens4/6 only, maximum18/255brightness increase (versus V2 max64); staggered phases taper away from shore. Loop2.88sec. Foreground and outside-band parity, periodicity and exact GIF colour fidelity verified. Eight pens per band; fullAGA high/low nibble palette updates needed in future native candidate plus localized bitmap glints. Shared-pen background pixels in shore bands can also brighten; this is not a semantic shoreline mask. Concept only, native performance unmeasured; dist unchanged.

## 2026-09-22 — Approved water animation v1 + separate waterfall preview

User explicitly approves shimmerV4 as named water-animation-v1 for an ingame trial, requests combined waterfall preview first. Frozen `assets/concept/drowned-water-animation-v1/` contains exact approved GIFs, manifest, generator snapshot, approval.json with hashes. Historical shimmerV1/V2 rejected; do not confuse their folder names with newly approved named water-animation-v1. Runtime integration remains outstanding.

`tools/preview_drowned_waterfall.py` generates separate `assets/concept/drowned-waterfall-preview-v1/combined-3x.gif` plus detail GIF. One rear-world patch(1224,140)..(1256,184),4frames120ms, descending streaks and small foam marks. Native8pens unchanged.32x44x3planes =528bytes/frame,2112bytes/four raw frames before staging/background overhead. Exact SPBM/GIF roundtrip, foreground/outside-patch parity and approved base hash preservation pass. Water belowy184 remains pixel-identical to approved V1 in combined GIF. New waterfall is unapproved, no runtime/020performance claim. Current dist untouched.

## 2026-09-22 — Full-level rear ambience candidate staged

User approved combined water/fall appearance and asked to spread animation. New `tools/build_drowned_rear_ambience.py` precomputes approved water-animation-v1 exactly plus4existing waterfall rectangles into `drowned-amb.bin`:205828bytes Fast data. `src/drowned_rear_ambience.h`, compile-isolated to DROWNED_FULL, uses one1848byte Chip DMA stage. Visible352px water window only,24frames/6simulationticks; staged source rows then Blitter copies both canonical rear and guarded display. At most one update family (water or1fall) per publication.4falls at rear x112/432/656/1216; last matches approved32x44patch exactly within aligned48x44rect. Other3adapt same streak/foam law to existingwater.

Shore-wave pens4/6 get three raster bands184/186/188 plus full high/low nibble restore at190; palette tables precomputed and per-inactive-list cache avoids repeatedpatching. Copper capacity896 versus768words adds512Chip across2lists, total extra explicit Chip2360bytes. Binary291316bytes, +2884. No extra full backdrop.

Important live architecture: rolling renderer composes front asynchronously, so rear single-buffer DMA runs immediately AFTER successful line0publication, not in asynchronous Bob draw. Entry afterline64 defers; first animatedfall row112(display156) leaves a conservative beam window. CPU writes only private stage; Blitter handles canonical/display updates and completes before reuse. Hardware beam timing/performance still requires user proof.

Tests: test_drowned_rear_ambience.py verifies actual C with ASAN/UBSAN, all24waterframes versus approved GIF, originalfall4frames, plane/row guards across1830waterrectangles, Copper64-word extension/phase, max6blits, late-entry deferral and allocation lifecycle. Full route +tail collectible tests pass after build completes (an earlier concurrent test saw a transient intermediate generator asset; final rerunpasses). Native build existing unrelated warnings only.

Staged `dist/Drowned-Level-020-HD/Drowned-Test`,74assets/56literalrefs,68official alpha.8 files unchanged. Previous staticrear build/evidence archived byte-identical `dist/older-builds/Drowned-Level-020-H-old-084445`. Difference from previous drawer: executable/ReadMe plus new drowned-amb.bin ONLY. Existing assets/gameplay/music byte-identical. User test020opening,precision,secondferryupper/checkpoint repeat,governor/station; no log in music-enabledvisualcandidate. No emulator launch, release, commit or push. Acceptance pending.
# 2026-09-25 — Level1 background-animation feasibility audit

Visual direction subsequently confirmed by the owner: sparse sky highlights,
authored animation rectangles over the existing lightning at the tower, as
Drowned does with waterfall frames, and a restrained crystal
glow reacting to its discharge. The next step is a
native-size storyboard and exact indexed-pixel bounds; no build or runtime
implementation has been requested.

The first offline storyboard now exists at
`assets/concept/level1-rear-ambience-study-v1/`: current idle rear, sparse
sky highlights, lightning over the existing bolt, crystal afterglow, and full
rear-panorama comparison. It changes no production source or media. Its tower
patch starts at rear row18, so Drowned's later-row post-publication timing does
not establish safety; runtime implementation remains gated on Level1-specific
beam/DMA proof and user visual review.

Source-only audit completed; no runtime, asset, dist, version or release change.
The preferred first visual study is a few unsynchronised, fixed rear-world
cloud-edge highlights across the purple sky. A tower lightning path and crystal
pulse are separate later gates. Broad PF2 palette animation is risky because
the eight pens are shared by sky, mountains and tower. The 1120x208x3 Level1
rear has both an 87,360-byte canonical Chip bitmap and a guarded display copy
of at least 89,856 bytes. Any bitmap animation must synchronize both and prove
the Level1 publication, scrolling and HUD boundaries. Incremental option costs,
unknown native timing and the user-run test gate are in
`LEVEL1_BACKGROUND_ANIMATION_RESEARCH.md`. Historical Level1 FPS and the 57%
WHDLoad loading result are not alpha.10 gameplay headroom. Current release
remains 0.7.0-alpha.10; no FS-UAE run, commit or push.
