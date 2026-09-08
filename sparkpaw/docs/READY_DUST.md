# READY dust

> Alpha.5 checkpoint (9 September 2026): campaign music plus mixed SFX now
> ships in HD, ADF and WHDLoad. Positive user HD/ADF reports are recorded;
> new WHDLoad audio and final hardware acceptance remain pending.
> See RELEASE_0_7_0_ALPHA_5.md. Earlier candidate/no-release statements below
> are historical and superseded by the explicitly authorized release.


Released in 0.7.0-alpha.3. See CURRENT_STATUS.md and RELEASE_0_7_0_ALPHA_3.md
for current acceptance and open platform gates. Candidate entries below are
historical; their active drawer/pending release statements are superseded.

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
