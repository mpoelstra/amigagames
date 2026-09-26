# Mr. Nutz ingame audio references — technical analysis

These are measurements of the two user-supplied MP3s, not a claim that Codex
heard them. Sources: `/Users/mpoelstra/Downloads/mdat.nature.mp3` and
`/Users/mpoelstra/Downloads/mdat.water_and_map.mp3`. Comparison: the existing
`drowned-v5/undertow-circuit-preview.wav` host render. No game files changed.

Method: FFmpeg mono decode to 11,025 Hz; 4096-point Hann FFT every 256 samples.
Pitch-class summaries use energy from 65–1000 Hz; brightness is a median
spectral centroid from 50–5000 Hz. Eight-second summaries show broad tonal
sections, not individual melody notes or certain chord labels. Onset
autocorrelation yields ambiguous beat candidates, so exact BPM is not claimed.
Reproduce with `tools/analyze_music_references.py` using the bundled Python
runtime with NumPy and the three audio paths as arguments.

| Recording | Length | Median spectral centroid | Energy above 2.2 kHz | Mean adjacent 8 s pitch-class change |
| --- | ---: | ---: | ---: | ---: |
| Nature | 169.0 s | 1265 Hz | 22.5% | 0.140 |
| Water and map | 116.68 s | 1113 Hz | 17.3% | 0.142 |
| Undertow Circuit v5 preview | 106.58 s | 1410 Hz | 29.8% | 0.091 |

The references have a clearer repeating onset pulse in the autocorrelation
than v5. Candidates cluster around 91–98 BPM for Nature and 105–118 BPM for
Water and map, but subdivisions, changing sections and MP3 capture can make
these differ from the tracker tempo. v5's authored tracker tempo is 144 BPM.

Nature's dominant pitch-class groups move from E/C/G near the start, to
G/F#/A around 56–104 s, then C/G/E around 112–144 s, and D/A/G near the end.
Water and map is G/D/C dominant for roughly 8–64 s, moves toward A/E/D
around 72–96 s, and changes again after 104 s. The filename suggests more
than one cue, so the late shift may mark a cue boundary rather than a modulation.
V5 remains E dominant in virtually every eight-second block, with only small
changes in its second and third pitch classes. Its arrangement repeats the
same eight-bar melodic cell across 64 bars by construction.

Practical composition implications for a future *original* Drowned proposal:
write distinct longer melodic sections and real harmonic movement while
keeping the rhythmic pulse; use sample tone and articulation for tracker
character rather than adding high-frequency brightness; keep channel 4 empty
for SFX. Do not copy reference note sequences or infer precise timbres from
these measurements. User listening remains the approval gate.
