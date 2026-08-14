#!/usr/bin/env python3
"""Create identical HD, LHA, ZIP, source and bootable ADF milestone builds."""
from __future__ import annotations

import os
import shutil
import struct
import subprocess
import sys
import tempfile
import time
import re
from pathlib import Path

from pack_adf_asset import decode as decode_adf_asset

ROOT = Path(__file__).resolve().parents[1]
DIST = ROOT / "dist"
STAGE_PARENT = ROOT / "build" / "release"
RELEASE_VERSION = "0.6.0-alpha.1"
ROADMAP_CHECKPOINT = "6B.2"
RELEASE_NAME = f"Sparkpaw-{RELEASE_VERSION}"
STAGE = STAGE_PARENT / RELEASE_NAME
ADF_EXECUTABLE = ROOT / "build" / "sparkpaw-adf"
ADF_FRONT_ASSET = ROOT / "build" / "adf-assets" / "storm-front.spr1"
ADF_REAR_ASSET = ROOT / "build" / "adf-assets" / "storm-rear.spr1"
ADF_STRIDER_ASSET = (
    ROOT / "build" / "adf-assets" / "clockwork-storm-strider.spr1"
)

RUNTIME_FILES = (
    "sparkpaw-title.spbm",
    "sparkpaw-level-loading.spbm",
    "sparkpaw-level-charging.spbm",
    "sparkpaw-hud-base.spbm",
    "sparkpaw-hud-health.spbm",
    "sparkpaw-hud-lives.spbm",
    "sparkpaw-hud-diamonds.spbm",
    "sparkpaw-diamond.spbm",
    "storm-front.spbm",
    "storm-rear.spbm",
    "storm-collision.bin",
    "sparkpaw-sprites4.spbm",
    "clockwork-beetle.spbm",
    "clockwork-storm-strider.spbm",
    "energy-shot.raw",
    "player-hurt.raw",
    "enemy-hit.raw",
    "enemy-death.raw",
    "strider-shot.raw",
    "jump.raw",
    "collect-spark.raw",
)

RUNTIME_README = f"""Sparkpaw: The Stormstone Quest
=================================

AGA alpha {RELEASE_VERSION}
Roadmap checkpoint: Phase {ROADMAP_CHECKPOINT} water-mechanics review
MrDig Productions - Copyright 2026

Phase 6B.2 mechanical water fall/restart and the opaque two-line HUD boundary
are accepted. Visual water presentation remains pending concept approval.

Requirements
------------

Amiga 1200 or compatible AGA Amiga, Motorola 68020, 2 MB Chip RAM and
8 MB Fast RAM.

Keep the complete {RELEASE_NAME} drawer together. Start it from Shell:

  CD {RELEASE_NAME}
  Sparkpaw

Controls
--------

Joystick port 2: left/right to run, up to jump and fire to shoot. Every fire
press launches a fast blue/cyan plasma pulse; rapid tapping supports several
pulses in flight. Hold down to crouch; combine down with left/right to
crouch-walk, and press fire to shoot from a dedicated low pose. Four guaranteed
and up to two optional low clockwork beetles patrol separate parts of the test
level with bounded position and speed variation. Only crouch-shots
can hit them; fire twice to destroy each one. Beetle contact removes one of
six internal half-heart health units, applies knockback and plays a short hurt
effect. Destroyed beetles can return indefinitely after a cooldown once their
patrol zone is safely off-screen. Reaching the far-right world edge resets the
current test level in memory so it can be replayed without rebooting.
A fixed full-width HUD band at the bottom shows the six health units as three
full, half or empty hearts. The Sparkpaw-head counter starts at x3, decreases on
each zero-health reset and temporarily cycles to x3 after the third loss until
the later game-over state is implemented. Twenty hovering diamonds form short
trails and original arcs throughout the level; the adjacent two-digit HUD
counter records them up to 49. Contact invulnerability is shown by a brief
whole-character blink.
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

Two non-interactive Clockwork Storm Striders patrol independently: one starts
on a raised platform and one on the floor. The raised Strider pauses in two
cyan compression stages at its right endpoint, jumps to the adjacent lower
floor, lands and recovers before resuming patrol. They do not damage Sparkpaw
or absorb shots yet. Check grounding, the complete jump and stale 64x64 pixels
while moving them on/off screen and restarting the level. Music remains outside
this milestone.
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
    for name in RUNTIME_FILES:
        target = STAGE / "assets" / "runtime" / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(ROOT / "assets" / "runtime" / name, target)
    (STAGE / "ReadMe.txt").write_text(RUNTIME_README, encoding="ascii")


def make_adf() -> Path:
    adf_root = ROOT / "build" / "adf-root"
    if adf_root.exists():
        shutil.rmtree(adf_root)
    shutil.copytree(STAGE, adf_root)
    shutil.copy2(ADF_EXECUTABLE, adf_root / "Sparkpaw")
    (adf_root / "assets" / "runtime" / "storm-front.spbm").unlink()
    shutil.copy2(
        ADF_FRONT_ASSET,
        adf_root / "assets" / "runtime" / "storm-front.spr1",
    )
    (adf_root / "assets" / "runtime" / "storm-rear.spbm").unlink()
    shutil.copy2(
        ADF_REAR_ASSET,
        adf_root / "assets" / "runtime" / "storm-rear.spr1",
    )
    (adf_root / "assets" / "runtime" /
     "clockwork-storm-strider.spbm").unlink()
    shutil.copy2(
        ADF_STRIDER_ASSET,
        adf_root / "assets" / "runtime" /
        "clockwork-storm-strider.spr1",
    )
    (adf_root / "S").mkdir()
    (adf_root / "S" / "startup-sequence").write_text(
        "Sparkpaw\n", encoding="ascii"
    )
    adf = DIST / f"{RELEASE_NAME}.adf"
    amitools = ROOT / ".toolchain" / "amitools"
    if not (amitools / "amitools" / "tools" / "xdftool.py").is_file():
        raise SystemExit("missing independent sparkpaw/.toolchain/amitools")
    env = os.environ.copy()
    env["PYTHONPATH"] = str(amitools)
    command = [
        sys.executable, "-m", "amitools.tools.xdftool", "-f", str(adf),
        # FFS is native to the target A1200. The ADF-only executable decodes
        # storm-front.spr1; HD archives retain ordinary storm-front.spbm.
        "format", "SparkpawM2A", "DOS1", "+", "boot", "install",
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
    with tempfile.TemporaryDirectory() as temp_dir:
        extracted = Path(temp_dir)
        subprocess.run([
            sys.executable, "-m", "amitools.tools.xdftool", "-r", str(adf),
            "read", "Sparkpaw", str(extracted / "Sparkpaw"),
            "+", "read", "S/startup-sequence",
            str(extracted / "startup-sequence"),
            "+", "read", "assets/runtime/storm-front.spr1",
            str(extracted / "storm-front.spr1"),
            "+", "read", "assets/runtime/storm-rear.spr1",
            str(extracted / "storm-rear.spr1"),
            "+", "read", "assets/runtime/clockwork-storm-strider.spr1",
            str(extracted / "clockwork-storm-strider.spr1"),
        ], cwd=ROOT, env=env, check=True)
        if (extracted / "Sparkpaw").read_bytes() != ADF_EXECUTABLE.read_bytes():
            raise SystemExit("ADF verification failed: Sparkpaw")
        if (extracted / "startup-sequence").read_bytes() != b"Sparkpaw\n":
            raise SystemExit("ADF verification failed: S/startup-sequence")
        packed = (extracted / "storm-front.spr1").read_bytes()
        if packed != ADF_FRONT_ASSET.read_bytes():
            raise SystemExit("ADF verification failed: storm-front.spr1")
        if decode_adf_asset(packed) != (
            ROOT / "assets" / "runtime" / "storm-front.spbm"
        ).read_bytes():
            raise SystemExit("ADF decode verification failed: storm-front.spr1")
        packed = (extracted / "storm-rear.spr1").read_bytes()
        if packed != ADF_REAR_ASSET.read_bytes():
            raise SystemExit("ADF verification failed: storm-rear.spr1")
        if decode_adf_asset(packed) != (
            ROOT / "assets" / "runtime" / "storm-rear.spbm"
        ).read_bytes():
            raise SystemExit("ADF decode verification failed: storm-rear.spr1")
        packed = (extracted / "clockwork-storm-strider.spr1").read_bytes()
        if packed != ADF_STRIDER_ASSET.read_bytes():
            raise SystemExit(
                "ADF verification failed: clockwork-storm-strider.spr1"
            )
        if decode_adf_asset(packed) != (
            ROOT / "assets" / "runtime" / "clockwork-storm-strider.spbm"
        ).read_bytes():
            raise SystemExit(
                "ADF decode verification failed: clockwork-storm-strider.spr1"
            )
    return adf


def make_source_zip() -> Path:
    source_root = ROOT / "build" / "source-release"
    source_stage = source_root / f"{RELEASE_NAME}-Source"
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
        str(DIST / f"{RELEASE_NAME}-Source"), "zip",
        source_root, source_stage.name,
    ))


def clean_dist_releases() -> None:
    """Keep dist reviewable: one current, consistently versioned release set."""
    DIST.mkdir(exist_ok=True)
    for path in DIST.iterdir():
        if path.is_file() and path.name.startswith("Sparkpaw-"):
            path.unlink()


def validate_release_identity() -> None:
    """Prevent packaged SemVer and the numbered roadmap phase from drifting."""
    version_match = re.fullmatch(r"0\.(\d+)\.\d+-[0-9A-Za-z.-]+", RELEASE_VERSION)
    phase_match = re.fullmatch(r"(\d+)[A-Z](?:\.\d+)?", ROADMAP_CHECKPOINT)
    if version_match is None:
        raise ValueError(f"invalid Sparkpaw prerelease version: {RELEASE_VERSION}")
    if phase_match is None:
        raise ValueError(f"invalid Sparkpaw roadmap checkpoint: {ROADMAP_CHECKPOINT}")
    if version_match.group(1) != phase_match.group(1):
        raise ValueError(
            "release/roadmap mismatch: "
            f"{RELEASE_VERSION} does not belong to Phase {ROADMAP_CHECKPOINT}"
        )


def main() -> None:
    validate_release_identity()
    copy_runtime()
    clean_dist_releases()
    zip_path = Path(shutil.make_archive(
        str(DIST / RELEASE_NAME), "zip", STAGE_PARENT, STAGE.name,
    ))
    lha_path = DIST / f"{RELEASE_NAME}.lha"
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
