#!/usr/bin/env python3
"""Run a self-starting Sparkpaw proof from a directory-backed FS-UAE DH0."""

from __future__ import annotations

import argparse
import datetime as dt
from pathlib import Path
import shutil
import subprocess
import time


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--project", type=Path, default=Path("sparkpaw"))
    parser.add_argument("--executable", type=Path, required=True)
    parser.add_argument("--name", required=True)
    parser.add_argument("--artifact", required=True)
    parser.add_argument("--runtime-dir", type=Path)
    parser.add_argument("--config", type=Path,
                        default=Path.home()/"Documents/FS-UAE/Configurations/a1200.fs-uae")
    parser.add_argument("--fs-uae", type=Path,
                        default=Path("/Applications/FS-UAE.app/Contents/MacOS/fs-uae"))
    parser.add_argument("--timeout", type=int, default=120)
    args = parser.parse_args()

    project = args.project.resolve()
    executable = args.executable.resolve()
    runtime = (args.runtime_dir or project/"assets/runtime").resolve()
    for path in (project, executable, runtime, args.config, args.fs_uae):
        if not path.exists():
            parser.error(f"missing required path: {path}")

    stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S")
    boot = project/"build/fsuae-selftest"/f"{args.name}-{stamp}"
    (boot/"S").mkdir(parents=True)
    shutil.copy2(executable, boot/"Sparkpaw-Proof")
    shutil.copytree(runtime, boot/"assets/runtime")
    (boot/"S/startup-sequence").write_text("Sparkpaw-Proof\n", encoding="ascii")
    artifact = boot/args.artifact

    command = [str(args.fs_uae), str(args.config),
               f"--hard-drive-0={boot}", "--fullscreen=0"]
    process = subprocess.Popen(command)
    deadline = time.monotonic()+args.timeout
    stable_size = None
    stable_polls = 0
    try:
        while time.monotonic()<deadline:
            if artifact.exists():
                size = artifact.stat().st_size
                stable_polls = stable_polls+1 if size and size==stable_size else 0
                stable_size = size
                if stable_polls>=2:
                    print(f"boot_drawer={boot}")
                    print(f"artifact={artifact}")
                    print(f"artifact_bytes={size}")
                    return 0
            if process.poll() is not None:
                raise RuntimeError(
                    f"FS-UAE exited before proof artifact ({process.returncode})")
            time.sleep(0.5)
        raise TimeoutError(
            f"proof artifact did not stabilize within {args.timeout}s: {artifact}")
    finally:
        if process.poll() is None:
            process.terminate()
            try:
                process.wait(timeout=5)
            except subprocess.TimeoutExpired:
                process.kill()
                process.wait()


if __name__ == "__main__":
    raise SystemExit(main())
