# Sparkpaw AGA art-quality contract

Sparkpaw targets polished, original AGA pixel art. “Readable” or “technically
valid” is not an art gate: production art must also carry deliberate volume,
material, lighting, silhouette and animation character at native resolution.
This contract applies to new characters, enemies, scenery, obstacles, pickups
and effects unless a more specific accepted baseline says otherwise.

## Required workflow

1. Establish the subject, visual identity and material language in a focused
   concept or native study. Treat accepted Sparkpaw art and the named reference
   games as quality bars, not as assets to copy.
2. Preserve the approved concept's perspective, broad connected light planes,
   occlusion and silhouette while reducing it into the exact runtime cell and
   owned palette. A reduced image is an underpainting, never automatically the
   finished sprite.
3. Clean and retopologize pixels at native size. Judge the result at 1x for
   gameplay truth and at nearest-neighbour enlargement for cluster inspection.
4. Review it against every relevant route palette and beside the accepted
   player/enemy/effect families before runtime integration.
5. Integrate only a meaningful art candidate, then measure memory, restore
   geometry, masks and cadence without silently simplifying the accepted art.

## Automatic rejection signals

- programmer-art blobs or primitive polygon reconstructions;
- flat grey fills standing in for stone, metal or volume;
- a uniform black keyline around the whole object;
- disconnected highlight noise, random dithering or decorative cracks that do
  not follow the object's planes;
- sticker, clip-art, WordArt or generic comic-cutout appearance;
- naïve rotation or scaling of native pixels as final animation art;
- one master sprite mechanically resized into the entire size family;
- saturated colour panels or glowing seams that overwhelm the material;
- detail that looks attractive enlarged but collapses into noise at native 1x;
- “temporary” art entering a user test drawer without being labelled and gated
  as an explicit technical placeholder.

## Positive native criteria

- an immediately readable, irregular silhouette with intentional negative
  space;
- two or three broad connected value planes that preserve depth at speed;
- material-coloured silhouette edges, reserving black for real occlusion;
- sparse highlights and fractures that describe form rather than outline it;
- independently composed size variants, with recognisable shared material but
  distinct masses;
- restrained identity accents drawn from the scene's owned palette;
- animation poses redrawn or re-reduced from suitable source poses, then
  native-cleaned; apparent volume and lighting must remain coherent;
- clean transparency masks and stable visual weight from pose to pose.

Image generation may help establish concept, material and lighting direction,
but generated pixels are never runtime art. Likewise, palette-aware reduction
may establish a native underpainting, but final acceptance remains a visual art
decision followed by a technical renderer gate.

The user should not need to restate this contract as “make it top AGA” in every
prompt. If a request asks for finished Sparkpaw art, this is the default bar.
