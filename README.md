# Amiga Games

Native Commodore Amiga game projects by MrDig Productions, developed with an
AGA/classic-Amiga-focused C, assembly, Python, Copper, Blitter, sprite, and
Paula toolchain.

## Projects

- `chipsnake/`: ChipSnake, a complete one-screen Snake game.
- `mrdigs-futsal/`: Futsal, an indoor arcade football game.
- `sparkpaw/`: Sparkpaw: The Stormstone Quest, an active AGA action-platformer
  campaign checkpoint, currently 0.7.0-alpha.2 (HD, two ADFs and WHDLoad).

Each project is self-contained and has its own README, Makefile, third-party
source where required, and locally installed `.toolchain`. Generated builds and
release archives are not versioned; published packages belong on GitHub
Releases.

See `docs/BUILDING.md` for host dependencies and local toolchain layout. The
current Sparkpaw work order is maintained in
[sparkpaw/docs/CURRENT_STATUS.md](sparkpaw/docs/CURRENT_STATUS.md).
`CODEX_HANDOFF.md` provides workspace contracts and historical context;
`docs/DEVELOPMENT_HISTORY.md` preserves the implementation history.

## Licensing

Original source code and software tools are available under the MIT License in
`LICENSE`. Original game art, characters, music, sound, and other creative
assets are not covered by MIT and remain protected as described in
`ASSETS-LICENSE.md`. Third-party licenses are listed in
`THIRD_PARTY_NOTICES.md` and alongside the relevant components.