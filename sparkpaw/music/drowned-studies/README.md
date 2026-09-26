# Drowned Turbines — three review-only MOD studies

The user's two Mr. Nutz MP3s were analysed numerically in
`../DROWNED_REFERENCE_ANALYSIS.md`. These are original new compositions; no
reference notes or samples were extracted or copied. They have not been
subjectively auditioned by Codex. User listening decides the musical direction.

| Study | Musical idea | BPM | Full loop | MOD bytes |
| --- | --- | ---: | ---: | ---: |
| Sluice Run | warm, melodic D minor with major-key lift | 116 | 99.27 s | 34,446 |
| Rainway | playful mallet/pulse over a shifting G minor route | 122 | 94.35 s | 34,446 |
| Pressure Line | stronger pulse and bass with E minor drive | 130 | 88.61 s | 34,446 |

Each folder contains a 30-second `short-preview.wav`, a complete `preview.wav`,
the playable `.mod`, split `score.bin`/`bank.bin`, and a manifest. Previews use
the existing micromod pipeline at 48/64 gain. All modules use only music
channels 1–3; every fourth-channel cell, including effects, is empty. The
fourth channel stays available to the game's software SFX mixer. The sample
bank is 21,074 Chip bytes and score 13,372 Fast bytes per study, subject to
normal allocation overhead. Each host preview peaks below -9 dBFS. The tonal
movement measured across eight-second blocks is greater than v5's, and the
frequency balance is less top-heavy; those metrics do not guarantee a better
listening result.

The runtime loader currently requires v5's exact 17,468-byte score and
21,622-byte bank. Integration, native ptplayer behaviour, SFX balance,
68020 cost, and HD/WHDLoad/ADF parity remain pending user selection.
Existing v5 game assets and the rejected v6 study are unchanged.

Regenerate with the bundled Python runtime with NumPy:

`python3 music/drowned-studies/generate.py`

The design uses original short 8-bit single-shot samples and a sampled triad
on one channel, reflecting capabilities documented in the original ProTracker
helpfile: https://github.com/echolevel/Protracker-2.3D-Helpfile-Manual .
