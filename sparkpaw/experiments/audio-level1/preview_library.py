"""Build the pinned host-only preview replayer from tracked source."""
from pathlib import Path
import subprocess,sys
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[1]
def library():
    target=ROOT/'build/audio-level1'/('libmicromod.dylib' if sys.platform=='darwin' else 'libmicromod.so')
    target.parent.mkdir(parents=True,exist_ok=True)
    source=HERE/'third_party/micromod/micromod.c'
    if not target.exists() or source.stat().st_mtime>target.stat().st_mtime:
        subprocess.run(['cc','-O2','-shared','-fPIC',str(source),'-o',str(target)],check=True)
    return target
