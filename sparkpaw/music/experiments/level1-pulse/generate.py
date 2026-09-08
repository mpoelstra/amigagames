"""Original 3-voice Level-1 candidate: Copper Sprint, 164 BPM. No reference assets."""
from pathlib import Path
import ctypes, hashlib, json, struct, wave, sys
import numpy as np
OUT=Path(__file__).resolve().parent
ROOT=OUT.parents[2]
RATE=3546895/428
BPM=164
BARS=32
rng=np.random.default_rng(70704)
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
x=t(.18);r=add('SYNC SAW C',sum(np.sin(2*np.pi*130.813*j*x)*(.75**j/j) for j in range(1,13))*np.exp(-x*19),34)
x=t(.26);c=add('MINOR POWER STAB',sum(np.sin(2*np.pi*f*x)+.2*np.sin(4*np.pi*f*x) for f in [130.813,155.563,195.998])*np.exp(-x*14),32)
x=t(.22);p=add('TURNAROUND TOM',np.sin(2*np.pi*(100*x+55*(1-np.exp(-x*25))/25))*np.exp(-x*22),35)
periods=[856,808,762,720,678,640,604,570,538,508,480,453,428,404,381,360,339,320,302,285,269,254,240,226,214,202,190,180,170,160,151,143,135,127,120,113]
grid=[[[0,0,0,0] for ch in range(4)] for row in range(BARS*16)]
def note(bar,step,ch,inst,m,vol):grid[bar*16+step][ch]=[inst,periods[m-36],12,vol]
roots=[52,52,55,50,52,48,50,51]*4
for bar,root in enumerate(roots):
    section=bar//8
    # Short break in third phrase, then full drive; no tempo slowdown.
    drumsteps=range(0,16,2) if not (section==2 and bar%8<2) else [0,4,8,12]
    for st in drumsteps:
        inst=s if st in (4,12) else k if st in (0,8) else o if st==10 else h
        note(bar,st,0,inst,48,samples[inst-1][2])
    if bar%4==3:
        for st,m in [(13,53),(14,50),(15,48)]:note(bar,st,0,p,m,31+st%2*3)
    for j,st in enumerate([0,2,3,6,8,10,11,14]):
        pitch=root+(12 if st in (6,14) else 0)
        note(bar,st,1,b,pitch,39 if j%4==0 else 33)
    if section==2:
        for st in [0,6,10]:note(bar,st,2,c,root,28)
    else:
        riff=([0,7,12,7,3,7,10,7] if section!=3 else [12,7,3,7,10,7,3,0])
        for j,st in enumerate([0,2,4,6,8,10,12,14]):
            note(bar,st,2,r,min(71,root+riff[j]),31 if j%2==0 else 25)
    if bar%8==7:note(bar,15,2,r,root+1,32)
grid[0][0][2:]=[15,6];grid[0][1][2:]=[15,BPM]
# Fourth track is entirely empty, even global commands use music tracks only.
assert all(row[3]==[0,0,0,0] for row in grid)
def enc(e):
    i,p,f,a=e;return bytes([(i&240)|(p>>8),p&255,((i&15)<<4)|f,a])
pat=BARS//4
header=b'COPPER SPRINT'.ljust(20,b'\0')
for n,data,v in samples:header+=n.encode()[:22].ljust(22,b'\0')+struct.pack('>HBBHH',len(data)//2,0,v,0,1)
header+=bytes(30*(31-len(samples)))+bytes([pat,0])+bytes(range(pat))+bytes(128-pat)+b'M.K.'
mod=header+b''.join(enc(e) for row in grid for e in row)+b''.join(d for _,d,_ in samples)
(OUT/'copper-sprint.mod').write_bytes(mod)
sys.path.insert(0,str(ROOT/'experiments/audio-level1'))
from preview_library import library
lib=ctypes.CDLL(str(library()))
lib.micromod_initialise.argtypes=[ctypes.c_void_p,ctypes.c_long]
lib.micromod_get_audio.argtypes=[ctypes.c_void_p,ctypes.c_long]
lib.micromod_calculate_song_duration.restype=ctypes.c_long
buf=ctypes.create_string_buffer(mod);assert lib.micromod_initialise(buf,44100)==0
count=lib.micromod_calculate_song_duration();assert abs(count/44100-BARS*4*60/BPM)<.1
lib.micromod_set_position(0);pcm=np.zeros((count,2),np.int16);lib.micromod_get_audio(pcm.ctypes.data,count)
assert abs(pcm.astype(np.int32)).max()<32767
# Preview uses the planned 48/64 music volume; no loudness normalization.
with wave.open(str(OUT/'copper-sprint-preview.wav'),'wb') as w:
    w.setparams((2,2,44100,0,'NONE','not compressed'));w.writeframes((pcm.astype(np.int32)*48//64).astype('<i2').tobytes())
report=dict(title='Copper Sprint',bpm=BPM,bars=BARS,music_channels=3,reserved_channel=3,seconds=count/44100,module_bytes=len(mod),sample_bytes=sum(len(d) for _,d,_ in samples),score_bytes=1084+pat*1024,sha256=hashlib.sha256(mod).hexdigest(),native_accepted=False,preview_engine='micromod; not native ptplayer evidence',samples=[dict(name=n,bytes=len(d),volume=v) for n,d,v in samples])
(OUT/'manifest.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
