# Undertow Circuit — review v1

Original Drowned Turbines composition, 19 September 2026. Pending user audition.
144 BPM, 64 bars, D minor/Bb/F/C, 106.58 seconds measured by micromod.
Three music voices: percussion, pulse bass, reed/bell/power-fifth melody.
Fourth MOD channel is empty including effects, reserved for gameplay SFX.

Arrangement: 8 bars arrival;16 theme;8 arpeggio drive;8 hollow bridge;
16 varied theme;8 return. Descending A–G–F–E motif with a rising response.
Nine original deterministic synthesized samples; no external sample licensing.
Composite harmonic samples use one Paula voice each, no runtime synth/mixing.

Listen to undertow-theme-preview.wav (about27sec) or the complete
undertow-circuit-preview.wav. Host micromod rendering at proposed48/64 gain;
not Amiga ptplayer, actual gameplay balance or hardware/FPS acceptance.
No clipped host samples. Start speed6/tempo144; module restart order0.

Native deliverables prepared but NOT installed into runtime:
- undertow-circuit.mod:37266bytes
- rain-score.bin:17468bytes Fast
- rain-bank.bin:19798bytes Chip

The raw37.3KB payload is above the original12–24KiB disk aspiration; no packing
or ADF fit claim. HD/WHDLoad/ADF must retain identical music quality. Current
414336bytes free Chip was measured WITHOUT gameplay music in this candidate;
include mixer buffers/player/allocator overhead and future finale art in tests.

Reproduce with Python + numpy using generate.py. It uses the existing local
micromod preview library, validates lengths, reserved channel, duration and peaks.
The bundled Codex Python runtime has numpy; the project .venv currently does not.

Integration must extend the selected-track descriptor and exact size validation
in level1_audio.c, add new effect mappings (checkpoint/pump-shot), and retain
CIA/vector ownership, death continuity, pause/results/unload and replay lifecycle.
Do not enable music while checkpoint still directly writes music-owned AUD1.
