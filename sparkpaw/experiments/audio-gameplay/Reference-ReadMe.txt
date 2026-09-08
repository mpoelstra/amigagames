LEVEL 1 AUDIO GAMEPLAY PROOF - v1

First gate: FS-UAE PAL A1200, 68030, 2 MB Chip + 8 MB Fast.
Start Level1-Audio from this drawer. It loads directly into Storm Ruins.
Same game controls and starting seed in A and B.

Play about 2 minutes in each version: follow a similar route, shoot while
jumping, collect diamonds, fight enemies, and try water/damage effects.
Compare controls, graphics, smoothness and effect clarity. In B the music
should continue steadily during busy action. SFX in B are on the left side.

Press LMB once or Escape to stop. This dedicated proof STOPS AUDIO, RETURNS
TO WORKBENCH/SHELL, then saves renderdiag.log in its own drawer. Wait for
the command to finish before resetting. It does not use the normal game's
frozen-screen diagnostic lifecycle. Each variant has its own log.

No intro, title, READY, results or Stormrail transitions in this focused
proof. At the Core, the scene remains playable until you stop; no results.
Their production implementations and release packages are unchanged.
This is not a release or proof of campaign-wide audio ownership.

Logging: hardware CIA-A TOD presentation intervals and effect counters,
not the CIA-B profiler. No per-frame file writes. Human routes may differ:
compare similar workloads; do not interpret different logs as automatic
proof of audio-caused FPS changes. 020 gate follows 030 user review.

A: original production SFX implementation, no music, original masked IRQs.
