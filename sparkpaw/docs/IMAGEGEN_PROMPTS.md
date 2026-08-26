# Built-in ImageGen prompts

These project-bound concept assets were generated with the built-in ImageGen tool and then copied into this workspace.

## Title concept

Create original 4:3 title-screen concept art for the Commodore Amiga 1200 AGA game `SPARKPAW`, subtitle `THE STORMSTONE QUEST`. Show layered storm clouds and mountains, ruined energy towers, forest silhouettes and a rocky technological foreground. Sparkpaw is an original amber-orange young feline adventurer with a navy hood/scarf and teal energy gauntlet. Use authentic premium 1990s Amiga AGA pixel art, crisp clusters and a visually limited palette. Keep both title lines legible. Do not resemble or copy any existing licensed character, logo or artwork.

## Gameplay and parallax concept

Create an original side-view Amiga AGA gameplay scene demonstrating three readable parallax depths: violet storm mountains and clouds, pine forest with broken energy towers and waterfalls, and mossy technological ruins with cyan circuitry. Show Sparkpaw running right and firing a cyan energy bolt, one clockwork beetle enemy and three Stormstone collectibles. Use crisp 1990s Amiga pixel art, a 4:3 320x256-inspired composition and a compact bottom HUD. Keep platforms readable and do not copy existing game art.

## Full-width gameplay HUD source v1

Reference: `sparkpaw-gameplay-concept.png`.

Create a single full-width horizontal bottom HUD bar matching the reference
artwork as closely as possible: chunky 1990s Amiga pixel art, dark navy inset
panels, heavy steel borders, rivets, cyan energy accents and warm amber
highlights. Include a faithful small Sparkpaw portrait, exactly three red heart
containers for current health, and a separate Sparkpaw head with the exact
text `x 3` for lives. Include a diamond/Stormstone panel prepared for a future
counter and one additional empty framed panel. Use one straight shallow strip
with no gameplay scene, perspective or background scenery. Keep hard pixel
edges and avoid gradients that become noise after eight-colour quantisation.

## Level-disk loading concept

Create original 4:3 loading-screen concept art for the Commodore Amiga 1200
AGA game `SPARKPAW: THE STORMSTONE QUEST`. On a near-black storm-blue
background, show one large three-quarter-view navy floppy disk as a physical
level artifact. Its label contains an original cyan Stormstone crystal emblem,
amber paw-shaped registration marks and restrained metallic circuitry. Add a
small cyan disk-access light and a few angular electric accents. Use crisp
premium 1990s Amiga AGA pixel art, large deliberate clusters, hard edges and a
visually limited palette. No characters, logos, instructions or loading text.
Do not reproduce any existing game's disk, box art, typography or composition.

The selected built-in Codex ImageGen source is preserved as
`assets/concept/sparkpaw-level-loading-concept-v2.png`. The project converter
uses a fixed centred 75-percent crop, fits it to 320x256 with Lanczos and uses
Fast Octree quantisation without dithering to preserve the cyan crystal and
amber paw details without spending palette entries on dark background noise.
It writes the indexed 64-colour preview and the matching six-plane SPBM runtime
asset deterministically. The presenter uses AGA border blanking rather than
sacrificing an image pen for neutral borders. Pillow performs format conversion
only; it does not draw or replace the authored source art.

## Simplified level-disk loading concept v3

Create a cleaner original Sparkpaw loading screen for a Commodore Amiga 1200
AGA game. Use the established title art as the palette and style reference and
the earlier Stormstone floppy only as an identity reference. Centre one compact,
front-facing fantasy-tech 3.5-inch floppy on pure black, with broad midnight
navy and steel pixel clusters, a large cyan Stormstone crystal, two restrained
amber paw accents and one cyan access light. Put the exact word `LOADING` below
it in large original angular cyan pixel lettering with a navy outline and small
amber corner accents. Preserve generous, deliberate black negative space and
make every shape readable after reduction to 320x256 and 64 indexed colours.
Avoid characters, scenery, smoke, gradients, tiny circuitry, texture noise,
extra text, logos, watermarks and copied typography or disk designs.

The built-in ImageGen result is preserved as
`assets/concept/sparkpaw-level-loading-concept-v3.png`. Unlike v2, it is already
a 4:3 composition and is fitted directly to 320x256 without the old 75-percent
crop. Fast Octree quantisation without dithering remains deterministic and
preserves the crisp cyan, amber and steel clusters.

For runtime presentation, both title and loading conversions reserve palette
pen 0 as pure black. This hides the otherwise coloured one-pixel `COLOR00`
border exposed by the Indivision AGA MK2 but normally outside CRT overscan. The
loading conversion swaps in its existing black pen without RGB changes. The
title merges its least-used colour into the nearest remaining palette entry
and preserves the original pen-0 colour in the vacated slot. The remapped title
is written as `assets/concept/sparkpaw-title-aga64-runtime-preview.png`; the
accepted indexed source preview remains unchanged.

### Silent preparation status variants

Using `sparkpaw-level-loading-concept-v3.png` as the edit target, change only
the bottom status text from `LOADING` to the exact phrase
`CHARGING STORMSTONE`. Preserve the floppy, Stormstone crystal, paws,
composition, black background, colours, hard-edged pixel-art treatment,
lighting and proportions. Centre the phrase on one line below the disk in a
smaller version of the same cyan/white/navy lettering with the same restrained
gold corner accents. Add no other objects, text, gradients or borders.

The built-in ImageGen edit is preserved as
`assets/concept/sparkpaw-level-charging-stormstone-concept-v1.png`. At build
time it is mapped onto the already-generated loading-screen palette, so the
runtime status switch cannot introduce a palette flash.

Hardware testing showed that the long phrase was visually subordinate to the
much larger `LOADING` word. A second built-in ImageGen edit changes only that
word to the exact text `CHARGING`, matching the original word's large height,
weight, cyan/white/navy treatment and gold corner accents while preserving all
other artwork. This accepted runtime source is stored as
`assets/concept/sparkpaw-level-charging-concept-v2.png`; v1 remains preserved
as design history.

## Animation reference sheet

Create a strict 4x4 pixel-art animation reference sheet for Sparkpaw on a perfectly uniform `#ff00ff` chroma-key background. Maintain identical costume, proportions and palette. Include exactly: idle, blink, four run poses, jump rise, jump apex, fall, land, crouch, aim, shoot, hurt, victory and death tumble. Use a shared limited 16-colour character palette, hard pixels, equal cells, no text, shadows, grid lines or licensed character resemblance.

## Airborne shooting refinement

Production sprite sheet: exactly four evenly spaced full-body Sparkpaw airborne
shooting poses, all facing right, matching the supplied jump sprite scale and combat
gauntlet design. Sequence rise/apex/fall/late-fall. Gauntlet aimed horizontally right
in all poses, tiny cyan muzzle glow only in frame 3. Pure flat `#ff00ff` background,
crisp hard-edged 1993 Amiga AGA pixel art, no text, no crop, no shadows and no
projectile trails. The chroma background is removed locally before palette reduction.

## Crouching shooting refinement

Production sprite sheet: exactly four evenly spaced full-body Sparkpaw
crouch-shooting poses in one horizontal strip, all facing right, using the
established crouch sheet for anatomy, scale, body mass and foot/knee baseline,
and the grounded combat sheet for the teal gauntlet barrel and recoil language.
Sequence crouched ready/raise, horizontal aim, discharge/recoil with a tiny
attached cyan muzzle glow, and recover. Keep every pose genuinely crouched at
one family-wide scale. Pure flat `#ff00ff` background, crisp hard-edged 1993
Amiga AGA pixel art, no text, crop, shadows, grids, projectile trails or loose
effects. The chroma background is removed locally before palette reduction.

## Clockwork beetle production replacement v2

Production enemy sheet: exactly nine isolated left-facing clockwork-beetle
poses in a clean 3x3 grid, closely matching the beetle in the Sparkpaw gameplay
concept and the established Sparkpaw sprite's pixel density. Use a squat heavy
brass-and-dark-steel body, large circular face plate with a bright cyan glass
lens, tall rounded segmented shell, black recessed joins, six sturdy jointed
legs and two compact ball-tipped antennae. Frames 0-3 are one consistent walk
cycle; frame 4 is the first-hit recoil with a brief cyan electrical crack;
frames 5-8 progress through sparking damage, collapsed shell, scattered parts
and tiny fading debris. Preserve one body scale and ground baseline across the
walk and hit poses. Use crisp premium 1990s Amiga AGA pixel clusters on a flat
`#ff00ff` chroma background. No text, labels, grid lines, scenery, floor,
shadows, gradients, watermark or unrelated effects.

The selected built-in ImageGen source is preserved as
`assets/enemies/clockwork-beetle-concept-v2-chroma.png`; the locally keyed
source is `clockwork-beetle-concept-v2-transparent.png`. The runtime generator
crops its 3x3 cells, applies one shared scale to the walk family and hit body,
bottom-aligns them to the accepted row-22 ground line and maps opaque pixels to
the fixed eight-colour foreground palette. Destruction frames may only shrink
to keep their spreading parts inside the same 32x24 cell. Runtime mirroring,
frame IDs, collision and Bob dimensions remain unchanged.

## Clockwork Storm Sentinel concept v1

Use case: stylized-concept. Phase 5 enemy concept sheet for a 1993 Commodore
Amiga AGA action platformer. Use the canonical Storm Ruins gameplay concept as
the world and colour-language reference, and the accepted clockwork beetle v2
sheet as the mechanical-family, material and damage-language reference.

Design one original medium-sized **Clockwork Storm Sentinel**, related to the
beetle but clearly a new role rather than a larger beetle. It is a stocky
two-legged ancient ruin guardian, roughly 1.2 times Sparkpaw's standing height,
with a broad dark-steel torso, weathered brass armour, moss nicks, a single
bright cyan glass core, piston legs and one oversized forearm that doubles as
an energy shield and short-range battering ram. Use a hunched, low-centred
silhouette with shield arm forward and a compact rear counterweight.

Show exactly four isolated right-facing full-body poses in one horizontal row:
guarded idle, slow heavy walk/contact, unmistakable cyan charge telegraph and a
short forward ram/recovery. Preserve one scale and ground baseline. Use crisp
premium early-1990s Amiga AGA pixel clusters on a perfectly flat `#ff00ff`
background. Dark navy/steel and muted violet dominate; restrained aged brass
connects it to the beetle; cyan is reserved for core, conduits and telegraph;
tiny moss-green wear accents are allowed. No text, labels, UI, grid, scenery,
floor, shadow, gradients, blur, watermark, extra enemies, projectile weapon,
jumping pose, humanoid-knight cliché or licensed resemblance.

The built-in ImageGen result is preserved as
`assets/enemies/clockwork-storm-sentinel-concept-v1-chroma.png`; the locally
keyed review source is `clockwork-storm-sentinel-concept-v1-transparent.png`.
This is concept art only. Do not directly auto-fit these rich large poses into
runtime cells. After visual acceptance, create a dedicated native-minded
production sheet with a fixed cell, shared scale/anchor and an explicit
eight-colour foreground mapping.

## Clockwork Storm Strider concept v1

Use case: stylized-concept. Alternative Phase 5 enemy concept using the Storm
Ruins gameplay concept and accepted beetle as world/family references, while
contrasting directly with the heavy Sentinel candidate.

Design one original medium-sized **Clockwork Storm Strider**: lean,
spring-loaded and fast, with a narrow dark-steel torso, long digitigrade piston
legs, compact aged-brass armour, swept-back stabilizer tail, small shoulder
vanes and one cyan glass core/visor. Its construction must support future
sprinting, crouch compression and committed leaps between authored platforms.
It is mechanical, not feline, humanoid or a larger beetle.

Show exactly four isolated full-body left-facing poses in one row: alert
stalking idle, fast low run contact, unmistakable crouched jump-load telegraph
and a long forward airborne leap with tucked legs. Preserve one anatomy and
body scale; grounded poses share a foot baseline. Use crisp early-1990s Amiga
AGA pixel clusters and prioritize small-scale limb readability. Dark
navy/steel and muted violet dominate, restrained brass links the family, and
cyan is reserved for visor/core, leg conduits and the charge. Flat `#ff00ff`
background. No text, grid, scenery, floor, shadow, loose effects, projectile,
shield, bulky torso, beetle plan, cat/fox/wolf likeness, humanoid knight or
licensed resemblance.

The built-in ImageGen source is preserved as
`assets/enemies/clockwork-storm-strider-concept-v1-chroma.png`; its locally
keyed review version is `clockwork-storm-strider-concept-v1-transparent.png`.
It is a concept candidate only. If selected, a dedicated fixed-cell production
sheet and eight-colour indexed preview must establish the actual dimensions,
anchors, collision and leap poses.

## Clockwork Storm Strider production v2

Use case: production-oriented sprite source. Create exactly eighteen isolated
right-facing full-body animation poses of the accepted Clockwork Storm Strider
in a strict 6-column by 3-row sheet, read left-to-right and top-to-bottom:
alert idle, alert weight shift; four coherent fast run phases; jump compression
start and charged compression; launch, long airborne flight and descent;
landing impact and recovery; hit recoil; core fracture, collapse, energy burst
and scattered debris destruction stages.

Preserve one anatomy and one body scale across frames 1-14: lean compact
dark-steel torso, long digitigrade piston legs, swept stabilizer tail, restrained
aged-brass armour, small shoulder vanes and one cyan glass visor/core. Grounded
poses share one foot baseline. The run must loop cleanly, compression must read
before launch, flight must travel horizontally, and the four destruction poses
must progress rather than introduce a different creature. Use crisp premium
1993 Amiga AGA pixel-art clusters with strong small-scale silhouettes on a
perfectly flat `#ff00ff` chroma background. No text, labels, grid lines,
scenery, floor, shadow, blur, gradients, shield, projectile, extra enemy,
humanoid or feline anatomy, or licensed resemblance.

The built-in ImageGen result is preserved as
`assets/enemies/clockwork-storm-strider-production-v2-chroma.png`; the keyed
source is `clockwork-storm-strider-production-v2-transparent.png`. The runtime
generator crops the 6x3 cells, applies one shared scale to frames 0-13, maps
opaque pixels deterministically to the fixed eight-colour foreground palette,
and creates exact mirrored cells. Destruction stages may only shrink when
their spreading parts exceed the 46x38 opaque envelope. The review preview is
`clockwork-storm-strider-48x40-aga8.png`; the packed asset has no gameplay
consumer yet. This quadruped v2 source is preserved as review history but was
superseded after the first in-game scale review.

## Clockwork Storm Strider biped concept v2

Use case: stylized-concept. Redesign the Strider as a permanently upright
bipedal agile enemy. It may retain canine/jackal cues in its swept sensor head,
ear-like vanes and stabilizer tail, but must never stand, run, compress or jump
on four legs. Show exactly four isolated right-facing poses in one row: tall
alert idle, fast bipedal run contact, deep two-legged jump compression and an
upright forward leap with both legs tucked. Preserve one anatomy, scale and
ground baseline. It should stand about ten percent taller than Sparkpaw and be
unmistakably larger than the beetle. Use long reverse-jointed piston legs, a
narrow substantial torso, two short balance arms, dark steel, restrained aged
brass and one cyan visor/core. Flat `#ff00ff` background. No quadruped pose,
hands on ground, robot-dog gait, humanoid knight, text, floor or scenery.

The accepted built-in ImageGen result is
`clockwork-storm-strider-biped-concept-v2-chroma.png`; its keyed review source
is `clockwork-storm-strider-biped-concept-v2-transparent.png`.

## Clockwork Storm Strider biped production v4

Use the accepted biped concept as locked anatomy. Create exactly eighteen
isolated right-facing poses in a strict 6x3 sheet: alert idle, weight shift;
four upright run phases; two two-legged compression phases; upright launch,
compact tucked flight and descent; two-foot landing and recovery; hit recoil;
core fracture, mechanical collapse, cyan burst and debris. Preserve one body
scale across frames 1-14 and one grounded foot baseline. The tall idle should
define the perceived height; tuck tail, arms and legs during motion so no wide
pose shrinks the family. Hands never touch the floor. Crisp early-1990s AGA
pixel clusters on flat `#ff00ff`; no text, grid, scenery, shadow, projectile,
shield, quadruped anatomy or licensed resemblance.

The accepted sources are
`clockwork-storm-strider-biped-production-v4-chroma.png` and
`clockwork-storm-strider-biped-production-v4-transparent.png`. Deterministic
generation maps them to the fixed eight-colour foreground palette in a 64x64
cell with a shared scale and mirrored direction. The native proof is
`clockwork-storm-strider-64x64-aga8.png`. An unused quadruped production-v3
experiment is preserved only as rejected scale-review history.

## Clockwork Storm Strider palette/polish v5

Edit the complete bipedal v4 sheet without changing its exact 6x3 layout,
eighteen poses, pose order, anatomy, scale, baselines or action meanings. The
in-game v4 review shows that shared aged-brass colours make Sparkpaw, beetles
and Strider all read orange. Repaint the Strider with dominant deep navy/black
steel, large coherent muted-violet armour panels, restrained royal-blue plates
and bright cyan only for visor, core and clean conduits. Use tiny cool-cream
highlights only when essential; use no orange, gold, brass, copper, amber, red
or warm brown.

Improve the native-size craftsmanship with slightly thicker legs and forearms,
stronger chest/hip masses, fewer tiny mechanical marks, larger intentional
pixel clusters, a clean dark outline and consistent top-left lighting. Preserve
the permanent bipedal motion and destruction sequence. Flat `#ff00ff` chroma;
no text, grid, scenery, shadow, blur, quadruped anatomy, shield or projectile.

The built-in ImageGen result is preserved as
`clockwork-storm-strider-biped-production-v5-chroma.png`; the keyed source is
`clockwork-storm-strider-biped-production-v5-transparent.png`. It supersedes v4
as the generator input without changing the accepted 64x64 runtime contract.

## Player hurt refinement v10

Production sprite strip: exactly four evenly spaced full-body Sparkpaw
hurt/recoil poses, all facing right and matching the established character
references in anatomy, head size, amber fur, cream muzzle, navy scarf, brown
boots and teal energy gauntlet. Sequence sudden contact impact, backward slide,
brief airborne recoil and controlled recovery/landing. Preserve one family
scale and coherent ground baseline; keep the gauntlet on the same physical arm.
Use crisp premium 1993 Amiga AGA pixel art on a perfectly flat `#ff00ff` chroma
background. No death pose, detached impact effects, blood, text, labels, grid,
floor, shadows, gradients or watermark.

The selected built-in ImageGen source is preserved as
`assets/sprites/sparkpaw-hurt-v10-chroma.png`; its locally keyed counterpart is
`sparkpaw-hurt-v10-transparent.png`. The generator appends the four poses as
frames 50-53 with one family-wide scale and deterministic mirrored-left cells.
Accepted frame IDs 0-49 are unchanged.

The first runtime review found the airborne recoil too upright and the recovery
too close to a neutral idle. A focused built-in ImageGen edit preserves poses
1-2, removes source residue near the sliding feet, tilts pose 3 into a compact
backward trajectory with tucked knees, and makes pose 4 a low hand-braced
landing bridge. The refined sources are
`assets/sprites/sparkpaw-hurt-v12-chroma.png` and
`sparkpaw-hurt-v12-transparent.png`; v10 remains preserved as review history,
while the runtime generator now consumes v12.

## Player crawl-hurt refinement v13

Production sprite strip: exactly four evenly spaced full-body Sparkpaw
crouch-hurt poses, all facing right and remaining below the established
crouch/crouch-fire silhouette height. Sequence compact crouched impact, low
backward slide, tucked low recoil and stable crouched recovery. Preserve the
same anatomy, amber/cream fur, navy scarf, boots and teal gauntlet, with one
family scale and the established ground baseline. Use crisp premium 1993 Amiga
AGA pixel art on flat `#ff00ff` chroma. No standing pose, tall airborne pose,
detached effects, blood, text, grid, floor, shadows, gradients or watermark.

Runtime review showed that v11 still read as seated and visibly exceeded the
physical crouch clearance. The focused v13 edit keeps all four poses horizontal:
low impact on forearms and knees, backward crawl-slide, head-tucked compression
and crawl recovery. The selected built-in ImageGen source is preserved as
`assets/sprites/sparkpaw-crouch-hurt-v13-chroma.png`; its keyed counterpart is
`sparkpaw-crouch-hurt-v13-transparent.png`. Frames 54-57 remain unchanged, and
their deterministic conversion caps the complete family at 24 opaque pixels.
The v11 sources remain preserved as review history.
# Phase 5F.2A Strider storm-gauntlet source

Built-in image generation edit, using
`clockwork-storm-strider-premium-idle-source.png` as the authoritative edit
target and character/style reference:

> Preserve the Clockwork Storm Strider exactly, but replace only the
> forward/right claw hand and lower forearm with a compact integrated
> storm-energy gauntlet/short arm cannon aimed horizontally right. Match the
> premium pixel-art scale, outline, metallic shading and indigo/violet/black/
> cyan palette. Keep the exact full-body pose, proportions and green chroma-key
> canvas. The tapered weapon should have roughly the mass of the original claw
> plus forearm, a small circular white-cyan muzzle and subtle violet conduits.
> Preserve head, visor, crest, torso, shoulder, left blade arm, hips, legs,
> feet, tail and lighting. No projectile, muzzle flash, extra limb, detached
> weapon, belly mount, oversized rectangle, text or watermark.

The generated chroma source and locally keyed transparent source are retained
as `clockwork-storm-strider-premium-shoot-source-v1-{chroma,transparent}.png`.
Runtime generation fits the complete transparent actor once and derives slots
9/10 from the same anatomy.

## Phase 6B water-hazard concept v1

Built-in image generation used
`assets/concept/sparkpaw-gameplay-concept-aga64-preview.png` as the strict
style, palette, scale, HUD, ruins and parallax reference:

> Create a gameplay-scene concept showing an unmistakable 80-pixel-wide
> water-filled break in the stone floor between two ruin platforms. The gap
> must read mechanically as a deadly fall/restart hazard, with a dark
> storm-water body below the walkable edge, a crisp cyan/blue-violet surface
> line, sparse pale foam highlights and wet mossy stone lips. Suggest depth
> through a dark vertical chasm and restrained reflections. Preserve the
> side-on AGA platformer camera, established navy/violet/cyan/teal/moss palette,
> bottom HUD and character design. Keep the gap five 16px tiles wide. Do not
> add text, labels, checkpoint, new enemy, UI changes or a broad bright-blue
> flat fill.

The built-in ImageGen result is preserved for review as
`assets/concept/sparkpaw-water-hazard-concept-v1.png`. It is concept-only and
must not become runtime art until MrDig accepts its visual direction.

### Water-hazard concept v2 colour/floor correction

V1 used the already quantized AGA64 preview as its strict visual reference and
therefore inherited flatter violets, lower contrast and cooler midtones. The
focused v2 built-in ImageGen edit instead uses the full-colour
`assets/concept/sparkpaw-gameplay-concept.png` as the authoritative edit target:

> Change only one ordinary floor section into a deadly water gap exactly five
> 16px tiles (80px) wide. Keep the left and right walking surfaces at the same
> vertical height as the source. Wet stone lips may rise only 5–10 pixels above
> that surface—no tall banks, raised floor or massive retaining blocks. Preserve
> the source's vivid saturation and strong contrast exactly. Fill the opening
> below the normal floor line with dark navy/violet storm water, a crisp cyan/
> blue surface, sparse white foam pixels, subtle depth and restrained bubbles.
> Preserve Sparkpaw, HUD, parallax, ruins, vegetation and platform scale.

The result is preserved as
`assets/concept/sparkpaw-water-hazard-concept-v2.png`; v1 remains as rejected
review history. V2 is still concept-only pending MrDig's approval.

### Water-hazard concept v3 thin-floor correction

MrDig clarified that “floor height” meant the full visible vertical mass below
the walking line, not an elevated edge. The supplied crop identified v2's large
ruin foundation as the rejected area. Consecutive review of the supplied
`testresults/thundercats-level1.mov` and screenshot was used only for the broad
layout principle of a thin bottom floor interrupted by water; no map, character,
art, palette, object or UI was copied.

The focused built-in ImageGen edit uses v2 as its Sparkpaw edit target:

> Reduce the ordinary floor on both sides to a thin low platform strip: one
> 16px tile plus at most a small underside lip. Remove the huge stone foundation,
> tall masonry, embedded machinery and vegetation mass below the walking line,
> revealing forest/parallax negative space. Preserve the walking-surface height
> and the five-tile jump distance. Keep tiny broken end caps only. Place dark
> navy/violet water with a narrow cyan/white highlight in the interruption near
> the thin floor strip. Preserve Sparkpaw's vivid palette, character, HUD,
> platforms, enemy and diamonds. Do not copy the reference game's artwork.

The result is preserved as
`assets/concept/sparkpaw-water-hazard-concept-v3.png`; v1 and v2 remain review
history. V3 is concept-only pending MrDig's approval.

### Water-hazard concept v4 HUD-ground correction

MrDig accepted v3's approximate floor thickness but identified the exposed
forest band between that floating floor and the HUD as incorrect. The v4
built-in ImageGen edit uses v3 as its edit target and the supplied crop as the
exact problem reference:

> Remove the entire exposed forest/parallax band below the ground. Move the
> complete thin ground strip downward until its underside touches the HUD's top
> border with no gap. Preserve the modest stone/moss thickness; do not recreate
> a massive foundation. Move the five-tile water interruption down with it so
> the water occupies the low break and meets the HUD boundary cleanly. Preserve
> the saturated palette, character, enemy, HUD and surrounding world.

The result is preserved as
`assets/concept/sparkpaw-water-hazard-concept-v4.png`; earlier versions remain
review history. V4 is concept-only pending MrDig's approval.

## Sparkpaw ledge-teeter review strip v1

Built-in ImageGen edit/reference workflow, 2026-08-14. Review-only source; not
yet consumed by the runtime generator. Reference images were the authoritative
`sparkpaw-sprites-reference-transparent.png` identity/action sheet and
`sparkpaw-idle-refinement-v3-transparent.png` scale/grounding sheet.

```text
Use case: stylized-concept
Asset type: review-only four-frame pixel-art sprite strip for an Amiga AGA game
Primary request: exactly four sequential right-facing ledge-teeter poses of the
same Sparkpaw: notice missing support, lean outward in alarm, throw arm and tail
back to recover, then return toward idle. Preserve the supplied character,
costume, proportions, 32x40 perceived scale inside a 48x48 cell, common boot
baseline, orange/cream/navy/brown/cyan palette identity and pixel treatment.
Exactly one horizontal row, isolated full bodies, no grid, text or effects.
Perfectly flat #ff00ff chroma-key background. No redesign, zoom, altered
gauntlet side, extra prop, platform, blur, translucent pixels or cast shadow.
```

Saved as `assets/sprites/sparkpaw-ledge-teeter-v1-chroma.png`; the skill's
standard chroma-removal helper produced
`assets/sprites/sparkpaw-ledge-teeter-v1-transparent.png`. Runtime slots 0..57
remain unchanged pending explicit visual approval and a 48x48 palette proof.

## Phase 6B.6 representative environment slice concept v1

Built-in ImageGen reference workflow, 2026-08-14. Review-only environment
source; not consumed by the runtime generator. References were the saturated
`sparkpaw-gameplay-concept.png` and approved low-ground water concept v4.

```text
Use case: stylized-concept
Asset type: review-only representative environment slice for a PAL A1200 AGA
side-scrolling platform game
Primary request: one polished environment-only Storm Ruins gameplay slice with
a short blue water gap interrupting a low stone-and-metal ground strip. Preserve
the saturated premium pixel-art identity and approved low floor. Ground runs to
the bottom edge with no forest band. Use dark carved ruin blocks, restrained
moss, cyan energy seams, clean banks, rich water with irregular full-width
wavelets and independently spaced bubbles, deep mountain/forest/ruin parallax
and one raised platform.
Composition: 16:10 gameplay viewport approximating the upper 208 lines of
320x256, continuous banks and one 80px-proportion centre opening.
Constraints: no character, enemy, beetle, Strider, collectible, diamond,
projectile, HUD, text or interface; no tall floor; surface only about 5-10px
above the bottom boundary; wave never above either bank; original structures.
Avoid: duplicate object scales, empty lower strip, photorealism, smoothing,
gradients, antialiasing, watermark, logo or text.
```

Saved as `assets/concept/sparkpaw-visual-slice-concept-v1.png`. Its atmosphere
and parallax direction were useful, but its bottom foundation was internally
rejected as roughly three times too tall for the intended native playfield.

### Visual slice concept v2 thin-ground correction

Built-in ImageGen edit workflow using v1 as the edit target and water concept
v4 as the low-ground authority:

```text
Change only the bottom ground and water-bank vertical geometry. Reduce the
foundation to a compact 40-55px total height in the 1586x992 review image, with
the underside touching the bottom edge. Move the water and both bank lips to
the same walkable height. Preserve the compact carved edge, cyan seams, tiny
moss cap and everything above it: saturated mountains, forest, ruined towers,
central ruin, waterfalls, lighting, palette and raised platform. Keep the wave
between the banks. No HUD, characters, enemies, collectibles, text or empty
lower band; avoid a thick foundation or cliff-height floor.
```

Saved as `assets/concept/sparkpaw-visual-slice-concept-v2.png`. V2 is the
current review candidate; neither concept is consumed by runtime generation.

## Phase 6B.6 extended parallax master v2

Built-in ImageGen edit workflow, 2026-08-14. The accepted v1 parallax master
was the edit target and visual authority. Its 640px native reduction produced
an abrupt repeated landmark boundary during the supplied FS-UAE scroll review.

```text
Use case: stylized-concept
Asset type: extended source master for a PAL Amiga AGA side-scrolling game's
quarter-speed rear parallax
Primary request: preserve the iconic left opening with storm vortex,
lightning-struck ruined tower, violet mountains and waterfalls, then extend it
into a much longer continuous Storm Ruins journey with unique mountain passes,
varied cloud banks, dense forest depth, smaller broken ruins, restrained
waterfalls and distant cyan energy traces. Major landmarks must not repeat.
Composition: ultra-wide panoramic background. Keep important detail in the
upper two-thirds for reduction to a 1024x208 native strip. Make the far right
calmer; do not duplicate the vortex or opening tower and avoid abrupt vertical
changes in colour, mountain height, clouds or forest height.
Constraints: environment only; no actors, collectibles, platforms, foreground,
HUD, text or watermark. Crisp hard-edged premium 1990s Amiga pixel art. Preserve
readability after exact fixed eight-colour indigo/violet/blue reduction.
```

Saved non-destructively as
`assets/concept/sparkpaw-parallax-master-concept-v2.png`. The v1 source remains
preserved. V2 is consumed only after deterministic REAR8 reduction and seam/
camera-range validation; REAR16 remains a separate preview/benchmark candidate.

## Phase 6B.6 foreground material study v1 and geometry-locked concept v3

Built-in ImageGen edit using `sparkpaw-visual-slice-concept-v2.png` as the sole
edit target. The first prompt requested premium AGA ruin foreground materials,
larger unique structures, chipped pale stone lips, dark/violet recesses, cyan
conduits, sparse vegetation and an authored right platform while preserving the
storm background and water opening. Its material direction succeeds, but it
adds a left raised structure and is therefore rejected as a geometry source.
It is preserved as `assets/concept/sparkpaw-foreground-material-study-v1.png`.

The corrective prompt repeated the same material request with a strict geometry
lock: preserve the continuous lowest walk line, identical central water gap and
only the existing right raised rectangular platform/support; add no ledge,
ramp, stairs, rubble on a walk surface or traversal silhouette. Change only
material inside/below those shapes, using large asymmetrical structural bays,
chipped pale stone/steel, deep navy/violet cavities, irregular braces,
restrained cyan conduits, masonry relief and sparse moss/amber accents. Preserve
all rear scenery exactly; no actors, HUD, text, gradients or blur.

The result is preserved as `assets/concept/sparkpaw-foreground-concept-v3.png`.
It is review-only and must not be converted or integrated before approval and a
fixed FRONT16 palette/geometry proof.

## Phase 6B.6 full parallax master v3

Built-in ImageGen generation/edit workflow, 2026-08-15, using parallax v2 as
the opening-composition reference and foreground concept v3 as the premium
Storm Ruins material/atmosphere reference. The first generation supplied strong
ruins and forest but framed mountains against the top edge; it was rejected.
The targeted revision preserved those materials while restoring a turbulent
upper-third storm sky, one left-quarter vortex/lightning tower, unique mountain
passes, ruins, waterfalls and a cold-green forest across the full panorama.
It explicitly excludes actors, collectibles, foreground platforms, HUD, text,
orange orbs, repeated landmarks, flat top streaks and right-edge filler.

Saved as `assets/concept/sparkpaw-parallax-master-concept-v3.png`. Exact review
uses the complete vertical composition reduced to 1024x208, rather than v2's
top-biased crop. `assets/levels/storm-parallax-copper-banded-v3-review.png`
records the exact three-bitplane/twelve-step Copper-palette result. V3 is
accepted for alpha.19 runtime integration; v1 and v2 remain preserved.

## Phase 6B.6 title-identity parallax master v4

Built-in ImageGen generation workflow, 2026-08-15. This is a completely new
generation, not an edit or repaint of v3. The runtime title preview is the
architectural identity authority; v3 is used only as a panorama-quality and
atmosphere reference. The prompt requires one left-quarter destination citadel
with a slender central gothic spire, two lower side turrets, a readable castle
base, sparse vertical cyan Stormstone channels and a lightning connection to a
large vortex. The remaining panorama contains unique storm clouds, mountain
passes, forest, lesser ruins and waterfalls, with no repeated main tower,
actors, collectibles, foreground, HUD, text, orange orb or right-side filler.

Saved as `assets/concept/sparkpaw-parallax-master-concept-v4.png`. Exact review
is preserved in `assets/levels/storm-parallax-copper-banded-v4-review.png`.
Because the original mountain band reduced cyan structure to violet, a second
hardware-exact preview assigns one of its eight indices to cyan and is retained
as `storm-parallax-copper-banded-v4-cyan-review.png`. Mountain depth remains
readable while the citadel heart, distant conduits and waterfalls regain the
title-world identity. V4 plus that palette role is accepted for alpha.20.

## Phase 6B.6 newly generated foreground kit v1

Built-in ImageGen generation workflow, 2026-08-15. Foreground concept v3 is the
material/lighting authority and the alpha.20 native viewport supplies scale and
geometry language, but the kit is generated entirely anew rather than painted
over either reference. It contains one long low ruin facade, six broad unique
platform families, short gothic columns and paired water-bank motifs on a flat
magenta chroma background. Required materials are chipped pale stone/steel,
deep navy/violet cavities, open braces, cyan Stormstone conduits, sparse moss
and restrained amber rivets; repeated 16px boxes, perspective, collision-changing
tops, actors, HUD and rear scenery are excluded.

The chroma source is preserved as
`assets/concept/sparkpaw-foreground-kit-concept-v1-chroma.png`; the cleaned RGBA
kit is `sparkpaw-foreground-kit-concept-v1.png`. Runtime conversion maps whole
platform/column pieces into the exact existing FRONT16/collision rectangles.
Broad platforms retain transparent open supports below their collision slab;
the y=200 floor remains an honest eight-pixel cap above the fixed HUD.

## Phase 6B.6 richer foreground kit v2

Built-in ImageGen generation workflow, 2026-08-15. The foreground kit v1 is
the material-identity reference, foreground concept v3 is the architecture and
quality target, and the runtime title preview anchors the world identity. The
new orthographic kit supplies seven horizontal slab families, four genuinely
different underside/support silhouettes, four grounded columns, mirrored
water-bank motifs and one long shallow ground facade. It explicitly excludes
the obsolete floating purple lozenges, hanging lamps/orbs and repeated central
T-supports. All walkable tops remain horizontal and collision-readable.

```text
Generate a completely new orthographic modular Storm Ruins foreground kit,
richer and more authored than v1. Arrange clearly separated short-to-long
platforms, asymmetric broken arches, offset buttresses, suspended rib vaults,
compact conduit trusses, grounded piers, water-bank endcaps and a long shallow
ground cap on flat #ff00ff. Use chipped pale stone/steel, charcoal/navy masonry,
deep violet recesses, structural cyan channels, tiny amber fasteners and sparse
moss. No floating diamonds, purple lozenges, hanging lamps, orbs, chains,
actors, collectibles, HUD, rear scenery, perspective, gradients or repeated
16px boxes. Crisp hard-edged premium late-era AGA pixel art.
```

Saved as `assets/concept/sparkpaw-foreground-kit-concept-v2-chroma.png`; the
cleaned RGBA source is `sparkpaw-foreground-kit-concept-v2.png`. V2 replaces
only generated foreground pixels; collision, renderer and gameplay stay fixed.

## Phase 6C.2 Stormkeeper's Waystation concept v1

Built-in ImageGen generation workflow, 2026-08-25. The prompt requests a new
side-view late-era AGA pixel-art Level 1 destination: a compact stone-and-steel
weather-station cottage/shrine, a crooked storm-bent pine growing through its
roof, restrained cyan conduits, one dark doorway and exactly one large cyan
Stormstone Core above a pedestal. Composition reserves a quiet approach on the
left and places the building/tree cluster on the right. Player, HUD, enemies,
ordinary shards, text and gameplay platforms are excluded.

The 320x208 composition study is preserved in
`docs/concepts/story-intro/assets/stormkeeper-waystation-concept-v1.png`. A
second prompt isolates only the house/tree/Core cluster as an orthographic
hard-edged cutout; its checker-backed RGB output is
`assets/concept/sparkpaw-stormkeeper-waystation-source-v1.png`. Runtime
generation keys the neutral checker, maps the cluster to FRONT16 and adds a
separately readable procedural Core. These generated sources are not direct
hardware-ready art.

## Phase 6C.2 Waystation v2 and animated Core source v1

Built-in ImageGen generation workflow, 2026-08-25. Waystation v2 was requested
as a reusable orthographic late-era AGA end-station asset: a larger detailed
stone-and-steel cottage/shrine with one empty arched Core socket, a prominent
crooked storm-bent pine, moss, bronze fittings and cyan conduits, but no Core,
actors, HUD, text, enemies or platforms. The source is preserved as
`assets/concept/sparkpaw-stormkeeper-waystation-source-v2.png`.

The companion Core request specified exactly six left-to-right frames in one
row: one asymmetrical cyan/blue Stormstone crystal with bronze keeper arcs and
small green energy accents, subtly hovering and pulsing while retaining a
stable 64x48 silhouette. It excluded actors, HUD, text, scenery and multiple
objects. The magenta-backed source is preserved as
`assets/concept/sparkpaw-stormstone-core-six-frame-source-v1.png`; its exact
FG_PALETTE reduction is
`assets/concept/sparkpaw-stormstone-core-aga16-preview-v1.png`.

The accepted clearing deliberately displays the waystation's mapped material
roles through the normal FRONT16 bank. This turns its foliage detail into warm
copper/orange lightning-scorched highlights, which gives stronger contrast
against the cold violet rear panorama than the investigated green palette.

The first FS-UAE Core-sprite integration was rejected because the established
early-fetch Stage 5L layout cannot safely feed another 64-pixel attached pair.
The accepted integrity correction instead maps the Core to FRONT16 and caches
it as a 64x48 Bob. Animation polish derives all six idle frames from the first
generated cell so silhouette, scale and mask remain exact; only internal light
roles change. The first twelve-frame pickup attempt used Core-to-gauntlet
threads, but supplied 60 fps evidence showed that their low endpoint vanished
behind the higher-priority player sprite. The v2 native-palette preview is
`assets/concept/sparkpaw-stormstone-core-aga16-preview-v2.png`: contraction,
a radial white/cyan/bronze release and fading fragments replace the directional
threads. These frames do not alter or regenerate Sparkpaw's player sheet.
