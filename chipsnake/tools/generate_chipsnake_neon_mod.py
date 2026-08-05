#!/usr/bin/env python3
"""Generate ChipSnake Neon Drive, an original dance/electronic ProTracker MOD."""

import math
import random
import struct
from pathlib import Path


OUTPUT = Path("music/chipsnake-neon.mod")
SAMPLE_RATE = 8363

PERIODS = {
    "C1": 856, "D1": 762, "E1": 678, "F1": 640, "G1": 570,
    "A1": 508, "B1": 453,
    "C2": 428, "D2": 381, "E2": 339, "F2": 320, "G2": 285,
    "A2": 254, "B2": 226,
    "C3": 214, "D3": 190, "E3": 170, "F3": 160, "G3": 143,
    "A3": 127, "B3": 113,
    "C4": 107, "D4": 95, "E4": 85, "F4": 80, "G4": 71,
    "A4": 64, "B4": 57,
}


def signed(values):
    return bytes((max(-128, min(127, round(v))) & 0xFF) for v in values)


def waveform(length, kind, drive=1.0):
    values = []
    for i in range(length):
        phase = i / length
        if kind == "saw":
            value = 2 * phase - 1
        elif kind == "square":
            value = 1 if phase < 0.28 else -1
        else:
            value = 4 * abs(phase - 0.5) - 1
        values.append(math.tanh(value * drive) * 112)
    return signed(values)


def kick(clap=False):
    rng = random.Random(20260729 if clap else 68020)
    values = []
    phase = 0.0
    length = 1200
    for i in range(length):
        t = i / SAMPLE_RATE
        frequency = 190 * math.exp(-t * 30) + 45
        phase += frequency / SAMPLE_RATE
        body = math.sin(phase * math.tau) * 124 * math.exp(-t * 13)
        click = (1 if i < 22 and i % 2 == 0 else -1) * 34 * math.exp(-t * 100)
        noise = 0
        if clap and 180 <= i < 1050:
            local = (i - 180) / SAMPLE_RATE
            burst = (math.exp(-local * 22)
                     * (0.65 + 0.35 * math.sin(local * math.tau * 31)))
            noise = rng.uniform(-72, 72) * burst
        values.append(body + click + noise)
    return signed(values)


def hat(open_hat=False):
    rng = random.Random(1312 if open_hat else 303)
    length = 1500 if open_hat else 340
    values = []
    previous = 0
    for i in range(length):
        noise = rng.randint(-127, 127)
        high = noise - previous * 0.82
        previous = noise
        decay = 12 if open_hat else 38
        values.append(high * 0.62 * math.exp(-i / SAMPLE_RATE * decay))
    return signed(values)


def riser():
    rng = random.Random(909)
    values = []
    previous = 0.0
    length = 3600
    for i in range(length):
        progress = i / length
        noise = rng.uniform(-110, 110)
        previous += (noise - previous) * (0.04 + progress * 0.42)
        values.append(previous * progress * (1 - progress * 0.25))
    return signed(values)


SAMPLES = [
    ("CLUB KICK", kick(), 64, 0, 2),
    ("KICK CLAP", kick(True), 64, 0, 2),
    ("CLOSED HAT", hat(), 42, 0, 2),
    ("OPEN HAT", hat(True), 48, 0, 2),
    ("SUB BASS", waveform(128, "triangle", 1.5), 64, 0, 128),
    ("NEON STAB", waveform(128, "saw", 2.4), 38, 0, 128),
    ("LASER LEAD", waveform(128, "square", 1.8), 46, 0, 128),
    ("NOISE RISER", riser(), 44, 0, 2),
]


def event(sample=0, note=None, effect=0, parameter=0):
    period = PERIODS.get(note, 0)
    return bytes((
        (sample & 0xF0) | ((period >> 8) & 0x0F),
        period & 0xFF,
        ((sample & 0x0F) << 4) | (effect & 0x0F),
        parameter & 0xFF,
    ))


ROOTS = ("A", "F", "C", "G")
BASS = {
    "A": ("A1", "A2", "E2", "G2"),
    "F": ("F1", "F2", "C2", "E2"),
    "C": ("C1", "C2", "G1", "B1"),
    "G": ("G1", "G2", "D2", "F2"),
}
STABS = {"A": "A2", "F": "F2", "C": "C3", "G": "G2"}
ARPS = {"A": 0x37, "F": 0x47, "C": 0x47, "G": 0x47}

HOOK_A = (
    "A3", None, "C4", "E4", None, "E4", "D4", "C4",
    "G3", None, "A3", "C4", None, "C4", "A3", "G3",
    "E3", "G3", "A3", None, "C4", "A3", "G3", "E3",
    "G3", None, "B3", "D4", "E4", "D4", "B3", "G3",
)
HOOK_B = (
    "E4", "C4", "A3", "C4", "E4", None, "G4", "E4",
    "C4", "A3", "G3", "A3", "C4", None, "E4", "C4",
    "A3", "C4", "E4", "A4", "G4", "E4", "D4", "C4",
    "B3", "D4", "G4", "D4", "E4", "D4", "B3", "G3",
)


def make_pattern(mode, hook=None):
    rows = [[event() for _ in range(4)] for _ in range(64)]
    for row in range(64):
        bar = row // 16
        root = ROOTS[bar]

        # 150 BPM at PAL 50 Hz: speed five, four tracker rows per beat.
        if row == 0:
            rows[row][0] = event(1, "C2", 0xF, 5)
        elif row % 4 == 0:
            rows[row][0] = event(2 if row % 8 == 4 else 1, "C2")

        if mode not in ("intro", "break"):
            position = row % 16
            if position in (0, 3, 6, 8, 11, 14):
                note = BASS[root][(position // 3) % 4]
                rows[row][1] = event(5, note, 0xC, 58 if position % 2 else 64)
        elif mode == "break" and row % 8 == 0:
            rows[row][1] = event(5, BASS[root][0], 0xC, 42)

        # Offbeat rave stabs leave the downbeat clear for kick and bass.
        if mode not in ("intro", "break") and row % 4 == 2:
            rows[row][2] = event(6, STABS[root], 0x0, ARPS[root])
        elif mode == "build" and row % 8 == 0:
            rows[row][2] = event(6, STABS[root], 0xC, 28 + row // 4)

        if hook and row % 2 == 0:
            note = hook[row // 2]
            if note:
                rows[row][3] = event(7, note, 0x4, 0x23)
        elif row % 4 == 2:
            rows[row][3] = event(4, "C3")
        elif row % 2 == 1:
            rows[row][3] = event(3, "C3", 0xC, 30)

    if mode == "intro":
        for row in range(0, 64, 8):
            rows[row][1] = event(5, BASS[ROOTS[row // 16]][0], 0xC, 38)
        for row in range(34, 64, 4):
            rows[row][2] = event(6, STABS[ROOTS[row // 16]], 0x0,
                                 ARPS[ROOTS[row // 16]])
    if mode in ("build", "climax"):
        rows[48][3] = event(8, "C3")
    if mode == "break":
        rows[0][2] = event(6, "A2", 0x0, 0x37)
        rows[16][2] = event(6, "F2", 0x0, 0x47)
        rows[32][2] = event(6, "C3", 0x0, 0x47)
        rows[48][3] = event(8, "C3")
    return b"".join(b"".join(row) for row in rows)


def sample_header(name, data, volume, loop_start, loop_length):
    if len(data) & 1:
        data += b"\0"
    header = (
        name.encode("ascii")[:22].ljust(22, b"\0")
        + struct.pack(">H", len(data) // 2)
        + bytes((0, volume))
        + struct.pack(">HH", loop_start // 2, max(1, loop_length // 2))
    )
    return header, data


def build():
    patterns = [
        make_pattern("intro"),
        make_pattern("build"),
        make_pattern("drop", HOOK_A),
        make_pattern("drop", HOOK_B),
        make_pattern("drop", HOOK_A),
        make_pattern("break"),
        make_pattern("build", HOOK_A),
        make_pattern("climax", HOOK_B),
        make_pattern("climax", HOOK_A),
        make_pattern("climax", HOOK_B),
    ]
    order = bytes((0, 1, 2, 3, 2, 4, 5, 1, 6, 7, 8, 9, 7, 8))

    headers, sample_data = [], []
    for name, data, volume, loop_start, loop_length in SAMPLES:
        if loop_length > 2:
            data = b"\0\0" + data
            loop_start += 2
        header, prepared = sample_header(
            name, data, volume, loop_start, loop_length
        )
        headers.append(header)
        sample_data.append(prepared)
    headers.extend([b"\0" * 30] * (31 - len(headers)))

    module = b"CHIPSNAKE NEON DRIVE".ljust(20, b"\0")
    module += b"".join(headers)
    module += bytes((len(order), 0x7F))
    module += order.ljust(128, b"\0")
    module += b"M.K."
    module += b"".join(patterns)
    module += b"".join(sample_data)
    OUTPUT.write_bytes(module)
    print(f"Wrote {OUTPUT} ({len(module)} bytes)")


if __name__ == "__main__":
    build()
