#!/usr/bin/env python3
"""Generate original Amiga-ready sound effects for MrDig's Futsal."""

import json
import math
import random
import struct
import wave
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
RATE = 11025
PERIOD = round(3546895 / RATE)


def pcm(values):
    peak = max(1.0, max(abs(v) for v in values))
    scale = min(1.0, 124.0 / peak)
    result = [max(-128, min(127, round(v * scale))) for v in values]
    result.append(0) if len(result) & 1 else None
    return result


def tone(seconds, frequency, decay=5, sweep=0, harmonics=()):
    phase = 0.0
    out = []
    for i in range(round(seconds * RATE)):
        t = i / RATE
        phase += (frequency + sweep * t) / RATE
        v = math.sin(math.tau * phase)
        for multiplier, level in harmonics:
            v += level * math.sin(math.tau * phase * multiplier)
        out.append(v * 120 * math.exp(-decay * t))
    return out


def noise(seconds, seed, decay=5, lowpass=0.25):
    rng, state, out = random.Random(seed), 0.0, []
    for i in range(round(seconds * RATE)):
        state += (rng.uniform(-1, 1) - state) * lowpass
        out.append(state * 150 * math.exp(-decay * i / RATE))
    return out


def mix(*tracks):
    length = max(map(len, tracks))
    return [sum(track[i] if i < len(track) else 0 for track in tracks)
            for i in range(length)]


def delayed(track, seconds):
    return [0.0] * round(seconds * RATE) + track


def kick():
    out, phase = [], 0.0
    for i in range(round(.19 * RATE)):
        t = i / RATE
        phase += (42 + 190 * math.exp(-28 * t)) / RATE
        click = random.Random(88 + i).uniform(-22, 22) * math.exp(-55 * t)
        out.append(120 * math.sin(math.tau * phase) * math.exp(-15 * t) + click)
    return out


def power_shot():
    return mix(
        [v * 1.1 for v in kick()],
        noise(.32, 68020, 10, .55),
        delayed(tone(.24, 150, 9, -280, ((2, .25),)), .035),
    )


def wall():
    return mix(tone(.13, 930, 22, -900, ((1.53, .5),)),
               noise(.10, 1200, 24, .7))


def bump():
    return mix(tone(.15, 105, 20, -260, ((2, .18),)),
               noise(.07, 3030, 28, .18))


def dribble():
    return mix(tone(.065, 135, 32, -420, ((2, .12),)),
               noise(.028, 68030, 42, .18))


def goal():
    notes = (523.25, 659.25, 783.99, 1046.5)
    return mix(*(delayed(tone(.30, f, 6, 0, ((2, .18),)), i * .13)
                 for i, f in enumerate(notes)))


def whistle():
    out = []
    for i in range(round(.52 * RATE)):
        t = i / RATE
        gate = 1.0 if int(t * 18) % 3 != 2 else .16
        vibrato = 45 * math.sin(math.tau * 7.5 * t)
        out.append(gate * (95 * math.sin(math.tau * (1850 + vibrato) * t)
                           + 28 * math.sin(math.tau * 3700 * t)))
    return out


def crowd():
    src = noise(1.20, 1987, .7, .08)
    return [v * min(1, i / (RATE * .18)) *
            min(1, (len(src) - i) / (RATE * .28)) for i, v in enumerate(src)]


def menu():
    return mix(tone(.08, 880, 15, 2600, ((2, .2),)))


def save():
    return mix(noise(.16, 2026, 16, .6),
               tone(.21, 140, 14, -380, ((2, .22),)))


EFFECTS = [
    ("kick", kick, 64, 4, 4),
    ("power-shot", power_shot, 64, 7, 8),
    ("wall-rebound", wall, 64, 3, 3),
    ("player-bump", bump, 60, 2, 4),
    ("dribble-tap", dribble, 30, 1, 8),
    ("goal", goal, 64, 10, 30),
    ("ref-whistle", whistle, 64, 9, 20),
    ("crowd-cheer", crowd, 64, 8, 45),
    ("menu-select", menu, 56, 1, 3),
    ("keeper-save", save, 64, 6, 8),
]


def build():
    preview_dir = ROOT / "assets" / "sfx" / "previews"
    raw_dir = ROOT / "sfx" / "raw"
    preview_dir.mkdir(parents=True, exist_ok=True)
    raw_dir.mkdir(parents=True, exist_ok=True)
    records, blocks, offset = [], [], 0
    for name, maker, volume, priority, cooldown in EFFECTS:
        values = pcm(maker())
        signed = bytes(v & 255 for v in values)
        unsigned = bytes(v + 128 for v in values)
        (raw_dir / f"{name}.raw").write_bytes(signed)
        with wave.open(str(preview_dir / f"{name}.wav"), "wb") as wav:
            wav.setparams((1, 1, RATE, len(unsigned), "NONE", "not compressed"))
            wav.writeframes(unsigned)
        records.append({
            "name": name, "offset": offset, "length": len(signed),
            "period": PERIOD, "volume": volume, "priority": priority,
            "cooldown_frames": cooldown,
        })
        blocks.append(signed)
        offset += len(signed)

    header_size = 8 + 32 * len(records)
    table = bytearray(b"MDFS" + struct.pack(">HH", 1, len(records)))
    for record in records:
        table += record["name"].encode("ascii")[:16].ljust(16, b"\0")
        table += struct.pack(
            ">IIHBBHH", header_size + record["offset"], record["length"],
            record["period"], record["volume"], record["priority"],
            record["cooldown_frames"], 0)
    bank = bytes(table) + b"".join(blocks)
    (ROOT / "sfx" / "futsal-sfx.bank").write_bytes(bank)
    (ROOT / "sfx" / "futsal-sfx.json").write_text(
        json.dumps({"format": "MDFS v1", "sample_rate": RATE,
                    "effects": records}, indent=2) + "\n")
    print(f"Wrote {len(records)} effects; packed bank is {len(bank)} bytes")


if __name__ == "__main__":
    build()
