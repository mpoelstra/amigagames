# Sparkpaw audio system direction — 7 September 2026

9 September 2026: the alpha.5 menu/audio extension investigation is recorded in
[AUDIO_OPTIONS_SOUNDTEST_RESEARCH.md](AUDIO_OPTIONS_SOUNDTEST_RESEARCH.md).
User confirms gameplay-only AUDIO MODE directly in OPTIONS for all editions.
HD/WHDLoad adds SOUNDTEST, opening a two-selector SFX TEST / MUSIC TEST submenu.
Both players and existing intro/title/results audio are preserved. Up/Down
selects rows; Left/Right changes values. Implemented HD candidate now staged in
`dist/Audio-Options-030-HD`; host checks pass, user native acceptance pending.

> Alpha.5 checkpoint (9 September 2026): campaign music plus mixed SFX now
> ships in HD, ADF and WHDLoad. Positive user HD/ADF reports are recorded;
> new WHDLoad audio and final hardware acceptance remain pending.
> See RELEASE_0_7_0_ALPHA_5.md. Earlier candidate/no-release statements below
> are historical and superseded by the explicitly authorized release.


Implementation update: user authorized the isolated Level-1 proof. See
[LEVEL1_AUDIO_PROOF.md](LEVEL1_AUDIO_PROOF.md). Intro/title remain four-voice;
each gameplay level should get its own upbeat accompaniment, not reuse title
music. Initial proof uses a bounded two-voice C mixer and ptplayer CIA with
explicit empty-channel isolation; broader production selection remains open.
Historical research-only wording below describes the prior planning boundary.


Research/design only. Baseline 0.7.0-alpha.4, commit 9142b18. The user's follow-up
asks for a broader, better structured and efficient audio system, with reliable
MOD playback independent of dropped game frames. No implementation authorized
in this research turn. This plan supersedes the initial borrowing-first proposal
in INGAME_MUSIC_AUDIT.md; its source inventory and evidence limits remain valid.

## Direction

Prefer **three uninterrupted hardware music voices plus one hardware output
mixing two SFX voices**, driven independently of the game loop. Keep **two music
plus two direct SFX** as the lower-cost fallback. Choose direct MOD or converted replay by verified fidelity, cost and ownership,
not name or format alone. The user explicitly accepts format conversion. ptplayer 6.4 is a strong
candidate, not a preselected replacement. Keep accepted LSP presentation intact
until a replacement passes an isolated fidelity/timing comparison.

Modern here means clear ownership, explicit sample/track contracts, bounded
interrupt work and independently testable interfaces. It does not mean moving
all Paula pitch conversion and mixing onto a general-purpose software engine.
No promise of improved gameplay FPS or successful stock-020 integration yet.

## How Amiga games approach it

Four Paula voices are a physical limit, not the number of sound events a game
may define. A game may choose music or SFX, reserve channels, steal a music
voice briefly, or mix several software voices into one Paula stream.

| Policy | What is actually simultaneous | Consequence |
| --- | --- | --- |
| Music OR effects | Up to four voices for the selected mode | Simple and cheap, but misses this user's goal |
| 3 music + 1 direct SFX | Three instruments and one effect | Music stays intact; shooting and hurt cannot overlap |
| 2 music + 2 direct SFX | Two instruments and two effects | Low CPU and stable effects, sparser musical arrangement |
| 4 music with stealing | Four total hardware voices, reassigned temporarily | Lost music notes, not six simultaneous voices |
| 3 music + 1 mixed SFX output | Three instruments and, for this proposal, two effects | Unbroken music plus effect overlap; paid CPU and output headroom |

Documented game practice: Frank Wille explicitly developed his player for
Sqrxz and Solid Gold to insert game effects into music. That demonstrates an
actual game use case, not a claim that those games use Sparkpaw's proposed
channel policy. [Author readme](https://aminet.net/mus/play/ptplayer.readme).
The modern ACE framework recommends reserved music channels with a separate
SFX mixer to avoid losing music notes. Its C port has different API/volume
semantics from the original assembler player; it is not a drop-in copy.
[ACE audio guide](https://github.com/AmigaPorts/ACE/blob/main/docs/programming/audio.md).
The mixer author's demonstration explicitly combines a three-channel MOD with
several effects on the fourth output. [Author example](https://powerprograms.nl/amiga/audio-mixing.html).
These sources support the design pattern; no unsupported channel-count claims
about other classic games are needed.

Three-channel music must be composed for three channels: lead, bass/harmony,
and rhythm/accompaniment. Alternating instruments on a tracker voice and short
sampled chords can sound rich, but simultaneous independent notes still require
voices or precombined samples. Muting one track of an arbitrary four-track song
is not correct playback. Keep existing four-voice intro/title music as authored.

## A real MOD player versus the audio clock

ptplayer is a direct ProTracker replay implementation based on the original
2.3 routine. Sparkpaw's current player is LSP, which converts MOD events offline.
The current fixed50 conversion is our integration choice, not a requirement
for all MOD players and not a ptplayer workaround. LSP also supports CIA tempo.
Idle silence words/DMA settling address hardware sample lifecycle; they are
separate from the ability to interpret MOD notes and effects correctly.

Paula continues playing an active sample without CPU work. The CPU must still
schedule notes, pitch/volume effects and new mixed buffers on time. A better
MOD interpreter called only after each game frame will still slow with the
frame rate. A CIA service can continue during a slow frame, provided interrupts
are enabled for it and its deadlines are met. Long interrupt masking still
breaks any player. A genuine VBlank interrupt could also decouple 50-Hz music
from rendering, but arbitrary MOD tempo needs appropriate CIA scheduling.

| Replay route | Fit and remaining work |
| --- | --- |
| LSP standard + proper CIA clock | Small predecoded core and existing converted assets; new arbitration/channel isolation needed. No fixed50 requirement. Remains a useful comparison baseline. |
| Frank Wille ptplayer 6.4 | Direct MOD, game-oriented controls, CIA DMA phases, documented mixer combination. Best-supported shortlist candidate; must prove selected MOD effects, channel isolation and OS/takeover. |
| ProTracker 2.3F native replayer | Useful tracker-family reference and alternative direct MOD candidate. Inspect standalone replay integration, licensing and channel controls before selecting; no claim it has a ready-made game SFX engine. |
| P61 family | Optimized converted-module route with CIA/channel configuration; relevant for constrained storage/CPU, but retains conversion and needs integration. Not the clearest match to the request for direct MOD playback. |
| libxmp / general PCM renderer | Broad format compatibility by rendering PCM in software. More format support is not a cheap Paula hardware replayer; software resampling/mixing and output must be budgeted. Not preferred on this 020 gameplay target. |

References: [LSP](https://github.com/arnaud-carre/LSPlayer),
[ptplayer](https://aminet.net/mus/play/ptplayer.readme),
[ProTracker 2.3F source](https://github.com/8bitbubsy/pt23f),
[P6112 source in an Amiga game repository](https://github.com/jonsole/amiga-defender/blob/master/p6112_play.s),
[libxmp project](https://github.com/libxmp/libxmp).
The 2.3F author describes repaired CPU-sensitive delays, which reinforces that
original replay ancestry alone does not guarantee correct hardware timing.
No inspected alternative has yet been measured inside Sparkpaw.

## Conversion and less conventional approaches

User clarification: changing the runtime music format is explicitly in scope
for research. MOD may remain the authoring/master format without being the
runtime format. No converter or new assets have been built here.

| Route | What it buys | What it costs / decision |
| --- | --- | --- |
| MOD -> LSP/P61, tempo preserved | Less live tracker interpretation; compact replay | Still four physical voices. Keep CIA tempo; verify actual effects and loops. Shortlisted alongside direct MOD. |
| Precombine selected drum/bass/chord phrases offline | Several musical parts can occupy one hardware voice, freeing an SFX voice | Additional samples, fixed internal balance/tempo, re-entry and loop seams. Strong bounded composition experiment, not a universal lossless 4-to-3 converter. |
| Precompute synthesized instruments during loading | Small synthesis recipes may replace large disk samples | Startup CPU and generated Chip bank remain; new timbre/workflow and deterministic-generation requirements. A disk-first research branch if capacity blocks sampled tracks. |
| Render full music to mono or stereo PCM | One or two hardware streams leave channels for SFX; fixed musical performance | Very large storage and RAM/streaming dependency. Poor current ADF fit. |
| Decode compressed prerecorded audio from Fast into Chip buffers | Less stored stream data; fixed-pitch output | Decoder CPU, buffers and deadlines, quality loss for lossy codecs. Not free channels or acceptable by default on 020. |
| Mix two music voices plus SFX onto one output | Could retain all four musical parts while two other music voices stay direct | Continuous pitched resampling plus SFX, bus headroom and fixed-side stereo. More expensive than equal-rate SFX-only mixing; reserve for a later measured need. |

For the offline-phrase option, combine rhythm with bass where the arrangement
repeats, keep the lead independent, and deduplicate identical phrases. A 2-second
mono phrase at 11 kHz costs about 22,000 raw bytes; eight distinct phrases cost
176,000. Sampled chords with short loops can be much smaller but sound less like
independent instruments when transposed. This can enrich 2+2 or 3+mixed layouts;
it cannot guarantee unchanged four-channel timbre/stereo at a smaller size.

PCM feasibility arithmetic, excluding metadata: one minute at 11,000 Hz/8-bit
is 660,000 bytes mono or 1,320,000 stereo; at 22,000 Hz it is 1,320,000 mono or
2,640,000 stereo. Even ideal fixed 4-bit coding only halves payload and adds
runtime decoding; this is not a measured codec ratio or a recommendation.
Fast-resident PCM still needs CPU-fed Chip buffers. Streaming from disk would
break today's no-gameplay-I/O/takeover assumptions; streaming from Fast avoids
DOS in play but not disk capacity or refill deadlines. HD-only feasibility does
not establish suitability for the common two-ADF release.

A useful external example of startup sample synthesis is Pink/Abyss' PreTracker
1.6 release note: samples are generated at init and replay uses packed register
updates. This demonstrates storage versus startup-work exchange, not transparent
MOD conversion or extra Paula hardware voices.
[Author release note](https://files.scene.org/view/demos/groups/abyss/ays_plasticdove.zip).
Retro Player/RCM supplies another MOD conversion/CIA design; its author limits
the general conversion model to players without live sample mutation. No game
SFX integration advantage is established over LSP/P61, so do not expand the
implementation shortlist merely because it is another format.
[RCM author documentation](https://github.com/juusu/retro-player).

Revised selection rule: choose the simplest direct or converted backend that
passes musical fidelity, independent timing, strict output ownership and actual
memory/media budgets. LSP without fixed50 is a legitimate contender; changing
format is not itself a sound-quality failure. No format can provide independent
voices unless samples are combined offline, mixed at runtime, or voices omitted.

## Proposed system ownership

Game events -> bounded audio requests -> audio service -> separate music and
SFX backends -> exclusively assigned Paula channels.

- Preserve game-facing event functions such as shot, hurt and Harrier warning.
  Remove direct hardware access from those entry points only in a later approved
  implementation. A small fixed request structure carries event, priority,
  volume and generation. Coalesce replaceable repeated shots, never accumulate
  stale effects; bounded overflow rules must protect critical cues.
- The service alone owns timer/vector/DMA installation, transitions and cleanup.
  Music advances on its tempo clock; mixed output replenishes on audio IRQs.
  Interrupts use preloaded data and bounded work, never DOS, allocation or Blitter.
- Normal mixed SFX design has two virtual lanes: plasma and the existing
  prioritized other-effects lane. Retain existing relative priorities and event
  cooldown behaviour initially; separate frame-based retrigger policy from
  sample-completion ownership. Additional virtual effects are a later choice.
- Reserve one whole Paula output for that mixer. In gameplay the MOD has exactly
  three active voices. A mixer output is on one physical stereo side: a real
  change from today's two-sided effects. Evaluate speakers, headphones and mono
  downmix; do not claim centred SFX. Duplicating it onto the other side costs
  another hardware voice and leaves two music voices.
- Event generation occurs when gameplay discovers the event; independent replay
  cannot remove delay caused by a slow game discovering a collision late.
- Event generation IDs also reject stale pending sounds after stop/replay/load.
  Completion is audio-time based. Stop IRQ/DMA access before freeing any bank.

Final intended modes: presentation can use four music voices; gameplay uses
3+mixed-SFX; SFX-only remains supported for results and fallback. Music-only
and muted modes can share the service without new game logic. This is an
architecture plan, not a request to add options/menu UI now. During transition
between four-voice music and 3+mixed mode, stop old ownership before switching;
never run both writers concurrently on the reserved channel.

## Budget and efficiency

The current fifteen SFX already share period 322 (~11.015 kHz). Mixing just these
two lanes needs no varying-pitch resampler. Keep hardware MOD instruments direct:
their pitches remain handled by Paula. Mixed SFX sources can move to Fast RAM,
with only output buffers in Chip; potentially recover most of the current
45,978 SFX Chip bytes, minus buffers and any intentionally retained direct cues.
This is a source-derived opportunity, not measured peak-memory savings.

Fast mixing reduces input amplitude to allow summing without overflow; for two
full-scale sources, half gain reserves worst-case headroom. That reduces useful
input resolution by roughly a bit. HQ processing preserves normal input data
but does not create extra output bit depth or unlimited headroom. Mixing must
honour the existing 54–64 effect volumes, avoid clipping and preserve the 45-ms
tally and full Core decay. No effect assets are changed during this research.

The AmigaAudioMixer author's period-322 table reports around 0.8–1.0% for
optimized 3–4-voice fast mixing on A1200/14-MHz-020/8-MB-Fast. This is external
configuration-specific evidence, not a Sparkpaw estimate. Its HQ table contains
an anomalous single-four-voice Fast result; do not adopt that number as a budget.
Measure the selected two-voice gain/quality configuration, including IRQ overhead
and interference with music. Default mixer buffering is video-period-sized;
the earlier audit's 256-byte example was illustrative, not this library's
actual buffer contract. [Mixer documentation and tables](https://github.com/DutchRetroGuy/AmigaAudioMixer/blob/main/Documentation/Documentation.md).

Provisional engineering gates for the isolated proof: target <=1 ms total audio
CPU per 20 ms (5%) in the worst tested window, report longest individual service
and zero missed output deadlines. This is an acceptance target, not a forecast.
An integrated game may need a tighter limit near its rendering deadline. Measure
SFX onset separately, aiming <=20 ms after request; report failures rather than
hide them with larger buffers. Exact buffer RAM comes from selected configuration.

Current profiler owns CIA-B Timer B under SPARKPAW_RENDER_DIAGNOSTIC in
platform_amiga.c. A CIA player using it cannot coexist with that profiler.
Use an independently owned measurement source/raster markers for the proof;
do not enable existing broad diagnostics or silently steal the player's timer.
CIA-A keyboard serial acknowledgement must also remain intact.

Music bank size and ADF remain separate gates. Raw title MOD has 233,926 sample
bytes versus 142,796 in its trimmed LSP bank. Direct MOD playback is not inherently
smaller. Preserve original MODs; investigate semantics-preserving trimming/shared
banks only with verification, not forced tempo conversion or loss of instruments.
Disk 1 has only 4,608 bytes free. Neither a new track nor duplicated players are
proved to fit. No new HD compression. Media feasibility must precede promoting
this architecture to a common HD/ADF/WHDLoad implementation.

## Plan, with one first proof

1. **Audio-only comparison, outside production.** Before native implementation,
   inspect the chosen phrase's direct/converted event timing and bank/score
   sizes offline; this narrows the backend choice without another game build. One self-contained future
   proof with identical passage, SFX sequence and workload: direct or tempo-preserving converted
   replay, three protected music voices, two SFX voices mixed on the fourth.
   The SFX mixer candidate is AmigaAudioMixer; music shortlist is ptplayer 6.4
   versus tempo-preserving LSP (P61 only if actual cost/storage warrants it).
   Use the same harness sequentially, not several active game drawers. Compare musical
   behaviour to a trusted native ProTracker replay before selecting the backend.
   Keep current LSP preview/baseline for provenance, not as sole fidelity oracle.
   Exercise tempo changes, vibrato, slides, sample offset, loop/retrigger and
   authored loop/end. A four-channel existing cue is checked separately in
   music-only mode, not mutilated into the three-channel exercise.
2. **Same proof's timing/listening gate.** Compare music-only, SFX-only and both;
   inject 40–60-ms foreground stalls while the audio IRQs remain serviceable.
   Include overlapping plasma/hurt and Harrier warnings, equal/lower-priority
   requests, full Core/tally, stop/restart and exit. No game renderer yet.
   Report CPU, lateness, clipping, onset and Chip/Fast allocations. User 030
   function/listening first, then target 020, then stated real-hardware CPU.
   No routine Codex-run emulator testing. This is the one proposed first proof,
   not several independent staged players or a full-system rewrite.
3. **Only after acceptance, replace ownership plumbing in one game candidate.**
   Keep game-facing calls and all renderer/gameplay/Stormrail content. Establish
   audio interrupts across owned display and OS-live loading. Verify every
   start, resident replay, results, CONTINUE, Escape, title return and F10 exit.
   Compare frame impact solely attributable to audio; general optimization stays
   parked. Verify both free Chip and largest block at transition peaks.
4. **Then consolidate presentation and packaging.** Move intro/title/READY to
   the selected service only after Hero Drive and Neon Sky parity, one-shot end,
   skip/fades and loading continuity pass. Eliminate the second replay backend
   only at that gate. Measure actual FFS growth/readback and each volume's cold
   dependencies before ADF adoption; keep current release files immutable.

If two-SFX mixing fails the 020 quality/cost gate, select 2 music + 2 direct
SFX first. A single SFX lane is possible only with an explicit loss-of-overlap
trade-off. Repeated music stealing is no longer the recommended default for
this user's emphasis on flowing music.

## Evidence and remaining decisions

Source proves current ownership, interrupt masking, SFX sizes/rate, profiler
timer conflict and fixed50 configuration. External author code/docs establish
available methods, not Sparkpaw compatibility. Proposed quality, memory savings,
CPU and frame independence remain hypotheses until native tests. No new native
result, build, asset, package, version, commit or push is produced by this plan.


## Decision — 8 September 2026

User approved pragmatic main-HD-game adoption of the proved Level-1 track and
3+mixed-SFX architecture. Further residual-performance/pickup-admission work
is parked. Implemented candidate and lifecycle evidence: LEVEL1_AUDIO_PROOF.md
("Pragmatic main-game adoption candidate"). Intro/title remain LSP four-channel;
ADF/WHDLoad adoption and full integrated native acceptance remain separate.


## Stormrail music continuation — 9 September 2026

Stormrail music integration — 9 September 2026. User reports the prior
main-game Level-1 candidate “lijkt allemaal goed” after the requested030 test;
no new log or configuration details were supplied. Preserve this general
positive report without inventing individual transition or020/hardware checks.

Main HD now selects Copper Sprint for Level1 and original Iron Horizon for
Stormrail. Iron Horizon:172 BPM,64 bars,~89s loop, three music voices, catchy
returning theme/power-fifth accents, two mixed SFX voices on Paula3. Same
backend/priority/gain/lifecycle; section selection happens only during load.
Only one track is resident. Intro/title/READY and original results tally stay
unchanged. Stormrail departure->flight->Harrier has no new load; life reset
keeps music playing, resident replay restarts it.

Sole active drawer: `Stormrail-Music-030-HD`, executable `Sparkpaw-Music`.
Use READY OPTIONS->STORMRAIL->START GAME for a focused030 audition. No need
to complete Level1 first. No diagnostics, A/B route or LMB log saving.
Normal native build/full host suite pass;56 staged assets match references,
61 release files remain identical. User Stormrail music/feel/native transitions
remain pending. Prior Level1 drawer archived intact. HD only; ADF/WHDLoad,
release/commit/push and general performance work remain out of scope.
See `sparkpaw/docs/STORMRAIL_MUSIC.md` and `STORMRAIL_MUSIC_GAME_TEST.txt`.

## Music campaign ADF — 9 September 2026

Music campaign ADF candidate — 9 September 2026. User reports the Stormrail
HD music “ok gaat goed” after the requested030 audition; no extra machine/log
or individual-transition details supplied. User now authorized an ADF version.

Sole active test set: `dist/Music-Campaign-ADF/Music-Disk1.adf` and
`Music-Disk2.adf`. Ordinary880-KiB FFS disks, PAL A1200/2MB Chip+8MB Fast.
Disk1 has30 blocks/15KiB free; Disk2 has318 blocks/159KiB free. Title/READY
Neon Sky, Level1 Copper Sprint, Stormrail Iron Horizon + mixed SFX; results
original SFX. Like alpha.4 ADF, story intro/Hero Drive omitted.

Disk-only: pack scores/banks/SFX with existing SPR1/SPL1 decoder; externalize
40192-byte READY masks as6423-byte packed data, loaded once into Fast before
READY. No gameplay decompression, no HD compression, no audio/visual changes.
Markers SP07M1/SP07M2 reject mixing these files with older SP07D disks.
Native compile/full host suite/actual C decode+per-disk readback checks pass;
HD executable byte-identical and61 alpha.4 release files preserved. Boot,
DF0 swap/DF1 discovery and audio remain user-native gates; no emulator run.
Use first030, OPTIONS->STORMRAIL for quick Disk2 testing. No A/B/log save.
Prior Stormrail HD drawer and previous multidisk build/evidence archived intact.
No release/commit/push. See `sparkpaw/docs/MUSIC_CAMPAIGN_ADF.md` and
`MUSIC_CAMPAIGN_ADF_TEST.txt`.
