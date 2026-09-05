---
name: extend-sparkpaw-animations
description: Safely create, review and integrate Sparkpaw game sprite art and animated runtime families while preserving Amiga A1200 palette, scale, grounding, cache, renderer and append-only contracts. Use for player or enemy poses, collectibles, projectiles, water, effects, Blitter Bobs, hardware sprites, sprite sheets, animation slots, frame timing or animation-selection logic.
---

# Extend Sparkpaw Animations

Treat every animation expansion as a fragile concept-to-runtime workflow. Keep
art, runtime conversion and behaviour changes separately reviewable.

## Establish the contract

1. Read `CODEX_HANDOFF.md`, `sparkpaw/README.md`, the relevant sections of
   `docs/DEVELOPMENT_HISTORY.md`, and `sparkpaw/docs/IMAGEGEN_PROMPTS.md`.
2. Inspect the consuming module and the matching generator/cache builder before
   changing art. Record frame IDs, dimensions, palette, transparency, baseline,
   facing layout, masks, memory domain, draw order and trigger priority.
3. Preserve the dual-playfield renderer, line-100 Copper staging, line-252 HUD
   switch, synchronized line-253 restore/update/draw pass, packed planar caches,
   camera culling and 2 MB Chip + 8 MB Fast target.
4. Never introduce CPU read-modify-write compositing in displayed Chip RAM.

## Select the runtime family

- **Player hardware sprites:** preserve six attached channels, 48x48 cells,
  perceived 32x40 scale, common boot baseline, exact mirrors and append-only
  IDs. At Phase 6B.5, 0..49 are baseline, 50..53 standing hurt, 54..57 crouched
  hurt and 58..61 ledge teeter.
- **Enemies:** preserve each type's dimensions, palette/material identity,
  collision cell, source-facing convention and `EnemyBobCache`. Never renumber
  accepted Strider slots 0..27 or consume another generic four-pool slot.
- **Collectibles/projectiles/effects:** preserve mask dimensions, Bob restore
  ownership, palette identity and relative restore/draw order. Do not conflate
  concept duplicates with new runtime sizes or objects.
- **Water/background animation:** use deterministic prebuilt planar frames and
  synchronized Blitter copies to clean and displayed buffers. Preserve bank
  joins, loop closure, update cadence and Bob ordering; avoid runtime randomness
  and full foreground duplication.

## Author concept-first

Use the imagegen skill for genuinely new raster art. Provide the authoritative
identity sheet and closest accepted family as separate references. Request one
coherent family on a flat removable chroma background. Explicitly forbid
redesign, per-frame zoom, palette drift, changed material balance, inconsistent
lighting, blur, labels, props and cast shadows unless the requested effect
requires them.

Save versioned chroma and transparent review sources under the matching
`sparkpaw/assets/` subfolder. Append the exact prompt and accept/reject status
to `sparkpaw/docs/IMAGEGEN_PROMPTS.md`. Do not integrate before user approval
when visual review was requested.

Procedural assets such as water or plasma must have an exact native-palette
preview generated from the same formula as runtime code.

## Prove native art and cache

1. Crop the approved source as one family and use one scale for all character
   frames; never resize cells independently.
   Audit every later fit or clamp too: a shared family-scale variable is not
   sufficient if an oversized frame subsequently triggers a per-frame fallback.
   Measure the final indexed bounds of every frame and flag unexpected outliers.
   Separate embedded muzzle flashes, trails and projectiles from actor bounds
   before scaling when runtime already renders that effect independently; do
   not shrink the complete character merely to retain a redundant effect.
2. Preserve pen-0 transparency and remap to the established target palette.
   A shared palette bank alone does not prove character/material consistency.
   For Sparkpaw, compare palette-role distribution as well as available RGB
   entries: fur base, fur shadow, muzzle/inner-ear cream, eye colour, scarf,
   gauntlet and outline must remain consistent with the accepted gameplay and
   HUD identity. A new pose may not swap highlight and shadow roles merely
   because both colours exist in the destination bank. Preserve shape and mask
   while correcting palette roles; never paste a differently scaled HUD icon
   into gameplay art as a shortcut.
3. Append frame IDs unless the user explicitly replaces rejected, unaccepted
   work. Update source count, headers, generator manifest and every cache loop
   together.
4. Use existing exact mirror/cache paths. Never add runtime CPU flipping or
   displayed-buffer compositing.
5. Inspect a nearest-neighbour enlargement of exact indexed runtime frames.
   Check anatomy/material identity, mass, grounding, clipping, palette balance,
   masks, facing continuity and first/last-frame loop closure.
   When replacing frames inside an accepted sheet, binary-compare the resulting
   SPBM with that baseline and require that only the intended logical slots and
   their deterministic mirrors differ.
6. Measure source/runtime/Chip growth and reject silent frame-count or package
   drift.

## Integrate behaviour separately

Give new states narrow deterministic predicates. Preserve existing state
priority and interruption rules. For Bobs, retain restore-before-background-
update-before-draw synchronization and relative projectile/enemy/collectible
order. Keep collision and physics unchanged unless runtime evidence proves a
specific dependency; then document the cause and test adjacent walls, residual
overlap, landing, reset, off-camera and immediate follow-up actions.

## Validate and release

Run `make PYTHON=../.venv/bin/python3` and
`make release PYTHON=../.venv/bin/python3` from `sparkpaw/`. Synchronize SemVer,
roadmap checkpoint, README, handoff, development history, packaged notes and the
sole four current files in `sparkpaw/dist`. Do not claim FS-UAE or real-hardware
verification without supplied evidence.
