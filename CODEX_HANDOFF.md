# Codex handoff: Amiga game workspace

Last updated: 14 August 2026

## Purpose and source of truth

This repository contains three native Commodore Amiga game projects. Sparkpaw
is the active AGA A1200 project. At the start of every task:

1. read this file completely;
2. read `sparkpaw/README.md`;
3. inspect current source, `git status`, recent commits and tags;
4. use source code and generated manifests as final authority.

The former long-form handoff is preserved verbatim as
`docs/DEVELOPMENT_HISTORY.md`. It contains the complete development
chronology, rejected experiments, supplied-test interpretation and older phase
checkpoints. It is historical evidence, not the current specification. Read
the relevant section before reopening a renderer/palette/timing problem,
reusing a rejected animation technique, investigating a regression with known
symptoms, or changing an accepted contract whose rationale is unclear.

This handoff is intentionally only the compact operational starting point for
continuing efficiently in a fresh task. It is not a running diary. At every
completed feature/phase, and always before a requested commit and push:

1. audit this handoff against the actual source, README, current phase and test
   evidence;
2. update it to contain only the resulting canonical state, invariants, open
   limitations and next step;
3. append the relevant implementation history, decisions, rejected attempts,
   supplied FS-UAE/hardware results and technical rationale to
   `docs/DEVELOPMENT_HISTORY.md`;
4. check that neither document presents an obsolete intermediate state as
   current, then include both documentation updates in the checkpoint commit.

Even when a feature required no rejected experiment, record its accepted
result and verification status in the history. Do not let the compact handoff
grow back into a chronological transcript; move detail to the history instead.

## Repository and workflow

```text
amigagame/
  CODEX_HANDOFF.md                     current canonical handoff
  docs/DEVELOPMENT_HISTORY.md          complete historical engineering log
  sparkpaw/                            active AGA action platformer
  chipsnake/                           finished/releasable prototype
  mrdigs-futsal/                       finished/releasable prototype
  backups/                             ignored local snapshots; never delete
  ACM_PDF/                             ignored Amiga reference manuals
```

- Branch `main` is the accepted shared state.
- Tag `sparkpaw-pre-modularisation` is the accepted four-beetle baseline before
  the source split.
- Keep each game self-contained. Do not add dependencies on sibling projects.
- Preserve ignored backups, recordings, logs, toolchains and test evidence.
- Work in small reviewable steps. Do not combine renderer changes with
  unrelated gameplay or asset changes.
- Always run `make` and `make release` after implementation work.
- MrDig performs authoritative FS-UAE and hardware testing. Never claim either
  unless he supplied that result.
- Use Git as normal recovery. Make a dated local backup as well before risky
  renderer, asset-format, audio/physics or bulk source-movement work.
- Sparkpaw releases use one SemVer prerelease stem for every artifact, currently
  `Sparkpaw-0.6.0-alpha.1`; platform text stays in the requirements rather than
  filenames. `tools/make_release.py` is the single version source. The SemVer
  minor tracks the broad roadmap phase (`0.6.x` for Phase 6), while the exact
  lettered checkpoint (currently Phase 6B.2) belongs in README and packaged
  release notes. Increment the prerelease counter for later meaningful packaged
  checkpoints within that phase. Treat release identity maintenance as part of
  every roadmap step without waiting for MrDig to request it. The release tool
  validates that the SemVer minor matches the numbered roadmap phase and removes
  older `Sparkpaw-*` files from `dist/`, so a successful `make release` leaves
  only one state-aligned artifact set.

Normal Sparkpaw commands, run from `sparkpaw/`:

```sh
make
make release
make bench       # isolated renderer bench only when relevant
```

The target is PAL A1200/68020 with 2 MB Chip RAM and 8 MB Fast RAM as the
accepted production minimum. Extra capacity is not permission for waste:
minimize Chip residency, prefer Fast for CPU-only data/code, release temporary
conversion data promptly and measure both memory and frame-time effects.
The workspace-local VBCC/NDK toolchain lives under ignored `.toolchain/`.

## How to handle supplied test evidence

- A screenshot is evidence for palette, scale, grounding, clipping and stable
  corruption. A short MOV is preferable for cadence, one-frame glitches,
  transitions, scrolling, collision and audio timing.
- For video, inspect metadata and consecutive frames. Do not diagnose a motion
  issue from only the first and last frames.
- Separate observation from hypothesis. Correlate visible frames with exact
  animation IDs, timers, facing and generated indexed assets.
- For Bob corruption, distinguish source pixels, masks/planes, cache indexing,
  restore dimensions, Blitter timing and display timing before changing code.
- Phone recordings can contain refresh beating, scaling and exposure effects.
- Make one narrow fix, rebuild both targets and provide a focused regression
  checklist. Commit only after the result is accepted where visual/runtime
  confirmation is required.

## Sparkpaw: current canonical state

### Display and renderer contracts

- PAL 320x256 at 50 Hz, five-screen 1280x256 resident world.
- AGA dual playfield: four-plane foreground plus three-plane rear; rear scrolls
  at quarter camera speed.
- The player is a 48x48, 15-colour object built from three attached hardware-
  sprite pairs and therefore consumes six sprite DMA channels.
- Copper pointer/palette updates are staged after hardware raster line 100,
  after their current-frame consumption and before the next wrap.
- The Copper switches from the scrolling foreground bitmap to the separate HUD
  bitmap at hardware line 252 (`44 + HUD_TOP`).
- The synchronized Bob restore/draw pass starts at line 253 and finishes before
  the next wrap. Line 300 is historical and must not be restored as the active
  threshold: it left too little PAL time for shifted 64x64 four-plane Bobs.
- Preserve packed planar caches, camera culling and this pass order:
  projectile erase, enemy restore, collectible restore, collectible draw,
  enemy draw, projectile draw, final Blitter wait.
- Background restore uses synchronized Blitter copies; Bob drawing uses the
  standard cookie-cut minterm. Never reintroduce CPU read-modify-write
  compositing in displayed Chip RAM.
- `frontClean` and `frontDisplay` remain separate. A Bob's stored drawn type,
  X and Y describe what must be restored even when a runtime slot changes owner.

The line-253 timing and four-plus-three renderer are user-accepted in FS-UAE.
No real-hardware verification has been supplied. Consult
`docs/DEVELOPMENT_HISTORY.md` before changing Copper allocation,
palette banks, BPLCON state, sprite priority or Bob timing.

### Player contract

- The original accepted player baseline is frames 0..49.
- Eight hurt frames were appended without renumbering it: standing 50..53 and
  crouched 54..57. Current total: 58 frames.
- Preserve family-wide scale, stable foot anchors, mirrored facing and the
  six-channel packed hardware-sprite format.
- Accepted movement includes run, jump/fall, landing, crouch/crouch-walk,
  momentum turn, idle performance, grounded/air/crouched shooting and hurt.
- Crouched fire uses its dedicated low pose and muzzle; standing and airborne
  fire remain separate.
- Three hearts are six half-heart units. Contact damage, knockback, bounded
  input lock, invulnerability blinking, life loss and level restart are active.
- Losing a life restarts the level but preserves collected diamonds. Reaching
  the temporary far-right endpoint also reloads the current test level; it is
  a placeholder for later `LEVEL_COMPLETE -> next level`.

### HUD, collectibles and audio

- The fixed 320x48 bottom HUD uses separate presentation buffers and modular
  health/lives/diamond patches. Never CPU-compose into a displayed buffer.
- Diamonds are masked, camera-culled Bobs with synchronized restore/draw.
  The counter awards a life at 50 and supports lives `x1` through `x9`.
- Up to six plasma projectiles share a packed synchronized Bob pipeline.
- Paula channel 0 owns rapid plasma playback. Prioritized gameplay effects use
  channel 1. A lethal beetle or Strider hit replaces the ordinary hit-pop with
  the short heavy enemy-death cue at priority 8, below player hurt (9) and
  above Strider fire (7). MrDig accepted this cue in supplied testing. Channels
  2-3 remain reserved for future music.

### Enemy and spawn foundation

- Level data contains typed persistent spawn candidates with authored safe X
  ranges, `{left,right,groundY}` surfaces, initial direction and respawn policy.
- Each complete level load selects 4-6 beetles. Strider level data contains two
  required and one optional candidate, but the current runtime gate activates
  only the first two; do not claim three visible/active Striders yet. Positions
  and fixed-point speeds are randomized from 48, 96 and 192.
- A generic four-slot active enemy pool loads candidates near the camera and
  parks state safely outside it. Persistent encounter count is not simultaneous
  Bob count.
- Destroyed respawning encounters receive a 250-500-frame cooldown and respawn
  only while their complete patrol surface is outside the camera. Enemies can
  therefore reappear indefinitely when the player backtracks.
- Enemy extinction no longer completes/reloads the level.

Clockwork beetles:

- 32x24, three-plane masked Bobs; four walk, one hit and four destruction frames.
- Two HP, independent state and speed-coupled cadence.
- Their low hitbox is intentionally reached by crouched shots; ordinary standing
  and airborne shots pass over them.
- Contact damage, hit/death effects and off-camera respawn are active.

Clockwork Storm Striders (accepted through Phase 5F.4):

- Upright 64x64, four-plane masked Bobs in the shared foreground `FRONT16` bank.
- Two guaranteed encounters currently patrol a raised platform and a long floor
  route. One optional third candidate exists in level data but is still blocked
  by the current runtime-readiness gate.
- Slots 0..7 are the accepted rigid mechanical gait. Its non-human stiffness is
  deliberate and was accepted as consistent in FS-UAE.
- Slot 8 is a planted premium frontal turn held for six frames only at an
  authored patrol extremum. Facing changes when the hold ends, then walk restarts
  at frame 0.
- Walk advancement must wrap immediately with `(animFrame + 1) & 7`. Allowing
  frame 7 to increment to 8 leaks the front pose for one frame mid-walk.
- Incidental solid/missing-support safety reversals must not select slot 8.
- Slots 9/10 are the accepted ranged charge/release and slots 11..17 are the
  accepted non-lethal hit reaction. Do not spend them on walk interpolation.
- Slots 18..23 are appended Phase 5D traversal poses: two cyan compression
  stages, flight, descent, planted landing and recovery. The original 0..17
  contract is unchanged.
- Slots 24..27 append the accepted four-stage death without renumbering
  traversal; the packed cache now contains 28 frames.
- The required raised-platform Strider owns one explicit, one-way test-level
  link from its current platform to the adjacent lower floor. Link geometry,
  launch velocity and gravity live in level data rather than AI code.
- Complete telegraph, fixed-point ballistic, landing and recovery state is
  copied into persistent spawn state when a camera slot is parked. Grounded
  patrol resumes on the authored destination only after recovery.
- The destination patrol runs beneath the clear underside of its x=320..415
  start platform and stays between the x=288 column face and x=496 low-platform
  face. Inset-foot bounds prevent the 64px body crossing either solid without
  adding the generic body-aware navigation reserved for Phase 5E.
- Striders deal accepted contact and ranged damage. They have three HP, use
  slots 11..17 for non-lethal hits and start slots 24..27 on a lethal third hit
  in grounded or traversal state. Safe off-camera respawn is active.
- Logical collision cells remain at their authored Y. Drawing uses the accepted
  two-pixel visual offset because source rows 62-63 are transparent.

The final Phase 5C.3 FS-UAE test confirmed stable gait, no mid-walk front-pose
leak and correct endpoint turns for raised and floor Striders. No real-hardware
test exists. Phase 5D was subsequently accepted from MrDig's supplied FS-UAE
tests: the Strider performs the authored jump, turns before the low platform and
then patrols beneath its original raised platform. No real-hardware result has
been supplied for Phase 5D.

## Current roadmap

Completed and accepted:

- Phase 1: incremental source modularisation.
- Phase 2: title/loading/application states.
- Phase 3: player damage, hurt art/audio, HUD, lives and diamonds.
- Phase 4: generic level encounters, variation and safe off-camera respawn.
- Phase 5A/5B: Strider contract, visual direction and AGA colour proof.
- Phase 5C.1: typed multi-enemy level data.
- Phase 5C.2: packed 64x64 Strider Bob integration.
- Phase 5C.3: raised/floor patrol, accepted eight-frame gait and endpoint turn.
- Phase 5D: one authored raised-to-floor jump link with persistent traversal
  state, accepted landing/recovery and safe destination patrol.
- Phase 5E: stable surface/link data, bidirectional low/high traversal,
  offscreen logical persistence, blocked/missed-flight recovery and a repeated
  gap-crossing loop for Strider 2.

### Completed: Phase 5E — broader authored Strider traversal

Generalize the accepted 5D proof without turning it into arbitrary pathfinding:

1. **5E.1 accepted in FS-UAE:** authored patrol surfaces have stable IDs;
   spawns reference a starting surface and traversal links connect source and
   destination surface IDs. The accepted 5D route remained unchanged;
2. **5E.2 accepted in FS-UAE/HD:** one explicit left-travelling
   return link climbs from the safe floor route to the original raised surface,
   proving that the same Strider can traverse links in both world directions;
3. return links, low/high transitions and an explicit open-air gap proof are
   accepted; real water/death-hazard semantics remain later level work;
4. missed/blocked landing and off-camera transition rules are implemented;
5. **5E.3 accepted in FS-UAE:** persistent Striders keep
   one logical world-space update per frame while their runtime/Bob slot is
   camera-parked, then restore the complete state when they approach the camera;
6. **5E.4 accepted in FS-UAE/HD:** launch checks require authored
   landing-foot support and clearance; blocked links reverse on the source
   surface, while missed/timed-out flights recover at their stored launch point
   and move away without changing destination ownership. The first HD recording
   exposed a premature fallback one update before the downward landing window;
   failure now requires passing the window in the travel direction. MrDig's
   corrected-loop retest restored the complete down, lower-floor patrol and
   return behaviour;
7. persistent authored routes now continue independently of the camera without
   camera-edge spawning or reset; unseen attacks remain excluded until combat.

The camera controls rendering, not route ownership. Do not simulate four Bobs
offscreen, but preserve route, position, direction and traversal state. Keep
combat animation/damage and renderer changes separate from this navigation pass.

**Phase 5E.5 accepted in FS-UAE/HD:** Strider 2 starts on the
existing x=848..991 high platform and crosses the adjacent 80px gap rightward
to the x=1072..1199 lower platform, then returns left. The first reviewed layout
used the remote left edge; its slowest randomized patrol took almost seven
seconds to reach the link and looked stuck in the supplied six-second recording.
The revised links are in the same camera scene as Sparkpaw's approach. This is
deliberately level data only: the continuous player floor, collision asset,
foreground art, renderer and combat remain unchanged. MrDig's supplied recording
confirms both crossings, planted landings and stable patrol recovery without
fallback, floor contact or platform intersection. ADF-specific and real-hardware
verification remain open.

### Completed: Phase 5F.1 — Strider contact damage

Accepted in FS-UAE/HD: active Striders contribute a fixed
body-aware contact box at logical offsets x+11..52 and y+7..61. It excludes
transparent 64px-cell margins and rows 62..63, then reuses the accepted player
damage, knockback, invulnerability, life-loss and reset path. Traversal and turn
state continue independently during contact. MrDig confirmed damage during low
posture, walking and jumping approaches. No shooting, enemy hurt/death art,
renderer changes or reserved animation slots are included.

### Accepted: Phase 5F.2A — ranged presentation polish

The Phase 5F.2 functional core is accepted in FS-UAE/HD: a grounded, fully
visible Strider may stop and use slot 9 for a 24-frame charge and slot 10 for
release when Sparkpaw is
48..208px ahead and within 44px vertically. It never begins while turning or
traversing. Six player projectile slots and two hostile slots have separate
ownership but share the accepted line-253 restore/draw pass and packed plasma
patterns. The slower hostile pulse damages Sparkpaw through the accepted damage
path and is consumed on contact. Offscreen cooldown may advance, but an unseen
attack never starts and a pending shot is discarded if parked. Slots 11..17
were still reserved at this checkpoint. No new sound or Paula ownership change
is made.
MrDig's supplied recording confirmed repeated shots, damage/invulnerability and
continued routes. Presentation remains deliberately open: the hostile pulse is
still Sparkpaw-cyan, no shot sound exists, and slots 9/10 do not yet show an
unmistakable weapon. The first 5F.2A procedural violet overlay was rejected in
FS-UAE because it appeared to emerge from the belly. It is removed. A new
premium attack source preserves the full accepted anatomy and replaces only the
forward claw/forearm with an integrated storm arm cannon; slots 9/10 derive from
that single source and the spawn point follows its mirrored white muzzle. Hostile
masks retain the same size but now map to hot orange/red with a white core. This
also distinguishes them from an authored violet parallax storm light that was
mistaken for persistent projectile residue in the supplied recording; it showed
no flight/impact animation and correctly survived runtime reset as level art.
A new original 0.16-second electrical thump uses Paula channel 1 at priority 7,
below player hurt (9) and
above ordinary enemy hit (6). Slots 11..17 and the accepted line-253 projectile/
Bob timing remain unchanged.
MrDig accepted the newly authored arm-cannon visual, mirrored muzzle and
orange/red projectile in FS-UAE/HD. The first shot sound worked but was judged
too light. Its source synthesis is replaced for review by a 0.20-second heavy
bass-body discharge with metallic crack and electrical tail; Paula 1 priority 7
and all gameplay timing remain unchanged.
An HD screenshot then exposed genuine orange hostile-shot residue after a
runtime reset. The reset snapshot loop covered only player slots 0..5 while its
restore loop consumed all eight entries, leaving hostile slots 6..7 with
undefined Bob restore state. Both loops now cover all eight slots. The adjacent
player spawn loop is also constrained to 0..5, so it cannot borrow hostile
slots. MrDig's focused HD/FS-UAE retest accepted the heavier discharge and
confirmed that no loose hostile shots remain after this correction. Native
`make` and `make release` pass. No real-hardware verification exists.

### Accepted: Phase 5F.3 — Strider HP and hit reaction

Striders now absorb player plasma through their accepted body-aware box and
start with three HP. Slots 11..17 form a separately testable stationary
14-frame recoil sequence with two ticks per pose. A hit may interrupt a
grounded ranged charge. Traversal remains dominant and cannot be interrupted;
hits during slots 18..23 remove HP and use the immediate plasma impact/sound
without scheduling a visually late ground recoil. Repeated shots
during the reaction are consumed without stacking damage. At one HP the
Strider remains alive and continues its accepted route, contact and ranged
behaviour; death, teardown and safe respawn remain deliberately deferred.
Slots 18..23 and the renderer are unchanged. Native `make` and `make release`
pass. MrDig's supplied FS-UAE recordings confirmed grounded damage and
traversal-hit registration, then exposed an unnatural delayed recoil plus a
shoot-fragment cutout affecting head/neck pixels. Both are corrected for
focused review. A subsequent supplied recording exposed the inherited charge
and hostile-shot origin five pixels above the corrected muzzle; both now centre
on local row 36 without moving the gun or changing attack timing. No
real-hardware result has been supplied. MrDig accepted the corrected result as
good enough to continue on 14 August 2026.

### Accepted: Phase 5F.4 — Strider death and safe respawn

A third grounded plasma hit now starts four appended destruction frames in
slots 24..27. This preserves the complete accepted 0..23 contract: hit remains
11..17 and traversal remains 18..23. After the 20-frame destruction, the
generic encounter lifecycle applies its existing 250–500-frame cooldown and
only rebuilds the Strider at its authored starting surface while that complete
surface is outside the camera. Contact and ranged behaviour stop while dying.
The third hit is also lethal during traversal: it cancels the active link and
starts the burst at the current world position. Renderer, authored routes and
Bob ordering are unchanged.
Native `make` passes. The raw append grows the Strider SPBM beyond the current
single-ADF capacity, so `make release` now fails with `No Free Blocks`; this is
an accepted temporary packaging limitation pending the separately planned
ADF-only optimization in `sparkpaw/docs/ADF_STORAGE_STRATEGY.md`. Two death-art
attempts were rejected: procedural idle compression had weak silhouettes, and
requantizing a legacy production burst first selected shared-bank orange, then
overcorrected toward violet. Slots 24..27 are now rebuilt only from exact
accepted idle/walk indexed pixels, using a generated master concept solely for
the four breakup beats. Walk is the canonical visual master; no foreign RGB
enters runtime death. Generation rejects orange pens 2/3 and any Strider frame
whose violet pixels exceed its steel/charcoal pixels. Death now matches walk's
measured material balance rather than merely sharing `FRONT16`.
MrDig accepted this walk-master visual basis on 14 August 2026. The supplied
10:00 FS-UAE recording verifies the death lifecycle and stronger destruction
silhouettes, but predates the final indexed colour correction; no final-colour
FS-UAE or real-hardware result is claimed.

### Later phases

- ADF Stage A reports raw/proxy-packed/CRC/projected-FFS sizes. The first Stage
  B proof is also implemented: only the ADF replaces `storm-front.spbm` with a
  7,689-byte SPR1 stream and separate loader, decoding through 512 bytes directly
  into final planes with size/CRC validation. The DOS1 image now builds with
  294 free blocks. HD ZIP/LHA remain on the ordinary executable and loose
  foreground SPBM. MrDig reported that this first ADF works correctly, accepting
  the foreground proof. MrDig then reported the foreground-plus-rear ADF works
  correctly too, accepting rear quarter-speed parallax. The next isolated build
  packs the Strider cache from 143,420 to 87,914 bytes and leaves 589 blocks
  free. MrDig reported this ADF works correctly as well, accepting the packed
  Strider cache. The ADF optimization is therefore at a safe checkpoint before
  generalizing the container or packing another family.
- Phase 6: level/progression work. The resident 2048px/eight-screen repeated-art
  memory experiment is accepted; next build the collision/pacing greybox, then
  checkpoint/progression state, encounters and finally unique art.

### Measured: Phase 6A — 2048px resident-memory experiment

`make phase6-memory` builds an isolated, directly runnable HD drawer under
`build/test/Sparkpaw-Phase6A-2048/` using repeated existing art and collision
across 2048px. Production remains 1280px. Only the experiment
defines `SPARKPAW_WORLD_W=2048`; it writes `phase6-memory.log` after complete
gameplay preparation with before/final free Chip RAM and largest-block values.
The exact host bitmap delta is 270,336 bytes (front source 98,304, rear 73,728,
front display 98,304), plus 672 collision bytes. This corrects the older rough
221 KiB estimate. MrDig ran the unpacked HD test on exactly 2 MB Chip/no Fast,
reached the repeated-art right edge and supplied the generated log. Immediately
after full gameplay preparation it reports 88,136 bytes free and an 86,816-byte
largest block. Releasing the still-visible charging bitmap and two title Copper
lists later returns about 62.8 KiB. This establishes the 2 MB/no-Fast stress
baseline but does not reject the resident design: MrDig subsequently accepted
2 MB Chip plus 8 MB Fast as the minimum and requested a complete optimization
audit before choosing an architecture. Initial inspection identifies roughly
325 KiB of converted player/enemy/collectible source sheets that remain in Chip
after their DMA caches are built. No real-hardware result exists. Do not turn
this build into the greybox, mistake disk compression for resident savings or
choose segmentation before the audit and a newly instrumented test.

Phase 6A.2 implements the first isolated memory-hygiene step without changing
rendering or gameplay: after the final hardware-sprite, beetle, Strider, plasma
and collectible caches and Copper palette are built, the 325,220-byte player/
enemy/collectible conversion sources are released. The 2048px log records Chip
and Fast free/largest values immediately before this release and after complete
preparation. Retest on 2 MB Chip plus 8 MB Fast is required before extrapolating
a longer resident width.

MrDig supplied successful repeated gameplay/right-edge/reset tests on 2 MB Chip
plus 8 MB Fast. Native non-displayable `AllocBitMap` still placed source planes
in Chip, so Phase 6A.2 now allocates CPU-only planar sources explicitly with
`MEMF_ANY`; their masks and planes use Fast when present and fall back to Chip
when it is absent. The final log measures 531,464 bytes Chip free with a
530,408-byte largest block both at the conversion peak and after source release:
there is no longer a narrow Chip loading peak. Fast rises from 6,349,280 during
conversion to 6,674,416 afterward, returning 325,136 bytes. Phase 6A is complete
and accepts 2048px resident as the Phase 6B greybox basis, not as a fixed final
level length. Continue measuring and do not spend the recovered margin loosely.

Next: Phase 6B.1 builds an eight-screen collision/pacing greybox without final
art, extra enemy requirements or renderer changes. Establish route beats and
play feel before deciding whether the finished level stays exactly 2048px.

### Accepted: Phase 6B.1 — eight-screen pacing greybox

Production now uses the accepted 2048x256 resident width. The original first
five screens and all accepted Strider surfaces/links remain unchanged; screens
six through eight append five greybox platform sections and three short columns.
Level data appends four beetle candidates (three required, one optional) and
twelve diamonds, while the simultaneous enemy pool remains four slots. The two
runtime-enabled Striders remain unchanged and the optional third stays gated.
There is still a continuous floor: water collision/death semantics are reserved
for 6B.2 and water presentation remains a separate renderer/asset step. Host
geometry checks, `make`, the 2048 memory regression and `make release` pass;
supplied FS-UAE pacing/geometry review is required.

The first supplied 6B.1 video exposed one non-word-aligned diamond at x=1704:
the former one-word cache could not provide the second source word required by
a shifted 16px Bob, producing persistent split diamond fragments. The diamond
cache now pads every row to two source words, making arbitrary X placement
shift-safe for 210 extra Chip bytes. Retest remains required.

The supplied `2026-08-14 15-46-37.mov` also exposed that the legacy boolean
beetle-shot gate rejected a visibly overlapping airborne shot against a beetle
on a raised platform. Player projectiles now mark crouched or airborne launches
as beetle-capable, but the existing geometric point/hitbox test remains
authoritative. Grounded standing shots therefore still pass over floor beetles;
crouch shots and genuinely overlapping airborne shots hit. MrDig accepted both
focused fixes in supplied FS-UAE testing. No real-hardware result exists.

Next isolated work: create and approve water concept art before implementing
the visual renderer/asset treatment. The mechanical floor interruption,
water/death region and safe life restart are already accepted.

### Accepted: Phase 6B.2 — mechanical water hazard

One authored 80px region at x=1584..1663 removes the continuous floor and
lands on the next low platform. Only those columns permit falling below the
normal 224px gameplay boundary. When the player contact box reaches y=224 in
the region, one life is removed and the accepted in-memory level restart runs;
diamonds already banked in the HUD remain preserved by the existing restart
contract. There is deliberately no water art, splash or new sound yet. Standard
and ADF builds pass, and MrDig accepted the fall/restart in supplied FS-UAE
testing.

MrDig accepted the mechanical result in supplied testing: Sparkpaw falls
through the authored region and the level restarts. No visual water is claimed.
The same review plus supplied real-Amiga ADF and HD observation exposed an
insufficiently isolated HUD/world boundary. The HUD asset contained only two
black separator scanlines; the rejected four-scanline experiment has been
reverted to the accepted two-scanline baseline. The current focused correction
fills those two rows across the full fetched width with non-zero dark HUD pen 1,
because pen 0 is transparent to hardware sprites. Preserve the line-252 display
switch and line-253 Bob pass. MrDig verified this correction in both FS-UAE and
on a real Amiga; the formerly moving/glitchy HUD-boundary pixels are fixed.

Level 1 target: an original Storm Ruins route materially longer than the current
five-screen test and the earlier 35-50-second proposal. Treat the first
2048px/eight-screen build as a resident-memory minimum experiment, then tune
actual length from a playable greybox rather than forcing an unmeasured time.
The supplied ThunderCats level 1 is a pacing/progression reference only. Do not
copy maps, characters, art, music or exact timing. Extra enemy types are not a
requirement for level 1: first obtain variety through route, height, hazards,
breathing space and placement of the accepted beetles and Striders.

## Known limitations and backlog

- No clean Workbench exit; reset the Amiga/emulator to leave gameplay.
- The first mechanical water/death region is accepted, but visual water,
  splash feedback and audio remain pending concept approval.
- Far-right reload is temporary; real level completion/progression is pending.
- Broader route graphs and additional hazard semantics remain future level
  work. Strider contact, ranged combat, hurt, death and respawn are accepted.
- Music is pending; define Paula ownership before integration.
- Game-over presentation and broader checkpoint/progression flow remain pending.
- Stock 68020/2 MB Chip RAM performance and real-hardware timing remain unproven.
- ADF-only compression, asset reconstruction and eventual multidisk options are
  researched; Stage A plus one ADF-only foreground proof exist. Preserve the
  HD path and consult
  `sparkpaw/docs/ADF_STORAGE_STRATEGY.md` before changing asset formats, release
  layout, disk I/O or adding a trackloader.

## Asset and animation rules

- Runtime uses deterministic project-owned SPBM assets. Rows are word aligned;
  masks follow planar bitmap data where applicable.
- Author right-facing animation families and generate exact indexed mirrors.
- Crop transparent padding, use one scale for a complete family and bottom-align
  by opaque foot pixels. Never auto-fit every pose independently.
- Collision dimensions are gameplay data, not transparent image bounds.
- Keep transparency at pen 0 and remap opaque pixels to the established palette.
- Update generator, frame metadata, C selectors, Makefile dependencies, previews
  and documentation together when changing a frame contract.
- Inspect final indexed previews at integer scale before asking for FS-UAE review.
- Record new generated-art prompts in `sparkpaw/docs/IMAGEGEN_PROMPTS.md`.

For rejected Strider gait sheets, renderer benches, palette migrations,
first-approach corruption, prime-restore failure and the reasoning behind the
rigged walk, consult `docs/DEVELOPMENT_HISTORY.md` before retrying an
old technique.

## Other projects

ChipSnake and MrDigs Futsal are finished/releasable prototypes. Their detailed
technical history now lives in `docs/DEVELOPMENT_HISTORY.md` because
Sparkpaw is active. If work switches project, read that project's README and
inspect its source/build rules before editing. Do not apply Sparkpaw's AGA
renderer assumptions to those projects.

## Recommended first prompt in a new Codex task

```text
We are continuing the Amiga game workspace in this repository.

First read CODEX_HANDOFF.md completely, then read sparkpaw/README.md and inspect
the current Sparkpaw source before changing anything. Sparkpaw is the active
AGA A1200 project. Check git status, recent commits and tags first; use
sparkpaw-pre-modularisation as the accepted four-beetle baseline.

Preserve the stable dual-playfield renderer, line-100 Copper-list staging,
accepted 50-frame player baseline plus eight appended hurt frames, and the
six-channel hardware-sprite player. Preserve the line-252 HUD display switch
and synchronized line-253 Blitter Bob pass used by plasma projectiles,
collectibles and the generic four-slot enemy pool. Preserve packed planar
caches, camera culling, Bob restore/draw ordering and the accepted beetle and
Strider animation/collision contracts. Strider runtime slots 0..7 are walk,
slot 8 is the endpoint turn, slots 9/10 are ranged charge/release, slots 11..17
are non-lethal hurt, slots 18..23 are traversal and slots 24..27 are death. Do
not reintroduce CPU read-modify-write compositing in displayed Chip RAM.

Work in small reviewable steps and do not combine renderer changes with
unrelated gameplay or asset changes. Do not modify or delete ignored local
backups or test evidence. Always run make and make release after implementation.
Keep release SemVer, roadmap checkpoint, packaged notes and the sole current
artifact set in dist synchronized as part of every roadmap step without waiting
for me to request it. Target 2 MB Chip plus 8 MB Fast RAM efficiently. Do not
claim FS-UAE or real-hardware verification unless I provide the result.

Current checkpoint: Phase 6B.2 mechanical water fall/restart is accepted. Its
visual water treatment awaits concept approval. The two-line opaque HUD-boundary
correction is accepted after successful FS-UAE and real-Amiga verification.

My next request is: ...
```
