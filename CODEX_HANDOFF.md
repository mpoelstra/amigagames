# Codex handoff: Amiga game workspace

## 2026-09-26 — alpha.13 Harrier release ready for Git publication

User accepted the focused 112x64 Harrier destruction and loud 1.24 s
three-hit cue, then explicitly requested ordinary-game release, docs, commit
and push. Current local release is 0.7.0-alpha.13 / Phase 7B.1. Normal HD,
standard/High RAM WHDLoad and all three ADF builds include the 64-tick
one-shot defeat before gate opening/results. HD/WHDLoad SOUNDTEST now has
HARRIER DEFEAT, loaded into Chip only when selected and freed on exit; ADF
menu remains without SOUNDTEST. Full host suite, `make`, `make release` and
`tools/verify_checkpoint_release.py` PASS. Disk free blocks: 16/184/105.
The initial 13-block ADF attempt remains under
`sparkpaw/build/alpha13-pre-soundtest-adf13-attempt`; wider lossless host
packing restored the 16-block floor without changing the decoder. Alpha.12
(181 files) and final focused test (78 files) are byte-identical in
`sparkpaw/dist/older-builds`. Nine alpha.13 artifacts and three drawers are
current in dist. Public itch downloads/devlog still alpha.8, no upload.
See release notes and `RELEASE_VERIFICATION_0.7.0-alpha.13.md` for hashes,
RAM/traffic budget and outstanding exact-media/68020/hardware tests.
The user confirmed that two deleted alpha.5 release-art files should be
included in the commit. No FS-UAE was launched by Codex. Commit/push is
explicitly authorized and remains to be done after final diff audit.


## 2026-09-26 — Harrier SFX still too quiet; denser cue staged

User reports the previous defeat cue below the Sparkpaw shot level even in
SFX ONLY. Old first 50 ms measured ~11.4 raw RMS vs shot 59.2. Revised only
`harrier_defeat()` sample: three impact-window RMS 63.0/72.3/84.0, sustained
final boom, soft saturation peak 122, 1.24 s / 13,672 bytes. Preview v6 WAV
is byte-equivalent to runtime signed PCM. Mixer gain remains 120/128 and
fallback Paula volume 64; one-shot/lifecycle/art unchanged. +1,324 bytes each
Stormrail-only Chip and Fast and raw storage, no new runtime voices or Blitter
cost. New test is in `sparkpaw/dist/Harrier-Death-030-HD/Harrier-Test`;
prior candidate archived intact in
`sparkpaw/dist/older-builds/Harrier-Death-030-H-old-193747`.
Focused tests and 68020-target compile pass; staging preserves all 181
alpha.12 release files. New raw cue SHA-256 is
`2b58180c43a5bed750df445957166f3a283fdb880fe42ff3e223cf5803a7e9`.
User audio acceptance remains pending. No FS-UAE launch, release, commit or
push.


## 2026-09-26 — Harrier screenshot correction staged for user test

User screenshot showed the explosion's straight left fire edge and reported
barely audible SFX. Generator flame range now starts at X=4; native masked
crop is 112x64 with the preview gate outside it. Static art is 66,560 bytes,
Chip stage 5,120 bytes. Sample peak rises to 124, music-mixer gain to 120/128
(about 2.24x combined nominal amplitude); defeat request retires a concurrent
shot voice to avoid overflow. Full host tests and native 68020-target compile
pass. Corrected 68030 test: `sparkpaw/dist/Harrier-Death-030-HD/Harrier-Test`.
The previous 96x64 candidate is intact under
`sparkpaw/dist/older-builds/Harrier-Death-030-H-old-192502` after staging
with `stage_hd_test.py --replace`. The active executable SHA-256 is
`feabf251d362e160af178361e4d9fd1f22978dc474ed477fce6510c24bd46814`;
cue SHA-256 is
`42a22fb1d49fef83907dc4981ca44cc65fe279e1e0b7733278fd160027a19430`.
User audio/visual acceptance and 68020 cadence remain pending. No FS-UAE
launch, release, commit or push.

## 2026-09-26 — Harrier defeat 68030 HD candidate awaiting user play

User approved the v4 fuller native-pixel fire/shard look and v5 higher-pitched
boom-boom-BOOM sample. Integrated a 64-tick defeat state between lethal hit and
gate opening, plus one 96x64 staged masked Bob and 12,348-byte Stormrail-only
sample. Focused campaign direct-start build is staged in
`sparkpaw/dist/Harrier-Death-030-HD/Harrier-Test`; use READY OPTIONS START
SECTION STORMRAIL. The staged ReadMe requests a short 68030 visual/function
pass, then a later 68020 cadence gate. Stager verified 76 assets, 61 binary
references and all 181 alpha.12 release files unchanged. Full host suite and
new frame/sample/lifecycle proof pass after correcting its Level1-only
audio-load allocation count and candidate ownership check.
No FS-UAE launched, release, commit or push. See CURRENT_STATUS and
HARRIER_DEFEAT_030_TEST.txt; keep all old builds/evidence intact.
The first 80x64 draft clipped the fire edge and was archived intact under
`dist/older-builds/Harrier-Death-030-H-old-180928`; the active drawer uses
the corrected 96x64 art clamped left of the closed gate.

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

## 2026-09-23 — CONTROL options candidate (unreleased)

The new OPTIONS row is CONTROL with JOYSTICK and JOYPAD. JOYSTICK uses Up
for jump and ignores port-2 button 2; JOYPAD uses button 2 for jump and ignores
Up. Primary Fire shoots in both modes; button 2 can no longer shoot. Keyboard
W/Space remain enabled in both modes with independent press edges, so a held
controller source does not suppress a fresh keyboard press. Stormrail flight
uses the selected controller jump/up source for vertical movement. The alpha.9
port-2 pin-9 pull-up and keyboard ACK corrections remain intact. MrDig reports
that the focused candidate "lijkt goed"; no exact route, controller, machine
or emulator configuration was supplied. This is a preliminary positive verdict,
not real-A1200 or affected-user verification. No new release build was made.
The integrated HD, ADF and packed-WHDLoad native builds and full host suite pass;
ADF/WHDLoad were compiled only, not packaged or played. The focused HD drawer is
`sparkpaw/dist/Controls-Mode-HD/Sparkpaw-Test`, staged with 74 referenced
assets; all 163 alpha.9 release files remained byte-identical. Its sole
runtime-asset difference from alpha.9 is `readymenu.spbm`. The approved
alpha.9 READY base and two main-menu patches are pinned byte-identically.
The earlier unplayed staging attempt is preserved in `dist/older-builds`.

## 2026-09-23 — alpha.9 release checkpoint

Final user smoke results: corrected WHDLoad and ordinary HD work in FS-UAE;
the rebuilt ADF recognizes Disk 3 in DF2/DF3. Exact emulator configuration
was not supplied. `make release`, full `make test`, independent ZIP/LHA
extraction and three-ADF file readback pass. FS-UAE was confirmed stopped.
Its modified WHDLoad test drawer and sidecar were archived with hashes, and
the clean drawer restored. Alpha.8 (68 files) and the three approved campaign
drawers (159 files) moved hash-identically to `dist/older-builds`. Root dist
holds only alpha.9 release artifacts and extracted review drawers. The
remaining task is commit and push; itch upload is outside scope.

Follow-up ADF drive support: MrDig put Disk 3 in FS-UAE DF2; the first
alpha.9 ADF did not recognize it until DF0/DF1, matching the source's
two-drive scan. At his request `disk_media.c` now scans DF0–DF3 and uses
the detected drive for subsequent asset reads. The host media test covers
Disk 3 marker and asset path selection in both DF2 and DF3. The old Disk 1
ADF candidate was archived hash-identically before rebuild; a focused user
test of the new ADF Disk 1 with Disk 3 in DF2/DF3 was supplied: MrDig agreed
to test both and reported "ja werkt" in FS-UAE. Configuration unspecified;
no real-A1200 or physical-floppy claim.

WHDLoad correction during alpha.9 review: MrDig played the first alpha.9
WHDLoad and reported title/loading flicker plus slow loading absent from the
approved `Campaign-WHD-Soundtest-SFX`. The release packager accidentally used
the generic slave's 5.5 MB ExpMem instead of the accepted packed slave's
3.5 MB. Rebuilt with `PACKED_WHDLOAD`; the new slave has `$380000` ExpMem and
differs from the accepted slave only in version text. All 74 packed assets
match; the corrected game executable is byte-identical to the rejected first
alpha.9 executable, so all controls changes remain. The rejected drawer and
ZIP/LHA are preserved with hash inventory in `dist/older-builds/`; the
release verifier and shipping skill now assert the packed memory value.
Archive extraction and ADF readback pass again. MrDig reported the corrected
WHDLoad works in FS-UAE; the memory mismatch explains the observed emulator
regression, without a real-hardware claim.

User authorized completion, packaging, documentation, commit and push for all
accepted development since official alpha.8. The three approved campaign
baselines are `Campaign-HD-Soundtest-SFX`, `Campaign-WHD-Soundtest-SFX` and
`Campaign-ADF-Disk3-Type`. MrDig also played `Controls-Pullup-HD/Sparkpaw-Test`:
it works well and feels unchanged on an unspecified configuration. This is a
successful regression playtest, not proof that the original user failures are
fixed; affected users have not verified them. The OPTIONS trigger and possible
68060 connection remain unproven. This checkpoint is alpha.9 with
the approved Drowned v5 music unchanged and rejects all alternative music
tests from runtime. Alpha.8 and the approved drawers were kept intact through
packaging and focused playtests, then archived hash-identically.
The controls-only drawer was moved byte-identically (76 SHA-256 matches) to
`sparkpaw/dist/older-builds/Controls-Pullup-HD-approved-regression` after the
new HD executable matched it exactly. No FS-UAE launch by Codex and no
real-hardware claim.
Current alpha.9 `make`, `make release` and full `make test` pass after repairing
two stale host-test extractors. Independent ZIP/LHA extraction, 74-asset parity,
icons, and all-file three-ADF readback pass (42/180/76 free blocks). The user
subsequently reported the targeted alpha.9 media work. The prior alpha.8 and
approved campaign files were archived byte-identically. The public itch download
baseline was freshly detected as alpha.8; itch upload is outside this task.
The 74 packed WHDLoad assets are byte-identical to the accepted candidate.
ADF file-level comparison against the accepted three-disk candidate shows
only the Disk 1 executable changed; Disks 2 and 3 retain identical contents.

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


Reusable optimization lessons: [Drowned/Level1 68020 findings](sparkpaw/docs/DROWNED_TURBINES_LESSONS.md#retained-68020-optimization-lessons--september-2026). Read before reopening FPS work; latest media decisions below remain authoritative.

## 2026-09-23 — session close: HD, WHDLoad and ADF campaign candidates user-approved

Latest user verdict: the newest `sparkpaw/dist/Campaign-HD-Soundtest-SFX` and `sparkpaw/dist/Campaign-WHD-Soundtest-SFX` are also approved ("ok bevonden"). This supersedes the pending-play wording immediately below. It is user acceptance of these exact HD/WHDLoad candidates, including the Drowned SFX Soundtest update, without a new detailed route-by-route or real-hardware report. Alongside the already played `Campaign-ADF-Disk3-Type`, these are the active campaign baselines for the next bug investigation. The earlier HD/WHDLoad drawers remain intact in `dist/older-builds/` as comparison baselines. Alpha.8 remains the official release; no commit, push, version bump or release.

Dist cleanup after session close: the user confirmed FS-UAE was stopped. The played HD baseline `Campaign-Drowned-020-HD`, played WHDLoad baseline `Campaign-WHD-Cache-020`, and its `.uaem` launcher were moved intact to `sparkpaw/dist/older-builds/`. All 157 moved files matched their pre-move SHA-256; all 68 official alpha.8 files remained byte-identical. Manifest: `sparkpaw/build/campaign-drowned/dist-cleanup-20260923-sfx.json`. Active `dist` root contains official alpha.8, three-disk `Campaign-ADF-Disk3-Type`, `Campaign-HD-Soundtest-SFX` and `Campaign-WHD-Soundtest-SFX`; all three are now user-approved. The stage/package scripts that use the earlier HD asset source read its archived path. References below to the old root paths are historical.

Earlier ADF report: `sparkpaw/dist/Campaign-ADF-Disk3-Type` passes the complete three-section campaign, START AT and disk swaps in FS-UAE. The user also approved its full-line INSERT DISK 3 art. This is ADF FS-UAE acceptance for the played routes; real A1200/physical floppy remains untested. The agreed three-disk ADF presentation has no Soundtest. Drowned's previously missing PUMP SHOT and CHECKPOINT are selectable after HEALTH PICKUP in HD/WHDLoad Soundtest. The host menu/cache checks and native 68020 builds pass; WHDLoad packaging verified all 74 stored assets by native reader. The newest HD candidate's assets match the earlier approved HD drawer byte-for-byte. The user subsequently approved both new HD/WHDLoad executables as stated above; the earlier approved drawers remain intact. Alpha.8 remains official; no commit, push, version bump or release.

Session close: user requested documentation and a fresh-session prompt for an unspecified user bug. No bug evidence or reproduction has been supplied yet. Next session should first identify observed behavior, exact build/media, steps, expected result and any screenshot/MOV/log, then investigate against the three currently approved candidates in root `dist`. The earlier HD/WHDLoad builds in `older-builds/` are preserved comparison baselines. Preserve all local changes and drawers. Do not launch FS-UAE on Codex's own initiative. Historical pending statements below are superseded by this paragraph where their status conflicts.

Latest user decision: the new full-line `INSERT DISK 3` typography in `sparkpaw/build/multidisk-probe/status/preview-2x.png` is visually approved ("het is nu mooi"). This approves the art preview, not yet the new `sparkpaw/dist/Campaign-ADF-Disk3-Type` runtime or full ADF campaign. Previous `Campaign-ADF-Disk3-Flow` was user-played and proved Disk 3 request/Direct Drowned loading, but had rejected art; the new ADF changed only `disk3-patch.spr1` per disk and passed technical readback. FS-UAE was checked stopped; rejected `Campaign-ADF-Disk3-Glyph` and all four files were archived hash-identically to `dist/older-builds/Campaign-ADF-Disk3-Glyph-rejected`; manifest `sparkpaw/build/campaign-drowned/adf/dist-cleanup-20260923-type-approved.json`. `dist` retains only the active three-disk ADF candidate, accepted HD, current WHDLoad and official alpha.8. Real A1200 remains untested.

Latest typography correction: user rejected `Campaign-ADF-Disk3-Glyph` too; screenshot `sparkpaw/testresults/Unassigned-rejected-ADF-code-glyph-3.png` plus TXT. Existing INSERT DISK 1/2 are **not a runtime font**: both are complete lines from imagegen raster sheet `sparkpaw-insert-disk-type-v1.png`, cropped, resized 216x24, quantized to the loading palette and inset in a 224x40 patch. After the user's correction, built-in imagegen used that sheet as edit/style reference to create a complete `INSERT DISK 3` line, saved as `sparkpaw/assets/concept/sparkpaw-insert-disk-type-v2-disk3.png`. `tools/generate_disk_status.py` now converts all three complete lines through the same pipeline; the packager copies all three results. New active `sparkpaw/dist/Campaign-ADF-Disk3-Type` has full three-disk readback (42/180/76 free blocks), matching palette/geometry guard test, and only `disk3-patch.spr1` differs per ADF from the previous candidate. Preview: `sparkpaw/build/multidisk-probe/status/preview-2x.png`. The rejected `Campaign-ADF-Disk3-Glyph` drawer was subsequently archived intact after FS-UAE stopped. User aesthetic and FS-UAE acceptance of the new full-line art pending; alpha.8 remains official.

Art correction follow-up: user rejected the `Campaign-ADF-Disk3-Art` preview because its numeral resembled two overlapping 2s. That candidate was never accepted. Reworked the native 3 to have a black/open left waist, a right lower stroke and one middle bar; host guard now checks those semantic glyph features as well as palette, approved 1/2 hashes and exact difference bounds. Updated `sparkpaw/docs/ADF_INSERT_DISK_ART_CONTRACT.md` and regenerated comparison preview. New active candidate is `sparkpaw/dist/Campaign-ADF-Disk3-Glyph` (42/180/76 free blocks; full ADF readback). FS-UAE was checked stopped; `Campaign-ADF-Disk3-Art` was archived hash-identically at `dist/older-builds/Campaign-ADF-Disk3-Art-rejected-preview`, manifest `sparkpaw/build/campaign-drowned/adf/dist-cleanup-20260923-disk3-glyph.json`. User visual/play acceptance pending.

Latest ADF result: user confirms `Campaign-ADF-Disk3-Flow` now visibly requests Disk 3 and loads Drowned after insertion. Its INSERT DISK 3 numeral is rejected as too large and inconsistent; user screenshot is preserved at `sparkpaw/testresults/Unassigned-rejected-ADF-disk3-typography.png` with TXT. Cause: packager pasted a separate 190x250 digit scaled to 27x32 over the approved Disk 2 strip. Replaced with native indexed `disk3_from_disk2()` built from the approved Disk 2 glyph rows; only 101 pixels in its 16x7 numeral cell change. Full visual/packaging contract and pinned Disk 1/2 hashes: `sparkpaw/docs/ADF_INSERT_DISK_ART_CONTRACT.md`; guard test `sparkpaw/tests/test_disk3_status_consistency.py` passes. New `sparkpaw/dist/Campaign-ADF-Disk3-Art` passed full three-disk readback (42/180/76 free blocks), host media test and exact inventory comparison: the sole changed file on each ADF is `assets/runtime/disk3-patch.spr1`. Visual preview: `sparkpaw/build/campaign-drowned/adf/status/insert-disk-1-2-3-preview.png`. FS-UAE user visual acceptance of the new art remains pending. FS-UAE was confirmed stopped; previous four-file candidate was archived hash-identically at `sparkpaw/dist/older-builds/Campaign-ADF-Disk3-Flow-typography-rejected`, manifest `sparkpaw/build/campaign-drowned/adf/dist-cleanup-20260923-disk3-art.json`. Alpha.8 remains official and untouched.

Latest ADF user test: `Campaign-ADF-Menu-Disk3` now shows Drowned Turbines in START AT, but selecting it and START GAME produces a black screen before any visible Disk 3 request. This rejects that candidate; the earlier Stormrail Continue black report is consistent but not independently retested here. Source review found the shared `APP_DROWNED_ENTRY` ADF path called `titleShowReplayLoading()` and `diskMediaRequire(3)` while direct READY still owned interrupts/Blitter and DOS was unavailable. Stormrail Continue had already called `platformReleaseForLoading(TRUE)`. Added that idempotent release at the shared Drowned entry before any media I/O. New `sparkpaw/dist/Campaign-ADF-Disk3-Flow` contains the fix; native 68020 build, complete three-disk readback (42/180/76 free blocks), ADF resolver and Drowned lifecycle host tests pass. FS-UAE gameplay/requester result remains pending. FS-UAE was checked stopped, so superseded `Campaign-ADF-Menu-Disk3` and its four files were archived hash-identically under `sparkpaw/dist/older-builds/Campaign-ADF-Menu-Disk3-black-screen`; manifest `sparkpaw/build/campaign-drowned/adf/dist-cleanup-20260923-direct-drowned-black.json`. Alpha.8 unchanged.

Dist cleanup follow-up: user confirmed FS-UAE stopped; app inventory showed FS-UAE not running (launcher still open). The rejected `Campaign-Drowned-ADF` drawer was moved intact to `sparkpaw/dist/older-builds/Campaign-Drowned-ADF-rejected-menu`. SHA-256 of all four files matched before/after; manifest: `sparkpaw/build/campaign-drowned/adf/dist-cleanup-20260923-menu-rejection.json`. Active ADF drawer remains `Campaign-ADF-Menu-Disk3`.

Latest user FS-UAE update: `sparkpaw/dist/Campaign-WHD-Cache-020` also exits with F10 in Drowned Turbines and its tested campaign transitions work. Together with the earlier report of stable TITLE/LOADING and fast loads, this accepts those observed WHDLoad behaviors in FS-UAE; real A1200 remains untested. The first three-disk ADF candidate is rejected: `sparkpaw/testresults/Unassigned-rejected-ADF-start-options-crossfield.mov` (paired TXT; original `2026-09-23 16-36-58.mov`) shows that Options cannot display Drowned as START AT and changing it alters SECOND BUTTON. Cause: ADF READY cache and index encoded two sections while input cycles through three. The user separately reports a black screen after Stormrail Continue without an apparent Disk 3 request; the video does not show this transition, so its cause remains open. The source requests disk 3 on that branch and the host DF0/DF1 resolver test passes. New unnumbered `sparkpaw/dist/Campaign-ADF-Menu-Disk3` has a three-section ADF menu cache and corrected index, with no intro or Soundtest. Native build and exact per-file readback passed; disks leave 42/180/76 free blocks. User ADF replay is pending. The rejected ADF drawer was subsequently archived intact after FS-UAE stopped. Alpha.8 remains official, unchanged; no commit/push/release.

Latest user FS-UAE report for `sparkpaw/dist/Campaign-WHD-Cache-020`: the current WHDLoad candidate runs well, TITLE/LOADING no longer flicker, and loading is fast again. This is user acceptance of the observed presentation/loading behavior on that configuration, not yet a report on F10, every campaign transition, ADF, or real hardware. The candidate uses lossless per-file compression: 49 SPL1, 17 SPR1, 6 SPD1, 2 raw; assets 5,040,578 -> 1,778,401 bytes. Shrinkler reduces the native executable 608,852 -> 131,072 bytes; installed drawer totals 1,930,568 bytes. PRELOAD caches the compressed files where memory allows; it is distinct from the compression itself. Alpha.8 remains official.

Dist cleanup completed after checking that FS-UAE itself was stopped (the FS-UAE Launcher remained open). Seven superseded WHDLoad drawers (`Campaign-Drowned-WHDLoad`, `Campaign-Drowned-WHD-Fix`, `Campaign-Drowned-WHD-Preload`, `Campaign-Drowned-WHD-Diag`, `Diag2`, `Diag3`, `Diag4`) and `Campaign-Drowned-WHDLoad.zip` were moved intact under `sparkpaw/dist/older-builds/`. Fresh per-file SHA-256 before/after manifest: `sparkpaw/build/campaign-drowned/whdload/dist-cleanup-20260923-post-diag4.json`; all hashes matched, including the Diag2/3/4 logs. The active root retains official alpha.8, accepted `Campaign-Drowned-020-HD`, `Campaign-Drowned-ADF`, and current `Campaign-WHD-Cache-020`. All 68 alpha.8 files remained byte-identical. Historical references below to `dist/Campaign-Drowned-WHD-*` now resolve under `dist/older-builds/`.

User FS-UAE screenshot `sparkpaw/testresults/Campaign-Drowned-WHDLoad-ExpMem-allocation-failure.png` shows the original unnumbered WHDLoad candidate failing before game startup with WHDLoad 20.0 "Can't allocate ExpMem." The slave requested 0x800000 ExpMem on an 8 MB Fast target and its icon used PRELOAD. Corrected unnumbered candidate is `sparkpaw/dist/Campaign-Drowned-WHD-Fix`: slave requests 0x580000 (5 MB game Fast + 512 KB Kickstart), icon omits PRELOAD, exact campaign executable and 74 assets retained. Header, icon, asset inventory and native assembly checked; user reports it reaches gameplay but flickers in TITLE/LOADING. Previous candidate remains intact; do not mistake it for corrected one.

User reports the no-PRELOAD fix reaches gameplay, but TITLE/LOADING flicker continuously during startup and after Escape; CHARGING/READY/gameplay are stable. Historical alpha.35 compressed HD loading was rejected at LOADING after 30.8 seconds, alpha.36 raw rollback worked: see `sparkpaw/docs/STORAGE_MEMORY_LOADING_AUDIT.md` and its preserved video. Separate rejected experiment `sparkpaw/dist/Campaign-Drowned-WHD-Preload` packs 74 assets losslessly (5,040,578 -> 1,778,401 bytes), Shrinkler executable (608,760 -> 131,024), requests 0x480000 ExpMem and restores PRELOAD. Native reader/packaging checks pass, but the user still saw flicker (below). Detail: `sparkpaw/docs/WHDLOAD_PRELOAD_PACKED_EXPERIMENT.md`. HD/ADF and alpha.8 artifacts unchanged.

Follow-up user playtest rejects this packed PRELOAD candidate too: TITLE/LOADING still flicker on cold boot and on Stormrail direct start; Escape sometimes restores stable screens, sometimes flickers. Direct Drowned returned to Workbench from LOADING in both raw/no-PRELOAD and packed/PRELOAD WHDLoad, with no visible error requester; accepted HD direct Drowned works and its TITLE/LOADING never flicker. Drowned has no separate CHARGING stage. Raw Diag2/3 subsequently isolated the renderer stride issue (below); no emulator was started by Codex.

User ran Diag2. Its preserved logs in that drawer show `entry` -> `platform_opened` -> `loading_image_ready` -> `gameplay_assets_ready` -> `collision_ready` -> `audio_ready` -> `renderer_prepare_failed`. `startupdiag.log` pinpoints `failed_aga32_layout_validation` after target/Copper/HUD/player sprites/rear guard prepared. At failure: Chip free/largest 884,712; Fast free 3,217,592/largest 3,217,064. Therefore generic memory exhaustion and packed-file decoding are not the direct cause of this Drowned return. Diag3 subsequently identified the invalid rear stride (below). Diag2/3 and their logs are preserved intact in `dist/older-builds/`.

User ran Diag3. Exact log: front stride 128, HUD stride 44, all logged plane pointers and blank plane 4-byte aligned, **Drowned rear stride 198**, so `aga32DisplayLayoutValid()` rejects it. Source rear SPBM row is 194 bytes; `prepareRearGuardedDisplay` added four guard bytes and asked graphics.library for 198-byte rows. The accepted HD build did not fail this validation; its actual allocated rear stride was not measured. The WHDLoad Kick31 run gave 198. Narrow source fix rounds rear guarded allocation request/requirement to 200 (and uses same rounding for Stormrail flight rear); the user then played raw diagnostic `sparkpaw/dist/Campaign-Drowned-WHD-Diag4` (below). Alpha.8 comparison: slave same 5932 bytes and only differs in FastMem header/code constants (alpha.8 asks 0x800000 ExpMem and has PRELOAD); alpha.8 has 59 runtime assets/3,558,686 bytes versus campaign 74/5,040,578 raw bytes. Alpha.8 lacks the Drowned 194-byte rear, and its smaller file set may change cache pressure. Flicker/long loading remains separately unresolved; packed PRELOAD did not remove it in user playtesting.

User played Diag4: Drowned reaches gameplay. Its preserved `startupdiag.log` shows rear stride 200 and `renderer_prepare_complete`; `whd-drowned-diag.log` reaches `renderer_ready` with 523,720 Chip and 3,633,536 Fast bytes free. User reports about five minutes to load, continued TITLE/LOADING flicker and F10 ineffective in Drowned. The 12.05-second recording `sparkpaw/testresults/Unassigned-rejected-Drowned-WHD-Diag4-loading-black-flashes.mov` (paired TXT) shows the LOADING art only in brief flashes against black; it does not measure the full load or F10. This rejects Diag4 as a complete WHDLoad candidate while confirming the stride correction. Source review found Drowned's namespaced loop omitted the shared WHDLoad quit-flag check. The new `DROWNED_CAMPAIGN_QUIT` path handles F10 and parent `platformRestore()` reinstates Workbench DMA/View after the module returns DOS-live. Host Drowned F10/lifecycle and Hunk namespace tests pass; native 68020 build passes. New `sparkpaw/dist/Campaign-WHD-Cache-020` packs the same 74 assets losslessly, uses Shrinkler and PRELOAD, removes diagnostic logging, and reduces ExpMem to 0x380000 to leave more host RAM for cache on 8 MB Fast. All codec readbacks and slave/header checks pass; **user FS-UAE result pending**. Full cache coverage, loading time, title stability, direct Drowned/F10 and other transitions are not proven. Official alpha.8's 68 files are byte-identical by before/after hashes. Superseded drawers and logs were archived intact as noted above.

MrDig explicitly approved the played `sparkpaw/dist/Campaign-Drowned-020-HD/Sparkpaw-Test` on FS-UAE: Level 1 -> Stormrail -> Drowned, Continue, carried lives/health/diamonds/score, Drowned results and Replay, direct Options entry, Undertow Circuit in Soundtest, and image/music. This supersedes the pending HD acceptance wording below. It does not approve WHDLoad, ADF or real hardware. Alpha.8 remains official; no version/release/commit/push.

WHDLoad adaptation has an unnumbered candidate in `sparkpaw/dist/Campaign-Drowned-WHDLoad`: 74 referenced assets, existing BootDOS slave, and a separately linked campaign executable with `SPARKPAW_WHDLOAD` F10 support. The separate unnumbered ADF candidate is `sparkpaw/dist/Campaign-Drowned-ADF`, three FFS DOS1 disks: boot/Level 1, Stormrail, Drowned. It uses verified lossless SPR1/SPL1/SPD1 assets and Shrinkler executable compression. Per-disk readback passed with 43/180/76 free blocks. As the user chose, ADF keeps its established no-intro, no-Soundtest presentation. Host media and native code checks passed; manual WHDLoad/ADF play and real A1200 tests remain pending. Preserve all dirty source/assets and the official alpha.8 bytes.

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


## 2026-09-22 — Busy discovery run1: incomplete trace, awaiting stop state

User reports played/log present. Executable SHA256 and source-generation ID
match staged busy_d3d8e3e3d83e1d406994_sparse. Initial read is61544bytes,
ends mid busy_stamp sample53; header advertises82samples and no post_run footer
is present. Same size on recheck. Raw preserved byte-exactly at
sparkpaw/testresults/Drowned-busy-020-run1-partial.log with provenance TXT.
Do not overwrite this initial snapshot if the source later finishes flushing.
Asked whether FS-UAE remains on the frozen save screen or was stopped/reset;
answer pending. Do not request a replay before resolving save completion.

Complete records0..52 only were analyzed separately, with explicit partial
status in build/drowned-busy/run1-partial-analysis.json. No footer was fabricated
and the strict complete-log analyzer correctly rejects the original file.
Header cadence aggregates cover3164intervals/3802fields; lateferry571/829,
but the detailed post-respawn trace is missing. No visual/audio verdict given.

Pre-respawn lateferry has8complete records (1..3drawn Spillwings,0..5shots).
Conditional scanline estimates: game median106.5lines/8valid, AI30/8,
ring+dynamic sync50.5/8, allrestores51.5/8, otherdraw38/8,
enemydraw22/6, Bobtotal204/6. These are nested/differently-valid scopes,
not additive medians or pureCPU/DMA timings. Same-frame emptyprobe is1line.
Rear updates usually2lines but maxima77..80lines across regions. Provisional
lead is combined scene workload/dynamic sync plus intermittent rear cost,
not enemy drawing alone. Trace endpoints near PALwrap are rejected where
TOD/raster epochs are ambiguous (e.g.sample44); do not silently repair them.
No code optimization selected, new build staged, emulator launched or release
changed during this evidence review. Preserve raw timestamps and await status.


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


## 2026-09-19 — Regional v2 full run: first ferry half is hotspot

User reached end; unexpected falling1-up. Evidence preserved as
Unassigned-Drowned-regions-v2-full-run.log/TXT; exact current executable verified,
complete footer, region_attribution=2; regional sums agree with global.
6334intervals:83two-field,0three-plus,49.35FPS,0ownership violations.
land49.47/14misses; precision49.38/6; approach49.80/2;
ferry_first47.82/34of748(4.55%); ferry_last49.43/27of2348(1.15%);
shore50.00/0of920. Firsthalf now measured investigation priority; bothferry
sections61/83misses. No family CPU/blitter attribution in minimal log.
Checkpoint1/1sound; extra_life1/1. Inherited Level1 extraLifeShouldReveal still
unguarded in game.c: needs Drowned gameplay/render exclusion; Core alreadyhidden.
No changes staged this analysis (user asked log first). Differentworkload and
18.4sec50FPSshore mean49.35vs48.82 does not prove optimization.


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


## 2026-09-19 — Checkpoint run1 log preserved; regional diagnostic bug

Preserved testresults/Unassigned-Drowned-checkpoint-regional-run1.log/TXT from
old-182507 drawer (first checkpoint, before no-Core fix), verified executable
against proof-checkpoint-first.json and complete post_run footer.4557intervals,
110two-field,0three-plus,48.82FPS,0ownership violations; checkpoint1request/1start.
User reports recurring drops. Chip456576free/largest455224, Fast5186632free.
REGIONAL SPLIT INVALID: rendererDiagnosticUpdateEntry memsets diagnosticCurrent
before drownedCadenceRecord reads its playerX, assigning all samples toland.
Fix next: retain previousplayerX independently and test full entry lifecycle.
Current no-Core stagedbuild has same bug; user doing another run, do not swap
build while playing. Whole-run cadence valid; cannot localize ferry fromthislog.
No runtime change made in this analysis.0.43FPS below prior49.25 with different
workload is not controlled regression evidence. See sidecar for exactprovenance.


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

## Standing communication preference — 8 September 2026

The user is a frontend software engineer with an interest in Amiga internals,
but does not write C or assembly. Give moderately more technical detail in
progress updates and findings by default: explain the approach, why it was
chosen, how the relevant hardware/code works, and how measurements support the
conclusion. Define unfamiliar C/assembly/hardware terms briefly in context.
For logs, include a few meaningful numbers and their practical interpretation;
distinguish source facts, host checks, supplied native evidence and hypotheses.
Do not reduce results to only “passes” or “looks good”, and do not overwhelm
with long essays, raw counter dumps or unexplained jargon. A few connected
paragraphs or a compact comparison table are usually sufficient. Use Dutch
for the conversation; the English release-notes agreement below still applies.
This is an explicit user preference, not a requirement for extra tests or work.

## Standing release-notes agreement

All player-facing "What's new since the latest itch version" content must be
written in English: devlog title, headings, bullets and compatibility note,
even when the conversation is Dutch. Save the complete text in the versioned
`sparkpaw/docs/RELEASE_NOTES_*.md` file and use that English text in the handoff.
Cover the full visible delta since the verified public version, not merely the
last local commit or a five-bullet packaging summary. Keep technical lessons
and unsupported performance/platform claims out of player-facing feature lists.

## Start here

Sparkpaw is the active project. At the start of a task:

1. read this file and `sparkpaw/README.md` completely;
2. inspect current source, `git status`, recent commits and tags;
3. use source/generated manifests as authority;
4. consult the relevant part of `docs/DEVELOPMENT_HISTORY.md` before reopening
   a known renderer, hardware, asset or gameplay problem.

This file is the compact current contract, not a diary. Put chronology,
rejected experiments and detailed evidence in `docs/DEVELOPMENT_HISTORY.md`.

Repository:

```text
amigagame/
  CODEX_HANDOFF.md
  docs/DEVELOPMENT_HISTORY.md
  sparkpaw/                 active AGA game
  chipsnake/                finished prototype
  mrdigs-futsal/            finished prototype
  backups/                  ignored; never delete
  ACM_PDF/                  ignored Amiga manuals
```

Branch `main` is the shared state. Preserve recordings, logs, toolchains and
backups.

## Build, release and verification

Target: PAL A1200/AGA, 68020+, 2 MB Chip plus 8 MB Fast RAM.

From `sparkpaw/` always run after implementation:

```sh
make PYTHON=../.venv/bin/python3
make release PYTHON=../.venv/bin/python3
```

Current release is `0.7.0-alpha.5`, Phase 7A.3: HD ZIP/LHA, Disk1/Disk2
ADF, WHDLoad ZIP/LHA and the extracted same-version HD drawer. Full inventory
and checksums: `sparkpaw/docs/RELEASE_0_7_0_ALPHA_5.md`.

### Historical alpha.68 baseline (preserved)

Alpha.68 is the Phase 6C.10 instant-Level-1-replay checkpoint. The results
prompt reads `REPLAY LEVEL`; confirmation still fades the six-plane score
display completely to black, but no longer unloads and reloads gameplay files,
audio, conversion caches or renderer allocations. Both rolling playfield
targets are restored from the clean canonical world, their dynamic histories
are cleared and the new gameplay Copper is published just after PAL frame wrap.

The first resident candidate was rejected by supplied FS-UAE/68030 HD evidence:
staggered collectible updates temporarily drew several diamonds at y=0. Fresh
collectibles now initialize their presentation coordinates from their authored
spawn positions. Supplied FS-UAE/68030 HD retesting accepts the corrected replay
as working well. Bounded native FS-UAE/68030 and FS-UAE/68020 proofs both reach
fresh gameplay frame 11 at camera x=0 with valid collectible coordinates,
1,049,392 Chip bytes free and a 985,984-byte largest Chip block. ADF, WHDLoad
and real-hardware runtime acceptance were initially pending at packaging time.
MrDig subsequently reports successful alpha.68 testing on his real
A1200/68030 through all three shipped launch paths: physical floppy/ADF,
WHDLoad and the ordinary HD drawer. No alpha.68 fault was reported on those
paths. This closes the supplied real-machine gate for that configuration only;
stock real-68020 and other accelerators remain unverified.
The bootable DOS1/FFS ADF uses 1,641 blocks (820 KiB) and leaves 119 free.
Final artifact sizes are 658,307-byte HD LHA, 657,742-byte HD ZIP,
901,120-byte ADF, 650,860-byte WHDLoad LHA and 651,576-byte WHDLoad ZIP.

Alpha.67 is the Phase 6C.9 secret-extra-life checkpoint. Reaching the chamber
beyond the Level-1 Core reveals one native masked `1UP` Bob at world x=3328.
It falls from playfield y=0 to the floor at y=178 and remains collectible there.
The level geometry makes crouching under the Core the route into the chamber,
but the reveal itself is based only on reaching the far-right x threshold.
Pickup awards one attempt up to x9, plays the unique `extra-life.raw` four-note
Paula cue and cannot be repeated after a hazard/life restart in the same level
attempt. A complete results replay creates a fresh attempt and restores it.

Supplied FS-UAE/68030 HD review accepts the final presentation and behaviour as
“wel ok voor nu”. The native framebuffer self-test also verifies the generated
mask, palette and corrected planar-row addressing without the story intro or
`dist/`; its PNG matches the accepted proof byte-for-byte. This is not
FS-UAE/68020, ADF, WHDLoad or real-hardware runtime acceptance. The reusable
self-test harness stages an ordinary temporary host directory as FS-UAE DH0
and compiles all shortcuts behind `SPARKPAW_EXTRA_LIFE_VISUAL_PROOF`, so normal
release binaries retain the complete intro and interactive flow.
The bootable DOS1/FFS ADF uses 1,639 blocks (819 KiB) and leaves 121 free.
Final artifact sizes are 657,959-byte HD LHA, 657,416-byte HD ZIP,
901,120-byte ADF, 650,888-byte WHDLoad LHA and 651,589-byte WHDLoad ZIP.

Alpha.66 is the Phase 6C.8 Paula one-shot audio-integrity checkpoint. All
current effects reload a two-byte Chip-RAM silence word after their first pass,
DMA restarts use a deterministic two-raster-line latch wait and voice ownership
is derived from each sample's byte duration with one update guard field. This
removes the measured repeated head after the 45-ms tally tick and lets Storm
Triumph resolve for its complete 1.15 seconds. Samples, triggers, volumes,
priorities and channel ownership are unchanged: player plasma owns Paula 0,
prioritized effects own Paula 1 and channels 2-3 remain reserved.

Supplied FS-UAE/68030 HD A/B evidence measures the baseline tally burst at about
61 ms and the corrected candidate at about 41 ms above threshold; the user says
B sounds better without claiming perfect timbre. A focused real-A1200/68030 HD
phone recording accepts Core, complete tally, prompt, replay loading and return
to gameplay without a new click, persistent whine or missing tally sequence.
This accepts the focused one-shot lifecycle on those two HD paths only.
FS-UAE/68020, ADF, WHDLoad and broader real-hardware alpha.66 acceptance remain
pending. The only memory addition is one two-byte Chip allocation; renderer,
gameplay, score arithmetic, controls and assets remain alpha.65.
The bootable DOS1/FFS ADF uses 1,621 blocks (810 KiB) and leaves 139 free.
Final artifact sizes are 652,140-byte HD LHA, 651,621-byte HD ZIP,
901,120-byte ADF, 645,318-byte WHDLoad LHA and 646,023-byte WHDLoad ZIP.

Alpha.65 is the Phase 6C.7 completion-integrity checkpoint. Stormstone Core
collection is now the sole Level-1 completion trigger; the obsolete far-right
test replay is removed. Diamonds collected in the current attempt remain
inactive across water, dry-gap and life-loss restarts, so their score cannot be
farmed, while a complete post-results replay correctly restores all diamonds
for a new attempt. Supplied FS-UAE/68030 HD testing accepts the solid right
boundary, diamond persistence and preserved Core/results/replay path.
FS-UAE/68020, ADF, WHDLoad and real-hardware alpha.65 acceptance remain pending.
The bootable alpha.65 DOS1/FFS ADF uses 1,616 blocks (808 KiB) and leaves 144
free; this is package/decode evidence, not ADF runtime acceptance. Final
artifacts are 651,536-byte HD LHA
(`45982aff27068dbb2e2b33dea6b3f7116d9a39a5355ba720bd704f87bd39219a`),
651,018-byte HD ZIP
(`c686444e24cf6a4b308ddf4c3a263848c657b88fb2dc06ba218a2830586eb28c`),
901,120-byte ADF
(`e9bb7b850d564176467b8ff14cd6e5060de6bb1629a8e5328a34aac44d174b45`),
645,127-byte WHDLoad LHA
(`fe5420b8991fae69446ecd5be47b0a61113ca2bf6e8be8596ab91e653f2b1eda`)
and 645,815-byte WHDLoad ZIP
(`bd7b48f7eb7fe06919866248111057d7d5a2cc91d758ad7d99d53e1dd22f5e26`).

Alpha.64 is the Phase 6C.6 score/results checkpoint. It adds an event-driven
four-digit HUD score, one-shot enemy and diamond awards, elapsed PAL-field
timing against a 120-second par, and an original double-buffered level-complete
tally with a bounded Paula tick and Fire-to-continue flow. Replay deliberately
performs a complete temporary Level-1 reload; the existing LOADING composition
remains visible during the slower 68020 rebuild. Native HUD digit copying and
coalesced projectile sweeps retain the accepted presentation while recovering
measured 68020 cost. Supplied FS-UAE/68030 and FS-UAE/68020 HD playthroughs
accept gameplay, score presentation, audio and replay. ADF, WHDLoad and real-
hardware runtime acceptance remain pending, as does replacement of the one-
level replay with a future multi-level state machine.
The bootable alpha.64 DOS1/FFS ADF uses 1,615 blocks (807 KiB) and leaves 145
free; this is package/decode evidence, not ADF runtime acceptance. Final
artifacts are 651,307-byte HD LHA
(`96647fa518a554fecda1d70bcf82c2c7cd4f200caa08d31a0115fc86f3a8ba76`),
650,739-byte HD ZIP
(`538214bad315a394d6491de793d40411ff9948c57bc9e97d64ebeac6a49277ae`),
901,120-byte ADF
(`3dd0da235c77904ef0a38c49c09aa30745b3d69086eb8ab68d93948ff477fde9`),
645,107-byte WHDLoad LHA
(`71b21b33ae1966d7d0d87bd571d13effb5228b3329ab9cb0477fbc03ff5b5293`)
and 645,733-byte WHDLoad ZIP
(`efd8b140363c52fd1879107f038aa700e0cceed81c3bd7e89220df284e31d8d3`).

Alpha.63 establishes one semantic native 16x21 diamond master for both the
world Bob and fixed HUD emblem. Generated SPBMs share an exact mask and
facet-role layout; only their fixed FRONT16/HUD8 palette mappings differ. The
host regression decodes both runtime assets and requires equality. Supplied
FS-UAE/68030 HD review accepts the clean shared design. All 48 positions, hover,
collision, counter/life award, Bob size, mask/cache and target-local renderer
ordering remain unchanged. FS-UAE/68020, ADF, WHDLoad and real-hardware runtime
acceptance remain pending.
The bootable alpha.63 DOS1/FFS ADF uses 1,461 blocks (730 KiB) and leaves 299
free; this is package/decode evidence, not ADF runtime acceptance. Final
artifacts are 604,520-byte HD LHA
(`f044aa16b95cca2b626b34a607f9a1f6034d0e407a3ce400edf6d9decc88dc44`),
604,014-byte HD ZIP
(`b4e0d4581dba7e512c31ff0c239a8ae106f918cff840144cee77bb2c8e290bb4`),
901,120-byte ADF
(`1d43a3c2ce52c690bf01ea8e5785302717f7b84b88e753faa7cb0a86dbe7a2da`),
598,605-byte WHDLoad LHA
(`ddd767ac9b7125cafeed21c8ba4dcdd5f565b0a40449933368ac5a86d82d4f85`)
and 599,247-byte WHDLoad ZIP
(`58ce16f369cee57b39980729adfdaa672c0707c93e0d4a3b8a9829240dbb7cb2`).

Alpha.62 changes only intro plate 1 at runtime. A fixed mixed-case
`LMB to skip intro` label sits at x=8, y=157 in a native 5x7 white face with a
one-pixel black shadow, immediately above the cyan divider. It remains static
through both scrolling passages; plates 2..5 stay unlabelled. One dedicated
white palette pen is reserved on plate 1 by merging its least-used art pen into
the nearest neighbour. Pure-black COLOR00, six-plane dimensions, allocation,
Copper code and immediate LMB skip input are unchanged. MrDig's supplied
FS-UAE/68030 HD test accepts the final presentation and full traversal through
title, LOADING, CHARGING and the ready menu. The first unnumbered drawer was
rejected because an obsolete Makefile subset omitted both ready assets; the
corrected manifest-driven drawer is accepted. Test tooling now stages from the
authoritative release manifest, preserves release inventories and forbids
SemVer/release work before acceptance. FS-UAE/68020, ADF, WHDLoad and real-
hardware runtime acceptance remain pending.
The bootable alpha.62 DOS1/FFS ADF uses 1,459 blocks (729 KiB) and leaves 301
free; this is package/decode evidence, not ADF runtime acceptance. Final
artifacts are 604,338-byte HD LHA
(`298e7574883d9c2f8f60a0b507422126af85de5fc8dd3a5bea1ef94535a34fcf`),
603,884-byte HD ZIP
(`1dc104503e652a5b60f64bf06f743a7abcdce013e905d3f24bf8c3fef406ee61`),
901,120-byte ADF
(`4b18b6280b4a71b4902319bc7c5f116b251d41ed97dc1caac1bad4859a99c443`),
598,675-byte WHDLoad LHA
(`1fcf8fb03d7cfe7659b556968cab71b0041cbe576b57abcd50b3451391fa6c5b`)
and 599,318-byte WHDLoad ZIP
(`b0a28532ebb79f2094bfffe4323b3cbb88a82d651dc5d1b0b887fb5751cec2cf`).

Alpha.61 changes only ordinary jump slots 10..13, crouch-fire slot 48 and their
deterministic mirrors. Jump retains its accepted poses, bottom anchor, timing
and physics but moves from 43..44 to 45..46 visible rows. Crouch-fire slot 48
removes only a redundant source muzzle flare: the separate runtime projectile
remains, and avoiding the former 56x29-to-48x25 emergency fit restores the
character to 28 visible rows between its 29..30-row neighbours. MrDig's
supplied FS-UAE/68030 HD review accepts ordinary jump, unchanged airborne fire
and corrected crouch fire. Air-fire remains intentionally 42..44 rows because
its compact posture is accepted and enlarging its long silhouette previously
caused weapon/anatomy regressions. Idle, run, landing, animation selection,
gameplay, collision, renderer, memory layout and audio otherwise remain
alpha.60. FS-UAE/68020, ADF, WHDLoad and real-hardware runtime acceptance
remain pending.
The bootable alpha.61 DOS1/FFS ADF uses 1,458 blocks (729 KiB) and leaves 302
free; this is package/decode evidence, not ADF runtime acceptance. Final
artifacts are 603,924-byte HD LHA
(`54466ce9e6a428ad453da0e6b7fe4f91e47d54bbcdfbcf24a1963373d1a37534`),
603,502-byte HD ZIP
(`ae695ca3e03035ee3916890aa6904bdd4ca57ae991c7d91ea22a85b51caef81d`),
901,120-byte ADF
(`c7f8ebde3c95eda33e83e6be95f93e6748fe5ced9160924e912102e3548683de`),
598,552-byte WHDLoad LHA
(`507e1e6bf43a0e644e00ef6f067387ea907cb3cb607f7b1daf66cc1da089cda7`)
and 599,205-byte WHDLoad ZIP
(`b74f07f8b6d20f044dcc1f4e9d9a43715a7bbe1e8e73c47aced8998c57933595`).

Alpha.60 changes only player side-idle/blink slots 0/1 and long-idle slots
26..37. They now share one 46-row scale, and slot 26 is pixel-identical to slot
0 so the long front-facing performance returns without a height step. MrDig's
supplied FS-UAE/68030 HD review accepts the ordinary idle/run improvement and
the final uniform idle family as good enough for this checkpoint. A rejected
whole-family reconstruction remains preserved only as source/evidence history
and must not be reconnected: it clipped crouch-fire's tail, enlarged airborne
fire and broke run/facing continuity. Runtime run, jump, crouch, shooting, hurt,
ledge, timing, gameplay, collision, camera, renderer and audio otherwise remain
alpha.59. FS-UAE/68020, ADF, WHDLoad and real-hardware acceptance remain
pending.
The bootable alpha.60 DOS1/FFS ADF uses 1,453 blocks (726 KiB) and leaves 307
free; this is package/decode evidence, not ADF runtime acceptance. Final
artifacts are 602,841-byte HD LHA
(`6cda312013e4e499f0d10c99a325560ca64fc2b0723572cb3debebb57bab3858`),
602,305-byte HD ZIP
(`e4aa45236bd1acf6fe9634e65215b6dfa0ffac335832f32c6edaab5810d548b6`),
901,120-byte ADF
(`b375098577d9328d72ac4f13d1ffe723c598250b2565c1a1d94ced10da409a6c`),
598,058-byte WHDLoad LHA
(`9be547b968ecca9fcc63d8e77ba4a872bff15688271365ab59ac34b6c45fab06`)
and 598,539-byte WHDLoad ZIP
(`383d1e1c501b6dda8dc13f189a3cd6c365faf7ef7facd33354954116e4ceed28`).

Alpha.59 replaces the ordinary 105..202px horizontal camera dead zone with a
single centred anchor. Sparkpaw's 32px logical body now has its visual centre
at screen x=160 while the accepted final Core clearing retains its fixed
maximum-camera composition. MrDig's supplied FS-UAE/68030 HD A/B/C test accepts
this exact centred candidate as better with no observed problems. The separate
16px directional-lookahead candidate is rejected because its return to centre
visibly shifts the image. Renderer, physics, level geometry, animation, assets
and audio are unchanged. FS-UAE/68020 cadence, ADF, WHDLoad and real-hardware
runtime acceptance remain pending and must not be inferred from the 68030 HD
visual/function result.
The bootable alpha.59 DOS1/FFS ADF uses 1,450 blocks (725 KiB) and leaves 310
free; this is package/decode evidence, not ADF runtime acceptance. Final
artifacts are 601,469-byte HD LHA
(`48573ffec3acfcd132eb7fe36c499836eb758cc1ff6baa34ecc0f0d11ba54801`),
601,106-byte HD ZIP
(`5689fba117c7e556c08f9ed383f9ef6a0bf5a484152395f995777c7c6b43c5f1`),
901,120-byte ADF
(`c912b6898e21d317f06acf57d41b517131b4ce84dc82f70d284464723d7cb75e`),
596,689-byte WHDLoad LHA
(`e37bbcedd4f92eba883e01e386fbe3a0c44836373095000165069e102d11ffcc`)
and 597,341-byte WHDLoad ZIP
(`985a5b75db20bebe9cacd60e7bdfa34f99c7ea2994231818d7a9fefd201af3a1`).

Both LHA artifacts are genuinely compressed with classic `-lh5-`, not stored
as the former Python-generated `-lh0-` members. Sparkpaw uses the ignored local
`.toolchain/lha/bin/lha` (classic LHa 1.14i) or an explicit absolute `LHA`
override, CRC-tests each completed archive and rejects output without an
`-lh5-` member. Homebrew Lhasa remains useful for independent list/test/extract
verification but cannot create archives.

Alpha.58 removes the intermittent ready-screen fragments visible on supplied
real-A1200 footage. Alpha.54 patched the displayed six-plane bitmap; alpha.58
seeds two complete ready buffers and patches only the hidden one. COP1LC is
armed during hardware lines 100..249 without COPJMP1, allowing the Copper to
adopt the inactive complete list at its natural vertical restart. The first
hardware retest still glitched while completely idle, proving publication was
not the sole cause. Full takeover also enabled hardware-sprite DMA although the
title/loading/ready Copper lists initialize no sprite pointers. Sprite DMA now
stays off until the gameplay Copper is installed. Never enable a DMA channel
before the active Copper/display state owns every pointer it can fetch.

Supplied FS-UAE/68030 HD and real-A1200/68030 HD testing accepts idle START
GAME/OPTIONS screens, rapid main-menu and JUMP/FIRE switching, and gameplay
entry. ADF and WHDLoad runtime acceptance remain pending. The second ready
buffer costs 61,440 bytes Chip RAM; gameplay, controls, assets, audio and Stage
5L/H7 are otherwise unchanged.
Subsequent supplied testing also reports the packaged alpha.58 result working
correctly on real hardware and on an Analogue Pocket. The launch path for that
additional confirmation was not recorded, so it does not by itself establish a
new physical-ADF or WHDLoad acceptance claim.
The alpha.58 bootable DOS1/FFS ADF uses 1,449 blocks (724 KiB) and leaves 311
free; this is package/decode evidence, not ADF runtime acceptance. Final
artifacts are 601,246-byte HD LHA
(`069f474984bb0ae6eaa8b69b807307bdfa001c91ad209bacc910b4052c6d2eb2`),
600,901-byte HD ZIP
(`b92b630d95f33fdb317065b4ac6896ec87fca16329009ecde6d1854eddc23ab2`),
901,120-byte ADF
(`26c3a01a2d56e91474d7a314895d6d26eca4e6930c531aa074b5d519ff187660`),
596,720-byte WHDLoad LHA
(`5f9384e27da7f5f1c92cafcd11fbceeb754d1915e01e13f939779e15995460ca`)
and 597,374-byte WHDLoad ZIP
(`b7f9ab3732cb6e65d0dccd9128c1da151f423e843de77d85083e76a2af94bd14`).

Alpha.55 is packaging-only: it promotes this real LHA compression into a new
release identity so the already published alpha.54 bytes are never reused for
different archive contents. Executables, runtime assets, archive layouts, ADF
game data, runtime loader, renderer, gameplay and audio are unchanged from
alpha.54; versioned names and packaged ReadMe text advance to alpha.55. Its
bootable DOS1/FFS ADF uses 1,445 blocks (722 KiB) and leaves 315 free.

Supplied real-A1200 evidence rejects alpha.55 WHDLoad intro traversal: plate 1
and both passages display, then the program returns cleanly to Workbench where
plate 2 should load. The ordinary alpha.55 HD build does not reproduce the
failure. Workbench visibly shortens the 31-character versioned WHDLoad drawer
to 30 characters, but successful launch and plate 1 make that parent name an
insufficient cause. A focused short-drawer `Sparkpaw-WHDIntroDiag` ZIP logs each
plate load, detailed asset failure, IoErr and Chip/Fast free/largest values to
`data/whdintrodiag.log`. The supplied log confirms plate 1 loads, then plate 2
fails at `Open` with IoErr 205 (`ERROR_OBJECT_NOT_FOUND`) while roughly 2.07 MB
Chip and 7.70 MB Fast remain free. This rules out allocation and read failure.
The 21-character diagnostic parent also rules out the versioned parent drawer
as the necessary cause; plate 2's own 31-character filename is the failing
WHDLoad boundary. Source comparison shows alpha.52 and alpha.54 use the same
intro names and WHDLoad compile/slave path; alpha.54 only adds the ready menu.
The relevant recent change is alpha.55 archive construction (`-lh0-` level-0
members to classic `-lh5-` with explicit directories), while the diagnostic
was delivered as ZIP. The earlier working package therefore depended on an
archive/extractor combination preserving overlength components. The manifest
also contains 36-character intro plate 3 and 32-character ready-menu names, so
all WHDLoad runtime components must be made <=30 rather than relying on archive
format behavior. The focused `Sparkpaw-WHDShortNames.zip` changed those assets
to `intro2.spbm`, `intro3.spbm` and `readymenu.spbm`. MrDig's supplied real-
A1200/68030 test accepts this correction through all five intro plates, title,
loading, charging and the ready menu.

Alpha.56 promotes that accepted correction universally. HD and WHDLoad use the
same short canonical SPBM names, while the ADF packer consumes those same
sources for its already-short SPR1 names. Release tooling rejects runtime or
extracted drawer components longer than 30 characters. The descriptive WHDLoad
artifact filename remains versioned, but it extracts to the Amiga-safe drawer
`Sparkpaw-0.6.0-a56-WHDLoad`. Asset bytes, renderer, gameplay and memory
configuration are unchanged from alpha.55.
The alpha.56 bootable DOS1/FFS ADF uses 1,446 blocks (723 KiB) and leaves 314
free; this is package/decode evidence, not new ADF gameplay acceptance.

Alpha.54 expands the accepted Phase 6C.4 ready screen into a two-item menu.
`START GAME` remains selected by default and Fire/Space follows the existing
fade into the already prepared level. `OPTIONS` exposes one session-only
`SECOND BUTTON` choice: `JUMP` preserves alpha.50, while `FIRE` keeps Up/W as
jump and merges the secondary button into the primary-Fire/Space press edge.
A shared-palette Fast-RAM patch atlas updates only the static central menu band;
the six-plane display, Stage 5L/H7 renderer, physics and audio are unchanged.
The final word-aligned x=80..239 atlas preserves both lower corner
compositions, omits credits from Options and uses symmetric JUMP/FIRE arrows.
Supplied FS-UAE/68030, FS-UAE/68020 and real-A1200/68030 HD testing accepts
presentation, direct start and both mappings. The 68020 production-style run
contains no cadence logger, so it adds no numerical FPS claim. ADF and WHDLoad
runtime acceptance of alpha.54 remain pending. The raw 50,124-byte menu atlas
packs to 40,805 bytes on ADF; the bootable DOS1/FFS image uses 1,444 blocks
(722 KiB) and leaves 316 free.

Alpha.50 adds an optional secondary-button jump input on joystick
port 2 alongside the preserved joystick Up and keyboard W paths. Port 2 pin 5
holds a CD32 pad in its reset state and active-low POTINP bit 14 reads the
ordinary second-button/Blue line. All jump sources merge before the existing
edge detector, so holding or overlapping them cannot retrigger a jump. Primary
Fire/Space still shoot; physics, animation, audio and renderer are unchanged.
MrDig's supplied real-A1200 HD test accepts the secondary-button jump. The
controller model was not recorded, and no FS-UAE, ADF, WHDLoad, Pocket or
separately identified CD32-pad acceptance is inferred.
The bootable alpha.50 DOS1/FFS ADF validates at 1,354 blocks (677 KiB), leaving
406 free; this is package/decode evidence, not ADF gameplay acceptance.

The WHDLoad packages wrap a dedicated WHDLoad executable/runtime in a Kickstart
3.1 BootDOS slave, fit the established 2 MB Chip plus 8 MB Fast target and
provide F10 as the WHDLoad exit key. They include neither WHDLoad nor Kickstart.
Package construction is host-verified. Supplied real-A1200/68030 testing first
accepted WHDLoad startup/loading but rejected the alpha.49 F10 exit after
Sparkpaw's custom-chip takeover. Alpha.51 promotes the accepted correction: a
WHDLoad-only executable catches raw F10 during direct CIA polling, restores
system ownership and returns through the slave to Workbench. The normal HD and
ADF executables remain separate and unchanged by this compile-time path.
The bootable alpha.51 DOS1/FFS ADF validates at 1,355 blocks (678 KiB), leaving
405 free; this is package/decode evidence, not new ADF gameplay acceptance.

Alpha.52 adds the same Sparkpaw project-icon artwork to ordinary HD and WHDLoad
packages. Its preferred layer is an embedded 86x93, 34-colour NewIcon accepted
in supplied real-A1200 evidence. Its classic fallback is deliberately 86x93 and
three bitplanes using only the eight standard OS 2.x/3.x Workbench pens; the
rejected 16-colour RomIcon experiment rendered with incorrect green/pink/grey
pens in the supplied FS-UAE Workbench. The eight-colour decoded preview is
accepted for now, but its exact FS-UAE display remains pending supplied retest.
HD uses `DefaultTool=Sparkpaw`; WHDLoad retains `DefaultTool=WHDLoad` plus
`SLAVE=Sparkpaw.Slave`, `PRELOAD` and `PAL`. The actual A1200
`ThunderCats.info` established the target NewIcons profile; the unrelated
downloaded icon variant must not be used as evidence.
The bootable alpha.52 DOS1/FFS ADF validates at 1,356 blocks (678 KiB), leaving
404 free. The ADF deliberately excludes `Sparkpaw.info`; its one-block growth
over alpha.51 is packaged ReadMe text, not icon or runtime data.

Alpha.49 reserves pure black palette pen 0 in all five intro plates and the
ready screen. This removes the full-height one-pixel `COLOR00` border exposed
by the Indivision but hidden in FS-UAE/CRT overscan. Supplied FS-UAE/68030 HD
and real-A1200/Indivision HD testing accept the fix. A host regression now
rejects any fullscreen direct-Copper SPBM whose palette bytes 12..14 are not
zero. Copper geometry, presenter timing, renderer and gameplay are unchanged.

Alpha.48 adds the accepted 64-colour pre-level ready screen after CHARGING and
after complete gameplay/renderer preparation. Supplied FS-UAE/68030 and
FS-UAE/68020 HD testing accepts the isolated crest-free wordmark, Level-1 edge
architecture, centred prompt/credits, Fire and Space controls and immediate
transition into gameplay. HD retains alpha.47's complete five-plate story
intro. The ADF deliberately omits only those cinematic plates and begins at the
existing title; loading, charging, ready screen and gameplay remain shared.
The bootable DOS1/FFS ADF uses 1,353 blocks (676 KiB), leaving 407 free; the
packager verifies every retained SPR1 stream against its source. ADF gameplay
and real-hardware acceptance remain pending.

MrDig mounts `sparkpaw/dist/` directly as an FS-UAE HD volume. Every
user-facing FS-UAE HD test or diagnostic drawer must therefore be created
directly under `sparkpaw/dist/`, following the established clearly named
stage/checkpoint structure and including its executable, `ReadMe.txt` and
complete required `assets/runtime/` subtree. Do not deliver such drawers only
under `build/` or `build/test/`; those are internal build intermediates and are
not visible in the mounted HD.

`sparkpaw/dist/` is exclusively MrDig's manual test surface. Codex may stage a
self-contained drawer there and perform read-only byte, hash and manifest
checks, but must never launch or boot an executable from `dist` itself. Every
Codex-run FS-UAE proof must instead use a separate drawer below
`sparkpaw/build/fsuae-selftest/`. Do not request permission to boot from
`dist`; hand the staged drawer to MrDig for the actual 68030/68020 test.

For every future A/B test, create one fully self-contained subdrawer per
variant. Each subdrawer must contain its own executable, complete runtime
assets and ReadMe, so `PROGDIR:renderdiag.log` is naturally unique. Never ask
MrDig to rename or move a log between variants.

Keep the mounted `sparkpaw/dist/` root uncluttered. It should contain only the
current alpha release artifact set (including both WHDLoad archives) and the
single currently active diagnostic
or A/B test set; `my-files/` and `older-builds/` remain as fixed storage
drawers. As soon as a diagnostic set is superseded or its evidence has been
preserved in `sparkpaw/testresults/`, move its complete self-contained drawers
and matching FS-UAE `.uaem` metadata intact into `sparkpaw/dist/older-builds/`.
Do not leave multiple generations of debug/test drawers in the mounted root,
and never delete their logs while tidying it.

Do not create the >100 MB Source ZIP unless MrDig explicitly requests it.
Opt-in command: `tools/make_release.py --include-source`.

MrDig supplies authoritative FS-UAE and real-hardware results. Never infer ADF
parity from HD, FS-UAE from host builds, or real hardware from emulation. His
real A1200 has a 68030, so stock-68020 performance is tested in FS-UAE; similar
behavior on a real 68020 is a hypothesis, not supplied hardware verification.

## Current acceptance boundary

- Phase 6C.1 twelve-screen traversal is accepted in supplied FS-UAE/HD testing.
- Alpha.29 elevated-beetle hit behavior is accepted in FS-UAE/HD.
- Alpha.33 overlapping-Strider presentation is accepted in FS-UAE/HD.
- Alpha.34 projectile/wall sweep is accepted in FS-UAE/HD.
- Alpha.39 restores acceptable presentation and speed in FS-UAE at 68030.
- Alpha.39 ADF works on the supplied real A1200/68030.
- Alpha.39 HD and ADF both work in FS-UAE.
- Alpha.39 HD is rejected on the real A1200: after CHARGING the HDMI output is
  mostly black with horizontal remnants and CRT output shows moving cyan noise.
- Alpha.40 HD works and is playable on the real A1200/68030 when roughly
  1.92 MB Chip RAM is free, including Boot With No Startup-Sequence and a
  two-colour Workbench. It fails after CHARGING with 1,430,032 Chip bytes free.
  This isolates the normal-Workbench failure to the current Chip-RAM budget,
  not MaxTransfer, the HD device or the gameplay renderer itself.
- Alpha.41 calculates about 642 KiB permanent Chip savings plus 54 KiB during
  status loading, without removing reachable pixels or animation frames. Its
  HD build loads and plays on the real A1200/68030 from a normal Workbench with
  about 1.4 MB Chip RAM free. This accepts the real-HD Chip-RAM gate. Its ADF
  uses 1,177 blocks and leaves 583; alpha.41 ADF regression is still pending.
- Real-A1200/68030 alpha.40 and alpha.41 footage both reject presentation in
  the first two-Strider scene due to intermittent enemy glitches and apparent
  cadence loss. Beetles are also reported to glitch occasionally. Because the
  issue predates alpha.41, do not blame its Fast-RAM masters or staging change
  without new evidence.
- Alpha.42 promotes the Stage 4G no-copy rolling renderer. Supplied FS-UAE/HD
  testing accepts clean enemy/projectile presentation, smooth cadence and the
  corrected HUD boundary. Its 1,983 measured intervals contain 1,952 one-field
  and 31 two-field updates (49.23 effective FPS), with zero ownership
  violations. New supplied recordings reject alpha.42 performance on FS-UAE
  at 68020 and on the real A1200/68030 at about 34.5 MHz. They do not show an
  obvious return of corruption or trails in sampled frames. The 49.23-FPS
  result is specific to the faster FS-UAE/68030 configuration and must not be
  extrapolated to target hardware. Alpha.42 ADF and Analogue Pocket acceptance
  remain pending; real-A1200 performance is explicitly rejected.
  The bootable alpha.42 ADF uses 1,186 blocks and leaves 574; this is package
  validation only, not ADF gameplay acceptance.
- Stage 5L keeps the coherent Stage 5G early-fetch playfield and replaces the
  six-channel player DMA layout with the same 48x48, 15-colour pixels inside
  one transparent-padded 64-pixel attached AGA pair on channels 0/1. Supplied
  FS-UAE/68030 HD testing reports no corruption, glitches or flicker. Its log
  records 2,163/2,163 one-field intervals (50.00 FPS) and zero ownership
  violations. Alpha.43 promotes that exact route to normal HD and ADF builds.
  Supplied real-A1200/68030 HD and physical-ADF tests, plus Analogue Pocket ADF,
  report no broad renderer corruption. All slower paths reject cadence; real HD
  also repeats or misses some sound events under load. A narrow intermittent
  ground/HUD seam disturbance remains. Package validation reports a bootable
  DOS1/FFS ADF using 1,190 blocks and leaving 570 free.
- Stage 5L in FS-UAE/68020 measures 26.38 effective FPS (136 one-field, 337
  two-field and 78 three-field intervals). This stress configuration is slower
  than the supplied real 34.5 MHz 68030, while FS-UAE/68030 is much faster.
- Alpha.44 packages the supplied FS-UAE/68030-HD-accepted H7 seam correction
  and accepted Stage2 defaults without changing Stage 5L geometry, assets or
  gameplay. Its DOS1/FFS ADF validates at 1,195 blocks used and 565 free.
  Alpha.44 HD/ADF gameplay on the real A1200 and ADF gameplay on Analogue
  Pocket remain pending supplied tests; package validation is not acceptance.
- Alpha.45 packages three more isolated Stage2 defaults accepted in supplied
  FS-UAE/68030 and FS-UAE/68020 HD tests: direct Strider traversal lookup,
  invariant Bob-register setup and a specialized aligned 16px entering-column
  copy. The latest matched 68020 A/B raises cadence 44.47 to 45.55 FPS and
  lowers ring-roll p95 9,878 to 2,782 CIA ticks; Bob-pass p95 falls 16,308 to
  9,142. Final supplied testing accepts alpha.45 presentation and cadence on
  the approximately 34.5 MHz real A1200/68030 from both HD and physical ADF,
  and accepts the Analogue Pocket 68020 ADF path. Its bootable
  DOS1/FFS ADF validates at 1,197 blocks used and 563 free; this is package
  construction evidence only.
- A post-alpha.45 minimal-cadence diagnostic removes nested CIA scopes and Bob
  family raster timing while retaining renderer-boundary cadence sampling.
  Supplied FS-UAE/HD testing reports normal presentation. FS-UAE/68030 records
  1,578/1,578 one-field intervals (50.00 FPS); FS-UAE/68020 records 1,104
  one-field and 33 two-field intervals out of 1,137 (48.58 FPS), with no
  three-field intervals or ownership violations. The immediately preceding
  fully instrumented 68020 reference measured 44.21 FPS under a different
  workload, proving that most of the apparent remaining 4--5 FPS deficit was
  profiler observer cost. Do not extrapolate this diagnostic result to ADF,
  Pocket or real hardware by itself. Those acceptances were supplied later as
  the separate final alpha.45 tests above.
- Alpha.39 ADF on an Analogue Pocket FPGA core shows widespread transient Bob,
  gameplay-field and HUD corruption at 68020/no-cache. Treat this as a useful
  missed-deadline stress signal, not as FS-UAE or real-A1200 equivalence.
- ADF full gameplay parity beyond the supplied real-hardware working result is
  not generalized to every configuration.

Evidence handling: inspect consecutive video frames, preserve bytes, rename new
timestamped files meaningfully and add matching `.txt` sidecars. Separate
observation from diagnosis.

## Display and renderer invariants

- PAL 320x256, resident 3072x256 world.
- AGA dual playfield: four-plane FRONT16 plus three-plane REAR8 at quarter
  camera scroll.
- Player remains a 48x48 15-colour actor, transparently padded in one attached
  64-pixel AGA hardware-sprite pair on DMA channels 0/1.
- Gameplay foreground uses two hidden/displayed target pairs. Each target has
  a logical 512px FRONT16 ring repeated across a 1536px physical stride; the
  resident clean world remains the canonical source for entering columns.
- Two complete Copper lists publish target/pointer state atomically at a fixed
  PAL boundary. CPU and Blitter never modify the displayed target.
- Copper switches to the separate 320x48 HUD at hardware line 252.
- Target-local Bob composition starts after update on the inactive target.
- Active family order: projectile erase, enemy restore, collectible restore,
  splash restore, water maintenance, splash draw, collectible draw, enemy draw,
  projectile draw, final Blitter wait.
- Use packed masks/caches, camera culling and Blitter copy/cookie-cut minterms.
- Never CPU read-modify-write displayed Chip RAM.
- Preserve the alpha.33 stable vertical enemy ordering and intersecting-Strider
  restore union.
- Alpha.37 tight per-frame enemy bounds is rejected: it damaged sprites.
- Alpha.38 hidden 512x208 viewport copying is rejected: it worsened 68020 and
  68030 cadence and logged 3,324 wraps in 3,722 compositions.
- Normal production logging must not insert per-family `WaitBlit` barriers.

The background pictures are resident bitplanes fetched by Agnus, not CPU-
redrawn every frame. Their DMA cost remains a valid performance measurement,
but “background rendering” is not an assumed CPU bottleneck.

## Current gameplay contracts

Player:

- Frames 0..49 accepted base; standing hurt 50..53, crouched hurt 54..57 and
  ledge balance 58..61 are append-only.
- Joystick port 2 Up and keyboard W feed the jump action. The ready-menu option
  assigns the secondary button to that same edge by default, or to the existing
  primary-Fire/Space shoot edge for the session.
- Preserve scale, feet, mirrored facing, run/jump/landing/crouch/turn/shoot/hurt
  selection, three hearts as six health units and accepted life/reset behavior.
- Sprint/jump performance investigation is parked; preserve existing physics.

Enemies/projectiles:

- Four camera-managed runtime enemy slots retain persistent spawn state.
- Beetles are 32x24, two HP. Standing fire naturally misses floor beetles by Y;
  crouch fire hits them; geometric standing fire hits elevated beetles.
- Striders are 64x64 FRONT16 Bobs with 28 append-only frames, three HP,
  authored traversal, ranged fire, hurt/death and off-camera persistence.
- Logical Strider collision Y is unchanged; drawing retains the accepted +2px
  visual offset for transparent source rows 62-63.
- Player shots sweep from the physics edge through each crossed X coordinate,
  testing solid geometry before enemy damage.
- Off-screen projectiles and water strips are camera-culled with synchronization
  on entry.

HUD/collectibles/audio:

- HUD is separate and internally double-buffered; only changed counters patch.
- Forty-eight masked diamonds hover through synchronized clean/display updates;
  counter/life award behavior is accepted. Diamond-art replacement is deferred.
- Player plasma owns Paula channel 0. Prioritized gameplay effects own channel
  1; player hurt outranks enemy death, which outranks Strider fire.

Water/route:

- Two animated water hazards and dry gaps are active in the accepted route.
- Full level traversal through the corrected second-water platform is accepted
  in FS-UAE/HD. ADF and real-hardware claims stay evidence-specific.

## Current work order

See [CURRENT_STATUS.md](sparkpaw/docs/CURRENT_STATUS.md). Performance work is
parked by explicit user decision. The current step is the 0.7.0-alpha.3 checkpoint and separate native ADF/WHDLoad/hardware presentation gates; historical plan:
authorized multidisk/loading plan. Asset selection changes belong to that
media boundary. Level-2 design remains separate.

### Historical work order and acceptance chronology

The dated entries below preserve intermediate evidence. Their “next”, “pending”
and drawer references are not active instructions; consult the status index.

#### Stormrail direct-start development baseline (2026-09-01)

Do not create a new alpha yet. The user-accepted v7 boarding build is the
historical Gate-1 baseline and is preserved under
`sparkpaw/dist/older-builds/Stormrail-Board-v7-030-HD`. The sole active focused
drawer is now `sparkpaw/dist/StormG2-Table-Cadence-020-HD`, executable
`StormG2-Cadence`. It
bypasses intro, title and Level 1 and begins at the short cliff approach. That
shortcut is compile-guarded by the Stormrail proof/user-test defines and must
not enter production or release targets.

Accepted Gate 1 consists only of familiar Level-1-like cliff material, a
visible point of no return, the hovering 104x46 Skimmer with subtle hover and
its existing cyan engine pulse, physical cockpit entry, canonical
contact/settle poses, the restored clean pre-experiment sink pose and the
approved occupied head-only flight pose. The rejected hand-authored sink
neck/scarf pixels must not return. Evidence and rationale live in
`sparkpaw/testresults/Phase 6D-step1-rejected-v7-invalid-sink-neck.*` and
`sparkpaw/docs/STORMRAIL_INTERLUDE_PLAN.md`.

Sparkpaw consistency is not limited to anatomy. Every later pose must preserve
the accepted character's fur base/shadow balance, muzzle and inner-ear cream,
eyes, scarf, gauntlet, outline, expression and proportions. Do not copy or
rescale the HUD portrait into gameplay art; derive and validate the same palette
roles in the pose's own native pixels.

Gate 2 and Gate 2.5 are now the preserved isolated control, art, architecture
and performance baseline. Add no campaign integration, results, ADF or
multidisk work implicitly; the final roughly 2.5-minute authored route remains
later Gate 6D.6 scope. Self-test small technical changes first and stage only
meaningful checkpoints for MrDig.

Flight owns `stormrail-flight-rear.spbm`, a bounded 768px REAR8 route span plus
352px fetch overlap, while accepted boarding still owns the unchanged v3 rear.
Both use the existing rolling 4+3 compositor and Copper publication; there is
no second renderer. The first candidate's per-X quantization produced visible
vertical colour seams and its direct rear switch was too abrupt. The revised
bitmap uses one stable semantic pen encoding across the complete loop. Four
route palettes still morph by `stormrailDistance` and scanline. After boarding,
the craft accelerates right out of the approach, the playfield fades to black
over eight palette steps while the HUD stays fixed, both Copper lists switch
under black, and the shooter world fades in as the craft re-enters from the
left. Once both flight targets are armed, the approach front and rear
allocations are released. Bounded FS-UAE/68030 evidence in
`sparkpaw/build/fsuae-selftest/stormrail-handoff-seam-v2-20260901-145629`
shows intact approach, fade, re-entry and route phases. The wrap proof in
`sparkpaw/build/fsuae-selftest/stormrail-fade-wrap-v2-20260901-145845`
crosses distance 32,768 and eleven rear loops with zero unsafe Blits and
1,305,232 Chip bytes free.

The user accepts the transition, route presentation, compact craft and final
corrected palette-table version in FS-UAE/68030, then reports no visual fault
in the FS-UAE/68020 cadence drawer
`sparkpaw/dist/StormG2-Table-Cadence-020-HD`. The first table build was rejected:
it emitted 12 rather than all 13 rear bands and exposed an out-of-bounds colour
strip immediately above the HUD. Its evidence is preserved under
`dist/older-builds`; the generator now asserts 13*8 words per palette.

Performance diagnosis found repeated C route interpolation—not parallax DMA,
ship Bobs or shots—as the stock-68020 blocker. The original targeted profile
measured `scroll_patch` at 29,413 median ticks and 24.40 FPS. A division-free
rewrite reduced that to 7,499 ticks. Final generation maps all 768 phases to 23
bit-exact 12-bit palettes in roughly 5.6 KiB readonly/Fast data; inspected
68020 assembly has no per-colour interpolation loop. Corrected 68030 profiling
measures 45 median `scroll_patch` ticks, 49.90 FPS and zero ownership
violations. Final low-overhead FS-UAE/68020 evidence records 49.26 FPS over
1,000 intervals: 992 one-field, 3 two-field, 5 three-plus, maximum four fields,
zero ownership violations and 214/214 requested/started shots. Remaining long
intervals occur around one-time handoff/target initialization; steady Flight
is 50 Hz. Stop here after Gate 2/2.5 until the user explicitly begins the next
content gate.

The prepared continuation prompt is
`sparkpaw/docs/NEXT_SESSION_STORMRAIL_GATE3_PROMPT.md`. Gate 3 is deliberately
bounded to two small enemy types, two deterministic formations, formation
completion plus one 3--5-diamond reward chain. It must preserve the accepted
empty-flight cadence and stop for explicit acceptance before hazards, the full
route, results or campaign integration.

Later integration remains mandatory and separate: Level-1 results must offer
resident `REPLAY LEVEL` versus load-bound `CONTINUE JOURNEY`; continuing must
unload Level-1-specific state under black/loading presentation before loading
Stormrail; each section owns a run score; later results also show cumulative
campaign score; and `REPLAY INTERLUDE` restores the immutable post-Level-1
snapshot and starts the interlude score at zero. No score may be banked twice.
Keep the current one-level ADF frozen until a measured multidisk proof is
justified; HD/WHDLoad development may continue independently.

### 1. Preserve the completed Stage 5L/H7 renderer baseline

Stage 5L plus H7 is the immutable renderer baseline. Do not change the fixed-HUD
FMODE/pointer split, ring ownership, early-fetch geometry, the wide Sparkpaw
pair, art or gameplay during residual performance work without new renderer
evidence and a separately gated correction.

H5 established bitplane rather than sprite origin. H6A then removed the seam
by masking FRONT16 colours on only the final transition scanline; H6B's REAR8
mask did not. The user explicitly accepts H6A in FS-UAE/68030 with no other
visible corruption; its log records 49.93 FPS and zero ownership violations.
The same operation was production-compiled and subsequently accepted by the
user in FS-UAE/68030 HD as H7. H7 intentionally created no log.

The original H7 gate accepted only FS-UAE/68030 HD. Final alpha.45 testing now
also accepts the complete result on real-A1200/68030 HD, physical ADF and
Analogue Pocket ADF. Do not reopen the Copper split, Stage 5L
fetch geometry, rolling ownership or wide-sprite layout during performance
work without new evidence of a renderer regression.

The title contract is unchanged: 35 black PAL frames after display takeover,
24 fade frames and 225 fully visible title frames. Faster loading before title
takeover must not be confused with a shortened Indivision stabilization delay.

### 2. Preserve the completed alpha.45 performance checkpoint

Use `sparkpaw/docs/PERFORMANCE_68020_PLAN.md`.

The broad C/assembly audit and ranked table produced the alpha.44/45 gains.
The final low-overhead diagnostic records 48.58 effective FPS on FS-UAE/68020:
1,104 one-field and 33 two-field intervals out of 1,137, no three-field misses
and zero ownership violations. This near-50-Hz result is a protected regression
baseline for every later feature and refactor.

The residual profiler and follow-up review already split CPU work, custom
register setup and `WaitBlit`, regenerated VBCC output, checked runtime copies
and re-audited Fast/Chip allocation. Most of the prior apparent 4--5 FPS gap was
observer cost from the detailed profiler. No forgotten continuous large copy,
runtime asset load or compiler helper remains as an active big-gun candidate.

A separate Phase 6C.2 checkpoint, released as alpha.46, extends Level 1
to 3392px with one enemy-free Stormstone Core clearing. It adds static FRONT16
waystation/tree/Core art and a single overlap-triggered call to the existing
in-memory replay path; it does not change Stage 5L/H7, add a Bob/Blit family or
define the permanent level-complete flow. Measured bitmap-source growth is
40,768 resident bytes and 12,739 packed ADF bytes; collision-map/cache growth
is 600 bytes. Host tests and native 68020 compilation pass. FS-UAE presentation
and the protected low-overhead 68020 timing comparison remain required before
acceptance. See `sparkpaw/docs/concepts/story-intro/LEVEL1_CORE_CLEARING_PLAN.md`.

Its first supplied FS-UAE/68030 HD gate is rejected before gameplay: CHARGING
remains visible with several short cyan fragments at the upper-left edge, and
the exact drawer contains no clean-exit `renderdiag.log`. Treat asset-load,
allocation and renderer-preparation failure as hypotheses until a focused
startup diagnostic distinguishes them. Do not create the 68020 gate yet.

The focused rerun records `failed_rear_guard_prepare`, with 786360 free Chip
bytes and a 784400-byte largest block. The problem is therefore not Chip
exhaustion. The 1120px source's padded physical stride plus the required four
guard bytes exceeded the destination stride requested from logical width.
The pending correction requests the guarded bitmap from
`(source->BytesPerRow + 4) * 8`. Retest only this 68030 HD candidate before any
68020 promotion.

The supplied corrected FS-UAE/68030 HD run reaches the final clearing and its
startup log records `renderer_prepare_complete`, with 732624 Chip bytes free.
The user rejects the presentation: an old platform remains at left, the
house/tree is pressed against the right edge and reduces to grey/black, the
Core is a flat static shape, and contact reloads before a reward beat is seen.
Planning now recommends a fixed final camera at x=3072, centred house/Core,
clearing-specific FRONT16 palette roles, an attached hardware-sprite Core pair,
a 32-frame collection state and a unique priority-11 Paula effect. See
`sparkpaw/docs/concepts/story-intro/LEVEL1_CORE_CLEARING_POLISH_PLAN.md`. No
68020 promotion is authorized yet.

The completed Phase 6C.2 work uses a 64x48 FRONT16 Core Bob rather
than the rejected extra attached sprite pair. Supplied FS-UAE/68030 evidence
accepts its integrity, calmer idle, centred clearing, Storm Triumph sound and
delayed replay, but its narrow directional pickup lines disappear behind the
higher-priority Sparkpaw sprite. The current focused candidate replaces only
those twelve pickup cells with an outward radial release and adds a two-field
foreground Copper-palette lift; player sprites, HUD palette, Bob dimensions,
world geometry and draw order remain fixed. Host tests and native 68020 compile
pass. Require a fresh FS-UAE/68030 visual gate before any 68020 timing gate.

The supplied 60 fps radial-burst MOV and explicit user verdict accept that
FS-UAE/68030 HD visual/function gate. Idle, two-field foreground illumination,
radial fragments, Storm Triumph and delayed replay all pass; HUD and rear
palette remain stable. The matching minimal-cadence FS-UAE/68020 HD gate was
then compared with alpha.45's 48.58 FPS result.

That matching FS-UAE/68020 HD minimal-cadence gate is now accepted as well.
The user reports normal visuals/gameplay; the exact drawer log contains 3,244
presentation intervals over about 65 seconds: 3,223 one-field, 21 two-field,
zero three-plus, maximum two fields, 49.67 effective FPS and zero ownership
violations. One Core request produces one Paula start. This protects rather
than resets alpha.45's 48.58-FPS baseline; differing routes prevent treating
the higher figure as an optimization gain. No ADF or hardware claim.

Future candidate order, only after a measured regression or new feature cost:

1. remove an actual Bob job or wait with proven safe CPU/Blitter overlap;
2. coalesce projectile geometry and enemy work rather than add a prepass;
3. add a Fast-RAM mirror only for measured repeated CPU reads from Chip;
4. use coarse hardware-facing scopes if a hardware-only gap returns;
5. do not repeat rejected diamond persistence, pointer-precompute, inline-wait,
   fetch-pruning or tiny-Blitter experiments unchanged.

Research 68020 scheduling, AGA bus arbitration, Blitter behavior and Fast/Chip
placement only for a concrete measured question. Keep any prototype isolated:
first FS-UAE/68030 presentation, then matched FS-UAE/68020 timing. Production
must remain diagnostic-free. A fixed 25 Hz game update with 50 Hz display
service remains a separate last-resort prototype, never uncontrolled skipping.

The diagnostic implementation is compiled out of production already. Split it
from `renderer.c` for auditability, and later split Copper/ring/Bob/sprite
modules, only through small commits whose normal executable remains byte-for-
byte identical. Source-file size and excluded `#ifdef` branches are not FPS
costs. Do not combine this mechanical refactor with a renderer optimization.

Preserve sprites, colours, 4+3 dual playfield and art. Do not retry alpha.37
bounds or alpha.38 full viewport copies. Benchmark candidates in isolation and
gate first on unchanged 68030 presentation, then FS-UAE 68020 timing.

Accepted Stage2 Sprite Stage result: A retains the unconditional
two-channel 1,600-byte Fast-to-Chip sprite image copy. B caches facing/frame per
alternating Chip stage and skips only identical image copies; position/control
words and Copper pointers remain per-update. The cache state-machine host test
and full host suite pass, and actual VBCC 68020 assembly confirms a direct
conditional branch around `CopyMem`. Gate the two self-contained drawers in
FS-UAE/68030 and FS-UAE/68020 testing reports both visually correct. On 68020,
sprite-stage median falls 423 to 74 CIA ticks and average 407 to 236; unmatched
whole-run cadence is 28.73 versus 29.92 FPS and is not attributed wholly to the
cache. B is now production default, with A preserved behind
`SPARKPAW_SPRITE_STAGE_ALWAYS_COPY_REFERENCE`. Overall 68020 cadence remains
rejected; continue with a larger measured hotspot.

Accepted Stage2 canonical-restore result: production rolling targets now keep
only their display bitmaps. Target-local history retains canonical world X as
well as physical ring X, so old Bobs restore directly from the canonical clean
world without changing draw order or bounds. Supplied FS-UAE/68030 and
FS-UAE/68020 HD testing reports the A/B variants visually correct with zero
ownership violations. On 68020, `ring_roll`, `ring_dynamic` and
`bob_compact_target` averages fall by about 39--41%, and the complete Bob pass
falls 26.2% in the supplied runs. Prepared-peak free Chip rises by exactly
319,488 bytes in the A/B gate. The final production-default FS-UAE/68030 log
records 49.95 FPS (1,147 one-field and one two-field interval), zero ownership
violations and 785,872 bytes free Chip at prepared peak; the user reports clean
presentation. Preserve the old architecture behind
`SPARKPAW_TARGET_CLEAN_REFERENCE`. Overall 68020 cadence remains rejected. No
ADF, Analogue Pocket or real-A1200 acceptance is claimed for this Stage2
change.

Accepted Stage2 enemy-state result: loaded runtime enemies are authoritative;
their complete state is copied to persistent spawn storage only when camera
parking occurs. Supplied FS-UAE/68030 and FS-UAE/68020 HD testing reports
normal presentation and correct parking/reactivation. On 68020, `enemies`
average falls 1,860 to 1,762 CIA ticks (-5.3%) and `game_update` average falls
3,865 to 3,707 (-4.1%). Retain this as default and keep the former per-tick
copy behind `SPARKPAW_ENEMY_COPY_EVERY_TICK_REFERENCE`. No ADF, Pocket or
real-hardware acceptance is inferred.

Accepted Stage2 phase-start result: the rolling main loop now starts its next
game update and inactive-target composition immediately after fixed-boundary
publication instead of idling until PAL raster line 100. Supplied FS-UAE/68030
and FS-UAE/68020 HD A/B testing reports both variants visually normal with zero
ownership violations. On 68020, effective cadence rises from 27.45 to 35.81 FPS
(+30.5%); one-field intervals rise from 17.9% to 60.4%, two-field intervals
fall from 853 to 428 and wraps from 875 to 466 under comparable measured work.
Immediate start is the production default; preserve the old gate only behind
`SPARKPAW_UPDATE_LINE100_REFERENCE`. Overall 68020 cadence remains below 50 Hz.
No ADF, Pocket or real-A1200 acceptance is inferred.

Supplied FS-UAE/68030 H3 conclusively rejects target-local collectibles.
Diamonds are stable away from enemies and collection is stable, but enemy Bob
restores erase overlapping target-local diamonds because canonical restore
sources are intentionally diamond-free. Performance no longer favors the
corrected route: `ring_dynamic` is 108 to 1 tick, but collectible restore+draw
rises roughly 544 to 772 and Bob-pass average is 3459 for A versus 3878 for B.
Further overlap invalidation would add redraws. H1/H2/H3 implementation, tests
and build targets are removed; retain canonical synchronization. No MOV,
FS-UAE/68020, ADF, Pocket or real-hardware acceptance is needed or inferred.

Accepted Stage2 hazard-cache result, initial 68030 gate: supplied FS-UAE/68030 HD A/B testing
reports normal presentation, collision, water death/reload and enemy behavior.
Both runs sustain 50.00 FPS. B's `game_update` average is 192 versus 211 CIA
ticks for A, but scene/projectile load is not matched closely enough to claim
that difference. The candidate uses 3 KiB non-Chip BSS; its subsequent 68020
decision is recorded immediately below.

The subsequent supplied FS-UAE/68020 HD A/B test reports both variants normal.
Cadence is effectively identical at 27.07 versus 27.05 FPS under unmatched
projectile load. Scoped results favor the cache: `player` average 922 to 875
ticks (-5.1%), median 756 to 599 (-20.8%), and `enemies` average 1760 to 1748
(-0.7%). Retain the cache as default and the prior scan behind
`SPARKPAW_COLLISION_HAZARD_SCAN_REFERENCE`; classify this as a small CPU win,
not a whole-game FPS gain. No ADF, Pocket or real-hardware claim is inferred.

Rejected Stage2 projectile-sweep result, initial 68030 gate: supplied FS-UAE/68030 HD testing
reports both variants visually and functionally normal at exactly 50.00 FPS.
Both issue 82 shot requests. Candidate B reduces `projectiles` average 20 to 12
CIA ticks, p95 91 to 46 and maximum 174 to 91; A has one additional hit and
kill, so scene load is close but not identical. Its subsequent 68020 rejection
is recorded immediately below.

The subsequent supplied FS-UAE/68020 HD result rejects that projectile-only
enemy prepass. Presentation remains correct, but `projectiles` average is flat
at 362 versus 366 ticks and median regresses 106 to 234. P95 improves 1291 to
847 and maximum 3529 to 1614, so spikes shrink, but ordinary frames pay for the
extra scan and total FPS does not improve. The implementation and build targets
were removed; retain the original pixel-ordered dispatcher. A future swept
candidate must coalesce geometry and enemy work together. No ADF, Pocket or
real-hardware claim is inferred.

Historical Stage2 collectible H1 is visually rejected in supplied FS-UAE/68030
HD testing: target-local diamonds flicker visible/invisible despite zero Copper
ownership violations. H1 did reduce `ring_dynamic` average 91 to 1 CIA tick
and Bob-pass average 3575 to 3081 (-13.8%). Root cause: entering roll columns
can overwrite a target-local diamond while its history still says drawn. H2
invalidated exactly those histories using an exhaustively tested entering-strip
overlap predicate; the subsequent H2 result is recorded below.

Supplied FS-UAE/68030 H2 also rejects target-local collectibles: A is correct,
but B still flickers and trembles between diamond Y positions despite zero
ownership violations. H2 retains a Bob-pass average reduction of about 13.2%.
Cause: the canonical `(frameCounter&3)==(index&3)` stagger is incompatible with
per-target history because alternating targets own opposite tick parities; an
index group can update one buffer but not the other. H3 removes that stagger
only for target-local mode and converges each target whenever its stored hover
Y is stale. H1/H2/H3 were ultimately rejected; H4's accepted replacement is
recorded later in this file.

### 3. Deferred visual work

Only after HD and performance acceptance, replace the world diamond as a
separate art checkpoint: native 16x21 indexed sprite, pen 0 only outside the
silhouette, opaque dark contour/facets and a clean lower point. Preserve Bob
size, mask/cache, hover and draw/restore order. Do not maintain independent HUD
and world raster masters. Author one semantic native 16x21 diamond master;
generate the pixel-perfect world Bob directly from it and stamp the identical
mask/facet-role geometry into the HUD after HUD-source reduction, using only a
documented palette-role mapping. Protect equality with a host regression. See
`sparkpaw/docs/DIAMOND_ART_PLAN.md`.

Workflow lesson: when repeated subjective asset revisions keep comparing two
outputs, challenge whether the pipeline has two competing sources of truth
before producing another variant. Prefer removing the structural comparison
error over blindly iterating the requested symptom.

### 4. Deferred ADF loading optimization

Use `sparkpaw/docs/ADF_LOAD_OPTIMIZATION_PLAN.md`. First measure physical FFS
layout versus runtime file order, then test a byte-identical layout-only ADF.
Consider a sequential ADF-only container only if layout changes are insufficient.

## Documentation rule

Keep this file concise and current. Append implementation narrative and rejected
experiments to `docs/DEVELOPMENT_HISTORY.md`. Update README, handoff, history,
packaged notes and SemVer together for every release candidate.

Focused HD packaging guard: `sparkpaw/tools/stage_hd_test.py` now scans the
selected executable for literal `PROGDIR:assets/runtime/...` dependencies and
automatically adds compile-guarded assets outside alpha.68's release manifest.
This fixes the repeated Stormrail LOADING hang caused by omitting
`stormrail-front.spbm`, `stormrail-rear.spbm`, `stormrail-flight-rear.spbm` and
`stormrail-family.spbm`. Every future focused drawer must additionally boot the
executable and runtime directory from the final `dist` drawer and visibly pass
LOADING; a source-tree proof alone is insufficient.

Stormrail Gate 3 is accepted in supplied FS-UAE/68030 presentation and
stock-FS-UAE/68020 cadence testing. The accepted 68020 log covers 2,108
Flight-only intervals, all one-field (50.00 FPS), with zero ownership
violations and 176/176 player shots started. Four formations, two enemy types,
enemy fire, hit/death/pickup/hurt audio, contact damage, life restart,
anti-farming awards, deterministic free and formation diamonds, compact
20/40/5 scoring, safe lower lanes and half-visible entry hits are accepted.
Do not begin Gate 4 obstacles or expand the route without explicit user intent.

### 2026-09-03 — Stormrail Debris 4 pending visual acceptance

The user rejected `2026-09-03 18-24-56.mov`: Debris3 starts variably but its
tail collapses into monotonous repeated groups and the endpoint is unclear.
The recording is catalogued as
`sparkpaw/testresults/Phase 6D-rejected-debris3-collapsed-repeating-tail.mov`
with a matching sidecar. Audit found a uniform 100-distance admission cadence,
a saturated six-slot pool and an over-weighted 6/29/13 big/shard/pillar mix;
the former test measured source windows rather than simultaneous visible mix.

Debris4 replaces it with one explicit 48-event timeline over distance
5800..11130: eight large, twenty-four shard and sixteen pillar cues, irregular
80..150 spacing, no bounce/reversal and at most one large block active. Two
large loot carriers reserve a slot by retiring only the oldest ordinary
non-large cue if the pool is full, preventing a lost reward or delayed tail.
The visible-snapshot test admits all required families and ends empty.

The compile-guarded production-renderer proof reaches mask `-1/65535`, peak
six / big peak one, zero active obstacles at distance 15200, zero unsafe Blits
and 1,290,992 free Chip bytes. Full host tests pass. The sole active drawer is
`sparkpaw/dist/Storm-Debris4-030-HD`, self-contained with 42 declared assets
and 40 executable-discovered references. It contains no startup-sequence.
Pacing6 and Debris3 are archived intact under `dist/older-builds`; alpha.68 is
untouched. Stop for the user's 68030 visual/feel verdict before any 68020 gate
or further route content.

Debris4 was then rejected with the catalogued recording
`Phase 6D-rejected-debris4-repetitive-field-abrupt-end.mov`. Real runtime
timing exposed the missing invariant: the bitmask rescanner admitted an old
event at distance 15199, immediately before the endpoint. Debris5 replaces it
with one monotone next-event cursor and asymmetric authored phrases. Its native
proof records final event 47 at 11331, last active debris at 11815 and endpoint
15500. Seven actual emulator captures include a clean moving empty-route frame;
all host tests pass. `dist/Storm-Debris5-030-HD` is the sole active drawer and
Debris4 is archived intact. Await the user's 68030 visual/feel verdict.

The user accepts Debris5 as “acceptabel genoeg” and it is preserved intact at
`dist/older-builds/Storm-Debris5-030-HD-accepted-enough`. Life-loss pickup
behavior is already consistent with Level 1: collected diamonds remain consumed
while their HUD remainder and score persist, preventing farming. Audit found
that missing rock drops were a real ID collision: loot IDs 12/13 overlapped
free line 3. Debris5.1 moves them to unique IDs 32/33 using a second persistent
pickup mask. It also applies bounded apparent-randomness polish only: five
offscreen start-X values and event-ID tumble phase instead of pool-slot phase.
The cursor, routes, density, art and endpoint are unchanged. Full tests and the
native 15500-distance proof pass; `dist/Storm-Debris5.1-030-HD` is the sole
active drawer pending the user's 68030 pickup/feel check.

Debris5.2 is accepted for now. It slows the second large loot carrier from -4
to -3 for a readable six-hit reward reveal and removes the
repetitive horizontal middle-lane traffic: only three medium/small horizontal
accents remain, all in upper/lower lanes; the rest are shallow diagonals. The
48-event monotone timeline, density, endpoint and accepted Debris5 fallback are
unchanged. Formation rewards already obey the Level-1-style persistence rule:
formations respawn after life loss, collected reward IDs do not, and only
uncollected members can return.

After accepting Debris5.2 as sufficient for now, the user requested a health
pickup proof. Heart1 adds a native 16x21 classic red heart within the existing
reward pool. Fixed ID 34 appears between early waves at the centre of three
widely spaced ring diamonds (IDs 35..37); the slower large debris
carrier's ID 33 now drops a heart instead of a diamond. Collection restores
two half-heart health units (one full HUD heart), capped at six, and uses the
existing collect sparkle at stronger volume. Both heart IDs persist across a
life restart. User review accepts the final heart silhouette and spacious ring
for now. The complete low-overhead stock-FS-UAE/68020 run records 49.96 FPS
over 2,886 intervals: 2,885 one-field, zero two-field, one three-field (maximum
three), zero ownership violations and 334/334 shots. Preserve archived Debris5
and Debris5.2 as fallbacks. At that checkpoint the sole active drawer was
`dist/Storm-Heart1-Cadence-020-HD`; no release or campaign files changed.

### 2026-09-03 — Stormrail Gate 5A patterns and Gate 4D dust accepted

Gate 5A preserves Debris5.2 and compacts the surrounding route. The accepted
focused slice has eight formations: one added pre-debris Dart/Orb spearhead,
a post-debris Dart curl, a free-diamond slalom with persistent IDs 38..41, a
mixed Dart/Orb crossing-rejoin and a final Dart fan/rejoin. Formation rewards
use persistent IDs 42..57 and no formation exceeds the five-enemy cap. The
route remains monotone on `stormrailDistance`, reaches a short empty reserve
and latches future finale space at 15500. Its cadence run measured 50.00 FPS
over 4,165 intervals, all one-field, zero ownership violations and 438/438
shots.

Gate 4D adds eight independent 16x3 non-colliding dust/grit slots before all
gameplay Bobs. Each has an authored y, phase and speed and crosses right-to-left
from monotone `stormrailDistance`. Slow cached colour selection uses
neutral-white, pale-blue and rare amber pens 9/6/3 as changing sunlight. No
runtime scaling, rotation, randomness, per-pixel work or per-frame allocation
exists. One slot occupies y=18; seven retain the accepted y=41..205 layout. A
y=211 experiment caused HUD/playfield corruption and v4 is archived rejected;
never restore that bound.

MrDig accepts final v5 on FS-UAE/68030 without the v4 HUD glitch. The supplied
stock-FS-UAE/68020 log at
`sparkpaw/dist/Storm-Dust-v5-Cad-020-HD/renderdiag.log` has SHA-256
`2c8d09c056d0ae352020d8475b0c664321249ba5f9fc1b21e6dfca34f975f792`.
It records 49.98 FPS over 6,036 intervals: 6,035 one-field, zero two-field, one
three-field (maximum three), zero ownership violations and 432/432 shots. Gate
4D is accepted. Alpha.68, releases, ADF/multidisk, Level 2 and campaign
integration remain untouched. The next bounded content step is Gate 6: the
specified fixed-camera Harrier, upper/lower gate turrets, gate opening and
automatic passage.

### 2026-09-04 — Stormrail Gate 6 finale pending 68030 acceptance

Gate 6 is implemented only behind the focused Stormrail compile guards. One
data-driven contract and a dedicated host simulation fix the encounter at the
existing monotone `stormrailDistance == 15500` latch: camera and autoscroll
stop, a 96x56 Harrier and independent upper/lower turrets fight with separated
telegraphed patterns, and all three must be destroyed. The final kill clears
the existing hostile-shot pool immediately, a local 48-tick timer opens the
gate, and the Skimmer centres at at most two pixels per tick before its
automatic passage. Route distance never advances during these phases.

The bounded automatic FS-UAE proof lives outside `dist`, under
`sparkpaw/build/fsuae-selftest/stormrail-gate6-greybox-final-20260903-222233`.
It reaches all four lifecycle phases at distance 15500 with zero distance
violations, zero hostile launches after combat, zero unsafe Blits, all three
actors dead, gate timer 48 and final Skimmer y=88. The sole active manual
drawer is `sparkpaw/dist/Storm-Gate6-030-HD`. Its native AGA art pass adds a
storm-purple armoured Harrier with steel facets, cyan cockpit and hot engine,
asymmetric upper/lower turrets and a ribbed energy gate without changing cache
sizes. The inspected renderer proof is under
`sparkpaw/build/fsuae-selftest/stormrail-gate6-aga-art-20260904-092835`.
MrDig rejects this pass as the same cheap, drawn programmer-art failure already
corrected in the debris workflow. It violates `AGA_ART_QUALITY_CONTRACT.md` and
must not be polished incrementally or restored. The intact rejected drawer is
archived as `dist/older-builds/Storm-Gate6-030-HD-rejected-programmer-art`; no
Gate-6 drawer is active. Gate 6 is specifically the Stormrail end fight at the
threshold to **Level 2: Storm Ruins**. Concept v1 for that destination-aware
Harrier/turret/gate composition is saved under `assets/concept/` and remains
review-only. Await explicit concept-direction approval before native reduction
or runtime integration; no Gate-6 68020 build is authorized. Never boot a
drawer from `dist` on Codex's behalf.

MrDig then accepts v1's material direction and v2's corrected flat side-view
composition: a narrow full-height right wall, embedded upper/lower turrets,
central gate, open left/middle dodge space and a Harrier proportionate to the
Skimmer. Native v1 reduces those callouts independently into FRONT16 and cleans
clusters/material edges at final size. Runtime uses the 96x56 Harrier, two
32x24 turrets and two unique 32x104 wall halves; the former procedural art is
gone. The cache increase is 4,160 Chip bytes versus the greybox proof, leaving
1,272,296 free and a 619,976-byte largest block. The automatic proof completes
at distance 15500 with zero unsafe Blits, distance violations or post-combat
fire. Its inspected complete frames retain the line-252 HUD and dark-route
contrast. `dist/Storm-Gate6-Native-v1-030-HD` is the sole active manual drawer,
pending explicit 68030 visual/feel acceptance. No 68020 drawer exists.

Gate-6 native v2 exposed a renderer invariant that must remain protected. The
shared finale actor cache stores every colour plane at the fixed 56-row maximum,
but the generic masked-Bob path advanced planes by each actor's visible height.
That happened to work for the original 56px Harrier and corrupted the reduced
46px Harrier plus both 24px turrets. The corrected finale call supplies the
fixed cache plane stride separately from visible Blit height. The discarded
colour-tweaking attempts were symptoms, not solutions. Complete FS-UAE frames
must now be inspected at early and late encounter times before any drawer is
staged; sheet-only review is insufficient. The selftest's automatic targeting
moves Sparkpaw sharply between targets, so that movement must not be described
as manual-build jitter. Dust movement was not causal.

User-supplied FS-UAE/68030 HD evidence for native v3 is catalogued as
`sparkpaw/testresults/Gate 6-pending-harrier-turret-detail-and-cadence.png`
with a matching TXT sidecar. It confirms coherent actor/HUD shapes after the
plane-stride fix, but Gate-6 visual acceptance remains pending: the Harrier has
too many large near-black interior areas and too little readable surface detail,
while both turrets are too pale and insufficiently colour-distinct. The user
also reports occasionally uneven Sparkpaw movement. A still image cannot
establish FPS, so preserve this as an open later cadence/temporal-evidence check;
do not infer a regression in the accepted Gate-4D 68020 baseline and do not run
the Gate-6 68020 cadence gate before explicit visual acceptance.

Native v4 addresses only that visual feedback. The rebuilt 80x46 Harrier now
uses stable FRONT8 blue-steel panels, ivory structural edges, a violet core and
small amber machinery accents, removing the large unreadable black regions.
The upper 32x24 turret is an ivory/cyan coil-cannon; the lower is a visibly
different ivory/violet fork-cannon. Complete early/mid/late FS-UAE selftest
frames under `sparkpaw/build/fsuae-selftest/stormrail-gate6-detail-v4-*` show
stable actor silhouettes, intact Skimmer/HUD and the intended colour split.
The encounter code, movement, dust, projectile pools and lifecycle were not
changed in this v4 polish. Visual/feel acceptance remains with the user and the
reported smoothness question remains deferred; no Gate-6 68020 cadence gate has
been run.

MrDig rejects native v4's turrets altogether and the blue-heavy Harrier colour
balance; evidence is preserved as
`sparkpaw/testresults/Gate 6-rejected-v4-turrets-and-blue-harrier.png` plus its
sidecar. Gate 6 is consequently superseded by the Harrier-only v5 contract:
one 80x46, 30-HP, 320-point actor (persistent award ID 60), no turrets, and one
20-tick-telegraphed three-shot violet fan every 112 local ticks. The fan uses
the unchanged four-hostile-shot pool atomically and emits fixed dy -2/0/+2 with
dx -4 only when three slots are free. The Harrier patrol expands smoothly to
x -20..+19 and y -28..+27 around base (188,81), while distance remains 15500.
Its art returns to the FRONT16 charcoal/steel ramp with violet and restrained
amber accents. The wall/gate remains unchanged.

The user additionally reports an intermittent subjective smoothness difference:
moving Sparkpaw without firing appeared to drop FPS, while moving and firing
appeared smoother. Record that exact contrast for later temporal/cadence
diagnosis. A still screenshot cannot establish it, and it must not broaden the
current visual gate or trigger the prohibited pre-acceptance Gate-6 68020 run.

Gate-6 Harrier-only v6 follows the approved difficulty direction. Research
favours learned, clearly telegraphed pattern variation and an end-loaded
intensity ramp over a larger HP sponge or unannounced homing fire. The 30-HP
Harrier therefore keeps the fixed fan at phase 32 of a 160-tick cycle. At 15 HP
or below it unlocks a separately warned Hunter Burst at phases 96/108/120;
each dx=-5 shot samples Sparkpaw when launched, clamps dy to -2..+2 and never
homes afterward. Fan and Hunter have 64/72-tick separation and use only the
existing four hostile slots. The gate source shape is unchanged, but its
violet pixels are remapped to muted steel so the formerly turret-covered purple
details no longer distract. The bounded FS-UAE proof completes with seven
hostile launches, distance 15500, zero unsafe Blits, zero distance violations
and zero post-combat fire. Visual/feel acceptance remains pending; no Gate-6
68020 cadence gate has run.

User capture `sparkpaw/testresults/Gate 6-v6-missing-hunter-burst.mov` exposed
a real v6 presentation mismatch: the renderer showed the Hunter warning above
15 HP while gameplay correctly suppressed the locked attack. V7 makes renderer
and game share one `HP <= 15` enable predicate, so a false charge is forbidden.
Fan and Hunter now each own distinct short, preloaded charge/fire cues on the
existing prioritized Paula gameplay voice. Fan fire remains a round violet
pulse; Hunter fire is a narrow amber-white needle; Sparkpaw's cyan shot and the
existing 5/4 projectile pools remain unchanged. The bounded proof under
`sparkpaw/build/fsuae-selftest/stormrail-gate6-hunter-v7-20260904-152810`
reports distance 15500, zero unsafe Blits/distance violations/post-combat fire,
two fan charges/six fan shots and one Hunter charge/shot before the automatic
player kills the Harrier. This verifies the Hunter launch path, not all three
shots surviving a lethal automated volley. Visual/audio/feel acceptance remains
with the user; no Gate-6 68020 cadence run is authorized yet.

Accepted Stage2 collectible H4 result: target-local diamond composition is the
production default. Supplied FS-UAE/68030 and FS-UAE/68020 HD testing reports
normal diamonds including enemy overlap, with zero ownership violations. On
68020, cadence rises from 35.31 to 42.15 FPS (+19.4%), `ring_dynamic` average
falls 3,940 to 101 ticks and Bob-pass average 11,086 to 8,499. Preserve the old
canonical diamond synchronization only behind
`SPARKPAW_COLLECTIBLE_CANONICAL_SYNC_REFERENCE`. Overall stock-68020 cadence
remains below 50 Hz. No ADF, Pocket or real-A1200 claim is inferred.

Accepted Stage2 alpha.45 results: direct traversal lookup raises matched
FS-UAE/68020 cadence 44.35 to 45.09 FPS and lowers `enemy_parked` average 792
to 601 ticks. Hoisted invariant Bob registers leave average cadence flat but
lower Bob p95 17,282 to 14,752. The specialized aligned-16px ring-column route
raises cadence 44.47 to 45.55 FPS and lowers `ring_roll` average 1,691 to 483
and p95 9,878 to 2,782 ticks. All supplied FS-UAE/68030 and FS-UAE/68020 HD
gates report normal presentation. Preserve the former routes behind explicit
reference flags. No ADF, Pocket or real-A1200 claim is inferred.
### 2026-09-04 — Gate 6 stock-68020 overlay-restore candidate pending user cadence

The user's first Gate-6 v12 cadence run looked visibly uneven even though its
minimal log measured 784/784 one-field combat intervals (50.00 FPS) and zero
ownership violations. A targeted split isolated the dynamic Stormrail Bob pass.
The first attempt to cache the whole closed gate was immediately rejected from
whole-frame evidence because dust restores punched horizontal holes in the
resident wall; it was never staged. The corrected compile-guarded candidate
omits only the repeated full-height gate background restore and still redraws
the wall overlay every frame. Its inspected automatic frames have an intact
wall, and the representative targeted Bob pass fell from roughly 220 to 188
raster lines (~15%). The observer-heavy profile is not cadence acceptance.

Exactly one active manual drawer now exists:
`sparkpaw/dist/Storm-G6-v13-Cad-020-HD`, executable `Sparkpaw-G6-Cad`.
The prior v12 drawer and its user log were archived intact below
`sparkpaw/dist/older-builds/Storm-Gate6-v12-Cad-020-HD-user-tested`.
Codex must not boot the dist drawer. The user must repeat the same 15-second
move-without-fire and 15-second move-with-held-fire stock-68020 run and save
`renderdiag.log` with one left-mouse press.

The user additionally made the final integration contract explicit: the full
Stormrail interlude has one initial load only. Distance 15500 must flow directly
into the Harrier encounter with no disk icon, Workbench, or loading screen.
All finale assets and sounds must already be resident. The direct finale test's
long Workbench startup is not the intended level-to-boss transition. Targeted
diagnostics retained 618,024 bytes Chip at the preparation low point and
1,270,312 bytes after run cleanup, so current evidence does not support Chip
exhaustion as the cause of that startup delay.

### 2026-09-04 — Stormrail complete interlude and Gate 6 accepted on 68030

The complete resident interlude flows from accepted boarding and the full
15500-distance route into Gate 6 without a second load. A 32-tick local arrival
slides the narrow full-height Storm Ruins wall and Harrier into the latched
composition. Arrival runs once: life loss resumes COMBAT, preserves remaining
Harrier HP, clears projectiles and restarts the attack cycle safely.

The final encounter is Harrier-only. Its accepted native 80x46 charcoal/steel
art, restrained violet/amber detailing and two distinct attacks replace every
turret iteration. It has 120 HP; Hunter still begins at 60 HP, so the harder
phase lasts longer rather than starting later. The fan launches three violet
lanes and Hunter launches three separately aimed amber-white needles with
distinct heavy cues. The kill awards 320 points once through persistent ID 60,
stops hostile fire, opens the gate and sends the Skimmer through at fixed
distance 15500.

Full2 user evidence exposed replayed arrival and small gate-edge notches.
Full3 preserves boss HP and marks the complete word-aligned Blitter restore
footprint. Full4 adds combat-only body collision: contact costs one half-heart
through the existing 36-tick invulnerability/hurt contract, holds the Skimmer
at the Harrier's left edge even during grace and never damages the boss. MrDig
accepts the complete 68030 flow, visuals, performance and contact. The sole
active drawer is `sparkpaw/dist/Storm-Interlude-Full4-030-HD`; superseded
drawers remain intact in `dist/older-builds`. Never boot it from Codex.

Workflow lesson: MrDig is the authoritative runtime/feel tester. Use host
contracts and native compilation, then stage one focused candidate. Automatic
FS-UAE runs outside `dist` are exceptional and limited to one named native-only
diagnostic that cannot be answered credibly otherwise. Never use them for a
private visual/audio/cadence polish loop after MrDig asks to test personally.
The next task is the post-Harrier interlude results screen, not Level 2,
campaign integration or a release.

Before results work, MrDig requested the matching complete-interlude
stock-68020 performance gate. `Storm-Interlude-Cad-020-HD` is now the sole
active drawer and Full4 is archived intact. Its executable differs only by the
minimal cadence/ownership diagnostic. Sampling starts at established flight
tick 400, includes route, distance-15500 transition, Harrier combat, opening
and automatic exit, then stops at `COMPLETE` so post-finish dwell cannot affect
the result. MrDig must perform this run and save `renderdiag.log` with one
left-mouse press; Codex must not boot it. Results work waits for that evidence.

MrDig completed that exact stock-68020 run. Its 5,177 measured intervals contain
5,175 one-field, one two-field and one three-field interval (maximum three), for
49.97 FPS with zero ownership violations. The run requested and started all
595 player shots and exercised 10 fan charges/fires plus five Hunter charges
and fifteen Hunter fire requests. The 214,228-byte `renderdiag.log` has SHA-256
`fe529b4a9143e992177c85b626f75f7f0a1096cbb19a82ef5c159e36924db460`.
This is effectively equal to Gate 4D's accepted 49.98 FPS and accepts the full
integrated Stormrail interlude on stock FS-UAE/68020 cadence. No ADF, WHDLoad
or real-hardware conclusion follows. The results-screen session may proceed.

MrDig fixes the first post-interlude results presentation to exact Level-1
reuse: the same four visible items (`ENEMIES x20`, `DIAMONDS x5`, `TIME x10`,
total `SCORE`), layout, art, tally order, skip/input behaviour and audio. Do not
create Stormrail-specific results art or add campaign totals yet. Only the
Gate-6 COMPLETE trigger, Stormrail source counters, separately contracted
interlude par-time and temporary test-end behaviour may differ.

The reused screen must retain the real `REPLAY LEVEL` option. After Fire and a
complete black fade it starts a fresh resident Stormrail run at the beginning
of departure/boarding, never Level 1, with no new asset load, disk icon or
Workbench. Reset section score/time, enemies, debris, pickups/award bits,
diamonds, lives/health, shots/input history and the complete Harrier/finale
lifecycle. Do not add `CONTINUE` or rename the prompt to `REPLAY INTERLUDE` in
this isolated step; host-test the reset and no-carry/no-farming invariants.

The first results candidate is now staged as the sole active manual drawer,
`sparkpaw/dist/Storm-Results1-030-HD`, executable `Sparkpaw-Results`. Its
contract fixes a 150-second / 7,500-field complete-interlude par and an
idempotent 0..1,500-point time bonus. Gate-6 `COMPLETE` snapshots Stormrail's
own enemies, diamonds, elapsed fields and live section score; the already-live
320-point award ID 60 is never added by results. The exact Level-1 presenter,
assets, prompt, order, input and tally audio are reused. Results builds retain
the departure sources after the flight handoff so Fire can fade fully black and
perform a resident `gameInit()`/renderer reset back to boarding without a load.
The accepted cadence drawer and its supplied log are archived intact as
`dist/older-builds/Storm-I-Cad-020-accepted`; its launcher is beside it. Host
tests and the normal, integrated and results native builds pass. Visual tally,
audio, input, black-fade replay and feel remain pending MrDig's explicit 68030
acceptance. Do not create a 68020 results gate, release or campaign integration
before that verdict.

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

## 2026-09-23 — Drowned lessons consolidated (documentation only)

See `sparkpaw/docs/DROWNED_TURBINES_LESSONS.md` for reusable art, background-animation, palette, buffer ownership, native Blitter-register and verification lessons. Includes the user's visual acceptance after the mask fix and distinguishes initial staging from subsequent direct-water optimization. Original ambience proposal now links to it. Current campaign/performance acceptance remains owned by CURRENT_STATUS and the engine audit; this documentation pass changed no runtime, assets, builds or dist files.
