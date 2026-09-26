"""Prepare/stage configuration-only A/B; preserve release and completed tests."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
from make_sparkpaw_icon import make_project_icon
from stage_hd_test import release_inventory
ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/whdload-config-audit-20260925'
NAMES=('WHD-Cache-A-8M','WHD-NoCache-B-8M')
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def inventory(folder):return {str(p.relative_to(folder)):sha(p) for p in folder.rglob('*') if p.is_file()}
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--stage',action='store_true')
    p.add_argument('--archive-loadstate',action='store_true',help='Requires user confirmation or verified completed emulator shutdown log')
    args=p.parse_args();manifest=BUILD/'cache-ab.json'
    if not args.stage:
        assert not manifest.exists(),'preserve existing preparation'
        baseline=ROOT/'build/whdload-readyfast-v2-20260924'
        original=json.loads((baseline/'banks-manifest.json').read_text())
        source=Path(original['stage']);assert inventory(source)==original['inventory']
        assert sha(source/'data/Sparkpaw')=='654ee3d446fd52f6a8e1c9dcc31be56a506729b961bba27257ca68697f390660'
        release=release_inventory();assert release
        result={'release_before':release,'source':str(source),'source_inventory':original['inventory'],'variants':{}}
        for i,name in enumerate(NAMES):
            target=BUILD/'stage'/name;assert not target.exists()
            shutil.copytree(source,target)
            if i:
                (target/'Sparkpaw.info').write_bytes(make_project_icon('WHDLoad',['SLAVE=Sparkpaw.Slave','PAL','NOCACHE']))
            (target/'ReadMe.txt').write_text(f'''Sparkpaw {name} - configuration-only comparison, NOT a release

{'A: existing slave CPU instruction-cache policy.' if not i else 'B: NOCACHE overrides CPU caches; intentionally diagnostic, not a speed fix.'}
Both use the EXACT played ReadyFast executable, same slave and five raw
banks, same 5 MB game + 512 KB Kickstart reservation. No PRELOAD/file cache.
Only B adds NOCACHE to Sparkpaw icon. CPU cache is distinct from file caching.
No Supervisor/register-sampling code or new renderer changes in this pair.

Same FS-UAE 68020 / 2 MB Chip + 8 MB Fast / PAL / no JIT.
Test A first, then B from a fresh emulator restart with the same settings.
For each: double-click Sparkpaw, let intro run, note time to first intro and
CHARGING. Start Level 1, play briefly, Escape. Repeat Level 1/Escape once.
At final READY press/release LEFT MOUSE ONCE. Expected frozen save display.
Wait several seconds, then F10 to exit WHDLoad/flush log; stop FS-UAE.
Separate log per drawer: data/load-times.log. Do not copy logs between drawers.
No SOUNDTEST detour, full campaign or real-hardware run required.
Report flicker, music/screen timing or any launch failure verbatim.

This test does not promise shorter CHARGING/startup. B may be slower.
Guest VBlank measurements exclude disk/host-switch wall time. Nested menu
and renderer scopes are included in their parents. Do not add them twice.
The header still says LevelTimes because executable bytes are unchanged.
''',encoding='ascii')
            result['variants'][name]=inventory(target)
        a,b=[BUILD/'stage'/n for n in NAMES]
        differences=[n for n in inventory(a) if sha(a/n)!=sha(b/n)]
        assert set(differences)=={'Sparkpaw.info','ReadMe.txt'},differences
        assert (a/'Sparkpaw.info').read_bytes()==(source/'Sparkpaw.info').read_bytes()
        assert b'NOCACHE' in (b/'Sparkpaw.info').read_bytes()
        assert all('load-times.log' not in n for v in result['variants'].values() for n in v)
        assert release_inventory()==release
        result['only_differences']=differences;result['manual_acceptance']='pending'
        manifest.write_text(json.dumps(result,indent=2)+'\n')
        print('Prepared A/B: identical executable/slave/banks; only icon option and instructions differ')
        return
    m=json.loads(manifest.read_text());assert release_inventory()==m['release_before']
    for name in NAMES:
        assert not (ROOT/'dist'/name).exists()
        assert inventory(BUILD/'stage'/name)==m['variants'][name]
    old=ROOT/'dist/WHD-LoadState-8M';archive=ROOT/'dist/older-builds/WHD-LoadState-8M-measured'
    assert old.is_dir() and args.archive_loadstate,'confirm stopped before archiving completed diagnostic'
    assert not archive.exists() and not archive.with_suffix('.uaem').exists()
    before=inventory(old);launcher=old.with_suffix('.uaem');launcher_hash=sha(launcher) if launcher.exists() else None
    old.rename(archive)
    if launcher.exists():launcher.rename(archive.with_suffix('.uaem'))
    assert inventory(archive)==before
    if launcher_hash:assert sha(archive.with_suffix('.uaem'))==launcher_hash
    (BUILD/'archived-loadstate.json').write_text(json.dumps({'destination':str(archive),'inventory':before,'launcher_sha256':launcher_hash},indent=2)+'\n')
    for name in NAMES:
        shutil.copytree(BUILD/'stage'/name,ROOT/'dist'/name)
        assert inventory(ROOT/'dist'/name)==m['variants'][name]
    assert release_inventory()==m['release_before']
    (BUILD/'staged.json').write_text(json.dumps({'drawers':list(NAMES),'verified':True,'manual_acceptance':'pending'},indent=2)+'\n')
    print('Staged',', '.join(NAMES))
if __name__=='__main__':main()
