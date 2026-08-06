# Built-in ImageGen prompts

These project-bound concept assets were generated with the built-in ImageGen tool and then copied into this workspace.

## Title concept

Create original 4:3 title-screen concept art for the Commodore Amiga 1200 AGA game `SPARKPAW`, subtitle `THE STORMSTONE QUEST`. Show layered storm clouds and mountains, ruined energy towers, forest silhouettes and a rocky technological foreground. Sparkpaw is an original amber-orange young feline adventurer with a navy hood/scarf and teal energy gauntlet. Use authentic premium 1990s Amiga AGA pixel art, crisp clusters and a visually limited palette. Keep both title lines legible. Do not resemble or copy any existing licensed character, logo or artwork.

## Gameplay and parallax concept

Create an original side-view Amiga AGA gameplay scene demonstrating three readable parallax depths: violet storm mountains and clouds, pine forest with broken energy towers and waterfalls, and mossy technological ruins with cyan circuitry. Show Sparkpaw running right and firing a cyan energy bolt, one clockwork beetle enemy and three Stormstone collectibles. Use crisp 1990s Amiga pixel art, a 4:3 320x256-inspired composition and a compact bottom HUD. Keep platforms readable and do not copy existing game art.

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
