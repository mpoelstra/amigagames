# Sparkpaw C architecture review

> Current status (2026-09-05): This is architectural rationale, not a request for a broad refactor. The existing section enum/decision contract and physically separated renderer units must be recognized. See CURRENT_STATUS.md for the bounded next proposal. See [status index](CURRENT_STATUS.md).

> Superseded as the active work plan by
> `C_ARCHITECTURE_AND_PERFORMANCE_PLAN.md`. This review remains the preserved
> architectural baseline and rationale.

Status: review baseline for the integrated campaign candidate; no broad
refactor is part of this candidate.

## Verdict

The current program remains technically viable for the accepted Level 1 plus
Stormrail build. Static allocation, explicit Chip/Fast ownership, bounded pools
and direct hot paths suit the 68020 target. File length alone is not a runtime
problem, and splitting hot Blitter/Copper code merely for aesthetics could make
generated code or timing worse.

The architecture should nevertheless be evolved before several more levels or
a multi-ADF build are added. The main scaling risks are implicit lifecycle and
section selection, not C itself:

1. `main.c` combines application flow, loading, results, replay, diagnostics
   and section transitions behind many compile-time branches.
2. `GameState` contains Level-1 and Stormrail state together, while `game.c`
   selects behaviour through `stormrailActive`. A third world would multiply
   conditionals and make resets harder to prove.
3. `assets.c` uses a Boolean world selector. Future levels and media volumes
   need an explicit section descriptor and manifest, not additional Booleans.
4. `renderer.c` owns shared low-level primitives, both world renderers,
   conversion caches, Copper construction, diagnostics and lifecycle. Its cold
   ownership paths are difficult to audit; the stale proof-pointer bug was a
   concrete example.
5. Several modules consult global state across boundaries (`player` asks
   `game` which section is active, while `game` also owns player orchestration).
   This makes initialization order and host testing less explicit.
6. Some integration tests assert source text. They are useful wiring guards,
   but behavioural contracts should increasingly live in pure host-testable C
   functions and typed descriptors.

## Target boundaries

Keep three distinct state lifetimes:

- `CampaignState`: values intentionally surviving a section boundary, such as
  carried lives, half-heart health and the live diamond meter.
- `SectionState`: a tagged Level-1 or Stormrail state, initialized and reset by
  that section alone. Score, elapsed time, enemies, pickups and awards belong
  here and start fresh for every section/replay.
- `RuntimeState`: renderer, audio, input and loaded-asset ownership, with an
  idempotent prepare/release contract and explicit active section id.

A future `SectionDescriptor` should provide section id, media/asset manifest,
init/update/render hooks, results extraction and replay policy. The campaign
flow consumes this descriptor; it must not know individual asset filenames.
This descriptor is also the natural boundary above which a future ADF-volume
loader can request and validate a disk.

## Safe incremental sequence

1. Freeze the accepted campaign candidate and retain cadence and visual
   baselines. Do not refactor during gameplay/content acceptance.
2. Extract the campaign/application state machine from `main.c` as pure
   transition decisions with host tests. Keep Amiga takeover calls at the thin
   outer boundary.
3. Replace the Boolean asset selector with `enum CampaignSection` plus a
   read-only asset manifest/descriptor. Preserve current filenames and load
   order byte-for-byte.
4. Split section state and update orchestration: Level 1 and Stormrail each get
   their own reset/update/result adapter. Keep shared player vitals explicit.
5. Split only cold renderer ownership first: allocation/conversion,
   prepare/reset/release, Level-1 composition and Stormrail composition. Keep
   measured hot Blitter primitives, Copper patching and inner loops together
   until before/after 68020 disassembly and cadence prove a move harmless.
6. Make every cleanup idempotent and test pointer/size pairs as ownership
   units. Then add failure-injection tests for partial prepares.
7. Add a media-volume field to the section asset manifest only when multi-ADF
   packaging is actually implemented. Disk UI and swapping stay above gameplay
   and rendering.

Each step should compile to an unnumbered candidate, pass host contracts and be
compared with the accepted 68020 cadence/visual baseline before the next step.
No pool growth, per-frame allocation, runtime scaling/rotation or renderer
ownership change is implied by this plan.
