#!/usr/bin/env python3
"""Generate an original slow, melancholic Hall of Fame ProTracker MOD."""

import math
import random
import struct
from pathlib import Path


OUTPUT = Path("music/chipsnake-hall.mod")
RATE = 8363
PERIODS = {
    "C1": 856, "D1": 762, "E1": 678, "F1": 640, "G1": 570,
    "A1": 508, "B1": 453, "C2": 428, "D2": 381, "E2": 339,
    "F2": 320, "G2": 285, "A2": 254, "B2": 226, "C3": 214,
    "D3": 190, "E3": 170, "F3": 160, "G3": 143, "A3": 127,
    "B3": 113, "C4": 107, "D4": 95, "E4": 85,
}


def signed(values):
    return bytes((max(-128, min(127, round(v))) & 255) for v in values)


def wave(length, shape):
    values = []
    for i in range(length):
        p = i / length
        if shape == "sine":
            value = math.sin(p * math.tau)
        elif shape == "triangle":
            value = 4 * abs(p - 0.5) - 1
        else:
            value = 0.72 * math.sin(p * math.tau)
            value += 0.28 * math.sin(p * math.tau * 2)
        values.append(value * 105)
    return signed(values)


def bell():
    values = []
    for i in range(2200):
        t = i / RATE
        envelope = math.exp(-t * 7.5)
        value = math.sin(t * math.tau * 620)
        value += 0.38 * math.sin(t * math.tau * 1243)
        values.append(value * envelope * 83)
    return signed(values)


def soft_noise():
    rng = random.Random(50000)
    values = []
    previous = 0
    for i in range(1000):
        noise = rng.randint(-100, 100)
        previous = (previous * 3 + noise) / 4
        values.append(previous * math.exp(-i / RATE * 13))
    return signed(values)


SAMPLES = [
    ("LONELY LEAD", wave(128, "warm"), 45, 0, 128),
    ("WARM BASS", wave(128, "triangle"), 55, 0, 128),
    ("GLASS ARP", wave(128, "sine"), 30, 0, 128),
    ("DISTANT BELL", bell(), 42, 0, 2),
    ("SOFT BRUSH", soft_noise(), 26, 0, 2),
]


def event(sample=0, note=None, effect=0, parameter=0):
    period = PERIODS.get(note, 0)
    return bytes((
        (sample & 0xF0) | ((period >> 8) & 15),
        period & 255,
        ((sample & 15) << 4) | (effect & 15),
        parameter & 255,
    ))


CHORDS = {
    "AM": ("A1", "A2", 0x37),
    "FM": ("F1", "F2", 0x47),
    "CM": ("C1", "C3", 0x47),
    "EM": ("E1", "E2", 0x37),
}

MELODIES = [
    ("E3", None, "C3", None, "A2", None, "C3", "D3",
     "E3", None, "G3", "E3", "D3", None, "C3", None),
    ("A3", None, "G3", "E3", "F3", None, "E3", "C3",
     "D3", "E3", "F3", None, "E3", "D3", "C3", None),
    ("C4", "B3", "A3", None, "G3", "E3", "D3", None,
     "E3", "G3", "A3", "G3", "E3", None, "D3", None),
    ("E3", None, "A3", "G3", "E3", "D3", "C3", None,
     "A2", "C3", "E3", None, "D3", "C3", "A2", None),
]


def pattern(chord_names, melody, fuller=False):
    rows = [[event() for _ in range(4)] for _ in range(64)]
    for row in range(64):
        chord = chord_names[row // 16]
        bass_root, arp_root, arp_effect = CHORDS[chord]
        if row == 0:
            rows[row][3] = event(effect=0xF, parameter=6)
        if row % 16 == 0:
            rows[row][1] = event(2, bass_root, 0xC, 52)
            rows[row][2] = event(3, arp_root, 0, arp_effect)
            if fuller:
                rows[row][3] = event(4, "C3", 0xF if row == 0 else 0,
                                     6 if row == 0 else 0)
        elif row % 16 == 8:
            rows[row][1] = event(2, CHORDS[chord][1], 0xC, 38)
        if row % 4 == 0:
            note = melody[row // 4]
            if note:
                rows[row][0] = event(1, note, 0x4, 0x12)
        if row % 8 == 4:
            rows[row][3] = event(5, "C3", 0xC, 20)
    return b"".join(b"".join(row) for row in rows)


def sample_header(name, data, volume, loop_start, loop_length):
    if len(data) & 1:
        data += b"\0"
    return (
        name.encode("ascii")[:22].ljust(22, b"\0")
        + struct.pack(">H", len(data) // 2)
        + bytes((0, volume))
        + struct.pack(">HH", loop_start // 2, max(1, loop_length // 2))
    ), data


def build():
    progressions = [
        ("AM", "FM", "CM", "EM"),
        ("FM", "CM", "EM", "AM"),
        ("AM", "CM", "FM", "EM"),
        ("FM", "EM", "AM", "AM"),
    ]
    patterns = [
        pattern(progressions[0], MELODIES[0]),
        pattern(progressions[1], MELODIES[1]),
        pattern(progressions[2], MELODIES[2], True),
        pattern(progressions[3], MELODIES[3], True),
    ]
    order = bytes((0, 1, 2, 3, 0, 2, 1, 3))
    headers, data_blocks = [], []
    for name, data, volume, loop_start, loop_length in SAMPLES:
        if loop_length > 2:
            data = b"\0\0" + data
            loop_start += 2
        header, data = sample_header(
            name, data, volume, loop_start, loop_length
        )
        headers.append(header)
        data_blocks.append(data)
    headers.extend([b"\0" * 30] * (31 - len(headers)))
    module = b"CHIPSNAKE HALL".ljust(20, b"\0")
    module += b"".join(headers)
    module += bytes((len(order), 0x7F)) + order.ljust(128, b"\0") + b"M.K."
    module += b"".join(patterns) + b"".join(data_blocks)
    OUTPUT.write_bytes(module)
    print(f"Wrote {OUTPUT} ({len(module)} bytes)")


if __name__ == "__main__":
    build()
