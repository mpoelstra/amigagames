#!/usr/bin/env python3
"""Rebuild a Makefile target into a separate directory with a vlink map.

Never stages or launches a game. Byte parity with the existing executable is
required: a map from a different build cannot explain that executable.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import subprocess


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("target")
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--make-arg", action="append", default=[],
                        help="Make variable assignment used to build this exact target")
    args = parser.parse_args()
    root = args.root.resolve()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)
    original = root / args.target
    before = digest(original)
    dry = subprocess.check_output(
        ["make", "-n", "-W", "Makefile", *args.make_arg, args.target], cwd=root, text=True
    ).replace("\\\n", " ")
    commands = [shlex.split(line) for line in dry.splitlines()
                if "/bin/vc " in line]
    if len(commands) != 1:
        raise SystemExit("Expected one compiler command; refusing ambiguous target")
    command = commands[0]
    sources = [arg for arg in command if arg.endswith(".c")]
    flags = command[1:]
    out_index = flags.index("-o")
    del flags[out_index:out_index + 2]
    flags = [arg for arg in flags if arg not in sources]
    sdk = root / ".toolchain/sdk"
    env = dict(os.environ, VBCC=str(sdk),
               PATH=str(sdk / "bin") + os.pathsep + os.environ["PATH"],
               TMPDIR=str(output))
    objects = []
    with (output / "compile.log").open("w") as log:
        for index, source in enumerate(sources):
            obj = output / f"{index:02d}-{Path(source).stem}.o"
            assembly = obj.with_suffix(".s")
            for mode, destination in [("-c", obj), ("-S", assembly)]:
                subprocess.run([command[0], *flags, mode, "-o", str(destination), source],
                               cwd=root, env=env, stdout=log, stderr=log, check=True)
            objects.append(str(obj))
    executable = output / "Sparkpaw"
    link = [str(sdk / "bin/vlink"), "-bamigahunk", "-x", "-Bstatic",
            "-Cvbcc", "-nostdlib", "-mrel", "-s", "-Rshort", "-M",
            str(sdk / "targets/m68k-amigaos/lib/startup.o"), *objects,
            "-L" + str(sdk / "targets/m68k-amigaos/lib"), "-lvc", "-o", str(executable)]
    with (output / "link.map").open("w") as mapping:
        subprocess.run(link, cwd=root, env=env, stdout=mapping, check=True)
    report = dict(target=args.target, sources=sources, compiler_command=command,
                  sha256=before, rebuilt_sha256=digest(executable),
                  original_unchanged=before == digest(original))
    (output / "manifest.json").write_text(json.dumps(report, indent=2) + "\n")
    if report["rebuilt_sha256"] != before or not report["original_unchanged"]:
        raise SystemExit("Parity failed: map is NOT verified against original executable")
    print(f"Byte-identical audit: {before}\nMap: {output / 'link.map'}")


if __name__ == "__main__":
    main()
