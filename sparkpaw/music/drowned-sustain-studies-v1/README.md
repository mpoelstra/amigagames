# Sustained Drowned studies v1 — preview only

User rejected Lantern Current's choppy/piano-like articulation and requested
several stronger Amiga-style variants with longer melodic lines/notes.

These three original proposals replace the rapidly decaying melody samples
with signed 8-bit looped instruments. Two-bar phrases contain 3–4 notes,
with selected legato portamento and delayed subtle ProTracker vibrato.
Harmony changes every two bars. Drums and bass continue in all sections.
No Mr. Nutz/title notes or audio extracted or used. Original local drum synthesis
from the preserved Lantern generator is reused; leads and bass are new.

| Proposal | Character | BPM | Duration | Mean / longest lead interval |
| --- | --- | ---: | ---: | ---: |
| Turbine Heart | Broad brass-synth hook, ensemble answer | 132 | 116.33 s | 1.08 / 1.82 s |
| Deepwater Run | Sustained ensemble, most spacious melody | 124 | 123.86 s | 1.29 / 2.42 s |
| Iron Tide | Rounded pulse lead, firmer kick drive | 140 | 109.64 s | 1.05 / 1.71 s |

Each folder contains short-preview.wav (40 seconds), preview.wav (full), and
loop-check.wav (eight seconds before/after actual restart), plus MOD,
rain-score.bin, rain-bank.bin, manifest and numerical spectral analysis.

Host checks: sample lengths/loop bounds and loop seam against ordinary waveform
steps; wholly empty channel four including effects; fixed 64 bars and measured
duration; no clipping before or after 48/64 preview gain; zero sample jump at
restart. End cadence has an authored short release/rest, no host crossfade.
All three peak at about -10.1 dBFS. This is existing micromod host rendering,
not native ptplayer evidence or subjective listening by Codex. Spectral
measurements do not guarantee absence of perceived harshness; user audition
is the approval gate. No loudness normalization or reference audio processing.

Per study: 17,468 Fast score bytes and 31,942 Chip sample bytes (+10,320 versus
v5); allocator/mixer/player overhead additional. Three Paula music channels,
fourth reserved for the unchanged SFX mixer. Synthesis/chorus is baked offline.
No runtime integration or claim of measured 020 performance, memory headroom,
SFX balance, ADF capacity or media acceptance. Loader exact-size changes and
HD/WHDLoad/ADF parity checks remain gated on selection and approval.

Existing 220 music/reference/current-HD hashes still match the earlier
reference-hashes.json; previous previews and shared source/docs are untouched.
No emulator, dist changes, release, commit or push.

Regenerate from repository root using Python with NumPy:
python3 sparkpaw/music/drowned-sustain-studies-v1/generate.py
