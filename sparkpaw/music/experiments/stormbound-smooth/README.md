# Stormbound Smooth — second listening version

User rejected v1 as rhythmically choppy, lacking a flowing melody and groove.
V2 keeps 144 BPM, 48 bars, four real MOD channels and the harmonic structure.
It rewrites the main theme into longer connected phrases, removes every C00
lead cut, lengthens/softens the lead and chord envelopes, regularizes the bass
into eighth notes, retains the backbeat through the middle and reduces fills.
The original v1 is preserved in ../stormbound.

Listen to Stormbound-Smooth-preview.mp3 or the lossless WAV. Both originate
from actual Stormbound-Smooth.mod playback with micromod; no extra backing
track. The generator verifies MOD structure, duration and unsaturated PCM.
The preview retains v1's stereo narrowing, constant gain and final short fade.
No aural model assessment or Amiga runtime acceptance is claimed. User listening
is the musical gate. No game integration, hardware-test package or release.

Reproduce with the bundled Python plus NumPy and existing micromod library;
run generate.py, then optionally encode the resulting WAV using ffmpeg/libmp3lame
at 192 kbit/s. Manifest contains exact MOD/sample size and hash. The larger
sustaining samples have not yet been optimized for Chip memory.
