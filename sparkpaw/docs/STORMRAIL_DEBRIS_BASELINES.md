# Stormrail debris baselines

## Debris 1.0 — gameplay reference, 3 September 2026

User status: accepted as a good first gameplay setup and suitably challenging.
The authored debris movement is accepted as a reference. The native masonry
art remains explicitly provisional: readable and functional, but judged too
cheap beside the accepted Dart and Orb polish. Do not treat its visual style as
the maximum AGA quality target.

Contract:

- 28 deterministic events driven by monotone `stormrailDistance`, 6600..9300;
- hard pool of six obstacle Bobs;
- primarily right-to-left movement at three speeds, with shallow vertical
  drift and safe-boundary reflection;
- prebuilt two-frame tumble animation, changing every eight fields;
- solid debris absorbs shots with impact audio and does not flash;
- selected debris is destructible and flashes for four fields on hit;
- two large six-HP loot blocks adopt the cracked frame at three HP and use
  existing diamond pickup IDs 12 and 13;
- ordinary destructible debris does not automatically drop diamonds;
- no runtime rotation, scaling, particles, split physics or allocation.

Recovery evidence:

- active 68030 drawer: `dist/Storm-Debris1.0-030-HD`;
- source snapshot: `dist/older-builds/Storm-Debris1.0-source-20260903.tar`;
- executable SHA-256:
  `a238783dd4e6240bfaa1d4f7b598efe9b0142f7237b50183bb5638bac3995193`;
- obstacle SPBM SHA-256:
  `b4b5423ffe5d9fd7ce72a057a9f41fd1d1b545fa22eafe11932240cfa3253971`;
- source snapshot SHA-256:
  `16f0fce3109375bb5fd2b6a65772a24b1a3212efa5caa0a464f3c3c48597841b`.

The bounded automatic proof reached all 28 event bits
(`obstacle_spawn_mask=268435455`), peak occupancy 6, zero unsafe blits and
1,290,992 free Chip bytes. This is technical evidence only; the user's 68030
play report is the authority for challenge and movement feel.

Future visual candidates should change the obstacle source/caches only and use
Debris 1.0 unchanged as the gameplay, timing, collision, loot and density A/B
baseline. If an art experiment harms readability, performance or feel, restore
the exact drawer/source snapshot above.

### Debris 2.0 concept gate

The supplied Project-X asteroid MOV confirms that apparent richness does not
require constant rotation: several rock interiors remain stable across
consecutive frames. Its quality comes from irregular mass, broad connected
light/shadow planes, material-coloured edges and sparse pits. Translate those
principles into Storm Ruins masonry; do not copy asteroid subject matter.

Concept study v1 is rejected because it produces complete miniature
architecture and scaled families. Study v2 is the pending direction: twelve
isolated broken masses across large, medium and small classes, with partial
arch/lintel/buttress clues and restrained Stormstone traces. Neither study is a
runtime asset. Native translation must be separately approved and hand-authored
at exact target sizes.

The first direct-polygon native v1 sheet is also rejected: it discards v2's
perspective, masonry depth and material transitions and collapses back into the
flat Debris-1 look. Do not extend that family. Native v2 instead starts with one
48x40 palette-aware underpainting taken from a single approved concept mass;
clean and retopologize its pixels at native size before attempting more pieces.

Native v3's blanket blur, black-to-violet remap and majority cleanup is rejected
because it smooths away depth and turns the underside into a flat purple mass.
Native v4 returns to the unblurred underpainting and makes only bounded pixel
edits: black touching transparency becomes steel shadow, supported lower shadow
pixels join coherent planes, and one two-pixel cyan/blue Stormstone glint marks
the central rib. Its exact visible bounds are 34x38 inside the 48x40 cell. This
single-object polish study is pending user review and is not runtime-connected.

The user accepts native v4 as the correct approach and requests the remaining
family in the same method. Native family v1 contains twelve unique indexed
objects derived independently from the approved v2 concept: four 48x40 large
cells, four 32x40 medium cells and four 16x16 small cells. Each source component
is independently fitted and palette-mapped; no native object is made by scaling
or rotating another native object. Cleanup changes only black silhouette pixels
and supported lower shadow clusters. The exact family SHA-256 is
`6e10029f19ec911a4a90bc7310c33b434eaff678852bf2d6440517f02ba1b5fd`.
It remains a concept asset pending user family review and is not connected to
the Gate-4 renderer or Debris 1.0 runtime sheet.

User review now accepts the native family v1 direction as “oogt ok” and asks
for continuation. This approves the volume-preserving method and the twelve
base masses, not yet their runtime integration. The next art gate is a compact
damage/tumble study: damage must read through displaced stone planes and a
silhouette chip rather than a neon crack, while tumble counterparts must be
prepared before native reduction and cleaned independently instead of rotating
the finished native pixels. Project-wide rejection and polish criteria are in
[the AGA art-quality contract](AGA_ART_QUALITY_CONTRACT.md).

Native animation study v1 implements that next gate without touching runtime.
It contains two independently reduced 48x40 tumble pairs, one 48x40 damaged
pose, two independently reduced 32x40 pairs and two 16x16 pairs. Turns are made
at concept resolution and each result passes through exact-size FRONT16
reduction and bounded native cleanup; no finished native cell is transformed.
The damaged pose combines a small missing silhouette chip, a stepped recessed
stone fracture and three restrained Stormstone catch pixels. Exact indexed
sheet SHA-256:
`477b929bae0d3810a5895f3049ed46535dfc7f01fd1ef9fbb20b43f105db1ea9`.
The first contact sheet accidentally placed its small-pair review row over the
second medium pair. User review identified the error; moving that review row
to y=96 fixes the overlap without changing any source cell.

Debris 2.0 is now connected as an art-only 68030 candidate while retaining the
exact 400x40 FRONT16 SPBM, cache geometry and Debris 1.0 gameplay contract.
Runtime slots contain two large base/tumble pairs, the damaged large frame,
three distinct small base/tumble pairs and one medium base/tumble pair. The
runtime obstacle SPBM SHA-256 is
`baa11660eac08c4058fd436b0c25e0eb8dfa0a130b0162ee983a650da317194f`.
The executable remains byte-identical to Debris 1.0 at
`a238783dd4e6240bfaa1d4f7b598efe9b0142f7237b50183bb5638bac3995193`.

The bounded production-renderer proof reaches all 28 event bits, peak
occupancy six, zero unsafe blits and 1,290,992 free Chip bytes. Its three
dedicated native debris captures show intact, damaged and mixed-size frames
without mask residue or clipping. This is technical evidence only. User review
accepts the 68030 art/feel result as visually good and authorizes the stock-
68020 cadence gate. The accepted 68030 drawer is archived intact as provenance;
no 68020 verdict is claimed until the matching low-overhead user log has been
inspected.

The next and only active drawer is `dist/Storm-Debris2-Cad-020-HD`. It retains
the exact accepted Debris-2 SPBM and gameplay while enabling only the minimal
cadence logger. Compare its complete debris workload with the empty Gate-2.5
baseline of 49.26 FPS over 1,000 intervals, 992 one-field intervals and zero
ownership violations. A material decline, three-field misses or any ownership
violation requires investigation before further content.

The first staged 68020 drawer is rejected before measurement: it inherited the
non-interactive proof cutoff at `STORMRAIL_CADENCE_END` and therefore froze
before reaching debris. The corrected interactive cadence build compiles that
cutoff out; the existing left-mouse diagnostic path is its sole save/stop
trigger. Do not interpret the rejected automatic stop as a performance result.

The user's corrected stock-FS-UAE/68020 run reaches and completes the debris
field before the manual LMB save. Its 166,404-byte `renderdiag.log` has SHA-256
`8285b2f092329d4236dc0eb1bf395f459bc92d93d1c01a6239b0f1bd6a2d103a`.
The low-overhead result is 49.95 FPS over 2,299 intervals: 2,298 one-field,
zero two-field, one three-plus interval, maximum three fields and zero renderer
ownership violations. The isolated worst interval is frame 2351.

This accepts Debris 2.0's measured 68020 performance. For comparison, the
protected empty Gate-2.5 log's fuller distribution is 992 one-field, three
two-field and five three-plus intervals (maximum four) over 1,000 intervals,
for 49.26 FPS. The Debris-2 run is longer, carries the complete enemies,
projectiles, pickups, audio events and six-slot obstacle field, yet improves
both effective FPS and outlier count. No targeted profiler or optimization is
warranted. User-observed visual smoothness remains distinct from this log
verdict and should be recorded if any stutter was noticed.

The user subsequently reports that the same stock-68020 run looked smooth.
Debris 2.0 therefore passes both measured and subjective 68020 cadence. Preserve
49.95 FPS / 2298 one-field / 0 two-field / 1 three-plus / 0 ownership
violations as the current complete-content performance reference before route
extension.

## Pacing 2 — current 68030 feel candidate

The accepted Debris 2 art, tumble pairs, hit feedback, collision rules and
six-object pool remain unchanged. The current candidate extends the debris
timeline from 32 to 48 distance events. It must read as one continuous authored
field using the already accepted mixture, motion vocabulary and organised-chaos
feel—not as an unchanged first field followed by a visibly different add-on.
Earlier formations are slightly closer together. Complete-formation rewards
now enter at x=316, move at three pixels per tick, use 18-pixel spacing and
select four compact arrangements; free diamond lines were moved away from those
reward moments so the four-slot reward pool cannot delay them.

The sole active user drawer is `dist/Storm-Pacing2-030-HD`. Its staging manifest
contains all 42 declared runtime files and 40 executable-discovered references;
the alpha.68 release inventory remained byte-for-byte unchanged. The accepted
Debris-2 68020 drawer and cadence log are archived under
`dist/even-older-builds/Storm-Deb2-020-accepted`. Pacing 2 has compile and host-
contract coverage, but no user visual acceptance or 68020 performance verdict
yet. Do not add dust particles until this pacing/debris extension is accepted.

The first staged Pacing-2 extension is rejected before acceptance: unbounded
pending events formed a late backlog in which fast small variants disappeared
while slow large blocks accumulated, producing an apparently endless
homogeneous row. The corrected timeline gives ordinary density events a
160-distance admission window, caps simultaneous large blocks at two and puts
both loot carriers in guaranteed early large slots. Its bounded native proof
handles all 48 event bits (`-1/65535`), reaches peak six / big peak two, reports
zero unsafe blits and retains 1,290,992 free Chip bytes. This is contract
evidence only; the replacement 68030 drawer still needs user visual acceptance.

Supplied 75.4-second FS-UAE/HD evidence rejects that replacement as well. The
opening retains variation, but its last third reads as a separate repeating
tail; free diamond line 3 also appears as four loose pickups inside debris.
The preserved evidence is
`testresults/Phase 6D-rejected-pacing2-repeating-debris-tail.mov` with its
matching sidecar and SHA-256 provenance.

Pacing 3 adds a hard endpoint but is rejected during observation of the
internal proof: its tail still repeats and one malformed upper-edge rock is
visible. Source audit finds the concrete art fault: three pillar events used
base frame 2 although the pillar family contains only frames 0-1, causing an
out-of-family buffer read. Pacing 4 corrects those indices and normalizes every
spawn frame through `stormrailObstacleSafeBaseFrame`; the host contract covers
big, shard and pillar limits.

Pacing 4 also removes the 160-distance admission wait for normal density cues.
A cue now spawns at its authored distance or is consumed immediately when the
six slots are already full; only two explicit loot carriers may wait. Table
simulation admits 41 visible events with a 13 big / 21 shard / 7 pillar mix and
average occupancy 4.32 versus approximately 4.56 for archived Debris 1.0. Free
line 3 moves to distance 5400 before debris starts at 5800. At distance 11200
the presentation camera stops and every Flight-owned object pool is cleared.
The bounded native proof records all 48 bits, peak six / big peak two, six
inactive slots at the endpoint, zero unsafe blits and 1,290,992 free Chip bytes.
The sole active drawer is `dist/Storm-Pacing4-030-HD`; visual acceptance remains
pending and no 68020 cadence verdict is claimed.

## Debris 3 — rejected visible-tail experiment

The 52.73-second user capture is preserved as
`testresults/Phase 6D-rejected-debris3-collapsed-repeating-tail.mov`. The field
begins with useful variation but its last part collapses into repeated small-
debris groups and fails to communicate a clean ending. Source audit found the
cause: a uniform event every 100 distance units (25 fields at cruise speed), a
saturated six-slot pool and a 6/29/13 big/shard/pillar source mix made slot
availability determine the visible composition. The old test checked source
windows and aggregate occupancy, not the simultaneous on-screen family mix.

## Debris 4 — pending 68030 visual gate

Debris 4 is one 48-event authored timeline from distance 5800 through 11130.
It balances eight large, twenty-four shard and sixteen pillar events, uses
non-uniform 80--150-distance spacing, and allows only one large block active.
The permanent timeline test now simulates admitted on-screen snapshots: busy
snapshots must retain large and pillar presence, may contain no more than four
shards, must admit both loot carriers and must drain completely after the last
event. Loot carriers explicitly reserve one of the six slots by retiring only
the oldest ordinary non-large cue; this prevents both loss and delayed backlog.

The bounded production-renderer proof reaches all 48 event bits (`-1/65535`),
peak six / big peak one, ends with all obstacle slots inactive, reports zero
unsafe Blits and retains 1,290,992 free Chip bytes. The test endpoint is 15200,
leaving 4070 distance units after the final debris event. This is technical
evidence only; visual/feel acceptance belongs to the user on 68030.

The Debris4 visual gate is rejected by the user. Its preserved recording shows
the sequence falling into a repeated large-plus-diagonal-small grammar and all
debris disappearing at the frozen endpoint, without readable moving empty
space. Real-runtime timing instrumentation proved why: the bitmask rescanner
admitted an old event as late as distance 15199. A full end mask therefore did
not prove monotone event consumption.

Debris5 replaces that scanner with one `stormrailDebrisNextEvent` cursor. The
cursor can only advance, so an old event cannot reopen. It also replaces the
repeated six-item template with asymmetric authored phrases. The native proof
now records final event id 47 at distance 11331, last active debris at 11815
and endpoint 15500, leaving 3685 moving empty units. Seven whole-emulator frame
captures across the field and reserve were inspected: compositions differ and
the last capture is debris-free while the route is still before its endpoint.
User 68030 acceptance remains required.

User review now marks Debris5 “acceptabel genoeg” as the fallback gameplay and
visual baseline. Preserve its staged executable/drawer intact even while small
polish candidates are evaluated. The user notes that apparent randomness can
still improve slightly.

Audit of the missing rock diamond found an ID collision, not a drop-rendering
failure: the two loot rocks used IDs 12/13 already owned by free line 3. If that
line was collected, `stormrailDropObstacleDiamond` correctly suppressed the
duplicate. Debris5.1 assigns the rocks unique IDs 32/33 via a second persistent
32-bit pickup mask. It also varies offscreen start X over 328..360 and derives
tumble phase from event ID rather than pool slot. Routes, density and the
monotone cursor remain unchanged.

Debris5.2 keeps Debris5 as the fallback and tunes two concrete readability
issues: the later large loot carrier moves at -3 rather than -4 so its six-hit
reward reveal has a longer aiming window, and medium/small horizontal motion
is limited to three upper/lower-lane accents. No medium/small horizontal event
crosses the middle band; all others retain straight shallow diagonal routes.
Formation reward IDs already use the persistent pickup masks, so collected
reward members stay consumed across a life restart while uncollected members
remain available.

The subsequent Heart1 candidate leaves Debris5.2 choreography unchanged. A
native 16x21 red heart uses the existing four-entry reward pool and
restores two half-heart HUD units, capped at six. One fixed ID 34 heart appears
between early formations with three widely spaced ring diamonds (IDs 35..37);
the slower large loot carrier's ID 33 is
changed from diamond to heart. Both remain consumed across life restart. The
sound reuses the diamond sparkle at stronger volume rather than adding another
sample or Paula channel.

User review accepts the final classic heart silhouette and the spacious
heart-centred three-diamond ring for now. The complete low-overhead stock-
FS-UAE/68020 run records 49.96 FPS over 2,886 intervals: 2,885 one-field, zero
two-field, one three-field (maximum three), zero ownership violations and
334/334 player shots. This is cadence-neutral against Debris2's 49.95 FPS.

Later Gate 5A pacing/pattern work and Gate 4D sunlit dust leave the complete
Debris5.2 48-event table, six-slot pool, art families, carrier speeds, drops and
natural empty exit unchanged. Gate 4D is a separate non-colliding eight-slot
renderer accent. Its accepted stock-FS-UAE/68020 log reaches 49.98 FPS over
6,036 intervals, with 6,035 one-field, zero two-field, one three-field, zero
ownership violations and 432/432 shots. This does not supersede Debris5.2 as
the recovery contract; it establishes cadence of the larger focused slice.

## Debris 1.1 — rejected art-only candidate

Debris 1.1 retains the complete 1.0 gameplay contract and the same 400x40,
4-bitplane cache dimensions. It revises only indexed pixels: more asymmetric
chipped silhouettes, uneven masonry joints, deeper neutral fracture faces,
sparser erosion marks, wider carved pillar mass and stronger prebuilt tumble
angles. Large blue/cyan side panels from the first internal pass were rejected
before staging; the review candidate returns those surfaces to neutral stone.
The executable remains byte-identical to Debris 1.0
(`a238783dd4e6240bfaa1d4f7b598efe9b0142f7237b50183bb5638bac3995193`);
only the obstacle SPBM changes, to
`d2d6a4d593b317594dd55906ad4c12ee2b4e23ec98af854230feffdc5ab2d0f3`.
The bounded proof again reaches all 28 events, peak occupancy 6, zero unsafe
blits and 1,290,992 free Chip bytes. User review rejects the art: it reads as a
less colourful, duller version of 1.0 with overly thick black edging and flat
comic-book/WordArt-like cutout forms. Preserve its drawer only as rejected
provenance. Debris 1.0 is restored as the sole active gameplay reference.

The corrective lesson from the supplied Zynaps and Project-X references is not
"add more internal linework". Their rocks build volume with connected clusters
of local shadow, midtone and highlight. Their outer edge is mostly the darkest
material colour, not a uniform black ink contour; highlights break before the
silhouette and crater/fracture marks follow planes instead of outlining every
stone course. Future Debris 2.0 work must begin from a fresh painted-volume
study, not another edit of the 1.0/1.1 line-art generator.
