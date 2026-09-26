"""Prepare a banked WHDLoad candidate outside dist; no release or staging."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import zipfile

from campaign_asset_manifest import HD_ALL, LEVEL1, STORMRAIL, DROWNED
from campaign_runtime_sources import source
from make_sparkpaw_icon import make_project_icon, make_readme_icon
from runtime_asset_refs import executable_runtime_files
from stage_campaign_whdload_packed import ALIASES
from stage_whdload_fastload import decoded
from stage_hd_test import release_inventory
from whd_bank_format import pack, unpack

ROOT = Path(__file__).resolve().parents[1]
NAME = 'WHD-LevelBanks-8M'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir', required=True, type=Path)
    parser.add_argument('--name', default=NAME)
    parser.add_argument('--production', action='store_true',
                        help='Trace-free raw release package; same bank and slave implementation')
    parser.add_argument('--raw-levels', action='store_true',
                        help='Experimental raw current-level banks; same fixed game memory')
    args = parser.parse_args()
    build = args.build_dir.resolve()
    assert (args.name.startswith('WHD-') or args.production) and len(args.name)<=30 and '/' not in args.name
    stage = build / 'stage' / args.name
    assert not stage.exists(), 'preserve prior preparation'
    before = release_inventory()
    assert before
    (build/'release-before.json').write_text(json.dumps(before, indent=2)+'\n')
    exe = build/'Sparkpaw-Campaign'
    meta = json.loads((build/'build.json').read_text())
    if args.production:
        assert args.raw_levels
        assert not any('LOAD_TRACE' in str(x) or 'LOAD_STATE' in str(x)
                       for x in meta['command'] + meta['module_command'])
    assert meta['sha256'] == sha(exe.read_bytes())
    for command in (meta['command'], meta['module_command']):
        assert all(flag in command for flag in (
            '-DSPARKPAW_WHD_BANKS', '-DSPARKPAW_WHD_HYBRID',
            '-DSPARKPAW_WHD_FAST_DECODE'))
    refs = set(executable_runtime_files(exe))
    assert {ALIASES.get(n, n) for n in refs} == HD_ALL
    # Actual loaders reuse these beyond Level 1: collisionLoad always reads
    # storm-collision for Stormrail; Drowned shares player/extra-life art.
    # Logical production group ownership alone is not a phase dependency map.
    shared_level1 = {'storm-collision.bin', 'sparkpaw-sprites4.spbm',
                     'sparkpaw-extra-life.spbm'}
    groups = {
        'intro': {f'intro{i}.spbm' for i in range(1, 6)},
        'level1': {n for n in LEVEL1 if not n.endswith('.raw')} - shared_level1,
        'level2': {n for n in STORMRAIL if not n.endswith('.raw')},
        'level3': {n for n in DROWNED if not n.endswith(('.raw','score.bin','bank.bin'))},
    }
    # Audio includes Soundtest's cross-level requests. Keep these relatively
    # small files resident, never trigger implicit disk I/O behind a menu.
    groups['common'] = HD_ALL - set().union(*groups.values())
    assert sum(map(len, groups.values())) == len(HD_ALL)
    baseline = {}
    if not args.production:
        with zipfile.ZipFile(ROOT/'dist/Sparkpaw-0.7.0-alpha.9-WHDLoad.zip') as z:
            assert z.testzip() is None
            baseline = {n: z.read('Sparkpaw-0.7.0-a9-WHDLoad/data/assets/runtime/'+n)
                        for n in refs}
    data = stage/'data'
    data.mkdir(parents=True)
    report = {}
    for group, logicals in groups.items():
        files, entries = {}, {}
        for name in sorted(refs):
            logical = ALIASES.get(name, name)
            if logical not in logicals:
                continue
            raw = source(logical).read_bytes()
            if not args.production and logical != 'readymenu.spbm':
                assert decoded(baseline[name]) == raw, logical
            body = raw if args.raw_levels or group in ('intro','common','level1') else baseline[name]
            assert decoded(body) == raw
            files[name] = body
            entries[name] = dict(logical=logical, stored=len(body), decoded=len(raw),
                                 codec='raw' if body == raw else body[:4].decode(),
                                 sha256=sha(body))
        bank = pack(files)
        assert unpack(bank) == files
        (data/(group+'.spb')).write_bytes(bank)
        report[group] = dict(bytes=len(bank), sha256=sha(bank), files=entries)
    shutil.copy2(exe, data/'Sparkpaw') # raw Hunk: no Shrinkler wait

    # Build a private include, never edit the SDK or overwrite another slave.
    dev = ROOT/'.toolchain/whdload-dev/WHDLoad'
    kick = dev/'Src/sources/whdload/kick31.s'
    original = kick.read_text()
    needle = '\t\tdc.w\t0\t\t\t;ws_DontCache'
    assert original.count(needle) == 1
    patched = original.replace(needle, '\t\tdc.w\t_bankDontCache-slv_base\t;ws_DontCache')
    local = build/'kick31-banks.s'
    local.write_text(patched)
    slave_source = (ROOT/'whdload/Sparkpaw.asm').read_text()
    assert slave_source.count('INCLUDE\twhdload/kick31.s') == 1
    slave_source = slave_source.replace('INCLUDE\twhdload/kick31.s', f'INCLUDE\t"{local}"')
    slave_source += '\n_bankDontCache\tdc.b\t"#?",0\n\tEVEN\n'
    local_slave = build/'Sparkpaw-banks.asm'
    local_slave.write_text(slave_source)
    # No PACKED_WHDLOAD: 5 MiB game + 512 KiB Kickstart, within 8 MiB target.
    slave_args = [str(ROOT/'.toolchain/sdk/bin/vasmm68k_mot'), '-m68000', '-Fhunkexe',
            '-nosym','-quiet','-nowarn=62',f'-I{ROOT/"whdload/include"}',
            f'-I{dev/"Include"}',f'-I{ROOT/".toolchain/ndk/Include_I"}',
            f'-I{dev/"Src/sources"}',str(local_slave),'-o',str(stage/'Sparkpaw.Slave')]
    # Use the same authoritative NDK location as the normal packager.
    from make_whdload import NDK_INCLUDE
    slave_args[8] = f'-I{NDK_INCLUDE}'
    subprocess.run(slave_args, cwd=ROOT, check=True)
    slave = (stage/'Sparkpaw.Slave').read_bytes()
    at = slave.index(b'WHDLOADS')
    assert struct.unpack_from('>I',slave,at+28)[0] == 0x580000
    relative = struct.unpack_from('>H',slave,at+24)[0]
    assert slave[at-4+relative:at-4+relative+3] == b'#?\0'
    # PRELOAD deliberately absent, and ws_DontCache also disables on-demand
    # retention; no accidental second copy of the game-owned bank buffers.
    (stage/'Sparkpaw.info').write_bytes(make_project_icon(
        'WHDLoad', ['SLAVE=Sparkpaw.Slave','PAL']))
    (stage/'ReadMe.txt.info').write_bytes(make_readme_icon())
    (stage/'ReadMe.txt').write_text('''Sparkpaw WHDLoad - Level Banks / 8 MB Fast
Unnumbered experimental build; NOT a release. Includes CONTROL options.

First test: FS-UAE A1200/AGA PAL, 68020, 2 MB Chip, 8 MB Fast, no JIT.
Double-click Sparkpaw. Keep the supplied ToolTypes; PRELOAD is intentionally
absent and the slave disables file caching. Own Kickstart 3.1/RTB required.

1. Cold start, do not skip intro. Report delay to first image, black gaps
   between plates, and music synchronization. Watch TITLE/LOADING/CHARGING/
   READY for flicker. The authored title/charging minimum holds still apply.
2. Start Level 1, play 10 seconds, Escape; repeat once. Check menu stability.
3. If stable, OPTIONS / START AT: Stormrail, then Drowned; play each briefly
   and Escape. A deliberate black loading interval at level changes is
   expected; repeated flashes during the visible loading screen are not.
4. Optionally check Soundtest intro/game-over/Drowned tracks. F10 exits.
No log-save mouse action and no full campaign/hardware replay requested yet.

Raw executable, all intro/menu/Level-1 data. Later graphics retain the faster
decoder. Three bulk reads at boot load common data, intro and Level 1;
intro data is freed afterwards. A level switch replaces just the level bank
before the loading image appears. No stats prefetch or change to REPLAY.
Common audio remains resident to cover Soundtest without hidden disk reads.

Memory: 5 MB game Fast + 512 KB Kickstart, no WHDLoad file cache. Bank memory
is inside that game budget, not in addition. Assets are still copied/decoded
to their normal Chip/Fast destinations; no rendering buffer aliases change.
This budget and actual timing are NOT yet accepted on an 8 MB machine.
If it fails, report the last screen or WHDLoad error verbatim. Do not add RAM
to turn a failed 8 MB gate into an apparent success.
''', encoding='ascii')
    if '-DSPARKPAW_WHD_LOAD_TRACE' in meta['command']:
        (stage/'ReadMe.txt').write_text('''Sparkpaw WHD-LevelTimes - loading measurements only
Unnumbered diagnostic, NOT an optimization or release.
Same corrected phase banks and slave as the working LevelBanks candidate.

FS-UAE A1200/AGA PAL, 68020, 2 MB Chip + 8 MB Fast, no JIT.
Double-click Sparkpaw; leave ToolTypes unchanged. Intro may be skipped.
1. Reach READY, start Level 1, play briefly, Escape back to READY.
2. OPTIONS / START AT: Stormrail, START GAME, play briefly, Escape.
3. OPTIONS / START AT: Drowned Turbines, START GAME, play briefly, Escape.
4. At the final READY screen, press and release LEFT MOUSE ONCE.
   The display deliberately freezes. Wait a few seconds, then press F10
   to exit WHDLoad and flush its pending writes, then stop FS-UAE.
   The log is data/load-times.log in THIS drawer. Report when done.
   Do not reset before WHDLoad has had the opportunity to save the file.

No mouse press is needed during gameplay. No full level completion or real
hardware run is requested. No automatic writes occur during measured loading.
The final explicit save may interrupt the frozen display; it is not gameplay.

Measurements: gameplay asset preparation, collision, audio, renderer build,
remaining CHARGING minimum wait, total READY build and its nested menu cache.
The minimum wait is elapsed-to-100-fields, not an extra unconditional 2 seconds.
Clock: guest OS VBlank fields /50 at PAL. Host disk/OS-switch wall time is NOT
measured. ready_menu_cache is INCLUDED in ready_total; do not add them together.
Free Chip/Fast snapshots are phase ends, not complete high-water measurements.
Timing adds a bounded RAM log and a few calls per load, no gameplay profiler.
Actual timing and log-save behavior remain pending this manual test.
''',encoding='ascii')
    if args.raw_levels:
        readme=stage/'ReadMe.txt'
        readme.write_text('''RAW LEVEL BANKS / 8 MB EXPERIMENT
Only change from measured LevelTimes: Level 2 and 3 source bundles are raw.
Executable, slave, common data, intro and Level 1 are byte-identical.
Only the CURRENT level is fetched at a black transition; this does not
preload the whole campaign. The unchanged decoder remains in the executable.
Game Fast reservation stays 5 MB + 512 KB Kickstart inside the 8 MB target.
Drowned needs 994949 extra source bytes. Earlier phase-end free-memory
measurements suggest 735491 Fast bytes remaining, NOT a proven peak budget.

Focused run: skip intro if desired; at READY choose START AT Stormrail,
start, play briefly, Escape. Repeat for Drowned, then save at READY with
LEFT MOUSE as below. Note any longer black disk-read interval, load failure
or display glitch. CHARGING/renderer preparation are unchanged by this test.
Do not increase RAM to make the test pass. No hardware run needed yet.

'''+readme.read_text(),encoding='ascii')
    if args.name == 'WHD-ReadyFast-8M':
        assert args.raw_levels and '-DSPARKPAW_WHD_LOAD_TRACE' in meta['command']
        (stage/'ReadMe.txt').write_text("Sparkpaw WHD-ReadyFast-8M - unnumbered test, not a release\n\nCompared with played WHD-LevelRaw-8M: faster READY cache reconstruction,\nno extra cache allocation, same raw current-level banks and memory budget.\nRenderer has five additional RAM-only loading scopes; no gameplay profiler.\nNested renderer scopes belong INSIDE renderer_prepare, not on top of it.\n\nUse same FS-UAE 68020 / PAL / 2 MB Chip + 8 MB Fast / no JIT.\nDouble-click Sparkpaw. Check intro/title/CHARGING transitions and READY.\nAt READY, briefly navigate OPTIONS and SOUNDTEST; check text, music and dust.\nStart Level 1, play briefly, Escape back to READY.\nAt final READY press/release LEFT MOUSE ONCE; screen deliberately freezes.\nWait a few seconds, then F10 to exit WHDLoad/flush writes; stop FS-UAE.\nThe log is data/load-times.log in THIS drawer. Report when done.\nNo full campaign or hardware test needed for this initial focused gate.\n\nNo automatic writes during loading. Guest VBlank fields /50 measure guest\npreparation only; disk/host-switch wall time is excluded. Nested menu-cache\nscope is included in ready_total. Free-memory values are phase endpoints,\nnot peaks. Memory fit and actual improvement require the user's test.\n",encoding='ascii')
    if args.name == 'WHD-LoadState-8M':
        assert args.raw_levels and '-DSPARKPAW_WHD_LOAD_STATE' in meta['command']
        (stage/'ReadMe.txt').write_text('Sparkpaw WHD-LoadState-8M - read-only state diagnostic, not a release\n\nSame READY optimization, raw banks, display and cache policy as ReadyFast.\nAdds RAM snapshots of CACR, DMA, interrupt enables/requests, CIA-B controls,\nand CIA-A TOD at load boundaries. CACR is READ via Exec Supervisor (68020+).\nNever writes CACR or reads/clears CIA ICR. No automatic log writes.\nState/phase rows share zero-based row numbers; values are decimal.\nThe extra state table adds diagnostic memory, not another asset cache.\n\nSame FS-UAE 68020 / PAL / 2 MB Chip + 8 MB Fast / no JIT.\n1. Double-click Sparkpaw. Let intro run; note rough time to first picture.\n2. At READY start Level 1, play briefly, Escape back to READY.\n3. Repeat Level 1 -> Escape once, using the same menu/audio settings.\n4. At READY press/release LEFT MOUSE ONCE. Screen deliberately freezes.\n   Wait several seconds, F10 to exit WHDLoad/flush writes, then stop FS-UAE.\n   Log: data/load-times.log in THIS drawer. Report when done.\nNo full levels, SOUNDTEST detour, or real-hardware test required.\n\nThis build is not expected to fix the slow return yet. If the first picture\nfails to appear, report the exact last screen/WHDLoad error; do not retry.\nboot_banks starts AFTER executable/Kickstart/platform startup. It is not the\nentire click-to-first-picture time. Guest VBlank excludes host-switch wall\n time; CIA-A TOD delta is supplemental, not a claimed wall-clock measurement.\nNested renderer scopes belong INSIDE renderer_prepare; ready_menu_cache is\nINSIDE ready_total. Free-memory snapshots are phase ends, not peak budgets.\n',encoding='ascii')
    assert release_inventory() == before
    inventory = {p.relative_to(stage).as_posix(): sha(p.read_bytes())
                 for p in stage.rglob('*') if p.is_file()}
    assert all(len(part) <= 30 for name in inventory for part in name.split('/'))
    result = dict(stage=str(stage), banks=report, inventory=inventory, expmem=0x580000,
                  raw_levels=args.raw_levels,
                  startup_bank_bytes=sum(report[g]['bytes'] for g in ('intro','common','level1')),
                  resident_bank_max=report['common']['bytes']+max(report[g]['bytes'] for g in ('level1','level2','level3')),
                  sdk_source_sha256=sha(original.encode()), slave_command=slave_args,
                  acceptance='package verification only; see release notes' if args.production else 'native/host only; manual 68020 8 MB pending', release=args.production)
    (build/'banks-manifest.json').write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items() if k not in ('banks','inventory','slave_command')},indent=2))


if __name__ == '__main__':
    main()
