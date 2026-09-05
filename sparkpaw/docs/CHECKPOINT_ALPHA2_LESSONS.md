# Lessons retained at 0.7.0-alpha.2

## Performance evidence

- Reproduce the original executable byte-identically before naming compiled-code
  comparisons after a historical release. Alpha.68 provenance is established;
  48.58 FPS belonged to alpha.45. The 49.30 collision-cache log lacks its footer.
- Different manual routes cannot establish a small causal FPS gain. User A/B
  play narrowed Level-1 differences to practically none; retain approved small
  changes without a speed claim. Performance research is explicitly parked.
- Audio split, gameUpdate extraction/completion caching and linker-order changes
  were rejected. Do not repeat them without substantially new evidence. Failed
  diagnostic alpha.68 charging runs are not valid performance measurements.
- Protect complete frame chains, renderer helpers, publication order, memory
  ownership and Stormrail evidence; file length/program size is not a diagnosis.

## Disk packaging and presentation

- A complete union of files across disks does not prove either disk is usable.
  The first pair omitted storm-collision.bin from Disk 2 even though collisionLoad
  opens it unconditionally during Stormrail entry. This blocked both DF0 swaps
  and DF1 starts. Check compiled dependencies PER VOLUME, including collision,
  shared audio and logical Level-1 assets still read by Stormrail preparation.
- Logical ownership and actual load/residency differ. Deliberate common-file
  duplication fits and preserves accepted lifetimes; do not force a renderer
  refactor merely to obtain an idealized physical group split.
- Decoder host tests alone do not validate media resolution or live Copper
  lifetimes. Exercise actual decoder code, per-volume coverage, CRC/readback,
  drive selection, failed media and source-to-package identity separately.
- The user expects Disk 2 already in DF1 to be discovered without a prompt;
  otherwise use the same loading image with only a status-strip change.
  INSERT now matches the bevelled cyan art. Technical helper lines and mouse
  cancellation were unwanted; wait automatically for the right disk.
- Never free an image while the current Copper still points at it. Publish the
  replacement before retiring the old allocation; preserve that cold boundary.
- Compression is lossless storage work, not an FPS optimization. Maintain the
  512-byte DOS buffer, bounded 4 KiB history and final Chip/Fast asset placement;
  add no per-frame allocation or reads.
- File order now follows first use and avoids payload wrap-around. Central FFS
  metadata and repeated reads still cause seeks. The Gotek OLED video does not
  quantify seek timing; no seconds-saved claim without matched hardware tests.

## Releases and evidence

- User approval is medium-specific: HD and ADF acceptance cannot establish a
  newly built campaign WHDLoad's startup, F10, memory or transition behaviour.
- Audit every build target, not just renamed archives. The old WHDLoad target
  compiled only one level and its slave still reported alpha.49. It now uses
  campaign/split-renderer flags, F10 hooks, all 48 files and the new slave version.
- Keep raw user evidence, failed candidates and alpha.68 immutable. Archive
  completed drawers only after hashing, and keep one current artifact set.
- Fresh live itch downloads override stale web-search caches: live page and
  detector show alpha.68 with a matching devlog; one cached tool page showed
  alpha.62. Record the live baseline and write the complete player-facing delta.
- Runtime assets and tools must be reproducible from committed sources; do not
  depend on a temporary audit manifest for production compiler flags or hashes.
