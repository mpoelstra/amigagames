#!/usr/bin/env python3
"""Independent archive, disk and inventory checks for alpha.9."""
import hashlib
import json
import re
import struct
import subprocess
import sys
import tempfile
import zipfile
from pathlib import Path

from make_release import DIST, RELEASE_NAME, RELEASE_VERSION, ROOT, RUNTIME_FILES
from make_sparkpaw_icon import make_project_icon
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


def verify_archive(drawer, artifact):
    expected = {p.relative_to(drawer).as_posix(): p.read_bytes()
                for p in drawer.rglob('*') if p.is_file()}
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
    report = json.loads((ROOT / 'build/campaign-drowned/adf/media.json').read_text())
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
    whd = DIST / 'Sparkpaw-0.7.0-a9-WHDLoad'
    assert (hd / 'Sparkpaw').read_bytes() == (
        ROOT / 'build/campaign-drowned/Sparkpaw-Campaign').read_bytes()
    assert (hd / 'Sparkpaw.info').read_bytes() == make_project_icon('Sparkpaw', [])
    assert set(executable_runtime_files(hd / 'Sparkpaw')) == set(RUNTIME_FILES)
    assert {p.name for p in (hd / 'assets/runtime').iterdir() if p.is_file()} == set(RUNTIME_FILES)
    for name in RUNTIME_FILES:
        assert (hd / 'assets/runtime' / name).read_bytes() == runtime_source(name).read_bytes()
    slave = (whd / 'Sparkpaw.Slave').read_bytes()
    assert f'Version {RELEASE_VERSION}'.encode() in slave
    assert struct.unpack_from('>I', slave, slave.index(b'WHDLOADS') + 28)[0] == 0x380000
    assert (whd / 'Sparkpaw.info').read_bytes() == make_project_icon(
        'WHDLoad', ['SLAVE=Sparkpaw.Slave', 'PRELOAD', 'PAL'])
    whd_refs = set(executable_runtime_files(ROOT / 'build/campaign-drowned/whdload-packed/Sparkpaw-Campaign'))
    assert {ALIASES.get(name, name) for name in whd_refs} == set(RUNTIME_FILES)
    assert {p.name for p in (whd / 'data/assets/runtime').iterdir() if p.is_file()} == whd_refs
    for name in whd_refs:
        body = (whd / 'data/assets/runtime' / name).read_bytes()
        decoded = (decode_rle(body) if body[:4] == b'SPR1' else
                   decode_lz(body) if body[:4] in (b'SPL1', b'SPD1') else body)
        assert decoded == runtime_source(ALIASES.get(name, name)).read_bytes(), name
    counts = {'HD': verify_archive(hd, RELEASE_NAME),
              'WHDLoad': verify_archive(whd, f'{RELEASE_NAME}-WHDLoad')}
    verify_adf()
    paths = [DIST / f'{RELEASE_NAME}{suffix}' for suffix in
             ('.zip', '.lha', '-WHDLoad.zip', '-WHDLoad.lha',
              '-Disk1.adf', '-Disk2.adf', '-Disk3.adf')]
    result = {'version': RELEASE_VERSION, 'archive_file_counts': counts,
              'three_adfs_read_back': True,
              'artifacts': {p.name: {'bytes': p.stat().st_size,
                                     'sha256': sha(p.read_bytes())} for p in paths}}
    (ROOT / 'build/checkpoint-release-verification.json').write_text(
        json.dumps(result, indent=2) + '\n')
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
