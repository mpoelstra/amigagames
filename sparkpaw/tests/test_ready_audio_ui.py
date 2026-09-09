"""Cache output versus frozen pre-fix rasterizer, both targets and skipped frames.

Also protects the offline/runtime boundary that caused the 68020 regression.
Host byte parity and work bounds are not a native cadence measurement.
"""
from pathlib import Path
import ctypes as c
import subprocess,tempfile,random,sys,re
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from generate_ready_ui_cache import rasterize,Selection,BANDS,U8,PTR
ADF='--adf' in sys.argv
size=int(re.search(r'READY_UI_CACHE_BYTES (\d+)',(ROOT/('src/ready_ui_cache_adf_size.h' if ADF else 'src/ready_ui_cache_size.h')).read_text())[1])
class History(c.Structure):
    _fields_=[('count',U8),('offset',c.c_ushort*48),('original',(U8*6)*48)]
class UI(c.Structure):
    _fields_=[('cache',U8*size),('mask',U8*2912),('selected',U8*5),('previous',(U8*5)*2)]

def contract():
    source=(ROOT/'src/ready_ui.c').read_text()
    compose=source[source.index('void readyUiCompose'):source.index('void readyUiApply')]
    apply=source[source.index('void readyUiApply'):]
    # Lookup/copy only. Rasterizer/diff discovery cannot quietly move back here.
    for forbidden in ('readyUiGlyphs','readyUiBaseMask','ready_ui_data.h','strlen(',
                      'atlasPatch(', 'dot(', 'text(', 'memcmp(', 'AllocMem('):
        assert forbidden not in source,forbidden
    assert 'readyUiMap[state*5+b]' in compose and 'readyUiBounds+' in apply
    assert 'u->cache[' not in compose+apply, 'No indexed image scans in owned loop'
    assert all(token not in compose for token in ('while(', 'for(y=', 'for(x=', 'for(p='))
    assert 'if(old==id) continue;' in apply
    assert 'readyUiInit(' not in compose+apply
    title=(ROOT/'src/title.c').read_text()
    prep=title[title.index('BOOL titleShowLevelReady'):title.index('static void readReadyMenuInput')]
    assert prep.index('readyUiInit(')<prep.index('fadeTo(loading,FALSE)')
    render=title[title.index('static void renderReadyDustFrame'):title.index('static void refreshReadyUI')]
    assert render.index('readyDustRestore(')<render.index('readyUiApply(')<render.index('readyDustDraw(')<render.index('waitOwnedDisplayFrame(')
    campaign=render.split('#ifdef SPARKPAW_CAMPAIGN')[1].split('#else')[0]
    assert 'readyUiApply(' in campaign
    refresh=title[title.index('static void refreshReadyUI'):title.index('#if READY_HAS_SOUNDTEST',title.index('static void refreshReadyUI'))]
    assert 'readyUiCompose(' in refresh
    make=(ROOT/'Makefile').read_text()
    sources=make[make.index('SOURCES ='):make.index('HEADERS =')] if 'SOURCES =' in make else make[:make.index('HEADERS =')]
    assert 'ready_ui_layout.c' not in sources and 'fixtures/' not in sources
    assert 'tests/test_ready_audio_ui.py' in make

def main():
    contract()
    subprocess.run([sys.executable,str(ROOT/"tools/generate_ready_ui_cache.py"),"--check"],check=True)
    fixture=ROOT/'tests/fixtures/ready-ui-before-cache'
    states=list(rasterize(fixture/'ready_ui.c',fixture))
    if ADF: states=[state for state in states if not state[0].page or not state[1]]
    raw=(ROOT/'assets/runtime/sparkpaw-ready-screen.spbm').read_bytes()
    clean=[raw[204+p*10240:204+(p+1)*10240] for p in range(6)]
    # Keep a representative of every band variant and exercise every pair.
    representatives=[{} for _ in BANDS]
    for i,(s,hd,pixels,mask) in enumerate(states):
        for b,(y,h) in enumerate(BANDS):
            key=b''.join(pixels[p*2912+y*28:p*2912+(y+h)*28] for p in range(6))+mask[y*28:(y+h)*28]
            representatives[b].setdefault(key,i)
    sequence=list(range(len(states)));random.Random(2718).shuffle(sequence)
    for variants in representatives:
        for a in variants.values():
            for b in variants.values():sequence.extend((a,b))
    with tempfile.TemporaryDirectory() as td:
        lib=Path(td)/'ui.so'
        obj=Path(td)/'ui.o'
        subprocess.run(['cc','-c','-fPIC','-O2',*(['-DSPARKPAW_MULTI_ADF'] if ADF else []),str(ROOT/'src/ready_ui.c'),'-o',str(obj)],check=True)
        subprocess.run(['cc','-shared','-fPIC','-O2','-Wall','-Wextra','-Werror',str(obj),str(ROOT/'src/ready_dust.c'),'-o',str(lib)],check=True)
        dll=c.CDLL(str(lib));ui=UI();dll.readyUiInit(c.byref(ui),None)
        histories=[History(),History()];pens=(U8*6)(1,2,3,4,5,6)
        buffers=[[(U8*10240).from_buffer_copy(p) for p in clean] for _ in range(2)]
        for step,index in enumerate(sequence):
            s,hd,pixels,mask=states[index]
            dll.readyUiCompose(c.byref(ui),None,c.byref(s),hd)
            dll.readyDustSetMenuMask(c.cast(c.byref(ui,UI.mask.offset),PTR) if s.page else None)
            if s.page:assert bytes(ui.mask)==mask,'Current glyph margin mask changed'
            expected=[bytearray(p) for p in clean]
            for p in range(6):
                for y in range(104):expected[p][(y+118)*40+6:(y+118)*40+34]=pixels[p*2912+y*28:p*2912+(y+1)*28]
            # Deliberately leave one target stale across arbitrary intervening states.
            for b in ([step%2] if step<len(states) and step%3 else [0,1]):
                target=(PTR*6)(*[c.cast(p,PTR) for p in buffers[b]])
                dll.readyDustRestore(target,c.byref(histories[b]))
                dll.readyUiApply(c.byref(ui),target,b)
                assert [bytes(p) for p in buffers[b]]==[bytes(p) for p in expected],(step,index,b)
                dll.readyUiApply(c.byref(ui),target,b)
                assert [bytes(p) for p in buffers[b]]==[bytes(p) for p in expected]
                dll.readyDustDraw(target,c.byref(histories[b]),c.c_ulong(step),pens,s.row if not s.page else 0)
                assert histories[b].count<=48
        assert c.sizeof(UI)==size+2912+15
    print('PASS:',len(states),'frozen-layout states;',len(sequence),'transitions including all band pairs, stale targets, masks and idempotence; Fast bytes',c.sizeof(UI))
if __name__=='__main__':main()
