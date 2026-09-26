#!/usr/bin/env python3
"""Stage a single unnumbered 8-MiB WHDLoad loading candidate, never a release."""
import argparse
import hashlib
import json
import shutil
import struct
import subprocess
import zipfile
from pathlib import Path

from campaign_asset_manifest import HD_ALL, LEVEL1, LEVEL1_HD_AUDIO, SHARED_GAMEPLAY
from campaign_runtime_sources import source
from make_sparkpaw_icon import make_project_icon, make_readme_icon
from pack_adf_asset import decode as decode_rle, pack as pack_rle
from pack_disk_asset import decode as decode_lz, pack as pack_lz
from runtime_asset_refs import executable_runtime_files
from stage_campaign_whdload_packed import ALIASES
from stage_hd_test import release_inventory

ROOT = Path(__file__).resolve().parents[1]
# Keep the high-ratio foreground/strider packed. Prioritize sequential intro,
# menu return and readiness, without pre-decoding a second resident asset set.
RAW_NAMES = ((SHARED_GAMEPLAY | LEVEL1 | LEVEL1_HD_AUDIO | {
    *(f'intro{i}.spbm' for i in range(1, 6)),
    'sparkpaw-title.spbm', 'sparkpaw-level-loading.spbm',
    'level-charge-patch.spbm', 'sparkpaw-ready-screen.spbm', 'readymenu.spbm',
}) - {'storm-front.spbm', 'clockwork-storm-strider.spbm'})


def sha(body):
    return hashlib.sha256(body).hexdigest()


def decoded(body):
    return (decode_rle(body) if body[:4] == b'SPR1' else
            decode_lz(body) if body[:4] in (b'SPL1', b'SPD1') else body)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir', required=True, type=Path)
    parser.add_argument('--reader', required=True, type=Path)
    parser.add_argument('--packed-decoder', action='store_true',
                        help='Compact candidate: faster decoder, original packed storage budget')
    args = parser.parse_args()
    build = args.build_dir.resolve()
    reader = args.reader.resolve()
    name = 'WHD-FastDecode-8M' if args.packed_decoder else 'WHD-FastLoad-8M'
    target = ROOT/'dist'/name
    stage = build/'stage'/name
    assert not target.exists() and not stage.exists(), 'preserve existing candidate'
    before = release_inventory()
    assert before, 'current release must remain present'
    (build/'release-before.json').write_text(json.dumps(before, indent=2)+'\n')
    exe = build/'Sparkpaw-Campaign'
    meta = json.loads((build/'build.json').read_text())
    flag = '-DSPARKPAW_WHD_FAST_DECODE' if args.packed_decoder else '-DSPARKPAW_WHD_HYBRID'
    assert flag in meta['command'] and flag in meta['module_command']
    if args.packed_decoder:
        assert '-DSPARKPAW_WHD_HYBRID' not in meta['command']
    assert sha(exe.read_bytes()) == meta['sha256']
    refs = set(executable_runtime_files(exe))
    assert {ALIASES.get(name, name) for name in refs} == HD_ALL
    archive = ROOT/'dist/Sparkpaw-0.7.0-alpha.9-WHDLoad.zip'
    with zipfile.ZipFile(archive) as package:
        assert package.testzip() is None
        prefix = 'Sparkpaw-0.7.0-a9-WHDLoad/'
        slave = package.read(prefix+'Sparkpaw.Slave')
        assert struct.unpack_from('>I', slave, slave.index(b'WHDLOADS')+28)[0] == 0x380000
        baseline = {name: package.read(prefix+'data/assets/runtime/'+name) for name in refs}
    assets = stage/'data/assets/runtime'
    assets.mkdir(parents=True)
    report = {}
    for name in sorted(refs):
        logical = ALIASES.get(name, name)
        original = source(logical).read_bytes()
        # CONTROL's current menu is the only asset change versus alpha.9.
        if logical != 'readymenu.spbm':
            assert decoded(baseline[name]) == original, logical
        if args.packed_decoder:
            body = (min((pack_rle(original), pack_lz(original)), key=len)
                    if logical == 'readymenu.spbm' else baseline[name])
        else:
            body = original if logical in RAW_NAMES else baseline[name]
        assert decoded(body) == original, logical
        path = assets/name
        path.write_bytes(body)
        if logical != 'storm-collision.bin' and logical != 'drowned-route.bin':
            subprocess.run([str(reader), str(path), str(source(logical)), '1'], check=True)
        report[name] = {'codec': 'raw' if body == original else body[:4].decode('ascii'),
                        'stored': len(body), 'decoded': len(original), 'sha256': sha(body)}
    stored_exe = exe
    if args.packed_decoder:
        stored_exe = build/'Sparkpaw-crunched'
        assert not stored_exe.exists(), 'preserve prior crunched executable'
        result = subprocess.run([str(ROOT/'build/shrinkler/Shrinkler'), '-1', '-p',
                                 str(exe), str(stored_exe)], capture_output=True, text=True, check=True)
        assert 'Verifying... OK' in result.stdout
        (build/'shrinkler-verification.txt').write_text(result.stdout)
    shutil.copy2(stored_exe, stage/'data/Sparkpaw')
    (stage/'Sparkpaw.Slave').write_bytes(slave)
    (stage/'Sparkpaw.info').write_bytes(make_project_icon(
        'WHDLoad', ['SLAVE=Sparkpaw.Slave', 'PRELOAD', 'PAL']))
    (stage/'ReadMe.txt.info').write_bytes(make_readme_icon())
    payload = sum(row['stored'] for row in report.values()) + stored_exe.stat().st_size
    # Static gate only. Host backups/Workbench fragmentation still need play.
    assert payload <= 13*1024*1024//4, 'exceeds 3.25 MiB candidate payload budget'
    if args.packed_decoder:
        assert payload <= 1910233+16384, 'exceeds accepted compact payload + 16 KiB'
    remainder = 8*1024*1024 - 0x380000 - payload
    readme = f'''Sparkpaw WHDLoad fast-loading candidate - 8 MB Fast
=================================================
Unnumbered test, NOT a release. Local release stays 0.7.0-alpha.9.
Includes the current unreleased CONTROL JOYSTICK/JOYPAD options.

First gate: FS-UAE A1200/AGA, PAL, 68030, 2 MB Chip, 8 MB Fast, no JIT.
Use the same normal Workbench and WHDLoad setup as the accepted baseline.
Open this drawer and double-click Sparkpaw (the WHDLoad project icon).
Keep PRELOAD enabled. Do not change cache or memory options for this test.
WHDLoad 19+ and your own A1200 Kickstart 3.1 ROM/RTB are required.

1. Cold start; do NOT skip the intro. Watch the first-image delay, black
   gaps between all five plates, and whether the music ends with the story.
2. Watch TITLE -> LOADING -> CHARGING -> READY. Report any flicker,
   black interruptions, corrupt pixels, crash or unexpectedly longer wait.
3. START GAME (Storm Ruins), play 10 seconds, Escape back to READY.
   Repeat once. Report the return loading time and presentation stability.
4. If stable, OPTIONS / START AT: Stormrail, then Drowned. Start each,
   play briefly, Escape to READY. F10 exits to Workbench.

No diagnostic logger; do not press left mouse to save a log.
A short verdict with approximate phase seconds is enough. No full campaign
replay or real-hardware test is requested at this first gate.

Changes: raw intro/title/loading/READY, HUD and selected Level-1 graphics,
SFX and gameplay music; raw executable avoids Shrinkler startup delay.
The large storm-front and strider stay packed, as do most section-2/3 assets
and presentation music. No eager whole-game unpack and no extra decoded cache.
Renderer, audio timing, screen holds, CPU-cache policy and slave are unchanged.

Payload: {payload} bytes; slave ExpMem: 3670016 bytes (3.5 MiB).
Arithmetic remainder of 8 MiB: {remainder} bytes BEFORE host overhead.
This is NOT proof of full PRELOAD. The 8 MB flicker/function gate is pending.
Larger hardware RAM cannot substitute for this 8 MB emulator gate.
Real-machine acceleration and intro sync are not yet verified.
'''
    if args.packed_decoder:
        readme = f'''Sparkpaw WHDLoad faster decoder - 8 MB Fast
=========================================
Unnumbered candidate; local release remains 0.7.0-alpha.9.
Includes unreleased CONTROL JOYSTICK/JOYPAD. Replaces the rejected raw mix.

FS-UAE first gate: A1200/AGA PAL, 68030, 2 MB Chip, 8 MB Fast, no JIT.
Use the same Workbench/WHDLoad environment as before. Double-click Sparkpaw.
PRELOAD must remain enabled. No memory/cache ToolType changes for this gate.
Requires WHDLoad 19+ and your own A1200 Kickstart 3.1 ROM/RTB.

1. Let the intro run unskipped through TITLE/LOADING/CHARGING to READY.
   Report any flicker first; then black intro gaps and music synchronization.
2. Start Level 1, play 10 seconds, Escape to READY. Repeat once.
3. Only if stable, direct-start Stormrail and Drowned, briefly play each,
   Escape back and exit with F10. Approximate load seconds are useful.
No diagnostic logger or mouse-save action. No full campaign replay required.

All assets are packed as in the accepted baseline (two collision files raw).
Only the current CONTROL menu has different decoded pixels. The executable
is Shrinkler-packed again; its startup depacker is NOT accelerated here.
This candidate parses compressed runs once per token and uses one CRC table
step per decoded byte instead of two. All CRC/bounds checks remain enabled.
No extra decoded asset cache, no raw expansion of the PRELOAD set, no changes
to renderer/CPU cache/slave/presentation holds/stats or REPLAY.

Payload: {payload} bytes; unchanged ExpMem: 3670016 bytes.
8-MiB arithmetic remainder BEFORE host overhead: {remainder} bytes.
This restores the accepted storage budget, but does not prove cache coverage.
Host decode parity is verified; actual speed and FS-UAE stability are pending.
Real-hardware improvement must be tested separately after this first gate.
'''
    (stage/'ReadMe.txt').write_text(readme, encoding='ascii')
    assert set(p.name for p in assets.iterdir()) == refs
    inventory = {p.relative_to(stage).as_posix(): sha(p.read_bytes())
                 for p in stage.rglob('*') if p.is_file()}
    for p in stage.rglob('*'):
        assert all(len(part) <= 30 for part in p.relative_to(stage.parent).parts)
    assert release_inventory() == before, 'release changed before staging'
    shutil.copytree(stage, target)
    assert inventory == {p.relative_to(target).as_posix(): sha(p.read_bytes())
                         for p in target.rglob('*') if p.is_file()}
    assert release_inventory() == before, 'release changed during staging'
    result = {'candidate': str(target), 'release': False, 'payload_bytes': payload,
              'expmem': 0x380000, 'host_remainder_before_overhead': remainder,
              'assets': report, 'inventory': inventory,
              'acceptance': 'host verified only; FS-UAE 8 MB and hardware pending'}
    (build/'candidate-manifest.json').write_text(json.dumps(result, indent=2)+'\n')
    print(target, 'payload', payload, 'host remainder before overhead', remainder)


if __name__ == '__main__':
    main()
