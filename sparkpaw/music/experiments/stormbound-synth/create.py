"""Original faster synth variations; no reference samples or melodies consumed."""
from pathlib import Path
import subprocess,sys,json
ROOT=Path(__file__).resolve().parent
base=ROOT.parent/'stormbound-variations'
variants=[('D-Voltage-Run',168,'A-Stormdrive'),('E-Neon-Sky',160,'B-Skyward'),('F-Turbo-Shards',176,'C-Paula-Power')]
for name,bpm,parent in variants:
 out=ROOT/name;out.mkdir(exist_ok=True)
 s=(base/parent/'generate.py').read_text()
 oldbpm={'A-Stormdrive':152,'B-Skyward':144,'C-Paula-Power':150}[parent]
 s=s.replace(f'BPM,SPEED,BARS={oldbpm},6,48',f'BPM,SPEED,BARS={bpm},6,48')
 s=s.replace(parent,name).replace(parent.upper(),name.upper())
 # Brighter evolving sampled analogue synth instead of the reed/brass-like voice.
 a=s.index('for det in [-.0014,.0014]:');b=s.index('LEAD=add(',a)
 detunes='[-.0028,0,.0028]' if name!='F-Turbo-Shards' else '[-.0018,.0018]'
 s=s[:a]+f'''for det in {detunes}:
    phase=TAU*f*(1+det)*t+.012*np.sin(TAU*5.3*t)*np.minimum(t/.3,1)
    for h in range(1,15):
        # Harmonic-limited saw/pulse hybrid with an opening then settling filter.
        amp=(1/h)*(1 if h%2 else .48)
        cutoff=3.5+5*np.exp(-t*1.7)
        lead+=amp*np.sin(h*phase+.06*h)*np.exp(-h/cutoff)*np.exp(-t*.24)
lead*=np.minimum(t/.008,1)
''' +s[b:]
 s=s.replace("'SHARD PULSE BASS'","'ANALOGUE OCTAVE BASS'")
 s=s.replace('np.tanh(1.4*bass)','np.tanh(1.9*bass)')
 s=s.replace('[(0,22),(2,33),(8,22),(10,31)]','[(0,25),(2,32),(8,25),(10,30)]')
 # Clockwork pluck becomes a sharper electric synth voice.
 s=s.replace('(np.sin(p)+.42*np.sin(2*p)+.2*np.sin(3*p))','(np.sin(p)+.30*np.sin(2*p)+.32*np.sin(3*p)+.12*np.sin(5*p))')
 s=s.replace('np.sin(p)+.22*np.sin(2*p)*np.exp(-t*4)+.07*np.sin(3*p)*np.exp(-t*9)', 'np.sin(p)+.34*np.sin(2*p)*np.exp(-t*1.2)+.18*np.sin(3*p)*np.exp(-t*2.4)+.07*np.sin(5*p)*np.exp(-t*4)')
 # New answering phrase for the electric A/C variants; B retains long arcs.
 if parent!='B-Skyward':
  a=s.index('B=[[');b=s.index('intro=[[',a)
  s=s[:a]+'''B=[[(0,64,4),(4,67,4),(8,69,8)],
 [(0,66,4),(4,69,4),(8,71,8)],
 [(0,71,8),(8,67,4),(12,64,4)],
 [(0,62,8),(8,67,8)],
 [(0,69,8),(8,64,4),(12,60,4)],
 [(0,64,4),(4,67,4),(8,71,8)],
 [(0,69,4),(4,66,4),(8,64,8)],
 [(0,63,4),(4,66,4),(8,71,8)]]
''' +s[b:]
 (out/'generate.py').write_text(s)
 with (out/'generation-report.txt').open('w') as f:
  subprocess.run([sys.executable,str(out/'generate.py')],check=True,stdout=f)
 subprocess.run(['ffmpeg','-y','-v','error','-i',str(out/(name+'-preview.wav')),'-codec:a','libmp3lame','-b:a','192k',str(out/(name+'-preview.mp3'))],check=True)
 j=json.loads((out/'manifest.json').read_text()); print(name,j['render_seconds'],j['peak_before_master'])
