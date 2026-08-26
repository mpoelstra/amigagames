# Level 1 Core clearing polish plan

Status: implemented, accepted and packaged as the alpha.46 checkpoint.
Supplied FS-UAE/68030 HD evidence accepts presentation and function; the
matching minimal-cadence FS-UAE/68020 HD run reaches 49.67 FPS with zero
three-field misses and ownership violations. ADF gameplay and real hardware
remain open.

## Accepted implementation outcome

- The final camera eases to x=3072 and frames continuous ground around the
  large waystation/tree silhouette without the old portal platform.
- Normal FRONT16 remains authoritative. Warm copper/orange lightning-scorched
  tree and conduit accents were preferred over the investigated green remap
  because they pop against the cold violet rear panorama.
- The attempted extra attached Core pair was rejected: the protected Stage
  5L early-fetch layout cannot safely supply it. The accepted Core is a 64x48
  masked FRONT16 Bob with stable mass and internal idle motion.
- The first directional pickup used thin lines toward Sparkpaw's gauntlet;
  supplied 60-fps evidence showed them ending low and disappearing behind the
  higher-priority player sprite. The accepted twelve-frame family contracts
  into a white centre, releases radial rays/fragments and briefly lifts only
  the foreground palette. No new Sparkpaw animation frames are involved.
- `02-storm-triumph.wav` supplies the selected Paula-ready Core reward. The
  temporary collection latch holds input and delays the existing in-memory
  replay by 50 PAL ticks; permanent progression remains later work.

## Review outcome

The clearing idea survives, but its first presentation does not. The supplied
native runtime screenshot establishes five concrete problems:

1. the old portal/platform still competes at the left edge;
2. the house/tree touches the right edge and has no resting ground beyond it;
3. Sparkpaw meets the Core at the normal 202px right-biased camera anchor;
4. FRONT16 reduction collapses the generated house/tree into pale grey/black;
5. the baked Core reads as a large flat diamond and immediate replay erases the
   reward moment.

The rear panorama itself is not stretched and appears coherent in the supplied
final-screen screenshot. Preserve its accepted first 1024px and 96px mirrored
quiet tail unless a moving runtime inspection later reveals a seam.

## Follow-up: natural one-way threshold

Prefer a physical point of no return over an invisible lock in a later
iteration: Sparkpaw may walk left within the clearing, but the final drop leaves
the preceding portal platform too high to climb back onto. This gives local
palette and performance work a clean end-station boundary while keeping the
world believable. It is deliberately not part of the first polish build.

## Recommended composition without another width increase

Keep the 3392px world. Recompose the final viewport at the existing maximum
camera origin of 3072px:

```text
screen x:  0       60             160             260       320
           | ground | house/tree | CORE | house/tree | ground |
world x: 3072    3132            3232             3332      3392
```

- Move the approximately 200px house/tree cluster from x=3176 to about x=3132.
- Move the Core and pedestal centre from about x=3278 to x=3232.
- Accepted art direction: retain the normal FRONT16 display bank so the mapped
  tree and fittings read as warm copper/orange against the cold violet rear.
  Green remains a tiny Core accent, not the clearing's dominant identity.
- End all old raised portal art at or before world x=3072, so it is completely
  outside the final fixed viewport.
- Keep a continuous y=200 floor from x=3072 to 3391, with approximately 60px
  of readable ground on both sides of the house silhouette.
- When Sparkpaw enters the quiet zone, ease the camera toward its existing
  maximum x=3072 instead of retaining the ordinary 202px player anchor. At the
  Core, Sparkpaw then stands around screen x=160. This is a final-zone camera
  composition only; global camera behavior remains unchanged.
- Begin the camera settle after the old portal has left the viewport. Avoid a
  snap: use the existing five-pixel camera slew and only change its target.

This meets the requested composition without another foreground, rear or
collision-width increase.

## Investigated AGA colour strategy: a clearing palette mode

Do not add a fifth foreground plane. AGA dual playfield supports at most four
planes per playfield; changing to a different playfield split would reopen the
accepted Stage 5L/H7 renderer for modest local benefit.

Instead, use the fact that the final viewport has no enemies, ordinary Storm
Shards, hazards or orange plasma. Once the old portal is fully offscreen, patch
a small set of FRONT16 palette roles in both inactive/next Copper lists:

| Existing pen role | Clearing role |
| --- | --- |
| red/orange accent 2 | deep moss green |
| amber accent 3 | pine-needle green |
| violet accent 7 or magenta 15 | lighter wet foliage/lichen |
| cream 4 | retain as moonlit edge/highlight |
| cyan 5/6 | retain for conduits and Core relationship |
| stone 8..11 | retain for readable architecture |

Author the final house pixels deliberately against this zone palette. Use green
for clustered foliage masses and roots, bronze/rust sparingly on brackets,
pale stone only on lit edges, and navy/violet in recesses. Avoid remapping every
grey pixel from the concept source; the house needs authored material regions,
not nearest-colour reduction.

This is a screen-wide palette-state change, not a horizontal per-scanline
Copper trick. A left/right-local palette split would need repeated mid-line
colour changes and restorations on many scanlines, consuming Copper space and
creating a fragile moving boundary. The zone swap costs only a handful of
colour-value patches when entering/leaving the clearing and adds no bitplane.
Sparkpaw uses the separate AGA sprite palette at entries 32..47 and therefore
keeps his accepted colours.

Fallback if the zone swap is visually insufficient: prove one isolated
hardware-sprite foliage overlay. Do not start there. It would consume another
attached sprite pair, complicate world positioning and compete with the Core's
more valuable sprite use.

## Rejected Core route: second attached AGA sprite pair

Use hardware sprite channels 2/3 as one attached 15-colour Core pair. Channels
0/1 remain Sparkpaw; channels 2..7 are currently pointed at the null sprite.
The Core can share Sparkpaw's existing sprite palette, which already contains
white, cyan, blue, teal, green, gold and deep violet. Benefits:

- a crisp 48–64px-wide silhouette independent of FRONT16 reduction;
- pointer/frame changes rather than foreground restore/draw Blits;
- clean disappearance on pickup without repairing baked house pixels;
- Sparkpaw's lower-numbered pair remains in front during overlap;
- no new playfield plane or palette bank.

Concept target: 40–48px visible crystal inside a transparent 64x56 cell, with
an asymmetrical faceted outline, dark inner fracture, white energy seam, cyan
body, teal lower glow and two restrained bronze/gold keeper arcs. It must not
reuse the ordinary Shard silhouette at a larger scale.

Idle family, suggested six frames:

- frames 0–3: one-pixel vertical float and restrained internal highlight move;
- frames 4–5: brighter two-frame pulse, then return;
- update at 10–12.5Hz while the display remains 50Hz;
- no per-frame zoom, blur, palette drift or rotation.

Before implementation, make a concept sheet and an exact indexed native-palette
preview. Measure sprite DMA, source/staging bytes, Copper pointer patches and
whether the extra pair affects the protected 68020 cadence.

## Collection state: make the reward visible

Replace immediate replay with one narrow temporary `CORE_COLLECT` state. It is
not the permanent level-complete/progression system.

Suggested 32 PAL-frame sequence (0.64 seconds):

1. **Latch, frames 0–3:** trigger once, stop player translation and ignore
   further Core overlap; camera remains locked on the centred composition.
2. **Release, frames 4–11:** keeper arcs open and the Core flashes once.
3. **Release burst, frames 12–23:** crystal contracts into a white-hot centre,
   then breaks into broad asymmetrical cyan/white/bronze radial rays and small
   fragments. A two-field foreground-palette lift briefly illuminates the
   shrine without touching the HUD or requiring a new player animation.
4. **Confirm, frames 24–31:** Core is absent, the last fragments fade and the
   unique sound resolves. A future permanent level-complete flow may add a
   dedicated receive pose; the temporary replay proof does not need one.
5. **Replay:** only now invoke the existing in-memory level reset.

Keep the duration deterministic. Do not allow shooting, damage, a second
pickup, HUD-counter mutation or ordinary Shard sound during the sequence. A
later real level-complete state may replace only the final replay action.

## Unique Core sound

Add one original 8-bit signed mono Paula sample, tentatively
`stormstone-core.raw`, at the existing channel-1 playback rate (period 322).

Supplied listening and FS-UAE review selects and accepts the more celebratory
`02-storm-triumph.wav` direction over the restrained first runtime sound. Its
Paula-ready mono 8-bit candidate is generated as
`sfx/raw/stormstone-core-triumph.raw`; alpha.46 packages its byte-identical
generated runtime form as `stormstone-core.raw`.
Target 0.6–0.8 seconds and roughly 7–10 KiB, even-byte aligned:

- short low electrical intake;
- clear glass/metal crystalline strike;
- rising two-note cyan shimmer;
- compact tail with no long reverb.

Give it priority 11, above water splash priority 10. The clearing has no enemy
or hazard events, so this cleanly owns gameplay channel 1 during collection.
Keep Paula channels 2–3 reserved for future music. Add separate diagnostic
request/start counters and prove that reset does not truncate the intended
audible tail; if necessary, delay replay to the 32-frame sequence boundary.

## Rejected or deferred techniques

- **Fifth foreground plane:** incompatible with the current 4+3 dual-playfield
  contract and disproportionate for one screen.
- **Full-screen 4+4 solely for the house:** improves REAR8, not FRONT16, and
  adds DMA without solving foreground colour ownership.
- **Mid-line Copper colour split around the house:** fragile, Copper-heavy and
  camera-dependent.
- **Large animated Core Bob as first choice:** feasible but adds restore plus
  draw Blits to the already protected Bob chain. Hardware channels 2/3 are a
  cleaner isolated proof.
- **Foliage sprites plus Core sprites immediately:** consumes multiple pairs
  before the palette-mode solution is evaluated.
- **Immediate full level-complete/results system:** owned by later progression
  work; keep this proof's final action as delayed in-memory replay.

## Completed approval and test record

1. Completed: exact 320x208 composition at camera x=3072 with no old
   platform, 60px ground margins and Sparkpaw/Core centred.
2. Completed: house/tree palette investigation retained current FRONT16
   with warm copper/orange material accents instead of the green remap.
3. Completed: six-frame Core concept sheet and exact native-palette
   nearest-neighbour review established the accepted silhouette and pulse.
4. Completed: four samples auditioned; Storm Triumph selected and integrated.
5. Completed: composition/camera/FRONT16 art passed FS-UAE/68030 review.
6. Completed with architectural change: Core Bob idle passed without trails,
   sprite loss, palette aliasing or HUD damage.
7. Completed: deterministic 50-tick collection latch, radial release, unique
   sound, delayed replay and reset cleanup pass supplied review.
8. Completed: minimal-cadence FS-UAE/68020 records 3,223 one-field, 21
   two-field, zero three-plus intervals, 49.67 FPS and zero ownership
   violations across 3,244 intervals.

No A/B pair is required for the first composition review. Use A/B only for the
current-versus-clearing palette proof or when 68020 cadence needs a freshly
matched baseline.

## Acceptance criteria

- no old portal/platform is visible in the final composed viewport;
- at Core contact, Sparkpaw and Core occupy the centre region;
- both left and right ground margins are plainly visible;
- house/tree reads as authored Storm Ruins architecture with warm storm-scorched
  foliage, cyan conduits, bronze detail and pale lit stone rather than clipart;
- rear panorama remains coherent, unstretched and seam-free while moving;
- Core is unique, crisp, animated and removable;
- collection is visible for approximately 0.6 seconds before replay;
- unique sound plays once and is not replaced by the ordinary Shard sound;
- no renderer ownership violation, sprite corruption, HUD damage or material
  decline from the alpha.45 presentation baseline;
- 68020 cadence remains separately measured and evidence-gated.
