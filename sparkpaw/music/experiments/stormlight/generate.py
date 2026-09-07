"""Original four-channel listening sketch. Requires numpy; no reference audio input.

All timbres are newly synthesized. This is a music experiment, not runtime data.
Run with the bundled Python. Optional rendering uses the upstream micromod C
library built at sparkpaw/build/music-reference-study/libmicromod.dylib.
"""
from pathlib import Path
import ctypes, hashlib, json, struct, wave
import numpy as np

OUT=Path(__file__).resolve().parent
RATE=3546895/428
BPM=125
SPEED=8
ROWS=6*64
TAU=2*np.pi
samples=[]
rng=np.random.default_rng(5092026)

def freq(midi): return 440*2**((midi-69)/12)

def tvec(seconds): return np.arange(int(seconds*RATE)//2*2)/RATE

def add(name,x,volume=48):
    x=np.asarray(x,dtype=float)
    x-=x.mean()
    # Short ramps keep sample boundaries quiet; every instrument is a one-shot.
    edge=min(24,len(x)//4)
    x[:edge]*=np.linspace(0,1,edge)
    x[-edge:]*=np.linspace(1,0,edge)
    x=x/max(abs(x).max(),1e-9)*116
    data=np.round(x).astype('int8').tobytes()
    if len(data)%2: data+=b'\0'
    assert len(data)<=65534
    samples.append((name,data,volume))
    return len(samples)

def echoes(x,amount=.15):
    y=x.copy()
    for sec,gain in [(.061,amount),(.109,amount*.55),(.173,amount*.3)]:
        d=int(sec*RATE)
        y[d:]+=x[:-d]*gain
    return y

def colored_noise(t,cut=2400):
    x=rng.normal(size=len(t))
    bins=np.fft.rfftfreq(len(t),1/RATE)
    filt=(bins/(bins+550))*np.exp(-(bins/cut)**4)
    return np.fft.irfft(np.fft.rfft(x)*filt,n=len(t))

t=tvec(.39)
phase=TAU*(48*t+104*(1-np.exp(-t*37))/37)
kick=np.sin(phase)*np.exp(-t*13)+.11*colored_noise(t)*np.exp(-t*180)
KICK=add('LOW ROUND KICK',kick,61)
t=tvec(.42)
snare=(.9*colored_noise(t,3100)*np.exp(-t*18)+.55*np.sin(TAU*174*t)*np.exp(-t*23)+.3*np.sin(TAU*319*t)*np.exp(-t*32))
SNARE=add('PAPER SNARE',echoes(snare,.13),49)
t=tvec(.095)
hat=colored_noise(t,3900)*np.exp(-t*55)
HAT=add('SOFT CLOSED HAT',hat,20)
t=tvec(.42)
OHAT=add('OPEN AIR HAT',colored_noise(t,3800)*np.exp(-t*13),24)
t=tvec(.5)
TOM=add('ROUND TOM',np.sin(TAU*(90*t+65*(1-np.exp(-t*22))/22))*np.exp(-t*12),48)
t=tvec(.9)
CRASH=add('DARK CRASH',colored_noise(t,3400)*np.exp(-t*5),25)

# Sampled bass: rounded fundamental and a short plucked upper spectrum.
t=tvec(1.15); f=freq(36)
bass=np.sin(TAU*f*t)*np.exp(-t*2.2)
for h,a in [(2,.38),(3,.18),(4,.09),(6,.035)]:
    bass+=a*np.sin(TAU*f*h*t+.12)*np.exp(-t*(4+h*2))
BASS=add('FINGER SYNTH BASS',bass*(1-np.exp(-t*1000)),55)

# A breathy, harmonically evolving lead; its attack differs from its sustain.
t=tvec(2.4); f=freq(48)
vib=.003*np.sin(TAU*5.2*t)*np.minimum(t/.25,1)
phase=TAU*f*(t+vib/(TAU*5.2))
lead=np.zeros_like(t)
for h,a in [(1,1),(2,.42),(3,.21),(4,.10),(5,.04)]:
    lead+=a*np.sin(h*phase+.07*h)*np.exp(-t*(.7+h*.12))
lead+=.16*np.sin(phase+2.1*np.sin(2*phase)*np.exp(-t*9))*np.exp(-t*7)
lead*=np.minimum(t/.012,1)*np.minimum((t[-1]-t)/.12,1)
LEAD=add('AMBER REED',echoes(lead,.18),46)
t=tvec(1.7); p=TAU*freq(48)*t
bell=(np.sin(p+1.4*np.sin(2*p)*np.exp(-t*4))+.2*np.sin(3.01*p)*np.exp(-t*6))*np.exp(-t*2.8)
BELL=add('GLASS RESPONSE',echoes(bell,.22),36)

# Each four-note chord is one sampled voice, with close inner voice movement.
voicings={
 'Dm9':[53,57,60,64], 'Bbmaj7':[53,57,58,62],
 'Fmaj7':[52,57,60,65], 'Cadd9':[52,55,60,62],
 'Gm9':[53,57,58,62], 'A7':[55,57,61,64],
 'Dm':[53,57,62,65],
}
CHORD={}
for name,notes in voicings.items():
    t=tvec(2.9); x=np.zeros_like(t)
    for k,note in enumerate(notes):
        f=freq(note)
        for det,amp in [(-.0018,.45),(.0018,.45)]:
            p=TAU*f*(1+det)*t+k*.61
            x+=amp*(np.sin(p)+.24*np.sin(2*p)*np.exp(-t*2)+.1*np.sin(3*p)*np.exp(-t*5))
    x*=np.minimum(t/.025,1)*np.exp(-t*1.2)*np.minimum((t[-1]-t)/.2,1)
    CHORD[name]=add(name+' SILK',echoes(x,.12),31)

periods=[856,808,762,720,678,640,604,570,538,508,480,453,
         428,404,381,360,339,320,302,285,269,254,240,226,
         214,202,190,180,170,160,151,143,135,127,120,113]
grid=[[[0,0,0,0] for _ in range(4)] for _ in range(ROWS)]
events=[]
def put(row,ch,inst=0,midi=None,fx=0,arg=0):
    assert 0<=row<ROWS and 0<=ch<4
    period=0 if midi is None else periods[midi-36]
    assert midi is None or 36<=midi<=71
    assert grid[row][ch]==[0,0,0,0],(row,ch,grid[row][ch])
    grid[row][ch]=[inst,period,fx,arg]
    if midi is not None: events.append(dict(row=row,channel=ch,instrument=inst,midi=midi))

def note(bar,step,ch,inst,midi,volume=None):
    put(bar*16+step,ch,inst,midi,12 if volume is not None else 0,volume or 0)

harmony=['Dm9','Bbmaj7','Fmaj7','A7',
         'Dm9','Bbmaj7','Fmaj7','Cadd9','Gm9','Dm9','Bbmaj7','A7',
         'Bbmaj7','Fmaj7','Gm9','A7',
         'Dm9','Bbmaj7','Fmaj7','Cadd9','Gm9','Bbmaj7','A7','Dm']
roots=[38,34,41,33,38,34,41,36,43,38,34,33,34,41,43,33,38,34,41,36,43,34,33,38]
# Bass source is recorded one octave lower; +12 in the score preserves the
# intended low register while retaining legal classic ProTracker periods.

for bar,(chord,root) in enumerate(zip(harmony,roots)):
    quiet=bar<4 or 12<=bar<16
    note(bar,0,2,CHORD[chord],48,25 if quiet else 32)
    if not quiet and bar%2==1:
        note(bar,10,2,CHORD[chord],48,20)
    bass_steps=[0,10] if quiet else [0,3,6,8,11,14]
    for j,step in enumerate(bass_steps):
        pitch=root+(12 if step in (6,14) else 0)
        if step==14 and bar in (7,11,15,22): pitch=roots[bar+1]-1
        note(bar,step,0,BASS,pitch+12,48 if quiet else (54 if j%2==0 else 44))
    if bar==0: continue
    if quiet:
        for step in [2,6,10,14]: note(bar,step,1,HAT,48,13)
        if bar in (3,15):
            for step,m in [(12,48),(13,46),(15,43)]: note(bar,step,1,TOM,m,35)
    else:
        drum={0:(KICK,48,60),2:(HAT,48,18),4:(SNARE,48,49),6:(HAT,48,20),
              8:(KICK,48,55),10:(OHAT,48,18),12:(SNARE,48,51),14:(HAT,48,20)}
        if bar%2==1: drum[11]=(KICK,48,41)
        if bar in (11,23):
            drum.update({13:(TOM,50,43),14:(TOM,46,40),15:(TOM,43,43)})
        if bar in (7,19): drum[15]=(SNARE,48,27)
        for step,(inst,m,v) in drum.items(): note(bar,step,1,inst,m,v)
    if bar in (4,16): note(bar,8,2,CRASH,48,23)

# Through-composed phrases. Durations are in rows and include deliberate rests.
phrases={
 0:[(4,62,3),(10,65,2)],
 1:[(2,69,4),(10,65,3)],
 2:[(4,64,3),(10,60,3)],
 3:[(8,61,2),(11,64,2),(14,69,2)],
 4:[(0,69,3),(4,65,2),(7,64,1),(8,62,5),(14,64,2)],
 5:[(0,65,5),(6,62,2),(10,60,3),(14,62,2)],
 6:[(0,64,3),(4,65,2),(7,69,3),(12,67,3)],
 7:[(0,64,6),(8,62,2),(11,60,3)],
 8:[(0,62,3),(4,65,3),(8,69,5),(14,67,2)],
 9:[(0,65,3),(4,64,2),(7,62,4),(13,57,2)],
 10:[(0,58,3),(4,62,2),(8,65,3),(12,64,2)],
 11:[(0,61,6),(8,64,3),(12,69,3)],
 12:[(2,65,5),(10,62,4)],
 13:[(0,64,5),(8,60,5)],
 14:[(2,62,4),(8,57,3),(12,58,3)],
 15:[(0,61,5),(8,64,2),(11,67,2),(14,69,2)],
 16:[(0,69,5),(6,65,2),(9,64,1),(10,62,3),(14,64,2)],
 17:[(0,65,3),(4,69,3),(8,70,5),(14,69,2)],
 18:[(0,67,3),(4,65,2),(7,64,3),(12,60,3)],
 19:[(0,64,6),(8,67,3),(12,69,3)],
 20:[(0,70,5),(6,69,2),(10,67,3),(14,65,2)],
 21:[(0,62,3),(4,65,3),(8,69,3),(12,65,3)],
 22:[(0,64,3),(4,61,3),(8,57,5),(14,61,2)],
 23:[(0,62,10)],
}
for bar,phrase in phrases.items():
    inst=BELL if bar<4 or 12<=bar<16 else LEAD
    for step,midi,dur in phrase:
        note(bar,step,3,inst,midi,38 if inst==BELL else 47)
        end=bar*16+step+dur
        if end<ROWS and end%16!=0 and all(p[0]!=end%16 for p in phrase):
            put(end,3,0,None,12,0)
        if dur>=5:
            put(bar*16+step+2,3,0,None,4,0x12)

# Explicit initial timing without replacing an instrument trigger.
put(0,1,0,None,15,SPEED)
put(1,1,0,None,15,BPM)

def event_bytes(e):
    ins,p,fx,arg=e
    return bytes([(ins&240)|(p>>8),p&255,((ins&15)<<4)|fx,arg])
head=b'STORM LIGHT SKETCH'.ljust(20,b'\0')
for name,data,vol in samples:
    head+=name.encode('ascii')[:22].ljust(22,b'\0')+struct.pack('>HBBHH',len(data)//2,0,vol,0,1)
head+=bytes(30*(31-len(samples)))
head+=bytes([6,127])+bytes(range(6))+bytes(122)+b'M.K.'
module=head+b''.join(event_bytes(e) for row in grid for e in row)+b''.join(s[1] for s in samples)
path=OUT/'StormLight.mod'; path.write_bytes(module)

# Structural verification independent of the authoring grid.
assert module[1080:1084]==b'M.K.'
assert len(module)==1084+6*1024+sum(len(s[1]) for s in samples)
for i in range(1084,1084+6*1024,4):
    a,b,c,d=module[i:i+4]; p=((a&15)<<8)|b; ins=(a&240)|(c>>4)
    assert p==0 or p in periods
    assert 0<=ins<=len(samples)
    assert c&15 in (0,4,12,15)
    if c&15==12: assert d<=64

report={'title':'Storm Light — listening sketch','status':'experimental; user listening pending',
        'channels':4,'patterns':6,'bars':24,'speed':SPEED,'bpm_command':BPM,
        'quarter_note_bpm_at_four_rows_per_beat':93.75,
        'expected_seconds':61.44,'module_bytes':len(module),'sample_bytes':sum(len(s[1]) for s in samples),
        'module_sha256':hashlib.sha256(module).hexdigest(),
        'samples':[{'name':n,'bytes':len(d),'volume':v} for n,d,v in samples],
        'reference_audio_used_as_sample_data':False}

libpath=OUT.parents[2]/'build/music-reference-study/libmicromod.dylib'
if libpath.exists():
    lib=ctypes.CDLL(str(libpath))
    lib.micromod_initialise.argtypes=[ctypes.c_void_p,ctypes.c_long]
    lib.micromod_calculate_song_duration.restype=ctypes.c_long
    lib.micromod_get_audio.argtypes=[ctypes.c_void_p,ctypes.c_long]
    data=ctypes.create_string_buffer(module)
    assert lib.micromod_initialise(data,44100)==0
    count=lib.micromod_calculate_song_duration()
    assert abs(count/44100-61.44)<.05
    lib.micromod_set_position(0)
    pcm=np.zeros((count,2),dtype=np.int16)
    lib.micromod_get_audio(pcm.ctypes.data,count)
    assert np.max(abs(pcm.astype(np.int32)))<32767
    # Preview narrows classic hard panning to 50%; no added reverb/compression.
    x=pcm.astype(float)
    mid=x.mean(axis=1,keepdims=True); x=mid+.5*(x-mid)
    report['raw_render_peak_dbfs']=float(20*np.log10(abs(x).max()/32768))
    gain=min(2.,.84*32767/abs(x).max())
    x*=gain
    x[-22050:]*=np.linspace(1,0,22050)[:,None]
    with wave.open(str(OUT/'StormLight-preview.wav'),'wb') as w:
        w.setparams((2,2,44100,0,'NONE','not compressed'))
        w.writeframes(np.round(x).astype('<i2').tobytes())
    report['render_seconds']=count/44100
    report['preview_gain']=gain
    report['render_engine']='upstream micromod; host preview, no hardware verification'
(OUT/'manifest.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
