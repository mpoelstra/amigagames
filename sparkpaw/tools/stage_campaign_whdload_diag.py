#!/usr/bin/env python3
"""Stage a raw-asset WHDLoad diagnostic for Drowned direct-entry failures."""
import hashlib
import shutil
import struct
from pathlib import Path

from make_sparkpaw_icon import make_project_icon, make_readme_icon
from runtime_asset_refs import executable_runtime_files

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/campaign-drowned/whdload-diag'
HD = ROOT / 'dist/older-builds/Campaign-Drowned-020-HD/assets/runtime'
STAGE = ROOT / 'dist/Campaign-Drowned-WHD-Diag4'

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    assert not STAGE.exists(), 'preserve previous test candidate'
    exe = BUILD / 'Sparkpaw-Campaign'
    slave = ROOT / 'build/campaign-drowned/whdload/Sparkpaw.Slave'
    body = slave.read_bytes(); at = body.index(b'WHDLOADS')
    assert struct.unpack_from('>I', body, at + 28)[0] == 0x580000
    names = executable_runtime_files(exe)
    assert len(names) == 74 and set(names) == {p.name for p in HD.iterdir() if p.is_file()}
    data = STAGE / 'data/assets/runtime'
    data.mkdir(parents=True)
    shutil.copy2(exe, STAGE / 'data/Sparkpaw')
    for name in names:
        shutil.copy2(HD / name, data / name)
        assert sha(HD / name) == sha(data / name)
    shutil.copy2(slave, STAGE / 'Sparkpaw.Slave')
    (STAGE / 'Sparkpaw.info').write_bytes(make_project_icon(
        'WHDLoad', ['SLAVE=Sparkpaw.Slave', 'PAL']))
    (STAGE / 'ReadMe.txt.info').write_bytes(make_readme_icon())
    (STAGE / 'ReadMe.txt').write_text(
        'Sparkpaw Drowned WHDLoad diagnostic\n'
        '==================================\n\n'
        'Unnumbered raw-asset diagnostic; alpha.8 remains official. '
        'PAL 68020, 2 MB Chip, 8 MB Fast, no JIT. This candidate rounds '
        'Drowned rear row stride from 198 to 200 bytes for AGA 32-bit fetch. '
        'Start Sparkpaw.info, '
        'choose Options -> Start at Drowned Turbines once, then stop. '
        'The diagnostic writes data/whd-drowned-diag.log with each loading '
        'boundary and free/largest Chip and Fast blocks. Renderer details '
        'may also be in data/startupdiag.log. Please return both complete '
        'logs even if the game returns to Workbench. If no log exists, '
        'report that explicitly. Gameplay/presentation acceptance is not '
        'claimed by this diagnostic.\n', encoding='ascii')
    assert b'PRELOAD' not in (STAGE / 'Sparkpaw.info').read_bytes()
    assert all(len(part) <= 30 for p in STAGE.rglob('*') for part in p.relative_to(STAGE.parent).parts)
    print(STAGE, len(names), 'raw assets', sha(STAGE / 'data/Sparkpaw'))

if __name__ == '__main__': main()
