"""Smoother second Sparkpaw title composition: four-channel ProTracker MOD + real MOD preview.
No reference audio, melody or sample data is consumed by this generator.
"""
from pathlib import Path
import ctypes, hashlib, json, struct, wave
import numpy as np

OUT=Path(__file__).resolve().parent
RATE=3546895/428
BPM,SPEED,BARS=150,6,40
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
SNARE=add('STEEL SNARE + HAT',echo(snare,.08),55)
t=time(.08);HAT=add('TIGHT AIR',noise(t)*np.exp(-t*68),24)
t=time(.32);OPEN=add('OPEN AIR',noise(t)*np.exp(-t*14),29)
t=time(.34);TOM=add('ENGINE TOM',np.sin(TAU*(95*t+95*(1-np.exp(-t*24))/24))*np.exp(-t*15),52)
t=time(.78);CRASH=add('CRASH AND KICK',noise(t,750,3400)*np.exp(-t*7)+.9*np.sin(TAU*(48*t+120*(1-np.exp(-t*45))/45))*np.exp(-t*18),56)
# A sharp but warm low E groove, with actual bass root recorded at C2.
t=time(2.5);p=TAU*freq(36)*t
bass=np.sin(p)+.44*np.sin(2*p)*np.exp(-t*5)+.2*np.sin(3*p)*np.exp(-t*11)+.08*np.sin(5*p)*np.exp(-t*20)
BASS=add('ANALOGUE OCTAVE BASS',np.tanh(1.9*bass)*(1-np.exp(-t*1400))*np.exp(-t*1.1),45)
# Lead: detuned harmonic brass with a brief metallic attack and decaying edge.
t=time(6.5);f=freq(48);lead=np.zeros_like(t)
for det in [-.0028,0,.0028]:
    phase=TAU*f*(1+det)*t+.012*np.sin(TAU*5.3*t)*np.minimum(t/.3,1)
    for h in range(1,15):
        # Harmonic-limited saw/pulse hybrid with an opening then settling filter.
        amp=(1/h)*(1 if h%2 else .48)
        cutoff=3.5+5*np.exp(-t*1.7)
        lead+=amp*np.sin(h*phase+.06*h)*np.exp(-h/(cutoff*.70))*np.exp(-t*.24)
lead*=np.minimum(t/.065,1)
LEAD=add('VIII-Hero-Drive LEAD',echo(lead,.20),47)
t=time(.70);p=TAU*freq(48)*t
BELL=add('CYAN CRYSTAL',echo((np.sin(p+1.3*np.sin(2*p)*np.exp(-t*8))+.18*np.sin(3*p))*np.exp(-t*5),.2),38)
t=time(.22);p=TAU*freq(48)*t
ARP=add('CLOCKWORK PLUCK',(np.sin(p)+.30*np.sin(2*p)+.32*np.sin(3*p)+.12*np.sin(5*p))*np.exp(-t*19),32)
# Exact chord voicings are pre-sampled as one MOD instrument each.
voicings={'Em':[55,59,64,66],'C':[55,60,64,67],'D':[57,62,66,69],
 'B7':[57,59,63,66],'Am':[57,60,64,67],'G':[55,59,62,67]}
CHORD={}
for name,notes in voicings.items():
    t=time(6.5);x=np.zeros_like(t)
    for k,m in enumerate(notes):
        for det in [-.0015,.0015]:
            p=TAU*freq(m)*(1+det)*t+k*.63
            x+=np.sin(p)+.34*np.sin(2*p)*np.exp(-t*1.2)+.18*np.sin(3*p)*np.exp(-t*2.4)+.07*np.sin(5*p)*np.exp(-t*4)
    x*=np.minimum(t/.18,1)*np.exp(-t*.27)
    CHORD[name]=add(name+' STORM STRINGS',echo(x,.10),34)
periods=[856,808,762,720,678,640,604,570,538,508,480,453,428,404,381,360,339,320,302,285,269,254,240,226,214,202,190,180,170,160,151,143,135,127,120,113]
grid=[[[0,0,0,0] for _ in range(4)] for _ in range(ROWS)]
def put(row,ch,inst=0,m=None,fx=0,arg=0):
    assert 0<=row<ROWS and 0<=ch<4 and (m is None or 36<=m<=71)
    assert grid[row][ch]==[0,0,0,0],(row,ch,grid[row][ch])
    grid[row][ch]=[inst,0 if m is None else periods[m-36],fx,arg]
def note(bar,step,ch,ins,m,vol): put(bar*16+step,ch,ins,m,12,vol)
COUNTS=[7, 7, 11, 7, 8]
TEMPOS=[150, 150, 164, 164, 164]
KIND=2
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
  if KIND==2 and scene in (2,3,4):steps=list(range(0,16,2))
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
# First 25 bars (~38.5s) are retained exactly. Rewrite only the finale.
MODE=1
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
expected_seconds=sum(n*240/t for n,t in zip(COUNTS,TEMPOS))
# Both timing commands on the first row, preserving its actual musical notes.
grid[0][0][2:]=[15,SPEED]
grid[0][2][2:]=[15,BPM]
def encode(e):
    i,p,f,a=e;return bytes([(i&240)|(p>>8),p&255,((i&15)<<4)|f,a])
patterns=ROWS//64
header=b'VIII-HERO-DRIVE'.ljust(20,b'\0')
for name,data,vol in samples:header+=name.encode()[:22].ljust(22,b'\0')+struct.pack('>HBBHH',len(data)//2,0,vol,0,1)
header+=bytes(30*(31-len(samples)))+bytes([patterns,0])+bytes(range(patterns))+bytes(128-patterns)+b'M.K.'
module=header+b''.join(encode(e) for row in grid for e in row)+b''.join(s[1] for s in samples)
assert len(samples)<=31 and len(module)==1084+patterns*1024+sum(len(s[1]) for s in samples)
assert module[1080:1084]==b'M.K.'
for at in range(1084,1084+patterns*1024,4):
    a,b,c,d=module[at:at+4];assert (((a&15)<<8)|b) in periods+[0]
    assert ((a&240)|(c>>4))<=len(samples) and (c&15) in [0,4,12,15]
    if c&15==12:assert d<=64
(OUT/'VIII-Hero-Drive.mod').write_bytes(module)
libpath=OUT.parents[3]/'build/music-reference-study/libmicromod.dylib'
lib=ctypes.CDLL(str(libpath));lib.micromod_initialise.argtypes=[ctypes.c_void_p,ctypes.c_long]
lib.micromod_calculate_song_duration.restype=ctypes.c_long
lib.micromod_get_audio.argtypes=[ctypes.c_void_p,ctypes.c_long]
buf=ctypes.create_string_buffer(module);assert lib.micromod_initialise(buf,44100)==0
count=lib.micromod_calculate_song_duration();seconds=count/44100
assert abs(seconds-expected_seconds)<.15,seconds
lib.micromod_set_position(0);pcm=np.zeros((count,2),dtype=np.int16)
lib.micromod_get_audio(pcm.ctypes.data,count)
assert np.max(abs(pcm.astype(np.int32)))<32767
x=pcm.astype(float);mid=x.mean(axis=1,keepdims=True);x=mid+.65*(x-mid)
gain=.88*32767/abs(x).max();x*=gain
# One complete loop, short preview-only ending fade. No external backing track.
x[-11025:]*=np.linspace(1,0,11025)[:,None]
with wave.open(str(OUT/'VIII-Hero-Drive-preview.wav'),'wb') as w:
    w.setparams((2,2,44100,0,'NONE','not compressed'));w.writeframes(np.rint(x).astype('<i2').tobytes())
report={'title':'Sparkpaw — VIII-Hero-Drive','status':'original intro finale candidate; user listening pending','bpm':BPM,'scene_tempos':TEMPOS,'scene_start_seconds':scene_times,'speed':SPEED,'channels':4,'patterns':patterns,'bars':BARS,'render_seconds':seconds,'module_bytes':len(module),'sample_bytes':sum(len(d) for _,d,_ in samples),'module_sha256':hashlib.sha256(module).hexdigest(),'peak_before_master':int(abs(pcm.astype(np.int32)).max()),'preview_gain':gain,'reference_samples_used':False,'runtime_integrated':False,'revision':'VIII-Hero-Drive: accepted opening preserved; continuous 164 BPM hopeful finale','render_engine':'micromod, actual MOD playback','samples':[{'name':n,'bytes':len(d),'volume':v} for n,d,v in samples]}
(OUT/'manifest.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
