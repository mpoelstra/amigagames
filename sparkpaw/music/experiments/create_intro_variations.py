"""Five-scene original introduction score studies, matching the runtime's ~61s."""
from pathlib import Path
import subprocess,sys
ROOT=Path(__file__).resolve().parent
source=(ROOT/'stormbound-synth/E-Neon-Sky/generate.py').read_text()
for variant in ['I-The-Sleeping-Sky','II-Archivolts-Lament','III-A-Shard-of-Hope']:
 out=ROOT/'intro-variations'/variant;out.mkdir(parents=True,exist_ok=True)
 s=source.replace('BPM,SPEED,BARS=160,6,48','BPM,SPEED,BARS=126,6,32').replace('E-Neon-Sky',variant).replace('E-NEON-SKY',variant.upper()[:20])
 s=s.replace('t=time(3.0);x=np.zeros_like(t)','t=time(6.5);x=np.zeros_like(t)').replace('np.minimum(t/.035,1)*np.exp(-t*.65)','np.minimum(t/.18,1)*np.exp(-t*.27)')
 s=s.replace('t=time(.70);p=TAU*freq(36)*t','t=time(2.5);p=TAU*freq(36)*t').replace('np.exp(-t*3.5),53','np.exp(-t*1.1),45')
 s=s.replace('lead*=np.minimum(t/.008,1)','lead*=np.minimum(t/.065,1)').replace('exp(-h/cutoff)','exp(-h/(cutoff*.70))')
 a=s.index('# 4-bar ignition');b=s.index('# Both timing commands',a)
 mood=['I-The-Sleeping-Sky','II-Archivolts-Lament','III-A-Shard-of-Hope'].index(variant)
 arrangement='''# Scene boundaries: 0, 6, 12, 20, 26 bars (~0,11.4,22.9,38.1,49.5s).
# Harmony changes every two bars so text can breathe over sustaining textures.
progression=['Em','Em','C','C','G','G','Am','Am','B7','B7','Em','B7',
 'Em','Em','Am','Am','C','C','B7','B7','C','C','G','G','D','D',
 'C','C','D','D','Em','Em']
roots={'Em':40,'C':36,'D':38,'B7':35,'Am':33,'G':31}
MOOD=MOOD_VALUE
# A continuous original motif moves from open fifths to darker chromatic
# responses, then rises into the hopeful final view of Sparkpaw and the tower.
melodies=[
 [64,67,71,69,67,62,64,60,63,66,64,63,64,67,69,64,67,64,66,63,67,71,67,62,66,69,67,64,66,69,71,64],
 [59,64,67,64,62,67,60,64,63,59,64,66,67,64,60,64,67,64,63,66,64,67,62,67,66,69,64,67,69,66,67,64],
 [64,71,67,64,67,62,64,69,66,63,64,59,64,67,69,64,67,72-12,66,63,67,71,67,69,66,69,67,71,69,66,67,64]
]
for bar,chord in enumerate(progression):
 scene=0 if bar<6 else 1 if bar<12 else 2 if bar<20 else 3 if bar<26 else 4
 root=roots[chord]+12
 # One slowly moving harmony voice; no rapid retriggered chord stabs.
 if bar%2==0:note(bar,0,2,CHORD[chord],48,25+scene*2)
 # Bass is restrained and regular, opening up in the storm/quest scenes.
 note(bar,0,1,BASS,root,28+scene*3)
 if scene in (2,4):note(bar,8,1,BASS,root,30+scene*2)
 # Long melodic line: mostly whole bars with occasional connected responses.
 m=melodies[MOOD][bar]
 note(bar,0,3,BELL if scene==0 and MOOD==1 else LEAD,m,34+scene*3)
 if bar%4==3 and bar<30:
  response=melodies[MOOD][bar+1]
  note(bar,12,3,LEAD,response,35+scene*3)
 else:put(bar*16+4,3,fx=4,arg=0x11)
 # Free fourth channel starts as a distant bell, becomes a slow storm pulse.
 if scene==0:
  if bar%2==0:note(bar,8,0,BELL,71 if chord=='Em' else 67,22)
 elif scene==1:
  if MOOD==1:
   for step in (4,12):note(bar,step,0,ARP,63 if chord=='B7' else 64,19)
  elif bar%2==0:note(bar,8,0,BELL,66,20)
 else:
  note(bar,0,0,KICK,48,34 if scene==2 else 29)
  note(bar,8,0,SNARE if MOOD==2 else TOM,48,25 if scene==2 else 22)
  if MOOD==2 and scene==4:
   note(bar,4,0,HAT,48,18);note(bar,12,0,HAT,48,18)
# Closing held tonic and a gradual four-voice fade, no sudden chopped ending.
for row in range(31*16+1,ROWS):
 for ch in range(4):
  grid[row][ch]=[0,0,12,max(0,round(32*(ROWS-1-row)/14))]
'''.replace('MOOD_VALUE',str(mood))
 s=s[:a]+arrangement+s[b:]
 # These scores are rendered through the same actual MOD player as Neon Sky.
 s=s.replace('original title candidate; user listening pending','original intro candidate; user listening pending')
 (out/'generate.py').write_text(s)
 with (out/'generation-report.txt').open('w') as report:
  subprocess.run([sys.executable,str(out/'generate.py')],stdout=report,check=True)
 subprocess.run(['ffmpeg','-y','-v','error','-i',str(out/(variant+'-preview.wav')),'-codec:a','libmp3lame','-b:a','192k',str(out/(variant+'-preview.mp3'))],check=True)
 print(variant,flush=True)
