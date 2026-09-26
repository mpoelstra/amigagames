"""Lantern Current v1: original reference-inspired Drowned review, not integrated."""
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

def sample_set():
    rng=np.random.default_rng(23092617)
    samples=[]
    def add(name,sec,fn,vol):
        t=np.arange(int(sec*RATE)//2*2,dtype=np.float64)/RATE
        x=fn(t)
        # Offline rounding of the sample spectrum, no runtime filter cost.
        x=np.convolve(x, [0.18,0.64,0.18], mode='same')
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
    reed=add('RIVER FLUTE C',.68,lambda t:(np.sin(2*np.pi*130.813*t)+.15*np.sin(2*np.pi*261.626*t)+.035*np.sin(2*np.pi*392.439*t))*(1-np.exp(-t*130))*np.exp(-t*3.8),28)
    bell=add('BLUE BELL C',.30,lambda t:(np.sin(2*np.pi*130.813*t)+.36*np.sin(2*np.pi*261.626*t)+.09*np.sin(2*np.pi*523.252*t))*np.exp(-t*9),23)
    mallet=add('WATER MARIMBA C',.40,lambda t:(np.sin(2*np.pi*130.813*t)+.20*np.sin(2*np.pi*391.8*t)*np.exp(-t*14))*np.exp(-t*10),25)
    pulse=add('VELVET PULSE C',.48,lambda t:np.tanh(1.7*(np.sin(2*np.pi*130.813*t)+.22*np.sin(2*np.pi*261.626*t)))*np.exp(-t*6.3),23)
    chord_minor=add('SAMPLED MINOR CHORD C',.18,lambda t:(np.sin(2*np.pi*130.813*t)+.53*np.sin(2*np.pi*155.563*t)+.43*np.sin(2*np.pi*195.998*t))*np.exp(-t*16),17)
    chord_major=add('SAMPLED MAJOR CHORD C',.18,lambda t:(np.sin(2*np.pi*130.813*t)+.53*np.sin(2*np.pi*164.814*t)+.43*np.sin(2*np.pi*195.998*t))*np.exp(-t*16),17)
    return samples,dict(kick=kick,snare=snare,hat=hat,tom=tom,bass=bass,reed=reed,bell=bell,mallet=mallet,pulse=pulse,minor=chord_minor,major=chord_major)

def enc(inst,pitch,fx=0,param=0):
    period=0 if pitch is None else PERIODS[pitch-36]
    return bytes([(inst&0xf0)|(period>>8),period&255,((inst&15)<<4)|fx,param])

# Explicitly composed melodic lines, not reference transcriptions or per-chord arpeggios.
# D minor opening, F-major lift, G-minor colour; A major leads back into D minor.
BPM=128
A=[
 'A4 D5 C5 A4 F4 G4', 'A4 F4 D4 F4 G4 A4',
 'F4 A4 C5 D5 C5 A4', 'G4 E4 G4 A4 G4 E4',
 'F4 G4 A4 C5 A4 G4', 'F4 D4 F4 A4 G4 F4',
 'E4 G4 Bb4 A4 G4 E4', 'E4 C#4 E4 G4 A4 C#5',
 'D5 C5 A4 F4 A4 C5', 'D5 Bb4 A4 F4 G4 A4',
 'C5 A4 G4 F4 G4 A4', 'G4 E4 D4 E4 G4 C5',
 'Bb4 A4 G4 D4 F4 G4', 'A4 F4 D4 F4 A4 G4',
 'E4 G4 A4 Bb4 A4 G4', 'E4 C#4 D4 E4 G4 A4']
B=[
 'C5 A4 F4 G4 A4 C5', 'D5 C5 Bb4 A4 F4 G4',
 'A4 C5 D5 C5 A4 F4', 'G4 A4 G4 E4 D4 E4',
 'F4 A4 G4 F4 E4 D4', 'D4 F4 G4 A4 Bb4 A4',
 'G4 Bb4 A4 G4 F4 E4', 'C#4 E4 G4 A4 G4 E4',
 'G4 Bb4 D5 C5 Bb4 A4', 'Bb4 A4 F4 D4 F4 A4',
 'A4 G4 E4 C4 E4 G4', 'A4 F4 D4 E4 F4 A4',
 'Bb4 D5 C5 Bb4 A4 G4', 'A4 C5 A4 F4 G4 A4',
 'G4 E4 D4 E4 G4 Bb4', 'A4 G4 E4 C#4 E4 A4']
C=[
 'D5 A4 F4 A4 C5 D5', 'C5 Bb4 A4 F4 A4 Bb4',
 'A4 C5 A4 G4 F4 A4', 'G4 C5 B4 G4 E4 G4',
 'G4 A4 Bb4 D5 C5 Bb4', 'A4 G4 F4 D4 F4 A4',
 'G4 E4 G4 Bb4 A4 G4', 'E4 G4 A4 G4 E4 C#4']
CHORD_A=['Dm','Bb','F','C','Dm','Bb','C','A']*2
CHORD_B=['F','Bb','F','C','Dm','Bb','C','A','Gm','Bb','C','Dm','Gm','F','C','A']
ROOTS={'Dm':38,'Bb':46,'F':41,'C':36,'A':45,'Gm':43}
PC={'C':0,'C#':1,'D':2,'E':4,'F':5,'G':7,'A':9,'Bb':10,'B':11}
# The sketch is voiced one octave lower for a warm C3–D4 lead range.
def midi(s):return 12*int(s[-1])+PC[s[:-1]]
# Keep the complete hook, then a contrasting answer and return, with no breakdown.
melodies=A+B+A[:8]+C+B[8:]+C
chords=CHORD_A+CHORD_B+CHORD_A[:8]+['Dm','Bb','F','C','Gm','Dm','C','A']+CHORD_B[8:]+['Dm','Bb','F','C','Gm','Dm','C','A']
assert len(melodies)==len(chords)==64
RHYTHMS=[(0,3,6,8,11,14),(0,4,7,9,12,14),(0,3,5,8,10,14),(0,3,6,10,12,15)]

def main():
    samples,inst=sample_set()
    grid=[[[0,None,0,0] for _ in range(4)] for _ in range(64*16)]
    def put(bar,step,ch,name,pitch,vol):
        assert 36<=pitch<=71
        grid[bar*16+step][ch]=[inst[name],pitch,12,vol]
    for bar,(line,chord) in enumerate(zip(melodies,chords)):
        root=ROOTS[chord]
        for step in range(0,16,2):
            name='kick' if step in (0,8) else 'snare' if step in (4,12) else 'hat'
            put(bar,step,0,name,48,{'kick':36,'snare':23,'hat':9}[name])
        # Quiet sampled chords fit between percussion hits using the same voice.
        for step in (7,15):
            put(bar,step,0,'minor' if chord.endswith('m') else 'major',root+12,14)
        if bar%8==7:
            put(bar,13,0,'tom',50,20)
            put(bar,15,0,'tom',45,23)
        for step,interval,vol in [(0,0,35),(3,12,24),(6,7,29),(8,0,33),(11,7,26),(14,12,25)]:
            put(bar,step,1,'bass',root+interval,vol)
        # Last bass note approaches the next root; cadence repeats naturally.
        nxt=ROOTS[chords[(bar+1)%64]]
        if bar%4==3 and bar!=63:put(bar,15,1,'bass',nxt-1 if nxt>36 else 47,22)
        lead='reed' if bar<16 or 32<=bar<48 else 'mallet' if bar<32 else 'pulse'
        for j,(step,note) in enumerate(zip((0,3,6,8,10,12) if bar==63 else RHYTHMS[bar%4],line.split())):
            put(bar,step,2,lead,midi(note),27 if j in (0,3) else 24)
        # One little answer ornament at phrase ends, retaining melody and groove.
        if bar%8==5:put(bar,15,2,'bell',midi(line.split()[-1]),18)
    # A short articulated pickup rest at the final cadence lets all one-shot
    # tails settle before tracker restart, without a host-only crossfade.
    put(63,12,1,'bass',45,24)
    for step in (13,14):
        for ch in range(3):grid[63*16+step][ch]=[0,None,10,5]
    for ch in range(3):grid[63*16+15][ch]=[0,None,12,0]
    for bar in range(64):
        block=grid[bar*16:(bar+1)*16]
        assert all(sum(bool(r[ch][0]) for r in block)>=6 for ch in range(3))
    grid[0][0][2:]=[15,6]
    grid[0][1][2:]=[15,BPM]
    header=b'LANTERN CURRENT V1'.ljust(20,b'\0')
    for name,data,vol in samples:
        assert len(data)%2==0 and data[:2]==b'\0\0'
        header+=name.encode()[:22].ljust(22,b'\0')+struct.pack('>HBBHH',len(data)//2,0,vol,0,1)
    header+=bytes(30*(31-len(samples)))+bytes([16,0])+bytes(range(16))+bytes(112)+b'M.K.'
    score=header+b''.join(enc(*c) for row in grid for c in row)
    bank=b''.join(d for _,d,_ in samples)
    mod=score+bank
    assert len(score)==17468
    assert all(score[i:i+4]==bytes(4) for i in range(1096,len(score),16))
    assert sum(int.from_bytes(mod[42+i*30:44+i*30],'big')*2 for i in range(31))==len(bank)
    mbuf=ctypes.create_string_buffer(mod)
    assert LIB.micromod_initialise(mbuf,PCM_RATE)==0
    count=LIB.micromod_calculate_song_duration()
    assert abs(count/PCM_RATE-120)<.35
    LIB.micromod_set_position(0)
    # Render past the actual restart; never fake a loop with a crossfade.
    raw=np.zeros((count+8*PCM_RATE,2),np.int16)
    LIB.micromod_get_audio(raw.ctypes.data,len(raw))
    raw_peak=int(abs(raw.astype('int32')).max())
    assert raw_peak<32767
    pcm=(raw.astype('int32')*48//64).astype('<i2')
    def wav(name,data):
        with wave.open(str(HERE/name),'wb') as w:
            w.setparams((2,2,PCM_RATE,0,'NONE','not compressed'));w.writeframes(data.tobytes())
    wav('preview.wav',pcm[:count])
    wav('short-preview.wav',pcm[:32*PCM_RATE])
    wav('loop-check.wav',pcm[count-8*PCM_RATE:])
    (HERE/'lantern-current.mod').write_bytes(mod)
    (HERE/'rain-score.bin').write_bytes(score)
    (HERE/'rain-bank.bin').write_bytes(bank)
    # Bound the join against ordinary waveform changes, flag a potential click.
    steps=abs(np.diff(pcm.astype('int32'),axis=0))
    join_step=int(steps[count-1].max())
    assert join_step<max(512,int(np.quantile(steps,.999)))
    report=dict(title='Lantern Current v1',bpm=BPM,bars=64,seconds=count/PCM_RATE,
        score_bytes=len(score),bank_bytes=len(bank),module_bytes=len(mod),
        music_channels=3,reserved_channel=3,music_gain='48/64',
        raw_peak=raw_peak,peak_dbfs=round(float(20*np.log10(abs(pcm.astype('int32')).max()/32768)),2),
        rms_dbfs=round(float(20*np.log10(np.sqrt(np.mean((pcm.astype(float)/32768)**2)))),2),
        clipped_samples=int(np.sum(abs(raw.astype('int32'))>=32767)),
        loop_join_step=join_step,loop_join_step_dbfs=round(float(20*np.log10(max(join_step,1)/32768)),2),
        sha256=hashlib.sha256(mod).hexdigest(),preview_engine='micromod',
        integrated=False,native_tested=False,subjectively_auditioned_by_codex=False)
    refs=json.loads((HERE/'reference-hashes.json').read_text())
    for path,digest in refs.items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==digest,path
    report['preserved_reference_files']=len(refs)
    (HERE/'manifest.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))
if __name__=='__main__':main()
