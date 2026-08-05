#!/usr/bin/env python3
"""Install the selected Pixel Sprint Clean candidate as the title module."""

import runpy
import shutil
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
CANDIDATE_SCRIPT = ROOT / "tools" / "generate_futsal_title_candidates.py"
OUTPUT = ROOT / "music" / "mrdigs-futsal-title.mod"


def main():
    namespace = runpy.run_path(str(CANDIDATE_SCRIPT))
    candidate = next(
        item for item in namespace["VARIANTS"]
        if item["file"] == "mrdigs-futsal-title-pixel-sprint-clean.mod"
    )
    namespace["OUTPUT"].mkdir(parents=True, exist_ok=True)
    namespace["build"](candidate)
    source = namespace["OUTPUT"] / candidate["file"]
    shutil.copyfile(source, OUTPUT)
    print(f"Installed {source.name} as {OUTPUT}")


if __name__ == "__main__":
    main()
