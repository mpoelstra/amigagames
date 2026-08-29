---
name: ship-sparkpaw-checkpoint
description: Complete and verify a Sparkpaw roadmap checkpoint by synchronizing source, SemVer, roadmap status, handoff, development history, packaged notes and the sole current release artifact set, then compare it with the currently downloadable itch.io version and produce player-facing release notes. Use when finishing, releasing, tagging, committing or pushing a Sparkpaw gameplay, renderer, art, audio, memory or packaging step.
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

## Establish the public itch baseline

Before choosing the release delta or writing user-facing notes, determine the
version currently downloadable from `https://mrdig.itch.io/sparkpaw`:

1. Run `scripts/detect_itch_release.py` from this skill directory. It reads the
   public page and derives the baseline from versioned Sparkpaw download names,
   not from the local repository version.
2. Cross-check the newest public devlog title or visible page text when
   available. Treat the downloadable artifacts as authoritative if an older
   devlog remains visible.
3. Record both the public baseline and the candidate version. Build the release
   note delta across every shipped checkpoint after that public baseline; do
   not describe only the final local commit when itch skipped intervening
   alphas.
4. If itch is unreachable, rate-limited or contains no recognizable versioned
   downloads, state that the public baseline is unverified and ask the user for
   the current itch version. Never silently substitute the repository's latest
   release or a remembered value.

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

Sparkpaw LHA releases require classic creation-capable LHa 1.14i at
`sparkpaw/.toolchain/lha/bin/lha`, or an absolute `LHA` override. Do not accept
Homebrew Lhasa as the creator: it can list, test and extract but cannot create
archives. For both HD and WHDLoad `.lha` files:

1. confirm archive creation and the packager's CRC test succeed;
2. inspect member methods and require file members to be `-lh5-`, never a
   silently restored all-`-lh0-` archive;
3. independently extract with Lhasa when available and compare byte-for-byte
   with the matching staged drawer;
4. report archive byte sizes and SHA-256 values in the release handoff.

Treat 30 characters as the maximum Amiga-safe length for every extracted path
component, including the top-level drawer. Audit both ZIP and LHA member paths
and require the packagers' name guards to pass. HD and WHDLoad must share the
same short canonical runtime filenames; ADF SPR1 creation must consume those
same short sources. A longer descriptive host artifact filename is allowed only
when its archived top-level drawer is separately shortened and versioned. Never
reintroduce the alpha.55 overlength intro/menu names: supplied real-A1200
WHDLoad testing proved an `Open`/IoErr 205 failure at the 31-character plate-2
component and accepted `intro2.spbm`, `intro3.spbm` and `readymenu.spbm`.

On a fresh Mac, install the tested Universal classic LHa
1.14i-ac20220213 build from `https://github.com/amigavision/LhA` into the local
ignored toolchain path. Keep the source URL/version in `docs/BUILDING.md`; never
commit a host binary into the repository.

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

## Write player-facing itch release notes

End every completed release handoff with a separate copy-ready section headed
exactly:

```text
Dit kun je als release notes gebruiken op basis van de huidige versie op itch:
```

Under it, provide:

- a short devlog title naming the new alpha;
- `What's new since alpha.N`, using the verified public itch baseline;
- concise bullets describing only changes a player can see, hear, control or
  experience;
- an optional brief compatibility/test note when it materially helps players.

Exclude build tooling, generators, internal architecture, cache strategies,
profilers, hashes, archive validation, source-file names and implementation
jargon. Translate technical work into its player-visible result: for example,
say “smoother combat on 68020 systems”, not “coalesced projectile sweep”. Do
not advertise fixes or platform support beyond supplied acceptance evidence.
Keep engineering verification and artifact details in the preceding developer
handoff, outside the copy-ready release notes.
