#!/usr/bin/env python3
"""Stage an independently verified, unnumbered three-disk ADF test set."""
import hashlib
import json
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / 'build/campaign-drowned/adf'
STAGE = ROOT / 'dist/Campaign-ADF-Disk3-Type'

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    assert not STAGE.exists(), 'candidate already exists; preserve it'
    proof = json.loads((SOURCE / 'media.json').read_text())
    assert len(proof['disks']) == 3
    STAGE.mkdir()
    for row in proof['disks']:
        disk = row['disk']
        source = SOURCE / f'Sparkpaw-Disk{disk}.adf'
        target = STAGE / source.name
        assert source.stat().st_size == 901120
        assert sha(source) == row['sha256']
        shutil.copy2(source, target)
        assert sha(target) == row['sha256']
    (STAGE / 'ReadMe.txt').write_text(
        'Sparkpaw three-disk campaign ADF test\n'
        '===================================\n\n'
        'Unnumbered candidate; alpha.8 remains official. PAL A1200/AGA, 68020, '
        '2 MB Chip, 8 MB Fast, no JIT. Boot Disk 1 in DF0. This ADF follows '
        'the accepted ADF title start; the HD/WHDLoad story plates are not on '
        'these disks. All gameplay images, effects and music are lossless.\n\n'
        'Disk 1: boot/title and Level 1. Disk 2: Stormrail. '
        'Disk 3: Drowned and results. ADF has no story intro or Soundtest, '
        'matching the established ADF presentation. '
        'Wait for INSERT DISK before changing DF0. With multiple drives, '
        'keep any requested disk in DF1 when possible; the loader checks both.\n\n'
        'Test cold boot, Options direct starts, Undertow Circuit in Drowned, '
        'Level 1 -> Continue -> Stormrail -> Continue -> Drowned, carried '
        'vitals/diamonds/score, Drowned Replay, Back to title and Escape. '
        'Music stays enabled. Please report wrong-disk, loading or sound '
        'failures and the exact disk/transition.\n\n'
        'First check: boot Disk 1 alone in DF0, select START AT DROWNED '
        'TURBINES, then START GAME. The loading screen should show an INSERT '
        'DISK 3 request. Swap Disk 3 into DF0 and check that gameplay starts. '
        'Repeat with Disk 1 in DF0 and Disk 3 already in DF1; no swap request '
        'is expected. If the screen stays black, note which disks are in '
        'DF0/DF1 and whether FS-UAE returns to Workbench.\n'
    , encoding='ascii')
    assert all(len(p.name) <= 30 for p in STAGE.iterdir())
    print(STAGE, [(r['disk'], r['free_blocks']) for r in proof['disks']])

if __name__ == '__main__': main()
