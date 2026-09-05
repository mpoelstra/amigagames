---
name: run-sparkpaw-test-cycle
description: Package, stage, guide and evaluate Sparkpaw diagnostic or A/B test builds through the established dist-folder workflow. Use when preparing a build for the user to play in FS-UAE, requesting renderdiag.log evidence, comparing candidate A/B performance or visuals, gating first on 68030 and then on 68020, or cleaning completed test drawers from sparkpaw/dist.
---

# Run Sparkpaw Test Cycle

Follow this workflow for every user-played Sparkpaw diagnostic or optimization
candidate. Keep packaging, testing and acceptance explicit and reproducible.

## Establish the test contract

1. Read `CODEX_HANDOFF.md`, `sparkpaw/README.md` and the relevant active plan.
2. Inspect `git status`, the current `sparkpaw/dist` layout and applicable
   Makefile package targets before building.
3. State the single hypothesis, changed subsystem, comparison baseline and
   acceptance criterion. Do not mix unrelated candidates.
4. Preserve the accepted renderer, gameplay and packaging contracts unless the
   test explicitly targets one of them.
5. Treat every unaccepted change as an unnumbered candidate. Do not edit
   SemVer, release notes, roadmap completion, packaged notes or release
   artifact names, and do not run `make release`. Only
   `ship-sparkpaw-checkpoint`, after explicit user release/shipping intent,
   may cross that boundary.
6. Inventory and hash every current `sparkpaw/dist/Sparkpaw-*` release file
   before staging. Stop if the documented current release set is unexpectedly
   absent unless the user explicitly authorizes candidate work while recovery
   remains pending. Require the inventory to be byte-identical afterwards.

## Audit architecture as part of performance

When a new level, mode or campaign section makes an accepted older section
slower, do not accept total program growth as the explanation. Establish
whether the older hot path is genuinely isolated at compile time as well as at
runtime.

- A runtime `if(sectionActive)` proves behavioural selection, not performance
  isolation. Inspect generated `-O2 -cpu=68020` assembly, function size,
  translation-unit reachability, hot-structure layout/stride and allocation
  placement whenever another section is compiled into the same module.
- Audit the complete per-frame chain, not only the visually worst scene or the
  largest measured renderer phase. Compare input, game/update, collision,
  enemies, projectiles, audio, renderer and publish control flow with the
  accepted source and compiler output. A broadly repeated small cost may be
  most visible in one busy encounter without originating there.
- Inspect final object/function order and link placement as well as each hot
  function in isolation. On the MC68020's small direct-mapped instruction
  cache, an extraction that produces cleaner or baseline-matching local
  assembly can still regress by shifting neighbouring hot code or cache-set
  relationships. Treat source order, object order and executable-size changes
  as testable performance inputs; change one placement hypothesis at a time.
- Compare a complete integrated build with a compile-isolated diagnostic that
  keeps the rest of campaign/game/main identical. Use the same minimal cadence
  instrumentation and normalize different run lengths by interval shares.
- If compile isolation materially restores cadence, select a real module/API
  ownership boundary. Never ship diagnostic stubs or simply duplicate a large
  hot path inside the same translation unit.
- A temporary dual-compiled legacy body may establish a reversible module seam
  without simultaneously rewriting proven low-level code. Treat it as a
  migration candidate only: measure executable/BSS/runtime memory, prove both
  sections, then extract shared primitives once before release or media-volume
  work. Do not mistake source inclusion or code duplication for the target
  architecture.
- Separate campaign state, section-local state and runtime renderer/asset
  ownership. Share only measured low-level primitives whose contracts are
  genuinely common; future media-volume selection belongs above gameplay and
  renderers.
- Treat shorter files and cleaner abstractions as hypotheses, not wins. Retain
  an architecture change only after host/native checks and matched cadence for
  every affected section. A gain in one level may not be paid for by another.
- Compare the test harness itself with the baseline at source and assembly
  level. Cache frame-stable predicates once, count hot calls, and verify that
  cadence-only completion holds, input checks and logging branches do not make
  one candidate execute extra work. Identical log fields do not by themselves
  prove identical observer cost.
- Trace historical numbers back to their original version and evidence. Do
  not rename an inherited performance floor after the current release. Treat
  manually played runs as workload-dependent; candidate rejection does not
  prove a particular CPU-cache mechanism. Use `tools/audit_link_layout.py` for
  address investigations and require byte parity before attributing a map to
  the played executable.
- When a historical instrumented build hangs or fails before gameplay, mark it
  invalid and stop asking the user to retry bounded variants unless a specific
  native-only diagnosis requires it. Fall back to protected evidence plus
  offline source/compiler comparison; never infer cadence from a failed loader.
- Revisit the active architecture/performance plan after every meaningful
  profile or cadence result. Record rejected structural experiments so they
  are not rediscovered as unmeasured “cleanup.”

## Package self-contained test drawers

- Stage ordinary HD candidates only with `tools/stage_hd_test.py`. It consumes
  `tools/make_release.py:RUNTIME_FILES`, validates exact byte parity and
  Amiga-safe names, preserves a replaced drawer under `dist/older-builds` and
  rejects any release-inventory change. Do not hand-copy assets or use a
  Makefile asset list as an independent package manifest.
- `stage_hd_test.py` also discovers literal `PROGDIR:assets/runtime/...`
  references embedded in the selected executable. Treat its discovery count
  as mandatory packaging evidence for compile-guarded builds; never assume the
  current release manifest alone covers an unreleased mode. Use
  `--extra-runtime` only for genuinely dynamic paths that cannot be embedded.
- Put every user-testable build inside `sparkpaw/dist`; the user mounts this
  directory directly in FS-UAE.
- Make each active drawer self-contained: include its uniquely named
  executable, all required `assets/runtime` files and a short `ReadMe.txt` with
  exact controls and evidence-saving instructions.
- For A/B work, create two complete sibling drawers. Never require the user to
  rename executables, swap assets or move a log between drawers.
- Use names that identify stage/candidate, A or B, machine gate and launch path,
  for example `Stage2-Thing-A-Baseline-FS-UAE-68030-HD`.
- Ensure each run writes `renderdiag.log` into its own drawer. Do not share a
  log pathname between A and B.
- Production builds must not contain diagnostic logging, profiling or test
  input code.
- Keep every Amiga-side path component, including the extracted root drawer and
  runtime filenames, at 30 characters or fewer. Reuse production's canonical
  short asset names across HD, WHDLoad and ADF tests; do not rely on a ZIP/LHA
  extractor to preserve longer names.
- Before handoff, compare the staged drawer with the complete authoritative
  manifest. A focused intro build still requires MrDig to traverse intro,
  title, LOADING, CHARGING and the ready menu; checking only the changed first
  screen is insufficient user acceptance.
- `sparkpaw/dist` is exclusively MrDig's manual test surface. Codex may stage
  and read/hash/manifest-check a drawer there, but must never launch or boot an
  executable from `dist` and must not ask for permission to do so. Every
  Codex-run FS-UAE proof belongs below `build/fsuae-selftest`; after staging,
  MrDig performs the actual boot from the `dist` drawer.

### Keep quick real-A1200 HD ZIPs minimal

When the user requests one quick HD ZIP for testing on a real A1200, optimize
the transfer package as well as the executable:

- use `tools/make_release.py`'s current `RUNTIME_FILES` manifest as the
  authoritative asset list, or an equivalently verified strict subset for the
  exact executable;
- never copy the complete `assets/runtime/` directory into such a package;
- exclude renderbench files, superseded status/intro variants, manifests and
  other development-only or unreachable runtime artifacts;
- include exactly one executable, one concise `ReadMe.txt` and only the files
  that executable can load;
- deliver one ZIP unless the user explicitly asks for an extracted drawer,
  LHA, ADF, WHDLoad package or comparison pair;
- when an LHA is explicitly requested, use the release packager's classic
  creation-capable LHa path and require `-lh5-` members plus a successful CRC
  test; Homebrew Lhasa alone is extraction-only;
- test the archive, report its compressed size and compare its contents with
  the ordinary release ZIP so unexpected growth is caught before delivery.

Do not trade away self-containment or byte parity merely to reduce ZIP size.
The goal is the smallest complete hardware-test package, not a hand-curated
archive whose loading path has not been verified.

Keep the `dist` root uncluttered:

- retain the sole current alpha release artifact set and extracted drawer;
- retain at most one active diagnostic set (one drawer or one A/B pair);
- move superseded test drawers and their `.uaem` launchers intact into
  `sparkpaw/dist/older-builds`;
- never delete, transcode or overwrite logs, recordings, save states, ignored
  backups or user evidence while tidying;
- leave `dist/my-files` untouched.

## Choose the lightest sufficient instrumentation

Decide the logging level before packaging. Logging is part of the workload and
can reduce 68020 cadence; never enable every profiler merely because it exists.

1. **Production/visual build:** compile diagnostics out. Use for release parity,
   subjective smoothness and hardware acceptance, but do not claim measured FPS
   when the build emits no cadence evidence.
2. **Minimal cadence build:** collect only presentation intervals, field-count
   distribution, missed deadlines and ownership violations. Use this for the
   trustworthy 68020 FPS gate and comparisons against the 48.58 FPS baseline.
3. **Targeted profiler:** add only the candidate function/family and the few
   scopes required to separate CPU work, Blitter waits and Chip-bus effects.
   Use while locating or proving one suspected cause.
4. **Broad profiler:** instrument many subsystems only during discovery when a
   hotspot is still unknown. Treat its displayed FPS as observer-contaminated,
   not as production cadence, and follow it with a minimal-cadence build.

For every level:

- accumulate counters and CIA timings in memory during play; do not perform
  per-frame or per-call file writes;
- write one bounded `renderdiag.log` only when the user presses the left mouse
  button;
- keep A and B instrumentation identical when comparing their numbers;
- document enabled scopes/macros in the drawer ReadMe and log header where
  practical;
- avoid nested scopes unless the parent/child split answers a concrete question;
- measure logging overhead with a paired minimal build when it could influence
  the conclusion;
- remove all diagnostic code paths from production builds at compile time.

## Respect the diagnostic save lifecycle

The established Sparkpaw diagnostic path does **not** return to Workbench.
After a left-mouse press it waits for button release, prepares a safe debug
flush, writes `renderdiag.log`, then deliberately holds the final image in a
permanent idle loop. Treat that frozen image as the expected save-complete
state, not as a crash or an instruction to wait for Workbench.

- Tell the user to press the left mouse button once after the requested run.
- Expect the image to freeze immediately while/after the bounded log is saved.
- Tell the user to wait a few seconds, then stop or reset FS-UAE; Workbench will
  not return from this diagnostic executable.
- Read the log from the exact drawer and confirm that it exists, is non-empty
  and belongs to the current run before analyzing it.
- Check the `post_run` footer for full render logs and recheck size/hash before
  preserving them. A stable file without its expected footer may be an
  interrupted flush; label complete header measurements separately from
  incomplete trace evidence. Saving can take substantially longer than a few
  seconds on 68020.
- Do not ask for repeated mouse presses, a clean application exit or Workbench
  restoration unless a future executable explicitly implements that behavior.
- Startup-only diagnostics may write their own bounded log during loading and
  therefore need no mouse press; document that exception in their ReadMe.

## Gate strictly on FS-UAE/68030 first

1. Build/package only the 68030 drawer or A/B pair needed for the first gate.
2. Give the user the exact drawer name(s), executable name(s), scene/action to
   reproduce and approximate play duration.
3. Tell the user to press the left mouse button after the run, expect the image
   to freeze, wait a few seconds for `renderdiag.log`, and then stop/reset
   FS-UAE. Do not promise a return to Workbench.
4. Wait for the user's result. Read the log directly from that exact drawer and
   correlate it with their visual/gameplay observation.
5. Reject or fix visual corruption, flicker, gameplay differences, ownership
   violations or packaging mistakes before creating/promoting the 68020 gate.

Do not infer FS-UAE acceptance from compilation or from log contents alone.
The user's observed result is required.

## Match the checkpoint cadence to the size of the change

### Focus a new level or mode during iteration

While a new level, interlude or game mode is still being built gate by gate,
prefer a compile-guarded user-test executable that starts directly at the
current section. It must bypass story intro, title and earlier completed levels,
use the real production renderer/assets/input for that section, and be clearly
named and documented as a focused candidate. Keep this shortcut out of normal
production and release targets.

Do not force repeated full-campaign playthroughs to review an isolated art,
control, encounter or transition gate. Restore and test the complete campaign
path only after the new section is coherent enough to validate loading,
results, replay/continue, persistence and unloading as one integrated flow.
Record both obligations in the active plan so a later session cannot mistake a
direct-start drawer for finished campaign integration.

Treat human playtesting as an early design input for large experiential changes,
not merely as a final verification step. A new level, game mode, control model,
major art direction, pacing structure, audio experience or campaign flow should
reach the user as soon as one coherent representative slice is playable. Before
staging it, perform the bounded technical smoke checks needed to exclude crashes,
corruption, broken packaging and obviously unreachable gameplay, but do not spend
many internal polish cycles deciding subjective feel on the user's behalf.

The early large-change drawer must still be meaningful: include the defining
controls, representative art, collision or route grammar, pacing and transition
needed to judge the idea. Do not ask the user to evaluate disconnected
placeholders that cannot answer the stated design question. State which parts are
representative and which remain intentionally unfinished.

For small fixes and tuning passes—such as a collision nudge, cooldown adjustment,
palette correction, asset placement fix or isolated visual glitch—self-test first
and bundle related changes. Avoid asking the user to replay a build for every
micro-change. Request another user pass at a meaningful accumulated checkpoint,
or sooner only when the result is inherently subjective, emulator automation
cannot exercise it, or exact hardware behaviour is the question.

Keep the evidence boundaries explicit: automated or self-run FS-UAE checks can
establish technical health and provide screenshots or logs, while the user's pass
establishes control feel, readability, challenge, visual quality and acceptance.
When the user rejects a large slice, record it as rejected and return with a
coherent revised slice rather than a stream of narrowly patched drawers.

## Measure FS-UAE/68020 second

Proceed only after the 68030 visual/function gate passes.

1. Package the corresponding 68020 drawer or A/B pair with identical code,
   assets, scene and instructions apart from the machine configuration.
2. Ask the user to perform the same workload and save each drawer's own
   `renderdiag.log` with the left mouse button; each image should freeze after
   the press and may then be stopped/reset after a short wait.
3. Analyze cadence separately from measured hotpath costs. Report at minimum
   presented intervals, one-/two-/three-field counts, effective FPS, missed
   20 ms deadlines and ownership violations when present in the log.
4. For A/B, compare like-for-like scenes and sample lengths. Use median, p95,
   maximum, calls per frame and total cost when those fields exist.
5. Account for diagnostic observer cost. Do not treat a heavily instrumented
   build's FPS as production cadence; prefer the established low-overhead
   cadence diagnostic for the regression guard.

When processing a log, first confirm its drawer/build and instrumentation mode,
then check sample duration and calls per frame before comparing costs. Separate
total cost from average-call cost, and CPU sections from `WaitBlit`/bus waiting.
Flag truncated, stale, mixed-workload or differently instrumented A/B logs
instead of deriving an FPS verdict from them. Preserve the raw log after
summarizing it.

Protect alpha.45's accepted low-overhead FS-UAE/68020 result of 48.58 FPS with
no three-field misses or ownership violations. Investigate a material decline
before accepting future work; do not quietly normalize a slower baseline.

## Analyze and record evidence

- Apply `analyze-amiga-test-evidence` when logs, screenshots, MOV files or
  hardware observations are supplied. Apply `catalog-sparkpaw-test-evidence`
  to new files in `sparkpaw/testresults` when its trigger conditions apply.
- Separate what the user observed, what the log measures, what source review
  proves and what remains a hypothesis.
- Record 68030 visuals/function, 68020 cadence, HD, ADF, Analogue Pocket and
  real-A1200 results as distinct acceptance boundaries.
- Never claim a platform or launch path accepted without the user's explicit
  result for that exact route.
- After an accepted or rejected candidate, update the active performance plan,
  Stage 2 audit, handoff and development history with measurements and lessons
  learned as appropriate.

## Finish a cycle

State clearly whether the candidate is accepted, rejected or still pending.
Keep an accepted optimization only after the requested gates pass. Return
superseded active drawers to `dist/older-builds`, leave the current release plus
at most the next active test set in the root, and report the exact remaining
drawer names. Do not create a release, commit, tag or push unless the user asks.
If the current release set was already missing before the cycle, report that
separately; never recreate a published SemVer from new bytes merely to refill
`dist`.


## Campaign multidisk coverage

Verify cold-load dependencies PER ADF against compiled references, excluding
only the opposite section's exclusive graphics. Union coverage is insufficient:
collisionLoad reads the Level-1 map even during Stormrail startup. Preserve
shared/carryover files until the loader is explicitly changed and tested.
Read back every file, check bitmaps with the actual C decoder, and test DF0
swap and already-present DF1 independently. Keep graphical approval separate
from full campaign/hardware acceptance. Archive failed pairs byte-identically.
