#!/usr/bin/env python3
"""Every integrated asset has exactly one owner and an Amiga-safe name."""
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from campaign_asset_manifest import ALL, GROUPS, LEVEL1, STORMRAIL  # noqa: E402
from runtime_asset_refs import executable_runtime_files  # noqa: E402

seen = set()
for group, names in GROUPS.items():
    overlap = seen & names
    assert not overlap, f"assets have multiple owners ({group}): {sorted(overlap)}"
    seen |= names
for name in ALL:
    assert len(name) <= 30, f"Amiga-unsafe asset name: {name}"
    assert (ROOT / "assets/runtime" / name).is_file(), f"missing owned asset: {name}"

campaign = ROOT / "build/sparkpaw-campaign-test"
if campaign.is_file():
    refs = set(executable_runtime_files(campaign))
    assert refs == ALL, (f"campaign refs without ownership={sorted(refs-ALL)}; "
                         f"owned but unreachable={sorted(ALL-refs)}")

source = (ROOT / "src/assets.c").read_text()
campaign_load = source.split("#ifdef SPARKPAW_CAMPAIGN", 1)[1].split(
    "#else\n#ifdef ADF_PACKED_ASSETS", 1)[0]
boundary = campaign_load.index("if(!loadStormrailGameplay) return TRUE;")
before, after = campaign_load[:boundary], campaign_load[boundary:]
for name in ("storm-front.spbm", "storm-rear.spbm", "sparkpaw-sprites4.spbm"):
    assert name in before
for name in ("stormrail-flight-rear.spbm", "stormrail-heart.spbm",
             "stormrail-family.spbm", "stormrail-obstacles.spbm"):
    assert name not in before and name in after
assert LEVEL1.isdisjoint(STORMRAIL)
print("PASS: all campaign assets have one explicit, Amiga-safe owner")
