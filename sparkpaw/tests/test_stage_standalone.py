"""Verify opt-in isolated tool staging and unchanged ordinary-game manifest default."""
import sys,tempfile,hashlib
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
import stage_hd_test as stage
with tempfile.TemporaryDirectory() as temp:
 root=Path(temp);runtime=root/'assets/runtime';runtime.mkdir(parents=True)
 (runtime/'game.dat').write_bytes(b'game');(runtime/'effect.raw').write_bytes(b'raw')
 override=root/'proof';override.mkdir();(override/'score.bin').write_bytes(b'score')
 exe=root/'audio';exe.write_bytes(b'PROGDIR:assets/runtime/effect.raw\0PROGDIR:assets/runtime/score.bin\0')
 readme=root/'readme';readme.write_text('audio')
 stage.ROOT=root;stage.DIST=root/'dist';stage.STAGE_ROOT=root/'staging';stage.RUNTIME_FILES=['game.dat']
 stage.DIST.mkdir();release=stage.DIST/'Sparkpaw-baseline';release.write_bytes(b'protected')
 args=['stage','--drawer','Proof','--executable',str(exe),'--executable-name','Audio','--readme',str(readme),'--standalone-runtime',str(override)]
 sys.argv=args;stage.main()
 files={p.name for p in (stage.DIST/'Proof/assets/runtime').iterdir()}
 assert files=={'effect.raw','score.bin'};assert release.read_bytes()==b'protected'
 # Default still stages the authoritative complete game manifest.
 exe.write_bytes(b'game executable');sys.argv=args[:];sys.argv[2]='Game';sys.argv=sys.argv[:-2];stage.main()
 assert (stage.DIST/'Game/assets/runtime/game.dat').read_bytes()==b'game'
 # Dynamic extras cannot bypass standalone literal-reference coverage.
 sys.argv=args+['--extra-runtime','unseen.dat']
 try:stage.main();raise AssertionError('accepted missing literal references')
 except SystemExit as e:assert 'literal references' in str(e)
 # Candidate additions retain the full manifest and cannot shadow game assets.
 exe.write_bytes(b'PROGDIR:assets/runtime/score.bin\0')
 (override/'game.dat').write_bytes(b'wrong override')
 sys.argv=args[:-2]+['--additional-runtime-dir',str(override)];sys.argv[2]='Extended'
 stage.main()
 assert (stage.DIST/'Extended/assets/runtime/game.dat').read_bytes()==b'game'
 assert (stage.DIST/'Extended/assets/runtime/score.bin').read_bytes()==b'score'
print('PASS: standalone reference scope, ordinary manifest default and release parity')
