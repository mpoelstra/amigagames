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

BUILD = ROOT / "build/campaign-drowned"
STAGE_ROOT = ROOT / "build/release-campaign"
WHD_SHORT = "Sparkpaw-0.7.0-a9-WHDLoad"


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


def main() -> None:
    if sys.argv[1:]:
        raise SystemExit("usage: make_release.py (source archive is not part of this release)")
    validate_release_identity()
    validate_amiga_names()
    assert set(RUNTIME_FILES) == set(HD_ALL)
    for name in RUNTIME_FILES:
        runtime_source(name)
    for tool, args in (("build_campaign_drowned.py", []),
                       ("build_campaign_drowned.py", ["--whdload-packed"]),
                       ("build_campaign_drowned.py", ["--adf"]),
                       ("generate_disk_status.py", []),
                       ("package_campaign_drowned_adf.py", [])):
        subprocess.run([sys.executable, str(ROOT / "tools" / tool), *args],
                       cwd=ROOT, check=True)
    from make_whdload import assemble
    assemble(packed=True)
    slave = (ROOT / "whdload/Sparkpaw.Slave").read_bytes()
    assert f"Version {RELEASE_VERSION}".encode() in slave
    import struct
    assert struct.unpack_from(">I", slave, slave.index(b"WHDLOADS") + 28)[0] == 0x380000

    hd_exe = BUILD / "Sparkpaw-Campaign"
    whd_exe = BUILD / "whdload-packed/Sparkpaw-Campaign"
    adf_exe = BUILD / "adf/Sparkpaw-Campaign"
    assert set(executable_runtime_files(hd_exe)) == set(RUNTIME_FILES)
    assert {PACKED_ALIASES.get(name, name) for name in
            executable_runtime_files(whd_exe)} == set(RUNTIME_FILES)
    assert len(executable_runtime_files(adf_exe)) >= 70
    played_hd = DIST / "older-builds/Controls-Pullup-HD-approved-regression/Sparkpaw-Test"
    if played_hd.is_file():
        assert hd_exe.read_bytes() == played_hd.read_bytes()

    if STAGE_ROOT.exists():
        shutil.rmtree(STAGE_ROOT)
    STAGE_ROOT.mkdir(parents=True)
    hd = STAGE_ROOT / RELEASE_NAME
    (hd / "assets/runtime").mkdir(parents=True)
    for name in RUNTIME_FILES:
        shutil.copy2(runtime_source(name), hd / "assets/runtime" / name)
    shutil.copy2(hd_exe, hd / "Sparkpaw")
    (hd / "Sparkpaw.info").write_bytes(make_project_icon("Sparkpaw", []))
    (hd / "ReadMe.txt").write_text(
        f"Sparkpaw {RELEASE_VERSION} HD\n"
        "PAL A1200/AGA, 68020+, 2 MB Chip and 8 MB Fast. Launch Sparkpaw.\n"
        "Three sections: Storm Ruins, Stormrail Skimmer and Drowned Turbines.\n"
        "OPTIONS selects a start section, audio mode and second-button mapping.\n"
        "SOUNDTEST includes Drowned music and effects. Esc returns to READY.\n"
        "Controls: port-2 joystick, Up/W jump, Fire/Space shoot, P pause.\n"
        "Second button can be assigned to Jump or Fire in OPTIONS.\n"
        "The pull-up and keyboard ACK corrections are included; reports on\n"
        "affected hardware have not yet been verified fixed.\n",
        encoding="ascii")
    checked_names(hd)

    whd = STAGE_ROOT / WHD_SHORT
    (whd / "data/assets/runtime").mkdir(parents=True)
    crunched = BUILD / "whdload-packed/Sparkpaw-crunched"
    result = subprocess.run([str(ROOT / "build/shrinkler/Shrinkler"), "-1", "-p",
                             str(whd_exe), str(crunched)], text=True,
                            capture_output=True, check=True)
    assert "Verifying... OK" in result.stdout
    shutil.copy2(crunched, whd / "data/Sparkpaw")
    (whd / "Sparkpaw.Slave").write_bytes(slave)
    (whd / "Sparkpaw.info").write_bytes(make_project_icon(
        "WHDLoad", ["SLAVE=Sparkpaw.Slave", "PRELOAD", "PAL"]))
    (whd / "ReadMe.txt.info").write_bytes(make_readme_icon())
    (whd / "ReadMe.txt").write_text(
        f"Sparkpaw {RELEASE_VERSION} WHDLoad\n"
        "PAL A1200/AGA, 68020+, 2 MB Chip and 8 MB Fast. WHDLoad 19+ and\n"
        "your own Kickstart 3.1 A1200 ROM/RTB are required. Start via the\n"
        "Sparkpaw Workbench icon. F10 exits to Workbench.\n"
        "Includes the full three-section campaign, intro and SOUNDTEST.\n"
        "Second button: Jump or Fire in OPTIONS. W jumps; Space shoots.\n"
        "The pull-up and keyboard ACK corrections are included; reports on\n"
        "affected hardware have not yet been verified fixed.\n",
        encoding="ascii")
    assets = whd / "data/assets/runtime"
    packed_names = set(executable_runtime_files(whd_exe))
    for name in packed_names:
        raw = runtime_source(PACKED_ALIASES.get(name, name)).read_bytes()
        if name in {'storm-collision.bin', 'drowned-route.bin'}:
            body = raw
        else:
            options = [pack_rle(raw), pack_lz(raw)]
            if name.endswith(('.lsbank', '-bank.bin')):
                options.append(pack_delta(raw))
            body = min(options, key=len)
        decoded = (decode_rle(body) if body[:4] == b"SPR1" else
                   decode_lz(body) if body[:4] in (b"SPL1", b"SPD1") else body)
        assert decoded == raw, name
        (assets / name).write_bytes(body)
    assert {p.name for p in assets.iterdir() if p.is_file()} == packed_names
    checked_names(whd)

    retain_or_copy(hd, DIST / RELEASE_NAME)
    retain_or_copy(whd, DIST / WHD_SHORT)
    outputs = [*archive(hd, RELEASE_NAME),
               *archive(whd, f"{RELEASE_NAME}-WHDLoad")]
    media = json.loads((BUILD / "adf/media.json").read_text())
    assert len(media["disks"]) == 3
    for row in media["disks"]:
        disk = row["disk"]
        source = BUILD / "adf" / f"Sparkpaw-Disk{disk}.adf"
        assert digest(source) == row["sha256"]
        target = DIST / f"{RELEASE_NAME}-Disk{disk}.adf"
        if not target.exists():
            shutil.copy2(source, target)
        outputs.append(target)
    report = {p.name: {"bytes": p.stat().st_size, "sha256": digest(p)} for p in outputs}
    (BUILD / "alpha9-release-artifacts.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
