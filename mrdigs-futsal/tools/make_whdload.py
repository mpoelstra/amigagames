#!/usr/bin/env python3
"""Build the self-contained MrDig's Futsal WHDLoad prototype package."""

import shutil
import struct
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEV = ROOT / ".toolchain" / "whdload-dev" / "WHDLoad"
NDK_INCLUDE = ROOT / ".toolchain" / "ndk" / "Include_I"
AMIGAINFO = ROOT / ".toolchain" / "amigainfo"
VASM = ROOT / ".toolchain" / "sdk" / "bin" / "vasmm68k_mot"
SLAVE_SOURCE = ROOT / "whdload" / "MrDigsFutsal.asm"
SLAVE = ROOT / "whdload" / "MrDigs-Futsal.Slave"
STAGE_ROOT = ROOT / "build" / "whdload"
STAGE = STAGE_ROOT / "MrDigs-Futsal-WHDLoad"
DIST = ROOT / "dist"

RUNTIME_FILES = {
    "futsal": "MrDigs-Futsal",
    "assets/title/mrdigs-futsal-title.iff":
        "assets/title/mrdigs-futsal-title.iff",
    "assets/options/mrdigs-futsal-options.iff":
        "assets/options/mrdigs-futsal-options.iff",
    "music/mrdigs-futsal-title.lsmusic":
        "music/mrdigs-futsal-title.lsmusic",
    "music/mrdigs-futsal-title.lsbank":
        "music/mrdigs-futsal-title.lsbank",
    "music/mrdigs-futsal-ingame.lsmusic":
        "music/mrdigs-futsal-ingame.lsmusic",
    "music/mrdigs-futsal-ingame.lsbank":
        "music/mrdigs-futsal-ingame.lsbank",
    "music/LICENSE.txt": "music/LICENSE.txt",
    "sfx/futsal-sfx.bank": "sfx/futsal-sfx.bank",
    "sfx/LICENSE.txt": "sfx/LICENSE.txt",
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
    require(
        DEV / "Src" / "sources" / "whdload" / "kick31.s",
        "WHDLoad kick31 source",
    )
    subprocess.run(
        [
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
        ],
        cwd=ROOT,
        check=True,
    )


def make_icon(destination):
    require(ROOT / "futsal.info", "Futsal Workbench icon")
    load, save, IconType = load_amigainfo_api()

    icon = load((ROOT / "futsal.info").read_bytes())
    icon.type = IconType.PROJECT
    icon.default_tool = "WHDLoad"
    icon.tooltypes = [
        "SLAVE=MrDigs-Futsal.Slave",
        "PRELOAD",
        "PAL",
    ]
    destination.write_bytes(save(icon))


def make_readme_icon(destination):
    load, save, IconType = load_amigainfo_api()

    icon = load((ROOT / "futsal.info").read_bytes())
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


def make_lha():
    output = DIST / "MrDigs-Futsal-WHDLoad.lha"
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

    shutil.copy2(SLAVE, STAGE / "MrDigs-Futsal.Slave")
    make_icon(STAGE / "MrDigs-Futsal.info")
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
        str(DIST / "MrDigs-Futsal-WHDLoad"),
        "zip",
        STAGE_ROOT,
        STAGE.name,
    )
    lha_path = make_lha()
    print(SLAVE)
    print(zip_path)
    print(lha_path)


if __name__ == "__main__":
    main()
