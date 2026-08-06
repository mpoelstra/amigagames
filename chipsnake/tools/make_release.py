#!/usr/bin/env python3
"""Build portable ChipSnake LHA and bootable ADF releases."""

import binascii
import os
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
STAGE = ROOT / "build" / "release" / "ChipSnake"
ARCHIVE = ROOT / "dist" / "ChipSnake-A1200.lha"
ZIP_ARCHIVE = ROOT / "dist" / "ChipSnake-A1200.zip"
SOURCE_ARCHIVE = ROOT / "dist" / "ChipSnake-Source.zip"
ADF_ARCHIVE = ROOT / "dist" / "ChipSnake-A1200.adf"
AMIGAINFO = ROOT / ".toolchain" / "amigainfo"

FILES = {
    "snake": "Snake",
    "snake.info": "Snake.info",
    "assets/intro/chipsnake-intro.iff": "assets/intro/chipsnake-intro.iff",
    "assets/highscore/chipsnake-highscore.iff": "assets/highscore/chipsnake-highscore.iff",
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

README = """ChipSnake for Amiga 1200
========================

MrDig Productions - Copyright 2026

Requirements: Amiga 1200, Motorola 68020, 2 MB Chip RAM.
Fast RAM and a hard disk are recommended.

Keep the complete ChipSnake drawer together. Open it in Workbench and
double-click the Snake icon, or use:

  CD ChipSnake
  Snake

Controls: cursor keys, W/A/S/D, or a joystick in port 2. Joystick fire
and Space start, restart, continue, or pause.
Press H on the title screen for the Hall of Fame. During name entry,
Backspace deletes and Return saves. Escape returns to the title from
gameplay and score screens; Escape on the title exits the program.

The ADF release is bootable. Insert it as DF0: and reset the Amiga;
S/startup-sequence launches ChipSnake automatically.
"""


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
    t = time.localtime(timestamp)
    year = max(1980, min(2107, t.tm_year))
    return (
        ((year - 1980) << 25)
        | (t.tm_mon << 21)
        | (t.tm_mday << 16)
        | (t.tm_hour << 11)
        | (t.tm_min << 5)
        | (t.tm_sec // 2)
    )


def lha_member(name, data, timestamp):
    encoded = name.encode("latin-1")
    body = (
        b"-lh0-"
        + struct.pack("<IIIBBB", len(data), len(data), dos_time(timestamp), 0x20, 0, len(encoded))
        + encoded
        + struct.pack("<H", crc16(data))
    )
    return bytes((len(body), sum(body) & 0xFF)) + body + data


def build_adf():
    adf_root = ROOT / "build" / "adf-root"
    if adf_root.exists():
        shutil.rmtree(adf_root)
    shutil.copytree(STAGE, adf_root)
    (adf_root / "S").mkdir()
    (adf_root / "S" / "startup-sequence").write_text(
        "Snake\n", encoding="ascii"
    )

    amitools = ROOT / ".toolchain" / "amitools"
    if not (amitools / "amitools" / "tools" / "xdftool.py").is_file():
        raise SystemExit(
            "missing local amitools; install it with: "
            f"{sys.executable} -m pip install "
            "--target .toolchain/amitools amitools"
        )

    env = os.environ.copy()
    env["PYTHONPATH"] = str(amitools)
    commands = [
        sys.executable, "-m", "amitools.tools.xdftool",
        "-f", str(ADF_ARCHIVE),
        "format", "ChipSnake", "DOS0",
        "+", "boot", "install",
    ]
    for directory in (
        "S", "assets", "assets/intro", "assets/highscore",
        "backgrounds", "music",
    ):
        commands.extend(("+", "makedir", directory))
    for path in sorted(adf_root.rglob("*")):
        if path.is_file():
            amiga_path = path.relative_to(adf_root).as_posix()
            commands.extend(("+", "write", str(path), amiga_path))
    subprocess.run(commands, cwd=ROOT, env=env, check=True)
    if ADF_ARCHIVE.stat().st_size != 901120:
        raise SystemExit(f"invalid ADF size: {ADF_ARCHIVE.stat().st_size}")

    subprocess.run(
        [
            sys.executable, "-m", "amitools.tools.xdftool",
            "-r", str(ADF_ARCHIVE), "boot", "show", "+", "list",
        ],
        cwd=ROOT, env=env, check=True,
    )
    with tempfile.TemporaryDirectory() as temp_dir:
        extracted = Path(temp_dir)
        subprocess.run(
            [
                sys.executable, "-m", "amitools.tools.xdftool",
                "-r", str(ADF_ARCHIVE),
                "read", "Snake", str(extracted / "Snake"),
                "+", "read", "Snake.info", str(extracted / "Snake.info"),
                "+", "read", "S/startup-sequence",
                str(extracted / "startup-sequence"),
            ],
            cwd=ROOT, env=env, check=True,
        )
        for name, source in (("Snake", ROOT / "snake"),
                             ("Snake.info", ROOT / "snake.info")):
            if (extracted / name).read_bytes() != source.read_bytes():
                raise SystemExit(f"ADF verification failed: {name}")
        if (extracted / "startup-sequence").read_bytes() != b"Snake\n":
            raise SystemExit("ADF verification failed: S/startup-sequence")
    print(ADF_ARCHIVE)


def build_source_archive():
    source_root = ROOT / "build" / "source-release"
    source_stage = source_root / "ChipSnake-Source"
    if source_root.exists():
        shutil.rmtree(source_root)
    source_stage.mkdir(parents=True)

    for filename in ("Makefile", "README.md", ".gitignore"):
        source = ROOT / filename
        if source.is_file():
            shutil.copy2(source, source_stage / filename)

    selections = {
        "src": ("*.c", "*.h", "*.s"),
        "tools": ("*.py",),
        "music": ("*.mod", "*.txt"),
        "backgrounds": ("*.iff", "*.png"),
        "assets": ("*.png", "*.iff"),
    }
    for directory, patterns in selections.items():
        for pattern in patterns:
            for source in (ROOT / directory).rglob(pattern):
                destination = source_stage / source.relative_to(ROOT)
                destination.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(source, destination)

    third_party = ROOT / "third_party" / "LSPlayer"
    if third_party.is_dir():
        shutil.copytree(
            third_party,
            source_stage / "third_party" / "LSPlayer",
            ignore=shutil.ignore_patterns(
                ".git", "build", "*.o", "*.obj", ".DS_Store"
            ),
        )

    archive = shutil.make_archive(
        str(SOURCE_ARCHIVE.with_suffix("")),
        "zip",
        source_root,
        source_stage.name,
    )
    print(archive)


def main():
    if STAGE.parent.exists():
        shutil.rmtree(STAGE.parent)
    STAGE.mkdir(parents=True)
    for source, destination in FILES.items():
        src = ROOT / source
        if not src.is_file():
            raise SystemExit(f"missing runtime file: {source}")
        dst = STAGE / destination
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src, dst)
    (STAGE / "ReadMe.txt").write_text(README, encoding="ascii")
    make_readme_icon(STAGE / "ReadMe.txt.info")

    ARCHIVE.parent.mkdir(parents=True, exist_ok=True)
    zip_path = shutil.make_archive(
        str(ZIP_ARCHIVE.with_suffix("")),
        "zip",
        STAGE.parent,
        STAGE.name,
    )
    print(zip_path)
    with ARCHIVE.open("wb") as archive:
        for path in sorted(STAGE.rglob("*")):
            if path.is_file():
                relative = path.relative_to(STAGE.parent).as_posix()
                archive.write(lha_member(relative, path.read_bytes(), path.stat().st_mtime))
        archive.write(b"\0")
    print(ARCHIVE)
    build_adf()
    build_source_archive()


if __name__ == "__main__":
    main()
