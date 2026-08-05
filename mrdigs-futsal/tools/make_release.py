#!/usr/bin/env python3
"""Build portable ZIP, LHA, and bootable ADF releases."""
import os
import shutil
import struct
import subprocess
import sys
import time
import binascii
from pathlib import Path

root = Path(__file__).resolve().parent.parent
stage = root / "build" / "release" / "MrDigs-Futsal"
dist = root / "dist"
amigainfo = root / ".toolchain" / "amigainfo"

runtime_readme = """MrDig's Futsal for Amiga 1200
=================================

MrDig Productions - Copyright 2026

An original 3-versus-3 indoor arcade football game. Keep the complete
MrDigs-Futsal drawer together because the game loads its title, music and
sound effects from the included subdirectories.

Requirements: Amiga 1200, Motorola 68020, 2 MB Chip RAM. Fast RAM and a hard
disk are recommended.

Start from Workbench by double-clicking the MrDigs-Futsal icon, or from Shell:

  CD MrDigs-Futsal
  MrDigs-Futsal

MATCH SETUP
-----------

Space or player 1 fire opens Match Setup from the title. Use up/down
to select and left/right to change players, 3v3/4v4 team size, court, CPU
level, music, goal limit, time limit, and starting game speed. Fire or Space
starts the match. In 4v4 each team has an autonomous goalkeeper that saves,
parries, and distributes the ball.

CONTROLS
--------

Player 1 controls blue with joystick port 2 or cursor keys/W/A/S/D. In
two-player mode, Player 2 controls red with joystick port 1. Tap fire or Space
to pass; hold and release it to shoot. Fire without possession does nothing;
defensive selection is automatic. Tab switches blue from the keyboard. P pauses. M toggles
in-game music. Escape returns to the title screen; Escape on the title exits.

Keys 1/2/3/4 select the court, 7/8/9 select AI difficulty, and plus/minus adjust
game speed.

The game was developed in C and 68000 assembly with AI-assisted graphics,
music, sound effects and Python asset/build tools. It runs as native AmigaOS
software without a modern game engine.
"""


def load_amigainfo_api():
    try:
        from amigainfo import load, save
        from amigainfo.models import IconType
    except ModuleNotFoundError as error:
        if error.name != "amigainfo":
            raise
        if not amigainfo.is_dir():
            raise SystemExit(
                "missing amigainfo; install amigainfo==0.6.0 in the active "
                "Python environment"
            )
        sys.path.insert(0, str(amigainfo))
        from amigainfo import load, save
        from amigainfo.models import IconType
    return load, save, IconType


def make_readme_icon(destination):
    load, save, IconType = load_amigainfo_api()

    icon = load((root / "futsal.info").read_bytes())
    icon.type = IconType.PROJECT
    icon.default_tool = "MultiView"
    icon.tooltypes = []
    destination.write_bytes(save(icon))
if stage.parent.exists():
    shutil.rmtree(stage.parent)
stage.mkdir(parents=True)
for source, relative in [
    (root/"futsal", "MrDigs-Futsal"),
    (root/"futsal.info", "MrDigs-Futsal.info"),
    (root/"assets/title/mrdigs-futsal-title.iff",
     "assets/title/mrdigs-futsal-title.iff"),
    (root/"assets/options/mrdigs-futsal-options.iff",
     "assets/options/mrdigs-futsal-options.iff"),
    (root/"music/mrdigs-futsal-title.lsmusic",
     "music/mrdigs-futsal-title.lsmusic"),
    (root/"music/mrdigs-futsal-title.lsbank",
     "music/mrdigs-futsal-title.lsbank"),
    (root/"music/mrdigs-futsal-ingame.lsmusic",
     "music/mrdigs-futsal-ingame.lsmusic"),
    (root/"music/mrdigs-futsal-ingame.lsbank",
     "music/mrdigs-futsal-ingame.lsbank"),
    (root/"sfx/futsal-sfx.bank", "sfx/futsal-sfx.bank"),
]:
    target = stage / relative
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, target)
(stage / "ReadMe.txt").write_text(runtime_readme, encoding="ascii")
make_readme_icon(stage / "ReadMe.txt.info")
dist.mkdir(exist_ok=True)
archive = shutil.make_archive(str(dist/"MrDigs-Futsal-A1200"), "zip",
                              stage.parent, stage.name)
print("Wrote", archive)
def crc16(data):
    crc = 0
    for byte in data:
        crc ^= byte
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc & 0xffff

def dos_time(timestamp):
    value = time.localtime(timestamp)
    return ((max(1980, min(2107, value.tm_year))-1980)<<25 |
            value.tm_mon<<21 | value.tm_mday<<16 | value.tm_hour<<11 |
            value.tm_min<<5 | value.tm_sec//2)

def member(name, data, timestamp):
    encoded = name.encode("latin-1")
    body = (b"-lh0-" + struct.pack("<IIIBBB",len(data),len(data),
            dos_time(timestamp),0x20,0,len(encoded)) + encoded +
            struct.pack("<H",crc16(data)))
    return bytes((len(body),sum(body)&255))+body+data

lha_archive = dist / "MrDigs-Futsal-A1200.lha"
with lha_archive.open("wb") as output:
    for path in sorted(stage.rglob("*")):
        if path.is_file():
            relative = path.relative_to(stage.parent).as_posix()
            output.write(member(relative,path.read_bytes(),path.stat().st_mtime))
    output.write(b"\0")
print("Wrote",lha_archive)

# Build an AmigaDOS OFS floppy. A bootable disk only needs a valid DOS
# bootblock and S/startup-sequence; the startup file launches the game from
# the volume root so all relative asset paths continue to work.
adf_root = root / "build" / "adf-root"
if adf_root.exists():
    shutil.rmtree(adf_root)
shutil.copytree(stage, adf_root)
(adf_root / "S").mkdir()
(adf_root / "S" / "startup-sequence").write_text(
    "MrDigs-Futsal\n", encoding="ascii"
)

adf_archive = dist / "MrDigs-Futsal-A1200.adf"
amitools = root / ".toolchain" / "amitools"
if not (amitools / "amitools" / "tools" / "xdftool.py").is_file():
    raise SystemExit(
        "missing local amitools; install it with: "
        f"{sys.executable} -m pip install --target .toolchain/amitools amitools"
    )

env = os.environ.copy()
env["PYTHONPATH"] = str(amitools)
commands = [
    sys.executable, "-m", "amitools.tools.xdftool",
    "-f", str(adf_archive),
    "format", "MrDigsFutsal", "DOS0",
    "+", "boot", "install",
]
for directory in ("S", "assets", "assets/title", "assets/options", "music", "sfx"):
    commands.extend(("+", "makedir", directory))
for path in sorted(adf_root.rglob("*")):
    if path.is_file():
        amiga_path = path.relative_to(adf_root).as_posix()
        commands.extend(("+", "write", str(path), amiga_path))
subprocess.run(commands, cwd=root, env=env, check=True)
if adf_archive.stat().st_size != 901120:
    raise SystemExit(f"invalid ADF size: {adf_archive.stat().st_size}")

# Re-open the finished image read-only to validate its filesystem and bootblock.
subprocess.run(
    [
        sys.executable, "-m", "amitools.tools.xdftool",
        "-r", str(adf_archive), "boot", "show", "+", "list",
    ],
    cwd=root, env=env, check=True,
)
print("Wrote",adf_archive)

# Package the editable, reproducible source separately from runtime releases.
# VBCC and the Amiga NDK are intentionally excluded because they have their
# own distribution terms; README.md explains where they must be installed.
source_root = root / "build" / "source-release"
source_stage = source_root / "MrDigs-Futsal-Source"
if source_root.exists():
    shutil.rmtree(source_root)
source_stage.mkdir(parents=True)
for filename in ("Makefile", "README.md"):
    shutil.copy2(root / filename, source_stage / filename)
for directory, patterns in {
    "src": ("*.c", "*.h", "*.s"),
    "tools": ("*.py",),
    "music": ("*.mod", "*.txt"),
    "assets": ("*.png", "*.iff", "*.wav"),
    "sfx": ("*.json", "*.txt", "*.raw"),
}.items():
    for pattern in patterns:
        for source in (root / directory).rglob(pattern):
            target = source_stage / source.relative_to(root)
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, target)
third_party = root / "third_party" / "LSPlayer"
if third_party.is_dir():
    shutil.copytree(
        third_party,
        source_stage / "third_party" / "LSPlayer",
        ignore=shutil.ignore_patterns(
            ".git", "build", "*.o", "*.obj", ".DS_Store"
        ),
    )
source_archive = shutil.make_archive(
    str(dist / "MrDigs-Futsal-Source"),
    "zip",
    source_root,
    source_stage.name,
)
print("Wrote", source_archive)
