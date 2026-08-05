#!/usr/bin/env python3
"""Generate a two-channel Pixel Sprint Clean gameplay arrangement."""

import math
import random
import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
OUTPUT = ROOT / "music" / "mrdigs-futsal-ingame.mod"
RATE = 8363
PERIODS = {
    "C2": 428, "D2": 381, "E2": 339, "F2": 320, "G2": 285, "A2": 254,
    "BB2": 240, "C3": 214, "D3": 190, "E3": 170, "F3": 160, "G3": 143,
    "A3": 127, "BB3": 120, "B3": 113, "C4": 107, "D4": 95,
    "E4": 85, "F4": 80, "G4": 71, "A4": 64,
}


def signed(values):
    return bytes(max(-128, min(127, round(v))) & 255 for v in values)


def groove(root):
    frequencies = {"C": 65.41, "A": 55.00, "F": 87.31, "G": 98.00}
    seconds, length = 1.6, round(RATE * 1.6)
    rng, out = random.Random(900 + ord(root[0])), []
    phase = 0.0
    for i in range(length):
        t, beat = i / RATE, (i / length) * 16
        step = int(beat)
        local = beat - step
        bass_gate = math.exp(-local * 5.4) if step in (0, 3, 6, 8, 11, 14) else 0
        freq = frequencies[root] * (2 if step in (6, 14) else 1)
        phase += freq / RATE
        # Rounded fundamental with only a soft second harmonic: retain the
        # clean candidate's bass character without its former buzzing timbre.
        bass = (math.sin(math.tau * phase) + .10 *
                math.sin(math.tau * phase * 2)) * 66 * bass_gate
        kick_phase = local * (.085 * RATE)
        kick = math.sin(math.tau * kick_phase) * 95 * math.exp(-local * 24) \
            if step in (0, 4, 8, 12) else 0
        hat = rng.uniform(-28, 28) * math.exp(-local * 34) \
            if step % 2 else 0
        snare = rng.uniform(-58, 58) * math.exp(-local * 20) \
            if step in (4, 12) else 0
        out.append(math.tanh((bass + kick + hat + snare) / 105) * 118)
    return signed(out)


def lead():
    """Four-cycle lead: legal PT octaves retain Pixel Sprint's high pitch."""
    values = []
    for index in range(256):
        phase = (index / 256) * 4
        value = math.sin(math.tau * phase)
        value += .32 * math.sin(math.tau * phase * 2)
        value += .12 * math.sin(math.tau * phase * 4)
        values.append(value * 78)
    return signed(values)


def event(sample=0, note=None, effect=0, parameter=0):
    period = PERIODS.get(note, 0)
    return bytes(((sample & 0xF0) | (period >> 8), period & 255,
                  ((sample & 15) << 4) | effect, parameter))


ROOTS = ("C", "A", "F", "G")
HOOKS = (
    ("E2", "G2", "C3", "G2", "E2", "D2", "E2", "G2",
     "A2", "E2", "C2", "E2", "G2", "A2", "G2", "E2"),
    ("F2", "A2", "C3", "A2", "G2", "F2", "E2", "C2",
     "D2", "G2", "B1", "A2", "G2", "D2", "E2", "G2"),
    ("C2", "E2", "G2", "C3", "B1", "G2", "E2", "D2",
     "E2", "A2", "C3", "B1", "A2", "E2", "C2", "E2"),
)


def pattern(index):
    rows = [[event() for _ in range(4)] for _ in range(64)]
    for row in range(64):
        quarter = row // 16
        if row % 16 == 0:
            rows[row][0] = event(quarter + 1, "C2",
                                 0xF if row == 0 else 0, 5 if row == 0 else 0)
        if row % 4 == 0:
            note = HOOKS[index % len(HOOKS)][row // 4]
            rows[row][1] = event(5, note, 4, 0x12)
        elif row % 8 == 6:
            root_note = {"C": "C2", "A": "A2", "F": "F2", "G": "G2"}[
                ROOTS[quarter]]
            rows[row][1] = event(5, root_note, 0xC, 34)
    # Match the title track's stable, intentionally brisk 150 BPM.
    first_note = HOOKS[index % len(HOOKS)][0]
    rows[0][1] = event(5, first_note, 0xF, 150)
    # Deliberately never write channels 2 and 3.
    return b"".join(b"".join(row) for row in rows)


def header(name, data, volume, loop=False):
    if len(data) & 1:
        data += b"\0"
    loop_start, loop_length = (0, len(data)) if loop else (0, 2)
    return (name.encode()[:22].ljust(22, b"\0") +
            struct.pack(">HBBHH", len(data) // 2, 0, volume,
                        loop_start // 2, loop_length // 2)), data


def build():
    # Leave generous Paula headroom for the two dedicated effects channels.
    samples = [(f"{root} GROOVE", groove(root), 26, False) for root in ROOTS]
    samples.append(("SPORTS LEAD", lead(), 22, True))
    headers, data = zip(*(header(*sample) for sample in samples))
    patterns = [pattern(i) for i in range(6)]
    order = bytes((0, 1, 2, 1, 3, 2, 4, 1, 5, 2))
    module = b"PIXEL SPRINT 2CH".ljust(20, b"\0")
    module += b"".join(headers) + b"\0" * (30 * (31 - len(headers)))
    module += bytes((len(order), 0x7F)) + order.ljust(128, b"\0") + b"M.K."
    module += b"".join(patterns) + b"".join(data)
    OUTPUT.write_bytes(module)
    print(f"Wrote {OUTPUT} ({len(module)} bytes); channels 2/3 reserved")


if __name__ == "__main__":
    build()
