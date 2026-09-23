#!/usr/bin/env python3
"""Stage the unnumbered three-section WHDLoad test without touching releases."""
import hashlib
import shutil
from pathlib import Path
from runtime_asset_refs import executable_runtime_files
from make_sparkpaw_icon import make_project_icon

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / 'build/campaign-drowned/whdload/Sparkpaw-Campaign'
HD = ROOT / 'dist/older-builds/Campaign-Drowned-020-HD/assets/runtime'
STAGE = ROOT / 'dist/Campaign-Drowned-WHD-Fix'
SLAVE_STAGE = ROOT / 'build/whdload/Sparkpaw-0.7.0-a8-WHDLoad'
SLAVE = ROOT / 'build/campaign-drowned/whdload/Sparkpaw.Slave'

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    assert SOURCE.is_file() and SLAVE_STAGE.is_dir() and SLAVE.is_file()
    assert not STAGE.exists(), 'candidate already exists; preserve it'
    names = executable_runtime_files(SOURCE)
    assert names and set(names) == {p.name for p in HD.iterdir() if p.is_file()}
    data = STAGE / 'data/assets/runtime'
    data.mkdir(parents=True)
    shutil.copy2(SOURCE, STAGE / 'data/Sparkpaw')
    for name in names:
        source = HD / name
        assert source.is_file() and len(name) <= 30
        shutil.copy2(source, data / name)
        assert digest(source) == digest(data / name)
    shutil.copy2(SLAVE, STAGE / 'Sparkpaw.Slave')
    shutil.copy2(SLAVE_STAGE / 'ReadMe.txt.info', STAGE / 'ReadMe.txt.info')
    (STAGE / 'Sparkpaw.info').write_bytes(make_project_icon(
        'WHDLoad', ['SLAVE=Sparkpaw.Slave', 'PAL']))
    (STAGE / 'ReadMe.txt').write_text(
        'Sparkpaw three-section WHDLoad test candidate\n'
        '=============================================\n\n'
        'Unnumbered test; alpha.8 remains the release. Requires PAL A1200/AGA, '
        '68020, 2 MB Chip, 8 MB Fast, WHDLoad 19+ and your legal Kickstart 3.1 '
        'A1200 ROM/RTB. Start with Sparkpaw.info or WHDLoad Sparkpaw.Slave. '
        'F10 exits. This candidate reserves 5 MB game Fast plus 512 KB Kickstart '
        'and starts without PRELOAD.\n\n'
        'Test Level 1 -> Continue -> Stormrail -> Continue -> Drowned, carried '
        'vitals/score, Drowned Replay and Back to title. Also start Drowned from '
        'Options and play Undertow Circuit in Soundtest. Music stays enabled.\n'
    , encoding='ascii')
    for path in STAGE.rglob('*'):
        assert all(len(part) <= 30 for part in path.relative_to(STAGE.parent).parts)
    assert set(executable_runtime_files(STAGE / 'data/Sparkpaw')) == {p.name for p in data.iterdir()}
    print(STAGE, len(names), 'assets', digest(STAGE / 'data/Sparkpaw'))

if __name__ == '__main__':
    main()
