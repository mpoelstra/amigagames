"""Smoother second Sparkpaw title composition: four-channel ProTracker MOD + real MOD preview.
No reference audio, melody or sample data is consumed by this generator.
"""
from pathlib import Path
import ctypes, hashlib, json, struct, wave
import numpy as np

OUT=Path(__file__).resolve().parent
RATE=3546895/428
BPM,SPEED,BARS=150,6,48
ROWS=BARS*16
rng=np.random.default_rng(202609051455)
TAU=2*np.pi
samples=[]
def freq(m): return 440*2**((m-69)/12)
def time(s): return np.arange(int(s*RATE)//2*2)/RATE
def add(name,x,vol=48):
    x=np.asarray(x,dtype=float); x-=x.mean()
    edge=min(20,len(x)//4)
    x[:edge]*=np.linspace(0,1,edge); x[-edge:]*=np.linspace(1,0,edge)
    data=np.rint(x/max(np.max(abs(x)),1e-10)*118).astype('int8').tobytes()
    assert len(data)%2==0 and len(data)<=65534
    samples.append((name,data,vol)); return len(samples)
def noise(t,low=1000,high=3800):
    x=rng.normal(size=len(t)); f=np.fft.rfftfreq(len(t),1/RATE)
    return np.fft.irfft(np.fft.rfft(x)*(f/(f+low))*np.exp(-(f/high)**6),n=len(t))
def echo(x,g=.16):
    out=x.copy()
    for seconds,gain in [(.10417,g),(.20833,g*.45)]:
        d=int(seconds*RATE);out[d:]+=gain*x[:-d]
    return out
# Composite drum samples make kick/hat and snare/hat genuinely simultaneous
# within a single Paula voice, leaving all three musical voices independent.
t=time(.29); hat=noise(t)*np.exp(-t*72)
kick=np.sin(TAU*(48*t+120*(1-np.exp(-t*45))/45))*np.exp(-t*18)
KICK=add('THUNDER KICK + HAT',kick+.13*hat,62)
t=time(.30); snare=(.70*noise(t,650,3400)*np.exp(-t*24)+.42*np.sin(TAU*183*t)*np.exp(-t*22)+.20*np.sin(TAU*325*t)*np.exp(-t*36))
backkick=np.sin(TAU*(48*t+120*(1-np.exp(-t*45))/45))*np.exp(-t*18)
SNARE=add('KICK SNARE STACK',echo(snare,.08)+.70*backkick,59)
t=time(.08);HAT=add('TIGHT AIR',noise(t)*np.exp(-t*68),24)
t=time(.32);OPEN=add('OPEN AIR',noise(t)*np.exp(-t*14),29)
t=time(.34);TOM=add('ENGINE TOM',np.sin(TAU*(95*t+95*(1-np.exp(-t*24))/24))*np.exp(-t*15),52)
t=time(.78);CRASH=add('CRASH AND KICK',noise(t,750,3400)*np.exp(-t*7)+.9*np.sin(TAU*(48*t+120*(1-np.exp(-t*45))/45))*np.exp(-t*18),56)
# A sharp but warm low E groove, with actual bass root recorded at C2.
t=time(.70);p=TAU*freq(36)*t
bass=np.sin(p)+.44*np.sin(2*p)*np.exp(-t*5)+.2*np.sin(3*p)*np.exp(-t*11)+.08*np.sin(5*p)*np.exp(-t*20)
BASS=add('SHARD PULSE BASS',np.tanh(1.4*bass)*(1-np.exp(-t*1400))*np.exp(-t*3.5),53)
# Lead: detuned harmonic brass with a brief metallic attack and decaying edge.
t=time(6.5);f=freq(48);lead=np.zeros_like(t)
for det in [-.0014,.0014]:
    phase=TAU*f*(1+det)*t+.018*np.sin(TAU*5.6*t)*np.minimum(t/.22,1)
    for h,a in [(1,1),(2,.14),(3,.32),(4,.05),(5,.16),(7,.09),(9,.045)]:
        lead+=a*np.sin(h*phase+.12*h)*np.exp(-t*(.12+.11*h))
lead*=np.minimum(t/.015,1)
LEAD=add('C-Paula-Power LEAD',echo(lead,.20),47)
t=time(.70);p=TAU*freq(48)*t
BELL=add('CYAN CRYSTAL',echo((np.sin(p+1.3*np.sin(2*p)*np.exp(-t*8))+.18*np.sin(3*p))*np.exp(-t*5),.2),38)
t=time(.55);p=TAU*freq(48)*t
ARP=add('CLOCKWORK PLUCK',(np.sin(p)+.42*np.sin(2*p)+.2*np.sin(3*p))*np.exp(-t*7),32)
# Exact chord voicings are pre-sampled as one MOD instrument each.
voicings={'Em':[55,59,64,66],'C':[55,60,64,67],'D':[57,62,66,69],
 'B7':[57,59,63,66],'Am':[57,60,64,67],'G':[55,59,62,67]}
CHORD={}
for name,notes in voicings.items():
    t=time(2.0);x=np.zeros_like(t)
    for k,m in enumerate(notes):
        for det in [-.0015,.0015]:
            p=TAU*freq(m)*(1+det)*t+k*.63
            x+=np.sin(p)+.22*np.sin(2*p)*np.exp(-t*4)+.07*np.sin(3*p)*np.exp(-t*9)
    x*=np.minimum(t/.02,1)*np.exp(-t*1.6)
    CHORD[name]=add(name+' STORM STRINGS',echo(x,.10),34)
periods=[856,808,762,720,678,640,604,570,538,508,480,453,428,404,381,360,339,320,302,285,269,254,240,226,214,202,190,180,170,160,151,143,135,127,120,113]
grid=[[[0,0,0,0] for _ in range(4)] for _ in range(ROWS)]
def put(row,ch,inst=0,m=None,fx=0,arg=0):
    assert 0<=row<ROWS and 0<=ch<4 and (m is None or 36<=m<=71)
    assert grid[row][ch]==[0,0,0,0],(row,ch,grid[row][ch])
    grid[row][ch]=[inst,0 if m is None else periods[m-36],fx,arg]
def note(bar,step,ch,ins,m,vol): put(bar*16+step,ch,ins,m,12,vol)
# 4-bar ignition, 8-bar theme, 8-bar lift, 8-bar dark machinery, 16-bar
# expanded return, 4-bar turnaround into the opening without a disk load.
progression=['Em','C','D','B7']+['Em','C','G','D','Em','C','Am','B7']+['C','D','Em','G','Am','C','D','B7']+['Am','Em','C','B7','Am','C','D','B7']+['Em','C','G','D','Em','C','Am','B7']+['C','D','Em','G','Am','C','B7','Em']+['C','D','B7','B7']
roots={'Em':40,'C':36,'D':38,'B7':35,'Am':33,'G':31}
for bar,chord in enumerate(progression):
    intro=bar<4; bridge=20<=bar<28; ending=bar>=44
    root=roots[chord]
    for j,step in enumerate([0,8] if intro else [0,2,4,6,8,10,12,14]):
        pitch=root+(12 if step in (6,14) else 0)
        if step==14 and bar in [3,11,19,27,35,43,47]: pitch=roots[progression[(bar+1)%48]]-1
        note(bar,step,0,BASS,pitch+12,45 if j%2==0 else 55)
    if bridge or (bar>=4 and bar%4 in (1,3)):
        tones={'Am':[57,60,64,69],'Em':[55,59,64,67],'C':[55,60,64,67],'D':[57,62,66,69],'B7':[57,59,63,66],'G':[55,59,62,67]}[chord]
        for j,step in enumerate(range(0,16,2)):note(bar,step,2,ARP,tones[[0,2,1,3,2,1,3,2][j]],27 if j%2 else 34)
    else:
        for step,vol in [(0,22),(2,33),(8,22),(10,31)]:note(bar,step,2,CHORD[chord],48,vol if not intro else vol-5)
    drums={0:(KICK,48,61),2:(HAT,48,25),4:(SNARE,48,59),6:(HAT,48,22),8:(KICK,48,58),10:(OPEN,48,24),12:(SNARE,48,59),14:(HAT,48,27)}
    if bar in [0,4,12,28,36,44]:drums[0]=(CRASH,48,55)
    if bar in [3,19,27,43,47]:
        drums.update({12:(SNARE,48,52),13:(TOM,53,48),14:(TOM,49,46),15:(TOM,45,50)})
    if intro:
        drums={k:v for k,v in drums.items() if k in ([0,6,12] if bar<2 else [0,4,8,12,13,14,15])}
    if bridge and bar<24:
        drums={k:(ins,m,max(18,v-8)) for k,(ins,m,v) in drums.items()}
    for step,(ins,m,vol) in drums.items():note(bar,step,1,ins,m,vol)
# Distinct 8-bar thematic sentence: rising E-G-B signature, descending answer,
# syncopated second half, and a leading-tone cadence. These are original notes.
A=[[(0,64,4),(4,67,4),(8,71,8)],
 [(0,69,4),(4,67,4),(8,64,8)],
 [(0,62,4),(4,67,4),(8,71,4),(12,69,4)],
 [(0,66,8),(8,64,4),(12,62,4)],
 [(0,64,4),(4,67,4),(8,71,8)],
 [(0,69,4),(4,67,4),(8,64,4),(12,60,4)],
 [(0,64,4),(4,69,8),(12,67,4)],
 [(0,66,4),(4,63,4),(8,59,4),(12,63,4)]]
B=[[(0,67,8),(8,64,4),(12,67,4)],
 [(0,69,8),(8,66,4),(12,62,4)],
 [(0,71,8),(8,67,4),(12,64,4)],
 [(0,67,8),(8,62,8)],
 [(0,69,8),(8,67,4),(12,64,4)],
 [(0,67,4),(4,69,4),(8,71,8)],
 [(0,69,4),(4,66,4),(8,64,4),(12,62,4)],
 [(0,63,4),(4,66,4),(8,69,4),(12,71,4)]]
intro=[[(0,64,5),(8,71,4)],[(2,67,4),(10,64,4)],[(0,66,5),(8,69,4)],[(2,63,3),(8,66,3),(12,71,3)]]
bridge=[[(0,69,8),(8,64,8)],[(0,67,8),(8,64,8)],
 [(0,67,8),(8,64,8)],[(0,66,8),(8,63,8)],
 [(0,64,4),(4,69,8),(12,67,4)],[(0,67,8),(8,64,8)],
 [(0,66,8),(8,69,8)],[(0,63,4),(4,66,4),(8,71,8)]]
last=[[(0,67,8),(8,64,8)],[(0,69,8),(8,66,8)],
 [(0,71,8),(8,66,8)],[(0,63,8),(8,66,8)]]
phrases=intro+A+B+bridge+A+B+last
for bar,phrase in enumerate(phrases):
    ins=BELL if bar<4 or 20<=bar<24 else LEAD
    for step,m,duration in phrase:
        note(bar,step,3,ins,m,38 if ins==BELL else (49 if bar>=28 else 46))
        # Let the sample ring until the next melodic note: no C00 gate.
        # Gentle vibrato starts after the attack, never an extra note trigger.
        if duration>=5:put(bar*16+step+2,3,fx=4,arg=0x11)
for bar in [5,9,17,29,33,41]:
    # Em/C-family minor/major color follows the written backing root.
    for step in [1,5,9,13]:
        put(bar*16+step,2,fx=0,arg=0x37 if progression[bar]=='Em' else 0x47)
# Both timing commands on the first row, preserving its actual musical notes.
grid[0][0][2:]=[15,SPEED]
grid[0][2][2:]=[15,BPM]
def encode(e):
    i,p,f,a=e;return bytes([(i&240)|(p>>8),p&255,((i&15)<<4)|f,a])
patterns=ROWS//64
header=b'C-PAULA-POWER'.ljust(20,b'\0')
for name,data,vol in samples:header+=name.encode()[:22].ljust(22,b'\0')+struct.pack('>HBBHH',len(data)//2,0,vol,0,1)
header+=bytes(30*(31-len(samples)))+bytes([patterns,0])+bytes(range(patterns))+bytes(128-patterns)+b'M.K.'
module=header+b''.join(encode(e) for row in grid for e in row)+b''.join(s[1] for s in samples)
assert len(samples)<=31 and len(module)==1084+patterns*1024+sum(len(s[1]) for s in samples)
assert module[1080:1084]==b'M.K.'
for at in range(1084,1084+patterns*1024,4):
    a,b,c,d=module[at:at+4];assert (((a&15)<<8)|b) in periods+[0]
    assert ((a&240)|(c>>4))<=len(samples) and (c&15) in [0,4,12,15]
    if c&15==12:assert d<=64
(OUT/'C-Paula-Power.mod').write_bytes(module)
libpath=OUT.parents[3]/'build/music-reference-study/libmicromod.dylib'
lib=ctypes.CDLL(str(libpath));lib.micromod_initialise.argtypes=[ctypes.c_void_p,ctypes.c_long]
lib.micromod_calculate_song_duration.restype=ctypes.c_long
lib.micromod_get_audio.argtypes=[ctypes.c_void_p,ctypes.c_long]
buf=ctypes.create_string_buffer(module);assert lib.micromod_initialise(buf,44100)==0
count=lib.micromod_calculate_song_duration();seconds=count/44100
assert abs(seconds-BARS*4*60/BPM)<.15,seconds
lib.micromod_set_position(0);pcm=np.zeros((count,2),dtype=np.int16)
lib.micromod_get_audio(pcm.ctypes.data,count)
assert np.max(abs(pcm.astype(np.int32)))<32767
x=pcm.astype(float);mid=x.mean(axis=1,keepdims=True);x=mid+.65*(x-mid)
gain=.88*32767/abs(x).max();x*=gain
# One complete loop, short preview-only ending fade. No external backing track.
x[-11025:]*=np.linspace(1,0,11025)[:,None]
with wave.open(str(OUT/'C-Paula-Power-preview.wav'),'wb') as w:
    w.setparams((2,2,44100,0,'NONE','not compressed'));w.writeframes(np.rint(x).astype('<i2').tobytes())
report={'title':'Sparkpaw — C-Paula-Power','status':'original title candidate; user listening pending','bpm':BPM,'speed':SPEED,'channels':4,'patterns':patterns,'bars':BARS,'render_seconds':seconds,'module_bytes':len(module),'sample_bytes':sum(len(d) for _,d,_ in samples),'module_sha256':hashlib.sha256(module).hexdigest(),'peak_before_master':int(abs(pcm.astype(np.int32)).max()),'preview_gain':gain,'reference_samples_used':False,'runtime_integrated':False,'revision':'C-Paula-Power: extended original melody and four-channel arrangement','render_engine':'micromod, actual MOD playback','samples':[{'name':n,'bytes':len(d),'volume':v} for n,d,v in samples]}
(OUT/'manifest.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
