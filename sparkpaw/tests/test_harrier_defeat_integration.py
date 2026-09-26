"""Offline checks for the approved boss defeat data and lifecycle contract."""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from generate_sparkpaw_sfx import energy_shot, harrier_defeat, pcm  # noqa: E402

header = (ROOT / "src/stormrail_harrier_death_art.h").read_text()
assert "#define STORM_DEATH_W 112" in header
assert "#define STORM_DEATH_H 64" in header
words = [int(x, 16) for x in re.findall(r"0x[0-9a-f]{4}", header)]
frame_words = 5 * 64 * 8
assert len(words) == 13 * frame_words

counts = []
for frame in range(13):
    at = frame * frame_words
    mask = words[at:at + 64 * 8]
    bits = words[at + 64 * 8:at + frame_words]
    assert all(mask[row * 8 + 7] == 0 for row in range(64))
    for plane in range(4):
        for cell, value in enumerate(bits[plane * 64 * 8:(plane + 1) * 64 * 8]):
            assert value & ~mask[cell] == 0
    counts.append(sum(word.bit_count() for word in mask))
assert counts[0] > 1000                 # visible original ship
assert max(counts[5:9]) > counts[0]     # full, local fire burst
assert counts[-1] < 20                  # sparse final residue

expected = bytes(value & 255 for value in pcm(harrier_defeat()))
sample = (ROOT / "sfx/raw/harrier-defeat.raw").read_bytes()
assert sample == expected
assert len(sample) == 13672
assert max(abs(value if value < 128 else value - 256) for value in sample) <= 124

def rms(values):
    return (sum(value * value for value in values) / len(values)) ** .5

boom = pcm(harrier_defeat())
shot = pcm(energy_shot())
span = round(.05 * 11025)
assert rms(boom[:span]) > rms(shot[:span])
assert rms(boom[round(.16 * 11025):round(.16 * 11025) + span]) > rms(boom[:span])
assert rms(boom[round(.40 * 11025):round(.40 * 11025) + span]) > 80
assert rms(boom[round(.70 * 11025):round(.70 * 11025) + span]) > 40

game = (ROOT / "src/game.c").read_text()
renderer = (ROOT / "src/renderer.c").read_text()
main = (ROOT / "src/main.c").read_text()
assert game.count("audioPlayHarrierDefeat();") == 1
assert "game.stormrailFinalePhase=STORMRAIL_FINALE_PHASE_DEFEAT;" in game
assert "stormrailRetireFinaleFire();\n            audioPlayHarrierDefeat();" in game
assert "game.stormrailFinalePhase=STORMRAIL_FINALE_PHASE_OPENING;" in game
assert "game.stormrailFinalePhase==STORMRAIL_FINALE_PHASE_COMPLETE" in game
assert "if(paused) {" in main and "gameUpdate();" in main
assert "stormrailHistory.finaleDeathDrawn=FALSE;" in renderer
assert "CopyMem((APTR)source,stormDeathMask" in renderer
assert "WaitBlit();\n                CopyMem((APTR)source" in renderer
assert "effectX=STORMRAIL_FINALE_GATE_X-STORM_DEATH_W;" in renderer
assert "game->stormrailDeathX-19" in renderer
catalog = (ROOT / "src/audio_catalog.h").read_text()
assert " ,120\n};" in catalog
mix = (ROOT / "src/level1_audio.c").read_text()
assert "if(id==FX_COUNT-1){mixer.voice[0].remaining=0;" in mix
audio = (ROOT / "src/audio.c").read_text()
title = (ROOT / "src/title.c").read_text()
layout = (ROOT / "tools/ready_ui_layout.c").read_text()
assert '"HARRIER DEFEAT"' in layout
assert "readySelection.sfx+19+delta)%19" in title
assert "audioPreviewPrepareHarrierDefeat()" in title
assert "audioPreviewReleaseHarrierDefeat();" in title
assert "if(id==18) { audioPlayHarrierDefeat(); return; }" in audio
assert 'loadSample("PROGDIR:assets/runtime/harrier-defeat.raw"' in audio
print("PASS: 13 masked native frames, approved sample, one-shot defeat lifecycle")
