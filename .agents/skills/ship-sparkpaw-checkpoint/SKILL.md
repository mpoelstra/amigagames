---
name: ship-sparkpaw-checkpoint
description: Complete and verify a Sparkpaw roadmap checkpoint by synchronizing source, SemVer, roadmap status, handoff, development history, packaged notes and the sole current release artifact set. Use when finishing, releasing, tagging, committing or pushing a Sparkpaw gameplay, renderer, art, audio, memory or packaging step.
---

# Ship Sparkpaw Checkpoint

Use this before `git-ship`; it supplies Sparkpaw-specific release gates and does
not replace careful source selection for a commit.

## Establish scope

1. Read `CODEX_HANDOFF.md`, `sparkpaw/README.md`, the relevant development
   history and current release scripts. Check status, recent commits and tags.
2. Identify the exact roadmap checkpoint, accepted evidence and open TODOs.
   Keep unrelated user changes, ignored backups and test evidence untouched.
3. Confirm source, version strings and docs describe the same implementation.
   Preserve the PAL A1200/68020 minimum of 2 MB Chip plus 8 MB Fast RAM.

## Build and package

From `sparkpaw/`, run:

```sh
make PYTHON=../.venv/bin/python3
make release PYTHON=../.venv/bin/python3
```

Do not ship if either command fails. Review warnings and package validation,
including executable, ADF and WHDLoad archive checks performed by the release
tooling. Confirm that `sparkpaw/dist` contains exactly the five current
consistently versioned artifacts (HD LHA, HD ZIP, ADF, WHDLoad LHA and WHDLoad
ZIP) plus the extracted same-version HD review drawer. A source ZIP is opt-in
and must only be produced when MrDig explicitly requests it. Do not delete
ignored local evidence or backups while cleaning release outputs. Never infer
WHDLoad startup or gameplay acceptance from successful package assembly.

For releases with Workbench launchers, verify both HD and WHDLoad project icons
with `amigainfo`: each must retain the shared 86x93 embedded 34-colour NewIcons
layer and 86x93 three-bitplane standard OS 2.x/3.x fallback. HD must use
`DefaultTool=Sparkpaw`; WHDLoad must use `DefaultTool=WHDLoad` plus `SLAVE`,
`PRELOAD` and `PAL`. Do not substitute a 16-colour RomIcon fallback: classic
icons store only pen numbers, and the supplied FS-UAE Workbench does not own the
RomIcon/FullPalette pen mapping. Keep `tests/test_sparkpaw_icon.py` passing.

## Synchronize the checkpoint record

- Advance SemVer only when the roadmap step warrants it; never reuse a released
  version for different contents.
- Update the roadmap/checkpoint statement, `sparkpaw/README.md`,
  `CODEX_HANDOFF.md`, `docs/DEVELOPMENT_HISTORY.md` and packaged release notes.
- Record build status, memory implications, preserved renderer/gameplay
  contracts, user-supplied acceptance and every remaining TODO consistently.
- Preserve the intermittent real-Amiga two-line HUD-boundary glitch as open
  until later hardware evidence explicitly closes it.
- Never claim FS-UAE, ADF gameplay parity or real-hardware verification unless
  the user supplied that specific result.

## Audit and hand off

Inspect the final diff, artifact names and `git status`. Verify that generated
files are expected and that renderer changes are not mixed with unrelated
gameplay or asset changes. Summarize what changed, both build commands, the
artifact set, supplied verification level and remaining risks. Commit and push
only when requested, using the generic `git-ship` skill after these gates pass.
