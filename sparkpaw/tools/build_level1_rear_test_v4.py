#!/usr/bin/env python3
"""Isolated native baseline/candidate compilation; never launch or release."""
import argparse, hashlib, json, os, shlex, subprocess
from pathlib import Path

R=Path(__file__).resolve().parents[1]
O=R/'build/level1-electric-v4-20260926'
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--baseline',action='store_true');a=ap.parse_args()
    O.mkdir(parents=True,exist_ok=True)
    plan=subprocess.check_output(['make','-Bn','build/sparkpaw-campaign-play','PYTHON=../.venv/bin/python3'],cwd=R,text=True).replace('\\\n',' ')
    args=shlex.split(next(l for l in plan.splitlines() if '+aos68k' in l and '-o build/sparkpaw-campaign-play' in l))
    i=args.index('-o');del args[i:i+2]
    # Keep historical candidates/baseline independent of current release defaults.
    args=[v for v in args if v not in ('-DSPARKPAW_LEVEL1_REAR_AMBIENCE','-DSPARKPAW_LEVEL1_REAR_AMBIENCE_RELEASE')]
    assert '-DSPARKPAW_LEVEL1_TWO_COPY_RING' in args
    name='baseline' if a.baseline else 'candidate'
    extra=['-DSPARKPAW_LEVEL1_RING_TEST','-DSPARKPAW_DROWNED_FPS','-DSPARKPAW_DISABLE_WAIT_PROFILE',
           '-DSPARKPAW_DROWNED_FPS_BUILD_ID=l1electric_v4_'+name,'src/drowned_fps.c']
    if not a.baseline: extra+=['-DSPARKPAW_LEVEL1_REAR_AMBIENCE','-I'+str(O)]
    env=dict(os.environ,VBCC=str(R/'.toolchain/sdk'),TMPDIR=str(R/'build/tmp'))
    env['PATH']=str(R/'.toolchain/sdk/bin')+':'+env['PATH']
    command=args+extra+['-o',str(O/name)]
    with (O/(name+'-compile.log')).open('w') as log:
        subprocess.run(command,cwd=R,env=env,stdout=log,stderr=subprocess.STDOUT,check=True)
    flags=[v for v in args+extra if not v.endswith(('.c','.o'))]
    for unit in ('renderer_level1_unit','renderer_stormrail_unit','game','main','audio_mix','platform_amiga'):
        with (O/(name+'-'+unit+'-asm.log')).open('w') as log:
            subprocess.run(flags+['-S','-o',str(O/(name+'-'+unit+'.s')),'src/'+unit+'.c'],cwd=R,env=env,stdout=log,stderr=subprocess.STDOUT,check=True)
    result={'command':command,'bytes':(O/name).stat().st_size,'sha256':hashlib.sha256((O/name).read_bytes()).hexdigest()}
    (O/(name+'.json')).write_text(json.dumps(result,indent=2)+'\n')
    print(name,result['bytes'],result['sha256'])
if __name__=='__main__':main()
