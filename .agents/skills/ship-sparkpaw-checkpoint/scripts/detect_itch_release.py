#!/usr/bin/env python3
"""Print the latest version present in Sparkpaw's public itch downloads."""

from __future__ import annotations

import argparse
import re
import sys
import urllib.request


DEFAULT_URL = "https://mrdig.itch.io/sparkpaw"
DOWNLOAD_RE = re.compile(
    rb"Sparkpaw-(\d+)\.(\d+)\.(\d+)-alpha\.(\d+)"
    rb"(?:-WHDLoad)?\.(?:adf|lha|zip)",
    re.IGNORECASE,
)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--url", default=DEFAULT_URL)
    args = parser.parse_args()
    request = urllib.request.Request(
        args.url,
        headers={"User-Agent": "Sparkpaw release baseline checker"},
    )
    try:
        with urllib.request.urlopen(request, timeout=20) as response:
            page = response.read()
    except Exception as error:
        print(f"itch baseline unavailable: {error}", file=sys.stderr)
        return 2
    versions = {tuple(map(int, match)) for match in DOWNLOAD_RE.findall(page)}
    if not versions:
        print("itch baseline unavailable: no versioned Sparkpaw downloads found",
              file=sys.stderr)
        return 3
    major, minor, patch, alpha = max(versions)
    print(f"{major}.{minor}.{patch}-alpha.{alpha}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
