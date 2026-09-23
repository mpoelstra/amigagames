"""Checkpoint audition v2: latch, low electrical hum, one fixed confirmation tone."""
from pathlib import Path
import math,random,wave,json
R=Path(__file__).resolve().parents[1];O=R/'assets/audio/checkpoint-v2';RATE=3546895/322
O.mkdir(parents=True,exist_ok=True);rng=random.Random(2320);samples=[];filtered=0
n=int(.48*RATE)//2*2
for i in range(n):
 t=i/RATE;filtered+=.38*(rng.uniform(-1,1)-filtered)
 # Short inharmonic metal contact, rather than a pitched reward note.
 v=(.60*filtered+.19*math.sin(math.tau*1393*t)+.13*math.sin(math.tau*2247*t))*math.exp(-t*115)*min(1,t/.001)
 # Soft machine power-up texture: no rising scale or frequency sweep.
 if .025<=t<.25:
  u=t-.025;env=min(1,u/.015)*min(1,(.25-t)/.06)
  v+=.12*env*(math.sin(math.tau*147*u)+.28*math.sin(math.tau*294*u)+.12*filtered)
 # A single restrained sustained confirmation ping, fixed pitch throughout.
 if t>=.19:
  u=t-.19;env=min(1,u/.008)*math.exp(-u*14)
  v+=.39*env*(math.sin(math.tau*880*u)+.08*math.sin(math.tau*1760*u))
 v*=min(1,(n-1-i)/(.035*RATE));samples.append(v)
scale=104/max(map(abs,samples));pcm=[round(v*scale) for v in samples];pcm[0]=pcm[-1]=0
raw=bytes(v&255 for v in pcm);(O/'checkpoint.raw').write_bytes(raw)
with wave.open(str(O/'checkpoint.wav'),'wb') as w:
 w.setnchannels(1);w.setsampwidth(1);w.setframerate(round(RATE));w.writeframes(bytes(v+128 for v in pcm))
assert n%2==0 and max(map(abs,pcm))<128
with wave.open(str(O/'checkpoint.wav'),'rb') as w:
 assert bytes((v-128)&255 for v in w.readframes(w.getnframes()))==raw
(O/'manifest.json').write_text(json.dumps(dict(status='audition only; not in runtime',period=322,rate=RATE,bytes=n,seconds=n/RATE,peak=max(map(abs,pcm)),voice=False,design='metal latch,147Hz electrical hum,one880Hz confirmation; no melodic sequence',supersedes='checkpoint-v1 rising three-note concept'),indent=2)+'\n')
print(n,'bytes; verified WAV/raw parity; v1 preserved')
