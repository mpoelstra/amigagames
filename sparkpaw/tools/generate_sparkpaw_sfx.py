#!/usr/bin/env python3
"""Generate original 8-bit mono Sparkpaw prototype effects and packed bank."""

import json, math, random, struct, wave
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RATE = 11025
PERIOD = round(3546895 / RATE)

def tone(seconds, frequency, decay=6, sweep=0, square=False):
    phase=0.0; out=[]
    for i in range(round(seconds*RATE)):
        t=i/RATE; phase+=(frequency+sweep*t)/RATE
        wave_value=1 if math.sin(math.tau*phase)>=0 else -1 if square else math.sin(math.tau*phase)
        if not square: wave_value=math.sin(math.tau*phase)
        out.append(118*wave_value*math.exp(-decay*t))
    return out

def noise(seconds, seed, decay=10, lowpass=.35):
    rng=random.Random(seed); state=0.0; out=[]
    for i in range(round(seconds*RATE)):
        state+=(rng.uniform(-1,1)-state)*lowpass
        out.append(state*145*math.exp(-decay*i/RATE))
    return out

def mix(*tracks):
    length=max(map(len, tracks))
    return [sum(t[i] if i<len(t) else 0 for t in tracks) for i in range(length)]

def delay(track, seconds): return [0.0]*round(seconds*RATE)+track

def notes(sequence, duration=.08):
    return [v for f in sequence for v in tone(duration, f, 8)]

def jump(): return mix(tone(.22,260,8,1500), tone(.14,520,12,900))
def land(): return mix(noise(.13,1200,18,.22),tone(.12,85,20,-120))
def energy_shot(): return mix(tone(.17,950,11,-2200,True),tone(.13,1700,15,-500))
def charged_shot(): return mix(tone(.34,430,6,1800,True),noise(.24,68020,12,.6),delay(tone(.22,1200,10,-1600),.05))
def enemy_hit(): return mix(tone(.14,190,18,-650,True),noise(.10,31337,22,.5))
def player_hurt(): return notes((440,370,294),.07)
def collect(): return notes((659,880,1175),.065)
def checkpoint(): return notes((392,523,659,784),.10)
def stormstone(): return mix(notes((523,659,784,1047),.13),delay(tone(.55,1319,5,-180),.22))
def menu(): return tone(.07,780,18,2100)

EFFECTS=[
    ("jump",jump,58,4,4),("land",land,48,2,4),
    ("energy-shot",energy_shot,60,5,3),("charged-shot",charged_shot,64,8,8),
    ("enemy-hit",enemy_hit,60,6,4),("player-hurt",player_hurt,64,9,12),
    ("collect-spark",collect,52,4,3),("checkpoint",checkpoint,60,7,15),
    ("stormstone",stormstone,64,10,30),("menu-select",menu,48,1,3),
]

def pcm(values):
    peak=max(1.0,max(abs(v) for v in values)); scale=min(1.0,124/peak)
    out=[max(-128,min(127,round(v*scale))) for v in values]
    if len(out)&1: out.append(0)
    return out

def build():
    raw_dir=ROOT/"sfx"/"raw"; preview=ROOT/"assets"/"sfx"/"previews"
    raw_dir.mkdir(parents=True,exist_ok=True); preview.mkdir(parents=True,exist_ok=True)
    records=[]; blocks=[]; offset=0
    for name,maker,volume,priority,cooldown in EFFECTS:
        values=pcm(maker()); signed=bytes(v&255 for v in values); unsigned=bytes(v+128 for v in values)
        (raw_dir/f"{name}.raw").write_bytes(signed)
        with wave.open(str(preview/f"{name}.wav"),"wb") as wav:
            wav.setparams((1,1,RATE,len(unsigned),"NONE","not compressed")); wav.writeframes(unsigned)
        records.append({"name":name,"offset":offset,"length":len(signed),"period":PERIOD,"volume":volume,"priority":priority,"cooldown_frames":cooldown})
        blocks.append(signed); offset+=len(signed)
    header=8+32*len(records); table=bytearray(b"SPFX"+struct.pack(">HH",1,len(records)))
    for r in records:
        table+=r["name"].encode("ascii")[:16].ljust(16,b"\0")
        table+=struct.pack(">IIHBBHH",header+r["offset"],r["length"],r["period"],r["volume"],r["priority"],r["cooldown_frames"],0)
    (ROOT/"sfx"/"sparkpaw-sfx.bank").write_bytes(bytes(table)+b"".join(blocks))
    (ROOT/"sfx"/"sparkpaw-sfx.json").write_text(json.dumps({"format":"SPFX v1","sample_rate":RATE,"effects":records},indent=2)+"\n")
    print(f"Wrote {len(records)} original effects")

if __name__=="__main__": build()
