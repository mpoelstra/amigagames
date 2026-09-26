# Lantern Current v1 — Drowned Turbines music review

User authorized a completely new original melody inspired by the two supplied
Mr. Nutz MP3s, without retaining Undertow v5's melody. This is one new preview,
not a runtime integration. Existing versions, studies and current HD rain assets
are preserved: 220 file hashes recorded and checked in reference-hashes.json.

128 BPM, 64 bars, about 120 seconds. Explicit longer melodic phrases in D minor,
F-major lift and G-minor colour; A-major cadence returns to D minor. Flute-like
lead, marimba answer and rounded pulse return, with continuous bass/percussion
and quiet sampled chords between drum hits. The final cadence has a short
articulated rest, not a breakdown or tempo change. No reference notes/samples
or title material imported. Reuses/adapts original local study synthesis code;
all new samples are generated offline, no runtime synthesis/filter work.

preview.wav: full cycle. short-preview.wav: first 32 seconds.
loop-check.wav: final eight seconds through the actual restart plus eight seconds.
All rendered through the existing micromod pipeline at 48/64, no normalization
or artificial crossfade. Raw and scaled renders checked for clipping; zero
clipped samples, scaled stereo peak -9.24 dBFS. Zero sample jump at restart.
The initial loop check exposed an abrupt tail at restart; authored volume slides
and the pickup rest resolve it within the MOD itself. Listening remains needed.

Measured brightness is lower than both MP3 references, intentionally prioritizing
warmth after earlier shrill feedback. This is not a perceptual matching claim.
The numerical spectral/tonal analysis is in spectral-analysis.json. Codex has
not subjectively auditioned this audio; reference analysis is signal analysis.

Three music channels; channel four is fully empty including effects. Score is
17,468 Fast bytes; sample bank 25,550 Chip bytes (+3,928 versus v5), excluding
player/mixer/allocator overhead. This fits the existing composition approach,
not proof of native 68020 budget or full-game free memory. Integration would
require updating the current exact bank-size checks and checking HD/WHDLoad/ADF
decoded parity, capacity, native replay and SFX balance after user approval.
No changes to the current game, mixer, loader, packages or shared handoff docs.
No emulator, release, commit or push.

Rebuild from repository root with Python + NumPy:
python3 sparkpaw/music/drowned-lantern-current-v1/generate.py
