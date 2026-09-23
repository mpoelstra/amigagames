#!/usr/bin/env python3
"""Opt-in Level1 split-renderer A/B. Music enabled, no launch/release writes."""
from pathlib import Path
import os,shlex,subprocess,json,hashlib
R=Path(__file__).resolve().parents[1];O=R/'build/level1-two-copy'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 O.mkdir(parents=True,exist_ok=True)
 plan=subprocess.check_output(['make','-Bn','build/sparkpaw-campaign-play','PYTHON=../.venv/bin/python3'],cwd=R,text=True).replace('\\\n',' ')
 args=shlex.split(next(l for l in plan.splitlines() if '+aos68k' in l and '-o build/sparkpaw-campaign-play' in l));i=args.index('-o');del args[i:i+2]
 args=[a for a in args if a!='-DSPARKPAW_LEVEL1_TWO_COPY_RING']
 assert '-DSPARKPAW_LEVEL1_MUSIC' in args and 'src/renderer_level1_unit.c' in args
 assert not any(x in args for x in ['-DSPARKPAW_RENDER_DIAGNOSTIC','-DSPARKPAW_DROWNED_FULL','-DSPARKPAW_LEVEL1_TWO_COPY_RING'])
 env=dict(os.environ,VBCC=str(R/'.toolchain/sdk'));env['PATH']=str(R/'.toolchain/sdk/bin')+':'+env['PATH']
 gen=hashlib.sha256(json.dumps(args).encode());inputs={}
 for p in sorted(set(R.glob('src/*.[ch]'))|set((R/'assets/runtime').glob('*'))|{R/a for a in args if a.endswith('.o')}):
  if p.is_file():inputs[str(p.relative_to(R))]=digest(p);gen.update(str(p.relative_to(R)).encode());gen.update(p.read_bytes())
 bid='l1ring_'+gen.hexdigest()[:20];proof={}
 for variant in 'AB':
  flags=['-DSPARKPAW_LEVEL1_TWO_COPY_RING'] if variant=='B' else []
  for measured in (False,True):
   key=variant+('-cadence' if measured else '-plain');extra=[]
   if measured:extra=['-DSPARKPAW_LEVEL1_RING_TEST','-DSPARKPAW_DROWNED_FPS','-DSPARKPAW_DISABLE_WAIT_PROFILE',f'-DSPARKPAW_DROWNED_FPS_BUILD_ID={bid}_{variant}','src/drowned_fps.c']
   command=args+flags+extra+['-o',str(O/key)]
   with (O/(key+'-compile.log')).open('w') as log:subprocess.run(command,cwd=R,env=env,stdout=log,stderr=subprocess.STDOUT,check=True)
   proof[key]={'command':command,'bytes':(O/key).stat().st_size,'sha256':digest(O/key)}
   if measured:proof[key]['build_id']=bid+'_'+variant
   units=['renderer_level1_unit','renderer_stormrail_unit','main','renderer_dispatch','game','audio_mix','level1_audio','platform_amiga'] if not measured else ['renderer_level1_unit','renderer_stormrail_unit','main','drowned_fps']
   for u in units:
    asmflags=[a for a in args+flags+extra if not a.endswith(('.c','.o'))]
    with (O/(key+'-'+u+'-asm.log')).open('w') as log:subprocess.run(asmflags+['-S','-o',str(O/(key+'-'+u+'.s')),'src/'+u+'.c'],cwd=R,env=env,stdout=log,stderr=subprocess.STDOUT,check=True)
 for u in ['renderer_stormrail_unit','main','renderer_dispatch','game','audio_mix','level1_audio','platform_amiga']:
  assert digest(O/('A-plain-'+u+'.s'))==digest(O/('B-plain-'+u+'.s')),u
 assert proof['A-plain']['sha256']=='e80b9c41de04bca7a519b9afaca05fdf9297684f70c6b98454e97c145795c470','Unflagged campaign baseline changed'
 # Same observer instrumentation in both interlude units; only Level1 changes.
 assert digest(O/'A-cadence-renderer_stormrail_unit.s')==digest(O/'B-cadence-renderer_stormrail_unit.s')
 (O/'builds.json').write_text(json.dumps(proof,indent=2)+'\n');(O/'source-hashes.json').write_text(json.dumps(inputs,indent=2)+'\n')
 print(json.dumps({k:{a:v for a,v in p.items() if a!='command'} for k,p in proof.items()},indent=2))
if __name__=='__main__':main()
