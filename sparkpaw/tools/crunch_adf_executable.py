"""Shrinkler pack an ADF executable, retaining verifier output and byte binding."""
from pathlib import Path
import subprocess,sys,json,hashlib,platform
ROOT=Path(__file__).resolve().parents[1]
def main():
 source=ROOT/'build/multidisk-probe/Sparkpaw'
 target=ROOT/'build/multidisk-probe/Sparkpaw-crunched'
 upstream=ROOT/'third_party/shrinkler'
 kind='mac' if platform.system()=='Darwin' else 'native'
 build=ROOT/'build/shrinkler'
 build.mkdir(exist_ok=True)
 with (build/'build.log').open('w') as log:
  subprocess.run(['make','PLATFORM='+kind,'BUILD_DIR='+str(build)],cwd=upstream,stdout=log,stderr=log,check=True)
 command=[str(build/'Shrinkler'),'-1','-p',str(source),str(target)]
 result=subprocess.run(command,text=True,capture_output=True,check=True)
 target.with_suffix('.log').write_text(result.stdout+result.stderr)
 if 'Verifying... OK' not in result.stdout:raise SystemExit('Shrinkler verification missing')
 def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
 target.with_suffix('.json').write_text(json.dumps(dict(command=command,input_sha256=sha(source),output_sha256=sha(target),input_bytes=source.stat().st_size,output_bytes=target.stat().st_size,verification='Shrinkler host hunk/relocation verifier; native boot pending',upstream='17cff110fcded387fe90e632805258d9c8359e94'),indent=2)+'\n')
 print(target,source.stat().st_size,'->',target.stat().st_size)
if __name__=='__main__':main()
