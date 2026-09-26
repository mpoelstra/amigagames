# Sparkpaw alpha.13 - The Harrier goes out with a BOOM

## What's new since alpha.8

- Defeat the Harrier at the end of Stormrail Skimmer and watch its hull rupture, erupt into a full pixel-art fireball and scatter sparks before the gate opens. A louder three-hit **boom, boom, BOOM** plays with the destruction.
- Preview the new Harrier defeat explosion sound in HD and WHDLoad SOUNDTEST.

- Steer the Skimmer up with the D-pad in JOYPAD mode. Flight uses all four directions in both controller modes; the second button remains the on-foot jump control in JOYPAD mode.
- Expanded HD and WHDLoad ReadMe files open with a personal note from MrDig, followed by the story, game information, edition-specific setup, controls and contact details.

- Storm Ruins now has electrical discharges across its purple sky. Energy travels down the existing tower lightning and lights its crystal; a distant building light also blinks while keeping its original shape.

- Continue from Stormrail Skimmer into **Drowned Turbines**, a third playable section with flooded platforms, moving pontoons, water jets, animated waterfalls and a final Rain Core encounter.
- Face turbine crabs, pump walkers and Spillwings. Find the checkpoint, collect diamonds and health, then review your results or replay the section.
- Carry lives, health, diamonds and score through all three sections. OPTIONS can also start directly at Drowned Turbines.
- Hear **Undertow Circuit** in Drowned Turbines. HD and WHDLoad SOUNDTEST include its music, Pump Shot and Checkpoint effects.
- Shorter WHDLoad loading waits on the tested FS-UAE 68020 / 8 MB setup: CHARGING to READY took about 8.9 seconds instead of 20.7 seconds in the controlled comparison, a 57% reduction. Intro transitions and music synchronization were also reported working without flicker. The wait before the first intro picture remains.
- Choose the standard WHDLoad edition for 8 MB Fast RAM, or the separate **High RAM** edition for machines with at least 16 MB Fast RAM. High RAM loads the whole campaign at startup; it has not yet been playtested.
- OPTIONS now offers **CONTROL: JOYSTICK or JOYPAD**. Joystick mode uses Up to jump; joypad mode uses button 2. The primary button shoots in both modes; W and Space remain available on the keyboard.
- Input handling includes second-button detection and keyboard acknowledgement corrections. A short controls test passed; affected users have not yet confirmed that their original hardware input problems are resolved.
- The floppy edition spans three disks, with an **INSERT DISK 3** prompt when needed. Requested disks can be found in DF0 through DF3. It keeps the title-screen start without story intro or SOUNDTEST.

Requires PAL A1200/AGA, 68020 or better and 2 MB Chip RAM. HD, ADF and standard WHDLoad target 8 MB Fast; WHDLoad High RAM requires at least 16 MB. WHDLoad requires your own Kickstart 3.1 A1200 ROM/RTB.

Loading results come from user-run FS-UAE tests of the preceding candidates, not a real-Amiga benchmark or an alpha.8 timing comparison. Alpha.8 could not start in the tested 8 MB configuration. The release builds have diagnostics removed; a fresh release playtest and real-hardware verification remain outstanding. The intermittent real-Amiga HUD-boundary glitch remains open.

The new Level1 animation was visually approved in a focused FS-UAE test. The Harrier destruction and revised effect were approved through focused previews and an HD test; full-release HD/WHDLoad/ADF replay, minimum-68020 performance and real-hardware checks remain pending.

The flight-control correction passes host input checks; native confirmation of this correction remains pending.
