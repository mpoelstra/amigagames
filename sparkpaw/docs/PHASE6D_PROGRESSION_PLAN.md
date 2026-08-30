# Phase 6D multi-level progression boundary

Status: next engine/design boundary after the accepted alpha.68 Phase 6C.10
checkpoint. Level-2 concept, art and route planning are intentionally owned by
a separate session and are not specified here.

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
