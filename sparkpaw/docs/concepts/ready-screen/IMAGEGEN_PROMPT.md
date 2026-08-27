# Ready-screen image-generation record

## Accepted logo extraction source v1

The accepted title image was supplied as the edit reference. The request was
to retain only the complete Sparkpaw lockup (crest, wordmark, paw in the A and
subtitle plaque), remove all title scenery and preserve crisp pixel edges. The
tool returned an RGB checkerboard rather than real alpha; the deterministic
generator therefore removes only bright neutral checkerboard pixels and keeps
the logo colour pixels. Text and final placement remain deterministic.

## Accepted background source v2

References: `sparkpaw-foreground-kit-concept-v2.png` for material language and
`sparkpaw-sprites-reference-transparent.png` for Sparkpaw identity/anatomy.

The prompt requested a 4:3 premium 1993--1995 AGA start-screen background with
a near-black field, a compact Sparkpaw hero pose in the lower-right quarter,
thin asymmetrical Level-1 stone/machine architecture at the sides and bottom,
and reserved empty areas for logo, prompt and lower-left credits. It explicitly
forbade text, logos, sky, copied ThunderCats content, thick UI boxes and a
separator under the future logo. The generated source is accepted as a concept
basis; the runtime generator performs native reduction, logo compositing,
typography and palette control.

An identical first background request failed due to the image-generation usage
limit and produced no project asset. The successful retry produced
`assets/sparkpaw-ready-background-source-v2.png`.

## Accepted layout refinement v3

The v2 background was edited to move Sparkpaw into the lower-right quarter,
lower him toward the bottom rail, replace his tall support with a narrower and
lower platform, and preserve a broad black credit field with balanced left and
right margins. Identity, anatomy, pose, Level-1 materials, black backdrop and
64-colour richness were held invariant. This v3 source is the production input;
v2 remains recorded as the first integrated composition.

## Accepted crest-removal wordmark v2

The isolated v1 logo was edited to remove only the complete winged circular
Stormstone crest above and behind the letters. The orange `SPARKPAW` wordmark,
paw print in the A, cyan contour and full subtitle plaque were held invariant.
As with v1, the tool returned a baked neutral checkerboard; the generator
removes that field deterministically. The crest-free wordmark is placed higher
and all logo, prompt and credit typography remains on the true x=160 axis.
