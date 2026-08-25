# IFF ANIM research for a Sparkpaw intro
Status: research only; no decoder/player or asset pipeline exists here.

## What the format provides

IFF ANIM stores the first frame as a normal ILBM image and later frames as
changes. The formal layout is a `FORM ANIM` containing a first `FORM ILBM`
with `BMHD`, `CMAP` and `BODY`, followed by ILBM forms with `ANHD` timing and
`DLTA` data. Traditional playback uses two screen buffers: apply each delta to
the hidden buffer, then flip it into view. With the common interleave of two,
most frames are deltas against the frame two positions earlier.

ANIM-5 uses byte-oriented vertical delta compression. It handles noisy images
compactly and became the common interchange choice. The specification also
notes a trade-off: some word/long-word delta modes can decode faster but pack
less efficiently. Frame timing is expressed in jiffies and should be tied to
vertical blank rather than the time the decoder happened to finish.

Important implementation consequences:

- an ANIM player is not merely an image loader; it needs chunk parsing, delta
  decoding, timing, buffering and palette-change rules;
- the display size and mode normally remain fixed through one ANIM;
- a palette change must be reflected consistently in both buffers;
- looped ANIMs require specific closing frames/deltas, not only replaying the
  byte stream from the beginning;
- the format can reduce storage, but it does not make decoding or Chip-memory
  cost free.

## Fit for this intro

IFF ANIM is technically plausible, but it is not the best first dependency for
four mostly static story plates. Sparkpaw already has direct AGA title/loading
presentation concepts, while adding a general ANIM-5 decoder would create a
new code, memory, timing and packaging surface before the narrative direction
is approved.

Recommended order:

1. **First proof:** four indexed stills plus scrolling text and palette fades.
2. **Light motion proof:** small pre-authored overlays or a few complete frame
   variants for lightning, pulses and the toast gag.
3. **IFF ANIM experiment only if measured useful:** build one standalone
   320x176 or 320x256 ANIM-5 clip and compare file size, peak buffers, decode
   time and integration complexity against the simple frame route.

Do not select ANIM because it is historically authentic alone. Select it if a
real candidate provides a better total disk/memory/decode result and remains
simple enough to verify on the target A1200 contract.

## Suggested authoring experiment

- Work in Deluxe Paint or another exporter that can produce compatible ILBM
  and ANIM-5 files.
- Lock width, height, depth and one palette before animating.
- Keep most pixels identical between frames; isolate changes spatially.
- Export a short 2–4 second PAL clip, preferably 10–12.5 visual frames/s for
  the deliberately limited style rather than 50 unique frames/s.
- Validate chunks independently on the host: `FORM ANIM`, first ILBM, ANHD
  operation/interleave/timing, DLTA bounds and consistent bitmap properties.
- Verify the actual exported variant. “IFF ANIM” is a family; decoder and file
  must agree on operation, interleave and Store/XOR behaviour.
- Test cold ADF and HD loading separately and never infer real hardware from
  host decoding or FS-UAE playback.

## Measurement checklist

- compressed bytes and ADF blocks per plate/clip;
- two display buffers plus decoder workspace at peak;
- time to read and decode the first frame versus later deltas;
- missed VBlank/frame behavior at 68020 stress speed;
- skip/abort cleanup during disk read and playback;
- palette correctness across both buffers;
- title/loading memory lifetime interaction;
- FS-UAE/68020, FS-UAE/68030, physical ADF and real-A1200 evidence kept
  explicitly separate.

## Sources

- [AmigaOS ANIM IFF CEL specification](https://wiki.amigaos.net/wiki/ANIM_IFF_CEL_Animations)
- [EA IFF 85 / ANIM specification scan](https://amiga.net.au/files/Tech_Amiga/Commodore_EA_IFF_85_Standard.pdf)
- [Amiga Graphics Archive animated examples](https://amiga.lychesis.net/specials/Animated.html)
- [Randelshofer Amiga animation overview](https://www.randelshofer.ch/animations/amiga.html)

The archive demonstrates that low-resolution PAL animation was used with
widely varying palette sizes and sometimes deliberately limited/cropped
material. It is useful as visual evidence, not as a player implementation
specification.
