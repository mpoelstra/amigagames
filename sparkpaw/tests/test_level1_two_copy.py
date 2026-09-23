"""Actual Level1 CPU ring kernels on real storm-front plus modeled Bob DMA.
Reuses the inverse pixel/ownership oracle; not native raster acceptance.
"""
from pathlib import Path
import runpy,subprocess,tempfile,json
R=Path(__file__).resolve().parents[1]
model=runpy.run_path(str(R/'tests/test_drowned_two_copy.py'))
s=model['body']
for text in ['#define SPARKPAW_DROWNED_JOINED\n','#define SPARKPAW_DROWNED_COLUMN_BLIT\n','#define SPARKPAW_DROWNED_PATCH_BLIT\n','#define SPARKPAW_DROWNED_ENEMY_BOUNDS\n','#include "drowned_column_blit.h"\n','#include "drowned_patch_copy.h"\n#include "drowned_reset_copy.h"\n']:
 assert text in s;s=s.replace(text,'')
s=s.replace('#define WORLD_W 5120','#define WORLD_W 3392\n#define PROTOTYPE_TARGET_W WIDTH')
s=s.replace('#include "drowned_ring_layout.h"','#define SPARKPAW_LEVEL1_RENDERER_TU_ISOLATION\n#include "drowned_ring_layout.h"')
s=s.replace('widths[]={24,32,64,96},heights[]={24,24,64,16}', 'widths[]={16,24,32,64},heights[]={21,24,24,64}')
s=s.replace('640','424').replace('y*320','y*212').replace('x<5120','x<3392').replace('4800','3072').replace('4801','3073').replace('9602','6146').replace('9601','6145').replace('10048','6592')
s=s.replace('drownedCopyResetTarget(&target[prepared]);','prototypeCopyInitial(&target[prepared]);')
s=s.replace('if(y>=drownedColumnTops[x/16])','')
s=s.replace('drownedCopyPatchRect(&target[prepared],0,197,5120,11);','prototypeCopyDynamicRect(&target[prepared],0,197,3392,11);')
# Add a second canonical update at varying height to cover the Level1 dynamic
# rectangle path beyond the water strip, before applying actor overlays.
s=s.replace('ringWords+=copiedWords-start;compareCanonical();','''
  int py=64,wx=target[prepared].origin+16*((tick*3)%32);
  for(int p=0;p<4;p++)((UWORD*)source[p])[py*212+wx/16]^=(UWORD)(17+tick);
  prototypeCopyDynamicRect(&target[prepared],0,py,WORLD_W,1);
  ringWords+=copiedWords-start;compareCanonical();''')
# Every resident rectangle that the candidate drops must be outside the
# visible view with16px margin. Retained rectangles include DMA word padding.
s=s.replace('assert(prototypeRectFits(a->world,a->w)&&a->x>=0&&a->x+a->w<=WIDTH);',
    'if(!prototypeRectFits(a->world,a->w)){assert(a->world+a->w<=camera-16||a->world>=camera+336);a->valid=0;continue;} assert(a->x>=0&&a->x+a->w<=WIDTH);')
s=s.replace('game->cameraX=camera;\n  prototypeRollTarget', 'game->cameraX=camera;\n  prototypeRollTarget')
s=s.replace('prototypeBuildOrigin=target[prepared].origin;', 'prototypeBuildOrigin=target[prepared].origin;\n  for(int w=16;w<=64;w+=8)for(int x=prototypeBuildOrigin;x+w<=prototypeBuildOrigin+512;x++){\n   int fit=prototypeRectFits(x,w),physical=prototypePhysicalX(x);\n   if(!fit)assert(x+w<=camera-16||x>=camera+336);\n   else assert(physical>=0&&((physical+w+15)&~15)<=WIDTH);\n  }')
results=[]
with tempfile.TemporaryDirectory() as td:
 p=Path(td);(p/'test.c').write_text(s)
 for candidate in (False,True):
  flags=['-DSPARKPAW_LEVEL1_TWO_COPY_RING'] if candidate else []
  subprocess.run(['cc','-O2','-std=c99','-fsanitize=address,undefined',*flags,'-I'+str(R/'src'),str(p/'test.c'),'-o',str(p/'test')],check=True)
  results.append(json.loads(subprocess.check_output([str(p/'test'),str(R/'assets/runtime/storm-front.spbm')],text=True)))
a,b=results
assert [a['copies'],b['copies']]==[3,2]
assert b['restore_words']<=a['restore_words'] and b['masked_words']<=a['masked_words']
print('PASS Level1 ASan/UBSan: actual CPU column/full/dynamic copies, real3392px source,6592frames/layout, all3073camera positions both directions; reset/wrap/overlap/guards/inactive ownership and identical AGA fetch phase. DMA modeled, not raster proof.')
print(json.dumps(results))
