#!/usr/bin/env python3
"""Create the small ADF-only SPR1 byte-run container."""
from __future__ import annotations

import argparse
import struct
import zlib
from pathlib import Path

MAGIC = b"SPR1"
HEADER = struct.Struct(">4sIII")


def encode(raw: bytes) -> bytes:
    packed = bytearray()
    offset = 0
    while offset < len(raw):
        run = 1
        while (offset + run < len(raw) and run < 128 and
               raw[offset + run] == raw[offset]):
            run += 1
        if run >= 3:
            packed.extend((0x80 | (run - 1), raw[offset]))
            offset += run
            continue
        literal = offset
        offset += run
        while offset < len(raw) and offset - literal < 128:
            run = 1
            while (offset + run < len(raw) and run < 128 and
                   raw[offset + run] == raw[offset]):
                run += 1
            if run >= 3:
                break
            offset += min(run, 128 - (offset - literal))
        packed.append(offset - literal - 1)
        packed.extend(raw[literal:offset])
    return bytes(packed)


def decode(container: bytes) -> bytes:
    if len(container) < HEADER.size:
        raise ValueError("truncated SPR1 header")
    magic, raw_size, expected_crc, packed_size = HEADER.unpack_from(container)
    payload = container[HEADER.size:]
    if magic != MAGIC or packed_size != len(payload):
        raise ValueError("invalid SPR1 header")
    raw = bytearray()
    offset = 0
    while offset < len(payload):
        token = payload[offset]
        offset += 1
        length = (token & 0x7f) + 1
        if token & 0x80:
            if offset >= len(payload):
                raise ValueError("truncated SPR1 run")
            raw.extend((payload[offset],) * length)
            offset += 1
        else:
            end = offset + length
            if end > len(payload):
                raise ValueError("truncated SPR1 literal")
            raw.extend(payload[offset:end])
            offset = end
        if len(raw) > raw_size:
            raise ValueError("SPR1 output overflow")
    crc = zlib.crc32(raw) & 0xffffffff
    if len(raw) != raw_size or crc != expected_crc:
        raise ValueError("SPR1 size/CRC mismatch")
    return bytes(raw)


def pack(raw: bytes) -> bytes:
    payload = encode(raw)
    container = HEADER.pack(
        MAGIC, len(raw), zlib.crc32(raw) & 0xffffffff, len(payload)
    ) + payload
    if decode(container) != raw:
        raise RuntimeError("SPR1 self-verification failed")
    return container


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("destination", type=Path)
    args = parser.parse_args()
    raw = args.source.read_bytes()
    container = pack(raw)
    args.destination.parent.mkdir(parents=True, exist_ok=True)
    args.destination.write_bytes(container)
    print(f"Packed {len(raw)} -> {len(container)} bytes: {args.destination}")


if __name__ == "__main__":
    main()
