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
ready screen; BACK TO TITLE clears the campaign. OPTIONS can start Stormrail,
choose gameplay audio, and open SOUNDTEST. P pauses/resumes gameplay.

This package now builds the full campaign with the existing WHDLoad F10 hooks,
not the old single-level executable. All 56 assets are included on the HD data
volume; no physical disk swaps are needed. Kickstart memory setup is unchanged:
2 MB Chip allocation minus 4 KiB and 8 MB expansion including its ROM area.

Direct OPTIONS Stormrail starts now show loading. Intro image memory is freed
only after display DMA stops; LMB skip is latched through fades. Joystick-Fire
text controls remain unchanged. These retained corrections and the new audio/options need alpha.7 hardware review.

Hero Drive plays during the story; Neon Sky accompanies title through READY.
Copper Sprint plays in Level 1; Iron Horizon plays in Stormrail, alongside
two effect voices software-mixed onto the fourth Paula channel. Results keep
their original tally effects; resident replay restarts the music. READY includes background wind particles,
orange sparks and improved menu handling on 68020.

Verification: package assembly, compiled asset coverage, archive extraction and
icon checks. Prior campaign WHDLoad was user-tested on real Amiga; this new
music/READY build still needs its own WHDLoad/native presentation check.
Test intro, both sections, Continue/replay/Esc/title return and F10. Physical
A1200/Gotek and Analogue Pocket tests remain separate.

The Sparkpaw project icon contains an 86x93, 34-colour NewIcons image and an
86x93 fallback using the eight standard OS 2.x/3.x Workbench pens.

No Kickstart ROM and no WHDLoad executable are redistributed in this package.

GAME OVER: final-life defeat shows total campaign score and Storm Light.
Fire returns to title. Storm Light is also in Soundtest.
Known issue: black after Fire was reported in an earlier HD game-over test;
not yet resolved. Carried health/lives now show from Stormrail boarding.
