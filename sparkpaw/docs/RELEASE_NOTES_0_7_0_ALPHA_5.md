# Sparkpaw 0.7.0-alpha.5 — Stormrail takes flight, and the whole adventure finds its soundtrack

## What's new since alpha.68

Alpha.68 ended with the first Core and a Level-1 replay. This update connects
that adventure to Stormrail: a complete Skimmer flight section with its own
enemies, scenery, hazards, boss encounter and results. Four original themes now accompany the journey, including music and sound
effects together during both playable sections. The complete campaign comes
in HD, WHDLoad and two-floppy ADF editions.

### A connected campaign

- **Continue beyond the first Core.** Finish Storm Ruins and choose CONTINUE
  to begin Stormrail, carrying your remaining lives, health and diamond meter.
- **Replay either section.** Level 1 keeps its instant replay, and Stormrail
  can now be replayed from its departure point without reloading its gameplay
  files. Stormrail replays restore the lives, health and diamonds brought into
  that section, giving you another attempt from the same starting position.
- **Choose where to begin.** The ready-screen OPTIONS menu now lets you start
  in Storm Ruins or jump straight to Stormrail. The secondary-button Jump/Fire
  setting remains available.
- **Return and start afresh.** BACK TO TITLE after Stormrail clears the campaign
  for a new run. Esc abandons the current run and returns to the ready screen,
  without replaying the story introduction.

### Board the Skimmer and take flight

- **A playable departure.** Jump from the broken cliff into the hovering
  Skimmer's cockpit to board it, then launch into an open, scrolling flight
  section. Sparkpaw has a new seated cockpit pose and a compact craft designed
  to leave room for steering and combat.
- **Horizontal shooter controls.** Steer through the open air, dodge incoming
  fire and keep shooting as the landscape scrolls past. Stormrail changes the
  pace from on-foot platforming to a roughly two-and-a-half-minute flight route.
- **A changing storm landscape.** Fly past mountain silhouettes, rain-veiled
  valleys, waterfalls and ruined structures, with shifting colours, layered
  scenery and passing dust. The departure fades into the flight environment
  before the action builds toward the final gate.

### Formations, rewards and a debris field

- **Two new flight enemies.** Storm Darts and Pulse Orbs attack in mixed
  formations, including curling paths, crossing lanes and fan-shaped groups
  that split and rejoin.
- **Rewards for clearing formations.** Destroy a whole formation to reveal a
  diamond chain. Additional diamond patterns and heart pickups offer brief
  recovery opportunities between encounters.
- **A choreographed debris field.** Navigate broken masonry and larger pieces
  of ruin. Shoot destructible debris, avoid solid obstacles and collect the
  diamond or health rewards carried by selected objects.
- **A complete encounter sequence.** Opening formations lead into the debris
  field, followed by more demanding flight patterns and a short approach to
  the finale.

### The Harrier finale and Stormrail results

- **A boss guarding the sealed gate.** The Harrier enters a fixed-screen arena
  with room to manoeuvre. Read its charge warnings and dodge its spreading
  fan shots; as its health falls, it adds aimed bursts that reward changing
  direction.
- **New combat sounds and effects.** Distinct charge and firing sounds accompany
  the Harrier's attacks, with violet fan shots and distinct amber aimed shots.
- **A proper finish.** Defeat the Harrier, watch the gate open and fly through
  before the section's results appear.
- **Results for your flight.** Stormrail uses the familiar tally presentation
  for enemies, diamonds, time bonus and score, followed by REPLAY LEVEL or
  BACK TO TITLE.

### Updated editions

- **A complete two-disk ADF edition.** Disk 1 boots into the title and contains
  Level 1; Disk 2 contains Stormrail and its finale. The cinematic story remains
  in the HD and WHDLoad editions, while ADF retains title, loading, charging
  and the ready screen.
- **Automatic second-drive support.** If Disk 2 is already in DF1, the game
  uses it without asking you to swap disks. Single-drive setups receive
  INSERT DISK 1/2 prompts when a change is needed.
- **Matching disk-change artwork.** The prompts reuse the familiar floppy
  picture with the same cyan, bevelled lettering style as LOADING and CHARGING.
  Insert the requested disk and loading resumes automatically.
- **Refreshed HD and WHDLoad packages.** Both contain the full campaign and
  story introduction. WHDLoad retains its F10 exit to Workbench.

### Four original music themes

- **Hero Drive** accompanies the cinematic story introduction in HD and
  WHDLoad, giving the opening its own full four-channel theme.
- **Neon Sky** starts at the large title screen and continues through loading,
  CHARGING, READY and OPTIONS. Floppy players get this title theme too.
- **Copper Sprint** brings a driving 164 BPM accompaniment to Level 1,
  alongside plasma fire, jumping, impacts, hurt, pickups and splashes.
- **Iron Horizon** gives Stormrail a faster 172 BPM shooter theme: pulsing
  bass, wide chord accents, a returning melody and an atmospheric bridge
  before the action builds again. Its roughly 89-second loop accompanies
  departure, flight and the Harrier fight without an extra music load.
- **Music and effects together.** Both gameplay themes leave room for the
  existing effects, including health and extra-life pickups, debris and the
  Harrier's distinct attack warnings and firing cues.
- **Music follows the run.** Losing a life does not restart the song. Results
  stop gameplay music for the familiar tally sounds; REPLAY LEVEL starts the
  section's track afresh. Returning to the ready screen brings back Neon Sky.

### A little Amiga audio engineering

Paula still has only four hardware channels. Intro and title music use all
four; gameplay music is composed for three. A small software mixer combines
**two effect voices onto the fourth Paula channel**: one for rapid plasma
shots and one for the other effects, with priorities for important cues.
This creates room for music and overlapping effects without pretending the
Amiga has unlimited channels. Competing effects still have to share that
second voice; this is not unlimited polyphony.

Gameplay uses a ProTracker-compatible player driven by CIA timers, so the
music clock is separate from the rate at which the game draws frames. Sample
mixing runs in small blocks, with working data in Fast RAM and the final audio
buffers in Chip RAM for Paula. Only the active section's track is loaded.
These choices keep the soundtrack practical for the 68020/2 MB Chip + 8 MB Fast
target. They are not a promise that every scene or accelerator runs at 50 FPS.

### A livelier READY screen and cleaner transitions

- **Wind before the storm.** Small streaks cross the dark READY background
  from right to left, with cool colour changes and occasional orange sparks.
  The logo, artwork and menu text remain in front of the effect.
- **Smoother READY navigation on 68020.** Menu selection is more responsive,
  with fewer interruptions to the music and particles when entering OPTIONS.
- **A cleaner CHARGING-to-READY transition.** Preparation now happens while
  CHARGING remains visible, shortening the intervening black pause.
- **A lighter Stormrail load.** Stormrail needs less Chip RAM and avoids
  loading unused graphics. It still needs more than about 1.45 MB free Chip
  RAM in the ordinary HD edition; a precise free-memory minimum is not known.
- **Title music on floppy too.** The two-ADF edition includes Neon Sky and
  the animated READY screen while continuing to omit the cinematic intro.

- **More reliable intro skipping.** Left-mouse skip requests are retained
  through fades, and outgoing intro display DMA is stopped before the image
  memory is freed.
- **Visible loading for direct Stormrail starts.** Starting Stormrail from
  OPTIONS uses the loading presentation in HD and WHDLoad as well as ADF,
  instead of leaving a long unexplained black screen.

### Fitting the soundtrack onto real floppies

The ADF edition uses **two ordinary 880 KiB disks**. Music scores, sample banks
and effects are packed losslessly on disk and unpacked before use; the music
and sound samples are not reduced to lower-quality floppy versions. There is
no decompression in the gameplay audio interrupt and no streaming music load
halfway through the Harrier encounter.

Another space saving comes from READY's fixed particle-mask tables: about
39.3 KiB of tables now occupy approximately 6.3 KiB on floppy. They are loaded
once into Fast RAM, preserving the same particle masking and menu appearance.
Together with the existing lossless artwork packing, this leaves **15 KiB free
on Disk 1 and 159 KiB on Disk 2**. HD/WHDLoad retain ordinary unpacked assets.

Use the two alpha.5 ADFs together. Updated disk identification prevents mixing
these packed-audio disks with an older pair. Single-drive disk-change prompts,
automatic DF1 detection and resident replay remain part of the experience.
The story intro is still HD/WHDLoad-only; ADF starts with title music and READY.

### Compatibility and testing

PAL A1200/AGA, 68020 or faster, **2 MB Chip RAM and 8 MB Fast RAM** remain the
target. The HD and two-ADF music candidates have received positive user
emulator testing. Alpha.5 includes the new soundtrack in WHDLoad too, with the
existing F10 exit. The packaged builds are now available for real-hardware
testing; new WHDLoad audio, physical floppy/Gotek and Pocket results are not
yet being claimed as verified.

An ordinary HD launch still needs sufficient free Chip RAM after Workbench
and other software have taken their share. A previous setup with roughly
1.45 MB free Chip could run Level 1 but could not reliably start Stormrail;
a precise free-memory threshold is not established. The intermittent
real-Amiga HUD-boundary issue also remains an open test point.

This is still an alpha: Stormrail extends the adventure, but the next full
platforming level and the complete planned game are not included yet.
