"""Build an unnumbered, no-intro disk-only campaign executable outside dist."""
from pathlib import Path
import json,os,subprocess,hashlib,shlex
ROOT=Path(__file__).resolve().parents[1]
def main():
 out=ROOT/'build/multidisk-probe';out.mkdir(exist_ok=True)
 dry=subprocess.check_output(['make','-nB','build/sparkpaw-campaign-play'],cwd=ROOT,text=True)
 commands=[shlex.split(line) for line in dry.replace('\\\n',' ').splitlines() if line.startswith(str(ROOT/'.toolchain/sdk/bin/vc'))]
 matches=[c for c in commands if '-o' in c and c[c.index('-o')+1]=='build/sparkpaw-campaign-play']
 assert len(matches)==1, 'production compile command must be unambiguous'
 cmd=matches[0];flags=cmd[1:cmd.index('-o')]
 flags=[f for f in flags if f!='-DSPARKPAW_STORY_INTRO']
 flags+=['-DSPARKPAW_MULTI_ADF','-DADF_PACKED_ASSETS']
 sources=cmd[cmd.index('-o')+2:]+['src/disk_media.c'];sdk=ROOT/'.toolchain/sdk'
 env=dict(os.environ,VBCC=str(sdk),PATH=str(sdk/'bin')+os.pathsep+os.environ['PATH'],TMPDIR=str(out))
 exe=out/'Sparkpaw'
 with (out/'compile.log').open('w') as log:
  subprocess.run([cmd[0],*flags,'-o',str(exe),*sources],cwd=ROOT,env=env,stdout=log,stderr=log,check=True)
 (out/'build.json').write_text(json.dumps({'flags':flags,'sources':sources,'source_sha256':{p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources+['src/renderer.c']},'sha256':hashlib.sha256(exe.read_bytes()).hexdigest(),'bytes':exe.stat().st_size},indent=2))
 print(exe,exe.stat().st_size)
if __name__=='__main__':main()
