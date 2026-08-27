#!/usr/bin/env python3
"""Protect the dual-layer HD/WHDLoad project-icon release contract."""
from __future__ import annotations

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / ".toolchain" / "amigainfo"))

from amigainfo import load
from make_sparkpaw_icon import make_project_icon


def inspect(data: bytes, default_tool: str, expected_tooltypes: list[str]):
    icon = load(data)
    assert icon.default_tool == default_tool
    assert icon.tooltypes[:len(expected_tooltypes)] == expected_tooltypes
    assert icon.classic is not None
    assert icon.classic.normal.header.width == 86
    assert icon.classic.normal.header.height == 93
    assert icon.classic.normal.header.depth == 3
    assert icon.classic.selected is None
    assert icon.newicon is not None
    assert icon.newicon.normal.width == 86
    assert icon.newicon.normal.height == 93
    assert len(icon.newicon.normal.palette) == 34
    assert icon.newicon.selected is None
    return icon


hd = inspect(make_project_icon("Sparkpaw", []), "Sparkpaw", [])
whdload = inspect(
    make_project_icon("WHDLoad", ["SLAVE=Sparkpaw.Slave", "PRELOAD", "PAL"]),
    "WHDLoad",
    ["SLAVE=Sparkpaw.Slave", "PRELOAD", "PAL"],
)
assert hd.classic.normal.planes == whdload.classic.normal.planes
assert hd.newicon.normal.palette == whdload.newicon.normal.palette
assert hd.newicon.normal.pixel_data == whdload.newicon.normal.pixel_data
print("PASS: HD and WHDLoad icons retain matching 8/34-colour image layers")
