"""Three original, review-only three-voice Drowned Turbines MOD studies."""
from pathlib import Path
import ctypes, hashlib, json, struct, sys, wave
import numpy as np

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[1]
RATE=3546895/428
PCM_RATE=44100
PERIODS=[856,808,762,720,678,640,604,570,538,508,480,453,428,404,381,360,
         339,320,302,285,269,254,240,226,214,202,190,180,170,160,151,143,135,127,120,113]
sys.path.insert(0,str(ROOT/'experiments/audio-level1'))
from preview_library import library
LIB=ctypes.CDLL(str(library()))
LIB.micromod_initialise.argtypes=[ctypes.c_void_p,ctypes.c_long]
LIB.micromod_get_audio.argtypes=[ctypes.c_void_p,ctypes.c_long]
LIB.micromod_calculate_song_duration.restype=ctypes.c_long

STUDIES={
 'sluice-run':dict(bpm=116,title='SLUICE RUN',lead=('reed','bell'),
  chords=[('Dm',38,False),('Bb',46,True),('F',41,True),('C',36,True),
          ('Gm',43,False),('Bb',46,True),('C',36,True),('A',45,True),
          ('Dm',38,False),('F',41,True),('Bb',46,True),('C',36,True),
          ('Gm',43,False),('F',41,True),('C',36,True),('A',45,True)],
  themes=[[(0,0),(3,2),(5,1),(8,2),(10,3),(13,2),(15,1)],
          [(0,0),(2,1),(5,2),(7,3),(10,2),(12,1),(14,0)],
          [(0,2),(3,3),(6,2),(8,1),(11,0),(13,1)],
          [(0,1),(3,2),(5,3),(8,2),(10,1),(13,2),(15,0)]],
  answer=[[(0,2),(2,3),(4,4),(7,3),(9,2),(12,1),(14,2)],
          [(0,3),(3,2),(5,1),(8,0),(10,1),(13,2)],
          [(0,1),(2,2),(5,3),(7,2),(10,1),(12,0),(14,1)],
          [(0,2),(3,1),(6,0),(8,1),(11,2),(14,0)]]),
 'rainway':dict(bpm=122,title='RAINWAY',lead=('mallet','pulse'),
  chords=[('Gm',43,False),('Eb',39,True),('Bb',46,True),('F',41,True),
          ('Gm',43,False),('Cm',36,False),('D',38,True),('Gm',43,False),
          ('Eb',39,True),('Bb',46,True),('F',41,True),('Gm',43,False),
          ('Cm',36,False),('Eb',39,True),('D',38,True),('D',38,True)],
  themes=[[(0,0),(2,1),(4,2),(7,1),(9,3),(12,2),(14,1)],
          [(0,2),(3,3),(5,2),(8,1),(10,0),(13,1)],
          [(0,0),(3,2),(5,3),(8,4),(10,3),(13,2)],
          [(0,2),(2,1),(5,0),(7,1),(10,2),(12,1),(14,0)]],
  answer=[[(0,3),(2,2),(5,1),(7,0),(10,1),(12,2),(14,3)],
          [(0,2),(3,3),(6,4),(8,3),(11,2),(14,1)],
          [(0,1),(2,2),(4,3),(7,2),(9,1),(12,0),(14,2)],
          [(0,3),(3,2),(5,1),(8,2),(10,0),(13,1)]]),
 'pressure-line':dict(bpm=130,title='PRESSURE LINE',lead=('pulse','reed'),
  chords=[('Em',40,False),('C',36,True),('G',43,True),('D',38,True),
          ('Am',45,False),('C',36,True),('G',43,True),('B',47,True),
          ('Em',40,False),('G',43,True),('D',38,True),('C',36,True),
          ('Am',45,False),('G',43,True),('D',38,True),('B',47,True)],
  themes=[[(0,0),(2,2),(4,3),(7,2),(9,1),(12,2),(14,0)],
          [(0,1),(3,2),(5,3),(8,4),(10,3),(13,2)],
          [(0,2),(2,1),(5,0),(7,2),(10,3),(12,2),(14,1)],
          [(0,0),(3,1),(6,2),(8,1),(11,3),(14,2)]],
  answer=[[(0,3),(2,4),(5,3),(7,2),(10,1),(12,2),(14,3)],
          [(0,2),(3,1),(5,0),(8,1),(10,2),(13,3)],
          [(0,3),(2,2),(4,1),(7,0),(9,2),(12,1),(14,2)],
          [(0,1),(3,2),(6,3),(8,2),(11,1),(14,0)]]),
}

def sample_set():
    rng=np.random.default_rng(240923)
    samples=[]
    def add(name,sec,fn,vol):
        t=np.arange(int(sec*RATE)//2*2,dtype=np.float64)/RATE
        x=fn(t)
        x-=x.mean()
        # Original single-shot samples; soft attack/release avoids clicks.
        x[:32]*=np.linspace(0,1,32)
        x[-48:]*=np.linspace(1,0,48)
        data=np.rint(x/(max(abs(x).max(),1e-9))*118).astype('int8').tobytes()
        data=b'\0\0'+data[2:]
        samples.append((name,data,vol))
        return len(samples)
    kick=add('DEEP VALVE KICK',.18,lambda t:np.sin(2*np.pi*(48*t+70*(1-np.exp(-t*42))/42))*np.exp(-t*28),38)
    snare=add('RAIN SNARE',.16,lambda t:(rng.standard_normal(len(t))*.7+np.sin(2*np.pi*172*t)*.35)*np.exp(-t*33),29)
    hat=add('STEEL TICK',.055,lambda t:(rng.standard_normal(len(t))-np.roll(rng.standard_normal(len(t)),1)*.4)*np.exp(-t*90),11)
    tom=add('PIPE TOM',.15,lambda t:np.sin(2*np.pi*(88*t+38*(1-np.exp(-t*26))/26))*np.exp(-t*24),24)
    bass=add('ROUND TRACKER BASS C',.32,lambda t:(np.sin(2*np.pi*65.406*t)+.27*np.sin(2*np.pi*130.813*t)+.1*np.sin(2*np.pi*196.219*t))*np.exp(-t*9),39)
    reed=add('WET REED C',.43,lambda t:(np.sin(2*np.pi*130.813*t)+.24*np.sin(2*np.pi*261.626*t)+.07*np.sin(2*np.pi*392.439*t))*np.exp(-t*5.7),28)
    bell=add('BLUE BELL C',.30,lambda t:(np.sin(2*np.pi*130.813*t)+.36*np.sin(2*np.pi*261.626*t)+.09*np.sin(2*np.pi*523.252*t))*np.exp(-t*9),23)
    mallet=add('COPPER MALLET C',.27,lambda t:(np.sin(2*np.pi*130.813*t)+.31*np.sin(2*np.pi*391.8*t))*np.exp(-t*10),25)
    pulse=add('SOFT PULSE C',.32,lambda t:np.tanh(1.7*(np.sin(2*np.pi*130.813*t)+.22*np.sin(2*np.pi*261.626*t)))*np.exp(-t*6.3),23)
    chord_minor=add('SAMPLED MINOR CHORD C',.18,lambda t:(np.sin(2*np.pi*130.813*t)+.53*np.sin(2*np.pi*155.563*t)+.43*np.sin(2*np.pi*195.998*t))*np.exp(-t*16),17)
    chord_major=add('SAMPLED MAJOR CHORD C',.18,lambda t:(np.sin(2*np.pi*130.813*t)+.53*np.sin(2*np.pi*164.814*t)+.43*np.sin(2*np.pi*195.998*t))*np.exp(-t*16),17)
    return samples,dict(kick=kick,snare=snare,hat=hat,tom=tom,bass=bass,reed=reed,bell=bell,mallet=mallet,pulse=pulse,minor=chord_minor,major=chord_major)

def enc(inst,pitch,fx=0,param=0):
    period=0 if pitch is None else PERIODS[pitch-36]
    return bytes([(inst&0xf0)|(period>>8),period&255,((inst&15)<<4)|fx,param])

def make(name,cfg):
    samples,inst=sample_set()
    bars=48
    grid=[[[0,None,0,0] for _ in range(4)] for _ in range(bars*16)]
    def put(bar,step,ch,instrument,pitch,volume=None):
        assert 36<=pitch<=71
        grid[bar*16+step][ch]=[instrument,pitch,12,volume if volume is not None else samples[instrument-1][2]]
    for bar in range(bars):
        cname,root,major=cfg['chords'][bar%16]
        phrase_group=cfg['themes'] if bar//16!=1 else cfg['answer']
        phrase=phrase_group[bar%4]
        for step in [0,2,4,6,8,10,12,14]:
            drum=inst['kick'] if step in (0,8) else inst['snare'] if step in (4,12) else inst['hat']
            put(bar,step,0,drum,48)
        if bar%2==1:put(bar,15,0,inst['major' if major else 'minor'],min(root+12,71),15)
        if bar%16==15:
            for step,pitch in [(13,52),(14,48),(15,45)]:put(bar,step,0,inst['tom'],pitch,23)
        bass_steps=[(0,0),(3,0),(6,7),(8,0),(11,12),(14,7)]
        if name=='rainway':bass_steps=[(0,0),(4,7),(7,0),(9,0),(12,12),(14,7)]
        for step,interval in bass_steps:
            pitch=root+interval
            if pitch>59:pitch-=12
            put(bar,step,1,inst['bass'],pitch,37 if step in (0,8,9) else 29)
        degrees=[0,4 if major else 3,7,9 if major else 10,12]
        base=60+root%12
        if base>65:base-=12
        for j,(step,degree) in enumerate(phrase):
            pitch=base+degrees[degree]
            if pitch>71:pitch-=12
            if pitch<55:pitch+=12
            lead=cfg['lead'][0] if bar//8%2==0 else cfg['lead'][1]
            if j in (2,5):lead=cfg['lead'][1] if lead==cfg['lead'][0] else cfg['lead'][0]
            put(bar,step,2,inst[lead],pitch,26 if lead=='reed' else 22)
    assert all(row[3]==[0,None,0,0] for row in grid)
    pattern_count=bars//4
    header=cfg['title'].encode()[:20].ljust(20,b'\0')
    for sample_name,data,volume in samples:
        header+=sample_name.encode()[:22].ljust(22,b'\0')+struct.pack('>HBBHH',len(data)//2,0,volume,0,1)
    header+=bytes(30*(31-len(samples)))+bytes([pattern_count,0])+bytes(range(pattern_count))+bytes(128-pattern_count)+b'M.K.'
    grid[0][0][2:]=[15,6]
    grid[0][1][2:]=[15,cfg['bpm']]
    mod=header+b''.join(enc(*cell) for row in grid for cell in row)+b''.join(data for _,data,_ in samples)
    out=HERE/name
    out.mkdir(exist_ok=True)
    (out/(name+'.mod')).write_bytes(mod)
    split=1084+pattern_count*1024
    (out/'score.bin').write_bytes(mod[:split])
    (out/'bank.bin').write_bytes(mod[split:])
    assert all(mod[i:i+4]==bytes(4) for i in range(1096,split,16))
    assert sum(int.from_bytes(mod[42+i*30:44+i*30],'big')*2 for i in range(31))==len(mod)-split
    mbuf=ctypes.create_string_buffer(mod)
    assert LIB.micromod_initialise(mbuf,PCM_RATE)==0
    count=LIB.micromod_calculate_song_duration()
    assert abs(count/PCM_RATE-bars*4*60/cfg['bpm'])<.35
    LIB.micromod_set_position(0)
    pcm=np.zeros((count,2),np.int16)
    LIB.micromod_get_audio(pcm.ctypes.data,count)
    pcm=(pcm.astype('int32')*48//64).astype('<i2')
    assert abs(pcm.astype('int32')).max()<32767
    with wave.open(str(out/'preview.wav'),'wb') as w:
        w.setparams((2,2,PCM_RATE,0,'NONE','not compressed'))
        w.writeframes(pcm.tobytes())
    with wave.open(str(out/'short-preview.wav'),'wb') as w:
        w.setparams((2,2,PCM_RATE,0,'NONE','not compressed'))
        w.writeframes(pcm[:30*PCM_RATE].tobytes())
    report=dict(title=cfg['title'],bpm=cfg['bpm'],bars=bars,seconds=count/PCM_RATE,
      music_channels=3,reserved_channel=3,module_bytes=len(mod),score_bytes=split,
      bank_bytes=len(mod)-split,peak_dbfs=round(float(20*np.log10(max(abs(pcm.astype('int32')).max()/32768,1e-8))),1),
      sha256=hashlib.sha256(mod).hexdigest(),host_preview_only=True)
    (out/'manifest.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report))

if __name__=='__main__':
    for key,cfg in STUDIES.items():make(key,cfg)
