"""Original Drowned Turbines score: Undertow Circuit v2, 144 BPM, 64 bars, three voices."""
from pathlib import Path
import ctypes, hashlib, json, struct, wave, sys
import numpy as np
OUT=Path(__file__).resolve().parent
ROOT=OUT.parents[1]
RATE=3546895/428
BPM=144
BARS=64
rng=np.random.default_rng(70920)
loops={}
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
# Loopable hero lead: coherent harmonic periods, attack then stable sustain.
# Eight whole fundamental cycles per506-sample loop. Octave/fifth also close.
u=np.arange(1012)/506
hero=np.sin(2*np.pi*8*u)+.34*np.sin(2*np.pi*16*u)+.17*np.sin(2*np.pi*24*u)+.10*np.sin(2*np.pi*12*u)
hero[:506]*=np.minimum(np.arange(506)/75,1)
data=np.rint(hero/max(abs(hero))*112).astype('int8').tobytes()
data=b'\0\0'+data[2:]
samples.append(('TURBINE HERO LEAD',data,30));hero_id=len(samples)
loops[hero_id]=(253,253) # word offset/length; no extra runtime voice
periods=[856,808,762,720,678,640,604,570,538,508,480,453,428,404,381,360,339,320,302,285,269,254,240,226,214,202,190,180,170,160,151,143,135,127,120,113]
grid=[[[0,0,0,0] for ch in range(4)] for row in range(BARS*16)]
def note(bar,step,ch,inst,m,vol):grid[bar*16+step][ch]=[inst,periods[m-36],12,vol]
# An eight-bar singable theme: recurring rising call, held peak, answering descent.
# 8 bars overture,16 main chorus,8 rhythmic variation,8 bridge,16 return,8 coda.
roots=[38,46,41,48]*16
phrases=[
 [(0,62),(2,65),(4,69),(10,67),(12,65)],
 [(0,65),(2,67),(4,69),(8,70),(12,69)],
 [(0,69),(2,67),(4,65),(10,64),(12,65)],
 [(0,67),(6,64),(8,62),(12,60),(14,61)],
 [(0,62),(2,65),(4,69),(10,67),(12,65)],
 [(0,65),(2,67),(4,70),(8,69),(12,65)],
 [(0,65),(2,67),(4,69),(8,67),(10,65),(12,64)],
 [(0,64),(4,67),(8,69),(12,67),(14,61)]]
def rest(bar,step,ch):grid[bar*16+step][ch]=[0,0,12,0]
for bar,root in enumerate(roots):
    bridge=32<=bar<40
    intro=bar<8
    # Firm half-bar kick/snare structure with quiet off-beat metal ticks.
    for st in ([0,4,8,12,14] if bridge else range(0,16,2)):
        inst=s if st in (4,12) else k if st in (0,8) else o if st==14 else h
        note(bar,st,0,inst,48,samples[inst-1][2]-(5 if bridge else 0))
    if bar%8==7 and not bridge:
        for st,m in [(12,53),(13,55),(14,50),(15,48)]:note(bar,st,0,p,m,31)
    elif bar%4==3 and not bridge:note(bar,15,0,s,48,25)
    for st in ([0,8,14] if bridge else [0,2,6,8,10,14]):
        note(bar,st,1,b,root+(12 if st in (6,14) else 0),38 if st in (0,8) else 31)
    if intro:
        if bar<4:
            for st in [0,8]:note(bar,st,2,c,root+12,29)
            for st,m in [(10,62),(12,65),(14,69)]:note(bar,st,2,bell,m,27)
        else:
            for st,m in phrases[bar%8]:note(bar,st,2,hero_id,m,25)
            rest(bar,15,2)
    elif bridge:
        for st,m in [(0,phrases[bar%8][0][1]),(6,phrases[bar%8][1][1]),(12,phrases[bar%8][-1][1])]:
            note(bar,st,2,bell,m,27)
    elif 24<=bar<32:
        for st,m in phrases[bar%8]:note(bar,st,2,r,m,31)
        for st in [7,15]:
            if grid[bar*16+st][2][0]==0:note(bar,st,2,bell,root+24 if root+24<=71 else root+12,20)
    else:
        phrase=phrases[bar%8]
        for st,m in phrase:
            # Held melodic peaks and deliberate breaths distinguish the chorus.
            note(bar,st,2,hero_id,m,32 if st==4 else 29)
        if bar%8 in (0,2,4,6):rest(bar,15,2)
        if bar>=56 and bar%4==3:
            note(bar,12,2,hero_id,64,28);note(bar,14,2,hero_id,61,26)
grid[0][0][2:]=[15,6];grid[0][1][2:]=[15,BPM]
# Fourth track is entirely empty, even global commands use music tracks only.
assert all(row[3]==[0,0,0,0] for row in grid)
def enc(e):
    i,p,f,a=e;return bytes([(i&240)|(p>>8),p&255,((i&15)<<4)|f,a])
pat=BARS//4
header=b'UNDERTOW CIRCUIT V2'.ljust(20,b'\0')
for i,(n,data,v) in enumerate(samples,1):
    start,length=loops.get(i,(0,1))
    assert start+length<=len(data)//2
    header+=n.encode()[:22].ljust(22,b'\0')+struct.pack('>HBBHH',len(data)//2,0,v,start,length)
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
report=dict(title='Undertow Circuit v2',bpm=BPM,bars=BARS,music_channels=3,reserved_channel=3,seconds=count/44100,module_bytes=len(mod),sample_bytes=sum(len(d) for _,d,_ in samples),score_bytes=1084+pat*1024,sha256=hashlib.sha256(mod).hexdigest(),native_accepted=False,preview_engine='micromod; not native ptplayer evidence',samples=[dict(name=n,bytes=len(d),volume=v) for n,d,v in samples])
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
