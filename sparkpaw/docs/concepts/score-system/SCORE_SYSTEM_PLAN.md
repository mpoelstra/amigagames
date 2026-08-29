# Sparkpaw HUD right-panel feature research

Status: score/results accepted in supplied FS-UAE/68030 and FS-UAE/68020 HD
testing as alpha.64 Phase 6C.6. Core-only completion and non-farmable diamonds
are accepted in supplied FS-UAE/68030 HD testing and promoted as alpha.65 Phase
6C.7. ADF, WHDLoad, alpha.65 FS-UAE/68020 and real-hardware runtime acceptance
remain pending.

## Implemented candidate contract

- unique authored enemy spawn: 20 points; a respawn cannot score twice;
- diamond: 5 points, independently of the existing extra-life counter;
- death/restart preserves score and elapsed time;
- diamonds already collected in the current level attempt remain absent after
  water, dry-gap and life-loss restarts, preventing repeat score farming;
- hidden PAL field counter, 120-second par and 10 points per saved second;
- four-digit HUD display clamps visually at 9999 while the internal total is
  32-bit;
- Core completion stops gameplay simulation, frees the gameplay renderer and
  loads a separate double-buffered 320x256 six-plane results presenter;
- Core collection is the sole Level-1 completion trigger; reaching or crouching
  against the solid right world boundary never restarts or completes the level;
- tally rows redraw only their small hidden numeric strips, play one bounded
  tick for every visible transfer and allow Fire to finish the active row.
- the four cyan row labels are regenerated as native 5x7 pixel faces on the
  same hard-pixel grid as the dynamic yellow values; no reduced concept-art
  lettering remains inside the table;
- after the final tally, a centred native `PRESS FIRE TO CONTINUE` prompt is
  revealed; the resident replay installs its gameplay Copper list only just
  after PAL frame wrap to avoid executing a partial lower-screen list;
- final Fire fades the result presenter fully to black before Exec/DOS reloads
  Level 1. During that full reload, the existing `LOADING` composition remains
  visible so the slower 68020 renderer rebuild cannot resemble a black-screen
  hang; completion then enters a fresh attempt without exposing the system
  View. A
  supplied 60-fps recording rejected keeping the custom score display visible
  during loading because graphics/Blitter preparation leaked one corrupted
  pre-fade frame. This
  resident replay is an explicitly temporary, Level-1-only convenience while
  the game has one playable level. A multi-level build must replace it with a
  level-flow/state-machine transition that selects and loads the next level.

## Existing contract and available space

The fixed 320x48 HUD already communicates the three persistent quantities that
exist in gameplay:

- current health as three hearts / six half-heart units;
- current attempt stock as `x1` through `x9`;
- diamonds as `00` through `49`, with the fiftieth awarding one attempt.

The rightmost framed panel was deliberately authored as an empty region for a
later selected status. In the current eight-colour preview it occupies roughly
the final 60 pixels of the bar, with a substantially smaller dark inset. It is
therefore suitable for one strong icon, a short meter or at most two compact
digits, but not explanatory text or several simultaneous values.

Any later implementation should preserve the separate 48-line HUD, its
double-buffered/event-driven update model, three-plane/eight-colour palette,
fixed Copper split and existing health/lives/diamond panels. A new display
should patch only when its logical value changes; permanent per-frame HUD
animation would need separate performance evidence.

## Revised recommended direction: live score plus level-complete tally

The preferred direction after reviewing the supplied ThunderCats comparison is
a score system. The reusable principle is not its logo, typography, values or
layout, but the separation between immediate play feedback and a short
post-level reward summary:

- ordinary enemy defeats and collectibles add to a visible running score;
- elapsed or remaining level time is tracked separately;
- level completion presents combat, time and total-score results on a dedicated
  screen.

In the local `thundercats-level1.mov` reference, inspected across 00:00–00:43.87
and as consecutive half-second samples from 00:34 onward, the gameplay HUD shows
a countdown and running score. Around 00:39–00:40 it changes to a dedicated
summary containing a kill multiplier, time multiplier and total score. This is
reference observation only; it is not Sparkpaw test or performance evidence.

For Sparkpaw, use the empty right panel for the **running score only**. Track
time internally and reveal it on the level-complete tally. This avoids forcing
both a readable multi-digit score and `MM:SS` into one roughly 60px frame, and
it preserves Sparkpaw's more exploratory pacing instead of placing a visible
countdown over the whole level.

### Proposed score categories

- **Enemy score:** award a fixed value only when an enemy actually completes
  its defeat transition. Beetles and Striders may have different values based
  on risk; respawning enemies require an anti-farming policy before their
  repeated kills can score indefinitely.
- **Collectible score:** every diamond may add a small immediate value while
  retaining the existing `00..49` life-award counter. Score and diamond count
  remain distinct: one is cumulative reward, the other is progression toward
  an extra attempt.
- **Kill bonus:** on level completion, summarize eligible defeated encounters.
  This can be either `kills × value` or a percentage/completion bonus. Avoid
  accidentally awarding the same points twice: if enemy points enter the live
  score immediately, the results screen should present that subtotal rather
  than add it again, unless an explicitly separate all-enemies-clear bonus is
  intended.
- **Time bonus:** convert performance against a level-specific par time into
  points, then add it once at completion.
- **Total score:** carry the completed result across levels; define separately
  whether a lost attempt preserves the score and whether game over submits it
  to a future high-score table.

### Recommended time model

Measure elapsed real play time, but do not show it continuously in the HUD.
For a PAL target the logical unit can be display fields accumulated at 50 Hz,
converted to whole seconds for the results screen. Do not derive time from the
number of completed gameplay updates: missed deadlines or a later update-rate
change would otherwise make a slow machine produce a different time bonus.

Pause the timer during title/loading, a future pause state and the complete
level-results transition. Decide explicitly whether death/restart time remains
part of the level attempt; the recommended arcade rule is that it does, because
death then has a score cost without erasing already earned points.

A friendly time formula is:

```text
time_bonus_seconds = max(0, par_seconds - elapsed_seconds)
time_bonus = time_bonus_seconds * level_time_multiplier
```

For example, a 120-second par completed in 84 seconds leaves 36 bonus seconds.
At 10 points per second that yields 360 points. Going over par gives zero
time bonus, not a failure. This rewards mastery without turning normal play
into a hard countdown. Per-level gold/silver/bronze time bands are an even
simpler alternative if exact seconds feel too dominant.

### HUD presentation options

1. **Selected:** four compact digits in the right panel. Supplied native-scale
   review rejected five digits because they crowded the right frame. The point
   economy was reduced rather than merely hiding the fifth digit.
2. **Score on two rows:** `SC` or a small storm-bolt/paw icon above/beside four
   digits. This is clearer than squeezing a literal `SCORE` label into the
   panel.
3. **Not recommended:** alternate score and time in the same panel. The meaning
   changes without player input and makes quick reading unreliable.
4. **Optional timed mode:** replace score with `M:SS` only in a deliberately
   timed challenge. This should be a mode-specific HUD meaning, not normal
   level behaviour.

The score patch should be event-driven: enemy defeat, collectible pickup and
level-complete bonus settlement. Internal time accounting need not redraw the
HUD every second if time is not displayed.

## Earlier alternative: contextual objective/status panel

Use the panel as one context-sensitive slot whose default state communicates
level progress or the next meaningful objective. This is a better fit than
inventing a fourth permanent currency: the roadmap already calls for
checkpoint/progression state, level completion, game over and eventually a
charged shot, while no additional collectible or inventory system is yet part
of the accepted game.

Suggested priority within that slot:

1. **Temporary critical state** — a flashing-but-bounded danger, boss or
   objective-complete symbol may temporarily take precedence.
2. **Current objective** — for example a small Stormstone/citadel emblem with
   `1/3`, a remaining-target count, or a compact destination arrow when a level
   has an explicit goal.
3. **Checkpoint/progress state** — otherwise show a lit checkpoint emblem or a
   small segmented route indicator.
4. **Neutral state** — before progression exists, retain the authored empty
   inset rather than displaying a meaningless placeholder.

This makes the panel extensible without changing its meaning unpredictably:
it always answers “what matters next?” Priority changes should occur only on
events, with a short, deterministic temporary hold when feedback must remain
visible.

## Candidate features

| Candidate | What it adds | Fit in the panel | Dependencies / caution | Recommendation |
| --- | --- | --- | --- | --- |
| Checkpoint / route progress | Shows the last activated checkpoint or 3–5 level segments | Excellent; icon or tiny segmented strip | Requires real checkpoint and progression semantics first | Best first permanent use |
| Current objective / remaining targets | Stormstone pieces, switches, prisoners or required enemies remaining | Excellent if limited to one icon plus two digits | Only use for authored mandatory goals; avoid turning every enemy into a counter | Best contextual extension |
| Charged-shot meter | Displays charge fill and ready state | Excellent as a short horizontal or radial meter | Charged shot is planned but not implemented; must not imply ammunition | Strong once that move exists |
| Boss/miniboss health | Replaces the normal panel during a major encounter | Good with a compact segmented bar | Needs an actual boss/miniboss and a clear enter/leave rule | Good temporary override |
| Ability/equipment icon | Shows the currently selected gauntlet mode or traversal tool | Good for one large readable icon | Worthwhile only if the player can actually switch abilities | Reserve for later systems |
| Keys / local quest item | Icon plus `0–3` for a level-local gate item | Good | Risks duplicating diamonds unless it drives distinct progression | Use only when level design requires it |
| Level timer | Minutes/seconds or a countdown | Marginal; digits fit but dominate the tiny panel | Adds pressure and frequent HUD updates; conflicts with exploratory pacing unless a timed mode exists | Not recommended for normal play |
| Score | Immediate reward for enemies, collectibles and completion | Good if native review proves four or five digits readable | Requires scoring, persistence and respawn anti-farming rules | Preferred direction |
| Enemy radar / direction arrow | Warns about an off-screen threat or destination | Visually compact | Can reduce discovery and becomes noisy if updated continuously | Use only for a specific authored encounter |
| Combo / multiplier | Rewards rapid combat or collection chains | Technically compact | Introduces a new scoring loop unrelated to the current platform-adventure roadmap | Defer |
| Portrait/status face | Changes expression for hurt, danger or victory | Visually attractive | Duplicates the existing portrait and health hearts; animation costs attention without new information | Low priority |
| Debug/FPS/memory indicator | Engineering telemetry | Fits numerically | Not player-facing and performance diagnostics belong to another workstream | Debug builds only, never the production meaning |

## Suggested staged product decision

### Stage A — scoring contract and native HUD proof

Define point values, respawn policy, death persistence, numeric ceiling and the
level-results arithmetic. Concept four-, five- and, only if credible, six-digit
variants in the exact complete 320x48 HUD. Do not implement gameplay state in
this art/readability decision.

### Stage B — event-driven live score

Add score only for existing, unambiguous events: final enemy defeat and diamond
pickup. Patch the inactive HUD buffer only when the value changes. Keep internal
time and level completion outside this first proof.

### Stage C — elapsed-time accounting and results screen

Introduce field-based elapsed time with explicit pause/restart semantics and an
original Sparkpaw results screen. Show combat/collection subtotal, time bonus
and final total without copying the reference screen's art or typography.

### Stage D — persistence and optional modes

Define between-level accumulation and eventual game-over/high-score behaviour.
Only then consider a timed challenge, checkpoint indicator, boss bar or ability
override for modes that need a different panel meaning.

## Acceptance questions for a future implementation checkpoint

- Can the information be understood at native 320x256 display scale without a
  legend?
- Does it communicate an implemented mechanic rather than promise a nonexistent
  one?
- Is there exactly one unambiguous meaning at a time and a documented priority
  when temporary states override the default?
- Are changes event-driven and bounded within the existing inactive-buffer HUD
  publication model?
- Does the original empty state remain available for levels or modes that do
  not use the feature?
- Are persistence rules explicit across damage, life loss, checkpoint reload,
  level completion and new game?
- Has visual and timing validation remained separate from the active renderer
  and performance baseline work?

## Recommendation

Use the panel for a compact **live score** and create a separate original
Sparkpaw level-complete tally for enemy/encounter results, time bonus and total
score. Track elapsed PAL display time internally; do not show a normal-play
countdown. Retain the contextual objective/status idea as a fallback or
mode-specific override, not as the default meaning.

Before implementation, decide four game-design contracts: exact enemy and
diamond values, whether respawned enemies can score repeatedly, whether death
preserves elapsed time and points, and whether the displayed score needs four,
five or six digits. Prototype those digits at native 320x48 HUD scale before
choosing the numeric ceiling.

## Feasibility and performance boundary

The complete direction is feasible on the existing PAL A1200/AGA target, but
only if gameplay HUD work and the level-complete presentation remain separate.
This is an architectural feasibility assessment, not measured FS-UAE or
hardware acceptance.

### Gameplay HUD score

The right panel shows a four-digit live score using the existing
three-plane/eight-colour HUD language. Do not create complete HUD bitmaps for
every possible score. Use a compact fixed-width digit atlas and patch only the
digits that changed into the inactive HUD buffer, following the existing
health/lives/diamond model.

Score changes are event-driven—enemy defeat and collectible pickup—not a
per-frame animation. An internal 32-bit score may exceed what the small HUD can
display. The selected policy clamps the HUD at 9999 while retaining a 32-bit
internal total. The dedicated results screen can show more digits than the HUD.

An elapsed-time accumulator is computationally negligible, but it should use
PAL display-field progression rather than completed gameplay updates. Because
normal play does not display the timer, it causes no per-second HUD redraw.

Expected gameplay cost if implemented this way:

- one small integer addition on a scoring event;
- one dirty-state comparison in the existing HUD update path;
- a few small digit Blits only when the visible score changes;
- no extra gameplay bitplane, Copper section, Bob, continuous animation or
  per-frame full-HUD copy.

### Level-complete screen

The v4 concept is a composition target, not a claim that its current source
pixels or colour count can be loaded unchanged. A production version can be
reduced to the existing title/loading presentation class: PAL 320x256, indexed
AGA art, broad pixel clusters and a deliberately bounded palette. A six-plane
64-colour planar screen requires 61,440 bitmap bytes before any additional
buffers or small animation assets; exact memory ownership and lifetime must be
measured at the later implementation checkpoint.

Once `LEVEL_COMPLETE` is active, ordinary gameplay simulation, rolling-world
composition, Bobs, water and projectile/enemy work do not need to run. The
results screen therefore does not compete with the gameplay renderer's current
50 Hz deadline. Its moving regions can be limited to numeric glyph patches, a
few cyan accents and optionally a small two- to four-frame Sparkpaw victory
hold. Large background, frame and title elements remain static.

To avoid tearing, update only a hidden/inactive results buffer and publish at a
fixed display boundary, or use another independently proven presentation
method. Do not CPU read-modify-write the displayed bitmap. Reuse of existing
title-presentation concepts is plausible, but must not be assumed safe until
its memory lifetime, transition and Copper ownership are audited.

### Tally sound

A 20–35 ms mono tally tick is a tiny audio asset. One active tick replaces the
previous tick; skipped animation values do not queue delayed sounds. Row-lock
and final-total sounds are short and mutually exclusive with the tick train.
During the results state no gameplay sound scheduling is required, so a later
checkpoint can give the tally an explicit channel without disturbing current
gameplay priority. Exact Paula ownership and any future music interaction must
still be designed and tested separately.

### Main risks

- treating the 1448x1086 concept as production art instead of authoring an
  exact 320x256 indexed reduction;
- storing full-screen variants for every tally state instead of patching glyphs;
- allowing both gameplay and level-complete renderers/audio to remain active;
- updating a displayed bitmap directly and introducing tearing;
- tying elapsed time to missed gameplay updates rather than PAL fields;
- combining this feature with the active renderer/performance workstream;
- accepting emulator presentation as real-A1200, ADF or stock-68020 evidence.

### Required future gates

1. Finish and freeze the independently owned performance/hardware baseline.
2. Approve one composition and make an exact 320x256 indexed preview only.
3. Measure bitmap, buffer, glyph and audio memory before integration.
4. Prove the live HUD score patch in isolation with rapid pickup/kill events.
5. Implement the level-complete state without the gameplay update/render path.
6. Verify animation cadence and audio synchronization in FS-UAE/68030 first.
7. Run matched stock-68020 timing and then evidence-specific HD/ADF/real-A1200
   gates; infer none from the others.
