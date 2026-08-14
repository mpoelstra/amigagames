#!/usr/bin/env python3
"""Measure ADF-only packing opportunities without changing release assets."""
from __future__ import annotations

import argparse
import json
import lzma
import zlib
from dataclasses import asdict, dataclass
from pathlib import Path

from make_release import ROOT, RUNTIME_FILES, RUNTIME_README

BLOCK_BYTES = 512
POINTERS_PER_BLOCK = BLOCK_BYTES // 4 - 56
ADF_BLOCKS = 1760
RESERVED_BOOT_BLOCKS = 2
ROOT_BLOCKS = 1
BITMAP_BLOCKS = 1
DIRECTORY_BLOCKS = 3  # S, assets and assets/runtime
FIXED_BLOCKS = RESERVED_BOOT_BLOCKS + ROOT_BLOCKS + BITMAP_BLOCKS + DIRECTORY_BLOCKS
OUTPUT_DIR = ROOT / "build" / "adf-report"


@dataclass(frozen=True)
class Measurement:
    path: str
    raw_bytes: int
    crc32: str
    raw_ffs_blocks: int
    zlib_bytes: int
    zlib_crc32: str
    zlib_ffs_blocks: int
    lzma_bytes: int
    lzma_crc32: str
    lzma_ffs_blocks: int


def ffs_file_blocks(size: int) -> int:
    """Return DOS1 blocks: one header, data blocks and pointer extensions."""
    data_blocks = (size + BLOCK_BYTES - 1) // BLOCK_BYTES
    extension_blocks = 0
    if data_blocks > POINTERS_PER_BLOCK:
        extension_blocks = (
            data_blocks - POINTERS_PER_BLOCK + POINTERS_PER_BLOCK - 1
        ) // POINTERS_PER_BLOCK
    return 1 + data_blocks + extension_blocks


def payloads() -> list[tuple[str, bytes]]:
    files = [("Sparkpaw", (ROOT / "sparkpaw").read_bytes())]
    files.extend(
        (f"assets/runtime/{name}", (ROOT / "assets" / "runtime" / name).read_bytes())
        for name in RUNTIME_FILES
    )
    files.extend((
        ("ReadMe.txt", RUNTIME_README.encode("ascii")),
        ("S/startup-sequence", b"Sparkpaw\n"),
    ))
    return files


def measure(path: str, raw: bytes) -> Measurement:
    zlib_data = zlib.compress(raw, level=9)
    lzma_data = lzma.compress(raw, format=lzma.FORMAT_XZ, preset=9)
    zlib_decoded = zlib.decompress(zlib_data)
    lzma_decoded = lzma.decompress(lzma_data, format=lzma.FORMAT_XZ)
    if zlib_decoded != raw or lzma_decoded != raw:
        raise RuntimeError(f"decode verification failed for {path}")
    raw_crc = f"{zlib.crc32(raw) & 0xffffffff:08x}"
    return Measurement(
        path=path,
        raw_bytes=len(raw),
        crc32=raw_crc,
        raw_ffs_blocks=ffs_file_blocks(len(raw)),
        zlib_bytes=len(zlib_data),
        zlib_crc32=f"{zlib.crc32(zlib_decoded) & 0xffffffff:08x}",
        zlib_ffs_blocks=ffs_file_blocks(len(zlib_data)),
        lzma_bytes=len(lzma_data),
        lzma_crc32=f"{zlib.crc32(lzma_decoded) & 0xffffffff:08x}",
        lzma_ffs_blocks=ffs_file_blocks(len(lzma_data)),
    )


def totals(rows: list[Measurement], prefix: str) -> dict[str, int]:
    byte_field = f"{prefix}_bytes"
    block_field = f"{prefix}_ffs_blocks"
    payload_blocks = sum(getattr(row, block_field) for row in rows)
    return {
        "bytes": sum(getattr(row, byte_field) for row in rows),
        "file_blocks": payload_blocks,
        "total_blocks": FIXED_BLOCKS + payload_blocks,
        "margin_blocks": ADF_BLOCKS - FIXED_BLOCKS - payload_blocks,
    }


def markdown(report: dict[str, object]) -> str:
    summary = report["summary"]
    lines = [
        "# Sparkpaw ADF-only storage measurement",
        "",
        "This host report is diagnostic only. zlib-9 and LZMA-9/XZ are ratio",
        "proxies, not selected Amiga codecs. HD ZIP/LHA files and runtime assets",
        "remain unchanged.",
        "",
        f"DOS1 projection: {ADF_BLOCKS} total 512-byte blocks; {FIXED_BLOCKS}",
        "fixed boot/root/bitmap/directory blocks. Each file includes its header,",
        f"data and one extension per {POINTERS_PER_BLOCK} data pointers.",
        "",
        "| Variant | Bytes | File blocks | Total blocks | Margin |",
        "|---|---:|---:|---:|---:|",
    ]
    for key, label in (("raw", "Raw current ADF"), ("zlib", "zlib-9 proxy"),
                       ("lzma", "LZMA-9/XZ proxy")):
        item = summary[key]
        lines.append(
            f"| {label} | {item['bytes']} | {item['file_blocks']} | "
            f"{item['total_blocks']} | {item['margin_blocks']} |"
        )
    lines.extend((
        "",
        "| Path | Raw | CRC32 | Raw blocks | zlib-9 | zlib blocks | LZMA-9 | LZMA blocks |",
        "|---|---:|---|---:|---:|---:|---:|---:|",
    ))
    for row in report["files"]:
        lines.append(
            f"| `{row['path']}` | {row['raw_bytes']} | `{row['crc32']}` | "
            f"{row['raw_ffs_blocks']} | {row['zlib_bytes']} | "
            f"{row['zlib_ffs_blocks']} | {row['lzma_bytes']} | "
            f"{row['lzma_ffs_blocks']} |"
        )
    lines.extend((
        "",
        "All proxy streams were decoded on the host and compared byte-for-byte;",
        "the recorded decode CRC32 must equal the raw CRC32.",
        "",
    ))
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output-dir", type=Path, default=OUTPUT_DIR)
    args = parser.parse_args()
    rows = [measure(path, raw) for path, raw in payloads()]
    report = {
        "schema": 1,
        "scope": "adf-only-measurement",
        "block_model": {
            "block_bytes": BLOCK_BYTES,
            "adf_blocks": ADF_BLOCKS,
            "pointers_per_header_or_extension": POINTERS_PER_BLOCK,
            "fixed_blocks": FIXED_BLOCKS,
        },
        "summary": {name: totals(rows, name) for name in ("raw", "zlib", "lzma")},
        "files": [asdict(row) for row in rows],
    }
    args.output_dir.mkdir(parents=True, exist_ok=True)
    json_path = args.output_dir / "adf-storage-report.json"
    markdown_path = args.output_dir / "adf-storage-report.md"
    json_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    markdown_path.write_text(markdown(report), encoding="utf-8")
    print(markdown(report))
    print(f"Wrote {json_path}")
    print(f"Wrote {markdown_path}")


if __name__ == "__main__":
    main()
