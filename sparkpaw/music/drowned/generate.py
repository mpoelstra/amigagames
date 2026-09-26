"""Original Drowned Turbines score: Undertow Circuit, 144 BPM, 64 bars, three voices."""
from pathlib import Path
import ctypes, hashlib, json, struct, wave, sys
import numpy as np
OUT=Path(__file__).resolve().parent
ROOT=OUT.parents[1]
RATE=3546895/428
BPM=144
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
x=t(.38);r=add('GLASS REED C', (np.sin(2*np.pi*130.813*x+1.0*np.sin(2*np.pi*261.626*x)*np.exp(-x*12))+.18*np.sin(2*np.pi*392.439*x))*np.exp(-x*8),30)
x=t(.42);c=add('HOLLOW FIFTH C',sum(np.sin(2*np.pi*f*x) for f in [130.813,195.998,261.626])*np.exp(-x*9),26)
x=t(.20);p=add('PRESSURE TOM',np.sin(2*np.pi*(96*x+48*(1-np.exp(-x*28))/28))*np.exp(-x*23),28)
x=t(.46);bell=add('RAIN LENS C',(np.sin(2*np.pi*130.813*x)+.38*np.sin(2*np.pi*261.626*x)*np.exp(-x*7)+.12*np.sin(2*np.pi*654.065*x)*np.exp(-x*17))*np.exp(-x*7),27)
periods=[856,808,762,720,678,640,604,570,538,508,480,453,428,404,381,360,339,320,302,285,269,254,240,226,214,202,190,180,170,160,151,143,135,127,120,113]
grid=[[[0,0,0,0] for ch in range(4)] for row in range(BARS*16)]
def note(bar,step,ch,inst,m,vol):grid[bar*16+step][ch]=[inst,periods[m-36],12,vol]
# D minor / Bb / F / C. Descending A-G-F-E motif, then a rising answer.
# 8 bars arrival,16 theme,8 drive,8 submerged bridge,16 expanded theme,8 return.
roots=[38,46,41,48]*16
hook=[ [69,67,65,64,62,65,64,62],
       [65,62,60,62,65,69,67,65],
       [65,64,60,64,65,67,69,67],
       [67,64,62,60,64,67,64,61] ]
for bar,root in enumerate(roots):
    bridge=32<=bar<40
    arrival=bar<8
    steps=[0,4,8,12,14] if bridge else range(0,16,2)
    for st in steps:
        inst=s if st in (4,12) else k if st in (0,8) else o if st==14 else h
        note(bar,st,0,inst,48,samples[inst-1][2]-(4 if bridge else 0))
    if bar%8==7 and not bridge:
        for st,m in [(13,53),(14,50),(15,48)]:note(bar,st,0,p,m,26)
    basssteps=[0,6,8,14] if bridge else [0,3,6,8,11,14]
    for st in basssteps:
        note(bar,st,1,b,root+(12 if st in (6,14) else 0),35 if st in (0,8) else 29)
    if arrival:
        for st in [0,8]:note(bar,st,2,c,root+12,24)
        if bar>=4:
            for st,m in [(10,hook[bar%4][0]),(14,hook[bar%4][1])]:note(bar,st,2,bell,m,24)
    elif bridge:
        for st,j in [(0,0),(6,2),(12,4)]:note(bar,st,2,bell,hook[bar%4][j],25)
    elif 24<=bar<32 or bar>=56:
        third=3 if bar%4==0 else 4
        for st,offset in zip([0,3,6,8,10,14],[0,7,12,third,7,12]):
            pitch=root+12+offset
            note(bar,st,2,r,pitch if pitch<=71 else pitch-12,25)
    else:
        for j,st in enumerate([0,3,5,6,8,10,12,14]):
            pitch=hook[bar%4][j]
            if 48<=bar<56 and j>=4: pitch=hook[bar%4][7-j+4]
            note(bar,st,2,bell if 40<=bar<48 else r,pitch,30 if j in (0,4) else 25)
grid[0][0][2:]=[15,6];grid[0][1][2:]=[15,BPM]
# Fourth track is entirely empty, even global commands use music tracks only.
assert all(row[3]==[0,0,0,0] for row in grid)
def enc(e):
    i,p,f,a=e;return bytes([(i&240)|(p>>8),p&255,((i&15)<<4)|f,a])
pat=BARS//4
header=b'UNDERTOW CIRCUIT'.ljust(20,b'\0')
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
report=dict(title='Undertow Circuit',bpm=BPM,bars=BARS,music_channels=3,reserved_channel=3,seconds=count/44100,module_bytes=len(mod),sample_bytes=sum(len(d) for _,d,_ in samples),score_bytes=1084+pat*1024,sha256=hashlib.sha256(mod).hexdigest(),native_accepted=False,preview_engine='micromod; not native ptplayer evidence',samples=[dict(name=n,bytes=len(d),volume=v) for n,d,v in samples])
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
