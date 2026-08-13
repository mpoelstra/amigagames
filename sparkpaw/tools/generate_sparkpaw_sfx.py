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

def gain(track,amount): return [value*amount for value in track]

def delay(track, seconds): return [0.0]*round(seconds*RATE)+track

def notes(sequence, duration=.08):
    return [v for f in sequence for v in tone(duration, f, 8)]

def jump(): return jump_soft_whoosh()
def land(): return mix(noise(.13,1200,18,.22),tone(.12,85,20,-120))
def energy_shot(): return mix(tone(.17,950,11,-2200,True),tone(.13,1700,15,-500))
def charged_shot(): return mix(tone(.34,430,6,1800,True),noise(.24,68020,12,.6),delay(tone(.22,1200,10,-1600),.05))
def strider_shot():
    # Heavy mechanical discharge: bass body, metal crack, short electric tail.
    return mix(gain(tone(.20,92,11,-70),.92),
               gain(tone(.13,184,19,-430,True),.52),
               gain(noise(.11,68520,23,.14),.64),
               gain(delay(tone(.11,620,21,-2400),.022),.34))
def enemy_hit(): return plasma_hit_energy_pop()

def player_hurt():
    """Selected short arcade-like feline chirp for accepted contact damage."""
    return hurt_meow_chirp()

def vocal_hurt(seconds,pitch_start,pitch_end,formants_start,formants_end,
               seed,vibrato=0,rasp=0):
    """Create an original formant-shaped grunt/yowl without recorded speech."""
    phase=0.0; out=[]; rng=random.Random(seed)
    for i in range(round(seconds*RATE)):
        t=i/RATE; progress=t/seconds
        pitch=pitch_start+(pitch_end-pitch_start)*progress
        if vibrato:
            pitch+=vibrato*math.sin(math.tau*(8+3*progress)*t)
        phase+=pitch/RATE
        f1=formants_start[0]+(formants_end[0]-formants_start[0])*progress
        f2=formants_start[1]+(formants_end[1]-formants_start[1])*progress
        voiced=0.0
        for harmonic in range(1,27):
            frequency=harmonic*pitch
            resonance=(.14+
                1.30*math.exp(-((frequency-f1)/155)**2)+
                .82*math.exp(-((frequency-f2)/245)**2))
            voiced+=math.sin(math.tau*harmonic*phase)*resonance/harmonic
        attack=min(1.0,t/.010)
        release=max(0.0,min(1.0,(seconds-t)/(.06+seconds*.08)))
        envelope=attack*release*(1.0-.22*progress)
        rough=rng.uniform(-1,1)*(rasp*(1.0-.45*progress))
        out.append(88*(voiced+rough)*envelope)
    return out

def hurt_stomp():
    # A body-impact thud followed by a very short involuntary low grunt.
    return mix(noise(.10,8086,24,.14),
               delay(vocal_hurt(.15,150,105,(560,900),(430,720),
                                 68000,5,.08),.012))

def hurt_oof():
    # Compact rounded "oef": readable, but less speech-like than the auw.
    return vocal_hurt(.23,175,112,(610,1050),(420,690),68001,7,.10)

def hurt_stomp_full():
    return mix(gain(noise(.13,8087,20,.16),.62),
               gain(tone(.16,112,15,-190),.56),
               delay(vocal_hurt(.20,168,102,(590,970),(420,700),
                                 68010,7,.10),.010))

def hurt_stomp_low():
    return mix(gain(noise(.11,8088,25,.12),.48),
               gain(tone(.18,92,13,-105),.66),
               delay(vocal_hurt(.21,138,82,(510,830),(365,610),
                                 68011,5,.11),.014))

def hurt_stomp_bass():
    return mix(gain(noise(.09,8089,28,.10),.42),
               gain(tone(.22,78,12,-70),.78),
               gain(tone(.14,156,17,-160),.30),
               delay(vocal_hurt(.18,148,91,(535,875),(385,640),
                                 68012,4,.08),.018))

def hurt_oof_full():
    return mix(vocal_hurt(.27,182,108,(640,1080),(415,680),
                          68013,8,.09),
               gain(delay(vocal_hurt(.20,118,82,(470,760),(350,580),
                                      68014,4,.05),.016),.36))

def hurt_oof_low():
    return mix(vocal_hurt(.28,148,84,(535,870),(365,600),
                          68015,7,.11),
               gain(tone(.18,96,15,-95),.34))

def hurt_oof_bass():
    return mix(vocal_hurt(.26,158,92,(560,910),(380,625),
                          68016,6,.09),
               gain(tone(.23,82,11,-75),.62),
               gain(delay(noise(.08,8090,30,.10),.006),.26))

def hurt_groan():
    # Longer low groan for a heavier impact or future stronger damage.
    return vocal_hurt(.46,158,88,(590,980),(390,650),68002,11,.13)

def hurt_cornered_cat():
    # Synthetic feline alarm/yowl: tense rise, then a falling release.
    first=vocal_hurt(.16,300,465,(820,1510),(980,1780),68003,20,.17)
    second=vocal_hurt(.25,460,245,(980,1780),(670,1250),68004,24,.20)
    return first+second

def hurt_meow_snap():
    # Very short alarmed "mraow": fast rise and an abrupt falling release.
    rise=vocal_hurt(.075,390,690,(850,1450),(1080,1900),68300,18,.13)
    fall=vocal_hurt(.105,680,330,(1050,1850),(720,1260),68301,20,.16)
    return rise+fall

def hurt_meow_yelp():
    # Brighter contact yelp that remains readable over rapid plasma effects.
    rise=vocal_hurt(.065,510,880,(980,1680),(1230,2150),68302,25,.10)
    fall=vocal_hurt(.090,850,410,(1210,2100),(790,1370),68303,22,.12)
    return mix(rise+fall,gain(noise(.055,68304,34,.20),.13))

def hurt_meow_rough():
    # Lower, raspier feline protest with more body than the bright yelp.
    rise=vocal_hurt(.085,315,520,(760,1290),(930,1640),68305,15,.20)
    fall=vocal_hurt(.125,510,255,(920,1600),(620,1080),68306,17,.24)
    return mix(rise+fall,gain(tone(.18,118,16,-80),.18))

def hurt_meow_chirp():
    # Compact arcade-like feline chirp, deliberately under a fifth second.
    first=vocal_hurt(.060,470,760,(920,1580),(1130,1980),68307,28,.08)
    second=vocal_hurt(.080,745,375,(1110,1940),(750,1300),68308,26,.10)
    return first+second

def jump_arcade_rise():
    return mix(gain(tone(.20,245,9,1180),.72),
               gain(delay(tone(.13,510,14,920),.025),.38))

def jump_soft_whoosh():
    return mix(gain(noise(.18,68100,14,.12),.36),
               gain(tone(.22,205,8,720),.62))

def jump_low_launch():
    return mix(gain(tone(.18,145,10,520),.76),
               gain(delay(tone(.12,390,15,650),.018),.34),
               gain(noise(.07,68101,28,.10),.24))

def jump_hup():
    return mix(vocal_hurt(.16,205,255,(570,990),(660,1180),
                          68102,7,.06),
               gain(tone(.14,260,13,580),.28))

def plasma_hit_crack():
    return mix(gain(noise(.11,68200,25,.58),.72),
               gain(tone(.13,310,18,-950,True),.48))

def plasma_hit_energy_pop():
    return mix(gain(tone(.15,1180,17,-5100,True),.66),
               gain(noise(.09,68201,30,.44),.40),
               gain(delay(tone(.10,260,22,-520),.025),.32))

def plasma_hit_shell():
    return mix(gain(tone(.18,430,16,-780),.62),
               gain(tone(.13,860,23,-1450),.38),
               gain(noise(.08,68202,29,.30),.42))

def plasma_hit_heavy():
    return mix(gain(tone(.19,125,13,-150),.72),
               gain(tone(.12,250,20,-620,True),.42),
               gain(noise(.10,68203,25,.18),.44))

def collect(): return notes((659,880,1175),.065)
def checkpoint(): return notes((392,523,659,784),.10)
def stormstone(): return mix(notes((523,659,784,1047),.13),delay(tone(.55,1319,5,-180),.22))
def menu(): return tone(.07,780,18,2100)

EFFECTS=[
    ("jump",jump,58,4,4),("land",land,48,2,4),
    ("energy-shot",energy_shot,60,5,3),("charged-shot",charged_shot,64,8,8),
    ("strider-shot",strider_shot,60,7,12),
    ("enemy-hit",enemy_hit,60,6,4),("player-hurt",player_hurt,64,9,16),
    ("collect-spark",collect,52,4,3),("checkpoint",checkpoint,60,7,15),
    ("stormstone",stormstone,64,10,30),("menu-select",menu,48,1,3),
]

HURT_PREVIEW_VARIANTS=[
    ("player-hurt-candidate-stomp",hurt_stomp),
    ("player-hurt-candidate-oof",hurt_oof),
    ("player-hurt-candidate-groan",hurt_groan),
    ("player-hurt-candidate-cornered-cat",hurt_cornered_cat),
    ("player-hurt-candidate-stomp-full",hurt_stomp_full),
    ("player-hurt-candidate-stomp-low",hurt_stomp_low),
    ("player-hurt-candidate-stomp-bass",hurt_stomp_bass),
    ("player-hurt-candidate-oof-full",hurt_oof_full),
    ("player-hurt-candidate-oof-low",hurt_oof_low),
    ("player-hurt-candidate-oof-bass",hurt_oof_bass),
    ("player-hurt-candidate-meow-snap",hurt_meow_snap),
    ("player-hurt-candidate-meow-yelp",hurt_meow_yelp),
    ("player-hurt-candidate-meow-rough",hurt_meow_rough),
    ("player-hurt-candidate-meow-chirp",hurt_meow_chirp),
]

JUMP_PREVIEW_VARIANTS=[
    ("jump-candidate-arcade-rise",jump_arcade_rise),
    ("jump-candidate-soft-whoosh",jump_soft_whoosh),
    ("jump-candidate-low-launch",jump_low_launch),
    ("jump-candidate-hup",jump_hup),
]

PLASMA_HIT_PREVIEW_VARIANTS=[
    ("plasma-hit-candidate-crack",plasma_hit_crack),
    ("plasma-hit-candidate-energy-pop",plasma_hit_energy_pop),
    ("plasma-hit-candidate-shell",plasma_hit_shell),
    ("plasma-hit-candidate-heavy",plasma_hit_heavy),
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
    audition_previews=(HURT_PREVIEW_VARIANTS+JUMP_PREVIEW_VARIANTS+
                       PLASMA_HIT_PREVIEW_VARIANTS)
    for name,maker in audition_previews:
        values=pcm(maker()); unsigned=bytes(v+128 for v in values)
        with wave.open(str(preview/f"{name}.wav"),"wb") as wav:
            wav.setparams((1,1,RATE,len(unsigned),"NONE","not compressed"))
            wav.writeframes(unsigned)
    print(f"Wrote {len(records)} original effects")
    print(f"Wrote {len(audition_previews)} audition previews")

if __name__=="__main__": build()
