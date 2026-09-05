# Sparkpaw current status and next work

## Current checkpoint: 0.7.0-alpha.2 / Phase 7A.2

User requested an official checkpoint, all package formats, documentation,
lessons learned, commit and push. Public itch remains alpha.68 (live download
names and newest devlog verified); no itch upload is part of this checkpoint.

Current release in `dist`: HD ZIP/LHA, Disk1/Disk2 ADF, WHDLoad ZIP/LHA and the
same-version extracted HD drawer. Protected original alpha.68 remains intact.
The previous local alpha.1 and completed disk test drawer are archived intact.
See [release inventory and tests](RELEASE_0_7_0_ALPHA_2.md).

| Medium / scope | Evidence and remaining gate |
| --- | --- |
| HD campaign | User accepted 030 functionality and retained small optimizations after no noteworthy 020 gain; game executable remains byte-identical |
| Two-ADF campaign | User explicitly approved the corrected ADFs and INSERT DISK 1/2; FS-UAE report, exact CPU not restated in final approval |
| Campaign WHDLoad | Built with full campaign, original quit hooks and 48 assets; package/host verification complete, native testing pending |
| Physical A1200 / Gotek | This campaign release still awaits medium-specific hardware tests and matched cold-load timing |
| Analogue Pocket | Separate unverified gate |
| Intermittent physical HUD boundary issue | Remains open; no new evidence closes it |

HD game SHA256:
`e6e20db68f3f67b1e05b1db2b555842dec3f2473d8c0b7a543d9e5a76c04354e`.
Source reproduces the accepted complete, logger-free campaign. Both sections
retain their gameplay, rendering, audio, replay, continue and return contracts.
Story is included in HD/WHDLoad and deliberately absent from ADF.

## Immediate next work

User native test of the new campaign WHDLoad: intro, title/loading/charging,
ready/options, Level 1, Continue, Stormrail/finale/results, resident replays,
Esc/title return and F10. Then physical A1200/Gotek and Pocket as separate tests.
No automatic routine FS-UAE launch. The release is the current manual package;
there is no separate active diagnostic/candidate drawer.

## Performance research stays parked

User comparisons narrowed Level-1 differences versus original alpha.68 to
practically none. Retain the accepted Bob/column/history changes without a
perceptible speed claim. Do not repeat rejected audio split, gameUpdate extraction,
completion-cache or linker-order experiments without substantially new evidence.
No new enemy-placement changes or architecture refactor are scheduled.
See [re-audit](LEVEL1_PERFORMANCE_EVIDENCE_REAUDIT.md) and
[production baseline audit](LEVEL1_PRODUCTION_BASELINE_AUDIT.md).

## Document authority

- [Lessons learned](CHECKPOINT_ALPHA2_LESSONS.md): evidence, packaging, ownership,
  presentation and release safeguards, including the missing collision-map error.
- [Phase 7 roadmap](PHASE7_CAMPAIGN_HARDWARE_PLAN.md): released/package boundary
  and pending hardware validation; Phase 6D retains progression design context.
- [Campaign loop](CAMPAIGN_LOOP_CONTRACT.md), [asset ownership](CAMPAIGN_ASSET_OWNERSHIP.md),
  [multidisk](MULTI_ADF_CAMPAIGN_PLAN.md): authoritative behaviour and media rules.
- Stormrail finale/results/debris contracts remain authoritative.
- Dated *_TEST.txt, NEXT_SESSION_* and old handoff entries are historical unless
  this index explicitly reactivates them. Do not resurrect archived candidates.
- Gate 2 typed loader selection and larger architecture work remain deferred;
  physical disk duplication preserves the actual current loader without refactor.
