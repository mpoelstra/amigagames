# WHDLoad loading measurements — 24 September 2026

## Current conclusion — 25 September 2026

This document retains the dated investigation history below. Current local
release is **0.7.0-alpha.10**; CONTROL and the retained loading optimizations
are included. Standard WHDLoad uses raw common/intro/current-level banks,
no PRELOAD/file cache, and NOCACHE; separate HighRAM uses raw files/PRELOAD
and requires >=16 MB Fast. Both reserve 5 MB game + 512 KB Kickstart.
The measured 57.16% reduction is CHARGING->READY20.68 ->8.86s in the prior
same-code FS-UAE020 A/B, not an alpha.8 or real-hardware speed comparison.
Alpha.8 failed ExpMem allocation on the 8 MB setup. First-image startup
investigation is deferred; final trace-free release/HighRAM playtests remain open.

Dist cleanup is complete: alpha.10 artifacts/drawers remain, previous root
items are intact in `dist/older-builds/alpha10-cleanup-20260925`, including
WHD-NoCache-B-8M, originalalpha8, alpha9 and Controls-Mode-HD.
See [current status](CURRENT_STATUS.md) and
[release verification](RELEASE_VERIFICATION_0.7.0-alpha.10.md).
Older statements about currentalpha9, pending A/B tests, active diagnostic
paths or waiting to archive describe their historical date, not next actions.


Source: user's completed FS-UAE run of WHD-LevelTimes-8M, requested 68020 /
2 MiB Chip /8 MiB Fast /PAL /no JIT gate. No new hardware claim or independently
read emulator configuration. Complete footer:36 rows, overflow0, every ok1.
Original log and parsed rows are preserved under
`build/whdload-times-analysis-20260924`. Original SHA-256: `46383de54edde4b5c93c747d99be1df34f220717016e9cd3f01ba5749b9b030d`.
The executed timing build and staged manifest are in
`build/whdload-times-20260923`. Its completed drawer including the original log
is archived intact as `dist/older-builds/WHD-LevelTimes-8M-measured`.

## Measured guest elapsed time

Fields divided by50. These are guest VBlank times, not CPU-only cycle counts
or physical storage/host OS-switch wall time. Visible fades/other unscoped
work can add time; no alpha8 matched-config timing exists in this log.

| Load visit | Gameplay assets | Collision | Audio | Renderer preparation | READY total | Menu cache (inside READY) | Minimum wait remainder |
|---|---:|---:|---:|---:|---:|---:|---:|
| Initial Level1 |0.58s|0.10s|0.40s|6.30s|3.42s|2.24s|0.00s|
| First Level1 return |1.92s|0.26s|1.08s|17.46s|9.20s|7.64s|0.00s|
| Stormrail |13.88s|0.08s|0.40s|5.08s|—|—|—|
| Return from Stormrail |0.58s|0.10s|0.40s|6.30s|3.42s|2.24s|0.00s|
| Drowned |14.90s|1.04s|0.44s|18.02s|—|—|—|
| Return from Drowned |0.58s|0.10s|0.40s|6.30s|3.42s|2.24s|0.00s|

Typical CHARGING work:6.30+3.42=9.72seconds. Minimum100fields was already
exceeded by preparation, so it adds ZERO wait. READY menu reconstruction
accounts for2.24of3.42seconds, not an additional2.24seconds.
Stormrail scoped loading total19.44seconds; Drowned34.40seconds. These closely
match the user's approximate20and30–35second visible waits.

First return from Level1 is an outlier:26.66seconds renderer+READY, compared
with9.72on all three other visits. Even collision/audio/raw copy phases are
slower. User EXPLICITLY denies pause, turbo or CPU/speed changes. Preserve this
as a real unexplained within-run variation, not emulator-setting contamination,
not proof of a specific CPU-cache/interrupt cause, and do not average it away.
No CACR/cache-control call was found in src/ or the project's slave wrapper;
that limited search does not exclude KickEmu/WHDLoad effects.

## Important phase boundary

The renderer_prepare label is NOT pure derived-graphics construction.
Drowned preparePontoon loads/decodes pontoon-clip.bin, and
prepareDrownedRearAmbience loads/decodes drowned-amb.bin through
assetsLoadDiskData. Thus its18.02seconds include further decompression. The
initial suggestion that those18seconds all required a different renderer
optimization was too strong; the raw-current-level experiment also reaches
these reads. Gameplay_assets includes allocation/copy/decoding, not just
codec time, and the log does not separately identify CRC costs.

## Next controlled experiment: raw current-level sources

`dist/WHD-LevelRaw-8M` changes ONLY the stored bytes of level2.spb and level3.spb
(and instructions). The timed executable, slave, icon, common, intro and
Level1 banks stay byte-identical to this measurement. The faster decoder
remains compiled in but these bank entries now take the raw path. No renderer,
READY, timer, gameplay, stats or replay modification. No whole-campaign
preload: one current-level bank is replaced only at an existing black load
transition. Host disk reads become larger; black-interval length needs review.

| Bank | Previously stored | Raw | Extra source memory | Projected free Fast at measured renderer end |
|---|---:|---:|---:|---:|
| Stormrail |236418|770896|534478|1641218|
| Drowned |440159|1435108|994949|735491|

Projection subtracts new source bytes from the measured phase-end snapshots;
it is NOT proof of peak/contiguous free memory or success on8MiB. Keep the
same5MiB game +512KiB Kickstart reservation. Each raw bank remains under the
2MiB parser cap. Startup sources stay2837614bytes; largest post-intro source
set becomes2872466bytes. The8MiB user gate must decide fit and actual speed.

All74raw entries equal canonical sources and pass the actual hybrid-reader
checks; actual bank lifetime/fault/format and14/14/17phase-dependency tests
pass. Collector tests pass. Earlier native/full-suite proof carries forward
via exact executable parity; no runtime source change or new native build was
needed for this storage experiment. Proof folder:
`build/whdload-levelraw-v2-20260924`. First packaging attempt was retained after
fixing an argument-variable shadowing error; no failed attempt was staged.

Focused retest: same020/8MiB setup, skip intro if desired; START AT Stormrail ->
brief play -> Escape, then Drowned -> brief play -> Escape. At READY left-click
once, wait for deliberate frozen save, F10 to flush/exit WHDLoad, stop FS-UAE.
Log is `dist/WHD-LevelRaw-8M/data/load-times.log`. Report load failures or black
read delays/flicker; do not add RAM to obtain success. CHARGING is not directly
optimized here; its timing and the unexplained slow return remain under study.
No emulator launch, release, commit or push.

## 2026-09-24 — raw current-level user test measured

User reports everything seems faster; CHARGING after intro/title and after
Escape now feels longest. Complete LevelRaw log:36 rows, all ok=1, overflow=0;
original bytes and parsed rows preserved in
`sparkpaw/build/whdload-levelraw-analysis-20260924` (repo-relative).
Guest PAL-field scoped loading: Stormrail19.44 ->5.96s; Drowned34.40 ->9.24s.
Stormrail assets13.88 ->0.40s, renderer unchanged5.08s. Drowned assets14.90
->0.70s and renderer18.02 ->7.06s (includes previously packed late assets).
CHARGING renderer6.30 +READY3.42 =9.72s at ALL four visits; minimum wait0.
Previous26.66s anomaly did not recur; cause remains unexplained.
Drowned renderer-end free Fast735488bytes; endpoint only, no peak proof.
Disk/host-switch wall time excluded. User has not supplied a separate explicit
flicker verdict or new hardware result. Keep candidate in place, no archive
or replacement while emulator stop is unconfirmed. Next investigation is
Level1 renderer preparation6.30s plus READY3.42s (menu reconstruction2.24s
included), preserving READY cache/display ownership and8MiB memory budget.
No runtime code change, release, commit or push in this results review.

## 2026-09-24 — READY cache optimization and renderer subphase candidate

After measured raw-bank success, user asks to continue. FS-UAE explicitly
confirmed stopped. Next candidate `dist/WHD-ReadyFast-8M` / Sparkpaw icon:
READY RLE chooses literal/zero once per run and uses memcpy/memset; delta
reconstruction processes four bytes per loop with bounded pointer increments.
No new menu cache or persistent allocation, no lazy work during READY input,
no framebuffer ownership, fade, music, gameplay or bank policy changes.
Native assembly confirms inline longword copy/clear and four XOR bytes per
loop. The archived before-assembly multiplies band height by196 on EVERY
byte; the candidate computes that length once per band. Exact speedup is NOT known until manual 020 measurement; do not promise
the full2.24s disappears. Source changes apply to HD and ADF menu preparation;
ADF pixel tests pass but native ADF/hardware acceptance remains untested.

Loading-only collector adds five nested renderer phases (setup, enemies,
effects, targets, final) within renderer_prepare. They must NOT be added to
parent time. Existing historical Stage2 optimized renderer was ~5.9s, but that
is a different workload/build, not a matched alpha8 comparison. No alpha8
source tag exists locally; the archived alpha8 artifacts remain protected.
Do not invent a precise alpha8 CHARGING baseline.

Build `build/whdload-readyfast-v2-20260924`:625704 bytes,
SHA256654ee3d446fd52f6a8e1c9dcc31be56a506729b961bba27257ca68697f390660.
All five raw banks, slave and icon match played LevelRaw byte-for-byte.
74 actual-reader asset checks, bank lifecycle/fault sanitizer checks,
14/14/17 section dependency checks and loading collector tests pass.
READY exhaustive parity:1436 HD states/6262 transitions,50 ADF states/328
transitions, cache allocations unchanged309471/104847 Fast bytes.
Full host suite PASSED with make test PYTHON=../.venv/bin/python3. Initial
system-Python attempt lacked dependencies; retained log, reran using project
venv. Played LevelRaw drawer+log+launcher archived hash-identically as
`dist/older-builds/WHD-LevelRaw-8M-measured`. ReadyFast is now staged and its
inventory verified; all26885 original dist files unchanged. Emulator result
remains pending.

Focused manual gate: same020/2MiB Chip/8MiB Fast/PAL/noJIT. Inspect initial
intro/title/CHARGING/READY; briefly navigate OPTIONS and SOUNDTEST for text,
music and dust, play Level1 briefly, Escape, left-click final READY once.
Expected frozen save state; wait, F10 to flush/exit WHDLoad, then stop FS-UAE.
No full campaign or real-hardware run requested yet. Read THIS drawer's
`data/load-times.log`. No release/version bump, commit, push or emulator launch.

## 2026-09-24 — ReadyFast result: successful menu optimization, slow returns recur

User: initial CHARGING ~8s, both Level1 Escape returns ~17s, WHDLoad launch to
first intro ~10s. Complete36-row/no-overflow log saved byte-identically in
`sparkpaw/build/whdload-readyfast-analysis-20260924`. All phases ok=1.
Initial renderer6.30 +READY1.76 =8.06s (previous9.72); menu cache0.60s versus
2.24 before. Renderer subphases: setup0.96, enemies2.00, effects3.12,
targets0.22, final0.00. Nested subphases already included in total.
BOTH Level1 returns: renderer17.48 +READY3.20 =20.68s; cache1.62/1.64s.
Setup2.80, enemies5.54, effects8.48/8.50, targets0.60, final0.06/0.04.
Even assets1.92 versus0.58, collision0.26 versus0.10, audio1.10 versus0.40:
broad phase slowdown, not just menu reconstruction or a fixed wait. Minimum
wait remains0. Earlier26.66s return anomaly has now recurred in a distinct
build; do not call it resolved or blame user settings. Exact mechanism is
still unproven (CPU/cache/interrupt/platform restore remain hypotheses).

Boot currently loads common+intro+Level1 banks before first intro:2837614
source bytes, plus WHDLoad/Kickstart/program startup and display preparation.
Thus raw intro does NOT imply an immediate first picture. User's10s startup
is valid observed wall time, not established by the current loading counters.
It should remain an optimization target, not be marked acceptable/normal.
No matched alpha8 startup timing. Later bank reads may differ from same-Level1
return: bank selector skips disk read when section stays1; this correlation
does not prove a WHDLoad cache-reset cause. No speculative runtime fix made
in this result review. Current ReadyFast drawer stays intact; emulator stop
has not been reconfirmed for this run. No release, commit, push or emulator run.

## 2026-09-24 — LoadState diagnostic isolates slow Level1 returns

User asked to continue and explicitly confirmed FS-UAE stopped. Source audit
of platformReleaseForLoading, audio stop/unload, ptplayer adapter, music and
KickEmu has NOT proved a fix. Slow return spans raw assets/collision/audio,
renderer and READY, so another isolated codec change cannot explain it.
Same-Level1 bank selection skips actual DOS/disk work; later-level selection
reads a bank. This is a useful comparison, not proof of cache/IRQ causation.

Next `dist/WHD-LoadState-8M` / Sparkpaw icon is diagnostic-only relative to
ReadyFast. Same optimized menu, raw banks, slave/cache policy and display code.
`--load-state` requires `--load-trace`; records before/after CACR (read through
Exec Supervisor,68020+), DMACONR, INTENAR, INTREQR, CIA-B CRA/CRB and CIA-A TOD.
No CACR writes, cache flush, interrupt changes, or CIA ICR reads (read clears
pending state). New phases: boot_banks, return_release, return_cleanup,
return_title. State lines reference zero-based phase-row indices; decimal
values. Existing nested renderer/menu phases must not be counted twice.
CIA-A TOD delta is supplementary: do not assume WHDLoad OS switches preserve
wall-clock meaning. boot_banks excludes WHDLoad/Kickstart/program/platform
startup; still ask approximate click-to-first-picture time. No synthetic CPU
benchmark or deliberate disk reload is introduced. Snapshot table increases
diagnostic BSS, not persistent asset cache. Slow-return improvement not promised.

Native build `build/whdload-loadstate-v2-20260924`:626708bytes, SHA256
b59aaf2898b1baf0f3dcff33b8efffbd76cb19822835ddf8f69010e5c5da9ee6.
First build retained: compiler could not open /var/tmp temporary file;
escalated retry succeeded. No emulator was started. Actual native object
contains MOVEC CACR->D0 and RTE, no MOVEC write. State/basic collector sanitizer
tests, bank lifecycle/fault tests,74 asset reader checks,14/14/17 section
reachability and Drowned/menu-resume/Stormrail loading tests pass. Full suite
not repeated for this diagnostic-only change; prior ReadyFast full suite
passed. Native Supervisor/state reads still require user's emulator gate.

ReadyFast drawer including original log and launcher archived hash-identically
at `dist/older-builds/WHD-ReadyFast-8M-measured`. Staged inventory verified and
all26885 initial dist files unchanged. Manual test same020/2MiB Chip/8MiB Fast/
PAL/noJIT: let intro run, Level1 brief play ->Escape twice, same settings,
left-click final READY, wait frozen, F10 flush/exit then stop FS-UAE. Read this
drawer's `data/load-times.log`. No SOUNDTEST detour or later-level/hardware test
required. If startup fails, capture/report exact error, no blind retries.
No release, version bump, commit or push. Alpha9 stays current.

Reference checked: https://whdload.de/docs/en/cache.html and
https://www.whdload.de/docs/autodoc.html (resload_SetCPU/SetCACR). WHDLoad owns
cache policy; we sample rather than force CACR bits. A CACR value alone is
not proof of per-region cacheability on MMU systems.

## 2026-09-24 — LoadState log complete, initial loading also slow

User reported F10 initially did not respond, then succeeded. Exact drawer
`dist/WHD-LoadState-8M/data/load-times.log` exists,13916bytes, complete=1,
43 phase rows plus43 state rows, overflow=0, all ok=1. Saved byte-identically
with parsed rows in `sparkpaw/build/whdload-loadstate-analysis-20260924`.
CHARGING renderer+READY:21.08s initially,21.04s and21.08s after Escape.
Initial renderer879fields, returns877/879; READY175 each. Thus this run is
already slow before gameplay; do NOT keep claiming only Escape triggers it.
Diagnostic build/placement/instrumentation and run-state differences remain
possible; this is not a matched fast/slow transition within this log.
CACR remains1 at all snapshots (no observed register transition); this alone
cannot exclude cache mapping/placement effects. Ordinary prep DMA991/INTENA
24620 remain consistent. CIA-A TOD closely matches guest VBlank durations
except boot/release boundaries. boot_banks9guest fields /12TOD ticks is NOT
click-to-first-picture wall time and does not explain the reported10seconds.
No proven cause/fix yet. F10's delayed response did not truncate this log;
its specific delay cause is unmeasured. No new runtime edit, build, archive,
release, commit, push or emulator launch. Current drawer retained intact.

## 2026-09-25 — WHDLoad configuration audit and identical-code A/B prepared

User correctly stresses raw intro/READY/Level1 and current-level banks are
ALREADY implemented. Do not present that as work still to be done. No new
runtime changes today. Audit binary SetCPU sequence at offset92: alpha8,
alpha9 and current all203c0000393e223c00007f3f4ead0060 (same value/mask/call).
Thus no demonstrated cache-policy change between releases. ExpMem differs:
alpha8 0x800000, alpha9 0x380000, current0x580000. Alpha8's whole8MiB
reservation must NOT be blindly reinstated on total8MiB target with host OS.
Global S:WHDLoad.prefs read explicitly read-only from configured HDF partition0:
all options commented out. No disk/config writes. Latest FS-UAE log Sept24
14:07 confirms68020/MMU0/JIT0/real/cycle-exact and8MiB Fast. Saved config file
still says030, demonstrating why actual run log matters. Preserve both facts;
do not blame changed settings. Audit snapshots in
`sparkpaw/build/whdload-config-audit-20260925/configuration-audit.json`.

Configuration-only pair STAGED in dist; manual A/B test pending:
WHD-Cache-A-8M and WHD-NoCache-B-8M. Both use byte-identical played ReadyFast
executable SHA654ee3d446fd52f6a8e1c9dcc31be56a506729b961bba27257ca68697f390660,
same slave/five raw banks/reservation. Only B adds NOCACHE to icon; ReadMe also
differs. CPU cache is NOT file caching. Neither changes PRELOAD, file-cache
exclusion, startup bank strategy or per-level loading. No new instrumentation,
Supervisor probe or build/link placement changes. This deliberately tests
whether instruction caching affects the fast/slow preparation regimes, NOT a
proposed NOCACHE speed fix. Same behavior in A/B would narrow the hypothesis,
not establish a root cause or prove alpha8 parity. Startup10s remains open.

Prepare/stage tool `sparkpaw/tools/stage_whdload_cache_ab.py` verifies source
inventory, exact executable and pair-only differences, protects releases and
archives completed LoadState intact with its original log/launcher after
stop evidence. The latest preserved FS-UAE log confirms streams flushed,
emulation thread stopped and end of main; the async stop question is thus
no longer needed for that completed session. Process listing was unavailable.
LoadState archived hash-identically in older-builds/WHD-LoadState-8M-measured.
Both dist A/B inventories and archived log match the saved proofs.
Manual plan after staging: same020/8MiB/PAL, fresh emulator start for each,
A thenB, intro then Level1/Escape twice, left-click READY, wait frozen, F10,
stop. Separate logs. No full campaign/hardware test. No release/commit/push.
Sources: https://whdload.de/docs/en/opt.html (NoCache vs NoFileCache) and
https://whdload.de/docs/en/cache.html. All timing conclusions remain pending.

## 2026-09-25 — configuration A/B: NOCACHE faster; original alpha8 restored

User played both on FS-UAE and explicitly stopped emulator. B feels twice as
fast CHARGING->READY; no flicker in either. Startup still feels long but user
now questions remembered alpha8 duration and requests original WHDLoad back.
Both complete36-row/no-overflow logs preserved with SHA and parsed phases in
`sparkpaw/build/whdload-cache-ab-results-20260925`. All rows ok=1.
A CHARGING20.68s on ALL3 visits (renderer17.48+READY3.20).
B CHARGING8.86s on ALL3 visits (renderer6.96+READY1.90),57.16% shorter.
A setup/enemies/effects:2.80/5.54/8.46-8.48; B1.14/2.20/3.34s.
Byte parity of played executable/slave/banks rechecked; saved icons confirm
A SLAVE/PAL, B SLAVE/PAL/NOCACHE, with no other added option. Evidence now
strongly isolates CPU-cache OPTION influence on this FS-UAE configuration;
it does NOT prove why caches hurt, a real-hardware benefit, or a general
production recommendation to disable caches. Do not keep calling NOCACHE
only a slower control; it actually won this manual loading comparison.
Gameplay cadence/full campaign and realhardware remain unmeasured.

A completed drawer/log/launcher archived intact as
`dist/older-builds/WHD-Cache-A-8M-measured`. B stays at `dist/WHD-NoCache-B-8M`
for the requested startup comparison; original B log safely copied above.
Original alpha8 WHD ZIP CRC passes and64 file payloads restored byte-for-byte
to `dist/Sparkpaw-0.7.0-a8-WHDLoad`; source ZIP and all26885 pre-investigation
dist files unchanged. Original slave, PRELOAD/PAL icon and executable retained.
No new build/release/version/commit/push; alpha9 remains current release.
Alpha8's original ExpMem0x800000 remains, not a claimed total8MiB fit; do not
silently alter its slave or emulator RAM. If original fails memory gate,
record exact error before designing a separate comparable configuration.

Next user comparison: same FS-UAE settings, original alpha8 Sparkpaw icon;
measure click-to-first-intro and CHARGING->READY separately. Note any WHDLoad
splash-window wait separately if possible. Alpha8 has NO diagnostic log-save
mouse gesture: exit normally using F10; do not request left-click/freeze.
New source work deferred until this requested reference comparison. Current
first-image10s remains user estimate, not inferred from bank guest timings.

## 2026-09-25 — alpha.10 release preparation; alpha.8 memory baseline rejected

User supplied a screenshot of original alpha.8 WHDLoad 20.0 failing with
"Can't allocate ExpMem." on the target FS-UAE setup. Preserved byte-for-byte
as `sparkpaw/testresults/Phase 7B.1-alpha8-WHDLoad-ExpMem-failure.png` plus TXT.
Its 8 MiB reservation cannot coexist with host overhead in total 8 MiB Fast.
User's real Amiga has substantially more Fast RAM. This is NOT alpha.8 timing
evidence: withdraw the remembered alpha.8 speed comparison on target memory.

User authorizes next alpha if first-intro improvement needs more investigation.
Bounded review finds raw executable and raw intro already present. Bank boot
measurement begins after Kickstart/executable/platform startup and excludes
host switch wall time, so it cannot explain the entire estimated 10 seconds.
The deliberate 35-PAL-field display lock is retained. No new startup change,
splash override or fresh emulator run; further cold-start profiling deferred.

Preparing 0.7.0-alpha.10, same Phase 7B.1, public itch baseline alpha.8.
Standard WHDLoad uses raw common/intro/current-level banks, raw executable,
5 MiB game + 512 KiB Kickstart, no file caching/PRELOAD, and the played B
NOCACHE option. READY run-copy/XOR optimization and fast decoder remain.
The diagnostic collector and CPU-state probe are absent from production builds.
Separate HighRAM binary uses raw individual assets and PRELOAD, normal CPU
cache policy, target >=16 MiB Fast. No dynamic profile or stats prefetch.
HighRAM package correctness is testable locally; gameplay is still unplayed.
The 57.16% result is specifically the identical-code FS-UAE 020 comparison:
CHARGING->READY20.68 ->8.86s, three visits each, user reports no flicker.
Not an alpha.8 comparison, total startup gain, or real-hardware claim.
Whole-campaign cadence and hardware cache effects remain unmeasured.

Release tooling now uses isolated versioned build directories, keeps previous
builds and evidence, and independently verifies both WHD profiles and 3 ADFs.
The old skill's packed/PRELOAD3.5MiB prescription is superseded for the standard
edition by the user's explicitly tested and requested banked8MiB workflow.
CONTROL fcd8573 is included; no commit/push or public upload authorized.
The intermittent real-Amiga two-line HUD-boundary glitch remains open.
Build/package validation and final archival status will be recorded below.


## 2026-09-25 — alpha.10 packaged and verified locally

Local current release is 0.7.0-alpha.10 / Phase7B.1. Public itch downloads
freshly checked: alpha.8. No upload, commit, push or FS-UAE launch performed.
`make PYTHON=../.venv/bin/python3`, `make release PYTHON=../.venv/bin/python3`,
full `make test PYTHON=../.venv/bin/python3` and independent
`tools/verify_checkpoint_release.py` all PASS. Existing compiler optimizer /
no-effect warnings and duplicate Makefile '&' target warnings remain.

Dist contains 9 new artifacts: HD ZIP/LHA, standard8MB WHDLoad ZIP/LHA,
separate HighRAM WHDLoad ZIP/LHA and Disk1/2/3 ADF; all3 extracted drawers.
Native builds are isolated in `build/release-0.7.0-alpha.10/{hd,banks,highram,adf}`.
Production binaries have no load trace/state flags or load-times.log string.
Standard banks:74 raw canonical assets, actual C ASan/UBSan backend tests,
malformed/failure lifecycle cases and all14/14/17 section dependency reads pass.
Independent ZIP CRC/LHa test/Lhasa extraction parity passes for HD79 files,
standardWHD11 files and HighRAM80 files. All Amiga components <=30 chars;
icons tested; 3ADF filesystems and every file read back. Stable SP09D media
identifiers are format-compatibility markers, not a displayed release version.
HighRAM raw preload5,655,834 bytes + slave5,767,168 =11,423,002 bytes before
host overhead; >=16MiB is a conservative target, not a hardware acceptance.

Reports, archive byte sizes/SHA256: build/release-0.7.0-alpha.10/
checkpoint-release-verification.json and release-artifacts.json.
Full logs: build/alpha10-{make,release,tests,verification}.log.
All27,206 files present in dist before this release and all79 runtime source
files verified unchanged. Main previous build backed up under
build/release-alpha10-preservation. Existing alpha9, restoredalpha8 and B test
remain in dist temporarily: latest FS-UAE log is still active, and the stop
question has not been answered. Do not move these mounted drawers until stop.
Controls-Mode-HD is unrelated and remains protected. Archive intact, neverdelete.

Final trace-free release has package/host verification only. A focused user
check remains: standard8MB on same020 settings, intro->READY, Level1/Escape,
then short Stormrail/Drowned starts; report flicker/load failure. No mouse-log
save is present. HighRAM requires a separate >=16MiB playtest. No need to
repeat broad startup debugging now. 57% remains the earlier same-code A/B
CHARGING result, not a measured final-binary or hardware performance guarantee.
