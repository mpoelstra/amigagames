# Stormrail interlude results contract

> Current status (2026-09-05): The isolated results gate was accepted and integrated. Campaign builds add BACK TO TITLE and restore carried vitals on replay according to CAMPAIGN_LOOP_CONTRACT.md. The isolated-only prompt/fresh-vitals scope below must not override the campaign contract. See [status index](CURRENT_STATUS.md).

Status: retained isolated results contract, extended by the integrated campaign contract.

The Gate-6 `COMPLETE` phase is the only results trigger. It occurs after the
120-HP Harrier is dead, hostile and player fire is retired, the 48-tick gate
opening has finished and the Skimmer has automatically passed x=328. Distance
remains exactly 15500. Arrival, combat or opening may never enter results.

The first results version reuses the Level-1 presenter and assets without a
visual variant: the same 320x256 six-plane composition, rows, fonts, colours,
`ENEMIES x20`, `DIAMONDS x5`, `TIME x10`, `SCORE`, tally order, acceleration,
Fire debounce, tally sound and selectable `REPLAY LEVEL` prompt. There is no
Stormrail label, campaign total, `CONTINUE` or new results art.

## Completed-run source and score

One immutable `StormrailResultsSnapshot` is finalized at `COMPLETE`. Its enemy
defeats, collected diamonds, monotone `elapsedFields` and live section score
come from that Stormrail run, never from Level 1. The live score already
contains every section award, including the Harrier's 320 points through award
ID 60. Results never mutate live score or add the Harrier award again.

The complete interlude's documented par is 150 seconds (7,500 PAL fields),
matching the intended roughly 2.5-minute route rather than the finale-local
timer. Whole elapsed seconds are floored exactly as on Level 1. Bonus seconds
are `max(0, 150 - elapsedFields / 50)` and are worth 10 points each, so the
bonus is bounded to 0..1,500 and can never be negative. Life loss and finale
retry do not reset `elapsedFields`. Finalization is idempotent: bonus and total
are calculated once. Visible `SCORE` is only the completed Stormrail live score
plus that one time bonus; campaign banking is outside this gate.

## Resident replay

After tally completion, Fire uses the existing debounce, fades the complete
score display to black and invokes the already-resident Stormrail reset. It
starts at departure/boarding without Level 1, loading presentation, disk icon,
Workbench or any asset load.

Before the new gameplay Copper is published, replay resets the section score,
elapsed clock and result snapshot; enemy/formation state and awards; the
Debris5.2 cursor, objects and awards; the complete Harrier/finale lifecycle and
120 HP; lives, six-unit health and invulnerability; diamond remainder and run
count; pickup/award bits and life awards; every player/hostile projectile; and
keyboard/joystick Fire history. No value or one-shot award from the discarded
run may carry into or be granted again by the reset itself.

Campaign continuation, score banking, Level 2, ADF/multidisk, release identity
and SemVer remain outside this contract.
