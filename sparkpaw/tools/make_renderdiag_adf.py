#!/usr/bin/env python3
"""Build and verify the separate writable Stage 1 diagnostic ADF."""
from __future__ import annotations

import os
import argparse
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"
STAGE = BUILD / "renderdiag-adf-root"
OUTPUT = BUILD / "renderdiag-stage1-adf" / "Sparkpaw-Renderdiag-alpha41.adf"
EXECUTABLE = BUILD / "sparkpaw-renderdiag-adf"
RUNTIME = ROOT / "assets" / "runtime"
PACKED = BUILD / "adf-assets"
AMITOOLS = ROOT / ".toolchain" / "amitools"

REPLACEMENTS = {
    "storm-front.spbm": "storm-front.spr1",
    "storm-rear.spbm": "storm-rear.spr1",
    "clockwork-storm-strider.spbm": "clockwork-storm-strider.spr1",
    "sparkpaw-sprites4.spbm": "sparkpaw-sprites4.spr1",
}
RUNTIME_FILES = (
    "sparkpaw-title.spbm", "sparkpaw-level-loading.spbm",
    "level-charge-patch.spbm", "sparkpaw-hud-base.spbm",
    "sparkpaw-hud-health.spbm", "sparkpaw-hud-lives.spbm",
    "sparkpaw-hud-diamonds.spbm", "sparkpaw-diamond.spbm",
    "storm-front.spbm", "storm-rear.spbm", "storm-collision.bin",
    "sparkpaw-sprites4.spbm", "clockwork-beetle.spbm",
    "clockwork-storm-strider.spbm", "energy-shot.raw", "player-hurt.raw",
    "enemy-hit.raw", "enemy-death.raw", "strider-shot.raw", "jump.raw",
    "collect-spark.raw", "water-splash.raw",
)


def main() -> None:
    parser=argparse.ArgumentParser()
    parser.add_argument("--executable",type=Path,default=EXECUTABLE)
    parser.add_argument("--output",type=Path,default=OUTPUT)
    parser.add_argument("--program-name",default="Sparkpaw-Renderdiag")
    parser.add_argument("--volume",default="SparkpawDiag")
    parser.add_argument("--readme",type=Path,
                        default=ROOT / "docs" / "RENDERDIAG.txt")
    args=parser.parse_args()
    executable=args.executable if args.executable.is_absolute() else ROOT/args.executable
    output=args.output if args.output.is_absolute() else ROOT/args.output
    readme=args.readme if args.readme.is_absolute() else ROOT/args.readme
    if STAGE.exists():
        shutil.rmtree(STAGE)
    runtime_stage = STAGE / "assets" / "runtime"
    runtime_stage.mkdir(parents=True)
    shutil.copy2(executable, STAGE / args.program_name)
    for name in RUNTIME_FILES:
        if name not in REPLACEMENTS:
            shutil.copy2(RUNTIME / name, runtime_stage / name)
    for packed_name in REPLACEMENTS.values():
        shutil.copy2(PACKED / packed_name, runtime_stage / packed_name)
    shutil.copy2(readme, STAGE / "ReadMe.txt")
    (STAGE / "S").mkdir()
    (STAGE / "S" / "startup-sequence").write_text(
        args.program_name+"\n", encoding="ascii"
    )

    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists():
        output.unlink()
    env = os.environ.copy()
    env["PYTHONPATH"] = str(AMITOOLS)
    command = [
        sys.executable, "-m", "amitools.tools.xdftool", "-f", str(output),
        "format", args.volume, "DOS1", "+", "boot", "install",
    ]
    for directory in ("S", "assets", "assets/runtime"):
        command.extend(("+", "makedir", directory))
    for path in sorted(STAGE.rglob("*")):
        if path.is_file():
            command.extend(("+", "write", str(path),
                            path.relative_to(STAGE).as_posix()))
    subprocess.run(command, cwd=ROOT, env=env, check=True)
    if output.stat().st_size != 901120:
        raise SystemExit(f"invalid diagnostic ADF size: {output.stat().st_size}")

    with tempfile.TemporaryDirectory() as temporary:
        extracted = Path(temporary)
        subprocess.run([
            sys.executable, "-m", "amitools.tools.xdftool", "-r", str(output),
            "read", args.program_name, str(extracted / "executable"),
            "+", "read", "S/startup-sequence", str(extracted / "startup"),
        ], cwd=ROOT, env=env, check=True)
        if (extracted / "executable").read_bytes() != executable.read_bytes():
            raise SystemExit("diagnostic ADF executable verification failed")
        if (extracted / "startup").read_bytes() != (args.program_name+"\n").encode("ascii"):
            raise SystemExit("diagnostic ADF startup verification failed")
    print(f"Wrote {output}")
    print("The image must remain writable so renderdiag.log can be flushed to DF0.")


if __name__ == "__main__":
    main()
