ChipSnake WHDLoad prototype 0.1
================================

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

Copy the complete ChipSnake-WHDLoad drawer to your Amiga hard disk. Do not move
files out of its data drawer.

From Workbench, double-click the ChipSnake icon. From Shell:

  CD <where-you-copied-it>/ChipSnake-WHDLoad
  WHDLoad ChipSnake.Slave PRELOAD

F10 quits immediately through WHDLoad. The normal game controls still work:
cursor keys, W/A/S/D, or joystick in port 2; Space/fire starts or pauses.

FIRST PROTOTYPE NOTES
---------------------

This slave runs the existing AmigaOS program inside WHDLoad Kickstart 3.1
emulation. That is appropriate because ChipSnake uses Intuition, graphics,
DOS and audio.device instead of directly taking over the custom chips.

Please test:

* title and in-game music pitch and tempo;
* intro, Hall of Fame and all three backgrounds;
* keyboard and joystick controls;
* returning/quitting without a crash;
* whether a changed highscore survives another WHDLoad launch.

The virtual WHDLoad filesystem is generally treated as read-only. If highscore
saving does not persist, a later slave revision can redirect that one file
through WHDLoad's dedicated save-file API.

This is an original homebrew game, made in C with generated support assets and
tools. No Kickstart ROM and no WHDLoad executable are redistributed here.

