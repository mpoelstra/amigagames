# Next Sparkpaw session prompt

```text
We are continuing the Amiga game workspace in this repository.

First read CODEX_HANDOFF.md completely, then read sparkpaw/README.md and inspect
the current Sparkpaw source before changing anything. Check git status, recent
commits and tags first. Sparkpaw is the active AGA A1200 project.

Preserve the stable dual-playfield renderer, line-100 Copper-list staging,
50-frame player animation contract, six-channel hardware-sprite player and the
synchronized Blitter Bob pipelines used by plasma projectiles and the four
clockwork beetles. Preserve packed planar caches, camera culling, Bob
restore/draw ordering and accepted animation/collision contracts. Do not
reintroduce CPU read-modify-write compositing in displayed Chip RAM.

My next request is:

Investigate why the earlier HD title presentation at commit `ef32941` looked
better than the current floppy-compatible direct AGA presentation introduced
at commit `f52c472`, without changing gameplay or immediately converting the
screens to 256 colours.

First perform an objective A/B comparison. Extract or regenerate the title
asset from both revisions and compare:

- the complete SPBM files;
- decoded dimensions, depth and all 81,920 palette indices;
- all 64 RGB palette entries;
- planar layout and row stride;
- the old OS-managed View palette/display setup;
- the current direct Copper palette-bank writes and display registers.

Use `ef32941` as the visual HD reference while keeping the current direct-Copper
architecture as the required ADF-compatible foundation. If the old and current
bitmap/palette data differ, feed the old accepted bitmap and palette unchanged
through the direct presenter. If they are identical, locate the display-path
difference before modifying any artwork or quantisation.

Build a testable HD package and bootable ADF that use the same assets. Always
run `make` and `make release`. Tell me exactly what differs and which hypothesis
the evidence supports. Do not change the stable gameplay renderer, its
line-100/line-300 phases, sprites, Bobs, collision, audio or animation
contracts.

Do not start with 256-colour, higher-resolution or HAM8 experiments. Those are
follow-up options only after the old HD appearance has been reproduced through
the direct AGA path. Do not claim FS-UAE or real-hardware visual verification
until I provide screenshots or test results.
```