# Level1 ambience v2 — quieter existing-art study

V1 was rejected on 26 September: too strong and insufficiently faithful to
the existing lightning/art. It remains intact beside this folder with its
generator snapshot. V2 is a visual proposal awaiting review, not an accepted
runtime candidate.

The original bolt's silhouette and width remain intact. Three, then nine
authored pixels inside its existing light core briefly move from pen7 to pen6,
then return. One pixel on the crystal's existing lower cyan facet briefly
dims; the crystal never expands. There is no added glow, altered vortex or
scattered highlight generation. Original artwork occupies 6.2 seconds of the
7.08-second review loop. Clouds remain static in this iteration so the tower
treatment can be judged; subtle ambience over the full level remains planned.

`sequence-4x.gif` is the full opening scene; `tower-detail-8x.gif` enlarges the
same pixels and timing. The small effect is intentionally easier to see in the
detail. The PNG storyboard follows idle, soften, low, recover, crystal, rest.
These are host previews using the existing indexed palette, no native proof.

All changing pixels fit one aligned rear rectangle (208,35,16,50): 300 bytes
per three-plane frame, 1,200 bytes for four unique complete states. A
conservative stage path would need 300 Chip bytes and six plane blits copying
600 destination bytes into canonical plus guarded rear per change. This is a
payload calculation, not a measured timing or complete allocation budget.
Beam scheduling, loading, actual memory and PAL/68020 cadence are still open.

Reproduce with `../.venv/bin/python3 tools/preview_level1_rear_ambience.py`
from `sparkpaw`. No runtime sources, production assets or dist files change.
