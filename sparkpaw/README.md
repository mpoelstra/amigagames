# Sparkpaw: The Stormstone Quest

Milestone 2A of an original Commodore Amiga 1200 AGA action platformer by
MrDig Productions.

Current release: `0.6.0-alpha.59`. Roadmap checkpoint: Phase 6C.5 centred
gameplay camera on the protected rolling renderer Stage 5L baseline. After all
loading and renderer preparation, a unique 64-colour AGA composition presents
the isolated crest-free Sparkpaw wordmark, Level-1 stone/machine borders,
centred menu/credits and lower-right Sparkpaw pose. `START GAME` is selected by
default, so joystick Fire or Space still fades directly into the already
prepared level. `OPTIONS` assigns the secondary button to `JUMP` or `FIRE` for
the current run. Supplied FS-UAE/68030 and
FS-UAE/68020 HD testing accepts presentation, both inputs and the immediate
gameplay transition. HD retains the complete Phase 6C.3 five-plate story intro;
the space-bound ADF deliberately omits only those cinematic plates and begins
at the existing title while retaining loading, charging and the ready screen.
Its bootable DOS1/FFS package statistics are recorded below; package/decode
evidence is not ADF gameplay acceptance.

Alpha.59 replaces the ordinary 105..202px horizontal gameplay-camera dead zone
with one centred anchor. Sparkpaw's 32px logical body now has its visual centre
at screen x=160 in both directions and while stationary. Supplied
FS-UAE/68030 HD A/B/C testing accepts this exact centred version as better with
no observed problems. A separate 16px directional-lookahead candidate is
rejected because its return to centre visibly shifts the image. The final Core
clearing keeps its accepted fixed maximum-camera composition. Renderer,
physics, level geometry, animation, assets and audio remain unchanged.
FS-UAE/68020 cadence, ADF, WHDLoad and real-hardware runtime acceptance remain
separate pending checks.
The bootable alpha.59 DOS1/FFS ADF uses 1,450 blocks (725 KiB) and leaves 310
free; this is package/decode evidence, not ADF runtime acceptance.

Alpha.58 removes the intermittent ready-screen fragments visible on supplied
real-A1200 footage. Menu changes patch only a hidden ready buffer. COP1LC is
armed during a safe mid-frame window without COPJMP1, so the Copper adopts the
complete inactive list at its natural vertical restart. The remaining idle
glitch was stale hardware-sprite DMA: full takeover enabled sprite DMA although
the title/loading/ready Copper lists own no sprite pointers. Sprite DMA now
remains off until the gameplay Copper is installed. Supplied FS-UAE/68030 HD
and real-A1200/68030 HD testing accepts idle START GAME/OPTIONS screens, rapid
menu switching and gameplay entry. ADF and WHDLoad runtime acceptance remain
pending. The extra hidden display bitmap costs 61,440 bytes Chip RAM; gameplay,
controls, assets, audio and Stage 5L/H7 are otherwise unchanged.
Subsequent supplied testing also reports alpha.58 working correctly on real
hardware and on an Analogue Pocket. Because the launch path was not specified
for that additional report, ADF and WHDLoad acceptance remain evidence-specific
rather than inferred from the platform result.
The alpha.58 bootable DOS1/FFS ADF uses 1,449 blocks (724 KiB) and leaves 311
free; this is package/decode evidence, not ADF runtime acceptance.

Alpha.49 additionally reserves pure black palette pen 0 in every fullscreen
intro and ready asset. This removes the one-pixel full-height `COLOR00` border
visible on the real A1200's Indivision output but hidden by FS-UAE and ordinary
CRT overscan. Supplied FS-UAE/68030 HD and real-A1200/Indivision HD testing
accept the correction. A host regression now enforces this asset contract.

Alpha.50 adds joystick-port-2 secondary-button jumping
as an alternative to the preserved joystick Up and keyboard W inputs. The
combined action retains the existing press-edge behavior; primary Fire and
Space still shoot. A supplied real-A1200 HD test accepts the secondary button.
No controller model or separate FS-UAE, ADF, WHDLoad, Pocket or CD32-pad result
was supplied. Jump physics, animation, audio and the renderer are unchanged.
The bootable DOS1/FFS ADF validates at 1,354 blocks (677 KiB), leaving 406
free; this proves package construction and retained-stream decode identity,
not ADF gameplay acceptance.

Alpha.54 replaces only the ready screen's central prompt with `START GAME` and
`OPTIONS`. Up/down selects; Fire/Space confirms. Inside Options, left/right
changes `SECOND BUTTON` between `JUMP` and `FIRE`, and Fire/Space returns to the
main menu. The setting defaults to the real-A1200-accepted alpha.50 jump
behavior on every launch. Menu variants share one exact 64-colour palette and
a Fast-RAM patch atlas. No extra displayable Chip bitmap, gameplay renderer
work, physics or audio change is introduced. Host tests and native 68020
compilation pass. The final central-only atlas preserves both lower corners,
omits credits from Options and keeps the JUMP/FIRE arrows symmetric. Supplied
FS-UAE/68030, FS-UAE/68020 and real-A1200/68030 HD testing accepts presentation,
direct start and both mappings. The 68020 production-style run contains no
cadence logger, so it makes no new numerical FPS claim. ADF and WHDLoad runtime
acceptance remain pending. The raw 50,124-byte menu atlas packs to 40,805 bytes
on ADF; the bootable DOS1/FFS image uses 1,444 blocks (722 KiB) and leaves 316
free.

Alpha.55 is a packaging-only release. The ordinary HD and WHDLoad LHA files now
contain genuine classic `-lh5-` compressed members instead of stored `-lh0-`
members. Executables, runtime assets, archive layouts, ADF game data, runtime
loader, renderer, gameplay and audio are unchanged from alpha.54; versioned
names and packaged ReadMe text advance to alpha.55. Package construction and
independent host extraction are verified, but no new FS-UAE, ADF, WHDLoad or
real-hardware runtime acceptance is inferred.

Subsequent supplied real-A1200 testing rejects the alpha.55 WHDLoad intro path:
the first plate displays both passages, then Sparkpaw returns to Workbench at
the plate-2 load boundary. The ordinary alpha.55 HD build completes the intro,
so this is WHDLoad-specific. The visibly shortened 31-character parent drawer
name alone cannot explain a launch that reaches plate 1. A focused
`Sparkpaw-WHDIntroDiag` package uses a 21-character drawer and records each
intro load plus asset/IoErr/Chip/Fast diagnostics; its hardware result remains
pending. The supplied log now confirms plate 1 loads and plate 2 fails at
`Open` with IoErr 205 (`ERROR_OBJECT_NOT_FOUND`) despite about 2.07 MB Chip and
7.70 MB Fast remaining. The short diagnostic parent rules out the versioned
drawer name as the necessary cause; plate 2's own 31-character runtime name is
the failing WHDLoad boundary. The corrected-package result is recorded below.

The focused `Sparkpaw-WHDShortNames.zip` correction replaces the three
overlength components with `intro2.spbm`, `intro3.spbm` and `readymenu.spbm`.
Supplied real-A1200/68030 testing accepts it through all five intro plates,
title, loading, charging and the ready menu. Alpha.56 makes these the canonical
names for HD and WHDLoad and uses the same short sources for ADF packing.
Release tooling rejects path components longer than 30 characters; the WHDLoad
archive extracts to `Sparkpaw-0.6.0-a56-WHDLoad`. Asset bytes, renderer,
gameplay and memory configuration remain unchanged.

Alpha.52 adds a shared Sparkpaw cover icon to the ordinary HD and WHDLoad
drawers. NewIcons-capable systems use the accepted 86x93, 34-colour embedded
image. Systems without NewIcons use an 86x93, eight-colour classic fallback
limited to the standard OS 2.x/3.x Workbench pens. Supplied real-A1200 evidence
accepts the NewIcons layer. The decoded eight-colour preview is accepted for
now; exact FS-UAE presentation remains pending retest. The rejected 16-colour
RomIcon fallback depended on a FullPalette pen layout absent from the supplied
FS-UAE Workbench. HD and WHDLoad share pixels but retain their distinct launch
metadata. ADF contents, gameplay, renderer, memory and audio are unchanged.
The bootable alpha.52 DOS1/FFS ADF validates at 1,356 blocks (678 KiB), leaving
404 free. Its staging explicitly removes the HD `.info`; the one-block increase
is packaged ReadMe text only and establishes no new ADF gameplay acceptance.

Alpha.51 promotes the WHDLoad-only F10 exit correction accepted in supplied
real-A1200/68030 testing. Once Sparkpaw owns the custom chips, its direct CIA
keyboard polling now catches raw F10, restores DMA/Copper/Exec ownership and
returns through the BootDOS slave to Workbench. The normal HD and ADF builds do
not compile this path. WHDLoad startup, loading and clean F10 return are accepted
on the supplied real A1200; FS-UAE, ADF and broader WHDLoad gameplay acceptance
are not inferred.
The bootable alpha.51 DOS1/FFS ADF validates at 1,355 blocks (678 KiB), leaving
405 free; this is package/decode evidence, not new ADF gameplay acceptance.

The preceding Phase 6C.2 Core-clearing checkpoint remains accepted.
Supplied FS-UAE/68030 HD testing accepts the centred waystation, animated Core,
radial pickup, Storm Triumph sound and delayed replay. The matching
FS-UAE/68020 HD minimal-cadence gate records 49.67 FPS over 3,244 intervals,
with no three-field misses or ownership violations. The preceding alpha.45
testing also accepts presentation and cadence
on the real A1200/68030 at about 34.5 MHz from both HD and physical ADF, and on
the Analogue Pocket 68020 ADF path. The low-overhead FS-UAE/68020 diagnostic
records 48.58 effective FPS with no three-field misses or ownership violations.
This near-50-Hz result is now a protected regression baseline: future features
must not casually reintroduce per-frame copies, broad scans or serialized Chip
work. Accepted Phase 6B.5 remains the player-gameplay baseline.
Supplied alpha.39 testing accepts presentation/cadence at 68030 in FS-UAE and
the ADF on a real A1200/68030. Both packages work in FS-UAE, but real-A1200 HD
output corrupts immediately after CHARGING. Alpha.40 changes only raw HD asset
input: every DOS read uses a 512-byte staging buffer followed by a CPU copy to
the final allocation, matching the working ADF reader's transfer granularity
and avoiding dependence on partition/device MaxTransfer and Mask behavior.
Supplied real-A1200/68030 testing now proves alpha.40 HD loads and plays with
about 1.92 MB Chip free, both after Boot With No Startup-Sequence and from a
two-colour Workbench. It reproduces the post-CHARGING corruption from a normal
Workbench with 1,430,032 Chip bytes free. The open HD defect is therefore the
resident/peak Chip-RAM budget, not unverified disk transfer. Alpha.41 removes
only unreachable world storage, keeps one floppy status bitmap, moves complete
player and Strider masters bit-identically to Fast RAM, and retains small Chip
DMA stages. The calculated permanent saving is about 642 KiB plus about 54 KiB
at the loading peak. Supplied real-A1200/68030 testing accepts alpha.41 HD from
a normal Workbench with about 1.4 MB Chip RAM free. Alpha.41 ADF regression is
pending. Alpha.40 and alpha.41 real-hardware evidence both reject the first
two-Strider scene due to intermittent enemy glitches and apparent cadence loss;
occasional beetle glitches are also reported. Stock-68020 FS-UAE performance
remains a separate rejected/open optimization track after that presentation
fault is isolated.
Alpha.42 promotes the Stage 4G no-copy rolling renderer: two hidden FRONT16
targets, two atomically published Copper lists, target-local Bob histories and
seam-safe 512x3 physical rings. Supplied FS-UAE/HD testing reports no enemy or
projectile trails and no HUD-boundary flicker. Its log measures 49.23 effective
FPS over 1,983 intervals. REAR8 parallax, assets, palettes, sprites, animation,
HUD and gameplay remain unchanged. Only entering columns and changed dynamic
regions synchronize on the inactive target; the displayed target is never
modified. Subsequent supplied recordings retain the clean sampled presentation
but reject cadence on FS-UAE/68020 and on a real A1200/68030 at about 34.5 MHz.
The 49.23-FPS result therefore characterizes only the faster FS-UAE/68030
configuration. ADF and Analogue Pocket acceptance remain pending; real-A1200
performance is explicitly rejected.
The bootable alpha.42 ADF package uses 1,186 blocks (593 KiB) and leaves 574
free; this is packaging validation, not ADF gameplay acceptance.
Alpha.43 keeps Stage 5G's coherent 32-bit playfield geometry but repacks the
unchanged 48x48, 15-colour player into one transparent-padded 64-pixel attached
AGA sprite pair on channels 0/1. Supplied Stage 5L FS-UAE/68030 HD testing
reports no corruption, glitches or flicker; its log measures 2,163/2,163
one-field intervals (50.00 FPS) and zero ownership violations. Real-A1200/68030
HD and physical-ADF runs, and Analogue Pocket ADF, retain broad visual stability
but reject performance. Real HD also repeats or misses some sound events under
load. A narrow intermittent disturbance remains at the ground/HUD seam.
The bootable alpha.43 ADF package validates at 1,190 blocks (595 KiB) with 570
blocks free; this validates packaging. The supplied physical ADF launches and
plays on the real A1200, but its cadence is rejected.
Stage 2 performance work after alpha.43 now uses display-only rolling targets:
Bob histories retain canonical source coordinates and restore directly from
the clean world, eliminating both target-local clean bitmaps. Supplied
FS-UAE/68030 and FS-UAE/68020 HD tests report clean presentation. The measured
68020 rolling/dynamic target scopes improve by about 39--41%, the Bob-pass
average by 26.2%, and prepared-peak free Chip improves by 319,488 bytes. The
production-default 68030 diagnostic records 49.95 FPS and zero ownership
violations. This architecture is now included in alpha.44 and alpha.45;
ADF/Pocket gameplay and real-A1200 acceptance for those releases remain
unsupplied, and overall 68020 cadence remains insufficient.
Subsequent accepted Stage2 target-local diamond composition removes diamonds
from broad canonical dynamic synchronization. Supplied FS-UAE/68020 HD A/B
testing reports normal presentation and raises effective cadence from 35.31 to
42.15 FPS (+19.4%), while `ring_dynamic` average falls from 3,940 to 101 CIA
ticks. Alpha.44 packages this work together with the FS-UAE/68030-HD-accepted
H7 seam correction. Alpha.44 ADF, Pocket and real-A1200 acceptance remain
pending.
Its bootable DOS1/FFS ADF package uses 1,195 blocks (597 KiB) and leaves 565
free. This is package validation only.
Alpha.45 adds three supplied-FS-UAE-tested Stage 2 defaults: direct Strider
traversal lookup, invariant Bob-register setup and a specialized aligned 16px
entering-column copy. The latest matched 68020 A/B raises cadence from 44.47
to 45.55 FPS and lowers ring-roll p95 from 9,878 to 2,782 CIA ticks; complete
Bob-pass p95 falls from 16,308 to 9,142. Presentation remains normal in the
supplied 68030 and 68020 HD tests. Subsequent supplied testing accepts alpha.45
on the real A1200/68030 at about 34.5 MHz from HD and physical ADF, including
good cadence, and accepts the Analogue Pocket 68020 ADF path.
The bootable alpha.45 DOS1/FFS ADF validates at 1,197 blocks (598 KiB), leaving
563 free. This is package construction evidence, not ADF gameplay acceptance.
The Stage 2 performance checkpoint is complete. The residual profiler split
CPU work, custom-register setup and `WaitBlit` cost and triggered a complete
C/assembly/Fast-versus-Chip recheck. It found no forgotten continuous large
copy or post-CHARGING asset work worth another speculative prototype.
The resulting minimal-cadence check compiles out nested CIA and Bob-family
measurements but retains boundary cadence. Supplied FS-UAE HD testing reports
normal presentation: 68030 remains at 50.00 FPS, while 68020 reaches 48.58 FPS
(1,104 one-field and 33 two-field intervals, no three-field misses). This shows
that most of the apparent remaining diagnostic deficit was profiler observer
cost. It does not establish alpha.45 ADF, Pocket or real-A1200 cadence.
Those hardware acceptances were subsequently supplied explicitly as described
above. Future optimization remains possible, but only behind a new measured
need: coarse hardware-facing scopes, safe reduction of actual Bob jobs/waits,
or a proven CPU-read-heavy Fast-RAM mirror. Rejected diamond persistence,
pointer precompute and inline-wait experiments must not be repeated unchanged.
Phase 6B.2 through 6B.4 water mechanics, presentation and impact feedback are
also accepted. Phase 6B.6 now contains the integrated extended REAR8 parallax
and foreground-material v1 candidates alongside the measured palette previews;
Supplied FS-UAE review rejects these latest art changes as the final quality
target while preserving the corrected rear composition. Supplied FS-UAE review
accepts isolated rb19c as a worthwhile REAR16 visual proof. Supplied rb20 logs
show its four-Strider synthetic load exceeds a frame even at 4+3, so it is not
a valid production decision. Supplied rb21 logs likewise exceed a frame because
they combine independent maxima. Supplied rd01 production evidence identifies
seven diamonds plus two enemies and water as its 172-line peak. Alpha.15 keeps
diamond hover but persists them in clean/display and staggers their updates;
supplied rd02 timing shows a later six-projectile/four-enemy frame is now the
227-line peak. Alpha.15 historically put logging and left-mouse exit in the HD
main executable; the next production checkpoint removes both from official HD
and ADF builds. Production remains 4+3. Alpha.17 adds a
production Copper palette morph across the fixed-height sky, mountain and
forest regions: the rear bitmap remains three planes, but twelve horizontal-
blank palette steps provide three purpose-built eight-colour material ranges
without an eighth bitplane or displayed-Chip CPU compositing. FS-UAE and real-
hardware review of this new morph remain pending. Supplied alpha.17 FS-UAE
evidence accepts the visual improvement and exposes one partial-diamond residue
from the sole non-word-aligned collectible. Alpha.18 gives only that slot a
two-word restore cache, adding 400 Chip bytes without slowing the other 31.
Alpha.19 completely replaces the rear art master: one coherent full-height
panorama now carries turbulent cloud detail through the upper edge, preserves
the opening vortex/lightning tower and continues through unique mountains,
storm ruins, waterfalls and cold-green forest without the former calm right-
hand filler. Runtime stays 1024x208 source, 2048x256x3 resident rear and the
same twelve-step Copper palette morph.
The opening tower/vortex is a narrative landmark shared with the title-screen
world: it foreshadows the distant end-level destination and is not disposable
background decoration.
Supplied alpha.19 FS-UAE review accepts the new panorama but finds that tower
too rock-like relative to the title. Alpha.20 uses an entirely new v4 master,
generated from the title architecture rather than repainting v3. Its slender
central spire, side turrets, castle base and cyan Stormstone heart survive the
exact three-plane reduction; one mountain-band colour role is reassigned to
cyan without adding Copper steps, planes or memory.
Supplied alpha.20 FS-UAE evidence accepts the title-identity background as the
new rear baseline. Alpha.21 begins the separate foreground replacement using a
newly generated orthographic ruin kit. Whole authored platform and column
families replace repeated per-tile boxes; broad platforms gain open decorative
braces, cyan conduits and gothic cavities while every collision rectangle,
walkable top, water opening and actor baseline remains unchanged.
Supplied alpha.21 FS-UAE review confirms the material improvement but rejects
the foreground as the final baseline: centred supports repeat too often, the
ground reads as one uniform tech strip, and the legacy purple lozenges and
hanging orange lamps have no gameplay or architectural purpose. Alpha.22
removes those decorations and consumes a completely new orthographic kit with
seven slab rhythms, four asymmetric support families, four grounded piers and
a long shallow ruin facade. This remains a strict art pass: collision bytes,
enemy surfaces, traversal links, actor baselines, water and renderer contracts
do not change. A dry non-water gap and taller L/portal compositions are a
separate future level-layout checkpoint because they affect pacing and collision.

Phase 6B.2 adds one mechanical water proof at x=1584..1663. The greybox floor
opens for 80 pixels and only those columns allow falling below the normal game
boundary. Sinking to y=224 removes one life and performs the existing safe
in-memory level restart. Phase 6B.3 now adds the approved visual basis: an
eight-pixel steel/stone ground cap immediately above the HUD, interrupted by
blue storm water with a cyan/white surface. Phase 6B.3B animates sixteen small
80x11 frames every two game ticks. A continuous shallow wave spans the full
opening while six differently phased bubble tracks rise and rest independently.
Synchronized Blitter copies update both clean and displayed foreground buffers.
Collision,
restart behaviour, renderer timing, splash and audio were unchanged in the
visual-only pass. Phase 6B.3A now moves the actual continuous floor and its
floor-owned enemy surfaces from y=208 to y=200, matching the visible cap instead
of letting actors sink into it. Sparkpaw's reset height follows the same
eight-pixel shift. Raised platforms and authored Strider links keep their
existing geometry.
MrDig accepted the fall/restart behaviour in supplied testing. A separate HUD
asset correction retains its top separator at two scanlines and now fills
those rows with an opaque dark HUD pen instead of transparent pen 0. The
line-252 display switch and 48px total HUD height remain unchanged. MrDig
initially verified the correction in FS-UAE and on a real Amiga. A later
real-hardware observation still reports intermittent glitchy behaviour there;
this is an explicit follow-up todo, not a reopened renderer change in 6B.3.
The approved visual basis is preserved at
`assets/concept/sparkpaw-water-hazard-concept-v4.png`. Extra small diamonds in
that generated concept are an ImageGen artefact and are deliberately excluded:
runtime retains the accepted diamond size, placement and Bob contract.
The exact sixteen-frame native palette treatment is previewed in
`assets/levels/storm-water-animation-aga-preview.png`. Its 7,040-byte Chip cache
updates between existing Bob restores and draws during the line-253 pass; the
relative projectile, enemy and collectible ordering remains unchanged.

Phase 6B.4 adds water-impact feedback without changing the accepted hazard
geometry. When Sparkpaw's centre enters the opening and his feet pass y=204,
the six hardware sprites are hidden and a dedicated four-frame cyan/white
splash Bob plays for sixteen game ticks. The splash is restored and drawn in
the synchronized line-253 pass and does not consume an enemy-pool slot. An
original 11.025 kHz mono water impact plays once on prioritized Paula gameplay
channel 1. Life loss occurs once at impact; the resident restart follows after
the short hold.

Phase 6B.5 appends four ledge-teeter poses as player slots 58..61 without
renumbering or replacing the accepted 0..57 contract. After ten stationary
grounded ticks, the family triggers only with 4..10 supported pixels across the
24px collision sole. Fewer than four supported pixels now releases `grounded`,
and is ignored by downward landing resolution, preventing a repeated
one-pixel collision from suspending Sparkpaw in the air. Sparkpaw faces the missing side and
cycles notice, outward lean, counter-swing and recovery, but only when the next
three pixels beside his full standing collision height are clear. Adjacent
platform walls therefore suppress the animation. When 1..3 residual edge
pixels are released, Sparkpaw is moved outward by exactly that overlap before
descending, preventing a hidden wall penetration from blocking his next jump.
Movement,
jumping, crouching, shooting, turning, hurt or loss of grounding interrupts it
through the existing higher-priority states. Collision maps and boxes remain
unchanged; only the minimum extreme-edge support needed for `grounded` is new.
MrDig accepted the final trigger timing, adjacent-wall suppression, edge fall
and immediate post-landing jump in supplied FS-UAE testing. No real-hardware
result is claimed.

Phase 6B.6 is split into concept approval, exact indexed previews, an isolated
renderer comparison and only then production integration. Its first review
source is `assets/concept/sparkpaw-visual-slice-concept-v2.png`: an environment-
only water section using the saturated concept identity, low ground strip,
compact bank edges, raised ruin platform and layered Storm Ruins parallax. It
contains no player, enemy, diamond or HUD and is not runtime art. Production
4+3 remains authoritative while feasible alternatives are evaluated.
MrDig accepted v2's lowest-ground
direction. A hardware audit corrected the proposed comparison: AGA supports at
most four planes per dual playfield, so eight fetchplanes are 4+4, never 5+3.
The generated 32-colour PF1 image is retained only as an art upper bound, not a
renderer candidate. A matched sixteen-frame sheet proves that the concept's
blue body, flowing full-width wavelets and asynchronous bubbles fit the current
4+3 FRONT16 bank using navy, blue, cyan and pale foam. Runtime water remains
unchanged. The approved parallax source was reduced with a fixed REAR8 palette
and integrated as the production rear layer. Supplied FS-UAE scrolling then
exposed the 640px source repeat as an abrupt landmark boundary. Its replacement
is a 1024x208 unique native span: it preserves the opening tower, vortex and
lightning, progresses through distinct mountain/ruin scenery and places the
first repeat beyond pixel 1007, the maximum rear sample required by the planned
3072px world at quarter speed. It adds storm clouds, mountain
depth, dense forest silhouettes, ruined towers, waterfalls and a cyan-lit
central ruin without an extra bitplane. Alpha.17 retains the same three-plane
bitmap and scroll factor but quantizes its fixed vertical regions against
separate sky, mountain and forest palettes. Twelve Copper changes are staged
in the preceding horizontal blanks to morph between those palettes, removing
a hard band while exposing roughly 24 authored rear colours across the frame.
The Copper allocation grows by 512 bytes of Chip RAM; bitmap size, Blitter work
and bitplane-fetch DMA do not change. An exact REAR16
comparison is retained for a later isolated 4+4 benchmark; it cannot add
foreground or water colours.
Additional bitmap memory is allowed when it produces a clear visual gain and
still fits the stock A1200, 2 MB Chip plus 8 MB Fast target. Such work remains a
measured isolated renderer step; the current production candidate needs no
extra plane.
The current separate foreground pass keeps every existing collision
rectangle and actor baseline but replaces the flat greybox slab treatment with
layered pale ruin lips, irregular steel panels, dark recesses, violet machinery,
cyan conduits and deterministic cracks. It uses the existing FRONT16 bank and
adds no bitmap, Blitter or Copper cost. Alpha.22 replaces the rejected v1
support rhythm and legacy decoration with richer authored variations. Larger
L/portal silhouettes and a dry gap remain a separate geometry/pacing step.

### Phase 6C.1 — twelve-screen route integration

Production is now 3072px/twelve screens. The four new screens are authored as
distinct route beats rather than a repeated tail: a broken low bridge, high
side-pier approach, second animated water opening, broad patrol court and a
final L/portal ruin around a second dry chasm. The complete level has two 80px
water hazards at x=1584..1663 and x=2432..2511 plus dry gaps at x=2112..2175
and x=2784..2863. Dry falls use the existing safe life/restart path without a
water splash; water retains the accepted splash/audio hold.

Seven required persistent Striders now live in world space. Four new route
families cross both water openings and both dry gaps in both directions. A
parked Strider still advances exactly once per frame without a Bob slot, so it
may naturally enter the visible camera from left or right at whatever point its
route has reached. Ten required plus zero, one or four optional beetle
candidates and 48 diamonds fill the longer route. The generic active enemy pool
remains four slots; more authored encounters do not create more simultaneous
enemy Bobs. The second water instance shares the same deterministic 7,040-byte
frame bank and adds only synchronized destination Blits, not another cache.
Renderer staging, animation IDs and palette ownership remain unchanged.

Alpha.24 corrects the final ascent rejected in supplied alpha.23 FS-UAE review.
The right bank now provides a low broken landing followed by a reachable 48px
rise to the portal; Sparkpaw's global jump remains unchanged. Dry gaps stay
distinct from water and now show fractured bank faces, a dark recessed lower
edge and severed cyan conduits. Platform undersides alternate four authored
depth rhythms while their collision-readable top slabs remain unchanged.

Supplied alpha.24 FS-UAE evidence rejects that first ascent correction. The low
landing is reachable, but its edge directly touches the 48px-higher portal wall;
horizontal collision cancels the jump before Sparkpaw's feet clear the lip.
Alpha.25 shortens the low landing to x=2864..2911, leaves x=2912..2927 open,
and places the complete portal deck at y=144, a 32px rise. Host simulation uses
the generated collision bytes and the real moveX-before-moveY order; starts at
four positions/speeds all land. Runtime FS-UAE confirmation remains pending.
The final diamond arc is re-aligned with full 21px-plus-hover clearance above
the y=144 deck, and the last beetle patrol now references that same surface. A
complete 48-item bounding-box audit corrects five older floor/platform overlaps;
every diamond now has collision clearance through its complete hover range.

Supplied alpha.25 FS-UAE evidence rejects the 32px rise from the natural extreme
right/max-speed launch. Alpha.26 places the complete portal deck at y=160, only
16px above the low landing after the existing 16px opening. The generated-map
model now exhausts all 21 valid launch origins and 83 speed samples from zero
through exact maximum: all 1,743 cases land. FS-UAE confirmation remains open.

Supplied alpha.26 FS-UAE evidence rejects that correction in practice too.
Alpha.27 removes the vertical step instead of adding a new wall-jump mechanic:
the low landing and complete portal deck now share y=176, with only the existing
16px horizontal opening between them. Jump physics and all renderer contracts
remain unchanged; supplied FS-UAE completion confirmation is pending.

Supplied alpha.27 FS-UAE evidence exposes that the actual blocked platform is
earlier at x=2320..2415: it stood at y=112, 64px above the preceding y=176
platform after a 32px opening. Alpha.28 lowers that complete platform and its
attached pier to y=160, a reachable 16px rise, without changing global jump
physics. The following water gap remains 80px and both Strider crossings are
retimed to the corrected banks. Supplied FS-UAE route confirmation is pending.
The unrelated later portal is restored from alpha.27's flat y=176 experiment to
the raised y=160 alpha.26 layout; independently audited diamond clearance stays.

Supplied alpha.28 FS-UAE/HD testing accepts the correction: Sparkpaw reaches the
lowered platform, continues through the remaining route and completes the full
level. This accepts Phase 6C.1 route traversal. ADF gameplay parity, real-A1200
behavior and a supplied full-run timing/memory log remain separate checks.

The resident width adds 131,072 bytes to each of `frontClean` and
`frontDisplay`, 98,304 rear bytes and 896 collision bytes versus 2048px:
360,448 extra bitmap bytes, before small extra collectible restore state.
Host builds/package validation pass and supplied FS-UAE/HD testing accepts full
route traversal. Supplied full-run memory remains viable, but production timing
is rejected/pending optimization; ADF parity and real hardware remain pending.

### Alpha.46 Phase 6C.2 — first Core clearing

Alpha.46 extends Level 1 from 3072px to 3392px with
one safe, enemy-free destination screen after the final portal. A large
Stormkeeper's Waystation—a caretaker cottage/shrine with a storm-bent tree—
frames the first Stormstone Core. Touching that Core currently invokes the
existing in-memory replay path; the later real level-complete/progression flow
remains separate work.

The waystation remains static FRONT16 art. Alpha.46 renders
the Core through one 64x48 FRONT16 Bob, keeps the accepted centred final camera
and uses the selected Storm Triumph reward sound before delayed replay. After
supplied 60 fps evidence showed thin gauntlet-directed lines disappearing
behind Sparkpaw, pickup now contracts into a radial release with a two-field
foreground illumination. It adds no player frames and does not alter HUD
palette ownership, world geometry or Core cache dimensions. Host tests and the
native 68020 build pass. The supplied 60 fps recording and explicit user
verdict accept the FS-UAE/68030 visual/function gate; a matching minimal-
cadence 68020 HD run also passes: 49.67 effective FPS across 3,244 intervals,
21 two-field, zero three-plus and zero ownership violations. This clears the
48.58-FPS regression baseline without establishing a new optimization result.
ADF and real hardware remain pending. Detailed intent and measurements live in
`docs/concepts/story-intro/LEVEL1_CORE_CLEARING_PLAN.md` and
`docs/concepts/story-intro/LEVEL1_CORE_CLEARING_POLISH_PLAN.md`.

Alpha.47 packages its five intro plates through the existing bounded streaming
SPR1 decoder on ADF while HD retains ordinary SPBM files. This preserves exact
pixels without requiring all five plates in memory; only one is resident at a
time. The bootable DOS1/FFS ADF validates at 1,707 blocks (853 KiB), leaving
53 blocks free. This is package/decode evidence only; ADF gameplay and real-
hardware intro acceptance remain pending supplied evidence.

Alpha.48 keeps that complete cinematic in HD/LHA/ZIP but deliberately omits it
from the space-bound ADF. The floppy still contains the normal title, LOADING,
CHARGING, the same 64-colour ready screen and identical gameplay. Its bootable
DOS1/FFS image validates at 1,353 blocks (676 KiB), leaving 407 free, and all
retained packed status/world streams decode identically to their sources. This
does not establish ADF, Analogue Pocket or real-A1200 gameplay acceptance.

Current follow-up keeps three contracts separate. Supplied alpha.28 FS-UAE
evidence rejects the world diamond's transparent dark facets and ragged lower
tip: replace it with one clean native 16x21 opaque-outline diamond without
changing Bob dimensions, cache or restore ordering. Separate evidence shows a
standing plasma sample geometrically crossing an elevated beetle but being
rejected by the legacy `lowShot` gate. Alpha.29 removes only that redundant
eligibility predicate: the existing beetle body rectangle remains authoritative,
so ordinary floor beetles remain crouch targets through actual Y overlap.
Host tests cover elevated standing hit, floor standing miss and floor crouch
hit. MrDig's supplied FS-UAE/HD retest accepts those cases and clean left-mouse
exit.

A separate alpha.28 recording exposes transient orange/dark pixels over the
first two simultaneous Striders. Supplied FS-UAE/HD evidence rejects alpha.30:
double-buffering the player sprite streams does not remove the flicker, and an
isolated Strider also fails while the other is far away. The accompanying log
records 3,525 wraps in 3,844 passes and a 237-line peak at camera 810. That peak
still performs both resident water updates although their x=1584 and x=2432
strips are off-screen. Alpha.31 removes the disproved sprite-bank experiment
and updates only water strips inside the camera plus 16-pixel margin; a strip
immediately receives the current frame when it re-enters. This preserves water
art/cache, FRONT16, Bob geometry, mask/restore ownership and draw order. Host
tests pass; focused FS-UAE/HD review is pending. The diamond remains unchanged.
Native/package validation passes; the alpha.31 ADF uses 1,551 blocks (775 KiB)
and leaves 209 free. Supplied FS-UAE/HD review accepts water-strip behavior but
still finds less-frequent upper-Strider corruption. Its zero-water peak remains
219 lines and 6,599 of 6,796 passes wrap. Alpha.32 stably orders enemy restore
and draw work from upper to lower world Y, so the upper Strider completes before
later lower enemies. Equal-Y enemies retain slot order and the collectible /
enemy / projectile family priority is unchanged. Art, geometry, masks, water,
collectible hover and diamond remain unchanged. Focused FS-UAE review is pending.
Native/package validation passes; the alpha.32 ADF uses 1,553 blocks (776 KiB)
and leaves 207 free. This is not ADF gameplay acceptance.
Supplied FS-UAE/HD review accepts separated Strider stability in alpha.32 but
retains small head/crest flicker only while two Striders overlap. Its peak is
214 lines with zero water updates. Alpha.33 restores the union of intersecting
Strider rectangles once instead of restoring the shared area twice, then uses
the unchanged stable upper-first draws. Non-overlapping restores, art, masks,
geometry, family priority, water, collectible hover and diamond are unchanged.
Focused FS-UAE/HD review remains pending.
Native/package validation passes; the alpha.33 ADF uses 1,555 blocks (777 KiB)
and leaves 205 free. This is not ADF gameplay acceptance.
MrDig's supplied FS-UAE/HD review accepts alpha.33 for separated and overlapping
Strider stability. A separate alpha.33 recording shows a flush crouch shot
spawning through a narrow pillar and hitting the beetle behind, while the same
shot stops when fired from farther left. Alpha.34 starts collision at Sparkpaw's
physical front edge and checks every crossed X pixel through the projectile's
new leading edge, with solid geometry before enemy damage at each position.
Visual muzzle origin, projectile speed/height, beetle hitbox and renderer are
unchanged. MrDig's supplied FS-UAE/HD review accepts this correction.
Native/package validation passes; the alpha.34 ADF uses 1,557 blocks (778 KiB)
and leaves 203 free. This is not ADF gameplay acceptance.

A newly supplied full-level FS-UAE/HD run at 68020 speed rejects general
performance acceptance: 2,293 of 3,015 measured Bob passes wrap, with a
311-line peak against the 59-line post-HUD window. Alpha.35 begins measured
optimization without changing dual playfield 4+3, Copper staging, line-252 HUD,
line-253 Bob start, packed caches or restore/draw priority. Projectiles outside
the camera plus 16-pixel margin are no longer drawn into the resident world;
their last visible Bob is still restored normally. The diagnostic now records
separate accumulated and peak raster-line costs for projectile, enemy,
collectible, water and splash work, and its stale alpha23 label is corrected.
The HD executable now reads the same four packed large assets as the working
ADF path, reducing disk I/O and removing that raw-versus-packed transition
difference. Its CRC check uses two nibble-table steps per unpacked byte instead
of eight polynomial bit steps; a standard CRC32 vector guards equivalence.
Supplied real-Amiga evidence accepts the packaged floppy launch but
rejects alpha.34 HD: immediately after CHARGING its display becomes corrupted.
Alpha.35 HD and stock-68020 timing therefore require focused user retesting;
neither is claimed accepted here. Diamond art remains unchanged.
Host tests, native build and package validation pass. The alpha.35 ADF uses
1,560 blocks (780 KiB) and leaves 200 blocks free; this is package evidence,
not ADF gameplay or performance acceptance.

Supplied alpha.35 FS-UAE/HD evidence rejects using the packed ADF asset route
for the normal executable: it remains on LOADING for the full recording, never
reaches CHARGING, and shows intermittent display marks. Alpha.36 restores the
previous raw-SPBM HD loader while retaining the independent off-screen
projectile culling, family profiler and corrected diagnostic label. Packed SPR1
and its faster verified CRC remain ADF-only. This is a focused rollback of the
rejected path, not FS-UAE acceptance or a fix claim for the separate real-Amiga
alpha.34 post-CHARGING corruption.
All seven host regressions, native build and release validation pass. The
alpha.36 ADF uses 1,561 blocks (780 KiB) and leaves 199 free; this remains host
package evidence only.

Supplied alpha.36 FS-UAE/HD evidence accepts that raw loading again reaches
gameplay, but rejects stock-68020 performance and enemy presentation: 3,709 of
4,139 passes wrap and a two-Strider frame takes 310 lines. The family profile
identifies enemies as the dominant cost (354,807 accumulated lines, peak 263),
followed by collectibles. Alpha.37 converts each enemy animation frame into a
tightly normalized Bob rectangle containing exactly its non-transparent pixels
and restores those recorded bounds. Intersecting Striders merge their actual
unequal rectangles. This removes transparent row/column blits without changing
art pixels, collision cells, animation cadence, draw priority, camera culling
or the display/Copper structure. FS-UAE 68020 review remains pending.
All seven host regressions, native build and release validation pass. The
alpha.37 ADF uses 1,563 blocks (781 KiB), leaving 197 free; this is host package
evidence only. Supplied FS-UAE/68020 review rejects alpha.37: sprites are
damaged and timing remains unacceptable. Alpha.38 removes that tight-bound
experiment and replaces the single displayed full-world foreground with two
hidden 512x256 FRONT16 viewport buffers. A completed camera-local clean-world
copy plus dynamic Bob composition is published atomically; no render operation
writes the displayed foreground and enemy/projectile/splash restore passes are
gone. The separate line-252 HUD, 4+3 dual playfield, rear parallax, player
hardware sprites, packed caches, camera culling and family priority remain.
FS-UAE/68020, ADF gameplay and real-A1200 verification are pending.

The supplied long alpha.28 `renderdiag.log` covers 27,819 frames and records
14,172 line-253 passes crossing PAL wrap. Its peak runs 251 raster lines (margin
-192) with five projectiles, two beetles, two Striders, one collectible and two
water updates. Prepared memory remains viable at 170,752 Chip bytes free
(169,456 largest) and 6,362,480 Fast bytes free, but full-run production timing
is rejected/pending. After the two small isolated corrections, profile work by
operation family and optimize the production 4+3 path without changing the
accepted display/Copper contracts. A possible fast two-shot Strider burst and
revised speed variation remain later gameplay/AI tuning.

This is a deliberately small but real engine test. It validates the risky
parts before broader enemy variety, music and level progression are added: a
native AGA dual-playfield display, two independently
hardware-scrolled five-screen world layers, a fluid 15-colour hardware-sprite
player, joystick input, solid platform collision, plasma projectiles and a
bounded clockwork-beetle vertical slice.

## Target

- Commodore Amiga 1200 or compatible AGA Amiga
- Motorola 68020
- 2 MB Chip RAM plus 8 MB Fast RAM minimum
- PAL 320x256 at a fixed 50 Hz
- Two stable 8-colour AGA playfields with true quarter-speed rear parallax
- 48x48 Sparkpaw poses made from three attached sprite pairs, with 24-bit AGA colour
- C with VBCC; small reproducible Python asset and release tools

The earlier 2 MB Chip/no-Fast Phase 6A result is retained only as a historical
stress measurement. Production graphics and performance decisions target the
stock 68020 configuration with the full 2 MB Chip plus 8 MB Fast minimum.

## Controls

- Ready menu: up/down selects, Fire/Space confirms; Options left/right assigns
  the secondary button to Jump or Fire for the current run
- Joystick port 2: left/right to run, up to jump and primary fire to shoot;
  the secondary button follows the selected option and defaults to jump
- Hold down to crouch; down plus left/right performs a slower crouch-walk
- Press fire while crouching or crouch-walking to shoot from a dedicated low pose
- Keyboard: `A`/`D` move, `W` jumps, `S` crouches and space shoots
- Production alpha.42 HD and ADF: reset the Amiga or emulator to leave
- Explicit debug build only: diagnostics buffer in Fast RAM; a deliberate
  flush writes/closes the log and then waits for reset without opening Workbench

Each separate fire press launches a fast blue/cyan plasma pulse from
Sparkpaw's right-hand gauntlet. Up to six pulses can remain in flight, so the
weapon responds to rapid tapping. Four guaranteed and up to two optional low
clockwork beetles patrol safe authored zones; their exact X positions and
48/96/192 movement speeds vary on each complete test replay. Their independent
walk cadence follows the selected speed. Standing shots naturally pass over
floor beetles, but any shot whose sample actually overlaps an elevated beetle
can hit it. Crouch and fire twice to destroy a floor beetle through a hit reaction and
four-stage destruction sequence. Contact with an active beetle now removes one
of six internal half-heart health units, applies brief knockback/input lock and
grants one second of invulnerability. Dedicated standing and crawl-height
hurt art is present; the eventual game-over presentation remains a focused
later step. A full-width fixed HUD band across the bottom now shows the
existing six health units as three full, half or empty hearts and reserves
separate framed panels for the active life counter and diamonds. Original
player-hurt, enemy-hit, enemy-death and accepted-jump effects share a prioritized Paula
gameplay channel;
the existing rapid plasma sound remains independently available. Destroyed
beetles receive a five-to-ten-second cooldown and can respawn indefinitely only
after their complete patrol zone is safely outside the camera. Returning through
earlier level areas therefore creates fresh encounters. Reaching the far-right
world edge temporarily resets the player, camera, projectiles, collectibles and
enemy encounter state in memory without reloading resident level assets. This
right-edge replay stands in for the later `LEVEL_COMPLETE -> next level` flow.
Alpha.40 HD still supports clean left-mouse Workbench restoration and writes
its high-water log only after takeover ends. This is now classified as temporary
review instrumentation: the next official HD and ADF builds are reset-to-exit,
while a separately named debug build retains logging but does not reopen
Workbench. Startup diagnostics can be written and closed before takeover.
Gameplay diagnostics require an explicit safe flush before reset because a
plain reset discards buffered RAM and cannot guarantee a valid DOS file.

Phase 5C.3 places two guaranteed Clockwork Storm Striders—one on a raised
platform and one on the floor—with one optional third encounter in the level
data. They patrol independently at randomized speeds using an eight-frame
rigid mechanical gait. At each authored patrol extremum they stop briefly in a
planted frontal turn pose, then resume from walk frame zero in the opposite
direction. Their 64x64 four-plane Bobs share one packed cache and retain the
accepted camera culling and synchronized background restore/draw pipeline.
This paragraph describes the original 5C.3 checkpoint; current Striders also
have the later accepted contact, ranged, hurt, destruction and respawn work.
Their cool
navy/violet/blue armour and cyan energy identity deliberately avoid the warm
orange palette shared by Sparkpaw and the beetles.

Phase 5D adds one deliberately authored traversal proof to the required raised-
platform Strider. At the platform's right launch zone it stops for two cyan
compression stages, follows one fixed ballistic arc to the adjacent lower
floor, lands in a planted pose and recovers before adopting the destination
patrol surface. That floor route stops at the next low-platform face so the
64px body does not pass through it, while extending left beneath the clear
underside of its original platform and stopping before the next column. The link
and trajectory are test-level data, so later level art and geometry can replace
them without redesigning the AI.
Runtime slots 0..17
retain their accepted/reserved meanings; traversal poses are appended in slots
18..23. Complete jump state survives camera-slot parking. The first FS-UAE
observation exposed a destination overlap; follow-up tests accepted the corrected
jump, low-platform turn and patrol beneath the original platform. Real-hardware
review remains open.

Phase 5E.1 replaces coordinate ownership with stable authored surface IDs while
preserving the accepted 5D behaviour. Enemy spawns now reference only their
starting surface; traversal links connect source and destination surface IDs,
and a Strider persists its current surface through camera-slot parking. Runtime
patrol bounds remain cached on the actor for unchanged movement and culling.
This data/AI refactor added no return jump, new route, art or renderer work and
was accepted in focused FS-UAE regression.

Phase 5E.2 adds exactly one explicit return link. After the floor Strider turns
left before the low platform, it reaches a second two-stage compression zone,
uses a steeper fixed arc around the raised platform's right face, lands back on
top and recovers before resuming its raised patrol. The existing traversal poses
are reused. This proves links in both world directions without offscreen
simulation, general pathfinding, new geometry or renderer changes. The first run
exposed that the completed downward link ID
was not cleared at recovery, which blocked all subsequent links; recovery now
releases that ID while preserving the destination surface and patrol state.
The corrected two-way loop was accepted from MrDig's supplied FS-UAE HD video;
ADF parity and real-hardware verification remain open.

Phase 5E.3 keeps persistent Striders moving logically while camera parking has
released their bounded runtime/Bob slot. Each parked encounter receives exactly
one ordinary world-space AI/physics update per game frame, but performs no Bob,
restore, cache or displayed Chip-RAM work. When its current route approaches the
camera, the active pool restores that complete position, surface, direction and
traversal phase. Beetle parking remains unchanged. This adds no spawn-at-camera
shortcut. MrDig's follow-up FS-UAE test accepted this behaviour.

Phase 5E.4 adds narrow failure rules without changing geometry: both extreme
landing-foot probes must
have authored support and body clearance before telegraph begins. A blocked
destination reverses the Strider on its source surface. A missed landing or
96-frame flight timeout restores the stored launch position/source surface,
shows planted recovery and resumes away from the failed link. Successful links
retain the accepted visual path.

The first 5E.4 HD/FS-UAE review found that the downward arc crosses destination
height one update just before its landing window. Failure detection now waits
until the Strider has actually passed that window in its direction of travel;
the independent 96-frame cap remains. MrDig's corrected HD/FS-UAE retest
accepted the restored down, lower-floor patrol and return loop. No ADF-specific
or real-hardware result was supplied.

Phase 5E.5 gives Strider 2 its first authored traversal. It starts on the
existing high platform at x=848..991, approaches right, telegraphs and jumps
across the adjacent 80px open-air gap to the lower x=1072..1199 platform, then
returns left. The rejected first layout put the link at the remote left edge;
the slowest existing randomized patrol needed almost seven seconds to reach it
and looked stuck in a six-second recording. The revised loop stays in the same
camera scene as Sparkpaw's approach. It changes only stable surfaces, spawn
ownership and link data; the continuous player floor, foreground art, collision
asset, renderer and combat remain unchanged. MrDig's follow-up HD/FS-UAE
recording accepted the complete repeated loop in
both directions, including both landings and patrol recovery. No ADF-specific
or real-hardware result was supplied.

Phase 5F.1 adds Strider body contact without adding an attack state. Its fixed
logical contact box covers x+11..52 and y+7..61 inside the 64px cell, excluding
transparent side margins and bottom rows. Overlap reuses Sparkpaw's accepted
half-heart damage, directional knockback, low-ceiling hurt selection,
invulnerability blink, life-loss and level-reset path. Strider walk, endpoint
turn and traversal state continue normally; shooting, Strider hurt/death,
renderer work and reserved slots 9..17 remain untouched. MrDig's broad
HD/FS-UAE contact test accepted 5F.1: low/lying approaches,
ordinary walking contact and jumping contact all damage Sparkpaw correctly.
No ADF-specific or real-hardware result was supplied.

Phase 5F.2 adds one camera-aware Strider ranged attack. A grounded Strider that
is fully visible, not turning/traversing and facing Sparkpaw at a horizontal
distance of 48..208px may stop for a 24-frame cyan charge in runtime slot 9,
show slot 10 for release and fire a slower hostile plasma pulse. Six player
projectile slots and two hostile slots are separately reserved, so enemy fire
cannot steal Sparkpaw's rapid-fire capacity. Both use the existing packed
patterns and synchronized line-253 Bob pass. Hostile contact is consumed and
reuses Sparkpaw's accepted damage path. No attack starts offscreen; a pending
shot is discarded if camera parking occurs. Slots 11..17 were still reserved
at this checkpoint and no new sound/Paula ownership was added. MrDig's
HD/FS-UAE recording accepted the
functional ranged core: repeated shots,
damage/invulnerability and continued routes work. The presentation is not final.
The hostile pulse still shares Sparkpaw's cyan identity, it has no sound, and
the derived attack poses lack a clearly readable gun or gauntlet. Phase 5F.2A
will polish only slots 9/10 with an integrated storm gauntlet/muzzle, give the
pulse a distinct hostile colour identity and add one short original shot sound
with explicit Paula priority. No ADF-specific or real-hardware result exists.

The first 5F.2A procedural violet overlay was rejected because it appeared to
come from the Strider's belly. It has been replaced by a genuine premium attack
source: the accepted body is preserved and only the forward claw/forearm becomes
an integrated storm arm cannon. Slots 9/10 derive from that one source; mirrored
cells and projectile origins use its white muzzle. Hostile plasma keeps the
accepted 16x9 mask/impact animation but now maps to hot orange/red with a white
core. This avoids confusion with an existing violet parallax light that the
supplied recording made look like persistent shot residue; the mark showed no
projectile animation and correctly remained after reload because it is level
art. The original 0.16-second electrical thump plays on
Paula channel 1 at priority 7: player hurt at 9 can interrupt it, while it can
replace ordinary effects at priority 6 or below. Gameplay timing is unchanged.
MrDig accepted the authored cannon, muzzle and orange/red projectile in the HD
FS-UAE review. The first sound was functional but too light; its source is now a
0.20-second heavier bass-body thump with a metallic crack and short electrical
tail. Paula ownership, priority 7 and attack timing are unchanged.
The subsequent review exposed genuine orange shot residue after a runtime
reset. Projectile reset now snapshots and restores Bob rectangles for all eight
pool entries, including hostile slots 6..7; player fire is explicitly limited
to slots 0..5. This lifecycle fix does not alter the line-253 pass or draw order.
MrDig's focused HD/FS-UAE retest accepted the heavier discharge and confirmed
that loose hostile shots no longer remain. Phase 5F.2A is accepted; no
real-hardware verification exists.

Lethal projectiles now select a dedicated original enemy-death cue instead of
the ordinary hit-pop. The shared beetle/Strider sound is a compact mechanical
drop with a shell break and metallic tail, inspired only in weight and brevity
by the supplied ThunderCats level-one gameplay reference. It does not copy its
sample. Paula channel 1 priority is 8: player hurt (9) may interrupt it, while
it replaces Strider fire (7), ordinary enemy hit (6) and lower effects. MrDig
accepted the resulting cue in supplied testing.

The HUD also shows the current attempt stock. A new test run starts at `x3`;
each zero-health reset steps through `x2` and `x1`. Until the dedicated
game-over state is implemented, losing the third attempt starts a fresh `x3`
test cycle. Reaching the right edge replays the test level while preserving the
current life stock and diamond count; final level-completion persistence will
be defined with real progression.
The HUD is modular rather than a table of complete life/health combinations:
one static base, compact health and lives patch atlases, and two presentation
buffers keep updates tear-free. Only a stale dynamic panel is copied with the
Blitter when its value changes; gameplay never CPU-composites displayed Chip
RAM. Twenty fixed diamonds form short trails and original arcs across the
five-screen test level. One fixed HUD-matched silhouette and a gentle
two-pixel hover animate their camera-culled Bobs without rotation. Picking one
up removes its Bob, plays a short arcade collect effect and increments the
compact two-digit HUD counter. The fiftieth pickup changes `49` to `00` and
awards one life. The display supports `x1` through `x9`; at `x9` the counter
remains at 49 rather than silently consuming unrewarded pickups.

## Build

Install a project-local VBCC/NDK toolchain under `.toolchain/` as described in
the repository's [build guide](../docs/BUILDING.md). The directory is
intentionally ignored by Git and must not depend on the ChipSnake or Futsal
toolchains. Install the host Python requirements from the repository root with
`python3 -m pip install -r requirements-dev.txt`. Then, from this directory:

```sh
make
```

This regenerates planar runtime assets and builds the native executable
`sparkpaw`. Run `make release` to rebuild all test packages:

- `dist/Sparkpaw-0.6.0-alpha.59.lha`
- `dist/Sparkpaw-0.6.0-alpha.59.zip`
- `dist/Sparkpaw-0.6.0-alpha.59.adf`
- `dist/Sparkpaw-0.6.0-alpha.59-WHDLoad.lha`
- `dist/Sparkpaw-0.6.0-alpha.59-WHDLoad.zip`
- extracted review drawer `dist/Sparkpaw-0.6.0-alpha.59/`

Release LHA files use genuine `-lh5-` compression. Packaging requires classic
LHa 1.14i at `.toolchain/lha/bin/lha` (ignored, project-local), or an equivalent
creation-capable binary selected with `LHA=/absolute/path/to/lha`. Homebrew's
Lhasa `lha` command is extraction-only and is not a valid substitute. Both HD
and WHDLoad packagers CRC-test the completed archive and reject output without
at least one `-lh5-` member.

The WHDLoad archives contain a versioned self-contained drawer with the
WHDLoad-specific executable and assets under `data/`, a Kickstart 3.1 BootDOS
slave, a Workbench icon and an installation/test ReadMe. They require WHDLoad 19 or
newer plus a legal A1200 Kickstart 3.1 ROM/RTB pair; neither is redistributed.
F10 exits through WHDLoad. Run `make whdload` to rebuild only these two
archives. Supplied real-A1200/68030 testing accepts startup, loading and the
alpha.51 direct-CIA F10 return to Workbench. The normal HD/ADF executable stays
separate; no FS-UAE, ADF or broader WHDLoad gameplay result is inferred.

Both launcher icons are generated from
`assets/concept/sparkpaw-newicon-cover-source-v1.png`. Their preferred 34-colour
NewIcons layer and standard eight-colour fallback are both 86x93. The HD icon
uses `DefaultTool=Sparkpaw`; the WHDLoad icon uses `DefaultTool=WHDLoad` with
`SLAVE`, `PRELOAD` and `PAL`. `tests/test_sparkpaw_icon.py` protects dimensions,
depths, palette count, shared pixels and launch metadata.

The source ZIP is deliberately omitted by default because it is over 100 MB.
Create it only on explicit request with
`../.venv/bin/python3 tools/make_release.py --include-source` after the build.

`tools/make_release.py` owns the SemVer prerelease value shared by the HD, ADF
and WHDLoad packages. Its minor component
tracks the broad roadmap phase (`0.6.x` for Phase 6); the exact lettered
checkpoint remains explicit in this README and the packaged ReadMe. Increment
the prerelease counter for later meaningful packaged checkpoints within the
same broad phase. Release packaging validates that the SemVer minor matches the
numbered roadmap phase and fails on drift. Maintaining this identity is part of
every roadmap step rather than a separate manual request. Each release removes
older `Sparkpaw-*` artifacts from `dist/` before writing one consistently named
ADF/LHA/ZIP/source set, preventing stale milestone files from being mistaken
for the current test build.

The DOS1/FFS ADF contains `S/startup-sequence` and boots directly. Its gameplay
data reconstructs to the same bytes as HD, but Stage B deliberately uses an
ADF-only executable plus SPR1 streams for front, rear, Strider and player data.
ZIP/LHA retain the ordinary executable, loose SPBM files and existing loader.

Run `make adf-report` for the ADF-only Stage A storage measurement. It writes
ignored JSON and Markdown reports under `build/adf-report/` with raw sizes,
CRC32, host-verified zlib-9/LZMA-9 proxy sizes and projected DOS1/FFS blocks.
This command does not alter the executable, runtime assets, HD ZIP/LHA layout or
Amiga loader. The proxy codecs measure opportunity only; neither is selected as
the eventual ADF codec.

Stage B first packed `storm-front.spbm` with the project-owned SPR1 byte-run
format; MrDig reported that supplied ADF works correctly. The same accepted
path now covers `storm-rear.spbm`, whose ADF was also accepted, plus the current
in-review Strider and player sources. The ADF executable streams all four directly into their final
allocated bitplanes using a 512-byte input buffer and validates raw size plus
CRC32; it never allocates a second complete foreground copy. Release validation
extracts every packed file and compares its host-decoded bytes with the
canonical SPBMs. Test the ADF from a cold boot through title, `LOADING`,
`CHARGING` and all twelve gameplay screens. Foreground and rear parity are
accepted; now exercise both Striders through walk, turn, shoot, hit, traversal
and death/respawn. A corrupt/truncated stream must fail loading rather than
enter gameplay.

Run `make bench` to build the isolated `sparkpaw-renderbench`. This small
program validates the dual-playfield foundation before it is allowed back
into the game; see `docs/RENDERBENCH.txt`.

Run `make phase6-memory` for the isolated 3072px Phase 6C.1 resident-memory
measurement. It writes a directly runnable HD test drawer under
`build/test/Sparkpaw-Phase6C1-3072/` using the exact production assets and
compiles only the test executable with `SPARKPAW_WORLD_W=3072`. Once gameplay
prepares successfully it writes
`phase6-memory.log` beside that executable with Chip RAM free/largest values.
Alpha.40's normal HD `sparkpaw` also records prepared-peak and post-run memory
in `renderdiag.log` on left-mouse exit. At the next checkpoint this moves to an
explicit debug build. Its gameplay samples stay in Fast RAM until a deliberate
safe flush writes and closes the file, after which it waits for reset rather
than returning to Workbench. The official executable contains neither facility.
The following Phase 6A measurements remain historical baseline evidence.
Phase 6A.2 releases the exact 325,220-byte player, enemy and collectible
conversion sources after their final DMA caches and the Copper palette have
been built. Its log records Chip and Fast free/largest values both immediately
before that release and after complete preparation.
MrDig's supplied 2 MB Chip plus 8 MB Fast tests ran correctly through the
repeated-art right edge. Explicit `MEMF_ANY` CPU-only planar allocations remove
the former Chip conversion peak: the final log measures 531,464 bytes Chip free
and a 530,408-byte largest block both during and after conversion. Fast rises
from 6,349,280 to 6,674,416 bytes when the 325,136-byte temporary representation
is released. Phase 6A is complete and 2048px is the Phase 6B greybox basis,
while final level length remains a later pacing decision.

Phase 6B.1 makes 2048px the production greybox width. It preserves the original
five-screen route, then adds five platform sections, four authored beetle
candidates and twelve diamonds across screens six through eight. The active
enemy renderer/pool is still limited to four camera-managed slots, and the
optional third Strider remains gated. The continuous floor intentionally stays
for this pacing review; the first water hazard is the separate Phase 6B.2 step.
Diamond Bobs now use a padded two-word source row so trails may use arbitrary X
positions without shifted Blitter reads crossing a cache row.
Any player shot may hit a beetle when the existing geometric body rectangle
overlaps. Grounded standing shots retain the natural floor-beetle miss through
their Y position, and crouch shots retain the ordinary low attack contract.

## Source layout

- `src/main.c`: startup, cleanup, explicit application states and the
  raster-phased top-level loop
- `src/title.c` / `src/title.h`: direct six-plane AGA title/loading presenter
  with ordinary (non-EHB) 64-colour palettes, black border blanking, smooth
  VBlank-synchronised 24-bit palette fades, Chip RAM Copper lists and complete
  screen switches while DOS remains live

The startup presentation leaves a black PAL display briefly in place for
scandoubler lock, then keeps the fully faded title visible before loading.
While files are read the loading screen says `LOADING`; once disk I/O is done
it switches to `CHARGING` during the silent sprite, Bob, bitplane and Copper
preparation phase. This second phase remains visible for at least two seconds,
including on accelerated systems. Both screens share one 64-colour palette.
- `src/renderer.c` / `src/renderer.h`: gameplay display, Copper construction,
  six-channel hardware-sprite player, fixed bottom HUD Copper split, packed
  render caches and Bob rendering behind an explicit renderer API
- `src/game.c` / `src/game.h`: gameplay initialization and update ordering,
  frame progression and camera state
- `src/platform_amiga.c` / `src/platform_amiga.h`: graphics-library lifetime,
  custom-chip takeover/restore, raster reads, port-2 secondary-button/POTGO
  handling and the required Blitter wait;
  Copper construction and concrete rendering commands remain in `renderer.c`
- `src/enemies.c` / `src/enemies.h`: fixed enemy pool, typed patrol AI, Strider
  walk/turn state, hit detection and damage state; runtime slots retain an
  explicit link to their spawn record
- `src/level_data.c` / `src/level_data.h`: compact typed enemy spawn records
  containing safe position ranges, stable authored patrol-surface IDs, initial
  direction and persistence policy; four beetle encounters
  are required and two are optional. Two required and one optional Strider
  record share the same bounded runtime-slot activation model. One explicit
  Phase 5D traversal link records its launch zone, source/destination surface
  IDs, landing zone and fixed ballistic parameters separately from the enemy AI
- `src/projectiles.c` / `src/projectiles.h`: projectile pool, spawn, movement,
  impact state and hit dispatch; packed plasma rendering remains with the
  renderer-sensitive code in `renderer.c`
- `src/collision.c` / `src/collision.h`: collision-map loading and solid-point,
  horizontal-span and vertical-span tile queries shared by gameplay modules
- `src/player.c` / `src/player.h`: player state, joystick input, shooting,
  movement/physics, the accepted 50-frame baseline and eight appended standing
  and crouched hurt poses; hardware
  sprite preparation and Copper updates remain in `renderer.c`
- `src/assets.c` / `src/assets.h`: SPBM loading, validation, gameplay-asset
  lifetime and cleanup plus a separate early-title lifetime; packed
  hardware-sprite and Bob cache preparation remains in `renderer.c`
- `src/audio.c` / `src/audio.h`: energy-shot, player-hurt, enemy-hit/death and
  jump sample loading, Paula channel 0 rapid-shot playback, prioritized channel
  1 gameplay effects
  and explicit hardware-active lifecycle control; channels 2-3 remain reserved
  for a future music layout
- `tools/generate_runtime_assets.py`: creates wide planar playfields, source
  sprite/enemy planes and masks, packed Bob caches and the tile collision map
- `tools/generate_sparkpaw_sfx.py`: regenerates the Paula-ready raw samples
- `tools/make_release.py`: creates the HD archives, source archive and ADF
- `assets/runtime/`: compact data loaded by the Amiga executable
- `assets/concept/`: full-resolution concept art and AGA preview conversions
- `assets/sprites/`: prototype animation art and named frame metadata
- `assets/enemies/`: native-resolution enemy art, preview and frame metadata
  (including the historical eighteen-frame upright Strider palette proof, the
  current 28-frame runtime contract and preserved concept/scale review sources)
- `assets/sfx/previews/`: WAV previews for later milestones
- `sfx/raw/`: signed 8-bit mono Paula-ready samples; the current build uses the
  energy-shot, player-hurt, enemy-hit and jump samples and reserves the others
  for later steps

## What to test

Walk through all twelve screen widths, jump onto and off every platform and
reverse direction frequently. Verify that the foreground follows the camera
while the mountain/tower layer moves at one quarter speed. Compare HD and ADF
behaviour. Useful reports include the exact location and whether the issue
concerns sprite flicker, tearing, collision, camera movement, parallax,
controls or display colours. Combine camera movement with rapid fire and watch
specifically for a one-frame mixed-scroll tear under both quiet and heavy
Blitter workloads.

Check especially whether Sparkpaw retains the same apparent size and foot
position while running, jumping, crouching, shooting and performing the idle
pose. Frames 0-49 use one anatomical scale per authored family
and stable shared anchors. It includes an eight-stage grounded run, a
scale-locked four-stage jump, three-stage landing, six-stage momentum-based
direction change, twelve-stage idle performance and dedicated grounded,
airborne and crouched firing poses. Frames 50-53 append contact impact,
backward slide, airborne recoil and recovery without renumbering the accepted
baseline. Frames 54-57 provide a separate 24-pixel-high crawl-recoil family so
contact recoil respects the low crouch clearance beneath a platform.
Crouching changes posture without enlarging
the character. More in-between poses can be added without changing the DMA
renderer.

For Phase 5C.3, verify a raised-platform Strider early in the level and a long-
route floor Strider farther right; a third authored Strider is optional per
level selection. Their eight-frame gait should remain mechanical and stable at
all randomized speeds. The frontal pose may appear only when direction changes
at an authored patrol endpoint—never between walk frames. Walk them on and off
screen, trigger life-loss/right-edge resets and watch for stale 64x64 Bob
pixels. They must not yet damage Sparkpaw or absorb shots. Beetle behaviour
must remain unchanged.

For Phase 5D, follow the raised-platform Strider until it reaches the right
launch zone. Verify two distinct compression holds, a continuous cyan-signalled
arc that remains wholly visible, an exact lower-floor landing, planted recovery
and only then resumed grounded patrol. Confirm that it now turns before the low
platform at x=496 instead of walking through it, then walks left beneath its
original x=320..415 platform and turns before the x=288 column. Move the camera
away during telegraph, flight and recovery, then return: each phase must resume
rather than reset or turn into a walk frame. Recheck life-loss and right-edge
level replay for stale 64x64 pixels. This phase still adds no Strider combat or
generic navigation.

For the Phase 5E.1 regression, repeat that complete accepted route and a life-
loss/right-edge reset. Behaviour must be identical: one downward jump only,
then the same safe floor patrol. Also revisit both required Striders after they
leave the camera; neither may reset to a wrong surface or trigger the jump twice.

For Phase 5E.2, remain near the first Strider after its downward jump. It must
turn at the low platform, travel left, telegraph again at x=440..444, clear the
right side of its original platform, land on top and recover. It should then
patrol left, turn, and eventually repeat the downward link. Watch especially for
platform intersection, a duplicated jump trigger, stale Bobs and incorrect
surface state after moving the camera away and returning.

For Phase 5E.3, leave the first Strider behind during each of these states:
ground patrol, compression, flight and recovery. Wait several seconds, return,
and verify it continued rather than freezing or restarting. Approach the route
from both sides and watch for natural entry, stale Bobs, double-speed movement
at the park/unpark boundary and a hidden actor inside the visible viewport.

For Phase 5E.4, first confirm several ordinary down/up loops remain visually
identical. Current geometry intentionally does not trigger failure. Regression
must show no refusal to launch, teleport or extra turn at either valid link.
Blocked/missed branches are host-contract coverage until a later authored test
surface deliberately exercises them.

For Phase 5E.5, approach Strider 2 around the later pair of raised platforms.
It should approach the right edge of the high platform, show the same two-stage
compression, clear the visible gap and land planted on the lower right platform.
After that patrol it should telegraph and jump left across the same gap, repeating
the loop. It must not snap back, touch the floor, intersect either platform edge
or disturb Strider 1's accepted loop.

For Phase 5F.1, touch each Strider during ordinary patrol, an endpoint turn and
both sides of a landing/recovery. Each accepted contact removes one half-heart,
plays the existing hurt sound, knocks Sparkpaw away from the Strider centre and
starts the accepted blink; continued overlap during blinking must not deal more
damage. Confirm crouched contact stays low under clearance and that losing all
health follows the existing life/reset flow. Neither Strider may freeze, reset
its route or select slots 9..17 after contact.

For Phase 5F.2, stand 48..208px in front of a grounded, fully visible Strider.
It should stop, hold a clearly readable cyan charge, release one slower plasma
pulse and resume its prior patrol direction after a cooldown. Verify the pulse
hits walls, damages Sparkpaw once, respects blinking and cannot reduce the six
player-shot capacity. Approach from behind, leave the camera during charge and
watch traversal/turn states: no unseen shot, mid-flight attack, stale projectile
or route reset may occur.

For the 5F.2A polish retest, also confirm that the arm cannon reads as integrated
at native resolution, the orange/red pulse differs from Sparkpaw's cyan shot
and from the static violet parallax lights, both facings emit from the white
muzzle, and the short electrical thump is
audible without masking or interrupting player hurt.
This checklist passed in HD/FS-UAE, including the later reset-residue retest.

For Phase 5F.3, hit either grounded Strider with standing, airborne and crouched
plasma. Each accepted hit should consume the shot and show the complete
slots-11..17 mechanical recoil for 14 frames; rapid shots during that reaction
must not stack damage or restart it. After two damaging hits the three-HP
Strider remains alive at one HP and resumes the same patrol/turn or ranged
behaviour. A hit during a grounded charge may cancel that charge. Shots during
slots 18..23 traversal must consume the shot and remove HP without interrupting
or renumbering the accepted route. The projectile impact/sound is the immediate
feedback; no delayed slots-11..17 recoil may play after landing. Also compare
slots 9/10 directly with walk and hurt:
head, torso, legs, grounding and colour balance must remain identical, with
only the forward forearm replaced by the arm cannon. Death, teardown and
respawn are deliberately absent in this test.

For the muzzle-alignment retest, slot 9's charge, slot 10's white muzzle and the
first hostile-pulse frame must share local row 36 in both facings. The pulse
must not appear above the cannon; speed, cadence and collision remain unchanged.

For Phase 5F.4, deliver the third damaging shot while the Strider is grounded.
It should stop contact/ranged behaviour and play appended slots 24..27: core
fracture, collapse, burst and sparse grounded debris. After the debris clears,
leave the complete authored starting surface outside the camera for roughly
five to ten seconds, then return and verify a fresh three-HP Strider follows its
route without stale pixels. Also deliver the third hit during compression and
flight: traversal must stop immediately and the burst must begin at the current
world position. Recheck both required Striders and beetle respawn. The HD
build is testable; the unoptimized single ADF is temporarily over capacity.
All Strider frames, including destruction, must retain the indigo/violet/cyan/
white identity. The accepted walk/idle indexed pixels are the visual master;
death fragments reuse those exact material pixels. Sparkpaw-orange pens 2 and
3 are forbidden, and violet may never outnumber steel/charcoal in any frame.

The Milestone 2A beetle art is a 32x24, nine-frame, three-plane masked Bob.
Four to six level instances share one packed art cache and retain independent
HP and animation state. A bounded four-slot runtime pool activates them near
the camera and parks their state when safely distant. Their fixed height keeps
ordinary standing shots above floor beetles, while the domed steel/violet
shell, round cyan lens and jointed legs follow the
gameplay concept at native AGA resolution. Enemy and plasma restore/draw passes
use synchronized Blitter DMA with standard-copy and cookie-cut minterms; the
68020 no longer composites their planar rows byte by byte in Chip RAM. Verify
that all selected beetles walk smoothly between their patrol limits, mirror cleanly
when turning and remain grounded. Verify standing fire still misses floor
beetles, crouch fire still triggers their hit/death lifecycle, and a standing
shot damages an elevated beetle when its sample crosses the existing rectangle.
Stress the renderer with several simultaneous plasma pulses and visible
beetles, and watch for residue where enemies and projectiles overlap.
On each beetle's second hit and each Strider's third hit, confirm the ordinary
hit-pop is replaced by one short heavier death cue. Test grounded and traversal
Strider deaths, rapid fire and simultaneous player damage; player hurt must win
priority and no death cue may repeat during the destruction frames or respawn.

For Phase 6A, boot an A1200 configuration with exactly 2 MB Chip RAM and no Fast
RAM, run `build/test/Sparkpaw-Phase6A-2048/Sparkpaw`, wait until gameplay
appears, then reset and return `phase6-memory.log`. Also confirm scrolling reaches
the repeated-art right edge without allocation failure or corruption. The host
projection is 270,336 extra bitmap bytes plus 672 collision bytes; acceptance
requires the measured prepared free Chip total and largest contiguous block.
MrDig completed this supplied FS-UAE test: the 2048px route ran to its right
edge, but the log reported only 88,136 bytes free and an 86,816-byte largest
block at the preparation peak. Even after the temporary charging presentation
is released, the estimated steady margin is only about 150 KiB. The fully
resident 2048px result is therefore a successful 2 MB/no-Fast stress
measurement, not a rejection of that layout. With 2 MB Chip plus 8 MB Fast now
accepted as the minimum, a complete efficiency audit and newly instrumented
test must precede the resident-versus-segmented decision. Additional capacity
must not justify duplicate or unnecessarily Chip-resident data.
