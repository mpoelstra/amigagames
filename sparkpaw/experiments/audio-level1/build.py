"""Build only the isolated audio proof; production targets/assets stay untouched."""
from pathlib import Path
import os,subprocess,hashlib,json,sys
TIMING="--timing" in sys.argv
CONTROL="--control" in sys.argv
assert not (TIMING and CONTROL)
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[1]
BUILD=ROOT/('build/audio-block-timing' if TIMING else ('build/audio-block-control' if CONTROL else 'build/audio-level1'));BUILD.mkdir(parents=True,exist_ok=True)
SDK=ROOT/'.toolchain/sdk';env=os.environ.copy();env['VBCC']=str(SDK);env['PATH']=str(SDK/'bin')+os.pathsep+env['PATH']
env['TMPDIR']=str(BUILD)
src=(HERE/'third_party/ptplayer.asm').read_text()
# Upstream channelmask suppresses LC/LEN, but empty channel can still write PER.
# This proof has a certified entirely empty track 4: consume its row without
# executing that voice. Keep pristine upstream source, generated patch auditable.
for old,new,count in [
 ('\tlea\tAUD3LC(a6),a5\n\tlea\tmt_chan4(a4),a2\n\tbsr\tmt_checkfx','\t; Proof: fourth track belongs exclusively to SFX mixer',2),
 ('\tlea\tAUD3LC(a6),a5\n\tlea\tmt_chan4(a4),a2\n\tbsr\tmt_playvoice','\taddq.l\t#4,a1\t; Proof: consume certified empty fourth-track event',1)]:
 assert src.count(old)==count,(old,src.count(old));src=src.replace(old,new)
src=src.replace("\tbsr\tmt_music\t\t; music with sfx inserted", "\taddq.l\t#1,_proofMusicTicks\n\tbsr\tmt_music\t\t; music with sfx inserted",1)
src="\txref\t_proofMusicTicks\n"+src
if TIMING:
 # Wrap complete OS CIA handlers, including both Timer-B DMA phases. Leave
 # upstream body and local-label scopes intact under a new global body label.
 for label,begin in [('mt_cia_timer_a_code','_proofTimerABegin'),('mt_cia_timer_b_code','_proofTimerBBegin')]:
  old=label+':'
  assert src.count(old)==1
  wrapper=(old+'\n\tmovem.l\td0-d7/a0-a6,-(sp)\n\tjsr\t'+begin+
   '\n\tmovem.l\t(sp)+,d0-d7/a0-a6\n\tbsr\t'+label+'_body'+
   '\n\tmovem.l\td0-d7/a0-a6,-(sp)\n\tjsr\t_proofTimerEnd'+
   '\n\tmovem.l\t(sp)+,d0-d7/a0-a6\n\trts\n'+label+'_body:')
  src=src.replace(old,wrapper)
 src='\txref\t_proofTimerABegin,_proofTimerBBegin,_proofTimerEnd\n'+src
(BUILD/'ptplayer-three.asm').write_text(src)
vasm=str(SDK/'bin/vasmm68k_mot')
for source,out,flags in [(BUILD/'ptplayer-three.asm','player.o',['-DOSCOMPAT=1','-DENABLE_SAWRECT=0']), (HERE/'irq.s','irq.o',[])]:
 subprocess.run([vasm,'-quiet','-Fhunk','-m68020',*flags,'-o',str(BUILD/out),str(source)],check=True,env=env,cwd=ROOT)
subprocess.run([str(SDK/'bin/vc'),'+aos68k','-O2','-cpu=68020','-notmpfile','-I'+str(ROOT/'.toolchain/ndk/Include_H'),'-o',str(BUILD/'Audio-Level1'),*(['-DAUDIO_TIMING',str(HERE/'timing.c')] if TIMING else (['-DAUDIO_CONTROL'] if CONTROL else [])),str(HERE/'proof.c'),str(HERE/'mix.c'),str(BUILD/'player.o'),str(BUILD/'irq.o')],check=True,env=env,cwd=ROOT)
mod=(ROOT/'music/experiments/level1-pulse/copper-sprint.mod').read_bytes()
patterns=max(mod[952:952+mod[950]])+1;split=1084+1024*patterns
assert all(mod[a:a+4]==bytes(4) for a in range(1084+12,split,16))
assert sum(int.from_bytes(mod[42+i*30:44+i*30],'big')*2 for i in range(31))==len(mod)-split
runtime=BUILD/'assets/runtime';runtime.mkdir(parents=True,exist_ok=True)
(runtime/'pulse-score.bin').write_bytes(mod[:split]);(runtime/'pulse-bank.bin').write_bytes(mod[split:])
report={'mixer':'block-v1','mixer_sha256':hashlib.sha256((HERE/'mix.c').read_bytes()).hexdigest(),'instrumentation':'ReadEClock-v1' if TIMING else ('none' if CONTROL else 'coarse-raster'),'upstream_sha256':hashlib.sha256((HERE/'third_party/ptplayer.asm').read_bytes()).hexdigest(),'generated_player_sha256':hashlib.sha256(src.encode()).hexdigest(),'executable_bytes':(BUILD/'Audio-Level1').stat().st_size,'executable_sha256':hashlib.sha256((BUILD/'Audio-Level1').read_bytes()).hexdigest(),'music_score_fast':split,'music_bank_chip':len(mod)-split,'sfx_output_chip':224,'native_verified':False}
(BUILD/'build.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
