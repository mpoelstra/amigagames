# Audio options and soundtest — research, 9 September 2026

Baseline: 0.7.0-alpha.5, main commit `a249c98`. The user subsequently
approved implementation and one HD test candidate, without a release or emulator
run. Existing local itch description, statistics and artwork are preserved.

## Soundtest load-return glitch candidate — 9 September 2026

Current test: `dist/Soundtest-Resume-HD/Sparkpaw-Audio`. User reports occasional
brief display glitches when starting a different module; playback then works.
The retained menu now reacquires ownership through a dedicated no-display-write
resume API instead of restarting its Copper list mid-frame. Applied to preview
loads and Neon reload on Soundtest exit. Cache/layout/audio behaviour retained;
see READY_UI_PERFORMANCE_CONTRACT.md for the regression guard and pending
native acceptance. Prior Fast-HD drawer archived intact.

## 020 regression repair — 9 September 2026

The user reports renewed navigation/music/dust stalls on 020. The dynamic
layout implementation below bypassed alpha.3's precomputed difference route.
Replaced it with offline deduplicated band pixels, exact masks and transition
bounds; no live rasterization or image comparisons. Current candidate is
`dist/Audio-Options-Fast-HD/Sparkpaw-Audio`; older layout-5 drawer archived
intact. Full details, memory tradeoff, automated guards and pending native gate:
[READY UI performance contract](READY_UI_PERFORMANCE_CONTRACT.md).
The 72,800-byte dynamic implementation and older drawer references below are
historical. Visual layout/audio/pause behaviour is retained.

## Implemented candidate — awaiting user native evidence

`dist/Audio-Options-030-HD/Sparkpaw-Audio` is the HD candidate, layout revision 5.
User screenshots showed that fixed maximum widths stopped movement but centred
the two pages around different axes. Both pages now share one exact gutter:
labels end at x=151 and values start at x=167, placing the 16px gap around the
320px screen's x=160 centre. Selection arrows are excluded from this geometry.
The setting-row pitch remains 16px. Modes read SFX ONLY / MUSIC ONLY /
SFX + MUSIC.
SOUNDTEST and BACK use the same small font as values, grey-white when
unselected and cyan when selected. Their selection lines are 12px wide and sit
8px from each label. SOUNDTEST starts 20px below AUDIO MODE; its BACK follows
13px later. On the soundtest page the 7px preview hint is vertically centred
between MUSIC TEST and BACK. It is hidden entirely while BACK is selected.
Soundtest shows PRESS FIRE TO PLAY SFX or MUSIC (STOP while playing), followed
by the same selectable BACK action. Harrier display names use FAN/HUNTER
without the redundant prefix so every value and navigation arrow fits.

SFX preview is edge-triggered: every fresh Fire press starts the selected
effect immediately, replacing a still-playing instance if needed. Holding Fire
does not repeat. Its hint therefore remains PRESS FIRE TO PLAY SFX. Music keeps
the explicit Fire play/stop toggle and displays PRESS FIRE TO STOP while active.

Up/Down selects rows, Left/Right changes values. Fire opens SOUNDTEST or returns
via BACK; existing Fire-on-setting return remains available. Soundtest has
two selectors plus BACK; Fire on a selector plays/stops, with full neutral-input
rearming after transitions. Gameplay mode survives replay/section/READY and
resets only on application launch. It excludes intro, menu and results audio.

Explicit previews bypass that mode. Neon is untouched until the first preview,
then suspended; previews are mutually exclusive and silence between them is
intentional. Leaving restores Neon from its beginning, reloading only if Hero
replaced its bank. SFX completion and Hero's once-only completion are detected.
Failed track loading shows TRACK UNAVAILABLE and keeps return available.

LSP and CIA/SFX are stopped before ownership changes. Track reads occur only
after releasing platform takeover; the existing screen stays visible. CIA
music-only previews do not install the AUD3 mixer interrupt. Copper borrows the
preloaded Level-1 score; Iron temporarily loads its score/bank without changing
the chosen starting section. Temporary data is freed only after replay stops.
Gameplay music-only similarly omits AUD3 mixing; FX-only uses original direct
Paula effects. No new asset, compression or authored music changes are involved.

The dynamic UI uses 72,800 Fast bytes, including two target histories and dust
protection. Its 224x104 patch preserves the accepted side-strip pixels, writes
only changed spans, and adds no Chip bitmap. Immutable UI data is 11,793 bytes.
No extra all-track resident set is introduced. SFX banks remain preloaded.
Native 68020 load/cadence and audible ownership transitions remain unproven.
ADF capacity must be checked before an ADF candidate: alpha.5's 15 KiB Disk-1
free space is tight, and this candidate's code/data growth is not free.

Build evidence: full `make test`, HD and WHDLoad native compilation pass. The
HD drawer manifest verifies 56 runtime files and preserves 65 release files.
No ADF candidate has been built or capacity-verified.

Host evidence: actual C UI parity across 830 states/both targets, effect/mode
suppression and persistence, menu input/preview lifecycle mocks, title retain/
restart, CIA music-only and temporary-preview cleanup. These are host checks,
not native audio or visual acceptance. No automatic FS-UAE run. Test details
and evidence boundaries accompany the drawer's ReadMe.

## Historical research below

The following source findings describe the pre-implementation baseline;
statements about missing APIs and proposed work are historical, superseded
by the candidate above.

## Recommendation

User-confirmed scope: put AUDIO MODE directly in the existing OPTIONS screen
in every edition, applying ONLY to Level 1 and Stormrail gameplay. HD/WHDLoad
also gets a SOUNDTEST entry in OPTIONS, opening a submenu with exactly two
selectors: SFX TEST and MUSIC TEST. There is no separate AUDIO submenu.
Keep both existing players and authored music. ADF soundtest is deferred:
optional disk audition needs a new cancellable loading/return path and Hero
Drive is absent from ADF, not waiting on the other disk.

This latest structure supersedes the earlier three-row AUDIO submenu proposal.
Up/Down selects rows and Left/Right changes values, like the existing menus.
Detailed preview behaviour remains a proposal; nothing is implemented or
native-accepted.

## Source findings

- `main.c` preloads Level 1, collision, all effects, its dormant music backend
  and renderer before `titleRunLevelReadyMenu`. Selecting START AT Stormrail
  changes a selector; it does not preload Stormrail until START GAME. Previewing
  Iron Horizon must never call the section-start helper or mutate that selector.
- `title.c:1039` handles press edges, including Fire release on initial entry.
  Up/Down choose START GAME/OPTIONS or the two option rows; Left/Right change
  SECOND BUTTON / START AT; Fire in OPTIONS returns to START GAME. Menu input
  uses primary joystick Fire or keyboard Space, independent of second-button
  gameplay mapping. There is no general submenu or preview state machine.
- READY is a 320x256 six-plane, fixed-palette display. The inspected existing
  campaign-options PNG matches the generator's composition. Menu patches occupy
  x=64..255, y=118..221; the generator uses a compact 5x7 font at six-pixel pitch.
  `readymenu.spbm` contains 12 complete 192x104 patches, 179,916 file bytes.
  `ready_patch.c` precomputes 12x12 row spans (29,952 BSS bytes); current-state
  dust masks total 40,192 bytes. These hardcoded state counts also occur in
  `title.c`, dust code, generator and tests. Adding enum values alone is unsafe.
- `music.c` reserves all four channels through audio.device for the session.
  LSP uses an Exec VBlank server during OS-live presentation/loading and explicit
  `musicOwnedFrame()` calls during exclusive READY display. `musicStop()` stops
  DMA AND frees its score/bank. There is no public retain/restart/suspend API.
  Hero Drive is once-only; Neon Sky loops. The assembly's once-only completion
  clears `lsp_enabled`, but does not clear C's `playing` flag: a soundtest status
  must not infer audible completion from `musicIsPlaying()` alone.
- `level1_audio.c` loads one certified split MOD score/bank plus all scaled
  effects and 2x112-byte Chip mixer buffers. It reserves CIA-B timers during
  preload without touching title Paula playback. Start rejects live LSP music.
  Current start ALWAYS enables the music and mixer together; music-only is a
  small new backend capability, not an existing switch.
- `tools/prepare_game_audio.py` adapts pristine ptplayer: timer-only install,
  no second audio.device allocation, deferred hardware init, caller-owned
  interrupt exclusion, and complete empty-fourth-track register isolation.
  Do not feed four-channel Hero Drive/Neon Sky through this three-voice adapter.
- `audio.c` dispatches requests to the mixer when running, otherwise to original
  direct Paula effects (shot on 0, prioritized others on 1). This fallback
  means stopping music alone does NOT implement music-only or SFX suppression.
  The gameplay-only suppression check must precede both routes, while leaving
  tally/results and explicit preview requests unaffected.
  `audioUpdate()` also needs a deliberately selected active backend.

## Complete runtime effect inventory

Source authority: `audio.h`, `audio.c`, `audio_catalog.h`, `audio_mix.h` and
call sites in `game.c` / `title.c`. Byte sizes below were read from current
runtime files, not estimated from a historic document. All use period 322,
approximately 11,015 signed 8-bit samples/second.

| ID | Suggested label | Source file | Bytes | Volume |
|---:|---|---|---:|---:|
| 0 | PLASMA SHOT | energy-shot.raw | 1,874 | 60 |
| 1 | PLAYER HURT | player-hurt.raw | 1,544 | 64 |
| 2 | ENEMY HIT | enemy-hit.raw | 1,654 | 60 |
| 3 | ENEMY DEATH | enemy-death.raw | 2,646 | 64 |
| 4 | STRIDER SHOT | strider-shot.raw | 2,206 | 64 |
| 5 | JUMP | jump.raw | 2,426 | 58 |
| 6 | DIAMOND PICKUP | collect-spark.raw | 2,152 | 58 |
| 7 | WATER SPLASH | water-splash.raw | 3,528 | 64 |
| 8 | STORMSTONE CORE | stormstone-core.raw | 12,678 | 64 |
| 9 | TALLY TICK | tally-tick.raw | 496 | 54 |
| 10 | EXTRA LIFE | extra-life.raw | 4,962 | 62 |
| 11 | HARRIER FAN CHARGE | harrier-fan-charge.raw | 2,646 | 64 |
| 12 | HARRIER FAN FIRE | harrier-fan-fire.raw | 2,426 | 64 |
| 13 | HARRIER HUNTER CHARGE | harrier-hunter-charge.raw | 2,756 | 64 |
| 14 | HARRIER HUNTER FIRE | harrier-hunter-fire.raw | 1,984 | 64 |
| 15 | HEALTH PICKUP | collect-spark.raw (shared) | 2,152 shared | 64 |

Fifteen unique samples total 45,978 Chip bytes, plus a two-byte silence word.
The existing mixer loads 48,130 Fast bytes, including a separately scaled
health copy. Debris, other enemies and Harrier destruction reuse hit/death/hurt;
there is no additional debris-specific sound. `renderbench-*.raw` files are
bitmap benchmark data, not omitted sound effects. Unshipped composition/SFX
experiments are not part of this catalogue.

## Proposed navigation and preview behaviour

Preserve READY's START GAME/OPTIONS. OPTIONS contains:

- SECOND BUTTON: existing JUMP / FIRE setting.
- START AT: existing STORM RUINS / STORMRAIL setting.
- AUDIO MODE: SOUND FX ONLY / MUSIC ONLY / SOUND FX + MUSIC (default),
  directly editable here in all editions.
- SOUNDTEST: HD/WHDLoad only; Fire opens its submenu.

The SOUNDTEST submenu has exactly two selectors:

- SFX TEST: all 16 effect variants.
- MUSIC TEST: Hero Drive / Neon Sky / Copper Sprint / Iron Horizon.

Up/Down selects rows, with the existing press-edge behaviour; Left/Right cycles
the selected setting/sample/track with wrap. Preserve Fire-return to START GAME
on the editable OPTIONS rows; Fire on SOUNDTEST opens the submenu. In SOUNDTEST,
Fire plays/stops the selected item. ADF retains the first three OPTIONS rows,
with no soundtest entry or disabled test selectors. A joystick-accessible return
control for the two-selector submenu still needs a concrete presentation choice;
do not silently add a third option or overload a normal preview press.

Layout must be revised for four OPTIONS rows on HD/WHDLoad, three on ADF, and
two test selectors on the separate page. Use the existing small font and show
long mode/track/sample values on their own line where needed. The longest effect
name is 125 pixels in the current small font, within the 192-pixel band; this
is text arithmetic, not approval of a complete layout. Keep logo, lower corner
art, palette, dust and hidden-buffer publication intact. No sound on navigation,
no automatic audition on entry/selection changes.

User constraint: the added OPTIONS entries are conditional on a readable fit,
including the existing FIRE: RETURN hint. Moving the menu content upward or
reducing its scale is permitted if necessary, but not yet implemented. Prefer
tighter vertical spacing before shrinking the existing 5x7 small font.

A source-based HD/WHDLoad layout candidate within the existing y=118..221 patch:
OPTIONS heading at y=124; SECOND BUTTON at 146; START AT at 162; AUDIO MODE
label at 178 and its full value at 188; SOUNDTEST at 202; footer at 214
(seven-pixel text ends at 220). The mode value gets its own line because the
current label/value columns cannot accommodate its full wording. This retains
the small font and leaves the logo/corner artwork outside the patch unchanged.
Keep FIRE: RETURN on the ordinary setting rows; the same reserved footer area
shows FIRE: OPEN when SOUNDTEST is selected, so the action is not misleading.
No claim of visually approved fit: inspect a later static native-resolution
layout before integration, including arrows, all mode values and the footer.
If the result is too cramped, adjust spacing/heading first; do not automatically
scale the complete screen or reduce readable text to make the extra row fit.

- Fire starts the selected item from its beginning. A fresh Fire on the same
  playing item stops it (FIRE: PLAY / FIRE: STOP). Moving to another selection
  stops the current preview and requires a fresh Fire to start the next.
- Entering SOUNDTEST or changing AUDIO MODE leaves background Neon Sky playing.
  Only starting an explicit preview temporarily stops it to give that preview
  exclusive Paula ownership. Proposed audition-session behaviour remains silence
  between previews/after Stop, an effect's end, Hero Drive's one-shot end or an
  error; leaving SOUNDTEST restores Neon Sky FROM THE BEGINNING, in every mode.
  The other three music previews loop until stopped. Neon Sky in MUSIC TEST is
  an explicit preview under the same rules. If no preview occurred, leaving
  SOUNDTEST does not restart or otherwise disturb the continuing title song.
  Do not promise sample-accurate resume; the current stop frees LSP data.
- Preview requests explicitly bypass the session mode, without modifying it.
  Thus SFX can be auditioned in MUSIC ONLY and a track in SOUND FX ONLY.
  Only one preview plays; no SFX mixed over four-channel presentation music.
- Seed/latch all inputs on submenu and loading boundaries; require release
  before another press. Consume one action per input sample; a held Fire that
  opens SOUNDTEST cannot also start/stop/restart playback. Discard presses
  made during loading, including the press used to cancel optional disk access.
  Preserve WHDLoad F10 checks at new waits/exits.

Avoid multiplying full-screen variants for all 16x4 item choices and 3 modes.
Prefer bounded row/value patches in Fast RAM and CPU writes to hidden Chip
planes. Track actual content per buffer and rebuild the corresponding dust
mask; an inactive text-union mask recreates the rejected invisible barriers.
Do preprocessing while presentation is visible/OS-live. Do not hand Fast
source pointers to the Blitter or grow the all-pairs table quadratically.
Actual atlas size and 020 menu-copy timing remain implementation measurements.

## Session mode semantics

| Context | SOUND FX ONLY | MUSIC ONLY | SOUND FX + MUSIC (default) |
|---|---|---|---|
| Intro (HD/WHDLoad) | Hero Drive | Hero Drive | Hero Drive |
| Title / loading / CHARGING / READY / OPTIONS | Neon Sky | Neon Sky | Neon Sky |
| Either gameplay section | Existing direct SFX | Section music, no SFX | Existing music + mixed SFX |
| Results | Existing cues | Existing cues | Existing cues |
| Explicit soundtest | Requested item | Requested item | Requested item |

Results retain their existing cues, timing and presentation in every mode;
there is no results music to add. The setting only controls in-game audio:
SFX-only retains visual attack warnings; music-only deliberately silences
in-game audio cues. Explicit previews temporarily replace menu background
music as described above, independently of the setting.

Initialize once at process start. Keep the mode across life/water resets,
resident replay, CONTINUE, direct Stormrail selection, Escape and BACK TO TITLE.
Do not store it in GameState or CampaignState, both of which are reset. Relaunch
returns to the default; no disk preferences. Life reset keeps a playing song's
position; results replay restarts the current section's song as alpha.5 does.

OPTIONS is reached AFTER the first intro/title and Level-1 preload, which is
now immaterial to mode semantics: presentation always keeps its existing music.
Mode edits affect the next gameplay entry, not the currently audible title song.
Returning to title does not replay the intro. No pre-intro selector or launch
setting is needed. Retain loaded gameplay audio on mode edits initially,
avoiding toggle-time reloads and resident-replay changes. Separating stopped
DMA from retained LSP data is needed for the bounded preview proof, not for
changing AUDIO MODE.

For SFX-only, the existing direct backend avoids continuous software mixing;
its panning/gain and game-update-based priority release differ from the mixed
path. This is existing behaviour, not bit-identical mixed output without music.
For music-only, start ptplayer with AUD3 DMA/vector/mixer disabled, and reject
ordinary in-game SFX requests before fallback, leaving results/previews audible. Do not merely mute the mixer's volume:
it would still generate about 98 buffer interrupts/second. Combined mode keeps
alpha.5's accepted path. The start/stop bookkeeping must handle each mode.

## Safe owner transitions and loading

Use an explicit active-owner state, separate from session preference and preview
selection: silent / LSP / direct SFX / ptplayer-only / ptplayer+mixed SFX.
This is a bounded coordinator around existing backends, not a new audio engine.

READY currently holds Forbid AND Disable. Forbid prevents task scheduling;
Disable masks interrupts. Gameplay selectively re-enables EXTER/CIA-B and AUD3
while retaining Forbid. Reusing this for preview requires a matching audio-only
stop that rebalances Disable, without releasing or replacing the READY display.
Do not invoke full `titleRelease()` to satisfy the LSP-live exclusion: that also
frees menu bitmaps/Copper lists. Do not invoke game init or renderer cleanup.

Switch sequence: stop old callback production; mask/clear owned pending IRQs;
stop timer-B's delayed-DMA phase/toggle and timer sources when leaving ptplayer;
stop audio DMA; restore AUD3 vector/filter/masks as appropriate; only then
release or replace audio data. Initialize the new owner under exclusion and
enable only its needed sources. When returning to LSP READY, disable preview
IRQs and return to exactly one polled LSP tick per owned frame. During OS-live
loads only the VBlank server may drive LSP. Never tick it by both paths.

One session audio.device reservation remains shared. ptplayer init/end and LSP
stop can touch all four channels; even a music-only preview needs exclusive
ownership. The old CIA-B renderer profiler cannot coexist with this backend.

For SFX audition prefer the existing original one-shot samples/gains in isolation,
with an explicit preview entry point and fresh preview cooldown/priority state.
Retain silence-word reload and latch waits, service completion in menu frames.
This auditions the source cue; it does not simulate mixed gameplay panning/gain.
No need for continuous mixing for a single solo sample. Music audition uses
the existing LSP versions for Hero/Neon and ptplayer-only for Copper/Iron.

Files cannot be opened in the owned READY loop. Publish a small LOADING AUDIO
status into a valid hidden buffer, quiesce audio, release OS locks through a
carefully bounded `platformReleaseForLoading(TRUE)`-style path, keep that
display alive, load/check the item, then reacquire the same READY display and
reseed input. Dust/input animation can pause during synchronous I/O; no seconds
or seamless-loading claim. Existing release-for-loading is a building block,
not a complete submenu return implementation. Failure must return to an intact
silent soundtest with TRACK UNAVAILABLE / NOT ENOUGH MEMORY, never the game's
fatal section-load flow. BACK remains available; a failed Neon reload returns
to a usable silent menu with a brief status rather than a retry loop.

The existing `level1AudioLoad()` is a singleton/full-backend loader, not a
reentrant track-preview API. Borrow the prepared Copper score/bank read-only
and reinitialize it for real gameplay afterwards. For Iron, use a separate
temporary score/bank descriptor with the same certified-format validation;
never overwrite the pointers for the dormant Level-1 preload. No arbitrary MOD
file browser, duplicate CIA registration or second scaled-SFX set is needed.

## RAM, CPU and media budgets

| Runtime track | Fast score bytes | Chip bank bytes | Shipped availability |
|---|---:|---:|---|
| Hero Drive (LSP) | 5,752 | 190,812 | HD/WHDLoad only |
| Neon Sky (LSP) | 7,625 | 142,796 | HD/WHDLoad, both ADF disks |
| Copper Sprint (split MOD) | 9,276 | 11,552 | HD/WHDLoad, Disk 1 |
| Iron Horizon (split MOD) | 17,468 | 11,552 | HD/WHDLoad, Disk 2 |

All effects are already loaded before OPTIONS and are physically on both ADFs.
Do not add audio payload for HD/WHDLoad: all four converted track pairs already
ship. Additional costs are code/menu data and bounded preview state. Preserve
the prepared Level-1 backend (11,776 Chip; 57,406 Fast in explicit allocations,
excluding code/structures), its legacy SFX and complete renderer allocations.

Prefer at most one temporary preview track plus the dormant prepared Level-1
track. Free stopped Neon before loading Hero: its bank increases this READY
audio peak by 48,016 bytes (~46.9 KiB), rather than adding a full 190,812 bytes
alongside Neon. Free Hero before restoring Neon. Iron costs one temporary
11,552-byte Chip bank / 17,468-byte Fast score; only the requested preview is
resident. The small initial no-I/O proof below may retain already-resident Neon.
No all-tracks cache. Check free AND largest Chip/Fast block at transitions;
2 MB installed Chip and old isolated-game free-memory figures are not proof
that Hero plus today's prepared READY fits every Workbench launch.

Direct SFX previews need no software mixer; ptplayer-only previews should not
run silent AUD3 buffers. Combined gameplay keeps the existing paid mixer cost:
the supplied isolated 020 measurement was 3.06% instrumented service time,
not a worst-case gameplay/menu budget. READY's LSP tick still shares the frame
with menu copies and dust. Restrict evaluation to new menu/owner work; general
gameplay performance investigation stays parked.

ADF alpha.5 has 30/318 free 512-byte blocks (15/159 KiB). The last explicit
24-block reserve leaves only 6 blocks/3 KiB above that reserve on Disk 1.
One additional raw full menu state alone is 14,976 bytes before its mask and
code; packed size is unknown until generated. Audio mode therefore needs a
compact menu extension and a later actual FFS capacity check even without
soundtest. Do not relax capacity guards or change HD compression to force fit.

If ADF soundtest is later requested: show Hero Drive as unavailable in this
edition (never INSERT for it). Show Copper DISK 1 and Iron DISK 2 before Fire.
Use resident data first; otherwise validate SP07M1/SP07M2 in DF0/DF1, auto-select
a present correct disk and prompt only when absent. Add a preview-only cancellable
wait with Fire-to-cancel; preserve the existing noncancellable campaign INSERT
contract. Current `diskMediaRequire()` loops indefinitely and on success calls
`titleShowReplayLoading()`, so it cannot simply be called from soundtest.
`diskMediaOpen()` only uses activeDrive; it does not auto-locate each track.

Neon is duplicated on both disks: after a successful Iron swap, restore it from
Disk 2 without demanding Disk 1 just for menu music. A prepared Level-1 start
can remain resident; do not promise the next cold load never needs Disk 1.
Cancellation must cope with the previous disk already ejected: return safely,
possibly with silent menu music until available, and no unbounded recovery loop.
Missing/corrupt files on a correctly identified disk are errors, not repeated
wrong-disk prompts. Never keep open files/locks across a swap. These extra media
states, UI patches and tests are the main reason to defer ADF soundtest. Default
scope omits SOUNDTEST and its two selectors on ADF and documents these as
HD/WHDLoad-only features. The optional ADF discussion above is deferred research,
not work authorized by the confirmed edition scope.

## One proposed first proof — not built

One isolated HD READY ownership round trip using only already-resident data:
Neon Sky -> Fire -> Copper Sprint music-only -> fresh Fire -> Neon from start.
Keep the current READY image, dust, prepared game, palettes and input. Retain
Neon's stopped bank just for this proof; no disk I/O, Hero, Iron, full catalogue,
ADF, new artwork or mode integration yet. Repeated held-Fire/press/release
cycles and then ordinary START GAME exercise the dangerous owner boundary.

First check host lifecycle invariants (exclusive owner, balanced masking,
timer-B quiescence, no AUD3 mixer in music-only, unchanged allocations and
input edges). After future authorization, compile/stage one unnumbered drawer
using run-sparkpaw-test-cycle; user audition on 030, then 020, with WHDLoad/F10
and real hardware remaining distinct later gates. Observe music tempo, clean
stop/restart, dust/page stability and successful subsequent gameplay. No broad
profiler or routine automatic FS-UAE run. Failure keeps alpha.5 intact.

## Evidence boundary

This investigation inspected source, generators, contracts, existing native
evidence summaries, runtime file lengths and one existing READY preview. It
ran no game, emulator, compiler or test suite. Ownership hazards and inventory
are source findings; layout fit beyond text arithmetic, feature effort, loading
latency and memory sufficiency are proposals/hypotheses. Existing alpha.5 user
HD/ADF reports and earlier 020 mixer evidence support their original scopes;
they do not prove a single soundtest transition. New WHDLoad audio/F10 and final
hardware acceptance remain open as recorded in RELEASE_0_7_0_ALPHA_5.md.
