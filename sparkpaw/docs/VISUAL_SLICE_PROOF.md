# Phase 6B.6 visual-slice proof

Generated deterministically from `sparkpaw-visual-slice-concept-v2.png`.
The same 320x208 scene, crop, PF1 mask and eight-colour PF2 reduction are used
for both images. The right side changes PF1 from 16 to 32 colours as an art-only
upper bound. It is **not** an implementable AGA 5+3 dual-playfield mode: AGA
alternates odd/even bitplanes and permits at most four planes per playfield.

## Host image comparison

- Native scene: 320x208; foreground mask: 5804 pixels
- 4+3 combined RGB RMS error: 18.89
- hypothetical 32-colour PF1 combined RGB RMS error: 18.32
- 4-plane PF1 masked RMS error: 21.44
- hypothetical 32-colour PF1 masked RMS error: 14.71
- Preview: `assets/levels/storm-visual-slice-aga-comparison.png`
- Matched sixteen-frame water proof:
  `assets/levels/storm-water-concept-aga-comparison.png`

These host colour errors measure palette fidelity only. They do not establish
Amiga frame time, Copper safety, Blitter capacity or subjective visual value.

## Exact extra-plane storage

The rejected hypothetical fifth PF1 plane would add one bit per foreground
pixel if the hardware mode existed:

- one 320x208 slice bitmap: 8,320 bytes
- one 2048x208 resident bitmap: 53,248 bytes
- one future 3072x208 resident bitmap: 79,872 bytes

Production keeps both a clean source and a displayed foreground representation,
so the 2048px minimum direct increase is 106,496 bytes across those
two full-width representations before Bob caches, water/effect caches, asset
headers or alignment. At 3072px it becomes 159,744 bytes. Exact
Chip/Fast placement must be measured in the Amiga bench rather than inferred.

## Copper-banded REAR8 production candidate

AGA's eighth bitplane is PF2's fourth plane, producing 4+4 rather than 5+3.
Therefore do not build or claim a 5+3 benchmark. Production 4+3 remains
authoritative. Alpha.17 uses twelve horizontal-blank Copper palette steps to
morph the existing three-plane rear through dedicated sky, mountain and forest
ranges. This changes no bitmap fetch or Blitter workload and adds 512 Chip
bytes to the Copper reservation. Host previews are complete; FS-UAE and real-
hardware timing/visual proof remain required. A future 4+4 comparison may measure rear-playfield colour gains,
but it cannot provide extra foreground/water colours. Pursue concept-style
water first inside FRONT16 using the matched four-colour proof.

## Extended REAR8 parallax

The first production candidate reduced a 640px master and tiled it across the
2048px rear bitmap. Supplied FS-UAE scrolling exposed its non-seamless repeated
landmarks. The replacement master reduces to a 1024x208 native strip with eight
indices per scanline, Copper-banded palettes and unique compositional progression.

- current 2048px world: maximum camera X 1728; rear pixels through 751 visible;
- planned 3072px world: maximum camera X 2752; rear pixels through 1007 visible;
- first padded repeat: rear pixel 1024;
- runtime `storm-rear.spbm` dimensions, depth and memory remain unchanged.

The generated REAR16 comparison uses the same source. Its RMS colour error is
19.99 versus 20.58 for REAR8. The visible benefit is additional cyan, green and
intermediate material separation; it remains an unintegrated 4+4 candidate
requiring the isolated Amiga renderer benchmark.
