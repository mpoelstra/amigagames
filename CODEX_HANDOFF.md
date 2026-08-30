# Codex handoff: Amiga game workspace

Last updated: 30 August 2026

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

Target: PAL A1200/AGA. The supplied real A1200/68030 at about 34.5 MHz may be
the minimum release CPU if stock-68020 50 Hz cannot be reached without visual
or gameplay concessions; FS-UAE/68020 remains the optimization stress target.
Memory minimum remains 2 MB Chip plus 8 MB Fast RAM.

From `sparkpaw/` always run after implementation:

```sh
make PYTHON=../.venv/bin/python3
make release PYTHON=../.venv/bin/python3
```

Current release is `0.6.0-alpha.68`, Phase 6C.10. A normal release contains:

- `Sparkpaw-0.6.0-alpha.68.lha`
- `Sparkpaw-0.6.0-alpha.68.zip`
- `Sparkpaw-0.6.0-alpha.68.adf`
- `Sparkpaw-0.6.0-alpha.68-WHDLoad.lha`
- `Sparkpaw-0.6.0-alpha.68-WHDLoad.zip`
- extracted review drawer `Sparkpaw-0.6.0-alpha.68/`

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
and real-hardware runtime acceptance remain pending.
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
- Sprint/jump performance is open; do not change physics before measuring it.

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

The supplied 60 fps radial-burst MOV and explicit user verdict now accept that
FS-UAE/68030 HD visual/function gate. Idle, two-field foreground illumination,
radial fragments, Storm Triumph and delayed replay all pass; HUD and rear
palette remain stable. The active next drawer is the matching minimal-cadence
FS-UAE/68020 HD gate. It must be compared with alpha.45's 48.58 FPS result and
must show no three-field misses or ownership violations.

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
