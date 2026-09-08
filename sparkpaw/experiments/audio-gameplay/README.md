# Level-1 audio gameplay candidate v1

This is a direct-start Level-1 ownership gate, not campaign integration.
Production source and release targets are not edited. Build A uses src/audio.c;
B uses the accepted block mixer and ptplayer three-voice playback with the same
16 effect mappings. A/B now means original SFX versus music+mixer, not the former
measured/unmeasured audio-only pair.

`main.c` calls the production gameUpdate -> rendererUpdateGameplay ->
rendererDrawGameplayBobs -> boundary-wait/publish sequence. Same fixed seed,
renderer flags and default secondary-button action in both. It omits all title,
READY, results and campaign routing. The Core scene remains playable until stop.
Reference A is a matched harness, not a byte-identical alpha.4 executable.

`build.py` generates platform_audio.c from the current platform source, with
asserted guarded insertions only. B balances the takeover Disable with Enable
while leaving Forbid in force, enabling only EXTER (CIA-B) and AUD3. No VBlank,
CIA-A keyboard interrupt or OS display handler is enabled. Existing polled
keyboard acknowledgements remain unchanged. On restore the candidate re-enters
Disable before stopping audio and uses the normal platform restore sequence.
A compiles those insertions out.

The OS-compatible ptplayer reserves audio and both CIA-B timers before takeover.
On stop, AbleICR/SetICR quiesce those owned timer sources before saved OS interrupt
masks are restored. AUD3 DMA/IRQ is stopped before resources/buffers are freed.
The vector is restored before uninstalling/freeing audio. This is one start/stop
lifecycle; it does not implement live music switching or restart without reload.

Requests and audioUpdate mask only AUD3 while publishing mixer state. CIA-B
music can continue. Cooldowns remain per game update, matching the production
API; sample durations are now consumed by DMA-buffer service independently of
rendering cadence. The initial zero buffer gives a buffer-boundary onset delay;
no new precise onset measurement or zero-underrun claim is made.

Cadence reads the CIA-A hardware TOD field counter after each publication. No
ReadEClock or CIA-B profile timer runs in this harness. It logs intervals,
1/2/3/other field counts, maximum, prepared free Chip/largest/Fast, camera and
SFX counters. It does not export renderer ownership violation counters or
establish audio CPU cost. Manual route differences and link layout can affect
A/B cadence; compare similar routes and durations. Source/runtime parity is not
native visual acceptance.

Host test uses actual adapter C and mixer with mocked Exec/Paula. It exercises
sample-load and player-install failure cleanup, all 16 mappings, a buffer call,
quiescence, vector/resource release and balanced Disable/Enable. It does not
simulate interrupts, chip-bus contention, Exec scheduling or the display.

Reproduce from repository root:

    python3 sparkpaw/experiments/audio-gameplay/build.py
    python3 sparkpaw/experiments/audio-gameplay/test.py
    .venv/bin/python3 sparkpaw/tests/test_stage_standalone.py

The compiler can require access to /var/tmp for its long multi-source command.
Build manifests are in build/audio-gameplay/build.json; generated source and
objects live there too. Frozen production LSP object is linked for the existing
platform/title APIs but no presentation music is started in this harness.

Use stage_hd_test.py with the ordinary full manifest for both variants and
--additional-runtime-dir build/audio-block-control/assets/runtime for B. That
option supplies only missing candidate files; existing runtime files win and
cannot be overridden. Do not use standalone subset staging for the game.

First user gate: PAL FS-UAE 68030, 2 MB Chip + 8 MB Fast, about two minutes each;
LMB or Escape stops audio, restores Workbench and saves the log. Then 020 after
030 review. No routine automatic emulator testing. Full campaign/READY/results,
Escape-to-menu, resident replay, CONTINUE, Stormrail, WHDLoad/F10 and ADF ownership
remain separate later gates. Disk 1 fit remains unresolved; no HD compression.

## Targeted phase trace

Build with --trace to generate build/audio-gameplay-trace. The active drawers
are Level1-Trace-A/B-030-HD. Four observations bracket update, renderer/Bobs
and publication; raw TOD and coherent raster values remain separate. Bounded
important and short-example pools retain water onset/end, reset, health and
long deltas without ordinary zero/two samples consuming the important pool.
Run test_trace.py for actual-C selection/bounds checks. No audio or clock
ownership change, no renderer timing correction. Observer cost is real; this
is a locator trace, not the uninstrumented cadence baseline.

## Fixed-work comparison

Build --fixed emits build/audio-gameplay-fixed with generated player_fixed.c.
prepare_fixed.py replaces only the playerReadInput hardware boundary, retaining
edge handling and all remaining player source. script.h indexes raw controls
by fixedStep, never time. fixed_main.c runs 1000 real game/render steps, measures
steps520–999 and auto-stops/saves. No trace.c is linked. test_fixed.py runs the
actual gameplay/physics/collision on host twice and asserts matching output,
no reset, target-region coverage and patrol bounds. This does not emulate the
renderer/audio or establish native replay equivalence.

Compare script completion, selected_state_hash, resets, camera bounds and
effect request counts before timing. Whole-script effects include warmup;
interval/readiness counters cover only 480 measured steps. Raw readiness
TOD/beam histogram has four TOD-delta rows (3 includes >=3) and beam bands
0–4,5–31,32–299,300+. No claim of exact visible deadline timestamps.
Active pair Level1-Fixed-A/B-030-HD, executable Level1-Audio; first 030 then020.
Do not steer. LMB aborts and invalidates the full-work comparison.
