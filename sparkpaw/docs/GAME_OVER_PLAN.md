# Sparkpaw game over — proposed implementation plan

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


9 September 2026. Planning only; no runtime, assets, packages or release identity
changed. Baseline: main at a249c98 plus the existing local audio/options/pause,
READY/Soundtest repairs and documentation/artwork changes. Preserve all of them.
Pending native acceptance of those repairs remains pending independently.

## Player experience

Three lives mean three total attempts, including the current attempt. Losing a
life with attempts remaining keeps the current section's accepted respawn rules.
Losing the last attempt ends the run permanently instead of restoring three lives.
This applies to enemy damage, water, dry gaps, Stormrail flight and the Harrier.

Proposed sequence:

1. Finish the short death feedback where already present (especially the water
   splash), with no respawn, further input actions, damage or score awards.
2. Fade gameplay to black through an explicitly safe display transition.
3. Show a dedicated 320x256 Sparkpaw composition with GAME OVER, TOTAL CAMPAIGN
   SCORE and a single BACK TO TITLE action; play a quiet original music loop.
4. Stay until a fresh Fire/Space press confirms. Require release of inherited
   Fire and a short readable entry hold, provisionally 50 PAL fields (1 second).
   No automatic return or countdown. Escape can select the same title return
   after the entry gate; preserve the separate WHDLoad F10 exit.
5. Fade out, clear the ended campaign and return to the large title, followed
   by the existing loading/CHARGING/READY flow. Do not replay the story. A new
   START GAME starts with three lives and zero score. Keep session control and
   gameplay-audio settings; clear pause and old action edges.

If fatal damage and another event coincide, the first latched terminal outcome
owns the update. Do not award or complete a section after fatal loss is latched.
Test this ordering explicitly rather than relying on main-loop branch order.

## Visual direction

An original quiet aftermath scene: Sparkpaw seated on a broken stone ledge,
gauntlet lowered, tired but hopeful, with a distant storm fading into dark blue.
Use the established orange/cream character, restrained cyan light and the
existing stone/metal material language. Keep the middle clear for large readable
GAME OVER lettering, one score and one action. No defeat tally or victory cues.

First deliver a concept and an exact 320x256 layout study for review. Follow
AGA_ART_QUALITY_CONTRACT.md and the imagegen skill when generating new raster
art; record the prompt and approval before runtime conversion. Clean the final
indexed pixels at native size. Target the established six-plane/64-colour
presentation, with palette pen zero pure black. The composition is distinct
from the victory screen, while sharing its display infrastructure where safe.

Start with static artwork and gentle fades. Optional subtle light movement
comes only after the screen and music work well. Precompose text/score into a
hidden buffer once; no live font rasterization, full-screen comparisons or
allocations in the waiting loop. Do not extend or bypass the READY cache for a
screen that needs only one action.

## Campaign score

Freeze an immutable defeat snapshot before any gameInit/reset/unload:

`finalCampaignScore = campaign.bankedScore + game.score`

- Death in Level 1: its earned live score, with no completion time bonus.
- Death in Stormrail after CONTINUE: accepted Level-1 total (including its
  earned completion bonus) plus current Stormrail live score.
- Direct OPTIONS start at Stormrail: zero Level-1 bank, so only Stormrail score.
- Ordinary life loss retains existing score/pickup/award persistence.
- Replaying a completed section retains existing campaign reset/snapshot rules;
  no repeated banking. Defeat awards no new time, enemy, diamond or boss bonus.
- Format the full 32-bit total, rather than reusing the four-digit HUD limit;
  verify the chosen font/layout can show ten digits without clipping.

Existing campaign_contract.h already owns bankedScore and post-Level-1 replay
semantics. Extend that boundary with a small defeat snapshot/helper; do not
construct defeat totals from the success-only StormrailResultsSnapshot.
No persistent high-score file in this first feature.

## Runtime implementation

game.c currently repeats the last-life refill in five locations: enemy damage,
flight health loss, finale health loss, water impact and dry-gap fall. Replace
these with one bounded life-loss decision and a latched terminal flag/outcome.
Nonterminal respawns retain existing timing, progress and Harrier HP rules.
Water must finish its current splash hold without calling resetLevelRuntime
when lives reach zero. Never pass zero lives into helpers that normalize invalid
vitals back to GAME_START_LIVES. Avoid publishing a zero-life gameplay HUD if
its existing atlas cannot represent that value.

Add explicit application handling for defeat in main.c. Route it through a
small game-over presenter/API, reusing low-level title fade/Copper/input helpers
where appropriate. Share the proven BACK TO TITLE sequence with success results
only to the extent needed to keep one correct cleanup/reset path; no broad
main/game/renderer refactor or changes to accepted results menus.

Freeze state before teardown. Quiesce gameplay CIA/audio interrupts and Paula
DMA before freeing music/mixer samples. Finish the outgoing display boundary
before retiring any bitmaps or Copper lists. Black palette alone does not stop
DMA. Preserve the documented Blitter cleanup order on return to title. The
recent retained-menu resume API is for an unchanged visible READY display and
must not be reused for a different screen takeover.

Prefer releasing the ended gameplay renderer/assets before allocating the new
screen and music bank. Determine the exact safe loading/black-display bridge
from title.c/platform_amiga.c; do not assume the success-results path already
fades gameplay or frees all relevant allocations. Measure peak Chip/Fast and
largest free Chip block through the complete transition and repeated new runs.
One 320x256x6 bitmap is 61,440 bytes before Copper/palette/allocator costs;
two are 122,880. Budget actual required buffers, not an assumed free reuse.

## Music and audio ownership

Compose a compact original cue, provisionally 20–30 seconds at 80–100 BPM:
soft synth chords, a small descending melody with a hopeful resolution, light
or absent drums and a clean loop. Offer a listening preview before integration.
These are creative targets, not measured file sizes or an approved composition.

Prefer the existing four-channel LSP presentation backend: no gameplay SFX need
to overlap this screen. Preserve MOD master and reproducible conversion; verify
the converted timing, loop and sample budget. No new replay engine or continuous
software music mixer. Stop gameplay audio before the cue starts; stop/free the
cue before loading Neon Sky. Ensure exactly one clock advances it through fades,
the owned screen loop and any OS-live loading boundary.

Treat game-over music as presentation audio, consistent with the current
gameplay-only AUDIO MODE setting. Thus it plays in all three existing modes,
like title music. Music load failure may fall back to a usable silent screen.
Adding it to HD/WHDLoad Soundtest is optional follow-up; if included, update the
offline state mapping/cache and exhaustive tests under READY_UI_PERFORMANCE_CONTRACT.

## Media feasibility gate

Measure the complete current local ADF build early, before committing to a large
art/sample budget: alpha.5's 15 KiB/159 KiB free Disk 1/2 figures are historical
release measurements, not current local capacity. Include the newer READY cache,
code, asset file headers and filesystem reserve in the accounting.

Game over must be reachable from either disk without an INSERT DISK 1 detour
before the defeat screen. Put required presentation/music files on both current
section disks or prove a bounded preloaded alternative. A single-drive return
to title may then use the existing INSERT DISK 1 loading flow after confirmation.
Check DF1 discovery separately. Keep ordinary two-880-KiB-ADF distribution as
the target; if the measured budget cannot fit, document the concrete options
before changing media count or cutting accepted content.

Use existing lossless disk-only packing; keep HD/WHDLoad loose assets. Update
logical ownership, production manifest, disk resolver/markers if required and
per-volume dependency checks together. No gameplay-time reads/decompression.

## Delivery and acceptance order

1. Capture the local baseline and write focused host tests for all five loss
   paths, terminal-event precedence, splash completion, score snapshot/banking,
   ordinary respawns and held-input behavior. Estimate media/memory from current
   data. This implementation plan is not runtime acceptance.
2. Produce one art concept/native-layout study and one music preview; review
   them before expensive integration/polish. Keep assets compact from the start.
3. Integrate one coherent HD candidate. Run native compilation and the host
   suite, especially campaign/replay, audio lifecycle, pause, READY cache and
   display ownership/failure tests. Do not run conflicting HD/ADF native builds
   concurrently: they share object outputs.
4. Stage one unnumbered self-contained Game-Over-HD test set with the standard
   stage_hd_test.py manifest/hashes. Include a compile-guarded focused launcher
   with one remaining life and controlled earned score for quick defeat review;
   keep shortcuts out of production. Cover both sections and finale without
   requiring repeated full-campaign playthroughs for every visual adjustment.
5. User FS-UAE/68030 review of appearance, music, input and title return first;
   then 68020 screen/music/transition behavior. Also test a normal three-life
   run, a real CONTINUE bank and repeated title/new-game cycles. Do not reopen
   broad performance research or run automatic FS-UAE as the default loop.
6. Validate actual per-disk contents, lossless decode/readback, reserve and
   DF0/DF1 game-over/title return; validate WHDLoad startup/audio/F10 separately.
   Real A1200 and Pocket remain explicit separate acceptance gates.
7. Record the new contract and evidence in campaign/status/handoff/history.
   Release version, packages, release notes, commit and push follow only a
   later shipping request. Preserve existing release bytes and local itch text,
   statistics, release header/art and pending alpha.6 documents throughout.

Done means the last life ends the campaign exactly once, the correct frozen
score is readable, the calm music and clean screen persist until confirmation,
and BACK TO TITLE reliably starts a fresh run without stale state or audio.


## Confirmed quality direction — 9 September 2026

User requires identical top-quality AGA game-over art and music on HD, WHDLoad
and ADF. No fewer colours, simpler instruments or missing music on floppy.
Only intro and Soundtest may be omitted there. Prefer lossless storage savings;
a third disk is preferable to reducing the approved presentation quality.
Implementation authorized by the user ("ok go").

## User-directed layout revision

Game-over layout revision (10 September): user screenshot marks the open
area left of Sparkpaw. Shared static text and dynamic total now center at
native x123; title y35, total label y70, digits y87, instruction y113.
BACK TO TITLE replaced by PRESS FIRE TO CONTINUE; Fire still returns to title.
Outlined small text remains readable over the clouds. Generated coordinate
constants keep the runtime total aligned with the offline preview.
New one-life HD candidate: sparkpaw/dist/GameOver-Centered-030-HD/GameOver-1Life.
Includes Storm Light Soundtest entry. Prior test drawers preserved.


## Open user report — black after Fire (10 September)

User confirms this occurred in the earlier HD game-over test after the screen
was visible for a long time. It predates the ADF compression candidate. Exact
HD executable/section, sound behaviour and duration remain unconfirmed. The
current normal-three-life HD counterpart is staged for comparison alongside
the ADF set. No cause or fix claimed; source inspection confirms the initial
input gate lasts only 50 frames.
