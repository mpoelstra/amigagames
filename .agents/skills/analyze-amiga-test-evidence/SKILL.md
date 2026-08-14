---
name: analyze-amiga-test-evidence
description: Inspect user-supplied Amiga screenshots, MOV recordings, logs and emulator or real-hardware reports to isolate Sparkpaw visual, timing, animation, audio, collision, Copper, Blitter Bob or packaging regressions without overstating verification. Use when reviewing testresults evidence, comparing builds, diagnosing intermittent glitches, or recording FS-UAE, ADF and real-A1200 acceptance status.
---

# Analyze Amiga Test Evidence

Keep diagnosis read-only until the user also asks for a fix. Treat the user's
FS-UAE and real-machine observations as authoritative evidence.

## Preserve and identify evidence

1. Read `CODEX_HANDOFF.md` section “How to handle supplied test evidence” and
   the relevant current-contract sections in `sparkpaw/README.md`.
2. Preserve the bytes of supplied evidence and never transcode or delete
   recordings, sidecars, logs, backups or emulator state. When reviewing new
   MOV or PNG evidence in `sparkpaw/testresults`, also apply
   `catalog-sparkpaw-test-evidence` for its safe rename and matching sidecar.
3. Record the exact file, build/version if knowable, launch path (HD or ADF),
   machine configuration, emulator/hardware source and user-described action.
   Do not fill missing provenance with assumptions.

## Inspect temporal evidence

- Read video metadata and inspect consecutive native frames around onset,
  steady state, loop boundaries and recovery; do not judge a timing defect from
  one thumbnail.
- Compare spatially stable landmarks to distinguish camera movement, object
  motion, animation-frame changes, buffer swaps and capture scaling.
- For audio, inspect onset and tail separately and distinguish the sample itself
  from retrigger, channel priority, DMA lifecycle and capture artifacts.
- For intermittent faults, identify the smallest reproducible sequence and list
  confirmed invariants as well as the failing state.

## Classify before proposing a change

- **Sprite/Bob:** separate source pixels, masks/planes, palette mapping, cache
  indexing, restore ownership, draw order, grounding and state selection.
- **Renderer:** separate line-100 Copper staging, inactive-list changes,
  bitplane pointers, palette writes, line-252 HUD switch, line-253 pass timing,
  clean/display buffer synchronization and CPU compositing.
- **Gameplay:** separate collision support, residual overlap, wall adjacency,
  ledge predicates, falling/landing transitions, input latches and reset state.
- **Packaging:** distinguish current HD executable, ADF contents, stale FS-UAE
  save-images and obsolete artifacts.

Use repository history and the accepted baseline to locate the first relevant
change. Prefer one causal hypothesis with cited code evidence over several
speculative fixes. If implementation is requested, make one focused correction
and test adjacent boundary cases without broadening scope.

## Report verification precisely

State separately what the recording visibly proves, what the user reported,
what host inspection confirms and what remains an inference. Never convert a
successful build into an FS-UAE claim, an FS-UAE result into ADF parity, or an
emulator result into real-hardware verification. Preserve contradictory or
intermittent hardware observations as open TODOs even when an earlier run was
accepted.
