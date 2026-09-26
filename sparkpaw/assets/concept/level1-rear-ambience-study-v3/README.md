# Level1 electrical ambience v3 — visual review

The user rejected v2 as almost invisible and explicitly requested a glowing
existing bolt, a visible electrical pulse travelling downward into the crystal,
and general electrical discharges throughout the level's purple sky.
V1 and V2 remain intact with generator snapshots.

This study uses the existing eight rear indices and raster palette. A narrow
glow follows the existing bolt while a brighter packet travels downward every
2.88 seconds; the crystal flashes at arrival and settles back to cyan. Eight
local cloud discharges span the 1120-pixel rear panorama, with offset timings
and alternating shapes. Full cycle: 48 frames at 120 ms = 5.76 seconds.
These are review timings, not an accepted native update schedule.

- `sequence-3x.gif`: opening camera, visible downward bolt pulse and crystal.
- `tower-detail-8x.gif`: the same animation enlarged.
- `four-cameras.gif`: cameras 0, 1024, 2048 and 3072 in reading order.
- `sky-panorama.gif`: complete rear panorama at native resolution.
- `storyboard.png`: labelled phases of the downward sequence.
- `frame-*-indices.png`: exact rear pen indices, stored as greyscale numbers
  0..7 for reproducibility, not intended as a colour preview.

All changes are restricted to rear rows y=0..89. The source
SPBM, foreground, runtime code and dist remain untouched. See
`verification.json` for exact GIF/PNG parity, source identity and tower-frame
count. The tower fits a 48x64x3 rectangle: 1,152 bytes/state, a conservative
1,152-byte Chip stage and 2,304 destination bytes/six plane blits for both rear
copies per upload. The 15 unique tower states total 17,280 raw bytes; eight
64x24 cloud rectangles each have nine unique states, totalling 41,472 bytes.
Combined complete-patch storage would be 58,752 raw Fast bytes before metadata,
bank duplication or compression. Package growth and native costs still need
a runtime asset layout. Multiple simultaneous visual effects in this GIF
do not prove they can be uploaded in the available PAL/68020 beam window.

Reproduce with `../.venv/bin/python3 tools/preview_level1_rear_ambience.py`
from `sparkpaw`. This is an unaccepted host visual study, not a playable build.
