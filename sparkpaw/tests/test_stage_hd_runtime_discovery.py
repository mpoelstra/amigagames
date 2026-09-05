#!/usr/bin/env python3
"""Prevent focused HD drawers from omitting compile-guarded runtime assets."""

import importlib.util
import sys
import tempfile
from pathlib import Path


ROOT = Path(__file__).parents[1]
sys.path.insert(0, str(ROOT / "tools"))
module_path = ROOT / "tools" / "runtime_asset_refs.py"
spec = importlib.util.spec_from_file_location("runtime_asset_refs", module_path)
runtime_asset_refs = importlib.util.module_from_spec(spec)
assert spec.loader is not None
spec.loader.exec_module(runtime_asset_refs)

with tempfile.TemporaryDirectory() as directory:
    executable = Path(directory) / "Sparkpaw-Test"
    executable.write_bytes(
        b"binary\0PROGDIR:assets/runtime/stormrail-front.spbm\0"
        b"PROGDIR:assets/runtime/stormrail-family.spbm\0"
        b"PROGDIR:assets/runtime/stormrail-front.spbm\0"
    )

    assert runtime_asset_refs.executable_runtime_files(executable) == [
        "stormrail-family.spbm",
        "stormrail-front.spbm",
    ]

stormrail_build = ROOT / "build" / "sparkpaw-stormrail-test"
if stormrail_build.is_file():
    discovered = set(runtime_asset_refs.executable_runtime_files(stormrail_build))
    assert {
        "stormrail-family.spbm",
        "stormrail-flight-rear.spbm",
        "stormrail-front.spbm",
        "stormrail-rear.spbm",
    } <= discovered

print("PASS: HD staging discovers compile-guarded runtime assets")
