"""Checkpoint audio audition only: mechanical catch + rising confirmation."""
from pathlib import Path
import math,random,wave,json
R=Path(__file__).resolve().parents[1];O=R/'assets/audio/checkpoint-v1';RATE=3546895/322
O.mkdir(parents=True,exist_ok=True);rng=random.Random(2320);samples=[];filtered=0
n=int(.62*RATE)//2*2
for i in range(n):
 t=i/RATE;filtered+=.3*(rng.uniform(-1,1)-filtered)
 v=.7*filtered*math.exp(-t*110)
 for onset,freq,amplitude in [(0.045,587.33,.65),(.145,880,.58),(.255,1174.66,.48)]:
  u=t-onset
  if u>=0:
   env=min(1,u/.006)*math.exp(-u*12)
   v+=amplitude*env*(math.sin(math.tau*freq*u)+.17*math.sin(math.tau*freq*2*u))
 v*=min(1,(n-1-i)/(.03*RATE));samples.append(v)
scale=104/max(map(abs,samples));pcm=[round(v*scale) for v in samples];pcm[0]=pcm[-1]=0
(O/'checkpoint.raw').write_bytes(bytes(v&255 for v in pcm))
with wave.open(str(O/'checkpoint.wav'),'wb') as w:
 w.setnchannels(1);w.setsampwidth(1);w.setframerate(round(RATE));w.writeframes(bytes(v+128 for v in pcm))
(O/'manifest.json').write_text(json.dumps(dict(status='audition only; not in runtime',period=322,rate=RATE,bytes=n,seconds=n/RATE,peak=max(map(abs,pcm)),voice=False),indent=2)+'\n')
assert n%2==0 and max(map(abs,pcm))<128
print('Checkpoint sound audition:',n,'bytes; no runtime integration')
