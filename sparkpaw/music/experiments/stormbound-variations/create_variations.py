"""Freeze three independent, reproducible variations of the preferred Smooth score."""
from pathlib import Path
import subprocess,sys
ROOT=Path(__file__).resolve().parent
base=(ROOT.parent/'stormbound-smooth/generate.py').read_text()
for label,bpm in [('A-Stormdrive',152),('B-Skyward',144),('C-Paula-Power',150)]:
    out=ROOT/label;out.mkdir(exist_ok=True)
    s=base.replace('BPM,SPEED,BARS=144,6,48',f'BPM,SPEED,BARS={bpm},6,48')
    s=s.replace("libpath=OUT.parents[2]/'build", "libpath=OUT.parents[3]/'build")
    s=s.replace('t=time(3.8);f=freq(48)', 't=time(6.5);f=freq(48)')
    s=s.replace('(.28+.18*h)','(.12+.11*h)')
    s=s.replace("'FLOWING STORM LEAD'",repr(label+' LEAD'))
    s=s.replace("b'STORMBOUND SMOOTH'",repr(label.upper().encode()))
    s=s.replace('Stormbound-Smooth',label).replace('Sparkpaw — Stormbound Smooth','Sparkpaw — '+label)
    s=s.replace("'v2: connected melody, even eighth-note bass, regular backbeat, sustained chords, no lead note cuts'",repr(label+': extended original melody and four-channel arrangement'))
    # More melodic continuity in the bridge and turnaround for every version.
    a=s.index('bridge=[[');b=s.index('phrases=intro+',a)
    s=s[:a]+'''bridge=[[(0,69,8),(8,64,8)],[(0,67,8),(8,64,8)],
 [(0,67,8),(8,64,8)],[(0,66,8),(8,63,8)],
 [(0,64,4),(4,69,8),(12,67,4)],[(0,67,8),(8,64,8)],
 [(0,66,8),(8,69,8)],[(0,63,4),(4,66,4),(8,71,8)]]
last=[[(0,67,8),(8,64,8)],[(0,69,8),(8,66,8)],
 [(0,71,8),(8,66,8)],[(0,63,8),(8,66,8)]]
'''+s[b:]
    if label!='B-Skyward':
        # Every beat has a kick; sample the kick+snare combination into one
        # drum voice, rather than pretending MOD has a fifth channel.
        s=s.replace("SNARE=add('STEEL SNARE + HAT',echo(snare,.08),55)","""backkick=np.sin(TAU*(48*t+120*(1-np.exp(-t*45))/45))*np.exp(-t*18)
SNARE=add('KICK SNARE STACK',echo(snare,.08)+.70*backkick,59)""")
        s=s.replace('4:(SNARE,48,55)','4:(SNARE,48,59)').replace('12:(SNARE,48,55)','12:(SNARE,48,59)')
        s=s.replace('51 if j%4==0 else 44','45 if j%2==0 else 55')
        # The offbeat bass accents give a pumping groove without mastering
        # sidechain processing or changing any gameplay audio channel.
        s=s.replace('[(0,30),(8,27)]','[(0,22),(2,33),(8,22),(10,31)]')
        s=s.replace('np.minimum(t/.035,1)*np.exp(-t*1.4)','np.minimum(t/.02,1)*np.exp(-t*1.6)')
    if label=='B-Skyward':
        s=s.replace('t=time(2.0);x=np.zeros_like(t)','t=time(3.0);x=np.zeros_like(t)')
        s=s.replace('np.exp(-t*1.4)','np.exp(-t*.65)')
        s=s.replace('[(0,30),(8,27)]','[(0,34)]')
        a=s.index('A=[[');b=s.index('intro=[[',a)
        s=s[:a]+'''A=[[(0,64,8),(8,71,8)],[(0,69,8),(8,67,8)],
 [(0,67,4),(4,71,8),(12,69,4)],[(0,66,12),(12,62,4)],
 [(0,64,8),(8,67,4),(12,71,4)],[(0,69,8),(8,64,8)],
 [(0,69,12),(12,67,4)],[(0,66,8),(8,63,8)]]
B=[[(0,67,12),(12,69,4)],[(0,69,8),(8,66,8)],
 [(0,71,12),(12,67,4)],[(0,67,8),(8,62,8)],
 [(0,69,8),(8,64,8)],[(0,67,4),(4,69,4),(8,71,8)],
 [(0,69,8),(8,66,8)],[(0,63,8),(8,71,8)]]
'''+s[b:]
        s=s.replace('[(1,1),(2,.40),(3,.26),(4,.10),(5,.07),(7,.025)]','[(1,1),(2,.32),(3,.30),(4,.08),(5,.12),(7,.045)]')
    if label=='C-Paula-Power':
        s=s.replace('[(1,1),(2,.40),(3,.26),(4,.10),(5,.07),(7,.025)]','[(1,1),(2,.14),(3,.32),(4,.05),(5,.16),(7,.09),(9,.045)]')
        s=s.replace('t=time(.22);p=TAU*freq(48)*t','t=time(.55);p=TAU*freq(48)*t')
        s=s.replace('np.exp(-t*19),32)','np.exp(-t*7),32)')
        s=s.replace('    if bridge:\n        tones=', '    if bridge or (bar>=4 and bar%4 in (1,3)):\n        tones=')
        s=s.replace("'B7':[57,59,63,66]}[chord]", "'B7':[57,59,63,66],'G':[55,59,62,67]}[chord]")
        # A few authentic tick-arpeggio gestures on the backing voice. These
        # never chop up the sustained melody on channel four.
        marker='# Both timing commands'
        inject='''for bar in [5,9,17,29,33,41]:
    # Em/C-family minor/major color follows the written backing root.
    for step in [1,5,9,13]:
        put(bar*16+step,2,fx=0,arg=0x37 if progression[bar]=='Em' else 0x47)
'''
        s=s.replace(marker,inject+marker)
    (out/'generate.py').write_text(s)
    subprocess.run([sys.executable,str(out/'generate.py')],check=True,stdout=(out/'generation-report.txt').open('w'))
    subprocess.run(['ffmpeg','-y','-v','error','-i',str(out/(label+'-preview.wav')),
                    '-codec:a','libmp3lame','-b:a','192k',str(out/(label+'-preview.mp3'))],check=True)
    print(label)
