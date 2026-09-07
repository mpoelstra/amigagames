# Stormbound — faster synth listening variations

Three original four-channel ProTracker sketches following the user-approved
smoother phrasing. Turrican II and Marvin are user-supplied stylistic directions;
no reference samples or transcribed melodies are used. Reference audio was
technically analysed, not aurally reviewed by the assistant.

- **D — Voltage Run (168 BPM):** pumping beat, detuned saw/pulse lead and a new answering phrase.
- **E — Neon Sky (160 BPM):** longer melodic arcs and sustained synth harmony.
- **F — Turbo Shards (176 BPM):** fastest version, brighter electric plucks and tracker arpeggios.

Each directory contains its standalone generator, MOD, actual micromod WAV
render, MP3 preview, manifest and generation report. Run `create.py` with the
NumPy-enabled Python runtime to regenerate all three from the preceding variants.
The existing local micromod library and ffmpeg are required.

MOD signatures, hashes, WAV duration and MP3 decoding checked. Rendered PCM
peaks remain below clipping before mastering. User listening remains the
musical acceptance test. These expanded sample budgets are not approved for
runtime integration; game assets and release packages are unchanged.
