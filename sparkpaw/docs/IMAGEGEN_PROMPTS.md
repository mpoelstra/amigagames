# Built-in ImageGen prompts

## 2026-09-21 — Ground cleanup staged; vegetation concept v1

User rejects v10 irregular ground/endcap pattern and orphan copperpost, prefers
large trees behind player and small trees/shrubs infront rooted toground. V11
removes extra post art and entire additional masking path/buffer; original gate
clipping remains. Floor uses one complete32px panel with regular joins and
continuous rail, not partial endcaps. Nativebuild and sprite-clipping hosttests
pass. Staged73assets/55refs,68officialreleasefiles unchanged, previousdrawer
archived byteexact. Ground/post screenshots catalogued Drowned-v10-ground-and-post-rejected-a/b.

First imagegen request hitusage limit; following user continuation succeeded.
Saved assets/concept/drowned-vegetation-v1.png and exactpromptTXT. Four distinct
spruce/fir/sapling/lowfern-heather silhouettes, rooted lowmoss bases. Review pending.
Source has soft backing/halo despite transparency request: remove before native
conversion, simplify fine detail into nativeclusters/currentpalette. No vegetation
runtime yet. No additionalanimation requested. Next native grounded preview then
safe foreground plant clipping only lowerlegs; avoid hazards/landings. Wholelevel
art/joining still follows finalslice acceptance. No release/commit/FPSclaim.


## 2026-09-21 — Consistent platform kit concept v2

User accepts camera and station/boiler quality, rejects cheap repeated platform
fragments. Authorizes whole-level material consistency and occasional conifer
accents, with focused final-section art proof FIRST, then full-route application
and joining. Built-in imagegen material reference station-v3 creates concept
assets/concept/drowned-platform-kit-v2.png + exactpromptTXT. Broad steel walkway,
substantial copper-bearing posts, three narrow pier heights, ground ends/middle,
conifer. Concept pending approval, no runtime or dist changes. Preserve geometry;
raised end caps must flatten to collision top; ground source needs adaptation to
8px floor strip, not new tall ground. Native16pen conversion/masks and scale review
next, then short user020 proof before spreading kit across whole level.


## 2026-09-21 — Rain station companion conifer review

User approves unique v2 station and requests a characteristic tree behind right
side, matching background conifers; each station should have its own tree/identity.
Built-in imagegen edit saved as assets/concept/drowned-rain-station-source-v3.png
with exact prompt TXT. Dark blue-green irregular mountain conifer behind tank/roof,
canopy above roof, warm windows and left instruments remain readable. V2 retained.
Concept review pending, no runtime/dist changes. Native conversion should preserve
building scale (roughly200x145) and allow tree to extend its envelope rather than
shrink building to fit the previous total bounds. Continue camera/Core/results
work after native art review.

## 2026-09-21 — Unique Rain weatherstation source v2

User rejects recoloured Level1 building; requests own Drowned station matching
original concept, same size and contrasting colours. Built-in imagegen uses lower
panel of drowned-finale-station-v1.png. Saved source and prompt at
assets/concept/drowned-rain-station-source-v2.png/.txt. Industrial steel hut,
copper rain tank/gutters, mast/rain gauge, amber windows, separate empty Core
pedestal. Source inspected; not native16pen reduction or runtime yet. Review
pending. Earlier station-v1 review explicitly rejected, retained. Existing
Governor v6 remains in dist. Next: native200x145 envelope conversion/composition,
then camera/reveal/Corepickup/results integration after visual approval.


## Stormrail Gate 6 — Level 2 Storm Ruins threshold concept v1

Built-in ImageGen concept workflow, 2026-09-04. The accepted Stormrail
interlude composition, dark route panorama, native debris family and Level-1
gameplay concept were supplied as separate identity, contrast, material and
quality references. The request identifies Gate 6 as the Stormrail end fight
at the threshold to **Level 2: Storm Ruins**, not an arbitrary rail checkpoint.

The concept asks for exactly one original enclosed Harrier boss, one upper and
one lower gate turret, and one monumental closed Storm Ruins gate. The Harrier
uses a hooked predatory silhouette, layered navy/steel armour, cyan storm core,
restrained copper joints and believable engines. The gate combines weathered
carved ruin stone with recessed machinery and cyan/violet energy, so the
destination architecture belongs to Storm Ruins while the defenders still read
as Stormrail technology. Broad flight lanes, connected value planes,
material-coloured edges and exact side-view reduction feasibility were required;
primitive polygons, flat fills, uniform black outlines, saturated purple slabs,
generic spaceships, neon stripe walls and noisy microdetail were forbidden.

Saved as
`assets/concept/sparkpaw-stormrail-gate6-storm-ruins-threshold-concept-v1.png`.
Status: material and visual direction accepted, spatial composition superseded
by v2. The angled three-quarter gate is unsuitable for the real flat side-view
arena, the Harrier is too large, and the wall consumes too much manoeuvre space.
It has not been reduced, cleaned, converted, cached or integrated into runtime.

### Gate 6 Storm Ruins threshold concept v2 — flat encounter correction

Built-in ImageGen edit workflow, 2026-09-04. V2 preserves v1's accepted
Harrier, turret, ruin-stone, metal and energy language while replacing the
encounter layout with a strict side elevation matching the actual shooter. The
left two-thirds remains open for movement, player fire and dodgeable hostile
patterns. One narrow boundary wall touches playfield ceiling and floor at the
far right, with distinct upper/lower turrets and a central closed gate. The
Harrier is separated from all three structures and reduced to roughly
1.5--1.8 times the occupied Skimmer's perceived mass. The lower magenta band
contains side-view-only orthographic source callouts.

Saved as
`assets/concept/sparkpaw-stormrail-gate6-storm-ruins-threshold-concept-v2.png`.
Status: spatial composition accepted as the basis for native v1. Generated
pixels remain concept-only; the runtime candidate uses a separate exact-size
FRONT16 reduction with material-role mapping and native cluster cleanup.

Native v1 is saved as
`assets/concept/sparkpaw-stormrail-gate6-native-v1-aga16.png`, with its 4x
nearest-neighbour inspection sheet at
`assets/concept/sparkpaw-stormrail-gate6-native-v1-review-4x.png`. The sheet
contains independently reduced 96x56 Harrier and 32x24 turret cells plus two
unique 32x104 wall halves. This is the first 68030 art candidate, not accepted
final art.

## Stormrail interlude composition v1/v2

Use case: pre-production composition and silhouette reference for the playable
Phase 6D Stormrail interlude. V1 requested one original four-panel 1993 AGA
pixel-art sequence: a very short Storm Ruins foot approach, Sparkpaw boarding
an open-cockpit Stormrail Skimmer, immediate rail launch and a fast horizontal
auto-scrolling shooter with upper/lower route readability. The established
Sparkpaw gameplay concept and current Storm visual slice were supplied only as
identity, palette, scale and environment references. The vehicle had to keep
Sparkpaw visibly exposed, use a long low navy/steel silhouette, cyan energy
drive and restrained amber trim; no HUD, text, logos or licensed designs.

Saved as `assets/concept/sparkpaw-stormrail-interlude-concept-v1.png`.

V1's shooter panel contained a beetle-like flying enemy. That contradicts the
interlude's new airborne combat language and would make Level 1's ground enemy
feel reused without purpose. V2 therefore retained the full composition,
Sparkpaw and Skimmer while replacing every beetle silhouette with exactly
three original flying machine roles: a slim cyan-nosed dart drone (Indexer), a
compact square lane-blocker with amber warning face (Correction Stamp), and a
larger crescent/claw capture drone with cyan core (Core Grapple). None may have
legs, antennae, a domed shell or a ground-beetle body plan.

Saved as `assets/concept/sparkpaw-stormrail-interlude-concept-v2.png`. This is
the selected composition reference for the first native playable slice; it is
not itself converted into runtime pixels.

### Stormrail runtime family v1/v2

V1 requested one clean magenta-backed production-source sheet derived from the
approved composition and canonical Sparkpaw identity: two 112x48-proportion
Skimmers and three distinct airborne drones. Its native FRONT16 reduction was
technically clean, but the departure screenshot exposed a story error: the
parked vehicle already contained Sparkpaw while the hardware-sprite Sparkpaw
stood beside it. V1 is therefore rejected for the parked frame.

The focused v2 edit changes only the top-left vehicle: Sparkpaw is removed and
the open cockpit/seat is reconstructed empty. The flying top-right Skimmer
retains its visible rider, and all three drones remain unchanged. V2 is stored
as `assets/concept/sparkpaw-stormrail-runtime-family-v2-chroma.png`; its exact
fixed-palette review is `sparkpaw-stormrail-runtime-family-v2-aga16.png` and it
is the source of `assets/runtime/stormrail-family.spbm`.

## Stormrail environment panorama and foreground kit v1

Built-in ImageGen generation workflow, 2026-08-30. The new ultra-wide rear
source establishes a Stormrail transit canyon after Level 1: diagonal rain,
violet mountains, deep forest and ruin silhouettes, waterfalls, cyan energy,
non-repeating broken turbine rings and one distant vortex/citadel destination.
Actors, enemies, projectiles, HUD, text and foreground gameplay rails were
excluded. It is stored as
`assets/concept/sparkpaw-stormrail-rear-panorama-v1.png`.

The companion orthographic kit supplies long energized rails, upper/lower
conduit spans, curved route endcaps, pylons and a broken turbine arch on flat
magenta. It is stored as
`assets/concept/sparkpaw-stormrail-foreground-kit-v1-chroma.png`. Both sources
are accepted for the isolated proof. Their hardware-exact reviews are
`sparkpaw-stormrail-rear-aga8-preview-v1.png` and
`sparkpaw-stormrail-front-aga16-preview-v1.png`; the matching proof-only
runtime assets are `stormrail-rear.spbm` and `stormrail-front.spbm`.

## Stormrail loading composition v1

Built-in ImageGen generation workflow, 2026-08-30, using the approved
interlude composition and rear panorama as separate references. The request
places a single readable Sparkpaw in the open-cockpit Skimmer on an energized
rail, against rain, waterfalls, turbine rings and the distant vortex. A dark
lower band is reserved for deterministic runtime text. Generated text, HUD,
enemies, collectibles, panels and duplicate actors/vehicles were excluded.

Saved as `assets/concept/sparkpaw-stormrail-loading-concept-v1.png`. Its exact
six-plane reduction is
`assets/concept/sparkpaw-stormrail-loading-aga64-preview-v1.png`. Although its
pen 0, Copper display and border are technically correct, supplied visual
review rejects the composition. Stormrail will reuse the accepted disk/loading
presentation instead; this generated plate is retained only as rejected
concept history and is not a proof or release dependency.

## Stormrail runtime family v6 — two-stage laid-back boarding family

No new image-generation request was used. The accepted empty v3 Skimmer hull,
its small attached cyan engine glow and Sparkpaw's accepted native player pose
16 are recomposed deterministically by `tools/generate_stormrail_assets.py`.
All six 112x48 vehicle frames use the identical hull pixels. Frames 1--5 keep
one fixed 98%-scale rider: first contact, settle, brief forward controls-check
sit, a light lean, then the approved laid-back pilot concept v1. The selected
concept keeps open focused eyes, an attached gauntlet, coherent bent legs and
both boots. It is reduced uniformly without geometric warping or rotation, so
head, torso and limbs retain their drawn anatomy. Translation and
cockpit occlusion create the descent; the rider and hull never shrink between
frames. Exact indexed review output is saved as
`assets/concept/sparkpaw-stormrail-runtime-family-v6-aga16.png`; integration is
pending focused FS-UAE/HD review.

V6 is rejected. The added row-sheared pose distorted Sparkpaw's head; the
subsequent canonical closed-eye pose read as sleeping, and the generated
laid-back concept lost canonical colour clustering and exposed a disconnected-
looking knee after 48x48 reduction. Runtime returns to the five-frame v5
baseline while the vehicle and cockpit are redesigned.

## Stormrail Skimmer concept v4 — high-contrast AGA craft direction

Built-in ImageGen concept pass using the rejected v3 Skimmer, the accepted
Stormrail rear panorama and canonical Sparkpaw sheet as references. Requested
a sleek airborne wedge, open negative space beneath the chassis, deep seat and
backrest, navy/charcoal and steel-blue material planes, silver highlights,
cyan energy and restrained copper trim. It explicitly excludes the former
grey slab/ground-piece silhouette and noisy micro-dithering.

Saved as `assets/concept/sparkpaw-stormrail-skimmer-concept-v4.png`. Status:
pending visual review. The empty craft has materially stronger silhouette and
contrast; rider posture and the feasibility of reducing its long hull to the
current 112x48 Bob remain deliberately unapproved. No runtime asset is derived
from this concept before explicit approval.

## Stormrail Skimmer concept v5 — compact interceptor refinement

Built-in ImageGen edit using v4 as the approved material/silhouette direction,
the Stormrail panorama as contrast reference and canonical Sparkpaw for scale.
The request preserves v4's navy/steel/silver planes, cyan engine and copper
trim while shortening the hull, nose, engine mass and tail by roughly 20--25%,
pulling the emitter toward the cockpit and retaining negative space underneath.
It also requests a deep supported reclined seat suitable for shooter controls.

Saved as `assets/concept/sparkpaw-stormrail-skimmer-concept-v5-compact.png`.
Status: pending visual review. It reads more like a fast interceptor and its
seat relationship is improved, but exact native width, rider identity and
112x48 feasibility remain unapproved. No runtime conversion has been made.

One subsequent built-in ImageGen extraction isolates only v5's occupied side
view onto exact magenta for deterministic scale comparison. It is saved as
`assets/concept/sparkpaw-stormrail-skimmer-v5-occupied-chroma.png`. The three
scale candidates are all derived from that one source by
`tools/generate_stormrail_scale_study.py`; ImageGen does not independently
redraw the compared sizes.

## Stormrail Skimmer concept v6 — head-only deep cockpit alternative

Built-in ImageGen edit using compact v5 as the strict vehicle direction,
canonical Sparkpaw for identity and the Stormrail panorama for contrast. It
retains the compact navy/steel/cyan/copper interceptor but deepens the pilot
pod so only Sparkpaw's head, ears and a small scarf edge remain visible. Torso,
gauntlet, knees, legs and boots are explicitly inside the shell rather than
removed from the character design.

Saved as `assets/concept/sparkpaw-stormrail-skimmer-concept-v6-head-only.png`.
Status: selected visual direction. The user approved both the compact cockpit
solution and Sparkpaw's calm, confident facial expression on 2026-08-31.
This direction materially simplifies native boarding and flight animation,
avoids small-scale seated-anatomy failures and permits a cleaner compact hull.
The native occupant must preserve this accepted focused expression and
canonical head proportions.

A subsequent ImageGen extraction isolates the approved occupied side view on
exact magenta as
`assets/concept/sparkpaw-stormrail-skimmer-v6-head-only-chroma.png`. The
deterministic native FRONT16 comparison is saved as
`assets/concept/sparkpaw-stormrail-skimmer-v6-head-only-scale-study.png` and
`assets/concept/sparkpaw-stormrail-skimmer-v6-head-only-scale-study-2x.png`.

A strict ImageGen edit then removed only Sparkpaw and supplied an empty deep
cockpit on exact magenta. That source is preserved as
`assets/concept/sparkpaw-stormrail-skimmer-v6-empty-chroma.png`. It is not used
as an independently animated hull: `tools/generate_stormrail_v6_native_family.py`
makes its 104x46 FRONT16 reduction canonical, then transplants only the
approved head/scarf/cockpit rectangle into the occupied frame. Empty and
occupied hull pixels are therefore identical outside the 23x23 pod region.
The native proof and background review are saved as:

- `assets/concept/sparkpaw-stormrail-skimmer-v6-native-family-aga16.png`
- `assets/concept/sparkpaw-stormrail-skimmer-v6-native-family-review.png`
- `assets/concept/sparkpaw-stormrail-skimmer-v6-native-family-review-3x.png`

No runtime integration has been made.

## Stormrail rear panorama v3 — approved open-shooter direction

Built-in ImageGen generation workflow, 2026-08-30, using the accepted Level-1
parallax v4 as the quality/layering reference and Stormrail interlude concept
v2 only for narrative, material and mood. The prompt requested a brand-new
ultra-wide Stormrail flight panorama with dense mountains, dark forest,
waterfalls, cyan-lit broken towers, suspended rail arches, mist, rain and one
restrained storm-vortex landmark. It explicitly excluded actors, Skimmer,
enemies, HUD, floor/ceiling, corridor obstacles, flat sky, three horizontal
strips, repeated wallpaper and a monochrome purple wash.

Saved as `assets/concept/sparkpaw-stormrail-rear-panorama-v3.png`; the exact
three-plane/twelve-transition preview is
`assets/concept/sparkpaw-stormrail-rear-aga8-preview-v3.png`. Supplied V2 review
finds this background materially better. It is retained as the visual basis for
the revised open horizontal shooter; the rejected corridor foreground is not
part of that acceptance.

## World diamond native-art direction v1 (concept reference)

Use case: visual design reference for one native 16x21 Amiga AGA masked Bob
collectible. The existing generated HUD preview and world-diamond preview were
supplied as separate identity/runtime references. The request retained the HUD
diamond's tall faceted identity and lighting direction while asking for a clean
symmetric silhouette, crisp lower point, opaque dark contour, cream upper
facet and cyan/blue inner facets. It explicitly forbade gradients,
antialiasing, semi-transparency, glow, shadows, text, extra gems and scenery.

Saved as `assets/concept/sparkpaw-diamond-world-direction-v1.png`. Retained as
concept history only: its enlarged generated pixels and extra shades are not
runtime input. The independently authored first 16x21 runtime interpretation
was rejected in supplied FS-UAE evidence because its visual weight read 180
degrees inverted relative to the HUD and its contour looked too rounded. V2
fixed that orientation but was also rejected because its narrow kite geometry
and simple cream slab no longer matched the broader HUD emblem. V3 reconstructs
the HUD's cream stem/left arm, cyan planes and blue lower core inside one clean
closed native polygon, but supplied review found its cream L too dominant and
its dark/blue facets too weak. V4 retains the v3 polygon and mask while reducing
and angling the cream highlight and restoring a stronger dark central cut and
blue lower/right mass. Supplied v4 evidence rejects the internal pen-1 cut as
a black slit. V5 reserves pen 1 for the contour and moves all depth into an
expanded blue lower half with a narrow cyan seam. FRONT16 pens 1/4/5/6 and the
Bob contract remain fixed.

## Player unified-rig reconstruction study v1 (rejected)

Use case: production-source experiment for rebuilding all 62 right-facing
Sparkpaw runtime poses around one canonical head and body rig. The supplied
indexed 48x48 runtime sheet was used as the authoritative pose-order, costume,
palette and identity reference. The request required a strict 4x16 sheet,
exactly 62 isolated poses, one skull/muzzle/ear/torso/limb/boot/gauntlet scale,
one ground baseline, hard 1993 AGA pixels and a flat `#ff00ff` background.

Saved as `assets/sprites/sparkpaw-unified-rig-study-v1-rejected.png`.
Rejected before runtime integration: the generated study contains only 50
poses, replaces or omits required actions and is not a trustworthy one-to-one
62-frame source. Its more stable anatomy supports the diagnosis, but no frame
IDs, generator inputs or runtime pixels consume it. The next attempt must be
split into small complete action families derived from one approved canonical
rig and validated together at exact 48x48 indexed size.

## Player unified-rig production families v1/v2 (rejected)

The attempted reconstruction begins with
`sparkpaw-canonical-rig-v1-{chroma,transparent}.png`: four model poses defining
one skull, muzzle, ear, torso, limb, boot and gauntlet scale. Built-in ImageGen
then derived small, exactly counted action sheets on flat `#ff00ff`, always
requiring the same anatomy, right-facing source, fixed ground baseline and no
per-pose zoom:

- `sparkpaw-unified-idle-blink-v1-*`: neutral plus eye-only blink;
- `sparkpaw-unified-run-v2-*`: corrected eight-stage loop with no idle cells;
- `sparkpaw-unified-jump-land-v1-*`: four air and three landing stages;
- `sparkpaw-unified-turn-idle-v1-*`: six momentum-turn plus twelve idle stages;
- `sparkpaw-unified-crouch-combat-v1-*`: crouch and grounded/low fire sources;
- `sparkpaw-unified-air-hurt-ledge-v1-*`: air fire and both hurt families;
- `sparkpaw-unified-ledge-v2-*`: isolated four-stage teeter without scenery.

Supplied FS-UAE/HD recordings reject this entire family set. Crouch-fire clips
the tail; air-fire enlarges the head and gauntlet; the idle strip rotates to
the opposite facing instead of returning through front; and the run/reversal
cycle contains compressed, clipped and wrong-facing frames. None of these
sources remain connected to runtime generation. They are preserved as rejected
review history because their anatomy experiment must not be mistaken for an
accepted replacement.

The useful converter lesson is retained independently: wide limbs and
gauntlets may clip at the 48px canvas edge but may never trigger per-frame
fit-to-cell scaling of the complete actor. Runtime generation returns to the
accepted alpha.59 sources, stable frame IDs 0..61 and exact mirrored facings.

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

## Stormrail route spans concept v1 — Gate 2.5 background study

Built-in ImageGen generation workflow, 2026-09-01, using the approved v3 rear
panorama only as environment, palette and quality reference. The prompt asks
for one continuous four-region route study: vortex mountains, a rain-veiled
ruin valley, a waterfall/broken-bridge basin and a cyan-lit rail citadel. It
requires a calm central combat band for the later measured cap of five small
enemies, five player shots and four hostile shots; dense highlights remain
high, deep or along the lower silhouette. Natural cloud, rain, mist, forest
and waterfall connector zones replace full-height duplicate seams. Actors,
HUD, text, corridors, collision walls and obvious mirrored/repeated wallpaper
are excluded.

Saved as `assets/concept/sparkpaw-stormrail-route-spans-concept-v1.png`.
Status: pending visual-direction review. It is concept material only and has
not been quantized, tiled or integrated into a runtime asset.

V2 is a built-in ImageGen colour/atmosphere edit using v1 as the strict
composition target and Level-1 parallax v4 only as a colour-diversity
reference. Geometry, horizon, calm combat band and four-region journey remain
fixed. The requested colour arc moves continuously from lavender-violet and
dark forest green through rain-veiled teal/blue-grey stone, pale waterfall mist
and muted bronze/moss into the colder cyan/indigo citadel. Broad overlapping
clouds, rain, mist, forest depth and material lighting must carry every change;
vertical colour divisions, panels, hard palette lines and sudden hue switches
are explicitly forbidden. Bright cyan, orange and cream remain sparse for
future actor/projectile readability.

Saved as `assets/concept/sparkpaw-stormrail-route-spans-concept-v2.png`.
Status: accepted as the source direction for an exact REAR8 feasibility study;
it is not itself runtime art.

## Stormrail route spans native REAR8 study v3

Local conversion study, 2026-09-01. The v2 concept was sampled as four
representative 320x208 playfield views and reduced to the real rear-playfield
constraint: three bitplanes, no more than eight colours on any scanline, AGA
12-bit colour values and sixteen smoothly interpolated Copper palette stages.
The palette banks are deliberately authored by route region rather than
derived independently per strip, so departure violet, rain-valley slate and
green, waterfall mist and moss, and citadel cyan remain distinct without hard
horizontal colour bands. V1 and v2 were internal conversion attempts and were
rejected as respectively noisy/muddy and too dull.

The v3 contact sheet is
`assets/concept/sparkpaw-stormrail-route-spans-aga8-study-v3.png`; its four
individual native-size views use the matching `*-aga8-study-v3.png` names.
`assets/concept/sparkpaw-stormrail-route-joins-aga8-study-v1.png` studies the
three intermediate palette/scene connectors. The readability composite
`assets/concept/sparkpaw-stormrail-route-combat-readability-aga8-v1.png`
places the accepted runtime vehicle over all four native views without
changing either palette owner.

Status: exact-size and palette-feasible static study, pending art-direction
review. It is not yet a seamless runtime strip and has not been integrated or
verified in FS-UAE.
## Stormrail compact flight-family studies — Gate 2 polish

Built-in ImageGen reference/edit workflow, 2026-09-01. The accepted v7 native
Stormrail family was the strict vehicle, Paw, palette-role and anatomy
reference. V1 explored a roughly 67-percent shooter silhouette on magenta;
the targeted v2 edit reduced overall bulk and Paw's relative scale while
preserving the black hull, orange rail and cyan engine/muzzle identity. The
v2 source is preserved as
`assets/concept/sparkpaw-stormrail-compact-flight-source-v1.png`.

Neither generated image is a runtime sprite: both remain proportion studies.
The first was rejected for an oversized Paw and heavy hull. V2 is useful as
direction only. Runtime art is a component-authored native 80x32 FRONT16 Bob:
the accepted empty hull and pilot delta are reduced independently, Paw is
seated separately, and the orange rail plus cyan engine/muzzle clusters are
redrawn at exact native resolution. The rejected sink-neck/scarf treatment is
explicitly excluded.

### Stormrail compact cockpit study v2

Built-in ImageGen reference/edit workflow, 2026-09-01. Inputs were the native
80x32 compact v1 Bob and the accepted v7 boarding/launch family. The request
kept the hull, right-facing direction, orange rail, cyan engine/emitter and
Sparkpaw palette roles fixed, while placing Paw lower and forward inside a
dark cockpit cavity with a continuous foreground rim across his lower
scarf/shoulders. Floating-head composition, oversized Paw, hull redesign and
the rejected sink-neck/scarf anatomy were explicitly forbidden.

Saved as
`assets/concept/sparkpaw-stormrail-compact-cockpit-study-v2.png`.

Status: cockpit depth/composition direction approved, but the first native v2
translation was rejected because an added red scarf/support cluster read as an
unfamiliar red stain. Runtime v2 therefore uses only the established orange,
cream and dark pilot roles at that support; it remains native 80x32 FRONT16
with binary alpha and one combined Bob mask. In-game acceptance remains
pending.

V3 removed the explicit scarf but retained a warm reddish cockpit/neck edge
and was rejected in supplied FS-UAE review. V4 was reauthored as one complete
ship-plus-pilot composition with the general gameplay sheet as the identity
reference: the visible cape beneath/behind the head is dark navy/blue-grey,
the top cockpit rim is neutral steel, and warm orange is limited to fur and
the long side energy rail. A matching isolated seated pilot source is saved as
`assets/concept/sparkpaw-stormrail-seated-pilot-source-v4.png`; it is reduced
offline and merged into one final 80x32 Bob, never rendered separately.

Status: v4 concept direction accepted by correction request; native in-game
review remains pending.

The isolated bust experiments still made the head dominate after native
reduction and were rejected before staging. The final path therefore converts
the complete v4 ship-plus-Sparkpaw composition as one indivisible source into
one 80x32 image and one mask. There is no separate pilot source, sprite,
position, draw or restore at runtime. During FRONT16 conversion the reddish
warm pen is disallowed for this complete Bob; fur and the long side rail use
the normal orange role instead. Native in-game review remains pending.

The first whole-source native reduction still left the head too dominant and
lost eye/muzzle/inner-ear separation, reading as an orange haze at gameplay
scale. Complete composition v5 keeps one indivisible ship-plus-pilot source
but seats a roughly 20-percent smaller Sparkpaw deeper in the cockpit while
retaining a visible blue-grey cape/torso wedge and forepaws. It explicitly
preserves HUD-consistent identity roles without copying or rescaling the HUD
portrait. V5 replaces v4 as the native conversion source.

LANCZOS reduction of the whole v5 composition was rejected before staging
because it blended the small face and cape into an orange haze. The selected
conversion uses nearest-neighbour sampling of that already pixel-authored
whole source, followed by one FRONT16 quantization and one binary mask. This
preserves separate eye/outline, cream muzzle and inner-ear clusters without
introducing any pilot overlay.
### Stormrail compact cockpit study v6 — larger integrated pilot candidate

- Source: `assets/concept/sparkpaw-stormrail-compact-cockpit-study-v5.png`
- Output: `assets/concept/sparkpaw-stormrail-compact-cockpit-study-v6.png`
- Status: native gameplay test candidate; visual acceptance must come from the
  user's FS-UAE screenshot rather than the host-side enlarged preview.
- Intent: preserve one combined ship-plus-Sparkpaw Bob and enlarge Sparkpaw by
  roughly 18% inside the cockpit so the HUD identity survives reduction to
  80x32. Keep cream muzzle/inner ears, readable eye, dark outline and a small
  dark blue-grey torso/cape anchoring him in the cockpit. No red scarf, collar,
  neck band or warm red strip under the head; no detached pilot overlay.
- Conversion remains nearest-neighbour into the established FRONT16 palette;
  ship, pilot, mask and draw stay one runtime object.

### Stormrail compact cockpit study v7 — conservative larger-pilot candidate

- Source: `assets/concept/sparkpaw-stormrail-compact-cockpit-study-v5.png`
- Output: `assets/concept/sparkpaw-stormrail-compact-cockpit-study-v7.png`
- Status: selected for native user-screenshot evaluation. The v6 18% study was
  rejected before staging because its ears reached the 32-pixel cell boundary
  and its head dominated the craft after reduction.
- Intent: enlarge the integrated pilot only about 10%, seat him slightly lower,
  and preserve clear ear, eye, cream muzzle and dark blue-grey torso clusters.
  The complete composition still converts and renders as one 80x32 Bob.

- User result: rejected. At native gameplay scale the larger face collapsed
  into dark eye/forehead holes that visually read as transparency. Restore v5
  as the drawer baseline; do not continue enlarging and reducing ImageGen
  concepts for this pilot. Any next attempt must be authored and judged from
  the exact indexed 80x32 frame with user screenshots as visual authority.
### Stormrail cockpit head v1 — dedicated head-only hardware-sprite source

- References: the user's rejected hybrid placement comparison,
  `sparkpaw-hud-concept-v1.png`, and `sparkpaw-48x48-aga16-source.png`.
- Output: `assets/concept/sparkpaw-stormrail-cockpit-head-v1.png`.
- Status: native 030 review candidate pending; generated source is never used
  directly and is reduced to an exact 16x15 FG_PALETTE sprite in slot 62.
- Intent: a focused right-facing Sparkpaw head with two complete orange/cream
  ears, HUD-like eye, cream muzzle and dark outline. Head only: no cape, scarf,
  collar, neck, torso, hands, ship or cockpit. The ship remains a separate
  80x32 Bob and the runtime head ends above its orange hull accent.
- Background handling: remove only light neutral pixels connected to the source
  canvas edge so enclosed pale eye and muzzle pixels can never become alpha.

### Stormrail cockpit head v2 — strict one-eye native-grid source

- References: cockpit head v1, the HUD concept and Level-1 sprite sheet.
- Output: `assets/concept/sparkpaw-stormrail-cockpit-head-v2.png`.
- Status: selected over v1 for native reduction; v1's three-quarter/two-eye
  construction lost its identity at 16x15.
- Intent: strict right-facing side profile, one readable eye, two complete
  orange/cream ears and a compact cream muzzle. The enlarged source is built
  from deliberately coarse logical pixel blocks and therefore uses nearest
  reduction before exact FG_PALETTE indexing. No cape, neck or body.

- Native result: rejected before staging. Its generated ear hierarchy was the
  reverse of the accepted Level-1/HUD identity. The 16x15 reduction lost the
  eye; the 18x17 reduction retained an eye but still read as a different fox.
  Runtime returned to a fresh, uncorrected Level-1 head extraction with only
  lower body rows omitted. The generated v1/v2 files remain review history and
  are not consumed by the build.

## Stormrail Debris 2.0 art study v1 — material-rich but too architectural

Built-in ImageGen generation, 3 September 2026. References were the supplied
Project-X asteroid capture for painted-volume restraint, the supplied Zynaps
debris capture for readability/density, the accepted Storm Ruins foreground
kit for material identity and the accepted Stormrail enemy family for palette
quality. Exact prompt:

```text
Use case: stylized-concept
Asset type: Sparkpaw Stormrail / Level-2 Storm Ruins debris pixel-art concept sheet; reference-only, not a runtime sprite sheet.
Input images: Image 1 is Project-X Amiga rock lighting/volume restraint reference only; Image 2 is Zynaps Amiga density and readable-rock reference only; Image 3 is the authoritative Sparkpaw Storm Ruins castle masonry/material identity; Image 4 is the accepted Sparkpaw native enemy polish and palette-role quality reference.
Primary request: Create one clean orthographic concept sheet showing three original families of airborne Storm Ruins debris: (A) shattered carved arch/lintel masonry, (B) broken gothic buttress/pillar fragments, and (C) storm-weathered massive castle wall chunks containing subtle structural Stormstone traces. For each family show one large, one medium, and two small related fragments, clearly separated and all genuinely different silhouettes. These are pieces of a ruined fantasy storm castle, never space asteroids.
Style/medium: premium late-era Amiga AGA pixel art concept, crisp deliberately placed pixel clusters, readable at small gameplay scale, painted volume rather than line art. Use broad faceted material planes, chipped asymmetric silhouettes, sparse pitting and erosion, deep fracture faces and consistent upper-left illumination. Darkest pixels are local material shadow only, not a continuous outline. Details follow planes and mass.
Composition/framing: organized object study on a flat solid magenta chroma background, generous separation, no overlap, no scene, no perspective floor, no cast shadows, no labels or text. Large pieces roughly 3x the visual width of small pieces. Include a few alternate true tumble orientations as separately redrawn views, with stable material identity and lighting.
Color palette: restrained Sparkpaw FRONT16-like roles—neutral charcoal, violet-grey, mid steel-grey, pale stone highlight, tiny controlled cool blue/cyan Stormstone reflections, extremely sparse muted bronze weathering. Preserve strong contrast without large saturated panels.
Constraints: original geometry; castle ruins unmistakable through carved arch curvature, masonry joints, broken moulding, buttress ribs and fractured blocks. Clean binary-looking edges suitable for later hand translation to native 48x40, 32x32 and 16x16 masked sprites. No scaling artifacts, anti-alias blur, gradients, semitransparency, thick black contour, comic inking, cel-shaded sticker look, WordArt, regular rectangular programmer blocks, neon cracks, decorative holes, purple sci-fi panels, round boulders, planets, metallic asteroids, enemies, ship, player, diamonds, explosions, HUD, typography, watermark.
```

Saved as `assets/concept/sparkpaw-stormrail-debris-study-v1.png`.
Status: rejected as the direct debris direction. Its material richness is useful,
but complete arches, bridges and towers read as miniature scenery; repeated
scale ladders would collapse at native size.

## Stormrail Debris 2.0 art study v2 — broken-mass review direction

Built-in ImageGen edit/generation, 3 September 2026. V1 is material reference,
Project-X is painted-volume reference and the Storm Ruins kit remains identity
authority. Exact prompt:

```text
Use case: stylized-concept
Asset type: revised Sparkpaw Storm Ruins airborne-debris pixel-art concept sheet.
Input images: Image 1 is the first study and is only the material richness reference; fix its complete-building, repeated/scaled-family and over-detailed problems. Image 2 is the Project-X Amiga reference for broad readable rock volumes and restrained internal marks, not for asteroid subject matter. Image 3 is the authoritative Storm Ruins material and architectural language.
Primary request: Create a completely revised sheet of exactly twelve isolated airborne broken castle-ruin chunks, arranged in three rows of four. Row 1: four large 48x40-proportion chunks. Row 2: four medium 32x24-proportion chunks. Row 3: four small 16x16-proportion chunks. Every piece must have unique geometry, not a resized duplicate. Mix unmistakable partial clues—one severed arch spring, one broken lintel corner, one buttress rib, one battered battlement cap, one fractured pillar drum, and irregular masonry masses—but no complete arch, bridge, tower, wall, doorway, facade or standing building.
Style/medium: premium late-era Amiga pixel art, deliberately chunky and simple enough for hand translation to 48x40/32x24/16x16. Painted mineral volume like the Project-X reference: three or four broad connected light/shadow planes per piece, a few purposeful chips/pits, deep fracture faces, consistent upper-left light. Reduce Image 1's tiny brick-by-brick detailing by at least 70 percent.
Composition: flat solid magenta chroma background, generous gaps, no overlap, no ground line, no cast shadows, no labels. Objects float at varied orientations and look torn from one Storm Ruins castle.
Color/material: charcoal-violet fracture shadow, dark steel-grey, mid weathered grey, pale stone highlight; tiny sparse cyan mineral/Stormstone glints on only three of the twelve objects; tiny muted moss/bronze flecks on at most two. The material remains neutral stone overall.
Critical constraints: no continuous black outer contour; edge pixels use local dark stone and broken highlights. No comic inking, sticker look, cel shading, WordArt, anti-aliasing, blur, gradients or semitransparency. No regular black seams around every brick. No scaled duplicates. No round asteroids, meteor craters, planets, metal rocks, sci-fi panels, neon cracks, decorative holes, ships, enemies, pickups, explosions, HUD, text or watermark. The large pieces must read as broken masses first and architectural remnants second.
```

Saved as `assets/concept/sparkpaw-stormrail-debris-study-v2.png`.
Status: pending user direction review. V2 is substantially closer: isolated
broken masses, unique size silhouettes and broad lighting planes. It remains a
high-resolution reference only; its remaining brick seams must be simplified
again during any native 48x40/32x24/16x16 hand translation.
# Stormrail Gate 4A ruin masonry concept — pending review (3 September 2026)

- References: rejected Gate-4A procedural obstacle sheet and accepted
  `sparkpaw-stormrail-runtime-family-v8-aga16.png` enemy family.
- Prompt intent: replace the smooth faceted boulder with one coherent family of
  genuine castle-ruin masonry: intact and damaged 48x40 targets plus three
  distinct 16x16 derived chunks. Requested layered blocks, a partial carved
  arch/lintel, chipped stone, cold upper-left lighting, clean masks and restrained
  warm fracture glints. Explicitly excluded purple panels, neon surface lines,
  holes, round pebbles, scaling artefacts and programmer geometry.
- Built-in ImageGen output:
  `/Users/mpoelstra/.codex/generated_images/01a062bd-c934-7733-964e-750369b28e6d/exec-fa932929-4e82-4aab-b1b4-3f0b3d5b7d6e.png`
- Status: **visual direction approved 3 September 2026**. The high-detail
  concept itself remains reference-only and was not scaled into runtime art;
  its masonry/arch language was redrawn natively at 48x40 and 16x16.


## INSERT DISK typography candidate

23 September 2026 follow-up: the oversized pasted numeral 3 and two native
code-drawn revisions were rejected by the user. Built-in imagegen edit used
`assets/concept/sparkpaw-insert-disk-type-v1.png` as the style/edit reference
and produced `assets/concept/sparkpaw-insert-disk-type-v2-disk3.png` (1918x820).
Prompt: "Edit the attached Sparkpaw INSERT DISK typography reference. Produce
one new, isolated horizontal text line that reads exactly 'INSERT DISK 3' on a
pure black background. Match the existing 'INSERT DISK 1' and 'INSERT DISK 2'
lines precisely in font construction, glyph width and height, baseline,
spacing, chunky angular pixel-art letter geometry, cyan/turquoise bevel, pale
top highlights, dark navy stepped outline, and small gold corner accents
around the numeral. The numeral 3 must read unmistakably as a single elegant
3, with a curved upper and lower bowl, open left side, distinct central waist,
and the same height and weight as the existing numeral 2. Do not create a
second 2 or stack shapes. No floppy illustration, no window, no extra text,
no different font. Wide 16:7 composition with generous pure black margins;
the line should occupy approximately the same fraction of width and height as
one reference line." Generator crops the full line and uses the same 216x24
native conversion as 1/2. Status: user visually approved the native 1/2/3
preview on 23 September 2026; ADF runtime/full-campaign acceptance remains
separate.

User screenshots reference plain INSERT and styled LOADING. Built-in imagegen
prompt: text-only INSERT DISK 1 and INSERT DISK 2, one line each, chunky angular
cyan/turquoise bevelled pixel letters, pale top bevel, navy stepped outline,
gold corner accents, black background; no floppy or window. Source saved as
assets/concept/sparkpaw-insert-disk-type-v1.png. generate_disk_status.py converts
the two bands to 216x24 in the unchanged loading palette inside the 224x40 patch.
Native and 2x preview inspected. Shared floppy artwork is byte-exact. User
visual/native acceptance pending; no release change.


## Game over — approved V2 (9–10 September 2026)

Built-in imagegen produced `assets/concept/sparkpaw-game-over-v1.png`,
then `sparkpaw-game-over-v2.png`, using the existing Sparkpaw cover source
as character reference. Prompt intent: full-screen AGA-style ruined landscape,
Sparkpaw on a stone ruin, quiet defeat, dark sky and open space above for
GAME OVER / total score / BACK TO TITLE. V1 looked too rested. The user
requested “Sparkpaw duidelijker verslagen tonen”; V2 used a bowed head and
slumped, exhausted pose, alive and without blood. The user explicitly approved:
“Ja, werk V2 uit”. This is a prompt-intent record, not a verbatim transcript.

`tools/generate_game_over.py` converts V2 to 320x256, six planes, 64 colours,
black pen 0 and reserved score ink 63. Static labels are baked offline; only
the total digits are written once into a hidden bitmap before display. Native
preview: `assets/concept/sparkpaw-game-over-aga64-preview.png`. Same artwork
for HD, WHDLoad and ADF. Native user acceptance remains pending.
# Drowned Turbines conceptboard V1 — 13 September 2026

Status: pending user concept review; no native conversion or runtime integration.
Generated with built-in imagegen. Saved: `assets/concept/drowned-turbines-board-v1.png`.
References: `assets/concept/sparkpaw-visual-slice-concept-v2.png` (environment style)
and `assets/concept/sparkpaw-stormkeeper-waystation-source-v2.png` (station identity).
The machine-finale panel is an undecided study. Small/large mobile enemies and
the separate Rain Core weather station follow the user's approved direction.

Exact prompt:

> Use case: stylized-concept. Create one polished pixel-art concept review board for Sparkpaw Level 2 DROWNED TURBINES. Reference image 1 establishes the existing world's premium pixel art, side-on platform readability and low ground strip; reference 2 establishes the friendly little Stormkeeper waystation architectural identity. These are supporting references, not edit targets. New setting: flooded rain machinery, broad round turbine housings, wet petrol steel, light ceramic walkable lips, restrained aged copper, turquoise water and cool rainlit depth. Four clearly separated panels in a landscape board: top left large representative gameplay side elevation with dry low foreground platforms, one modest water gap, raised route, distant turbine silhouette and a small telegraphing pressure outlet; top right quiet end-level weather-station cottage with ceramic pitched roof, copper rain gauges, warm doorway and a distinct floating droplet-shaped Rain Core in front, calm water and safe dry approach, echo reference cottage proportions but make a new rain station; bottom left smaller enemy design study, a low steel cleaning beetle with copper brushes, plus a larger articulated bipedal pump warden with long spring-jointed legs and integrated arm nozzle, both facing sideways, charcoal steel dominant, restrained violet/cyan accents, small warm attack indicator, deliberate material depth, coherent scale with larger about twice smaller height; bottom right an optional machine-finale side elevation study: three fixed reachable platform tiers, small locks on piping, large static turbine behind, clear safe standing spaces and one vertical pressure jet, exit leads toward weather station. Labels only DROWNED TURBINES, BYPASS, RAIN STATION, ENEMIES, MACHINE STUDY. No hero character, HUD, fake screenshot UI, collectible diamonds or text paragraphs. This is concept art, not native runtime output. Restrained colours, carefully clustered pixel art, broad light planes, dark recesses, irregular rich silhouettes, no uniform black outlines, no isometric perspective, no realistic rendering, no excessive neon. Keep large calm readable space above platforms and distinguish dark non-colliding background machinery from light walkable foreground. The machine study is an option, not a boss creature.
# Drowned native-resolution source V1 — 13 September 2026

Built-in imagegen; reference `assets/concept/drowned-turbines-board-v1.png`,
whose overall direction the user approved. New source saved as
`assets/concept/drowned-turbines-slice-source-v1.png`; native conversion and
review outputs under `assets/concept/drowned-native-v1/`. Native review pending.
No runtime integration. Exact source-generation prompt:

> Create a single environment-only side-view pixel-art source for the approved Sparkpaw Drowned Turbines concept (reference). Expand only the upper-left BYPASS scene into one full 1536x1024 image, no borders, no labels, no text, no characters, no enemies, no HUD. Preserve large misty circular turbines, dark petrol steel, aged copper pipes and layered wet mountain depth. Crucial composition for subsequent 320x208 native palette conversion: a low dry continuous foreground ledge across bottom ~6% of image except one water gap from x=40% to x=65%. Walkable ground perfectly horizontal at 94% image height. One raised platform on the right from x=70% to the right edge at 65% image height, thick visible pale ceramic lip and dark steel face; one narrow supporting pillar below it at x=85%. The rest is distant noncolliding parallax scenery, subdued, with a giant recognizable turbine occupying middle-left background and distant waterfalls. No waterfall or pressure jet in front of walkable surfaces for this static material proof. Broad connected light and shadow planes, beautiful deliberate pixel clusters, controlled detail, no tiny glitter, no mossy ruin wall replacing metalwork, no isometric perspective. This is an art source, not a screenshot. Keep the upper and central play area spacious and calm. Pixel-art lighting and depth consistent with reference.
# Drowned clean rear V1 — 13 September 2026

Built-in imagegen edit of approved `drowned-turbines-slice-source-v1.png`.
Saved as `assets/concept/drowned-rear-clean-source-v1.png`. Foreground removed
for independent parallax, not a new art direction. Inspected: platforms, bank
edges and near-field pipes removed. Native palette and layer study is under
`assets/concept/drowned-layers-v1/`; native motion/hardware acceptance pending.

Exact prompt:

> Edit the supplied approved Drowned Turbines source into a CLEAN BACKGROUND-ONLY parallax layer for a side-scrolling Amiga platform game. Preserve the large turbines, mountain valley, atmospheric depth, dark petrol/blue tones and waterfalls, retaining their composition and premium clustered pixel-art treatment. REMOVE ALL foreground: the entire low floor ledge along the bottom, both bank edges, the raised platform at right with all its supports and nearby foreground rock mound, and the tall near-field wall and foreground pipes at far left. Inpaint these removed areas with continuous distant misty forest, rock and basin scenery matching the existing depth; absolutely no floating islands, platforms, walkable bright rims or front-facing near walls. At the bottom the distant water basin recedes quietly in muted blue, no bright foamy foreground water strip. No new objects, text, HUD, characters, enemies or frame. Important: this must be usable as the independently scrolling rear layer: there must be no remaining duplicate foreground platforms anywhere. Keep original image size and identity of background landmarks.
# Drowned full-level rear panorama — 13 September 2026

User requests full-level background composition alongside the bounded slice.
Built-in imagegen, reference `assets/concept/drowned-rear-clean-source-v1.png`.
V1: `assets/concept/drowned-full-rear-source-v1.png`, preserved but not used for
native conversion: generated aspect too tall; squeezing would deform turbines.
Exact V1 prompt:

> Use case: stylized-concept. Create ONE continuous ultra-wide BACKGROUND-ONLY parallax panorama for the entire Sparkpaw Drowned Turbines platform level, in the exact clustered premium pixel-art atmosphere of the reference. Request very wide canvas about 3360x624, aspect 5.38:1, full image used by landscape, no panels. This is the background behind gameplay, no walkable foreground, platforms, ground strips, characters, enemies, HUD, text, borders, floating Core or foreground cottage. Compose the actual picture in this left-to-right order with smooth natural transitions and one connected distant landscape: left 0-25% open rainlit maintenance inlet, mountain silhouettes, distant slender intake pipes and a single circular turbine landmark; 25-50% denser flooded turbine courtyard with two large distinct round housings, broad connecting pipes and falling water; 50-75% imposing main pressure-control hall across the basin, heavy tall ribbed architecture and one enormous enclosed turbine, darkest and most enclosed but still side-on distant; final 75-100% gradually opens into a quiet sheltered rain-garden valley with receding machinery, calm mist, a few tiny distant copper rain-gauge towers and light breaking through clouds, calm dark empty lower space where a separately drawn foreground weather-station cottage will later sit. Keep all landmarks distant and non-colliding; no boss or finale action, because finale gameplay is undecided. All four regions must share horizon, materials, lighting direction and continuous water-basin/forest depth; no visible vertical joins, partitions or abrupt colour changes. Petrol/navy shadows, steel blue mist, pale blue rainlight, extremely restrained muted copper. Strong readable turbine silhouettes, broad connected value planes; details must survive reduction to a 1120x208 eight-colour image. Avoid excessive tiny noise, mirrored/repeated scenery, neon, purple storm ruins, isometric perspective, photographic realism. Do not copy one repeating turbine composition across the panorama. Upper portion contains atmosphere/architecture, lower portion is calm distant basin/forest with no bright foreground edges. One coherent panoramic painting, not a contact sheet.

V2: `assets/concept/drowned-full-rear-source-v2.png`, letterboxed composition,
reference V1. Inspected crop `(0,190,2057,572)` preserves proportions to within
0.2% at 1120x208. Native indexed output, manifest, full camera review and six
example views: `assets/concept/drowned-panorama-v1/`. User review pending;
no runtime integration. The station is separate foreground art; Core a Bob.
Exact V2 prompt:

> Create a letterboxed ultra-wide parallax panorama using the provided Drowned Turbines panorama as style/content reference. IMPORTANT LAYOUT: output a regular landscape image with a PERFECT SOLID BLACK LETTERBOX covering the top 25% and bottom 25%. ALL SCENERY MUST BE INSIDE the middle 50% horizontal strip from edge to edge. The central strip must have approximately 5.38:1 aspect ratio, like a very wide film shot, with complete round turbine silhouettes that fit within that narrow strip, NOT squashed or cropped. The black bars are intentional technical crop guides. No text, no frames within the strip. Compose a continuous distant background from LEFT: open maintenance inlet with a small round turbine and slender pipe towers; then two distinct medium round turbines and flowing water; then one imposing tall pressure-control building with a larger enclosed ROUND turbine; then RIGHT: gradually quieter sheltered valley and receding rain-gauge towers, with calm space for a separately rendered foreground weather station. No house in this image. No foreground floor, platforms, bank lips, player, HUD, enemies, Core or boss. Background-only atmospheric mountain/forest/industrial valley. Same restrained steel-blue, petrol and navy pixel art, broad light/shadow planes, connected mist, sparse distant waterfalls. Preserve circle proportions, fit all landmarks to narrow central band by composing a distant wide-angle landscape. No repeated panels or vertical seams; one continuous strip. Exact clean black letterbox top and bottom is crucial.
# Drowned polish direction — 13 September 2026

Built-in imagegen. New material/form study saved as
`assets/concept/drowned-polish-direction-v1.png`. User review pending.
Previous playable foreground and mechanism blockouts rejected visually.
This is not indexed native art or an approved collision layout: generated
platform height, floor depth and detail density require redesign at real scale.
No runtime integration or new test build in this step.

Exact prompt:

> Use case: stylized-concept.
> Create a new premium pixel-art art-direction concept for DROWNED TURBINES, a side-view Amiga AGA platform game set in a flooded weather power station. One coherent environment scene, landscape aspect about 320:208, no panels, no typography, no HUD, no characters or enemies. This is an art review, not a screenshot.
> Art objective: exceptionally deliberate, crisp low-resolution pixel clusters, clean controlled contours, readable material planes, restrained highlights, no automatic photo pixelation, no stippled noise or dirty dithering. Design as if drawn on a 320 by 208 pixel grid and enlarged with nearest-neighbor; do not rely on subpixel detail, smooth gradients, bloom or high-resolution painted texture.
> Composition: fixed side-on platform gameplay. Main walkable thin steel deck at 96 percent image height, one small water opening toward the left; a raised maintenance platform on the left at about 77 percent height with a bright steel cap and sturdy beautifully designed supporting bracket. Keep the middle lower space open for a 48px-tall player. At right a compact functional sluice shutter within a substantial bolted industrial frame, guided metal slats, visible upper roller housing and pipes anchoring it to the plant. A small amber pressure gauge/control panel is physically attached to this assembly, facing the approaching player, no floating button or floating bar. Shutter belongs to the foreground and occupies lower right roughly 72 native pixels tall and 48 pixels wide. Near middle-left a small pressure nozzle mounted flush into the deck with a narrow cyan water jet, shaped by connected curling water clusters and a few meaningful droplets, not a straight laser.
> Background: beautiful distant monumental ROUND turbine housing integrated into a hydroelectric weather station across a deep basin. Simplify and strengthen its architectural silhouette, larger calm shadow masses, a restrained distant waterfall, layered mountain shapes and sparse grouped conifers instead of endless granular tree texture. Distinct depth separation: muted navy/petrol/steel-blue background, dark warm charcoal foreground with cool clean steel edging, selective aged copper plumbing and tiny amber controls. Background detail quieter than interactive machinery; beautiful intentional use of negative space. Suggest a restrained 8-colour background ramp and a 16-colour foreground ramp, not a rainbow. No purple ruins, lava, generic floating stone blocks, excessive rust, overly ornate greebling, particle fog or full-screen rain. Crisp attractive pixel-art craft and readable playability are paramount. No claims or labels about actual memory/performance.
# Drowned foreground native review — 13 September 2026

User approved polish-direction-v1 as art direction. Built-in imagegen edit
reference: `assets/concept/drowned-polish-direction-v1.png`.
Output: `assets/concept/drowned-polish-parts-source-v1.png`.
Source did not obey exact layout; components were separately scaled into
native review geometry. Palette/index conversion, source-key masks and
unchanged player/HUD composition: `tools/preview_drowned_polish.py`.
Result `assets/concept/drowned-polish-native-v1/preview-4x.png` remains a
static art review, not accepted runtime graphics. Rear master unchanged.

Exact prompt:

> Edit the reference into a FOREGROUND-ONLY production art study for the same Amiga pixel-art game. Preserve the approved beautifully designed dark steel, cool silver edges, bolted brackets, copper pipework and functional shutter identity. Remove ALL distant scenery and background water, sky, mountains, buildings and trees; use perfectly solid RGB black for all empty space. No text, HUD, player, labels or decorative borders.
> Critical layout: image aspect 320:208. Design the art on a coarse logical 320x208 grid, enlarged uniformly about 4x; deliberate low-resolution connected pixel clusters and clean silhouettes, not high-resolution speckling. Exact intended coordinates on this logical grid: floor walking surface at y200, deck occupies ONLY y200..207; break the deck at x80..159 (leave solid black there for separately animated water). Left raised platform is x0..63, top y160, cap 8 pixels deep, one sturdy triangular steel bracket beneath it extending to y199. No tall platform or overhead wall. The playable central area x160..223 above the floor must remain black and empty for the separately composited existing player. At right, the closed shutter door occupies x256..287, y128..199; bolt-on side guides and upper roller housing extend to x248..295, y112..199. A pressure control unit is physically attached on its LEFT, x228..247,y157..184, with an amber gauge and copper feedpipe joining the door casing. Make it clearly one integrated machine, not a floating rectangle. Door bottom meets floor at y200. Keep x296..319 clear apart from floor. No pressure jet in this first foreground review.
> Materials: broad 3-4 tone charcoal/steel shading, one-pixel rim highlights, sparse purposeful bolts and seams. Copper is a limited accent, not orange outlining everywhere. Maximum visual palette about 15 opaque colours plus black transparent key, tonal cohesion with the reference. No glow, blur, gradients, excessive tiny texture, painterly noise, random stippling or repeated rough stone. Flat SIDE VIEW appropriate for side-scrolling platform collision, not isometric. Frame and support art must read instantly after reduction to 320x208. This is a precise layout adaptation of the reference, not a concept poster.
# Drowned pressure jet animation — 13 September 2026

Native foreground review accepted by user. Built-in imagegen generated a
4x2 water-only animation sheet: `assets/concept/drowned-jet-source-v1.png`.
Native conversion uses one uniform whole-sheet scale and fixed cell crops;
no per-frame fit or baseline correction. Frames mapped to pens 0/5/6/11 of
the approved-review foreground palette. Fixed nozzle cropped from approved
`drowned-polish-direction-v1.png`; player/HUD source unchanged.
Review outputs: `assets/concept/drowned-jet-native-v1/`, generated by
`tools/preview_drowned_jet.py`. Frame sheet inspected. Animated GIF is an
offline art review, not native timing evidence. User animation review pending.

Exact prompt:

> Create one precise 4-column by 2-row sprite animation contact sheet, on perfectly solid black background, all 8 cells identical size, no labels, no gutters, no outlines around cells. Each cell represents exactly 32x64 logical pixels enlarged by the same integer scale. Overall image aspect 2:1. Crisp low-resolution pixel clusters for a beautiful Amiga AGA side-view game, no antialiasing, photographic texture or blur.
> Subject: the SAME slender vertical industrial WATER pressure jet in every cell, cyan water with a few white highlights, darker blue inner shadow. Not electricity, not a laser, not smoke. Source point fixed at bottom centre of every cell, ground at logical y60. No nozzle or machinery drawn: only water on black; nozzle will be a separate fixed foreground asset.
> Read cells left to right then next row: cell1 small pressure bubbling at bottom (height8px); cell2 rising short water plume(height24px); cell3 full-height water jet(height48px) with slight left-curving top and two detached droplets; cell4 full-height jet with different flowing connected clusters and slightly right-curving top; cell5 full-height jet another flow phase, top curling left; cell6 full-height jet another flow phase top slightly right, designed to loop back to cell3; cell7 collapsing plume(height28px) and falling droplets; cell8 last low splashes(height10px).
> Keep active jet's main shaft narrow, about6 native pixels across, smoothly connected from logical y58 to y16, most of width reserved for sparse droplets, total effect contained within x6..25 and y8..60. The active four frames must look like a coherent flowing water column, not four unrelated explosions. Only 4 opaque colours: medium blue, cyan, pale cyan and white; empty space pure black. Every cell same baseline, same scale, same centre. Strong clean silhouettes readable at actual 32x64. No text, numbers, characters, ground, equipment, gradients, glow or mist.


## Drowned geyser v2 — 13 September 2026 — concept pending

Built-in imagegen. Existing jet/nozzle rejected after MOV review.
New source: `assets/concept/drowned-geyser-source-v2.png`.
Exact prompt and generation constraints: `assets/concept/drowned-geyser-source-v2.txt`.
Four-stage recessed boiling-water direction, not native palette or integrated art.


## Drowned geyser v2 — approved and expanded, 13 September 2026

User: “ok oogt ok ga verder”. Approved recessed-water direction; built-in
imagegen expanded the family to eight water-only frames using the approved
source as reference. Saved `assets/concept/drowned-geyser-animation-source-v2.png`;
exact prompt in its matching TXT. Native output under `drowned-geyser-native-v2/`
uses existing water pens, fixed family scale and shared row registration.
Integrated in isolated Slice3, native playtest acceptance still pending.


## Solid sluice housing — 13 September 2026 — concept pending

User accepts gate passage but rejects airy upper machinery and jumping through
it. Built-in imagegen produced `assets/concept/drowned-solid-gate-source-v1.png`
from supplied closed-gate screenshot. Exact prompt in matching TXT.
Closed/open concept pair; one canonical frame required for native integration.
Solid roller housing, opaque lower beam, same industrial identity. No runtime
integration before review. See DROWNED_GATE_SOLIDITY_REVIEW.md for physics plan.

Solid gate concept accepted (“ok ja mooi ga verder”). Slice4 integrates one
opaque housing crop into the existing native frame; no independent closed/open
whole-image conversions. Native gameplay acceptance remains pending.

## Drowned enemy concept v1 —15 September2026

Built-in ImageGen; pending user design review, not runtime integration.
Source: `assets/enemies/drowned-enemies-concept-v1.png`.
Exact prompt archived in `assets/enemies/drowned-enemies-concept-v1-prompt.txt`.
The model produced a shaded dark background instead of flat magenta and
incorrect relative native scale; see DROWNED_ENEMY_ART_PLAN.md. The poses are
concept directions only; production must use a locked native master and rig.

### Exact prompt

Use case: stylized-concept. Create a premium Amiga AGA side-view platform-game ENEMY CONCEPT SHEET for Sparkpaw's Drowned Turbines, an abandoned rain-powered pumping station. References show the existing 64x64 biped enemy, 32x24 small enemy, and industrial environment. They are scale/material/legibility references, NOT designs to copy. This is design review, not a runtime-ready sheet.

Exactly TWO new mechanical enemy designs, four poses each, in a spacious strict 4-column by 2-row layout. Uniform flat magenta #ff00ff background throughout, no text, labels, dividers, ground shadows, scenery or loose projectiles. Facing right in every pose. Crisp hand-crafted pixel-art clusters, deliberate stepped edges, strong silhouette, consistent top-left highlights, no smooth gradients, noisy dithering, blur, miniature rivet noise or 3D rendering.

TOP ROW: small TURBINE CRAB, designed for a 32x24 native cell: low squat storm-grey turbine shell with a visible recessed three-bladed rotor in its SIDE panel, FOUR articulated walking legs total, two nearer legs clearly distinct and two darker far legs correctly occluded. Compact folded gripping claw integrated at front; no fifth/sixth walking limb. Rear exhaust slot, turquoise sensor slit. Strong coherent plates. It must NOT resemble a recoloured beetle. Same chassis dimensions throughout.
Four poses: 1 confident idle with planted feet; 2 compressed attack anticipation with claw tucked and rotor shutter opening, feet still logical; 3 short hit recoil, same intact anatomy; 4 destruction endpoint with the SAME chassis collapsed close to ground, rotor stopped, bent legs still attached and front plate split, tiny localized pale-cyan sparks contained in envelope. No generic explosion replacing the animal.

BOTTOM ROW: larger PUMP WALKER designed for a 64x64 native cell, drawn at approximately twice crab width and more than twice crab height, SAME magnification as crab. Permanently BIPEDAL, two sturdy hydraulic legs with clear hip/knee/ankle pivots and broad industrial feet; not quadrupedal, no canine head or tail, not knight/humanoid. Compact pressure-vessel torso, protected round pressure gauge high on shoulder, one heavy short forward-directed pump nozzle mounted to torso (not an extra limb), one small counterbalance service arm. U-shaped protected hose with clear fixed attachment points, no sprawling cables. Rounded boiler mass contrasted with rectangular boot shapes. A cyan inspection slit conveys intent. Dominant cool slate and dark steel; restrained weathered copper only at pipe connectors and gauge rim, NOT a mostly orange robot. Designed so later walking, recoil, jump compression and knees-folding death maintain the identical anatomical rig.
Four poses: 1 tall idle; 2 unmistakable pressure-charge telegraph, knees bend slightly, body leans back, nozzle points right, small cyan gauge brightening but NO muzzle flash or projectile; 3 weighty impact recoil with same body mass, feet still connected; 4 believable collapsed disabled machine: knees folded, pressure vessel lowered, nozzle tilted down, small escaped steam puff inside same 64x64 envelope. No growing limbs, detached replacement robot, massive debris or scale change.

Use a restrained game-compatible palette: deep navy outlines, slate shadows, grey-steel midtones, cool off-white specular accents, muted copper connectors, saturated cyan reserved for water pressure/sensors. Make the crab and walker immediately distinct at tiny game size, with recognizable material masses instead of microtexture. Each family uses one scale for ALL poses including death; do not resize a pose to fit. Leave abundant clean margins. Preserve exact number and attachment of limbs across poses; read as animateable machinery.

## Drowned enemy colour variant v2 —15 September2026

Built-in ImageGen edit of approved design direction v1. Colour review pending;
source `assets/enemies/drowned-enemies-concept-v2.png`, v1 retained.
No runtime integration or exact pixel-preservation claim.

undefined

## Drowned native idle source v1 —15 September2026

Built-in ImageGen, approved v2 colour direction as reference. Isolated idle
source saved assets/enemies/drowned-enemies-idle-source-v1.png. Real alpha
verified; subsequent deterministic native palette/cell conversion is offline
review only. Native approval pending, no runtime change.

Create a production-oriented isolated idle-master source from the approved reference enemies. Exactly TWO figures, both right-facing, no other poses. LEFT: the copper turbine crab from top-left reference, four connected walking legs and compact claw, dark rotor, cyan slit. RIGHT: the cool-grey biped pumpwalker from bottom-left reference with exactly two hydraulic legs, same pressure tank, gauge, nozzle, hose and service arm. Preserve the approved mechanical anatomy and material colors. True TRANSPARENT alpha background, no background gradient, no floor, no shadows, no glows outside silhouettes, no text or borders. Widely separate the figures with ample transparent margin. Both feet planted on same baseline. Crab roughly HALF the pumpwalker's width and around one third its height: intended final cells32x24 and64x64. Crisp deliberate pixel-art clusters, simplified durable shapes suitable for native Amiga pixels: leg connections must remain readable, avoid surface speckle and micro-rivets. Keep pumpwalker idle tall rather than squat, leave room for its pose within64x64. Copper-brown shell for crab, dark steel legs/rotor; cool slate steel for walker with copper confined to small fittings. Cyan reserved for tiny sensor and pressure accents. No redesign, extra limbs or extra objects. This is one stable idle silhouette per creature for subsequent exact palette conversion and fixed-part animation; do not make an animation sheet.


## Drowned pontoon concept v1 — 16 September 2026

Built-in ImageGen. Concept pending user review; not runtime art. Source: assets/concept/drowned-pontoon-concept-v1.png. Exact prompt: assets/concept/drowned-pontoon-concept-v1.txt. Steel/copper low deck with foreground water overlap. Generated enlarged concept has more detail and slight top-face perspective than target native96px art; native simplification and exact palette/water matching still required.

## 2026-09-16 — Spillwing compact concept review

User requested smaller flying enemy and approved24x24 cell direction, body
approximately18x12. Generated spillwing-concept-v1.png with built-in ImageGen,
existing enemy material sheet as reference; exact prompt in same-name TXT.
Compact steel pod, short connected top rotor, copper fittings and forward sensor.
Provisional24x24 exact foreground-palette size audition produced by
 tools/preview_spillwing_idle.py in assets/enemies/spillwing-idle-review-v1/.
Scale strip uses existing player and approved96px pontoon unchanged. Source
concept and provisional native clusters await user review; no animation,
AI, cache, runtime or test-drawer changes. Next refine native sensor/silhouette,
rotor poses, anticipation/swoop, hit and electric death only after review.
No per-frame autoscaling or new actor anatomy. Full level/upper route remains
planned; native flight envelope must stay compact for its clearance.

## 2026-09-19 — Drowned checkpoint beacon concept v1 (pending review)

Generated concept: assets/concept/drowned-checkpoint-beacon-v1.png.
Prompt requested one industrial steel/copper checkpoint marker in three aligned
side-view states STANDBY / ACTIVATING / SAVED, targeting32x48 native pixels,
dark navy background, hinged metal pennant and amber-to-mint/cyan indicator,
AGA pixel clusters, no cloth/holograms/character. Concept review only, not a
runtime sheet. Result has excessive presentation glow and an inconsistent
intermediate hinge location; neither may carry into native animation.
Full prompt retained in accompanying concept TXT. No approval or integration yet.

## 2026-09-19 — Drowned checkpoint production components v1

Reference: assets/concept/drowned-checkpoint-beacon-v1.png (user approved).
Generated source: assets/concept/drowned-checkpoint-parts-v1.png, copied intact
from exec-60718085-c23b-4927-b015-6d45f73fccc3.png; original preserved.
Prompt intent (reconstructed after context compaction, not a verbatim transcript):
three separate pixel-art components on magenta: identical upright static bodies,
amber/cyan medallion and no pennant; detached horizontal pennant with left hinge;
24x48 body,18x7 arm intent, large nearest-neighbour presentation. No scenery,
labels or glow, keep approved steel/copper identity and fixed hinge.
Output instead used a dark halo backdrop, so it is NOT used as transparent art.
Explicit silhouette excludes that backdrop; same body for every frame, only
lamp pens change. Native fit28x48body/26x9arm in48x48 cell; fixedpivot21,11;
offline rigid rotation90,80,65,45,25,10,0,0deg, owned16pen foreground palette.
Native frames/animation reviewed from exact indexed data in
assets/concept/drowned-checkpoint-native-v1; generator tools/build_drowned_checkpoint.py.
Integrated under the user's explicit approval of beacon and soundv2. Native
in-game animation/readability still awaits user020 test, no runtime claim.

## 2026-09-20 — Drowned finale/station concept v1, review pending

Concept review v1, 2026-09-20. Not approved or native runtime art.
Reference: drowned-pontoon-native-v1/scene-4x.png (palette/material only).
Prompt:
Create a polished PIXEL ART concept review board for the final section of the Amiga AGA game Sparkpaw: Drowned Turbines. Use attached screenshot as material and palette reference only, not its raft layout. Two wide horizontal side-view panels stacked, with NO text, NO HUD, NO logos. True readable pixel clusters, limited palette, dark blue steel, muted copper pipes, cyan water and amber indicators; avoid grain and overbusy microtexture. TOP PANEL: an outdoor hydroelectric Pressure Governor machine encounter. A solid dark-steel turbine regulator spans three connected buttresses with thick copper pipework; exactly THREE clearly readable small circular shootable shutter targets at low, middle, low heights. The middle one above a sturdy raised platform; two low ones accessible from dry ground. No faces or monster anatomy. Machine occupies lower half of playable height, rainy blue mountains and distant turbines behind. Clear dry waiting ledge at left, shallow water channels with ONE narrow pressure vent, exit sluice at right. Mechanical shutters and tiny gauges could animate, main machine solid static construction, not a giant animated boss. BOTTOM PANEL: quiet reward area farther beyond the machine: a small separate rain weather-station cabin of steel/copper with sloping rain collector roof, weather vane and round blue lens; a single luminous cyan drop-shaped Rain Core on a little pedestal beside the cabin. Dry accessible path. Cozy functional weather house, NOT another combat machine. Clear foreground/background separation. One small orange fox adventurer (matching reference, same relative sprite scale about40px high in a208px playfield) in each panel only for scale. Concept illustration not claiming final runtime fidelity. Keep everything orthographic side-on, not isometric, no photoreal shading, no bloom, no smooth gradients. Intent: attractive feasible small static foreground assets with restrained tiny animated details.

Review notes: generated fox is scale reference, never replacement player art.
Top machine targets suggest low/middle/low, but platforms and accessible dry
approach require real-physics layout before integration. Bottom path gap is
illustrative only, not approved collision geometry. Background/HUD unchanged
in actual runtime. Rain Core silhouette/color require native palette review.
Mostly static machine; animated parts limited to shutters, indicators, one jet.

## 2026-09-20 — Governor native-art source v1, review pending

2026-09-20: source for native Pressure Governor art. Review pending.
Tool produced RGBA transparency rather than requested magenta; preserved.
Reference: drowned-finale-station-v1.png.
Exact prompt:
Create ONE isolated pixel-art industrial pressure regulator tower asset for Sparkpaw Drowned Turbines, based on the dark steel/copper machine in the TOP panel of the reference. Asset extraction source on a perfectly flat pure magenta #ff00ff background, no scenery, no text, no numbers, no character, no ground shadow. Strict orthographic FRONT/SIDE game elevation with no isometric perspective. Tall sturdy narrow tower silhouette, width to height about 5:8 (intended native80x128 pixels), entirely visible with generous magenta margins. Dark navy-steel riveted casing, thick restrained copper pipes running vertically down both sides, a capped pressure cylinder on top, a small analog gauge in the upper third, broad grounded metal feet at bottom. In the LOWER HALF, a prominent circular recessed pressure port with bright cyan inner glass, framed by a heavy dark steel ring; this lower target port is the gameplay focus. Three tiny amber indicator lamps immediately below port, visibly part of the machine. Make the port large enough to read at native24x32 pixels; no spikes, no gun barrel, no monster face. Professional hand-crafted Amiga AGA pixel art, crisp coherent pixel clusters and restrained highlights, strong material planes, low-noise shading, limited16color appearance using black/navy/slate gray/copper/cream/cyan. Avoid excessive stippling, fuzzy antialiasing, smooth gradients, bloom and neon edging. Enlarge coarse pixels cleanly so the asset remains readable when reduced to80x128. Keep machinery solid with clear silhouette and weight. Only one tower. The reference is identity/material guidance, not an instruction to reproduce the whole environment.


## 2026-09-22 Drowned flowers v1 — concept only

Source: assets/concept/drowned-flowers-v1.png; exact prompt in matching TXT.
Coral/amber low flower clumps; user review pending. Not in runtime.

22September: user explicitly approved integration of both flower species. Native16pen conversion uses violet A and cream B,12sparse placements; A behind player/B foreground feet. No new generated image.

## 2026-09-22 — Drowned panorama seam continuation

Reference: `assets/concept/drowned-panorama-v1/panorama-2x.png`; then its last320x208px enlarged3x as `drowned-panorama-v2/end-reference.png`.

Two whole-panorama studies requested a rightward extension with unchanged scale, waterline, blue8color palette and no foreground objects. Rejected: composition/scale was changed too much; retained as study-1.png and study-2.png.

Final extension prompt: Outpaint this game background to the RIGHT. Widen canvas to approximately2304x624 (3.69:1 wide image). Keep reference unchanged on LEFT960x624. Add1344px to RIGHT with continuous blue mountain lake, dark pine trees on craggy waterfalls, quiet forest grove and distant thin ruined bridges. Same pixel density, exact8-color restricted palette, no smoothing. Waterline y543/624 remains level. Clouds continue naturally. No vertical join, repeat, horizontal stretching, foreground objects, text, HUD or letterbox.

Result: `assets/concept/drowned-panorama-v2/extension-source.png` (actual2172x724). Review conversion crops excess bottom lake, indexes to original palette, joins via offline connected overlap. Native review at1552x208. Status: visual review pending, no runtime/dist integration.
