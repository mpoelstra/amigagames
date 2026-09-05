# Stormrail Gate 6 fixed-camera finale / Storm Ruins threshold contract

Status: accepted integrated FS-UAE/68030 Gate-6 contract as of 2026-09-04.
It is not campaign/results integration or a release checkpoint.

## Boundary

Gate 6 is the Stormrail end fight at the threshold to **Level 2: Storm Ruins**.
It begins exactly once when monotone `stormrailDistance` reaches 15500.
The value remains exactly 15500 through combat, gate opening, automatic exit
and the focused proof's completed hold. `cameraX` latches to the bounded
presentation phase derived at that coordinate. No finale seconds, approach
distance or exit distance are added and no ordinary formation, reward or
Debris5.2 event may spawn after the latch.

The camera grammar is a strict flat 2D side elevation. One continuous Storm
Ruins boundary wall touches both the top and bottom of the 208-pixel playfield
at the far right. Wall and closed central door occupy only 32 screen pixels of
width, preserving the left and middle arena for firing and avoidance. The
Harrier stays in front of and visually separate from the wall. Its 80 by 46
native target is only about
1.5--1.8 times the occupied Skimmer's perceived mass, not a capital ship. No
decorative masonry may imply collision or narrow the accepted safe bounds.

The latch begins with a 32-tick local arrival beat while distance and camera
are already fixed. The full-height wall moves left from x=320 to x=288 at one
pixel per tick; the Harrier enters alongside it from x+128 at four pixels per
tick. Player steering remains available, but hostile and player fire are
disabled until both reach the accepted combat composition. This replaces a
single-frame pop without adding routedistance, loading, scaling or allocation.

The encounter reuses the five-player-shot and four-hostile-shot pools. It adds
one fixed encounter actor, not a generic enemy slot: one 80x46 Harrier around
base (188,81). There are no turrets. The closed gate occupies x=288..319 across the full 208px
playfield. All dimensions and positions are native pixels. There is no runtime
scaling, rotation, per-pixel effect, per-frame allocation or gameplay-pool
increase.

The Harrier's gameplay and collision contract remains 80x46. Its authored
non-transparent pixels occupy the exact local rectangle x=8..70, y=1..45.
The renderer may therefore cache, restore and draw only that 63x45 rectangle
at actor position + (8,1). A host test must prove every excluded source pixel
is pen 0. This is a pixel-equivalent Blitter optimization only: movement, shot
origin, collision, HP, score and the actor's logical position remain 80x46.

During combat only, the Harrier follows two differently phased, deterministic
triangle patrols of x=-20..+19 and y=-28..+27 around its base. Collision,
telegraph and shot
origin use that same derived position. The player shot remains the accepted
cyan plasma. The Harrier uses a visually distinct violet/magenta storm pulse
with a light core. No additional
projectile slot is allocated.

The accepted eight Gate-4D dust slots continue to move during every finale
phase. Their authored shapes, colours, speeds and safe y positions remain
unchanged; only their travel phase advances from the monotone local finale
visual timer while `stormrailDistance` remains exactly 15500.

## Data-driven actors and attacks

| Actor | HP | score | persistent award ID | pattern |
|---|---:|---:|---:|---|
| Harrier | 120 | 320 | 60 | 160-tick cycle: fixed fan, then low-health Hunter burst |

The base fan fires at phase 32 after one visible 20-tick charge warning. The
complete three-shot beat is skipped unless three existing hostile
slots are free; it is never delayed and never emits a partial fan. All three
shots share dx=-4 while their fixed dy values -2, 0 and +2 create two diagonal
lanes and one horizontal lane. They do not home. The long telegraph, fixed
geometry and open arena preserve a readable escape route without blind fire or
burst backlog.

At 60 HP or below, the Harrier additionally unlocks the Hunter burst in the
same 160-tick cycle. A visibly different 20-tick warning precedes three single
shots at phases 96, 108 and 120. Each shot independently samples the current
Skimmer position, clamps dy to -2..+2, then travels at dx=-5 without homing.
This makes changing direction useful while leaving 64 ticks between the fan
and first Hunter shot and 72 ticks between the last Hunter shot and next fan.
No pattern overlaps by design and a blocked shot is skipped rather than queued.
The Hunter warning is governed by the exact same `HP <= 60` predicate as its
launches: a warning without an enabled shot is forbidden. Fan and Hunter each
own a short, preloaded charge cue and a distinct fire cue on the existing
prioritized gameplay-effect Paula voice. The fan remains a round violet storm
pulse; Hunter rounds are narrow amber-white tracking needles. These identities
do not alter the player's accepted cyan-white projectile or enlarge either
projectile pool.
Existing collision,
one-health-unit damage, 36-tick invulnerability and hurt audio remain shared.

During `COMBAT` the Skimmer's accepted 60x24 damage rectangle at local
(10,4) also collides with the live Harrier's existing 80x46 actor rectangle.
Contact removes exactly one health unit (one half heart), starts the same
36-tick invulnerability and hurt cue as hostile fire, and places the Skimmer
flush against the Harrier's left edge. Contact never damages the Harrier and
cannot repeat while invulnerability is active. The bounded 3px player step is
far smaller than either rectangle, so the overlap test cannot tunnel between
adjacent ticks. Contact is disabled during arrival, opening and exit.

Player shots become valid only after the complete finale composition is
latched. Each projectile resolves against the gate-facing actor first along its
travel direction; one shot can damage only one actor. Non-lethal hits reuse the
enemy-hit cue and a four-tick flash. A lethal hit reuses enemy-death audio and
awards its score once through ID 60. Life restart stays inside the encounter:
it preserves remaining Harrier HP, skips the one-shot arrival, clears all
projectiles and restarts the attack timer at a safe cycle boundary. The
persistent score bit remains set, matching formations and debris carriers. The
gate cannot open until the Harrier is destroyed.

## Lifecycle

| Phase | Entry condition | Bounded behaviour | Exit condition |
|---|---|---|---|
| `ARRIVAL` | distance first reaches 15500 | camera/distance latched; wall and Harrier enter from the right; steering remains; all fire disabled | local tick 32 |
| `COMBAT` | arrival tick 32 | player retains flight control/fire; gate closed; table-driven Harrier fan active | Harrier has zero HP |
| `OPENING` | Harrier dies | clear all hostile slots immediately; disable every hostile charge/launch; open gate for 48 local ticks | gate is fully open |
| `EXIT` | opening tick 48 | disable manual movement/fire; move Skimmer at most 2px/tick toward y=88 and x=112, then accelerate right by 4px/tick | Skimmer x exceeds 328 |
| `COMPLETE` | automatic passage finishes | hold a clean focused-proof end state with hostile fire disabled | later campaign/results gate only |

During `OPENING`, `EXIT` and `COMPLETE`, hostile activity must be zero on every
tick. Existing player shots are retired when combat ends. Gate opening and
exit use `stormrailFinaleTick`, never `stormrailDistance`. Automatic centring
cannot move more than two pixels on either axis per tick and may not cross the
playfield bounds. The focused Gate-6 build stops in `COMPLETE`; it does not add
results, Level 2, campaign banking, loading, SemVer, release or ADF behaviour.

The eventual complete interlude performs one initial load before play. Reaching
distance 15500 and entering this finale must never show a loading screen, disk
icon or Workbench again: the wall, gate, Harrier, projectiles and sounds are
already resident, and the route-to-finale transition remains one continuous
flow. A direct-to-finale test drawer may spend longer preparing at process
startup, but that startup behaviour is not an encounter transition contract.

The closed gate remains a dynamic masked wall overlay: restore the prior gate
rectangle through the established flight-blank path and redraw both halves
after dust, projectiles and the Skimmer. Do not cache the gate in either display
or rolling clean targets. V14 produced broad stale-bitplane corruption and V15
still allowed dust/projectile restores to punch holes through transparent wall
pixels. Both approaches are permanently rejected. Further performance work
must leave the gate's restore/draw ownership and ordering unchanged.

The permitted row-repair optimization remains an overlay, not a gate cache.
Each rolling target receives one complete closed-gate draw when it first owns
the encounter. On later closed-gate frames, restores and pre-gate Bobs mark
only intersected playfield rows; contiguous marked runs are redrawn from the
original gate mask at the normal gate layer before foreground projectiles.
Opening still uses complete moving halves. No gate pixels are ever installed
in a clean target, and unmarked wall rows retain their already-owned overlay.

## Protected baseline and acceptance order

Gate 1, Gate 2/2.5, Gate 3, all eight Gate-5A formations/rewards, exact
Debris5.2, Gate-4D dust, the 13-stage/23-state palette table, line-252 HUD and
all rolling-renderer ownership boundaries remain unchanged. The rejected dust
slot at y=211 remains forbidden.

The first automatic proof must use production update/render paths behind a
compile guard, start immediately before distance 15500, exercise the complete
lifecycle and write a bounded contract log outside `dist`. Finale actor caches
may use a shared maximum cell, but the Blitter's colour-plane stride must remain
that allocated maximum height; it must never be recomputed from an individual
actor's smaller visible height. This remains binding for the 80x46 Harrier.

Host tests, native compilation and supplied user play now accept the complete
68030 route/finale, including stable silhouettes, distinct projectiles, dust,
wall integrity, restart behaviour, contact and the line-252 HUD. Full4 is the
accepted archived reference drawer. Any later renderer change must repeat those
checks. The supplied complete-interlude stock-68020 cadence result is accepted
at 49.97 FPS with one two-field, one three-field and zero ownership violations;
the post-finale results screen is now the subsequent separate gate.
