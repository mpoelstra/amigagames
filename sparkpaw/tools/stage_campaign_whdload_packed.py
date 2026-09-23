#!/usr/bin/env python3
"""Stage a separate WHDLoad preload experiment; never alter accepted HD media."""
import hashlib
import json
import shutil
import struct
import subprocess
import sys
from pathlib import Path

from make_sparkpaw_icon import make_project_icon, make_readme_icon
from pack_adf_asset import pack as pack_rle, decode as decode_rle
from pack_disk_asset import pack as pack_lz, pack_delta, decode as decode_lz
from runtime_asset_refs import executable_runtime_files

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / 'build/campaign-drowned/whdload-packed'
HD = ROOT / 'dist/older-builds/Campaign-Drowned-020-HD/assets/runtime'
STAGE = ROOT / 'dist' / (sys.argv[1] if len(sys.argv) == 2 else 'Campaign-WHD-Cache-020')
ALIASES = {
    'game-over.spr1': 'game-over.spbm',
    'level-charge-patch.spr1': 'level-charge-patch.spbm',
    'level-complete.spr1': 'sparkpaw-level-complete.spbm',
    'level-ready-menu.spr1': 'readymenu.spbm',
    'level-ready.spr1': 'sparkpaw-ready-screen.spbm',
    'sparkpaw-level-loading.spr1': 'sparkpaw-level-loading.spbm',
    **{f'intro{i}.spr1': f'intro{i}.spbm' for i in range(1, 6)},
}
RAW = {'storm-collision.bin', 'drowned-route.bin'}

def sha(data):
    return hashlib.sha256(data).hexdigest()

def main():
    assert not STAGE.exists(), 'candidate already exists; preserve it'
    exe = SOURCE / 'Sparkpaw-Campaign'
    slave = (SOURCE / 'Sparkpaw.Slave').read_bytes()
    at = slave.index(b'WHDLOADS')
    assert struct.unpack_from('>I', slave, at + 28)[0] == 0x380000
    refs = executable_runtime_files(exe)
    assert len(refs) == 74
    assert {ALIASES.get(n, n) for n in refs} == {p.name for p in HD.iterdir() if p.is_file()}
    crunched = SOURCE / 'Sparkpaw-crunched'
    result = subprocess.run([str(ROOT / 'build/shrinkler/Shrinkler'), '-1', '-p',
                             str(exe), str(crunched)], text=True,
                            capture_output=True, check=True)
    assert 'Verifying... OK' in result.stdout
    reader = ROOT / 'build/multidisk-probe/tests/reader'
    assert reader.is_file()
    data = STAGE / 'data/assets/runtime'
    data.mkdir(parents=True)
    shutil.copy2(crunched, STAGE / 'data/Sparkpaw')
    report = {}
    for name in refs:
        source = HD / ALIASES.get(name, name)
        original = source.read_bytes()
        if name in RAW:
            body, codec = original, 'raw'
        else:
            options = [(pack_rle(original), 'SPR1'), (pack_lz(original), 'SPL1')]
            if name.endswith(('.lsbank', '-bank.bin')):
                options.append((pack_delta(original), 'SPD1'))
            body, codec = min(options, key=lambda item: len(item[0]))
            assert (decode_rle(body) if codec == 'SPR1' else decode_lz(body)) == original
        target = data / name
        target.write_bytes(body)
        if codec != 'raw':
            subprocess.run([str(reader), str(target), str(source), '1'], check=True,
                           stdout=subprocess.DEVNULL)
        assert sha(target.read_bytes()) == sha(body)
        report[name] = {'source': source.name, 'raw': len(original),
                        'stored': len(body), 'codec': codec, 'sha256': sha(body)}
    (STAGE / 'Sparkpaw.Slave').write_bytes(slave)
    (STAGE / 'Sparkpaw.info').write_bytes(make_project_icon(
        'WHDLoad', ['SLAVE=Sparkpaw.Slave', 'PRELOAD', 'PAL']))
    (STAGE / 'ReadMe.txt.info').write_bytes(make_readme_icon())
    (STAGE / 'ReadMe.txt').write_text(
        'Sparkpaw campaign WHDLoad packed preload experiment\n'
        '==================================================\n\n'
        'Unnumbered test candidate. Alpha.8 remains official. PAL 68020, '
        '2 MB Chip, 8 MB Fast, no JIT. Start using Sparkpaw.info. '
        'This variant requests 3.5 MB ExpMem and uses PRELOAD. The original '
        'HD artwork, music, gameplay and introduction decode losslessly.\n\n'
        'Soundtest update: PUMP SHOT and CHECKPOINT follow HEALTH PICKUP. '
        'Select each and press Fire to hear it. Then verify cold TITLE, '
        'direct Drowned entry and F10 still work. This candidate needs '
        'your FS-UAE playtest; real hardware is untested.\n',
        encoding='ascii')
    assert b'PRELOAD' in (STAGE / 'Sparkpaw.info').read_bytes()
    assert all(len(part) <= 30 for p in STAGE.rglob('*') for part in p.relative_to(STAGE.parent).parts)
    inventory = {str(p.relative_to(STAGE)): sha(p.read_bytes())
                 for p in STAGE.rglob('*') if p.is_file()}
    (SOURCE / 'packed-media.json').write_text(json.dumps({
        'exe_sha256': sha(exe.read_bytes()),
        'crunched_sha256': sha(crunched.read_bytes()),
        'raw_asset_bytes': sum(row['raw'] for row in report.values()),
        'stored_asset_bytes': sum(row['stored'] for row in report.values()),
        'assets': report, 'stage_inventory': inventory,
        'acceptance': 'technical only; FS-UAE and real hardware pending',
    }, indent=2) + '\n')
    print(STAGE, len(refs), 'assets', sum(row['stored'] for row in report.values()),
          'bytes; executable', crunched.stat().st_size, 'bytes')

if __name__ == '__main__':
    main()
