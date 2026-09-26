# Undertow Circuit v6 — review only

Separate melodic proposal for Drowned Turbines. The v5 source and all staged game
assets remain intact. This version is not integrated into HD, WHDLoad or ADF.

The 150 BPM, 64-bar score has two alternating eight-bar call/answer melodies,
short pulse, reed and glass lead colours, continuous bass and percussion, and
small chord punctuations. The harmony resolves from B7 into E minor at restart.
Its fourth MOD channel contains no notes or effects; the game's Paula AUD3
software SFX output remains reserved. No title music notes or samples are used.

Listen to `undertow-circuit-preview.wav` for the complete 102.4-second loop, or
`undertow-theme-preview.wav` for the middle theme. Both use the existing
micromod host render at 48/64 music volume, without normalization. The host
preview peaks at -10.0 dBFS and has no clipped samples. The sample bank is
27,834 bytes Chip and score is 17,468 bytes Fast. The current native loader
expects v5's 21,622-byte bank, so integration would require an explicit size
contract change after approval. Native ptplayer, gameplay SFX balance and
68020 performance remain untested.

Rebuild with the bundled Python runtime with NumPy using `generate.py`.
