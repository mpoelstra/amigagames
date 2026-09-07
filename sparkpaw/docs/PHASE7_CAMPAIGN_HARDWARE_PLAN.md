# Phase 7 — campaign releases and hardware validation

Alpha.3 checkpoint (7 September 2026): see RELEASE_0_7_0_ALPHA_3.md for the
current artifact set and acceptance matrix. HD READY/menu/transition user
accepted, previous music/dust ADF user accepted; final alpha.3 ADF/WHDLoad
presentation and hardware tests remain open. Disk1 free: 9 blocks; Disk2: 345.
Historical candidate status below does not supersede this checkpoint.

## Phase 7A.1: local HD checkpoint (historical)

0.7.0-alpha.1 packaged the full story, Level 1 and Stormrail campaign from the
accepted e6e20db6... HD executable. It was not uploaded to itch and is now
archived intact under dist/older-builds. Phase 6D remains its progression design.

## Phase 7A.2: all-format alpha checkpoint

0.7.0-alpha.2 packages that same HD game plus the user-approved two-ADF route
and a newly rebuilt campaign WHDLoad. Disk 1 boots title/Level 1 without story;
Disk 2 holds Stormrail/finale/results. Both drives are scanned before prompting.
INSERT uses the original loading picture and styled status strip, with no helper
text or mouse cancellation. Shared assets are deliberately duplicated as needed.

The user explicitly approved ADF functionality and both disk prompts. Ordinary
HD approval is retained. On 2026-09-05 the user reports successful real-Amiga
HD and campaign WHDLoad tests, without enumerating individual F10/intro checks.
A separate HD run with about 1.45 MB free Chip RAM plays Level 1 but crashes
on Stormrail. See STORMRAIL_CHIP_RAM_AUDIT.md for the bounded memory follow-up.
Physical ADF/Gotek, cold timing and Analogue Pocket remain open. Minimum stays
PAL A1200/AGA, 68020+, 2 MB Chip + 8 MB Fast; installed capacity is not free
launch memory. The intermittent physical HUD issue remains open.

Current inventory, hashes and manual instructions: RELEASE_0_7_0_ALPHA_2.md.
Public itch baseline is still alpha.68. Checkpoint commit/push is complete; no further release or commit/push is
authorized by the current maintenance request. Frame-performance research and larger refactors stay
parked. This checkpoint does not declare all proposed architecture gates done.

## Chip2 HD acceptance — 2026-09-06

On 2026-09-06 the user accepted Storm-Chip2-HD.zip on a real A1200 and
requested retaining its optimizations in the main game. The normal development
source, executable and runtime assets already contain Chip1 + Chip2; they are
now the accepted development baseline, not a new release. Stormrail still needs
more than approximately 1.45 MB free Chip RAM; the precise free/largest-block
threshold and native savings remain unmeasured. Individual transition/replay
checks and other media are not inferred from this general HD acceptance.
