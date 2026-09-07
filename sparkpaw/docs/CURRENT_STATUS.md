# Sparkpaw current status and next work

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
