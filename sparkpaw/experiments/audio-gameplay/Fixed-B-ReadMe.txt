FIXED LEVEL-1 AUDIO COMPARISON v1

First gate: PAL FS-UAE A1200 / 68030 / 2 MB Chip + 8 MB Fast.
Start Level1-Audio and LET IT PLAY ITSELF. No joystick/keyboard input.
It executes 1000 fixed game steps, about 20 seconds at full speed,
then stops audio, returns to Workbench and saves renderdiag.log.
LMB aborts early; an aborted run is marked incomplete and is not comparable.

A: original effects. B: Copper Sprint + block mixer.
Same starting seed and fixed movements, jumps and shots. Real game physics,
collisions, enemies, projectiles and renderer. No invulnerability or teleport.
Only steps 520-999 are timed; the opening route warms the real game state.
Host route traverses camera ~892-1133 in that window, without a water reset.
Native route equivalence is checked with a selected-state hash and events.

Watch for sound/visual/control anomalies; do not try to steer. Wait for the
command to finish before reset. Leave each log in its drawer. First 030,
then same pair on 020 after review. Use the same emulator speed settings.

Light logging: fixed-work publication totals, readiness TOD/raster buckets,
retry frames, selected game-state hash and effect counters. No detailed
phase records or CIA-B profiler. Readiness values are raw clock observations,
not exact visible deadline timestamps. Observer cost is shared, not zero.
The state hash excludes elapsed wall time and renderer history; matching
hashes support comparable player/game routes, not a proof of all state bits.

No production changes, renderer boundary fix, campaign/results integration
or release. The previous manual gameplay/trace evidence is preserved.

This drawer: B
