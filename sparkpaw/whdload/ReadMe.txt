Sparkpaw WHDLoad {RELEASE_VERSION}
=================================

MrDig Productions - Copyright 2026
Roadmap checkpoint: Phase {ROADMAP_CHECKPOINT}

REQUIREMENTS
------------

* An Amiga 1200-class AGA WHDLoad setup
* Motorola 68020 or faster
* WHDLoad 19 or newer installed
* 2 MB Chip RAM and at least 8 MB Fast RAM
* A legal Kickstart 3.1 (40.068) A1200 image and matching RTB file

The Kickstart files are intentionally NOT included. WHDLoad normally expects:

  DEVS:Kickstarts/kick40068.A1200
  DEVS:Kickstarts/kick40068.A1200.RTB

The RTB is available in the freely distributable WHDLoad Kickstart support
archive. The ROM image must come from your own Amiga or licensed ROM package.

INSTALLATION AND START
----------------------

Copy the complete {RELEASE_NAME}-WHDLoad drawer to your Amiga hard disk. Keep
the data drawer and all its contents together.

From Workbench, double-click the Sparkpaw icon. From Shell:

  CD <where-you-copied-it>/{RELEASE_NAME}-WHDLoad
  WHDLoad Sparkpaw.Slave PRELOAD

F10 exits immediately through WHDLoad. Sparkpaw's official executable remains
reset-to-exit when it is run without WHDLoad.

CONTROLS AND TEST SCOPE
-----------------------

Joystick port 2 controls movement; up jumps and Fire shoots. Keyboard A/D/W/S
and Space provide the matching test controls. Fire or Space advances the ready
screen. Test the complete five-plate intro, title, LOADING, CHARGING, ready
screen and all twelve gameplay screens through the Stormstone Core clearing.

This first WHDLoad package wraps the alpha.49 HD executable and runtime in
Kickstart 3.1 emulation. Please verify F10 exit, presentation, sound, controls,
full-level traversal and repeated launches. No WHDLoad, ADF or real-hardware
acceptance is claimed until user-supplied testing establishes it.

No Kickstart ROM and no WHDLoad executable are redistributed in this package.
