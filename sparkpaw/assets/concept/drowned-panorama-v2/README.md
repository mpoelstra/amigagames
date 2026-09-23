# Continuous rear panorama study — 2026-09-22

User reported a hard vertical seam before the first governor while testing the flowers/diamonds build. `build_drowned_full.py` repeats the 1120px authored background to fill 1520px, creating a visible end/start join. This is asset composition, not an FPS or sprite issue.

Studies 1 and 2: rejected for changing the existing architecture scale/composition instead of providing a faithful extension. Kept for provenance, not runtime inputs.

End reference: last320x208 native pixels enlarged3x. New generation uses this to extend the right end while preserving the original composition as much as possible. Target1552x208 includes32px beyond the visible end for conservative fetch coverage. Same8pens,3planes,quarter scroll. New art requires visual review before integration.

The current dist drawer remains untouched while the user tests. No release, renderer or gameplay changes.

User approved ingame trial on22September2026. Full builder now consumes the authored SPBM directly; native visual acceptance pending.
