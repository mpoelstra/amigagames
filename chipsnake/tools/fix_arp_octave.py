#!/usr/bin/env python3
"""Transpose ChipSnake's 64-sample arpeggio instrument down one octave."""

import sys
from pathlib import Path


def fix(path: Path):
    data = bytearray(path.read_bytes())
    if data[1080:1084] != b"M.K.":
        raise ValueError(f"{path} is not a four-channel ProTracker MOD")
    song_length = data[950]
    patterns = max(data[952:952 + song_length]) + 1
    changed = 0
    for pattern in range(patterns):
        for row in range(64):
            # The generated arrangements dedicate channel 1 to sample 3.
            offset = 1084 + pattern * 1024 + row * 16 + 4
            sample = (data[offset] & 0xF0) | (data[offset + 2] >> 4)
            period = ((data[offset] & 0x0F) << 8) | data[offset + 1]
            if sample == 3 and period:
                period *= 2
                if period > 0x0FFF:
                    raise ValueError(f"period overflow in {path}")
                data[offset] = (data[offset] & 0xF0) | (period >> 8)
                data[offset + 1] = period & 0xFF
                changed += 1
    if changed == 0:
        raise ValueError(f"no arpeggio notes found in {path}")
    path.write_bytes(data)
    print(f"{path}: transposed {changed} arpeggio notes")


for filename in sys.argv[1:]:
    fix(Path(filename))
