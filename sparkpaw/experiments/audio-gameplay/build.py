"""Build isolated Level-1 A/B; no production source edits or release targets."""
from pathlib import Path
import os,subprocess,hashlib,json,re,sys
TRACE="--trace" in sys.argv
FIXED="--fixed" in sys.argv
assert not (TRACE and FIXED)
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1]
BUILD=ROOT/('build/audio-gameplay-fixed' if FIXED else ('build/audio-gameplay-trace' if TRACE else 'build/audio-gameplay'));BUILD.mkdir(parents=True,exist_ok=True)
SDK=ROOT/'.toolchain/sdk';env=os.environ.copy();env['VBCC']=str(SDK);env['PATH']=str(SDK/'bin')+os.pathsep+env['PATH'];env['TMPDIR']=str(BUILD)
# Reproduce the same certified three-voice player and score/bank as the proof.
subprocess.run(['python3',str(ROOT/'experiments/audio-level1/build.py'),'--control'],check=True)
proof=(ROOT/'experiments/audio-level1/proof.c').read_text()
a=proof.index('static const char *names');b=proof.index('static UBYTE *load(',a)
(BUILD/'catalog.h').write_text('/* Generated verbatim from accepted audio proof catalog. */\n'+proof[a:b])
p=(ROOT/'src/platform_amiga.c').read_text()
old='    audioSetHardwareActive(TRUE);'
assert p.count(old)==1
p=p.replace(old,old+'''
#ifdef AUDIO_GAME_CANDIDATE
    /* Exec remains forbidden. Only CIA-B music and AUD3 may interrupt the
       owned renderer. CIA-A keyboard stays polled; no OS VBlank display writes. */
    hardware->intena=0xa400;
    Enable();
#endif''')
old='    if(interruptsDisabled) {'
assert p.count(old)==3
p=p.replace(old,old+'''
#ifdef AUDIO_GAME_CANDIDATE
        /* Balance the candidate-only Enable at takeover before quiescing audio. */
        Disable();
#endif''')
(BUILD/'platform_audio.c').write_text(p)
irq=(ROOT/'experiments/audio-level1/irq.s').read_text()
irq=irq.replace('        xdef _proof_vblank_irq\n','').replace('        xref _proofVBlank\n','')
irq=irq[:irq.index('_proof_vblank_irq:')]
(BUILD/'irq.s').write_text(irq)
subprocess.run([str(SDK/'bin/vasmm68k_mot'),'-quiet','-Fhunk','-m68020','-o',str(BUILD/'irq.o'),str(BUILD/'irq.s')],check=True,env=env)
# Use release rendering flags, no campaign/Stormrail or broad diagnostics.
flags=['SPARKPAW_ROLLING_PROTOTYPE','SPARKPAW_BLITTER_PRIORITY_CANDIDATE','SPARKPAW_AGA32_FETCH_CANDIDATE','SPARKPAW_AGA32_ORIGIN_CORRECTED','SPARKPAW_AGA32_LEFT_GUARD','SPARKPAW_AGA64_PLAYER_SPRITE','SPARKPAW_HUD_SEAM_FRONT_BLACK','SPARKPAW_SPRITE_STAGE_CACHE']
sources=['renderer','hud','assets','collision','collectibles','performance_profile','enemies','game','level_data','player','projectiles','title','ready_patch','ready_dust','music']
if FIXED:
 from prepare_fixed import player_source
 (BUILD/'player_fixed.c').write_text(player_source())
base=[str(HERE/('fixed_main.c' if FIXED else 'main.c')),str(BUILD/'platform_audio.c')]+[str(ROOT/f'src/{s}.c') for s in sources]+[str(ROOT/'build/music_lsp.o')]
if FIXED:base=[str(BUILD/'player_fixed.c') if x==str(ROOT/'src/player.c') else x for x in base]
assert (ROOT/'build/music_lsp.o').is_file()
# Only the reference audio object gets effect counters. Never enable the
# renderer/platform CIA-B profiler when compiling the gameplay harness.
subprocess.run([str(SDK/'bin/vc'),'+aos68k','-O2','-cpu=68020','-notmpfile',
 '-I'+str(ROOT/'.toolchain/ndk/Include_H'),'-DSPARKPAW_RENDER_DIAGNOSTIC',
 '-c','-o',str(BUILD/'audio-reference.o'),str(ROOT/'src/audio.c')],check=True,env=env)
if TRACE:base+=['-DAUDIO_GAME_TRACE',str(HERE/'trace.c')]
reports={}
for name,candidate in [('A',False),('B',True)]:
 extra=[str(BUILD/'audio-reference.o')]
 if candidate:extra=['-DAUDIO_GAME_CANDIDATE',str(HERE/'audio_gameplay.c'),str(ROOT/'experiments/audio-level1/mix.c'),str(ROOT/'build/audio-block-control/player.o'),str(BUILD/'irq.o')]
 out=BUILD/('Level1-Audio-'+name)
 cmd=[str(SDK/'bin/vc'),'+aos68k','-O2','-cpu=68020','-notmpfile','-I'+str(ROOT/'.toolchain/ndk/Include_H'),'-I'+str(ROOT/'src'),'-I'+str(ROOT/'experiments/audio-level1'),'-I'+str(BUILD),'-I'+str(HERE),*['-D'+x for x in flags],'-o',str(out),*base,*extra]
 subprocess.run(cmd,check=True,env=env,cwd=ROOT)
 reports[name]={'sha256':hashlib.sha256(out.read_bytes()).hexdigest(),'bytes':out.stat().st_size,'candidate':candidate,'flags':flags}
reports['platform_source_sha256']=hashlib.sha256(p.encode()).hexdigest()
reports['trace']=TRACE
reports['fixed']=FIXED
if FIXED:reports['script_sha256']=hashlib.sha256((HERE/'script.h').read_bytes()).hexdigest()
reports['native_verified']=False
(BUILD/'build.json').write_text(json.dumps(reports,indent=2)+'\n');print(json.dumps(reports,indent=2))
