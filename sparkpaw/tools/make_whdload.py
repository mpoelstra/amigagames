#!/usr/bin/env python3
"""Build the versioned Sparkpaw WHDLoad packages."""
from __future__ import annotations

import shutil
import subprocess
import sys
import zipfile
from pathlib import Path

from make_release import (
    RELEASE_NAME,
    RELEASE_VERSION,
    ROADMAP_CHECKPOINT,
    RUNTIME_FILES,
    make_lha,
)
from make_sparkpaw_icon import make_project_icon, make_readme_icon

ROOT = Path(__file__).resolve().parents[1]
DEV = ROOT / ".toolchain" / "whdload-dev" / "WHDLoad"
NDK_INCLUDE = ROOT / ".toolchain" / "ndk" / "Include_I"
VASM = ROOT / ".toolchain" / "sdk" / "bin" / "vasmm68k_mot"
SLAVE_SOURCE = ROOT / "whdload" / "Sparkpaw.asm"
SLAVE = ROOT / "whdload" / "Sparkpaw.Slave"
STAGE_ROOT = ROOT / "build" / "whdload"
ARCHIVE_NAME = f"{RELEASE_NAME}-WHDLoad"
STAGE_NAME = f"Sparkpaw-{RELEASE_VERSION.replace('-alpha.', '-a')}-WHDLoad"
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


def main() -> None:
    global STAGE_ROOT, STAGE_NAME, STAGE
    diagnostic = sys.argv[1:] == ["--intro-diagnostic"]
    if sys.argv[1:] and not diagnostic:
        raise SystemExit("usage: make_whdload.py [--intro-diagnostic]")
    if diagnostic:
        executable = ROOT / "build" / "sparkpaw-whdload-introdiag"
        STAGE_ROOT = ROOT / "build" / "whdload-introdiag"
        STAGE_NAME = "Sparkpaw-WHDIntroDiag"
        STAGE = STAGE_ROOT / STAGE_NAME
    else:
        executable = ROOT / "build" / "sparkpaw-whdload"
    if not diagnostic:
        from runtime_asset_refs import executable_runtime_files
        assert set(executable_runtime_files(executable)) == set(RUNTIME_FILES)
        assert f"Version {RELEASE_VERSION}" in SLAVE_SOURCE.read_text()
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
    if diagnostic:
        readme = """Sparkpaw alpha.55 WHDLoad intro diagnostic
==============================================

Real-A1200 focused test only. Launch Sparkpaw from its Workbench icon and do
not skip the intro. Let it run until it either reaches the title or returns to
Workbench. Then return data/whdintrodiag.log from this exact drawer.

The short drawer name and ZIP packaging deliberately avoid the suspected
31-character parent-name boundary. This build changes only bounded intro-load
logging; gameplay, renderer, assets and WHDLoad memory configuration match the
alpha.55 WHDLoad release. F10 remains the exit key.
"""
    else:
        readme = (ROOT / "whdload" / "ReadMe.txt").read_text(encoding="ascii").format(
            RELEASE_NAME=RELEASE_NAME, RELEASE_VERSION=RELEASE_VERSION,
            ROADMAP_CHECKPOINT=ROADMAP_CHECKPOINT, STAGE_NAME=STAGE_NAME,
        )
    (STAGE / "ReadMe.txt").write_text(readme, encoding="ascii")
    make_icons()
    DIST.mkdir(exist_ok=True)
    archive_name = STAGE_NAME if diagnostic else ARCHIVE_NAME
    for path in STAGE.rglob("*"):
        for component in path.relative_to(STAGE_ROOT).parts:
            if len(component) > 30:
                raise SystemExit(f"Amiga path component exceeds 30 characters: {component}")
    zip_path = Path(shutil.make_archive(str(DIST / archive_name), "zip", STAGE_ROOT, STAGE_NAME))
    lha_path = None if diagnostic else make_lha(
        STAGE_ROOT, STAGE_NAME, DIST / f"{archive_name}.lha"
    )
    with zipfile.ZipFile(zip_path) as archive:
        required = {
            f"{STAGE_NAME}/Sparkpaw.Slave", f"{STAGE_NAME}/Sparkpaw.info",
            f"{STAGE_NAME}/ReadMe.txt", f"{STAGE_NAME}/data/Sparkpaw",
        }
        missing = required.difference(archive.namelist())
        if missing:
            raise SystemExit(f"WHDLoad ZIP verification failed: {sorted(missing)}")
    print("Wrote", zip_path)
    if lha_path is not None:
        print("Wrote", lha_path)
    print("Prepared", STAGE)


if __name__ == "__main__":
    main()
