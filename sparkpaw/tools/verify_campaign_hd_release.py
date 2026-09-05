#!/usr/bin/env python3
"""Verify current HD ZIP/LHA contents against source and protected alpha.68."""
import hashlib
import json
import re
import subprocess
import tempfile
import zipfile
from pathlib import Path
from make_release import ROOT, DIST, RELEASE_NAME, RUNTIME_FILES, RUNTIME_README, validate_release_identity
from make_sparkpaw_icon import make_project_icon


def main():
    validate_release_identity()
    expected = {'Sparkpaw': (ROOT/'build/sparkpaw-campaign-play').read_bytes(),
                'Sparkpaw.info': make_project_icon('Sparkpaw', []),
                'ReadMe.txt': RUNTIME_README.encode('ascii')}
    expected.update({f'assets/runtime/{n}':(ROOT/'assets/runtime'/n).read_bytes() for n in RUNTIME_FILES})
    assert (ROOT/'sparkpaw').read_bytes() == expected['Sparkpaw']
    drawer = DIST/RELEASE_NAME
    assert {str(p.relative_to(drawer)) for p in drawer.rglob('*') if p.is_file()} == set(expected)
    for name,data in expected.items():
        assert (drawer/name).read_bytes() == data
    with zipfile.ZipFile(DIST/f'{RELEASE_NAME}.zip') as z:
        assert z.testzip() is None
        names=[n for n in z.namelist() if not n.endswith('/')]
        assert set(names)=={f'{RELEASE_NAME}/{n}' for n in expected}
        for n in z.namelist():
            assert all(len(c)<=30 for c in Path(n).parts)
        for n,data in expected.items():
            assert z.read(f'{RELEASE_NAME}/{n}')==data
    lha = DIST/f'{RELEASE_NAME}.lha'
    listing=subprocess.check_output(['/opt/homebrew/bin/lha','v',str(lha)],text=True)
    stored = [line for line in listing.splitlines() if '-lh0-' in line]
    # Classic LHa legitimately stores the tiny incompressible tally sample.
    # Explicit exception: reject any other stored member or all-stored archive.
    assert len(stored)==1 and stored[0].endswith('/assets/runtime/tally-tick.raw')
    assert '-lh5-' in listing
    assert all(m in ('-lh5-','-lh0-','-lhd-') for m in re.findall(r'-lh[0-9d]-',listing))
    with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='lha-release-verify-') as tmp:
        subprocess.run(['/opt/homebrew/bin/lha','x',str(lha)],cwd=tmp,stdout=subprocess.DEVNULL,check=True)
        base=Path(tmp)/RELEASE_NAME
        assert {str(p.relative_to(base)) for p in base.rglob('*') if p.is_file()}==set(expected)
        for name,data in expected.items():assert (base/name).read_bytes()==data,name
    inventory=ROOT/'docs/ALPHA68_ARTIFACT_SHA256.json'
    for name,digest in json.loads(inventory.read_text()).items():
        if name.startswith('dist/Sparkpaw-'):
            assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest,name
    result={'version':RELEASE_NAME,'files':len(expected),'assets':len(RUNTIME_FILES),
            'executable_sha256':hashlib.sha256(expected['Sparkpaw']).hexdigest(),
            'zip_lha_drawer_byte_parity':True,'icon_byte_parity':True,
            'alpha68_inventory_unchanged':True,'lha_stored_exception':'496-byte tally-tick.raw; classic LHa incompressible fallback, CRC and bytes verified','artifacts':{p.name:{'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in [DIST/f'{RELEASE_NAME}.zip',lha]}}
    (ROOT/'build/release07-verification.json').write_text(json.dumps(result,indent=2))
    print(json.dumps(result,indent=2))


if __name__=='__main__':main()
