# Building the Amiga games

## Host requirements

- macOS or another host supported by the selected VBCC toolchain
- Python 3
- Pillow and amigainfo, installed with
	`python3 -m pip install -r requirements-dev.txt`
- CMake and a host C++ compiler for the Light Speed Player converter
- `zip` and classic LHa 1.14i with archive-creation support for complete
  release packaging. Sparkpaw expects it at
  `sparkpaw/.toolchain/lha/bin/lha`, or at the absolute path supplied through
  `LHA`. Homebrew's `lhasa` formula can test and extract LHA files but cannot
  create them, so it is not sufficient for release packaging. The tested macOS
  Universal build is LHa 1.14i-ac20220213 from
  `https://github.com/amigavision/LhA`, itself built from
  `https://github.com/jca02266/lha`.

Compiler SDKs and proprietary reference material are not redistributed by this
repository. Install a separate local toolchain in every project that you want
to build:

```text
GAME/.toolchain/sdk/               VBCC with the +aos68k target and VASM/VLink
GAME/.toolchain/ndk/Include_H/     compatible AmigaOS NDK C headers
```

Release and WHDLoad scripts may additionally expect their documented local
tools below `GAME/.toolchain/`. These directories are ignored by Git. Never
make one game depend on a sibling project's private toolchain.

## Build commands

Run commands from the selected game directory:

```sh
make
make release
```

`make` regenerates required runtime conversions and builds the root executable.
For Sparkpaw 0.7, the root executable is the exact logger-free campaign build.
`make release` creates campaign HD ZIP/LHA, Disk1/Disk2 ADF, WHDLoad ZIP/LHA
and the extracted HD drawer. The WHDLoad target now uses the campaign flags
and sources. Run `tools/verify_checkpoint_release.py` for independent checks.
Physical hardware and new campaign WHDLoad native acceptance remain separate. Alpha.68 files are protected;
no source ZIP is produced unless explicitly requested.

Sparkpaw release manifests use one set of Amiga-safe runtime names for HD,
WHDLoad and the ADF source streams. No extracted filename or drawer component
may exceed 30 characters. The release scripts enforce this boundary; do not
restore descriptive long runtime names or derive the extracted WHDLoad root
directly from its longer public artifact filename.

The Makefiles default to `python3`. Override the interpreter when required:

```sh
make PYTHON=/path/to/python3
```

An isolated host environment is recommended:

```sh
python3 -m venv .venv
.venv/bin/python -m pip install -r requirements-dev.txt
make PYTHON="$(pwd)/../.venv/bin/python"
```

## Clean checkout expectations

The repository includes authored source, source assets, editable MOD files,
project metadata, and required vendored source such as Light Speed Player. It
does not include installed SDKs, local test recordings, historical ZIP backups,
or release archives.

Some small Amiga runtime assets remain versioned when the current project does
not yet expose a complete Makefile rule to regenerate them. They should only be
removed from version control after a clean checkout can recreate them
deterministically.

MrDig performs authoritative FS-UAE and real-Amiga testing. A successful host
cross-build does not prove display, audio, input, or PAL timing behaviour.


### Campaign multidisk and WHDLoad checkpoint

The lower-level disk scripts still write build/multidisk-probe; make release
runs them and publishes the versioned ADF pair. Completed test drawers are
archived; the current manual set is Sparkpaw-0.7.0-alpha.2. See
sparkpaw/docs/RELEASE_0_7_0_ALPHA_2.md for all six hashes, native evidence and
pending WHDLoad/hardware tests. No automatic emulator launch is part of release.
