"""Actual-C host replay of all proof events, and a labelled non-native preview."""
from pathlib import Path
import ctypes as C,subprocess,json,wave,re
import numpy as np
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];BUILD=ROOT/'build/audio-level1'
subprocess.run(['cc','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-g',str(HERE/'mix.c'),str(HERE/'test_mix.c'),'-o',str(BUILD/'test-mix')],check=True)
subprocess.run([str(BUILD/'test-mix')],check=True)
subprocess.run(['cc','-shared','-fPIC','-O2',str(HERE/'mix.c'),'-o',str(BUILD/'libmix.dylib')],check=True)
class Voice(C.Structure):_fields_=[('data',C.c_void_p),('remaining',C.c_uint32),('priority',C.c_uint8)]
class Effect(C.Structure):_fields_=[('data',C.c_void_p),('length',C.c_uint32),('priority',C.c_uint8),('cooldown',C.c_uint8)]
class Mixer(C.Structure):_fields_=[('voice',Voice*2),('cooldown',C.c_uint8*16),('requests',C.c_uint32*16),('starts',C.c_uint32*16),('rejected',C.c_uint32*16),('rendered',C.c_uint32),('completed',C.c_uint32*2),('overlaps',C.c_uint32)]
lib=C.CDLL(str(BUILD/'libmix.dylib'))
lib.mixRender.argtypes=[C.POINTER(Mixer),C.c_void_p,C.c_uint]
lib.mixSchedule.argtypes=[C.POINTER(Mixer),C.POINTER(Effect),C.c_uint32]
src=(HERE/'proof.c').read_text()
paths=re.findall(r'"PROGDIR:assets/runtime/([\w-]+\.raw)"',src);assert len(paths)==16
arrays={n:list(map(int,re.search(n+r'\[FX_COUNT\]=\{([^}]+)',src)[1].split(','))) for n in ['priorities','volumes','cooldowns']}
f=(Effect*16)();keep=[]
for i,p in enumerate(paths):
 raw=np.frombuffer((ROOT/'assets/runtime'/p).read_bytes(),np.int8)
 scaled=np.trunc(raw.astype(np.int32)*arrays['volumes'][i]/128).astype(np.int8)
 assert scaled.min()>=-64 and scaled.max()<=63
 keep.append(scaled);f[i]=Effect(scaled.ctypes.data,len(scaled),arrays['priorities'][i],arrays['cooldowns'][i])
m=Mixer();rate=3546895/322;total=round(4684/50*rate);out=np.zeros(total+112,np.int8)
field=0
for pos in range(0,total,112):
 while field<min(4684,int(pos/rate*50)):
  field+=1;lib.mixSchedule(C.byref(m),f,field)
 lib.mixRender(C.byref(m),out.ctypes.data+pos,112)
assert all(n>=1 for n in m.starts),list(m.starts)
assert m.overlaps>0 and m.rejected[5]>0
assert m.completed[0]>1 and m.completed[1]>=16
mod=(ROOT/'music/experiments/level1-pulse/copper-sprint.mod').read_bytes();split=9276
assert all(mod[a:a+4]==bytes(4) for a in range(1096,split,16))
# Ensure native reserved-channel patch removes every per-frame channel-4 entry.
built=(BUILD/'ptplayer-three.asm').read_text()
assert 'lea\tmt_chan4(a4),a2\n\tbsr\tmt_checkfx' not in built
assert 'lea\tmt_chan4(a4),a2\n\tbsr\tmt_playvoice' not in built
report=dict(starts=list(m.starts),suppressed=list(m.rejected),completed=list(m.completed),overlap_buffers=m.overlaps,source_fx_bytes=sum(len(x) for x in keep),native_verified=False)
(BUILD/'host-checks.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
# Offline listening aid only: actual MOD micromod + actual C SFX, hard Paula stereo.
from preview_library import library
mic=C.CDLL(str(library()))
mic.micromod_initialise.argtypes=[C.c_void_p,C.c_long];mic.micromod_get_audio.argtypes=[C.c_void_p,C.c_long]
buf=C.create_string_buffer(mod);assert mic.micromod_initialise(buf,44100)==0
n=round(4684/50*44100);pcm=np.zeros((n,2),np.int16);mic.micromod_get_audio(pcm.ctypes.data,n)
# micromod internal Paula scale is 127 for volume-64 fullscale input in each side.
x=pcm.astype(np.int32)*48//64
fx=np.interp(np.arange(n)/44100,np.arange(len(out))/rate,out.astype(float))
x[:,0]+=np.rint(fx*127).astype(np.int32)
assert abs(x).max()<32767
preview=ROOT/'music/experiments/level1-pulse/copper-sprint-fx-preview.wav'
with wave.open(str(preview),'wb') as w:
 w.setparams((2,2,44100,0,'NONE','not compressed'));w.writeframes(x.astype('<i2').tobytes())
print('Offline preview only:',preview)
