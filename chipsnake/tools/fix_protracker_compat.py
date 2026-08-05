#!/usr/bin/env python3
"""Keep ChipSnake MOD periods inside classic ProTracker's C-1..B-3 range.

For every instrument that uses an extended period (< 113), all of that
instrument's periods are moved down one or more octaves. Its waveform is sped
up by the matching power of two, preserving the audible pitch and melody.
"""

from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
MODULES = (
    "chipsnake-title.mod",
    "chipsnake-circuit.mod",
    "chipsnake-turbo.mod",
    "chipsnake-thunder.mod",
    "chipsnake-neon.mod",
    "chipsnake-hall.mod",
)
MIN_CLASSIC_PERIOD = 113


def sample_number(event):
    return (event[0] & 0xF0) | (event[2] >> 4)


def period(event):
    return ((event[0] & 0x0F) << 8) | event[1]


def set_period(event, value):
    event[0] = (event[0] & 0xF0) | ((value >> 8) & 0x0F)
    event[1] = value & 0xFF


def accelerate(data, start, length, factor):
    """Put `factor` waveform cycles in the same loop/sample duration."""
    if length <= 2:
        start, length = 0, len(data)
    source = data[start:start + length]
    if not source:
        return
    data[start:start + length] = bytes(
        source[(index * factor) % length] for index in range(length)
    )


def fix(path):
    data = bytearray(path.read_bytes())
    if len(data) < 1084 or data[1080:1084] != b"M.K.":
        raise SystemExit(f"{path}: not a four-channel ProTracker module")

    pattern_count = max(data[952:1080]) + 1
    score_start = 1084
    score_end = score_start + pattern_count * 1024
    factors = {}

    for offset in range(score_start, score_end, 4):
        event = data[offset:offset + 4]
        number = sample_number(event)
        value = period(event)
        if number and value and value < MIN_CLASSIC_PERIOD:
            factor = 1
            while value * factor < MIN_CLASSIC_PERIOD:
                factor *= 2
            factors[number] = max(factors.get(number, 1), factor)

    if not factors:
        print(f"{path.name}: already classic ProTracker compatible")
        return

    for offset in range(score_start, score_end, 4):
        event = data[offset:offset + 4]
        number = sample_number(event)
        value = period(event)
        if number in factors and value:
            set_period(event, value * factors[number])
            data[offset:offset + 4] = event

    sample_offset = score_end
    for number in range(1, 32):
        header = 20 + (number - 1) * 30
        length = int.from_bytes(data[header + 22:header + 24], "big") * 2
        loop_start = int.from_bytes(
            data[header + 26:header + 28], "big"
        ) * 2
        loop_length = int.from_bytes(
            data[header + 28:header + 30], "big"
        ) * 2
        if number in factors:
            block = data[sample_offset:sample_offset + length]
            accelerate(block, loop_start, loop_length, factors[number])
            data[sample_offset:sample_offset + length] = block
        sample_offset += length

    path.write_bytes(data)
    details = ", ".join(
        f"instrument {number} x{factor}"
        for number, factor in sorted(factors.items())
    )
    print(f"{path.name}: fixed {details}")


def main():
    for filename in MODULES:
        fix(ROOT / "music" / filename)


if __name__ == "__main__":
    main()
