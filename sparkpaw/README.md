# Sparkpaw: The Stormstone Quest

Milestone 2A of an original Commodore Amiga 1200 AGA action platformer by
MrDig Productions.

Current release: `0.5.0-alpha.1`. Roadmap checkpoint: accepted Phase 5F.4 with
ADF optimization Stage B in progress; Phase 6 has not started.

This is a deliberately small but real engine test. It validates the risky
parts before broader enemy variety, music and level progression are added: a
native AGA dual-playfield display, two independently
hardware-scrolled five-screen world layers, a fluid 15-colour hardware-sprite
player, joystick input, solid platform collision, plasma projectiles and a
bounded clockwork-beetle vertical slice.

## Target

- Commodore Amiga 1200 or compatible AGA Amiga
- Motorola 68020
- 2 MB Chip RAM; Fast RAM recommended
- PAL 320x256 at a fixed 50 Hz
- Two stable 8-colour AGA playfields with true quarter-speed rear parallax
- 48x48 Sparkpaw poses made from three attached sprite pairs, with 24-bit AGA colour
- C with VBCC; small reproducible Python asset and release tools

## Controls

- Joystick port 2: left/right to run, up to jump and fire to shoot
- Hold down to crouch; down plus left/right performs a slower crouch-walk
- Press fire while crouching or crouch-walking to shoot from a dedicated low pose
- Keyboard: `A`/`D` move, `W` jumps, `S` crouches and space shoots
- Reset the Amiga or emulator to leave this engine milestone

Each separate fire press launches a fast blue/cyan plasma pulse from
Sparkpaw's right-hand gauntlet. Up to six pulses can remain in flight, so the
weapon responds to rapid tapping. Four guaranteed and up to two optional low
clockwork beetles patrol safe authored zones; their exact X positions and
48/96/192 movement speeds vary on each complete test replay. Their independent
walk cadence follows the selected speed. Standing and
airborne shots deliberately pass over
them: crouch and fire twice to destroy each one through a hit reaction and
four-stage destruction sequence. Contact with an active beetle now removes one
of six internal half-heart health units, applies brief knockback/input lock and
grants one second of invulnerability. Dedicated standing and crawl-height
hurt art is present; the eventual game-over presentation remains a focused
later step. A full-width fixed HUD band across the bottom now shows the
existing six health units as three full, half or empty hearts and reserves
separate framed panels for the active life counter and diamonds. Original
player-hurt, enemy-hit, enemy-death and accepted-jump effects share a prioritized Paula
gameplay channel;
the existing rapid plasma sound remains independently available. Destroyed
beetles receive a five-to-ten-second cooldown and can respawn indefinitely only
after their complete patrol zone is safely outside the camera. Returning through
earlier level areas therefore creates fresh encounters. Reaching the far-right
world edge temporarily resets the player, camera, projectiles, collectibles and
enemy encounter state in memory without reloading resident level assets. This
right-edge replay stands in for the later `LEVEL_COMPLETE -> next level` flow.
Mouse exit is disabled because clean Workbench
restoration remains a separate technical milestone; reset the Amiga or
emulator to leave the current build.

Phase 5C.3 places two guaranteed Clockwork Storm Striders—one on a raised
platform and one on the floor—with one optional third encounter in the level
data. They patrol independently at randomized speeds using an eight-frame
rigid mechanical gait. At each authored patrol extremum they stop briefly in a
planted frontal turn pose, then resume from walk frame zero in the opposite
direction. Their 64x64 four-plane Bobs share one packed cache and retain the
accepted camera culling and synchronized background restore/draw pipeline.
This paragraph describes the original 5C.3 checkpoint; current Striders also
have the later accepted contact, ranged, hurt, destruction and respawn work.
Their cool
navy/violet/blue armour and cyan energy identity deliberately avoid the warm
orange palette shared by Sparkpaw and the beetles.

Phase 5D adds one deliberately authored traversal proof to the required raised-
platform Strider. At the platform's right launch zone it stops for two cyan
compression stages, follows one fixed ballistic arc to the adjacent lower
floor, lands in a planted pose and recovers before adopting the destination
patrol surface. That floor route stops at the next low-platform face so the
64px body does not pass through it, while extending left beneath the clear
underside of its original platform and stopping before the next column. The link
and trajectory are test-level data, so later level art and geometry can replace
them without redesigning the AI.
Runtime slots 0..17
retain their accepted/reserved meanings; traversal poses are appended in slots
18..23. Complete jump state survives camera-slot parking. The first FS-UAE
observation exposed a destination overlap; follow-up tests accepted the corrected
jump, low-platform turn and patrol beneath the original platform. Real-hardware
review remains open.

Phase 5E.1 replaces coordinate ownership with stable authored surface IDs while
preserving the accepted 5D behaviour. Enemy spawns now reference only their
starting surface; traversal links connect source and destination surface IDs,
and a Strider persists its current surface through camera-slot parking. Runtime
patrol bounds remain cached on the actor for unchanged movement and culling.
This data/AI refactor added no return jump, new route, art or renderer work and
was accepted in focused FS-UAE regression.

Phase 5E.2 adds exactly one explicit return link. After the floor Strider turns
left before the low platform, it reaches a second two-stage compression zone,
uses a steeper fixed arc around the raised platform's right face, lands back on
top and recovers before resuming its raised patrol. The existing traversal poses
are reused. This proves links in both world directions without offscreen
simulation, general pathfinding, new geometry or renderer changes. The first run
exposed that the completed downward link ID
was not cleared at recovery, which blocked all subsequent links; recovery now
releases that ID while preserving the destination surface and patrol state.
The corrected two-way loop was accepted from MrDig's supplied FS-UAE HD video;
ADF parity and real-hardware verification remain open.

Phase 5E.3 keeps persistent Striders moving logically while camera parking has
released their bounded runtime/Bob slot. Each parked encounter receives exactly
one ordinary world-space AI/physics update per game frame, but performs no Bob,
restore, cache or displayed Chip-RAM work. When its current route approaches the
camera, the active pool restores that complete position, surface, direction and
traversal phase. Beetle parking remains unchanged. This adds no spawn-at-camera
shortcut. MrDig's follow-up FS-UAE test accepted this behaviour.

Phase 5E.4 adds narrow failure rules without changing geometry: both extreme
landing-foot probes must
have authored support and body clearance before telegraph begins. A blocked
destination reverses the Strider on its source surface. A missed landing or
96-frame flight timeout restores the stored launch position/source surface,
shows planted recovery and resumes away from the failed link. Successful links
retain the accepted visual path.

The first 5E.4 HD/FS-UAE review found that the downward arc crosses destination
height one update just before its landing window. Failure detection now waits
until the Strider has actually passed that window in its direction of travel;
the independent 96-frame cap remains. MrDig's corrected HD/FS-UAE retest
accepted the restored down, lower-floor patrol and return loop. No ADF-specific
or real-hardware result was supplied.

Phase 5E.5 gives Strider 2 its first authored traversal. It starts on the
existing high platform at x=848..991, approaches right, telegraphs and jumps
across the adjacent 80px open-air gap to the lower x=1072..1199 platform, then
returns left. The rejected first layout put the link at the remote left edge;
the slowest existing randomized patrol needed almost seven seconds to reach it
and looked stuck in a six-second recording. The revised loop stays in the same
camera scene as Sparkpaw's approach. It changes only stable surfaces, spawn
ownership and link data; the continuous player floor, foreground art, collision
asset, renderer and combat remain unchanged. MrDig's follow-up HD/FS-UAE
recording accepted the complete repeated loop in
both directions, including both landings and patrol recovery. No ADF-specific
or real-hardware result was supplied.

Phase 5F.1 adds Strider body contact without adding an attack state. Its fixed
logical contact box covers x+11..52 and y+7..61 inside the 64px cell, excluding
transparent side margins and bottom rows. Overlap reuses Sparkpaw's accepted
half-heart damage, directional knockback, low-ceiling hurt selection,
invulnerability blink, life-loss and level-reset path. Strider walk, endpoint
turn and traversal state continue normally; shooting, Strider hurt/death,
renderer work and reserved slots 9..17 remain untouched. MrDig's broad
HD/FS-UAE contact test accepted 5F.1: low/lying approaches,
ordinary walking contact and jumping contact all damage Sparkpaw correctly.
No ADF-specific or real-hardware result was supplied.

Phase 5F.2 adds one camera-aware Strider ranged attack. A grounded Strider that
is fully visible, not turning/traversing and facing Sparkpaw at a horizontal
distance of 48..208px may stop for a 24-frame cyan charge in runtime slot 9,
show slot 10 for release and fire a slower hostile plasma pulse. Six player
projectile slots and two hostile slots are separately reserved, so enemy fire
cannot steal Sparkpaw's rapid-fire capacity. Both use the existing packed
patterns and synchronized line-253 Bob pass. Hostile contact is consumed and
reuses Sparkpaw's accepted damage path. No attack starts offscreen; a pending
shot is discarded if camera parking occurs. Slots 11..17 were still reserved
at this checkpoint and no new sound/Paula ownership was added. MrDig's
HD/FS-UAE recording accepted the
functional ranged core: repeated shots,
damage/invulnerability and continued routes work. The presentation is not final.
The hostile pulse still shares Sparkpaw's cyan identity, it has no sound, and
the derived attack poses lack a clearly readable gun or gauntlet. Phase 5F.2A
will polish only slots 9/10 with an integrated storm gauntlet/muzzle, give the
pulse a distinct hostile colour identity and add one short original shot sound
with explicit Paula priority. No ADF-specific or real-hardware result exists.

The first 5F.2A procedural violet overlay was rejected because it appeared to
come from the Strider's belly. It has been replaced by a genuine premium attack
source: the accepted body is preserved and only the forward claw/forearm becomes
an integrated storm arm cannon. Slots 9/10 derive from that one source; mirrored
cells and projectile origins use its white muzzle. Hostile plasma keeps the
accepted 16x9 mask/impact animation but now maps to hot orange/red with a white
core. This avoids confusion with an existing violet parallax light that the
supplied recording made look like persistent shot residue; the mark showed no
projectile animation and correctly remained after reload because it is level
art. The original 0.16-second electrical thump plays on
Paula channel 1 at priority 7: player hurt at 9 can interrupt it, while it can
replace ordinary effects at priority 6 or below. Gameplay timing is unchanged.
MrDig accepted the authored cannon, muzzle and orange/red projectile in the HD
FS-UAE review. The first sound was functional but too light; its source is now a
0.20-second heavier bass-body thump with a metallic crack and short electrical
tail. Paula ownership, priority 7 and attack timing are unchanged.
The subsequent review exposed genuine orange shot residue after a runtime
reset. Projectile reset now snapshots and restores Bob rectangles for all eight
pool entries, including hostile slots 6..7; player fire is explicitly limited
to slots 0..5. This lifecycle fix does not alter the line-253 pass or draw order.
MrDig's focused HD/FS-UAE retest accepted the heavier discharge and confirmed
that loose hostile shots no longer remain. Phase 5F.2A is accepted; no
real-hardware verification exists.

Lethal projectiles now select a dedicated original enemy-death cue instead of
the ordinary hit-pop. The shared beetle/Strider sound is a compact mechanical
drop with a shell break and metallic tail, inspired only in weight and brevity
by the supplied ThunderCats level-one gameplay reference. It does not copy its
sample. Paula channel 1 priority is 8: player hurt (9) may interrupt it, while
it replaces Strider fire (7), ordinary enemy hit (6) and lower effects. MrDig
accepted the resulting cue in supplied testing.

The HUD also shows the current attempt stock. A new test run starts at `x3`;
each zero-health reset steps through `x2` and `x1`. Until the dedicated
game-over state is implemented, losing the third attempt starts a fresh `x3`
test cycle. Reaching the right edge replays the test level while preserving the
current life stock and diamond count; final level-completion persistence will
be defined with real progression.
The HUD is modular rather than a table of complete life/health combinations:
one static base, compact health and lives patch atlases, and two presentation
buffers keep updates tear-free. Only a stale dynamic panel is copied with the
Blitter when its value changes; gameplay never CPU-composites displayed Chip
RAM. Twenty fixed diamonds form short trails and original arcs across the
five-screen test level. One fixed HUD-matched silhouette and a gentle
two-pixel hover animate their camera-culled Bobs without rotation. Picking one
up removes its Bob, plays a short arcade collect effect and increments the
compact two-digit HUD counter. The fiftieth pickup changes `49` to `00` and
awards one life. The display supports `x1` through `x9`; at `x9` the counter
remains at 49 rather than silently consuming unrewarded pickups.

## Build

Install a project-local VBCC/NDK toolchain under `.toolchain/` as described in
the repository's [build guide](../docs/BUILDING.md). The directory is
intentionally ignored by Git and must not depend on the ChipSnake or Futsal
toolchains. Install the host Python requirements from the repository root with
`python3 -m pip install -r requirements-dev.txt`. Then, from this directory:

```sh
make
```

This regenerates planar runtime assets and builds the native executable
`sparkpaw`. Run `make release` to rebuild all test packages:

- `dist/Sparkpaw-0.5.0-alpha.1.lha`
- `dist/Sparkpaw-0.5.0-alpha.1.zip`
- `dist/Sparkpaw-0.5.0-alpha.1.adf`
- `dist/Sparkpaw-0.5.0-alpha.1-Source.zip`

`tools/make_release.py` owns the SemVer prerelease value. Each release removes
older `Sparkpaw-*` artifacts from `dist/` before writing one consistently named
ADF/LHA/ZIP/source set, preventing stale milestone files from being mistaken
for the current test build.

The DOS1/FFS ADF contains `S/startup-sequence` and boots directly. Its gameplay
data reconstructs to the same bytes as HD, but Stage B deliberately uses an
ADF-only executable plus `storm-front.spr1` in place of `storm-front.spbm`.
ZIP/LHA retain the ordinary executable, loose SPBM files and existing loader.

Run `make adf-report` for the ADF-only Stage A storage measurement. It writes
ignored JSON and Markdown reports under `build/adf-report/` with raw sizes,
CRC32, host-verified zlib-9/LZMA-9 proxy sizes and projected DOS1/FFS blocks.
This command does not alter the executable, runtime assets, HD ZIP/LHA layout or
Amiga loader. The proxy codecs measure opportunity only; neither is selected as
the eventual ADF codec.

Stage B first packed `storm-front.spbm` with the project-owned SPR1 byte-run
format; MrDig reported that supplied ADF works correctly. The same accepted
path now covers `storm-rear.spbm`, whose ADF was also accepted, plus the current
in-review Strider cache. The ADF executable streams all three directly into their final
allocated bitplanes using a 512-byte input buffer and validates raw size plus
CRC32; it never allocates a second complete foreground copy. Release validation
extracts both packed files and compares their host-decoded bytes with the
canonical SPBMs. Test the ADF from a cold boot through title, `LOADING`,
`CHARGING` and all five gameplay screens. Foreground and rear parity are
accepted; now exercise both Striders through walk, turn, shoot, hit, traversal
and death/respawn. A corrupt/truncated stream must fail loading rather than
enter gameplay.

Run `make bench` to build the isolated `sparkpaw-renderbench`. This small
program validates the dual-playfield foundation before it is allowed back
into the game; see `docs/RENDERBENCH.txt`.

## Source layout

- `src/main.c`: startup, cleanup, explicit application states and the
  raster-phased top-level loop
- `src/title.c` / `src/title.h`: direct six-plane AGA title/loading presenter
  with ordinary (non-EHB) 64-colour palettes, black border blanking, smooth
  VBlank-synchronised 24-bit palette fades, Chip RAM Copper lists and complete
  screen switches while DOS remains live

The startup presentation leaves a black PAL display briefly in place for
scandoubler lock, then keeps the fully faded title visible before loading.
While files are read the loading screen says `LOADING`; once disk I/O is done
it switches to `CHARGING` during the silent sprite, Bob, bitplane and Copper
preparation phase. This second phase remains visible for at least two seconds,
including on accelerated systems. Both screens share one 64-colour palette.
- `src/renderer.c` / `src/renderer.h`: gameplay display, Copper construction,
  six-channel hardware-sprite player, fixed bottom HUD Copper split, packed
  render caches and Bob rendering behind an explicit renderer API
- `src/game.c` / `src/game.h`: gameplay initialization and update ordering,
  frame progression and camera state
- `src/platform_amiga.c` / `src/platform_amiga.h`: graphics-library lifetime,
  custom-chip takeover/restore, raster reads and the required Blitter wait;
  Copper construction and concrete rendering commands remain in `renderer.c`
- `src/enemies.c` / `src/enemies.h`: fixed enemy pool, typed patrol AI, Strider
  walk/turn state, hit detection and damage state; runtime slots retain an
  explicit link to their spawn record
- `src/level_data.c` / `src/level_data.h`: compact typed enemy spawn records
  containing safe position ranges, stable authored patrol-surface IDs, initial
  direction and persistence policy; four beetle encounters
  are required and two are optional. Two required and one optional Strider
  record share the same bounded runtime-slot activation model. One explicit
  Phase 5D traversal link records its launch zone, source/destination surface
  IDs, landing zone and fixed ballistic parameters separately from the enemy AI
- `src/projectiles.c` / `src/projectiles.h`: projectile pool, spawn, movement,
  impact state and hit dispatch; packed plasma rendering remains with the
  renderer-sensitive code in `renderer.c`
- `src/collision.c` / `src/collision.h`: collision-map loading and solid-point,
  horizontal-span and vertical-span tile queries shared by gameplay modules
- `src/player.c` / `src/player.h`: player state, joystick input, shooting,
  movement/physics, the accepted 50-frame baseline and eight appended standing
  and crouched hurt poses; hardware
  sprite preparation and Copper updates remain in `renderer.c`
- `src/assets.c` / `src/assets.h`: SPBM loading, validation, gameplay-asset
  lifetime and cleanup plus a separate early-title lifetime; packed
  hardware-sprite and Bob cache preparation remains in `renderer.c`
- `src/audio.c` / `src/audio.h`: energy-shot, player-hurt, enemy-hit/death and
  jump sample loading, Paula channel 0 rapid-shot playback, prioritized channel
  1 gameplay effects
  and explicit hardware-active lifecycle control; channels 2-3 remain reserved
  for a future music layout
- `tools/generate_runtime_assets.py`: creates wide planar playfields, source
  sprite/enemy planes and masks, packed Bob caches and the tile collision map
- `tools/generate_sparkpaw_sfx.py`: regenerates the Paula-ready raw samples
- `tools/make_release.py`: creates the HD archives, source archive and ADF
- `assets/runtime/`: compact data loaded by the Amiga executable
- `assets/concept/`: full-resolution concept art and AGA preview conversions
- `assets/sprites/`: prototype animation art and named frame metadata
- `assets/enemies/`: native-resolution enemy art, preview and frame metadata
  (including the historical eighteen-frame upright Strider palette proof, the
  current 28-frame runtime contract and preserved concept/scale review sources)
- `assets/sfx/previews/`: WAV previews for later milestones
- `sfx/raw/`: signed 8-bit mono Paula-ready samples; the current build uses the
  energy-shot, player-hurt, enemy-hit and jump samples and reserves the others
  for later steps

## What to test

Walk through all five screen widths, jump onto and off every platform and
reverse direction frequently. Verify that the foreground follows the camera
while the mountain/tower layer moves at one quarter speed. Compare HD and ADF
behaviour. Useful reports include the exact location and whether the issue
concerns sprite flicker, tearing, collision, camera movement, parallax,
controls or display colours. Combine camera movement with rapid fire and watch
specifically for a one-frame mixed-scroll tear under both quiet and heavy
Blitter workloads.

Check especially whether Sparkpaw retains the same apparent size and foot
position while running, jumping, crouching, shooting and performing the idle
pose. Frames 0-49 use one anatomical scale per authored family
and stable shared anchors. It includes an eight-stage grounded run, a
scale-locked four-stage jump, three-stage landing, six-stage momentum-based
direction change, twelve-stage idle performance and dedicated grounded,
airborne and crouched firing poses. Frames 50-53 append contact impact,
backward slide, airborne recoil and recovery without renumbering the accepted
baseline. Frames 54-57 provide a separate 24-pixel-high crawl-recoil family so
contact recoil respects the low crouch clearance beneath a platform.
Crouching changes posture without enlarging
the character. More in-between poses can be added without changing the DMA
renderer.

For Phase 5C.3, verify a raised-platform Strider early in the level and a long-
route floor Strider farther right; a third authored Strider is optional per
level selection. Their eight-frame gait should remain mechanical and stable at
all randomized speeds. The frontal pose may appear only when direction changes
at an authored patrol endpoint—never between walk frames. Walk them on and off
screen, trigger life-loss/right-edge resets and watch for stale 64x64 Bob
pixels. They must not yet damage Sparkpaw or absorb shots. Beetle behaviour
must remain unchanged.

For Phase 5D, follow the raised-platform Strider until it reaches the right
launch zone. Verify two distinct compression holds, a continuous cyan-signalled
arc that remains wholly visible, an exact lower-floor landing, planted recovery
and only then resumed grounded patrol. Confirm that it now turns before the low
platform at x=496 instead of walking through it, then walks left beneath its
original x=320..415 platform and turns before the x=288 column. Move the camera
away during telegraph, flight and recovery, then return: each phase must resume
rather than reset or turn into a walk frame. Recheck life-loss and right-edge
level replay for stale 64x64 pixels. This phase still adds no Strider combat or
generic navigation.

For the Phase 5E.1 regression, repeat that complete accepted route and a life-
loss/right-edge reset. Behaviour must be identical: one downward jump only,
then the same safe floor patrol. Also revisit both required Striders after they
leave the camera; neither may reset to a wrong surface or trigger the jump twice.

For Phase 5E.2, remain near the first Strider after its downward jump. It must
turn at the low platform, travel left, telegraph again at x=440..444, clear the
right side of its original platform, land on top and recover. It should then
patrol left, turn, and eventually repeat the downward link. Watch especially for
platform intersection, a duplicated jump trigger, stale Bobs and incorrect
surface state after moving the camera away and returning.

For Phase 5E.3, leave the first Strider behind during each of these states:
ground patrol, compression, flight and recovery. Wait several seconds, return,
and verify it continued rather than freezing or restarting. Approach the route
from both sides and watch for natural entry, stale Bobs, double-speed movement
at the park/unpark boundary and a hidden actor inside the visible viewport.

For Phase 5E.4, first confirm several ordinary down/up loops remain visually
identical. Current geometry intentionally does not trigger failure. Regression
must show no refusal to launch, teleport or extra turn at either valid link.
Blocked/missed branches are host-contract coverage until a later authored test
surface deliberately exercises them.

For Phase 5E.5, approach Strider 2 around the later pair of raised platforms.
It should approach the right edge of the high platform, show the same two-stage
compression, clear the visible gap and land planted on the lower right platform.
After that patrol it should telegraph and jump left across the same gap, repeating
the loop. It must not snap back, touch the floor, intersect either platform edge
or disturb Strider 1's accepted loop.

For Phase 5F.1, touch each Strider during ordinary patrol, an endpoint turn and
both sides of a landing/recovery. Each accepted contact removes one half-heart,
plays the existing hurt sound, knocks Sparkpaw away from the Strider centre and
starts the accepted blink; continued overlap during blinking must not deal more
damage. Confirm crouched contact stays low under clearance and that losing all
health follows the existing life/reset flow. Neither Strider may freeze, reset
its route or select slots 9..17 after contact.

For Phase 5F.2, stand 48..208px in front of a grounded, fully visible Strider.
It should stop, hold a clearly readable cyan charge, release one slower plasma
pulse and resume its prior patrol direction after a cooldown. Verify the pulse
hits walls, damages Sparkpaw once, respects blinking and cannot reduce the six
player-shot capacity. Approach from behind, leave the camera during charge and
watch traversal/turn states: no unseen shot, mid-flight attack, stale projectile
or route reset may occur.

For the 5F.2A polish retest, also confirm that the arm cannon reads as integrated
at native resolution, the orange/red pulse differs from Sparkpaw's cyan shot
and from the static violet parallax lights, both facings emit from the white
muzzle, and the short electrical thump is
audible without masking or interrupting player hurt.
This checklist passed in HD/FS-UAE, including the later reset-residue retest.

For Phase 5F.3, hit either grounded Strider with standing, airborne and crouched
plasma. Each accepted hit should consume the shot and show the complete
slots-11..17 mechanical recoil for 14 frames; rapid shots during that reaction
must not stack damage or restart it. After two damaging hits the three-HP
Strider remains alive at one HP and resumes the same patrol/turn or ranged
behaviour. A hit during a grounded charge may cancel that charge. Shots during
slots 18..23 traversal must consume the shot and remove HP without interrupting
or renumbering the accepted route. The projectile impact/sound is the immediate
feedback; no delayed slots-11..17 recoil may play after landing. Also compare
slots 9/10 directly with walk and hurt:
head, torso, legs, grounding and colour balance must remain identical, with
only the forward forearm replaced by the arm cannon. Death, teardown and
respawn are deliberately absent in this test.

For the muzzle-alignment retest, slot 9's charge, slot 10's white muzzle and the
first hostile-pulse frame must share local row 36 in both facings. The pulse
must not appear above the cannon; speed, cadence and collision remain unchanged.

For Phase 5F.4, deliver the third damaging shot while the Strider is grounded.
It should stop contact/ranged behaviour and play appended slots 24..27: core
fracture, collapse, burst and sparse grounded debris. After the debris clears,
leave the complete authored starting surface outside the camera for roughly
five to ten seconds, then return and verify a fresh three-HP Strider follows its
route without stale pixels. Also deliver the third hit during compression and
flight: traversal must stop immediately and the burst must begin at the current
world position. Recheck both required Striders and beetle respawn. The HD
build is testable; the unoptimized single ADF is temporarily over capacity.
All Strider frames, including destruction, must retain the indigo/violet/cyan/
white identity. The accepted walk/idle indexed pixels are the visual master;
death fragments reuse those exact material pixels. Sparkpaw-orange pens 2 and
3 are forbidden, and violet may never outnumber steel/charcoal in any frame.

The Milestone 2A beetle art is a 32x24, nine-frame, three-plane masked Bob.
Four to six level instances share one packed art cache and retain independent
HP and animation state. A bounded four-slot runtime pool activates them near
the camera and parks their state when safely distant. Their fixed height keeps
ordinary standing and airborne shots above them, while the domed steel/violet
shell, round cyan lens and jointed legs follow the
gameplay concept at native AGA resolution. Enemy and plasma restore/draw passes
use synchronized Blitter DMA with standard-copy and cookie-cut minterms; the
68020 no longer composites their planar rows byte by byte in Chip RAM. Verify
that all selected beetles walk smoothly between their patrol limits, mirror cleanly
when turning, remain grounded, ignore standing and airborne fire, react to the
first crouch-shot and play all four destruction stages after the second.
Stress the renderer with several simultaneous plasma pulses and visible
beetles, and watch for residue where enemies and projectiles overlap.
On each beetle's second hit and each Strider's third hit, confirm the ordinary
hit-pop is replaced by one short heavier death cue. Test grounded and traversal
Strider deaths, rapid fire and simultaneous player damage; player hurt must win
priority and no death cue may repeat during the destruction frames or respawn.
