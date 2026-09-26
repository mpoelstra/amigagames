"""Stage a verified level-bank candidate; preserve retired tests intact."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil

from stage_hd_test import release_inventory
from prepare_whdload_banks import ROOT, NAME


def inventory(folder):
    return {p.relative_to(folder).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
            for p in folder.rglob('*') if p.is_file()}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir',required=True,type=Path)
    parser.add_argument('--archive-fastdecode',action='store_true',
                        help='Only after user confirms FS-UAE stopped')
    parser.add_argument('--archive-current',action='store_true',
                        help='Archive previous LevelBanks after confirmed FS-UAE stop')
    parser.add_argument('--archive-working-banks',action='store_true',
                        help='For timing candidate, archive working LevelBanks after confirmed stop')
    parser.add_argument('--archive-measured-times',action='store_true',
                        help='Archive completed timing run, including its log, after stop')
    parser.add_argument('--archive-played-raw',action='store_true',help='User confirmed FS-UAE stopped')
    parser.add_argument('--archive-played-readyfast',action='store_true',help='User confirmed FS-UAE stopped')
    args=parser.parse_args()
    build=args.build_dir.resolve()
    manifest=json.loads((build/'banks-manifest.json').read_text())
    release=json.loads((build/'release-before.json').read_text())
    assert release_inventory()==release, 'release inventory changed'
    stage=Path(manifest['stage']);target=ROOT/'dist'/stage.name
    assert stage.name in (NAME,'WHD-LevelTimes-8M','WHD-LevelRaw-8M','WHD-ReadyFast-8M','WHD-LoadState-8M')
    assert inventory(stage)==manifest['inventory']
    assert (build/'decoder-parity.json').is_file()
    assert (build/'bank-tests/backend').is_file()
    dependencies=json.loads((build/'dependency-result.json').read_text())
    assert len(dependencies)==3 and all(row['passed'] for row in dependencies)
    if stage.name=='WHD-LoadState-8M':
        assert not target.exists(), 'preserve existing candidate'
        assert (build/'state-tests/trace').is_file()
        raw=ROOT/'dist/WHD-ReadyFast-8M'
        assert raw.is_dir() and args.archive_played_readyfast, 'confirm stopped before archiving played raw build'
        archive=ROOT/'dist/older-builds/WHD-ReadyFast-8M-measured'
        assert not archive.exists() and not archive.with_suffix('.uaem').exists()
        original=inventory(raw);launcher=raw.with_suffix('.uaem')
        launcher_hash=hashlib.sha256(launcher.read_bytes()).hexdigest() if launcher.exists() else None
        raw.rename(archive)
        if launcher.exists():launcher.rename(archive.with_suffix('.uaem'))
        assert inventory(archive)==original
        if launcher_hash:
            assert hashlib.sha256(archive.with_suffix('.uaem').read_bytes()).hexdigest()==launcher_hash
        (build/'archived-readyfast.json').write_text(json.dumps(dict(
            destination=str(archive),inventory=original,launcher_sha256=launcher_hash),indent=2)+'\n')
    if stage.name=='WHD-ReadyFast-8M':
        assert (build/'trace-tests/trace').is_file()
        raw=ROOT/'dist/WHD-LevelRaw-8M'
        assert raw.is_dir() and args.archive_played_raw, 'confirm stopped before archiving played raw build'
        archive=ROOT/'dist/older-builds/WHD-LevelRaw-8M-measured'
        assert not archive.exists() and not archive.with_suffix('.uaem').exists()
        original=inventory(raw);launcher=raw.with_suffix('.uaem')
        launcher_hash=hashlib.sha256(launcher.read_bytes()).hexdigest() if launcher.exists() else None
        raw.rename(archive)
        if launcher.exists():launcher.rename(archive.with_suffix('.uaem'))
        assert inventory(archive)==original
        if launcher_hash:
            assert hashlib.sha256(archive.with_suffix('.uaem').read_bytes()).hexdigest()==launcher_hash
        (build/'archived-raw.json').write_text(json.dumps(dict(
            destination=str(archive),inventory=original,launcher_sha256=launcher_hash),indent=2)+'\n')
    if stage.name=='WHD-LevelRaw-8M':
        assert (build/'trace-tests/trace').is_file()
        measured=ROOT/'dist/WHD-LevelTimes-8M'
        if measured.exists():
            assert args.archive_measured_times, 'confirm completed run/stop before archive'
            archive=ROOT/'dist/older-builds/WHD-LevelTimes-8M-measured'
            assert not archive.exists() and not archive.with_suffix('.uaem').exists()
            original=inventory(measured);launcher=measured.with_suffix('.uaem')
            launcher_hash=hashlib.sha256(launcher.read_bytes()).hexdigest() if launcher.exists() else None
            measured.rename(archive)
            if launcher.exists():launcher.rename(archive.with_suffix('.uaem'))
            assert inventory(archive)==original
            if launcher_hash:
                assert hashlib.sha256(archive.with_suffix('.uaem').read_bytes()).hexdigest()==launcher_hash
            (build/'archived-times.json').write_text(json.dumps(dict(
                destination=str(archive),inventory=original,launcher_sha256=launcher_hash),indent=2)+'\n')
    if stage.name=='WHD-LevelTimes-8M':
        assert (build/'trace-tests/trace').is_file()
        working=ROOT/'dist'/NAME
        if working.exists():
            assert args.archive_working_banks, 'confirm stopped before archiving working banks'
            archive=ROOT/'dist/older-builds/WHD-LevelBanks-8M-working'
            assert not archive.exists() and not archive.with_suffix('.uaem').exists()
            original=inventory(working);launcher=working.with_suffix('.uaem')
            launcher_hash=hashlib.sha256(launcher.read_bytes()).hexdigest() if launcher.exists() else None
            working.rename(archive)
            if launcher.exists():launcher.rename(archive.with_suffix('.uaem'))
            assert inventory(archive)==original
            if launcher_hash:
                assert hashlib.sha256(archive.with_suffix('.uaem').read_bytes()).hexdigest()==launcher_hash
            (build/'archived-working-banks.json').write_text(json.dumps(dict(
                destination=str(archive),inventory=original,launcher_sha256=launcher_hash),indent=2)+'\n')
    if target.exists():
        assert args.archive_current, 'confirm stopped before replacing current test'
        archive=ROOT/'dist/older-builds/WHD-LevelBanks-8M-shared-miss'
        assert not archive.exists() and not archive.with_suffix('.uaem').exists()
        original=inventory(target);launcher=target.with_suffix('.uaem')
        launcher_hash=hashlib.sha256(launcher.read_bytes()).hexdigest() if launcher.exists() else None
        target.rename(archive)
        if launcher.exists():launcher.rename(archive.with_suffix('.uaem'))
        assert inventory(archive)==original
        if launcher_hash:
            assert hashlib.sha256(archive.with_suffix('.uaem').read_bytes()).hexdigest()==launcher_hash
        (build/'archived-levelbanks.json').write_text(json.dumps(dict(
            destination=str(archive),inventory=original,launcher_sha256=launcher_hash),indent=2)+'\n')
    old=ROOT/'dist/WHD-FastDecode-8M'
    if old.exists():
        assert args.archive_fastdecode, 'confirm stopped before archiving old test'
        archive=ROOT/'dist/older-builds/WHD-FastDecode-8M-startup-slow'
        assert not archive.exists() and not archive.with_suffix('.uaem').exists()
        original=inventory(old)
        launcher=old.with_suffix('.uaem')
        launcher_hash=hashlib.sha256(launcher.read_bytes()).hexdigest() if launcher.exists() else None
        old.rename(archive)
        if launcher.exists():launcher.rename(archive.with_suffix('.uaem'))
        assert inventory(archive)==original
        if launcher_hash:
            assert hashlib.sha256(archive.with_suffix('.uaem').read_bytes()).hexdigest()==launcher_hash
        (build/'archived-fastdecode.json').write_text(json.dumps(
            dict(destination=str(archive),inventory=original,launcher_sha256=launcher_hash),indent=2)+'\n')
    shutil.copytree(stage,target)
    assert inventory(target)==manifest['inventory']
    assert release_inventory()==release
    (build/'staged.json').write_text(json.dumps(dict(target=str(target),
        inventory=manifest['inventory'],acceptance='manual test pending'),indent=2)+'\n')
    print(target)


if __name__=='__main__':main()
