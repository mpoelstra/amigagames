# Level-1 performance evidence re-audit — 2026-09-05

Current status: [CURRENT_STATUS.md](CURRENT_STATUS.md). Performance research
is explicitly parked by user decision. The Bob/column/history changes are
retained in production and the played e6e20db6... build. 030 passed; the user
reported no noteworthy 020 change and authorized retention. No FPS gain or
explanation of the tiny alpha.68 difference is claimed. No test is pending.

The audit below preserves original comparisons and offline probes. Its
“played baseline” cf861a, Bob-only b0eb6ca and then-offline probes are historical
stages; the current production and drawer SHA is recorded in the status index.
Original source evidence, measurements and rejection boundaries remain valid.

## Corrections to earlier attribution

- The 48.58 FPS / 1,137 intervals / 33 two-field reference originates in
  `PERFORMANCE_68020_STAGE2_AUDIT.md` on the **alpha.45** baseline. Calling it an
  exact alpha.68 measurement was incorrect. Alpha.68 remains the user's
  accepted gameplay/feel reference, with no successful directly matched
  alpha.68 diagnostic run in this investigation.
- The 49.30, 46.66 and 47.71 headers are measured outcomes on different manual
  playthroughs. They support conservative candidate rejection, but cannot by
  themselves establish cache aliasing or separate the effects of two edits.
  The game-update extraction and completion-cache edit were tested together.
- A charging failure is not proof of failed Fast-RAM allocation. The smaller
  trace also failed, and no failure-stage/allocator evidence identified the
  cause. Both historical diagnostic builds remain invalid.
- The preserved `Phase 6D-pass-level1-collision-cache-020-renderdiag.log`
  lacks `post_run`, retaining 1,004 trace rows rather than the expected 1,024.
  Its cadence header is present, but final trace/memory-flush completeness is
  unverified. Do not label that file a fully completed diagnostic flush.

## Confirmed instrumentation difference

The minimal mode disables nested CIA scopes and family timers. It still clears
210 bytes of trace state plus 22 bytes of object counters per frame, snapshots
world/HUD pointers and enemies, copies trace records and samples phase times.
In the split build six diagnostic hooks additionally call through
`renderer_dispatch.c`, unlike direct hooks in the old single-renderer build.
Generated 68020 assembly confirms these are ordinary wrappers, including
argument repacking, nested `jsr` and `rts`; they are not optimized tail calls.

This extra work exists only in diagnostic builds. Its FPS effect is not yet
measured. The previously corrected gameplay dispatch does not remove it.
Changing game code based solely on these diagnostic comparisons risks tuning
around instrumentation overhead. Comparing diagnostic and production behaviour
is therefore a higher-priority next experiment than another object reorder.

## Reproducible address evidence

`tools/audit_link_layout.py` captures the chosen target's actual Makefile
compiler command, compiles objects and assembly in a separate directory, and
links with the current aos68k configuration plus a map. It fails unless the
rebuilt executable is byte-identical to the existing target. The script never
stages or launches the game; use a fresh output directory for each invocation.

Example from repository root:

```
.venv/bin/python3 sparkpaw/tools/audit_link_layout.py \
  build/sparkpaw-campaign-level1-physical-split \
  --output sparkpaw/build/layout-audit-collision-cache-verified
```

Verified SHA256:
`6dc44147b09155042e4f9fbd85366213b907c036c2ca351f1446d821fc848b2e`.

Selected CODE-hunk-relative offsets (not absolute runtime addresses):

| Symbol | Offset |
| --- | ---: |
| main | 0x0bf4 |
| rendererLevel1UpdateGameplay | 0x17684 |
| rendererLevel1DrawGameplayBobs | 0x17780 |
| rendererStormrailUpdateGameplay | 0x22e8c |
| rendererStormrailDrawGameplayBobs | 0x22fd8 |
| rendererDiagnosticUpdateEntry | 0x275ec |
| rendererDiagnosticBoundary | 0x276f0 |

These establish placement only. Proving a cache mechanism also requires actual
execution/access patterns and relevant runtime placement. Neither source-list
resemblance nor a map alone proves that mechanism.

## Earlier decision gate (superseded by logger-free result)

Keep the current production hot paths and assets stable. First quantify or
remove the split-specific observer difference in a separately named diagnostic
experiment, verify the generated call sites, and keep its production output
byte-identical. Do not combine that experiment with function extraction,
completion caching, padding or object reordering. A changed test harness also
requires a newly labelled baseline; its FPS cannot be treated as an automatic
production optimization. User acceptance of whole-level feel remains open.

## Prepared observer experiment (offline only)

`SPARKPAW_DIRECT_DIAGNOSTIC_CLIENT` in `renderer.h` now offers direct diagnostic
calls under the existing section selector. It is enabled only by explicit
experimental flags and does not affect the retained target. It handles both
renderer owners, so selecting Stormrail never invokes the Level-1 trace hooks.

Build with the existing target recipe and an alternate output variable:

```
make CAMPAIGN_LEVEL1_PHYSICAL_SPLIT_TARGET=build/sparkpaw-direct-trace-experiment \
  CFLAGS='+aos68k -O2 -cpu=68020 -notmpfile -I$(NDK_INCLUDE) -DSPARKPAW_DIRECT_DIAGNOSTIC_CLIENT' \
  build/sparkpaw-direct-trace-experiment
```

Verified offline results:

- Experimental executable SHA256:
  `77a260a4bf474dcca9fc37ce592f6720afd9e65e1d2bc2de5b4b9837e8d0ec5d`.
- An independently linked map reproduces that hash exactly.
- All six calls from main to the diagnostic wrappers are absent from the
  experimental assembly; both section-specific call destinations remain.
- Only the main object changes. Both renderer objects, their measurement code
  and all other compiled objects remain byte-identical to the retained build.
- Rebuilding the normal target still produces the original `6dc441…` hash.

No native test, performance acceptance or dist staging has been performed.
The executable layout necessarily changes with main's calls, so this does not
isolate CPU cycles from code placement. It does bound the change to observer
dispatch and must not be described as a demonstrated gameplay speedup.
