#!/usr/bin/env python3
"""Generate three bright, original Amiga dance-title MOD candidates."""

import math
import random
import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
OUTPUT = ROOT / "music" / "candidates"
RATE = 8363
NAMES = ("C", "CS", "D", "DS", "E", "F", "FS", "G", "GS", "A", "AS", "B")
PERIODS = {}
for octave in range(1, 6):
    for semitone, name in enumerate(NAMES):
        distance = semitone + (octave - 1) * 12
        PERIODS[f"{name}{octave}"] = max(28, round(856 / (2 ** (distance / 12))))


def signed(values):
    return bytes((max(-128, min(127, round(value))) & 255)
                 for value in values)


def loop_wave(length, kind, level=105):
    values = []
    for index in range(length):
        phase = index / length
        if kind == "bass":
            value = .74 * math.sin(math.tau * phase)
            value += .26 * math.sin(math.tau * phase * 2)
        elif kind == "chord":
            value = 2 * phase - 1
            value += .24 * math.sin(math.tau * phase * 3)
        elif kind == "lead":
            value = math.sin(math.tau * phase)
            value += .38 * math.sin(math.tau * phase * 2)
            value += .18 * math.sin(math.tau * phase * 4)
        else:
            value = 1 if phase < .25 else -.72
        values.append(math.tanh(value * 1.25) * level)
    return signed(values)


def kick(with_clap=False):
    rng = random.Random(1200 if with_clap else 500)
    values, phase = [], 0.0
    for index in range(1150):
        time = index / RATE
        frequency = 190 * math.exp(-time * 31) + 52
        phase += frequency / RATE
        value = 122 * math.sin(math.tau * phase) * math.exp(-time * 15)
        if with_clap and 150 < index < 900:
            value += rng.uniform(-78, 78) * math.exp(
                -(index - 150) / RATE * 26
            )
        values.append(value)
    return signed(values)


def hat():
    rng = random.Random(20260730)
    values, previous = [], 0.0
    for index in range(300):
        value = rng.uniform(-1, 1)
        high = value - previous * .82
        previous = value
        values.append(high * 90 * math.exp(-index / RATE * 52))
    return signed(values)


def sparkle():
    values = []
    for index in range(850):
        time = index / RATE
        value = math.sin(math.tau * 1760 * time)
        value += .45 * math.sin(math.tau * 2637 * time)
        values.append(value * 74 * math.exp(-time * 18))
    return signed(values)


def clean_bass():
    """Rounded sine/triangle hybrid without the buzzy pulse harmonics."""
    values = []
    for index in range(256):
        phase = index / 256
        sine = math.sin(math.tau * phase)
        triangle = 4 * abs(phase - .5) - 1
        values.append((sine * .82 + triangle * .18) * 96)
    return signed(values)


def glass_chord():
    """Bright, smooth chord voice replacing the droning saw sample."""
    values = []
    for index in range(256):
        phase = index / 256
        value = math.sin(math.tau * phase)
        value += .24 * math.sin(math.tau * phase * 2)
        value += .10 * math.sin(math.tau * phase * 3)
        values.append(value * 72)
    return signed(values)


def protracker_lead():
    """Four-cycle lead so C-2..C-3 retain the former C-4..C-5 pitch.

    Classic ProTracker only defines periods through B-3. VLC accepts the
    extended periods used by the first draft, but Amiga players do not all
    handle them consistently.
    """
    values = []
    for index in range(256):
        phase = (index / 256) * 4
        value = math.sin(math.tau * phase)
        value += .38 * math.sin(math.tau * phase * 2)
        value += .18 * math.sin(math.tau * phase * 4)
        values.append(math.tanh(value * 1.25) * 92)
    return signed(values)


SAMPLES = (
    ("BRIGHT KICK", kick(), 64, False),
    ("KICK CLAP", kick(True), 64, False),
    ("BOUNCE BASS", loop_wave(256, "bass"), 55, True),
    ("SUNNY CHORD", loop_wave(256, "chord", 96), 36, True),
    ("SHIMMER LEAD", loop_wave(256, "lead", 92), 49, True),
    ("TINY HAT", hat(), 31, False),
    ("STAR PLUCK", sparkle(), 42, False),
)

CLEAN_SAMPLES = (
    ("BRIGHT KICK", kick(), 64, False),
    ("KICK CLAP", kick(True), 64, False),
    ("CLEAN ROUND BASS", clean_bass(), 53, True),
    ("GLASS CHORD", glass_chord(), 34, True),
    ("PT SHIMMER LEAD", protracker_lead(), 49, True),
    ("TINY HAT", hat(), 31, False),
    ("STAR PLUCK", sparkle(), 42, False),
)


def event(sample=0, note=None, effect=0, parameter=0):
    period = PERIODS.get(note, 0)
    return bytes((
        (sample & 0xF0) | ((period >> 8) & 15),
        period & 255,
        ((sample & 15) << 4) | (effect & 15),
        parameter & 255,
    ))


VARIANTS = (
    {
        "file": "mrdigs-futsal-title-stadium-sunrise.mod",
        "title": "STADIUM SUNRISE",
        "bpm": 138,
        "roots": ("A", "FS", "D", "E"),
        "bass": {
            "A": ("A1", "A2", "E2", "GS2"),
            "FS": ("FS1", "FS2", "CS2", "E2"),
            "D": ("D1", "D2", "A1", "CS2"),
            "E": ("E1", "E2", "B1", "D2"),
        },
        "chord": {"A": "A2", "FS": "FS2", "D": "D2", "E": "E2"},
        "arp": {"A": 0x47, "FS": 0x37, "D": 0x47, "E": 0x47},
        "hook_a": (
            "A3", None, "CS4", "E4", "A4", "GS4", "E4", "CS4",
            "FS3", "A3", "CS4", "E4", "FS4", "E4", "CS4", "A3",
            "D4", "FS4", "A4", "FS4", "E4", "D4", "CS4", "A3",
            "B3", "CS4", "E4", "GS4", "FS4", "E4", "CS4", None,
        ),
        "hook_b": (
            "E4", "FS4", "A4", "B4", "A4", "E4", "CS4", "B3",
            "CS4", "E4", "FS4", "A4", "FS4", "E4", "CS4", "A3",
            "A3", "CS4", "D4", "FS4", "A4", "FS4", "E4", "D4",
            "B3", "E4", "GS4", "B4", "A4", "GS4", "E4", None,
        ),
    },
    {
        "file": "mrdigs-futsal-title-pixel-sprint.mod",
        "title": "PIXEL SPRINT",
        "bpm": 145,
        "roots": ("C", "A", "F", "G"),
        "bass": {
            "C": ("C2", "C3", "G2", "B2"),
            "A": ("A1", "A2", "E2", "G2"),
            "F": ("F1", "F2", "C2", "E2"),
            "G": ("G1", "G2", "D2", "F2"),
        },
        "chord": {"C": "C3", "A": "A2", "F": "F2", "G": "G2"},
        "arp": {"C": 0x47, "A": 0x37, "F": 0x47, "G": 0x47},
        "hook_a": (
            "E4", "G4", "C5", "G4", "E4", "D4", "E4", "G4",
            "A4", "E4", "C4", "E4", "G4", "A4", "G4", "E4",
            "F4", "A4", "C5", "A4", "G4", "F4", "E4", "C4",
            "D4", "G4", "B4", "A4", "G4", "D4", "E4", None,
        ),
        "hook_b": (
            "C4", "E4", "G4", "C5", "B4", "G4", "E4", "D4",
            "E4", "A4", "C5", "B4", "A4", "E4", "C4", "E4",
            "A4", "C5", "A4", "F4", "A4", "G4", "E4", "C4",
            "D4", "F4", "G4", "B4", "A4", "G4", "E4", None,
        ),
    },
    {
        "file": "mrdigs-futsal-title-pixel-sprint-clean.mod",
        "title": "PIXEL SPRINT CLEAN",
        "bpm": 145,
        "clean_samples": True,
        "roots": ("C", "A", "F", "G"),
        "bass": {
            "C": ("C2", "C3", "G2", "B2"),
            "A": ("A1", "A2", "E2", "G2"),
            "F": ("F1", "F2", "C2", "E2"),
            "G": ("G1", "G2", "D2", "F2"),
        },
        "chord": {"C": "C3", "A": "A2", "F": "F2", "G": "G2"},
        "arp": {"C": 0x47, "A": 0x37, "F": 0x47, "G": 0x47},
        "hook_a": (
            "E4", "G4", "C5", "G4", "E4", "D4", "E4", "G4",
            "A4", "E4", "C4", "E4", "G4", "A4", "G4", "E4",
            "F4", "A4", "C5", "A4", "G4", "F4", "E4", "C4",
            "D4", "G4", "B4", "A4", "G4", "D4", "E4", None,
        ),
        "hook_b": (
            "C4", "E4", "G4", "C5", "B4", "G4", "E4", "D4",
            "E4", "A4", "C5", "B4", "A4", "E4", "C4", "E4",
            "A4", "C5", "A4", "F4", "A4", "G4", "E4", "C4",
            "D4", "F4", "G4", "B4", "A4", "G4", "E4", None,
        ),
    },
    {
        "file": "mrdigs-futsal-title-skyline-cup.mod",
        "title": "SKYLINE CUP",
        "bpm": 134,
        "roots": ("G", "D", "E", "C"),
        "bass": {
            "G": ("G1", "G2", "D2", "FS2"),
            "D": ("D1", "D2", "A1", "CS2"),
            "E": ("E1", "E2", "B1", "D2"),
            "C": ("C1", "C2", "G1", "B1"),
        },
        "chord": {"G": "G2", "D": "D2", "E": "E2", "C": "C2"},
        "arp": {"G": 0x47, "D": 0x47, "E": 0x37, "C": 0x47},
        "hook_a": (
            "B3", "D4", "G4", "A4", "B4", "A4", "G4", "D4",
            "FS4", "A4", "D5", "CS5", "A4", "FS4", "E4", "D4",
            "E4", "G4", "B4", "G4", "FS4", "E4", "D4", "B3",
            "C4", "E4", "G4", "B4", "A4", "G4", "D4", None,
        ),
        "hook_b": (
            "G4", "A4", "B4", "D5", "B4", "A4", "G4", "E4",
            "FS4", "A4", "B4", "A4", "FS4", "E4", "D4", "A3",
            "B3", "E4", "G4", "B4", "A4", "G4", "E4", "D4",
            "E4", "G4", "C5", "B4", "G4", "E4", "D4", None,
        ),
    },
)


def pattern(variant, energy, hook=None, alternate=False):
    rows = [[event() for _ in range(4)] for _ in range(64)]
    roots = variant["roots"]
    for row in range(64):
        root = roots[row // 16]
        position = row % 16

        if row == 0:
            rows[row][0] = event(1, "C2", 0xF, 6)
        elif row % 4 == 0:
            rows[row][0] = event(2 if row % 8 == 4 else 1, "C2")
        elif row % 2 == 1 and (energy > 0 or row >= 17):
            rows[row][0] = event(6, "C3", 0xC, 20 if row % 4 == 1 else 14)

        bass_positions = (0, 4, 8, 12) if energy == 0 else (0, 3, 6, 8, 11, 14)
        if position in bass_positions:
            sequence = variant["bass"][root]
            note = sequence[(position // 3 + (1 if alternate else 0)) & 3]
            rows[row][1] = event(3, note, 0xC, 60 if position % 4 == 0 else 52)

        if row == 0:
            rows[row][2] = event(0, None, 0xF, variant["bpm"])
        elif row % 4 == 2 and (energy > 0 or row >= 18):
            rows[row][2] = event(
                4, variant["chord"][root], 0, variant["arp"][root]
            )
        elif row % 8 == 7 and energy >= 2:
            rows[row][2] = event(
                7, "C2" if variant.get("clean_samples") else "C4",
                0xC, 24
            )

        if hook and row % 2 == 0:
            note = hook[row // 2]
            if note:
                if variant.get("clean_samples"):
                    # Keep every score period inside ProTracker's C-1..B-3
                    # table. The four-cycle sample preserves the audible pitch.
                    note = note[:-1] + str(int(note[-1]) - 2)
                effect = 0x4 if row % 8 == 0 else 0
                rows[row][3] = event(5, note, effect, 0x12 if effect else 0)
    return b"".join(b"".join(row) for row in rows)


def sample_header(name, data, volume, loop):
    if len(data) & 1:
        data += b"\0"
    if loop:
        data = b"\0\0" + data
        loop_start, loop_length = 2, len(data) - 2
    else:
        loop_start, loop_length = 0, 2
    header = (
        name.encode("ascii")[:22].ljust(22, b"\0")
        + struct.pack(">HBBHH", len(data) // 2, 0, volume,
                      loop_start // 2, loop_length // 2)
    )
    return header, data


def build(variant):
    patterns = (
        pattern(variant, 0),
        pattern(variant, 1, variant["hook_a"]),
        pattern(variant, 2, variant["hook_a"]),
        pattern(variant, 2, variant["hook_b"], True),
        pattern(variant, 1, tuple(reversed(variant["hook_a"]))),
        pattern(variant, 2, variant["hook_b"]),
    )
    order = bytes((0, 1, 2, 3, 2, 4, 1, 5, 3, 2, 4, 5))
    headers, blocks = [], []
    samples = CLEAN_SAMPLES if variant.get("clean_samples") else SAMPLES
    for name, data, volume, loop in samples:
        header, block = sample_header(name, data, volume, loop)
        headers.append(header)
        blocks.append(block)
    headers.extend([b"\0" * 30] * (31 - len(headers)))
    module = variant["title"].encode("ascii")[:20].ljust(20, b"\0")
    module += b"".join(headers)
    module += bytes((len(order), 0x7F)) + order.ljust(128, b"\0") + b"M.K."
    module += b"".join(patterns) + b"".join(blocks)
    path = OUTPUT / variant["file"]
    path.write_bytes(module)
    print(f"Wrote {path} ({len(module)} bytes, {variant['bpm']} BPM)")


if __name__ == "__main__":
    OUTPUT.mkdir(parents=True, exist_ok=True)
    for candidate in VARIANTS:
        build(candidate)
