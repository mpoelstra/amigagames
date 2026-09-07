# Stormbound — three variations on Smooth

2026-09-05. The user prefers Smooth over the first choppy tune and asks for
several more epic, flowing, pumping and recognizably Amiga alternatives.
Smooth is preserved; its relative approval is not final game-music acceptance.

- **A — Stormdrive, 152 BPM:** four-on-the-floor kick (kick/snare combined
  inside the drum sample on beats 2/4), offbeat bass accents and chord response.
  The original connected theme remains prominent. About 75.76 seconds.
- **B — Skyward, 144 BPM:** longer rewritten thematic arcs, several three-beat
  held notes, whole-bar string chords, broader lead and a steady backbeat.
  About 79.93 seconds; the most spacious melodic alternative.
- **C — Paula Power, 150 BPM:** pumping composite drums, brighter odd-harmonic
  lead, more backing arpeggios and selected classic MOD tick-arpeggio effects.
  The lead remains connected. About 76.8 seconds.

All are original four-channel ProTracker MODs, not layered DAW mockups. Each
MP3/WAV is actual micromod replay with the same 65% stereo separation and
0.88-full-scale peak target, constant gain and final short fade. No external
compressor, sidechain, reverb or fifth channel. Musical offbeat accents create
the pumping pattern. Peak matching is not perceptual loudness matching.
Longer original one-shot samples sustain the melodies; every lead C00 cut is
absent. Structural/duration/hash/PCM-peak validation passes. No listening-model
review or real-Amiga playback is claimed; user listening is the musical gate.

See comparison.json and individual manifests for exact sizes/hashes. Module
sizes are approximately 193/242/196 KiB; these expanded listening arrangements
are not optimized Chip budgets. No game integration, memory-test package,
release, commit or push. Existing source/gameplay and earlier tunes untouched.

Reproduce all with create_variations.py using bundled Python with NumPy,
ffmpeg/libmp3lame and the existing micromod library. Each generated folder also
contains a standalone generator. No reference audio/sample data is consumed.
