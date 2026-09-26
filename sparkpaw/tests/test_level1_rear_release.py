"""Exercise production rear allocation/cleanup through packed-media loader seam."""
from pathlib import Path
import runpy,subprocess,tempfile
R=Path(__file__).resolve().parents[1]
fixture=runpy.run_path(str(R/'tests/test_level1_rear_ambience_v5.py'))
C=fixture['C']
loader=r'''
/* Decoder byte parity is checked separately by release ADF/bank readback. */
static UBYTE *assetsLoadDiskData(const char *name,ULONG flags,ULONG *size) {
    FILE *f;UBYTE *data; (void)name;*size=0;
    if(fileMode==1||fileMode==2)return NULL;
    *size=L1_REAR_BYTES+(fileMode==4?1:0);
    data=AllocMem(*size,flags);if(!data){*size=0;return NULL;}
    f=fopen("assets/runtime/l1-electric.bin","rb");assert(f);
    assert(fread(data,1,L1_REAR_BYTES,f)==L1_REAR_BYTES);fclose(f);
    if(fileMode==3)data[0]='X';
    return data;
}
'''
C=C.replace('#include "level1_rear_ambience.h"','#include "level1_rear_ambience_release_data.h"\n'+loader+'\n#include "level1_rear_ambience.h"')
# Generated data has no include guard; undef duplicate declaration by wrapping first include.
C=C.replace('#include "level1_rear_ambience_release_data.h"', '#define L1_REAR_BYTES 99268UL')
with tempfile.TemporaryDirectory(prefix='l1-release-',dir=R/'build') as tmp:
    src=Path(tmp)/'test.c';src.write_text(C)
    for mode in ('SPARKPAW_MULTI_ADF','SPARKPAW_WHD_PACKED'):
        exe=Path(tmp)/mode
        subprocess.run(['cc','-std=c99','-O1','-g','-fsanitize=address,undefined','-D'+mode,'-DSPARKPAW_LEVEL1_REAR_AMBIENCE_RELEASE','-I'+str(R/'src'),str(src),'-o',str(exe)],check=True)
        subprocess.run([str(exe)],cwd=R,check=True)
        print('PASS production loader seam:',mode)

# Also exercise the shipped hot path with all diagnostic counters compiled out.
import re
plain=fixture['C'].replace('#define SPARKPAW_LEVEL1_REAR_HOST_TEST\n','')
plain=plain.replace('&&!l1RearUnsafe','').replace('assert(l1RearUnsafe==1);','')
plain=re.sub(r' printf\("PASS actual C:.*?;\n', ' puts("PASS production path without counters");\n',plain)
with tempfile.TemporaryDirectory(prefix='l1-production-',dir=R/'build') as tmp:
    src=Path(tmp)/'test.c';src.write_text(plain);exe=Path(tmp)/'test'
    subprocess.run(['cc','-std=c99','-O1','-g','-fsanitize=address,undefined','-DSPARKPAW_LEVEL1_REAR_AMBIENCE_RELEASE','-I'+str(R/'src'),str(src),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],cwd=R,check=True)
