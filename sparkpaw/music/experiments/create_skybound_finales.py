"""Preserve accepted Skybound opening, offer three energetic 164-BPM finales."""
from pathlib import Path
import subprocess,sys,json
ROOT=Path(__file__).resolve().parent
source=(ROOT/'hope-variations/VI-Skybound/generate.py').read_text()
for mode,name in enumerate(['VII-Sparkpaw-Rising','VIII-Hero-Drive','IX-Shards-of-Victory']):
 out=ROOT/'skybound-finales'/name;out.mkdir(parents=True,exist_ok=True)
 s=source.replace('VI-Skybound',name).replace('VI-SKYBOUND',name.upper()).replace("header=b'"+name.upper()+"'.ljust", "header=b'"+name.upper()[:20]+"'.ljust")
 s=s.replace('TEMPOS=[150, 150, 164, 150, 172]','TEMPOS=[150, 150, 164, 164, 164]')
 s=s.replace('scene in (2,4)','scene in (2,3,4)')
 a=s.index('# Resolve to G major');b=s.index('expected_seconds=',a)
 extra='''# First 25 bars (~38.5s) are retained exactly. Rewrite only the finale.
MODE=MODE_VALUE
lines=[
 [[67,71],[69,66],[71,67],[69,66],[67,71],[69,66],[69,66],
  [67,71],[69,66],[71,67],[67,71],[69,67],[69,66],[71,69],[67]],
 [[64,67],[67,71],[71,67],[69,66],[67,71],[66,69],[69,66],
  [67,64],[69,66],[71,67],[67,71],[69,67],[66,69],[71,69],[67]],
 [[67,71],[71,67],[67,71],[69,66],[67,64],[66,69],[69,66],
  [67,71],[69,66],[71,67],[67,71],[69,67],[69,66],[71,69],[67]]
][MODE]
for bar in range(25,BARS):
 for step in range(16):grid[bar*16+step][3]=[0,0,0,0]
 phrase=lines[bar-25]
 for k,m in enumerate(phrase):
  note(bar,k*8,3,LEAD,m,44 if MODE==0 else 43)
  put(bar*16+k*8+3,3,fx=4,arg=0x11)
 if MODE==1:
  # Four-on-the-floor energy using the existing drum voice; no extra channel.
  for step in range(16):grid[bar*16+step][0]=[0,0,0,0]
  for step in (0,4,8,12):note(bar,step,0,KICK,48,39)
  for step in (2,6,10,14):note(bar,step,0,SNARE if step in (6,14) else HAT,48,30 if step in (6,14) else 20)
 if MODE==2 and bar in (28,30,34,36,38):
  # Brief tracker ornaments in the harmony voice, while the long lead sustains.
  for step in (10,12,14):
   grid[bar*16+step][2]=[0,0,0,0]
   note(bar,step,2,ARP,[67,71,69][(step-10)//2],23)
 if bar==38:
  # Original rising/answering flourish, resolving into the final tonic.
  for step in range(8,16):grid[bar*16+step][3]=[0,0,0,0]
  for step,m in [(8,67),(10,69),(12,71),(14,69)]:note(bar,step,3,LEAD,m,45)
# Final tonic arrives with a crash; release only at the end, never slow down.
grid[(BARS-1)*16][0]=[0,0,0,0]
note(BARS-1,0,0,CRASH,48,42)
for row in range((BARS-1)*16+12,ROWS):
 for ch in range(4):grid[row][ch]=[0,0,12,round(30*(ROWS-1-row)/3)]
'''.replace('MODE_VALUE',str(mode))
 s=s[:a]+extra+s[b:]
 s=s.replace('original title candidate; user listening pending','original intro finale candidate; user listening pending')
 s=s.replace('bright morning, dark middle, hopeful return; scene tempo changes','accepted opening preserved; continuous 164 BPM hopeful finale')
 (out/'generate.py').write_text(s)
 with (out/'generation-report.txt').open('w') as f:subprocess.run([sys.executable,str(out/'generate.py')],stdout=f,check=True)
 subprocess.run(['ffmpeg','-y','-v','error','-i',str(out/(name+'-preview.wav')),'-codec:a','libmp3lame','-b:a','192k',str(out/(name+'-preview.mp3'))],check=True)
 print(name,json.loads((out/'manifest.json').read_text())['render_seconds'],flush=True)
