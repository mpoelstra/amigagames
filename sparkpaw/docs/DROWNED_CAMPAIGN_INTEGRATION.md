# Drowned campaign integration — staged candidate, 2026-09-23

Latest acceptance: MrDig also approved the newest
`dist/Campaign-HD-Soundtest-SFX` and `dist/Campaign-WHD-Soundtest-SFX`
candidates after the Drowned Pump Shot and Checkpoint Soundtest addition.
These join the previously accepted `dist/Campaign-ADF-Disk3-Type` as the
current user-approved campaign test set. The user did not supply a new
route-by-route or real-A1200 report with this verdict. Earlier pending-play
statements below are historical; alpha.8 remains the official release.

Dist note: the played HD `Campaign-Drowned-020-HD` and WHDLoad
`Campaign-WHD-Cache-020` baselines now live intact under `dist/older-builds/`
after the user confirmed FS-UAE stopped. The accepted three-disk ADF remains
at `dist/Campaign-ADF-Disk3-Type`; the two Soundtest SFX candidates remain
active in root `dist` and are now user-approved. Earlier root paths below are
historical. All moved-file and alpha.8 hashes were verified in
`build/campaign-drowned/dist-cleanup-20260923-sfx.json`.

## ADF acceptance update — 2026-09-23

MrDig played `dist/Campaign-ADF-Disk3-Type` in FS-UAE: the full campaign, START AT and disk swaps pass. The complete-line INSERT DISK 3 art is approved. This accepts those emulator routes; physical floppy/real A1200 remains untested. The agreed ADF presentation has no Soundtest. HD/WHDLoad candidates `dist/Campaign-HD-Soundtest-SFX` and `dist/Campaign-WHD-Soundtest-SFX` add Drowned pump shot and checkpoint SFX to Soundtest; native/host checks passed and the user subsequently approved both.

The Soundtest host preloads those two existing samples (2,864 + 5,286 bytes of
Chip memory), while the namespaced Drowned engine retains its own gameplay
audio allocation and exclusive hardware ownership. The ADF build does not
allocate the extra previews. Its accepted disk images were not rebuilt for
the Soundtest change. The new READY cache and menu behavior pass host tests;
native HD/WHDLoad executables compile. The user subsequently approved both
latest candidates without a detailed new audio/regression breakdown. At
session close, the next user bug has not been described.

## HD acceptance update — 2026-09-23

MrDig played and explicitly approved `dist/Campaign-Drowned-020-HD/Sparkpaw-Test`: full three-section campaign, Continue and carried vitals/diamonds/score, Drowned results and Replay, direct Options start and Undertow Circuit in Soundtest. Image and music are accepted for this HD route. The earlier pending-HD statements below are historical. WHDLoad, ADF and real hardware remain unaccepted and require their own tests. Alpha.8 remains official.

User approved Level1 ring B and requests a complete playable campaign:
Level1 -> results Continue -> Stormrail -> results Continue -> Drowned ->
results Replay/Back to title; carried lives, health and diamond meter; direct
Drowned start through Ready/Options; Drowned v5 music in Soundtest.
Alpha.8 remains official, no release/commit/push/emulator launch authorized.

Implementation boundary: preserve independent compile-time gameplay layouts.
Drowned uses5120px world and different game/enemy state from3392px Level1.
Link its existing engine as a namespaced in-process module behind one cold
campaign-entry function. No subprocess, handoff files or per-frame dispatch.
Parent releases hardware/assets/audio before entry; module owns its complete
renderer/audio/lifecycle until return. No simultaneous prepared worlds. Global
symbols including audio IRQ/ptplayer/LSP and graphics.library base must remain
isolated; only explicit entry ABI and SDK/OS calls cross that boundary.
Namespace transform must preserve code/data bytes and relocation offsets,
resolve internal references and reject unknown object records. Verify with
real native link and fixture tests before using it for a candidate.

Entry snapshot immutable: score bank, lives, health, live diamond remainder,
secondary-button action, audio mode and seed. Drowned starts fresh local score,
time, enemy/collectible state; replay restores entry snapshot without awarding
previous results again. Direct start gives ordinary fresh vitals/zero bank.
Stormrail Continue banks its finalized total exactly once. Drowned results use
existing six-plane tally/presenter and menu behavior, with the existing120-second par/time-bonus rule. No new results illustration is
needed for this first integration. Drowned Escape returns to ready; final
Back to title clears campaign state. Input edges reset at every transition.

First implementation/test scope is integrated HD on current68020 target with
music. ADF/WHDLoad packaging, disk capacity, hardware and campaign cadence must
remain separate acceptance boundaries; never rebuild alpha.8 artifacts.

Ready UI extends OFFLINE band caches, not runtime text rendering. Add third
start selection and sixth track; retain old states and exhaustive pixel/mask/
skipped-hidden-target tests, regenerate caches. New menu label must fit existing
layout. Soundtest loads only while OS live; preserve exclusive audio ownership.

Gates: accepted log preservation -> module/link isolation proof -> campaign
snapshot/results logic -> lifecycle integration -> Ready/music cache extension
-> host/native/regression/asset checks -> single user-played integrated drawer.
No full acceptance claim before real user start/replay/Continue/Esc/back/menu
music tests. Preserve all existing dirty work and logs before staging.

## 2026-09-23 — Level1 B accepted; integrated three-section HD candidate staged

User played Level1 A/B, reports it works and explicitly retains B. Saved complete
logs `testresults/Level1-ring-{A,B}-020-run1.log` with provenance sidecars and
`build/level1-two-copy/run1-analysis.json`. Whole-run A48.73/B49.02 FPS; unequal
routes/reset exposure, no causal speedup claim. LEVEL1_TWO_COPY_RING now belongs
to RELEASE_RENDERER_FLAGS, still effective only inside renderer_level1_unit.
Official alpha.8 files/version remain untouched; this supersedes opt-in status.

User then requested complete integration. New HD target `make campaign-drowned
PYTHON=../.venv/bin/python3` produces a single three-section executable:
Storm Ruins -> Continue -> Stormrail -> Continue -> Drowned -> Replay/Back.
Ready Options adds direct Drowned; Soundtest adds UNDERTOW CIRCUIT (v5).
Lives, health, diamond remainder and banked score pass through an immutable
section entry; replay restores entry vitals/fresh local tally without double
banking. Drowned uses existing stats art/tally, own enemies/diamonds/elapsed/score
and existing120s par policy. Direct start gives3lives/6health/0diamonds/0bank.

See `sparkpaw/docs/DROWNED_CAMPAIGN_INTEGRATION.md` (root-relative) for architecture
and acceptance boundaries. Drowned is a separate namespaced in-process engine,
no subprocess/per-frame section branch. Parent relinquishes renderer/audio/DMA
before module entry; module closes before title/Ready return. Static code grows;
this is a migration seam, not the final shared-primitives architecture for disks.

Checks passed: actual driver ASan/UBSan lifecycle including replay, Escape,
gameover, pause and seven injected loader/result failure boundaries; campaign
snapshots/HUD carry; real vasm/vlink namespace fixture, unchanged object payloads,
288 isolated definitions; menu controls including third section/sixth music track;
preview start/stop/failure/IRQ ownership; old1070menu states unchanged, new1292
states/5478transitions and ADF50states/328transitions; new labels visually checked
from host rasterization. Actual native Level1/Stormrail renderers, dispatcher,
game, mixer and platform assembly exactly match played B. Audio gameplay prefix
unchanged, new helper only in menu path. Nine Drowned units audited against
ordinary full flags: unchanged code except appended game/player carry helpers.
New main driver and final link placement still require runtime acceptance.

Active `dist/Campaign-Drowned-020-HD/Sparkpaw-Test`,606528bytes,
SHA256 de11d25a9e4334d70a5d0ab4961e62c9af055471bce51950e9261094bb6401e3.
74assets/74compiled references; all74assets match prior Drowned drawer; all68
alpha.8 release files byte-identical. Loader allocation table totals693244bytes
(including BSS, before runtime assets); requested8MB Fast supports code growth.
No new gameplay Chip buffers; actual integrated free-memory/fragmentation and
transition audio still need manual evidence. Ordinary music build, no diagnostics,
no mouse-save freeze, no measured integrated FPS claim.

User confirmed FS-UAE stopped. Three superseded drawers with logs and launchers
moved intact/hash-verified to `dist/older-builds/20260923-campaign-inputs`.
Only integrated candidate plus official alpha.8 set remain active. No emulator,
release, commit or push. HD manual acceptance pending: direct Drowned/Escape,
Soundtest return, Stormrail Continue and HUD carry, Drowned finish/replay/back,
then full campaign inclLevel1. ADF/WHDLoad/hardware not integrated or accepted.


Runtime acceptance, not host tests, is required for Copper/DMA safety at actual
PAL raster timing, sufficient fragmented Chip headroom, music transition quality
and cadence after final link placement. Existing Drowned busy evidence had
261040bytes Chip free, not a measurement of this integrated executable. Results
add three320x256x6plane bitmaps (~184320bytes), glyphs and Copper while gameplay
remains resident for replay; loading/results headroom is therefore a manual gate.
