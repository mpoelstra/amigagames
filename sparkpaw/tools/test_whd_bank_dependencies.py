"""Execute real per-section asset/collision selection against resident banks.

This audits file reachability, not bitmap allocation, decoding or rendering.
The separate bank/decoder tests own those byte and lifetime contracts.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess

from test_whd_banks import ROOT, STUBS


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir',required=True,type=Path)
    parser.add_argument('--output-dir',required=True,type=Path)
    args=parser.parse_args()
    build=args.build_dir.resolve();out=args.output_dir.resolve()
    out.mkdir(parents=True,exist_ok=False)
    manifest=json.loads((build/'banks-manifest.json').read_text())
    meta=json.loads((build/'build.json').read_text())
    assets=(ROOT/'src/assets.c').read_text()
    helpers=assets[assets.index('#ifdef SPARKPAW_DROWNED_SLICE\nstatic BOOL loadDrownedPatches'):assets.index('BOOL assetsLoadTitle(void)')]
    collision=(ROOT/'src/collision.c').read_text()
    collision=collision[collision.index('BOOL collisionLoad(void)'):collision.index('BOOL collisionSolidAt(')]
    backend='\n'.join(line for line in (ROOT/'src/whd_banks.c').read_text().splitlines()
                      if not line.startswith('#include'))
    fields=sorted(set(re.findall(r'&([A-Za-z][A-Za-z0-9]*),',helpers)))
    stubs=r'''
struct PlanarAsset { unsigned width,height; };
static BOOL loadStormrailGameplay,stormrailCollision;
static unsigned missing,requested;
static UBYTE collision[1]; /* Reachability only; production dimensions unchanged. */
static BOOL gameStormrailActive(void) { return loadStormrailGameplay; }
static BPTR auditOpen(const char *name,LONG mode) {
    BPTR f=whdBankOpen(name,mode);requested++;
    if(!f) { fprintf(stderr,"MISSING %s\n",name);missing++; }
    return f;
}
static BOOL loadAsset(const char *name,struct PlanarAsset *asset,
                      UBYTE depth,BOOL dma) {
    BPTR f; (void)depth;(void)dma;
    /* Satisfy only the dimensions used to branch in loadDrownedPatches. */
    asset->width=strstr(name,"checkpoint.spbm")?48:16;
    asset->height=strstr(name,"checkpoint.spbm")?384:5422;
    f=auditOpen(name,MODE_OLDFILE);if(f)whdBankClose(f);
    return TRUE; /* Collect all unavailable names, not only the first. */
}
#define Open auditOpen
#define Read whdBankRead
#define Close whdBankClose
'''
    driver=r'''
int main(int argc,char **argv) {
    unsigned section,before;
    assert(argc==3);root=argv[1];section=(unsigned)atoi(argv[2]);
    assert(whdBanksBoot()&&whdBanksFinishIntro()&&whdBanksSelect(section));
    loadStormrailGameplay=section==2;before=reads;
    assert(assetsLoadGameplay());
#ifdef SPARKPAW_LEVEL1_REAR_AMBIENCE
    if(section==1) {
        UBYTE magic[4]; BPTR animation=auditOpen("PROGDIR:assets/runtime/l1-electric.bin",MODE_OLDFILE);
        assert(animation&&whdBankRead(animation,magic,4)==4&&!memcmp(magic,"L1A3",4));
        whdBankClose(animation);
    }
#endif
    (void)collisionLoad();
    assert(reads==before);whdBanksClose();assert(!resident&&!disk);
    printf("section %u: %u required reads, %u unavailable\n",section,requested,missing);
    return missing?1:0;
}
'''
    c=out/'dependencies.c'
    c.write_text(STUBS+backend+stubs+'\nstatic struct PlanarAsset '+','.join(fields)+';\n'+helpers+collision+driver)
    results=[]
    for kind,command,sections in [('host',meta['command'],[1,2]),('drowned',meta['module_command'],[3])]:
        exe=out/kind
        # Timing does not select assets; its separate collector has own tests.
        flags=[f for f in command if f.startswith('-D') and f!='-DSPARKPAW_WHD_LOAD_TRACE']
        subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-Wno-unused-function',
                        '-Wno-unused-variable','-fsanitize=address,undefined',
                        *flags,str(c),'-o',str(exe)],check=True)
        for section in sections:
            run=subprocess.run([str(exe),str(Path(manifest['stage'])/'data'),str(section)],
                               capture_output=True,text=True)
            print(run.stdout+run.stderr,end='')
            results.append(dict(section=section,passed=run.returncode==0,
                                output=run.stdout+run.stderr))
    (out/'result.json').write_text(json.dumps(results,indent=2)+'\n')
    if not all(r['passed'] for r in results):raise SystemExit(1)
    proof=build/'dependency-result.json'
    assert not proof.exists(), 'preserve previous proof'
    proof.write_text(json.dumps(results,indent=2)+'\n')


if __name__=='__main__':main()
