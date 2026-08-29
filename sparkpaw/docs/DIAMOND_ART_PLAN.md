# Shared native diamond art plan

Status: shared-master implementation accepted in supplied FS-UAE/68030 HD
review and promoted as alpha.63. FS-UAE/68020, ADF, WHDLoad and real-hardware
runtime acceptance remain pending.

## Decision

Stop independently drawing a HUD diamond and a world diamond. Their current
pipelines make exact agreement impossible: the HUD emblem originates in a
LANCZOS-reduced full-HUD concept, while the world collectible is authored at
native 16x21 indexed resolution.

Define one authoritative semantic 16x21 pixel master with these roles:

- transparent/background;
- closed dark contour;
- cream highlight;
- cyan light plane;
- darker lower facet.

Generate the world Bob and its mask directly from that master without crop,
resize, filtering or colour quantization. After the full HUD source has been
reduced, erase its old diamond and stamp the same master geometry at the fixed
HUD position. Translate roles to the fixed FRONT16 and HUD8 palettes; do not
pretend the two palette banks contain identical RGB colours.

The HUD currently has no dedicated equivalent of FRONT16 blue pen 5. Select
its existing dark navy/grey role for the lower facet unless a complete HUD
palette audit proves one pen can safely change. Do not repurpose a HUD pen on
visual preference alone because the frame, portrait, hearts and counters share
that three-plane palette.

The implemented candidate maps that role to FRONT16 pen 5 and existing HUD8
pen 3. Contour, cream and cyan map to FRONT16 1/4/6 and HUD8 2/4/7. No palette
entry changes, filtering or runtime work are introduced.

## Required proof

Add a host regression that decodes both generated assets and requires:

- exact 16x21 logical geometry;
- identical occupied mask;
- identical semantic facet role at every occupied pixel;
- pen 0 only outside the world silhouette;
- no resizing/filtering step between the master and either stamped result.

Implemented as `tests/test_shared_diamond_master.py`; it decodes the real
generated SPBMs rather than trusting preview files and currently passes.

The runtime contracts remain fixed: 32-pixel padded source row, 16x21 visible
Bob, existing mask/cache, 48 positions, hover, collision, counter/life award,
target-local composition and restore/draw order.

## Visual gate

Review the shared result first in FS-UAE/68030 HD at native gameplay scale and
inside the complete HUD. Judge it as one design expressed through two fixed
palette banks, not as two independently authored pictures. Only after that
visual/function acceptance consider the ordinary evidence-specific 68020,
ADF or hardware gates. Do not advance SemVer or release artifacts beforehand.

## Collaboration rule captured by this iteration

When successive visual revisions circle around a comparison target, pause and
inspect source ownership, transformations and palette constraints before
making another variant. Codex should explicitly challenge a requested local
edit when a shared master, invariant or pipeline correction would solve the
underlying mismatch more reliably. User feedback remains authoritative about
the observed result, but it does not require preserving a flawed production
method.
