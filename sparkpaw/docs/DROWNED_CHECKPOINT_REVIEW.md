# Drowned checkpoint — concept and implementation review, 2026-09-19

## 2026-09-19 — Approved beacon + sound v2 integrated; 020 test pending

User approved sound v2 and requested integration with the approved beacon.
Current active drawer: sparkpaw/dist/Drowned-Level-020-HD/Drowned-Test.
Native3520px joined candidate includes checkpoint at2320,152, fixed-pivot
8-frame48x48 marker, amber->cyan lamp and approved5286-byte period322 sound.
Body/pennant derived from imagegen parts-v1; backdrop excluded by explicit
silhouette, one rigid offline pivot, existing foreground16pens. No runtime
rotation/alpha blending. Own9216-byte Fast planar atlas, no new Chip stage;
existing610-byte stage reused; sound adds5286 Chip bytes plus allocator overhead.
Actual prepared free memory/cadence and animation/audio feel await user log/play.

Grounded touch triggers once without reward/refill. Life loss respawns2320,161,
camera2176, full normal health,75ticks protection. Boat resets2400/waits;
opened/opening gate retained, score/collected diamonds/enemy reward flags retained,
old Bob restore histories retained. Fresh attempt clears checkpoint/gate.
Crab approach patrol shortened2192..2288, spawn2240..2256 to reserve safe marker.
Joined camera end-lock3072 fix and120-byte per-region cadence counters included;
not a claimed FPS optimization. Existing water batching/pontoon lookup retained.

Native68020 compile passes (only existing main/ready_ui warnings). Actual C
host tests with sanitizers pass:19600activation cases in each mode, regional
counts, joined/legacy camera, actual game reset plus real player/mechanism/
enemy/projectile/collectible modules; boat-middle-boat, four upper jumps,
11spawns/3families and bounded frames. Renderer/audio native feel not host-proven.
Native frame sheet inspected; atlas decoder parity and approved raw byte parity.

Staging verified70assets/51compiled literalrefs. All68official alpha.8 release
files byte-identical. Previous played drawer including log archived byte-exact
at dist/older-builds/Drowned-Level-020-H-old-181929. New authoritative candidate
proof: build/drowned-joined/proof-checkpoint.json (proof-joined.json historical).
No emulator launched; no release, commit or push. Ask user to activate, die on
ferry, check saved lamp/respawn/boat/progress and save log once with left mouse;
image intentionally freezes, wait15sec then reset. Regional FPS evidence pending.

Historical preparation notes below are superseded by the integration status above.


Priority: checkpoint before the ferry, and locate the reported late-ferry drops.
Current dist remains the played connected build. No checkpoint art or sound has
been integrated and the prepared checkpoint module is not linked yet.

## Latest evidence

Verified played executable against proof-joined.json; complete log preserved as
Unassigned-Drowned-joined-49-25fps.log/TXT.7100intervals,108two-field,0three-plus,
49.25FPS;0ownership violations.461864Chip free/largest460512. User repeatedly
lost lives around the ferry, did not clear ending, reports noticeable first
late-ferry encounter drops. Whole-level aggregate cannot confirm basin cadence.

Source discovery: updateCamera inherited Level1's playerX>=3072 end-lock.
In joined3520px world, this threshold is inside the final ferry section and
requests camera3200 even while the player is at3072. Joined-only correction
keeps ordinary centered/clamped camera; other builds retain original behavior.
Actual-function test confirms camera2928 at player3072 versus legacy final lock.
This is a camera bug, not proven cause of FPS misses.

Minimal diagnostic regions prepared: land<1376, precision<2128, approach<2400,
ferry_first<2800, ferry_last<3200, shore. Intervals attributed to previous
playerX, with totals/fields/missed/three-plus/max per region. No additional timer
reads or frame-time file I/O.120bytes counters plus bounded update work; observer
cost nonzero. Regional totals tested. Stale pontoon log bounds corrected.

## Visual proposal

assets/concept/drowned-checkpoint-beacon-v1.png is an imagegen concept board,
NOT an animation sheet. Weatherproof steel/copper marker, mechanical pennant,
amber standby and mint/cyan saved lamp. Use a compact40x48 target with a32px
footprint; precise native dimensions to prove in the art pass. No collision or
barrier behavior. Foot seated at floor200; marker aroundx2320, safe before2400.

Review is required before new art enters runtime, per user's established
concept-first request. Board's middle pose is only motion intent: native art
must have a single fixed hinge and rigid pennant across all frames (the concept
middle pose shifts its hinge). Keep base/column locked, no scaling or bobbing
of the entire object. Discard concept glow/background; palette-owned crisp
pixels only. Six-to-eight activation poses in about32ticks; stable saved state
with restrained optional lamp animation, no expensive broad glow.

Sound audition: assets/audio/checkpoint-v1/checkpoint.wav (6828-byte signed
8-bit raw counterpart, Paula period322, approximately0.62sec, peak104).
Mechanical catch then three rising electronic notes; synthesized deterministically
by tools/build_checkpoint_sound.py. No voice in this proposal. Audition only;
not yet added to audio IDs, loader, manifest or event priorities.

## Gameplay contract

- First grounded pass through x2320..2351 at floor198..200 activates once.
  No extra life, score, diamond or immediate health refill reward.
- Life loss after activation respawns player at2320,161 with the existing
  full-health reset semantics and one life consumed. Camera must initialize
  to the spawn's clamped centered target immediately, not pan fromx0.
- Boat returns to2400 heading right, still waits for boarding. Clear projectiles
  and splash; reset encounter movement/attack states. Checkpoint remains saved.
- Preserve collected diamonds/score and enemy score-awarded flags, already
  supported by existing resident reset helpers. Keep the opened gate state.
  Fresh attempt clears checkpoint and gate. Game over stays terminal.
- Reserve safe arrival: shorten last Crab patrol from2192..2368 to2192..2288
  and move its spawn2256..2288 to2240..2256 during integration. No Spillwing
  attack should target the respawn area immediately; test whole swoop bounds.
- Preserve both inactive target histories while clearing old Bobs and moving
  camera; never simply discard drawn flags. Test deaths during activation,
  over water, on upper ledges, after collected diamonds and repeated deaths.

Prepared src/drowned_checkpoint.c/.h owns only active flag and bounded32tick
activation; it is not yet wired into game/renderer. Actual C tests cover19600
activation cases, one-shot event, animation saturation, life persistence and
fresh reset. Tests also cover regional counters and actual camera in joined
and legacy modes. End-to-end checkpoint respawn remains to implement and prove.

## Next step after concept approval

Create consistent native poses + short animated review, then integrate the
checkpoint through game reset, renderer canonical patch ownership and audio
contracts. Target roughly15KB checkpoint frame cache +6.8KB sound in Chip;
measure actual preparation peak and runtime allocation. Retain all existing
water/pontoon optimizations. Build one connected020 test with regional cadence
and checkpoint, preserve current drawer/log intact, then user playtest.

## 2026-09-19 — Checkpoint beacon approved, sound revised

User approves the beacon concept. User finds v1 sound too similar to1-up and
agrees to latch+soft hum+one confirmation tone, no voice. Generated audition
assets/audio/checkpoint-v2/checkpoint.wav/raw via build_checkpoint_sound_v2.py:
~0.48sec,5286bytes,period322,peak104, WAV/raw byte parity verified. V1 preserved.
Existing1-up generator uses four rising notes659/784/988/1319Hz plus high tail;
v2 uses inharmonic click,147Hz hum,one fixed880Hz ping, no note sequence.
Sound subjective review pending; no claim of listened verification.
Beacon direction approved; native fixed-pivot poses/animation and actual
checkpoint game/reset/render/audio integration remain to complete. Dist remains
played joined build; prepared camera/regional changes not yet staged.

## 2026-09-19 — Checkpoint run1 log preserved; regional diagnostic bug

Preserved testresults/Unassigned-Drowned-checkpoint-regional-run1.log/TXT from
old-182507 drawer (first checkpoint, before no-Core fix), verified executable
against proof-checkpoint-first.json and complete post_run footer.4557intervals,
110two-field,0three-plus,48.82FPS,0ownership violations; checkpoint1request/1start.
User reports recurring drops. Chip456576free/largest455224, Fast5186632free.
REGIONAL SPLIT INVALID: rendererDiagnosticUpdateEntry memsets diagnosticCurrent
before drownedCadenceRecord reads its playerX, assigning all samples toland.
Fix next: retain previousplayerX independently and test full entry lifecycle.
Current no-Core stagedbuild has same bug; user doing another run, do not swap
build while playing. Whole-run cadence valid; cannot localize ferry fromthislog.
No runtime change made in this analysis.0.43FPS below prior49.25 with different
workload is not controlled regression evidence. See sidecar for exactprovenance.

## 2026-09-19 — Regional v2 full run: first ferry half is hotspot

User reached end; unexpected falling1-up. Evidence preserved as
Unassigned-Drowned-regions-v2-full-run.log/TXT; exact current executable verified,
complete footer, region_attribution=2; regional sums agree with global.
6334intervals:83two-field,0three-plus,49.35FPS,0ownership violations.
land49.47/14misses; precision49.38/6; approach49.80/2;
ferry_first47.82/34of748(4.55%); ferry_last49.43/27of2348(1.15%);
shore50.00/0of920. Firsthalf now measured investigation priority; bothferry
sections61/83misses. No family CPU/blitter attribution in minimal log.
Checkpoint1/1sound; extra_life1/1. Inherited Level1 extraLifeShouldReveal still
unguarded in game.c: needs Drowned gameplay/render exclusion; Core alreadyhidden.
No changes staged this analysis (user asked log first). Differentworkload and
18.4sec50FPSshore mean49.35vs48.82 does not prove optimization.
