# Level1 rear ambience — visual study v1

This is a non-runtime, native-indexed study generated from the current
`storm-rear.spbm` and `storm-front.spbm`. Run
`../.venv/bin/python3 tools/preview_level1_rear_ambience.py` from `sparkpaw`
to reproduce it. No production asset, palette, Copper list or executable is
changed. The 320x208 panels use camera 0 and approximate the existing PF2
raster palette from `renderer.c`; FS-UAE/real-hardware display is unverified.

- `sequence-4x.gif`: idle, sparse cloud highlight, charge, discharge,
  crystal afterglow and original rest image. Durations are review pacing,
  not a runtime scheduling contract.
- `storyboard-2x.png`: the same six frames in reading order, left to right.
- `idle-panorama-2x.png` and `clouds-panorama-2x.png`: the full 1120-pixel
  rear world to inspect coverage beyond the opening camera.
- `*-native.png`: individual 320x208 frames with foreground occlusion.

The current lightning is the idle drawing. The charge and discharge modify
only indexed pixels over it. The crystal reaction changes a few nearby rear
indices; its cyan comes from the existing raster palette. The study intentionally
does not create a new bolt, add colours or animate an entire palette band.

## Provisional bounds

The tower effect should be **one combined rectangle** so the lightning update
cannot overwrite a separately updated crystal. A word-aligned 48x70x3-plane
rectangle at rear (192,18) contains both study regions and costs 1,260 bytes
per complete phase. Four authored states would require 5,040 raw Fast bytes,
before metadata or compression. One 1,260-byte Chip stage and 2,520 destination
bytes to update both existing rear representations are the conservative DMA
path; up to six plane blits plus waits per update. This is arithmetic, not a
measured bus or CPU duration.

Eight cloud sites occupy at most word-aligned 64x28x3-plane rectangles (672
bytes each). Two complete states per site would be at most 10,752 raw Fast
bytes; the largest Chip stage remains the 1,260-byte tower stage. Only one
site or tower update should occur after a publication. If the native budget
fails, reduce site count or remain static.

The tower patch begins at rear row 18, much earlier than Drowned's animated
water row 112. A Drowned-style post-publication update therefore has a narrow
and unmeasured beam window. Before runtime integration, prove an actual safe
schedule on PAL/68020, both rear-copy lifetimes, reverse-scroll exposure,
reset/replay, and no HUD or gameplay cadence regression. No native acceptance
or frame-rate claim follows from these PNG/GIF previews.
