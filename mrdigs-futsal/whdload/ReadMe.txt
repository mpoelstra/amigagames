MrDig's Futsal WHDLoad prototype 0.1
====================================

MrDig Productions - Copyright 2026

REQUIREMENTS
------------

* An Amiga 1200-class WHDLoad setup
* WHDLoad 19 or newer installed
* 2 MB Chip RAM and at least 2 MB Fast RAM
* A legal Kickstart 3.1 (40.068) A1200 image and matching RTB file

The Kickstart files are intentionally NOT included. WHDLoad normally expects:

  DEVS:Kickstarts/kick40068.A1200
  DEVS:Kickstarts/kick40068.A1200.RTB

The RTB file is supplied by the freely distributable WHDLoad kickstart support
archive. The ROM image must come from your own Amiga or licensed ROM package.

INSTALLATION AND START
----------------------

Copy the complete MrDigs-Futsal-WHDLoad drawer to your Amiga hard disk. Keep
the data drawer and all its contents together.

From Workbench, double-click the MrDigs-Futsal icon. From Shell:

  CD <where-you-copied-it>/MrDigs-Futsal-WHDLoad
  WHDLoad MrDigs-Futsal.Slave PRELOAD

F10 quits immediately through WHDLoad. The game's normal Escape behaviour
continues to work.

FIRST PROTOTYPE TESTS
---------------------

Please test:

* title and in-game music pitch and tempo;
* player sprite stability during movement;
* kick, whistle, dribble, rebound and goal sound effects;
* keyboard, joystick port 2, and two-player joystick port 1 controls;
* every Match Setup option and sudden-death overtime;
* all floor styles and AI levels;
* repeated kickoffs and goals;
* returning/quitting without a crash.

This slave runs the existing AmigaOS program inside WHDLoad Kickstart 3.1
emulation. It enables audio.device and retains the game's own VBlank music
interrupt and Paula-based sound effects.

This is an original homebrew game, made in C with generated support assets and
tools. No Kickstart ROM and no WHDLoad executable are redistributed here.
