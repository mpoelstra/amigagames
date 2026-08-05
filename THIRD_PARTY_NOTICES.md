# Third-party notices

This repository contains third-party source code that remains under its own
license. The root MIT License applies only to original project code and tools.

## Light Speed Player

ChipSnake and Futsal each vendor a project-local copy of Light Speed Player by
Arnaud Carré, based on upstream commit
`fa9c93cd304ead7076e0fce8a1c89d9286d81f9c`. Both copies include the same local
build-portability changes for the 1.31 converter: ADPCM sources are included,
the Unix build pre-includes `WindowsCompat.h`, and the compatibility header
provides the required bounded string helper. Light Speed Player is distributed
under the MIT License found at:

- `chipsnake/third_party/LSPlayer/LICENSE`
- `mrdigs-futsal/third_party/LSPlayer/LICENSE`

## Shrinkler

The vendored Light Speed Player source includes Shrinkler components by Aske
Simon Christensen and other noted contributors. Their terms are preserved at:

- `chipsnake/third_party/LSPlayer/src/external/Shrinkler/LICENSE.txt`
- `mrdigs-futsal/third_party/LSPlayer/src/external/Shrinkler/LICENSE.txt`

## Music and sound asset notices

Project-specific `music/LICENSE.txt` and `sfx/LICENSE.txt` files take priority
for files in those directories. In particular, assets explicitly released
under CC0 remain CC0 and are not restricted by `ASSETS-LICENSE.md`.

Compiler toolchains, Amiga NDK files, WHDLoad development packages, reference
manuals, and other locally installed dependencies are intentionally excluded
from this repository.