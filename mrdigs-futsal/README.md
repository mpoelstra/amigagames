# MrDig's Futsal

An original 3-versus-3 indoor arcade football game for classic Commodore
Amiga computers. The complete arena remains visible on one 320x256 screen,
and the ball stays in play by rebounding from the walls.

Keep the complete game drawer together. On Workbench, double-click the
MrDigs-Futsal football icon to start the game.

The ADF release is a bootable 880 KB Amiga floppy image. Insert it as DF0:
and reset the Amiga; `S/startup-sequence` launches the game automatically.

The game explicitly opens a PAL screen and advances its converted music at a
stable 50 Hz, regardless of whether it was launched from Workbench or booted
directly from floppy. The music itself is authored at 150 BPM to retain the
preferred brisk arrangement. The program also enables the 68020 instruction
cache while running and page-flips between two complete frame bitmaps at
vertical blank, avoiding the timing and sprite-tearing differences of a
minimal boot without SetPatch.

MrDig Productions, Copyright 2026.

## How this AI-made game was developed

MrDig supplied the game idea, creative direction, tuning decisions, and
hands-on testing. AI assisted with the C and 68000 assembly implementation,
computer-player behaviour, pixel-art and title concepts, music, sound effects,
debugging, and the Python development tools. The finished program is native
Amiga code and does not depend on a modern game engine.

The game is written in C, including the directional player sprites and
animated football, with assembly used to connect the Light Speed Player to a
stable vertical-blank interrupt. Python tools convert the AI-assisted title
concept into a 16-colour planar IFF/ILBM image; generate sound samples, the
Workbench icon, and ProTracker modules; and package ZIP, LHA, and bootable ADF
releases. VBCC cross-compiles everything for AmigaOS and the Motorola 68000
family.

## Current assets

- `assets/concepts/mrdigs-futsal-title-concept.png`: generated Amiga-style
  title-screen concept.
- `music/mrdigs-futsal-title.mod`: **Pixel Sprint Clean**, a bright,
  ProTracker-compatible four-channel dance title track with clean bass,
  glassy chords, and a recurring original melody.
- `music/mrdigs-futsal-title.lsmusic` and `.lsbank`: Light Speed Player 1.31
  conversions for later Amiga integration.
- `music/mrdigs-futsal-ingame.mod`: separate 65-second gameplay track. It
  deliberately uses only Paula channels 0 and 1; channels 2 and 3 contain no
  tracker events and are reserved for sound effects.
- `music/mrdigs-futsal-ingame.lsmusic` and `.lsbank`: fixed-50-Hz Light Speed
  Player conversion of the gameplay track.
- `assets/sfx/mrdigs-futsal-ingame-amiga-preview.wav`: converter-produced
  preview of the gameplay music through its simulated Amiga/Paula output.
- `assets/sfx/previews/*.wav`: nine host-computer previews of the effects.
- `sfx/raw/*.raw`: signed 8-bit mono, 11025-Hz Amiga sample data.
- `sfx/futsal-sfx.bank`: packed big-endian runtime sound bank.
- `sfx/futsal-sfx.json`: readable bank manifest.
- `tools/generate_futsal_title_mod.py`: reproducible title-music generator.
- `tools/generate_futsal_ingame_mod.py`: reproducible two-channel gameplay
  music generator.
- `tools/generate_futsal_sfx.py`: reproducible sound-effect generator and
  bank packer.

## Planned channel layout

- Title screen: title music may use all four Paula channels.
- Gameplay: music uses channels 0 and 1 only.
- Gameplay effects: channels 2 and 3, allowing left/right positioning and
  effect interruption without disturbing the music.

The effects bank starts with `MDFS`, a big-endian version and effect count,
then one 32-byte record per effect: 16-byte name, 32-bit data offset, 32-bit
length, 16-bit Paula period, volume, priority, cooldown frames, and a reserved
16-bit field. Sample blocks follow the table at even lengths.

## First playable prototype

The `futsal` executable is a configurable one-screen implementation:

- one-player blue-versus-CPU and two-player blue-versus-red matches;
- selectable 3v3 or 4v4 team size; in 4v4 each side gains an autonomous
  goalkeeper that tracks shots, catches normal balls, parries powershots,
  and distributes possession to a field player;
- enclosed pitch with ball rebounds from the side and end walls;
- open goals on the left and right;
- configurable goal limits of 5, 10, or 15 and time limits of 3, 5, or
  10 minutes; a tied match continues as sudden-death overtime;
- off-screen frame composition for smooth, flicker-resistant drawing;
- separate title and two-channel gameplay music, muted by default in matches;
- positional effects on the two reserved Paula channels.
- eight-direction pixel-art player graphics with two-frame running animation,
  shadows, a selection marker, and an animated round football;
- a cached pseudo-3D indoor arena with perspective floor seams, raised near
  and far walls, recessed goal mouths, metal frames, and pixel netting;
- a colour-panel scoreboard with separate red and blue team sections.

Controls:

- From the title screen, Space or player 1 fire opens Match Setup.
  Use up/down to select an option, left/right to change it, and fire or Space
  to start. Escape returns to the title.
- Player 1 controls blue with the joystick in port 2. Player 2 controls red
  with the joystick in port 1 when two-player mode is selected.
- Tap fire to pass; hold and release fire to shoot. Fire without possession
  does nothing; defensive player selection is automatic for both teams.
- Arrow keys or W/A/S/D: move the selected blue player.
- Tab: select the next blue player.
- While a CPU player controls the ball, selection automatically moves to a
  meaningfully closer blue defender. Manual Tab selection gets a short grace
  period before automatic defensive switching resumes.
- Tap Space: assisted pass to a teammate and switch to the receiver.
- Hold and release Space: charge and take an eight-directional shot.
- Diagonal movement is normalized to the same overall speed as straight movement.
- First contact cushions the ball into close control at the player’s feet;
  turning changes the dribbling direction, while opponent contact can steal it.
  Control is temporary: after at most about two seconds—or sooner while
  running and turning—the ball is pushed loose and must be caught again.
- P: pause and display the match overlay.
- M: toggle gameplay music on or off; matches begin with music muted and
  sound effects remain active.
- 1/2/3/4: switch between wood, green, checkered, and Boing Ball courts
  courts. The selected arena is cached, so switching adds no per-frame cost.
- 7/8/9: select easy, medium, or hard computer AI. Difficulty changes its
  reaction rate, running speed, pressure, passing choices, and shot frequency.
  Medium is the default and matches the human player's acceleration and speed;
  Easy uses the previous Medium tuning. CPU difficulty is ignored in
  two-player matches; both teams' unselected players use equal support logic.
- Plus/minus: increase or decrease player and ball speed from the 100%
  default, in 10% steps over a 60%–240% test range. The new 100% is
  physically identical to the previous build's 150% setting.
- Escape: return to the title screen; Escape on the title quits.

Match Setup provides player count, 3v3/4v4 team size, court, CPU difficulty,
in-game music, goal limit, time limit, and starting game speed. Goalkeepers
are AI-controlled in both one- and two-player 4v4 matches. Starting speeds are
80%, 100%, 120%, or 150%; the existing plus/minus adjustment remains available
during a match.

Run `make` to regenerate the title IFF and build the Amiga executable. The
project contains its own VBCC installation, Amiga NDK headers, and LSPlayer
copy, so it has no build dependency on another game directory.

## Building from source

The source ZIP contains all game-owned source code, Python tools, source
artwork, MOD files, sound data, and the MIT-licensed Light Speed Player source.
VBCC and the Amiga NDK are deliberately not included because they have their
own distribution terms.

Install or copy the build dependencies to:

```text
.toolchain/sdk/               VBCC installation with the +aos68k target
.toolchain/ndk/Include_H/     AmigaOS NDK C headers
```

You also need Python 3, CMake, and a host C++ compiler. CMake is only needed
when the included LSP converter has not yet been built.

Build from the project root:

```sh
make              # generate assets/music and build futsal
make release      # build ZIP, LHA, ADF, and Source.zip
make clean        # remove the executable and object files
```

The main build stages are:

1. Convert the title concept PNG to a 320x256, 16-colour IFF/ILBM.
2. Generate the title and two-channel gameplay MOD files.
3. Convert MOD music to `.lsmusic` scores and `.lsbank` sample banks.
4. Generate and pack signed 8-bit Paula sound effects.
5. Compile the C sources and LSP assembly wrapper with VBCC/VASM.
6. Package the Workbench, LHA, ZIP, and bootable ADF releases.

Important source locations:

- `src/futsal.c`: gameplay, rendering, input, physics, and AI
- `src/modplayer.c` and `src/lspplayer.s`: music loading and playback
- `src/sfxplayer.c`: two-channel Paula sound-effect playback
- `tools/`: reproducible graphics, music, sound, icon, and release tools
- `assets/concepts/`: source title artwork
- `music/*.mod`: editable ProTracker source music
- `third_party/LSPlayer/`: included MIT-licensed replay source
- `Makefile`: complete cross-build dependency graph

Runtime output is written to `dist/`. For a hard-drive installation, preserve
the complete game drawer and its `assets`, `music`, and `sfx` subdirectories.
