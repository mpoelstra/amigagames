---
name: build-sparkpaw-visual-slice
description: Design, implement and measure one representative concept-quality Sparkpaw AGA foreground and parallax slice while preserving gameplay geometry, palette ownership, renderer timing and memory budgets. Use for visual-slice work, environment-art upgrades, foreground or rear-playfield redesigns, AGA palette experiments, parallax polish, or measured 4+3 versus 5+3 dual-playfield renderer proofs.
---

# Build Sparkpaw Visual Slice

Treat the slice as an isolated art-and-renderer experiment, not permission to
repaint the level or mix in unrelated gameplay.

## Establish the baseline

1. Read `CODEX_HANDOFF.md`, `sparkpaw/README.md`, the current roadmap and the
   relevant renderer and image-generation history before changing anything.
2. Check status, recent commits and tags. Preserve ignored backups and evidence.
   Use `sparkpaw-pre-modularisation` as the accepted four-beetle baseline where
   comparison is relevant.
3. Record the current playfield split, palette banks, bitplane allocation,
   scroll factors, asset storage, Chip/Fast usage and frame timing.
4. Preserve line-100 Copper staging, the line-252 HUD switch, the synchronized
   line-253 restore/update/draw pass, packed planar caches, camera culling and
   restore-before-draw ordering. Never add CPU read-modify-write compositing to
   displayed Chip RAM.

## Bound the experiment

- Select one representative screen-width region containing foreground, rear
  parallax, a platform or ground strip and the water treatment where useful.
- Keep collision surfaces, actor baselines and playable height unchanged unless
  the user explicitly approves a separate geometry change.
- Use the accepted concept art as the direction for colour, material depth,
  silhouette and lighting. Do not infer multiple runtime objects from concept
  duplicates or decorative scale variation.
- Use the imagegen skill for genuinely new raster concepts. Log prompts and
  accept/reject status in `sparkpaw/docs/IMAGEGEN_PROMPTS.md`; do not integrate
  review art before approval.

## Prove the AGA design

1. Convert approved art to exact native indexed assets and inspect a nearest-
   neighbour enlargement. Check palette identity, banding, seams, tile joins,
   scroll joins and HUD contact at 320x256.
2. Keep reusable or packed source data in Fast RAM and only DMA-visible working
   data in Chip RAM. Measure all increases; target stock PAL A1200/68020 with
   2 MB Chip plus 8 MB Fast RAM.
3. Compare the accepted 4+3 dual-playfield layout with any 5+3 candidate using
   the same scene, workload and measurement method. Report palette benefit,
   Chip cost, conversion/copy cost, Copper cost and worst observed frame time.
4. Treat 5+3 as an experiment until measured evidence and visual review justify
   it. Keep the accepted 4+3 path recoverable and do not silently change global
   renderer contracts.
5. Animate water or environmental effects with deterministic prebuilt planar
   frames and synchronized Blitter copies, including both clean and displayed
   buffers. Prove loop closure and bank joins.

## Integrate in reviewable steps

Separate concept approval, asset conversion, renderer proof and production
integration. Do not combine renderer work with enemy, player, collision, audio
or unrelated level changes. For animation families, also apply
`extend-sparkpaw-animations`.

After implementation, run `make PYTHON=../.venv/bin/python3` and
`make release PYTHON=../.venv/bin/python3` from `sparkpaw/`. Synchronize the
checkpoint documentation and sole current four-file artifact set. State only
host-build facts until the user supplies FS-UAE or real-hardware evidence.
