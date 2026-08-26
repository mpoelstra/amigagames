#!/usr/bin/env python3
"""Generate a short signed 8-bit Paula Stormstone collection flourish."""
from pathlib import Path
import math
import random

RATE = 11025
DURATION = 0.82
OUT = Path(__file__).resolve().parents[1] / "sfx/raw/stormstone-core.raw"

rng = random.Random(0x53544F52)
samples = bytearray()
for index in range(int(RATE * DURATION)):
    t = index / RATE
    env = min(1.0, t / 0.018) * max(0.0, 1.0 - t / DURATION) ** 1.7
    sweep = 310.0 + 880.0 * (t / DURATION)
    crystal = math.sin(2 * math.pi * sweep * t)
    octave = math.sin(2 * math.pi * sweep * 2.01 * t + 0.7)
    shimmer = math.sin(2 * math.pi * 2380 * t) * math.exp(-7.0 * t)
    thunder = (rng.random() * 2 - 1) * math.exp(-14.0 * t)
    value = int(max(-127, min(127, 92 * env *
                (0.58 * crystal + 0.24 * octave + 0.12 * shimmer +
                 0.06 * thunder))))
    samples.append(value & 0xFF)
if len(samples) & 1:
    samples.append(0)
OUT.parent.mkdir(parents=True, exist_ok=True)
OUT.write_bytes(samples)
print(f"wrote {OUT} ({len(samples)} bytes)")
