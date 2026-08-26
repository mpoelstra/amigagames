# Level 1 Stormstone Core clearing plan

Status: historical first prototype, superseded by the accepted polish outcome
in `LEVEL1_CORE_CLEARING_POLISH_PLAN.md`. The initial right-edge composition,
flat Core and immediate replay remain rejected. The corrected checkpoint passes
supplied FS-UAE/68030 visual/function review and a 49.67-FPS minimal-cadence
FS-UAE/68020 HD gate with zero three-field misses and ownership violations.
ADF gameplay and real hardware remain untested. No Makefile, version or release
change is part of this checkpoint.

## Recommended scene: the Stormkeeper's Waystation

Extend the Storm Ruins route by one visible 320px field beyond the current
final portal. The portal becomes a threshold rather than the automatic end.
Beyond it lies a quiet **Stormkeeper's Waystation**: a small ruined weather
station that reads visually as both a cottage and a machine shrine.

The key silhouette combines the user's house/tree ideas:

- a compact stone-and-steel caretaker house, approximately half a screen wide;
- an open arched doorway containing a low cyan-conduit pedestal;
- one old storm-bent pine growing through the collapsed right roof;
- roots wrapped around the foundation and one severed conduit entering them;
- the first Stormstone Core floating above the pedestal in the doorway;
- the distant tower/vortex still visible behind or just beyond the roofline.

The tree makes the location feel protected and older than the machinery. The
house gives the Core a reason to be there: it was a local weather station, not
a random prize left on open ground. The doorway supplies a naturally dark
frame, making the cyan Core readable without a modern UI marker.

## Route and composition

Implemented spatial beat:

```text
existing final portal | short empty approach | waystation + tree | quiet edge
                      |<------ one 320px visible field ------->|
```

- Keep one continuous, collision-readable ground surface across the new field.
- Add no raised gameplay platforms, gaps, water, enemies or ordinary hazards.
- Place no Storm Shards in the final half-screen; the Core must be the sole
  collectible focus.
- Give Sparkpaw roughly 80–112px of empty approach before the Core enters the
  centre third of the viewport.
- Place the Core around standing chest/head height above its pedestal so it is
  reached by walking into it, not by solving one last jump.
- Leave enough quiet space beyond it that pickup does not occur against the
  hard right edge.
- Compose house, tree and tower as three depth anchors; avoid filling the whole
  screen with foreground detail.

The scene is a decompression beat. The player has already demonstrated mastery;
the reward should be unmistakable and safe.

## Core visual contract

The first Core must not look like a scaled ordinary diamond.

- Suggested visible size: roughly 28–36px wide and 36–48px high, subject to an
  exact native-resolution review.
- Asymmetrical crystal silhouette rather than the Shards' small clean diamond.
- Mechanical bronze/steel holding collar or two broken retaining arcs.
- Cyan centre, pale energy edge and one deep navy interior facet.
- Slow 2–3px hover and restrained four-state pulse are optional later motion.
- No rotation is necessary; a stable iconic silhouette reads better.
- The ordinary Storm Shard symbol remains small, regular and numerous.

Conceptual terminology:

| Object | Scale/frequency | Function |
| --- | --- | --- |
| Storm Shard | small, many per level | Gauntlet charge; fifty award one life |
| Stormstone Core | large, one at level end | Required quest item and level-complete trigger |
| Stormstone | reconstructed finale object | Restores the weather network |

## Temporary prototype behavior

Touching the Core now:

1. uses one rectangle-overlap test against the large static Core;
2. invokes the existing in-memory level replay path immediately;
3. preserves the current lives and Storm Shard counter contracts through that
   existing reset path.

This is explicitly a temporary stand-in. Another session owns the real
`LEVEL_COMPLETE`, results presentation, progression and next-level flow. Do not
design permanent save/progression semantics into this temporary trigger.

The first proof deliberately has no settle delay, disappearance animation or
new render family. Those remain presentation options for the later real
`LEVEL_COMPLETE` flow.

## World-length decision and cost gate

The prototype increases world width from 3072px to 3392px and rear coverage
from 1024px to 1120px. Maximum camera origin is 3072px. At that origin the
quarter-speed rear fetch starts at 768px; 1120px supplies the complete fetched
span plus a 16px guard.

Measured alpha.45-to-prototype costs are:

| Data | Raw resident growth | Packed ADF growth |
| --- | ---: | ---: |
| FRONT16 source | 33,280 bytes | 7,306 bytes |
| REAR8 source | 7,488 bytes | 5,433 bytes |
| collision map/cache | 600 bytes | not separately packed here |
| bitmap total | 40,768 bytes | 12,739 bytes |

The 280-byte collision-map growth and 320-byte hazard-cache growth are static
non-bitmap data. The foreground and rear source planes keep their established
allocation path. No new enemy, projectile, Bob, restore job, palette bank,
Copper work or per-frame Blit is introduced; the steady-state gameplay addition
is one four-bound rectangle comparison.

The remaining acceptance gate is evidence, not another architecture change:

- inspect the final screen and Core contact/replay in FS-UAE/68030;
- compare a low-overhead FS-UAE/68020 run with the protected 48.58-FPS baseline;
- only make a hardware claim from new supplied hardware evidence.

The first 68030 attempt did not reach that gate. Screenshot evidence is
preserved as `testresults/Phase 6C.2-rejected-fs-uae-68030-charging-glitch.png`
with a matching sidecar. No `renderdiag.log` exists in the candidate drawer.
The focused startup run then recorded `failed_rear_guard_prepare` with a
784400-byte largest free Chip block. This rejects exhaustion and isolates the
logical-width allocation: graphics.library can pad the 1120px source and prior
1152px destination into the same physical stride class, leaving no actual
four-byte guard. The corrected 68030 candidate allocates from the source's
physical `BytesPerRow + 4`; the supplied retest reaches gameplay and the final
clearing with 732624 Chip bytes free after renderer preparation. The art review
rejects the right-edge composition, grey/black house reduction, static Core and
immediate replay. Do not promote it to 68020 before the planned polish passes a
new visual/function gate.

If the measured resident cost is rejected, fallback is to repurpose the current
last visible field after the final chasm: remove its late combat/collectible
clutter and place a smaller waystation composition there. That saves width but
weakens the desired clean separation from the final route challenge.

## Implemented art staging

The generated waystation/tree source is preserved in
`assets/concept/sparkpaw-stormkeeper-waystation-source-v1.png`. Runtime
generation keys its neutral checker, scales it to a 200x145 silhouette and maps
it into the existing FRONT16 material bank. A larger procedural Core is baked
into the same static foreground for this first proof. The exact generated
320x208 review is `assets/levels/storm-level1-core-clearing-aga-preview.png`.

Keeping the Core static is intentional: it proves scale, destination framing
and completion semantics at zero new rendering cost. A later removable or
animated Core must be separately measured.

## Acceptance questions

- Does the player immediately understand that this is a safe destination?
- Does the Core dominate the view without resembling an ordinary Shard?
- Does the waystation feel native to the Storm Ruins rather than a generic
  fantasy cottage?
- Is the crooked tree adding history and asymmetry rather than visual clutter?
- Can the tower/vortex remain visible enough to connect this Core to the larger
  quest?
- Is the one-screen extension worth its measured memory and package cost?
