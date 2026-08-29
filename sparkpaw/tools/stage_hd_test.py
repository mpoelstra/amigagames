#!/usr/bin/env python3
"""Stage one self-contained Sparkpaw HD test drawer without touching releases."""
from __future__ import annotations

import argparse
import hashlib
import os
import shutil
from datetime import datetime
from pathlib import Path

from make_release import DIST, ROOT, RUNTIME_FILES

STAGE_ROOT = ROOT / "build" / "hd-test-stage"


def file_hash(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def release_inventory() -> dict[str, str]:
    inventory: dict[str, str] = {}
    if not DIST.exists():
        return inventory
    for path in sorted(DIST.glob("Sparkpaw-*")):
        if path.is_file():
            inventory[path.name] = file_hash(path)
        elif path.is_dir():
            for child in sorted(item for item in path.rglob("*") if item.is_file()):
                inventory[str(child.relative_to(DIST))] = file_hash(child)
    return inventory


def validate_component(name: str) -> None:
    if not name or name in {".", ".."} or "/" in name or "\\" in name:
        raise SystemExit(f"invalid Amiga path component: {name!r}")
    if len(name) > 30:
        raise SystemExit(f"Amiga path component exceeds 30 characters: {name}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("--drawer", required=True)
    parser.add_argument("--executable", required=True, type=Path)
    parser.add_argument("--executable-name", required=True)
    parser.add_argument("--readme", required=True, type=Path)
    parser.add_argument("--replace", action="store_true")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    validate_component(args.drawer)
    validate_component(args.executable_name)
    for name in RUNTIME_FILES:
        validate_component(name)

    executable = args.executable.resolve()
    readme = args.readme.resolve()
    if not executable.is_file():
        raise SystemExit(f"missing executable: {executable}")
    if not readme.is_file():
        raise SystemExit(f"missing ReadMe: {readme}")

    before = release_inventory()
    destination = DIST / args.drawer
    staged = STAGE_ROOT / args.drawer
    if staged.exists():
        shutil.rmtree(staged)
    runtime_destination = staged / "assets" / "runtime"
    runtime_destination.mkdir(parents=True)
    shutil.copy2(executable, staged / args.executable_name)
    shutil.copy2(readme, staged / "ReadMe.txt")

    for name in RUNTIME_FILES:
        source = ROOT / "assets" / "runtime" / name
        if not source.is_file():
            raise SystemExit(f"runtime manifest source missing: {source}")
        shutil.copy2(source, runtime_destination / name)

    expected = {"ReadMe.txt", args.executable_name}
    expected.update(f"assets/runtime/{name}" for name in RUNTIME_FILES)
    actual = {
        str(path.relative_to(staged))
        for path in staged.rglob("*")
        if path.is_file()
    }
    if actual != expected:
        raise SystemExit(
            f"staged file set differs from runtime manifest: "
            f"missing={sorted(expected-actual)} extra={sorted(actual-expected)}"
        )
    for name in RUNTIME_FILES:
        source = ROOT / "assets" / "runtime" / name
        copied = runtime_destination / name
        if file_hash(source) != file_hash(copied):
            raise SystemExit(f"staged runtime asset differs: {name}")

    if destination.exists():
        if not args.replace:
            raise SystemExit(f"destination exists; use --replace: {destination}")
        archive_root = DIST / "older-builds"
        archive_root.mkdir(parents=True, exist_ok=True)
        suffix = datetime.now().strftime("-old-%H%M%S")
        archived = archive_root / f"{args.drawer[:30-len(suffix)]}{suffix}"
        if archived.exists():
            raise SystemExit(f"archive destination already exists: {archived}")
        os.replace(destination, archived)

    destination.parent.mkdir(parents=True, exist_ok=True)
    os.replace(staged, destination)
    after = release_inventory()
    if after != before:
        raise SystemExit("release artifact inventory changed while staging HD test")
    print(f"Staged {destination}")
    print(f"Verified {len(RUNTIME_FILES)} authoritative runtime assets")
    print(f"Preserved {len(before)} release files byte-for-byte")


if __name__ == "__main__":
    main()
