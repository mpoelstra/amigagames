LEVEL 1 AUDIO - PHASE TRACE v1

First gate: PAL FS-UAE 68030, 2 MB Chip + 8 MB Fast.
Start Level1-Audio. A = original SFX; B = music + block mixer.

Use a similar short route in each, about 45-60 seconds. Shoot and jump,
then deliberately fall into water ONCE in EACH version and let the
respawn finish. Continue briefly after respawn.
Press LMB or Escape to stop, return to Workbench and save renderdiag.log.
Wait until the command finishes before resetting. Leave both logs in place.
After the first 030 trace gate, repeat on 020 with identical settings/route.

This is a targeted timing TRACE, not an audio or gameplay fix. Same content,
controls, music, mixer and IRQ ownership as the previous gameplay proof.
It retains up to 96 long/event records plus 16 short-delta examples.
Four sample points bracket update, drawing and publication; each logs
raw TOD/raster plus water, health, frame reset and publication attempts.
All output is written AFTER stop; no per-frame disk I/O.

The trace has observer cost. Compare phases and events; do not treat its
FPS as an uninstrumented performance baseline. Native cause is unresolved.
No CIA timers are reprogrammed, no extra interrupt handler, no broad profiler.
Production intro/title, Stormrail, transitions and releases are untouched.

B: Copper Sprint + two mixed SFX voices.
