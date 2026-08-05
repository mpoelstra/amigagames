#!/usr/bin/env python3
"""Convert ChipSnake's earlier 150 BPM/speed-5 MODs to PAL-stable timing."""

import sys
from pathlib import Path


def normalize(path: Path):
    data = bytearray(path.read_bytes())
    if data[1080:1084] != b"M.K.":
        raise ValueError(f"{path} is not a four-channel ProTracker MOD")
    song_length = data[950]
    patterns = max(data[952:952 + song_length]) + 1
    for offset in range(1084, 1084 + patterns * 1024, 4):
        effect = data[offset + 2] & 0x0F
        parameter = data[offset + 3]
        if effect == 0x0F and parameter in (125, 150):
            # The generated tracks start at 125 BPM. Remove redundant tempo
            # commands so each row contains at most one Fxx timing operation.
            data[offset + 2] &= 0xF0
            data[offset + 3] = 0
        elif effect == 0x0F and parameter == 5:
            data[offset + 3] = 4
        elif effect == 0x04:
            data[offset + 2] &= 0xF0
            data[offset + 3] = 0
    for pattern in range(patterns):
        # Row 1, channel 3 is unused by these generated arrangements.
        offset = 1084 + pattern * 1024 + 16 + 12
        data[offset:offset + 4] = bytes((0, 0, 0x0F, 4))
    path.write_bytes(data)


for filename in sys.argv[1:]:
    normalize(Path(filename))
