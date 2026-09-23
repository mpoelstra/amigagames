"""Real assembler/linker fixture plus immutable payload checks and rejection."""
from pathlib import Path
import tempfile,subprocess,sys
R=Path(__file__).resolve().parents[1];sys.path.insert(0,str(R/'tools'))
from hunk_namespace import transform,namespace_objects,HunkError
with tempfile.TemporaryDirectory() as td:
 p=Path(td);asm=R/'.toolchain/sdk/bin/vasmm68k_mot';link=R/'.toolchain/sdk/bin/vlink'
 (p/'module.s').write_text(''' section code,code
 xdef _drownedCampaignRun
 xdef _private
 xref _sdk
_drownedCampaignRun:
 jsr _private
 jsr _sdk
 rts
_private:
 moveq #7,d0
 rts
 section data,data
 xdef _data
_data: dc.l _private,123
 section bss,bss
 xdef _state
_state: ds.l 2
''')
 (p/'sdk.s').write_text(' section code,code\n xdef _sdk\n_sdk: rts\n')
 for n in ['module','sdk']:subprocess.run([str(asm),'-quiet','-Fhunk','-o',str(p/(n+'.o')),str(p/(n+'.s'))],check=True)
 raw=(p/'module.o').read_bytes();assert transform(raw)[0]==raw
 out,names=namespace_objects([p/'module.o'],p/'renamed');new=out[0].read_bytes()
 assert transform(raw)[3]==transform(new)[3]
 assert '__drowned__private' in transform(new)[1] and '_private' not in transform(new)[1]
 assert '_sdk' in transform(new)[2] and '_drownedCampaignRun' in transform(new)[1]
 subprocess.run([str(link),'-bamigahunk','-nostdlib','-e','_drownedCampaignRun',str(out[0]),str(p/'sdk.o'),'-o',str(p/'linked')],check=True)
 inverse={v:k for k,v in names.items()};assert transform(new,inverse)[0]==raw
 for broken in [raw[:-1],b'\0\0\x12\x34']:
  try:transform(broken);raise AssertionError('bad object accepted')
  except HunkError:pass
print('PASS native Hunk namespace: real vasm/vlink resolution, external SDK kept, exact reversible symbols, code/data unchanged, invalid/truncated rejected')
