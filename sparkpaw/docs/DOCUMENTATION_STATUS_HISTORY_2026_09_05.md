# Superseded status excerpts — 2026-09-05

Historical only. Current decisions are in CURRENT_STATUS.md. These excerpts
are preserved verbatim to retain provenance; their instructions are obsolete.

## CODEX_HANDOFF.md

# Codex handoff: Amiga game workspace

## Small optimizations retained; investigation parked — 2026-09-05

MrDig reports no noteworthy change in the requested 020 comparison and
explicitly authorizes keeping the small optimizations. Together with the
previous accepted 030 gate, this closes this candidate cycle as retained,
with no claimed perceptible improvement or measured FPS gain. The tiny overall
alpha.68 feel difference is not explained or declared solved. No separately
enumerated new platform/Stormrail result is inferred from this brief report.

The tested Bob correction, specialized Level-1 column copy and omitted
unconsumed pre-publication history exposure are now promoted to production
`src/renderer.c`. A normal `make build/sparkpaw-campaign-play` reproduces the
played executable byte-for-byte, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
The sole active `Campaign-Play-HD` drawer is unchanged. Original alpha.68 and
release inventories remain unchanged. The old cf861a build is additionally
preserved as `build/level1-production-audit/Sparkpaw-cf861a-before-promotion`.

Promotion found and fixed a missing Makefile prerequisite: both shared split
campaign targets now explicitly depend on included `src/renderer.c`. Before
that fix Make incorrectly reported the stale executable up to date. Verified:
after rebuilding, ordinary `make -q` returns 0; with `-W src/renderer.c` it
returns 1 (rebuild required). No compiler flags or object order changed.
Production renderer bytes equal the candidate already covered by the full
host suite, column ASan/UBSan test and history checks; normal link parity is
now verified as well. No additional native test is needed for identical bytes.

Recommendation: park speculative performance work and continue normal game
work. Reopen for a reproducible material regression or genuinely new evidence,
not another sequence of unmeasured tiny candidates. No release, commit, SemVer,
ADF/multidisk operation or emulator self-run occurred. This status supersedes
all older pending-acceptance and unpromoted-source statements below.


## Small renderer optimizations: 030 user gate accepted — 2026-09-05

MrDig reports “030 is ok” for the active combined small-renderer candidate.
This accepts the requested FS-UAE/68030 functional gate. The same logger-free
`Campaign-Play-HD/Sparkpaw-Play` remains active, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
Next is stock-FS-UAE/68020 whole-level feel and Stormrail regression checking
with this exact executable. No rebuild, extra drawer or logging is needed.
No 020 result, measured FPS gain or production-source promotion is implied.
This supersedes older pending-030 statements below.


## Active small-renderer-optimizations candidate — 2026-09-05

At MrDig's request, both small Level-1 optimizations are now implemented in an
isolated candidate on top of the played Bob-only source: specialized column
copy and omission of the unconsumed pre-publication actor-history exposure.
The final publication exposure stays intact. The two related changes are
bundled to avoid a user replay for each micro-change; individual FPS attribution
is therefore not possible from this combined candidate.

Active: `Campaign-Play-HD/Sparkpaw-Play`, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
The complete played Bob-only drawer is preserved at `sparkpaw/dist/older-builds/Campaign-Play-HD-old-021815`.
Exactly one full test drawer remains active. All 48 runtime assets and embedded
references were verified, and all 45 release files are byte-identical.

Full host suites passed in both the working project and the isolated candidate.
The candidate's initial host compile needed its missing `build/tmp` directory;
after creating it, the full suite passed. A 512-column ASan/UBSan test verifies
four planes, triple ring copies, padding and guard rows; 10,000 history states
match after publication. Native direct and independent object links match.
The specialized column and Bob bodies match alpha.68 after label normalization;
publication assembly matches the played parent. All 16 other objects, including
Stormrail, are byte-identical. Link addresses can still shift; FPS gain and
native gameplay acceptance remain unproven.

First user gate: FS-UAE/68030, Level 1, replay/Esc and campaign transitions,
plus Stormrail. Only after acceptance, use the same executable on stock 020.
Instructions: `sparkpaw/docs/CAMPAIGN_SMALL_OPTS_030_TEST.txt` from workspace root.
No FS-UAE was launched by Codex. No release/SemVer/ADF change occurred.
Production `src/renderer.c` and `build/sparkpaw-campaign-play` still retain the
older baseline pending candidate acceptance. Candidate source, separate patches,
map, assembly and checks live in `build/level1-production-audit/small-opts-*`
(and `column-only.patch`, `history-only.patch`). This supersedes older active
build and pending-test statements below.


## Latest user comparison: near parity — 2026-09-05

After the accepted 68030 gate, MrDig played both the original alpha.68 still in
dist and the currently staged Bob-only HD candidate. The difference is now
reported as practically negligible: both show occasional dips during extreme
running and shooting; the integrated version still feels very slightly less
smooth overall. The first water jump was an example, explicitly not a requested
hotspot. This follow-up to the 020 comparison is subjective: no FPS measurement,
controlled route/seed or causal Bob-fix gain is established. It does not add a
separate new Stormrail/020 acceptance claim.

The active executable remains SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6` in
`Campaign-Play-HD/Sparkpaw-Play`. Production source/build still retain the older
`cf861a...` baseline. No source promotion, new drawer or emulator run occurred.
The investigation now also targets general Level-1 deadline headroom; see
`LEVEL1_PRODUCTION_BASELINE_AUDIT.md`, “Broader optimization audit”. Earlier
pending-test/status paragraphs below are historical and superseded here.


## Bob-only 68030 user gate accepted — 2026-09-05

MrDig reports that the 68030 version is fine. This accepts the requested
FS-UAE/68030 functional gate for the staged Bob-only campaign candidate.
The exact executable remains SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6`.
Next is the stock-FS-UAE/68020 subjective Level-1 comparison using the same
logger-free `Campaign-Play-HD/Sparkpaw-Play`; no rebuild or second drawer is
needed. Compare whole-level feel and the early busy encounter with the prior
logger-free baseline and original alpha.68; verify Stormrail stays smooth.
No 68020 result, FPS measurement or production-source promotion is implied.

Last updated: 30 August 2026


## Active Bob-only user gate — 2026-09-05

After the user's follow-up, the verified Bob-only correction is now staged in
`sparkpaw/dist/Campaign-Play-HD/Sparkpaw-Play`, SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6`.
The prior complete logger-free drawer is preserved byte-for-byte as
`sparkpaw/dist/older-builds/Campaign-Play-HD-old-014253`. All 48 runtime assets match the previous drawer;
48 embedded references and all 45 release files were verified.

First gate is user-run FS-UAE/68030 for Level 1, Stormrail and transitions,
then stock-68020 feel only after that gate passes. Instructions:
`sparkpaw/docs/CAMPAIGN_BOB_DIRECT_030_TEST.txt`. There is exactly one active full drawer.
The column probe, RAM and generated-art findings are not combined into this
candidate. Production `src/renderer.c` and `build/sparkpaw-campaign-play` still
contain the previous baseline; the staged executable comes from the isolated
`build/level1-production-audit/bob-direct-compiled/Sparkpaw`. The reviewed patch
remains outside production source until acceptance. No emulator was launched.

## Current campaign performance investigation (2026-09-05)

MrDig has played the complete logger-free `sparkpaw/dist/Campaign-Play-HD` on
stock FS-UAE/68020. Level 1 still feels slightly but noticeably less smooth
than the original alpha.68; Stormrail is fine. Logging is not the complete
explanation. This is not an FPS measurement or a new release acceptance.

The sole active drawer remains `Campaign-Play-HD`, executable `Sparkpaw-Play`.
Its SHA256 and `build/sparkpaw-campaign-play` remain
`cf861a89702d5642c4292502c1b40bc834992c54c7d407925f685ff00dee5656`.
Read `sparkpaw/docs/LEVEL1_PERFORMANCE_EVIDENCE_REAUDIT.md` and the new
`sparkpaw/docs/LEVEL1_PRODUCTION_BASELINE_AUDIT.md` before acting.

The ordinary alpha.68 executable has now been reproduced byte-identically
without diagnostics (`fde43fce34be0ab97a787690fff8e701e17ef023ed99568f7c1a984d2f534111`).
Its verified compiler/link map is `build/layout-audit-alpha68-release-verified`.
The production comparison finds two concrete extra Level-1 helper costs: a
nested masked-Bob argument adapter and two extra instructions per entering-
column row/plane. A single Bob correction has offline register/compile checks;
a separate column probe restores alpha.68 code shape. Both remain under
`build/level1-production-audit`, not in production source or `dist`.

RAM is also larger: executable Hunk requests +86,696 bytes (ordinary memory),
Harrier samples +9,812 Chip bytes and one extra sprite slot +3,200 Fast bytes.
Actual runtime CODE/BSS/stack placement and fragmentation are unmeasured.
Generated hostile impact patterns also differ despite unchanged source files;
that separate art-isolation finding is documented, not changed.

Retain physical renderer separation, direct calls and collision caching.
Do not repeat rejected audio split, gameUpdate extraction/completion caching
or object reorder without new evidence. 48.58 FPS is alpha.45, not alpha.68;
the 49.30 log has no post_run footer. Cache attribution remains hypothetical.
No FS-UAE launch, new drawer, release, SemVer or ADF/multidisk work was done.
The direct-diagnostic-call experiment remains offline and lower priority.

### Historical campaign attribution (superseded by the audits above)

The following records earlier experiments; alpha.68/FPS and causal claims in
this historical account must be interpreted through the re-audit corrections.

Do not normalize the integrated Level-1 slowdown as an unavoidable consequence
of a larger game. A matched minimal-cadence stock-68020 A/B isolates it to the
combined renderer translation unit: the complete integrated renderer records
44.56 FPS (217 two-field plus three three-plus intervals out of 1,845), while
the same integrated campaign/game/main with a diagnostic Level-1-only
`renderer.c` records 48.23 FPS (101 two-field, zero three-plus out of the longer
2,755 run), close to alpha.68's protected 48.58 FPS. Both report zero ownership
violations. Retained-trace Bob average falls 156.13 to 123.42 raster lines and
publish-wait 118.00 to 102.68.

The diagnostic B uses cold Stormrail link stubs and is evidence only; never
ship it or enter Stormrail with it. The next implementation boundary is a real
physical Level-1/Stormrail renderer separation with explicit shared
Copper/Blitter primitives. Require Level-1 minimal cadence first, then complete
Stormrail cadence and campaign transition/replay checks. The reusable review
rule is also recorded in `run-sparkpaw-test-cycle`: runtime selection is not
compile-time performance isolation, and architectural cleanliness is accepted
only when emitted 68020 code plus all affected cadence gates remain green.

The isolated Stormrail results version was accepted by the user. The next
candidate connects the playable loop without changing alpha.68 packages:
Level 1 results offer `REPLAY LEVEL` / `CONTINUE`; Continue performs a
black/loading-bounded renderer and asset transition into Stormrail. Stormrail
results offer `REPLAY LEVEL` / `BACK TO TITLE`; replay remains resident and
Back to Title clears the campaign snapshot before the large title, existing
ready/start screen and a fresh Level 1. It is staged only in
`sparkpaw/dist/Campaign-Loop1-030-HD`; native acceptance is pending.

The first staged candidate was rejected in FS-UAE/68030 because compiling
Stormrail support still unconditionally disabled all ordinary Level-1 Bob draw
families. Evidence is preserved as
`sparkpaw/testresults/Phase 6D-rejected-campaign-level1-missing-bobs.*`. The
corrected candidate gates Bob/update/collision paths by runtime section, loads
and builds no Stormrail-only assets/caches during Level 1, clears released
cache pointers for repeated campaign loops, restores the complete story intro
and waits for the user's existing `START GAME` input. Retest is pending.


## sparkpaw/README.md

# Sparkpaw: The Stormstone Quest

## Small optimizations retained; investigation parked — 2026-09-05

MrDig reports no noteworthy change in the requested 020 comparison and
explicitly authorizes keeping the small optimizations. Together with the
previous accepted 030 gate, this closes this candidate cycle as retained,
with no claimed perceptible improvement or measured FPS gain. The tiny overall
alpha.68 feel difference is not explained or declared solved. No separately
enumerated new platform/Stormrail result is inferred from this brief report.

The tested Bob correction, specialized Level-1 column copy and omitted
unconsumed pre-publication history exposure are now promoted to production
`src/renderer.c`. A normal `make build/sparkpaw-campaign-play` reproduces the
played executable byte-for-byte, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
The sole active `Campaign-Play-HD` drawer is unchanged. Original alpha.68 and
release inventories remain unchanged. The old cf861a build is additionally
preserved as `build/level1-production-audit/Sparkpaw-cf861a-before-promotion`.

Promotion found and fixed a missing Makefile prerequisite: both shared split
campaign targets now explicitly depend on included `src/renderer.c`. Before
that fix Make incorrectly reported the stale executable up to date. Verified:
after rebuilding, ordinary `make -q` returns 0; with `-W src/renderer.c` it
returns 1 (rebuild required). No compiler flags or object order changed.
Production renderer bytes equal the candidate already covered by the full
host suite, column ASan/UBSan test and history checks; normal link parity is
now verified as well. No additional native test is needed for identical bytes.

Recommendation: park speculative performance work and continue normal game
work. Reopen for a reproducible material regression or genuinely new evidence,
not another sequence of unmeasured tiny candidates. No release, commit, SemVer,
ADF/multidisk operation or emulator self-run occurred. This status supersedes
all older pending-acceptance and unpromoted-source statements below.


## Small renderer optimizations: 030 user gate accepted — 2026-09-05

MrDig reports “030 is ok” for the active combined small-renderer candidate.
This accepts the requested FS-UAE/68030 functional gate. The same logger-free
`Campaign-Play-HD/Sparkpaw-Play` remains active, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
Next is stock-FS-UAE/68020 whole-level feel and Stormrail regression checking
with this exact executable. No rebuild, extra drawer or logging is needed.
No 020 result, measured FPS gain or production-source promotion is implied.
This supersedes older pending-030 statements below.


## Active small-renderer-optimizations candidate — 2026-09-05

At MrDig's request, both small Level-1 optimizations are now implemented in an
isolated candidate on top of the played Bob-only source: specialized column
copy and omission of the unconsumed pre-publication actor-history exposure.
The final publication exposure stays intact. The two related changes are
bundled to avoid a user replay for each micro-change; individual FPS attribution
is therefore not possible from this combined candidate.

Active: `Campaign-Play-HD/Sparkpaw-Play`, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
The complete played Bob-only drawer is preserved at `sparkpaw/dist/older-builds/Campaign-Play-HD-old-021815`.
Exactly one full test drawer remains active. All 48 runtime assets and embedded
references were verified, and all 45 release files are byte-identical.

Full host suites passed in both the working project and the isolated candidate.
The candidate's initial host compile needed its missing `build/tmp` directory;
after creating it, the full suite passed. A 512-column ASan/UBSan test verifies
four planes, triple ring copies, padding and guard rows; 10,000 history states
match after publication. Native direct and independent object links match.
The specialized column and Bob bodies match alpha.68 after label normalization;
publication assembly matches the played parent. All 16 other objects, including
Stormrail, are byte-identical. Link addresses can still shift; FPS gain and
native gameplay acceptance remain unproven.

First user gate: FS-UAE/68030, Level 1, replay/Esc and campaign transitions,
plus Stormrail. Only after acceptance, use the same executable on stock 020.
Instructions: `sparkpaw/docs/CAMPAIGN_SMALL_OPTS_030_TEST.txt` from workspace root.
No FS-UAE was launched by Codex. No release/SemVer/ADF change occurred.
Production `src/renderer.c` and `build/sparkpaw-campaign-play` still retain the
older baseline pending candidate acceptance. Candidate source, separate patches,
map, assembly and checks live in `build/level1-production-audit/small-opts-*`
(and `column-only.patch`, `history-only.patch`). This supersedes older active
build and pending-test statements below.


## Latest user comparison: near parity — 2026-09-05

After the accepted 68030 gate, MrDig played both the original alpha.68 still in
dist and the currently staged Bob-only HD candidate. The difference is now
reported as practically negligible: both show occasional dips during extreme
running and shooting; the integrated version still feels very slightly less
smooth overall. The first water jump was an example, explicitly not a requested
hotspot. This follow-up to the 020 comparison is subjective: no FPS measurement,
controlled route/seed or causal Bob-fix gain is established. It does not add a
separate new Stormrail/020 acceptance claim.

The active executable remains SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6` in
`Campaign-Play-HD/Sparkpaw-Play`. Production source/build still retain the older
`cf861a...` baseline. No source promotion, new drawer or emulator run occurred.
The investigation now also targets general Level-1 deadline headroom; see
`LEVEL1_PRODUCTION_BASELINE_AUDIT.md`, “Broader optimization audit”. Earlier
pending-test/status paragraphs below are historical and superseded here.



## README superseded active gate

Current user gate: `dist/Campaign-Play-HD/Sparkpaw-Play` now contains the
Bob-only correction (`b0eb6ca7…32ed6`), pending 68030 functionality and then
stock-020 feel. The previous logger-free baseline is archived intact. See
[the current test instructions](docs/CAMPAIGN_BOB_DIRECT_030_TEST.txt).
Production source still retains the baseline; this candidate was built in its
isolated audit tree. The following paragraph records the prior user result.

Post-alpha.68 integrated candidate: the sole active drawer is
`dist/Campaign-Play-HD`, the complete logger-free campaign. MrDig has played it
on stock FS-UAE/68020: Level 1 still feels slightly less smooth than original
alpha.68; Stormrail is fine. Logging is not the complete explanation. The
normal alpha.68 executable is now reproduced byte-identically, and the offline
[production comparison](docs/LEVEL1_PRODUCTION_BASELINE_AUDIT.md) identifies
extra Bob-wrapper and column-copy instructions, records RAM deltas and separates
proven work from unmeasured cadence/placement hypotheses. No new candidate was
staged and no runtime source was changed. Earlier candidate-status paragraphs
below are historical; the current audit and campaign-loop contract take priority.


## README old progression/results gate

Phase 6C is complete through this checkpoint. The next engine boundary is
[Phase 6D multi-level progression](docs/PHASE6D_PROGRESSION_PLAN.md): keep the
accepted instant Level-1 replay as one explicit branch and add a distinct
continue-to-next-level branch only when a real second level exists. Level-2
concept, art and route planning are intentionally handled in a separate
session.

Stormrail development preserves accepted Gate 1 boarding and Gate 2/2.5
open-flight baselines in `dist/older-builds`. Accepted Full4 is archived intact;
the complete stock-68020 cadence drawer is accepted and archived intact. The
sole active manual drawer is now `dist/Storm-Results1-030-HD`. It reuses the
exact Level-1 results screen after Gate-6 `COMPLETE`, takes all four source
values from Stormrail, uses one 150-second complete-route par and returns from
`REPLAY LEVEL` to a fresh resident departure/boarding run after a full black
fade. Host/native checks pass; its visible tally, audio, input and replay feel
await supplied 68030 acceptance.
## Handoff old work order

## Current work order

### 0. Current handoff after Phase 6C.10

Phase 6C is complete through alpha.68. Do not reopen the accepted Level-1
score/results, secret-extra-life, one-shot-audio or instant-replay work without
new evidence. The next engine boundary is Phase 6D: introduce an explicit
multi-level progression state that can choose replay or continue without
embedding Level-2 design in the Level-1 reset. Level-2 creative/layout work is
owned by a separate session. See `sparkpaw/docs/PHASE6D_PROGRESSION_PLAN.md`.

The supplied real-A1200/68030 alpha.68 gate now covers physical ADF, WHDLoad
and ordinary HD launch. Analogue Pocket alpha.68 and a physical stock-68020
remain optional separate compatibility checks, not blockers for beginning the
Phase 6D design.


## PHASE6D_PROGRESSION_PLAN.md

# Phase 6D multi-level progression boundary

Status: active post-alpha.68 design boundary. The separate Level-2 workstream
defines the first continuation as the Stormrail Skimmer interlude in
`STORMRAIL_INTERLUDE_PLAN.md`. Its focused direct-start boarding slice is now an
accepted development baseline, not a release or an integrated continuation.
Its separate Gate-2/2.5 open-flight renderer baseline is also accepted in
FS-UAE/68030 and reaches 49.26 FPS in the final low-overhead FS-UAE/68020 run.

2026-09-04 integration candidate: the decision/snapshot contract and hosttests
are in `CAMPAIGN_LOOP_CONTRACT.md`. Native acceptance of both menu branches and
asset lifetimes is pending. Stormrail keeps `REPLAY LEVEL` and adds
`BACK TO TITLE`; it does not replace replay.
The isolated work has since accepted Gate 3 combat and the current Gate-4
debris/health-pickup slice. Its complete stock-68020 workload reaches 49.96 FPS
with zero ownership violations. The isolated route has since accepted Gate 5A
enemy-pattern/pacing expansion and Gate 4D's eight-slot sunlit dust/grit field.
The latter's longer stock-68020 run reaches 49.98 FPS over 6,036 intervals,
with zero two-field intervals, one three-field interval, zero ownership
violations and 432/432 player shots. Gate 6 is now an accepted Harrier-only
fixed-camera finale at distance 15500. The complete integrated 68030 drawer
proves the route transition, one-shot arrival, 120-HP two-phase encounter,
preserved boss HP across life loss, intact dynamic wall, half-heart body
contact, fire stop, gate opening and automatic passage. The matching supplied
low-overhead complete-interlude stock-68020 run is accepted at 49.97 FPS over
5,177 intervals, with one two-field, one three-field, zero ownership violations
and 595/595 shots. The next isolated feature is the post-interlude results screen.
Its score/time contract and host tests must precede implementation; campaign
banking and Level 2 remain separate.
The first unnumbered implementation candidate now passes those host and native
checks and is staged only for 68030 user review as `Storm-Results1-030-HD`.
It uses a 150-second complete-interlude par, exact Level-1 presentation reuse
and resident return to departure. Campaign banking and Level 2 are still absent;
acceptance remains pending the visible tally/audio/input/replay test.
This file continues to own only the generic multi-level state/loading boundary.

## Purpose

Replace the temporary one-level-only results decision with an explicit flow
that can support both replay and continuation when a second playable level
exists. Preserve alpha.68's instant resident replay as the Level-1 `REPLAY`
branch; do not turn that reset function into an implicit level loader.

```text
LEVEL PLAY -> CORE COMPLETE -> RESULTS -> REPLAY CURRENT
                                     -> CONTINUE TO NEXT
```

Until Level 2 is integrated, the results screen continues to expose only
`REPLAY LEVEL`. Add `CONTINUE` only when it leads to a real, loadable next level.

## Contracts to define before implementation

- one explicit current-level identifier and next-level selection result;
- separation between a resident replay reset and a cross-level asset load;
- which values persist between levels: total score, remaining lives and any
  later progression flags;
- which values reset per level: timer, encounter awards, diamonds, Core and
  local secrets;
- failure behaviour when the next level cannot load;
- ADF storage/load budget and HD/WHDLoad path names for additional level data;
- score-screen input/menu behaviour once both `REPLAY` and `CONTINUE` exist.

## Protected alpha.68 behaviour

- `REPLAY LEVEL` fades the score display fully to black;
- both rolling targets are restored from the canonical current-level world;
- dynamic Bob history is cleared before publication;
- collectibles start at valid authored presentation coordinates;
- gameplay returns at a complete PAL frame boundary without a LOADING screen;
- Level 1 begins as a fresh attempt with score/time/lives/local progress reset;
- no change to Stage 5L/H7 display ownership, HUD split or player sprite layout.

## First implementation gate

Create a state/API proof, not Level-2 art: represent `REPLAY_CURRENT` and
`CONTINUE_NEXT` as distinct decisions, keep only replay selectable in the
shipping one-level build, and host-test the persistence/reset matrix. The proof
must not allocate or package placeholder Level-2 assets.

After the separate Level-2 session supplies an accepted asset/memory contract,
measure its load path independently on FS-UAE/68030 and then FS-UAE/68020 before
adding a second results option. Physical ADF, WHDLoad and normal HD remain
separate acceptance gates for the first multi-level build.

During isolated Stormrail development, use the compile-guarded direct-start
test target and its single active `dist` drawer so the user does not replay the
intro and Level 1 for every gate. This shortcut must never enter a production
target. Once the interlude is coherent, the integrated gate must still prove:
Level-1 results with distinct `REPLAY LEVEL` and `CONTINUE JOURNEY`; resident
Level-1 replay; black/loading-bounded Level-1 unload and Stormrail load; exact
post-Level-1 snapshot restoration on interlude replay; and single banking of
section and campaign scores.

## LEVEL1_PRODUCTION_BASELINE_AUDIT.md superseded headline status

# Level 1: verified production comparison — 2026-09-05

## Small optimizations retained; investigation parked — 2026-09-05

MrDig reports no noteworthy change in the requested 020 comparison and
explicitly authorizes keeping the small optimizations. Together with the
previous accepted 030 gate, this closes this candidate cycle as retained,
with no claimed perceptible improvement or measured FPS gain. The tiny overall
alpha.68 feel difference is not explained or declared solved. No separately
enumerated new platform/Stormrail result is inferred from this brief report.

The tested Bob correction, specialized Level-1 column copy and omitted
unconsumed pre-publication history exposure are now promoted to production
`src/renderer.c`. A normal `make build/sparkpaw-campaign-play` reproduces the
played executable byte-for-byte, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
The sole active `Campaign-Play-HD` drawer is unchanged. Original alpha.68 and
release inventories remain unchanged. The old cf861a build is additionally
preserved as `build/level1-production-audit/Sparkpaw-cf861a-before-promotion`.

Promotion found and fixed a missing Makefile prerequisite: both shared split
campaign targets now explicitly depend on included `src/renderer.c`. Before
that fix Make incorrectly reported the stale executable up to date. Verified:
after rebuilding, ordinary `make -q` returns 0; with `-W src/renderer.c` it
returns 1 (rebuild required). No compiler flags or object order changed.
Production renderer bytes equal the candidate already covered by the full
host suite, column ASan/UBSan test and history checks; normal link parity is
now verified as well. No additional native test is needed for identical bytes.

Recommendation: park speculative performance work and continue normal game
work. Reopen for a reproducible material regression or genuinely new evidence,
not another sequence of unmeasured tiny candidates. No release, commit, SemVer,
ADF/multidisk operation or emulator self-run occurred. This status supersedes
all older pending-acceptance and unpromoted-source statements below.


## Small renderer optimizations: 030 user gate accepted — 2026-09-05

MrDig reports “030 is ok” for the active combined small-renderer candidate.
This accepts the requested FS-UAE/68030 functional gate. The same logger-free
`Campaign-Play-HD/Sparkpaw-Play` remains active, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
Next is stock-FS-UAE/68020 whole-level feel and Stormrail regression checking
with this exact executable. No rebuild, extra drawer or logging is needed.
No 020 result, measured FPS gain or production-source promotion is implied.
This supersedes older pending-030 statements below.


## Active small-renderer-optimizations candidate — 2026-09-05

At MrDig's request, both small Level-1 optimizations are now implemented in an
isolated candidate on top of the played Bob-only source: specialized column
copy and omission of the unconsumed pre-publication actor-history exposure.
The final publication exposure stays intact. The two related changes are
bundled to avoid a user replay for each micro-change; individual FPS attribution
is therefore not possible from this combined candidate.

Active: `Campaign-Play-HD/Sparkpaw-Play`, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
The complete played Bob-only drawer is preserved at `sparkpaw/dist/older-builds/Campaign-Play-HD-old-021815`.
Exactly one full test drawer remains active. All 48 runtime assets and embedded
references were verified, and all 45 release files are byte-identical.

Full host suites passed in both the working project and the isolated candidate.
The candidate's initial host compile needed its missing `build/tmp` directory;
after creating it, the full suite passed. A 512-column ASan/UBSan test verifies
four planes, triple ring copies, padding and guard rows; 10,000 history states
match after publication. Native direct and independent object links match.
The specialized column and Bob bodies match alpha.68 after label normalization;
publication assembly matches the played parent. All 16 other objects, including
Stormrail, are byte-identical. Link addresses can still shift; FPS gain and
native gameplay acceptance remain unproven.

First user gate: FS-UAE/68030, Level 1, replay/Esc and campaign transitions,
plus Stormrail. Only after acceptance, use the same executable on stock 020.
Instructions: `sparkpaw/docs/CAMPAIGN_SMALL_OPTS_030_TEST.txt` from workspace root.
No FS-UAE was launched by Codex. No release/SemVer/ADF change occurred.
Production `src/renderer.c` and `build/sparkpaw-campaign-play` still retain the
older baseline pending candidate acceptance. Candidate source, separate patches,
map, assembly and checks live in `build/level1-production-audit/small-opts-*`
(and `column-only.patch`, `history-only.patch`). This supersedes older active
build and pending-test statements below.


## Latest user comparison: near parity — 2026-09-05

After the accepted 68030 gate, MrDig played both the original alpha.68 still in
dist and the currently staged Bob-only HD candidate. The difference is now
reported as practically negligible: both show occasional dips during extreme
running and shooting; the integrated version still feels very slightly less
smooth overall. The first water jump was an example, explicitly not a requested
hotspot. This follow-up to the 020 comparison is subjective: no FPS measurement,
controlled route/seed or causal Bob-fix gain is established. It does not add a
separate new Stormrail/020 acceptance claim.

The active executable remains SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6` in
`Campaign-Play-HD/Sparkpaw-Play`. Production source/build still retain the older
`cf861a...` baseline. No source promotion, new drawer or emulator run occurred.
The investigation now also targets general Level-1 deadline headroom; see
`LEVEL1_PRODUCTION_BASELINE_AUDIT.md`, “Broader optimization audit”. Earlier
pending-test/status paragraphs below are historical and superseded here.


## Bob-only 68030 user gate accepted — 2026-09-05

MrDig reports that the 68030 version is fine. This accepts the requested
FS-UAE/68030 functional gate for the staged Bob-only campaign candidate.
The exact executable remains SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6`.
Next is the stock-FS-UAE/68020 subjective Level-1 comparison using the same
logger-free `Campaign-Play-HD/Sparkpaw-Play`; no rebuild or second drawer is
needed. Compare whole-level feel and the early busy encounter with the prior
logger-free baseline and original alpha.68; verify Stormrail stays smooth.
No 68020 result, FPS measurement or production-source promotion is implied.

## Active Bob-only user gate — 2026-09-05

After the user's follow-up, the verified Bob-only correction is now staged in
`dist/Campaign-Play-HD/Sparkpaw-Play`, SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6`.
The prior complete logger-free drawer is preserved byte-for-byte as
`dist/older-builds/Campaign-Play-HD-old-014253`. All 48 runtime assets match the previous drawer;
48 embedded references and all 45 release files were verified.

First gate is user-run FS-UAE/68030 for Level 1, Stormrail and transitions,
then stock-68020 feel only after that gate passes. Instructions:
`docs/CAMPAIGN_BOB_DIRECT_030_TEST.txt`. There is exactly one active full drawer.
The column probe, RAM and generated-art findings are not combined into this
candidate. Production `src/renderer.c` and `build/sparkpaw-campaign-play` still
contain the previous baseline; the staged executable comes from the isolated
`build/level1-production-audit/bob-direct-compiled/Sparkpaw`. The reviewed patch
remains outside production source until acceptance. No emulator was launched.

Status: offline investigation, no runtime acceptance and no new staged drawer.
This extends `LEVEL1_PERFORMANCE_EVIDENCE_REAUDIT.md`; its corrections remain
binding. Production source and the played executable are unchanged.

## User result and conclusion

MrDig played the complete logger-free `dist/Campaign-Play-HD` on stock
FS-UAE/68020. Level 1 remains slightly but noticeably less smooth than the
original alpha.68 release. Stormrail is fine on 020. Logging therefore cannot
be the complete explanation. This is subjective whole-level evidence, not a
new FPS number or an independently enumerated transition/replay acceptance.

The strongest new evidence is **actual extra executed renderer-helper work**:

1. Ordinary masked Level-1 Bobs enter an additional argument-repacking wrapper
   introduced for Stormrail's separate cache-plane height.
2. The specialized entering-column copy executes two unnecessary additional
   instructions for each of its 208 rows on each of four planes.

These are proven differences in production compiler output against a
byte-identical release reproduction. Their contribution to perceived cadence
has not been measured. Neither executable growth nor cache conflicts have been
established as the cause. Do not combine fixes or claim the whole regression
solved from these offline results.


## LEVEL1_PERFORMANCE_EVIDENCE_REAUDIT.md superseded headline status

# Level-1 performance evidence re-audit — 2026-09-05

## Small optimizations retained; investigation parked — 2026-09-05

MrDig reports no noteworthy change in the requested 020 comparison and
explicitly authorizes keeping the small optimizations. Together with the
previous accepted 030 gate, this closes this candidate cycle as retained,
with no claimed perceptible improvement or measured FPS gain. The tiny overall
alpha.68 feel difference is not explained or declared solved. No separately
enumerated new platform/Stormrail result is inferred from this brief report.

The tested Bob correction, specialized Level-1 column copy and omitted
unconsumed pre-publication history exposure are now promoted to production
`src/renderer.c`. A normal `make build/sparkpaw-campaign-play` reproduces the
played executable byte-for-byte, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
The sole active `Campaign-Play-HD` drawer is unchanged. Original alpha.68 and
release inventories remain unchanged. The old cf861a build is additionally
preserved as `build/level1-production-audit/Sparkpaw-cf861a-before-promotion`.

Promotion found and fixed a missing Makefile prerequisite: both shared split
campaign targets now explicitly depend on included `src/renderer.c`. Before
that fix Make incorrectly reported the stale executable up to date. Verified:
after rebuilding, ordinary `make -q` returns 0; with `-W src/renderer.c` it
returns 1 (rebuild required). No compiler flags or object order changed.
Production renderer bytes equal the candidate already covered by the full
host suite, column ASan/UBSan test and history checks; normal link parity is
now verified as well. No additional native test is needed for identical bytes.

Recommendation: park speculative performance work and continue normal game
work. Reopen for a reproducible material regression or genuinely new evidence,
not another sequence of unmeasured tiny candidates. No release, commit, SemVer,
ADF/multidisk operation or emulator self-run occurred. This status supersedes
all older pending-acceptance and unpromoted-source statements below.


## Small renderer optimizations: 030 user gate accepted — 2026-09-05

MrDig reports “030 is ok” for the active combined small-renderer candidate.
This accepts the requested FS-UAE/68030 functional gate. The same logger-free
`Campaign-Play-HD/Sparkpaw-Play` remains active, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
Next is stock-FS-UAE/68020 whole-level feel and Stormrail regression checking
with this exact executable. No rebuild, extra drawer or logging is needed.
No 020 result, measured FPS gain or production-source promotion is implied.
This supersedes older pending-030 statements below.


## Active small-renderer-optimizations candidate — 2026-09-05

At MrDig's request, both small Level-1 optimizations are now implemented in an
isolated candidate on top of the played Bob-only source: specialized column
copy and omission of the unconsumed pre-publication actor-history exposure.
The final publication exposure stays intact. The two related changes are
bundled to avoid a user replay for each micro-change; individual FPS attribution
is therefore not possible from this combined candidate.

Active: `Campaign-Play-HD/Sparkpaw-Play`, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
The complete played Bob-only drawer is preserved at `sparkpaw/dist/older-builds/Campaign-Play-HD-old-021815`.
Exactly one full test drawer remains active. All 48 runtime assets and embedded
references were verified, and all 45 release files are byte-identical.

Full host suites passed in both the working project and the isolated candidate.
The candidate's initial host compile needed its missing `build/tmp` directory;
after creating it, the full suite passed. A 512-column ASan/UBSan test verifies
four planes, triple ring copies, padding and guard rows; 10,000 history states
match after publication. Native direct and independent object links match.
The specialized column and Bob bodies match alpha.68 after label normalization;
publication assembly matches the played parent. All 16 other objects, including
Stormrail, are byte-identical. Link addresses can still shift; FPS gain and
native gameplay acceptance remain unproven.

First user gate: FS-UAE/68030, Level 1, replay/Esc and campaign transitions,
plus Stormrail. Only after acceptance, use the same executable on stock 020.
Instructions: `sparkpaw/docs/CAMPAIGN_SMALL_OPTS_030_TEST.txt` from workspace root.
No FS-UAE was launched by Codex. No release/SemVer/ADF change occurred.
Production `src/renderer.c` and `build/sparkpaw-campaign-play` still retain the
older baseline pending candidate acceptance. Candidate source, separate patches,
map, assembly and checks live in `build/level1-production-audit/small-opts-*`
(and `column-only.patch`, `history-only.patch`). This supersedes older active
build and pending-test statements below.


## Latest user comparison: near parity — 2026-09-05

After the accepted 68030 gate, MrDig played both the original alpha.68 still in
dist and the currently staged Bob-only HD candidate. The difference is now
reported as practically negligible: both show occasional dips during extreme
running and shooting; the integrated version still feels very slightly less
smooth overall. The first water jump was an example, explicitly not a requested
hotspot. This follow-up to the 020 comparison is subjective: no FPS measurement,
controlled route/seed or causal Bob-fix gain is established. It does not add a
separate new Stormrail/020 acceptance claim.

The active executable remains SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6` in
`Campaign-Play-HD/Sparkpaw-Play`. Production source/build still retain the older
`cf861a...` baseline. No source promotion, new drawer or emulator run occurred.
The investigation now also targets general Level-1 deadline headroom; see
`LEVEL1_PRODUCTION_BASELINE_AUDIT.md`, “Broader optimization audit”. Earlier
pending-test/status paragraphs below are historical and superseded here.


## Active Bob-only user gate — 2026-09-05

After the user's follow-up, the verified Bob-only correction is now staged in
`dist/Campaign-Play-HD/Sparkpaw-Play`, SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6`.
The prior complete logger-free drawer is preserved byte-for-byte as
`dist/older-builds/Campaign-Play-HD-old-014253`. All 48 runtime assets match the previous drawer;
48 embedded references and all 45 release files were verified.

First gate is user-run FS-UAE/68030 for Level 1, Stormrail and transitions,
then stock-68020 feel only after that gate passes. Instructions:
`docs/CAMPAIGN_BOB_DIRECT_030_TEST.txt`. There is exactly one active full drawer.
The column probe, RAM and generated-art findings are not combined into this
candidate. Production `src/renderer.c` and `build/sparkpaw-campaign-play` still
contain the previous baseline; the staged executable comes from the isolated
`build/level1-production-audit/bob-direct-compiled/Sparkpaw`. The reviewed patch
remains outside production source until acceptance. No emulator was launched.

The collision-cache implementation remains the retained candidate, not a claim
that production Level 1 has achieved the user's alpha.68 feel. No new emulator
run was performed for this audit.

## New logger-free result and baseline comparison

MrDig has now played the complete `Campaign-Play-HD` on stock FS-UAE/68020:
Level 1 remains slightly but noticeably less smooth than original alpha.68;
Stormrail is fine. The exact played SHA256 is
`cf861a89702d5642c4292502c1b40bc834992c54c7d407925f685ff00dee5656`.
This supplies no FPS, but excludes logging as the complete explanation.

The ordinary alpha.68 release was subsequently reproduced byte-identically,
without diagnosis flags. `LEVEL1_PRODUCTION_BASELINE_AUDIT.md` records provenance,
the production frame chain, two concrete extra renderer-helper costs, RAM
requests and generated asset drift. Its independent offline Bob correction
and column probe are not staged or accepted. Production source and the current
drawer remain unchanged. This comparison supersedes the observer experiment
as the immediate investigation priority; historical corrections below remain.


## C_ARCHITECTURE_AND_PERFORMANCE_PLAN.md superseded headline status

# Sparkpaw C architecture and performance plan

> Current status (2026-09-05): Performance research is explicitly parked. Retained renderer changes are in production. Gate 2 is the next bounded proposal: the section enum already exists; extend it into cold asset selection. Old experiment instructions below are historical, not pending tasks. See [status index](CURRENT_STATUS.md).

## Small optimizations retained; investigation parked — 2026-09-05

MrDig reports no noteworthy change in the requested 020 comparison and
explicitly authorizes keeping the small optimizations. Together with the
previous accepted 030 gate, this closes this candidate cycle as retained,
with no claimed perceptible improvement or measured FPS gain. The tiny overall
alpha.68 feel difference is not explained or declared solved. No separately
enumerated new platform/Stormrail result is inferred from this brief report.

The tested Bob correction, specialized Level-1 column copy and omitted
unconsumed pre-publication history exposure are now promoted to production
`src/renderer.c`. A normal `make build/sparkpaw-campaign-play` reproduces the
played executable byte-for-byte, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
The sole active `Campaign-Play-HD` drawer is unchanged. Original alpha.68 and
release inventories remain unchanged. The old cf861a build is additionally
preserved as `build/level1-production-audit/Sparkpaw-cf861a-before-promotion`.

Promotion found and fixed a missing Makefile prerequisite: both shared split
campaign targets now explicitly depend on included `src/renderer.c`. Before
that fix Make incorrectly reported the stale executable up to date. Verified:
after rebuilding, ordinary `make -q` returns 0; with `-W src/renderer.c` it
returns 1 (rebuild required). No compiler flags or object order changed.
Production renderer bytes equal the candidate already covered by the full
host suite, column ASan/UBSan test and history checks; normal link parity is
now verified as well. No additional native test is needed for identical bytes.

Recommendation: park speculative performance work and continue normal game
work. Reopen for a reproducible material regression or genuinely new evidence,
not another sequence of unmeasured tiny candidates. No release, commit, SemVer,
ADF/multidisk operation or emulator self-run occurred. This status supersedes
all older pending-acceptance and unpromoted-source statements below.


## Small renderer optimizations: 030 user gate accepted — 2026-09-05

MrDig reports “030 is ok” for the active combined small-renderer candidate.
This accepts the requested FS-UAE/68030 functional gate. The same logger-free
`Campaign-Play-HD/Sparkpaw-Play` remains active, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
Next is stock-FS-UAE/68020 whole-level feel and Stormrail regression checking
with this exact executable. No rebuild, extra drawer or logging is needed.
No 020 result, measured FPS gain or production-source promotion is implied.
This supersedes older pending-030 statements below.


## Active small-renderer-optimizations candidate — 2026-09-05

At MrDig's request, both small Level-1 optimizations are now implemented in an
isolated candidate on top of the played Bob-only source: specialized column
copy and omission of the unconsumed pre-publication actor-history exposure.
The final publication exposure stays intact. The two related changes are
bundled to avoid a user replay for each micro-change; individual FPS attribution
is therefore not possible from this combined candidate.

Active: `Campaign-Play-HD/Sparkpaw-Play`, SHA256
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
The complete played Bob-only drawer is preserved at `sparkpaw/dist/older-builds/Campaign-Play-HD-old-021815`.
Exactly one full test drawer remains active. All 48 runtime assets and embedded
references were verified, and all 45 release files are byte-identical.

Full host suites passed in both the working project and the isolated candidate.
The candidate's initial host compile needed its missing `build/tmp` directory;
after creating it, the full suite passed. A 512-column ASan/UBSan test verifies
four planes, triple ring copies, padding and guard rows; 10,000 history states
match after publication. Native direct and independent object links match.
The specialized column and Bob bodies match alpha.68 after label normalization;
publication assembly matches the played parent. All 16 other objects, including
Stormrail, are byte-identical. Link addresses can still shift; FPS gain and
native gameplay acceptance remain unproven.

First user gate: FS-UAE/68030, Level 1, replay/Esc and campaign transitions,
plus Stormrail. Only after acceptance, use the same executable on stock 020.
Instructions: `sparkpaw/docs/CAMPAIGN_SMALL_OPTS_030_TEST.txt` from workspace root.
No FS-UAE was launched by Codex. No release/SemVer/ADF change occurred.
Production `src/renderer.c` and `build/sparkpaw-campaign-play` still retain the
older baseline pending candidate acceptance. Candidate source, separate patches,
map, assembly and checks live in `build/level1-production-audit/small-opts-*`
(and `column-only.patch`, `history-only.patch`). This supersedes older active
build and pending-test statements below.


## Latest user comparison: near parity — 2026-09-05

After the accepted 68030 gate, MrDig played both the original alpha.68 still in
dist and the currently staged Bob-only HD candidate. The difference is now
reported as practically negligible: both show occasional dips during extreme
running and shooting; the integrated version still feels very slightly less
smooth overall. The first water jump was an example, explicitly not a requested
hotspot. This follow-up to the 020 comparison is subjective: no FPS measurement,
controlled route/seed or causal Bob-fix gain is established. It does not add a
separate new Stormrail/020 acceptance claim.

The active executable remains SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6` in
`Campaign-Play-HD/Sparkpaw-Play`. Production source/build still retain the older
`cf861a...` baseline. No source promotion, new drawer or emulator run occurred.
The investigation now also targets general Level-1 deadline headroom; see
`LEVEL1_PRODUCTION_BASELINE_AUDIT.md`, “Broader optimization audit”. Earlier
pending-test/status paragraphs below are historical and superseded here.


## Bob-only 68030 user gate accepted — 2026-09-05

MrDig reports that the 68030 version is fine. This accepts the requested
FS-UAE/68030 functional gate for the staged Bob-only campaign candidate.
The exact executable remains SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6`.
Next is the stock-FS-UAE/68020 subjective Level-1 comparison using the same
logger-free `Campaign-Play-HD/Sparkpaw-Play`; no rebuild or second drawer is
needed. Compare whole-level feel and the early busy encounter with the prior
logger-free baseline and original alpha.68; verify Stormrail stays smooth.
No 68020 result, FPS measurement or production-source promotion is implied.

## Active Bob-only user gate — 2026-09-05

After the user's follow-up, the verified Bob-only correction is now staged in
`dist/Campaign-Play-HD/Sparkpaw-Play`, SHA256
`b0eb6ca75269094f78cd5f9784783b90b8fc298e33c22af5e7ee549da3832ed6`.
The prior complete logger-free drawer is preserved byte-for-byte as
`dist/older-builds/Campaign-Play-HD-old-014253`. All 48 runtime assets match the previous drawer;
48 embedded references and all 45 release files were verified.

First gate is user-run FS-UAE/68030 for Level 1, Stormrail and transitions,
then stock-68020 feel only after that gate passes. Instructions:
`docs/CAMPAIGN_BOB_DIRECT_030_TEST.txt`. There is exactly one active full drawer.
The column probe, RAM and generated-art findings are not combined into this
candidate. Production `src/renderer.c` and `build/sparkpaw-campaign-play` still
contain the previous baseline; the staged executable comes from the isolated
`build/level1-production-audit/bob-direct-compiled/Sparkpaw`. The reviewed patch
remains outside production source until acceptance. No emulator was launched.

Status: active post-alpha.68 engineering plan for the integrated Level 1 plus
Stormrail campaign. This combines the former architecture review with measured
68020 gates. Architecture is accepted only when player-visible behaviour and
cadence remain accepted.

Evidence correction (2026-09-05): read
`LEVEL1_PERFORMANCE_EVIDENCE_REAUDIT.md` before selecting the next experiment.
The 48.58-FPS number below originates from alpha.45, not a direct alpha.68
measurement. Later claims that function extraction proved a cache-layout
regression are hypotheses, not established causes. Six additional diagnostic
wrapper calls are confirmed in the integrated build; production cadence/feel
acceptance remains open.

Current result (2026-09-05): MrDig played the complete logger-free
`Campaign-Play-HD` on stock FS-UAE/68020. Level 1 still feels slightly but
noticeably less smooth than original alpha.68; Stormrail is fine. No FPS is
inferred. Logging is not the complete explanation. Read
`LEVEL1_PRODUCTION_BASELINE_AUDIT.md` for the new byte-identical alpha.68
reproduction, whole-frame/helper comparison, explicit RAM deltas and generated
asset differences. A Bob-adapter correction is validated offline only; a
separate column compiler probe exposes 1,664 extra instructions per column.
Production source and the sole active drawer are unchanged. These concrete
helper costs take priority over the direct-diagnostic-call experiment or new
layout speculation. No runtime placement or full causal cadence claim exists.
