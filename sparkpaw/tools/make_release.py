#!/usr/bin/env python3
"""Create campaign HD, two-disk ADF and WHDLoad alpha packages.

The legacy one-level ADF function is retained only as historical tooling.
"""
from __future__ import annotations

import os
import shutil
import subprocess
import sys
import tempfile
import re
from pathlib import Path

from pack_adf_asset import decode as decode_adf_asset
from make_sparkpaw_icon import make_project_icon

ROOT = Path(__file__).resolve().parents[1]
DIST = ROOT / "dist"
STAGE_PARENT = ROOT / "build" / "release"
RELEASE_VERSION = "0.7.0-alpha.2"
ROADMAP_CHECKPOINT = "7A.2"
RELEASE_NAME = f"Sparkpaw-{RELEASE_VERSION}"
STAGE = STAGE_PARENT / RELEASE_NAME
ADF_EXECUTABLE = ROOT / "build" / "sparkpaw-adf"
ADF_FRONT_ASSET = ROOT / "build" / "adf-assets" / "storm-front.spr1"
ADF_REAR_ASSET = ROOT / "build" / "adf-assets" / "storm-rear.spr1"
ADF_STRIDER_ASSET = (
    ROOT / "build" / "adf-assets" / "clockwork-storm-strider.spr1"
)
ADF_PLAYER_ASSET = ROOT / "build" / "adf-assets" / "sparkpaw-sprites4.spr1"
ADF_INTRO_ASSETS = tuple(
    ROOT / "build" / "adf-assets" / name
    for name in (
        "intro1.spr1",
        "intro2.spr1",
        "intro3.spr1",
        "intro4.spr1",
        "intro5.spr1",
    )
)
ADF_STATUS_ASSETS = (
    ROOT / "build" / "adf-assets" / "sparkpaw-level-loading.spr1",
    ROOT / "build" / "adf-assets" / "level-charge-patch.spr1",
    ROOT / "build" / "adf-assets" / "level-ready.spr1",
    ROOT / "build" / "adf-assets" / "level-ready-menu.spr1",
    ROOT / "build" / "adf-assets" / "level-complete.spr1",
)
ADF_RAW_NAMES = {
    "intro1.spr1": "intro1.spbm",
    "intro2.spr1": "intro2.spbm",
    "intro3.spr1": "intro3.spbm",
    "intro4.spr1": "intro4.spbm",
    "intro5.spr1": "intro5.spbm",
    "sparkpaw-level-loading.spr1": "sparkpaw-level-loading.spbm",
    "level-charge-patch.spr1": "level-charge-patch.spbm",
    "level-ready.spr1": "sparkpaw-ready-screen.spbm",
    "level-ready-menu.spr1": "readymenu.spbm",
    "level-complete.spr1": "sparkpaw-level-complete.spbm",
}
LHA = Path(os.environ.get("LHA", ROOT / ".toolchain" / "lha" / "bin" / "lha"))

from campaign_asset_manifest import ALL as CAMPAIGN_RUNTIME_FILES
from runtime_asset_refs import executable_runtime_files

RUNTIME_FILES = tuple(sorted(CAMPAIGN_RUNTIME_FILES))


def validate_amiga_names() -> None:
    overlength = sorted(name for name in (*RUNTIME_FILES, RELEASE_NAME) if len(name) > 30)
    if overlength:
        raise SystemExit(
            f"runtime filenames exceed the Amiga-safe 30-character limit: {overlength}"
        )

RUNTIME_README = f"""Sparkpaw: The Stormstone Quest
=================================
Version {RELEASE_VERSION} - Phase {ROADMAP_CHECKPOINT}
MrDig Productions - Copyright 2026

The full-campaign alpha: play Storm Ruins, recover the Lightning Core,
then continue into the Stormrail Skimmer interlude. Board the Skimmer by
jumping into its cockpit, fly through enemy formations and ruined debris,
collect health and diamonds, and face the Harrier at the sealed gate.

Level 1 results offer REPLAY LEVEL or CONTINUE. Stormrail results offer
REPLAY LEVEL or BACK TO TITLE. Replay stays resident; continuing carries
remaining lives, health and the live diamond meter into Stormrail.
Esc abandons the run and returns to the ready screen. OPTIONS includes a
section start selector for testing and the secondary-button assignment.

Requirements: PAL Amiga A1200 / AGA, 68020 or better, 2 MB Chip + 8 MB Fast RAM.
This package is the complete ordinary HD edition. Copy the entire drawer to
your hard drive and launch Sparkpaw (Workbench icon or Shell). Keep the
assets/runtime directory beside the executable. No installer is needed.

Controls: joystick port 2 or keyboard. Keyboard A/D move, W jumps, S crouches,
and Space shoots/confirms. Joystick directions move; Up jumps, Down crouches,
and Fire shoots/confirms. The secondary joystick button can be
assigned to Jump or Fire in OPTIONS. In flight use directions to steer.
Stop/reset after play; this build has no diagnostic log-save action.

Compared with the public 0.6.0-alpha.68 release: complete Stormrail flight,
formations, health pickups, environmental debris, Harrier finale and results;
connected campaign flow, carried vitals, resident replay and section selection.
The Level-1 renderer retains small tested optimizations, but no noticeable
speed increase is claimed.

Verification: full campaign user testing on FS-UAE/68030 and stock-68020
configuration. This exact game executable matches the accepted HD test build.
Real-A1200 acceptance of this campaign version is still pending. The older
alpha.68 real-hardware results do not establish this version's compatibility.
The intermittent real-Amiga HUD-boundary issue remains open.

This alpha also has a separate two-ADF edition and WHDLoad ZIP/LHA. ADF
starts without the story intro; Disk 2 holds Stormrail. Disk swaps and INSERT
presentation are user-approved in FS-UAE. Real Gotek/Pocket tests and this new
campaign WHDLoad runtime remain pending. Alpha.68 packages remain unchanged.
"""


def make_lha(stage_root: Path, stage_name: str, output: Path) -> Path:
    """Create and CRC-test a genuinely compressed, Amiga-compatible LHA."""
    if not LHA.is_file():
        raise SystemExit(
            f"missing LHA archiver: {LHA}\n"
            "Install classic LHa 1.14i under .toolchain/lha/bin/lha "
            "or set LHA=/absolute/path/to/a creation-capable lha."
        )
    output.unlink(missing_ok=True)
    subprocess.run(
        [str(LHA), "aq2o5", str(output.resolve()), stage_name],
        cwd=stage_root,
        check=True,
    )
    subprocess.run(
        [str(LHA), "tq2", str(output.resolve())],
        check=True,
        stdout=subprocess.DEVNULL,
    )
    if b"-lh5-" not in output.read_bytes():
        raise SystemExit(f"LHA compression verification failed: {output}")
    return output


def copy_runtime() -> None:
    if STAGE_PARENT.exists():
        shutil.rmtree(STAGE_PARENT)
    STAGE.mkdir(parents=True)
    shutil.copy2(ROOT / "sparkpaw", STAGE / "Sparkpaw")
    (STAGE / "Sparkpaw.info").write_bytes(make_project_icon("Sparkpaw", []))
    refs = set(executable_runtime_files(ROOT / "sparkpaw"))
    if refs != set(RUNTIME_FILES):
        raise SystemExit(f"campaign release asset mismatch: {refs ^ set(RUNTIME_FILES)}")
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
    (adf_root / "Sparkpaw.info").unlink()
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
    (adf_root / "assets" / "runtime" / "sparkpaw-sprites4.spbm").unlink()
    shutil.copy2(
        ADF_PLAYER_ASSET,
        adf_root / "assets" / "runtime" / "sparkpaw-sprites4.spr1",
    )
    # The floppy edition deliberately omits the cinematic five-plate intro;
    # HD keeps the complete story sequence. The normal title still leads into
    # LOADING, CHARGING and the shared ready screen on ADF.
    for packed in ADF_INTRO_ASSETS:
        raw_name = ADF_RAW_NAMES[packed.name]
        (adf_root / "assets" / "runtime" / raw_name).unlink()
    for packed in ADF_STATUS_ASSETS:
        raw_name = ADF_RAW_NAMES[packed.name]
        (adf_root / "assets" / "runtime" / raw_name).unlink()
        shutil.copy2(packed, adf_root / "assets" / "runtime" / packed.name)
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
        extract_command = [
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
            "+", "read", "assets/runtime/sparkpaw-sprites4.spr1",
            str(extracted / "sparkpaw-sprites4.spr1"),
        ]
        for packed_path in ADF_STATUS_ASSETS:
            extract_command.extend((
                "+", "read", f"assets/runtime/{packed_path.name}",
                str(extracted / packed_path.name),
            ))
        subprocess.run(extract_command, cwd=ROOT, env=env, check=True)
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
        packed = (extracted / "sparkpaw-sprites4.spr1").read_bytes()
        if packed != ADF_PLAYER_ASSET.read_bytes():
            raise SystemExit("ADF verification failed: sparkpaw-sprites4.spr1")
        if decode_adf_asset(packed) != (
            ROOT / "assets" / "runtime" / "sparkpaw-sprites4.spbm"
        ).read_bytes():
            raise SystemExit(
                "ADF decode verification failed: sparkpaw-sprites4.spr1"
            )
        for packed_path in ADF_STATUS_ASSETS:
            packed = (extracted / packed_path.name).read_bytes()
            if packed != packed_path.read_bytes():
                raise SystemExit(
                    f"ADF verification failed: {packed_path.name}"
                )
            raw_name = ADF_RAW_NAMES[packed_path.name]
            if decode_adf_asset(packed) != (
                ROOT / "assets" / "runtime" / raw_name
            ).read_bytes():
                raise SystemExit(
                    f"ADF decode verification failed: {packed_path.name}"
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
        "tests": ("*.c",),
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
    """Keep protected alpha.68 and archived user evidence intact."""
    DIST.mkdir(exist_ok=True)
    # This HD-first release must not erase the user's original alpha.68 baseline.
    # Subsequent media artifacts are deliberately not manufactured here.


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
    validate_amiga_names()
    include_source = sys.argv[1:] == ["--include-source"]
    if sys.argv[1:] and not include_source:
        raise SystemExit("usage: make_release.py [--include-source]")
    validate_release_identity()
    copy_runtime()
    clean_dist_releases()
    zip_path = Path(shutil.make_archive(
        str(DIST / RELEASE_NAME), "zip", STAGE_PARENT, STAGE.name,
    ))
    review_drawer = DIST / RELEASE_NAME
    if review_drawer.exists():
        shutil.rmtree(review_drawer)
    shutil.copytree(STAGE, review_drawer)
    lha_path = make_lha(STAGE_PARENT, STAGE.name, DIST / f"{RELEASE_NAME}.lha")
    paths = [zip_path, lha_path]
    # Rebuild/read back the same accepted two-disk route; no legacy one-level ADF.
    for tool in ("generate_disk_status.py", "test_multidisk_probe.py",
                 "build_multidisk_probe.py", "package_multidisk_probe.py"):
        subprocess.run([sys.executable, str(ROOT / "tools" / tool)], cwd=ROOT, check=True)
    for disk in (1, 2):
        source = ROOT / "build/multidisk-probe" / f"Sparkpaw-Disk{disk}.adf"
        destination = DIST / f"{RELEASE_NAME}-Disk{disk}.adf"
        shutil.copy2(source, destination)
        assert source.read_bytes() == destination.read_bytes()
        paths.append(destination)
    subprocess.run([sys.executable, str(ROOT / "tools/make_whdload.py")], cwd=ROOT, check=True)
    if include_source:
        paths.append(make_source_zip())
    for path in paths:
        print("Wrote", path)
    print("Prepared", review_drawer)


if __name__ == "__main__":
    main()
