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
tooling. Confirm that `sparkpaw/dist` contains nine consistently versioned
artifacts: HD LHA/ZIP, Disk1/Disk2/Disk3 ADF, standard WHDLoad LHA/ZIP and
High RAM WHDLoad LHA/ZIP, plus all three matching extracted drawers.
Preserve explicitly protected alpha.68 baselines; archive superseded
local releases byte-identically. Never relabel a single-level WHDLoad build. A source ZIP is opt-in
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
`DefaultTool=Sparkpaw`; both WHDLoad editions use `DefaultTool=WHDLoad`
plus `SLAVE` and `PAL`. Standard 8 MB banked WHDLoad uses `NOCACHE` and
no `PRELOAD`; High RAM uses `PRELOAD`. Check against the current packager.
Do not substitute a 16-colour RomIcon fallback: classic
icons store only pen numbers, and the supplied FS-UAE Workbench does not own the
RomIcon/FullPalette pen mapping. Keep `tests/test_sparkpaw_icon.py` passing.

## Verify player ReadMe completeness

Before packaging, review `tools/game_readme.py` and each generated HD,
standard WHDLoad and High RAM `ReadMe.txt` for completeness and accuracy:

- Begin with the user-authored personal note from `docs/PERSONAL_NOTE.txt`,
  including its attribution and contact address. Preserve the wording;
  only plain-text link formatting, whitespace and line wrapping may change.
- Include the game title, actual version and edition, general introduction,
  established story, current playable content and alpha status.
- Include MrDig / MrDig Productions information, the official itch game URL,
  and contact/feedback instructions. Do not invent personal details.
- Verify machine/RAM requirements, launch/installation and quit instructions
  against the actual edition. Standard and High RAM WHDLoad must describe
  their own PRELOAD/cache settings rather than sharing contradictory advice.
- Explain menus, keyboard, on-foot JOYSTICK/JOYPAD controls and Skimmer flight
  separately. Match the implemented controls and distinguish pending fixes
  from the contents of an existing release.
- Keep the text readable on Amiga: plain ASCII, short lines, clear headings,
  and no developer-only implementation detail in ordinary player instructions.

After packaging, extract and read the actual `ReadMe.txt` from every HD and
WHDLoad ZIP/LHA. Require byte parity with the intended edition's generated
text, a complete opening personal note, current version/edition, working
references to included files and all sections above. A source-template check
alone is insufficient. Record this check in the release verification report.
Preserve older release packages; never silently rewrite a numbered release.

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

Standing user agreement: all player-facing release-note content MUST be in
English: devlog title, What's new heading, body/bullets and compatibility note.
This applies even when the conversation is Dutch. The fixed Dutch handoff
wrapper below is not part of the player-facing copy. Save the full English text
in sparkpaw/docs/RELEASE_NOTES_<version>.md and reuse that canonical text.

Cover the complete visible delta since the verified public itch version, not
only the final local commit or packaging work. Scale detail to the actual
changes: campaign flow, new playable content, scenery, enemies/rewards/hazards,
finale/audio/results and edition-specific changes. Distinguish retained baseline
features from new ones. Avoid a token five-bullet summary when it hides most of
the release, and do not inflate length with internal implementation details.

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


## Campaign checkpoint verification

Run tools/verify_checkpoint_release.py after packaging: require per-volume
ADF dependencies, all-file readback, independent ZIP/LHA extraction and icons.
WHDLoad must compile campaign plus quit hooks; verify slave version and actual
shortened extraction drawer in ReadMe. Native acceptance is medium-specific.
The current standard banked and High RAM editions both use `ExpMem` at byte 28
after `WHDLOADS` of `$580000`. Verify their distinct cache/PRELOAD policies
and memory budgets with `tools/verify_campaign_release.py`. The historical
packed/PRELOAD `$380000` candidate is not the current 8 MB banked profile;
never substitute that header or enable PRELOAD on the standard edition.
Compare asset hashes and executable behavior claims separately; a passing
archive check does not prove startup behavior. Preserve a rejected test package in
`dist/older-builds` before replacing its versioned candidate, and request a
focused user startup/loading retest before acceptance.
Document classic LHa's 496-byte tally-tick.raw lh0 incompressible fallback if
used; reject other silent storage. Prefer fresh itch download HTML/detector
and current devlog over stale web-search caches when they disagree.
