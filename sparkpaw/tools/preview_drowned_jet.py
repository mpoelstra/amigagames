"""Native indexed animation review, not gameplay or a renderer benchmark."""
from pathlib import Path
import json
from PIL import Image
from preview_drowned_native import decode,indexed
from preview_drowned_polish import PAL
from generate_runtime_assets import save_spbm
ROOT=Path(__file__).resolve().parents[1];OUT=ROOT/'assets/concept/drowned-jet-native-v1'
def keyed(rgb,palette):
 im=indexed(rgb,palette,first=1)
 mask=Image.frombytes('L',rgb.size,bytes(255 if max(rgb.getpixel((x,y)))>32 else 0 for y in range(rgb.height) for x in range(rgb.width)))
 out=Image.new('P',rgb.size,0);out.putpalette(im.getpalette());out.paste(im,(0,0),mask);return out

def paste(dst,im,at):
 mask=Image.frombytes('L',im.size,bytes(255 if p else 0 for p in im.tobytes()))
 dst.paste(im.convert('RGB'),at,mask)
def main():
 OUT.mkdir(parents=True,exist_ok=True)
 src=Image.open(ROOT/'assets/concept/drowned-jet-source-v1.png').convert('RGB')
 w,h=src.size;frames=[]
 # One scale for the ENTIRE grid, no per-frame fit/zoom or baseline shifts.
 scaled=src.resize((256,128),Image.Resampling.BOX)
 for i in range(8):
  cell=scaled.crop(((i%4)*64+16,(i//4)*64,(i%4)*64+48,(i//4+1)*64))
  # Map only to existing blue/cyan/steel-highlight roles, never purple/amber.
  small=[PAL[0],PAL[5],PAL[6],PAL[11]]
  tmp=keyed(cell,small);im=Image.new('P',(32,64));im.putpalette([v for c in PAL for v in c]+[0]*720)
  im.putdata(bytes([0,5,6,11][v] for v in tmp.tobytes()));frames.append(im)
 atlas=Image.new('P',(32,512));atlas.putpalette(frames[0].getpalette())
 for i,im in enumerate(frames):atlas.paste(im,(0,i*64))
 save_spbm(OUT/'jet.spbm',atlas,PAL,4);atlas.save(OUT/'jet-indexed.png')
 assert decode(OUT/'jet.spbm').tobytes()==atlas.tobytes()
 base_dir=ROOT/'assets/concept/drowned-polish-native-v1'
 base=Image.new('RGB',(320,256));base.paste(Image.open(base_dir/'rear-indexed.png').convert('RGB'),(0,0))
 paste(base,Image.open(base_dir/'front-indexed.png'),(0,0))
 previous=Image.open(base_dir/'preview-1x.png').convert('RGB')
 base.paste(previous.crop((80,197,160,208)),(80,197))
 base.paste(previous.crop((0,208,320,256)),(0,208))
 player=decode(ROOT/'assets/runtime/sparkpaw-sprites4.spbm').crop((0,0,48,48));paste(base,player,(186,152))
 source=Image.open(ROOT/'assets/concept/drowned-polish-direction-v1.png').convert('RGB')
 nozzle=keyed(source.crop((432,773,511,822)).resize((16,10),Image.Resampling.BOX),PAL)
 # Reuse the approved nozzle source. Base fixed across all water frames.
 def scene(index):
  im=base.copy()
  if index is not None:paste(im,frames[index],(160,131))
  paste(im,nozzle,(168,190))
  return im
 sequence=[None,0,0,1]+[2,3,4,5]*3+[6,7,None]
 times=[2000,300,300,100]+[80]*12+[120,160,400]
 images=[scene(i) for i in sequence]
 for im in images:assert im.crop((0,208,320,256)).tobytes()==previous.crop((0,208,320,256)).tobytes()
 images[0].save(OUT/'preview-1x.gif',save_all=True,append_images=images[1:],duration=times,loop=0,disposal=2)
 enlarged=[im.resize((960,768),Image.Resampling.NEAREST) for im in images]
 enlarged[0].save(OUT/'preview-3x.gif',save_all=True,append_images=enlarged[1:],duration=times,loop=0,disposal=2)
 scene(3).resize((1280,1024),Image.Resampling.NEAREST).save(OUT/'active-4x.png')
 sheet=Image.new('RGB',(256,64),(15,22,30))
 for i,im in enumerate(frames):paste(sheet,im,(i*32,0))
 sheet.resize((1024,256),Image.Resampling.NEAREST).save(OUT/'frames-4x.png')
 bounds=[]
 for im in frames:
  bounds.append(Image.frombytes('L',im.size,bytes(255 if p else 0 for p in im.tobytes())).getbbox())
 (OUT/'manifest.json').write_text(json.dumps({'status':'offline animation art review pending','frames':8,'cell':[32,64],
  'source_size':[w,h],'uniform_grid_scale':[256,128],'bounds':bounds,'pens':[0,5,6,11],
  'raw_planes_bytes':8192,'optional_masks_bytes':2048,'two_frame_chip_stage_planes_bytes':2048,
  'timing_ms':times,'sequence':sequence,'limitations':['GIF timing is illustrative, not native FPS','no collision or damage acceptance',
  'no runtime integration','no full-screen animation cache intended; local frames only','shared foreground palette audit remains pending']},indent=2)+'\n')
 print('8 indexed frames, fixed scale, unchanged HUD, SPBM roundtrip verified')
if __name__=='__main__':main()
