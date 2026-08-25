# Story line and short intro plan
Status: proposal only. No runtime or release implementation is authorized by
this document.

## Creative premise: The Misfiled Storm

Long before Sparkpaw's time, the Storm Ruins regulated dangerous weather with
Stormstones. The tower at the centre of the valley housed **Grand Archivolt**,
an enormous caretaker machine charged with cataloguing storms and releasing
their energy safely.

Centuries without maintenance have damaged one word in Archivolt's final
instruction. “File storms after use” has become “file storms. All of them.” He
now drags every cloud, lightning bolt, Stormstone shard and unsecured metal
object toward his archive. The result is a valley of violent local storms,
empty skies elsewhere, electrified water, confused patrol machines and a tower
that is rapidly running out of drawers.

Sparkpaw is a young inventor, not a chosen monarch or professional hero. A
small shard in his workshop reacts to the theft. His improvised detector works,
but because it shares a coil with his toast warmer, it also launches breakfast
into the ceiling. He follows the energy trail to repair the system. The comedy
comes from machinery following bad instructions with absolute dignity; the
danger remains sincere.

## Antagonist and final boss

### Grand Archivolt, Keeper of Inclement Records

Archivolt is a tower-scale clockwork storm warden: elegant crown-shaped frame,
six articulated conductor arms, one bright cyan archive core and a tiny brass
service bell. His manners are impeccable and his interpretation of procedure
is catastrophic. His recurring line can be: **“Weather is untidy.”**

The boss should feel like the logical source of existing enemies. Striders are
mobile filing cranes; beetles are floor-cleaning indexers; hostile orange
plasma is an overheated correction stamp. This creates narrative cohesion
without requiring those enemies to change in the present concept phase.

Possible later three-part encounter:

1. **Sort:** conductor arms place lightning columns and sweep loose archive
   crates across readable lanes.
2. **Stamp:** the core opens and marks sections of the arena for delayed storm
   strikes; Sparkpaw uses the arm rhythm to reach exposed relays.
3. **Overflow:** jammed drawers spill captured clouds and one harmless shower
   of labels, socks and toast. The core is vulnerable while Archivolt rings his
   own complaints bell for assistance.

Defeat need not destroy him. Sparkpaw replaces the damaged instruction plate.
Archivolt reads the restored order, pauses, and quietly asks whether the last
several centuries should be entered as overtime. This leaves him available as
an uneasy ally, level hub or future source of quests.

### Alternative names

- **The Grand Archivolt** — recommended; original, storm/electric and archival
  meaning, readable as a boss name.
- **Lord Cumulonimbus the Third** — broader comedy, less mechanical identity.
- **The Barometric Baron** — memorable but more cartoon-villain than ancient
  caretaker.

The supplied Banshee opening actually spells its ruler **Blardax Maldrear** in
the visible story text. Sparkpaw should borrow only the economy of that reveal,
not the name, empire or plot.

## Reference lessons

Visual inspection of the supplied openings suggests this useful combination:

- **Banshee:** one large readable narrative statement at a time; subdued still
  art lets the text dominate; introduce the antagonist early.
- **Marvin's Marvellous Adventure:** framed character plates carry personality
  and permit an overt joke without needing elaborate animation.
- **Fury of the Furries:** a still can feel alive through small local movement,
  fades and wipes; full-screen continuous animation is unnecessary.

Do not copy their wording, characters, typography, layouts or story events.

## Recommended intro: four plates, about 34 seconds

Target one clean 320x256 PAL presentation. Fire skips to the title immediately;
after a brief safety delay, fire may also advance the current plate. Never make
the player wait through the intro on every boot.

| Beat | Time | Still and small motion | Scrolling text |
| --- | ---: | --- | --- |
| 1. The old system | 0:00–0:07 | Wide valley and tower; two-frame lightning fork; tower core pulses once. | “For centuries, the Storm Tower kept every tempest in its proper place.” |
| 2. The mistake | 0:07–0:15 | Archivolt framed like an official portrait; one eye opens, a drawer rattles, tiny bell moves. | “Then its keeper found one damaged order: FILE THE STORMS. ALL OF THEM.” |
| 3. The alarm | 0:15–0:24 | Sparkpaw asleep in workshop; detector flashes; toast rises and sticks out of frame. | “Miles away, Sparkpaw's storm detector finally worked. So did the toast alarm.” |
| 4. The quest | 0:24–0:34 | Sparkpaw on ridge facing the tower; scarf flutters; one shard travels toward the vortex. | “With the sky disappearing—and breakfast ruined—Sparkpaw set off to correct the filing.” |

After plate 4, fade to the existing title. If the title is shown before the
intro in a later flow, return to it without replaying loading work.

### Text composition

Use a stable text safe area rather than placing prose arbitrarily over detail:

- still area approximately 320x176;
- dark text band approximately 320x80, or an inset 288px-wide plaque;
- three lines visible at once, roughly 34–38 characters per line depending on
  the final font;
- slow upward scroll of only the current short passage, then a 1.0–1.5 second
  fully static reading hold;
- cream body text, cyan key phrase, no more than one highlighted phrase per
  plate;
- avoid long all-caps prose; reserve it for Archivolt's broken instruction.

The still should establish subject and location before the first text line
enters. A quick fade between plates is cleaner than moving both image and text
simultaneously.

## Animation scope

### Minimum viable intro

Four still ILBM-style screens, palette fades, a text scroller and input skip.
This already meets the intended pacing and should be the first isolated proof.

### Preferred light animation

Animate only one or two small regions per plate, 2–4 frames each:

- lightning fork: three frames, non-looping;
- Stormstone/core pulse: four palette or bitmap states;
- Archivolt eye and bell: two frames with a long irregular hold;
- detector lamp and toast: lamp palette cycle plus a short 5-frame vertical
  movement;
- scarf: three restrained frames.

These can be authored as small overlays or full still variants. That choice is
an implementation measurement, not decided here.

## Production gates for a future dedicated session

1. Approve premise, Archivolt design, text tone and four-beat order.
2. Draw exact 320x256 composition wireframes with real font metrics.
3. Produce four indexed stills and verify palette/legibility in FS-UAE.
4. Build a standalone intro proof, not an edit to gameplay or the renderer.
5. Measure disk blocks, Chip/Fast peak, load time and frame cadence.
6. Add only the small animation that survives the same gate.
7. Integrate intro/title/loading flow only after explicit approval and after
   the other workstream's performance/hardware boundary allows it.

## Explicit non-goals

- no runtime, renderer, Makefile, dist, version or release changes now;
- no claim that IFF ANIM is required or already supported;
- no full-motion cartoon, speech audio or large character animation;
- no changes to current gameplay geometry, enemies or boss implementation;
- no performance, ADF, Pocket or real-hardware acceptance inference.
