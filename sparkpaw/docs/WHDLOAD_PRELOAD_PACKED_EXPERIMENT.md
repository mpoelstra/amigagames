# WHDLoad packed PRELOAD experiment — 2026-09-23

Current outcome after this historical experiment: the later
`dist/Campaign-WHD-Cache-020` is user-played in FS-UAE with stable TITLE/LOADING,
fast loads, working Drowned F10 and tested transitions. Its compressed assets
and PRELOAD remain; the earlier rejected drawer and Diag results below explain
how that candidate was reached. A separate, now user-approved
`dist/Campaign-WHD-Soundtest-SFX` adds Drowned pump shot and checkpoint to
Soundtest. The played `Campaign-WHD-Cache-020` baseline and `.uaem` launcher
were subsequently archived intact under `dist/older-builds/` after FS-UAE
stopped; all hashes matched. Real A1200 behavior is untested. Alpha.8 is still the official
release.

The rejected `Campaign-Drowned-WHD-Preload` drawer and all raw Diag drawers
were moved intact to `dist/older-builds/` after FS-UAE stopped. The current
test candidate remains `dist/Campaign-WHD-Cache-020`.

**FS-UAE result:** rejected as a flicker correction. User reports TITLE/LOADING
still flicker on cold startup and Stormrail direct start. Direct Drowned returns
to Workbench from LOADING in both packed and raw WHDLoad candidates, while the
accepted HD path works. Subsequent raw Diag2/3 logs isolate a 198-byte Drowned
rear stride that fails the AGA 32-bit layout check; the focused fix is staged in
`dist/Campaign-Drowned-WHD-Diag4` for user playtesting. This crash is separate
from the flicker. The byte savings below remain technical storage results, not
runtime acceptance.

The accepted HD campaign stays unchanged. This unnumbered WHDLoad experiment is
`dist/Campaign-Drowned-WHD-Preload`; alpha.8 remains the official release.

## Reason and prior rejection

The first campaign WHDLoad slave requested 8 MB ExpMem on an 8 MB Fast target
and failed with "Can't allocate ExpMem." Reducing ExpMem to 5.5 MB and removing
PRELOAD let the user start the game, but FS-UAE showed repeated TITLE/LOADING
flicker during startup and after Escape; CHARGING, READY and gameplay were
stable. This is user observation, not a captured timing measurement.

This is **not** the first attempt to apply ADF compression outside ADF.
`STORAGE_MEMORY_LOADING_AUDIT.md` and `docs/DEVELOPMENT_HISTORY.md` record the
alpha.35 HD SPR1 experiment on four large assets. Supplied FS-UAE video
`testresults/Phase 6C.1-alpha35-rejected-fsuae-hd-loading-stall.mov` remains
on LOADING for 30.8 seconds, with user-reported artifacts; alpha.36 restored
raw HD assets and reached gameplay. No root cause of the packed failure was
proven. That result is a mandatory first gate for this new experiment.

## What differs in this bounded candidate

The candidate keeps the standalone namespaced Drowned engine, gameplay,
palettes, intro, music and screen art. It changes the **WHDLoad-only file
representation** and reuses the tested SPR1/SPL1/SPD1 streaming reader. All
74 compiled runtime references have exact source mappings and host C-reader
round trips. The two collision files remain raw. The executable is compressed
with Shrinkler's verified Amiga executable mode.

| Item | Original WHD campaign | Packed PRELOAD experiment |
| --- | ---: | ---: |
| Asset file bytes | 5,040,578 | 1,778,401 |
| Executable file bytes | 606,748 | 131,024 |
| ExpMem request incl. Kickstart | 8 MB in first candidate; 5.5 MB in no-PRELOAD fix | 4.5 MB |
| PRELOAD | First candidate yes; no-PRELOAD fix no | Yes |

The packed on-disk payload totals 1,909,425 bytes before filesystem overhead,
slave and icon. This does **not** prove all files preload on a real 8 MB Fast
configuration. It leaves about 3.5 MB Fast outside the slave's ExpMem request,
from which WHDLoad, Workbench and preload must all fit. Runtime allocations
within the 4 MB emulated game Fast remain unmeasured in FS-UAE.

WHDLoad's own documentation says `Preload` stores as many files as fit and
stops when memory runs short. It also says `PreloadSmart` only prioritizes files
observed in previous runs, so it cannot guarantee a clean first cold start.
Its programming guide explains that an uncached file access switches to the
host OS. Sources: https://www.whdload.de/docs/en/opt.html and
https://www.whdload.de/docs/en/howto.html.

## Technical proof and acceptance boundary

Native 68020 campaign build and slave assembly succeeded. Slave header reads
0x480000 ExpMem and the icon includes PRELOAD. Shrinkler verified its output.
Every packed file round trips to the byte-identical accepted HD asset via the
actual C reader; all 74 staged names are compiled references. Existing
Drowned lifecycle, parent audio/menu/F10, namespace and packed-reader tests
pass; the original test did not cover F10 inside the namespaced Drowned loop.
Ordinary WHD and ADF executable hashes remain unchanged by the guarded source
path. No emulator was started by Codex.

**First user gate:** cold boot in FS-UAE at PAL 68020/2 MB Chip/8 MB Fast/no
JIT; check that TITLE -> LOADING -> CHARGING -> READY completes without flicker,
stall or artifacts. Then Escape to title and repeat. Only after that test
Continue, direct Options starts, Soundtest, Drowned results and Replay. A
successful first gate would not yet prove all runtime transitions or real
hardware. If the alpha.35 loading symptom recurs, retain this candidate as a
rejected experiment and investigate the decoder/ownership before changing HD.

## Diag4 result and reduced-reservation candidate

MrDig's raw/no-PRELOAD Diag4 run reaches Drowned gameplay after the rear-stride
correction, but he reports about five minutes of loading, continuing TITLE and
LOADING flicker, and F10 not working in Drowned. The 12.05-second FS-UAE clip
`testresults/Unassigned-rejected-Drowned-WHD-Diag4-loading-black-flashes.mov`
shows brief LOADING-art flashes against mostly black frames. It does not show
the complete load or F10. The Diag4 logs reach `renderer_ready` with 523,720
Chip and 3,633,536 Fast bytes free. This rejects Diag4 as a WHDLoad candidate
while supporting the narrow 200-byte stride fix.

Source review found the namespaced Drowned loop polls the shared raw keyboard
input but did not check its WHDLoad quit flag. The new explicit module quit
return lets the parent restore the saved Workbench View and DMA, then exit.
The native build and host Drowned lifecycle/F10 test pass. This route still
needs a user FS-UAE exit test.

`dist/Campaign-WHD-Cache-020` is a separate unnumbered candidate with the same
74 lossless packed assets, Shrinkler executable, and PRELOAD. It removes the
per-stage diagnostic file writes and reduces the slave's ExpMem request from
0x480000 to 0x380000 (3 MB game Fast plus 512 KB Kickstart). This leaves an
extra 1 MB of the 8 MB Fast target available to WHDLoad's host file cache.
The staged assets total 1,778,401 bytes and the executable is 131,072 bytes.
WHDLoad's documented PRELOAD behavior stops when memory runs short; the new
allocation improves the available budget but does not prove full cache
coverage. Uncached loads switch to the host OS and are a supported hypothesis
for the observed long black interruptions; the supplied logs do not timestamp
reads, so the cause and the five-minute time distribution remain unmeasured.
MrDig subsequently reports that this current WHDLoad candidate runs well in
FS-UAE: TITLE and LOADING do not flicker and loading is fast again. The exact
read timings and full cache coverage were not logged. F10 in Drowned, the full
campaign, ADF and real-hardware operation remain separate test gates.
