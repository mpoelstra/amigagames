# Campaign loop contract

> Current status (2026-09-05): This contract is implemented in the 0.7.0-alpha.2 campaign. See CURRENT_STATUS.md for user evidence boundaries; the user approved HD and ADF; new campaign WHDLoad is pending native validation. See [status index](CURRENT_STATUS.md).

Status: user-approved HD and ADF flow; WHDLoad/hardware gates remain separate.

## Visible flow

```text
TITLE -> existing ready/start screen -> LEVEL 1
LEVEL 1 results -> REPLAY LEVEL -> fresh resident LEVEL 1
                -> CONTINUE     -> black/loading-bounded STORMRAIL load
STORMRAIL results -> REPLAY LEVEL  -> fresh resident STORMRAIL departure
                  -> BACK TO TITLE -> large title -> ready/start -> fresh LEVEL 1
LEVEL 1 or STORMRAIL -> ESC -> presentation rebuild -> ready/start
```

The score composition, four tally rows, order, acceleration, tally sound and
Fire debounce remain the accepted six-plane presenter. The only new score-art
content is menu text drawn in that presenter's existing native font and colour.
There is no dead `CONTINUE` action.

## State and one-shot rules

- `CONTINUE` accepts the completed Level-1 result exactly once and records one
  immutable post-Level-1 snapshot: lives, remaining health in half-heart units,
  the current diamond meter and recovered Lightning Core.
- Stormrail restores those three player vitals. It begins with fresh
  section-local score (always zero), time, enemies, diamond tally, awards,
  pickups and projectile/input history. Carried diamonds are the live meter,
  not pre-awarded Stormrail tally diamonds.
- Stormrail `REPLAY LEVEL` keeps the accepted post-Level-1 snapshot but resets
  every Stormrail-local value and restores the same carried vitals. It performs
  no asset load and no second award.
- `BACK TO TITLE` clears the campaign snapshot. Starting again from the title
  therefore creates a genuinely fresh Level-1 run.
- Escape during gameplay abandons the active run, clears its campaign snapshot
  and rebuilds the normal Level-1 preload without replaying the story intro.
  It stops at the ready/options screen; gameplay never resumes until a fresh
  `START GAME` confirmation. The start selector defaults to `STORM RUINS`.
- A held Fire cannot select twice. Direction must return to neutral before a
  second menu move, and Fire must be released after tally skip before confirm.
- Cross-section load failure remains on a controlled black/loading display and
  exits through the existing fatal-load path; it may not publish partially
  prepared gameplay or mutate the accepted post-Level-1 snapshot.

## Protected renderer/runtime baseline

Level-1 replay remains resident. Stormrail replay remains resident. Only
Level-1 `CONTINUE` and Stormrail `BACK TO TITLE` cross an asset boundary. No
pool grows, no per-frame allocation is introduced, and the accepted Stormrail
distance/finale/Harrier/award/HUD/ownership/cadence contracts remain unchanged.
# Section isolation and future media split

The complete per-file ownership and future-volume rules live in
`CAMPAIGN_ASSET_OWNERSHIP.md` and are enforced by host tests.

The campaign executable may contain both implementations, but the active section
selects a disjoint gameplay asset set before `assetsLoadGameplay`: Level 1 uses
the byte-exact accepted alpha.68 front, rear and player sheets; Stormrail uses
its own front, rear and extended cockpit-player sheet. Stormrail-only graphics conversion
sources are not loaded, converted, restored, drawn or copied through rolling
history while Level 1 is active. This is not a claim that every Stormrail byte
is absent: the production audit records 9,812 bytes of Harrier Chip samples
loaded by shared audio setup, plus resident code/static data. Those capacity
findings are not proven performance causes. The Level-1 Copper keeps the accepted alpha.68
rear-palette wait positions.

This is the required logical boundary for a later multi-ADF layout, but it is
not itself a multi-ADF implementation. A future disk build must put a media
manifest/loader above this section selector (including disk-change UI and packed
asset names); it must not reintroduce compile-time world selection inside the
renderer or game state.

## Disk-only adaptation, user-approved for alpha.2

Cold section loading checks the required disk in DF0/DF1 before gameplay reads.
An already-present disk 2 skips INSERT. Otherwise the shared loading picture
shows INSERT DISK 2 (or 1 on return); only its status strip changes. Current-level
replay remains resident. Wait for INSERT before swapping in a single drive.
See MULTI_ADF_030_TEST.txt for continuation, direct Stormrail selection, replay,
Esc, title return and two-drive tests. Existing HD flow and release bytes remain
unchanged; a failed read uses cleanup, never partial gameplay publication.
