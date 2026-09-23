# Level1 two-copy ring transfer

> Reusable lessons from the full Drowned/Level1 optimization round: [retained 68020 lessons](DROWNED_TURBINES_LESSONS.md#retained-68020-optimization-lessons--september-2026). This is a synthesis of evidence, not a new experiment or an override of [current media/acceptance status](CURRENT_STATUS.md).

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

## 2026-09-23 — Level1 two-copy candidate staged; opt-in only

User asked to continue after the bounded Drowned analysis. Level1 transfer now
implemented as SPARKPAW_LEVEL1_TWO_COPY_RING, effective ONLY inside the existing
renderer_level1_unit.c isolation. Shared ring header derives internal
SPARKPAW_RING_TWO_COPY; Drowned uses same retained layout. Stormrail unit keeps
three copies/base512. No default campaign/release flag changed.

Important Level1-specific finding:3392px world-end clamp differs from5120px
Drowned. At camera3072, resident origin2880 maps to physical-96 with base96.
Visible fetch remains safe, but full resident-window membership alone is not
proof of valid Bob destination. Candidate prototypeRectFits therefore checks
signed physical>=0 and physical+width<=1024 before existing residency check.
Exhaustive host tests show rejected rectangles up to actual max64px are outside
visible320px view plus16px margin. Histories store only prior successful draws;
restore addresses stay target-local. This guard also covers splash drawing,
which has no separate screen-cull predicate. Source/generated assembly reviewed.
Do not remove this guard or claim Drowned's full-resident geometry proof applies
unchanged to Level1. Guard has CPU cost; net FPS gain still unmeasured.

Actual C host ASan/UBSan: real storm-front3392x208,6592frames/layout, all3073camera
positions forward/back, wrap, teleports, CPU initial/column/dynamic rectangles,
clipping, mutable canonical strips,16/24/32/64px overlapping Bobs, restore
history, active-target immutability and memory guards, exact AGA fetch phase.
DMA is modeled, no native raster proof. First naive all-resident actor test
failed at world end; explicit guard and offscreen-rejection proof resolve it.
Reference/candidate transfer counts differ slightly because synthetic invisible
Bobs are now rejected. No world/gameplay/enemy removal is performed.

Drowned actual kernel regression10048frames/layout still passes unchanged.
TOD counter tests now exercise both Drowned/Level1 regions; save/music ownership,
reset/wrap/pause behavior and campaign Level1 asset/isolation tests pass.
Shared header included in Make dependencies; level1-two-copy build/test targets.

Four native builds compile. A plain matches captured current campaign baseline
(not a claim of alpha.8 byte parity). B plain adds44bytes for layout/bounds.
Seven non-Level1 plain units including Stormrail, dispatcher, main, game and
audio are byte-identical A/B. Stormrail cadence unit also identical. Native
allocation calls verify two1024x208x4 bitmaps versus1536:106496byte payload
saving. CPU column/rectangle loops store two destinations instead of three;
this is not a measured whole-frame FPS gain. Retained ordinary Drowned rebuild
still b78af227b85ffa7faa1e1ed8af637b98e6b01e74da6b3ef6f6ce4762f03fd738.

Diagnostic reuse: existing music-safe read-only TOD counter, with Level1 X bins
512,1024,1536,2048,2560,3072,3328 and unambiguous Level1 log header/variant/ID.
No CIA profiler. Equal seeded direct-start paths; completion held for LMB log,
so these diagnostics never transition to Stormrail. Plain controls retain the
normal campaign. Title/ready, normal completion/transition and hardware remain
separate acceptance boundaries. Observer overhead unmeasured, no exact deadline
or runtime ownership counters. No new production cadence claim.

Active manual pair:dist/Level1-Ring-{A,B}-020-HD/Level1-Test; each59runtime assets
and59compiled references checked, asset sets identical.68alpha.8 release files
byte-identical. Host/native details:build/level1-two-copy/{builds.json,
source-hashes.json,native-audit.json,host-ring-proof.txt,drowned-regression.txt}.

A-plain: plain; 305692bytes; SHA256 e80b9c41de04bca7a519b9afaca05fdf9297684f70c6b98454e97c145795c470

A-cadence: l1ring_49e244338f68fb793341_A; 306788bytes; SHA256 24e1221a28a872b9f858520a4d55bea313209a2f5ad6390efe34b43a2d13668a

B-plain: plain; 305736bytes; SHA256 0175c25a8534422c922506415756e54871bcc1010399a495506c02ef08e568c4

B-cadence: l1ring_49e244338f68fb793341_B; 306828bytes; SHA256 53779275a92815e2b9a3fcc8cb7f51ced7e04cc2b61e4421e6bb46262a1aa6b7

Pending user pass: same68020/2MBChip+8MBFast/PAL50/noJIT scope; A then B,
60..90seconds comparable water/enemy/shot workload with reversal and death.
Especially B right boundary/Core, reverse movement near end. LMB press/release,
frozen image expected, wait15seconds then reset/stop. Keep both logs and report
image/music plus subjective cadence. No030gate claimed; user explicitly keeps
this020 investigation. Candidate not promoted before manual evidence.

Temporary housekeeping exception: user has not answered whether FS-UAE stopped
after Busy2. Its completed log is preserved, but do not move its potentially
mounted drawer. Superseded Drowned-Busy2-020-HD remains intact in dist for now;
only new Level1 pair is active. Archive Busy2 plus .uaem byte-identically after
stopped confirmation. pgrep unavailable in sandbox; no emulator launched.
No commit,push or release. Alpha.8 remains official. Interlude optimization is
deferred at user's request, not inferred from Drowned performance evidence.
