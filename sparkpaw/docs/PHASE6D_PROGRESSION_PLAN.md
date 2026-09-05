# Phase 6D multi-level progression boundary

> Work order update: user chose the 0.7.0-alpha.2 all-format checkpoint after
> accepting HD and ADF; new campaign WHDLoad/hardware gates remain open. Standalone Gate-2 refactoring is superseded;
> use it only as needed at the media boundary. See PHASE7_CAMPAIGN_HARDWARE_PLAN.md.

Current status: see [CURRENT_STATUS.md](CURRENT_STATUS.md). The released
Level-1 -> Stormrail campaign and results flow are implemented and user-tested.
Published alpha.68 remains the protected one-level release. Do not treat an
old isolated results drawer as active or request implementation of CONTINUE again.

## Implemented progression contract

[CAMPAIGN_LOOP_CONTRACT.md](CAMPAIGN_LOOP_CONTRACT.md) owns the exact decisions,
post-Level-1 snapshot, resident section replay, return-to-title and Escape rules.
`enum CampaignSection`, `ResultDecision` and `CampaignState` already exist.
Stormrail is the loadable next section; a separate playable Level 2 does not
need to exist to justify the current Level-1 CONTINUE action.

Preserve black/loading-bounded cross-section loads, resident replay without
asset reload, one-shot score/snapshot decisions and reset of section-local state.
[CAMPAIGN_ASSET_OWNERSHIP.md](CAMPAIGN_ASSET_OWNERSHIP.md) owns asset grouping.
Integrated Stormrail replay restores the carried snapshot; its old isolated
fresh-vitals replay is historical, not the campaign replay rule.

## Next bounded engineering proposal

Architecture Gate 2: extend existing typed section identity into the asset
selection boundary, which still uses a Boolean Stormrail selector. Prove valid
selection and partial-load cleanup while preserving exact files/load order and
visible transitions. Do not combine this with hotpath extraction or redesign.
This is a proposal following documentation reconciliation; no code was changed.

## Deferred

Performance tuning and encounter-placement experiments are explicitly parked.
Section-state extraction and broader cold-renderer ownership changes remain
later architecture gates. Level-2 content remains a separate design workstream.
No SemVer/release, ADF/multidisk, WHDLoad or new platform acceptance is implied.
The original plan is preserved in DOCUMENTATION_STATUS_HISTORY_2026_09_05.md.
