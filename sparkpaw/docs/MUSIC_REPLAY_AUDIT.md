# Music replay audit — 7 September 2026

> Alpha.5 checkpoint (9 September 2026): campaign music plus mixed SFX now
> ships in HD, ADF and WHDLoad. Positive user HD/ADF reports are recorded;
> new WHDLoad audio and final hardware acceptance remain pending.
> See RELEASE_0_7_0_ALPHA_5.md. Earlier candidate/no-release statements below
> are historical and superseded by the explicitly authorized release.


Update, 7 September 2026: the user reopens this research for gameplay music/SFX
and then the whole audio system. See [AUDIO_SYSTEM_PLAN.md](AUDIO_SYSTEM_PLAN.md)
for the current proposed direction and [INGAME_MUSIC_AUDIT.md](INGAME_MUSIC_AUDIT.md)
for source findings. No presentation player or release changes. Historical
parked/candidate statements below do not override this research-only scope.

User tentatively reports music disturbance/skipping and requests an explanation
and alternatives. No recording, timestamp or exact phase/medium supplied yet.
This is a read-only runtime audit; active HD/ADF test bytes are preserved.

## Current implementation

LightSpeedPlayer v1.31 standard replay, derived from the existing Futsal wrapper.
The original MOD is converted with -shrink -fixed50hz into a score and sample
bank. No ADPCM is used. ADF envelopes are lossless and decoded before replay.
Paula reads the 8-bit samples directly from Chip; score is in Fast memory.
Hero Drive intro is one-shot, Neon Sky title/LOADING/CHARGING/READY loops.
Gameplay and results are SFX only. Original MP3 previews use micromod, while
runtime uses LSP: a good MP3 alone does not verify runtime timing or integration.

With OS alive, an Exec VBlank server ticks at 50 Hz, including during DOS I/O.
At READY platformFinishTakeover disables interrupts. Title's beam-synchronised
wait then calls musicOwnedFrame manually, preserving existing audio DMA bits.
The wrapper busy-waits 12 raster lines after LSP tick before applying DMACON.
Stop disables replay and audio DMA before freeing sample/score allocations.

## Findings and uncertainty

1. Fixed50 conversion quantizes timing. A 160-BPM CIA module normally has
   64 tracker ticks/second (BPM/2.5); 164 BPM gives 65.6. Our converted stream
   updates at 50 Hz. This retains average pace approximately but is not exact
   tick timing, including pitch effects. Upstream LSPEncoder::IsEmulatedBpmTick
   and SetBPM confirm the emulation. This is a fidelity limitation, not proof
   of the user's particular perceived skip.
2. READY replay depends on the foreground loop. If it crosses a PAL boundary
   doing work, the next wait advances just one music frame, without elapsed-time
   recovery. OS-to-polled takeover also changes the tick phase. Source proves
   these risks; no missed frame or audible fault has been measured.
3. The VBlank wrapper holds CPU/interrupt service for a 12-line DMA wait
   (~0.77 ms PAL), even when a tick does not restart a note. The wait's modulo-512
   subtraction is not the actual PAL frame modulus: crossing the PAL frame end
   can satisfy it too early. Normal READY calls start near frame start; delayed
   OS interrupt onset near frame end is a contingent concern, not an observed
   cause. A timer/Copper-driven DMA latch avoids this busy wait and wrap issue.
4. CPU stalls with interrupts enabled do not automatically halt VBlank replay.
   Disk load alone is not evidence of starvation. Long interrupt masking can
   delay service; any player needs correct ownership under both OS and takeover.
5. Sample timbre, preview gain/panning/filter/emulation and replay scheduling
   are different questions. Existing successful host decoding/PCM checks cannot
   rule out native scheduling or Paula register timing faults.

## Alternatives

- Keep LSP, use proper tempo-driven CIA scheduling: smallest conceptual player
  change and retained converted data approach. Reconvert without fixed50hz,
  preserve tempo changes, and safely integrate interrupt/DMA ownership. The
  upstream easy CIA driver explicitly does not restore OS state; do not paste
  it into DOS-loading Sparkpaw. It also cannot run while all interrupts remain
  disabled. Need a deliberate consistent presentation interrupt model.
- Frank Wille's ptplayer 6.4: native ProTracker replay, CIA timing, music volume
  and optional game SFX support. OSCOMPAT registers CIA interrupts and allocates
  audio.device channels for OS-live operation. Default hardware takeover mode
  is not interchangeable. Strong comparison candidate for original-MOD fidelity
  and later unified SFX ownership; not an automatic audio-quality guarantee.
- ptplayer VBlank/NO_TIMERS: supports explicit external calling but does not
  solve our frame dependence. Its documentation requires separate timed music,
  DMA-enable and repeat-pointer phases; Fxx tempo is restricted in VBlank mode.
- Software mixing/extra channels: unnecessary for current presentation-only
  four-channel music. For future gameplay music, define Paula channel ownership
  (reserved channels or controlled SFX takeover) first; simultaneous independent
  four-channel music and current SFX writers would conflict.

## Recommended next bounded step

First locate the report by medium, phase and timestamp. Compare the original
MOD render, converted LSP simulated replay and supplied native capture of the
same passage. If only READY/transition differs, inspect scheduling before
rewriting instruments. For a later implementation, prefer one consistent
CIA-timed presentation playback path, with an isolated LSP-vs-ptplayer comparison
if needed. Do not create a second full active test set or infer user approval
for a player replacement from this research request. No FS-UAE run performed.

## Primary sources

- https://github.com/arnaud-carre/LSPlayer — v1.31 modes, conversion and CIA option.
- https://github.com/arnaud-carre/LSPlayer/blob/main/LightSpeedPlayer_cia.asm —
  timing/DMA driver and explicit missing OS restoration.
- https://aminet.net/package/mus/play/ptplayer — author's v6.4 readme, OSCOMPAT,
  CIA, VBlank/NO_TIMERS, SFX support and timing obligations.
- Local LSPEncoder.cpp in mrdigs-futsal/third_party/LSPlayer/src: fixed50 emulation.
- Local src/music.c, src/music_lsp.s, src/title.c and src/platform_amiga.c:
  actual Sparkpaw lifecycle and timing, not assumed library behaviour.
