# Gate solidity review — 13 September 2026

User accepts the improved geyser and through-gate overlap from Slice3, but
rejects the see-through upper machinery and jumping through the header.
The MOV was initially absent, then supplied and inspected: 11.066667 seconds,
664 nominal 60Hz capture frames. Consecutive frames starting at 0.65s establish
that the head crosses the header and continues upward. Capture FPS is not
gameplay timing evidence.

## Preserved evidence

All under `testresults`, each with matching TXT metadata/review:

- `Unassigned-Drowned-Slice3-jump-through-header.mov`
- `Unassigned-Drowned-Slice3-closed-gate-header.png`
- `Unassigned-Drowned-Slice3-open-gate-header.png`

MOV renamed byte-identically; clipboard PNGs copied byte-identically and their
original temp files retained. Derivative sheets under build/drowned-review-evidence:
`gate-jump-overview.jpg` and `gate-jump-consecutive.jpg`.

## Cause and proposed coherent behaviour

`drownedGateSolid()` models only the retracting leaf; at full opening it returns
false everywhere. No permanent roof/header collider exists. Player jump code
therefore correctly passes through what collision considers empty space.
The right-post mask is a separate visual effect; it cannot stop the jump.

The upper part of the authored indexed front contains pen-zero holes. They are
transparent in a binary sense, not a translucent-metal effect. Treating the
machinery as an opaque roller housing makes its structural meaning clearer.
New concept `assets/concept/drowned-solid-gate-source-v1.png` is pending review.
It preserves the steel/copper identity while enclosing the shutter mechanism.
The open frame retains its housing, guides and lower beam; shutter segments
roll into the housing rather than implying a full-height panel disappears.

After concept acceptance:

1. Convert a SINGLE fixed frame/housing from the approved art. Closed/open
   concepts must not be separately resized and pasted as two different frames.
   Retain fixed near/far posts and identical machine geometry in every state.
2. Keep the passage at floor level free after opening. Posts represent the
   far/near sides of the passage, so adding two walk-blocking columns would
   contradict the accepted depth cue. The crossbeam DOES span the path.
3. Add a permanent bounded solid header that survives opening/reset. Upward
   contact stops velocity and lets gravity return Paw to the floor, without
   damage or a special input mode. Side entry at header height must also stop;
   its top is solid if reached from above. Ordinary floor traversal stays free.
4. Align the local ceiling response with the visible head, not just the smaller
   damage/body box. Existing player HIT_TOP is +5 relative to player.y; sprite
   storage begins at player.y-8. Merely adding a roof would still allow visible
   head overlap. Audit actual jumping/standing/crouch frame bounds and apply a
   local gate clearance rule without changing Level1's physics or sprite cache.
5. Share explicit header coordinates between art and collision. Current
   optimized span/sweep queries assume tile-constant collision. Use aligned
   bounds or explicit additional boundaries and pixel-reference tests; do not
   silently introduce an arbitrary edge those queries can skip.
6. Extend tests with interior jump, left/right edge approach, opening during
   jump, underside collision, traversal both ways, crouch/stand, life reset and
   immutable inactive-stage masking. Then normal/slice builds and a coherent
   030 test drawer before the 020 measurement gate.

Retain HUD, geiser, route and accepted depth. No new world-sized layers or
per-frame allocation needed. Actual 020 performance still requires measurement.
No runtime/dist changes in this concept/evidence turn; Slice3 remains active.

## Implementation after concept approval — Slice4

User approved the concept. Native implementation reuses the accepted gate frame,
pipes, panel and leaf, replacing only its hollow central housing with the metal
housing crop from the approved concept. One fixed opaque 32x16 component is
placed at x800/y120; the 14 gate states share exactly that same housing. Generator
reads geometry from drowned_slice.h, avoiding separate art/collision numbers.
Only 393 indexed front pixels differ from Slice3, all within the housing;
all geyser patches are byte-identical. Native closed/open enlargement:
`build/drowned-slice/assets/gate-review-6x.png`.

Permanent collision covers x800..831/y120..135. Vertical span queries explicitly
intersect this rectangle before the normal tile probes, preserving its y120 edge.
Horizontal extents remain tile aligned, including projectile sweep behaviour.
The normal body hitbox remains unchanged; compile-isolated player code also
checks the visible head band y-8..y+4 against the header when moving upward or
sideways, and when deciding if crouch can unfold. This conservatively keeps the
48px sprite top below the beam. The near-post stage mask remains unchanged.

Actual C tests establish: jump inside stops at player.y144 (visible top136),
returns to floor y161 and retains health; both head-only side approaches stop;
landing on the housing yields player.y81; normal crouch/stand and floor passage
work; reset restores closed leaf while retaining the header. Sweep hits800/831;
pixel-reference horizontal/vertical span checks include the y120 edge.
Native slice and normal campaign builds and the full host suite pass.

Current drawer `dist/Drowned-Slice4-030-HD/Drowned-Slice`, user 030 review pending.
Stager checked 63 assets, 47 executable references; 68 alpha.8 files unchanged.
Slice3's 65 files archived intact at older-builds/20260913-before-drowned-slice4/.
Package proof: build/drowned-slice/slice4-package-proof.json. Same local atlas
and transfer-stage sizes; no new asset memory. No measured 020 timing claim.
No emulator run, release, version change, commit or push.
