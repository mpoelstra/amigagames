#!/usr/bin/env python3
"""Generate an original four-channel ProTracker MOD for MrDig's ChipSnake."""

import math
import random
import struct
from pathlib import Path


OUTPUT = Path("music/chipsnake-thunder.mod")
SAMPLE_RATE = 8363

PERIODS = {
    "C1": 856, "D1": 762, "EB1": 720, "F1": 640, "G1": 570,
    "AB1": 538, "BB1": 480,
    "C2": 428, "D2": 381, "EB2": 360, "F2": 320, "G2": 285,
    "AB2": 269, "BB2": 240, "C3": 214, "D3": 190, "EB3": 180,
    "F3": 160, "G3": 143, "AB3": 135, "BB3": 120, "C4": 107,
}


def signed_bytes(values):
    return bytes((max(-128, min(127, int(value))) & 0xFF) for value in values)


def pulse(length, duty, harmonics=1):
    values = []
    for i in range(length):
        phase = i / length
        value = 88 if phase < duty else -88
        if harmonics > 1:
            value += 22 if (phase * harmonics) % 1 < 0.5 else -22
        values.append(value)
    return signed_bytes(values)


def triangle(length):
    return signed_bytes(
        100 * (4 * abs(((i / length) + 0.25) % 1 - 0.5) - 1)
        for i in range(length)
    )


def kick():
    values = []
    phase = 0.0
    length = 700
    for i in range(length):
        frequency = 150 - 105 * (i / length)
        phase += frequency / SAMPLE_RATE
        envelope = (1 - i / length) ** 2.4
        values.append(math.sin(phase * math.tau) * 124 * envelope)
    return signed_bytes(values)


def noise_sample(length, seed, decay, tone=0):
    rng = random.Random(seed)
    values = []
    previous = 0
    for i in range(length):
        noise = rng.randint(-127, 127)
        if tone:
            noise = (noise + previous * tone) / (tone + 1)
        previous = noise
        values.append(noise * ((1 - i / length) ** decay))
    return signed_bytes(values)


SAMPLES = [
    ("CHIP LEAD", pulse(128, 0.25, 2), 48, 0, 128),
    ("ROUND BASS", triangle(128), 62, 0, 128),
    ("ARP PULSE", pulse(64, 0.5), 38, 0, 64),
    ("KICK", kick(), 64, 0, 2),
    ("SNARE", noise_sample(900, 2026, 1.7, 2), 52, 0, 2),
    ("HI HAT", noise_sample(240, 68020, 3.2), 35, 0, 2),
]


def event(sample=0, note=None, effect=0, parameter=0):
    period = PERIODS.get(note, 0)
    return bytes((
        (sample & 0xF0) | ((period >> 8) & 0x0F),
        period & 0xFF,
        ((sample & 0x0F) << 4) | (effect & 0x0F),
        parameter & 0xFF,
    ))


def make_pattern(chord, melody, ending=False):
    rows = [[event() for _ in range(4)] for _ in range(64)]
    chord_notes = {
        # ARP PULSE has a 64-sample cycle, one octave above the 128-sample
        # reference waves.  These notes are therefore written an octave down.
        "CM": ("C1", "EB1", "G1"),
        "AB": ("AB1", "C2", "EB2"),
        "BB": ("BB1", "D2", "F2"),
        "FM": ("F1", "AB1", "C2"),
    }[chord]
    bass_notes = {
        "CM": ("C1", "C2", "G1", "BB1"),
        "AB": ("AB1", "AB2", "EB2", "G1"),
        "BB": ("BB1", "BB2", "F2", "AB1"),
        "FM": ("F1", "F2", "C2", "EB2"),
    }[chord]

    for row in range(64):
        if row == 0:
            # The track starts at 125 BPM. Only set speed here, avoiding two
            # Fxx timing commands on the same tracker row.
            rows[row][3] = event(effect=0xF, parameter=4)
            rows[row][2] = event(2, bass_notes[0])
        elif row % 4 == 0:
            bass_step = (row // 4) % 4
            rows[row][2] = event(2, bass_notes[bass_step])
        elif row % 4 == 2:
            # Short offbeat octave/fifth answers give the bass more momentum.
            bass_step = ((row // 4) + 2) % 4
            rows[row][2] = event(2, bass_notes[bass_step],
                                 effect=0xC, parameter=48)

        # A right-channel counter-line alternates chord tones every two rows.
        if row % 2 == 0:
            arp_note = chord_notes[(row // 2) % 3]
            rows[row][1] = event(3, arp_note, effect=0xC, parameter=38)

        step = row // 4
        if row % 4 == 0 and step < len(melody) and melody[step]:
            rows[row][0] = event(1, melody[step])
        elif row % 4 == 2 and step + 1 < len(melody):
            # Quiet anticipations make the lead flow instead of sounding like
            # isolated quarter notes.
            passing_note = melody[step + 1]
            if passing_note:
                rows[row][0] = event(1, passing_note,
                                     effect=0xC, parameter=34)

        if row % 8 == 0 and row != 0:
            rows[row][3] = event(4, "C2")
        elif row % 8 == 4:
            rows[row][3] = event(5, "C3")
        elif row % 2 == 0:
            rows[row][3] = event(6, "C3")

    rows[1][3] = event(effect=0xF, parameter=4)
    if ending:
        rows[56][0] = event(1, "C4")
        rows[60][0] = event(1, "G3")
    return b"".join(b"".join(row) for row in rows)


def sample_header(name, data, volume, loop_start, loop_length):
    if len(data) & 1:
        data += b"\0"
    encoded_name = name.encode("ascii")[:22].ljust(22, b"\0")
    return (
        encoded_name
        + struct.pack(">H", len(data) // 2)
        + bytes((0, volume))
        + struct.pack(">HH", loop_start // 2, max(1, loop_length // 2))
    ), data


def build():
    melodies = [
        # Original heroic call: short-short-long shapes over a descending
        # minor progression, deliberately distinct from the reference tune.
        ["C3", "C3", "G3", "C4", "BB3", "G3", "EB3", "G3",
         "AB3", "AB3", "F3", "C4", "BB3", "AB3", "G3", None],
        ["BB3", "BB3", "F3", "BB3", "C4", "BB3", "G3", "F3",
         "D3", "F3", "G3", "BB3", "AB3", "G3", "F3", "D3"],
        ["AB3", "C4", "AB3", "EB3", "F3", "AB3", "C4", "BB3",
         "AB3", "G3", "F3", "EB3", "C3", "EB3", "G3", "AB3"],
        ["F3", "F3", "C4", "AB3", "G3", "F3", "EB3", "C3",
         "D3", "EB3", "F3", "AB3", "G3", "F3", "EB3", None],
        # Rising fanfare response.
        ["C3", "EB3", "G3", "C4", "G3", "BB3", "C4", "G3",
         "EB3", "G3", "BB3", "C4", "BB3", "G3", "F3", "EB3"],
        ["AB3", "EB3", "AB3", "C4", "AB3", "G3", "EB3", "C3",
         "F3", "G3", "AB3", "C4", "BB3", "AB3", "G3", "F3"],
        ["BB3", "D3", "F3", "BB3", "F3", "G3", "BB3", "C4",
         "BB3", "G3", "F3", "D3", "F3", "G3", "AB3", "BB3"],
        ["C4", "G3", "EB3", "C3", "EB3", "G3", "BB3", "C4",
         "AB3", "F3", "D3", "F3", "G3", "BB3", "C4", None],
        # Final hook variation and answer.
        ["C3", "G3", "C4", "BB3", "G3", "EB3", "F3", "G3",
         "AB3", "C4", "BB3", "AB3", "G3", "F3", "EB3", "G3"],
        ["C4", "BB3", "AB3", "G3", "F3", "EB3", "D3", "C3",
         "G3", "AB3", "BB3", "C4", "G3", "EB3", "C3", None],
    ]
    patterns = [
        make_pattern("CM", melodies[0]),
        make_pattern("BB", melodies[1]),
        make_pattern("AB", melodies[2]),
        make_pattern("FM", melodies[3]),
        make_pattern("CM", melodies[4]),
        make_pattern("AB", melodies[5]),
        make_pattern("BB", melodies[6]),
        make_pattern("CM", melodies[7]),
        make_pattern("CM", melodies[8]),
        make_pattern("FM", melodies[9]),
        make_pattern("AB", melodies[5]),
        make_pattern("BB", melodies[6]),
        make_pattern("CM", melodies[8]),
        make_pattern("CM", melodies[9], True),
    ]
    order = bytes(range(14))

    headers = []
    sample_data = []
    for index, item in enumerate(SAMPLES):
        if index == 0:
            # Reserve a silent first word for compatibility with classic
            # Paula replay routines, then loop the lead waveform after it.
            name, data, volume, loop_start, loop_length = item
            item = (name, b"\0\0" + data, volume,
                    loop_start + 2, loop_length)
        header, data = sample_header(*item)
        headers.append(header)
        sample_data.append(data)
    headers.extend([b"\0" * 30] * (31 - len(headers)))

    module = b"CHIPSNAKE THUNDER".ljust(20, b"\0")
    module += b"".join(headers)
    module += bytes((len(order), 0x7F))
    module += order.ljust(128, b"\0")
    module += b"M.K."
    module += b"".join(patterns)
    module += b"".join(sample_data)

    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.write_bytes(module)


if __name__ == "__main__":
    build()
