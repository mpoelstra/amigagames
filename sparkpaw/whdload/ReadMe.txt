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

Copy the complete {STAGE_NAME} drawer to your Amiga hard disk. Keep
the data drawer and all its contents together.

From Workbench, double-click the Sparkpaw icon. From Shell:

  CD <where-you-copied-it>/{STAGE_NAME}
  WHDLoad Sparkpaw.Slave PRELOAD

F10 exits immediately through WHDLoad. Sparkpaw's official executable remains
reset-to-exit when it is run without WHDLoad.

CONTROLS AND TEST SCOPE
-----------------------

Joystick port 2 controls movement. The ready-screen OPTIONS menu assigns the
secondary button to JUMP (the default) or FIRE for the current run; joystick Up
and keyboard W always jump. Primary Fire and keyboard Space shoot and confirm
menu choices. Full story, title, loading/charging and ready/options are included.
Play Storm Ruins, recover the Lightning Core, and CONTINUE into Stormrail.
Board the Skimmer, fly through formations/debris, then complete the Harrier
finale and results. Both sections support resident replay. Esc returns to the
ready screen; BACK TO TITLE clears the campaign. OPTIONS can start Stormrail.

This package now builds the full campaign with the existing WHDLoad F10 hooks,
not the old single-level executable. All 48 assets are included on the HD data
volume; no physical disk swaps are needed. Kickstart memory setup is unchanged:
2 MB Chip allocation minus 4 KiB and 8 MB expansion including its ROM area.

Verification: assembly, compilation, file coverage, archive extraction and icon
checks only for this campaign WHDLoad build. Earlier alpha.68 real-A1200 startup
and F10 observations do not establish the new campaign's native acceptance.
Test intro, both sections, Continue/replay/Esc/title return and F10 before claiming
WHDLoad compatibility. Ordinary HD and ADF campaign approval does not substitute.
Physical A1200/Gotek and Analogue Pocket tests remain separate.

The Sparkpaw project icon contains an 86x93, 34-colour NewIcons image and an
86x93 fallback using the eight standard OS 2.x/3.x Workbench pens.

No Kickstart ROM and no WHDLoad executable are redistributed in this package.
