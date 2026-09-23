#!/usr/bin/env python3
"""Build the accepted local art into an isolated candidate; never runtime/dist."""
from pathlib import Path
import json,shutil,re
from PIL import Image,ImageDraw
from generate_runtime_assets import save_spbm
from preview_drowned_native import decode,indexed
from preview_drowned_polish import PAL
ROOT=Path(__file__).resolve().parents[1];OUT=ROOT/'build/drowned-slice/assets'
NATIVEPAL=[tuple((v>>4)*17 for v in c) for c in PAL]
def main():
 OUT.mkdir(parents=True,exist_ok=True)
 material=Image.open(ROOT/'assets/concept/drowned-polish-native-v1/front-indexed.png')
 front=Image.new('P',(960,208),0);front.putpalette(material.getpalette())
 floor=material.crop((160,200,320,208))
 for x in range(0,960,160):front.paste(floor,(x,200))
 platforms=((128,160,96),(528,160,64));collision=bytearray(60*14)
 for x,y,w in platforms:
  front.paste(material.crop((0,160,64,200)),(x,y))
  if w>64:front.paste(material.crop((16,160,48,168)),(x+64,y))
  for col in range(x//16,(x+w)//16):collision[(y//16)*60+col]=1
 for x in (240,608):front.paste(0,(x,197,x+80,208))
 # Scene offset 544 maps approved inner leaf x256 to world x800.
 front.paste(material.crop((228,112,308,200)),(772,112))
 # One fixed housing, shared by every shutter state. Read geometry from the
 # same constants as collision; dark steel is opaque, never chroma keyed.
 contract=(ROOT/'src/drowned_slice.h').read_text()
 bounds=[int(re.search(r'#define DROWNED_HEADER_'+key+r' (\d+)',contract)[1])
         for key in ('LEFT','TOP','RIGHT','BOTTOM')]
 hx,hy,hr,hb=bounds
 source=Image.open(ROOT/'assets/concept/drowned-solid-gate-source-v1.png').convert('RGB')
 housing=indexed(source.crop((340,184,548,280)).resize((hr-hx,hb-hy),Image.Resampling.BOX),PAL,first=1)
 assert 0 not in housing.tobytes()
 front.paste(housing,(hx,hy))
 nozzle=decode(ROOT/'assets/concept/drowned-geyser-native-v2/lip.spbm')
 jet=decode(ROOT/'assets/concept/drowned-geyser-native-v2/geyser.spbm')
 def paste_mask(dst,im,at):
  mask=Image.frombytes('L',im.size,bytes(255 if p else 0 for p in im.tobytes()))
  dst.paste(im,at,mask)
 paste_mask(front,nozzle,(448,194))
 atlas=Image.new('P',(96,96*23),0);atlas.putpalette(material.getpalette())
 # 0 idle, 1..8 approved jet cells. Full local base included for exact restore.
 for f in range(9):
  patch=front.crop((448,112,544,208))
  if f:paste_mask(patch,jet.crop((0,(f-1)*64,32,f*64)),(0,19))
  paste_mask(patch,nozzle,(0,82));atlas.paste(patch,(0,f*96))
 # 9 closed, 10 confirmation, 11..22 the same twelve reviewed lift positions.
 leaf=front.crop((800,136,834,197))
 lifts=[0,2,5,10,16,23,31,39,47,54,59,61]
 for f in range(9,23):
  patch=front.crop((768,112,864,208));d=ImageDraw.Draw(patch)
  lift=0 if f<11 else lifts[f-11]
  d.rectangle((32,24,65,84),fill=0)
  if lift<61:patch.paste(leaf.crop((0,lift,34,61)),(32,24))
  if f!=9:
   d.rectangle((10,57,11,58),fill=11 if f==10 else 6)
   d.rectangle((14,57,15,58),fill=11 if f==10 else 6)
  atlas.paste(patch,(0,f*96))
 # Pack the unchanged pixels into consecutive planar words. Width16 makes
 # each bitmap row a single word; each frame is one contiguous DMA-stage copy.
 packed=Image.new('P',(16,5422));packed.putpalette(material.getpalette())
 cursor=0
 for f in range(23):
  box=(0,19,32,83) if f<9 else (0,24,80,85)
  cell=atlas.crop((0,f*96,96,(f+1)*96)).crop(box)
  flat=Image.frombytes('P',(16,len(cell.tobytes())//16),cell.tobytes())
  flat.putpalette(material.getpalette());packed.paste(flat,(0,cursor));cursor+=flat.height
 assert cursor==5422
 # Preserve an ordinary image atlas for inspection, not runtime transfer.
 atlas.putpalette([v for c in NATIVEPAL for v in c]+[0]*720)
 atlas.save(OUT/'drowned-patches.png')
 for name,im in [('drowned-front',front),('drowned-patches',packed)]:
  im.putpalette([v for c in NATIVEPAL for v in c]+[0]*720)
  save_spbm(OUT/(name+'.spbm'),im,NATIVEPAL,4)
  if name=='drowned-front':im.save(OUT/(name+'.png'))
  result=decode(OUT/(name+'.spbm'));assert result.tobytes()==im.tobytes();assert result.getpalette()==im.getpalette()
 shutil.copyfile(ROOT/'assets/concept/drowned-panorama-v1/drowned-rear.spbm',OUT/'drowned-rear.spbm')
 (OUT/'drowned-collision.bin').write_bytes(collision)
 (OUT/'manifest.json').write_text(json.dumps({'world':[960,208],'platforms':platforms,'water':[240,608],
 'patch_frames':23,'patch_size':[96,96],'patch_y':112,'patch_x':[448,768],
 'atlas_fast_planes':43376,'chip_stage_planes':610,'packed_bitmap':[16,5422],
 'transfer_rects':[[448,131,32,64],[768,136,80,61]],'front_palette':NATIVEPAL,
 'gate_lifts':lifts,'header_bounds':bounds,'status':'Slice4 native user review pending'},indent=2)+'\n')
 review=Image.new('P',(192,96));review.putpalette(front.getpalette())
 review.paste(atlas.crop((0,9*96,96,10*96)),(0,0))
 review.paste(atlas.crop((0,22*96,96,23*96)),(96,0))
 review.resize((1152,576),Image.Resampling.NEAREST).save(OUT/'gate-review-6x.png')
 print('23 pixel-identical packed patches: 43376 Fast bytes, 610 Chip stage bytes')
if __name__=='__main__':main()
