# Storm Light — original listening sketch

2026-09-05. Experimental music only; user listening verdict pending.
No game integration, release, new channel policy or hardware acceptance.

Listen to `StormLight-preview.wav`; `StormLight.mod` is the actual four-channel
ProTracker source module. The preview is rendered from that MOD with upstream
micromod, not from a separate unlimited-channel arrangement. It narrows the
player's stereo separation to 50%, applies one constant gain, and fades the
last half second. There is no mastering reverb or compressor; short echoes are
part of the original instrument samples. The module itself loops naturally to
the introduction; the WAV contains one 61.44-second traversal.

## Composition

- 0:00–0:10.24: glass motif, soft sampled harmony and sparse bass/percussion.
- 0:10.24–0:30.72: eight-bar reed theme, syncopated bass, snare backbeat.
- 0:30.72–0:40.96: quieter response and a dominant preparation.
- 0:40.96–1:01.44: varied theme return and D-minor resolution.

Six patterns, 24 bars, 16 original signed 8-bit mono samples. Quarter-note
pulse is 93.75 BPM with four rows per beat (F7D tempo, speed 8).
Channels: bass, percussion, sampled chords/occasional cymbal, melody.
Four-note chord voicings fit one Paula voice each because they are authored
as instrument samples. Bass sample tuning preserves the low register while
the score stays inside the classic C-1–B-3 period table. Only ordinary note,
volume, vibrato and timing commands are used. Sample bytes and current hashes
are in `manifest.json`. Memory use has not been optimized for game integration.

## What the supplied references establish

The two user-supplied Marvin MP3s identify Nils Ruzicka in their embedded tags:
`A Little Bit Harder` is 142.042750 s and `Going Uuuupp!` is 160.804083 s.
The supplied ThunderCats MOV is 91.016667 s and shows a browser playing a
YouTube title-screen video, not a direct emulator test. Frames at 5 and 40 s
were inspected. Its original bytes were retained under the meaningful name
`sparkpaw/testresults/Unassigned-reference-thundercats-title-music.mov` with
a matching TXT sidecar. Existing gameplay reference frames at 20 s were also
inspected and kept separate from the title reference.

All three recordings were decoded in memory for signal analysis at 16 kHz.
Approximate L/R correlations: Marvin 0.979 / 0.979; ThunderCats 0.475.
Approximately 67%, 62%, and 69% respectively of measured 0–8 kHz spectral
energy is below 200 Hz. This suggested testing a warm, bass-led balance rather
than a very bright lead-heavy mix. The Marvin onset analysis has a common
93.75/187.5 BPM candidate; tempo is ambiguous and this is not a verified
transcription. Recording EQ and stereo processing cannot be attributed to the
original Amiga engines from these files alone.

The `dw.Thundercats` file is 30,418 bytes. Inspection of its 68000 initialization
code, following the sample layout in [NostalgicPlayer's David Whittaker reader](https://github.com/neumatho/NostalgicPlayer/blob/main/Source/Agents/Players/DavidWhittaker/DavidWhittakerWorker.cs),
finds nine sample blocks of 9612, 2178, 5500, 5300, 2000, 64, 64, 16 and 4
bytes. The last four loop from zero; several contain only two distinct byte
values. This corrects the earlier overly broad suggestion that short primitive
waveforms themselves explain poor Amiga music. Longer samples, tiny waveforms
and musical articulation can coexist. This inspection is not a complete DW
replay or confirmation of the title subtune number.

No supplied melody, pattern or sample was copied into the sketch. The generator
does not read the references. The sketch explores original harmony, phrasing,
evolving instrument attacks and the measured broad tonal/rhythmic direction.
Signal analysis cannot establish musical quality; this has not been assessed
through a listening-model tool or accepted by the user.

## Reproduction and validation

Run `generate.py` with Python plus NumPy to regenerate the MOD and manifest.
Optional WAV rendering requires the upstream [micromod C player](https://github.com/martincameron/micromod/tree/master/micromod-c)
compiled as `sparkpaw/build/music-reference-study/libmicromod.dylib` from
`micromod.c` and `micromod.h` (`cc -O2 -shared -fPIC micromod.c -o libmicromod.dylib`).
Downloaded player source remains in the ignored build directory, outside the
game. The generator verifies MOD size, signature, sample lengths, period range,
instrument references, effect subset and volume limits. Independent micromod
replay verifies the expected 61.44 s and checks for clipped output samples.

This is host decoding evidence only. ProTracker/Amiga playback and subjective
musical quality remain unverified. Sparkpaw currently reserves only two voices
for future music; fitting this four-voice sketch into gameplay is a separate
decision and is not implemented here.
