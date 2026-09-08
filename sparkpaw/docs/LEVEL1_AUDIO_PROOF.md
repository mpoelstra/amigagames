# Level-1 audio proof — 7 September 2026

> Alpha.5 checkpoint (9 September 2026): campaign music plus mixed SFX now
> ships in HD, ADF and WHDLoad. Positive user HD/ADF reports are recorded;
> new WHDLoad audio and final hardware acceptance remain pending.
> See RELEASE_0_7_0_ALPHA_5.md. Earlier candidate/no-release statements below
> are historical and superseded by the explicitly authorized release.


User authorized one loose audio proof after clarifying: intro/title retain all
four channels; each gameplay level gets its own uptempo accompaniment composed
for fewer voices; every existing effect remains represented.

Sole new test drawer: **dist/Audio-Level1-030-HD**, executable **Audio-Level1**.
Start from an Amiga Shell or Workbench Execute Command, PAL FS-UAE/68030,
2 MB Chip + 8 MB Fast. About 47 seconds music only, then all effects and combat
combinations; automatic stop/save/OS return at ~94 seconds. LMB stops early and
saves too. This new standalone program implements a clean exit, not the normal
game diagnostic's frozen display. Log is in the same drawer: renderdiag.log.

Copper Sprint: original 164-BPM, three-voice, 32-bar loop. ptplayer 6.4 CIA,
proof-only complete empty-channel isolation, two-voice fixed-rate C SFX mixer.
All 15 existing samples plus louder health variant are exercised. Debris shares
enemy hit/death and hurt; no missing stone-specific file is invented. Production
intro/title/gameplay/Stormrail/results code is unchanged. Results cues are part
of the audition, not new results music integration.

Sample allocations: 11,552 Chip music, 9,276 Fast score, 224 Chip output buffers,
48,130 Fast effect copies including louder-health copy. Code/BSS/OS overhead
additional. Executable 18,252 bytes. Drawer 87,037 bytes, 17 runtime files.
Executable SHA256: 5d25d735ad479f3e9084b632d4ab7334c4944d8dd6e72001a5b951b5f95b69fd.

Native compile, actual-C sanitizer/tail/priority tests, all-effects host replay,
preview peak check, existing music lifecycle and campaign ownership checks pass.
Stager tests preserve normal manifest defaults. All 61 alpha.4 release files
are hash-identical and all 15 staged SFX match their original bytes.
Initial preparation involved no automatic emulator run. Supplied 030 listening
result is recorded below; 020 and confirmed real-hardware gates remain open. Coarse mixer raster timings exclude music service
cost: this is not full 020 CPU budget or integrated-game frame performance proof.
No ADF, release, commit or push. Disk 1's 4,608-byte limit remains unresolved.

```
.venv/bin/python3 sparkpaw/tools/stage_hd_test.py \
  --drawer Audio-Level1-030-HD \
  --executable sparkpaw/build/audio-level1/Audio-Level1 \
  --executable-name Audio-Level1 \
  --readme sparkpaw/experiments/audio-level1/ReadMe.txt \
  --standalone-runtime sparkpaw/build/audio-level1/assets/runtime
```

For source, detailed effect mapping, replay patch rationale, reproduction and
limits see ../experiments/audio-level1/README.md (relative to sparkpaw/).

## Supplied complete 030 listening run — 7 September 2026

Evidence: `testresults/audio-level1-030-complete.log` and matching `.txt`
provenance sidecar. User listened to the entire proof and reports:
“op 030, klonk allemaal wel goed”. This is positive subjective 030 audio evidence,
not integrated gameplay acceptance. CPU is confirmed; emulator versus physical
hardware and exact RAM configuration were not explicitly reconfirmed.

The log records 4,685 fields (approximately 94 seconds), 6,154 music ticks,
9,230 audio services and 1,033,760 mixed samples (exactly 9,230 × 112).
All 16 effect entries, including the health-volume variant, started at least
once. All starts/suppressions match the host script: 38 plasma starts; six
intentional low-priority jump suppressions, with the solo jump started.
79 buffers contained overlapping effects. These are software service counters,
not captured output-waveform measurements. The complete footer alone also
appears on early exit; the field count plus the user's report establish the
full scripted run here.

**Timing remains inconclusive.** The 256-line maximum and 196 coarse late gaps
must not be reported as measured CPU peaks or audio dropouts. Source inspection
finds separately read VPOSR/VHPOSR in `line()`, allowing a torn raster value
at the 8-bit boundary. In addition, the timestamp combines raster position with
a software VBlank count that level-4 audio can observe before level-3 VBlank
updates it. These are plausible measurement artifacts; this log cannot establish
which events were artifacts or exclude real delays. Music IRQ cost is excluded.
Before drawing a CPU budget conclusion, use coherent timing and a monotonic
timebase without taking either timer from the music player. No runtime change
was made during this evidence review. Next useful gate is standalone 68020
listening; gameplay integration and Disk-1 fit remain separate decisions.

## Supplied complete 020 listening run — 7 September 2026

Evidence: `testresults/audio-level1-020-complete.log` and matching provenance
sidecar. User reports complete 020 playback with no perceived difference from
the 030. Same standalone version was requested, under PAL FS-UAE with 2 MB Chip
and 8 MB Fast; the log itself does not identify machine configuration.

Both runs record 4,685 fields, 9,230 audio services and 1,033,760 mixed samples.
Every effect's request/start/suppression count is identical, including the six
intentional jump suppressions. The 020 records 6,155 music ticks versus 6,154,
and 82 overlapping buffers versus 79. These differences do not by themselves
indicate an audible defect; overlap counts depend on scheduling alignment.
The 020 coarse raster maximum is 273, total 164,877 and late-gap count 751;
retain these raw values without converting them into CPU load or dropout
counts because of the measurement limitations described above.

Standalone listening is now positive on both reported CPU configurations.
This supports continuing the three-music/two-mixed-SFX approach. It does not
establish available CPU headroom during gameplay, real-hardware acceptance,
or ADF fit. Next development step is reliable isolated audio-cost measurement
and then a separately scoped Level-1 gameplay candidate; production remains
unchanged during this evidence review.

## E-clock timing gate — 7 September 2026

User authorized reliable audio-cost measurement, followed by a scoped Level-1
gameplay candidate. The latter remains gated on the native cost evidence: the
current game disables interrupts throughout takeover, and its diagnostic timer
uses CIA-B Timer B, also required by ptplayer. Do not link this OS-live proof
into gameplay or enable the existing profiler alongside the player.

Active set: **Audio-EClock-A-030-HD** (measured) and
**Audio-EClock-B-030-HD** (unmeasured control), both executable **Audio-Level1**.
Each runs the same 94-second script and returns to the OS after saving its own
renderdiag.log. First gate remains PAL FS-UAE/68030, 2 MB Chip + 8 MB Fast,
then the same set on 020 after 030 review. The previous accepted listening
drawer and .uaem were moved intact to dist/older-builds; both raw logs remain
under testresults. No automatic emulator run.

A uses timer.device ReadEClock, explicitly documented as callable from
interrupts in the [AmigaOS 3 autodoc](https://developer.amigaos3.net/autodocs/timer.device/ReadEClock.html).
It reads the OS clock, without claiming/programming a CIA timer. It measures
the SFX buffer body and wraps both complete OS-compatible ptplayer CIA
callbacks, including both Timer-B DMA phases. Exclusive time removes nested
measured music from the mixer scope. Short Disable/Enable sections serialize
accounting, not the whole audio service. Unsigned low-32 subtraction handles
rollover; the 94-second run is far below a complete 32-bit clock period.

ReadEClock returns the conversion frequency. Divide ticks by that frequency
for seconds; use summed exclusive ticks / elapsed ticks for the instrumented
service-time fraction. This includes clock/accounting overhead and possible
other OS preemption, excludes Exec dispatch and outer register saves, and is
not an exact CPU utilization figure. A 256-pair clock-read calibration is
logged raw and never silently subtracted. Max start gaps are service-spacing
observations, not DMA underrun counts. This revision does not establish
worst sliding-20-ms cost, precise onset latency or a zero-underrun guarantee.
A/B listening compares whether instrumentation itself changes audible behaviour;
B is not an independent CPU meter. No budget acceptance until native review.

Host verification: actual timing C under a deterministic Exec/clock shim
passes ASan/UBSan checks for nesting, exclusive attribution, wrap, spacing,
active lifecycle and balanced interrupt masking. Both native builds compile.
A/B runtime assets are byte-identical; timer.device reference exists only in A.
All 61 protected release hashes remain intact. Build provenance is in
build/audio-level1-timing/build.json and build/audio-level1-control/build.json.

Reproduce with `python3 experiments/audio-level1/build.py --timing` and
`--control` (from sparkpaw); host timing test is
`python3 experiments/audio-level1/test_timing.py`. Stage with the standard
stager's standalone-runtime option and matching Timing-/Control-ReadMe.txt.
Music, effects, gains, player channel-isolation patch and workload are unchanged.
No production runtime, ADF, version, commit or push changes.

## Supplied E-clock A/B 030 gate — 7 September 2026

Both logs preserved as testresults/audio-eclock-030-measured.log and
audio-eclock-030-control.log with provenance sidecars. User played both and
reports no strange sounds. This completes the requested 030 listening gate;
exact emulator speed/JIT settings are not embedded in the logs.

Both runs have 4,685 fields and identical effect requests/starts/suppressions.
A has 9,229 buffers and 6,153 music ticks; B has 9,230 and 6,154 respectively.
Overlap counts are 79/81. These small aggregate differences do not establish
an audible defect. The timing stack finishes at zero with no overflow/underflow.

A records 66,563,305 ticks at 709,379 Hz: 93.833 seconds. Summed exclusive
service bodies are 174,089 ticks, or 0.262% of elapsed time in this run.
Maximum inclusive mixer body is 1.580 ms, music Timer A 0.333 ms and DMA
Timer B 0.278 ms. Maximum mixer start spacing is 10.367 ms, versus nominal
112-sample duration about 10.17 ms; this alone is not an underrun detector.
Calibration spans 0–46 ticks (222 total for 256 pairs). In particular the
zero readings and very low music averages require caution about emulator
timing granularity/speed: these numbers are not a physical CPU benchmark.
No full CPU percentage, worst-window budget pass or zero-dropout claim is made.

Next gate: same A/B set on PAL FS-UAE 68020, same RAM and workload; preserve
030 logs before drawer logs are overwritten. No rebuild is needed. Gameplay
ownership changes remain pending target-machine timing evidence.

## Supplied E-clock A/B 020 gate — 7 September 2026

Preserved audio-eclock-020-measured.log and audio-eclock-020-control.log with
sidecars in testresults. User reports both full runs without strange sounds.
Both record 4,685 fields, 9,230 buffer services, 1,033,760 samples and identical
effect request/start/suppression counts. A/B music ticks are 6,154/6,153 and
overlap buffers 80/79. Timing stack balances with no over-/underflow.

The measured 020 run is materially more expensive than the 030 result:
66,570,724 ticks at 709,379 Hz; exclusive SFX 8,016,250 ticks, music Timer A
736,826 and DMA Timer B 113,714. Combined instrumented service-body fraction
is 13.32% (SFX 12.04%, music/DMA 1.28%). Average inclusive mixer service is
1.240 ms, maximum 2.866 ms. Maximum mixer start gap is 10.701 ms, not an
underrun count. Clock pair calibration averages 47.59 microseconds, minimum
31 ticks and maximum 333. Instrumentation/OS overhead and omitted outer
dispatch still prevent equating these values with exact production CPU usage.
The 030/020 gap cannot be attributed to CPU model alone without exact emulator
settings; the earlier 0.262% is not a useful stock-020 cost forecast.

**Listening passes; efficiency is not accepted.** The measured average already
exceeds the provisional 5% worst-window target, so no evidence supports moving
this unoptimized C mixer directly into the gameplay timing budget. Do not
subtract a single clock calibration value from every scope as an overhead fix.
Source mixRender still checks both voices for each byte, including silent
buffers. A bounded audio-only next step is block-based silent/copy/add paths
with exact sample-tail and priority parity, then the same measured/control
workload. This is a source-grounded optimization hypothesis, not a measured win.
Gameplay ownership/integration remains pending this efficiency gate; no general
gameplay performance investigation is reopened. No runtime edits in this review.

## Block mixer candidate — 7 September 2026

User authorized reducing the isolated mixer cost. `mixRender` now divides the
buffer at voice endings: zero-fill when silent, copy when one voice remains,
and byte-add when both remain. Voice pointers, remaining lengths, priorities
and completion counters update at span boundaries, not on every output byte.
No sample/gain, request policy, cooldown, output size/rate, DMA lifecycle,
player, clock-probe or workload changes. Production code is untouched.

Frozen pre-change `mix_reference.h` is a host-only oracle. Actual-C ASan/UBSan
tests compare every byte and the complete Mixer state across 40,000 randomized
request/update/render operations and 13,225 paired tails around the 112-byte
DMA boundary. Include zero counts, odd pointers, unequal/coincident endings,
replacement and output canaries. All pass, as does the all-effects script.
VBCC -O2/68020 assembly confirms inline wide copy/clear loops and no per-byte
voice-state tests in the add loop; no runtime library call in mixRender.
This is compiler evidence of reduced work, not native speed acceptance.

Active pair: **Audio-Block-A-030-HD** (same EClock-v1 measurement) and
**Audio-Block-B-030-HD** (unmeasured control). Run Audio-Level1 for 94 seconds
on PAL FS-UAE/68030, 2 MB Chip + 8 MB Fast, same emulator settings as before.
Each saves its own log and returns to the OS. After positive 030 review, use
the same pair on 020 and compare with preserved 13.32% instrumented reference.
A/B here denotes measurement/control, not old/new mixers; old/new timing uses
the preserved previous logs. The former EClock pair and .uaem metadata are
archived intact in dist/older-builds. No routine automatic emulator run.

A is 20,304 bytes, B 18,236 bytes (+292 each relative to the previous pair).
No extra audio RAM; all 17 runtime files remain byte-identical. Player-generated
hashes and instrumentation are unchanged. Native build manifests are in
build/audio-block-timing and build/audio-block-control. All 61 alpha.4 hashes
remain intact. Build with experiments/audio-level1/build.py --timing/--control.
The native efficiency gate and gameplay integration remain pending; no new
release, ADF, commit or push.

## Supplied block-v1 030 result — 7 September 2026

Preserved audio-block-030-measured/control.log and provenance sidecars under
testresults. User completed both, noticed no strange sounds but explicitly
qualified confidence in the listening impression; record provisional positive
listening rather than definitive audio-quality acceptance. Requested gate was
030; exact emulator configuration is not embedded in the logs.

Both logs contain 4,685 fields, 9,230 buffers, 6,154 music ticks, 1,033,760
samples, 81 overlapping buffers and identical effect counts. A's timing stack
is balanced with no errors. Exclusive ticks: mixer 26,230, music 13,845,
DMA timer 1,179 over 66,570,111 elapsed at 709,379 Hz: combined instrumented
service fraction 0.062%, compared with previous 030 0.262%. Mixer total falls
about 83.8% versus the previous 030 measurement. Maximum mixer body is
321 ticks (0.453 ms). Calibration includes zero-tick reads (166 ticks total
for 256 pairs), so preserve the emulator timing caveat and do not extrapolate
a stock-020 budget or physical CPU speedup.

Technical 030 counters are consistent. Same pair on 020 is the next cost gate;
subjective quality remains provisionally positive. Previous 020 13.32% remains
the comparison baseline, not the new 030 value. No rebuild or runtime change.

## Supplied block-v1 020 result — 7 September 2026

Preserved testresults/audio-block-020-measured.log and -control.log, each
with a provenance sidecar. User explicitly confirms both runs on 020 and no
strange sounds. A records 4,685 fields / 9,230 buffers / 6,154 music ticks;
B 4,684 / 9,227 / 6,152. Both reach the automatic completion threshold and
all effect request/start/suppression counts match. The small endpoint and
overlap differences (81/79) are not evidence of audible failure. A timing
stack is balanced with no overflow/underflow.

At 709,379 Hz over 66,570,094 elapsed ticks, exclusive measured service
ticks are 1,183,381 mixer, 737,802 music and 114,620 DMA. Combined service
fraction is **3.06%**, versus previous 020 **13.32%**. Mixer alone is
**1.78%**, down from 12.04%; its measured total is about **85.2% lower**.
Music/DMA remains about 1.28%, a useful consistency check for the focused
change. Inclusive mixer average is 0.183 ms and maximum 0.718 ms (previous
1.240 / 2.866 ms). Calibration is 30–35 ticks, total 8,313 for 256 pairs.

The isolated candidate now has positive 020 listening plus measured reduction
in mixer work. This supports proceeding to a scoped Level-1 integration proof.
The average is below 5%, but is not a pass of the proposed worst-20-ms-window
target: current scopes include instrumentation/OS preemption, omit dispatch
and outer saves, and do not measure sliding windows or real-game contention.
No integrated frame-rate, precise onset, zero-underrun or real-hardware claim.

Next authorized development: isolated Level-1 gameplay candidate preserving
all game-facing effect calls, independently clocked audio and safe interrupt/
DMA ownership. Keep production intro/title, Stormrail and release baselines
unchanged; do not reuse the conflicting CIA-B game profiler. General gameplay
performance remains parked. No additional standalone listening round is needed
for byte-identical playback unless new changes or defects justify it.

## Direct Level-1 gameplay v1 candidate — 7 September 2026

User authorized proceeding after the block mixer's supplied 020 result. Active
set: **Level1-Audio-A-030-HD** (original SFX) and **Level1-Audio-B-030-HD**
(Copper Sprint + block mixer); executable **Level1-Audio** in both. This changes
A/B meaning from measured/control to original-audio/new-audio. Both start
directly in Level 1 and use the same production game/renderer source, flags,
seed and controls. Play similar routes for about two minutes. LMB or Escape
stops, returns to Workbench and writes each drawer's renderdiag.log.

B enables only EXTER/CIA-B music and AUD3 during owned display, keeping Exec
Forbid and keyboard polling. A generated, guarded platform copy balances the
changed interrupt nesting. The audio adapter keeps all 16 game-facing calls;
only AUD3 is masked during voice publication. Stop quiesces owned CIA sources
before restoring OS masks and frees audio after DMA stops. No source file
under src/ was changed. See experiments/audio-gameplay/README.md for ownership,
build/test reproduction and remaining transition obligations.

Logging uses hardware CIA-A TOD for publication intervals, not the conflicting
CIA-B profiler. This first gate does not measure CPU scopes or export renderer
ownership counters. Prepared memory and effect counts are included. Manual
route differences limit cadence comparisons. No native startup, input, audio,
visual, exit or cadence acceptance is claimed yet. 030 first, then 020.

Native builds compile: A 117,392 bytes, B 128,256 bytes. The adapter's actual C
passes ASan/UBSan host tests for load/install failures, all event mappings,
buffer service, quiescence and cleanup; real IRQ scheduling remains native-only.
Standard stager verifies all 52 common runtime files identically; B adds only
the 9,276-byte score and 11,552-byte sample bank. Candidate additions cannot
override production runtime files (host-tested). All 61 release hashes match.
Previous Audio-Block pair and launch metadata archived intact with logs.

No intro/title/READY/results/Stormrail flow is exercised in this candidate.
Those production paths remain unchanged, but later complete campaign ownership
verification is mandatory before adoption. No ADF, release, commit or push.

## Supplied Level-1 gameplay 030 result — 7 September 2026

Logs preserved as testresults/level1-audio-030-a.log and -b.log, with provenance
sidecars. User reports both look OK with no sound issues or perceived FPS drops.
B was played longer after falling into water. This is positive gameplay/audio
evidence for the requested 030 gate, not a controlled identical-route benchmark.
Both logs have complete footers and end at camera X=3,072.

A: 2,054 publication intervals, 2,053 hardware fields; 2,043 one-field, five
two-field, zero three-field, six other. B: 3,925 intervals/fields; 3,881 one,
22 two, zero three, 22 other. Max is two fields in both, so all 'other' samples
are zero-field deltas. Raw two-field shares are 0.24%/0.56%, but zero/two
pairs and asynchronous TOD/publication sampling prevent interpreting these
as exact missed-render-deadline counts. Do not report >50 FPS from A's totals,
or claim zero measured frame drops. Source tracks post-publication snapshots,
not exact Copper-visible transition times. No renderer ownership counter exists
in these logs; zero ownership violations is not established.

B starts all 134 plasma requests, one hurt, one water splash, one extra life
and one Core cue. It also starts 46 enemy hits, 25 deaths, two Strider shots,
29 jumps and 37 diamonds. 1,342 mixer buffers contain overlapping effects.
The priority/cooldown suppressions are logged; unequal manual routes preclude
expecting identical A/B event counts. Harrier, health and tally were not
triggered in this Level-1 run; prior standalone coverage remains separate.

Prepared free Chip A/B: 704,840 / 739,088 bytes (+34,248 B), largest blocks
703,368 / 737,616. Free Fast: 6,222,072 / 6,151,776 (-70,296 B). This is a
prepared-state snapshot including executable/OS differences, not a measured
peak or proof of transition memory safety.

Next: same exact A/B builds on PAL FS-UAE 020 with 2 MB Chip + 8 MB Fast,
comparable routes and stop/save. Keep timing caveats; use user-observed
smoothness alongside counts. Native 030 startup/play/save is now supported,
but full campaign/presentation/Stormrail/ADF/hardware remain untested here.
No runtime change or rebuild during this evidence review.

## Supplied Level-1 gameplay 020 result — 8 September 2026

Preserved testresults/level1-audio-020-a.log and -b.log with provenance
sidecars. User explicitly reports 020, no perceived FPS difference and audio
OK; B ran longer after a water fall. Both logs have complete footers and end
at camera X=3,072. Positive subjective gameplay/audio evidence, not matched
route timing or full performance acceptance.

| Raw measurement | A original SFX | B music+mixer |
|---|---:|---:|
| Publication intervals | 2,141 | 3,494 |
| Hardware fields | 2,155 | 3,546 |
| Approx duration at 50 Hz | 43.10 s | 70.92 s |
| Aggregate publications/second at 50 Hz | 49.68 | 49.27 |
| One-field deltas | 2,125 | 3,148 |
| Two-field deltas | 15 | 189 |
| Three-field deltas | 0 | 2 |
| Other deltas | 1 | 155 |
| Maximum delta | 2 | 7 |

These are post-publication TOD readings, not exact visible frame timestamps.
A's other sample must be zero. For B, the counters imply 153 zero-field and
two seven-field samples: known bins sum to 3,532 fields, leaving 14 for the
other bucket, with maximum seven and allowed positive values >=4. This can
only be two sevens. The zero/two pattern was already observed on 030. The
two three-field and two seven-field observations must remain open; seven PAL
fields nominally span 140 ms. The aggregate log has no event timestamps to
locate them relative to water, respawn, IRQ activity or publication boundaries.
Do not discard them as proven clock artifacts or attribute them to audio.
The 0.41/s aggregate A/B difference and unmatched routes cannot establish an
audio-caused FPS regression. No zero-deadline-miss or zero-ownership-violation
claim is supported. Resolve the timing observation/attribution before accepting
performance or promoting to production. Prefer source review of the sampling
boundary, then a bounded event-correlated trace if necessary; do not reopen
general gameplay optimization or ask for another identical blind run.

B starts all 136 plasma, five hurt, one water, one Core and one Strider request;
51 enemy hits, 20 deaths, 28 jumps and 39 diamonds. 1,248 buffers contain
overlapping effects. No 1UP/Harrier/health/tally in this run. 4,657 music ticks
and 6,987 mixer services show sustained service over the longer run, not a
proof that no transient interruption occurred.

Prepared free Chip is 703,544 / 738,496 bytes (+34,952 B); largest blocks
702,480 / 737,304. Free Fast is 6,222,136 / 6,150,520 (-71,616 B). These
snapshots support the expected Chip-to-Fast sample tradeoff but include code/
OS differences and do not measure peak memory. No source/build/release changes
during evidence review. Full transitions, results, Stormrail and ADF remain
separate later gates.

## Phase trace candidate — 8 September 2026

User authorized investigating the 020 long-delta observations. Source review
confirms that rendererPublishGameplay writes COP1LC/COPJMP1 before updating
active target state and calling prototypeExposeHistoryUnion. The harness reads
TOD only after the function returns. Thus the old counter is not located at
the hardware publication write. Existing platformRasterLine reads VPOSR/VHPOSR
separately; no runtime raster-reading correction is included in this trace.
Water expiry calls resetLevelRuntime, which resets enemy/collectible/projectile
and player state. These are candidate explanations/locations, not native proof
of the two seven-field observations.

New active pair **Level1-Trace-A-030-HD / Level1-Trace-B-030-HD**, executable
**Level1-Audio**, retains original SFX / music+mixer meanings. It adds four
phase observations: before gameUpdate, after gameUpdate, after renderer update
and Bob drawing, after successful publication. Store raw TOD and a high/low/
high-checked raster reading separately; never synthesize a falsely precise
combined timestamp. Audio IRQs stay serviceable and no CIA timer is changed.

Trace records include sequence, old publication delta, publication attempts,
game frame before/after, camera, water timer and health. Up to 96 >=3-field/
reset/water-onset-or-end/health events are saved separately from the first 16
ordinary zero/two examples. Water countdown frames alone do not consume the
important pool. Capacity drops are explicitly counted; no file writes until
stop. Static record storage is 113 records (about 6.6 KiB on 68k); measurement
adds work to both variants. Exact visible deadlines, pure CPU cost and IRQ
causation are still not measured. Use this to locate gaps by phase/event;
do not compare its headline FPS directly with the untraced baseline.

Both native builds compile; actual trace C under a host shim passes sanitizer
checks for phase values, event selection, independent pool bounds and drop
accounting. No native run yet. A/B sizes 119,444 / 130,308 bytes. Generated
platform hash is unchanged. All corresponding runtime files and all 61 release
hashes are unchanged. Previous gameplay pair and metadata/logs archived intact.

First gate: same PAL FS-UAE 030 configuration, 45–60 seconds each; deliberately
fall into water once in BOTH versions, let respawn complete and continue briefly.
LMB/Escape saves and returns. Then same 020 route after 030 review. This is a
targeted trace workload, not another blind repeat. No audio/gameplay fix, no
production source edit, no general performance work, release, commit or push.
Build: experiments/audio-gameplay/build.py --trace; host check: test_trace.py.
Manifest/generated sources in build/audio-gameplay-trace.

## Supplied phase-trace 030 result — 8 September 2026

Preserved level1-trace-030-a/b.log and provenance sidecars. User says “ok done”;
no new explicit subjective assessment. Both complete logs establish the
requested water/respawn route: A 1,369 intervals/fields (~27.38 s), B 1,558
intervals/1,557 fields (~31.14 s). Each records one water effect and exactly
two important records (water onset and reset); zero important records dropped.
Neither has a >=3-field interval. Each retains 16 short examples, with 10/7
additional short examples dropped as designed.

A water onset seq1086 and reset seq1102 both delta=1, attempts=1; B onset
seq1265 and reset seq1281 likewise. Reset visibly changes game frame/camera
to zero in the captured state. These particular 030 water events do not
reproduce the earlier long 020 observations. Do not generalize to 020.

Short traces strengthen the sampling-phase explanation: e.g. A seq39 ends
at TOD638/raster8 with delta2, seq40 ends TOD638/raster1 with delta0. Both
use one publication attempt. There is a raster wrap while the endpoint TOD
value stays unchanged. Equivalent examples exist in B. Raw TOD/raster are
separate reads, not an atomic clock; delayed endpoint sampling and TOD update
phase can produce zero/two pairs without proving missed visible frames.
The phenomenon also occurs in A without the new audio interrupts, so it is
not uniquely introduced by B's music/mixer. This supports a measurement-phase
explanation for these short examples, but does not explain the old seven-field
020 observations or establish exact visible cadence.

Next: same trace pair on 020, deliberate water once in each, allow respawn and
continue briefly. No build change required. Long-event capacity is available
and the trace captures the intended context. Native reason for long 020
intervals remains open; no performance fix or production promotion.

## Supplied phase-trace 020 result — 8 September 2026

Logs preserved as testresults/level1-trace-020-a/b.log with sidecars. User
reports done; no new subjective assessment. Complete footers, zero important
records dropped. A: 1,569 intervals / 1,593 fields (~31.86 s); B: 1,232 /
1,310 (~26.20 s). Different manual routes and trace overhead remain caveats.

**Seven-field gaps are reproduced without new audio.** A seq1304 (water expiry,
frame1302->0, camera1456->0) and seq1305 (next frame) both delta7/attempts1.
B seq930 (frame928->0, camera1436->0) and seq931 likewise. For each, TOD
advances six counts between phase1 (after gameUpdate) and phase2 (after
renderer update/Bobs). GameUpdate itself stays within the same TOD count;
no repeated publication attempts. Water onset is delta1 in both. Thus native
evidence locates the long work in rendering during reset and the following
frame, and rules out new audio as a necessary cause of these seven-field gaps.
It does not establish that audio adds no cost to them.

Source explanation: prototypeRollTarget calls prototypeCopyInitial when an
origin jump exceeds PROTOTYPE_RING_W. A water reset moves the camera from
~1,450 to zero. The alternating targets then each need their new window. This
fits two successive long rendering passes. The trace does not instrument that
function specifically; full-window rebuild attribution is a strong source-based
inference, not a measured function-cost breakdown. General renderer optimization
remains parked; do not turn this finding into a respawn rewrite.

**Separate B cadence concern remains.** Six delta3 records (seq542,554,574,622,
776,890) occur outside water/reset near camera768–1378. All have attempts1.
Phase2 raster values are 3,31,31,13,8,10; five exceed the renderer's accepted
publication window (line<=4). Phase1->2 shows wrap/field advancement, followed
by waiting for publication. Source loop always waits through line300 before
trying publication; attempts1 counts calls, not skipped opportunities during
that wait. Audio IRQ work, trace overhead and different routes can shift a
borderline render pass past its window. These records support a rendering/
publication-margin concern, not CIA playback stalls or a proven isolated cause.
A has no delta3 records in this run. Do not dismiss B's residual cost as only
TOD noise, nor use trace headline rates (~49.25/47.02) as production FPS.

Next bounded audio work should evaluate remaining IRQ/mixer cost and its
interaction with the narrow publication window, using comparable workloads
and low observer cost. Preserve renderer boundary contract; no timing fix
is authorized merely by locating the gap. Full audio performance acceptance
and campaign adoption remain open. No runtime/build changes during review.

## Fixed-work gameplay A/B candidate — 8 September 2026

User authorized a repeatable short comparison without the detailed trace.
Active pair: **Level1-Fixed-A-030-HD / Level1-Fixed-B-030-HD**, executable
**Level1-Audio**. A uses original effects; B music/block mixer. Both automatically
execute 1,000 game steps (~20 seconds at 50 updates/s), stop, restore OS and
save their own log. LMB abort is separately marked script_complete=false
(`complete=0` on the script line); complete footer only proves log flush.

Input is indexed by fixed game step, not elapsed wall time. The generated
player source changes only playerReadInput's hardware input boundary and
preserves rising-edge semantics; movement/physics/animation code is unchanged.
Same starting seed, no invulnerability, teleport or enemy modifications. First
520 steps are approach/warmup. Exactly 480 subsequent publication intervals
form the measured window. Host real-C gameplay simulation (game, collision,
level data, enemies, player, collectibles, projectiles; real collision asset)
repeats identically under sanitizers, without resets. It spends 297 total
steps in camera750–1000; the measured patrol is camera892–1133, final931.
This is a wider representative combat region than the initial approximate
750–1000 suggestion. Host mock clock/audio and absent rendering mean native
state equivalence is still a gate, not assumed from the simulation.

A selected-state rolling hash covers player x/y/health and game camera/score/
frame number each step. Compare matching completed scripts, hashes, resets,
route bounds and effect request counts before attributing A/B timing. It
excludes elapsed fields, pointers and renderer history; it is not a full-state
or cryptographic proof. Differences invalidate a simple same-work comparison.

Light measurement retains publication interval totals (including explicit
zero deltas), retry-frame count and a 4x4 histogram of raw readiness TOD
difference versus beam-line bands (0–4,5–31,32–299,300+). TOD bucket3 includes
all values>=3. The two readiness reads happen after drawing in the measured
window only; accounting/hash runs after publication. No phase record arrays,
per-frame disk I/O, timing changes or CIA-B profiler. Delta/beam phase is
still not an atomic timestamp: aggregate fixed-work time plus histogram is
comparison evidence, not exact visible missed-deadline classification. Small
endpoint quantization and observer overhead remain. Do not promote a trace
headline FPS or a raw readiness band into a production acceptance claim.

Native A/B compile (118,684 / 129,548 bytes). Generated platform and audio
backend/player/mixer are unchanged. Standard stager confirms all 52/54 assets
match prior variants; detailed trace strings absent. All 61 alpha.4 hashes
match and src/ has no edits. Prior trace pair/logs/metadata archived intact.
No native execution yet. First gate is 030 unattended playback and log review,
then same builds on 020 with the same emulator speed/settings. Native route,
visual and audio behaviour remain pending. No automatic FS-UAE run, release,
commit or push. Build: experiments/audio-gameplay/build.py --fixed. Host route
check: test_fixed.py. Artifacts/manifest in build/audio-gameplay-fixed.


## Fixed-work supplied 030 evidence — 8 September 2026

User reports “beide gedraaid” following the requested PAL 68030 test. CPU/RAM
settings are not embedded in these logs; no new explicit subjective listening
or visual verdict was supplied. Raw logs and provenance sidecars preserved as
`testresults/level1-fixed-030-a.log` and `level1-fixed-030-b.log`.

Both complete all 1000 steps, selected-state hash -463786273, zero resets,
297 steps in camera750–1000, measured camera892–1133, final camera931/health3.
Normalized effect requests/starts/suppression also match: shots114/114/0,
hurt3/3/0, hits17/17/0, deaths4/4/0, jump14/9/5, pickup14/10/4; other events
zero. This supports matched selected workload, not a full-state equality proof
or coverage of every effect family.

Both measured windows contain 480 publication intervals over 480 PAL fields
(about 9.6 seconds). A has one zero/two-field pair, B three; neither has
three-plus-field intervals or publication retries. No aggregate timing penalty
is demonstrated for this route on the requested 030 configuration. Raw TOD
phase/endpoint quantization still precludes claiming exact visible drop counts.
All 480 readiness observations in each variant occupy raster32–299; the broad
bucket does not quantify CPU headroom or exact publication margin.

B records 1313 music ticks, 1971 mixer services, 220752 mixed samples and 682
overlap buffers across the run. Prepared free Chip is 739088 versus704840
bytes (+34248); free Fast is6151000 versus6220760 (-69760). These are whole
candidate snapshots, not isolated mixer allocations or peak-memory proof.

Next: run these exact active drawers on PAL 68020, 2 MB Chip + 8 MB Fast,
without steering; allow automatic completion/save/OS return. No rebuild or
runtime changes during this evidence review. Full campaign transitions and
production adoption remain separate gates.


## Fixed-work supplied 020 evidence — 8 September 2026

User reports “ok beide gedraaid” after the same-build PAL 68020 / 2 MB Chip +
8 MB Fast request. Configuration is not embedded in logs and no new subjective
sound/visual assessment was supplied. Preserved raw logs/provenance:
`testresults/level1-fixed-020-a.log`, `level1-fixed-020-b.log`.

Both complete1000, selected-state hash -463786273, resets0, band steps297,
measured camera892–1133, final camera931/health3, matching 030 route. All
normalized event requests match. Starts/suppression match except pickup:
A14/10/4 versus B14/11/3 (requested/started/suppressed). Do not describe the
actual audible workload as identical. This route does not cover all effects.

For480 measured intervals A uses482 fields (0 zero,478 one,2 two), B486
(20 zero,434 one,26 two). Both have zero3+ and zero publication retry frames.
Nominal PAL elapsed9.64/9.72 seconds, B+80ms or0.83% versusA for this window.
This is aggregate cadence, not mixer CPU utilization or exact visible FPS.
Unlike the 030 pair's equal480 fields, 020 shows small residual elapsed cost;
a single pair and one extra effect start cannot isolate its cause.

Readiness A:474 raster32–299,4 raster300+,2 raster5–31. B:473 raster32–299,
1 raster300+,3 raster0–4,3 raster5–31. TODdelta2 occurs onceA/threeB, all at
raster5–31. This supports occasional boundary proximity; raw sequential TOD/
beam reads and broad bins do not prove an exact missed-window count.
B ticks1375, mixer services2065, samples231280, overlap buffers689 across
whole run (not only the480-step measurement). Prepared free Chip A704248 /
B737792 (+33544); free Fast A6218632 / B6149048 (-69584).

Source finding: original audio.c holds gameplayPriority until gameplayDmaTicks
expires in audioUpdate (game updates, estimated sample fields). mix.c clears
priority when remaining samples reach zero in the audio interrupt; cooldowns
still decrement by game update. Thus identical event requests need not yield
identical admissions when wall time per update changes. This is a plausible
explanation for the extra pickup, not an event-level proof from these totals.

Next proposed bounded work: inspect/host-test priority admission around sample
completion and delayed game updates, then assess remaining audio-only IRQ cost
against the narrow publication window. Specify intended priority/release
semantics before any fix. Do not change rendering, add broad profiling, or
request another identical routine native run. Candidate not yet approved for
production transitions/campaign. Evidence review changed documentation only;
no rebuild, emulator launch, release, commit or push.


## Pragmatic main-game adoption candidate — 8 September 2026

User explicitly stopped micro-optimization and approved integrating the proved
track and effects in the main game. The 020 fixed-work residual (+80ms in a
~9.6s window) and one extra pickup admission remain recorded, not blockers or
new optimization tasks. Repeated supplied listening/feel was positive; this
is not a new assertion of exact visible FPS or real-hardware acceptance.

Normal HD `make` enables SPARKPAW_LEVEL1_MUSIC for the production campaign
play target. `audio.c` retains the original effect backend for READY/results
and Stormrail, dispatching Level-1 gameplay calls to `level1_audio.c` and the
proved block mixer in `audio_mix.c`. All 16 APIs, samples, gains and priorities
are retained; proof counters and scripted input are omitted. The new score is
byte-identical to the auditioned Copper Sprint MOD, now under music/level1.

Lifecycle: preload score/bank/Fast effects during the ordinary Level-1 load;
reserve CIA-B timers without touching title playback; start only after
`titleRelease` at APP_PLAYING entry. The platform keeps Forbid but balances
its Disable with one Enable while only EXTER/CIA-B and AUD3 are enabled.
Every loading/restore/debug boundary balances that Enable before stopping the
player, clearing pending CIA requests and restoring the AUD3 vector. Timer B
is explicitly stopped and its DMA-phase toggle reset before replay so an
interrupted delayed-DMA phase cannot survive into a new song. Water/life reset
stays inside gameplay and keeps music running; resident replay reinitializes
the song without file I/O. Results use the original SFX path. Cross-section
loads free the Level-1 backend, so Stormrail owns no new music/mixer IRQ work.

The pristine ptplayer6.4 upstream is retained in third_party/ptplayer with its
license. tools/prepare_game_audio.py applies auditable changes: the proved
empty fourth-track reservation, timer-only OS install/remove (music.c already
owns audio.device), deferred player initialization, and caller-owned interrupt
masking for setters. No MOD effect/timing conversion is added. The CIA player
uses Fxx tempo; the LSP title path is unchanged. Native interrupt latency and
all complete-game transitions still require user playback.

Memory tradeoff deliberately favors safe resident results: legacy Chip SFX
remain loaded, alongside 11552 bytes music-bank Chip +224 bytes mixer-output
Chip, 9276 bytes score Fast +48130 bytes scaled effects Fast (allocation and
code overhead additional). The earlier proof's net Chip saving does NOT apply
to this integration. No runtime allocation occurs in the audio IRQ. New asset
payload is20828 bytes plus executable growth; ADF/WHDLoad have not adopted
this backend and no release/media files were rebuilt. HD-specific logical
asset ownership is separate from the accepted media manifest.

Verification: normal native build succeeds (existing renderer optimizer
warnings only); full host suite succeeds. Actual backend tests cover every
allocation failure, install failure, preloading with title live, refusing a
premature start, 24 start/stop cycles, vector restore, IRQ quiesce and balanced
cleanup under ASan/UBSan. Production mixer output/state matches the independent
sample-at-a-time oracle for40000 randomized operations and13225 paired tails.
Standard staging verifies54 runtime references/assets and preserves61 release
files. Fixed A/B drawers and metadata archived intact. No emulator launched.

Active `dist/Level1-Music-Game-030-HD/Sparkpaw-Music` is the normal full game,
without diagnostics or LMB log saving. Ask for one functional 030 play through
READY/start, Escape/restart and results/replay/continue as reached. No new A/B
performance loop. After functional acceptance, corresponding020 check remains.
No SemVer, release, commit or push.


## Stormrail music continuation — 9 September 2026

Stormrail music integration — 9 September 2026. User reports the prior
main-game Level-1 candidate “lijkt allemaal goed” after the requested030 test;
no new log or configuration details were supplied. Preserve this general
positive report without inventing individual transition or020/hardware checks.

Main HD now selects Copper Sprint for Level1 and original Iron Horizon for
Stormrail. Iron Horizon:172 BPM,64 bars,~89s loop, three music voices, catchy
returning theme/power-fifth accents, two mixed SFX voices on Paula3. Same
backend/priority/gain/lifecycle; section selection happens only during load.
Only one track is resident. Intro/title/READY and original results tally stay
unchanged. Stormrail departure->flight->Harrier has no new load; life reset
keeps music playing, resident replay restarts it.

Sole active drawer: `Stormrail-Music-030-HD`, executable `Sparkpaw-Music`.
Use READY OPTIONS->STORMRAIL->START GAME for a focused030 audition. No need
to complete Level1 first. No diagnostics, A/B route or LMB log saving.
Normal native build/full host suite pass;56 staged assets match references,
61 release files remain identical. User Stormrail music/feel/native transitions
remain pending. Prior Level1 drawer archived intact. HD only; ADF/WHDLoad,
release/commit/push and general performance work remain out of scope.
See `sparkpaw/docs/STORMRAIL_MUSIC.md` and `STORMRAIL_MUSIC_GAME_TEST.txt`.

## Music campaign ADF — 9 September 2026

Music campaign ADF candidate — 9 September 2026. User reports the Stormrail
HD music “ok gaat goed” after the requested030 audition; no extra machine/log
or individual-transition details supplied. User now authorized an ADF version.

Sole active test set: `dist/Music-Campaign-ADF/Music-Disk1.adf` and
`Music-Disk2.adf`. Ordinary880-KiB FFS disks, PAL A1200/2MB Chip+8MB Fast.
Disk1 has30 blocks/15KiB free; Disk2 has318 blocks/159KiB free. Title/READY
Neon Sky, Level1 Copper Sprint, Stormrail Iron Horizon + mixed SFX; results
original SFX. Like alpha.4 ADF, story intro/Hero Drive omitted.

Disk-only: pack scores/banks/SFX with existing SPR1/SPL1 decoder; externalize
40192-byte READY masks as6423-byte packed data, loaded once into Fast before
READY. No gameplay decompression, no HD compression, no audio/visual changes.
Markers SP07M1/SP07M2 reject mixing these files with older SP07D disks.
Native compile/full host suite/actual C decode+per-disk readback checks pass;
HD executable byte-identical and61 alpha.4 release files preserved. Boot,
DF0 swap/DF1 discovery and audio remain user-native gates; no emulator run.
Use first030, OPTIONS->STORMRAIL for quick Disk2 testing. No A/B/log save.
Prior Stormrail HD drawer and previous multidisk build/evidence archived intact.
No release/commit/push. See `sparkpaw/docs/MUSIC_CAMPAIGN_ADF.md` and
`MUSIC_CAMPAIGN_ADF_TEST.txt`.
