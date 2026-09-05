"""Discover literal Sparkpaw runtime asset paths embedded in an executable."""

import re
from pathlib import Path


RUNTIME_REFERENCE = re.compile(rb"PROGDIR:assets/runtime/([A-Za-z0-9._-]+)")


def executable_runtime_files(executable: Path) -> list[str]:
    return sorted(
        {match.decode("ascii") for match in RUNTIME_REFERENCE.findall(executable.read_bytes())}
    )
