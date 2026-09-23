"""Compile the real full renderer and reject vbcc mask-register readback."""
from pathlib import Path
import os,re,shlex,subprocess
R=Path(__file__).resolve().parents[1]
text=subprocess.check_output(['make','-Bn','drowned-full','PYTHON=../.venv/bin/python3'],cwd=R,text=True).replace('\\\n',' ')
line=next(line for line in text.splitlines() if '+aos68k' in line and '-o build/sparkpaw-drowned-full' in line)
args=shlex.split(line);i=args.index('-o');del args[i:i+2]
args=[arg for arg in args if not arg.endswith(('.c','.o'))]
output=R/'build/drowned-full/rear-mask-audit.s'
args+=['-S','-o',str(output),'src/renderer.c']
env=dict(os.environ,VBCC=str(R/'.toolchain/sdk'))
env['PATH']=str(R/'.toolchain/sdk/bin')+':'+env['PATH']
subprocess.run(args,cwd=R,env=env,check=True)
asm=output.read_text()
readback=r'move\.w\s+\(a\d\),\(68,a\d\)'
assert not re.search(readback,asm),'write-only last-mask register readback into first mask'
separate=r'move\.w\s+#-1,\(68,(a\d)\)\s+move\.w\s+#-1,\(70,\1\)'
assert re.search(separate,asm),'expected separate immediate mask writes missing'
before=R/'build/drowned-full/rear-debug-before-fix.s'
if before.exists():
    assert re.search(readback,before.read_text()),'saved failing native output must reproduce readback'
print('PASS native -O2/68020: separate immediate mask writes; no prior write-only readback pattern')
