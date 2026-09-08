"""Original Stormrail score: Iron Horizon, 172 BPM, 64 bars, three voices."""
from pathlib import Path
import ctypes, hashlib, json, struct, wave, sys
import numpy as np
OUT=Path(__file__).resolve().parent
ROOT=OUT.parents[1]
RATE=3546895/428
BPM=172
BARS=64
rng=np.random.default_rng(70909)
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
x=t(.19);phase=2*np.pi*(48*x+100*(1-np.exp(-x*45))/45)
k=add('DRIVE KICK HAT',np.sin(phase)*np.exp(-x*22)+.07*noise(x,85),42)
x=t(.16);s=add('METAL SNARE HAT',.5*noise(x,35)+.7*np.sin(2*np.pi*185*x)*np.exp(-x*30),39)
x=t(.045);h=add('CLOSED HAT',noise(x,90),19)
x=t(.13);o=add('OPEN HAT',noise(x,26),23)
x=t(.21);b=add('PULSE BASS C',sum(np.sin(2*np.pi*65.406*j*x)*a for j,a in [(1,1),(2,.45),(3,.23),(5,.1)])*np.exp(-x*16),39)
x=t(.18);r=add('HORIZON LEAD C',sum(np.sin(2*np.pi*130.813*j*x)*(.9**j/j) for j in range(1,13))*np.exp(-x*10),34)
x=t(.26);c=add('WIDE POWER FIFTH',sum(np.sin(2*np.pi*f*x)+.2*np.sin(4*np.pi*f*x) for f in [130.813,195.998,261.626])*np.exp(-x*14),32)
x=t(.22);p=add('TURNAROUND TOM',np.sin(2*np.pi*(100*x+55*(1-np.exp(-x*25))/25))*np.exp(-x*22),35)
periods=[856,808,762,720,678,640,604,570,538,508,480,453,428,404,381,360,339,320,302,285,269,254,240,226,214,202,190,180,170,160,151,143,135,127,120,113]
grid=[[[0,0,0,0] for ch in range(4)] for row in range(BARS*16)]
def note(bar,step,ch,inst,m,vol):grid[bar*16+step][ch]=[inst,periods[m-36],12,vol]
# Am - F - C - G; a four-bar hook returns across a 64-bar arc.
# Power fifths are baked into ONE sample/voice, never extra runtime channels.
roots=[45,41,48,43]*16
hook=[
 [57,64,69,67,64,60,62,64],
 [65,64,60,57,60,64,65,64],
 [60,64,67,69,67,64,62,60],
 [62,67,69,67,62,59,62,64]]
for bar,root in enumerate(roots):
    atmospheric=32<=bar<40
    arrival=bar<8
    drumsteps=range(0,16,2) if not atmospheric else [0,4,8,10,12,14]
    for st in drumsteps:
        inst=s if st in (4,12) else k if st in (0,8) else o if st==14 else h
        note(bar,st,0,inst,48,samples[inst-1][2])
    if bar%8==7:
        for st,m in [(12,53),(13,51),(14,50),(15,48)]:note(bar,st,0,p,m,35)
    elif bar%4==3 and not atmospheric:
        note(bar,15,0,s,48,32)
    for st in ([0,6,8,14] if atmospheric else [0,2,3,6,8,10,11,14]):
        note(bar,st,1,b,root+(12 if st in (6,14) else 0),38 if st in (0,8) else 32)
    if atmospheric or arrival:
        for st in [0,6,12]:note(bar,st,2,c,root+12,27 if atmospheric else 30)
        if bar%4==3:note(bar,14,2,r,64,29)
    elif 24<=bar<32 or bar>=56:
        # Driving arpeggio links the two hook statements and loops to departure.
        third=3 if bar%4==0 else 4
        for j,st in enumerate(range(0,16,2)):
            pitch=root+12+[0,7,12,7,third,7,12,7][j]
            note(bar,st,2,r,pitch if pitch<=71 else pitch-12,29)
    else:
        for j,st in enumerate([0,3,4,6,8,10,12,14]):
            pitch=hook[bar%4][j]
            if 48<=bar<56 and j in (1,5):pitch=hook[bar%4][(j+1)%8]
            note(bar,st,2,r,pitch,33 if j in (0,2,4) else 28)
grid[0][0][2:]=[15,6];grid[0][1][2:]=[15,BPM]
# Fourth track is entirely empty, even global commands use music tracks only.
assert all(row[3]==[0,0,0,0] for row in grid)
def enc(e):
    i,p,f,a=e;return bytes([(i&240)|(p>>8),p&255,((i&15)<<4)|f,a])
pat=BARS//4
header=b'IRON HORIZON'.ljust(20,b'\0')
for n,data,v in samples:header+=n.encode()[:22].ljust(22,b'\0')+struct.pack('>HBBHH',len(data)//2,0,v,0,1)
header+=bytes(30*(31-len(samples)))+bytes([pat,0])+bytes(range(pat))+bytes(128-pat)+b'M.K.'
mod=header+b''.join(enc(e) for row in grid for e in row)+b''.join(d for _,d,_ in samples)
(OUT/'iron-horizon.mod').write_bytes(mod)
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
with wave.open(str(OUT/'iron-horizon-preview.wav'),'wb') as w:
    w.setparams((2,2,44100,0,'NONE','not compressed'));w.writeframes((pcm.astype(np.int32)*48//64).astype('<i2').tobytes())
report=dict(title='Iron Horizon',bpm=BPM,bars=BARS,music_channels=3,reserved_channel=3,seconds=count/44100,module_bytes=len(mod),sample_bytes=sum(len(d) for _,d,_ in samples),score_bytes=1084+pat*1024,sha256=hashlib.sha256(mod).hexdigest(),native_accepted=False,preview_engine='micromod; not native ptplayer evidence',samples=[dict(name=n,bytes=len(d),volume=v) for n,d,v in samples])
(OUT/'manifest.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
