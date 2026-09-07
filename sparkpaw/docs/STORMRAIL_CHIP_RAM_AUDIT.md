# Stormrail Chip RAM audit — 2026-09-05

## Supplied evidence

User reports successful ordinary HD and campaign WHDLoad testing of the latest
alpha (current checkpoint 0.7.0-alpha.2) on a real Amiga. In a separate HD case,
about 1.45 MB free Chip RAM permits startup and Level 1, but Stormrail crashes.
Follow-up: failure is immediate during Stormrail loading, with black output
and flicker at the top, matching the user's earlier low-Chip startup symptom.
With Workbench reduced to two colours, free Chip rises to 1.8+ MB and the same
ordinary HD version works including Stormrail. WHDLoad also runs Stormrail
successfully when launched from the approximately 1.45 MB-free Workbench setup.
This paired HD observation strongly supports insufficient available Chip RAM;
the exact failing allocation and partial-failure display/cleanup path remain
unmeasured. WHDLoad's working result does not establish the same runtime free
memory layout as ordinary HD. Exact bytes/largest block and individual F10/
intro/replay checks were not supplied. No new recording/log supplied; physical
ADF/Gotek and Pocket acceptance are not inferred.

The documented 2 MB installed Chip + 8 MB Fast minimum does not guarantee that
Workbench leaves enough free or sufficiently contiguous Chip memory.

## Source findings and ranked candidates

1. **Remove unused world rows first.** Runtime SPBM headers show Level 1 front
   and rear at 208 rows; Stormrail front, rear and flight-rear are all 256 rows.
   `tools/generate_stormrail_environment.py` has WORLD_H=256 and GAMEPLAY_H=208;
   `src/renderer.c` has WORLD_H=HUD_TOP=208, with a separate HUD. Unpadded
   source storage in the bottom 48 rows is 81,408 bytes front plus 20,160 bytes
   for each rear: 121,728 bytes. `prepareRearGuardedDisplay()` additionally
   allocates both rear display copies at source height, so shrinking these also
   saves roughly 41 KiB, giving about 160 KiB total Chip potential. Actual
   graphics.library row padding and allocation overhead require native measure.
   Before implementation, prove all visible rows/palettes remain byte-identical
   and all fetch/restore/replay bounds fit 208; retain the horizontal guard.
2. **Skip Level-1-only preparation in Stormrail.** Common preparation still
   builds beetle and Strider caches/stages, Core, extra life, water and splash.
   Core alone requests 18*48*4*2*5 = 34,560 Chip bytes. Strider masters already
   use Fast RAM; only their Chip stages count as Chip savings. Guard every
   consumer/reset/cleanup before skipping anything. Audio selection is a later
   bounded option; do not repeat rejected broad audio/performance refactors.
3. **Review rear source placement separately.** Source rear images coexist with
   guarded display copies in Chip RAM. CPU-only/Fast sources could save more,
   but alternative renderer branches and validation currently reference the
   source bitmap, so this is not a drop-in allocation-flag change.

Do not reclaim departure sources after boarding: resident Stormrail replay
needs them. Do not defer Harrier art/audio loading until the finale. Preserve
4+3 palettes, ring/Copper/HUD ownership, animation frames, accepted transitions,
resident replays and all release artifact bytes.

## Failure handling and validation

`main.c` releases Level-1 renderer/audio before loading Stormrail on Continue;
there is no obvious complete-Level-1-world overlap there. Replay loading also
retires the old results images before preparation. The existing failure path
prints a generic transition error and calls cleanup; the observed black/flickering failure starts during loading, but the exact
allocation, partial-preparation or cleanup failure remains unmeasured.
Audit partial-allocation cleanup and preserve active DMA/Copper ownership;
report the failed stage, requested bytes and free/largest Chip values only
through safe DOS ownership. A preflight threshold alone cannot handle all
fragmentation or allocation failures.

Next implementation should be one focused height correction with host pixel
and bounds checks/native compilation, then one user-tested candidate. Capture
free/largest Chip at launch, before/after Stormrail preparation and results;
exercise direct selection, Continue, resident replay and return. If the fault
persists, collect a focused stage log before another change. No routine automatic
FS-UAE test, new release, commit or push. This audit changes documentation only;
no claimed measured savings or lower supported free-memory threshold.

## Candidate 1 staged — 2026-09-05

User authorized optimization and exactly one ordinary HD ZIP for real hardware.
Implemented only WORLD_H=GAMEPLAY_H=208 in the Stormrail environment generator.
All three generated worlds lose their bottom 48 zero rows. Original alpha.2
palette and per-plane visible bytes compare identically; source payload shrinks
by exactly 121,728 bytes. Both guarded rear displays inherit the reduced height,
so expected total Chip saving remains about 160 KiB (native padding-dependent).
No executable, renderer, gameplay, audio, cleanup or other asset changes.
Unused Level-1 caches and improved allocation-failure handling remain deferred.

`make PYTHON=../.venv/bin/python3` and the full host suite passed, with existing
compiler warnings. Extended test_runtime_memory_layout.py guards the 208-row
headers and accepted palette/visible-plane hashes. Rebuilt HD executable SHA256
remains e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e.

Sole active manual package: `dist/Storm-Chip1-HD.zip`, 867,513 bytes (about
847 KiB), versus alpha.2 HD ZIP 886,173 bytes. Exactly one executable, ReadMe
and 48 manifest/executable-verified runtime files; no icon or extra artifacts.
ZIP CRC/content readback passes; all path components fit 30 characters. All
102 inventoried baseline release files remain byte-identical. Internal staging
and verification live in build/storm-chip1; no extracted candidate remains in
dist. ZIP SHA256: c3859e7a9bf23a42dc7d8f9efbdcf05b903cc6cf61e80424563d3572bbb141b6.

Await user real-Amiga test at approximately 1.45 MB free Chip: direct Stormrail
selection and Level-1 Continue, boarding/flight/finale/results, resident replay
and return to ready. No automatic FS-UAE, other package, SemVer, release or
commit/push. Savings and low-memory acceptance are not yet measured on hardware.

## Candidate 2 staged — 2026-09-05

User authorized further optimizations except disk compression. First bounded
follow-up skips the four Level-1-only graphics source loads in the campaign
Stormrail branch and the six cache families counted in the storage audit.
Production/startup/full-diagnostic preparation expressions share this selection;
the Level-1 renderer compiles away the guard. animateWater now explicitly exits
for Stormrail before using its omitted cache, including during departure.
Existing Core/extra-life/enemy/splash draw guards and zeroed target histories
protect other consumers; cleanup already accepts null cache pointers. Shared
player/diamond/HUD/static collectible preparation and all audio remain intact.
No logical/physical world-width or rear-pointer change is mixed into this gate.

Additional requested Chip saving: 69,720 bytes (68.09 KiB). Avoids 187,240 bytes
of reads per Stormrail load and the associated conversion; those four source
sheets already used Fast RAM and are not counted as Chip savings. Expected
combined saving with Chip1 is about 228 KiB, not yet native-measured. These
files remain packaged because Level 1 uses them, so this is not an installed
storage reduction. The ZIP is 100 bytes larger than Chip1 due to executable/
ReadMe differences, not asset growth.

New test_stormrail_load_selection.py compiles the actual C load-selection and
production cache-preparation expressions with dependency stubs under ASan/UBSan.
It exercises repeated section switches and individual dependency failures,
verifies omitted caches/files and short-circuit read failure. Full host suite
and native HD build pass, with existing warnings. All 48 staged assets match
Chip1 byte-for-byte. The executable now intentionally differs from alpha.2.

Sole active package: dist/Storm-Chip2-HD.zip, 867,613 bytes, 50 files.
ZIP SHA256 d959f3d1e1984f4a5273d566188c912a4c15391d0f4974af76c76babbbb1ca60.
Executable SHA256 a1e09562203e2036d27a3e56a4278a85ae5242041f0350cddfd0b298df5cd526.
All 102 baseline release files remain identical. Chip1 ZIP moved intact to
older-builds; staging/logs/verification in build/storm-chip2. Await native
Stormrail direct/Continue/replay and return-to-Level-1 regression. No emulator,
ADF/WHDLoad/LHA, new release, SemVer, commit or push.

## Chip2 HD acceptance — 2026-09-06

On 2026-09-06 the user accepted Storm-Chip2-HD.zip on a real A1200 and
requested retaining its optimizations in the main game. The normal development
source, executable and runtime assets already contain Chip1 + Chip2; they are
now the accepted development baseline, not a new release. Stormrail still needs
more than approximately 1.45 MB free Chip RAM; the precise free/largest-block
threshold and native savings remain unmeasured. Individual transition/replay
checks and other media are not inferred from this general HD acceptance.
