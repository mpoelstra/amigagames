#!/usr/bin/env python3
"""Build/read back the unaccepted, no-intro two-disk campaign proof.

Explicit first-use order improves payload locality; it is not a DOS seek trace.
Never writes dist or the accepted HD/original alpha.68 artifacts.
"""
import hashlib,json,os,subprocess,sys
from pathlib import Path
from campaign_asset_manifest import SHARED_PRESENTATION,SHARED_GAMEPLAY,LEVEL1,STORMRAIL
from pack_disk_asset import pack as lz,decode as unlz
from pack_adf_asset import pack as rle,decode as unrle
from runtime_asset_refs import executable_runtime_files
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'build/multidisk-probe'
sys.path.insert(0,str(ROOT/'.toolchain/amitools'))
from amitools.fs.blkdev.BlkDevFactory import BlkDevFactory
from amitools.fs.ADFSVolume import ADFSVolume
from amitools.fs.FSString import FSString
ALIAS={'readymenu.spbm':'level-ready-menu.spr1','sparkpaw-ready-screen.spbm':'level-ready.spr1','sparkpaw-level-complete.spbm':'level-complete.spr1'}
ORDER='''sparkpaw-title.spbm sparkpaw-level-loading.spbm level-charge-patch.spbm
storm-front.spbm stormrail-front.spbm storm-rear.spbm stormrail-rear.spbm
sparkpaw-sprites4.spbm sparkpaw-sprites4-storm.spbm clockwork-beetle.spbm clockwork-storm-strider.spbm
sparkpaw-hud-base.spbm sparkpaw-hud-health.spbm sparkpaw-hud-lives.spbm sparkpaw-hud-diamonds.spbm sparkpaw-hud-score.spbm
sparkpaw-diamond.spbm stormstone-core.spbm sparkpaw-extra-life.spbm
stormrail-flight-rear.spbm stormrail-heart.spbm stormrail-family.spbm stormrail-obstacles.spbm storm-collision.bin
energy-shot.raw player-hurt.raw enemy-hit.raw enemy-death.raw strider-shot.raw
harrier-fan-charge.raw harrier-fan-fire.raw harrier-hunter-charge.raw harrier-hunter-fire.raw
jump.raw collect-spark.raw water-splash.raw stormstone-core.raw tally-tick.raw extra-life.raw
sparkpaw-ready-screen.spbm readymenu.spbm sparkpaw-level-complete.spbm sparkpaw-score-glyphs.spbm
disk1-patch.spbm disk2-patch.spbm'''.split()
def sha(data):return hashlib.sha256(data).hexdigest()
def main():
 common=(SHARED_PRESENTATION-{n for n in SHARED_PRESENTATION if n.startswith('intro')})|SHARED_GAMEPLAY|{'disk1-patch.spbm','disk2-patch.spbm'}
 sets=[common|LEVEL1|{n for n in STORMRAIL if n.endswith('.raw')},common|STORMRAIL|(LEVEL1-{'storm-front.spbm','storm-rear.spbm','sparkpaw-sprites4.spbm'})]
 allnames=set.union(*map(set,sets));assert allnames==set(ORDER)
 payloads={};rows={}
 packed=OUT/'packed';packed.mkdir(exist_ok=True)
 for name in ORDER:
  source=(OUT/'status'/name) if name.startswith('disk') else ROOT/'assets/runtime'/name
  raw=source.read_bytes();data=raw;target=name
  if name.endswith('.spbm'):
   data=min((rle(raw),lz(raw)),key=len);assert (unlz(data) if data[:4]==b'SPL1' else unrle(data))==raw
   target=ALIAS.get(name,name[:-5]+'.spr1')
   p=packed/target;p.write_bytes(data)
   subprocess.run([str(OUT/'tests/reader'),str(p),str(source),'1'],check=True)
  assert len(target)<=30
  payloads[name]=(target,data)
  rows[name]={'disk_name':target,'raw_bytes':len(raw),'stored_bytes':len(data),'codec':data[:4].decode() if name.endswith('.spbm') else 'raw','decoded_sha256':sha(raw)}
 embedded=executable_runtime_files(OUT/'Sparkpaw')
 available={target for target,data in payloads.values()}
 for name in embedded:
  target=name[:-5]+'.spr1' if name.endswith('.spbm') else name
  assert target in available, ('missing compiled reference',name)
 # Per-volume coverage is mandatory: union coverage misses cold shared reads
 # such as collisionLoad(), which still reads the map during Stormrail entry.
 section_graphics=[{'storm-front.spr1','storm-rear.spr1','sparkpaw-sprites4.spr1'},
                   {n[:-5]+'.spr1' for n in STORMRAIL if n.endswith('.spbm')}]
 references={n[:-5]+'.spr1' if n.endswith('.spbm') else n for n in embedded}
 for i,names in enumerate(sets):
  present={payloads[n][0] for n in names}
  required=references-section_graphics[1-i]
  assert required<=present, ('missing section load dependency',i+1,sorted(required-present))
 env=os.environ.copy();env['PYTHONPATH']=str(ROOT/'.toolchain/amitools')
 reports=[]
 for disk,names in enumerate(sets,1):
  stage=OUT/f'disk{disk}-files';stage.mkdir(exist_ok=True)
  files={'Sparkpaw.disk':f'SP07D{disk}\n'.encode()}
  if disk==1:files={'Sparkpaw':(OUT/'Sparkpaw').read_bytes(),'S/startup-sequence':b'Sparkpaw\n',**files}
  for name in ORDER:
   if name in names:
    target,data=payloads[name];files['assets/runtime/'+target]=data
  adf=OUT/f'Sparkpaw-Disk{disk}.adf'
  cmd=[sys.executable,'-m','amitools.tools.xdftool','-f',str(adf),'format',f'SP07D{disk}','DOS1']
  if disk==1:cmd+=['+','boot','install']
  for directory in (['S'] if disk==1 else [])+['assets','assets/runtime']:cmd+=['+','makedir',directory]
  subprocess.run(cmd,env=env,check=True)
  dev=BlkDevFactory().open(str(adf),read_only=False);vol=ADFSVolume(dev);vol.open()
  # Keep standard central root/directory metadata. Start payload allocation at
  # the beginning to avoid the default mid-disk -> end -> beginning wrap.
  # Only the formatter allocation cursor changes, never the FFS format.
  vol.bitmap.find_start_off=0
  for target,data in files.items():
   p=stage/target;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
   vol.write_file(data,FSString(target))
  vol.close();dev.close()
  assert adf.stat().st_size==901120
  dev=BlkDevFactory().open(str(adf),read_only=True);vol=ADFSVolume(dev);vol.open();assert vol.is_ffs and vol.boot.dos_type==0x444f5301
  if disk==1:assert vol.boot.valid_chksum and vol.boot.boot_code
  layout=[];seen=set();directories=[]
  def walk(node):
   for child in node.get_entries():
    if child.is_dir():
     directories.append({'path':str(child.get_node_path_name()),'block':child.block.blk_num});walk(child)
    else:
     name=str(child.get_node_path_name());data=child.get_file_data();assert bytes(data)==files[name],name;seen.add(name)
     blocks=child.data_blk_nums
     layout.append({'path':name,'bytes':len(data),'sha256':sha(data),'header':child.block.blk_num,'extensions':child.ext_blk_nums,'data_blocks':blocks,'runs':sum(i==0 or b!=blocks[i-1]+1 for i,b in enumerate(blocks))})
  walk(vol.get_root_dir());assert seen==set(files)
  report={'disk':disk,'sha256':sha(adf.read_bytes()),'free_blocks':vol.get_free_blocks(),'used_blocks':vol.get_used_blocks(),'root_block':vol.root.blk_num,'directories':directories,'write_order':list(files),'files':layout}
  assert report['free_blocks']>=32,report['free_blocks']
  byname={r['path']:r for r in layout}
  ordered=[b for name in files for b in byname[name]['data_blocks']]
  assert ordered==sorted(ordered), 'payload allocation must not wrap backwards'
  dev.close();reports.append(report)
 (OUT/'media.json').write_text(json.dumps({'assets':rows,'disks':reports,'limitation':'Readback and source-derived first-use order; no native timing or runtime acceptance.'},indent=2)+'\n')
 print(json.dumps([{k:v for k,v in r.items() if k not in ('files','write_order')} for r in reports],indent=2))
if __name__=='__main__':main()
