"""Original Drowned Turbines review score: Undertow Circuit v6."""
from pathlib import Path
import ctypes, hashlib, json, struct, wave, sys
import numpy as np
OUT=Path(__file__).resolve().parent
ROOT=OUT.parents[1]
RATE=3546895/428
BPM=150
BARS=64
rng=np.random.default_rng(70919)
samples=[]
def t(sec): return np.arange(int(sec*RATE)//2*2)/RATE
def add(name,x,volume):
    x=np.asarray(x); x=x-x.mean(); n=min(24,len(x)//4)
    x[:n]*=np.linspace(0,1,n);x[-n:]*=np.linspace(1,0,n)
    b=np.rint(x/max(abs(x).max(),1e-9)*118).astype('int8').tobytes()
    b=b'\0\0'+b[2:] # native tracker idle-loop word; explicitly authored
    samples.append((name,b,volume));return len(samples)
def noise(tt,decay):
    x=rng.standard_normal(len(tt));return (x-np.roll(x,1)*.65)*np.exp(-tt*decay)
# Original wet-metal palette; tonal layers baked into samples, not mixed at runtime.
x=t(.22);phase=2*np.pi*(46*x+85*(1-np.exp(-x*36))/36)
k=add('SUBMERGED KICK',np.sin(phase)*np.exp(-x*20)+.035*noise(x,95),40)
x=t(.19);s=add('VALVE SNARE',.55*noise(x,31)+.35*np.sin(2*np.pi*173*x)*np.exp(-x*23),31)
x=t(.06);h=add('RAIN TICK',noise(x,85),14)
x=t(.18);o=add('BRUSHED METAL',noise(x,27)+.15*np.sin(2*np.pi*1750*x)*np.exp(-x*35),18)
x=t(.28);b=add('UNDERTOW BASS C',sum(np.sin(2*np.pi*65.406*j*x)*v for j,v in [(1,1),(2,.33),(3,.18)])*np.exp(-x*13),37)
x=t(.60);r=add('WARM REED C', (np.sin(2*np.pi*130.813*x)+.19*np.sin(2*np.pi*261.626*x)+.055*np.sin(2*np.pi*392.439*x))*np.minimum(x/.012,1)*np.exp(-x*6),28)
x=t(.42);c=add('HOLLOW FIFTH C',sum(np.sin(2*np.pi*f*x) for f in [130.813,195.998,261.626])*np.exp(-x*9),26)
x=t(.20);p=add('PRESSURE TOM',np.sin(2*np.pi*(96*x+48*(1-np.exp(-x*28))/28))*np.exp(-x*23),28)
x=t(.46);bell=add('RAIN LENS C',(np.sin(2*np.pi*130.813*x)+.38*np.sin(2*np.pi*261.626*x)*np.exp(-x*7)+.12*np.sin(2*np.pi*654.065*x)*np.exp(-x*17))*np.exp(-x*7),27)
periods=[856,808,762,720,678,640,604,570,538,508,480,453,428,404,381,360,339,320,302,285,269,254,240,226,214,202,190,180,170,160,151,143,135,127,120,113]
grid=[[[0,0,0,0] for ch in range(4)] for row in range(BARS*16)]
def note(bar,step,ch,inst,m,vol):grid[bar*16+step][ch]=[inst,periods[m-36],12,vol]
# A new, stepped 8-bit pulse and gently detuned tracker bell supply the extra
# colour. Each is one short Paula sample, never a runtime synth or extra voice.
x=t(.31)
square=np.tanh(2.2*(np.sin(2*np.pi*130.813*x)+.30*np.sin(2*np.pi*261.626*x)))
lead=add('TURBINE PULSE C',square*np.exp(-x*5.8)*np.minimum(x/.006,1),24)
x=t(.29)
glass=add('GLASS CURRENT C',(np.sin(2*np.pi*130.813*x)+.25*np.sin(2*np.pi*262.8*x)+.12*np.sin(2*np.pi*392.439*x))*np.exp(-x*8),23)
x=t(.15)
chord=add('VALVE CHORD C',(np.sin(2*np.pi*130.813*x)+.55*np.sin(2*np.pi*164.814*x)+.45*np.sin(2*np.pi*195.998*x))*np.exp(-x*20),17)
# New call/answer phrases and a second melody. Harmony changes every two bars;
# the last B7 bar leads directly back into the first E-minor bar at the loop.
roots=[40,40,43,43,38,38,36,47,45,45,48,48,38,38,47,47]*4
theme_a=[
 [(0,64),(2,67),(4,71),(7,69),(9,67),(12,66),(14,67)],
 [(0,64),(3,66),(5,67),(8,71),(10,69),(13,67)],
 [(0,67),(2,71),(5,69),(7,67),(10,66),(12,64),(14,62)],
 [(0,67),(3,69),(6,71),(8,67),(11,64),(14,66)],
 [(0,66),(2,69),(4,71),(7,69),(9,66),(12,64),(14,62)],
 [(0,66),(3,64),(5,62),(8,66),(10,69),(13,66)],
 [(0,64),(2,67),(5,69),(7,67),(10,64),(12,62),(14,60)],
 [(0,63),(3,66),(5,69),(8,66),(10,63),(13,59)] ]
theme_b=[
 [(0,71),(2,69),(4,67),(7,64),(9,67),(12,69),(14,71)],
 [(0,69),(3,67),(5,64),(8,66),(10,67),(13,69)],
 [(0,71),(2,69),(4,67),(7,66),(9,64),(12,62),(14,67)],
 [(0,69),(3,71),(5,67),(8,64),(10,66),(13,67)],
 [(0,69),(2,71),(4,69),(7,66),(9,64),(12,62),(14,66)],
 [(0,66),(3,69),(5,71),(8,69),(10,66),(13,64)],
 [(0,67),(2,69),(5,67),(7,64),(10,62),(12,64),(14,67)],
 [(0,66),(3,63),(5,59),(8,63),(10,66),(13,63)] ]
for bar,root in enumerate(roots):
    section=bar//8
    for st in [0,2,4,6,8,10,12,14]:
        inst=s if st in (4,12) else k if st in (0,8) else o if st==14 else h
        note(bar,st,0,inst,48,samples[inst-1][2])
    # Short chord punctuation gives a fuller tracker texture between drum hits.
    if bar%2==1:note(bar,15,0,chord,root+12,16)
    if bar%8==7:
        for st,m in [(13,53),(14,50),(15,48)]:note(bar,st,0,p,m,23)
    for st,interval in [(0,0),(3,0),(6,7),(8,0),(11,12),(14,7)]:
        note(bar,st,1,b,root+interval,34 if st in (0,8) else 27)
    phrase=(theme_a if section%2==0 else theme_b)[bar%8]
    for j,(st,pitch) in enumerate(phrase):
        inst=lead if section%4 in (0,3) else r
        if j in (2,5) or (section%4==2 and j%3==0):inst=glass
        note(bar,st,2,inst,pitch,24 if inst==lead else 25 if inst==r else 20)
# Every bar carries the same drive; the tune and lead colour evolve in sections.
for bar in range(BARS):
    assert sum(bool(row[0][0]) for row in grid[bar*16:(bar+1)*16])>=8
    assert sum(bool(row[1][0]) for row in grid[bar*16:(bar+1)*16])==6
    assert sum(bool(row[2][0]) for row in grid[bar*16:(bar+1)*16])>=6
grid[0][0][2:]=[15,6];grid[0][1][2:]=[15,BPM]
# Fourth track is entirely empty, even global commands use music tracks only.
assert all(row[3]==[0,0,0,0] for row in grid)
def enc(e):
    i,p,f,a=e;return bytes([(i&240)|(p>>8),p&255,((i&15)<<4)|f,a])
pat=BARS//4
header=b'UNDERTOW CIRCUIT V6'.ljust(20,b'\0')
for n,data,v in samples:header+=n.encode()[:22].ljust(22,b'\0')+struct.pack('>HBBHH',len(data)//2,0,v,0,1)
header+=bytes(30*(31-len(samples)))+bytes([pat,0])+bytes(range(pat))+bytes(128-pat)+b'M.K.'
mod=header+b''.join(enc(e) for row in grid for e in row)+b''.join(d for _,d,_ in samples)
(OUT/'undertow-circuit.mod').write_bytes(mod)
sys.path.insert(0,str(ROOT/'experiments/audio-level1'))
from preview_library import library
lib=ctypes.CDLL(str(library()))
lib.micromod_initialise.argtypes=[ctypes.c_void_p,ctypes.c_long]
lib.micromod_get_audio.argtypes=[ctypes.c_void_p,ctypes.c_long]
lib.micromod_calculate_song_duration.restype=ctypes.c_long
buf=ctypes.create_string_buffer(mod);assert lib.micromod_initialise(buf,44100)==0
count=lib.micromod_calculate_song_duration();assert abs(count/44100-BARS*4*60/BPM)<.3, (count/44100,BARS*4*60/BPM)
lib.micromod_set_position(0);pcm=np.zeros((count,2),np.int16);lib.micromod_get_audio(pcm.ctypes.data,count)
assert abs(pcm.astype(np.int32)).max()<32767
# Preview uses the planned 48/64 music volume; no loudness normalization.
with wave.open(str(OUT/'undertow-circuit-preview.wav'),'wb') as w:
    w.setparams((2,2,44100,0,'NONE','not compressed'));w.writeframes((pcm.astype(np.int32)*48//64).astype('<i2').tobytes())
report=dict(title='Undertow Circuit v6 review',bpm=BPM,bars=BARS,music_channels=3,reserved_channel=3,seconds=count/44100,module_bytes=len(mod),sample_bytes=sum(len(d) for _,d,_ in samples),score_bytes=1084+pat*1024,sha256=hashlib.sha256(mod).hexdigest(),native_accepted=False,preview_engine='micromod; not native ptplayer evidence',samples=[dict(name=n,bytes=len(d),volume=v) for n,d,v in samples])
(OUT/'manifest.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))

split=1084+pat*1024
assert sum(int.from_bytes(mod[42+i*30:44+i*30],'big')*2 for i in range(31))==len(mod)-split
assert all(mod[a:a+4]==bytes(4) for a in range(1096,split,16))
(OUT/'rain-score.bin').write_bytes(mod[:split])
(OUT/'rain-bank.bin').write_bytes(mod[split:])
# Short review cut includes arrival into first full theme, not a separate mix.
start=int(8*4*60/BPM*44100);end=int(24*4*60/BPM*44100)
with wave.open(str(OUT/'undertow-theme-preview.wav'),'wb') as w:
    w.setparams((2,2,44100,0,'NONE','not compressed'));w.writeframes((pcm[start:end].astype(np.int32)*48//64).astype('<i2').tobytes())
