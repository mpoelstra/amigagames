# Gameplay music and Paula sharing — 7 September 2026

> Alpha.5 checkpoint (9 September 2026): campaign music plus mixed SFX now
> ships in HD, ADF and WHDLoad. Positive user HD/ADF reports are recorded;
> new WHDLoad audio and final hardware acceptance remain pending.
> See RELEASE_0_7_0_ALPHA_5.md. Earlier candidate/no-release statements below
> are historical and superseded by the explicitly authorized release.


The later user-requested system-wide direction in [AUDIO_SYSTEM_PLAN.md](AUDIO_SYSTEM_PLAN.md)
supersedes the borrowing-first recommendation/proof below. Source findings remain
valid; prefer evaluating 3 music + 2 software SFX on one output.

Research only, against clean commit `9142b18` (0.7.0-alpha.4). No player,
asset, build, emulator, release, commit or push changes. The user reopens player
research only for simultaneous gameplay music/SFX; general performance research
stays parked. Target: PAL A1200/68020, 2 MB Chip + 8 MB Fast.

## Evidence boundary

**Source findings** below come from current audio.c/audio_contract.h, music.c,
music_lsp.s, vendored LightSpeedPlayer.asm, platform_amiga.c, game.c, title.c,
actual runtime sample sizes, MOD headers/patterns and the alpha.4 release record.
Upstream ptplayer 6.4 readme and assembly were fetched from the author's Aminet
package and inspected in /tmp, not installed or vendored into the project.
**Design hypotheses** are explicitly proposals, without measured 020 cost.
**Native evidence:** existing presentation-music and one-shot acceptance remains
as recorded in CURRENT_STATUS, TITLE_AND_INTRO_MUSIC and the handoff. There is
no Sparkpaw native evidence for concurrent gameplay music/SFX with any player.
A module preview, library benchmark or source trace cannot supply that evidence.

Consulted contracts: CAMPAIGN_LOOP_CONTRACT, CAMPAIGN_ASSET_OWNERSHIP,
MULTI_ADF_CAMPAIGN_PLAN, STORMRAIL_GATE6_FINALE_CONTRACT, READY_DUST,
CHECKPOINT_ALPHA2_LESSONS, MUSIC_REPLAY_AUDIT and RELEASE_0_7_0_ALPHA_4.
The run-sparkpaw-test-cycle skill informs only the proposed future test boundary.

## What currently owns the hardware

- Hero Drive ends and frees its bank before Neon Sky loads. Neon Sky ends after
  READY fade, before gameplay; gameplay/results are SFX-only.
- audio.c owns channel 0 exclusively for player plasma; channel 1 arbitrates
  all other effects. Channels 2/3 have no gameplay writer. Equal priority
  replaces the older sound, lower priority is rejected while a higher one owns
  channel 1. Per-effect cooldowns also suppress requests; there is no queue.
- Priorities: Core 11; water/extra-life 10; hurt 9; enemy death/Hunter fire 8;
  Strider fire and other Harrier cues 7; hit 6; pickup 5; jump 4; tally 3.
  Protect these identities and priorities, including Harrier warning/fire cues.
- Fifteen actual SFX files total **45,978 Chip bytes**, plus the two-byte silence
  allocation (allocation overhead excluded). Ignore renderbench *.raw files.
  Every SFX uses period 322: 3,546,895 / 322 = approximately 11,015.2 samples/s.
  Plasma lasts about 170 ms, hurt 140 ms, tally 45 ms, Core 1.151 s.
- startOneShot waits two raster-line changes before DMA start and again before
  installing the silence reload word. Duration counters include one guard
  field. Their update follows game/presenter calls, not an independent clock:
  slow game updates may retain ownership longer, but the sample itself plays
  at Paula speed and then reloads silence. Do not regress alpha.66 by restoring
  sample-head looping or shortening the Core cue.
- LSP writes volume, period, pointer/length and DMA state for all four channels.
  Its wrapper stops all four and music.c reserves all four via audio.device.
  An empty MOD track or merely masking the final DMA-on word is not sufficient
  ownership isolation. Init, tick, delayed writes, stop and SFX expiry all count.
- LSP's stream is incremental register events. Dropping writes during an SFX
  loses music state unless a new shadow-state/commit layer continues decoding.
  Restoring old registers cannot reconstruct the elapsed DMA sample position.

## Timing is part of sharing

platformFinishTakeover uses Disable() and clears INTENA, then gameplay polls
hardware. Simply installing a CIA player does not allow it to run there.
Current music uses OS VBlank during presentation/loading and a manual tick in
READY. Reusing one tick per game update would slow music on missed fields.
Bursting missed ticks afterwards cannot restore notes which should have sounded.

The fixed50 stream quantizes non-125-BPM tracker timing: 160 BPM means 64 tracker
ticks/s, represented by 50 updates/s. This is separate from foreground stalls.
The local wrapper also busy-waits 12 scanlines, approximately 0.77 ms per tick,
roughly 3.8% of a 20 ms field before replay work. These are PAL arithmetic,
not measured 68020 utilisation. The known modulo-512 wrap concern remains.

For a future gameplay player prefer a timer-driven audio service independent
of game updates. It needs a narrow interrupt ownership design: CIA-B timers,
level-6 vector/VBR, CPU interrupt mask, INTENA/INTREQ, pending interrupts,
register preservation and balanced OS restoration. Do not blindly Enable()
the OS during gameplay. Serialize main-thread SFX requests with the interrupt;
keep disk I/O, allocation and renderer calls out of it. Existing input, loading,
WHDLoad F10 and exit paths must retain their contracts. This is bounded audio
integration work, not authorization for general renderer/performance research.

## Channel arrangements

Paula has four independent 8-bit DMA voices, physically paired 0/3 and 1/2 for
stereo. A voice can play one sample at one period. Simultaneous music and SFX
need fewer music voices, temporary loss of a voice, or CPU-generated mixed PCM.

| Arrangement | Musical result | SFX / recovery | Main trade-off |
| --- | --- | --- | --- |
| Two music + two reserved SFX | Unbroken melody and bass/harmony; sparse rhythm | Existing 0/1 concurrency retained; no music restoration needed | Safest baseline; compose specifically for two voices, not mute half of Neon Sky |
| Three music + one SFX | Fuller continuous arrangement | Plasma and hurt now compete on one voice | Changes an accepted gameplay-audio property; reject as first choice |
| Two protected music + one borrowable music + one reserved SFX | Melody and harmonic foundation continue; accompaniment drops during effects | Plasma stays on 0; prioritized effects borrow 1; music protected on 2/3 | Preferred hypothesis: three music voices when quiet, two plus two SFX in combat |
| Four music with arbitrary stealing | Fullest music in quiet passages | Repeated shots may repeatedly remove melody/bass | Automatic least-used selection does not know musical importance; unsuitable default |
| Three music + one software-mixed SFX output | Three continuous hardware music voices | Two software voices can preserve plasma plus priority lane | CPU, buffers, mixing headroom/quality and one-sided SFX stereo; later fallback |
| Fully mixed music/SFX | Flexible voice count | Requires resampling and tracker-to-mixer integration | Broadest CPU/quality/complexity cost; not a first 020 proof |

For the preferred arrangement: 0 = plasma only, 1 = short rhythm/arpeggio accents
borrowed by existing prioritized effects, 2 = bass/harmony foundation, 3 = lead.
The melody must work with channel 1 absent for whole combat passages. Short
retriggered accompaniment gives predictable re-entry; never put a long essential
pad or melodic phrase there. Sampled chords/arpeggios can enrich two voices,
but cost sample storage, restrict independent notes and shift timbre when pitched.
This is an authored music design, not extra free Paula channels.

Do not add another automatic SFX allocation policy which accidentally overrides
current priorities. Retain cooldowns/event order initially and the dedicated
plasma lane. Keep music quieter than warnings/hurt, using music-only master
volume where available. Hardware separation guarantees access, not perceptual
clarity: listening must establish balance. Results remain SFX-only initially;
Core, tally and final cues keep full duration. No new result music is proposed.

## Player comparison

**Keep LSP.** Retains accepted presentation assets and small predecoded replay.
For two music voices, add complete register/DMA ownership masking while still
consuming the complete stream. For borrowing, additionally maintain logical
music state and define re-entry at a new note, including loop pointers and
volume/period changes. Standard LSP offers no integrated priority SFX API in the
vendored player. CIA scheduling is possible but its easy upstream driver warns
that it does not restore OS state. Core speed claims exclude Sparkpaw's wrapper,
interrupt integration and new arbitration. LSP is the lowest data-migration cost,
but a custom borrowing engine is a larger correctness burden than its tiny
replay core suggests. Keep it unchanged for intro/title in the first experiment.

**ptplayer 6.4 — preferred gameplay comparison.** Author's public-domain player
already has fixed/automatic SFX channels, priority arbitration, music protection,
channel muting and independent music volume. CIA timers schedule replay and DMA
phases without a busy wait. Use full features, not MINIMAL (which removes SFX).
Patterns can reside in Fast and sample data separately in Chip (_mt_init A1).
It processes tracker commands at runtime, so code/tables and CPU differ from
LSP; no measured Sparkpaw 020 budget exists. OSCOMPAT manages OS-side resources;
it does not solve Sparkpaw's interrupt-disabled takeover automatically.

Source inspection of mt_playvoice/mt_checkfx/start_sfx shows blocked channels
skip notes and most local effects, then unblock after sample completion is
observed. **This is not sample-accurate resume of a sustained music note.** The
song position continues, but the stolen voice can stay silent until an eligible
new note. Author short explicit notes/instruments on that track and test the
longest gap. mt_musicmask protects against automatic selection; fixed requests
must obey our explicit policy too. mt_channelmask is a different control.

Migration hazards: external samples need a zero first word for idle looping;
12 of the 15 Sparkpaw SFX fail that requirement (including plasma and tally).
Do not modify release samples or assume address zero is safe. A future proof
can use separate Chip copies with a prepended zero word and include its two
bytes in the length, preserving the complete original waveform. This adds about
0.182 ms leading silence at period 322; inspect actual player behaviour. Trigger
requests start on service ticks rather than necessarily immediately as today:
measure request-to-start delay and rapid shot replacement. mt_end also stops SFX.

**AmigaAudioMixer (DutchRetroGuy/roondar).** Relevant as a bounded SFX mixer
beside a player, not a MOD-player replacement. Its documented fast mode uses
preprocessed lower-amplitude samples; HQ uses normal 8-bit input at greater CPU
cost. Input may live in Fast; DMA output must be Chip. A music player must avoid
its output channel. The author's A500 throughput numbers are not Sparkpaw 020
measurements. Pin the exact version/configuration before any implementation.
[Author documentation](https://github.com/DutchRetroGuy/AmigaAudioMixer/blob/main/Documentation/Documentation.md).

Our illustrative two-SFX mixer arithmetic: at 11,015 samples/s, two active
sources require about 22,030 input contributions/s plus output writes and IRQ
work. With 256-byte buffers, two buffers need 512 Chip bytes and one buffer lasts
23.24 ms; scheduling/queueing can add latency. Smaller buffers reduce that latency
but increase interrupt frequency. Scaling each input by 1/2 provides worst-case
sum headroom at about one bit less input resolution; saturation instead clips
coincident peaks. Existing differing volumes need preserved gain handling.
Resampling music at independent pitches costs more than adding equal-rate SFX.
A mixer is plausible, not free, and cannot keep four direct music channels plus
SFX unless music also relinquishes or enters a mixed output.

## Memory and ADF feasibility

Current title bank = 142,796 Chip bytes; score = 7,625 Fast bytes. Intro bank =
190,812 Chip; score = 5,752 Fast. These banks currently do not survive into play.
Keeping a music bank changes gameplay/results/replay and cold-transition peaks,
including Stormrail's existing free/largest-block sensitivity. Installed 2 MB
is not 2 MB available. No music bank may overlap a cold section load by accident.
Stop its DMA/interrupt access before freeing; preserve resident replay without
new disk reads. On optional allocation failure prefer the existing SFX-only path.

Raw MODs are not cheap substitutes for converted data: actual neon-sky.mod is
247,298 bytes (12 patterns, 233,926 sample bytes), hero-drive.mod 434,202
(10 patterns, 422,878 sample bytes). Both contain notes on every channel. These
are source-file counts, not playback density or suitability judgments. A new
small track should have its own short looping instruments; trimmed unused tails
and a reusable sample bank can help, but sharing must be built into the loader
and lifetimes, not assumed because timbres sound alike. Keep the current sources
and generators under music intact.

Alpha.4 Disk 1 has **4,608 bytes / nine FFS blocks free**, Disk 2 176,640 bytes.
Executable growth and file headers consume that too. Two new even one-byte
files already need two data and two header blocks in the simple case. Disk 2's
free space is not usable by Disk 1 without a changed, explicitly validated load
plan. Extra swaps to fetch Level-1 music are a user-visible trade-off, not a
transparent solution. A three-track arrangement does not imply a smaller bank.

No current evidence proves a new track/player fits Disk 1. Existing ADF-only
lossless SPL1/SPR1 may be evaluated with exact FFS/readback checks later; no HD
compression, new codec, art removal or release repacking is authorized here.
Keeping LSP for presentation plus ptplayer for gameplay duplicates replay code,
and possibly sample content: budget both. A later unified player might save
this duplication but would reopen all accepted intro/title/READY transitions.
If capacity cannot be recovered within separately agreed scope, defer ADF music
or discuss media choices explicitly rather than silently shipping HD-only parity.

## One small proposed proof — not built

Hypothesis: three authored music voices with protected lead/foundation and one
borrowable accompaniment retain a flowing melody while the existing two SFX
lanes remain intelligible, without software mixing.

Propose one standalone audio-only, unnumbered HD proof using ptplayer 6.4. No game
renderer, campaign changes, replacement presentation player or new ADF. A short
approximately 45-second three-voice phrase with a deliberately small bank
(provisional ceiling 24 KiB Chip, a design budget, not an ADF-fit claim) plays
first alone, then with a deterministic sequence of unchanged-waveform SFX:
rapid plasma plus hurt, Harrier charge/fire/death conflicts, pickup, and a full
Core cue. Include a long accompaniment note and effect-end-between-notes case
to expose restoration limits rather than hide them. Repeat the same passage.

Timer-owned replay must continue through a controlled foreground delay of
40–60 ms. Log only bounded in-memory request/start/completion timestamps,
missed audio deadlines, maximum interrupt duration and allocation totals;
write after stopping/releasing hardware. Check idle-word handling and cleanup
before staging via the test-cycle workflow. Preserve alpha.4 files by inventory.
The controlled delay proves audio independence only, not game integration cost.

Acceptance after a separately authorized implementation: user-run 030 listening
and function first, then PAL 020/2 MB Chip/8 MB Fast timing/listening, then real
A1200 with exact CPU stated. Require no lead interruption, no truncated critical
cue, correct priority, no head-repeat/whine, stable pulse across delay and clean
exit. Provisional latency target: no more than one music service interval plus
DMA staging; report measured worst case, do not silently accept queued lag.
Also report longest accompaniment absence after SFX. If the musical result is
poor, consider two permanently reserved music voices or the bounded two-SFX
mixer next; do not start several candidate engines in parallel.

Standalone success is not Stormrail/gameplay/ADF/WHDLoad acceptance. Only a later
explicit integration step may assess additional frame cost and campaign peak
memory against alpha.4; general performance investigations remain parked.

## Primary references

- [LSP author source/documentation](https://github.com/arnaud-carre/LSPlayer)
  and the local vendored player; no upstream benchmark adopted as native evidence.
- [ptplayer 6.4 author readme](https://aminet.net/mus/play/ptplayer.readme),
  [source package](https://aminet.net/mus/play/ptplayer.lha): inspected version 6.4,
  especially mt_playvoice, mt_checkfx and start_sfx.
- [Paula hardware channel reference](https://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node00D9.html)
  includes correction of the online stereo-side transposition; physical pairs
  remain 0/3 and 1/2.
