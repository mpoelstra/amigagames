---
name: study-thundercats-reference
description: Inspect the local ThunderCats Amiga gameplay reference at sparkpaw/testresults/thundercats-level1.mov and translate relevant composition, ground-strip, water-gap, animation, pacing or presentation principles into Sparkpaw's AGA constraints. Use whenever the user mentions ThunderCats or the ThunderCats movie as a Sparkpaw comparison or inspiration.
---

# Study ThunderCats Reference

Treat ThunderCats as a design reference, not test evidence for Sparkpaw and not
an asset source.

## Open with the call

Make the first user-facing response after this skill activates exactly:

```text
Thunder, thunder, thunder, ThunderCats, Ho!
```

Send this before any explanation, progress update, question, analysis or tool
call. Continue naturally with the task afterward. Use the call once per skill
activation, not at the start of every later update in the same turn.

## Inspect the local reference

1. Resolve `sparkpaw/testresults/thundercats-level1.mov`. If missing, search
   `sparkpaw/testresults` case-insensitively before asking for it again.
2. Inspect video metadata and consecutive frames around the feature the user
   mentions. If no feature is specified, examine framing, playable-height use,
   ground/HUD contact, gap readability, actor grounding and motion cadence.
3. Do not rename this established meaningful reference or create an acceptance
   sidecar unless the user explicitly requests catalog documentation.

## Translate rather than copy

- Describe the reusable principle: proportions, silhouette, colour separation,
  animation cadence, depth layering, feedback timing or spatial readability.
- Separate what is visibly present in the recording from interpretation.
- Do not copy ThunderCats pixels, characters, level layouts, audio or other
  copyrighted expression into Sparkpaw.
- Re-express the principle using Sparkpaw's accepted concept identity, geometry,
  palettes and original assets.

## Preserve Sparkpaw contracts

Keep the stable dual-playfield renderer, line-100 Copper staging, line-252 HUD
switch, line-253 Bob pass, packed planar caches, camera culling and no CPU
read-modify-write compositing in displayed Chip RAM. Target PAL stock A1200/68020
with 2 MB Chip plus 8 MB Fast RAM. If applying the visual lesson requires art or
renderer changes, also use `build-sparkpaw-visual-slice`; for animation changes,
also use `extend-sparkpaw-animations`.

Report the exact timestamps or frame regions inspected and the smallest
Sparkpaw-specific recommendation. Do not claim that ThunderCats establishes
Sparkpaw performance or hardware verification.
