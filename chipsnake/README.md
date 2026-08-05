# MrDig's ChipSnake

A first, small Snake game for classic Amiga computers. It opens a private
320x256, 16-colour screen and uses only standard AmigaOS 2.x APIs.

The game opens with a classic Amiga-style intro built from the original
ChipSnake concept art. It is converted to a native 320x256, 16-colour ILBM and
combined with blinking stars, colour-cycling accents and a smooth credit
marquee composed in an off-screen buffer.

MrDig Productions
Copyright 2026
This game is made by 100% AI

## How this AI-made game was developed

MrDig supplied the concept, creative direction, gameplay decisions, and
hands-on testing. AI assisted with the C and 68000 assembly implementation,
pixel-art concepts, music composition, debugging, and the Python development
tools. The game is a native Amiga executable: it does not use a modern engine
or an emulation layer.

The runtime is written in C with a small assembly wrapper for the Light Speed
Player. Python scripts convert AI-assisted concept art and source PNG files to
16-colour planar IFF/ILBM images, generate ProTracker MOD music, check classic
ProTracker compatibility, create the Workbench icon, and package ZIP, LHA, and
bootable ADF releases. VBCC cross-compiles the program for AmigaOS.

## Target

- Amiga 1200 with 2 MB Chip RAM
- AmigaOS 2.0 (Kickstart 37) or later
- PAL display
- Keyboard or joystick in port 2

The generated code does not intentionally require AGA. An expanded Amiga 500
with Kickstart 2.04 or later may work, but the supported and tested target is
the Amiga 1200. A stock 512 KB, Kickstart 1.3 Amiga 500 is not supported.

The ADF release is a bootable 880 KB Amiga floppy image. Insert it as DF0:
and reset the Amiga; `S/startup-sequence` launches ChipSnake automatically.

## Music

`music/chipsnake-title.mod` is **ChipSnake Circuit**, the main original
four-channel ProTracker title tune. It has twelve pattern sections, a long
recurring melody with variations, stereo counter-arpeggios, dense drums and a
strong octave/fifth bass line with offbeat answers.

`music/chipsnake-turbo.mod` preserves the shorter **ChipSnake Turbo**
arrangement as a fallback. Both tracks use synthesized pulse, triangle and
noise samples and contain no samples taken from other modules. The compositions
and samples are released under CC0; see `music/LICENSE.txt`.

`music/chipsnake-thunder.mod` is a third, longer alternative. **ChipSnake
Thunder** uses an original heroic call-and-response melody, rising fanfare
sections, rapid arpeggios and a galloping bass approach inspired by broad
late-1980s Hubbard/Whittaker game-music techniques. It does not reproduce the
melody or arrangement of ThunderCats. `music/chipsnake-circuit.mod` preserves
the current main track independently from future generator changes.

`music/chipsnake-neon.mod` is the separate **ChipSnake Neon Drive** dance
track: a 150 BPM four-on-the-floor arrangement with club kicks, clap layers,
syncopated bass, offbeat rave stabs, hats, risers, and an original lead hook.
It plays on the title screen and is also part of the gameplay rotation.

`music/chipsnake-hall.mod` is the slow, minor-key **ChipSnake Hall** module.
It plays during high-score name entry and on the Hall of Fame screen.

Regenerate the module reproducibly with:

```sh
python3 tools/generate_chipsnake_mod.py
```

Running `make` builds the current LSP 1.31 converter from its included source
when necessary and regenerates any `.lsmusic`/`.lsbank` pair whose MOD source
has changed. The converter uses `-fixed50hz`, so the game can drive playback
without taking over a CIA timer.

The title screen plays `music/chipsnake-neon.mod`. Starting or restarting a
game randomly selects `music/chipsnake-circuit.mod`,
`music/chipsnake-turbo.mod`, or `music/chipsnake-neon.mod`; an immediate repeat
is prevented so the soundtrack changes between successive games.

## Hall of Fame

Food is worth 100 points. The ten best scores are stored in
`PROGDIR:chipsnake.highscores`, beside the executable. The file has a version,
fixed-size records, and a checksum; missing or damaged data restores the
built-in table led by MrDig. Qualified players can enter up to 15 characters,
using Backspace to edit and Return to save. The game writes a temporary file
and retains a backup while replacing the table.

Press `H` on the title screen to view the Hall of Fame without playing.
Return or Space then returns to the title screen. After a completed game,
Return or Space on the Hall of Fame immediately starts another game instead.
Escape always returns from gameplay, name entry, or the Hall of Fame to the
title screen. Only Escape on the title screen exits the program.

Replay uses Arnaud Carré's MIT-licensed Light Speed Player 1.31. During the
cross-build, each source MOD is converted into an `.lsmusic` score and an
`.lsbank` Paula sample bank. The game loads the score into normal memory, loads
the bank into Chip RAM, reserves all four Paula channels through `audio.device`,
and advances the player from an AmigaOS vertical-blank interrupt server. Music
timing is therefore independent of title animation, blits, and the main game
loop. DMA restart timing is measured from the Amiga raster position rather than
from CPU-speed-dependent delay loops.

The title screen displays `AUDIO DEVICE UNAVAILABLE` if the four Paula channels
cannot be reserved. The integration is in `src/modplayer.c` and
`src/lspplayer.s`; the upstream player and converter are under
`third_party/LSPlayer`.

Keep the `music` drawer next to the executable when copying the game to an
Amiga or FS-UAE hard-drive directory. The `.lsmusic` and `.lsbank` files are
required at runtime; the `.mod` files are source assets and may be omitted from
an Amiga installation.

## Controls

- Space on the title screen: start
- Escape on the title screen: quit
- Cursor keys or W/A/S/D: steer
- Joystick in port 2: steer; Fire starts, pauses, continues, or restarts
- Space: pause/continue
- N: start a new game with another randomly selected background
- Escape during gameplay or score screens: return to the title
- Escape on the title screen: quit cleanly to Workbench or the Shell

Food automatically moves to another valid location after seven seconds if it
has not been eaten. The timeout uses the Amiga vertical-blank frequency, so it
works consistently on both PAL and NTSC machines and pauses when the game is
paused.

The playfield wraps at every edge. Leaving through the left or right returns
the snake on the opposite horizontal edge; leaving through the top or bottom
returns it on the opposite vertical edge. The destination must still be free
of obstacles and the snake's body.

## IFF/ILBM backgrounds

Every new game randomly loads one of these files:

- `backgrounds/circuit.iff`
- `backgrounds/boing.iff`
- `backgrounds/workbench.iff`

They are standard uncompressed 320x216, 16-colour ILBM files. The game loads
their palette and planar bitmap data directly.

The palette has a gameplay meaning, inspired by classic paint-package level
workflows:

- Pens 0-4: safe, walkable background colours
- Pens 5-11: solid obstacle colours
- Pen 12: snake body (reserved by the game)
- Pen 13: food (reserved by the game)
- Pen 14: snake head (reserved by the game)
- Pen 15: status text (reserved by the game)

Collision is derived from the pixels in the centre of each 8x8 grid cell.
This means visible obstacle artwork and collision cannot silently drift apart.
Food is never placed in a solid cell.

Generated levels also contain an `SNKM` collision-grid chunk for inspection
and other tools, but the game no longer depends on it.

The source PNGs are in `assets/source`, and the conversion tool is
`tools/png_to_ilbm.py`. PNG previews next to the IFF files are for development
only and do not have to be copied to the Amiga.

Keep the `backgrounds` drawer next to the `snake` executable when copying the
game to an Amiga or an FS-UAE hard-drive directory. Also copy
`assets/intro/chipsnake-intro.iff`, preserving that directory path, to display
the illustrated title screen. If an IFF file is absent or invalid, the game
falls back to its corresponding code-drawn screen.

An uncompressed Deluxe Paint ILBM that follows the dimensions, depth and
palette-index rules can already be interpreted without `SNKM`. The remaining
custom-level work is directory scanning, filename display, validation and
support for ByteRun1-compressed BODY data.

## Building with the workspace-local VBCC

The compiler, assembler, linker, and AmigaOS target are installed under
`.toolchain/sdk`. The official AmigaOS 3.2 NDK is under `.toolchain/ndk`.
Nothing needs to be added to your shell profile.

Run:

```sh
make
```

This invokes the local compiler with:

```sh
.toolchain/sdk/bin/vc +aos68k -O2 \
  -I.toolchain/ndk/Include_H -o snake src/snake.c
```

To explicitly generate code for any classic 68000 Amiga:

```sh
make CFLAGS="+aos68k -O2 -cpu=68000"
```

Copy the resulting `snake` executable to the Amiga hard disk and run it from
the Shell:

```text
snake
```

It can also be launched from Workbench after adding a project icon or placing
it in a drawer that has an appropriate tool icon.

### Building from the source ZIP

The source archive deliberately does not redistribute VBCC or the proprietary
Amiga NDK. Before building, provide these directories:

```text
.toolchain/sdk/               VBCC installation with the +aos68k target
.toolchain/ndk/Include_H/     AmigaOS NDK C headers
```

The included `third_party/LSPlayer` directory contains the MIT-licensed player
source and host-side converter. Its converter requires CMake and a host C++
compiler the first time it is built. Python 3 is required for asset and release
tools.

From the project root:

```sh
make              # convert music as needed and build snake
make release      # additionally build ZIP, LHA, ADF, and Source.zip
make clean        # remove the executable and object files
```

Runtime releases appear in `dist/`. To test the hard-drive build, keep
`snake`, `snake.info`, `assets`, `backgrounds`, and `music` together. The ADF
can be inserted in DF0: and booted directly.

## Source layout

- `src/snake.c`: game, input, rendering, and AmigaOS resource handling
- `src/modplayer.c`: LSP score/bank loading and audio.device ownership
- `src/lspplayer.s`: VBlank-driven Light Speed Player and raster DMA wrapper
- `assets/intro/chipsnake-intro.iff`: native 16-colour title artwork
- `assets/highscore/chipsnake-highscore.iff`: native Hall of Fame artwork
- `tools/png_to_intro_ilbm.py`: reproducible title-art conversion
- `music/chipsnake-title.mod`: original four-channel title music
- `music/chipsnake-circuit.mod`: preserved longer melodic arrangement
- `music/chipsnake-turbo.mod`: preserved compact fallback arrangement
- `music/chipsnake-thunder.mod`: heroic late-1980s-style alternative
- `music/chipsnake-neon.mod`: original dance/electronic alternative
- `music/chipsnake-hall.mod`: melancholic Hall of Fame music
- `tools/generate_chipsnake_mod.py`: reproducible MOD generator
- `tools/generate_chipsnake_neon_mod.py`: reproducible dance MOD generator
- `tools/generate_chipsnake_hall_mod.py`: reproducible Hall of Fame generator
- `tools/generate_chipsnake_circuit_mod.py`: preserved Circuit generator
- `tools/fix_arp_octave.py`: one-time octave repair for older generated MODs
- `music/*.lsmusic`: converted LSP scores loaded into normal memory
- `music/*.lsbank`: converted LSP sample banks loaded into Chip RAM
- `third_party/LSPlayer`: MIT-licensed LSP player and host converter
- `tools/make_release.py`: ZIP/LHA/ADF/source packaging
- `Makefile`: reproducible VBCC cross-build
