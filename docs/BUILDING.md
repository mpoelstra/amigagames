# Building the Amiga games

## Host requirements

- macOS or another host supported by the selected VBCC toolchain
- Python 3
- Pillow and amigainfo, installed with
	`python3 -m pip install -r requirements-dev.txt`
- CMake and a host C++ compiler for the Light Speed Player converter
- `zip` and an LHA-compatible archiver for complete release packaging

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
`make release` rebuilds the executable and creates the project's ZIP, LHA, ADF,
source, and WHDLoad outputs where supported. Sparkpaw's `make release` now
creates its versioned WHDLoad LHA and ZIP beside the HD and ADF artifacts;
`make whdload` rebuilds only that pair. Generated files appear below
`build/` and `dist/` and are intentionally not committed.

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
