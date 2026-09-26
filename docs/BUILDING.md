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
Use the release drawers for the complete three-section campaign; the release
packager separately links the Drowned module into each media-specific build.
`make release` currently creates nine alpha.12 artifacts: HD ZIP/LHA,
standard 8-MB WHDLoad ZIP/LHA, separate >=16-MB HighRAM WHDLoad ZIP/LHA,
and Disk1/Disk2/Disk3 ADF, plus three extracted drawers. Run
`tools/verify_checkpoint_release.py` for independent checks. Current packaging
uses isolated `build/release-<version>` outputs and refuses to overwrite prior
staging. Preserve previous builds; do not delete them to force a rebuild.

Use `make release PYTHON=../.venv/bin/python3` from `sparkpaw` for the current
campaign, not the legacy standalone `make whdload` packager/template.
Standard WHDLoad uses raw level banks, no PRELOAD and NOCACHE; HighRAM is a
separate raw-file build using PRELOAD and normal CPU cache policy. These are
intentional different profiles. Never add PRELOAD to the 8-MB banked edition.
Three-disk media identifiers SP09D1/2/3 remain compatibility markers.
Host/package verification is separate from native/hardware play acceptance.
Archive superseded outputs intact under dist/older-builds; protect evidence.

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

## Sparkpaw alpha.3 music and media

Approved MOD sources live in sparkpaw/music; audition MP3/WAV files stay local.
The Makefile uses the repository's existing mrdigs-futsal/third_party/LSPlayer
CMake converter with -shrink -fixed50hz. Only sample tails are trimmed; no
ADPCM and no HD disk compression. Runtime .lsmusic/.lsbank files are generated.
Build with make PYTHON=../.venv/bin/python3 and make release from sparkpaw,
then run tools/verify_checkpoint_release.py with the same Python environment.
The ADF pair retains lossless SPL1/SPR1 and explicitly allows a smaller free
reserve (alpha.3 Disk1: 9 blocks). All-file readback remains mandatory.

## Sparkpaw alpha.7 ADF packing

`make release` builds the ordinary HD/WHDLoad campaign and native ADF variant.
The ADF executable is compressed by `sparkpaw/tools/crunch_adf_executable.py`
using vendored Shrinkler source (upstream 17cff110fcded387fe90e632805258d9c8359e94).
Its host binary is built under ignored `sparkpaw/build/shrinkler`; no host
binary is committed. Hash-bound input/output verification and a 32-block reserve
per disk are mandatory. SPD1 sample banks decode byte-exactly via the existing
streaming LZ reader plus a delta accumulator. Native startup timing remains
a user test gate. See `sparkpaw/docs/ADF_COMPRESSION_RESEARCH.md`.

## Alpha.11 Level1 ambience packaging

`make PYTHON=../.venv/bin/python3` generates the approved v5 planar data through
`tools/build_level1_rear_release.py`. `make release PYTHON=../.venv/bin/python3`
builds four complete campaign variants via `make_campaign_release.py` and emits
HD, standard/High RAM WHDLoad ZIP/LHA plus three ADFs. New `l1-electric.bin`
belongs to Level1; ADF uses its packed reader, standard WHDLoad its bank reader.
ADF packaging may select optimal-parsed SPL1/SPD1 with unchanged runtime format
and uses Shrinkler preset 3. Source images/audio are losslessly reconstructed
and all disk files read back. Run `tools/verify_checkpoint_release.py` for the
current campaign verifier. Release identity is alpha.12; never rebuild older numbered releases
with new bytes. Preserve superseded drawers and logs under dist/older-builds.

## Player ReadMe release gate

`tools/game_readme.py` generates HD and both WHDLoad ReadMe files from the
shared player content and `docs/PERSONAL_NOTE.txt`. Preserve that user-authored
note verbatim apart from plain-text formatting. Review edition-specific
requirements, controls and links. `tools/verify_checkpoint_release.py` now
requires complete matching ReadMe text in all six extracted ZIP/LHA archives,
including the opening note and current version, before release handoff.
