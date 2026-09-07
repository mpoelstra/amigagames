#!/usr/bin/env python3
"""Execute production C loading/preparation selections with failing dependencies."""
from pathlib import Path
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
assets = (ROOT / 'src/assets.c').read_text()
loader = assets.split('BOOL assetsLoadGameplay(void)', 1)[1].split(
    '\nBOOL assetsLoadTitle(void)', 1)[0]
renderer = (ROOT / 'src/renderer.c').read_text()
prepare = renderer.split('    if(!LEVEL1_PREPARE(buildEnemyPatterns', 1)[1]
prepare = '    if(!LEVEL1_PREPARE(buildEnemyPatterns' + prepare.split(
    '        return FALSE;', 1)[0] + '        return FALSE;\n    return TRUE;\n'
asset_names = sorted(set(re.findall(r'(?<!&)&([A-Za-z]+)', loader)))
source = r'''
#include <assert.h>
#include <string.h>
#define BOOL int
#define TRUE 1
#define FALSE 0
#define SPARKPAW_CAMPAIGN
#define SPARKPAW_STORMRAIL_PROOF
static int loadStormrailGameplay;
static const char *files[32];
static int fileCount,failAt=-1;
static int loadAsset(const char *name,int *asset,int depth,int dma) {
    (void)asset;(void)depth;(void)dma;
    files[fileCount]=name;
    return fileCount++!=failAt;
}
'''
source += 'static int ' + ','.join(asset_names) + ';\n'
source += 'BOOL assetsLoadGameplay(void)' + loader
source += r'''
static struct {int stormrailActive;} state,*game=&state;
static int enemyCaches[2];
#define ENEMY_TYPE_CLOCKWORK_BEETLE 0
#define ENEMY_TYPE_CLOCKWORK_STORM_STRIDER 1
static int calls[12],jobs,failJob;
static int job(int id) {calls[id]++;jobs++;return id!=failJob;}
static int buildEnemyPatterns(int *cache,int fast) {
    (void)fast;return job(cache==enemyCaches?0:1);
}
#define prepareStriderStages() job(2)
#define buildCorePattern() job(3)
#define buildExtraLifePattern() job(4)
#define buildWaterPatterns() job(5)
#define buildSplashPatterns() job(6)
#define buildPlasmaPatterns() job(7)
#define buildStormrailPatterns() job(8)
#define buildDiamondPattern() job(9)
#define buildHeartPattern() job(10)
#define prepareStaticCollectibles() job(11)
#define LEVEL1_PREPARE(expression) (game->stormrailActive?TRUE:(expression))
static int prepare(void) {
'''
source += prepare + '}\n'
source += r'''
int main(void) {
    int mode,repeat,i,j,count;
    for(repeat=0;repeat<3;repeat++) for(mode=0;mode<2;mode++) {
        loadStormrailGameplay=mode;
        fileCount=0;failAt=-1;assert(assetsLoadGameplay());count=fileCount;
        assert(count==13);
        for(i=0;i<count;i++) {
            int exclusive=strstr(files[i],"clockwork-")!=0 ||
                strstr(files[i],"stormstone-core")!=0 ||
                strstr(files[i],"extra-life")!=0;
            if(mode) assert(!exclusive);
        }
        assert(strstr(files[0],mode?"stormrail-front":"storm-front"));
        for(i=0;i<count;i++) {
            failAt=i;fileCount=0;assert(!assetsLoadGameplay());
            assert(fileCount==i+1); /* no reads after a failed dependency */
        }
        state.stormrailActive=mode;
        memset(calls,0,sizeof(calls));failJob=-1;jobs=0;assert(prepare());
        for(j=0;j<7;j++) assert(calls[j]==!mode);
        assert(calls[8]==mode && calls[10]==mode);
        assert(calls[7]==1 && calls[9]==1 && calls[11]==1);
        for(i=0;i<12;i++) {
            int needed=(i<7?!mode:(i==8||i==10?mode:1));
            memset(calls,0,sizeof(calls));failJob=i;jobs=0;
            assert(prepare()==!needed);
        }
    }
    return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='sparkpaw-load-selection-') as directory:
    path = Path(directory)
    (path / 'test.c').write_text(source)
    subprocess.run(['cc', '-std=c99', '-Wall', '-Wextra', '-Werror',
                    '-fsanitize=address,undefined', str(path / 'test.c'),
                    '-o', str(path / 'test')], check=True)
    subprocess.run([str(path / 'test')], check=True)
print('PASS: actual campaign C selection, section switches and dependency failures')
