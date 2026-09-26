"""Three original sustained-lead MOD proposals, preview only."""
from pathlib import Path
import ctypes, hashlib, json, struct, sys, wave, runpy
import numpy as np
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[1]
SR=44100
PERIODS=[856,808,762,720,678,640,604,570,538,508,480,453,428,404,381,360,339,320,302,285,269,254,240,226,214,202,190,180,170,160,151,143,135,127,120,113]
sys.path.insert(0,str(ROOT/'experiments/audio-level1'))
from preview_library import library
LIB=ctypes.CDLL(str(library()))
LIB.micromod_initialise.argtypes=[ctypes.c_void_p,ctypes.c_long]
LIB.micromod_get_audio.argtypes=[ctypes.c_void_p,ctypes.c_long]
LIB.micromod_calculate_song_duration.restype=ctypes.c_long
BASE=runpy.run_path(str(HERE.parent/'drowned-lantern-current-v1/generate.py'))
# Each line spans TWO bars. Offsets are sixteenth-note rows, not retriggers.
# Written independently of the earlier themes and the supplied recordings.
THEMES={
 'turbine-heart': dict(title='TURBINE HEART',bpm=132,style='brass',key=0,
  a=[[(0,57),(8,62),(24,60),(28,57)],[(0,58),(12,57),(20,53)],
     [(0,57),(16,60),(24,62)],[(0,55),(12,52),(24,55),(28,57)],
     [(0,62),(16,65),(24,64),(28,62)],[(0,65),(12,62),(24,58)],
     [(0,60),(16,57),(24,55)],[(0,57),(12,61),(24,64),(28,61)]],
  b=[[(0,65),(16,64),(24,60)],[(0,62),(12,60),(24,57)],
     [(0,58),(16,62),(24,65)],[(0,64),(12,60),(24,55)],
     [(0,62),(8,65),(24,67),(28,65)],[(0,65),(16,62),(24,58)],
     [(0,60),(12,57),(24,55)],[(0,61),(16,64),(24,61),(28,57)]]),
 'deepwater-run': dict(title='DEEPWATER RUN',bpm=124,style='strings',key=3,
  a=[[(0,62),(20,60),(28,57)],[(0,58),(16,57),(24,53)],
     [(0,60),(12,57),(24,53)],[(0,55),(20,52),(28,55)],
     [(0,57),(8,62),(24,64)],[(0,65),(20,62),(28,58)],
     [(0,60),(16,57),(24,55)],[(0,57),(20,61),(28,57)]],
  b=[[(0,65),(20,64),(28,60)],[(0,62),(16,57),(24,53)],
     [(0,62),(20,65),(28,62)],[(0,64),(16,60),(24,55)],
     [(0,67),(20,65),(28,62)],[(0,65),(16,62),(24,58)],
     [(0,60),(20,57),(28,55)],[(0,61),(16,64),(24,57)]]),
 'iron-tide': dict(title='IRON TIDE',bpm=140,style='pulse',key=2,
  a=[[(0,57),(6,62),(20,60),(28,57)],[(0,58),(12,62),(24,57)],
     [(0,60),(16,57),(24,53)],[(0,55),(10,57),(24,55),(28,52)],
     [(0,62),(12,65),(24,64)],[(0,65),(16,62),(24,58)],
     [(0,60),(10,57),(24,55)],[(0,61),(16,64),(24,61),(28,57)]],
  b=[[(0,65),(12,64),(24,60)],[(0,62),(16,60),(24,57)],
     [(0,58),(8,62),(24,65)],[(0,64),(16,60),(24,55)],
     [(0,67),(12,65),(24,62)],[(0,65),(16,62),(24,58)],
     [(0,60),(8,57),(24,55)],[(0,61),(16,64),(24,61),(28,57)]])}
# Two-bar chords let the melodic line breathe across bar boundaries.
HARM_A=[38,46,41,36,38,46,36,45]
HARM_B=[41,38,46,36,43,46,36,45]

def palette(style):
    original,_=BASE['sample_set']()
    samples=[(name,data,vol,0,2) for name,data,vol in original[:4]]
    def sustain(name,flavour,vol):
        # 128 cycles across an even 8128-byte loop; ~C3, no loop padding.
        # Sidebands make a small repeating chorus; no runtime chorus/mixing.
        size=8128;prefix=1016
        u=np.arange(size)/size
        def body(v):
            phase=2*np.pi*128*v
            if flavour=='strings':
                return sum(np.sin(h*phase+.22*np.sin(2*np.pi*v))*a for h,a in [(1,1),(2,.36),(3,.18),(4,.09),(5,.045)])+.17*np.sin(phase+2*np.pi*v)
            if flavour=='brass':
                return sum(np.sin(h*phase)*a for h,a in [(1,1),(2,.50),(3,.23),(4,.09),(5,.055)])
            if flavour=='pulse':
                return np.sin(phase)+.32*np.sin(3*phase)+.12*np.sin(5*phase)+.20*np.sin(2*phase)
            return np.sin(phase)+.34*np.sin(2*phase)+.12*np.sin(3*phase)
        loop=body(u)
        pre=body(np.arange(-prefix,0)/size)*np.minimum(np.arange(prefix)/180,1)
        x=np.r_[pre,loop];x=x/max(abs(x).max(),1e-8)*112
        data=np.rint(x).astype('int8').tobytes()
        loop_pcm=np.frombuffer(data[prefix:],dtype='int8').astype(int)
        assert abs(loop_pcm[-1]-loop_pcm[0])<=int(abs(np.diff(loop_pcm)).max())+1
        samples.append((name,data,vol,prefix,size))
        return len(samples)
    bass=sustain('ANALOGUE BASS C', 'bass',33)
    lead=sustain('SUSTAIN '+style.upper(),style,27)
    alternate=sustain('ANSWER '+style.upper(),'strings' if style=='brass' else 'brass' if style=='strings' else 'pulse',25)
    return samples,bass,lead,alternate

def make(slug,cfg):
    out=HERE/slug;out.mkdir(exist_ok=True)
    samples,bass,lead,alternate=palette(cfg['style'])
    grid=[[[0,0,0,0] for _ in range(4)] for _ in range(64*16)]
    def put(row,ch,inst=0,note=0,fx=0,param=0):
        assert not note or 36<=note<=71,(row,note)
        grid[row][ch]=[inst,PERIODS[note-36] if note else 0,fx,param]
    shift=cfg['key']
    chord_roots=(HARM_A+HARM_B+HARM_A+HARM_B)
    for bar in range(64):
        row=bar*16;root=chord_roots[bar//2]+shift
        while root>47:root-=12
        # Solid backbeat, occasional syncopated kick, softer metallic ticks.
        for step in range(0,16,2):
            inst=1 if step in (0,8) else 2 if step in (4,12) else 3
            put(row+step,0,inst,48,12,{1:39,2:30,3:11}[inst])
        if cfg['style']=='pulse':put(row+10,0,1,48,12,32)
        if bar%8==7:
            put(row+13,0,4,50,12,25);put(row+15,0,4,45,12,27)
        # Less frantic bass, tied durations, one octave response per bar.
        for step,interval,vol in [(0,0,34),(6,0,29),(8,12,28),(12,7,29)]:
            put(row+step,1,bass,root+interval,12,vol)
        for step in (5,11,15):put(row+step,1,0,0,10,3)
    events=[]
    for section in range(4):
        theme=cfg['a'] if section in (0,2) else cfg['b']
        instrument=lead if section in (0,2,3) else alternate
        for phrase,notes in enumerate(theme):
            for n,(offset,pitch) in enumerate(notes):
                row=section*256+phrase*32+offset
                pitch+=shift
                # Theme return changes the long note contour in two phrases.
                if section==2 and phrase in (2,6) and n==0:pitch+=2
                assert pitch<=71
                events.append((row,pitch,instrument))
    for idx,(row,pitch,instrument) in enumerate(events):
        end=events[idx+1][0] if idx+1<len(events) else 1024
        # Legato slides join selected phrase-internal steps without reattack.
        slide=idx>0 and row%32!=0 and idx%3==1 and events[idx-1][2]==instrument
        if slide:put(row,2,0,pitch,3,5)
        else:put(row,2,instrument,pitch,12,27 if row%32==0 else 25)
        for r in range(row+1,end):
            if slide and r<row+4:put(r,2,0,0,3,5)
            elif r>=row+3:put(r,2,0,0,4,0x32) # delayed gentle vibrato
        # Restore stable volume after a preceding phrase release.
        if slide and row+4<end:put(row+4,2,0,0,12,25)
    # Phrase ending releases, no quiet middle section. Only loop has a tiny rest.
    for r in (1021,1022):
        for ch in (0,1,2):put(r,ch,0,0,10,4)
    for ch in (0,1,2):put(1023,ch,0,0,12,0)
    grid[0][0][2:]=[15,6];grid[0][1][2:]=[15,cfg['bpm']]
    header=cfg['title'].encode()[:20].ljust(20,b'\0')
    for name,data,vol,start,length in samples:
        assert len(data)%2==start%2==length%2==0 and start+length<=len(data)
        header+=name.encode()[:22].ljust(22,b'\0')+struct.pack('>HBBHH',len(data)//2,0,vol,start//2,length//2)
    header+=bytes(30*(31-len(samples)))+bytes([16,0])+bytes(range(16))+bytes(112)+b'M.K.'
    def enc(e):
        i,p,f,a=e;return bytes([(i&240)|(p>>8),p&255,((i&15)<<4)|f,a])
    score=header+b''.join(enc(e) for row in grid for e in row)
    bank=b''.join(s[1] for s in samples);mod=score+bank
    assert len(score)==17468 and all(score[i:i+4]==bytes(4) for i in range(1096,len(score),16))
    mbuf=ctypes.create_string_buffer(mod);assert LIB.micromod_initialise(mbuf,SR)==0
    count=LIB.micromod_calculate_song_duration()
    assert abs(count/SR-64*4*60/cfg['bpm'])<.4
    LIB.micromod_set_position(0)
    raw=np.zeros((count+8*SR,2),np.int16);LIB.micromod_get_audio(raw.ctypes.data,len(raw))
    assert abs(raw.astype('int32')).max()<32767
    pcm=(raw.astype('int32')*48//64).astype('<i2')
    def wav(name,data):
        with wave.open(str(out/name),'wb') as w:w.setparams((2,2,SR,0,'NONE','not compressed'));w.writeframes(data.tobytes())
    wav('preview.wav',pcm[:count]);wav('short-preview.wav',pcm[:40*SR]);wav('loop-check.wav',pcm[count-8*SR:])
    join=int(abs(pcm[count].astype(int)-pcm[count-1].astype(int)).max());assert join<512
    (out/(slug+'.mod')).write_bytes(mod);(out/'rain-score.bin').write_bytes(score);(out/'rain-bank.bin').write_bytes(bank)
    holds=np.diff([e[0] for e in events]+[1024])*6*2.5/cfg['bpm']
    report=dict(title=cfg['title'],bpm=cfg['bpm'],seconds=count/SR,score_bytes=len(score),bank_bytes=len(bank),
      lead_events=len(events),mean_note_seconds=round(float(holds.mean()),2),longest_note_seconds=round(float(holds.max()),2),
      peak_dbfs=round(float(20*np.log10(abs(pcm.astype(int)).max()/32768)),2),
      clipped_samples=0,loop_join_step=join,music_channels=3,reserved_channel=3,
      sha256=hashlib.sha256(mod).hexdigest(),preview_engine='micromod',gain='48/64',integrated=False,native_tested=False)
    (out/'manifest.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':
    for slug,cfg in THEMES.items():make(slug,cfg)
