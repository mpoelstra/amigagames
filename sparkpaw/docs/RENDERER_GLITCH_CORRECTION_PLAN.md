# Sparkpaw renderer-glitch correction — completed checkpoint

Status: completed through Stage 5L and promoted as `0.6.0-alpha.43`.
The broad rolling-renderer corruption is fixed in supplied FS-UAE/68030,
real-A1200/68030 HD, real-A1200/68030 ADF and Analogue Pocket ADF testing.
Performance is not accepted. One narrow ground/HUD boundary flicker remains a
separate Copper-split follow-up.

## Accepted architecture

- Two complete Copper lists are patched only while inactive and atomically
  published at a fixed PAL boundary.
- Two hidden FRONT16 targets own independent Bob histories. CPU and Blitter
  never modify the displayed target.
- Each target is a logical 512-pixel ring repeated three times across a 1536px
  physical stride. Only entering 16-pixel columns and changed dynamic regions
  are maintained; no complete viewport shift or copy occurs.
- REAR8 parallax, all authored assets, palettes, animation, HUD and gameplay
  contracts remain intact.
- Gameplay uses the Stage 5G FMODE1 `$20..$d0` 48-byte fetch with a real
  four-byte left guard. The fixed HUD restores `$30..$d0` and FMODE1.
- Sparkpaw's unchanged 48x48, 15-colour, 62-frame art is transparently padded
  to one 64-pixel attached AGA sprite pair on channels 0/1. Wide streams use
  the proper eight-byte POS/CTL blocks, eight data words per row and explicitly
  eight-byte-aligned Chip-RAM staging.

## Supplied acceptance and rejection boundary

- Stage 5L FS-UAE/68030 HD: no corruption, glitches or flicker; 2,163/2,163
  one-field intervals, 50.00 FPS and zero ownership violations.
- Alpha.43 real A1200/68030 HD: launches and is broadly graphically stable;
  performance is rejected, sound events can repeat or be missed under load,
  and more free Chip RAM appeared necessary than before.
- Alpha.43 physical ADF on the real A1200/68030: launches and retains the same
  broad graphical stability; performance is rejected.
- Alpha.43 ADF on Analogue Pocket: launches without the former broad graphics
  corruption; performance is rejected.
- All tested paths retain a small intermittent disturbance at the fixed
  ground/HUD boundary. `Renderer alpha43-rejected-real-a1200-hd-performance-
  and-hud-seam-flicker.MOV` records it on real hardware.

Do not convert these results into stock-68020 acceptance. FS-UAE/68020 Stage
5L measures 26.38 effective FPS: 136 one-field, 337 two-field and 78 three-field
intervals over 551 intervals, with 270 composition wraps.

## Lessons learned

1. Atomic Copper publication alone is insufficient; displayed-memory writes
   also require inactive target ownership.
2. Full compact-viewport copies and recenter shifts are too expensive or hitch.
   A repeated logical ring makes work proportional to newly exposed columns.
3. Restore history and draw coordinates must remain in the same physical
   domain. Mixing world/ring coordinates caused trails for every moving Bob.
4. HUD setup must begin immediately after the preceding DDFSTOP. Disabling
   BPLCON0 at the split corrupts the HUD and is rejected.
5. AGA fetch width, DDF origin, fine-scroll phase, physical guards and modulo
   form one contract. Changing one in isolation caused offsets, strips, jumps
   or black displays.
6. Synthetic linear renderbench success does not prove a production rolling
   ring. FMODE3 passed isolated calibration but failed with alternating targets.
7. Earlier bitplane fetches can steal later sprite slots. `$20` and `$28`
   produced coherent playfields but corrupted the former six-channel actor.
8. Fewer sprite channels do not automatically mean less DMA, but placing the
   unchanged actor in the earliest wide attached pair solved component loss and
   leaves later channels available for measured future experiments.
9. Never mask a fetch/HUD defect by editing ground or HUD artwork. Boundary
   fixes belong in Copper timing and ownership.
10. Diagnostic cadence must count PAL fields and effective FPS. Raster-line
    timestamps alone alias after wrap and previously hid missed frames.

## Rejected branches retained only as history

- Stage 3 one-visible-Strider budget was unnecessary for correctness.
- Stage 4A/4B full copies and physical recentering: cadence loss/hitches.
- Stage 4C coordinate mismatch: universal Bob trails.
- Stage 4F BPLCON0 split blanking: complete HUD corruption.
- Stage 5E insufficient left coverage and Stage 5G six-channel sprite loss.
- Stage 5H–5J FMODE3: isolated proof passed, production origin jumped.
- Stage 5K first artifact malformed its Copper MOVE; corrected Stage 5K2 still
  lost six-channel Sparkpaw components.

Detailed chronology and exact evidence remain in `docs/DEVELOPMENT_HISTORY.md`.

## Remaining work, routed elsewhere

1. Fix only the narrow HUD-boundary timing regression. Stage 5L adds an FMODE
   restore MOVE to the already tight pre-HUD Copper setup; verify scheduling
   rather than reopening ring or sprite geometry.
2. Continue measured cadence work in `PERFORMANCE_68020_PLAN.md`, using the
   supplied 34.5 MHz 68030 as the allowed minimum if stock 68020 cannot reach
   the target without visual/gameplay loss.
3. Measure prepared and steady Chip free/largest blocks on real alpha.43 HD.
4. Instrument logical audio-event requests separately from Paula playback;
   repeated and absent effects under load are not yet isolated.
