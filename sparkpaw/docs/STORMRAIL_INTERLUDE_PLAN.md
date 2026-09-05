# Stormrail Skimmer interlude plan

> Current status (2026-09-05): The complete interlude, finale, results and campaign continuation are implemented. The current retained drawer is Campaign-Play-HD; no isolated results gate is pending. Dated intermediate statuses and rejected directions below are historical. See [status index](CURRENT_STATUS.md).

Status: active approved direction, 2026-09-01. The first two corridor-based
proofs are rejected. The v3 rear panorama and Skimmer identity remain useful;
continuous ceiling/floor corridors, the former approach geometry and isolated
single-enemy loop do not. Gate 1, the cliff approach and physical jump into the
Skimmer, is accepted as the development baseline after v7 pose restoration.
Gate 2's isolated open-flight control baseline is coherent enough to preserve.

Integration update (2026-09-04): the current candidate preserves Stormrail's
resident `REPLAY LEVEL` and adds a real `BACK TO TITLE` branch. Back to Title
clears the campaign run, shows the large title and existing ready/start screen,
then starts a fresh Level 1. Native acceptance is pending.
Gate 2.5 now has an accepted distance/timeline, bounded-background and
stock-68020 performance baseline before any enemy or hazard work begins.
Campaign progression, release packaging, version and ADF remain unchanged.
Phase 6C.10 / alpha.68 Level-1 replay remains the protected baseline.

Accepted Gate-2.5 baseline: after boarding, the craft accelerates right
out of the unchanged departure, the playfield fades through eight palette
steps while the HUD remains fixed, both existing Copper lists switch under
black, and flight fades in as the craft re-enters from the left. Flight uses a
separate 768px REAR8 route span with 352px fetch overlap. The span progresses
from violet departure mountains through rain-grey/green depth into cyan
waterfall ruins and closes with a 128px overlap-add cloud/terrain join. Its
complete loop uses one stable semantic 3-bit pen encoding; the rejected first
conversion's per-X encoding caused visible vertical seams. Four palette banks
still morph from `stormrailDistance`; the FRONT16 vehicle and HUD banks remain
isolated. Once both flight targets are armed, the approach front and rear
allocations are freed. Automatic FS-UAE/68030 proof crossed distance 32,768
and eleven rear loops with zero unsafe Blits and 1,305,232 Chip bytes free.
Supplied FS-UAE/68030 testing accepts the transition, route art, compact craft
and corrected 13-stage palette table without visible corruption. The table
maps 768 phases to 23 exact 12-bit palettes in about 5.6 KiB readonly/Fast
data. Final low-overhead FS-UAE/68020 evidence records 49.26 FPS over 1,000
intervals: 992 one-field, 3 two-field and 5 three-plus, maximum four fields,
zero ownership violations and 214/214 requested/started shots. The remaining
outliers occur around the one-time handoff/target initialization; steady open
flight presents at 50 Hz. Preserve this CPU budget before adding enemies.

## Current approved open-shooter direction

This section supersedes every later reference in this document to a conduit
corridor, continuous floor/ceiling collision, route forks, `FIRE TO BOARD`,
the three-lock sluice climax or a single recycled enemy loop. Those passages
remain only as rejected design history until the document is fully condensed.

The interlude is a roughly 2.5-minute classic horizontal shoot-'em-up in the
spirit of R-Type, Apidya and Bubble and Squeak's genre change. The v3 Stormrail
mountain/forest/ruin panorama remains visible as the principal environment.
Near-field ruins, waterfalls and bridge fragments may pass as non-colliding
parallax decoration. Only sparse, isolated and clearly telegraphed hazards—
such as a floating rock, broken rail fragment or storm-charged debris—may
temporarily constrain movement. There is no continuous ground after launch,
no tunnel and no route that can be traversed on foot.

### Staged implementation gates

1. **Cliff approach and boarding.** Sparkpaw starts on a short, raised,
   Level-1-material cliff. Its broken right edge is a visible point of no
   return. The Skimmer hovers just beyond the gap within one comfortable jump.
   Walking right falls into the void; descending into a generous visible
   cockpit zone is the only boarding trigger. No Fire teleport, proximity
   trigger or timer is allowed.
2. **Open-flight control slice.** Remove all corridor collision/art and prove
   rapid fire, brisk autoscroll, readable bounds and the unobscured v3 vista.
2.5. **Distance/background foundation.** Decouple deterministic encounter
   distance from the finite Level-1 camera, repeat a bounded rear panorama
   without a pointer jump and sustain a representative 45--60 second empty
   flight. This is an architecture/art gate, not the final route duration.

The first Gate-2.5 single-panorama loop is rejected by supplied FS-UAE/HD
evidence: its join is visibly vertical, approach pixels later re-enter the
foreground ring and the unbounded logical camera eventually crosses signed
16-bit renderer coordinates, after which the Skimmer disappears. The corrected
contract keeps 32-bit encounter distance separate from a bounded 512px flight
presentation phase. The accepted approach must scroll completely left of the
playfield before both inactive flight targets are cleared and its canonical
front bitmap is released; it may never pop out while visible.

Background direction now uses several authored join-compatible regions rather
than one obvious wallpaper loop. Vortex mountains, rain-veiled ruin valley,
waterfall/bridge basin and rail citadel may share calm cloud/mist/forest
connector zones. REAR8 carries broad depth at quarter speed; later sparse,
non-colliding FRONT16 silhouettes provide full-speed parallax. Preserve a calm
central combat band and budget against five small enemies, five player shots
and four hostile shots before adding decorative animation. Rain/cloud polish
must use a few deterministic prebuilt phases or Copper/palette modulation, not
per-pixel CPU work or a large always-active Bob field.
3. **First formation loop.** Add two small enemy types, two deterministic
   flight patterns, complete-formation detection and one diamond-chain reward.
   Use `NEXT_SESSION_STORMRAIL_GATE3_PROMPT.md` as the bounded implementation
   handoff and stop for explicit acceptance after this gate.
4. **Debris-field obstacle vocabulary.** Accepted for now: native Storm Ruins
   masonry, one monotone 48-event choreography, a six-object cap, readable
   destructibility and persistent diamond/health rewards.
5. **Post-debris encounter vocabulary.** Add a small number of Project-X-like
   loop/curl/re-entry patterns after the field, initially reusing Darts and
   Orbs. Gate 5A starts with two flows derived from the local Project-X
   testresult reference and one short persistent-ID free-diamond pattern
   between them. Expand enemy art only after pattern feel and the 68020 budget
   pass.
6. **Stormrail Harrier finale — accepted.** One 80x46 Harrier combines a fixed
   fan with a low-health aimed Hunter Burst before the Storm Ruins gate.
7. **Interlude results, then campaign integration.** First present the finished
   section's statistics without Level 2. Later bank section and campaign scores
   once, retain the post-Level-1 replay snapshot and transition toward Level 2.

The first Gate-7 candidate is staged for 68030 user review. It fixes the
complete-route par at 150 seconds, finalizes one immutable result snapshot only
at Gate-6 `COMPLETE`, reuses the Level-1 presenter exactly and keeps the real
`REPLAY LEVEL` prompt. Fire fades fully black and resets the still-resident
interlude to departure/boarding. Host/native checks pass; visual tally, audio,
input and replay feel are not accepted until the user tests
`dist/Storm-Results1-030-HD`.

Only the current gate is implementation scope. Later gates may be represented
in planning but must not be smuggled into an earlier test build.

### Gate-1 lessons carried forward

- Judge large experiential changes early in a focused user drawer; use internal
  FS-UAE proofs for corruption, masks, logs and obvious visual defects, not as
  a substitute for human motion/art acceptance.
- Keep one direct-start candidate under `dist` during isolated interlude work.
  It bypasses intro and Level 1 only for iteration and never proves campaign
  loading or belongs in a release target.
- Preserve the craft's authored scale and silhouette throughout boarding.
  Apparent resizing between adjacent frames reads as a broken transition.
- Prefer canonical Sparkpaw frames and real cockpit occlusion. Never invent
  anatomy with a few ad-hoc pixels; if a bridge pose is not coherent, restore
  the last clean frame pixel-for-pixel and defer new art.
- Sparkpaw's identity contract covers anatomy and palette roles: fur base and
  shadow, muzzle/inner-ear cream, eyes, scarf, gauntlet, outline, expression and
  proportions must agree with the accepted gameplay/HUD character. Sharing a
  palette bank is insufficient when a new source reverses the orange/shadow
  balance. Correct the source-derived roles without copy-pasting or rescaling a
  HUD portrait.
- Review concept art at exact native AGA size against the actual background.
  Attractive source art can still become muddy, overlarge, too black or too
  similar to terrain after palette reduction.
- Build and accept one interaction gate completely before adding the next.
  Gate 1 stops after boarding; Gate 2 starts with control/scroll/fire and still
  contains no enemies or hazards.
- Preserve supplied MOV/PNG evidence with accepted/rejected status and concise
  sidecars so later sessions do not revive discarded corridor, beetle, vehicle
  or boarding variants.

### Gate-2/2.5 engineering and review lessons

- Keep Stormrail as a mode derived from the protected rolling renderer, with
  explicit ownership of its own rear/front assets, Bobs and hardware sprites.
  Platform and flight code may share the compositor, but must never publish or
  retire each other's live objects.
- Treat the fade-to-black as a real ownership boundary. Scroll the departure
  platform fully out of view, switch both Copper targets only under black, and
  release approach assets only after both flight targets are armed.
- A looping indexed panorama needs both sufficient fetch overlap and one stable
  semantic pen meaning across its complete width. Per-region or per-X palette
  remapping creates visible vertical joins even when source pixels align.
- On 68020, inspect the emitted vbcc assembly whenever a small-looking C loop is
  unexpectedly expensive. Variable modulo and division inside the per-frame,
  per-band palette path compiled to repeated `divsl`/`divs.l` operations and
  was the dominant slowdown, not the parallax DMA, shots or Skimmer Bob.
- Prefer exhaustive build-time generation for a small bounded state space. The
  accepted route maps 768 phases to 23 bit-exact palettes in about 5.6 KiB of
  readonly/Fast data, removing interpolation work without consuming Chip RAM.
  Compare every generated phase with the reference calculation.
- Derive or assert table dimensions at every producer/consumer boundary. The
  first lookup build generated 12 stages while the renderer consumed 13; the
  resulting out-of-bounds read appeared as a corrupted strip above the HUD.
  The generator now asserts exactly 13*8 words for every palette.
- Use the targeted profiler to locate costs and the low-overhead cadence logger
  to judge delivered frame pacing. The profiler perturbs timing; it is evidence
  about attribution, not the final FPS verdict. Document automatic stop/freeze
  behaviour precisely so a mouse click is not mistaken for the stop trigger.
- Gate presentation on 68030 first, then cadence on 68020. Logs prove timing,
  ownership and shot counts; actual emulator frames and user captures prove
  seams, masks, anatomy, scale and feel. Neither evidence class replaces the
  other.
- Native-sized art and clean masks beat repeated pixel repair or naive scaling.
  The compact hull Bob plus attached hardware-pilot sprite can read as one craft
  while preserving facial detail, provided their anchors and mode lifecycle are
  inseparable. Sparkpaw's cockpit art remains provisionally accepted for this
  gate; future polish must start from the Level-1/HUD identity, not revive the
  rejected improvised neck/scarf anatomy.
- The empty slice is now the performance budget, not proof that later content is
  free. Every enemy/formation increment must retain responsive controls, rapid
  fire, zero renderer-ownership violations and near-50 Hz steady flight before
  the next increment is added.

### Debris-art and test-cycle lessons carried forward

- “Top AGA art” is a standing production contract, not a request for merely
  readable placeholders. Preserve approved perspective, connected light and
  shadow planes, material and silhouette through palette-aware native
  underpainting and exact-size pixel cleanup. Reject programmer blobs, flat
  polygon reconstructions, uniform black keylines and mechanical native-pixel
  rotations before they reach a user drawer.
- Concept-resolution transforms may establish a new pose, but every pose must
  be independently reduced and native-cleaned. Judge stable visual mass and
  lighting at 1x; an attractive enlarged contact sheet is insufficient.
- Contact sheets are evidence and can contain layout defects independent of
  the source cells. Check cell rectangles for overlap before presenting them.
  The Debris-2 animation-v1 small row initially overlapped a medium review cell;
  user review caught it and the corrected row no longer overlaps.
- Keep human art and feel review early. Internal FS-UAE work is limited to the
  shortest crash, mask, renderer and contract proof needed before staging; do
  not privately iterate through multiple subjective visual candidates.
- Automatic FS-UAE proofs are exceptional diagnostics, not the normal
  iteration loop. Prefer host contracts plus native compilation, then let
  MrDig judge one focused drawer. Motion, cadence, audio impact and encounter
  feel are user evidence; stop private iterations once that test is requested.
- `dist` belongs to complete, self-contained user-test drawers only. Never run
  internal proofs directly from it or add proof startup sequences, screenshots,
  temporary logs or `.uaem` sidecars. Internal FS-UAE drawers live only below
  `build/fsuae-selftest`; retain exactly one active user-test drawer in `dist`.
- Preserve packaging through the authoritative staging manifest. A direct
  source-tree proof does not show that a user drawer contains every runtime
  asset, while a manifest-parity check does not replace the user's actual
  loading and visual test.

## Enemy, formation and reward plan

### Gate-3 encounter and budget contract (implementation candidate)

- Timeline ownership: formation 0 triggers once at monotone distance 520 and
  formation 1 once at distance 1560. Neither camera phase nor the 768px rear
  wrap can rearm them.
- Pools are compile-time bounded to five enemies, five player shots, four
  hostile shots and four reward diamonds. Failure to find a free slot skips a
  shot; it never overwrites a live object.
- Formation 0 is five one-hit Storm Darts in a staggered fast wave. Formation
  1 is four two-hit Pulse Orbs on a slower crossing curve; each may release at
  most one left-moving pulse at an authored age.
- A member that leaves the left boundary marks its formation incomplete. Only
  reducing every member to zero HP before escape emits the four-diamond chain.
- Gate-3 state and both rolling-target restore histories exist only inside the
  compile-guarded Stormrail mode. Level-1 enemy, projectile and collectible
  pools are neither updated nor rendered during flight.
- CPU/Chip budget: no allocation per encounter or frame, no runtime scaling,
  rotation, division-heavy curve interpolation or per-pixel effects. Enemy
  caches add no new Chip allocation: Gate 3 consumes two already generated
  48x32 masked frames. Worst-case dynamic work is 5 enemy updates, 25
  player-shot overlap checks, 4 hostile updates and 4 reward checks per field,
  plus bounded restore/draw jobs for the same objects.
- Acceptance protects the empty 49.26-FPS/1000-interval baseline, 992
  one-field intervals, zero ownership violations and 214/214 shots. The first
  human gate is visual/function on 68030; only an accepted version proceeds to
  a like-for-like low-overhead 68020 cadence run.

### Gate-3.5 combat-feedback and cadence contract

The supplied corrected Gate-3 FS-UAE/68030 HD drawer reaches and plays the
slice successfully. Its two-formation function gate is accepted; subjective
review finds held fire too fast and requests complete Level-1-like combat
feedback before later hazards or Gate 4.

- A new Fire press emits immediately subject to a four-field minimum cooldown.
  Holding Fire waits eight fields and then repeats every six fields. Deliberate
  tapping can therefore outpace comfortable hold-fire without requiring it.
- Non-lethal enemy hits use the existing enemy-hit cue; lethal hits use only
  the existing enemy-death cue; reward pickup uses the existing collect cue.
  Player contact and hostile pulses use the existing player-hurt cue and its
  higher gameplay-channel priority.
- Every hostile Stormrail pulse uses the exact existing animated Level-1
  Strider projectile cache and its hot red/orange palette roles with white
  core. Cyan remains exclusive to Sparkpaw fire; no second projectile asset or
  runtime recolouring path is introduced.
- Enemy contact uses an inset native collision rectangle, removes one of the
  six half-heart health units, separates the Skimmer slightly left and shares
  the 36-field flight invulnerability/flash window with hostile shots. Enemies
  survive contact and cannot be rammed for free damage.
- The representative slice expands to four formations while retaining exactly
  two enemy types and the 5/5/4 enemy/player-shot/hostile-shot pool caps: Dart
  wave, Orb crossing curve, Dart high/low pincer and one mixed escort pattern.
  A formation starts only when its monotone distance has passed and the prior
  enemy set is empty; panorama wrap cannot trigger or overlap it.
- Zero flight health resets only the focused flight encounter snapshot. It
  cannot consume or rewrite Level-1 campaign state.
- Hazards, new enemy families, clouds, destructible ruins, the complete route,
  results and campaign integration remain later gates.

### Gate-3.6 pickup economy and later finale reservation

The next bounded gate reduces both manual and held fire once more, adds only
authored free-flight diamond lines between the accepted formations and then
measures the complete workload on 68020 before obstacle work begins.

The supplied FS-UAE/68030 Gate-3.6 review accepts all four formations, restart,
audio feedback, collision, compact scoring, deterministic diamonds, the fixed
lower split route and the half-visible enemy hit threshold. Proceed with the
like-for-like low-overhead 68020 cadence gate before adding obstacles or other
Gate-4 content.

The supplied stock-FS-UAE/68020 cadence run then accepts the complete Gate-3
workload visually and records 2,108 consecutive Flight-only intervals at
50.00 FPS: 2,108 one-field, zero two-field or three-plus intervals, maximum one
field, zero ownership violations and 176/176 requested/started player shots.
This exceeds the intended 1,000-interval window because the user saved with
the left mouse button after all four waves while the flight timer still
saturated at 1,000. The sample remains valid and stronger than the accepted
empty baseline; the timer now saturates only at 65,535 so future cadence builds
can stop automatically at tick 1,400. Gate 3 is accepted on 68030 presentation
and 68020 cadence. Stop before Gate 4 until the user explicitly continues.

### Gate 4 debris contract and accepted-for-now checkpoint

The current recovery baseline is Debris5.2, not the earlier 28-event/bouncing
proof described historically below. It is one monotone 48-event timeline over
distance 5800..11330, with asymmetric whole-field choreography and a clean
moving reserve before the 15500 focused endpoint. The six-slot pool admits
each event once; no old event can reopen at the tail. It uses all accepted
large, shard and pillar families, prevents simultaneous large overlap and
avoids repetitive horizontal medium/small traffic through the middle band.
Debris5 remains archived as the fallback.

Destructible carriers flash and reuse existing impact/death cues. The earlier
carrier drops persistent diamond ID 32. The deliberately slower six-hit
carrier drops persistent health ID 33. A native 16x21 red heart restores two
half-heart units, capped at six. A second heart (ID 34) appears between early
waves at the centre of three widely spaced diamonds (IDs 35..37), exactly
filling rather than enlarging the four-slot reward pool. The user accepts the
art, arrangement and current gameplay for now.

The complete low-overhead stock-FS-UAE/68020 run records 49.96 FPS across
2,886 intervals: 2,885 one-field, zero two-field, one three-field, maximum
three, zero ownership violations and 334/334 player shots. This is effectively
equal to Debris2's 49.95 FPS and protects the empty 49.26-FPS baseline. Gate 4
is therefore closed for the present slice; later density or combinations must
be remeasured.

#### Historical Gate 4A--4C evolution

Visual review on 3 September 2026 rejects the first procedural faceted-rock
family before Gate-4A acceptance. Its smooth boulder mass, saturated purple
face, bright fracture lines and pebble-like shards read below the accepted Dart
and Orb quality and do not convincingly belong to the Stormrail ruins. Preserve
it only as rejected provenance; do not use that art direction as the basis for
later debris.

The accepted replacement direction is actual castle-ruin masonry: irregular
pieces of carved lintel/arch, broken slab and pillar, with chipped edges,
restrained cold highlights, rough fracture faces and no decorative holes or
neon cracks. Gate 4B proved that the family reads correctly, but its isolated
four-slot, 17-event presentation was too sparse and its loot moment too easy to
miss.

The 3 September Zynaps reference review establishes the Gate-4C feel target.
Its apparent chaos comes primarily from five to seven persistent large shapes
sharing a leftward stream, not literal random entry from every screen edge.
Different speeds, shallow vertical drift, tumbling orientations, overlap and
new arrivals before old exits create narrow moving pockets. Sparkpaw therefore
uses 28 deterministic `stormrailDistance` events from 6600 through 9300 and a
hard six-obstacle pool. Most pieces enter from the right and travel left at one
of three speeds; selected pieces drift vertically and reflect at the safe top
or bottom limit. The field remains authored and repeatable.

The user's first 68030 play report accepts this density, challenge and movement
as the **Debris 1.0** gameplay reference. It also explicitly leaves the masonry
graphics below final quality. Preserve its exact recovery contract and hashes
in `STORMRAIL_DEBRIS_BASELINES.md`; subsequent work is an art-only A/B until a
new gameplay change is separately approved.

Animation uses only prebuilt native FRONT16 frames: two arch orientations, two
slab orientations, six small-fragment orientations and two pillar orientations.
Objects alternate frames every eight fields. There is no runtime rotation,
scaling, particle system or allocation. The 400x40 source is converted once at
load into bounded mask/bitplane caches (about 12 KiB Chip total).

Selected pieces remain destructible and flash for four fields on a valid hit;
solid pieces produce the impact sound but never flash. The unmistakable large
loot arches take six hits, switch permanently to a cracked arch for their final
three HP and release existing pickup IDs 12 or 13 when destroyed. Ordinary
debris no longer splits, because the dense field already supplies pressure and
extra fragments would muddy collision readability. The unique 1UP remains
deferred.

Player shots may connect once half of the large silhouette has entered the
viewport (screen X=296), matching the accepted enemy-entry fairness rule. The
small one-hit shards become hittable when fully visible. This gives an alert
player time to retreat and break the rock without permitting blind off-screen
fire to erase it.

- Pool capacity is six obstacle Bobs. An event is not consumed until a slot is
  available, so a saturated field cannot silently skip authored entries.
- Large and small obstacle contact each cost one health unit and reuse the
  accepted hurt/invulnerability contract. A colliding obstacle is consumed so
  it cannot drain multiple health units after invulnerability expires.
- The large rock awards 20 points once per interlude attempt and each shard 5;
  persistent award bits prevent life-restart farming, matching enemy awards.
- Reuse existing enemy-hit and enemy-death audio. No new Paula channel or
  sample allocation belongs to this gate.
- Move the fourth existing free diamond line behind/through the rock encounter
  rather than increasing the current 32-ID pickup budget.
- Runtime art is one native FRONT16 SPBM with five 48x40 large frames, six
  16x16 fragment frames and two 32x40 pillar frames.
- Render ownership is Flight-only and target-local, with restore-before-draw
  history for all six slots. Gate 3's five enemies, five player shots, four
  hostile shots and four rewards remain unchanged.

Gate first on visual/readability, claustrophobic manoeuvring and the six-hit
loot moment in FS-UAE/68030, then measure stock-68020 cadence. Do not extend the
route, add clouds, 1UP or finale content until this slice is accepted.

Debris 2.0 now passes that gate. User review accepts its 68030 art and feel; the
complete stock-FS-UAE/68020 run is also reported visually smooth and records
49.95 FPS across 2,299 intervals, 2,298 one-field, zero two-field, one
three-plus (maximum three) and zero ownership violations. This becomes the
current performance reference for the next route increment.

### Gate 4D Storm Ruins blue dust-depth field

The supplied Project-X footage demonstrates how a continuously populated field
of sub-sprite specks and short streaks strengthens speed and depth without
becoming gameplay. Adapt that principle as blue Storm Ruins dust, grit and tiny
stone splinters rather than literal stars. The field may be clearly present as
a whole, while every individual mark stays below shots, diamonds and enemy
silhouettes in brightness and visual priority. This is a visual accent only:
no collision, damage, pickups, score, audio, physics response or interaction.

- Use a fixed pool of eight independent 16x3 accent slots: one-pixel specks
  and broken two-to-five-pixel grit marks. Every slot crosses continuously from
  the right edge to the left edge; no shared cluster or formation is allowed.
- Give every independent particle its own constant speed and starting phase.
  It may change slowly between prebuilt neutral-white, pale-blue and rare warm
  amber variants to suggest changing sunlight incidence; never
  compute colours per pixel or blink rapidly. Spread particles across the whole
  flight area. Draw them before gameplay Bobs so enemies, debris, shots,
  rewards and the Skimmer always remain readable.
- Use prebuilt native indexed/masked cells. No
  runtime scaling, rotation, alpha, per-pixel effects or allocation.
- Spawn/recycle deterministically from monotone `stormrailDistance`, with
  authored Y positions, speed class and quiet gaps. Presentation-camera wrap
  must not rearm the sequence, and life restart must restore it predictably.
- Keep the combined field continuously and substantially populated throughout
  flight, with staggered offscreen recycle gaps so it never pulses as one
  obvious burst. It must make the world feel faster, never conceal hostile
  shots, diamonds, destructible flashes or safe openings.
- Give both targets explicit Flight-only restore history and clear them at the
  fade/ownership boundary. Platform Level 1 must never see their state.
- Gate one meaningful visual version early on 68030. If accepted, repeat the
  low-overhead 68020 cadence test against the current complete reference of
  49.96 FPS and zero ownership violations. Reject the layer if its tiny visual
  gain consumes material cadence or creates mask trails.

Gate 4D is accepted. The first wide-cell proof was rejected because its marks
read as clusters. The independent eight-slot version is accepted in supplied
FS-UAE/68030 review after moving one redundant mid-height slot to y=18 and
retaining all other safe positions. A later y=211 experiment visibly corrupted
the playfield/HUD presentation and is preserved only as a rejected drawer; it
must not be restored. The final cached palette cycle is neutral-white,
pale-blue and rare amber (front pens 9, 6 and 3), explained as changing
sunlight on the same grit.

The accepted low-overhead stock-FS-UAE/68020 run covers 6,036 presentation
intervals: 6,035 one-field, zero two-field, one three-field (maximum three),
49.98 FPS, zero ownership violations and 432/432 requested/started player
shots. This longer run is cadence-neutral against the protected 49.96-FPS
Heart1 baseline. No extra gameplay pool, collision, score, audio or per-frame
allocation was added.

Gate 4D was implemented only after the route timing, Debris5.2 field and Gate
5A pacing increment below had been accepted. The following bullets remain the
protected combat/pickup contract beneath the visual accent.

- A new Fire press remains immediate but has a six-field minimum interval.
  Holding Fire waits ten fields, then repeats every nine fields. Five player
  shots remain the hard pool cap.
- Free-flight diamonds are deterministic authored records keyed to monotone
  `stormrailDistance`, never random screen spawns. Gate 3.6 begins with four
  short lines: horizontal introduction, shallow diagonal, high/low choice and
  one risk line before a formation. At most four free diamonds may be active.
- Formation 3's high/low Dart split keeps its two lower 32px Bobs within
  Y=128..152 and Y=152..176. Do not restore the rejected Y=176 base plus
  triangle offset: it crossed the Y=208 playfield boundary and made the lowest
  Dart disappear and reappear while the renderer correctly rejected the
  unsafe frames.
- Player shots may hit a 48px enemy from screen X=296 onward, when at least
  half its silhouette has entered the 320px viewport. Before that, shots pass
  through without damage. This prevents held autofire from erasing a wave
  wholly offscreen without imposing a conspicuous full-entry immunity.
- Match Level 1's compact four-digit score economy: award 20 points per Storm
  Dart, 40 per tougher firing Pulse Orb and 5 per diamond. The 3--5 diamond
  chain is the current formation-completion reward; do not add the rejected
  250-point direct bonus. If later playtesting shows that a separate clear
  bonus is needed, start at 20 points and give it a unique section-local award
  ID so life restart cannot farm score. Gate 3 uses a persistent score-award
  bit per formation member, matching Level 1's `spawnState.scoreAwarded`; the
  encounter may respawn after life loss but that member cannot score twice.
  The same unique-award contract applies
  before later rocks, turrets or the Harrier may score. A complete `REPLAY
  INTERLUDE` may expose awards again only because it rolls the discarded
  section score and campaign snapshot back together.
- Every authored pickup and formation reward owns one immutable section-local
  ID. Once collected, that ID stays consumed across contact, health loss and
  focused flight restart. Score, the campaign diamond remainder and any life
  award are therefore applied exactly once per interlude attempt.
- Each contact or hostile hit removes one of the six health units. At zero
  health, Stormrail consumes exactly one life, restores full health and
  restarts at the beginning of open flight immediately after the accepted
  boarding/fade handoff. The short cliff approach is not replayed for every
  life. Collected pickup IDs remain consumed across this restart, matching
  Level 1's no-farming life-loss rule; formations and later hazards reset from
  the flight timeline. The final no-lives/game-over campaign decision remains
  a later integration gate; the focused candidate retains the existing
  prototype fallback after the last displayed life.
- The complete roughly 2.5-minute interlude may contain about 70--100 available
  diamonds across formation rewards, calm free-flight lines and later authored
  obstacle-risk routes. A normal successful run should collect roughly 40--70;
  a near-complete skilled run may approach the full total. This makes diamond
  pursuit a substantial secondary objective rather than occasional decoration.
- Route count does not expand runtime Bob pressure. At most four free/reward
  diamonds are active on screen; inactive authored records are compact
  distance/position/ID data plus collected bits. Longer chains stream through
  the same bounded four-object pool.
- Crossing 50 uses the existing campaign rule: subtract 50 from the diamond
  remainder and award one life up to x9. Stormrail may award at most two lives
  in one accepted run, tracked by an explicit section-local award counter.
  Diamonds collected after that cap still grant their normal score and may
  contribute to the retained campaign remainder, but cannot produce a third
  interlude life. Depending on the inherited post-Level-1 remainder and player
  completion, the intended yield is roughly one to two lives.
- `REPLAY INTERLUDE` restores the immutable post-Level-1 snapshot, including
  its exact lives and diamond remainder, then clears Stormrail pickup IDs for a
  fresh run. A life earned in the discarded run is rolled back with its score
  and diamonds, so replay cannot bank or farm it. Continuing banks the accepted
  interlude result exactly once.
- A later obstacle gate may place deterministic diamond lines around one
  destructible rock and one telegraphed falling-debris family. It must not hide
  required pickups behind opaque clouds or random unavoidable geometry.
- The single Stormrail `1UP` is deferred until that obstacle grammar exists.
  Preferred contract: breaking one authored heavy ruin object reveals the
  existing `1UP`, which then falls vertically through a risky lane for a short
  bounded window. Its unique ID follows the same no-farming rules.

Reserve the final 20--25 seconds of the authored route for a fixed-camera
arrival encounter. At the finale latch, autoscroll and the bounded presentation
camera stop; `stormrailDistance` remains nondecreasing and fixed at the authored
finale coordinate while a local encounter timer drives the Harrier, two gate
anchors/turrets, hostile shots and poort state. No ordinary formation or pickup
may spawn after the latch. When the large enemy and both weak points are clear,
enemy fire is retired, the gate opens, the Skimmer recentres and controlled
rightward acceleration resumes through the gate. Results/campaign loading and
Level 2 remain later gates; this reservation authorizes no boss or castle art
yet.

Target four small airborne families. Prefer strong silhouettes and pattern
reuse over many large sprite sheets:

1. **Storm Dart:** smallest and fastest; straight lines, V formations,
   sinus chains and diagonal dives.
2. **Arc Wasp:** agile looping attacker; curls, crossing arcs and pincers.
3. **Pulse Orb:** slower formation anchor; takes more punishment and fires one
   readable energy pulse.
4. **Rail Hunter:** larger escort/leader; multiple hits and a short two- or
   three-shot spread.

Target 8--10 data-driven patterns across those families: V, sine chain,
diagonal descent, loop, high/low pincer, staggered wall, escorted leader,
outward fan/rejoin, fast dive and mixed obstacle weave. Begin performance work
with at most five small enemies, five player shots and four hostile shots
simultaneously; raise the enemy cap toward six only after measured 68020 proof.

Individual kills award score. Destroying every member of a formation before it
escapes awards a separate formation bonus and spawns a short chain of 3--5
canonical campaign diamonds along the formation's recent path. Authored diamond
arcs may also mark optional risk/reward lines around sparse hazards. Nearby
diamonds may receive only a very small attraction assist; there is no screenwide
magnet. The existing diamond HUD, five-point value and 50-diamond extra-life
contract remain unchanged. A full interlude should contain roughly 70--100
available diamonds, with an ordinary successful run collecting about 40--70
and a near-complete skilled run approaching the authored total.

The finale is one large **Stormrail Harrier**, roughly 96x56 native pixels or
an equivalent measured multipart Bob. It uses three readable phases: aimed and
spread fire; a charged horizontal lightning lance with strong visual/audio
warning and upper/lower escape; then faster movement plus one small escort
formation. It is a short moving climax, not a stationary bullet sponge.

## Decision summary

After collecting the Level-1 Lightning Core, Sparkpaw reaches the accepted
Level-1 results screen. That screen eventually offers two explicit choices:

```text
LEVEL 1 RESULTS -> REPLAY LEVEL     -> resident alpha.68 Level-1 reset
                -> CONTINUE JOURNEY -> load Stormrail interlude
```

The interlude lasts about 2.5 minutes in total. It begins with only roughly
5--10 seconds of ordinary Sparkpaw platform control in one compact Stormrail
departure composition that deliberately retains the ground, materials and
atmosphere of Level 1. The open-cockpit Skimmer is already visible nearby;
Sparkpaw walks to the cliff and physically jumps down into its generous visible
cockpit zone. This is a brief,
playable connective beat, not another platform section. Almost the complete
remaining duration is a fast, forced horizontal auto-scrolling shooter inspired
primarily by Bubble and Squeak's sudden shooter change. ThunderCats' vehicle
level contributes the readable hero-to-vehicle transition and clearly visible
rider, but not its comparatively sparse corridor or exact visual design.

The route remains one continuous horizontal scrolling journey. It has no room
grid, vertical level screens, free map exploration or backward camera travel.
Occasional formation/hazard combinations may make a readable upper or lower
flight lane safer before opening again; they must not form continuous terrain
corridors. Parallax remains an essential Sparkpaw presentation feature.

## Narrative purpose

The first Lightning Core reactivates an ancient Stormkeeper maintenance
vehicle at a small departure station beyond the completed Level-1 journey. The
Stormrail Skimmer was built to carry caretakers through the conduits connecting
the remote weather stations.

When Sparkpaw launches, Grand Archivolt detects an unauthorized transfer and
reverses the route. Rainwater, clouds, loose machinery and archive drones are
drawn toward the Sky Archive. The interlude therefore shows rather than merely
states that the complete weather network has been reversed.

Sparkpaw eventually opens a jammed main sluice and is swept toward the next
station. The Skimmer arrives damaged at the Drowned Turbines, establishing the
Rain Core and Level 2 without turning the interlude into a detached bonus game.

Suggested short Archivolt interruption:

> UNAUTHORISED WEATHER TRANSFER. PLEASE REMAIN PERFECTLY STILL.

This line is optional and must not interrupt control once the shooter begins.

## Vehicle identity

The Stormrail Skimmer is a small open maintenance vehicle: part railbike, part
conduit flyer. Sparkpaw remains plainly visible in its open cockpit.

Visual identity:

- ancient Stormkeeper engineering rather than a generic science-fiction jet;
- compact horizontal silhouette with a readable nose, cockpit and rear rail
  clamp;
- navy/violet body, pale steel structure, warm copper fittings and cyan
  Lightning-Core energy;
- folded rail gear while parked and energized conductor vanes after launch;
- Sparkpaw's head, scarf/ears and gauntlet remain recognizable at native size;
- damage may bend a vane or expose cyan arcing, but must not turn the vehicle
  into unstructured particle noise.

The first native-size proof should test a combined Sparkpaw-plus-Skimmer image
inside one transparent-padded 64x64 attached sprite pair. If that makes either
Sparkpaw or the vehicle unreadable, the isolated shooter display may instead
keep Sparkpaw on channels 0/1 and use a masked vehicle Bob or a second attached
pair. The rejected extra Core pair in Level 1 does not decide this separate
display mode, but any additional sprite DMA must be independently proven.

The first production-renderer proof found the original 112x48 visible Skimmer
too large for fair navigation through the accepted turbine/rail composition.
The cache cell remains 112x48, but the visible vehicle is uniformly reduced to
88x38 inside it; Sparkpaw remains readable. Collision uses a still smaller
60x28 inset body so scarf, antennae, fins and energy glow are never lethal.

## Rejected corridor pacing script (historical record only)

Everything in this section through the later concept gates is retained only to
explain the rejected v1/v2 direction. It is not implementation guidance. The
current open-shooter gates at the top of this document and checkpoints 6D.4+
override its Fire prompt, conduit routes, floods, locks and enemy count.

Target duration is about 7,500 PAL fields including the boarding approach.
Exact timings remain adjustable after the first mechanical playtest.

### 0:00--0:10 — Stormrail Departure

- Start in ordinary platform control with the accepted Sparkpaw movement,
  jump, crouch and shooting language.
- Use one fixed or nearly fixed screen-width composition; add no platform
  challenge, enemy encounter, collectible trail or exploratory detour.
- Retain Level 1's ground height, Storm Ruins material language, cold rear
  atmosphere and warm/cyan accents so the opening initially feels like a direct
  continuation of the completed level.
- Place Sparkpaw only a short walk from the dormant Skimmer. The vehicle, launch
  rail and conduit mouth are visible immediately.
- Lightning-Core energy wakes lamps and rail conductors as Sparkpaw approaches.
- A small `FIRE TO BOARD` prompt appears only within the cockpit trigger.
- Fire starts a short deterministic boarding/latch animation, closes ordinary
  movement input and launches right into the shooter.

### 0:10--0:35 — acceleration and control lesson

- The camera begins forced horizontal movement and never stops completely.
- The player can move the Skimmer freely within safe screen bounds and Fire
  rapid cyan plasma.
- Sparse conduit debris teaches vertical movement and the first destructible
  gate without meaningful punishment.
- Rear scenery immediately establishes faster multi-layer parallax than a
  static tunnel would provide.

### 0:35--1:10 — broken conduit run

- Small index drones enter in simple lines, arcs and paired high/low patterns.
- Foreground conduit ribs and broken rail pieces create speed and near-field
  motion without obscuring collision edges.
- Introduce the first upper/lower fork. Both paths remain viable:
  - upper route: tighter geometry and more shootable targets;
  - lower route: more moving debris and a safer firing lane;
  - neither route is a hidden difficulty trap or permanent branch;
  - both rejoin clearly within about 8--12 seconds.

### 1:10--1:45 — reversed flood

- Clouds, water and loose machinery visibly stream toward the archive.
- A flood front or pressure wave advances from the left, reducing hesitation
  space while leaving a fair recovery margin around the Skimmer.
- The route alternates between open speed sections and short readable squeeze
  gates; it must not become pixel-perfect memorization.
- A second high/low fork lets the player choose between a turbulent lower water
  channel and a narrow upper maintenance rail.

### 1:45--2:15 — Archivolt correction squad

Level 1's clockwork beetles are explicitly excluded from this flying section.
They are grounded patrol enemies and read as an arbitrary placeholder in the
air. The interlude instead starts a dedicated airborne machine family. None
may inherit the beetle's legs, antennae, domed shell or grounded body plan;
even the earliest playable proof must use these new silhouettes.

Use at most three clearly distinct enemy families in the first complete
interlude:

1. **Indexer:** crosses in a straight or shallow wave and fires at most one
   readable shot.
2. **Correction stamp:** marks one horizontal lane before a delayed strike,
   encouraging vertical movement without filling the screen with bullets.
3. **Core grapple:** briefly aligns with Sparkpaw and telegraphs a capture beam;
   destroying it or leaving the lane breaks the attempt.

Formation density and scroll pressure should create intensity. Raw projectile
count should not. The player must retain a readable safe lane at all times.

### 2:15--2:30 — main sluice climax

- Three large illuminated locks appear in sequence while the route narrows.
- Destroying them opens the main sluice; this is a moving set piece rather than
  a stationary boss arena.
- The released flood overtakes the route, damages the Skimmer and carries it
  into the Drowned Turbines transfer pipe.
- When Level 2 exists, transition directly into its arrival/loading flow.
- Before Level 2 exists, show a temporary destination card and allow
  `REPLAY INTERLUDE`; never expose a dead `CONTINUE` option.

## Movement, pressure and route design

Bubble and Squeak is the primary feel reference:

- constant forced rightward progress;
- relatively fast foreground motion;
- frequent but readable enemies and obstacles;
- immediate firing response;
- short escalation beats rather than long empty travel;
- a surprising but complete genre change with its own visual identity.

Sparkpaw-specific safeguards:

- horizontal world/camera progress is monotonic;
- the player moves in both axes only inside the current 320x208 playfield;
- no vertical room transitions, multi-screen map or backtracking;
- forks are short lanes inside the same continuously scrolling world;
- show a fork entrance early enough to choose deliberately;
- use silhouette, lighting and object flow to show where lanes merge;
- never place unavoidable damage at a merge point;
- do not require knowledge of a fork before it first appears;
- recovery after a hit must be possible without scrolling the player into a
  second unavoidable obstacle.

The initial scroll curve should accelerate over the first 15 shooter seconds,
hold a brisk base speed, briefly lift during the reversed flood and peak only
during the final sluice. Exact pixels-per-field values require native playtests.

## Controls and failure contract

First proof:

- joystick/keyboard left, right, up and down move the Skimmer;
- Fire/Space fires rapid plasma;
- no inertia-heavy steering;
- no secondary weapon, smart bomb, charge shot or power-up economy yet;
- the vehicle faces right throughout ordinary play;
- contact or hostile fire removes health and grants a short invulnerability
  window;
- zero health restarts the interlude from its own start/checkpoint snapshot;
- interlude failure does not consume or rewrite the completed Level-1 result in
  the first implementation.

Whether a later polished campaign consumes lives on interlude failure remains a
separate balance decision. The first genre-change proof should teach its new
controls without erasing Level-1 progress.

## Score and campaign-state contract

Every ordinary level and every replayable interlude owns a separate run score.
The campaign also retains a cumulative score across completed sections.

Required values:

```text
campaign_base_score   score already banked before the current section
section_run_score     score earned in the current level/interlude attempt
campaign_total_score  campaign_base_score + accepted section result
```

### Level 1

- Level-1 HUD and result tally show the Level-1 run score.
- Completing Level 1 banks that result into the campaign score exactly once.
- `REPLAY LEVEL` retains alpha.68 semantics: it starts a completely fresh
  Level-1 run and therefore resets both the Level-1 run score and the campaign
  state derived from that not-yet-accepted branch.
- `CONTINUE JOURNEY` creates an immutable post-Level-1 progression snapshot
  containing the accepted Level-1 result, campaign score, lives and Lightning
  Core state.

### Stormrail interlude

- The interlude starts with `campaign_base_score` equal to the score banked
  after Level 1.
- Its HUD shows the interlude run score as the primary live score; a total need
  not be squeezed into the existing gameplay HUD.
- Its first isolated results implementation reuses the current Level-1 screen
  exactly: `ENEMIES x20`, `DIAMONDS x5`, `TIME x10` and total `SCORE`, with the
  same art, layout, tally, input and audio. Separate `INTERLUDE SCORE` and
  `CAMPAIGN SCORE` presentation is deferred to campaign integration.
- This isolated version retains the existing `REPLAY LEVEL` prompt. Confirming
  it fades fully to black and starts a fresh resident Stormrail interlude at
  departure/boarding, without Level 1, another asset load or Workbench. It
  resets every local run counter, pickup/award bit, lives/health, projectile/
  input history and finale state. A distinct `REPLAY INTERLUDE` label remains
  future campaign polish.
- `REPLAY INTERLUDE` restores the immutable post-Level-1 snapshot:
  - campaign base returns to the exact score accepted after Level 1;
  - interlude run score returns to zero;
  - Lightning Core remains recovered;
  - Level-1 local diamonds, enemies, timer and secret are not replayed or
    re-evaluated;
  - lives and other campaign values return to their post-Level-1 snapshot,
    unless later balance explicitly decides that interlude retries consume a
    persistent life.
- Continuing to Level 2 banks the accepted interlude score exactly once.

### Later result screens

Starting with the first result screen after Level 1, every result presenter
must include:

- current level/interlude score breakdown;
- current section total;
- cumulative campaign score after accepting that section;
- `REPLAY <SECTION>` and `CONTINUE` only when both actions lead to real states.

Host tests must cover double-confirm, replay and load failure so a section score
can never be banked twice.

## Progression and lifetime architecture

Do not fold the interlude into the existing Level-1 reset.

Planned boundaries:

- `LevelId`: at minimum `LEVEL_1`, `INTERLUDE_1` and later `LEVEL_2`;
- `ResultDecision`: `REPLAY_CURRENT` or `CONTINUE_NEXT`;
- `CampaignState`: banked score, remaining lives, recovered Cores and later
  progression flags;
- `SectionSnapshot`: immutable state from immediately before the current
  replayable section;
- section-local timer, awards, collectibles, enemies and secrets;
- separate cross-section loader and resident current-section replay path.

Protected alpha.68 behavior remains the `LEVEL_1 + REPLAY_CURRENT` branch.
`CONTINUE_NEXT` must unload Level-1-specific bitmaps/caches/data under a black or
controlled loading display before it loads the interlude. Only measured common
assets may remain resident.

If interlude loading fails, retain the accepted post-Level-1 snapshot and show a
safe retry/return decision. Never silently fall back to replaying or rebanking
Level 1.

## Display and renderer direction

The interlude may use an isolated shooter display mode, but it must retain the
recognizable Sparkpaw presentation:

- PAL 320x256;
- 320x208 active playfield plus the established 48px HUD boundary where
  feasible;
- 4+3 dual-playfield as the first candidate;
- FRONT16 near conduit structure and collision-readable obstacles;
- REAR8 clouds, distant pipework, rain and storm-depth parallax;
- monotonic horizontal scroll with at least two visibly different depth rates;
- fixed HUD that remains stable while the playfields move;
- masked/cached Bobs and hardware sprites rather than CPU compositing into the
  displayed bitmap;
- no CPU read-modify-write of displayed Chip RAM;
- first visual/function gate on FS-UAE/68030, then measured FS-UAE/68020.

A 2.5-minute route must not be stored as one 7,000+-pixel resident world. Use a
bounded horizontally rolling/tiled conduit representation, reusable rear spans
and time/world-distance-authored landmarks, enemies, forks and set pieces. The
route remains deterministic even if its art uses repeated building blocks.

## Concept-art and native-asset gates

### Compact Skimmer native scale study — pending rider/vehicle approval

The approved v5 compact-interceptor direction is compared from one identical
occupied side-view source against the exact AGA8 Stormrail rear art. The study
uniformly reduces the craft into FRONT16 and never changes proportions between
candidates:

| Bounding box | Actual silhouette | Screen occupancy | Approx. masked 4-plane Bob/frame |
| --- | --- | --- | --- |
| 96x44 | 96x44 | 30.0% width, 21.2% playfield height | 3,080 bytes |
| 104x46 | 101x46 | 31.6% width, 22.1% playfield height | 3,680 bytes |
| 112x48 | 106x48 | 33.1% width, 23.1% playfield height | 3,840 bytes |

The current recommendation is the 104x46 box. At 96x44 Sparkpaw, cockpit and
material highlights become marginal; 112x48 retains the strongest detail but
reads heavier and consumes more dodge space. Because 104 and 112 both require
eight 16-bit source words including shift safety, 104's main benefit is agility
and screen composition rather than a large cache saving.

Review artifacts:

- `assets/concept/sparkpaw-stormrail-skimmer-v5-scale-study.png`
- `assets/concept/sparkpaw-stormrail-skimmer-v5-scale-study-2x.png`

This study approves no runtime vehicle or rider. The v5 gameplay baseline stays
active until the compact hull and its native rider family are explicitly
accepted.

The head-only deep-cockpit alternative is preserved at
`assets/concept/sparkpaw-stormrail-skimmer-concept-v6-head-only.png`. It keeps
the approved compact-interceptor materials while hiding the seated body
naturally inside a protective pod. If selected, boarding still needs visible
contact and settle frames, but ordinary flight can use a canonical head/ears
occupant plus small reaction poses rather than a complete seated anatomy
family. This is expected to improve silhouette stability, animation scope and
native readability. The user selected this direction and its calm, confident
facial expression on 2026-08-31; full-rider v5 remains concept history and the
current playable v5 runtime remains the safe fallback until replacement art is
actually approved in FS-UAE.

The selected head-only source has its own deterministic FRONT16 scale study:

| Bounding box | Actual silhouette | Screen occupancy | Approx. masked 4-plane Bob/frame |
| --- | --- | --- | --- |
| 96x44 | 96x41 | 30.0% width, 19.7% playfield height | 3,080 bytes |
| 104x46 | 104x44 | 32.5% width, 21.2% playfield height | 3,680 bytes |
| 112x48 | 112x48 | 35.0% width, 23.1% playfield height | 3,840 bytes |

The provisional native recommendation is 104x46. The head-only cockpit makes
96x44 genuinely viable, but 104x44 preserves the approved eye direction,
ears, scarf edge, cyan engine and copper trim more confidently. The 112x48
candidate is clearest but starts to read heavier and removes too much dodge
space for the fast horizontal shooter. Review artifacts:

- `assets/concept/sparkpaw-stormrail-skimmer-v6-head-only-scale-study.png`
- `assets/concept/sparkpaw-stormrail-skimmer-v6-head-only-scale-study-2x.png`

This selects an art direction and provisional size only. It does not approve a
runtime sprite family or alter the current test drawer.

#### Native 104x46 empty/occupied proof

The first selected-direction native family now contains two FRONT16 frames:
an empty powered Skimmer and the same hull with Sparkpaw visible in the deep
cockpit. The empty frame is the canonical hull. The occupied frame differs
only inside the 23x23 cockpit/pilot rectangle; every pixel outside it is
identical, preventing hull scale, alignment or silhouette pops during
boarding. A native steel inset panel, restrained copper speed stripe and cyan
powered-cell tick keep the large flank from collapsing into a near-black mass
against the Stormrail background. At eight source words including shift safety, each masked four-plane
frame is approximately 3,680 bytes; both review frames total approximately
7,360 bytes before any later packing or deduplication.

Review artifacts:

- `assets/concept/sparkpaw-stormrail-skimmer-v6-native-family-aga16.png`
- `assets/concept/sparkpaw-stormrail-skimmer-v6-native-family-review.png`
- `assets/concept/sparkpaw-stormrail-skimmer-v6-native-family-review-3x.png`

This was an art-gate proposal only. It was superseded by the accepted v7
boarding baseline and later compact Gate-2 flight craft described above.

#### Boarding transition direction

The head-only craft does not require a new distorted seated-body family.
Sparkpaw keeps canonical player pose 16 through jump, contact and initial
settle. The unchanged empty hull is then drawn in front of him as real cockpit
occlusion. A dedicated sink beat switches to the approved cockpit head at its
final scale, five pixels above the flight position; the last beat moves that
same head down without resizing or deformation. The craft remains fixed across
all beats. Native storyboard artifacts:

- `assets/concept/sparkpaw-stormrail-skimmer-v6-boarding-storyboard.png`
- `assets/concept/sparkpaw-stormrail-skimmer-v6-boarding-storyboard-3x.png`

This paragraph records the earlier proposal stage. It was superseded by the
implemented and accepted v7 boarding baseline below.

Implementation candidate v7 now exercises that proposal in the production
Amiga renderer. The visible craft remains 104x46 inside a temporary 112x88
transparent boarding cell. Contact and settle use canonical player pose 16
behind the fixed hull; sink and flight use the same approved cockpit head at
one scale. A bounded FS-UAE/68030 auto-board proof captured all three native
transition Bobs without palette, mask, bitplane or silhouette corruption and
reached flight mode with 888,048 Chip bytes free and an 824,656-byte largest
Chip block. Whole-emulator captures also verify the empty and final occupied
compositions with intact HUD/display boundaries. The initial motion review
requested the alignment corrections recorded below.

User-supplied v7 motion evidence is catalogued as
`testresults/Phase 6D-step1-pending-v7-boarding-alignment.mov`. It finds the
sequence broadly successful at speed and requests small contact/settle
alignment adjustments plus better continuity in sink. Contact is four native
pixels left and settle eight pixels left; pose 1, flight pose, hull geometry,
scale and timing remain unchanged. A first hand-authored neck/scarf bridge in
sink was rejected in
`testresults/Phase 6D-step1-rejected-v7-invalid-sink-neck.mov` because it did
not form coherent Sparkpaw anatomy. That complete addition has been removed
and the earlier clean sink frame restored pixel-for-pixel. A repeated bounded
FS-UAE proof shows intact contact/settle/sink frames. The user accepts the
restored result on 2026-09-01 with “ja dit is goed zo, strik erom.” This closes
Gate 1 and authorizes only the Gate-2 open-flight control slice.

A final palette-consistency correction after that motion acceptance normalizes
the separate cockpit source to Sparkpaw's established fur roles: 61 pixels in
each of sink and flight move from the red-orange shadow pen to the ordinary
orange base pen. Frames 0--2, every mask, silhouette, facial feature, cream
muzzle, placement and hull pixel remain unchanged. Bounded production-renderer
FS-UAE captures are clean. The restored v7 sequence and its later Gate-2
handoff were accepted; do not reopen the rejected hand-authored neck/scarf
experiment.

### Gate A — vehicle concept sheet

Create and approve:

- parked side view with Sparkpaw for scale;
- boarding/open-cockpit relationship;
- flying side silhouette;
- folded and deployed conductor vanes;
- damaged/final-sluice state;
- Sparkpaw-visible palette/material callouts.

Concept art is review material, not a runtime asset. Record generation prompts
and accept/reject status in `docs/IMAGEGEN_PROMPTS.md`.

### Gate B — departure-station composition

Create one 320x208 composition containing the parked Skimmer, launch rail,
conduit mouth and only enough ordinary Level-1-like ground for a 5--10-second
walk to the cockpit. Approve silhouette, material hierarchy and immediate
boarding readability before collision or level data is authored.

### Gate C — shooter visual slice

Create one representative 320x208 scene containing:

- combined Sparkpaw/Skimmer candidate at native scale;
- FRONT16 conduit and clear upper/lower fork;
- REAR8 storm/parallax identity;
- one enemy, one player shot and one marked hostile lane;
- existing or provisional fixed HUD;
- nearest-neighbour enlarged review and exact indexed palette report.

### Gate D — native animations

Only after concept approval, plan exact runtime families:

- hover/engine idle: four frames;
- restrained up/down banking poses;
- firing response: two frames;
- hit response: two frames;
- boarding/launch: roughly six to eight frames;
- damaged exit/crash: roughly six frames;
- three bounded drone families;
- sluis locks, impacts and original water/energy effects.

Animation slots, anchors, masks, cache dimensions and palette ownership become
contracts only after native review. Apply the Sparkpaw animation workflow before
runtime integration.

## Audio direction

The first interlude needs an original compact sound set:

- Skimmer rail wake/launch;
- steady engine texture only if it can coexist with future music and does not
  monopolize a Paula channel awkwardly;
- player shot may begin with the accepted plasma identity but can be shortened
  for the higher fire rate;
- drone hit/death;
- warning cue for marked lanes/flood pressure;
- main-sluice break and crash/arrival.

Paula channels 2--3 remain reserved for future music until a measured interlude
audio layout explicitly assigns them. Do not design a continuous engine loop
before that music decision.

## ADF and multidisk policy

The alpha.68 ADF uses 1,641 of 1,760 DOS1/FFS blocks and leaves only 119 free.
Do not force the interlude into that disk or reduce approved art to preserve a
single-disk claim.

Short-term release policy:

- keep the accepted alpha.68 one-level ADF available as an explicitly named
  Level-1 demo while HD/WHDLoad gain the interlude;
- do not imply that the demo ADF contains campaign sections present only in
  HD/WHDLoad;
- measure executable growth on Disk 1 after every progression/shooter step.

Design level paths as multidisk-ready from the beginning:

```text
Disk 1 / volume SPARKPAW     boot, executable, common assets, Level 1
Disk 2 / volume SPARKPAW-I1  Stormrail departure and shooter level pack
Disk 3 / volume SPARKPAW-L2  later Drowned Turbines level pack
```

Use volume-qualified AmigaDOS paths, never hard-coded `DF0:` paths. Support a
second drive naturally, validate volume/section/version/CRC and avoid disk
ping-pong during a section. Progression remains in RAM across a swap.

Build the first ordinary DOS1 multidisk proof only after the HD/WHDLoad visual
slice is accepted and its real asset budget is known. Do not begin with a raw
MFM trackloader. If multidisk is not yet reliable, the one-level demo ADF remains
frozen rather than blocking the main edition.

## Implementation checkpoints

### 6D.1 — progression boundary

- Add Level IDs, replay/continue decisions, campaign state and section snapshot
  contracts.
- Preserve the shipping one-level screen with only `REPLAY LEVEL` selectable.
- Host-test reset, persistence and no-double-bank score behavior.
- Allocate/package no placeholder interlude assets.

### 6D.2 — Stormrail concept package

- Vehicle concept sheet.
- Departure-station composition.
- Shooter visual slice with one upper/lower fork.
- Enemy and material language.
- No runtime integration before explicit visual approval.

### 6D.3 — exact native AGA proof

- Convert approved art to exact indexed assets.
- Review at native 320x208 and nearest-neighbour enlargement.
- Measure palette roles, raw/packed bytes, Chip/Fast placement and candidate
  sprite/Bob layout.

### 6D.4 — boarding vertical slice

- **Accepted focused development baseline (2026-09-01).** The direct-start test
  intentionally bypasses intro, Level 1 and campaign progression.
- Retain ordinary Sparkpaw control for only a short, challenge-free walk in a
  Level-1-like ground composition.
- End the cliff visibly and require a physical jump into the hovering cockpit;
  no Fire, proximity or timer teleport.
- Retain the accepted v7 craft, pulse/hover, contact/settle alignment, restored
  clean sink pose and final occupied pose as the Gate-2 baseline.
- End safely before a full shooter is required.

### 6D.5 — shooter mechanical proof

- First prove a short open-flight control slice: smooth handoff from the
  accepted boarding hold, full directional movement, rapid fire, brisk forced
  horizontal auto-scroll, readable screen bounds and strong unobscured
  parallax. No enemies, hazards or corridors in that first sub-gate.
- Before formations, prove Gate 2.5: `stormrailDistance` is the monotonic
  deterministic timeline coordinate, while the renderer consumes bounded
  reusable foreground/rear spans. The first implementation uses a 768px rear
  loop plus its fetch overlap and keeps the flight compositor derived from the
  protected rolling renderer. A 45--60 second empty test is representative;
  the roughly 2.5-minute authored route remains Gate 6D.6 scope.
- Then add two small enemy types, two deterministic formation patterns,
  complete-formation detection and one 3--5 diamond reward chain.
- Retain parallax and fixed-HUD stability.
- Measure first on 68030 presentation, then 68020 cadence.

### 6D.6 — complete interlude

- Author the full roughly 2.5-minute deterministic timeline.
- Expand to four small enemy families and 8--10 reusable formations, add sparse
  telegraphed debris and finish with the Stormrail Harrier encounter.
- Add interlude results with section and campaign scores.
- `REPLAY INTERLUDE` restores the exact post-Level-1 snapshot.

### 6D.7 — production acceptance

- Host suite and native build.
- Automatic bounded framebuffer/log proofs where useful.
- FS-UAE/68030 complete visual/function gate.
- FS-UAE/68020 cadence and memory gate.
- Supplied real-A1200/68030 HD and WHDLoad gates.
- No ADF or other-hardware claim without separate evidence.

### 6D.8 — multidisk ADF proof

- Measure real Disk-1 executable margin and packed Disk-2 contents.
- Build ordinary labelled DOS1 volumes with a complete interlude on Disk 2.
- Verify correct disk, wrong disk, missing disk, retry, DF1 and progression
  preservation.
- Gate separately in FS-UAE, Gotek/ADF and supplied real-floppy or hardware
  evidence.

## Acceptance criteria

- ordinary Sparkpaw control lasts only about 5--10 seconds on familiar
  Level-1-like ground and leads directly to a visible open-cockpit vehicle;
- boarding is player-triggered and the genre change is immediately readable;
- shooter duration is close to 2.5 minutes including the short platform lead-in;
- scroll is continuously horizontal, brisk and forced;
- parallax remains visually strong throughout;
- encounter density feels intense without unavoidable damage or bullet clutter;
- at least two short, fair upper/lower route choices rejoin cleanly;
- Sparkpaw remains recognizable in/on the Skimmer at native size;
- Level-1 replay remains byte/functionally isolated from cross-section loading;
- every section has its own run score;
- later result screens show the current section score and total campaign score;
- replaying the interlude restores the exact accepted post-Level-1 campaign
  snapshot and starts the interlude score at zero;
- no section result can be banked twice;
- HD/WHDLoad progress is not blocked by single-ADF capacity;
- any multidisk edition uses ordinary validated volume swaps before considering
  lower-level disk loading.

## Reference record

- Bubble and Squeak Amiga longplay, supplied timestamp around 03:01: primary
  feel reference for the abrupt shooter change, forced auto-scroll, density and
  pace.
- ThunderCats Amiga longplay, supplied timestamp around 05:46--05:48: visible
  reference for entering a recognizable ride-on vehicle and immediately using
  it in a horizontally constrained action corridor.
- Local `testresults/thundercats-level1.mov`, inspected across its 43.87-second
  Level-1 excerpt: supporting reference for a low fixed HUD, large readable
  silhouettes and clear obstacle/enemy separation. It is not the vehicle-level
  footage and supplies no Sparkpaw performance or acceptance evidence.

These sources provide design principles only. Sparkpaw uses original vehicle,
route, enemy, animation, audio and pixel assets.
