# Game over on two ordinary ADFs

## Two-disk compression candidate — 10 September 2026

Two standard ADFs now fit the complete centered game-over scene and unchanged
Storm Light music. Shrinkler reduces the native executable 224,388 -> 89,760
bytes; lossless SPD1 delta+LZ stores Storm Light's sample bank in 72,031 bytes
and Neon Sky's in 70,892. Native data reader: 85 host cases pass, including
full banks and corruption; disk marker/DF0/DF1 source checks pass. Shrinkler
hunk/relocation verification and complete ADF readback pass. Free blocks:
Disk1 73 (36.5 KiB), Disk2 185 (92.5 KiB), both above the 32-block reserve.
New markers SP07G1/SP07G2 prevent mixing with alpha.5 disks.

Only active test: **GameOver-2Disk-030-ADF**, with normal **three lives**.
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


## Implementation update — 10 September 2026

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

The original planning snapshot follows; its statements that no assets or code exist are historical.


9 September 2026. Follow-up to GAME_OVER_PLAN.md. Planning and read-only host
size analysis only. Game-over code, art and music do not exist yet; no new ADF
has been compiled, packaged or runtime-tested in this planning turn.

## Recommendation

Begin implementation with a bounded ADF capacity build, then integrate game
over against its measured budget. Keep the common two-disk edition and the
same game-over experience as HD. Do not begin a broad compression/renderer
research project or build a release before the feature exists.

## Concrete first optimization: ADF-only READY cache

Source inspection finds ready_ui.c unconditionally includes the complete
ready_ui_cache_data.h. Its generator contains 878 states: 2 main, 60 HD options,
48 ADF options and 768 Soundtest states. ADF has no Soundtest and only needs
50 states (main plus its own options). Current ADF compilation does not select
a smaller table.

A read-only host calculation decoded the current delta stream, selected those
50 states, retained their exact band pixels/masks, reconstructed deltas in
existing variant order and applied the current zero/literal packing algorithm.
It counted mappings, four-byte offsets and all-pairs rectangle tables too:

| Quantity | Current shared table | Proposed ADF subset |
| --- | ---: | ---: |
| Packed bands + bounds + mappings + offsets | 77,657 B | 39,627 B |
| Decoded band storage | 275,184 B | 95,648 B |
| Band variants | 77 total | 25 total (3/6/6/7/3) |
| Reachable states represented | 878 | 50 |

Potential main-table saving: 38,030 bytes (37.14 KiB). Potential decoded-cache
reduction: 179,536 bytes (175.33 KiB Fast). Small descriptors, compiler/linker
alignment, revised selection code and filesystem blocks are excluded. These
are reproducible data estimates, not an actual executable/ADF or timing result.
Do not add 37 KiB directly to alpha.5's 15 KiB reserve: the local menu changes
already increased the baseline relative to that release.

Implementation: make the generator emit separate HD and ADF tables/allocation
sizes, selected at compile time; remap the ADF state indices explicitly. Keep
all ADF options, controls, audio modes, artwork and masks byte-identical.
Decode once during CHARGING; preserve per-hidden-target IDs, CPU copies from
Fast RAM and the existing music/dust publication sequence. Never restore live
glyph drawing to save table space. Check all 50 states, all retained band pairs,
skipped target states and generated freshness; keep the HD oracle unchanged.

## Build and measure before further optimization

1. Preserve current local work, hashes of released artifacts and the ordinary
   HD executable. Archive existing build/multidisk-probe evidence intact before
   running tools that overwrite that directory. No dist cleanup is needed for
   this internal capacity probe.
2. Compile the current local ADF baseline and then the ADF-cache subset,
   sequentially using the existing no-intro campaign flags. Record executable
   bytes, source hashes and flags. Native builds share object paths, so never
   overlap HD and ADF builds.
3. Build the actual DOS1/FFS images with the existing packager/decoder. If the
   baseline overflows, retain the failed result and calculate exact block demand
   from payloads, extension blocks, directories and headers; do not lower guards
   merely to obtain a successful status. Report both disks separately.
4. Use the normal 32-block/16-KiB reserve as the target. The alpha.5 explicit
   24-block exception is historical, not a default for future growth. Available
   game-over budget equals actual post-optimization free blocks minus reserve,
   code growth and complete asset filesystem cost.

## Artwork and music within that budget

Keep the planned dedicated 320x256 game-over scene and compact music cue as the
first choice. Measure exact packed candidates early, before visual polish.
Use existing lossless disk-only SPR1/SPL1 packing. LSP already supports the
presentation ownership needed; no new player is justified by this feature.

Author a small dedicated sample bank with short looped instruments and a short
score. Compare its actual stored and decoded size with existing-bank reuse.
Reuse is only a saving when the bank is already resident or avoids a real disk
payload: reloading Neon Sky's 142,796-byte Chip bank is not automatically cheap.
Do not reduce accepted gameplay music or sample fidelity to make room.

If the measured deficit remains, investigate in this order:

1. Packed external storage of the ADF-only READY table using the existing
   decoder, including metadata/file overhead and startup cost. Externalization
   alone saves nothing; compare total disk bytes against its embedded form.
2. Proven unused legacy menu-atlas portions after cache initialization. Trace
   title, main menu, dust and error paths before replacing the asset with a
   smaller generated subset; its presence is not proof of redundancy.
3. A lossless game-over art delta against a deliberately shared presentation
   base, only if the composition remains approved and combined storage is
   measurably smaller. A different illustration may have a poor delta ratio.

Full executable compression, a new codec, removing existing content or a third
disk are fallback design decisions only after these bounded options are measured.
Moving arbitrary files to Disk 2 cannot solve dependencies needed before the
first disk swap. Do not remove collision/audio carryover on naming alone.

## Disk and memory lifecycle

Put required game-over files on BOTH section disks, or explicitly prove a
preloaded shared alternative with its additional memory cost. Defeat on Disk 2
must not request Disk 1 before showing the score and calm music. Confirmation
of BACK TO TITLE may then request Disk 1 through the existing loading display.
Check DF1 discovery and single-drive swaps independently.

Prefer loading defeat assets only after the run is over and gameplay DMA,
interrupts and renderer allocations are safely retired. Preserve a controlled
loading/black display while doing so. This avoids permanently adding artwork
and a music bank to the already constrained gameplay Chip peak. The tradeoff
is a disk read on defeat; measure actual waiting time before promising instant
presentation. Preloading compressed bytes into Fast is a later measured option.

Update logical ownership, physical disk sets, filename aliases, first-use order,
compiled reference coverage and media markers where necessary together. Stop
audio/DMA before freeing its buffers; no filesystem access inside replay IRQs.

## Delivery gates

First deliver a capacity report with actual baseline/subset disk totals and
remaining game-over allowance. Then implement the common gameplay transition,
approved scene and cue, and create one unnumbered HD/ADF candidate set when it
is coherent. No empty mock-up is an accepted game-over build.

Host gates: exact ADF-state pixels/masks; all fatal-loss paths and score/reset
rules; native compile; full relevant suite; actual-C packed decode; complete
per-volume dependency checks and file readback; disk boot/checksum/reserve.

User gates: 030 appearance/music/input first, then 020; defeat in Level 1,
Stormrail flight and finale, held Fire, title return, fresh start, DF0 swap and
DF1 discovery. Physical A1200/Gotek, WHDLoad and Pocket remain separate gates.
No automatic emulator loop or release/version/commit/push in the capacity phase.


## Confirmed quality direction — 9 September 2026

User requires identical top-quality AGA game-over art and music on HD, WHDLoad
and ADF. No fewer colours, simpler instruments or missing music on floppy.
Only intro and Soundtest may be omitted there. Prefer lossless storage savings;
a third disk is preferable to reducing the approved presentation quality.
Implementation authorized by the user ("ok go").
