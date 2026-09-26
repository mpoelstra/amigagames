#!/usr/bin/env python3
"""Independent archive, disk and inventory checks for the current release."""
import hashlib
import json
import re
import struct
import subprocess
import sys
import tempfile
import zipfile
from pathlib import Path

from game_readme import game_readme, personal_note
from make_release import DIST, RELEASE_NAME, RELEASE_VERSION, ROOT, RUNTIME_FILES
from make_sparkpaw_icon import make_project_icon
from make_campaign_release import BUILD, WHD_SHORT, HIGH_SHORT
from whd_bank_format import unpack
from runtime_asset_refs import executable_runtime_files
from stage_campaign_whdload_packed import ALIASES
from campaign_runtime_sources import source as runtime_source
from pack_adf_asset import decode as decode_rle
from pack_disk_asset import decode as decode_lz

sys.path.insert(0, str(ROOT / '.toolchain/amitools'))
from amitools.fs.blkdev.BlkDevFactory import BlkDevFactory
from amitools.fs.ADFSVolume import ADFSVolume
from amitools.fs.FSString import FSString


def sha(data):
    return hashlib.sha256(data).hexdigest()


def verify_archive(drawer, artifact, edition):
    expected = {p.relative_to(drawer).as_posix(): p.read_bytes()
                for p in drawer.rglob('*') if p.is_file()}
    readme = expected['ReadMe.txt'].decode('ascii')
    assert readme == game_readme(RELEASE_VERSION, edition)
    assert readme.startswith(personal_note() + '\n\n')
    assert max(map(len, readme.splitlines())) <= 80
    for section in ('WELCOME','THE STORY','IN THIS ALPHA','REQUIREMENTS',
                    'INSTALLATION','CONTROLS','ABOUT THE CREATOR',
                    'DOWNLOADS, NEWS AND FEEDBACK','ALPHA STATUS'):
        assert section in readme, section
    assert 'https://mrdig.itch.io/sparkpaw' in readme
    assert 'mrdigamiga@outlook.com' in readme
    assert 'ReleaseNotes.txt' in expected
    assert all(all(len(part) <= 30 for part in Path(name).parts)
               for name in expected)
    with zipfile.ZipFile(DIST / f'{artifact}.zip') as archive:
        assert archive.testzip() is None
        names = {name for name in archive.namelist() if not name.endswith('/')}
        assert names == {f'{drawer.name}/{name}' for name in expected}
        assert all(all(len(part) <= 30 for part in Path(name).parts)
                   for name in archive.namelist())
        for name, body in expected.items():
            assert archive.read(f'{drawer.name}/{name}') == body, name
    lha = DIST / f'{artifact}.lha'
    creator = ROOT / '.toolchain/lha/bin/lha'
    subprocess.run([str(creator), 'tq2', str(lha)], check=True,
                   stdout=subprocess.DEVNULL)
    listing = subprocess.check_output(['/opt/homebrew/bin/lha', 'v', str(lha)], text=True)
    methods = re.findall(r'-lh[0-9d]-', listing)
    assert '-lh5-' in methods
    stored = [line for line in listing.splitlines() if '-lh0-' in line]
    allowed_stored = {
        'assets/runtime/tally-tick.raw',
        'data/assets/runtime/tally-tick.raw',
        'data/assets/runtime/collect-spark.raw',
        'data/Sparkpaw',  # Shrinkler output is already compressed.
    }
    assert all(any(line.endswith('/' + name) for name in allowed_stored)
               for line in stored), stored
    assert set(methods) <= {'-lh5-', '-lh0-', '-lhd-'}
    with tempfile.TemporaryDirectory(dir=ROOT / 'build') as temp:
        subprocess.run(['/opt/homebrew/bin/lha', 'x', str(lha)], cwd=temp,
                       check=True, stdout=subprocess.DEVNULL)
        extracted = Path(temp) / drawer.name
        assert {p.relative_to(extracted).as_posix() for p in extracted.rglob('*')
                if p.is_file()} == set(expected)
        for name, body in expected.items():
            assert (extracted / name).read_bytes() == body, name
    return len(expected)


def verify_adf():
    report = json.loads((BUILD / 'adf/media.json').read_text())
    assert len(report['disks']) == 3
    for row in report['disks']:
        disk = row['disk']
        adf = DIST / f'{RELEASE_NAME}-Disk{disk}.adf'
        assert adf.stat().st_size == 901120
        # ADFSVolume writes volume timestamps during a rebuild. The versioned
        # image is retained byte-exactly; compare every stored file instead.
        device = BlkDevFactory().open(str(adf), read_only=True)
        volume = ADFSVolume(device)
        volume.open()
        assert volume.is_ffs and volume.boot.dos_type == 0x444f5301
        if disk == 1:
            assert volume.boot.valid_chksum and volume.boot.boot_code
        seen = set()
        def walk(node):
            for child in node.get_entries():
                if child.is_dir():
                    walk(child)
                else:
                    seen.add(str(child.get_node_path_name()))
        walk(volume.get_root_dir())
        assert seen == set(row['files'])
        for name, checksum in row['files'].items():
            node = volume.get_file_path_name(FSString(name))
            assert node is not None and sha(bytes(node.get_file_data())) == checksum
            assert all(len(part) <= 30 for part in Path(name).parts)
        assert volume.get_free_blocks() == row['free_blocks'] >= 16
        device.close()


def main():
    hd = DIST / RELEASE_NAME
    whd = DIST / WHD_SHORT
    high = DIST / HIGH_SHORT
    assert (hd / 'Sparkpaw').read_bytes() == (BUILD / 'hd/Sparkpaw-Campaign').read_bytes()
    assert (hd / 'Sparkpaw.info').read_bytes() == make_project_icon('Sparkpaw', [])
    assert set(executable_runtime_files(hd / 'Sparkpaw')) == set(RUNTIME_FILES)
    assert {p.name for p in (hd / 'assets/runtime').iterdir()} == set(RUNTIME_FILES)
    for name in RUNTIME_FILES:
        raw = runtime_source(name).read_bytes()
        assert (hd / 'assets/runtime' / name).read_bytes() == raw
        assert (high / 'data/assets/runtime' / name).read_bytes() == raw
    for stage, variant, types in (
        (whd, 'banks', ['SLAVE=Sparkpaw.Slave', 'PAL', 'NOCACHE']),
        (high, 'highram', ['SLAVE=Sparkpaw.Slave', 'PRELOAD', 'PAL'])):
        slave = (stage / 'Sparkpaw.Slave').read_bytes()
        assert f'Version {RELEASE_VERSION}'.encode() in slave
        at = slave.index(b'WHDLOADS')
        assert struct.unpack_from('>I', slave, at + 28)[0] == 0x580000
        dontcache = struct.unpack_from('>H', slave, at + 24)[0]
        if variant == 'banks':
            assert slave[at-4+dontcache:at-4+dontcache+3] == b'#?\0'
        else:
            assert dontcache == 0
        assert (stage / 'Sparkpaw.info').read_bytes() == make_project_icon('WHDLoad', types)
        exe = stage / 'data/Sparkpaw'
        assert exe.read_bytes() == (BUILD / variant / 'Sparkpaw-Campaign').read_bytes()
        meta = json.loads((BUILD / variant / 'build.json').read_text())
        assert not any('LOAD_TRACE' in str(a) or 'LOAD_STATE' in str(a)
                       for a in meta['command'] + meta['module_command'])
        assert b'load-times.log' not in exe.read_bytes()
    entries = {}
    for group in ('common', 'intro', 'level1', 'level2', 'level3'):
        files = unpack((whd / 'data' / (group + '.spb')).read_bytes())
        assert not set(entries) & set(files)
        entries.update(files)
    whd_refs = set(executable_runtime_files(whd / 'data/Sparkpaw'))
    assert set(entries) == whd_refs
    assert {ALIASES.get(name, name) for name in whd_refs} == set(RUNTIME_FILES)
    for name, body in entries.items():
        assert body == runtime_source(ALIASES.get(name, name)).read_bytes(), name
    assert {p.name for p in (whd / 'data').iterdir()} == {
        'Sparkpaw', 'common.spb', 'intro.spb', 'level1.spb', 'level2.spb', 'level3.spb'}
    assert {p.name for p in (high / 'data/assets/runtime').iterdir()} == set(RUNTIME_FILES)
    high_preload = sum(p.stat().st_size for p in (high / 'data').rglob('*') if p.is_file())
    # Explicit reservation plus raw preload leaves several MiB for the host at 16 MiB.
    assert 0x580000 + high_preload < 13 * 1024 * 1024
    counts = {'HD': verify_archive(hd, RELEASE_NAME, 'hd'),
              'WHDLoad': verify_archive(whd, f'{RELEASE_NAME}-WHDLoad', 'whd'),
              'WHDLoad-HighRAM': verify_archive(high, f'{RELEASE_NAME}-WHDLoad-HighRAM', 'high')}
    verify_adf()
    paths = [DIST / f'{RELEASE_NAME}{suffix}' for suffix in
             ('.zip', '.lha', '-WHDLoad.zip', '-WHDLoad.lha',
              '-WHDLoad-HighRAM.zip', '-WHDLoad-HighRAM.lha',
              '-Disk1.adf', '-Disk2.adf', '-Disk3.adf')]
    result = {'version': RELEASE_VERSION, 'archive_file_counts': counts,
              'readme_all_six_archives_verified': True,
              'three_adfs_read_back': True, 'trace_free': True,
              'bank_entries_raw_verified': len(entries), 'highram_preload_bytes': high_preload,
              'highram_reservation_plus_preload_bytes': 0x580000 + high_preload,
              'highram_budget_is_not_playtest': True,
              'artifacts': {p.name: {'bytes': p.stat().st_size,
                                     'sha256': sha(p.read_bytes())} for p in paths}}
    (BUILD / 'checkpoint-release-verification.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
