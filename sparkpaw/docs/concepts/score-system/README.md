# Sparkpaw score-system concept package

Status: research and concept art only. Nothing in this directory is a runtime
asset or an accepted implementation contract. It does not modify the active
renderer, HUD, performance diagnostics, build, package, version or release.

Contents:

- `SCORE_SYSTEM_PLAN.md` — feature choices, scoring/time semantics, HUD options,
  staged decision gates and the performance/memory feasibility boundary;
- `sparkpaw-level-complete-score-concept-v1.png` — rich composition concept for
  the stable final keyframe of the level-complete screen;
- `sparkpaw-level-complete-score-concept-v2-minimal.png` — calmer character
  alternative with one shared scoreboard frame and strict tabular columns;
- `sparkpaw-level-complete-score-concept-v3-title-emblem.png` — symmetrical
  alternative using Sparkpaw's project-owned title identity;
- `sparkpaw-level-complete-score-concept-v4-title-character.png` — combined
  title-emblem and character direction with a balanced Stormstone counterweight;
- `ANIMATION_STORYBOARD.md` — proposed results-counting sequence and timing;
- `IMAGEGEN_PROMPT.md` — exact prompt and reference roles used for concept v1;
- `IMAGEGEN_ALTERNATIVE_PROMPTS.md` — exact prompts for concepts v2 through v4.

## Concept-v1 review intent

Judge the information hierarchy and celebratory character, not production
feasibility or exact pixels. The concept is deliberately richer and larger than
the eventual PAL 320x256 screen. If the direction is accepted, a later isolated
art checkpoint should reduce it to an exact indexed AGA preview and measure its
memory/display requirements without entering the active performance branch.

The shared hierarchy is:

1. `LEVEL COMPLETE` confirmation;
2. enemy, diamond and time result rows;
3. one dominant total score;
4. Sparkpaw victory pose and Stormstone energy as original game identity.

The storyboard also defines a three-level original tally-sound family: one
short tick for every visible score transfer, a higher row-lock confirmation and
a fuller Stormstone resolve when the final total settles. These are design
notes only; no audio sample or Paula integration exists yet.

The ThunderCats recording was used only to study the principle of transferring
bonus values into a total. No reference pixels, logo, typography, characters or
layout are used by the Sparkpaw concept.

## Alternative comparison

### V1 — rich Stormstone celebration

Strong atmosphere and character, but rejected as the preferred layout because
the large ring, scenery, character and independently sized header compete with
the numbers. Retain it as an upper-bound mood study.

### V2 — minimal Sparkpaw

Keeps a small victory pose while reducing the backdrop to broad quiet shapes.
Header, rows and total live inside one frame. This is the preferred character
direction. A later exact-grid redraw must use fixed-width digits and explicit
column coordinates; ImageGen alignment is illustrative rather than pixel-exact.

### V3 — title emblem

Uses the project-owned `SPARKPAW` wordmark, winged Stormstone medallion and
subtitle-plaque language as a compact top identity. The score table is centred
below it and Sparkpaw is not repeated as a character. This is the preferred
symmetrical/title-integrated direction.

### V4 — title emblem plus Sparkpaw

Combines v3's centred project-owned title identity and clean table with v2's
small full-body victory pose. Sparkpaw stands on a restrained left pedestal; a
smaller Stormstone pedestal balances the right side without competing with the
score. The central title, subtitle plaque and score table remain independently
centred. This is the preferred combined presentation candidate.

For both v2 and v3, production layout should define one right edge for every
numeric value and use fixed-width digits. Multiplication expressions should be
split into three subcolumns—count, `x`, value—so `7 x 200`, `42 x 25` and
`36 x 100` align structurally rather than merely sharing their last digit.
