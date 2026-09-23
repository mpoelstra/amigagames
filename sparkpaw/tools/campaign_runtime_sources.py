"""Resolve the 74 approved campaign assets from reproducible build inputs."""
from pathlib import Path

from campaign_asset_manifest import HD_ALL

ROOT = Path(__file__).resolve().parents[1]


def source(name: str) -> Path:
    if name not in HD_ALL:
        raise ValueError(f"asset outside campaign inventory: {name}")
    for parent in (ROOT / 'assets/runtime', ROOT / 'build/drowned-full/assets'):
        path = parent / name
        if path.is_file():
            return path
    raise FileNotFoundError(f"campaign asset was not built: {name}")
