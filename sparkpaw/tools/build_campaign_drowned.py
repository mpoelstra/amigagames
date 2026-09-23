#!/usr/bin/env python3
"""Link the three-section HD candidate. Never stage, launch or release."""
from pathlib import Path
import hashlib,json,os,shlex,subprocess,sys
from hunk_namespace import namespace_objects,transform
R=Path(__file__).resolve().parents[1];O=R/'build/campaign-drowned'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def command(target):
 plan=subprocess.check_output(['make','-Bn',target,'PYTHON=../.venv/bin/python3'],cwd=R,text=True).replace('\\\n',' ')
 args=shlex.split(next(l for l in plan.splitlines() if '+aos68k' in l and '-o '+target in l));i=args.index('-o');del args[i:i+2];return args

def main():
 whdpacked=sys.argv[1:]==['--whdload-packed'];whddiag=sys.argv[1:]==['--whdload-diag'];whdload=sys.argv[1:]==['--whdload'] or whdpacked or whddiag;adf=sys.argv[1:]==['--adf']
 if sys.argv[1:] and not (whdload or adf):raise SystemExit('usage: build_campaign_drowned.py [--whdload|--whdload-packed|--whdload-diag|--adf]')
 out=O/('whdload-packed' if whdpacked else 'whdload-diag' if whddiag else 'whdload' if whdload else 'adf') if (whdload or adf) else O
 out.mkdir(parents=True,exist_ok=True);sdk=R/'.toolchain/sdk';env=dict(os.environ,VBCC=str(sdk));env['PATH']=str(sdk/'bin')+':'+env['PATH']
 host=command('build/sparkpaw-campaign-play')+['-DSPARKPAW_CAMPAIGN_DROWNED']
 module=command('build/sparkpaw-drowned-full')+['-DSPARKPAW_DROWNED_CAMPAIGN_MODULE']
 if whdload:host+=['-DSPARKPAW_WHDLOAD'];module+=['-DSPARKPAW_WHDLOAD']
 if whddiag:module+=['-DSPARKPAW_WHDLOAD_DIAG','-DSPARKPAW_STARTUP_DIAGNOSTIC']
 if whdpacked:
  host+=['-DSPARKPAW_WHD_PACKED','-DADF_PACKED_ASSETS']
  module+=['-DSPARKPAW_WHD_PACKED','-DADF_PACKED_ASSETS']
 if adf:
  flags=['-DSPARKPAW_MULTI_ADF','-DADF_PACKED_ASSETS','-DSPARKPAW_THREE_ADF','-DSPARKPAW_DROWNED_THREE_ADF']
  host+=flags;module+=flags
  host=[a for a in host if a!='-DSPARKPAW_STORY_INTRO']
  module=[a for a in module if a!='-DSPARKPAW_STORY_INTRO']
  host.append('src/disk_media.c');module.append('src/disk_media.c')
 sources=[a for a in module[1:] if a.endswith(('.c','.o'))];flags=[a for a in module[1:] if a not in sources];objects=[]
 with (out/'module-compile.log').open('w') as log:
  for i,src in enumerate(sources):
   if src=='src/main.c':src='src/drowned_campaign.c'
   if src.endswith('.o'):objects.append(R/src);continue
   p=out/f'module-{i:02d}-{Path(src).stem}.o';objects.append(p)
   subprocess.run([module[0],*flags,'-c','-o',str(p),src],cwd=R,env=env,stdout=log,stderr=log,check=True)
 renamed,names=namespace_objects(objects,out/'namespaced')
 (out/'namespace.json').write_text(json.dumps(names,indent=2)+'\n')
 mod=out/'drowned-module.o'
 subprocess.run([str(sdk/'bin/vlink'),'-bamigahunk','-r','-x',*[str(p) for p in renamed],'-o',str(mod)],cwd=R,env=env,check=True)
 definitions=transform(mod.read_bytes())[1]
 assert all(s=='_drownedCampaignRun' or s.startswith('__drowned_') for s in definitions),definitions
 exe=out/'Sparkpaw-Campaign';cmd=host+[str(mod),'-o',str(exe)]
 with (out/'campaign-compile.log').open('w') as log:subprocess.run(cmd,cwd=R,env=env,stdout=log,stderr=log,check=True)
 manifest={'command':cmd,'module_command':module,'bytes':exe.stat().st_size,'sha256':digest(exe),'namespaced_symbols':len(names),'module_exports':sorted(definitions),'release':False,'whdload':whdload,'whdpacked':whdpacked,'whddiag':whddiag,'adf':adf}
 (out/'build.json').write_text(json.dumps(manifest,indent=2)+'\n');print('Built',manifest['bytes'],manifest['sha256'],'isolated symbols',len(names))
if __name__=='__main__':main()
