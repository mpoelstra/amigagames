# Sparkpaw — Stormbound

Original title-music candidate, 2026-09-05. User listening verdict pending.
Listen to Stormbound-preview.mp3 or lossless Stormbound-preview.wav.
Stormbound.mod is the actual four-channel ProTracker module, 117,026 bytes;
its 16 original synthesized instruments contain 103,654 sample bytes. Neither
value is a verified game-integration Chip budget; replay buffers/code and
presentation lifetime need separate assessment.

## Musical direction

144 BPM, speed 6, 48 bars / 12 patterns, about 79.935 seconds per loop.
E minor with a B7 leading-tone cadence, warmer C/G passages and a contrasting
A-minor mechanical sequence. Original rising E-G-B theme with a descending
answer, syncopated bass, backbeat, composite kick/hat and snare samples, tom
turnarounds and sampled string chords within four simultaneous voices.

- 0:00–0:06.66: crystal call and engine ignition.
- 0:06.66–0:20: main eight-bar hero theme.
- 0:20–0:33.31: brighter ascending response.
- 0:33.31–0:46.63: clockwork ostinato / darker machinery.
- 0:46.63–1:13.27: full theme and response return with stronger lead.
- 1:13.27–1:19.935: turnaround into the opening.

The storm danger and malfunctioning machines supply harmonic tension; the
clear returning melody represents Sparkpaw's determined, inventive adventure.
This is a newly composed faster direction after Storm Light, not a tempo-only
edit. Old sketch and all game/test packages remain untouched.

## References and verification limits

User likes the supplied Turrican II title recording and requests more pace,
including ThunderCats/Marvin as contextual references. Turrican frame at 30 s
was inspected and audio decoded for onset analysis; octave-ambiguous candidates
91.46/178.57/120.97 BPM are not a transcription. No listening-model capability
was available; signal checks cannot establish musical quality. User listening
is the acceptance gate. No melody, sample or pattern from references is copied;
generate.py reads none of them. ThunderCats study skill was explicitly skipped.
Reference bytes are preserved in testresults/Unassigned-reference-turrican2-title-music.mov
with a matching TXT sidecar and original filename/hash.

Run generate.py with NumPy and the existing micromod library in
build/music-reference-study/libmicromod.dylib. It generates the MOD, manifest
and WAV by replaying that exact MOD, verifies legal instruments/periods/effects,
lengths, duration and no saturated PCM peaks. The preview narrows stereo to
65%, applies constant gain and fades only the last 0.25 seconds; no additional
instruments, compressor or reverb bus. Short echoes are authored in samples.
MP3 is an optional 192-kbit/s encoding of this WAV using ffmpeg.

Micromod rendered the complete loop; raw peak 24,308/32,768. An attempted second
MOD decode with local ffmpeg was unsupported (format not recognized); it is
not counted as independent MOD-engine verification. WAV/MP3 formats and stream
durations were checked separately. No emulator or real-Amiga playback claimed.

Not integrated into the title yet. The current title's brief hold, menu/loading
lifetime and four-channel ownership must be explicitly designed before adding
music to the game. Preserve all gameplay Paula effects and reclaim music memory
before gameplay. No release/commit/push or new hardware-test package.
