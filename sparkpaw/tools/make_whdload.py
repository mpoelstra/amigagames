#!/usr/bin/env python3
"""Build the versioned Sparkpaw WHDLoad packages."""
from __future__ import annotations

import shutil
import struct
import subprocess
import sys
import time
import zipfile
from pathlib import Path

from make_release import RELEASE_NAME, RELEASE_VERSION, ROADMAP_CHECKPOINT, RUNTIME_FILES
from make_sparkpaw_icon import make_project_icon, make_readme_icon

ROOT = Path(__file__).resolve().parents[1]
DEV = ROOT / ".toolchain" / "whdload-dev" / "WHDLoad"
NDK_INCLUDE = ROOT / ".toolchain" / "ndk" / "Include_I"
VASM = ROOT / ".toolchain" / "sdk" / "bin" / "vasmm68k_mot"
SLAVE_SOURCE = ROOT / "whdload" / "Sparkpaw.asm"
SLAVE = ROOT / "whdload" / "Sparkpaw.Slave"
STAGE_ROOT = ROOT / "build" / "whdload"
STAGE_NAME = f"{RELEASE_NAME}-WHDLoad"
STAGE = STAGE_ROOT / STAGE_NAME
DIST = ROOT / "dist"


def require(path: Path, description: str) -> None:
    if not path.is_file():
        raise SystemExit(f"missing {description}: {path}")


def assemble() -> None:
    require(VASM, "vasm assembler")
    require(DEV / "Include" / "whdload.i", "WHDLoad include files")
    require(NDK_INCLUDE / "exec" / "execbase.i", "NDK assembler includes")
    require(DEV / "Src" / "sources" / "whdload" / "kick31.s", "WHDLoad kick31 source")
    subprocess.run([
        str(VASM), "-m68000", "-Fhunkexe", "-nosym", "-quiet", "-nowarn=62",
        f"-I{ROOT / 'whdload' / 'include'}", f"-I{DEV / 'Include'}",
        f"-I{NDK_INCLUDE}", f"-I{DEV / 'Src' / 'sources'}",
        str(SLAVE_SOURCE), "-o", str(SLAVE),
    ], cwd=ROOT, check=True)


def make_icons() -> None:
    (STAGE / "Sparkpaw.info").write_bytes(make_project_icon(
        "WHDLoad", ["SLAVE=Sparkpaw.Slave", "PRELOAD", "PAL"]
    ))
    (STAGE / "ReadMe.txt.info").write_bytes(make_readme_icon())


def crc16(data: bytes) -> int:
    crc = 0
    for byte in data:
        crc ^= byte
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc & 0xFFFF


def dos_time(timestamp: float) -> int:
    value = time.localtime(timestamp)
    return ((max(1980, min(2107, value.tm_year)) - 1980) << 25 |
            value.tm_mon << 21 | value.tm_mday << 16 | value.tm_hour << 11 |
            value.tm_min << 5 | value.tm_sec // 2)


def lha_member(name: str, data: bytes, timestamp: float) -> bytes:
    encoded = name.encode("latin-1")
    body = (b"-lh0-" + struct.pack("<IIIBBB", len(data), len(data),
            dos_time(timestamp), 0x20, 0, len(encoded)) + encoded +
            struct.pack("<H", crc16(data)))
    return bytes((len(body), sum(body) & 255)) + body + data


def make_lha() -> Path:
    output = DIST / f"{STAGE_NAME}.lha"
    with output.open("wb") as archive:
        for path in sorted(STAGE.rglob("*")):
            if path.is_file():
                relative = path.relative_to(STAGE_ROOT).as_posix()
                archive.write(lha_member(relative, path.read_bytes(), path.stat().st_mtime))
        archive.write(b"\0")
    return output


def main() -> None:
    if sys.argv[1:]:
        raise SystemExit("usage: make_whdload.py")
    executable = ROOT / "build" / "sparkpaw-whdload"
    assemble()
    if STAGE_ROOT.exists():
        shutil.rmtree(STAGE_ROOT)
    data = STAGE / "data"
    (data / "assets" / "runtime").mkdir(parents=True)
    shutil.copy2(SLAVE, STAGE / "Sparkpaw.Slave")
    require(executable, "WHDLoad game executable")
    shutil.copy2(executable, data / "Sparkpaw")
    for name in RUNTIME_FILES:
        source = ROOT / "assets" / "runtime" / name
        require(source, f"runtime asset {name}")
        shutil.copy2(source, data / "assets" / "runtime" / name)
    readme = (ROOT / "whdload" / "ReadMe.txt").read_text(encoding="ascii").format(
        RELEASE_NAME=RELEASE_NAME, RELEASE_VERSION=RELEASE_VERSION,
        ROADMAP_CHECKPOINT=ROADMAP_CHECKPOINT,
    )
    (STAGE / "ReadMe.txt").write_text(readme, encoding="ascii")
    make_icons()
    DIST.mkdir(exist_ok=True)
    zip_path = Path(shutil.make_archive(str(DIST / STAGE_NAME), "zip", STAGE_ROOT, STAGE_NAME))
    lha_path = make_lha()
    with zipfile.ZipFile(zip_path) as archive:
        required = {
            f"{STAGE_NAME}/Sparkpaw.Slave", f"{STAGE_NAME}/Sparkpaw.info",
            f"{STAGE_NAME}/ReadMe.txt", f"{STAGE_NAME}/data/Sparkpaw",
        }
        missing = required.difference(archive.namelist())
        if missing:
            raise SystemExit(f"WHDLoad ZIP verification failed: {sorted(missing)}")
    print("Wrote", zip_path)
    print("Wrote", lha_path)
    print("Prepared", STAGE)


if __name__ == "__main__":
    main()
