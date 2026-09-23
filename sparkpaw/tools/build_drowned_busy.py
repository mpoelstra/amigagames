#!/usr/bin/env python3
"""Build sparse busy-scene discovery and offline plain/minimal controls. No launch."""
from pathlib import Path
import hashlib, json, os, shlex, subprocess
R=Path(__file__).resolve().parents[1]
O=R/'build/drowned-busy-two-copy'
def main():
 O.mkdir(parents=True,exist_ok=True)
 plan=subprocess.check_output(['make','-Bn','drowned-full','PYTHON=../.venv/bin/python3'],cwd=R,text=True).replace('\\\n',' ')
 args=shlex.split(next(s for s in plan.splitlines() if '+aos68k' in s and '-o build/sparkpaw-drowned-full' in s))
 pos=args.index('-o');del args[pos:pos+2]
 assert all(f not in args for f in ['-DSPARKPAW_RENDER_DIAGNOSTIC','-DSPARKPAW_DROWNED_RESET_BLIT'])
 env=dict(os.environ,VBCC=str(R/'.toolchain/sdk'));env['PATH']=str(R/'.toolchain/sdk/bin')+':'+env['PATH']
 h=hashlib.sha256(json.dumps(args).encode())
 inputs=set(R.glob('src/*.[ch]'))|set((R/'build/drowned-full').glob('*.h'))|set((R/'build/drowned-full/assets').glob('*'))|{R/a for a in args if a.endswith(('.c','.o'))}
 for p in sorted(inputs):
  if p.is_file():h.update(str(p.relative_to(R)).encode());h.update(p.read_bytes())
 bid='busy_'+h.hexdigest()[:20]; proof={}
 for variant in ('plain','minimal','sparse'):
  extra=[] if variant=='plain' else ['-DSPARKPAW_DROWNED_TWO_COPY_TEST','-DSPARKPAW_DROWNED_FPS',f'-DSPARKPAW_DROWNED_FPS_BUILD_ID={bid}_{variant}','src/drowned_fps.c']
  if variant=='sparse':extra+=['-DSPARKPAW_DROWNED_BUSY','src/drowned_busy.c']
  cmd=args+extra+['-o',str(O/variant)]
  with (O/(variant+'-compile.log')).open('w') as log:subprocess.run(cmd,cwd=R,env=env,stdout=log,stderr=subprocess.STDOUT,check=True)
  proof[variant]={'command':cmd,'sha256':hashlib.sha256((O/variant).read_bytes()).hexdigest(),'bytes':(O/variant).stat().st_size,'build_id':bid+'_'+variant}
  for unit in ('main','game','renderer','drowned_fps','drowned_busy'):
   if variant=='plain' and unit in ('drowned_fps','drowned_busy'):continue
   if variant=='minimal' and unit=='drowned_busy':continue
   flags=[a for a in args+extra if not a.endswith(('.c','.o'))]
   with (O/(variant+'-'+unit+'-asm.log')).open('w') as log:subprocess.run(flags+['-S','-o',str(O/(variant+'-'+unit+'.s')),'src/'+unit+'.c'],cwd=R,env=env,stdout=log,stderr=subprocess.STDOUT,check=True)
 baseline=R/'build/drowned-two-copy/B-plain'
 assert proof['plain']['sha256']==hashlib.sha256(baseline.read_bytes()).hexdigest(),'Plain hot paths changed unexpectedly'
 (O/'builds.json').write_text(json.dumps(proof,indent=2)+'\n')
 print(json.dumps({k:{a:v[a] for a in ('sha256','bytes','build_id')} for k,v in proof.items()},indent=2))
if __name__=='__main__':main()
