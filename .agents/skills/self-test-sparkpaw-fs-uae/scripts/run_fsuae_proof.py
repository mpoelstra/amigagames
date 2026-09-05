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
    parser.add_argument("--screenshot-at", action="append", type=float,
                        default=[], metavar="SECONDS")
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
    screenshots = boot/"screenshots"
    screenshots.mkdir()

    command = [str(args.fs_uae), str(args.config),
               f"--hard-drive-0={boot}", "--fullscreen=0",
               f"--screenshots-output-dir={screenshots}",
               f"--screenshots-output-prefix={args.name}"]
    process = subprocess.Popen(command)
    started = time.monotonic()
    pending_shots = sorted(args.screenshot_at)
    deadline = time.monotonic()+args.timeout
    stable_size = None
    stable_polls = 0
    try:
        while time.monotonic()<deadline:
            elapsed = time.monotonic()-started
            while pending_shots and elapsed>=pending_shots[0]:
                before = set(screenshots.glob("*.png"))
                try:
                    subprocess.run([
                        "osascript", "-e", 'tell application "FS-UAE" to activate',
                        "-e", 'tell application "System Events" to keystroke "s" using command down'
                    ], check=True, capture_output=True, text=True)
                except subprocess.CalledProcessError as error:
                    subprocess.run([
                        "osascript", "-e", 'tell application "FS-UAE" to activate'
                    ], check=True)
                    shot = screenshots / (
                        f"host-display-{len(args.screenshot_at)-len(pending_shots)+1:03d}.png")
                    subprocess.run(["/usr/sbin/screencapture", "-x", str(shot)],
                                   check=True)
                    print("screenshot_fallback=host-display "
                          f"reason={error.stderr.strip()}")
                time.sleep(0.4)
                # FS-UAE can accept Command-S without emitting a file (for
                # example after a macOS permission restart). Treat that as a
                # failed internal capture and take the documented full-host
                # fallback instead of timing out with no visual evidence.
                if not set(screenshots.glob("*.png"))-before:
                    subprocess.run([
                        "osascript", "-e", 'tell application "FS-UAE" to activate'
                    ], check=True)
                    shot = screenshots / (
                        f"host-display-{len(args.screenshot_at)-len(pending_shots)+1:03d}.png")
                    subprocess.run(["/usr/sbin/screencapture", "-x", str(shot)],
                                   check=True)
                    print("screenshot_fallback=host-display "
                          "reason=FS-UAE emitted no internal PNG")
                pending_shots.pop(0)
            if artifact.exists():
                size = artifact.stat().st_size
                stable_polls = stable_polls+1 if size and size==stable_size else 0
                stable_size = size
                captured = sorted(screenshots.glob("*.png"))
                # One FS-UAE action emits crop/full/real variants. Count
                # actions, not PNG files, so one early frame cannot satisfy a
                # multi-phase visual proof.
                internal_actions = len(list(screenshots.glob("*-crop-*.png")))
                host_actions = len(list(screenshots.glob("host-display-*.png")))
                if stable_polls>=2 and not pending_shots and \
                        internal_actions+host_actions>=len(args.screenshot_at):
                    print(f"boot_drawer={boot}")
                    print(f"artifact={artifact}")
                    print(f"artifact_bytes={size}")
                    for shot in captured:
                        print(f"screenshot={shot}")
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
