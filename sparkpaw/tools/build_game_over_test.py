"""Build the normal HD campaign with one starting life, outside dist."""
from pathlib import Path
import os,shlex,subprocess,json,hashlib
ROOT=Path(__file__).resolve().parents[1]
out=ROOT/'build/game-over-work';out.mkdir(exist_ok=True)
dry=subprocess.check_output(['make','-nB','build/sparkpaw-campaign-play'],cwd=ROOT,text=True)
commands=[shlex.split(line) for line in dry.replace('\\\n',' ').splitlines() if line.startswith(str(ROOT/'.toolchain/sdk/bin/vc'))]
cmd=next(c for c in commands if '-o' in c and c[c.index('-o')+1]=='build/sparkpaw-campaign-play')
exe=out/'GameOver-1Life';cmd[cmd.index('-o')+1]=str(exe);cmd.insert(1,'-DSPARKPAW_GAME_OVER_TEST')
sdk=ROOT/'.toolchain/sdk';env=dict(os.environ,VBCC=str(sdk),PATH=str(sdk/'bin')+os.pathsep+os.environ['PATH'],TMPDIR=str(out))
with (out/'build-1life.log').open('w') as log:subprocess.run(cmd,cwd=ROOT,env=env,stdout=log,stderr=log,check=True)
(out/'build-1life.json').write_text(json.dumps(dict(command=cmd,sha256=hashlib.sha256(exe.read_bytes()).hexdigest()),indent=2)+'\n')
print(exe)
