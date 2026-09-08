# Campaign asset ownership contract

> Alpha.5 checkpoint (9 September 2026): campaign music plus mixed SFX now
> ships in HD, ADF and WHDLoad. Positive user HD/ADF reports are recorded;
> new WHDLoad audio and final hardware acceptance remain pending.
> See RELEASE_0_7_0_ALPHA_5.md. Earlier candidate/no-release statements below
> are historical and superseded by the explicitly authorized release.


> Current status (2026-09-05): Current ownership rules remain in force. The user-approved two-ADF release duplicates shared and still-requested carryover assets; typed loader/renderer restructuring remains deferred. Performance/placement changes are parked. See [status index](CURRENT_STATUS.md).

`tools/campaign_asset_manifest.py` is the authoritative logical ownership list.
Every literal runtime reference in the campaign executable must occur in exactly
one group. `tests/test_campaign_asset_ownership.py` rejects missing, duplicate,
unreachable or unowned files and names longer than Amiga's 30-character limit.

## Ownership groups

- **Shared presentation:** story plates, title, loading/charging, ready/menu,
  results presenter/glyphs and tally audio. These may live on a boot/common
  volume because neither gameplay world owns them.
- **Shared gameplay:** the five HUD sheets, diamond, common shot/hurt/hit/death
  and collection audio. Both current sections intentionally use these.
- **Level 1:** `storm-front`, `storm-rear`, collision map, the byte-exact
  alpha.68 player sheet, beetle/strider, Level-1 movement/water/strider audio,
  extra life and the Stormstone Core with their audio.
- **Stormrail:** its front/rear/flight-rear, extended cockpit player sheet,
  vehicle/enemy/finale family, heart, obstacle family and Harrier audio.

The similarly named Stormstone Core belongs to **Level 1**: it is the visible
and collectible objective at the ruin and leads into Stormrail; its name does
not make it a Stormrail-owned asset.

## Runtime boundary

Before gameplay loading, `loadStormrailGameplay` selects the section-specific
front, rear and player sheet. Level 1 must never load Stormrail flight rear,
heart, family or obstacles. The renderer must not copy, restore or draw
Stormrail history while `stormrailActive` is false. The accepted alpha.68
Level-1 front/rear/player hashes are guarded separately by
`test_campaign_level1_isolation.py`.

Some Level-1-owned conversion sources are currently resident during Stormrail
because both sections still enter one common renderer-preparation function.
That is harmless in the current single-HD drawer, but it is deliberately **not
claimed as physical multi-ADF separation**.

## Current physical two-ADF rule

Logical groups above remain authoritative; physical volumes intentionally
include every file requested by the unchanged section loader. Disk 1 contains
shared files plus Level 1 and common Harrier audio. Disk 2 contains shared files,
Stormrail and the current Level-1 conversion/audio carryover. Story is HD-only.
`tools/package_multidisk_probe.py` derives both sets from the group manifest and
verifies compiled references and every packed/decoded file. Disk-message strips
are disk-only generated presentation assets, outside the HD release manifest.

This supersedes the earlier requirement to split renderer preparation before
packaging: measured capacity permits duplication without an unrelated renderer
refactor. It does not claim physical residency isolation. Media selection stays
above the gameplay loader, and gameplay/renderers select by section, never disk
number. New gameplay assets still require one logical group and ownership tests.
See MULTI_ADF_CAMPAIGN_PLAN.md for the media format and separate hardware gates.

## Production residency audit — 2026-09-05

Logical ownership is not yet complete physical residency isolation. The
logger-free Level-1 graphics loader excludes Stormrail-only source bitmaps,
but `audioLoad()` still unconditionally loads the four Harrier samples
(9,812 requested Chip bytes), and the global 63-frame sprite conversion builds
one additional slot (3,200 requested Fast bytes). Shared generated plasma
patterns 7/8/9 also change Level-1 hostile impact pixels despite identical
runtime asset files. No cache-size or Blit-count increase follows from those
pixel differences. See `LEVEL1_PRODUCTION_BASELINE_AUDIT.md`; these are audit
findings, not changes made to accepted Stormrail assets or ownership lifetimes.

## Accepted Chip2 development baseline — 2026-09-06

The current source now omits beetle, Strider, Core and extra-life conversion
loads and their six associated cache families during Stormrail only. Shared
static-collectible preparation and all audio remain unchanged. Existing alpha.2
packages and their physical disk carryover lists are untouched; files remain
in the HD manifest because Level 1 still needs them. The user accepted Chip2 HD on a real A1200 on 2026-09-06;
approximately 1.45 MB free Chip RAM remains insufficient. See STORMRAIL_CHIP_RAM_AUDIT.md; this candidate does not claim full
section residency isolation or change the accepted media baseline.

## Title music candidate — 2026-09-06

neon-sky.lsmusic/lsbank are shared presentation assets in the development
manifest. Score uses Fast memory; bank uses Chip until the READY fade ends.
Neither belongs to gameplay residency. Released media are unchanged; new ADF
capacity and WHDLoad playback are unverified. See TITLE_AND_INTRO_MUSIC.md.

## Intro music — 7 September 2026

hero-drive.lsmusic/lsbank are shared presentation assets. Their one-shot lifetime
ends on story completion/skip, before title bank loading. No gameplay residency.
Current media baselines remain untouched. See TITLE_AND_INTRO_MUSIC.md.
