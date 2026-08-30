# Sparkpaw itch.io publication copy

## Project settings

- **Title:** `Sparkpaw: The Stormstone Quest`
- **Project URL:** `mrdig.itch.io/sparkpaw`
- **Short description:** `A new AGA action-platformer built for the Amiga 1200.`
- **Classification:** Games
- **Kind of project:** Downloadable
- **Release status:** In development
- **Pricing:** Free
- **Tags:** Amiga, Commodore Amiga, Amiga 1200, AGA, Retro,
  Platformer, Action, Pixel Art, Singleplayer

## Main itch.io page

### A storm is waking in the ruins

**Sparkpaw: The Stormstone Quest** is an original side-scrolling action
platformer for the Commodore Amiga 1200. Play as Sparkpaw, an amber-orange
feline adventurer and young inventor armed with a teal energy gauntlet, and
cross ancient stone-and-machine ruins to recover the scattered Cores of the
Stormstone.

This public alpha contains the complete first-world route: twelve scrolling
screens of platforms, water hazards, ruined machinery, collectibles and
clockwork enemies, followed by the Stormkeeper's Waystation and the first
Stormstone Core. Run, crouch, jump, fire rapid plasma shots, fight Clockwork
Beetles and towering Storm Striders, and follow the distant storm-lit citadel
through a layered parallax landscape.

Sparkpaw is a real native Amiga game rather than a browser recreation or a
modern game wearing a retro skin. It is written in C for VBCC with direct use
of AGA bitplanes, Copper lists, the Blitter, attached hardware sprites, Paula
audio and PAL raster timing.

### Current alpha features

- One extended, completable Level 1 route and first Core-clearing sequence
- A 64-colour five-panel story introduction in the HD and WHDLoad editions
- Dedicated title, loading, charging and pre-level ready menu
- Ready-menu option assigns the joystick secondary button to Jump or Fire
- Dual AGA playfields with quarter-speed rear parallax
- A fluid 15-colour, 48x48 Sparkpaw built from an attached AGA sprite pair
- Running, variable jumping, crouching, crouch-walking and dedicated combat poses
- Rapid energy fire, animated impacts, health, lives and newly unified native
  Storm Shard artwork shared by the world and HUD
- Four-digit live score with one-time enemy awards and diamond points
- Animated Level Complete statistics for enemies, diamonds, time bonus and total
- A hidden Level-1 extra life with a falling `1UP` pickup and unique sound
- Near-instant `REPLAY LEVEL` flow after the results screen
- Clockwork Beetles and 64x64 Storm Striders with patrol, traversal, ranged
  attack, hurt, destruction and respawn behaviour
- Animated water hazards, splashes and ledge-balance animation
- Corrected one-shot Paula effects without repeated tails or truncated endings
- HD, bootable ADF and WHDLoad packages

This is an **in-development alpha**, not the finished five-world adventure.
Music, broader enemy variety, later levels, final progression and the complete
game-over/victory flow are still to come. Feedback from Amiga users is very
welcome, especially when the exact machine, accelerator, RAM, display setup
and package type are included.

### Requirements

- PAL Commodore Amiga 1200 or compatible AGA system
- 2 MB Chip RAM and 8 MB Fast RAM
- 68020 or faster CPU
- Joystick in port 2

The game is actively stress-tested at stock-68020 speed in FS-UAE. The most
complete supplied real-hardware acceptance so far is from an A1200 with a
roughly 34.5 MHz 68030; alpha.68 was reported successful there through physical
floppy/ADF, WHDLoad and the ordinary HD version. Performance on every
accelerator, FPGA core or exact stock configuration is not implied.

### Controls

- **Ready menu:** Up/down selects; Fire/Space confirms; Options uses left/right
  to assign the secondary button to Jump or Fire for the current run
- **Joystick:** left/right to run, up to jump, down to crouch and primary fire
  to shoot; the secondary button follows the ready-menu choice and defaults to
  Jump
- **Keyboard:** A/D to move, W to jump, S to crouch, Space to shoot
- **Story intro:** Fire reveals/advances; hold Fire or use the left mouse
  button to skip
- **WHDLoad:** F10 returns to Workbench

### Downloads

- **ADF:** bootable floppy image. To preserve floppy space it omits the five
  cinematic story plates, but retains the title, loading, ready screen and game.
- **HD:** extract the LHA or ZIP to a hard drive and launch Sparkpaw from its
  drawer.
- **WHDLoad:** LHA and ZIP packages for an existing WHDLoad installation;
  WHDLoad and Kickstart files are not included.

If you are unsure, use the HD or WHDLoad edition for the complete presentation.

### Development: MrDig + AI, one test at a time

This was not a one-prompt game. Before the story, here is what the repository
actually records.

#### Sparkpaw by the numbers — alpha.58 publication snapshot

- **23 days** from the first public repository baseline to alpha.58
- **14 active development dates** recorded in Git
- **65 Sparkpaw commits**
- **58 numbered alpha iterations** in the current development line
- **374 preserved test-evidence files**
- **81 gameplay and hardware-test videos**
- **90 diagnostic and performance logs**
- **52 screenshots** documenting presentation, bugs and hardware differences
- **150 written test reports and evidence sidecars**
- **174 preserved old test-build drawers**, including accepted references,
  rejected experiments and A/B candidates
- **26,352 lines** across the current C headers/sources, Python tools and
  Sparkpaw development/test documentation
- **1,743 simulated launch cases** for one particularly stubborn traversal fix
- Approximately **642 KiB of permanent Chip RAM** recovered during the
  real-hardware memory investigation, plus about **54 KiB** during status loading
- **48.58 effective FPS** in the low-overhead FS-UAE/68020 stress diagnostic
- **49.67 effective FPS** in the later 68020 Core-clearing performance run
- An estimated **80–120 combined development and testing hours**

The final number is deliberately a range. Git records commits, not the time
spent playing a build, moving files to a real Amiga, filming a CRT, comparing
individual video frames, discussing whether a platform looks wrong, or
discovering at midnight that the profiler has been profiling itself into a
slower game. The evidence makes “dozens of hours” an understatement and makes
**roughly one hundred hours of collaboration** a fair description, but it is
still an estimate rather than a timesheet.

Sparkpaw was created through an intensive collaboration between **MrDig** and
**AI coding agent Codex**. MrDig set the creative direction, played every
meaningful build, judged the art and feel, and supplied the evidence that an AI
cannot manufacture: recordings, logs and observations from FS-UAE, a real
A1200, physical ADF boots, WHDLoad and an Analogue Pocket. Codex helped design
and implement the engine, assets and tooling, analyse evidence, propose
isolated experiments, build test packages and preserve the results.

What made the project hard was not generating a hero image or making a sprite
move. The hard part was making many Amiga subsystems agree every PAL frame.
The foreground, parallax, Copper, Blitter Bobs, hardware sprites, HUD and audio
all compete for finite Chip RAM, DMA time and raster time. A change that looked
perfect in a fast emulator could hitch at 68020 speed, expose a seam through an
Indivision, corrupt a Bob on real hardware or fail only when Workbench left too
little free Chip RAM.

That produced a deliberately empirical workflow: change one variable, package
a named A/B build, test at 68030 and 68020 speed, inspect video frame by frame,
read the CIA/raster timings, reject the losing hypothesis and retain the
evidence. Many optimisations were correctly thrown away. Failed work was not
hidden; it became the map that led to the stable build.

## Launch devlog

### Building Sparkpaw the hard way: 22 days inside the AGA

Sparkpaw began as a playable AGA platformer prototype, but turning that into a
public alpha meant solving a chain of problems that only become visible when a
game meets an actual Amiga frame budget. Or, put differently: drawing a cat is
easy; persuading Alice, Lisa, Paula, the CIAs, the Copper, the Blitter, FS-UAE,
WHDLoad, an FPGA and a thirty-something-year-old physical machine to agree on
where that cat is every 1/50th of a second is where the hobby begins.

**5–6 August — turning a prototype into a maintainable game.** The original
code was split into player, collision, projectile, enemy, audio, asset,
renderer and platform modules. A real application-state flow followed, with a
title and loading presentation displayed directly through the AGA Copper.

**11–13 August — from movement test to combat game.** Sparkpaw gained polished
animation, contact damage, health, lives, Storm Shards and improved Clockwork
Beetles. The larger 64x64 Storm Strider forced a move to the production AGA
4+3 dual-playfield layout. Its behaviour grew from patrol and turn poses into
authored jumps between surfaces, persistent off-screen simulation, ranged
attacks, hit reactions, destruction and respawning. The important part was not
just adding states; every animation slot, palette, mask, grounding point and
background restore had to remain compatible with the Blitter renderer.

Sparkpaw himself grew into a surprisingly demanding little customer. His
visible 48x48 body uses fifteen colours and originally occupied three attached
hardware-sprite pairs: six of the Amiga's eight sprite DMA channels. That buys
smooth movement and a colourful hero, but leaves very little room for casual
“we'll just use another sprite” thinking. Crouching, firing low, turning,
jumping, falling, landing, getting hurt and balancing at a ledge all needed
append-only animation slots, stable feet, mirrored anchors and compatible
palette ownership. A one-pixel grounding error is much more noticeable when
the alleged agile hero appears to hover over the floor like a nervous Roomba.

**14–15 August — making Level 1 look and play like a world.** Memory proofs
established how far the resident level could grow. Water, splashes, ledge
feedback and a twelve-screen route were added. The rear playfield was
re-authored several times to carry storm clouds, mountains, forest, ruins,
waterfalls and a recognizable distant citadel at quarter-speed scroll. Copper
palette bands increased perceived colour depth without adding another
bitplane. Foreground kits were also rejected and replaced until the platforms
read as ruined stone-and-machine architecture rather than repeated boxes.

The final gameplay display is AGA dual playfield in a four-plus-three-plane
arrangement: FRONT16 for the foreground and REAR8 for the distant landscape.
AGA does not offer the tempting five-plus-three combination, no matter how
politely one asks. The rear bitmap moves at one quarter of the camera speed and
uses twelve horizontal-blank Copper palette changes to give clouds, mountains
and forest their own colour character without paying for another bitmap plane.
That is the sort of trick which feels gloriously excessive until one remembers
that the Copper was specifically invented to do gloriously excessive things
while the CPU is busy elsewhere.

Traversal required its own forensic loop. Several “fixed” final ascents still
failed at particular approach speeds or collision orderings. Host simulations
eventually covered 1,743 launch cases, but real play still revealed that the
actual blockage was earlier in the route. The lesson was uncomfortable and
useful: an exhaustive test of the wrong location is still the wrong test. The
computer was extremely confident; Sparkpaw was still stuck on the scenery.

**15–20 August — the renderer fight.** Intersecting Striders exposed flicker,
so restore rectangles were ordered and overlapping regions merged. Projectile
collision was changed to sweep every crossed pixel so a crouched shot could
not tunnel through a narrow pillar. Stock-68020 profiling then showed that the
otherwise-correct renderer missed too many frame deadlines.

Several plausible fixes failed. Tight enemy rectangles damaged sprites. A
camera-local hidden viewport copied too much and became slower. Early rolling
renderer prototypes produced corruption, trails, periodic scroll hitches and
HUD boundary artefacts. AGA fetch experiments exposed half-step origins,
left-edge stripes, lost sprite DMA and line-252 HUD seams. Each failure was
packaged and measured instead of being explained away.

The Bob problem deserves its own small monument. Enemies, collectibles,
projectiles, splashes and animated water are Blitter objects drawn into the
foreground. Before a Bob moves, its old background has to be restored; after
the world updates, it has to be drawn again in the correct family order. Two
Striders crossing each other exposed tiny head and crest flickers because
their restore regions interacted. Stable top-to-bottom ordering fixed the
separated case; intersecting restore rectangles then had to be unioned so
shared background was restored exactly once. “Just draw the robot” had become
rectangle topology synchronized to the raster beam, as tradition demands.

The accepted Stage 5L design uses two hidden/display target pairs, a logical
512-pixel FRONT16 ring repeated across a seam-safe physical stride, a resident
clean world as the canonical source for entering columns, and two Copper lists
that publish target and pointer state atomically. The CPU and Blitter never
modify the displayed target. Sparkpaw's original six-channel sprite layout was
also repacked into one transparent-padded 64-pixel attached AGA pair, keeping
the same visible 48x48, 15-colour art while avoiding DMA loss at the wider fetch.

The HUD performs a Copper switch at hardware line 252 and the dynamic Bob pass
is synchronized immediately below it. This boundary was responsible for an
impressive collection of thin seams, stray pixels and intermittent remnants.
The eventual fix was not “add enough waits until it stops”; it was to establish
which playfield owned the corruption, guard the fetch geometry and preserve an
atomic display contract. A useful Amiga rule emerged: if a glitch is only one
scanline high, it can still consume an entire evening.

**19–25 August — emulator truth versus hardware truth.** One HD build worked
in FS-UAE and from ADF on the real machine, yet failed from a normal real
Workbench. Testing with different free-memory states isolated the cause:
available Chip RAM, not the hard drive or renderer. Moving master assets to
Fast RAM and staging only what the custom chips needed saved about 642 KiB of
permanent Chip RAM plus roughly 54 KiB during status loading.

This was a particularly good example of emulator truth versus hardware truth.
The same executable could behave under FS-UAE, boot from physical floppy on the
real A1200 and then collapse after CHARGING when launched from that A1200's
ordinary Workbench. The decisive “debugger” was booting with different amounts
of free Chip RAM. At roughly 1.92 MB free it worked; around 1.43 MB it did not.
No amount of staring suspiciously at MaxTransfer could change that arithmetic.

Performance was equally configuration-specific. A fast FS-UAE/68030 run could
report 50.00 FPS while FS-UAE/68020 exposed missed fields and a real 34.5 MHz
68030 showed different cadence. Dozens of paired Stage-2 tests compared Copper
copying, Bob setup, collision spans, enemy caching, ring-column updates,
collectible redraw policies and Blitter/CPU alternatives. Some
“optimisations” were slower or visually wrong. The accepted combination and
lower-overhead instrumentation brought the 68020 stress run to 48.58 effective
FPS; the later Core-clearing run measured 49.67 FPS. It also revealed that the
profiler itself had been consuming several apparent frames per second.

The optimisation graveyard is worth listing because it tells more truth than a
victory graph. Longword stores were not automatically better. Blitter copies
were not automatically better. Tight per-frame enemy bounds saved work and
damaged sprites. A hidden camera-local viewport sounded elegant and generated
thousands of wraps while making both 68020 and 68030 cadence worse. Some
collectible dirty-state strategies flickered; some enemy-cache shortcuts
corrupted beetles or Striders; some wider-fetch settings ate hardware-sprite
components; some HUD fixes merely moved the seam. There are 171 preserved old
test drawers because “rejected” is a result, not a request to quietly delete
the evidence.

Timing was captured with CIA and raster instrumentation, split across ring
rolling, Bob setup, enemy, projectile, collectible, water, Copper and other
families. That instrumentation also taught us observer effect in its most
literal form: nested profiling scopes and per-family raster sampling made the
game look several FPS slower. After removing most of the measuring apparatus,
the patient immediately improved. This is medically frowned upon but perfectly
normal in performance engineering.

**26–27 August — making an engine build feel like a release.** Level 1 gained
the Stormkeeper's Waystation and its animated Core reward. Five story plates,
a dedicated ready screen, secondary-button jumping, WHDLoad packaging, a clean
F10 return to Workbench and dual-layer NewIcon/classic Workbench icons followed.
Even here hardware had the last word: a one-pixel COLOR00 border hidden by
emulator and CRT overscan was visible through an Indivision, and a colourful
classic icon fallback mapped to the wrong Workbench pens. Both were corrected
and added to the project's growing list of explicit contracts.

This is why I describe Sparkpaw as an AI collaboration, not an AI-generated
game. AI made it possible to explore and document an unusual number of ideas
quickly. Human taste, repeated play, physical hardware and the willingness to
say “this build is rejected” decided which ideas survived. The current alpha
is the accumulated result of that conversation.

## Current alpha.68 upload labels

- `Sparkpaw 0.6.0 alpha.68 — Bootable ADF`
- `Sparkpaw 0.6.0 alpha.68 — HD version (LHA)`
- `Sparkpaw 0.6.0 alpha.68 — HD version (ZIP)`
- `Sparkpaw 0.6.0 alpha.68 — WHDLoad (LHA)`
- `Sparkpaw 0.6.0 alpha.68 — WHDLoad (ZIP)`
