#!/usr/bin/env python3
"""Build and read back the unnumbered three-ADF campaign candidate."""
import hashlib
import json
import os
import subprocess
import sys
from pathlib import Path

from campaign_asset_manifest import (SHARED_PRESENTATION, SHARED_GAMEPLAY,
                                     LEVEL1, STORMRAIL, LEVEL1_HD_AUDIO,
                                     STORMRAIL_HD_AUDIO, DROWNED as DROWNED_FILES)
from pack_adf_asset import pack as pack_rle, decode as decode_rle
from pack_disk_asset import pack as pack_lz, pack_delta, decode as decode_lz
from pack_disk_asset_optimal import pack as pack_opt, pack_delta as pack_delta_opt
from runtime_asset_refs import executable_runtime_files
from campaign_runtime_sources import source as runtime_source

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'build/campaign-drowned/adf'
STATUS = OUT / 'status'
sys.path.insert(0, str(ROOT / '.toolchain/amitools'))
from amitools.fs.blkdev.BlkDevFactory import BlkDevFactory
from amitools.fs.ADFSVolume import ADFSVolume
from amitools.fs.FSString import FSString

ALIASES = {
    'readymenu.spbm': 'level-ready-menu.spr1',
    'sparkpaw-ready-screen.spbm': 'level-ready.spr1',
    'sparkpaw-level-complete.spbm': 'level-complete.spr1',
}
DIRECT_RAW = {'storm-collision.bin', 'drowned-route.bin'}
WIDE_PACK = {'sparkpaw-sprites4.spbm', 'storm-rear.spbm',
             'sparkpaw-title.spbm', 'sparkpaw-ready-screen.spbm',
             'sparkpaw-level-complete.spbm', 'clockwork-storm-strider.spbm',
             'sparkpaw-level-loading.spbm', 'stormstone-core.spbm',
             'l1-electric.bin'}
MENU = {'sparkpaw-title.spbm', 'sparkpaw-level-loading.spbm',
        'level-charge-patch.spbm', 'sparkpaw-ready-screen.spbm',
        'readymenu.spbm', 'neon-sky.lsmusic', 'neon-sky.lsbank',
        'ready-dust-mask.bin'}
RESULTS = {'sparkpaw-level-complete.spbm', 'sparkpaw-score-glyphs.spbm',
           'game-over.spbm', 'tally-tick.raw'}
TRACKS = {'hero-drive.lsmusic', 'hero-drive.lsbank',
          'storm-light.lsmusic', 'storm-light.lsbank',
          'pulse-score.bin', 'pulse-bank.bin', 'rail-score.bin',
          'rail-bank.bin', 'rain-score.bin', 'rain-bank.bin'}
EFFECTS = {name for name in (set(SHARED_PRESENTATION) | set(SHARED_GAMEPLAY) |
                          set(LEVEL1) | set(STORMRAIL) | set(DROWNED_FILES))
           if name.endswith('.raw')}
DROWNED = {'checkpoint.spbm', 'drowned-patches.spbm', 'drowned-rear.spbm',
           'drowned-route.bin', 'drowned-route.spbm', 'pump-walker.spbm',
           'rain-core.spbm', 'spillwing.spbm', 'turbine-crab.spbm',
           'sparkpaw-sprites4.spbm', 'sparkpaw-extra-life.spbm',
           'stormstone-core.spbm', 'rain-score.bin', 'rain-bank.bin'}
PATCHES = {f'disk{i}-patch.spbm' for i in range(1, 4)}
COMMON = ((set(SHARED_PRESENTATION) - {'readymenu.spbm'}) -
          {n for n in SHARED_PRESENTATION if n.startswith(('intro', 'hero-drive.'))}) | set(SHARED_GAMEPLAY) | PATCHES | {'ready-dust-mask.bin'}
SETS = [
    (COMMON - {'disk1-patch.spbm'}) | set(LEVEL1) | set(LEVEL1_HD_AUDIO),
    COMMON | set(STORMRAIL) | set(STORMRAIL_HD_AUDIO) |
    (set(LEVEL1) - {'l1-electric.bin', 'storm-front.spbm', 'storm-rear.spbm',
                    'sparkpaw-sprites4.spbm'}),
    DROWNED | set(SHARED_GAMEPLAY) |
    (EFFECTS - {n for n in EFFECTS if n.startswith('harrier-')} -
     {'strider-shot.raw'}) |
    {'drowned-amb.bin', 'pontoon-clip.bin', 'sparkpaw-level-loading.spbm',
     'storm-light.lsmusic', 'storm-light.lsbank'} | RESULTS | PATCHES,
]

def verify_phase_dependencies():
    """Check cold-start and transition files against each individual disk."""
    harrier = {n for n in EFFECTS if n.startswith('harrier-')}
    level1 = (COMMON - {'disk1-patch.spbm'}) | set(LEVEL1) | set(LEVEL1_HD_AUDIO)
    stormrail = (COMMON | set(STORMRAIL) | set(STORMRAIL_HD_AUDIO) |
                 (set(LEVEL1) - {'l1-electric.bin', 'storm-front.spbm', 'storm-rear.spbm',
                                 'sparkpaw-sprites4.spbm'}))
    drowned = (DROWNED | set(SHARED_GAMEPLAY) | (EFFECTS - harrier -
               {'strider-shot.raw'}) | {'drowned-amb.bin', 'pontoon-clip.bin',
               'sparkpaw-level-loading.spbm', 'storm-light.lsmusic',
               'storm-light.lsbank'} | RESULTS | PATCHES)
    for disk, required in enumerate((level1, stormrail, drowned)):
        missing = required - SETS[disk]
        assert not missing, ('disk phase dependencies', disk + 1, sorted(missing))
    assert not (harrier & SETS[0]), 'Level 1 cold load must skip Harrier samples'
    assert harrier <= SETS[1], 'Stormrail must load all Harrier samples'
    assert not (harrier & SETS[2]), 'Drowned cold load must skip Harrier samples'
    assert {'sparkpaw-level-loading.spbm', 'disk1-patch.spbm'} <= SETS[1]
    assert {'sparkpaw-level-loading.spbm', 'disk1-patch.spbm'} <= SETS[2]

def sha(data):
    return hashlib.sha256(data).hexdigest()

def status_assets():
    STATUS.mkdir(parents=True, exist_ok=True)
    source = ROOT / 'build/multidisk-probe/status'
    for i in (1, 2, 3):
        (STATUS / f'disk{i}-patch.spbm').write_bytes(
            (source / f'disk{i}-patch.spbm').read_bytes())
    header = (ROOT / 'src/ready_dust_mask.h').read_text()
    import re
    mask = bytes(int(x) for a in re.findall(r'=\{(.*?)\};', header, re.S)
                 for x in re.findall(r'\d+', a))
    assert len(mask) == 40192
    (STATUS / 'ready-dust-mask.bin').write_bytes(mask)

def payload(name):
    source = STATUS / name if name in PATCHES or name == 'ready-dust-mask.bin' else runtime_source(name)
    raw = source.read_bytes()
    if name in DIRECT_RAW:
        return name, raw, source, 'raw'
    options = [pack_lz(raw), pack_rle(raw), pack_opt(raw)]
    if name in WIDE_PACK:
        options.append(pack_opt(raw,candidates=256))
    if name.endswith(('.lsbank', '-bank.bin')):
        options.extend((pack_delta(raw), pack_delta_opt(raw)))
    data = min(options, key=len)
    assert (decode_lz(data) if data[:4] in (b'SPL1', b'SPD1')
            else decode_rle(data)) == raw
    target = ALIASES.get(name, name[:-5] + '.spr1' if name.endswith('.spbm') else name)
    return target, data, source, data[:4].decode('ascii')

def main():
    global OUT, STATUS
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir', type=Path)
    args = parser.parse_args()
    if args.build_dir:
        OUT = args.build_dir.resolve()
        STATUS = OUT / 'status'
    verify_phase_dependencies()
    status_assets()
    exe = OUT / 'Sparkpaw-crunched'
    source_exe = OUT / 'Sparkpaw-Campaign'
    assert source_exe.is_file()
    crunch = subprocess.run([str(ROOT / 'build/shrinkler/Shrinkler'), '-3', '-p',
                             str(source_exe), str(exe)], text=True,
                            capture_output=True, check=True)
    assert 'Verifying... OK' in crunch.stdout
    (OUT / 'shrinkler.log').write_text(crunch.stdout + crunch.stderr)
    refs = set(executable_runtime_files(OUT / 'Sparkpaw-Campaign'))
    logical = set().union(*SETS)
    missing = {name for name in refs if name not in logical and
               not (name.endswith('.spr1') and name[:-5] + '.spbm' in logical) and
               name not in set(ALIASES.values())}
    assert not missing, ('unpackaged executable references', sorted(missing))
    data = {name: payload(name) for name in sorted(logical)}
    reader = ROOT / 'build/multidisk-probe/tests/reader'
    assert reader.is_file()
    packed = OUT / 'packed'; packed.mkdir(exist_ok=True)
    for name, (target, body, source, codec) in data.items():
        if codec != 'raw':
            p = packed / target; p.write_bytes(body)
            subprocess.run([str(reader), str(p), str(source), '1'], check=True,
                           stdout=subprocess.DEVNULL)
    env = dict(os.environ, PYTHONPATH=str(ROOT / '.toolchain/amitools'))
    reports = []
    for disk, names in enumerate(SETS, 1):
        files = {'Sparkpaw.disk': f'SP09D{disk}\n'.encode('ascii')}
        if disk == 1:
            files = {'Sparkpaw': exe.read_bytes(),
                     'S/startup-sequence': b'Sparkpaw\n', **files}
        # Stable, source-derived order: title/loader first, then section data.
        for name in sorted(names):
            target, body, _, _ = data[name]
            files[f'assets/runtime/{target}'] = body
        assert len(files) == len(names) + (3 if disk == 1 else 1)
        adf = OUT / f'Sparkpaw-Disk{disk}.adf'
        cmd = [sys.executable, '-m', 'amitools.tools.xdftool', '-f', str(adf),
               'format', f'SP09D{disk}', 'DOS1']
        if disk == 1: cmd += ['+', 'boot', 'install']
        for directory in (['S'] if disk == 1 else []) + ['assets', 'assets/runtime']:
            cmd += ['+', 'makedir', directory]
        subprocess.run(cmd, env=env, check=True, stdout=subprocess.DEVNULL)
        dev = BlkDevFactory().open(str(adf), read_only=False)
        volume = ADFSVolume(dev); volume.open(); volume.bitmap.find_start_off = 0
        for target, body in files.items():
            volume.write_file(body, FSString(target))
        volume.close(); dev.close()
        assert adf.stat().st_size == 901120
        dev = BlkDevFactory().open(str(adf), read_only=True)
        volume = ADFSVolume(dev); volume.open()
        assert volume.is_ffs and volume.boot.dos_type == 0x444f5301
        if disk == 1: assert volume.boot.valid_chksum and volume.boot.boot_code
        seen = set()
        def walk(node):
            for child in node.get_entries():
                if child.is_dir(): walk(child)
                else:
                    path = str(child.get_node_path_name())
                    assert bytes(child.get_file_data()) == files[path], path
                    seen.add(path)
        walk(volume.get_root_dir())
        assert seen == set(files)
        report = {'disk': disk, 'sha256': sha(adf.read_bytes()),
                  'free_blocks': volume.get_free_blocks(),
                  'used_blocks': volume.get_used_blocks(),
                  'file_count': len(files), 'payload_bytes': sum(map(len, files.values())),
                  'files': {name: sha(body) for name, body in files.items()}}
        assert report['free_blocks'] >= 16, report
        reports.append(report); dev.close()
    (OUT / 'media.json').write_text(json.dumps({
        'executable_sha256': sha(source_exe.read_bytes()),
        'crunched_sha256': sha(exe.read_bytes()),
        'disks': reports,
        'acceptance': 'technical readback only; native play and hardware unproven',
    }, indent=2) + '\n')
    print([(r['disk'], r['free_blocks'], r['file_count']) for r in reports])

if __name__ == '__main__': main()
