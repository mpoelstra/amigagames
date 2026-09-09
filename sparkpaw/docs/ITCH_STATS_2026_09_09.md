# Itch development statistics — 9 September 2026

Snapshot at commit a249c98, Sparkpaw 0.7.0-alpha.5. These figures describe
this local working archive, including ignored evidence and old builds. They
cannot all be reproduced from a fresh Git checkout. No estimate of unique
experiments or total hours is implied.

- Git: `git log --format=%ad --date=short -- sparkpaw`: 83 entries,
  19 distinct commit dates, 2026-08-05 through 2026-09-09 (35 elapsed days).
  Includes documentation commits; does not count uncommitted work as commits.
- `sparkpaw/testresults`, recursive regular files: 664 total. Categorized
  evidence: 124 MOV + 1 MP4, 136 LOG, 106 PNG + 2 JPG, 294 TXT = 663.
  One extensionless file is excluded from the published evidence count.
  Images are not all guaranteed screenshots; TXT includes sidecar metadata.
- `sparkpaw/dist/older-builds`: 449 distinct directories with a direct
  ReadMe.txt/ReadMe.md/ReadMe (case-insensitive) and a direct file starting with
  Amiga HUNK_HEADER bytes 00 00 03 F3. This counts nested archived test drawers,
  not only the 142 top-level directories. It does not deduplicate binaries or
  prove acceptance; 461 directories have a ReadMe regardless of executable.
- `sparkpaw/build`: 89 immediate subdirectories. These include generated
  asset directories, packaging and audits, not 89 independent experiments.
- `sparkpaw/music/experiments`: 9 immediate subdirectories.
- `sparkpaw/experiments`: 2 immediate proof project directories
  (audio-level1 and audio-gameplay).
- Tracked Sparkpaw files, current working contents, excluding `/third_party/`:
  19,712 lines across 104 C/header files; 13,392 lines across 102 Python files;
  217 lines across 4 assembly files; 22,331 lines across 297 MD/TXT/HTML files.
  Counted with splitlines(); includes comments, blank lines and generated
  tracked sources. Documentation includes some packaged or repeated copy.
  These line counts were taken before adding this snapshot and its HTML block;
  public copy deliberately rounds them to approximately 33,000 and 22,000.
- Product counts and disk free space: RELEASE_0_7_0_ALPHA_5.md,
  ALPHA5_ARTIFACT_SHA256.json and src/audio_catalog.h.

The former 80–120-hour estimate remains historical, not a new measured total.
No extrapolation from commits, files or elapsed calendar days was performed.
Old narrow performance measurements were omitted from the page statistics to
avoid presenting them as current whole-campaign or hardware FPS guarantees.
