# Copper Sprint standalone audio proof

Authorized after the audio-system research; not production integration.
First user gate: `dist/Audio-Level1-030-HD/Audio-Level1`, PAL FS-UAE/68030.
ReadMe.txt contains the exact 94-second audition/exit instructions. LMB saves
and returns to the OS; this standalone program deliberately does not use the
normal game's frozen diagnostic exit. No emulator was run by Codex.

## Implementation and limits

Original three-voice Level-1 track, 164 BPM, 32 bars (~46.81 seconds), looping.
11,552-byte Chip instrument bank, 9,276-byte Fast MOD header/patterns. Music uses
Paula 0/1/2. Intro Hero Drive and title-through-READY Neon Sky remain untouched
and retain their complete four-voice arrangements. Later levels require their
own music; this is not a proposed shared soundtrack for all levels.

Pristine public-domain ptplayer 6.4 is vendored from the author's Aminet package:
https://aminet.net/mus/play/ptplayer.lha . Its OSCOMPAT CIA timers are used here.
`build.py` generates, without editing upstream, a tightly scoped three-voice
replay variant: channel 4 row data is consumed, but its per-note/effect functions
are skipped. The authored fourth track is asserted entirely empty. This matters
because upstream `mt_channelmask` suppresses sample pointer/length reloads but
is not complete register isolation (e.g. AUDPER still has empty-voice paths).
Init/master-volume/end happen before/after the SFX mixer; no live master-volume
call may touch its channel. Music tick counting is one proof-only counter.
This is hardware ownership adaptation, not a tempo or MOD-event workaround.

The first proof uses a small host-testable C two-voice mixer rather than adopting
the larger configurable AmigaAudioMixer library. Its exact fixed-rate task needs
no pitch plugin, dynamic allocation, callback framework or live gain multiplies.
This is a bounded experimental implementation, not a claim of beating that
library. Production audio backend selection remains open after listening/timing.

Signed source SFX are copied to Fast and scaled once by original volume/128:
original relative gain plus two-voice headroom. Their files are unchanged; no
leading sample bytes are removed. Two 112-byte Chip buffers feed Paula 3 at
period 322. Each audio interrupt fills the nonplaying next buffer. End-of-sample
outputs zero; no sample-head repetition and no music channel stealing. The
health variant duplicates 2,152 Fast bytes for its separate gain and shares the
pickup cooldown. Total SFX Fast sample copies: 48,130 bytes. Program/BSS/OS
allocation overhead are additional, not included in the sample budget.

Event code disables only AUD3 IRQ while publishing voices; CIA music remains
serviceable. Other work uses OS-live interrupts. This does NOT yet prove a safe
transition into Sparkpaw's interrupt-disabled gameplay. It does prove the intended
separation in source, with user native proof pending. Foreground 3-field stalls
are inserted periodically in the effects half; the event script itself is driven
by VBlank. No gameplay update, renderer or timer profiler is linked.

## Full effect inventory

| Event | Runtime sample | Volume / priority / cooldown fields |
| --- | --- | --- |
| Plasma | energy-shot.raw | 60 / dedicated / 0 |
| Hurt / debris contact | player-hurt.raw | 64 / 9 / 16 |
| Enemy or debris hit | enemy-hit.raw | 60 / 6 / 4 |
| Enemy death or debris break | enemy-death.raw | 64 / 8 / 6 |
| Strider / regular Stormrail hostile fire | strider-shot.raw | 64 / 7 / 12 |
| Jump | jump.raw | 58 / 4 / 4 |
| Diamond pickup | collect-spark.raw | 58 / 5 / 3 |
| Health pickup | collect-spark.raw | 64 / 5 / shared pickup cooldown |
| Water fall | water-splash.raw | 64 / 10 / 20 |
| Core / full completion cue | stormstone-core.raw | 64 / 11 / 55 |
| Results tally | tally-tick.raw | 54 / 3 / 1 |
| Extra life / 1UP | extra-life.raw | 62 / 10 / 20 |
| Harrier fan charge | harrier-fan-charge.raw | 64 / 7 / 16 |
| Harrier fan fire | harrier-fan-fire.raw | 64 / 7 / 8 |
| Harrier hunter charge | harrier-hunter-charge.raw | 64 / 7 / 16 |
| Harrier hunter fire | harrier-hunter-fire.raw | 64 / 8 / 6 |

Debris mappings were checked in game.c's obstacle hit/destruction/contact paths;
there is no separately loaded stone sample in alpha.4. The audition includes all
samples, including Stormrail/result extras, without pretending they are Level-1
gameplay events. Equal priority replaces old sounds, lower priority is suppressed.

## Reproduce and check

From repository root, using Python with NumPy for generation/render checks:

```
python3 sparkpaw/music/experiments/level1-pulse/generate.py
python3 sparkpaw/experiments/audio-level1/build.py
python3 sparkpaw/experiments/audio-level1/test.py
.venv/bin/python3 sparkpaw/tests/test_stage_standalone.py
```

The build uses the existing local VBCC/vasm/NDK. Host-only micromod C/header and
BSD license are pinned under third_party/micromod from
https://github.com/martincameron/micromod/tree/master/micromod-c . C source hash
15481720d1ba6c7b023f699ae80fe8a12bcbfe5fb1847243b53ab537c0356a5a
matches the previously used reference library's source. preview_library.py
builds it locally; no temporary external dependency is required. Native proof
uses ptplayer, not micromod. Offline previews are not native listening evidence.

Staging uses tools/stage_hd_test.py's explicit --standalone-runtime mode:
only 17 literal executable runtime references are staged (15 original SFX files,
score and bank). The ordinary game's full-manifest default is preserved and
separately tested. Stage command is recorded in docs/LEVEL1_AUDIO_PROOF.md.

Actual-C sanitizer tests cover tail completion/silence, overlapping samples,
priority/cooldown and replacement. Actual-C scripted replay starts all sixteen
event variants; it records 79 mixed-overlap buffers, 38 completed plasma samples,
29 completed secondary samples and six deliberately suppressed jumps. Tests of
existing presentation-music lifecycle and campaign ownership also pass. These
are host results; native onset, stereo balance, cracks and interrupt behaviour
remain open. The log's raster cost is coarse mixer-only timing and includes
preemption; no claim about full audio CPU percentage or gameplay FPS.

## Block-v1 follow-up

The active renderer uses silence/copy/add spans. Frozen mix_reference.h is
host-test-only. test_mix.c compares complete outputs/state to it, including
randomized operations and boundary tails. Build --timing/--control now emits
build/audio-block-timing and build/audio-block-control. Active staged pair is
Audio-Block-A/B-030-HD; native cost acceptance pending. EClock instrumentation
and the 94-second workload are unchanged from the preserved reference pair.
