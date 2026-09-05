# Phase 7 — campaign releases and hardware validation

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
HD approval is retained. The campaign WHDLoad has only offline build/package
verification so far; earlier alpha.68 hardware observations do not apply to it.

Open gates: native WHDLoad intro/complete campaign/F10; physical A1200 HD/ADF/
WHDLoad; Gotek cold timing; Analogue Pocket. Minimum remains PAL A1200/AGA,
68020 or better, 2 MB Chip + 8 MB Fast. Intermittent physical HUD issue stays open.

Current inventory, hashes and manual instructions: RELEASE_0_7_0_ALPHA_2.md.
Public itch baseline is still alpha.68. Commit/push is authorized; itch upload
is not part of this task. Frame-performance research and larger refactors stay
parked. This checkpoint does not declare all proposed architecture gates done.
