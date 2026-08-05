#!/usr/bin/env python3
"""Create identical HD, LHA, ZIP, source and bootable ADF milestone builds."""
from __future__ import annotations

import os
import shutil
import struct
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DIST = ROOT / "dist"
STAGE_PARENT = ROOT / "build" / "release"
STAGE = STAGE_PARENT / "Sparkpaw-Milestone2A"

RUNTIME_README = """Sparkpaw: The Stormstone Quest
=================================

AGA Milestone 2A enemy vertical slice
MrDig Productions - Copyright 2026

Requirements
------------

Amiga 1200 or compatible AGA Amiga, Motorola 68020, 2 MB Chip RAM.
Fast RAM is recommended but not required by this milestone.

Keep the complete Sparkpaw-Milestone2A drawer together. Start it from Shell:

  CD Sparkpaw-Milestone2A
  Sparkpaw

Controls
--------

Joystick port 2: left/right to run, up to jump and fire to shoot. Every fire
press launches a fast blue/cyan plasma pulse; rapid tapping supports several
pulses in flight. Hold down to crouch; combine down with left/right to
crouch-walk, and press fire to shoot from a dedicated low pose. Four low
clockwork beetles patrol separate parts of the test level. Only crouch-shots
can hit them; fire twice to destroy each one. These enemies cannot damage
Sparkpaw and do not respawn.
Reset the Amiga or emulator to leave this bare-metal milestone. Mouse exit is
disabled so an accidental click cannot interrupt a test.

What to test
------------

Walk through all five screen widths, jump onto and off each platform, reverse
direction often, and compare the movement on HD and ADF. The rear scenery
must move at one quarter of the foreground speed. Sparkpaw now uses fixed
48x48, 15-colour AGA poses for idle, blink, run, jump, fall, landing and
crouching and shooting. After two seconds at rest Sparkpaw performs an idle
flourish. Check that all beetles remain grounded, turn cleanly, ignore standing
and airborne shots, react to the first crouch-shot, and complete their
destruction sequence after the second. Watch for Bob residue when plasma pulses
overlap an enemy or two beetles approach the same screen edge.
Please look for sprite flicker, apparent size changes, foot sliding, tearing,
collision errors, camera jumps and parallax glitches.

This milestone intentionally contains no player damage, random enemy spawning,
music, collectables or menus. It validates one complete enemy vertical slice.
"""


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


def copy_runtime() -> None:
    if STAGE_PARENT.exists():
        shutil.rmtree(STAGE_PARENT)
    STAGE.mkdir(parents=True)
    shutil.copy2(ROOT / "sparkpaw", STAGE / "Sparkpaw")
    for name in (
        "storm-front.spbm", "storm-rear.spbm",
        "storm-collision.bin", "sparkpaw-sprites4.spbm",
        "clockwork-beetle.spbm",
        "energy-shot.raw",
    ):
        target = STAGE / "assets" / "runtime" / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(ROOT / "assets" / "runtime" / name, target)
    (STAGE / "ReadMe.txt").write_text(RUNTIME_README, encoding="ascii")


def make_adf() -> Path:
    adf_root = ROOT / "build" / "adf-root"
    if adf_root.exists():
        shutil.rmtree(adf_root)
    shutil.copytree(STAGE, adf_root)
    (adf_root / "S").mkdir()
    (adf_root / "S" / "startup-sequence").write_text(
        "Sparkpaw\n", encoding="ascii"
    )
    adf = DIST / "Sparkpaw-Milestone2A-A1200.adf"
    amitools = ROOT / ".toolchain" / "amitools"
    if not (amitools / "amitools" / "tools" / "xdftool.py").is_file():
        raise SystemExit("missing independent sparkpaw/.toolchain/amitools")
    env = os.environ.copy()
    env["PYTHONPATH"] = str(amitools)
    command = [
        sys.executable, "-m", "amitools.tools.xdftool", "-f", str(adf),
        "format", "SparkpawM2A", "DOS0", "+", "boot", "install",
    ]
    for directory in ("S", "assets", "assets/runtime"):
        command.extend(("+", "makedir", directory))
    for path in sorted(adf_root.rglob("*")):
        if path.is_file():
            command.extend(("+", "write", str(path), path.relative_to(adf_root).as_posix()))
    subprocess.run(command, cwd=ROOT, env=env, check=True)
    if adf.stat().st_size != 901120:
        raise SystemExit(f"invalid ADF size: {adf.stat().st_size}")
    subprocess.run([
        sys.executable, "-m", "amitools.tools.xdftool", "-r", str(adf),
        "boot", "show", "+", "list",
    ], cwd=ROOT, env=env, check=True)
    return adf


def make_source_zip() -> Path:
    source_root = ROOT / "build" / "source-release"
    source_stage = source_root / "Sparkpaw-Milestone2A-Source"
    if source_root.exists():
        shutil.rmtree(source_root)
    source_stage.mkdir(parents=True)
    for filename in ("Makefile", "README.md"):
        shutil.copy2(ROOT / filename, source_stage / filename)
    for directory, patterns in {
        "src": ("*.c", "*.h", "*.s"),
        "tools": ("*.py",),
        "docs": ("*.md", "*.txt"),
        "assets": ("*.png", "*.iff", "*.json", "*.gpl", "*.spbm", "*.bin"),
        "sfx": ("*.raw", "*.json", "*.bank"),
    }.items():
        for pattern in patterns:
            for source in (ROOT / directory).rglob(pattern):
                target = source_stage / source.relative_to(ROOT)
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(source, target)
    return Path(shutil.make_archive(
        str(DIST / "Sparkpaw-Milestone2A-Source"), "zip",
        source_root, source_stage.name,
    ))


def main() -> None:
    copy_runtime()
    DIST.mkdir(exist_ok=True)
    zip_path = Path(shutil.make_archive(
        str(DIST / "Sparkpaw-Milestone2A-A1200"), "zip", STAGE_PARENT, STAGE.name,
    ))
    lha_path = DIST / "Sparkpaw-Milestone2A-A1200.lha"
    with lha_path.open("wb") as output:
        for path in sorted(STAGE.rglob("*")):
            if path.is_file():
                relative = path.relative_to(STAGE_PARENT).as_posix()
                output.write(lha_member(relative, path.read_bytes(), path.stat().st_mtime))
        output.write(b"\0")
    adf_path = make_adf()
    source_path = make_source_zip()
    for path in (zip_path, lha_path, adf_path, source_path):
        print("Wrote", path)


if __name__ == "__main__":
    main()
