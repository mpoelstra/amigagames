# Drowned Slice 2 review — 13 September 2026

The supplied 20-29-15 recording shows passage through the opened shutter.
The missing cue is depth: the player covers the complete structure. User wants
the left post behind Sparkpaw and the right post in front of him.
The 20-30-09 recording rejects the jet's raised brown nozzle and bright cyan
column: it should resemble boiling water/a geyser behind a structure.

## Evidence

Original MOV bytes preserved under testresults, with matching TXT sidecars:

- `Unassigned-Drowned-Slice2-sluice-depth-review.mov`
- `Unassigned-Drowned-Slice2-pressure-jet-art-rejected.mov`

Overview and consecutive-frame contact sheets are in
`build/drowned-review-evidence/`. These recordings do not independently establish
CPU model, binary identity, 50 FPS, ADF or real-hardware acceptance.

## Near post correction

The player is an attached AGA hardware sprite, so drawing another foreground
Bob last does not establish the requested overlap. The isolated Drowned renderer
now clips player pixels only against the opaque right-post silhouette at
world x834..851, y112..199. Left post and collision behaviour stay as before.
The silhouette is read once from the actual indexed foreground at preparation.

Only the inactive player stage is edited, after restoring its Fast master.
Both attached channels and all four sprite data planes receive the same mask.
Control words, 64px padding, terminator and source masters are preserved.
Previously clipped stages force a fresh source copy even if the pose is cached,
including after leaving the post or returning from invulnerability blinking.
World coordinates make the mask independent of scrolling and facing direction.

Cost: 352 bytes for the resident silhouette plus two small stage flags, no new
Chip allocation or bitplanes. During overlap a fresh 1600-byte pair copy and
at most 48x3 word-mask iterations are required; there is one restoration copy
per previously clipped stage after departure. No per-frame allocations.
Actual 68020 frame time remains to be measured; this is not a 50 FPS claim.

Native slice and normal campaign builds pass. Full host suite passes, including
the new pixel-reference and actual setHardwareSprite staging test under
ASan/UBSan: both directions, changing scroll, repeated poses, blink/splash,
inactive-stage ownership, leaving the post, immutable masters and terminators.
No emulator was launched. The dist Slice2 drawer has NOT been replaced.

## Geyser concept v2 — pending review

`assets/concept/drowned-geyser-source-v2.png` is a new four-stage concept:
low bubbling, swelling warning, erupting water, falling droplets. A low iron
lip hides the recessed outlet. Blue liquid carries the volume, with smaller
cyan edges and pale foam highlights. This addresses both material and base.
The former jet is animated already; its cyan/white balance makes it read as a
flame despite using the same water pen family. New concept colours are not yet
mapped to the native palette, and the concept sheet is not an animation proof.

Next: review this direction before art integration, then convert to the existing
palette and effect envelope, inspect native clusters and cycle closure. Keep
the ground at y200 and HUD at y208; no decorative strip between them. Steam
must remain sparse and bounded. Stage one coherent revised candidate with
the near-post correction and approved new jet, using the normal HD test stager.
Review walking through the gate in both directions, standing/jumping against
its edge, leaving/re-entering the camera, and life reset. Then 68020 timing/RAM.
Do not advance release version, commit, push or overwrite alpha.8.

## Follow-up — Slice3 staged after concept approval

User accepted the geyser direction and asked to proceed. The native family is
now integrated and staged at `dist/Drowned-Slice3-030-HD/Drowned-Slice`, together
with the near-post correction. Eight 32x64 frames, shared water pens 0/5/6/11;
1796 blue / 609 cyan / 174 pale pixels across the water atlas. The source sheet
uses one uniform resize, with the second row registered +4px as a whole to
match baselines. No per-pose zoom. Fixed 32x6 lip at world x448/y194, under the
water origin x448/y131. The warning alternates low frames every five ticks;
active damage bounds and phase timing are unchanged.

Generators: `tools/prepare_drowned_geyser.py`, `tools/build_drowned_slice_assets.py`.
Native roundtrip/palette/bounds checks pass. `tools/preview_drowned_slice3.py`
produces an offline 4-second GIF from the actual patch atlas; static background,
water-gap sample and HUD are not an emulator/timing proof. New source prompt
is in `assets/concept/drowned-geyser-animation-source-v2.txt`.

Slice/normal native builds and full host suite pass. Stager checked 63 declared
assets and 47 executable references. Package proof and before/after inventory
are under `build/drowned-slice/slice3-*.json`. All 68 alpha.8 release files and
all 65 archived Slice2 files preserved byte-identically. Old drawer now under
`dist/older-builds/20260913-before-drowned-slice3/`. New manual visual/function
acceptance is pending; no emulator run, measured FPS claim or release.
