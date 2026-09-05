#!/usr/bin/env python3
"""Independent release inventory, WHDLoad archive/icon and ADF checks."""
import hashlib,json,subprocess,tempfile,zipfile,sys
from pathlib import Path
from make_release import ROOT,DIST,RELEASE_NAME,RELEASE_VERSION,RUNTIME_FILES
from make_whdload import STAGE_NAME,STAGE,ARCHIVE_NAME
from make_sparkpaw_icon import make_project_icon
from runtime_asset_refs import executable_runtime_files
sys.path.insert(0,str(ROOT/'.toolchain/amitools'))
from amitools.fs.blkdev.BlkDevFactory import BlkDevFactory
from amitools.fs.ADFSVolume import ADFSVolume
from amitools.fs.FSString import FSString

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 subprocess.run([sys.executable,str(ROOT/'tools/verify_campaign_hd_release.py')],check=True)
 assert set(executable_runtime_files(STAGE/'data/Sparkpaw'))==set(RUNTIME_FILES)
 assert (STAGE/'data/Sparkpaw').read_bytes()==(ROOT/'build/sparkpaw-whdload').read_bytes()
 assert (STAGE/'Sparkpaw.info').read_bytes()==make_project_icon('WHDLoad',['SLAVE=Sparkpaw.Slave','PRELOAD','PAL'])
 assert f'Version {RELEASE_VERSION}'.encode() in (STAGE/'Sparkpaw.Slave').read_bytes()
 expected={p.relative_to(STAGE).as_posix():p.read_bytes() for p in STAGE.rglob('*') if p.is_file()}
 assert set(expected)=={'Sparkpaw.Slave','Sparkpaw.info','ReadMe.txt','ReadMe.txt.info','data/Sparkpaw'}|{'data/assets/runtime/'+n for n in RUNTIME_FILES}
 for n in RUNTIME_FILES:assert expected['data/assets/runtime/'+n]==(ROOT/'assets/runtime'/n).read_bytes()
 with zipfile.ZipFile(DIST/f'{ARCHIVE_NAME}.zip') as z:
  assert z.testzip() is None
  files={n for n in z.namelist() if not n.endswith('/')}
  assert files=={STAGE_NAME+'/'+n for n in expected}
  for n,data in expected.items():assert z.read(STAGE_NAME+'/'+n)==data
  for n in z.namelist():assert all(len(c)<=30 for c in Path(n).parts)
 lha=DIST/f'{ARCHIVE_NAME}.lha'
 listing=subprocess.check_output(['/opt/homebrew/bin/lha','v',str(lha)],text=True)
 stored=[l for l in listing.splitlines() if '-lh0-' in l]
 assert len(stored)==1 and stored[0].endswith('/data/assets/runtime/tally-tick.raw')
 assert '-lh5-' in listing
 with tempfile.TemporaryDirectory(dir=ROOT/'build') as tmp:
  subprocess.run(['/opt/homebrew/bin/lha','x',str(lha)],cwd=tmp,check=True,stdout=subprocess.DEVNULL)
  base=Path(tmp)/STAGE_NAME
  assert {p.relative_to(base).as_posix() for p in base.rglob('*') if p.is_file()}==set(expected)
  for n,data in expected.items():assert (base/n).read_bytes()==data
 media=json.loads((ROOT/'build/multidisk-probe/media.json').read_text())
 for disk in (1,2):
  p=DIST/f'{RELEASE_NAME}-Disk{disk}.adf';assert p.stat().st_size==901120
  assert digest(p)==media['disks'][disk-1]['sha256']
  dev=BlkDevFactory().open(str(p),read_only=True);v=ADFSVolume(dev);v.open()
  assert v.is_ffs
  if disk==1:assert v.boot.valid_chksum and v.boot.boot_code
  for row in media['disks'][disk-1]['files']:
   node=v.get_file_path_name(FSString(row['path']));assert hashlib.sha256(node.get_file_data()).hexdigest()==row['sha256']
  assert bytes(v.get_file_path_name(FSString('assets/runtime/storm-collision.bin')).get_file_data())==(ROOT/'assets/runtime/storm-collision.bin').read_bytes()
  dev.close()
 paths=[DIST/(RELEASE_NAME+s) for s in ('.zip','.lha','-Disk1.adf','-Disk2.adf','-WHDLoad.zip','-WHDLoad.lha')]
 report={'version':RELEASE_VERSION,'HD_game_sha256':digest(ROOT/'sparkpaw'),'WHDLoad_game_sha256':digest(STAGE/'data/Sparkpaw'),'WHDLoad_slave_sha256':digest(STAGE/'Sparkpaw.Slave'),'artifacts':{p.name:{'bytes':p.stat().st_size,'sha256':digest(p)} for p in paths},'native_evidence':'HD and two-ADF user accepted; new campaign WHDLoad and physical hardware gates pending'}
 (ROOT/'build/checkpoint-release-verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
