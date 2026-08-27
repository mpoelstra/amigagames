# Story line and short intro plan
Status: story contract and five-plate intro implemented and accepted for the
alpha.47 Phase 6C.3 release checkpoint. Optional local animation remains later
polish rather than a release dependency.

## Creative premise: The Misfiled Storm

Long before Sparkpaw's time, the central **Stormstone** machine regulated the
valley's weather. Four installed Cores governed Lightning, Rain, Wind and
Warmth; a fifth Balance Core coordinated them. Remote weather stations normally
distributed that energy safely. **Grand Archivolt** was the caretaker machine
charged with guarding the Stormstone and containing only dangerous weather.

Centuries without maintenance corrupted his order into: **“CONTAIN ALL
WEATHER. RELEASE NOTHING.”** Archivolt obeyed literally. He removed all five
Cores, sealed the four elemental Cores inside their remote stations, kept the
Balance Core in his archive and reversed the network. The stations now pull
their elements inward without limit: rain becomes floods, wind becomes
hurricanes, warmth burns in one place while others freeze, and lightning never
stops.

Sparkpaw is a young inventor, not a chosen monarch or professional hero. He
watches the storms tear through his home and decides someone must free the
Cores. His shard-powered gauntlet can locate and safely carry them back to the
Stormstone. The comedy comes from an impeccably polite machine following a
catastrophic order; Sparkpaws motive and the danger remain sincere.

## The Stormstone and the quest

The **Stormstone** is the ancient machine at the heart of the network; its five
large **Stormstone Cores** were always distinct installed components. Each of
the five levels ends with the recovery of one Core. Returning all five restores
the machine and the natural weather cycle.

The numerous small diamond collectibles already present in the game are
**Storm Shards**: minor charged crystals shed by the damaged network while the
Cores were transported through the ruins. Sparkpaw's gauntlet collects their
residual charge. Fifty Shards provide one emergency Storm Charge, retaining the
current fifty-collectibles-award-one-life rule without implying that fifty
small items form a major Core.

Sparkpaw can locate and safely handle the Cores because his teal gauntlet is
powered by a legally acquired Storm Shard. The complete quest is therefore:

> Collect Storm Shards to power the gauntlet, recover one Stormstone Core per
> level, then return all five to the Stormstone and restore the natural sky.

The first Storm Ruins level should eventually end in a quiet one-screen Core
clearing after its existing final portal. See `LEVEL1_CORE_CLEARING_PLAN.md`.

### Five-level quest spine

| Level | Station identity | Core | Story function |
| --- | --- | --- | --- |
| 1 | **Storm Ruins**: broken conduits, storm water and the Stormkeeper's Waystation | **Lightning Core** | Sparkpaw proves the detector works and learns that the weather was deliberately separated. Implemented in alpha.46. |
| 2 | **Drowned Turbines**: flooded pumps, sluices and rain machinery | **Rain Core** | Restoring circulation reveals that Archivolt routes every weather system toward the archive. |
| 3 | **Gale Foundry**: pressure furnaces, fans and airborne machinery | **Wind Core** | Sparkpaw follows the transport current and sees the archive physically filling with captured sky. |
| 4 | **Ember Observatory**: sun mirrors, seasonal instruments and unstable heat | **Warmth Core** | The valley freezes outside the station while the last route into Archivolt's tower opens. |
| 5 | **Archivolt's Sky Archive**: drawers, storm vaults and the tower core | **Balance Core** | Archivolt uses the coordinating Core in the final battle. All five are then returned to the Stormstone. |

These are strong working names, not locked production titles. Every level ends
at a distinct Stormkeeper station built from the Level 1 visual grammar: a
quiet arrival, one landmark larger than Sparkpaw, a floating Core and a short
celebratory collection beat. The station changes with its biome rather than
reusing the same cottage five times.

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

## Recommended intro: five plates, about 32–36 seconds

Target one clean 320x256 PAL presentation. Fire skips to the title immediately;
after a brief safety delay, fire may also advance the current plate. Never make
the player wait through the intro on every boot.

| Beat | Time | Still and small motion | Scrolling text |
| --- | ---: | --- | --- |
| 1. Balance | 0:00–0:06 | Valley, five weather stations and central tower; cloud drift and one Core pulse. | “For centuries, five Stormstone Cores kept the valley's weather in balance.” |
| 2. The mistake | 0:06–0:13 | Archivolt among archive drawers; one eye opens and the service bell twitches. | “Then Grand Archivolt found one damaged instruction: FILE EVERY STORM.” |
| 3. The fracture | 0:13–0:20 | Stormstone divides into five signed Cores; trails point toward five biome silhouettes. | “He split the Stormstone into five Cores and filed each kind of weather separately.” |
| 4. The alarm | 0:20–0:27 | Sparkpaw's detector visibly shares a coil and cable with a toaster slot; its pulse launches one slice. | “Sparkpaw's homemade detector found the first Core. Unfortunately, it still thought it was a toaster.” |
| 5. The quest | 0:27–0:36 | Sparkpaw on a ridge facing the tower; five empty gauntlet indicators imply progress. | “Recover every Core. Rebuild the Stormstone. Put the sky back where it belongs.” |

After plate 5, fade to the existing title. If the title is shown before the
intro in a later flow, return to it without replaying loading work.

### Text composition

Use a stable text safe area rather than placing prose arbitrarily over detail:

- still area 320x168 in the first layout proof;
- dark text band from y=168 through y=255 with a roughly 296x64 text safe area;
- three lines visible at once, roughly 34–38 characters per line depending on
  the final font;
- slow upward entrance of only the current passage over 1.0–1.5 seconds, then
  a 2.5–3.0 second fully static reading hold;
- cream body text, cyan key phrase, no more than one highlighted phrase per
  plate;
- avoid long all-caps prose; reserve it for Archivolt's broken instruction.

The still establishes subject and location before the first line enters. Art
and prose never move simultaneously. Fire reveals the complete current text, a
second press advances, and holding Fire skips. Use a quick fade between plates.

## Animation scope

### Minimum viable intro

Five still ILBM-style screens, palette fades, a text scroller and input skip.
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

1. Approve premise, Archivolt design, text tone and five-beat order.
2. Draw exact 320x256 composition wireframes with real font metrics.
3. Produce five indexed stills and verify palette/legibility in FS-UAE.
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
