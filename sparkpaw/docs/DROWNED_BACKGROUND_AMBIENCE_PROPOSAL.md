# Optional background ambience — 22 September 2026

> Historical proposal. Water and waterfall animation were subsequently implemented
> and visually accepted after a native Blitter-mask correction. See
> [Drowned Turbines lessons](DROWNED_TURBINES_LESSONS.md) for the retained findings,
> and [current status](CURRENT_STATUS.md) for later work and test acceptance.

Research/proposal only; no runtime, asset or dist changes. User accepts continuous panorama visually and asks for optional subtle animation appropriate to68020/2MBChip/8MBFast.

## Current renderer findings

Rear1552x208,3planes/eight shared pens; cameraX>>2. Front4planes. All distant landscape is baked into a single rear bitmap. renderer.c:copperRearPalette uses the same Drowned palette across existing horizontal colour bands. Global colour cycling would therefore animate mountains/trees/clouds too. AGA64 player uses2hardware sprite channels; do not assume remaining channels are available until Core allocation, priority, bank and fetch-mode ownership are audited. Rear source is copied into a guarded display bitmap; any changing rear patches must address that ownership explicitly. Retain Copper100staging,252HUD switch and253restore/update boundary.

## Recommended order

1. Quiet distant-water shimmer: restricted horizontal rear-water colour band, at most1–2subtly varying pens, precomputed low-amplitude sequence at about6.25–12.5changes/sec. Existing shared pens require an art/pen-usage audit within the band: avoid glowing shore rocks. Colour modulation creates shimmer, not directional flowing pixels. No whole-panorama cycling, broad flashes or foreground-water change. Copper list edits occur only in safe prepared lists with palette restored for HUD. Small tables/list growth rather than new full bitmaps; precise cost measured after prototype.
2. One representative distant waterfall:4prebuilt frames,12.5Hz, about16x48native pixels with a slightly broader foam foot. Suggest at most2visible patches and no simultaneous update bursts.3planes at word-aligned16x48 =288bytes/frame,1152bytes/four frames, before padding/background restore and staging. Assets in Fast; DMA source staging in Chip. Copy complete authored rectangular background patch instead of per-pixel particles or masked droplets. Restore original on leave if using a reusable moving window. Account for separate source/guarded display copies; no CPU writes to displayed Chip RAM. Never allocate1552x208full frames for this.
3. Optional very occasional2–3distant birds as one small silhouette group,3poses, roughlyevery20–40seconds in a calm stretch. Silhouette and location must distinguish from targetable Spillwings. Own visual path, not enemy pool/collision/AI. Hardware sprite is a hypothesis pending channel/bank/priority audit; otherwise a small background patch requires restore and movement cost. No guaranteed free slots/performance claim.

## Alternatives

Small distant steam vent:8–12pxwide/16–24pxhigh,4frames,slow local pulse behind turbine ruins. Strong thematic fit; choose instead of another waterfall rather than accumulating effects.

Cloud drift or separate mist parallax: more involved. Current art overlaps cloud, mountain and tree silhouettes on the same scanlines. Changing horizontal scroll by a raster band moves everything in that band and can shear mountains; no safe generic independent layer. Real independent movement requires separated artwork plus compositing or deliberately redesigned non-overlapping bands. Large translucent mist is particularly expensive with limited palette and shared Chip bandwidth. Defer.

Water-only line scroll could be a later Copper experiment after isolating a clean horizontal lake strip. It must preserve PF1fine-scroll bits, fetch guards, palette restores and shore alignment. Defer until a small colour-based proof is worthwhile.

## Test gate / budget policy

First a native-size animation preview, then one isolated optional runtime candidate versus byte-preserved baseline. Test at same camera/motion on busy second-ferry upper route with several Spillwings, checkpointrepeat, and governor finale. Log current memory/free-largest allocations. Keep music and all gameplay identical. Instrumentation must be compatible with music (prior CIA conflict means do not simply enable old broad profiler). No fixed FPS-cost estimate before native evidence. Reject if repeat matched tests add deadline misses/hitches. Prefer global bounded effect cadence over load-dependent popping. No decorative animation per-frame50Hz requirement; gameplay stays50Hz target. Maximum2visible waterfall/steam patches initially, no extra full bitmap or extra bitplane. Static fallback acceptable; this is not a release blocker.

## Primary reference

Commodore Amiga Hardware Reference Manual, Chapter2 (mirrored): https://www.amigarealm.com/computing/knowledge/hardref/ch2.htm . Copper WAIT/MOVE enables raster-timed palette/register changes but consumes bus cycles when executing; not literally free. Chapter6 DMA discussion search reference: https://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node012B.html (search excerpt available, full fetch failed during research). Chip bus contention matters alongside CPU instruction cost. Recommendations above are project-specific engineering proposals, not measured results from these manuals.
