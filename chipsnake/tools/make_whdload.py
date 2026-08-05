#!/usr/bin/env python3
"""Build the ChipSnake WHDLoad prototype package."""

import shutil
import struct
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEV = ROOT / ".toolchain" / "whdload-dev" / "WHDLoad"
NDK_INCLUDE = ROOT / ".toolchain" / "ndk" / "Include_I"
VASM = ROOT / ".toolchain" / "sdk" / "bin" / "vasmm68k_mot"
SLAVE_SOURCE = ROOT / "whdload" / "ChipSnake.asm"
SLAVE = ROOT / "whdload" / "ChipSnake.Slave"
STAGE_ROOT = ROOT / "build" / "whdload"
STAGE = STAGE_ROOT / "ChipSnake-WHDLoad"
DIST = ROOT / "dist"
AMIGAINFO = ROOT / ".toolchain" / "amigainfo"

RUNTIME_FILES = {
    "snake": "Snake",
    "assets/intro/chipsnake-intro.iff": "assets/intro/chipsnake-intro.iff",
    "assets/highscore/chipsnake-highscore.iff":
        "assets/highscore/chipsnake-highscore.iff",
    "backgrounds/boing.iff": "backgrounds/boing.iff",
    "backgrounds/circuit.iff": "backgrounds/circuit.iff",
    "backgrounds/workbench.iff": "backgrounds/workbench.iff",
    "music/chipsnake-neon.lsmusic": "music/chipsnake-neon.lsmusic",
    "music/chipsnake-neon.lsbank": "music/chipsnake-neon.lsbank",
    "music/chipsnake-hall.lsmusic": "music/chipsnake-hall.lsmusic",
    "music/chipsnake-hall.lsbank": "music/chipsnake-hall.lsbank",
    "music/chipsnake-circuit.lsmusic": "music/chipsnake-circuit.lsmusic",
    "music/chipsnake-circuit.lsbank": "music/chipsnake-circuit.lsbank",
    "music/chipsnake-turbo.lsmusic": "music/chipsnake-turbo.lsmusic",
    "music/chipsnake-turbo.lsbank": "music/chipsnake-turbo.lsbank",
    "music/LICENSE.txt": "music/LICENSE.txt",
}


def load_amigainfo_api():
    try:
        from amigainfo import load, save
        from amigainfo.models import IconType
    except ModuleNotFoundError as error:
        if error.name != "amigainfo":
            raise
        if not AMIGAINFO.is_dir():
            raise SystemExit(
                "missing amigainfo; install amigainfo==0.6.0 in the active "
                "Python environment"
            )
        sys.path.insert(0, str(AMIGAINFO))
        from amigainfo import load, save
        from amigainfo.models import IconType
    return load, save, IconType


def require(path, description):
    if not path.is_file():
        raise SystemExit(f"missing {description}: {path}")


def assemble():
    require(VASM, "vasm assembler")
    require(DEV / "Include" / "whdload.i", "WHDLoad include files")
    require(NDK_INCLUDE / "exec" / "execbase.i", "NDK assembler includes")
    require(DEV / "Src" / "sources" / "whdload" / "kick31.s",
            "WHDLoad kick31 source")
    command = [
        str(VASM),
        "-m68000",
        "-Fhunkexe",
        "-nosym",
        "-quiet",
        "-nowarn=62",
        f"-I{ROOT / 'whdload' / 'include'}",
        f"-I{DEV / 'Include'}",
        f"-I{NDK_INCLUDE}",
        f"-I{DEV / 'Src' / 'sources'}",
        str(SLAVE_SOURCE),
        "-o",
        str(SLAVE),
    ]
    subprocess.run(command, cwd=ROOT, check=True)


def make_icon(destination):
    require(ROOT / "snake.info", "ChipSnake Workbench icon")
    load, save, IconType = load_amigainfo_api()

    icon = load((ROOT / "snake.info").read_bytes())
    icon.type = IconType.PROJECT
    icon.default_tool = "WHDLoad"
    icon.tooltypes = [
        "SLAVE=ChipSnake.Slave",
        "PRELOAD",
        "PAL",
    ]
    destination.write_bytes(save(icon))


def make_readme_icon(destination):
    load, save, IconType = load_amigainfo_api()

    icon = load((ROOT / "snake.info").read_bytes())
    icon.type = IconType.PROJECT
    icon.default_tool = "MultiView"
    icon.tooltypes = []
    destination.write_bytes(save(icon))


def crc16(data):
    crc = 0
    for byte in data:
        crc ^= byte
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc & 0xFFFF


def dos_time(timestamp):
    value = time.localtime(timestamp)
    year = max(1980, min(2107, value.tm_year))
    return (
        ((year - 1980) << 25)
        | (value.tm_mon << 21)
        | (value.tm_mday << 16)
        | (value.tm_hour << 11)
        | (value.tm_min << 5)
        | (value.tm_sec // 2)
    )


def lha_member(name, data, timestamp):
    encoded = name.encode("latin-1")
    body = (
        b"-lh0-"
        + struct.pack(
            "<IIIBBB",
            len(data), len(data), dos_time(timestamp), 0x20, 0, len(encoded),
        )
        + encoded
        + struct.pack("<H", crc16(data))
    )
    return bytes((len(body), sum(body) & 0xFF)) + body + data


def make_archive():
    output = DIST / "ChipSnake-WHDLoad.lha"
    with output.open("wb") as archive:
        for path in sorted(STAGE.rglob("*")):
            if path.is_file():
                relative = path.relative_to(STAGE_ROOT).as_posix()
                archive.write(
                    lha_member(relative, path.read_bytes(), path.stat().st_mtime)
                )
        archive.write(b"\0")
    return output


def main():
    assemble()
    if STAGE_ROOT.exists():
        shutil.rmtree(STAGE_ROOT)
    (STAGE / "data").mkdir(parents=True)

    shutil.copy2(SLAVE, STAGE / "ChipSnake.Slave")
    make_icon(STAGE / "ChipSnake.info")
    shutil.copy2(ROOT / "whdload" / "ReadMe.txt", STAGE / "ReadMe.txt")
    make_readme_icon(STAGE / "ReadMe.txt.info")
    for source, destination in RUNTIME_FILES.items():
        src = ROOT / source
        require(src, f"runtime file {source}")
        dst = STAGE / "data" / destination
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src, dst)

    DIST.mkdir(parents=True, exist_ok=True)
    zip_path = shutil.make_archive(
        str(DIST / "ChipSnake-WHDLoad"),
        "zip",
        STAGE_ROOT,
        STAGE.name,
    )
    lha_path = make_archive()
    print(SLAVE)
    print(zip_path)
    print(lha_path)


if __name__ == "__main__":
    main()
