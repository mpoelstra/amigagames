# Controls investigation — 23 September 2026

Status update, 23 September: MrDig played
`dist/Controls-Pullup-HD/Sparkpaw-Test` and reports that it works well, with no
perceived difference. The machine/controller configuration was not specified.
This is a successful regression playtest for that candidate; it does not
establish that either reporter's original failure is fixed, because MrDig
could not reproduce that failure before the correction. The affected users
have not verified it. OPTIONS as a trigger and a 68060 connection remain
unproven. The three accepted campaign candidates retain their earlier
acceptance; their bytes have not been changed by this test. This document's
earlier unnumbered-candidate statements describe the investigation stage.

## Reports and confidence

User 1 reportedly played alpha.8 WHDLoad with a Monster joystick: primary fire
plus stick worked until using button 2. A 68060 is suspected, not confirmed.
User 2 reports normal HD becomes unable to combine jump/shoot after OPTIONS,
including keyboard; changing settings does not help. Exact HD version, machines,
controller model/switch setting and reproduction captures/logs are unavailable.
MrDig cannot reproduce with his FS-UAE or real Amiga configuration.

The source findings below establish defects and a matching failure mechanism,
not a confirmed diagnosis of each user's exact machine.

## Findings and correction

1. `platform_amiga.c` wrote POTGO $3000: CD32 reset on port-2 pin 5 is high,
   but pin 9 remains a proportional input without the required button pull-up.
   Commodore's Hardware Reference Manual explicitly requires OUTRY and DATRY
   both set for a switch-to-ground button. A passive controller can discharge
   the capacitor on press without a reliable high on release. POTINP bit 14
   then continues to look pressed. The candidate uses $f000, retaining the
   existing CD32 reset policy and adding the pin-9 pull-up. START stays clear.
   Both writes now follow Disable(), preventing OS interrupts from overwriting
   setup during OwnBlitter/WaitBlit/Forbid. Sampling remains later in existing
   frame-paced presentation/gameplay paths, not immediately after the write;
   no extra per-frame reconfiguration or CPU delay loop.
2. `playerReadInput()` merges Up/W/button-2 before jump edge detection and
   primary/Space/button-2 before fire edge detection. A stuck-low secondary
   therefore suppresses future edges of its selected action, even on keyboard.
   Changing SECOND BUTTON can move the blocked action from jump to fire.
   Actual-C host tests demonstrate this under a passive-pin model and verify
   recovery with the correct pull-up. This model does not measure real Paula.
   Input merging is deliberately unchanged: overlapping sources for the same
   action still require release before another edge; no autofire introduced.
3. Keyboard ACK counted two raster transitions, despite requiring >=85 us.
   Starting just before a line boundary gives only about one full line
   (~64 us PAL/~63.5 us NTSC). The candidate asserts SPMODE, then samples the
   initial line and waits three transitions: at least two complete lines
   (>126 us for the game's PAL/NTSC modes). Extra execution time on a slow CPU
   can mask the old defect, so accelerators are relevant to this separate
   finding; no 060-specific failure has been measured.
4. OPTIONS entry itself only selects the page; SECOND BUTTON changes only on
   Left/Right on its row. Menu code does not directly change POTGO. Soundtest
   loading does release/reacquire hardware, and both takeover paths are fixed.
   Menu state/transition tests pass. No proven options-only corruption or
   mutual exclusion between jump and fire was found. Game update calls shot
   and jump handling independently; cooldown/turn/hurt/crouch rules remain.

The pre-fix platform snapshot rebuilt with current campaign source produces
EXACTLY the approved `Campaign-HD-Soundtest-SFX/Sparkpaw-Test` bytes. This pins
the comparison to the accepted campaign baseline, not merely git HEAD (which
is older than alpha.8). The original electrical policy is documented since
alpha.50 in the handoff. An exact alpha.8 source rebuild was not performed.

Monster's Super Deluxe 8/16bit product page describes a rear switch that maps
button 2 to joystick Up. That differs from a native pin-9 second button; the
user's exact model and switch position are unknown. Do not blame the brand or
claim all Monster controllers have the same wiring.

## Checks and limits

- `tests/test_control_hardware.py`: actual extracted C with ASan/UBSan;
  modeled old stuck-line failure, pull-up recovery, 100 repeated cycles per
  mapping, primary+Up, W+Space, secondary+complementary action, held jump/fire,
  flight level-input behavior, F10/P and every sub-line phase across PAL/NTSC
  frame wrap at twelve polling speeds. The host model is not electrical proof.
- Existing control-options, retained-menu ownership, READY input/menu rendering
  (HD and ADF), pause, Drowned lifecycle, audio-mode and level1-audio checks pass.
- Complete native 68020 integrated HD build passes. HD/WHDLoad/ADF platform
  compilation to assembly passes; inspected pull-up write after Disable and
  ACK wait. No full new WHDLoad/ADF package or runtime acceptance claimed.
- `make test PYTHON=../.venv/bin/python3` stops at existing
  `test_drowned_patch_stage.py`: its renderer extraction truncates a nested
  conditional, giving an unterminated `#ifdef` and missing animation functions.
  That test reads renderer.c, not the changed platform/input files. Neither
  renderer.c nor that test was changed in this investigation. Full suite is
  therefore NOT green; relevant checks above were run separately.
- Candidate executable remains 614716 bytes, SHA-256
  `ae11f1f17c4d85f90601fb5cd80fa9e8187a44ef2dd03722371367e8cc67ce02`.
  Stager verifies 74 assets and 74 literal runtime references. All 74 assets
  exactly match accepted HD. All 228 pre-existing files across official
  alpha.8 (68 files) and the three approved candidates retain their hashes.
- Evidence, baseline source snapshot/rebuild, assembly, logs and manifests:
  `build/control-investigation/`. No FPS or hardware-compatibility claim.

## Manual candidate

`dist/older-builds/Controls-Pullup-HD-approved-regression/Sparkpaw-Test` is the
played controls-only drawer, archived with all 76 file hashes unchanged.
The three user-protected accepted campaign drawers are retained in place.
ReadMe contains the short play matrix: without OPTIONS; open/leave OPTIONS
unchanged; repeated B2 presses/releases; both B2 assignments; keyboard and
mixed inputs; Soundtest return; direct Stormrail/Drowned entry. Start with
usual FS-UAE/68030 regression play, then affected physical controller/machine.

For the reporters, record accelerator/CPU, OS, exact build, controller model
and switch position. Distinguish OPTIONS alone from changing an option or
entering Soundtest. A keyboard-only run with controller disconnected BEFORE
launch helps distinguish joystick-line latching from keyboard handshake.
Ultimately affected hardware must confirm the correction; an unaffected
FS-UAE or MrDig machine can only establish absence of an observed regression.

## Primary references

- Commodore Amiga Hardware Reference Manual, chapter 8, Digital I/O on the
  Controller Port and How Keyboard Data Is Received (archived manual):
  https://bastya.net/AmigaDevDocs/hard_8.html
  OUT+DAT high for button pull-up; up to 300 us capacitor settling; >=85 us ACK.
- Manufacturer's controller description (model is an example, not identified
  as the reporter's exact unit):
  https://monsterjoysticks.com/super-deluxe-8-16bit-retro-joystick-cherry-red
