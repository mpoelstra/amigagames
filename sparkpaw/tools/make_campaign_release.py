#!/usr/bin/env python3
"""Package the accepted three-section campaign with current controls code."""
from __future__ import annotations

import hashlib
import json
import shutil
import subprocess
import sys
from pathlib import Path

from campaign_asset_manifest import HD_ALL
from make_release import (DIST, RELEASE_NAME, RELEASE_VERSION, ROOT,
                          RUNTIME_FILES, make_lha, validate_amiga_names,
                          validate_release_identity)
from make_sparkpaw_icon import make_project_icon, make_readme_icon
from pack_adf_asset import pack as pack_rle, decode as decode_rle
from pack_disk_asset import pack as pack_lz, pack_delta, decode as decode_lz
from runtime_asset_refs import executable_runtime_files
from stage_campaign_whdload_packed import ALIASES as PACKED_ALIASES
from campaign_runtime_sources import source as runtime_source

BUILD = ROOT / "build" / f"release-{RELEASE_VERSION}"
STAGE_ROOT = BUILD / "stage"
WHD_SHORT = f"Sparkpaw-{RELEASE_VERSION.replace('-alpha.', '-a')}-WHDLoad"
HIGH_SHORT = f"Sparkpaw-a{RELEASE_VERSION.split('.')[-1]}-WHD-HighRAM"


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def checked_names(root: Path) -> None:
    for path in root.rglob("*"):
        assert all(len(part) <= 30 for part in path.relative_to(root.parent).parts), path


def archive(stage: Path, archive_name: str) -> tuple[Path, Path]:
    zip_path = DIST / f"{archive_name}.zip"
    lha_path = DIST / f"{archive_name}.lha"
    if not zip_path.exists():
        zip_path = Path(shutil.make_archive(str(DIST / archive_name), "zip",
                                            stage.parent, stage.name))
    if not lha_path.exists():
        lha_path = make_lha(stage.parent, stage.name, lha_path)
    return zip_path, lha_path


def retain_or_copy(stage: Path, target: Path) -> None:
    if target.exists():
        generated = {p.relative_to(stage).as_posix(): digest(p)
                     for p in stage.rglob('*') if p.is_file()}
        current = {p.relative_to(target).as_posix(): digest(p)
                   for p in target.rglob('*') if p.is_file()}
        assert generated == current, f"existing release drawer differs: {target}"
    else:
        shutil.copytree(stage, target)


def run(tool, *args):
    subprocess.run([sys.executable, str(ROOT / "tools" / tool), *map(str, args)],
                   cwd=ROOT, check=True)


def build_variant(name, *flags):
    out = BUILD / name
    # Never overwrite a previous build. A resumed package run uses its manifest.
    if not out.exists():
        run("build_campaign_drowned.py", *flags, "--output-dir", out)
    meta = json.loads((out / "build.json").read_text())
    assert digest(out / "Sparkpaw-Campaign") == meta["sha256"]
    assert not any("LOAD_TRACE" in str(a) or "LOAD_STATE" in str(a)
                   for a in meta["command"] + meta["module_command"])
    return out / "Sparkpaw-Campaign"


def main() -> None:
    if sys.argv[1:]:
        raise SystemExit("usage: make_release.py")
    validate_release_identity()
    validate_amiga_names()
    assert set(RUNTIME_FILES) == set(HD_ALL)
    for name in RUNTIME_FILES:
        runtime_source(name)
    hd_exe = build_variant("hd")
    whd_exe = build_variant("banks", "--whdload-banks")
    high_exe = build_variant("highram", "--whdload")
    adf_exe = build_variant("adf", "--adf")
    assert set(executable_runtime_files(hd_exe)) == set(RUNTIME_FILES)
    assert set(executable_runtime_files(high_exe)) == set(RUNTIME_FILES)
    assert {PACKED_ALIASES.get(n, n) for n in executable_runtime_files(whd_exe)} == set(RUNTIME_FILES)
    assert len(executable_runtime_files(adf_exe)) >= 70
    # Disk artwork was already generated and accepted; don't overwrite that build.
    assert all((ROOT / f"build/multidisk-probe/status/disk{i}-patch.spbm").is_file()
               for i in (1, 2, 3))
    if not (BUILD / "adf/media.json").exists():
        run("package_campaign_drowned_adf.py", "--build-dir", BUILD / "adf")

    if not (BUILD / "banks/banks-manifest.json").exists():
        run("prepare_whdload_banks.py", "--build-dir", BUILD / "banks",
            "--name", WHD_SHORT, "--raw-levels", "--production")
    manifest = json.loads((BUILD / "banks/banks-manifest.json").read_text())
    bank_stage = Path(manifest["stage"])
    if not (BUILD / "banks/bank-tests").exists():
        run("test_whd_banks.py", "--build-dir", BUILD / "banks")
    if not (BUILD / "bank-dependencies").exists():
        run("test_whd_bank_dependencies.py", "--build-dir", BUILD / "banks",
            "--output-dir", BUILD / "bank-dependencies")

    STAGE_ROOT.mkdir(parents=True, exist_ok=True)
    hd = STAGE_ROOT / RELEASE_NAME
    whd = STAGE_ROOT / WHD_SHORT
    high = STAGE_ROOT / HIGH_SHORT
    # Individual stages are only created once. Failed attempts remain reviewable.
    for path in (hd, whd, high):
        assert not path.exists(), f"preserve prior staging attempt: {path}"
    (hd / "assets/runtime").mkdir(parents=True)
    (high / "data/assets/runtime").mkdir(parents=True)
    for name in RUNTIME_FILES:
        shutil.copy2(runtime_source(name), hd / "assets/runtime" / name)
        shutil.copy2(runtime_source(name), high / "data/assets/runtime" / name)
    shutil.copy2(hd_exe, hd / "Sparkpaw")
    shutil.copy2(high_exe, high / "data/Sparkpaw")
    shutil.copytree(bank_stage, whd)
    from make_whdload import assemble
    assemble(output=high / "Sparkpaw.Slave")
    from game_readme import game_readme
    hd_readme = game_readme(RELEASE_VERSION, "hd")
    whd_readme = game_readme(RELEASE_VERSION, "whd")
    high_readme = game_readme(RELEASE_VERSION, "high")
    for stage, tool, types, readme in (
        (hd, "Sparkpaw", [], hd_readme),
        (whd, "WHDLoad", ["SLAVE=Sparkpaw.Slave", "PAL", "NOCACHE"], whd_readme),
        (high, "WHDLoad", ["SLAVE=Sparkpaw.Slave", "PRELOAD", "PAL"], high_readme)):
        (stage / "Sparkpaw.info").write_bytes(make_project_icon(tool, types))
        (stage / "ReadMe.txt.info").write_bytes(make_readme_icon())
        (stage / "ReadMe.txt").write_text(readme, encoding="ascii")
        shutil.copy2(ROOT / "docs" / f"RELEASE_NOTES_{RELEASE_VERSION}.md", stage / "ReleaseNotes.txt")
        checked_names(stage)
    import struct
    for stage in (whd, high):
        slave = (stage / "Sparkpaw.Slave").read_bytes()
        assert f"Version {RELEASE_VERSION}".encode() in slave
        assert struct.unpack_from(">I", slave, slave.index(b"WHDLOADS") + 28)[0] == 0x580000
    outputs = []
    for stage, name in ((hd, RELEASE_NAME), (whd, f"{RELEASE_NAME}-WHDLoad"),
                        (high, f"{RELEASE_NAME}-WHDLoad-HighRAM")):
        retain_or_copy(stage, DIST / stage.name)
        outputs.extend(archive(stage, name))
    media = json.loads((BUILD / "adf/media.json").read_text())
    assert len(media["disks"]) == 3
    for row in media["disks"]:
        source = BUILD / "adf" / f"Sparkpaw-Disk{row['disk']}.adf"
        assert digest(source) == row["sha256"]
        target = DIST / f"{RELEASE_NAME}-Disk{row['disk']}.adf"
        assert not target.exists()
        shutil.copy2(source, target)
        outputs.append(target)
    report = {p.name: {"bytes": p.stat().st_size, "sha256": digest(p)} for p in outputs}
    (BUILD / "release-artifacts.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
