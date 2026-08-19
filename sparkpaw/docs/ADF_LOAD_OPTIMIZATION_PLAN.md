# Sparkpaw ADF load optimization plan

Status: deferred planning. Do not implement this while the real-hardware Bob
glitch investigation or stock-68020 performance work is active.

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
- Preserve the original alpha.41 ADF as the byte-level and timing baseline.

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
