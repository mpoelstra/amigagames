# Sparkpaw ADF load optimization plan

Current alpha.2: corrected two-ADF functionality and styled INSERT prompts
are user-approved. Measured Gotek speed benefit remains unproven. See
RELEASE_0_7_0_ALPHA_2.md and CHECKPOINT_ALPHA2_LESSONS.md.

## New video evidence — 2026-09-05

The user's brother supplied `testresults/Phase 6C.10-observed-gotek-loading.mp4`
(original name `gotek-loading.mp4`), with matching TXT sidecar. The 83.26-second
clip visibly carries an alpha.68 image label and shows repeated wide OLED
track changes, including returns to 39/40; the ready menu is visible at the
end. It is not a complete cold-boot timing measurement or proof of exact ADF
hash/firmware/CPU. See GOTek_VIDEO_EVIDENCE.md for the evidence boundary.

The protected local alpha.68 image places root metadata at block880/cylinder40
and S/assets/runtime directory blocks at866-868/cylinder39. This makes metadata
lookup/cache behaviour an additional plausible contributor. The first layout
proof must model directory/header access as well as payload order. Video alone
does not identify block reads or prove cache misses, fragmentation or time saved.
No game/ADF/configuration was changed during analysis.


Status: reactivated for offline implementation by the user on 2026-09-05 after
parking frame-performance work. HD 0.7.0-alpha.1 goes first; disk implementation
and native loading acceptance remain separate. See MULTI_ADF_CAMPAIGN_PLAN.md
for actual two-disk prototype capacity, loader/packing details and protected
alpha.68 physical layout. The user-approved no-intro pair combines lossless
compression and source-ordered layout; it is not the isolated Stage B experiment
below. Native timing and Gotek benefit remain unmeasured.

## Observation and scope

A Gotek/FlashFloppy user reported that its OLED track indicator repeatedly
moves between early and late disk tracks while the current ADF loads. The load
time is also subjectively long. This can indicate that files are physically
laid out in a different order from Sparkpaw's startup read order; it does not
by itself prove fragmentation or a defective filesystem.

This plan targets only ADF seek locality and startup throughput. It must not
change art quality, decoded asset bytes, gameplay, renderer behavior, HD asset
loading, memory ownership or the accepted DOS1/FFS boot contract.

## Current runtime order to verify

Capture the exact DOS opens and compare them with the physical block order in
a freshly generated ADF. The expected high-level sequence is:

1. `Sparkpaw` from `S/startup-sequence`;
2. title, LOADING image and CHARGING status patch;
3. foreground, rear, player, beetle and Strider graphics;
4. HUD base and counter graphics, then the world diamond;
5. collision data;
6. energy, hurt, enemy, Strider, jump, collect and water audio.

The release builder currently populates a fresh filesystem independently of
that runtime sequence. First prove whether this causes the observed long seeks;
do not infer it from filenames or the Gotek display alone.

## Stage A: measure an unchanged baseline

- Emit a release-time manifest containing every file's byte size, FFS block
  range, track range and fragmentation count.
- Trace open/read order in a dedicated debug ADF without adding diagnostics to
  production builds.
- Calculate cumulative track travel and identify each backward/long seek.
- Time cold boot to title, title to LOADING, LOADING to CHARGING and CHARGING to
  gameplay on the same Gotek configuration. If possible, repeat on a physical
  floppy; emulator wall-clock time is supporting evidence only.
- Use the original alpha.68 ADF as the current protected byte/layout baseline;
  preserve alpha.41 evidence as historical context, not a fabricated current test.

## Stage B: layout-only proof

Build a fresh DOS1/FFS image whose file creation order follows measured runtime
consumption. Keep the executable and every asset byte-identical. Put files that
are read together contiguously where FFS permits it, and keep startup files near
the boot area. Generate the same block/track manifest and compare:

- extracted files and decoded SPR1 payloads byte-for-byte;
- total occupied/free blocks;
- cumulative track travel and fragmentation;
- the four cold-start timing stages;
- boot, title, loading transition and complete gameplay on ADF.

Accept this stage only when seek travel and/or measured load time improves with
no visual, audio, memory or gameplay regression. A faster Gotek result alone
does not establish physical-floppy or HD behavior.

## Stage C: only if layout is insufficient

Prototype one sequential ADF-only asset container with a compact index of
offsets, sizes, formats and CRC32 values. Read forward through the existing
small staging buffer and decode directly into the same final allocations.
Retain per-asset integrity checks and deterministic failure reporting. The HD
package should keep its ordinary files unless separate measurements justify a
shared format.

Reject this escalation if added code, buffers or decoding raise Chip-RAM peak,
damage startup reliability, or save less time than the layout-only build.

## Acceptance gate

The final evidence must include generated layout reports, byte/decode parity,
bootable release validation and supplied Gotek or real-floppy timings. Do not
claim that track-head motion affected the alpha.40/alpha.41 HD Chip-RAM issue:
the ADF observation is a separate deferred loading optimization.
