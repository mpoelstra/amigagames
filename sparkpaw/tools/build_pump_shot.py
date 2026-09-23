"""Deterministic Pump Walker pressure discharge, native Paula period 322."""
from pathlib import Path
import math,random,wave,json
R=Path(__file__).resolve().parents[1];OUT=R/'assets/enemies/pump-shot-v1';RATE=3546895/322

def main():
 OUT.mkdir(exist_ok=True);rng=random.Random(68020);phase=0.;air=0.;values=[]
 n=round(.26*RATE)//2*2
 for i in range(n):
  t=i/RATE;phase+=math.tau*(68+100*math.exp(-t*45))/RATE
  air+=.24*(rng.uniform(-1,1)-air)
  attack=min(1,t/.0015);tail=min(1,(n-1-i)/(.018*RATE))
  body=(math.sin(phase)+.40*math.sin(2*phase)+.18*math.sin(3*phase))*math.exp(-t*15)
  crack=air*2.8*math.exp(-t*65)
  vent=air*.6*math.exp(-t*10)*(1-math.exp(-t*55))
  values.append((body+crack+vent)*attack*tail)
 scale=112/max(abs(v) for v in values);pcm=[round(v*scale) for v in values]
 pcm[0]=pcm[-1]=0;raw=bytes(v&255 for v in pcm)
 (OUT/'pump-shot.raw').write_bytes(raw)
 target=R/'build/drowned-slice/assets/pump-shot.raw';target.write_bytes(raw)
 with wave.open(str(OUT/'pump-shot.wav'),'wb') as w:
  w.setnchannels(1);w.setsampwidth(1);w.setframerate(round(RATE));w.writeframes(bytes(v+128 for v in pcm))
 assert len(raw)%2==0 and max(pcm)<127 and min(pcm)>-128
 (OUT/'manifest.json').write_text(json.dumps({'bytes':len(raw),'period':322,'rate':RATE,'seconds':len(raw)/RATE,'peak':max(map(abs,pcm)),'format':'signed 8-bit mono raw; WAV unsigned preview'},indent=2)+'\n')
 print(len(raw))
if __name__=='__main__':main()
