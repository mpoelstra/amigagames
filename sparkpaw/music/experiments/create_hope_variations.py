"""Three original scene-shaped revisions of A Shard of Hope."""
from pathlib import Path
import subprocess,sys,json
ROOT=Path(__file__).resolve().parent
source=(ROOT/'intro-variations/III-A-Shard-of-Hope/generate.py').read_text()
variants=[('IV-Morning-Sparks',[6,6,8,6,6],[132,128,120,130,132]),
 ('V-Clockwork-Dawn',[7,6,10,6,7],[150,132,150,132,150]),
 ('VI-Skybound',[7,7,11,7,8],[150,150,164,150,172])]
for kind,(name,counts,tempos) in enumerate(variants):
 out=ROOT/'hope-variations'/name;out.mkdir(parents=True,exist_ok=True)
 s=source.replace('BPM,SPEED,BARS=126,6,32',f'BPM,SPEED,BARS={tempos[0]},6,{sum(counts)}').replace('III-A-Shard-of-Hope',name).replace('III-A-SHARD-OF-HOPE',name.upper())
 a=s.index('# Scene boundaries:');b=s.index('# Both timing commands',a)
 arr='''COUNTS=COUNTS_VALUE
TEMPOS=TEMPOS_VALUE
KIND=KIND_VALUE
# Clear tonal arc: G-major morning, minor machinery/storm, major hopeful return.
chords=[['G','C','G','D','C','D','G'],['Em','Em','Am','B7','Em','B7','B7'],
 ['Em','Am','Em','B7','C','Am','Em','B7','Am','B7','B7'],
 ['C','C','G','D','Em','D','D'],['C','D','G','G','C','D','G','G']]
roots={'Em':40,'C':36,'D':38,'B7':35,'Am':33,'G':31}
# The hopeful seed (E-B-G-E) returns after a sunnier major opening.
themes=[
 [[67,71,69,67,64,66,67],[64,67,64,63,59,63,66],[64,69,67,63,64,60,64,66,69,63,66],[67,71,67,69,64,66,69],[67,69,71,67,64,66,67,67]],
 [[71,67,64,69,67,66,67],[64,59,60,63,67,66,63],[67,64,69,66,67,64,64,63,69,66,63],[64,67,71,69,67,66,69],[67,69,71,67,69,66,67,67]],
 [[67,71,67,69,64,66,67],[64,67,69,66,64,63,59],[64,69,67,66,67,69,64,63,69,66,63],[67,71,67,69,67,66,69],[67,69,71,67,64,69,71,67]]]
bar=0
scene_times=[]
for scene,(count,tempo) in enumerate(zip(COUNTS,TEMPOS)):
 scene_times.append(sum(COUNTS[i]*240/TEMPOS[i] for i in range(scene)))
 for local in range(count):
  chord=chords[scene][local];m=themes[KIND][scene][local];root=roots[chord]+12
  # Broad harmony, reset only on change or after two bars to preserve sustain.
  if local==0 or local%2==0 or chord!=chords[scene][local-1]:
   note(bar,0,2,CHORD[chord],48,27 if scene==0 else 31)
  # Keep long connected lead phrases; a few answering quarter notes add life.
  note(bar,0,3,LEAD,m,38 if scene==0 else 42)
  if local%2==1 and local+1<count:
   response=themes[KIND][scene][local+1]
   note(bar,8 if KIND==2 else 12,3,LEAD,response,39 if scene==0 else 43)
  else:put(bar*16+3,3,fx=4,arg=0x11)
  # Three different bass grooves: lilting, syncopated, driving eighth notes.
  steps=([0,6,10] if KIND==0 else [0,6,8,14] if KIND==1 else [0,4,8,12])
  if scene==0:steps=[0,8] if KIND==0 else [0,6,12]
  if KIND==2 and scene in (2,4):steps=list(range(0,16,2))
  for k,step in enumerate(steps):
   pitch=root+(7 if k%3==2 and chord!='B7' else 0)
   note(bar,step,1,BASS,pitch,32 if scene==0 else 38)
  # Morning is buoyant but light. Middle introduces low toms and tension;
  # finale opens into an energetic backbeat instead of falling back to sleep.
  if KIND==0:
   beat=[(0,KICK,31),(6,HAT,20),(8,SNARE,28),(14,HAT,18)]
  elif KIND==1:
   beat=[(0,KICK,34),(3,HAT,18),(6,KICK,28),(8,SNARE,33),(11,HAT,20),(14,KICK,26)]
  else:
   beat=[(0,KICK,37),(2,HAT,19),(4,SNARE,34),(6,HAT,19),(8,KICK,36),(10,HAT,19),(12,SNARE,35),(14,OPEN,20)]
  if scene==0:beat=[(0,KICK,25),(4,HAT,16),(8,BELL,23),(12,HAT,17)]
  if scene==1:beat=[(0,KICK,30),(4,HAT,16),(8,TOM,27),(12,HAT,18)]
  if local==count-1 and scene in (1,2,3):
   beat=[e for e in beat if e[0]<12]+[(12,TOM,28),(14,TOM,32)]
  for step,ins,vol in beat:note(bar,step,0,ins,67 if ins==BELL else 48,vol)
  if local==0:
   # Tempo changes are authored in the MOD, not playback-rate MP3 edits.
   grid[bar*16][2][2:]=[15,tempo]
   put(bar*16+1,2,fx=12,arg=27 if scene==0 else 31)
  bar+=1
# Resolve to G major, leaving a long final lead before a gentle terminal fade.
for row in range((BARS-1)*16+8,ROWS):
 for ch in range(4):grid[row][ch]=[0,0,12,round(30*(ROWS-1-row)/7)]
expected_seconds=sum(n*240/t for n,t in zip(COUNTS,TEMPOS))
'''.replace('COUNTS_VALUE',repr(counts)).replace('TEMPOS_VALUE',repr(tempos)).replace('KIND_VALUE',str(kind))
 s=s[:a]+arr+s[b:]
 s=s.replace('assert abs(seconds-BARS*4*60/BPM)<.15,seconds','assert abs(seconds-expected_seconds)<.15,seconds')
 s=s.replace("'bpm':BPM,","'bpm':BPM,'scene_tempos':TEMPOS,'scene_start_seconds':scene_times,")
 s=s.replace('extended original melody and four-channel arrangement','bright morning, dark middle, hopeful return; scene tempo changes')
 (out/'generate.py').write_text(s)
 with (out/'generation-report.txt').open('w') as f:subprocess.run([sys.executable,str(out/'generate.py')],stdout=f,check=True)
 subprocess.run(['ffmpeg','-y','-v','error','-i',str(out/(name+'-preview.wav')),'-codec:a','libmp3lame','-b:a','192k',str(out/(name+'-preview.mp3'))],check=True)
 print(name,json.loads((out/'manifest.json').read_text())['render_seconds'],flush=True)
