# Level-complete tally animation storyboard

Status: presentation proposal only. Timings are design targets, not runtime or
hardware verification.

## Reference observation

The local `sparkpaw/testresults/thundercats-level1.mov` was inspected across
00:00–00:43.87, with consecutive samples around the results transition. The
level remains visible at 00:41.20, passes through black at 00:41.30–00:41.40,
then shows the results screen. At 00:41.50 its time value is visibly at `7`
while the score reads `7850`; at 00:41.60 the transfer has completed and the
score reads `8450`. Subsequent frames show a brief alternating/pulsing logo
treatment. This describes the recording only and is not Sparkpaw evidence.

The normal-speed YouTube reference supplied by the user was also inspected from
02:52 through approximately 02:55 on 25 August 2026. It shows the intended
sequence more clearly than the accelerated local capture: `KILL BONUS` counts
down while `SCORE` rises in 100-point steps (visible samples include kill `17`
with score `6550`, then kill `5` with score `7750` roughly one second later).
The time category remains fixed until the kill transfer completes. The reusable
principle is sequential category transfer at roughly arcade-tick speed, not an
instantaneous results reveal. The external reference does not establish
Sparkpaw timing or hardware behavior.

## Smallest Sparkpaw-specific sequence

Use a readable sequence that normally completes in about 4–6 seconds. Preserve
the reference principle of visible decrement/increment steps, but cap every
category so large Sparkpaw totals cannot create a long wait.

| Phase | Suggested duration | Presentation |
| --- | ---: | --- |
| Arrival | 12 PAL fields / 0.24 s | Gameplay fades to deep navy; the Stormstone ring draws one restrained cyan arc. |
| Header | 18 fields / 0.36 s | `LEVEL COMPLETE` settles into its frame; Sparkpaw changes to the victory hold. |
| Enemy row | about 5–8 fields per unit, bounded to 2 s | Enemy count decrements while fixed-value points enter the total, then the row locks with one cream flash. |
| Diamond row | about 3–5 fields per unit, bounded to 2 s | Diamond subtotal transfers in coarser chunks so a large collection count does not dominate the sequence. |
| Time row | about 3–5 fields per bonus second, bounded to 2 s | Remaining bonus seconds count down while the total rises; long bonuses accelerate after the first readable steps. |
| Total confirmation | 24 fields / 0.48 s | A cyan pulse travels down the central studs, the total enlarges or brightens for two beats, then returns to its stable final keyframe. |
| Input hold | minimum 50 fields / 1.0 s | Final values remain completely still and readable before fire can continue. |

## Counting behaviour

- Keep the underlying arithmetic instantaneous and deterministic; only the
  displayed transfer is animated.
- Allow fire to accelerate a running category after a short safety delay, but
  never skip the stable final state.
- Use one short tick for every visible transfer step and one stronger original
  confirmation sound per completed row. Audio ownership is deferred; this is
  not permission to change the current Paula channel contract.
- Count in useful chunks rather than one point at a time. A `36 x 100` time
  bonus can decrement the visible seconds while adding 100 to the total per
  step, accelerating after the first few steps.
- Prevent score overflow before animation starts and render the final arithmetic
  from authoritative values, not accumulated animation increments.
- If a category is zero, show `0` briefly and lock the row; do not omit it, so
  the screen layout remains stable.

## Proposed tally sound language

The counting sound should be a tiny original electronic/mechanical `tik`: a
bright cyan-energy transient with a very short tonal tail. It should feel like
points entering a Stormstone mechanism, not like a menu click, coin, weapon or
collectible sound.

Suggested sound family:

| Event | Sound | Intended function |
| --- | --- | --- |
| Every visible count step | 20–35 ms dry electrical/mechanical tick | Makes each decrement/increment pair feel connected. |
| Final step of one row | 70–110 ms higher two-note lock/chime | Confirms that the category is settled before the next begins. |
| Final total confirmation | 180–260 ms fuller Stormstone resolve | Rewards completion without becoming a long fanfare. |
| Accelerated/skipped transfer | A bounded faster tick train, then the normal row lock | Preserves feedback without stacking many samples or delaying input. |

Pitch may rise very slightly within a row—at most three or four discrete pitch
steps—while volume stays constant. Reset the pitch when the next category
starts. This creates momentum without requiring a separate sample for every
number. The final-total resolve should share the same tonal identity but add a
low body and clean cyan-energy top, making it clearly distinct from ordinary
count ticks.

Playback rules for a future audio checkpoint:

- trigger a tick only when the displayed count and score visibly change;
- never queue every missed tick if animation acceleration skips values;
- permit at most one active tally tick, replacing the preceding tick cleanly;
- keep row-lock and final-total sounds non-overlapping with the tick train;
- do not reuse plasma, diamond-pickup, enemy-hit or menu sounds;
- define Paula channel ownership only after gameplay has stopped and the
  results-screen audio/music layout is known;
- generate and review the sound separately before integrating it with runtime
  code or making any hardware-performance claim.

## Visual motion budget

Animate values and a small number of cyan circuit accents. Keep the background,
large frame and Sparkpaw pose mostly static. This preserves readability and
gives a future implementation a bounded set of changing regions without making
any current renderer-performance claim.
